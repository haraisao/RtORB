/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*!
 * @file connection.c
 * @if jp
 * @brief Server,Client間をsocketによる通信をするための接続処理を行う。
 * @else
 * @brief Connection between Server and Client by socket network.
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netdb.h>
#include <ctype.h>
#include <RtORB/corba.h>

#define use_socket(CONN) 1

/*!
 * @if jp
 * @brief ServerにてClientからの接続待ち用にSocketを作成し、Portを割り当てる。 (refer giop.c)
 * @else
 * @brief Create 'socket' and assign 'port' for Server connection from Client.
 * @endif
 * @param conn Profile of GIOP Connection
 * @param port socket port
 * @return socket descriptor (or -1:Fail)
 */
int make_server_connection(GIOP_Connection *conn, int port)
{
  int sock = make_server_socket_port(port);
  if (sock < 0) { return -1; }

  if(!conn->port) conn->port = get_socket_port(sock);
  else conn->port = port;

  conn->sock = sock;
  return sock;
}

GIOP_ConnectionHandler * make_client_connection(GIOP_Connection *conn)
{
  GIOP_ConnectionType type;
  GIOP_ConnectionHandler *h = NULL;

  if (use_socket(conn)) {
    if (!conn->sock) {
      int sock = make_client_socket_port((char*)conn->hostname, conn->port);
      if (sock < 0) {
	return NULL;
      }
      conn->sock = sock;
    }
    type = CONNECTION_TYPE_SOCKET;
  }

  h = (GIOP_ConnectionHandler *)RtORB_alloc(sizeof(GIOP_ConnectionHandler), "make_client_connection");

  h->type = type;
  h->sock = conn->sock;

  return h;
}

