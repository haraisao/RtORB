/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 * socket.h
 *
 *      Copyright(c) Isao Hara, 2006
 *
 * $Revision: 1.2 $
 * $Date: 2008/05/29 04:54:57 $
 * $Id: socket.h,v 1.2 2008/05/29 04:54:57 yoshi Exp $
 *
 */
/*!
 * @file socket.h
 * @if jp
 * @brief ソケット通信関連処理
 * @else
 * @brief Socket Connection
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __FUNC_SOCKET_H__
#define __FUNC_SOCKET_H__

/*
struct sockaddr_in {
    short   sin_family; アドレスファミリー
    u_short sin_port; ポート番号(htonsでネットワークバイトオーダーに変換しておく) 
    struct  in_addr sin_addr; IPアドレスを格納したin_addr構造体
    char    sin_zero[8]; 0で初期化しておく
};
*/

/*!
 * @if jp
 * @brief Server用に指定したPORTに対するsocketを作成し、PORTのbind処理をしlistenでClientからの接続要求待ちをする。作成したsocketのFileDescriptorを返す。
 * @else
 * @brief Make socket for waiting for connection from Client, bind 'PORT' to socket, and wait by 'listen()'.
 * @endif
 * @param port_no Port number
 * @return socket file descriptor (or '-1':error)
 */
int make_server_socket_port(int port_no) ;

/*!
 * @if jp
 * @brief サーバ側で'listen'していたソケットに対してクライアントから'connect'要求のあったsocketに対し、接続処理と以後の通信処理を行い、そのソケットディスクリプタを返す。
 * @else
 * @brief Accept action at server side.
 * @endif
 * @param port_no FD_SET number used as 'listen'  at server side
 * @return new socket file descriptor
 */
int32_t accept_connection(int32_t port_no) ;

/*!
 * @if jp
 * @brief FD_SET型のデータの中身を単純コピーする。
 * @else
 * @brief copy FD_SET data.
 * @endif
 * @param target copy target
 * @param src reference
 * @return max number of File Descriptor
 */
int copy_fds_set(fd_set *target, fd_set *src) ;

/*!
 * @if jp
 * @brief (T.B.D)
 * @else
 * @brief (T.B.D)
 * @endif
 * @param sockbits (T.B.D)
 * @param server_port (T.B.D)
 * @param time_out (T.B.D)
 * @param connection_request (T.B.D)
 * @param command_request (T.B.D)
 * @return (T.B.D)
 */
int select_server_sockets(fd_set *sockbits, int server_port,
		struct timeval time_out,
	void (*connection_request)(int), int  (*command_request)(GIOP_ConnectionHandler *));

/*!
 * @if jp
 * @brief (T.B.D)
 * @else
 * @brief (T.B.D)
 * @endif
 * @param server_port (T.B.D)
 * @param time_out_float (T.B.D)
 * @param idle (T.B.D)
 * @param connection_request (T.B.D)
 * @param command_request (T.B.D)
 * @return void
 */
void server_socket_loop(int server_port, float time_out_float, void (*idle)(),
  void (*connection_request)(int), int (*command_request)(GIOP_ConnectionHandler *));

/*!
 * @if jp
 * @brief socketに結びつけられたPORT番号をホストバイトオーダー形式で返す。
 * @else
 * @brief return PORT number linked to socket descriptor. (HostByteOrder format)
 * @endif
 * @param sock socket file descriptor
 * @return Port number (HostByteOrder format)
 */
short get_socket_port(int sock);

/*!
 * @if jp
 * @brief 指定したsocketのFDに該当するIPアドレスを返す。
 * @else
 * @brief get IP address.
 * @endif
 * @param sock socket file descriptor
 * @return IP address
 */
char *get_ip_address(int sock);

/*!
 * @if jp
 * @brief 指定したHOSTとPORT番号に接続するためのsocketを作成し、情報をセットしてサーバに接続要求を出す。
 * @else
 * @brief Make socket for connection to server using HOST and PORT, and request to connect to server by 'connect()'.
 * @endif
 * @param hostname Host Name
 * @param port Port Number
 * @return socket file descriptor for connection
 */
int make_client_socket_port(char *hostname, int port);

/*!
 * @if jp
 * @brief (T.B.D)
 * @else
 * @brief (T.B.D)
 * @endif
 * @param client_port (T.B.D)
 * @param timeout (T.B.D)
 * @param exception_request (T.B.D)
 * @param command_request (T.B.D)
 * @return (T.B.D)
 */
int select_client_socket( int client_port, int timeout,
		void (*exception_request)(), int (*command_request)());

/*!
 * @if jp
 * @brief socketプロファイル情報を初期化する。
 * @else
 * @brief Initialize socket profile.
 * @endif
 * @return void
 */
void init_SockProfile();

/*!
 * @if jp
 * @brief fdで指定したFD_SET型データを'0'で初期化するクリア処理。
 * @else
 * @brief Clear action. Initialize FD_SET data to '0'.
 * @endif
 * @param fd number of file descriptor
 * @return void
 */
void clear_SockProfile(int fd);

/*!
 * @if jp
 * @brief fdで指定したFD_SET型データのプロファイル情報(argのみ)を設定する。
 * @else
 * @brief Set 'arg' profile of FD_SET socket info.
 * @endif
 * @param fd number of file descriptor
 * @param arg argument of function
 * @return void
 */
void set_SockProfile_arg(int fd, void *arg);

/*!
 * @if jp
 * @brief fdで指定したFD_SET型データのプロファイル情報(argのみ)を返す。
 * @else
 * @brief Return 'arg' profile of FD_SET socket info.
 * @endif
 * @param fd number of file descriptor
 * @return argument of function
 */
void* get_SockProfile_arg(int fd);
/*
void set_SockProfile_type(int fd, int type);
void set_SockProfile_arg(int fd, void *arg);
void set_SockProfile_service(int fd, int (*command_request)());
*/


/*
void get_Servers_from_SockProfile(int *res, int *n, int start_fd);
void copy_sockport_profile(int dist, int src);
*/

/*!
 * @if jp
 * @brief fdで指定したFD_SET型データのプロファイル情報を設定する。
 * @else
 * @brief Set profile of FD_SET socket info.
 * @endif
 * @param fd number of file descriptor
 * @param type Socket Type (CLOSED/SERVER/SERVICE/CLIENT)
 * @param connection_request function pointer of connect action
 * @param disconnect_request function pointer of disconnect action
 * @param command_request function pointer of request action
 * @return 戻り値 number of file descriptor
 */
int set_SockProfile(int fd, int type,
		    int (*connection_request)(int, void*),
		    int (*disconnect_request)(int, void*), int (*command_request)(GIOP_ConnectionHandler*));

/*!
 * @if jp
 * @brief fdで指定したFD_SET型データのクリア処理。'0'に初期化する。
 * @else
 * @brief Clear and initialize FD_SET data.
 * @endif
 * @param fd number of file descriptor
 * @param clear_func function pointer of 'clear' action
 * @return number of file descriptor
 */
int clear_socket_profile(int fd, void (*clear_func)(int));

/*
int check_current_sockbits(int fd);
int next_active_socket(fd_set *socks, int start_fd);
int select_socket_ports( fd_set *sockbits, struct timeval time_out);
*/

/*!
 * @if jp
 * @brief GLOBAL変数で定義されているSocketProfile[FD_SETSIZE]からサーバーとして
 *        動作するファイルディスクリプタ（Socket fd)を抽出し、selectシステムコールの
 *        入力待ち fd_set を作成する。このfd_setは、main_socket_bitsというGLOBAL変数で定義されている。
 * @else
 * @brief initialize FD_SET.
 * @endif
 * @return void
 */
void init_socket_servers();

/*!
 * @if jp
 * @brief 外部からのMessage待ちを行う。受け取った後、もしくはタイムアウト後は親処理に戻す。中身は'select_socket_port()'を呼ぶのみ。
 * @else
 * @brief Wait for messages with time_out.
 * @endif
 * @param time_out interlval time(msec)
 * @return void
 */
void select_socket_servers(struct timeval time_out);

/*!
 * @if jp
 * @brief (T.B.D)
 * @else
 * @brief (T.B.D)
 * @endif
 * @param time_out_float (T.B.D)
 * @param idle (T.B.D)
 * @param arg (T.B.D)
 * @return void
 */
void socket_main_loop(float time_out_float, void (*idle)(), void *arg);

#endif
