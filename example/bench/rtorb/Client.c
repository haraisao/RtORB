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
call_echoString(RTC_Echo e, CORBA_Environment *env){
  char src[] = "<html>\n <head>\n <title>TEST HTML</title>\n</head>\n <body>\n <h2>\n Hello, My Home Page\n </h2>\n </body>\n </html>";
  char *res;

  res = RTC_Echo_echoString(e, src,  env);
  RtORB_free(res, "call_echoString");

  return;
}

void
call_TimedLongSeq(RTC_Echo e, CORBA_Environment *env){
  RTC_TimedLongSeq *res = NULL;
  struct timezone tz;
  int i;
  RTC_TimedLongSeq val;

  gettimeofday((struct timeval *)&val.tm, &tz);

  val.data._length = val.data._maximum = 6;
  val.data._buffer = CORBA_sequence_CORBA_long_allocbuf(6);
//  val.data._buffer = alloca(sizeof(long) * 6);
  for(i=0;i<val.data._length;i++){
     val.data._buffer[i] = i;
  }

  res =  RTC_Echo_echoTimedLongSeq(e, &val, env);
  
#if 0
  fprintf(stderr, "Length = %d\n",  res->data._length);

  for(i=0;i<res->data._length;i++){
     fprintf(stderr, "Val[%d] = %d  --->", i, val.data._buffer[i]);
     fprintf(stderr, "Result[%d] = %d\n", i, res->data._buffer[i]);
  }
#endif

  RtORB_free(val.data._buffer, "call_TimedLongSeq(val.data._buffer)");
//  RtORB_free(res->data._buffer, "call_TimedLongSeq(res->data._buffer)");
  RTC_TimedLongSeq__freekids(res, NULL);

  return;
}

int
main(int argc, char **argv){
  CORBA_Environment env;
  CORBA_ORB orb;
  char *ior;
  int i;
  struct timeval tv1, tv2;
  struct timezone tz;
  float dt;
  int n_loop = 10000;

  orb = (CORBA_ORB)CORBA_ORB_init(&argc, argv, 0, &env);

  RTC_Echo myEcho = (RTC_Echo)getObjectReference(orb, &env);

  if(!myEcho){
    CORBA_ORB_shutdown(orb, 1,  &env);
    CORBA_ORB_destroy(orb, &env);
    exit(1);
  }

  n_loop = 100000;
  if(argc > 1) n_loop = atoi(argv[1]);

  gettimeofday(&tv1, &tz);

  for(i=0;i<n_loop;i++){
    call_echoString(myEcho, &env);
    call_TimedLongSeq(myEcho, &env);
  }
  gettimeofday(&tv2, &tz);

  dt = (tv2.tv_sec - tv1.tv_sec) * 1000 + (tv2.tv_usec - tv1.tv_usec) / 1000.0;
  fprintf(stderr, "Time %f ms\n", dt);


  if(orb)CORBA_ORB_shutdown(orb, 1,  &env);
  if(orb) CORBA_ORB_destroy(orb, &env);
}
