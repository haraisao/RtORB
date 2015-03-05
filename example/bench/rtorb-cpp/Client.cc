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
 *  $Revision: 1.4 $
 *  $Date: 2008/05/29 07:55:13 $
 *  $Id: Client.cc,v 1.4 2008/05/29 07:55:13 yoshi Exp $
*/
#include <sys/time.h>
#include <signal.h>
#include "echo.h"

/*
 *
 */
void *get_value_by_name(CORBA_any *val, char *label);

CORBA::Object_ptr
getObjectReference(CORBA::ORB_var &orb){
  CosNaming::NamingContext_var rootContext;

  try{
    CORBA::Object_var obj = orb->resolve_initial_references("NameService");
    rootContext = CosNaming::NamingContext::_narrow(obj);

    if(! rootContext  ) {
      std::cerr << "Fail to resolve NameService" << std::endl;
      return (CORBA::Object*)NULL;
    }
//  }catch(CORBA::ORB::InvalidName& ex) {
  }catch(...) {
    std::cerr << "Service required is invalid [does not exist]." << std::endl;
    return (CORBA::Object*)NULL;
  }

  CosNaming::Name name;
  name.length(1);

  //  name[0].id   = (const char *)"test";
  // name[0].kind = (const char *)"my_context";
  name[0].id   = (const char *)"Echo";
  name[0].kind = (const char *)"Object";

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
  return (CORBA::Object*)NULL;
}


void hello(RTC::Echo_ptr e){
	const char *src = "<html>\n <head>\n <title>TEST HTML</title>\n</head>\n <body>\n <h2>\n Hello, My Home Page\n </h2>\n </body>\n </html>";
  char *res = "";

  res = e->echoString(src);

  free(res);
  return;
}

static void echo_TimedLongSeq(RTC::Echo_ptr e)
{
  if( !e ) {
     std::cerr << "hello: The object reference is nil!\n" << std::endl;
     return;
   }

  RTC::TimedLongSeq *src = new RTC::TimedLongSeq();

  src->tm.sec = 1234;
  src->tm.nsec = 5678;

  src->data.length(6);
 
  src->data[0] = 6;
  src->data[1] = 5;
  src->data[2] = 4;
  src->data[3] = 3;
  src->data[4] = 2;
  src->data[5] = 1;

  RTC::TimedLongSeq *retval = e->echoTimedLongSeq(*src);
#if 1
  std::cerr << "echo_TimedLongDeq:! \n" << retval->data.length() << std::endl;
    for(unsigned long i=0; i<retval->data.length() ;i++){
      std::cerr << "retval[" << i << "]=" <<retval->data[i] << std::endl;
    }
#endif

}

static CORBA::ORB_var orb;

static void quit(int arg)
{
  if (orb) {
    printf("Destroying ORB...\n");
    orb->shutdown(true);
    orb->destroy();
    printf("done\n");
  }
  exit(0);
}

int
main(int argc, char **argv){
  struct timeval tv1, tv2;
  struct timezone tz;
  float dt;
  int i;

  int n_loop;

  try{
    orb = CORBA::ORB_init(argc, argv);

    if(argc > 0) std::cerr << argc << ":" << argv[1] << std::endl;

    signal(SIGINT, quit);
    signal(SIGTERM, quit);

    CORBA::Object_var obj = getObjectReference(orb);
    RTC::Echo_var myecho = RTC::Echo::_narrow(obj);
	   
    if(!myecho){
      orb->destroy();
      exit(1);
    }

    n_loop = 100000;

   if(argc > 1) n_loop = atoi(argv[1]);

    gettimeofday(&tv1, &tz);
    for(i=0;i<n_loop;i++){
      hello(myecho);
      echo_TimedLongSeq(myecho);
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
