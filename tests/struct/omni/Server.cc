/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
//
// Server.cc - This is the source code of the StructTest-server.
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
//        id:StructTester kind:object
//

#include <iostream>
using namespace std;

#ifdef ORB_IS_RTORB
#include "StructTest.h"
#else
#include "StructTest.hh"
#endif

const string ServerName = "StructTest-server";

ostream& operator<<(ostream& out, const StructTest::Point& p)
{
    out << "(" << p.x << ", " << p.y << ")";
    return out;
}

ostream& operator<<(ostream& out, StructTest::Point& p)
{
    return (out << (const StructTest::Point&)p);
}

//
// class Tester_impl: implementation class for StructTest::Tester
//
class Tester_impl : public POA_StructTest::Tester,
	            public PortableServer::RefCountServantBase
{
public:
    inline Tester_impl() { _p.x = _p.y = 0; }
    StructTest::Point ret();
    void get(StructTest::Point& p);
    void set(const StructTest::Point& p);
    void update(StructTest::Point& p);
private:
    StructTest::Point _p;
};

StructTest::Point
Tester_impl::ret()
{
    clog << "Tester_impl::ret() ==> " << this->_p << endl;
    return this->_p;
}

void
Tester_impl::get(StructTest::Point& p)
{
    p = this->_p;
    clog << "Tester_impl::get() ==> " << _p << endl;
}

void 
Tester_impl::set(const StructTest::Point& p)
{
    clog << "Tester_impl::set(" << p << ")" << endl;
    this->_p = p;
}

void
Tester_impl::update(StructTest::Point& p)
{
    clog << "Tester_impl::update(): " << p;
    p = this->_p;
    clog << " ==> " << p << endl;
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
#ifdef ORB_IS_RTORB
    PortableServer::POA_ptr root_poa;
#else
    PortableServer::POA_var root_poa;
#endif
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
#ifdef ORB_IS_RTORB
    CORBA::Object_ptr obj = orb->resolve_initial_references("NameService");
#else
    CORBA::Object_ptr obj = orb->resolve_initial_references("NameService");
#endif
    return CosNaming::NamingContext::_narrow(obj);
}

PortableServer::POA_ptr
Server::getRootPOA()
{
#ifdef ORB_IS_RTORB
    CORBA::Object_ptr obj = orb->resolve_initial_references("RootPOA");
#else
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
#endif
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
    name[0].id   = (const char*)"StructTester";
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
  } catch(CORBA::Exception& ex) {

    cerr << "Caught CORBA::Exception." << endl;
#ifdef ORB_IS_RTORB
    cerr << ex.msg() << endl;
#endif
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
