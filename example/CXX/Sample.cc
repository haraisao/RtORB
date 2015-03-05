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
 *  $Id: Sample.cc,v 1.1.1.1 2008/01/08 07:56:08 yoshi Exp $
*/
#include "echo.h"

/*
 *
 */

class Echo_i : public Echo_impl
{
  public:
    long status;

  public:
   Echo_i(PortableServer::POA_ptr);

   char * echoString( const char *);
   char * echoString2(const char *, CORBA_string *);
   char * echoString3(CORBA_string *);
   long plus(long , long);
   void plus2(long , long, long *);
   float divide(float, float) ;
   void divide2(float, float, float *);
};

Echo_i::Echo_i(PortableServer::POA_ptr poa) : Echo_impl(poa){ }

char *Echo_i::echoString(const char *mesg){
  std::cerr << "Call echoString" << std::endl;
//  std::cerr << this->_impl << ": "<< this->status++  << std::endl;
  return strdup(mesg);
}

char *Echo_i::echoString2(const char *mesg, CORBA_string *res){
}
char *Echo_i::echoString3(CORBA_string  *mesg){
}
long Echo_i::plus(const long x, const long y){
}
void Echo_i::plus2(const long x, const long y, long *ans){
}
float Echo_i::divide(const float x, const float y){
}
void Echo_i::divide2(const float x, const float y, float *ans){
}

/*
 *
 */

void
hello(Echo_i *e){
  char src[] = "Hello! World.";
  char *res = "";

  res = e->echoString(src);

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
  int i;

  try{
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);


    Echo_i *myEcho = new Echo_i(poa);

    hello(myEcho);

#if 0
    hello2(myEcho, &env);
    kakeru(myEcho, &env);
    plus(myEcho, &env);
#endif

    orb->shutdown(true);
    orb->destroy();
  }catch(...){
    std::cerr << " Catch error." << std::endl;
  }
  return 1;
}

