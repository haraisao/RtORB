/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 *  RtORB:
 *    pthread.c
 *
 *  Copyright(C) 2006, Isao Hara, AIST
 *
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/01/08 07:56:06 $
 *  $Id: pthread.c,v 1.1.1.1 2008/01/08 07:56:06 yoshi Exp $
 *
 */
/*!
 * @file pthread.c
 * @if jp
 * @brief 
 * @else
 * @brief 
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#include <RtORB/corba.h>

#ifdef USE_THREAD
pthread_t *
RunThread(pthread_t *thr, void *(*func)(), void *arg, int detach){
    pthread_create(thr, NULL, func, arg);
    if(detach) pthread_detach(*thr);
    return thr;
}
#endif
