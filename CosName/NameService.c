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
 *  $Revision: 1.3 $
 *  $Date: 2008/05/29 04:54:57 $
 *  $Id: NameService.c,v 1.3 2008/05/29 04:54:57 yoshi Exp $
*/
/*!
 * @file NameService.c
 * @if jp
 * @brief Simple CORBA Server (NameService)
 * @else
 * @brief Simple CORBA Server (NameService)
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <RtORB/corba.h>
#include "CosNaming.h"

extern CORBA_CosNaming_NamingContext
impl_CosNaming_NamingContext__create(PortableServer_POA, CORBA_Environment *);

static CORBA_ORB orb = NULL;  /*!< TODO */

/*!
 * @if jp
 * @brief CORBA ORBをシャットダウンし削除する。(強制終了処理)
 * @else
 * @brief Shut down and destroy CORBA ORB
 * @endif
 * @param arg (TODO)
 * @return void
 */
static void quit(int arg)
{
  CORBA_Environment ev;
  if (orb) {
    printf("Destroying ORB...\n");
    CORBA_ORB_shutdown(orb, 1, &ev);
    CORBA_ORB_destroy(orb, &ev);
    orb = NULL;
    printf("done\n");
  }
  exit(0);
}

/*!
 * @if jp
 * @brief 説明 (TODO)
 * @else
 * @brief Abstract (TODO)
 * @endif
 * @param argc (TODO)
 * @param argv (TODO)
 * @return error code
 */
int main(int argc, char **argv){
  CORBA_Environment env;    /*!< TODO */

  /////  Set default port = 2809
  int i;
  int default_port = 1;

  for(i=1;i<argc;i++){
    if(strcmp(argv[i], "-ORBServerPort") == 0 ) {
	default_port = 0;
        break;
    }
  }

  if(default_port){
    argv[argc++] = "-ORBServerPort";
    argv[argc++] = "2809";
  }
//////
  // create and initialize CORBA ORB
  orb = (CORBA_ORB)CORBA_ORB_init(&argc, argv, 0, &env);

  // set signal to quit() 
  signal(SIGINT, quit);
  signal(SIGTERM, quit);

  //get RootPOA Reference
  CORBA_Object poa_obj = CORBA_ORB_resolve_initial_references(orb,"RootPOA", &env);
  PortableServer_POA poa = poa_obj->poa;
  catchDefaultException(&env);

  //create Naming context
  CosNaming_NamingContext rootContext = (CosNaming_NamingContext)impl_CosNaming_NamingContext__create(poa, &env);
  catchDefaultException(&env);

  fprintf(stderr, " %s \n", CORBA_ORB_object_to_string(orb, rootContext, &env));
  PortableServer_POA_reinstall_object(poa, rootContext, "NameService", &env);

////////////////  
  // get POA Manager
  PortableServer_POAManager poa_manager =
         (PortableServer_POAManager)PortableServer_POA__get_the_POAManager(poa, &env);

  // activate POA Manager
  PortableServer_POAManager_activate(poa_manager, &env);

/////////////////

  // run ORB --> server Roop action
  CORBA_ORB_run(orb, &env);

  fprintf(stderr, "Terminate CORBA_ORB_run()\n");
  CORBA_ORB_shutdown(orb, 1, &env);

  exit(1);
}
