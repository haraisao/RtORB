/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 *  Simple CORBA Client
 *
 *  Copyright(C) 2006, Isao Hara.AIST
 *
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/01/08 07:56:08 $
 *  $Id: Client.c,v 1.1.1.1 2008/01/08 07:56:08 yoshi Exp $
*/
#include <stdio.h>
#include <pthread.h>
#include <RtORB/corba.h>
#include <sys/time.h>
#include "echo.h"
#include "CosName/CosNaming.h"


/*
 *
 */

void *get_value_by_name(CORBA_any *val, char *label);

CORBA_Object
getObjectReference(CORBA_ORB orb, CORBA_Environment *env){

  CosNaming_NamingContext rootContext;
  CORBA_Object retval;

  rootContext = (CosNaming_NamingContext)CORBA_ORB_resolve_initial_references(orb, "NameService", env);
  if(! rootContext  ) {
     fprintf(stderr, "Fail to resolve NameServicn");
     return NULL;
  }
    CosNaming_Name *name = CosNaming_Name__alloc();
    name->_buffer = CosNaming_Name_allocbuf(2);
    name->_length = name->_maximum = 2;

    name->_buffer[0].id   = "test";       // string copied
    name->_buffer[0].kind = "my_context"; // string copied
    name->_buffer[1].id   = "Echo";
    name->_buffer[1].kind = "Object";

    retval = (CORBA_Object)CosNaming_NamingContext_resolve(rootContext, name, env);

    switch(env->_major){
      case CORBA_NO_EXCEPTION:
	 return retval;
      case CORBA_USER_EXCEPTION:
        fprintf(stderr, " UserExcption: %s\n", env->_repo_id);
        if(!strcmp(ex_CosNaming_NamingContext_NotFound, env->_repo_id)){
	  int i;
	  CosNaming_NamingContext_NotFound *ex =
		  (CosNaming_NamingContext_NotFound *)env->_params._val;

          fprintf(stderr, "why = %d \n", ex->why);
	  for(i=0;i< ex->rest_of_name._length; i++){
           fprintf(stderr, "id[%d] = %s \n", i, ex->rest_of_name._buffer[i].id);
           fprintf(stderr, "kind[%d] = %s \n",i,  ex->rest_of_name._buffer[i].kind);
	  }
	}
	break;
      case CORBA_SYSTEM_EXCEPTION:
        fprintf(stderr, " System Exception!!\n");
	break;
      default:
        fprintf(stderr, " Unknown Error!!\n");
	break;
   }

   return NULL;
}




void
hello(Echo e, CORBA_Environment *env){
  char src[] = "<html>\n <head>\n <title>TEST HTML</title>\n</head>\n <body>\n <h2>\n Hello, My Home Page\n </h2>\n </body>\n </html>";
  char *res = "";

  /*
  char **arg;
  *arg = src;
  */

  res = Echo_echoString(e, src,  env);
  //res = Echo_echoString(e, arg,  env);

  fprintf(stderr, "I said, \"%s\". The Echo object replied, \"%s\"\n", src, res);
  //free(res);
  return;
}

#if 1
void
hello2(Echo e, CORBA_Environment *env){
  char src[] = "Hello2 from client";
  char *res = "";
  char *res1;

  /*
  char **arg;
  *arg = strdup(src);
  */

  res = Echo_echoString2(e, src, &res1, env);
  fprintf(stderr, " The Echo object hello2, \"%s\" -- \"%s\"\n", src, res1);
//  fprintf(stderr, " The Echo object hello2, \"%s\" -- \"%s\"\n", *arg, res);
//  fprintf(stderr, " The Echo object hello2, \"%s\" -- \"%s\"\n", src, res);
//  free(*arg);
  return;
}

void
plus(Echo e, CORBA_Environment *env){
  long val;
  long val2;
  long a, b;

   a=10; b=20;

  val = Echo_plus(e, a, b,  env);
  Echo_plus2(e, b, a, &val2,  env);

  fprintf(stderr, " %d + %d =%d \n", a, b, val);
  fprintf(stderr, " %d + %d =%d \n", b, a, val2);
}

void
divide(Echo e, CORBA_Environment *env){
  float val;
  float val2;
  float a, b;

  a=2.0; b=3.0;

  val = Echo_divide(e, a, b,  env);
  Echo_divide2(e, b, a, &val2,  env);

  fprintf(stderr, " %f / %f =%f \n", a, b, val);
  fprintf(stderr, " %f / %f =%f \n", b, a,val2);
}

#endif

int
main(int argc, char **argv){
  CORBA_Environment env;
  CORBA_ORB orb;
  char *ior;
  int i;
  struct timeval tv1, tv2;
  struct timezone tz;
  float dt;

  int n_loop;

  orb = (CORBA_ORB)CORBA_ORB_init(&argc, argv, 0, &env);

#if 0
  if(argc != 2){
    fprintf(stderr, "usage: client <object reference>\n");
     exit(1);
  }
  ior = strdup(argv[1]);

fprintf(stderr, "string_to_object\n");
  Echo myEcho = (Echo)CORBA_ORB_string_to_object(orb, ior, &env);
fprintf(stderr, "... end string_to_object\n");

  catchDefaultException(&env);
  CORBA_Object__narrow(myEcho, TC_Echo, &env);
  catchDefaultException(&env);
	  
  free(ior);
#else
  Echo myEcho = (Echo)getObjectReference(orb, &env);

  if(!myEcho){
    CORBA_ORB_shutdown(orb, 1,  &env);
    CORBA_ORB_destroy(orb, &env);
    exit(1);
  }
#endif
  //n_loop = 10000;
  n_loop = 1;

  gettimeofday(&tv1, &tz);
  for(i=0;i<n_loop;i++){
fprintf(stderr, "hello");
    hello(myEcho, &env);
#if 0
    hello2(myEcho, &env);
    divide(myEcho, &env);
    plus(myEcho, &env);
#endif
fprintf(stderr, "...end\n");
  }
  gettimeofday(&tv2, &tz);

  dt = (tv2.tv_sec - tv1.tv_sec) * 1000 + (tv2.tv_usec - tv1.tv_usec) / 1000.0;
  fprintf(stderr, "Time %f ms\n", dt);


  if(orb)CORBA_ORB_shutdown(orb, 1,  &env);
  if(orb) CORBA_ORB_destroy(orb, &env);
}
