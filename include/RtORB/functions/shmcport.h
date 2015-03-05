/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/* $Id: shmcport.h,v 1.2 2008/05/29 04:54:57 yoshi Exp $ */ 
/*!
 * @file shmcport.h
 * @if jp
 * @brief 
 * @else
 * @brief 
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __FUNC_SHMCPORT_H__
#define __FUNC_SHMCPORT_H__

#ifdef USE_SHMC

/*!
 * @if jp
 * @brief 
 * @else
 * @brief 
 * @endif
 * @param h 
 * @param type 
 * @param connection_proc 
 * @param disconnect_proc 
 * @param command_proc 
 * @return void
 */
void set_ShmcProfile(Shmc *h, int type, 
		     int (*connection_proc)(),
		     int (*disconnect_proc)(),
		     int (*command_proc)()
		     );

/*!
 * @if jp
 * @brief 
 * @else
 * @brief 
 * @endif
 * @param h 
 * @param arg 
 * @return void
 */
void set_ShmcProfile_arg(Shmc *h, void *arg);

/*!
 * @if jp
 * @brief 
 * @else
 * @brief 
 * @endif
 * @param h 
 * @return void
 */
void *get_ShmcProfile_arg(Shmc *h);

/*!
 * @if jp
 * @brief 
 * @else
 * @brief 
 * @endif
 * @return void
 */
void init_shmc_servers();

/*!
 * @if jp
 * @brief 
 * @else
 * @brief 
 * @endif
 * @param timeout 
 * @return void
 */
void select_shmc_servers(struct timeval timeout);

#endif

#endif /* __FUNC_SHMCPORT_H__ */
 
