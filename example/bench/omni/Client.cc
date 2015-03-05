/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
// eg3_clt.cc - This is the source code of example 3 used in Chapter 2
//              "The Basics" of the omniORB user guide.
//
//              This is the client. It uses the COSS naming service
//              to obtain the object reference.
//
// Usage: eg3_clt
//
//
//        On startup, the client lookup the object reference from the
//        COS naming service.
//
//        The name which the object is bound to is as follows:
//              root  [context]
//               |
//              text  [context] kind [my_context]
//               |
//              Echo  [object]  kind [Object]
//

#include <sys/time.h>
#include <echo.hh>

#ifdef HAVE_STD
#  include <iostream>
   using namespace std;
#else
#  include <iostream.h>
#endif

static CORBA::Object_ptr getObjectReference(CORBA::ORB_ptr orb);

static void hello(RTC::Echo_ptr e)
{
  if( CORBA::is_nil(e) ) {
    cerr << "hello: The object reference is nil!\n" << endl;
    return;
  }

  CORBA::String_var src = (const char*) "<html>\n <head>\n <title>TEST HTML</title>\n</head>\n <body>\n <h2>\n Hello, My Home Page\n </h2>\n </body>\n </html>";

  CORBA::String_var dest = e->echoString(src);

}

static void echo_TimedLongSeq(RTC::Echo_ptr e)
{
  if( CORBA::is_nil(e) ) {
    cerr << "hello: The object reference is nil!\n" << endl;
    return;
  }

  RTC::TimedLongSeq_var src = new RTC::TimedLongSeq();
  src->tm.sec = 1234;
  src->tm.nsec = 5678;

  src->data.length(6);
  src->data[0] = 6;
  src->data[1] = 5;
  src->data[2] = 4;
  src->data[3] = 3;
  src->data[4] = 2;
  src->data[5] = 1;

  RTC::TimedLongSeq_var retval = e->echoTimedLongSeq(src);

#if 0  
  cerr << "echo_TimedLongDeq:! \n" << retval->data.length() << endl;
  for(unsigned long i=0; i<retval->data.length() ;i++){
     cerr << "retval[" << i << "]=" <<retval->data[i] << endl;
  }
#endif

}
//////////////////////////////////////////////////////////////////////

int
main (int argc, char **argv) 
{
   struct timeval tv1,tv2;
   struct timezone tz;
   float dt;
   long n_loop = 100000;

  try {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

//    if(argc > 0) std::cerr << argc << ":" << argv[1] << std::endl;

    CORBA::Object_var obj = getObjectReference(orb);

    RTC::Echo_var echoref = RTC::Echo::_narrow(obj);

    if(argc > 1) n_loop = atoi(argv[1]);

    gettimeofday(&tv1, &tz);
    for (CORBA::ULong count=0; count < n_loop; count++){
      hello(echoref);
      echo_TimedLongSeq(echoref);
    }

    gettimeofday(&tv2, &tz);
      dt = (tv2.tv_sec - tv1.tv_sec) * 1000 + (tv2.tv_usec - tv1.tv_usec) / 1000.0;
        fprintf(stderr, "Time %f ms\n", dt);

    orb->destroy();
  }
  catch(CORBA::COMM_FAILURE& ex) {
    cerr << "Caught system exception COMM_FAILURE -- unable to contact the "
         << "object." << endl;
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

static CORBA::Object_ptr
getObjectReference(CORBA::ORB_ptr orb)
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
      return CORBA::Object::_nil();
    }
  }
  catch(CORBA::ORB::InvalidName& ex) {
    // This should not happen!
    cerr << "Service required is invalid [does not exist]." << endl;
    return CORBA::Object::_nil();
  }

  // Create a name object, containing the name test/context:
  CosNaming::Name name;
  name.length(2);

  name[0].id   = (const char*) "test";       // string copied
  name[0].kind = (const char*) "my_context"; // string copied
  name[1].id   = (const char*) "Echo";
  name[1].kind = (const char*) "Object";
  // Note on kind: The kind field is used to indicate the type
  // of the object. This is to avoid conventions such as that used
  // by files (name.type -- e.g. test.ps = postscript etc.)

  try {
    // Resolve the name to an object reference.
    return rootContext->resolve(name);
  }
  catch(CosNaming::NamingContext::NotFound& ex) {
    // This exception is thrown if any of the components of the
    // path [contexts or the object] aren't found:
    cerr << "Context not found." << endl;
  }
  catch(CORBA::COMM_FAILURE& ex) {
    cerr << "Caught system exception COMM_FAILURE -- unable to contact the "
         << "naming service." << endl;
  }
  catch(CORBA::SystemException&) {
    cerr << "Caught a CORBA::SystemException while using the naming service."
	 << endl;
  }

  return CORBA::Object::_nil();
}
