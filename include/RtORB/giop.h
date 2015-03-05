/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*!
 * @file giop.h
 * @if jp
 * @brief GIOP定義情報のincludeとClient接続用functionの定義
 * @else
 * @brief Including GIOP-definition , Function for Client connection.
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __GIOP_H__
#define __GIOP_H__

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <unistd.h>

#include <RtORB/giop-defs.h>
#include <RtORB/util.h>


#ifdef __cplusplus
extern "C"
{
#endif 

/*!
 * @if jp
 * @brief ClientからServer接続用にSocketを作成し、Portやホストを割り当てる。 GIOP接続用ハンドラを用意し、これらを割り当て、返す。  (refer rtorb.c)
 * @else
 * @brief Create 'socket' and assign 'port' and 'HOST' for Client connection to Server. Make GIOP_Connection_Handler and asign each param, and return it. 
 * @endif
 * @param conn Profile of GIOP Connection
 * @return Handler of GIOP Connection
 */
GIOP_ConnectionHandler * make_client_connection(GIOP_Connection *conn);

#ifdef __cplusplus
}
#endif 

#endif
