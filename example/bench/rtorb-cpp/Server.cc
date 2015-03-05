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
 *  $Revision: 1.4 $
 *  $Date: 2008/05/29 07:55:13 $
 *  $Id: Server.cc,v 1.4 2008/05/29 07:55:13 yoshi Exp $
*/

#include <signal.h>
#include "echo.h"

using namespace std;

class TimedLongSeq_IF {
  public:
     RTC::TimedLongSeq *impl;

  public:
     TimedLongSeq_IF() : impl(0){}
     TimedLongSeq_IF(RTC::TimedLongSeq *val) : impl(val){}
     TimedLongSeq_IF(long n)  {
             impl = new RTC::TimedLongSeq();
	     impl->data.length(n);
     }

     ~TimedLongSeq_IF(){
    /*
        if(impl){
         if(impl->data._maximum > 0)
		 RtORB_free(impl->data._buffer,"TimedLongSeq");
	}
*/
     }

     long length(){
       if(!impl) impl = new RTC::TimedLongSeq();
       return impl->data.length();
     }

     long length(long n){
       if(!impl) impl = new RTC::TimedLongSeq();
       impl->data.length(n);
     }

     long data(long idx){
       if(!impl) return 0;
       return (impl->data)[idx];
     }

     void data(long idx, long val){
       if(!impl) return ;
       (impl->data)[idx] = val;
        return;
     }
};

boolean bindObjectToName(CORBA::ORB_var & orb, CORBA::Object_ptr obj);

class Echo_i : public POA_RTC::Echo
{
  public:
    long status;

  public:
  //   Echo_i(PortableServer::POA_ptr);

   CORBA_string echoString(const CORBA_char * mesg);
  RTC::TimedLongSeq * echoTimedLongSeq(const RTC::TimedLongSeq & x);
  RTC::TimedStringSeq * echoTimedStringSeq(const RTC::TimedStringSeq & x);
};

/*
Echo_i::Echo_i(PortableServer::POA_ptr poa) : Echo__impl()
{
}
*/

char *Echo_i::echoString(const char *mesg){
//  std::cerr << "Call echoString" << std::endl;
//  std::cerr << this->_impl << ": "<< this->status++  << std::endl;
  return strdup(mesg);
}

RTC::TimedLongSeq * Echo_i::echoTimedLongSeq(const RTC::TimedLongSeq & x){
   TimedLongSeq_IF retval;
   retval.length( x.data.length() );

   for(int i=0; i< x.data.length(); i++){
//      std::cerr << "X[" << i << "]=" << x->data._buffer[i] << std::endl;
      retval.data(i, x.data[i]* 2);
   }
//   std::cerr <<  std::endl;

   return retval.impl;
}
RTC_TimedStringSeq * Echo_i::echoTimedStringSeq(const RTC::TimedStringSeq & x){
  return NULL;
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
		 
int main(int argc, char **argv){

 try{
   orb = CORBA::ORB_init(argc, argv);

   signal(SIGINT, quit);
   signal(SIGTERM, quit);

   CORBA::Object_var poa_obj = orb->resolve_initial_references("RootPOA");

   PortableServer::POA_ptr poa = PortableServer::POA::_narrow(poa_obj);

   //Echo_i *myEcho = new Echo_i(poa);
   Echo_i *myEcho = new Echo_i;

//   PortableServer::ObjectId_var myechoid = poa->activate_object(myEcho);
//   obj = myecho->_this();
   poa->activate_object(myEcho);
   CORBA::Object_ptr ref = myEcho->_this();

  if( !bindObjectToName(orb, ref) ) return 1;
  
  PortableServer::POAManager_var poa_manager = poa->the_POAManager();
  poa_manager->activate();

  orb->run();

  orb->shutdown(true);
  orb->destroy();

 }catch(...){
  cerr << "Caught unknown exception." << endl;
 }
 return 0;

}

boolean
bindObjectToName(CORBA::ORB_var &orb, CORBA::Object_ptr objref){
  CosNaming::NamingContext_var rootContext;
  CosNaming::NamingContext_var testContext;
  CosNaming::Name contextName;
  CosNaming::Name objectName;

  CORBA::Object_var obj = orb->resolve_initial_references("NameService");
  rootContext = CosNaming::NamingContext::_narrow(obj);

  /*
  contextName.length(1);
  contextName[0].id = "Echo";
  contextName[0].kind = "Object";

  try{
    testContext = rootContext->bind_new_context(contextName);

  } catch(CosNaming::NamingContext::AlreadyBound& ex){
    std::cerr << "!!!! Already Bound "<< contextName[0].id   << ", " 
	    << contextName[0].kind  << " !!!" << std::endl;

    CORBA::Object_var obj ;

    obj = rootContext->resolve(contextName);
    testContext = CosNaming::NamingContext::_narrow(obj);

    if(!obj){
      std::cerr << "Failed to narrow object." << std::endl;
      return 0;
    }

  } catch (CORBA::SystemException& ex){
    std::cerr << "System Excetion occured." << std::endl;
    return 0;
  }
  */

  objectName.length(1);
  objectName[0].id = "Echo";
  objectName[0].kind = "Object";

  try{
    rootContext->bind(objectName, objref);
  } catch(CosNaming::NamingContext::AlreadyBound& ex){
    std::cerr << "Sorry, Already Bound." << std::endl;
    rootContext->rebind(objectName, objref);

  } catch(CORBA::SystemException& ex){
    std::cerr << "System Excetion occured." << std::endl;
    return 0;
  } catch(...) {
    std::cerr << "Unknown exception occured." << std::endl;
    return 0;
  }

  return 1;
}

