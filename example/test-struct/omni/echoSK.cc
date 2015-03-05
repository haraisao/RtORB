/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
// This file is generated by omniidl (C++ backend)- omniORB_4_0. Do not edit.

#include "echo.hh"
#include <omniORB4/IOP_S.h>
#include <omniORB4/IOP_C.h>
#include <omniORB4/callDescriptor.h>
#include <omniORB4/callHandle.h>
#include <omniORB4/objTracker.h>


OMNI_USING_NAMESPACE(omni)

static const char* _0RL_library_version = omniORB_4_0;



void
NameComponent::operator>>= (cdrStream &_n) const
{
  _n.marshalString(id,0);
  _n.marshalString(kind,0);

}

void
NameComponent::operator<<= (cdrStream &_n)
{
  id = _n.unmarshalString(0);
  kind = _n.unmarshalString(0);

}

Echo_ptr Echo_Helper::_nil() {
  return ::Echo::_nil();
}

CORBA::Boolean Echo_Helper::is_nil(::Echo_ptr p) {
  return CORBA::is_nil(p);

}

void Echo_Helper::release(::Echo_ptr p) {
  CORBA::release(p);
}

void Echo_Helper::duplicate(::Echo_ptr p) {
  if( p && !p->_NP_is_nil() )  omni::duplicateObjRef(p);
}

void Echo_Helper::marshalObjRef(::Echo_ptr obj, cdrStream& s) {
  ::Echo::_marshalObjRef(obj, s);
}

Echo_ptr Echo_Helper::unmarshalObjRef(cdrStream& s) {
  return ::Echo::_unmarshalObjRef(s);
}

Echo_ptr
Echo::_duplicate(::Echo_ptr obj)
{
  if( obj && !obj->_NP_is_nil() )  omni::duplicateObjRef(obj);

  return obj;
}


Echo_ptr
Echo::_narrow(CORBA::Object_ptr obj)
{
  if( !obj || obj->_NP_is_nil() || obj->_NP_is_pseudo() ) return _nil();
  _ptr_type e = (_ptr_type) obj->_PR_getobj()->_realNarrow(_PD_repoId);
  return e ? e : _nil();
}


Echo_ptr
Echo::_unchecked_narrow(CORBA::Object_ptr obj)
{
  if( !obj || obj->_NP_is_nil() || obj->_NP_is_pseudo() ) return _nil();
  _ptr_type e = (_ptr_type) obj->_PR_getobj()->_uncheckedNarrow(_PD_repoId);
  return e ? e : _nil();
}


Echo_ptr
Echo::_nil()
{
#ifdef OMNI_UNLOADABLE_STUBS
  static _objref_Echo _the_nil_obj;
  return &_the_nil_obj;
#else
  static _objref_Echo* _the_nil_ptr = 0;
  if( !_the_nil_ptr ) {
    omni::nilRefLock().lock();
    if( !_the_nil_ptr ) {
      _the_nil_ptr = new _objref_Echo;
      registerNilCorbaObject(_the_nil_ptr);
    }
    omni::nilRefLock().unlock();
  }
  return _the_nil_ptr;
#endif
}

const char* Echo::_PD_repoId = "IDL:Echo:1.0";


_objref_Echo::~_objref_Echo() {}


_objref_Echo::_objref_Echo(omniIOR* ior, omniIdentity* id) :
   omniObjRef(::Echo::_PD_repoId, ior, id, 1)
   
   
{
  _PR_setobj(this);
}

void*
_objref_Echo::_ptrToObjRef(const char* id)
{
  if( id == ::Echo::_PD_repoId )
    return (::Echo_ptr) this;
  
  if( id == ::CORBA::Object::_PD_repoId )
    return (::CORBA::Object_ptr) this;

  if( omni::strMatch(id, ::Echo::_PD_repoId) )
    return (::Echo_ptr) this;
  
  if( omni::strMatch(id, ::CORBA::Object::_PD_repoId) )
    return (::CORBA::Object_ptr) this;

  return 0;
}

// Proxy call descriptor class. Mangled signature:
//  _cstring_i_cstring
class _0RL_cd_01531D3E51C81A80_00000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_01531D3E51C81A80_00000000(LocalCallFn lcfn,const char* op_,size_t oplen,_CORBA_Boolean upcall=0):
     omniCallDescriptor(lcfn, op_, oplen, 0, 0, 0, upcall) {}
  
  void marshalArguments(cdrStream&);
  void unmarshalArguments(cdrStream&);

  void unmarshalReturnedValues(cdrStream&);
  void marshalReturnedValues(cdrStream&);
  
  
  CORBA::String_var arg_0_;
  const char* arg_0;
  CORBA::String_var result;
};

void _0RL_cd_01531D3E51C81A80_00000000::marshalArguments(cdrStream& _n)
{
  _n.marshalString(arg_0,0);

}

void _0RL_cd_01531D3E51C81A80_00000000::unmarshalArguments(cdrStream& _n)
{
  arg_0_ = _n.unmarshalString(0);
  arg_0 = arg_0_.in();

}

void _0RL_cd_01531D3E51C81A80_00000000::marshalReturnedValues(cdrStream& _n)
{
  _n.marshalString(result,0);

}

void _0RL_cd_01531D3E51C81A80_00000000::unmarshalReturnedValues(cdrStream& _n)
{
  result = _n.unmarshalString(0);

}

// Local call call-back function.
static void
_0RL_lcfn_01531D3E51C81A80_10000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_01531D3E51C81A80_00000000* tcd = (_0RL_cd_01531D3E51C81A80_00000000*)cd;
  _impl_Echo* impl = (_impl_Echo*) svnt->_ptrToInterface(Echo::_PD_repoId);
  tcd->result = impl->echoString(tcd->arg_0);


}

char* _objref_Echo::echoString(const char* mesg)
{
  _0RL_cd_01531D3E51C81A80_00000000 _call_desc(_0RL_lcfn_01531D3E51C81A80_10000000, "echoString", 11);
  _call_desc.arg_0 = mesg;

  _invoke(_call_desc);
  return _call_desc.result._retn();


}
// Proxy call descriptor class. Mangled signature:
//  _cstring_i_cstring_o_cstring
class _0RL_cd_01531D3E51C81A80_20000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_01531D3E51C81A80_20000000(LocalCallFn lcfn,const char* op_,size_t oplen,_CORBA_Boolean upcall=0):
     omniCallDescriptor(lcfn, op_, oplen, 0, 0, 0, upcall) {}
  
  void marshalArguments(cdrStream&);
  void unmarshalArguments(cdrStream&);

  void unmarshalReturnedValues(cdrStream&);
  void marshalReturnedValues(cdrStream&);
  
  
  CORBA::String_var arg_0_;
  const char* arg_0;
  CORBA::String_var arg_1;
  CORBA::String_var result;
};

void _0RL_cd_01531D3E51C81A80_20000000::marshalArguments(cdrStream& _n)
{
  _n.marshalString(arg_0,0);

}

void _0RL_cd_01531D3E51C81A80_20000000::unmarshalArguments(cdrStream& _n)
{
  arg_0_ = _n.unmarshalString(0);
  arg_0 = arg_0_.in();

}

void _0RL_cd_01531D3E51C81A80_20000000::marshalReturnedValues(cdrStream& _n)
{
  _n.marshalString(result,0);
  _n.marshalString(arg_1,0);

}

void _0RL_cd_01531D3E51C81A80_20000000::unmarshalReturnedValues(cdrStream& _n)
{
  result = _n.unmarshalString(0);
  arg_1 = _n.unmarshalString(0);

}

// Local call call-back function.
static void
_0RL_lcfn_01531D3E51C81A80_30000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_01531D3E51C81A80_20000000* tcd = (_0RL_cd_01531D3E51C81A80_20000000*)cd;
  _impl_Echo* impl = (_impl_Echo*) svnt->_ptrToInterface(Echo::_PD_repoId);
  tcd->result = impl->echoString2(tcd->arg_0, tcd->arg_1.out());


}

char* _objref_Echo::echoString2(const char* mesg, CORBA::String_out res)
{
  _0RL_cd_01531D3E51C81A80_20000000 _call_desc(_0RL_lcfn_01531D3E51C81A80_30000000, "echoString2", 12);
  _call_desc.arg_0 = mesg;

  _invoke(_call_desc);
  res = _call_desc.arg_1._retn();
  return _call_desc.result._retn();


}
// Proxy call descriptor class. Mangled signature:
//  _cstring_n_cstring
class _0RL_cd_01531D3E51C81A80_40000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_01531D3E51C81A80_40000000(LocalCallFn lcfn,const char* op_,size_t oplen,_CORBA_Boolean upcall=0):
     omniCallDescriptor(lcfn, op_, oplen, 0, 0, 0, upcall) {}
  
  void marshalArguments(cdrStream&);
  void unmarshalArguments(cdrStream&);

  void unmarshalReturnedValues(cdrStream&);
  void marshalReturnedValues(cdrStream&);
  
  
  CORBA::String_var arg_0_;
  char** arg_0;
  CORBA::String_var result;
};

void _0RL_cd_01531D3E51C81A80_40000000::marshalArguments(cdrStream& _n)
{
  _n.marshalString(*arg_0,0);

}

void _0RL_cd_01531D3E51C81A80_40000000::unmarshalArguments(cdrStream& _n)
{
  arg_0_ = _n.unmarshalString(0);
  arg_0 = &arg_0_.inout();

}

void _0RL_cd_01531D3E51C81A80_40000000::marshalReturnedValues(cdrStream& _n)
{
  _n.marshalString(result,0);
  _n.marshalString(*arg_0,0);

}

void _0RL_cd_01531D3E51C81A80_40000000::unmarshalReturnedValues(cdrStream& _n)
{
  result = _n.unmarshalString(0);
  arg_0_ = *arg_0;
  *arg_0 = (char*) _CORBA_String_helper::empty_string;
  *arg_0 = _n.unmarshalString(0);

}

// Local call call-back function.
static void
_0RL_lcfn_01531D3E51C81A80_50000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_01531D3E51C81A80_40000000* tcd = (_0RL_cd_01531D3E51C81A80_40000000*)cd;
  _impl_Echo* impl = (_impl_Echo*) svnt->_ptrToInterface(Echo::_PD_repoId);
  tcd->result = impl->echoString3(*tcd->arg_0);


}

char* _objref_Echo::echoString3(CORBA::String_INOUT_arg mesg)
{
  _0RL_cd_01531D3E51C81A80_40000000 _call_desc(_0RL_lcfn_01531D3E51C81A80_50000000, "echoString3", 12);
  _call_desc.arg_0 = &(char*&) mesg;

  _invoke(_call_desc);
  return _call_desc.result._retn();


}
// Proxy call descriptor class. Mangled signature:
//  _clong_i_clong_i_clong
class _0RL_cd_01531D3E51C81A80_60000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_01531D3E51C81A80_60000000(LocalCallFn lcfn,const char* op_,size_t oplen,_CORBA_Boolean upcall=0):
     omniCallDescriptor(lcfn, op_, oplen, 0, 0, 0, upcall) {}
  
  void marshalArguments(cdrStream&);
  void unmarshalArguments(cdrStream&);

  void unmarshalReturnedValues(cdrStream&);
  void marshalReturnedValues(cdrStream&);
  
  
  CORBA::Long arg_0;
  CORBA::Long arg_1;
  CORBA::Long result;
};

void _0RL_cd_01531D3E51C81A80_60000000::marshalArguments(cdrStream& _n)
{
  arg_0 >>= _n;
  arg_1 >>= _n;

}

void _0RL_cd_01531D3E51C81A80_60000000::unmarshalArguments(cdrStream& _n)
{
  (CORBA::Long&)arg_0 <<= _n;
  (CORBA::Long&)arg_1 <<= _n;

}

void _0RL_cd_01531D3E51C81A80_60000000::marshalReturnedValues(cdrStream& _n)
{
  result >>= _n;

}

void _0RL_cd_01531D3E51C81A80_60000000::unmarshalReturnedValues(cdrStream& _n)
{
  (CORBA::Long&)result <<= _n;

}

// Local call call-back function.
static void
_0RL_lcfn_01531D3E51C81A80_70000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_01531D3E51C81A80_60000000* tcd = (_0RL_cd_01531D3E51C81A80_60000000*)cd;
  _impl_Echo* impl = (_impl_Echo*) svnt->_ptrToInterface(Echo::_PD_repoId);
  tcd->result = impl->plus(tcd->arg_0, tcd->arg_1);


}

CORBA::Long _objref_Echo::plus(CORBA::Long x, CORBA::Long y)
{
  _0RL_cd_01531D3E51C81A80_60000000 _call_desc(_0RL_lcfn_01531D3E51C81A80_70000000, "plus", 5);
  _call_desc.arg_0 = x;
  _call_desc.arg_1 = y;

  _invoke(_call_desc);
  return _call_desc.result;


}
// Proxy call descriptor class. Mangled signature:
//  void_i_clong_i_clong_o_clong
class _0RL_cd_01531D3E51C81A80_80000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_01531D3E51C81A80_80000000(LocalCallFn lcfn,const char* op_,size_t oplen,_CORBA_Boolean upcall=0):
     omniCallDescriptor(lcfn, op_, oplen, 0, 0, 0, upcall) {}
  
  void marshalArguments(cdrStream&);
  void unmarshalArguments(cdrStream&);

  void unmarshalReturnedValues(cdrStream&);
  void marshalReturnedValues(cdrStream&);
  
  
  CORBA::Long arg_0;
  CORBA::Long arg_1;
  CORBA::Long arg_2;
};

void _0RL_cd_01531D3E51C81A80_80000000::marshalArguments(cdrStream& _n)
{
  arg_0 >>= _n;
  arg_1 >>= _n;

}

void _0RL_cd_01531D3E51C81A80_80000000::unmarshalArguments(cdrStream& _n)
{
  (CORBA::Long&)arg_0 <<= _n;
  (CORBA::Long&)arg_1 <<= _n;

}

void _0RL_cd_01531D3E51C81A80_80000000::marshalReturnedValues(cdrStream& _n)
{
  arg_2 >>= _n;

}

void _0RL_cd_01531D3E51C81A80_80000000::unmarshalReturnedValues(cdrStream& _n)
{
  (CORBA::Long&)arg_2 <<= _n;

}

// Local call call-back function.
static void
_0RL_lcfn_01531D3E51C81A80_90000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_01531D3E51C81A80_80000000* tcd = (_0RL_cd_01531D3E51C81A80_80000000*)cd;
  _impl_Echo* impl = (_impl_Echo*) svnt->_ptrToInterface(Echo::_PD_repoId);
  impl->plus2(tcd->arg_0, tcd->arg_1, tcd->arg_2);


}

void _objref_Echo::plus2(CORBA::Long x, CORBA::Long y, CORBA::Long& ans)
{
  _0RL_cd_01531D3E51C81A80_80000000 _call_desc(_0RL_lcfn_01531D3E51C81A80_90000000, "plus2", 6);
  _call_desc.arg_0 = x;
  _call_desc.arg_1 = y;

  _invoke(_call_desc);
  ans = _call_desc.arg_2;


}
// Proxy call descriptor class. Mangled signature:
//  _cfloat_i_cfloat_i_cfloat
class _0RL_cd_01531D3E51C81A80_a0000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_01531D3E51C81A80_a0000000(LocalCallFn lcfn,const char* op_,size_t oplen,_CORBA_Boolean upcall=0):
     omniCallDescriptor(lcfn, op_, oplen, 0, 0, 0, upcall) {}
  
  void marshalArguments(cdrStream&);
  void unmarshalArguments(cdrStream&);

  void unmarshalReturnedValues(cdrStream&);
  void marshalReturnedValues(cdrStream&);
  
  
  CORBA::Float arg_0;
  CORBA::Float arg_1;
  CORBA::Float result;
};

void _0RL_cd_01531D3E51C81A80_a0000000::marshalArguments(cdrStream& _n)
{
  arg_0 >>= _n;
  arg_1 >>= _n;

}

void _0RL_cd_01531D3E51C81A80_a0000000::unmarshalArguments(cdrStream& _n)
{
  (CORBA::Float&)arg_0 <<= _n;
  (CORBA::Float&)arg_1 <<= _n;

}

void _0RL_cd_01531D3E51C81A80_a0000000::marshalReturnedValues(cdrStream& _n)
{
  result >>= _n;

}

void _0RL_cd_01531D3E51C81A80_a0000000::unmarshalReturnedValues(cdrStream& _n)
{
  (CORBA::Float&)result <<= _n;

}

// Local call call-back function.
static void
_0RL_lcfn_01531D3E51C81A80_b0000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_01531D3E51C81A80_a0000000* tcd = (_0RL_cd_01531D3E51C81A80_a0000000*)cd;
  _impl_Echo* impl = (_impl_Echo*) svnt->_ptrToInterface(Echo::_PD_repoId);
  tcd->result = impl->divide(tcd->arg_0, tcd->arg_1);


}

CORBA::Float _objref_Echo::divide(CORBA::Float x, CORBA::Float y)
{
  _0RL_cd_01531D3E51C81A80_a0000000 _call_desc(_0RL_lcfn_01531D3E51C81A80_b0000000, "divide", 7);
  _call_desc.arg_0 = x;
  _call_desc.arg_1 = y;

  _invoke(_call_desc);
  return _call_desc.result;


}
// Proxy call descriptor class. Mangled signature:
//  void_i_cfloat_i_cfloat_o_cfloat
class _0RL_cd_01531D3E51C81A80_c0000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_01531D3E51C81A80_c0000000(LocalCallFn lcfn,const char* op_,size_t oplen,_CORBA_Boolean upcall=0):
     omniCallDescriptor(lcfn, op_, oplen, 0, 0, 0, upcall) {}
  
  void marshalArguments(cdrStream&);
  void unmarshalArguments(cdrStream&);

  void unmarshalReturnedValues(cdrStream&);
  void marshalReturnedValues(cdrStream&);
  
  
  CORBA::Float arg_0;
  CORBA::Float arg_1;
  CORBA::Float arg_2;
};

void _0RL_cd_01531D3E51C81A80_c0000000::marshalArguments(cdrStream& _n)
{
  arg_0 >>= _n;
  arg_1 >>= _n;

}

void _0RL_cd_01531D3E51C81A80_c0000000::unmarshalArguments(cdrStream& _n)
{
  (CORBA::Float&)arg_0 <<= _n;
  (CORBA::Float&)arg_1 <<= _n;

}

void _0RL_cd_01531D3E51C81A80_c0000000::marshalReturnedValues(cdrStream& _n)
{
  arg_2 >>= _n;

}

void _0RL_cd_01531D3E51C81A80_c0000000::unmarshalReturnedValues(cdrStream& _n)
{
  (CORBA::Float&)arg_2 <<= _n;

}

// Local call call-back function.
static void
_0RL_lcfn_01531D3E51C81A80_d0000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_01531D3E51C81A80_c0000000* tcd = (_0RL_cd_01531D3E51C81A80_c0000000*)cd;
  _impl_Echo* impl = (_impl_Echo*) svnt->_ptrToInterface(Echo::_PD_repoId);
  impl->divide2(tcd->arg_0, tcd->arg_1, tcd->arg_2);


}

void _objref_Echo::divide2(CORBA::Float x, CORBA::Float y, CORBA::Float& ans)
{
  _0RL_cd_01531D3E51C81A80_c0000000 _call_desc(_0RL_lcfn_01531D3E51C81A80_d0000000, "divide2", 8);
  _call_desc.arg_0 = x;
  _call_desc.arg_1 = y;

  _invoke(_call_desc);
  ans = _call_desc.arg_2;


}
// Proxy call descriptor class. Mangled signature:
//  void_i_cName_i_cCORBA_mObject
class _0RL_cd_01531D3E51C81A80_e0000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_01531D3E51C81A80_e0000000(LocalCallFn lcfn,const char* op_,size_t oplen,_CORBA_Boolean upcall=0):
     omniCallDescriptor(lcfn, op_, oplen, 0, 0, 0, upcall) {}
  
  void marshalArguments(cdrStream&);
  void unmarshalArguments(cdrStream&);

    
  
  Name_var arg_0_;
  const Name* arg_0;
  CORBA::Object_var arg_1_;
  CORBA::Object_ptr arg_1;
};

void _0RL_cd_01531D3E51C81A80_e0000000::marshalArguments(cdrStream& _n)
{
  (const Name&) *arg_0 >>= _n;
  CORBA::Object::_marshalObjRef(arg_1,_n);

}

void _0RL_cd_01531D3E51C81A80_e0000000::unmarshalArguments(cdrStream& _n)
{
  arg_0_ = new Name;
  (Name&)arg_0_ <<= _n;
  arg_0 = &arg_0_.in();
  arg_1_ = CORBA::Object::_unmarshalObjRef(_n);
  arg_1 = arg_1_.in();

}

// Local call call-back function.
static void
_0RL_lcfn_01531D3E51C81A80_f0000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_01531D3E51C81A80_e0000000* tcd = (_0RL_cd_01531D3E51C81A80_e0000000*)cd;
  _impl_Echo* impl = (_impl_Echo*) svnt->_ptrToInterface(Echo::_PD_repoId);
  impl->bind(*tcd->arg_0, tcd->arg_1);


}

void _objref_Echo::bind(const Name& n, ::CORBA::Object_ptr obj)
{
  _0RL_cd_01531D3E51C81A80_e0000000 _call_desc(_0RL_lcfn_01531D3E51C81A80_f0000000, "bind", 5);
  _call_desc.arg_0 = &(Name&) n;
  _call_desc.arg_1 = obj;

  _invoke(_call_desc);



}
_pof_Echo::~_pof_Echo() {}


omniObjRef*
_pof_Echo::newObjRef(omniIOR* ior, omniIdentity* id)
{
  return new ::_objref_Echo(ior, id);
}


CORBA::Boolean
_pof_Echo::is_a(const char* id) const
{
  if( omni::ptrStrMatch(id, ::Echo::_PD_repoId) )
    return 1;
  
  return 0;
}

const _pof_Echo _the_pof_Echo;

_impl_Echo::~_impl_Echo() {}


CORBA::Boolean
_impl_Echo::_dispatch(omniCallHandle& _handle)
{
  const char* op = _handle.operation_name();

  if( omni::strMatch(op, "echoString") ) {

    _0RL_cd_01531D3E51C81A80_00000000 _call_desc(_0RL_lcfn_01531D3E51C81A80_10000000, "echoString", 11, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if( omni::strMatch(op, "echoString2") ) {

    _0RL_cd_01531D3E51C81A80_20000000 _call_desc(_0RL_lcfn_01531D3E51C81A80_30000000, "echoString2", 12, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if( omni::strMatch(op, "echoString3") ) {

    _0RL_cd_01531D3E51C81A80_40000000 _call_desc(_0RL_lcfn_01531D3E51C81A80_50000000, "echoString3", 12, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if( omni::strMatch(op, "plus") ) {

    _0RL_cd_01531D3E51C81A80_60000000 _call_desc(_0RL_lcfn_01531D3E51C81A80_70000000, "plus", 5, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if( omni::strMatch(op, "plus2") ) {

    _0RL_cd_01531D3E51C81A80_80000000 _call_desc(_0RL_lcfn_01531D3E51C81A80_90000000, "plus2", 6, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if( omni::strMatch(op, "divide") ) {

    _0RL_cd_01531D3E51C81A80_a0000000 _call_desc(_0RL_lcfn_01531D3E51C81A80_b0000000, "divide", 7, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if( omni::strMatch(op, "divide2") ) {

    _0RL_cd_01531D3E51C81A80_c0000000 _call_desc(_0RL_lcfn_01531D3E51C81A80_d0000000, "divide2", 8, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if( omni::strMatch(op, "bind") ) {

    _0RL_cd_01531D3E51C81A80_e0000000 _call_desc(_0RL_lcfn_01531D3E51C81A80_f0000000, "bind", 5, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }


  return 0;
}

void*
_impl_Echo::_ptrToInterface(const char* id)
{
  if( id == ::Echo::_PD_repoId )
    return (::_impl_Echo*) this;
  
  if( id == ::CORBA::Object::_PD_repoId )
    return (void*) 1;

  if( omni::strMatch(id, ::Echo::_PD_repoId) )
    return (::_impl_Echo*) this;
  
  if( omni::strMatch(id, ::CORBA::Object::_PD_repoId) )
    return (void*) 1;
  return 0;
}

const char*
_impl_Echo::_mostDerivedRepoId()
{
  return ::Echo::_PD_repoId;
}

POA_Echo::~POA_Echo() {}

