/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 *  Simple CORBA Server
 *
 *  Copyright(C) 2006, Isao Hara.AIST
 *
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/01/08 07:56:08 $
 *  $Id: Server.c,v 1.1.1.1 2008/01/08 07:56:08 yoshi Exp $
*/
#include <stdio.h>
#include <pthread.h>
#include <RtORB/corba.h>
#include "echo.h"


int main(int argc, char **argv){
  CORBA_Environment env;
  CORBA_ORB orb;

  orb = (CORBA_ORB)CORBA_ORB_init(&argc, argv, 0, &env);

  PortableServer_POA poa = (PortableServer_POA)
	  CORBA_ORB_resolve_initial_references(orb,"RootPOA", &env);
  catchDefaultException(&env);

  Echo myEcho = impl_Echo__create(poa, &env);
  catchDefaultException(&env);

  fprintf(stderr, " %s \n", CORBA_ORB_object_to_string(orb, myEcho, &env));

////////////////  
  PortableServer_POAManager poa_manager =
         (PortableServer_POAManager)PortableServer_POA__get_the_POAManager(poa, &env);
  PortableServer_POAManager_activate(poa_manager, &env);

/////////////////

  CORBA_ORB_run(orb, &env);

  fprintf(stderr, "Terminate CORBA_ORB_run()\n");
  CORBA_ORB_shutdown(orb, 1, &env);

}
