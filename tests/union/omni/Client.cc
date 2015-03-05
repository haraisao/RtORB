/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
//
// Client.cc - This is the source code of UnionTest-client.
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
//        id:UnionTester kind:object
//

#include <sys/time.h>

#include <iostream>
using namespace std;

#ifdef ORB_IS_RTORB
#include "UnionTest.h"
#else
#include "UnionTest.hh"
#endif

ostream& operator<<(ostream& out, const UnionTest::Data& data)
{
    switch (data._d())
    {
    case 1:
        out << data.num();
        break;
    case 2:
        out << data.str();
        break;
    default:
        break;
    }
    return out;
}

ostream& operator<<(ostream& out, UnionTest::Data& data)
{
    return (out << (const UnionTest::Data&)data);
}

ostream& operator<<(ostream& out, UnionTest::Data_var& data)
{
    //return (out << (UnionTest::Data&)data);
    return (out << (const UnionTest::Data&)data);
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
  CORBA::Object_var obj = orb->resolve_initial_references("NameService");
  return CosNaming::NamingContext::_narrow(obj);
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
    UnionTest::Tester_ptr getTester();
private:
    UnionTest::Tester_var tester;
};

TesterClient::TesterClient(CORBA::ORB_var& orb): Client(orb)
{
    this->tester = getTester();
}

UnionTest::Tester_ptr
TesterClient::getTester()
{
  CosNaming::Name name;
  name.length(1);
  name[0].id   = (const char*)"UnionTester";
  name[0].kind = (const char*)"object";

  CORBA::Object_var obj = getObjectReference(name);
  return UnionTest::Tester::_narrow(obj);
}

void 
TesterClient::test_set_get_number()
{
    cout << "test_set_get_number:" << endl;

    UnionTest::Data_var data_out = new UnionTest::Data;
    tester->get(data_out.out());
    cout << "  tester->get() ==> " << data_out << endl;

    UnionTest::Data_var data_in = new UnionTest::Data;
    data_in->num(7);
    tester->set(data_in.in());
    cout << "  tester->set(" << data_in << ")" << endl;

    tester->get(data_out.out());
    cout << "  tester->get() ==> " << data_out << endl << endl;
}

void 
TesterClient::test_set_get_string()
{
    cout << "test_set_get_string:" << endl;

    UnionTest::Data_var data_out = new UnionTest::Data;
    tester->get(data_out.out());
    cout << "  tester->get() ==> " << data_out << endl;

// both cases work
#if 1
    UnionTest::Data data_in;
    data_in.str((const char*)"seven");
    tester->set(data_in);
    cout << "  tester->set(" << data_in << ")" << endl;
#else
    UnionTest::Data_var data_in = new UnionTest::Data;
    data_in->str((const char*)"seven");
    tester->set(data_in.in());
    cout << "  tester->set(" << data_in << ")" << endl;
#endif

    tester->get(data_out.out());
    cout << "  tester->get() ==> " << data_out << endl << endl;
}

void
TesterClient::test_update()
{
    cout << "test_update:" << endl;

    UnionTest::Data_var data_ret = tester->ret();
    cout << "  tester->ret() ==> " << data_ret << endl;

    UnionTest::Data_var data_inout = new UnionTest::Data;
    data_inout->str((const char*)"eleven");
    cout << "  data = " << data_inout << endl;

    tester->update(data_inout.inout());
    cout << "  tester->update(data)" << endl;
    cout << "  data = " << data_inout << endl << endl;
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
  catch(CORBA::UserException &e) {
    cerr << "Caught CORBA::Exception." << endl;
    cerr << e.msg() << endl;
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
