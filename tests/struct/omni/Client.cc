/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
//
// Client.cc - This is the source code of StructTest-client.
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
//        id:StructTester kind:object
//

#include <sys/time.h>

#include <iostream>
using namespace std;

#ifdef ORB_IS_RTORB
#include "StructTest.h"
#else
#include "StructTest.hh"
#endif

ostream& operator<<(ostream& out, const StructTest::Point& p)
{
    out << "(" << p.x << ", " << p.y << ")";
    return out;
}

ostream& operator<<(ostream& out, StructTest::Point& p)
{
    return (out << (const StructTest::Point&)p);
}

//////////////////////////////////////////////////////////////////////

class Client
{
public:
    Client(CORBA::ORB_var& orb);
    CORBA::Object_ptr getObject(const CosNaming::Name& name);
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
  CORBA::Object_var obj = orb->resolve_initial_references("NameService");
  return CosNaming::NamingContext::_narrow(obj);
}

CORBA::Object_ptr
Client::getObject(const CosNaming::Name& name)
{
    return root_context->resolve(name);
}

//////////////////////////////////////////////////////////////////////

class TesterClient: public Client
{
public:
    TesterClient(CORBA::ORB_var& orb);
    void run();
    void test_set_get();
    void test_set_ret();
    void test_update();
protected:
    StructTest::Tester_ptr getTester();
private:
    StructTest::Tester_var tester;
};

TesterClient::TesterClient(CORBA::ORB_var& orb): Client(orb)
{
    this->tester = getTester();
}

StructTest::Tester_ptr
TesterClient::getTester()
{
  CosNaming::Name name;
  name.length(1);
  name[0].id   = (const char*)"StructTester";
  name[0].kind = (const char*)"object";

  CORBA::Object_var obj = getObject(name);
  return StructTest::Tester::_narrow(obj);
}

void 
TesterClient::test_set_get()
{
    cout << "test_set_get:" << endl;

    StructTest::Point p_out;
    tester->get(p_out);
    cout << "  tester->get() ==> " << p_out << endl;

    StructTest::Point p_in;
    p_in.x = 10;
    p_in.y = 20;
    tester->set(p_in);
    cout << "  tester->set(" << p_in << ")" << endl;

    tester->get(p_out);
    cout << "  tester->get() ==> " << p_out << endl << endl;
}

void 
TesterClient::test_set_ret()
{
    cout << "test_set_ret:" << endl;

    StructTest::Point p_out;
    p_out = tester->ret();
    cout << "  tester->ret() ==> " << p_out << endl;

    StructTest::Point p_in;
    p_in.x = 20;
    p_in.y = 30;
    tester->set(p_in);
    cout << "  tester->set(" << p_in << ")" << endl;

    p_out = tester->ret();
    cout << "  tester->ret() ==> " << p_out << endl << endl;
}

void
TesterClient::test_update()
{
  cout << "test_update:" << endl;

  StructTest::Point p_ret = tester->ret();
  cout << "  tester->ret() ==> " << p_ret << endl;

  StructTest::Point_var p_inout = new StructTest::Point;
  p_inout->x = p_inout->y = 0;
  cout << "  point = " << p_inout << endl;

  tester->update(p_inout.inout());
  cout << "  tester->update(point)" << endl;
  cout << "  point = " << p_inout << endl << endl;
}

void 
TesterClient::run()
{
    test_set_get();
    test_set_ret();
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
