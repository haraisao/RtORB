/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 * RtORB:
 *   Util.c
 *
 *  Copyright(C) 2006, Isao Hara, AIST
 *
 *  $Revision: 1.3 $
 *  $Date: 2008/05/21 05:11:30 $
 *  $Id: util.c,v 1.3 2008/05/21 05:11:30 yoshi Exp $
 */
/*!
 * @file util.c
 * @if jp
 * @brief 各種ユーティリティ用関数群
 * @else
 * @brief Various utility functions
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#include <RtORB/corba.h>
#if defined(USE_UUID)
#if defined(FreeBSD)
#include <uuid.h>
#else
#include <uuid/uuid.h>
#endif
#else
#include <stdio.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/time.h>
#if defined(Linux) && defined(__STRICT_ANSI__)
#include <sys/socket.h>
#include <linux/if.h>
#endif
#endif

#include <RtORB/giop.h>
#include <RtORB/util.h>

/***  toString  ***/
static char Char2Hex[]={
	 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0,
 	10,11,12,13,14,15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 	10,11,12,13,14,15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0 ,0, 0
};   /*!< Character to Octet (Char2Hex[0]=tbl[48]='0' ,[65]=A('10') ,[97]=a('10')) refer 'ASCII code table' */

static char Hex2Char[]={
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e','f'
};   /*!< Octet to Character (10->1 ... 15->f) */
/******* GIOP ************/
/*!
 * @if jp
 * @brief タイムアウトを設けた、read()関数によるデータの読み込みを行う。
 * @else
 * @brief Read data by 'read()' with Time out.
 * @endif
 * @param fd file discriptor
 * @param buf buffer for putting data
 * @param len data size
 * @param sec sec
 * @param usec micro sec
 * @return byte count of data for reading  (or 0:Timeout, -1:Fail)
 */
int read_with_timeout(int fd, char *buf, int len, int32_t sec, int32_t usec){
  fd_set socks;
  struct timeval timeout;
  timeout.tv_sec=sec;
  timeout.tv_usec=usec;

  FD_ZERO(&socks);
  FD_SET(fd, &socks);
  if(select(fd+1, &socks, 0, 0, &timeout) == 0){
    fprintf(stderr,  "Read timeout\n");
    return 0;
  }
  return read(fd, buf, len);
}

/*!
 * @if jp
 * @brief タイムアウトを設けた、write()関数によるデータの書き込みを行う。
 * @else
 * @brief Write data by 'write()' with Time out.
 * @endif
 * @param fd file discriptor
 * @param buf buffer for getting data
 * @param len data size
 * @param sec sec
 * @param usec micro sec
 * @return byte count of data for writing (or 0:Timeout, -1:Fail)
 */
int32_t write_with_timeout(int fd, char *buf, int len, int32_t sec, int32_t usec){
  fd_set socks;
  struct timeval timeout;

  timeout.tv_sec=sec;
  timeout.tv_usec=usec;

  FD_ZERO(&socks);
  FD_SET(fd, &socks);
  if(select(fd+1, 0, &socks, 0, &timeout) == 0){
    fprintf(stderr,  "Write timeout\n");
    return 0;
  }
  return (int32_t)write(fd, buf, len);
}

int32_t readBytes(int32_t fd, char *buf, int32_t len){
  int res;
  char *buf_p = buf;

  while(len){
    res = read(fd, buf_p, len);
    if(res <= 0 ){
      return -1; } 
    len -= res;
    buf_p += res;
  }
  return 1;
}

int32_t writeBytes(int32_t fd, char *buf, int32_t len){
  int res=0;
  char *buf_p = buf;

  while(len){
    res = write(fd, buf_p, len);
    if(res < 0 ){
      fprintf(stderr, "Error in writeByte (%d)\n", res);
      return -1;
    }
    len -= res;
    buf_p += res;
  }
  return 1;
}
/************************************/
/*           Object ID              */

#if defined(USE_UUID)
char *new_ObjectID(){
  uuid_t uuid;
#if defined(FreeBSD)
  char *id;
  uint32_t status;

  uuid_create(&uuid, &status);
  if(status != uuid_s_ok){
    fprintf(stderr, "ERROR in new_ObjectID: uuid_create\n");
    return NULL;
  }
  uuid_to_string(&uuid, &id, &status);
  if(status != uuid_s_ok){
    fprintf(stderr, "ERROR in new_ObjectID: uuid_to_string \n");
    return NULL;
  }
#else
  char *id = (char *)RtORB_alloc(37, "new_ObjectID"); 

  uuid_generate(uuid);
  uuid_unparse(uuid, id);
#endif
  return id;
}
#else
char *new_ObjectID(){
  int fd;
  struct ifreq *ifr;
  struct timeval tv;
#ifndef Linux
  struct timezone tz;
#endif

  struct ifreq *ifend;
  struct ifreq ifreq;
  struct ifconf ifc;
  struct ifreq ifs[64];

  char *ID = (char *)RtORB_alloc(33, "new_ObjectID"); 
  memset(ID, 0, 33);

#ifdef Linux
  gettimeofday(&tv, NULL);
#else
  gettimeofday(&tv, &tz);
#endif

  fd = socket(AF_INET,SOCK_STREAM, 0);

  ifc.ifc_len = sizeof(ifs);
  ifc.ifc_req = ifs;

  memset(ifreq.ifr_hwaddr.sa_data, 0, 6);

  if (fd < 0 || ioctl(fd, SIOCGIFCONF, &ifc) < 0){
    printf("ioctl(SIOCGIFCONF): %m\n");
  }else{
    ifend = ifs + (ifc.ifc_len / sizeof(struct ifreq));
    for (ifr = ifc.ifc_req; ifr < ifend; ifr++){
      if (ifr->ifr_addr.sa_family == AF_INET){
        strncpy(ifreq.ifr_name, ifr->ifr_name,sizeof(ifreq.ifr_name));
        if(ioctl(fd, SIOCGIFHWADDR, &ifreq) > 0) break;
      }
    }
  }
  close(fd);

  sprintf(ID, "RtORB%.2X%.2X%.2X%.2X%.2X%.2X",
	(unsigned char)ifreq.ifr_hwaddr.sa_data[0],
   	(unsigned char)ifreq.ifr_hwaddr.sa_data[1],
   	(unsigned char)ifreq.ifr_hwaddr.sa_data[2],
   	(unsigned char)ifreq.ifr_hwaddr.sa_data[3],
   	(unsigned char)ifreq.ifr_hwaddr.sa_data[4],
  	(unsigned char)ifreq.ifr_hwaddr.sa_data[5]);
  sprintf(ID+16, "%010X", (int)tv.tv_sec);
  sprintf(ID+26, "%06X", (int)tv.tv_usec);

  return ID;
}
#endif

/************************************/
/*        DEBUG                     */
void  dumpMessage(unsigned char *buf, int32_t size){
  int i;

  for(i=0;i<size;i++){
    fprintf(stdout, "%02x ", buf[i]);
    if((i % 8) == 7) fprintf(stdout, "\n");
  }
  fprintf(stdout, "\n");
  return;
}

void dump_value_by_typecode(void *val, CORBA_TypeCode tc){
#if DEBUG
  int i;
  SKIP_ALIAS(tc);

  switch(tc->kind){
    case tk_null:
    case tk_void:
       fprintf(stderr, "NULL or void found\n");
       break;
    case tk_char:
    case tk_octet:
    case tk_boolean:
       fprintf(stderr, "Octet = %c \n", *((char *)val));
       break;
    case tk_short:
    case tk_ushort:
       fprintf(stderr, "Short = %d \n", *((short *)val));
       break;
    case tk_long:
    case tk_ulong:
    case tk_enum:
       fprintf(stderr, "Long = %d \n", (int)(*((int32_t *)val)));
       break;
    case tk_float:
       fprintf(stderr, "Float = %f \n", *((float *)val));
       break;
    case tk_double:
       fprintf(stderr, "Float = %lf \n", *((double *)val));
       break;
    case tk_string:
       fprintf(stderr, "String = %s, 0x%x \n", *((char **)val),  (int)(*((char **)val)));
       break;
    case tk_sequence:
       fprintf(stderr, "Sequence found --0x%x ---\n", val);
       fprintf(stderr, "  len=%d ---\n", ((CORBA_SequenceBase *)val)->_length);
       dump_value_by_typecode(((CORBA_SequenceBase *)val)->_buffer, tc->member_type[i]);
       fprintf(stderr, "==== Sequence\n");
       break;
    case tk_except:
    case tk_struct:
       fprintf(stderr, "Struct found-- 0x%x ----\n", val);
       for(i=0;i<tc->member_count;i++){
         fprintf(stderr, "address val = %x\n", (int)val);
	 dump_value_by_typecode(val,  tc->member_type[i]);
	 val += size_of_typecode(tc->member_type[i], F_DEMARSHAL);
       }
       fprintf(stderr, "==== Struct\n");
       break;
    case tk_union:
       fprintf(stderr, "Union found\n");
       break;
    case tk_objref:
       fprintf(stderr, "Object found\n");
       break;
    case tk_any:
      {
	CORBA_any *any = (CORBA_any*)val;
	fprintf(stderr, "Any found : typecode = %d\n", any->_type->kind);
	void *v = CORBA_any_get_value(any);
	if (v) {
	  switch(any->_type->kind) {
	  case tk_null:
	    fprintf(stderr, "Dump Any(null)\n");
	    break;
	  case tk_string:
	    fprintf(stderr, "Dump Any(string) : %s\n", *(char**)v);
	    break;
	    /*
	  case tk_objref:
	    fprintf(stderr, "Dump Any(Object) : %s\n", (char*)any->_value+);
	    break;
	    */
	  default:
	    fprintf(stderr, "Dump Any(not implemented)\n");
	    break;
	  }
	}
      }
      break;
    default:
       fprintf(stderr, "Unknown typecode %d\n", (int)tc->kind);
       break;
  }
#endif
  return;
}

/*******************************************/
char ixdigit(char a){ return  Char2Hex[a -'0']; }  /* '0':48 */

char hexdigit(char c){
   char buf = c & 0x0F;
   if (buf < 10) return '0' + buf;
   return 'a' + buf - 10;  /* 'a':97 */
}

octet *String2Octet(char *str){
  octet *result;
  int i,j,len2;
  int len = strlen(str);

  len2 = len >> 1;
  result = (octet *)RtORB_alloc(len2, "String2Octet");

  for(i=0;i<len2;i++){
    j=2*i;
    result[i] = (ixdigit(str[j]) << 4) | ixdigit(str[j+1]);
  }
  return result;
}

/*!
 * @if jp
 * @brief  Octet型(8 binary bit)をCharacter文字に変換し、文字列(配列)とする。
 * @else
 * @brief  Convert Octet to String.
 * @endif
 * @param str Octet data
 * @param len length of Octet data
 * @return character array (=String)
 */
char *Octet2String(octet *str, int len){
  char *result;
  int i,len2;

  len2 = len << 1;
  result = (char *)RtORB_alloc(len2, "Octet2String");

  for(i=0;i<len;i++){
    result[i*2] = Hex2Char[str[i] >> 4];
    result[i*2+1] = Hex2Char[str[i] & 0x0f];
  }
  return result;
}

void
String__delete(char *str, char *info){
 if(str) RtORB_free(str, info);
}

int32_t
RtORB_strlen(char *str){
 if (str) return strlen(str);
 return 0;
}

char *Get_IP_Address(){
  if(_ORB_){
    return RtORB_strdup(_ORB_->hostname, "Get_IP_Address");
  }else{
    char *addr;

    int fd = socket(AF_INET,SOCK_STREAM, 0);
    addr = (char *)get_ip_address(fd);
    close(fd);

    return addr;
  }
}
