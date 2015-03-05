/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/* $Id: server_loop.c,v 1.2 2008/05/29 04:54:56 yoshi Exp $ */
/*!
 * @file server_loop.c
 * @if jp
 * @brief Serverループ処理。ソケットの初期化、select()システムコールによる入力待ち処理と、データ受信後のリクエスト処理を行う。
 * @else
 * @brief Server roop action
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#include <RtORB/corba.h>

void server_loop(float time_out_float, void (*idle)(void*), void *arg)
{
  struct timeval time_out;

  init_socket_servers();

  time_out.tv_sec = (uint32_t)(time_out_float / 1000);
  time_out.tv_usec = (time_out_float - time_out.tv_sec * 1000) * 1000 ;
#if DEBUG
  fprintf(stderr, "timeout = %d, %d\n", (int)time_out.tv_sec, (int)time_out.tv_usec);
#endif

  while (1) {
#if _CYCLIC_TASK
      gettimeofday(&time1, &tz); 
#endif

      select_socket_servers(time_out);

      if (idle){
        (*idle)(arg); 
      }

#if _CYCLIC_TASK
      gettimeofday(&time2, &tz); 

      time_cost = (time2.tv_sec - time1.tv_sec) * 1000000 + time2.tv_usec
	      - time1.tv_usec - time_out_float * 1000 ;
      if (time_cost > 0) usleep(time_cost);
#endif 
  }
  return;
}

