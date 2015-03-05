/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
//
// Client.cc - This is the source code of AnyTest-client.
//
// Usage: Client -ORBInitRef NameService=<corbaloc>
//
//  e.g.: Client -ORBInitRef NameService=corbaloc::localhost/NameService
//
//  On startup, Client looks up the object reference from the COS naming service.
//
//  The name which the object is bound to is as follows:
//        id:root   kind:context
//         |
//        id:AnyTester kind:object
//

#include <sys/time.h>

#include <iostream>
using namespace std;

//#include <omniURI.h>

#ifdef ORB_IS_RTORB
#include "AnyTest.h"
#else
#include "AnyTest.hh"
#endif

ostream& operator<<(ostream& out, const CORBA::Any& a)
{

#if 1
    char *s;
    if (a >>= CORBA::Any::to_string(s)) { out << "string : "  << s;}
    CORBA::ULong n;
    if (a >>= n) {  out << "long : " << n; }
#else
    const CORBA::ULong Bound = 1000;
    if (a >>= CORBA::Any::to_string(s, Bound)) { out << s; }
#endif

    return out;
}

ostream& operator<<(ostream& out, CORBA::Any& data)
{
    return (out << (const CORBA::Any&)data);
}

ostream& operator<<(ostream& out, CORBA::Any_var& data)
{
     return (out << (const CORBA::Any&)*((CORBA::Any*&)data));
}

//////////////////////////////////////////////////////////////////////

class Client
{
public:
    Client(CORBA::ORB_var& orb);
    CORBA::Object_ptr getObjectReference(const CosNaming::Name& name);
protected:
    CosNaming::NamingContext_ptr getRootContext();
private:
    CORBA::ORB_var& orb;
    CosNaming::NamingContext_var root_context;
};

Client::Client(CORBA::ORB_var& _orb): orb(_orb)
{
    this->root_context = getRootContext();
}

CosNaming::NamingContext_ptr
Client::getRootContext()
{
  CORBA::Object_var ptr = orb->resolve_initial_references("NameService");
  return CosNaming::NamingContext::_narrow(ptr);
}

CORBA::Object_ptr
Client::getObjectReference(const CosNaming::Name& name)
{
    return root_context->resolve(name);
}

//////////////////////////////////////////////////////////////////////

class TesterClient: public Client
{
public:
    TesterClient(CORBA::ORB_var& orb);
    void run();
    void test_set_get_number();
    void test_set_get_string();
    void test_update();
protected:
    AnyTest::Tester_ptr getTester();
private:
    AnyTest::Tester_var tester;
};

TesterClient::TesterClient(CORBA::ORB_var& orb): Client(orb)
{
    this->tester = getTester();
}

AnyTest::Tester_ptr
TesterClient::getTester()
{
#if 1
    CosNaming::Name name;
    name.length(1);
    name[0].id   = (const char*)"AnyTester";
    name[0].kind = (const char*)"object";
#else
    const char* desc = "AnyTester.object";
    CosNaming::Name_var name = omni::omniURI::stringToName(desc);
#endif

#if 0
    CORBA::String_var desc = omni::omniURI::nameToString(name);
    cerr << "Object-name=" << desc << endl;
#endif

    CORBA::Object_var obj = getObjectReference(name);
    return AnyTest::Tester::_narrow(obj);
}

void 
TesterClient::test_set_get_number()
{
    cout << "test_set_get_number:" << endl;

    CORBA::Any_var data_out = new CORBA::Any;
    tester->get(data_out.out());
    cout << "  tester->get() ==> " << data_out << endl;

    CORBA::Any_var data_in = new CORBA::Any;
    data_in <<= (CORBA::ULong)7;
    tester->set(data_in.in());
    cout << "  tester->set(" << data_in << ")" << endl;

    tester->get(data_out.out());
    cout << "  tester->get() ==> " << data_out << endl << endl;
}

void 
TesterClient::test_set_get_string()
{
    cout << "test_set_get_string:" << endl;

    CORBA::Any_var data_out = new CORBA::Any;
    tester->get(data_out.out());
    cout << "  tester->get() ==> " << data_out << endl;

    CORBA::Any_var data_in = new CORBA::Any;
    data_in <<= (const char*)"seven";
    tester->set(data_in.in());
    cout << "  tester->set(" << data_in << ")" << endl;

    tester->get(data_out.out());
    cout << "  tester->get() ==> " << data_out << endl << endl;
}

void
TesterClient::test_update()
{
  cout << "test_update:" << endl;

  CORBA::Any_var a_ret;
  a_ret = tester->ret();
  cout << "  tester->ret() ==> " << a_ret << endl;

  CORBA::Any_var a_inout = new CORBA::Any;
  a_inout <<= (const char*)"eleven";
  cout << "  any = " << a_inout << endl;

  tester->update(a_inout.inout());
  cout << "  tester->update(any)" << endl;
  cout << "  any = " << a_inout << endl << endl;
}

void 
TesterClient::run()
{
    test_set_get_number();
    test_set_get_string();
    test_update();
}

//////////////////////////////////////////////////////////////////////

int
main (int argc, char **argv) 
{
  try {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    TesterClient tester(orb);
    tester.run();
    orb->destroy();
  }
  catch(CosNaming::NamingContext::NotFound& ex) {
    // This exception is thrown if any of the components of the
    // path [contexts or the object] aren't found:
    cerr << "Context not found." << endl;
  }
  catch(CORBA::COMM_FAILURE& ex) {
    cerr << "Caught system exception COMM_FAILURE -- unable to contact the "
         << "object." << endl;
  }
  catch(CORBA::SystemException&) {
    cerr << "Caught CORBA::SystemException." << endl;
  }
#ifdef ORB_IS_RTORB
  catch(CORBA::UserException& ex) {
    cerr << "Caught CORBA::Exception." << endl;
    cerr << ex.msg() << endl;
  }
#endif
  catch(CORBA::Exception&) {
    cerr << "Caught CORBA::Exception." << endl;
  }
#ifdef ORB_IS_OMNIORB
  catch(omniORB::fatalException& fe) {
    cerr << "Caught omniORB::fatalException:" << endl;
    cerr << "  file: " << fe.file() << endl;
    cerr << "  line: " << fe.line() << endl;
    cerr << "  mesg: " << fe.errmsg() << endl;
  }
#endif
  catch(...) {
    cerr << "Caught unknown exception." << endl;
  }
  return 0;
}
