/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 *  RtORB: corba.c
 *
 *  Copyright(C) 2006, Isao Hara, AIST
 *
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/01/08 07:56:06 $
 *  $Id: corba.c,v 1.1.1.1 2008/01/08 07:56:06 yoshi Exp $
 */
/*!
 * @file corba.c
 * @if jp
 * @brief output assertion message.
 * @else
 * @brief output assertion message.
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#include <RtORB/corba.h>

void do_assert(boolean c, const char* info){
  if(!c) fprintf(stderr, "Asserttion error : %s \n", info);
}

