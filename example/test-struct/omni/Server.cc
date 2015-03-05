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

class Echo_i : public POA_Echo,
	       public PortableServer::RefCountServantBase
{
public:
  inline Echo_i() {}
  virtual ~Echo_i() {}
  virtual void bind (const Name& n, CORBA::Object* obj);
  virtual char * echoString (const char * mesg);
  virtual char* echoString2(const char* mesg, CORBA::String_out res);
  virtual char* echoString3(char*& mesg);
  virtual CORBA::Long plus(CORBA::Long x, CORBA::Long y);
  virtual void plus2(CORBA::Long x, CORBA::Long y, CORBA::Long& ans);
  virtual CORBA::Float divide(CORBA::Float x, CORBA::Float y);
  virtual void divide2(CORBA::Float x, CORBA::Float y, CORBA::Float& ans);
};


void Echo_i::bind(const Name& name, CORBA::Object* obj)
{
  cerr << "Upcall ID:    " << name[0].id << endl;
  cerr << "Upcall Kind:  " << name[0].kind << endl;
  return ;
}

char * Echo_i::echoString(const char * mesg)
{
  cerr << "Upcall " << mesg << endl;
  return CORBA::string_dup(mesg);
}

char* Echo_i::echoString2(const char* mesg, CORBA::String_out res){
  cerr << "Upcall " << mesg << endl;

  res =  CORBA::string_dup("OK");
  return CORBA::string_dup(mesg);
}
char* Echo_i::echoString3(char*& mesg){
  cerr << "Upcall " << mesg << endl;

  return CORBA::string_dup(mesg);
}
CORBA::Long Echo_i::plus(CORBA::Long x, CORBA::Long y){
  return  x + y;
}
void Echo_i::plus2(CORBA::Long x, CORBA::Long y, CORBA::Long& ans){
  ans = x + y;
  return;
}
CORBA::Float Echo_i::divide(CORBA::Float x, CORBA::Float y){
  return  x / y;
}
void Echo_i::divide2(CORBA::Float x, CORBA::Float y, CORBA::Float& ans){
  ans = x / y;
  return ;
}
//////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
  try {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);

    Echo_i* myobj = new Echo_i();
      
    PortableServer::ObjectId_var myobjid = poa->activate_object(myobj);

    // Obtain a reference to the object, and print it out as a
    // stringified IOR.
    obj = myobj->_this();
    CORBA::String_var sior(orb->object_to_string(obj));
    cerr << "'" << (char*)sior << "'" << endl;

    myobj->_remove_ref();

    PortableServer::POAManager_var pman = poa->the_POAManager();
    pman->activate();

    orb->run();
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
