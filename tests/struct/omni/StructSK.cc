/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
// This file is generated by omniidl (C++ backend)- omniORB_4_0. Do not edit.

#include "Struct.hh"
#include <omniORB4/IOP_S.h>
#include <omniORB4/IOP_C.h>
#include <omniORB4/callDescriptor.h>
#include <omniORB4/callHandle.h>
#include <omniORB4/objTracker.h>


OMNI_USING_NAMESPACE(omni)

static const char* _0RL_library_version = omniORB_4_0;



void
StructTest::Point::operator>>= (cdrStream &_n) const
{
  x >>= _n;
  y >>= _n;

}

void
StructTest::Point::operator<<= (cdrStream &_n)
{
  (CORBA::Long&)x <<= _n;
  (CORBA::Long&)y <<= _n;

}

StructTest::Tester_ptr StructTest::Tester_Helper::_nil() {
  return ::StructTest::Tester::_nil();
}

CORBA::Boolean StructTest::Tester_Helper::is_nil(::StructTest::Tester_ptr p) {
  return CORBA::is_nil(p);

}

void StructTest::Tester_Helper::release(::StructTest::Tester_ptr p) {
  CORBA::release(p);
}

void StructTest::Tester_Helper::duplicate(::StructTest::Tester_ptr p) {
  if( p && !p->_NP_is_nil() )  omni::duplicateObjRef(p);
}

void StructTest::Tester_Helper::marshalObjRef(::StructTest::Tester_ptr obj, cdrStream& s) {
  ::StructTest::Tester::_marshalObjRef(obj, s);
}

StructTest::Tester_ptr StructTest::Tester_Helper::unmarshalObjRef(cdrStream& s) {
  return ::StructTest::Tester::_unmarshalObjRef(s);
}

StructTest::Tester_ptr
StructTest::Tester::_duplicate(::StructTest::Tester_ptr obj)
{
  if( obj && !obj->_NP_is_nil() )  omni::duplicateObjRef(obj);

  return obj;
}


StructTest::Tester_ptr
StructTest::Tester::_narrow(CORBA::Object_ptr obj)
{
  if( !obj || obj->_NP_is_nil() || obj->_NP_is_pseudo() ) return _nil();
  _ptr_type e = (_ptr_type) obj->_PR_getobj()->_realNarrow(_PD_repoId);
  return e ? e : _nil();
}


StructTest::Tester_ptr
StructTest::Tester::_unchecked_narrow(CORBA::Object_ptr obj)
{
  if( !obj || obj->_NP_is_nil() || obj->_NP_is_pseudo() ) return _nil();
  _ptr_type e = (_ptr_type) obj->_PR_getobj()->_uncheckedNarrow(_PD_repoId);
  return e ? e : _nil();
}


StructTest::Tester_ptr
StructTest::Tester::_nil()
{
#ifdef OMNI_UNLOADABLE_STUBS
  static _objref_Tester _the_nil_obj;
  return &_the_nil_obj;
#else
  static _objref_Tester* _the_nil_ptr = 0;
  if( !_the_nil_ptr ) {
    omni::nilRefLock().lock();
    if( !_the_nil_ptr ) {
      _the_nil_ptr = new _objref_Tester;
      registerNilCorbaObject(_the_nil_ptr);
    }
    omni::nilRefLock().unlock();
  }
  return _the_nil_ptr;
#endif
}

const char* StructTest::Tester::_PD_repoId = "IDL:StructTest/Tester:1.0";


StructTest::_objref_Tester::~_objref_Tester() {}


StructTest::_objref_Tester::_objref_Tester(omniIOR* ior, omniIdentity* id) :
   omniObjRef(::StructTest::Tester::_PD_repoId, ior, id, 1)
   
   
{
  _PR_setobj(this);
}

void*
StructTest::_objref_Tester::_ptrToObjRef(const char* id)
{
  if( id == ::StructTest::Tester::_PD_repoId )
    return (::StructTest::Tester_ptr) this;
  
  if( id == ::CORBA::Object::_PD_repoId )
    return (::CORBA::Object_ptr) this;

  if( omni::strMatch(id, ::StructTest::Tester::_PD_repoId) )
    return (::StructTest::Tester_ptr) this;
  
  if( omni::strMatch(id, ::CORBA::Object::_PD_repoId) )
    return (::CORBA::Object_ptr) this;

  return 0;
}

// Proxy call descriptor class. Mangled signature:
//  _cStructTest_mPoint
class _0RL_cd_B5C5DB90E7926E71_00000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_B5C5DB90E7926E71_00000000(LocalCallFn lcfn,const char* op_,size_t oplen,_CORBA_Boolean upcall=0):
     omniCallDescriptor(lcfn, op_, oplen, 0, 0, 0, upcall) {}
  
  
  void unmarshalReturnedValues(cdrStream&);
  void marshalReturnedValues(cdrStream&);
  
  
  StructTest::Point result;
};

void _0RL_cd_B5C5DB90E7926E71_00000000::marshalReturnedValues(cdrStream& _n)
{
  (const StructTest::Point&) result >>= _n;

}

void _0RL_cd_B5C5DB90E7926E71_00000000::unmarshalReturnedValues(cdrStream& _n)
{
  (StructTest::Point&)result <<= _n;

}

// Local call call-back function.
static void
_0RL_lcfn_B5C5DB90E7926E71_10000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_B5C5DB90E7926E71_00000000* tcd = (_0RL_cd_B5C5DB90E7926E71_00000000*)cd;
  StructTest::_impl_Tester* impl = (StructTest::_impl_Tester*) svnt->_ptrToInterface(StructTest::Tester::_PD_repoId);
  tcd->result = impl->ret();


}

StructTest::Point StructTest::_objref_Tester::ret()
{
  _0RL_cd_B5C5DB90E7926E71_00000000 _call_desc(_0RL_lcfn_B5C5DB90E7926E71_10000000, "ret", 4);


  _invoke(_call_desc);
  return _call_desc.result;


}
// Proxy call descriptor class. Mangled signature:
//  void_o_cStructTest_mPoint
class _0RL_cd_B5C5DB90E7926E71_20000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_B5C5DB90E7926E71_20000000(LocalCallFn lcfn,const char* op_,size_t oplen,_CORBA_Boolean upcall=0):
     omniCallDescriptor(lcfn, op_, oplen, 0, 0, 0, upcall) {}
  
  
  void unmarshalReturnedValues(cdrStream&);
  void marshalReturnedValues(cdrStream&);
  
  
  StructTest::Point arg_0_;
  StructTest::Point* arg_0;
};

void _0RL_cd_B5C5DB90E7926E71_20000000::marshalReturnedValues(cdrStream& _n)
{
  (const StructTest::Point&) *arg_0 >>= _n;

}

void _0RL_cd_B5C5DB90E7926E71_20000000::unmarshalReturnedValues(cdrStream& _n)
{
  (StructTest::Point&)*arg_0 <<= _n;

}

// Local call call-back function.
static void
_0RL_lcfn_B5C5DB90E7926E71_30000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_B5C5DB90E7926E71_20000000* tcd = (_0RL_cd_B5C5DB90E7926E71_20000000*)cd;
  StructTest::_impl_Tester* impl = (StructTest::_impl_Tester*) svnt->_ptrToInterface(StructTest::Tester::_PD_repoId);
  impl->get(*tcd->arg_0);


}

void StructTest::_objref_Tester::get(Point& p)
{
  _0RL_cd_B5C5DB90E7926E71_20000000 _call_desc(_0RL_lcfn_B5C5DB90E7926E71_30000000, "get", 4);
  _call_desc.arg_0 = &(Point&) p;

  _invoke(_call_desc);



}
// Proxy call descriptor class. Mangled signature:
//  void_i_cStructTest_mPoint
class _0RL_cd_B5C5DB90E7926E71_40000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_B5C5DB90E7926E71_40000000(LocalCallFn lcfn,const char* op_,size_t oplen,_CORBA_Boolean upcall=0):
     omniCallDescriptor(lcfn, op_, oplen, 0, 0, 0, upcall) {}
  
  void marshalArguments(cdrStream&);
  void unmarshalArguments(cdrStream&);

    
  
  StructTest::Point arg_0_;
  const StructTest::Point* arg_0;
};

void _0RL_cd_B5C5DB90E7926E71_40000000::marshalArguments(cdrStream& _n)
{
  (const StructTest::Point&) *arg_0 >>= _n;

}

void _0RL_cd_B5C5DB90E7926E71_40000000::unmarshalArguments(cdrStream& _n)
{
  (StructTest::Point&)arg_0_ <<= _n;
  arg_0 = &arg_0_;

}

// Local call call-back function.
static void
_0RL_lcfn_B5C5DB90E7926E71_50000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_B5C5DB90E7926E71_40000000* tcd = (_0RL_cd_B5C5DB90E7926E71_40000000*)cd;
  StructTest::_impl_Tester* impl = (StructTest::_impl_Tester*) svnt->_ptrToInterface(StructTest::Tester::_PD_repoId);
  impl->set(*tcd->arg_0);


}

void StructTest::_objref_Tester::set(const Point& p)
{
  _0RL_cd_B5C5DB90E7926E71_40000000 _call_desc(_0RL_lcfn_B5C5DB90E7926E71_50000000, "set", 4);
  _call_desc.arg_0 = &(Point&) p;

  _invoke(_call_desc);



}
// Proxy call descriptor class. Mangled signature:
//  void_n_cStructTest_mPoint
class _0RL_cd_B5C5DB90E7926E71_60000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_B5C5DB90E7926E71_60000000(LocalCallFn lcfn,const char* op_,size_t oplen,_CORBA_Boolean upcall=0):
     omniCallDescriptor(lcfn, op_, oplen, 0, 0, 0, upcall) {}
  
  void marshalArguments(cdrStream&);
  void unmarshalArguments(cdrStream&);

  void unmarshalReturnedValues(cdrStream&);
  void marshalReturnedValues(cdrStream&);
  
  
  StructTest::Point arg_0_;
  StructTest::Point* arg_0;
};

void _0RL_cd_B5C5DB90E7926E71_60000000::marshalArguments(cdrStream& _n)
{
  (const StructTest::Point&) *arg_0 >>= _n;

}

void _0RL_cd_B5C5DB90E7926E71_60000000::unmarshalArguments(cdrStream& _n)
{
  (StructTest::Point&)arg_0_ <<= _n;
  arg_0 = &arg_0_;

}

void _0RL_cd_B5C5DB90E7926E71_60000000::marshalReturnedValues(cdrStream& _n)
{
  (const StructTest::Point&) *arg_0 >>= _n;

}

void _0RL_cd_B5C5DB90E7926E71_60000000::unmarshalReturnedValues(cdrStream& _n)
{
  (StructTest::Point&)*arg_0 <<= _n;

}

// Local call call-back function.
static void
_0RL_lcfn_B5C5DB90E7926E71_70000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_B5C5DB90E7926E71_60000000* tcd = (_0RL_cd_B5C5DB90E7926E71_60000000*)cd;
  StructTest::_impl_Tester* impl = (StructTest::_impl_Tester*) svnt->_ptrToInterface(StructTest::Tester::_PD_repoId);
  impl->update(*tcd->arg_0);


}

void StructTest::_objref_Tester::update(Point& p)
{
  _0RL_cd_B5C5DB90E7926E71_60000000 _call_desc(_0RL_lcfn_B5C5DB90E7926E71_70000000, "update", 7);
  _call_desc.arg_0 = &(Point&) p;

  _invoke(_call_desc);



}
StructTest::_pof_Tester::~_pof_Tester() {}


omniObjRef*
StructTest::_pof_Tester::newObjRef(omniIOR* ior, omniIdentity* id)
{
  return new ::StructTest::_objref_Tester(ior, id);
}


CORBA::Boolean
StructTest::_pof_Tester::is_a(const char* id) const
{
  if( omni::ptrStrMatch(id, ::StructTest::Tester::_PD_repoId) )
    return 1;
  
  return 0;
}

const StructTest::_pof_Tester _the_pof_StructTest_mTester;

StructTest::_impl_Tester::~_impl_Tester() {}


CORBA::Boolean
StructTest::_impl_Tester::_dispatch(omniCallHandle& _handle)
{
  const char* op = _handle.operation_name();

  if( omni::strMatch(op, "ret") ) {

    _0RL_cd_B5C5DB90E7926E71_00000000 _call_desc(_0RL_lcfn_B5C5DB90E7926E71_10000000, "ret", 4, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if( omni::strMatch(op, "get") ) {

    _0RL_cd_B5C5DB90E7926E71_20000000 _call_desc(_0RL_lcfn_B5C5DB90E7926E71_30000000, "get", 4, 1);
    _call_desc.arg_0 = &_call_desc.arg_0_;
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if( omni::strMatch(op, "set") ) {

    _0RL_cd_B5C5DB90E7926E71_40000000 _call_desc(_0RL_lcfn_B5C5DB90E7926E71_50000000, "set", 4, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if( omni::strMatch(op, "update") ) {

    _0RL_cd_B5C5DB90E7926E71_60000000 _call_desc(_0RL_lcfn_B5C5DB90E7926E71_70000000, "update", 7, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }


  return 0;
}

void*
StructTest::_impl_Tester::_ptrToInterface(const char* id)
{
  if( id == ::StructTest::Tester::_PD_repoId )
    return (::StructTest::_impl_Tester*) this;
  
  if( id == ::CORBA::Object::_PD_repoId )
    return (void*) 1;

  if( omni::strMatch(id, ::StructTest::Tester::_PD_repoId) )
    return (::StructTest::_impl_Tester*) this;
  
  if( omni::strMatch(id, ::CORBA::Object::_PD_repoId) )
    return (void*) 1;
  return 0;
}

const char*
StructTest::_impl_Tester::_mostDerivedRepoId()
{
  return ::StructTest::Tester::_PD_repoId;
}

POA_StructTest::Tester::~Tester() {}

