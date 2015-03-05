/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
//
// Server.cc - This is the source code of the UnionTest-server.
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
//        id:UnionTester kind:object
//

#include <iostream>
using namespace std;

#ifdef ORB_IS_RTORB
#include "UnionTest.h"
#else
#include "UnionTest.hh"
#endif

const string ServerName = "UnionTest-server";

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

//
// class Tester_impl: implementation class for StructTest::Tester
//
class Tester_impl : public POA_UnionTest::Tester,
	            public PortableServer::RefCountServantBase
{
public:
    inline Tester_impl() { _data = new UnionTest::Data; }
    UnionTest::Data* ret();
    void get(UnionTest::Data_out data);
    void set(const UnionTest::Data& data);
    void update(UnionTest::Data& data);
private:
    UnionTest::Data_var _data;
};

UnionTest::Data*
Tester_impl::ret()
{
    UnionTest::Data_var data = this->_data;
    clog << "Tester_impl::ret() ==> " << (UnionTest::Data&)data << endl;
    return data._retn();
}

void
Tester_impl::get(UnionTest::Data_out data)
{
    clog << "Tester_impl::get()";
    data = new UnionTest::Data(this->_data);
    clog << " ==> " << *(data.ptr()) << endl;
}

void
Tester_impl::set(const UnionTest::Data& data)
{
    clog << "Tester_impl::set(" << data << ")" << endl;
    //this->_data = data;  ... this doesn't work
    this->_data = new UnionTest::Data(data);
}

void
Tester_impl::update(UnionTest::Data& data)
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
    name[0].id   = (const char*)"UnionTester";
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
    cerr << "Caught CORBA::Exception." << endl;
    cerr << ex.msg() << endl;
  }
#endif
  catch(CORBA::Exception& ex) {
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
