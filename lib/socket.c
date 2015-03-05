/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 * socket.c
 *
 *      Copyright(c) Isao Hara, 2006
 *
 * $Revision: 1.3 $
 * $Date: 2008/05/29 04:54:56 $
 * $Id: socket.c,v 1.3 2008/05/29 04:54:56 yoshi Exp $
 *
 */
/*!
 * @file socket.c
 * @if jp
 * @brief ソケット通信関連処理
 * @else
 * @brief Socket Connection
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#include <math.h>
#include <RtORB/corba.h>
#include <RtORB/sockport.h>

#if defined(Linux) && defined(__STRICT_ANSI__)
#include <linux/if.h>

#endif

#if defined(Darwin) || defined(FreeBSD)
#include <ifaddrs.h>
#include <netinet/tcp.h>
#endif

struct sockport_profile  SockProfile[FD_SETSIZE];     /*!< Socket Port Profile */
static  fd_set main_socket_bits;                      /*!< FD_SET type's union for file descriptor including socket */
#ifdef USE_THREAD
static pthread_mutex_t SOCK_MUTEX = PTHREAD_MUTEX_INITIALIZER;
#define SOCKET_LOCK() pthread_mutex_lock(&SOCK_MUTEX)
#define SOCKET_UNLOCK() pthread_mutex_unlock(&SOCK_MUTEX)
#else
#define SOCKET_LOCK() 
#define SOCKET_UNLOCK() 
#endif

/*
struct sockaddr_in {
    short   sin_family; アドレスファミリー
    u_short sin_port; ポート番号(htonsでネットワークバイトオーダーに変換しておく) 
    struct  in_addr sin_addr; IPアドレスを格納したin_addr構造体
    char    sin_zero[8]; 0で初期化しておく
};
*/

/*
 *   Server Side
 */
int make_server_socket_port(int port_no) {
  int server_sock;
  struct sockaddr_in *server_addr;
  int reuse = 1;

  server_addr=(struct sockaddr_in *)RtORB_alloc(sizeof(struct sockaddr_in),
		  "make_server_socket_port");
  memset(server_addr, 0, sizeof (*server_addr));
  server_addr->sin_family      = AF_INET;
  server_addr->sin_port        = htons(port_no);
  server_addr->sin_addr.s_addr = htonl(INADDR_ANY);

  server_sock = socket (AF_INET, SOCK_STREAM, 0);

  if (server_sock < 0) {
	  fprintf(stderr, "Fail to create a socket\n");
	  free(server_addr);
	  return -1;
  }

  setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));

  if (bind(server_sock, (struct sockaddr *)server_addr, sizeof(*server_addr)) < 0){
    fprintf (stderr, "Fail to bind a socket\n");
    close(server_sock);
    return -1;
  }

  free(server_addr);

  if (listen (server_sock, 2) < 0 ){
      fprintf (stderr, "listen failed\n");
      close(server_sock);
      return -1;
  }
  return server_sock;
}

int32_t accept_connection(int32_t port_no) {
  struct sockaddr_in  client_addr;
  int   client_len, snew, opt_len, stat;
  int32_t  opt;

  client_len = sizeof(struct sockaddr_in);
  snew = accept(port_no, (struct sockaddr *)&client_addr, (socklen_t*)&client_len);
 
  if (snew>0) {
    stat=getsockopt(snew, SOL_SOCKET, SO_REUSEADDR, &opt, (socklen_t*)&opt_len);
    opt = 1;
    stat=setsockopt(snew, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    stat=getsockopt(snew, SOL_SOCKET, SO_REUSEADDR, &opt, (socklen_t*)&opt_len);

#if Darwin
    /* for MacOSX, we should set TCP_NOPUSH flag to zero. */
    opt = 0;
    stat=setsockopt(snew, IPPROTO_TCP, TCP_NOPUSH, &opt, sizeof(opt));
#endif

    }
  return(snew);
}

int copy_fds_set(fd_set *target, fd_set *src){
  int k;
  int	 nfds = 1;

  for(k= FD_SETSIZE; k >0 ;k--){
    if(FD_ISSET(k, src)){
      FD_SET(k, target);
      if(nfds < k) nfds = k+1;
    }
  }
  return nfds;
}

int select_server_sockets(fd_set *sockbits, int server_port, 
	struct timeval time_out,
	void (*connection_request)(int),
	int  (*command_request)(GIOP_ConnectionHandler *))
{
  int	 nfds;
  fd_set socks;
  int	 stat, i = 0, newsock;

  FD_ZERO(&socks);
  nfds = copy_fds_set(&socks, sockbits);

  stat=select(nfds, &socks, 0, 0 /*&exceptions*/ , &time_out);
  if (stat>0) {
    if (FD_ISSET(server_port, &socks)) { 
      fprintf(stderr, "=== New Connection\n");
      newsock= accept_connection(server_port); /* new connection */
      if (newsock>0) FD_SET(newsock, sockbits);
      FD_CLR(server_port, &socks);
      if (connection_request){
        (*connection_request)(newsock);
      }
      stat--;
    } else {
      i=0;
      while (stat > 0 && i < nfds) {
        if (FD_ISSET(i, &socks)) {
			GIOP_ConnectionHandler h;
          h.sock = i;
          h.type = CONNECTION_TYPE_SOCKET;
	    fprintf(stderr, "Request!! (%d)\n", i);
          if((*command_request)(&h) == -1) FD_CLR(i, sockbits);
          FD_CLR(i, &socks);
          stat--;
        }
      i++; 
      }
    }
  }
  return i; 
}

void server_socket_loop(int server_port, float time_out_float, void (*idle)(),
  void (*connection_request)(int), 
  int (*command_request)(GIOP_ConnectionHandler *))
{
  fd_set sockbits;
  struct timeval time_out;
#ifndef Linux
  struct timezone tz;
#endif
  struct timeval time1,time2;

  FD_ZERO(&sockbits);
  FD_SET(server_port, &sockbits);

  time_out.tv_sec = (unsigned int)(time_out_float / 1000.0);
  time_out.tv_usec = (time_out_float - time_out.tv_sec * 1000) * 1000 ;

  while (1) {

#ifdef Linux
      gettimeofday(&time1, NULL); 
#else
      gettimeofday(&time1, &tz); 
#endif
      select_server_sockets(&sockbits, server_port, time_out,
		connection_request, command_request);

      if (idle) (*idle)(); 

#ifdef Linux
      gettimeofday(&time2, NULL); 
#else
      gettimeofday(&time2, &tz); 
#endif

#if 0
      time_cost = (time2.tv_sec - time1.tv_sec) * 1000000 + time2.tv_usec
	      - time1.tv_usec - time_out_float * 1000 ;

      if (time_cost > 0) usleep(time_cost);
      else usleep(1);

#endif

  }
}

/**
 *
 *   find a hostname and a port number from a socket descriptor
 *
 */

short get_socket_port(int sock){
  int len;
  struct sockaddr_in addr;

  len = sizeof(struct sockaddr_in);

  if (getsockname(sock, (struct sockaddr *)&addr, (socklen_t*)&len) < 0) return -1;
  return  ntohs(addr.sin_port);
}

#if defined(Linux) || defined(Cygwin)
char *get_ip_address(int sock){
  struct sockaddr_in *addr=0;
  struct ifreq ifreqs[100];
  struct ifconf ifconf;
  int i, num;
  
  ifconf.ifc_len = sizeof(ifreqs);
  ifconf.ifc_req = ifreqs;

  if(ioctl(sock, SIOCGIFCONF, &ifconf) < 0)
  {
    return (char *)NULL;
  }
  num = ifconf.ifc_len / sizeof(struct ifreq);

  for(i=0; i< num ; i++){

    addr = (struct sockaddr_in *)&ifreqs[i].ifr_ifru.ifru_addr;
    char *ip_addr = (char *)inet_ntoa(addr->sin_addr);


#if 0
    fprintf(stderr, "Host = %s \n", ip_addr);
#endif

    if( (addr->sin_family == AF_INET)
#if 1
       && (strcmp(ip_addr,"127.0.0.1") != 0)
       && (strncmp(ip_addr,"169.254.", 8) != 0)
//       && (strncmp(ip_addr,"172.", 4) != 0)
    //   && (strncmp(ip_addr,"192.168.0", 9) != 0)
    //   && (strncmp(ip_addr,"192.168.1", 9) != 0)
       && (strcmp(ip_addr,"0.0.0.0") != 0)
#endif
        )
      {
           return RtORB_strdup(ip_addr, "get_ip_address");
      }
  }
  return (char *)NULL;
}
#else
char *get_ip_address(int sock){
  int stat;
  struct ifaddrs *ifap;
  char * res = NULL;
  struct sockaddr *addr=NULL;
  struct sockaddr_in *addr2=NULL;

  stat = getifaddrs(&ifap); 

  if(stat == 0){
    while(ifap != NULL){
      addr = ifap->ifa_addr;
      addr2 = (struct sockaddr_in *)addr;
      char *ip_addr = (char *)inet_ntoa(addr2->sin_addr);

      if( (addr->sa_family == AF_INET) &&
        (strncmp(ip_addr, "127.0.0", 7) != 0) &&
        (strncmp(ip_addr, "172.", 4) != 0) &&
        (strcmp(ip_addr, "0.0.0.0") != 0)
      ){
#if 0
    fprintf(stderr, "Host = %s \n", ip_addr);
#endif
        res = RtORB_strdup(ip_addr, "get_ip_address");
        break;
      }else{
        ifap = ifap->ifa_next;
      }
    }
  }else{
    fprintf(stderr, "ERROR: fail to getifaddrs\n");
  }
//  free(ifap);

  return res;
}

#endif

/*
 *
 * Client side
 *
 **/
int make_client_socket_port(char *hostname, int port)
{
	struct sockaddr_in sock_addr;
	struct hostent *hp;
	int fd;

	if(!hostname) return -1;
	if((hp = gethostbyname(hostname)) == NULL){
		fprintf(stderr,"host %s not valid\r\n", hostname);
		return -1;
	}

        memcpy((char *) &(sock_addr.sin_addr), hp->h_addr_list[0], hp->h_length); 
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(port);

	/* create socket */
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
/*
		 perror("socket");
*/
		 return(-1);
	}

	if(connect(fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0){
/*
		perror("connect");
*/
		return(-1);
	}
	
	return fd;
}

#if 0 /* no use */
int select_client_socket( int client_port, int timeout,
		void (*exception_request)(), int (*command_request)())
{
  int	 nfds;
  fd_set socks;
  int	 stat;
  struct timeval time_out;

  //time_out.tv_sec = (int)floor(timeout / 1000);
  time_out.tv_sec = timeout / 1000;
  time_out.tv_usec = (timeout - time_out.tv_sec * 1000) * 1000 ;

  FD_ZERO(&socks);
  FD_SET(client_port, &socks);

  nfds = client_port+1;
  stat=select(nfds, &socks, 0, 0 /*&exceptions*/, &time_out);

  if (stat>0) {
    if (FD_ISSET(client_port, &socks)) { 
      if(command_request){
        if((*command_request)(client_port) == -1){
          close(client_port);
	  return 0;
        }
      }
      FD_CLR(client_port, &socks);
    }
  }

  return 1; 
}
#endif /* no use */


/*
   TEST Implementation
*/

void init_SockProfile(){
  int i;

  SOCKET_LOCK();

  for(i=0;i<FD_SETSIZE;i++){
    memset(&SockProfile[i], 0, sizeof(struct sockport_profile));
  }

  SOCKET_UNLOCK();

  return;
}

void clear_SockProfile(int fd){

  SOCKET_LOCK();

  memset(&SockProfile[fd], 0, sizeof(struct sockport_profile));

  SOCKET_UNLOCK();
  
  return;
}

/*
void set_SockProfile_type(int fd, int type){

  SOCKET_LOCK();
  
  SockProfile[fd].type = type;

  SOCKET_UNLOCK();
  return;
}
*/
void set_SockProfile_arg(int fd, void *arg){
  
  SOCKET_LOCK();
  
  SockProfile[fd].arg = arg;

  SOCKET_UNLOCK();
  return;
}

void *get_SockProfile_arg(int fd)
{
  return SockProfile[fd].arg;
}

/*
void set_SockProfile_service(int fd,
		int (*command_request)()){

  SOCKET_LOCK();
  SockProfile[fd].command_proc = command_request;
  SOCKET_UNLOCK();

  return;
}
*/

/*!
 * @if jp
 * @brief SocketProfile配列からタイプが'SOCK_SERVER'のもののindexを抽出する。
 * @else
 * @brief get index array for server from socket profile.
 * @endif
 * @param res array of index of Socket profile ('SOCK_SERVER')
 * @param n counts of 'SOCK_SERVER'
 * @param start_fd start number of 'FD'
 * @return void
 */
static void get_Servers_from_SockProfile(int *res, int *n, int start_fd){
  int i;
  int current;

  SOCKET_LOCK();

  for(i=start_fd, current=0;i<FD_SETSIZE;i++){
    if(SockProfile[i].type == SOCK_SERVER){
      res[ current++ ]=i;
    }
  }
  *n=current;

  SOCKET_UNLOCK();

  return;
}

/*!
 * @if jp
 * @brief SocketProfile情報をメモリ上で複製する。
 * @else
 * @brief copy Socket Profile.
 * @endif
 * @param dist target for copy
 * @param src reference for copy
 * @return void
 */
static void copy_sockport_profile(int dist, int src){

  SOCKET_LOCK();
  
  memcpy(&SockProfile[dist], &SockProfile[src], sizeof(struct sockport_profile));

  SOCKET_UNLOCK();

  return;
}

int set_SockProfile(int fd, int type,
		int (*connection_request)(int, void*),
		int (*disconnect_request)(int, void*),
		int (*command_request)(GIOP_ConnectionHandler*)){
  SOCKET_LOCK();
  
  SockProfile[fd].type = (int32_t)type;
  SockProfile[fd].command_proc = (int32_t (*)(GIOP_ConnectionHandler*))command_request;
  SockProfile[fd].connection_proc = (int32_t (*)(int, void*))connection_request;
  SockProfile[fd].disconnect_proc = (int32_t (*)(int, void*))disconnect_request;

  FD_SET(fd, &main_socket_bits);

  SOCKET_UNLOCK();

  return fd;
}

int clear_socket_profile(int fd, void (*clear_func)(int)){
  clear_SockProfile(fd);

  SOCKET_LOCK();

  FD_CLR(fd, &main_socket_bits);

  SOCKET_UNLOCK();

  if(clear_func) (*clear_func)(fd);

  return fd;
}

/*!
 * @if jp
 * @brief 新しいファイルディスクリプタを割り当て可能なポートを探し、その番号を返す。
 * @else
 * @brief find active socket port and return the 'FD'.
 * @endif
 * @param socks FD_SET type
 * @param start_fd start number of FD_SET
 * @return active port number (none:-1)
 */
static int next_active_socket(fd_set *socks, int start_fd){
  int i;

  for (i = start_fd; i < FD_SETSIZE; i++)
    if (FD_ISSET(i, socks))  return i;

  return -1;
}

/*!
 * @if jp
 * @brief socketの受信待ち処理、データ受信後の処理を行う。
 * 
 *        (1)selectシステムコールでmain_socket_bitsに設定されたsocketの受信待ちを行う。タイムアウトであれば、特に何もしない。\n
 *        (2)受信すべきsocketポートが発見された場合には、そのすべてに対して以下の処理を行う。\n
 *        (2-1) 受信socket（active_portとする）がSOCK_SERVERであった場合。__すなわち SockProfile[active_port].type == SOCK_SERVER であった場合は、新たなリクエストであるので、accept_connection関数を呼び出して、通信用のsocket(newfdとする）を作成する。__そしてSockProfile[newfd].type = SOCK_SERVICE に設定し、さらにSockProfile[socket_fd].connection_procに関数が設定されていれば、その関数を呼び出す。 RtORBでは、この関数は設定されていないはずである。\n
 *        (2-2)受信socketがSOCK＿SERVICEまたはSOCK_CLIENTの場合に（実際には、SOCK＿CLIENTではありえない）、 SockProfile[active_port].command_procに設定された関数を実行する。__RtORBでは、 PortableServer_enqueue_request関数になっているハズである。この関数からの返値が　-1 だった場合には、通信エラーの発生を意味するので、SockProfile[active_port].disconnect_procで設定された関数（RtORBでは、NULLのハズなので実行しない）を実行し、このsocketを閉し、SockProfile[active_port].type = SOCK_CLOSED　とする。\n
 * @else
 * @brief Wait data from socket, and actions after receiving data.   
 * @endif
 * @param sockbits FD_SET type 
 * @param time_out interlval time(msec)
 * @return n:number of file descriptor, 0:time_out, -1:error
 */
int select_socket_ports( fd_set *sockbits, struct timeval time_out)
{
  int	 nfds;
  fd_set socks;
  int	 stat, res, newsock;
  int active_port;
  int (*cmnd_func)(GIOP_ConnectionHandler*);
  int (*cnct_func)(int, void*);
  int (*discn_func)(int, void*);
  GIOP_ConnectionHandler h;

  struct timeval tv;


  FD_ZERO(&socks);
  nfds = copy_fds_set(&socks, sockbits);

  tv.tv_sec = time_out.tv_sec;
  tv.tv_usec = time_out.tv_usec;

  res = stat = select(nfds, &socks, 0, 0, &tv);
  active_port = 0;

  while( stat > 0 ){
    if((active_port = next_active_socket(&socks, active_port) ) < 0 )
    {
      fprintf(stderr, "Invalid port is active...\n");
      return -1;
    }

    SOCKET_LOCK();
    cmnd_func = (int (*)(GIOP_ConnectionHandler*))SockProfile[active_port].command_proc;
    SOCKET_UNLOCK();

    switch (SockProfile[active_port].type){
      case SOCK_SERVER:
#if DEBUG
	fprintf(stderr, "Request Connection fd=%d\n", active_port);
#endif
        newsock = accept_connection(active_port);     /* new connection */

        if (newsock>0) {
          FD_SET(newsock, sockbits);
	  copy_sockport_profile(newsock, active_port);

	  SOCKET_LOCK();
          SockProfile[newsock].type = SOCK_SERVICE;
	  SOCKET_UNLOCK();

#if DEBUG
	  fprintf(stderr, "New Connection fd=%d\n", newsock);
#endif
	}

        cnct_func = (int (*)(int, void*))SockProfile[active_port].connection_proc;
	if(cnct_func) (*cnct_func)(active_port, SockProfile[active_port].arg );

        FD_CLR(active_port, &socks);
        stat--;
	break;
      case SOCK_CLIENT:
      case SOCK_SERVICE:
	h.type = CONNECTION_TYPE_SOCKET;
	h.sock = active_port;

        if(cmnd_func &&  (*cmnd_func)(&h) == -1 ){
          discn_func = (int (*)(int, void*))SockProfile[active_port].disconnect_proc;
	  if(discn_func) (*discn_func)(active_port, SockProfile[active_port].arg );

#if DEBUG
	  fprintf(stderr, "Request Close Connection fd=%d\n", active_port);
#endif
	  close(active_port);

          FD_CLR(active_port, sockbits);

	  SOCKET_LOCK();
          SockProfile[active_port].type = SOCK_CLOSED;
	  SOCKET_UNLOCK();
	}

        FD_CLR(active_port, &socks);
        stat--;
	break;
      default:
        fprintf(stderr, "Socket already closed...\n");
        stat--;
	break;
      }
  }

  return res; 
}

void init_socket_servers()
{
  int i;
  int sockServers[FD_SETSIZE];
  int nSock;

  get_Servers_from_SockProfile(sockServers, &nSock, 0);

  SOCKET_LOCK();

  FD_ZERO(&main_socket_bits);  /* initialize FD_SET */
  for(i=0;i<nSock;i++){ FD_SET(sockServers[i], &main_socket_bits); }  /* set FD */

  SOCKET_UNLOCK();
}

void select_socket_servers(struct timeval time_out)
{
  select_socket_ports( &main_socket_bits, time_out);
}

