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
 *  $Revision: 1.3 $
 *  $Date: 2008/03/12 10:51:54 $
 *  $Id: Client.cc,v 1.3 2008/03/12 10:51:54 yoshi Exp $
*/
#include <sys/time.h>
#include "echo.h"

/*
 *
 */
void *get_value_by_name(CORBA_any *val, char *label);

CORBA::Object_ptr
getObjectReference(CORBA::ORB_ptr orb){
  CosNaming::NamingContext_var rootContext;

  try{
    CORBA::Object_var obj = orb->resolve_initial_references("NameService");
    rootContext = CosNaming::NamingContext::_narrow(obj);

    if(! rootContext  ) {
      std::cerr << "Fail to resolve NameService" << std::endl;
      return (void*)NULL;
    }
//  }catch(CORBA::ORB::InvalidName& ex) {
  }catch(...) {
    std::cerr << "Service required is invalid [does not exist]." << std::endl;
    return (void*)NULL;
  }

  CosNaming::Name name;
  name.length(2);

  name[0].id   = (const char *)"test";
  name[0].kind = (const char *)"my_context";
  name[1].id   = (const char *)"Echo";
  name[1].kind = (const char *)"Object";

  try{

    return  rootContext->resolve(name);

  } catch(CosNaming::NamingContext::NotFound& ex) {
    std::cerr << "Context not found." << std::endl;
#if 0
      CosNaming_NamingContext_NotFound *ex =
		  (CosNaming_NamingContext_NotFound *)env->_params._value;

     fprintf(stderr, "why = %d \n", ex->why);
     for(i=0;i< ex->rest_of_name._length; i++){
      fprintf(stderr, "id[%d] = %s \n", i, ex->rest_of_name._buffer[i].id);
      fprintf(stderr, "kind[%d] = %s \n",i,  ex->rest_of_name._buffer[i].kind);
     }
#endif
  }catch(CORBA::SystemException &ex){
    std::cerr << " System Exception!!" << std::endl;
  }catch(...){
    std::cerr << " Unknown Error!!" << std::endl;
  }
  return (void*)NULL;
}


void hello(Echo_ptr e){
  const char *src = "<html>\n <head>\n <title>TEST HTML</title>\n</head>\n <body>\n <h2>\n Hello, My Home Page\n </h2>\n </body>\n </html>";
  char *res = "";

//  res = Echo_echoString(e->_impl, src,  &e->ev);
  res = e->echoString(src);

//  fprintf(stderr, "I said, \"%s\". The Echo object replied, \"%s\"\n", src, res);
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
  struct timeval tv1, tv2;
  struct timezone tz;
  float dt;
  int i;

  int n_loop;

  try{
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    CORBA::Object_var obj = getObjectReference(orb);
    Echo_var myecho = Echo::_narrow(obj);

	   
    if(!myecho){
      orb->destroy();
      exit(1);
    }

    n_loop = 100000;

    gettimeofday(&tv1, &tz);
    for(i=0;i<n_loop;i++){
      hello(myecho);
#if 0
      hello2(myecho, &myecho->ev);
      divide(myecho, &myecho->ev);
      plus(myecho, &myecho->ev);
#endif
    }
    gettimeofday(&tv2, &tz);

    dt =(tv2.tv_sec-tv1.tv_sec)*1000+(tv2.tv_usec-tv1.tv_usec)/1000.0;
    std::cerr << "Time "<< dt <<  "msec" << std::endl ;

    orb->destroy();
  }catch(...){
    std::cerr << "Unknown error occured."<< std::endl;
  }
  return 1;
}
