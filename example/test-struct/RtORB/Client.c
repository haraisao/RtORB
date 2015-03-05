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

void
echo_bind(Echo e, CORBA_Environment *env){
  Name *name;

  name = (Name *)malloc(sizeof(Name));
  name->_maximum = 1;
  name->_length = 1;
  name->_buffer = (NameComponent *)malloc(sizeof(NameComponent));

  name->_buffer[0].id = "test";
  name->_buffer[0].kind = "my_context";

  Echo_bind(e, name, e,  env);

  return;
}

void
main(int argc, char **argv){
  CORBA_Environment env;
  CORBA_ORB orb;
  char *ior;
  int i;

  orb = (CORBA_ORB)CORBA_ORB_init(&argc, argv, 0, &env);

  if(argc != 2){
    fprintf(stderr, "usage: client <object reference>\n");
     exit(1);
  }
  ior = strdup(argv[1]);

  Echo myEcho = (Echo)CORBA_ORB_string_to_object(orb, ior, &env);
  catchDefaultException(&env);

  free(ior);

  hello(myEcho, &env);
  echo_bind(myEcho, &env);


  if(orb)CORBA_ORB_shutdown(orb, 1,  &env);
  if(orb) CORBA_ORB_destroy(orb, &env);
}
