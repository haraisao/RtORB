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
#include "CosName/CosNaming.h"

CORBA_boolean
bindObjectToName(CORBA_ORB orb, CORBA_Object obj, CORBA_Environment *env){
  CosNaming_NamingContext rootContext;
  CosNaming_Name *contextName;
  CosNaming_NamingContext testContext;
  CosNaming_Name *objectName;

  rootContext = CORBA_ORB_resolve_initial_references(orb, "NameService", env);
  catchDefaultException(env);

  contextName = CosNaming_Name__alloc();
  contextName->_buffer = CosNaming_Name_allocbuf(1);
  contextName->_length = contextName->_maximum = 1;

  contextName->_buffer[0].id = "test";
  contextName->_buffer[0].kind = "my_context";

  testContext = CosNaming_NamingContext_bind_new_context(CORBA_Object_dup(rootContext), contextName, env);

  if(catchException(env, ex_CosNaming_NamingContext_AlreadyBound) ){
    fprintf(stderr, "!!!! Already Bound %s, %s \n",
	    contextName->_buffer[0].id ,contextName->_buffer[0].kind ); 
    testContext = CosNaming_NamingContext_resolve(rootContext, contextName, env);
    if(catchException(env, NULL) ){
      return 0;
    }
  }else if(catchException(env, "SystemException")){
    fprintf(stderr, "SystemException  \n"); 
    return 0;
  }else if(env->_major) return 0;


  objectName = CosNaming_Name__alloc();
  objectName->_buffer = CosNaming_Name_allocbuf(1);
  objectName->_length = objectName->_maximum = 1;

  objectName->_buffer[0].id = "Echo";
  objectName->_buffer[0].kind = "Object";

  CosNaming_NamingContext_bind(testContext, objectName, obj, env);

  if(catchException(env, ex_CosNaming_NamingContext_AlreadyBound) ){
    fprintf(stderr, "Sorry, Already_Bound  \n"); 
    CosNaming_NamingContext_rebind(testContext, objectName, obj, env);
    if(catchException(env, NULL) ){
      fprintf(stderr, "Error, %s \n", env->_repo_id ); 
      return 0;
    }
  }else if(catchException(env, "SystemException")){
    fprintf(stderr, "SystemException  \n"); 
    return 0;
  }else if(catchException(env, NULL) ){
    fprintf(stderr, "Error, %s \n", env->_repo_id ); 
    return 0;
  }

  return 1;
}

int main(int argc, char **argv){
  CORBA_Environment env;
  CORBA_ORB orb;

  orb = (CORBA_ORB)CORBA_ORB_init(&argc, argv, 0, &env);

  CORBA_Object poa_obj = (CORBA_Object)
	  CORBA_ORB_resolve_initial_references(orb,"RootPOA", &env);

  PortableServer_POA poa = poa_obj->poa;

  catchDefaultException(&env);

  Echo myEcho = (Echo)impl_Echo__create(poa, &env);
  catchDefaultException(&env);

#if NO_NAMESERVICE
  fprintf(stderr, " %s \n", CORBA_ORB_object_to_string(orb, myEcho, &env));
  fprintf(stderr, "\nObject_key =  %s \n", myEcho->object_key);
#else
  if(!bindObjectToName(orb, myEcho, &env)) return 1;
#endif

////////////////  
  PortableServer_POAManager poa_manager =
         (PortableServer_POAManager)PortableServer_POA__get_the_POAManager(poa, &env);
  PortableServer_POAManager_activate(poa_manager, &env);

/////////////////

  CORBA_ORB_run(orb, &env);

  fprintf(stderr, "Terminate CORBA_ORB_run()\n");
  CORBA_ORB_shutdown(orb, 1, &env);

}
