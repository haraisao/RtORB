/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 *  sockport.h
 *	Copyright(C) 2006, Isao Hara, AIST
 *
 *  $Revision: 1.3 $
 *  $Date: 2008/05/29 04:54:57 $
 *  $Id: sockport.h,v 1.3 2008/05/29 04:54:57 yoshi Exp $
 *
 */
/*!
 * @file sockport.h
 * @if jp
 * @brief RtORB通信用ソケットポートのプロファイル情報の定義
 * @else
 * @brief Definition profiles of socket port for RtORB. 
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __SOCKPORT_H__
#define __SOCKPORT_H__

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/time.h>

#include <sys/ioctl.h>
#include <net/if.h>

#ifdef __cplusplus
extern "C"
{
#endif 

#ifndef FD_SETSIZE
#define FD_SETSIZE	32
#endif
#ifndef NFDBITS
#define NFDBITS		32
#endif

#ifndef __SOCK_ENUM__
#define __SOCK_ENUM__
/*! Socket Type */
enum {
  SOCK_CLOSED=0, SOCK_SERVER, SOCK_SERVICE, SOCK_CLIENT
};


/*!
 * @struct sockport_profile
 * @brief Socket Port Profile
 *
 *        GIOPの低レベルSocket管理用。接続断時の処理登録、メッセージ受信時の処理をSocketごとに設定可能。
 *        実際には、connection_proc, disconnection_procともに　NULLが代入され、すべてのSocketに対して、
 *        command_proc = RecvMessage; となる。
 *        配列長の[FD_SETSIZE]は、selectシステムコール時に待つことができるFDの最大値のため、膨大な数の可能性があり使用に注意が必要。
 * @param type Socket Type (CLOSED/SERVER/SERVICE/CLIENT)
 * @param arg argument of function
 * @param connection_proc function pointer of connect action
 * @param command_proc function pointer of request action
 * @param disconnect_proc function pointer of disconnect action
 */
struct sockport_profile{
  int32_t type;
  void *arg;
  int32_t (*connection_proc)(int, void*);
  int32_t (*command_proc)(GIOP_ConnectionHandler*);
  int32_t (*disconnect_proc)(int, void*);
};

#endif

/* 
   Global variable for socket FD
 */
extern  struct sockport_profile  SockProfile[FD_SETSIZE];
/*
  2007/10/08 yoshi@msi.co.jp

  This causes compile error.
*/
/* extern  fd_set main_socket_bits;*/

#ifdef __cplusplus
}
#endif 

#endif
