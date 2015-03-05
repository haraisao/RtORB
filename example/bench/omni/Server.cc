/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */

#include <echo.hh>

#ifdef HAVE_STD
#  include <iostream>
   using namespace std;
#else
#  include <iostream.h>
#endif

static CORBA::Boolean bindObjectToName(CORBA::ORB_ptr, CORBA::Object_ptr);


class Echo_i : public POA_RTC::Echo,
	       public PortableServer::RefCountServantBase
{
public:
  inline Echo_i() {}
  virtual ~Echo_i() {}
  virtual char* echoString(const char* mesg);
  virtual RTC::TimedLongSeq *echoTimedLongSeq(const RTC::TimedLongSeq& x);
  virtual RTC::TimedStringSeq *echoTimedStringSeq(const RTC::TimedStringSeq& x);
};


char* Echo_i::echoString(const char* mesg)
{
//  fprintf(stderr, "echoString, %s\n", mesg);
  return CORBA::string_dup(mesg);
}

RTC::TimedLongSeq *
Echo_i::echoTimedLongSeq(const RTC::TimedLongSeq& x){
  RTC::TimedLongSeq *retval ;

//  fprintf(stderr, "Length = %d\n", x.data.length());
  retval = new RTC::TimedLongSeq();
  retval->data.length(x.data.length());

  for(unsigned long  i=0;i < x.data.length();i++){
//     fprintf(stderr, "Data[%d] = %d\n",i,  x.data[i]);
     retval->data[i] = x.data[i] * 2;
  }
  return retval;
}
RTC::TimedStringSeq *
Echo_i::echoTimedStringSeq(const RTC::TimedStringSeq& x){
	RTC::TimedStringSeq *retval ;
  retval = (RTC::TimedStringSeq *)&x;
  return retval;

}

//////////////////////////////////////////////////////////////////////

int
main(int argc, char **argv)
{
  try {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);

    Echo_i* myecho = new Echo_i();

    PortableServer::ObjectId_var myechoid = poa->activate_object(myecho);

    // Obtain a reference to the object, and register it in
    // the naming service.
    obj = myecho->_this();

    CORBA::String_var x;
    x = orb->object_to_string(obj);
    cerr << x << "\n";

    if( !bindObjectToName(orb, obj) )
      return 1;

#if 1
    myecho->_remove_ref();

    PortableServer::POAManager_var pman = poa->the_POAManager();
    pman->activate();

    orb->run();
#endif
  }
  catch(CORBA::SystemException&) {
    cerr << "Caught CORBA::SystemException." << endl;
  }
  catch(CORBA::Exception&) {
    cerr << "Caught CORBA::Exception." << endl;
  }
  catch(omniORB::fatalException& fe) {
    cerr << "Caught omniORB::fatalException:" << endl;
    cerr << "  file: " << fe.file() << endl;
    cerr << "  line: " << fe.line() << endl;
    cerr << "  mesg: " << fe.errmsg() << endl;
  }
  catch(...) {
    cerr << "Caught unknown exception." << endl;
  }

  return 0;
}

//////////////////////////////////////////////////////////////////////

static CORBA::Boolean
bindObjectToName(CORBA::ORB_ptr orb, CORBA::Object_ptr objref)
{
  CosNaming::NamingContext_var rootContext;

  try {
    // Obtain a reference to the root context of the Name service:
    CORBA::Object_var obj;
    obj = orb->resolve_initial_references("NameService");

    // Narrow the reference returned.
    rootContext = CosNaming::NamingContext::_narrow(obj);

    if( CORBA::is_nil(rootContext) ) {
      cerr << "Failed to narrow the root naming context." << endl;
      return 0;
    }
  }
  catch(CORBA::ORB::InvalidName& ex) {
    // This should not happen!
    cerr << "Service required is invalid [does not exist]." << endl;
    return 0;
  }

  try {
    // Bind a context called "test" to the root context:

    CosNaming::Name contextName;
    contextName.length(1);
    contextName[0].id   = (const char*) "test";       // string copied
    contextName[0].kind = (const char*) "my_context"; // string copied
    // Note on kind: The kind field is used to indicate the type
    // of the object. This is to avoid conventions such as that used
    // by files (name.type -- e.g. test.ps = postscript etc.)

    CosNaming::NamingContext_var testContext;
    try {
      // Bind the context to root.
      cerr << "Bind the context to root." << endl;
      testContext = rootContext->bind_new_context(contextName);
      cerr << "Finished bind the context to root." << endl;
    }
    catch(CosNaming::NamingContext::AlreadyBound& ex) {
      // If the context already exists, this exception will be raised.
      // In this case, just resolve the name and assign testContext
      // to the object returned:
      CORBA::Object_var obj;
      obj = rootContext->resolve(contextName);
      testContext = CosNaming::NamingContext::_narrow(obj);
      if( CORBA::is_nil(testContext) ) {
        cerr << "Failed to narrow naming context." << endl;
        return 0;
      }
    }

    // Bind objref with name Echo to the testContext:
    CosNaming::Name objectName;
    objectName.length(1);
    objectName[0].id   = (const char*) "Echo";   // string copied
    objectName[0].kind = (const char*) "Object"; // string copied

    try {
      cerr << "Bind the object to testContext." << endl;
      testContext->bind(objectName, objref);
      cerr << "Finish Bind." << endl;
    }
    catch(CosNaming::NamingContext::AlreadyBound& ex) {
      cerr << "Rebind Object." << endl;
      testContext->rebind(objectName, objref);
    }
    // Note: Using rebind() will overwrite any Object previously bound
    //       to /test/Echo with obj.
    //       Alternatively, bind() can be used, which will raise a
    //       CosNaming::NamingContext::AlreadyBound exception if the name
    //       supplied is already bound to an object.

    // Amendment: When using OrbixNames, it is necessary to first try bind
    // and then rebind, as rebind on it's own will throw a NotFoundexception if
    // the Name has not already been bound. [This is incorrect behaviour -
    // it should just bind].
  }
  catch(CORBA::COMM_FAILURE& ex) {
    cerr << "Caught system exception COMM_FAILURE -- unable to contact the "
         << "naming service." << endl;
    return 0;
  }
  catch(CORBA::SystemException&) {
    cerr << "Caught a CORBA::SystemException while using the naming service."
	 << endl;
    return 0;
  }

  return 1;
}
