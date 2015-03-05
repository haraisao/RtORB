/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/* $Id: server_loop.h,v 1.2 2008/05/29 04:54:57 yoshi Exp $ */ 
/*!
 * @file server_loop.h
 * @if jp
 * @brief Serverループ処理
 * @else
 * @brief Server roop action
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __FUNC_SERVER_LOOP_H__
#define __FUNC_SERVER_LOOP_H__

/*!
 * @if jp
 * @brief Serverループ処理。ソケットの初期化、select()システムコールによる入力待ち処理と、データ受信後のリクエスト処理を行う。
 * @else
 * @brief Server roop action. Initialize socket, waiting for input data by select() and sending request after receiving data.
 * @endif
 * @param time_out_float interval time of select() function
 * @param idle function pointer which is called after receiving data
 * @param arg argument of function
 * @return void
 */
void server_loop(float time_out_float, void (*idle)(void*), void *arg);

#endif /* __FUNC_SERVER_LOOP_H__ */
 
