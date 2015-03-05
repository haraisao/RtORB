/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
//
// Client.cc - This is the source code of SequenceTest-client.
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
//        id:SequenceTester kind:object
//

#include <sys/time.h>

#include <iostream>
using namespace std;

#ifdef ORB_IS_RTORB
#include "SequenceTest.h"
#else
#include "SequenceTest.hh"
#endif

ostream& operator<<(ostream& out, const SequenceTest::LongSeq& seq)
{
    out << "(";
    for (CORBA::ULong i = 0; i < seq.length(); i++) {
        if (i > 0) { out << ", "; }
        out << seq[i];
    }
    out << ")";
    return out;
}

ostream& operator<<(ostream& out, SequenceTest::LongSeq& seq)
{
    return (out << (const SequenceTest::LongSeq&)seq);
}

ostream& operator<<(ostream& out, SequenceTest::LongSeq_var& seq)
{
    return (out << (SequenceTest::LongSeq&)seq);
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
    void test_set_get();
    void test_set_ret();
    void test_update();
protected:
    SequenceTest::Tester_ptr getTester();
private:
    SequenceTest::Tester_var tester;
};

TesterClient::TesterClient(CORBA::ORB_var& orb): Client(orb)
{
    this->tester = getTester();
}

SequenceTest::Tester_ptr
TesterClient::getTester()
{
  CosNaming::Name name;
  name.length(1);
  name[0].id   = (const char*)"SequenceTester";
  name[0].kind = (const char*)"object";

  CORBA::Object_var obj = getObjectReference(name);
  return SequenceTest::Tester::_narrow(obj);
}

void 
TesterClient::test_set_get()
{
    cout << "test_set_get:" << endl;

    SequenceTest::LongSeq_var seq_out;
    tester->get(seq_out);
    cout << "  tester->get() ==> " << seq_out << endl;

    SequenceTest::LongSeq seq_in;
    seq_in.length(3);
    seq_in[0] = 3;
    seq_in[1] = 5;
    seq_in[2] = 7;
    tester->set(seq_in);
    cout << "  tester->set(" << seq_in << ")" << endl;

    tester->get(seq_out);
    cout << "  tester->get() ==> " << seq_out << endl << endl;
}

void 
TesterClient::test_set_ret()
{
    cout << "test_set_ret:" << endl;

    SequenceTest::LongSeq_var seq_ret;
    seq_ret = tester->ret();
    cout << "  tester->ret() ==> " << seq_ret << endl;

    SequenceTest::LongSeq seq_in;
    seq_in.length(3);
    seq_in[0] = 3;
    seq_in[1] = 5;
    seq_in[2] = 8;
    tester->set(seq_in);
    cout << "  tester->set(" << seq_in << ")" << endl;

    seq_ret = tester->ret();
    cout << "  tester->ret() ==> " << seq_ret << endl << endl;
}

void
TesterClient::test_update()
{
  cout << "test_update:" << endl;

  SequenceTest::LongSeq_var seq_ret = tester->ret();
  cout << "  tester->ret: " << seq_ret << endl;

  SequenceTest::LongSeq_var seq_inout = new SequenceTest::LongSeq;
  seq_inout->length(4);
  seq_inout[0] = 3;
  seq_inout[1] = 5;
  seq_inout[2] = 7;
  seq_inout[3] = 11;
  cout << "  seq = " << seq_inout << endl;

  tester->update(seq_inout.inout());
  cout << "  tester->update(seq)" << endl;
  cout << "  seq = " << seq_inout << endl << endl;
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
