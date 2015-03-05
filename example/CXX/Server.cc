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
 *  $Revision: 1.5 $
 *  $Date: 2008/03/21 23:50:42 $
 *  $Id: Server.cc,v 1.5 2008/03/21 23:50:42 yoshi Exp $
*/

#include <echo.h>

using namespace std;


boolean bindObjectToName(CORBA::ORB_ptr orb, CORBA::Object_ptr obj);

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

Echo_i::Echo_i(PortableServer::POA_ptr poa) : Echo_impl(poa) { }

char *Echo_i::echoString(const char *mesg){
  std::cerr << "Call echoString" << std::endl;
  std::cout << mesg << std::endl;

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

int main(int argc, char **argv){

 try{
   CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
   CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");

   PortableServer::POA_ptr poa = PortableServer::POA::_narrow(obj);

   Echo_i *myEcho = new Echo_i(poa);

  if( !bindObjectToName(orb, *myEcho) ) return 1;
  
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
bindObjectToName(CORBA::ORB_ptr orb, CORBA::Object_ptr objref){
  CosNaming::NamingContext_var rootContext;
  CosNaming::NamingContext_var testContext;
  CosNaming::Name contextName;
  CosNaming::Name objectName;

  CORBA::Object_var obj = orb->resolve_initial_references("NameService");
  rootContext = CosNaming::NamingContext::_narrow(obj);

  contextName.length(1);
  contextName[0].id = "test";
  contextName[0].kind = "my_context";

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

  objectName.length(1);
  objectName[0].id = "Echo";
  objectName[0].kind = "Object";

  try{
    testContext->bind(objectName, objref);
  } catch(CosNaming::NamingContext::AlreadyBound& ex){
    std::cerr << "Sorry, Already Bound." << std::endl;
    testContext->rebind(objectName, objref);

  } catch(CORBA::SystemException& ex){
    std::cerr << "System Excetion occured." << std::endl;
    return 0;
  } catch(...) {
    std::cerr << "Unknown exception occured." << std::endl;
    return 0;
  }

  return 1;
}

