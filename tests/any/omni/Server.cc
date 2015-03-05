/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
//
// Server.cc - This is the source code of the AnyTest-server.
//
// Usage: Server -ORBInitRef NameService=<corbaloc>
//
//  e.g.: Server -ORBInitRef NameService=corbaloc::localhost/NameService
//
//  On startup, Server registers a servant into the COS naming service.
//
//  The name which the servant is bound to is as follows:
//        id:root   kind:context
//         |
//        id:AnyTester kind:object
//

#include <iostream>
using namespace std;

#ifdef ORB_IS_RTORB
#include "AnyTest.h"
#else
#include "AnyTest.hh"
#endif

const string ServerName = "AnyTest-server";

ostream& operator<<(ostream& out, const CORBA::Any& a)
{
    CORBA::ULong n;
    if (a >>= n) { out << n; }

    char* s;
#if 1
    if (a >>= CORBA::Any::to_string(s)) { out << s; }
#else
    const CORBA::ULong Bound = 1000;
    if (a >>= CORBA::Any::to_string(s, Bound)) { out << s; }
#endif

    return out;
}

ostream& operator<<(ostream& out, CORBA::Any& a)
{
    return (out << (const CORBA::Any&)a);
}

//
// class Tester_impl: implementation class for AnyTest::Tester
//
class Tester_impl : public POA_AnyTest::Tester,
	            public PortableServer::RefCountServantBase
{
public:
    inline Tester_impl() {
      const char *str = "string";
      _data <<= str;
    }
    CORBA::Any* ret();
    void get(CORBA::Any_out data);
    void set(const CORBA::Any& data);
    void update(CORBA::Any& data);
private:
    CORBA::Any _data;
};

CORBA::Any*
Tester_impl::ret()
{
#if 1
    CORBA::Any* p = new CORBA::Any(this->_data);
    clog << "Tester_impl::ret() ==> " << *p << endl;
    return p;
#else
    CORBA::Any_var data = this->_data;
    clog << "Tester_impl::ret() ==> " << (CORBA::Any&)data << endl;
    return data._retn();
#endif
}

void
Tester_impl::get(CORBA::Any_out data)
{
    clog << "Tester_impl::get()";
    data = new CORBA::Any(this->_data);
    clog << " ==> " << *(data.ptr()) << endl;
}

void
Tester_impl::set(const CORBA::Any& data)
{
    clog << "Tester_impl::set(" << data << ")" << endl;
    this->_data = data;
}

void
Tester_impl::update(CORBA::Any& data)
{
    clog << "Tester_impl::update: " << data;
    data = this->_data;
    clog << " ==> " << data << endl;
}

//////////////////////////////////////////////////////////////////////

class Server
{
public:
    Server(CORBA::ORB_var& orb);
    virtual ~Server();
    void registerServant(const CosNaming::Name& name, Tester_impl* servant);
    void run();
private:
    CORBA::ORB_var& orb;
    CosNaming::NamingContext_var root_context;
    PortableServer::POA_ptr root_poa;
protected:
    CosNaming::NamingContext_ptr getRootContext();
    PortableServer::POA_ptr getRootPOA();
};

Server::Server(CORBA::ORB_var& _orb): orb(_orb)
{
    root_context = getRootContext();
    root_poa = getRootPOA();
}

Server::~Server()
{
    //poa->destroy(TRUE, TRUE);
}

CosNaming::NamingContext_ptr
Server::getRootContext()
{
    CORBA::Object_var obj = orb->resolve_initial_references("NameService");
    return CosNaming::NamingContext::_narrow(obj);
}

PortableServer::POA_ptr
Server::getRootPOA()
{
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    return PortableServer::POA::_narrow(obj);
}

void
Server::registerServant(const CosNaming::Name& name, Tester_impl* servant)
{
    //PortableServer::ObjectId_var sid = root_poa->activate_object(servant);
    root_poa->activate_object(servant);

    CORBA::Object_ptr ref = servant->_this();

    try {
        root_context->bind(name, ref);
    }
    catch(CosNaming::NamingContext::AlreadyBound& ex) {
        root_context->rebind(name, ref);
    }

    servant->_remove_ref();
}

void
Server::run()
{
    PortableServer::POAManager_var mgr = root_poa->the_POAManager();
    mgr->activate();
    orb->run();
}

//////////////////////////////////////////////////////////////////////

class TesterServer: public Server
{
public:
    TesterServer(CORBA::ORB_var& orb): Server(orb) { registerTester(); }
private:
    void registerTester();
};

void 
TesterServer::registerTester()
{
    CosNaming::Name name;
    name.length(1);
    name[0].id   = (const char*)"AnyTester";
    name[0].kind = (const char*)"object";

    Tester_impl* servant = new Tester_impl();
    registerServant(name, servant);
    cout << "Servant registered as: id=" << name[0].id << ", kind=" << name[0].kind << endl;
}

//////////////////////////////////////////////////////////////////////

int
main(int argc, char **argv)
{
  try {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    TesterServer server(orb);
    cout << ServerName << " running.." << endl;
    server.run();
    orb->destroy();
  }
  catch(CORBA::ORB::InvalidName& ex) {
    // This should not happen!
    cerr << "Service required is invalid [does not exist]." << endl;
  }
  catch(CORBA::COMM_FAILURE& ex) {
    cerr << "Caught system exception COMM_FAILURE -- unable to contact the "
         << "naming service." << endl;
  }
  catch(CORBA::SystemException&) {
    cerr << "Caught CORBA::SystemException." << endl;
  }
#ifdef ORB_IS_RTORB
  catch(CORBA::UserException& ex) {
    cerr << "Caught CORBA::UserException." << endl;
    cerr << ex.msg() << endl;
  }
#else
  catch(CORBA::Exception& ex) {
    cerr << "Caught CORBA::Exception." << endl;
  }
#endif
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
