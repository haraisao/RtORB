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
 *  $Id: Sample.c,v 1.1.1.1 2008/01/08 07:56:08 yoshi Exp $
*/
#include <stdio.h>
#include <pthread.h>
#include <RtORB/corba.h>
#include "echo.h"

/*
 *
 */

void
hello(Echo e, CORBA_Environment *env){
  char src[] = "Hello! World.";
  char *res = "";

  res = Echo_echoString(e, src,  env);

  fprintf(stderr, "I said, \"%s\". The Echo object replied, \"%s\"\n", src, res);
  free(res);
  return;
}

#if 0
void
hello2(Echo e, CORBA_Environment *env){
  char src[] = "Hello2 from client";
  char *res = "";
  char *res1;

  res = Echo_echoString2(e, src, &res1, env);
  fprintf(stderr, " The Echo object hello2, \"%s\" -- \"%s\"\n", src, res1);
  return;
}

void
plus(Echo e, CORBA_Environment *env){
  long val;
  long val2;

  val = Echo_plus2(e, &val2, 10, 20,  env);

  fprintf(stderr, " 10 + 20 =%d \n", val);
  fprintf(stderr, " 10 - 20 =%d \n", val2);
}

void
kakeru(Echo e, CORBA_Environment *env){
  float val;
  float val2;

  val = Echo_kakeru2(e, &val2, (float)2.0, (float)3.0,  env);

  fprintf(stderr, " 2 * 3 =%f \n", val);
  fprintf(stderr, " 2 / 3 =%f \n", val2);
}
#endif

int
main(int argc, char **argv){
  CORBA_Environment env;
  CORBA_ORB orb;
  char *ior;
  int i;

  orb = (CORBA_ORB)CORBA_ORB_init(&argc, argv, 0, &env);

  PortableServer_POA poa = (PortableServer_POA)
	  CORBA_ORB_resolve_initial_references(orb,"RootPOA", &env);
  catchDefaultException(&env);


  Echo myEcho = (Echo)impl_Echo__create(poa, &env);

  hello(myEcho, &env);
#if 0
  hello2(myEcho, &env);
  kakeru(myEcho, &env);
  plus(myEcho, &env);
#endif


  CORBA_ORB_shutdown(orb, 1,  &env);
  CORBA_ORB_destroy(orb, &env);
}

