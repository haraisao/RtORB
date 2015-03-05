/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*!
 * @file corba.cc
 * @if jp
 * @brief (TODO)
 * @else
 * @brief (TODO)
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#include <RtORB/corba.hh>
#include <assert.h>

CORBA_boolean CORBA_Object_struct::_is_equivalent(CORBA_Object_struct *other)
{
#if DEBUG
  std::cerr << "#### Call CORBA_Objec_structt::_is_equivalnt " << std::endl;
#endif
  if (!other) { return FALSE; }
  CORBA_Environment ev;
  memset(&ev, 0x00, sizeof(CORBA_Environment));
  return CORBA_Object_is_equivalent(this, other, &ev);
}

CORBA_Object_struct & CORBA_Object_struct::operator=(const CORBA_Object_struct &o)
{
//  CORBA_Object_dup((CORBA_Object_struct *)&o);
#if DEBUG
  std::cerr << "==== operator= ==== " << o.ref << std::endl;
#endif
  return *this;
}

namespace CORBA{

  CORBA_boolean is_nil(Object_ptr ptr)
  {
    if(!ptr){
      return true;
    }else{
        return false;
      if (ptr.object()){
        return false;
      }else{
        return true;
      }
    }
  }

  /* #############################################
   *
   * class CORBA::Object
   *
   * ############################################# */

  Object::Object() : _ref(1){
    _impl = 0;
    memset(&ev, 0, sizeof(CORBA_Environment));
  }

  Object::Object(CORBA_Object obj) : _ref(1){
    _impl = CORBA_Object_dup(obj);
  //  _impl = obj;
    memset(&ev, 0, sizeof(CORBA_Environment));
  }

  Object & Object::operator=(const Object& other)
  {
//    if(this->_impl) CORBA_Object_free(this->_impl);
   // _impl = CORBA_Object_dup(other._impl);
    _impl = other._impl;
    return *this;
  }

  Object* Object::duplicate()
  {
    _ref++;
    return this;
  }

  bool Object::_release()
  {
    if(_ref < 1){
      return false;
    }
    _ref--;
    return true;
  }

  Object::Object(const Object& other)
  {
//    _impl = CORBA_Object_dup(other._impl);
    _impl = other._impl;
    _ref = 1;
    memcpy(&ev, &other.ev, sizeof(other.ev));
  }

  Object::~Object(){
    if(_impl){
     if(CORBA_Object_free(_impl) < 1){
#if DEBUG
        std::cerr << "#### Deconstruct Object " << std::endl;
#endif
	_impl = 0;
     }
    }
  }

  Object_ptr Object::_duplicate(Object_ptr ptr)
  {
    if (!ptr) { return Object_ptr(); }
    CORBA_Object_dup(ptr.impl());
    return ptr;
  }

  Object_var Object::_duplicate(Object_var var)
  {
    if (!var) { return Object_var(); }
    CORBA_Object_dup(var.impl());
    return var;
  }

#if 0
  Boolean Object::_is_equivalent(CORBA_Object other)
  {
#if DEBUG
    std::cerr << "#### Call Object::_is_equivalnt 1 " << std::endl;
#endif
    if (!_impl || !other) { return FALSE; }
    return CORBA_Object_is_equivalent(_impl, other, &ev);
  }
#endif

  Boolean Object::_is_equivalent(Object_ptr other)
  {
#if DEBUG
    std::cerr << "#### Call Object::_is_equivalnt " << std::endl;
#endif
    if (!_impl || !other) { return FALSE; }
    Boolean res =  CORBA_Object_is_equivalent(_impl, other.impl(), &ev);
    other->_release();
    return res;
  }

  CORBA_Object Object::release()
  {
#if DEBUG
//    std::cerr << "#### Call Object::release " << _impl << std::endl;
#endif
    CORBA_Object tmp = _impl;
    _impl = NULL;
    return tmp;
  }

  CORBA_boolean Object::_non_existent()
  {
    CORBA_Environment env;
    CORBA_Object tmp = _impl;
    if(_ref > 0) return FALSE;
    if(!_impl) return TRUE;
    return CORBA_Object_non_existent(_impl, &env);
  }

  /* #############################################
   *
   * class CORBA::Object_var
   *
   * ############################################# */
  Object_var::Object_var() : _obj(NULL)
  {
    COD_DUMP(("in Object_var()"));
  }
    
  Object_var::Object_var(const Var &o) : _obj(NULL) {
    COD_DUMP(("in Object_var(const Object_var &)"));

    /*
      CORBA_Object impl = o._impl;
      if (impl) {
      CORBA_Object_duplicate(impl, getEnv(impl));
      }
    */
  }
    
  Object_var::Object_var(Object *p) : _obj(p)
  {
    COD_DUMP(("in Object_var(Object *)"));


    /*
      if (p) {
      CORBA_Object impl = p->impl();
      CORBA_Object_duplicate(impl, NULL);
      }
    */
  }

  CORBA_Environment * Object_var::getEnv(CORBA_Object impl)
  {
    /*
      if (!impl || !impl->impl_obj) { return NULL; }
      Object * obj = reinterpret_cast<Object*>(impl->impl_obj);
      return &obj->ev;
    */
    return NULL;
  }

  Object_var::Object_var(CORBA_Object impl) : _obj(NULL) {

    COD_DUMP(("in Object_var(CORBA_Object)"));
    /*
      if (_impl) {
      CORBA_Object_duplicate(_impl, getEnv(impl));
      }
    */
    set(impl);
  }

  void Object_var::set(CORBA_Object impl)
  {
    if (impl) {
      _obj = new Object(impl);
    }
  }

  void Object_var::free_()
  {
#if DEBUG
    std::cerr << "#### Call Object_var::free_() " << std::endl;
#endif
    if(_obj){
#if DEBUG
      std::cerr << "#### Call Object_var::free_() _obj=NULL " << _obj <<std::endl;
#endif
      if(!_obj->_release()){
        delete _obj;
      }
      _obj = NULL;
    }
  }

  Object_var::Object_var(Object_ptr ptr) : _obj(ptr.object())
  {
    COD_DUMP(("in Object_var(Object_ptr)"));
    ;
  }
    
  Object_var::~Object_var() {
    free_();
    /*
      if (_impl) {
      CORBA_Object_release(_impl, getEnv(_impl));
      }
    */
  }

  Object_var & Object_var::operator=(CORBA_Object impl) {
    free_();
    set(impl);
    return *this;
  }

  Object_var & Object_var::operator=(Object_ptr ptr)
  {
    free_();
    _obj = (ptr.object())->duplicate();
    return *this;
  }

  Object_var::operator Object_ptr ()
  {
    return Object_ptr(_obj);
  }
    
  Object * Object_var::operator->()
  {
    return _obj;
  }

  bool Object_var::operator!() {
    return _obj == NULL? true: false;
  }

  Object * Object_var::object()
  {
    return _obj;
  }
  /* #############################################
   *
   * class CORBA::Object_ptr
   *
   * ############################################# */
  Object_ptr::Object_ptr(CORBA_Object impl) : _obj(NULL)
  {
#if DEBUG
    std::cerr << "*** Object_prt 1 "<< std::endl;
#endif
    //set(impl);
    if (impl) {
      _obj = new Object(impl);
    }else{
      _obj = NULL;
    }
  }

  Object_ptr::Object_ptr(const Object_ptr& ptr)
  {
#if DEBUG
    std::cerr << "*** Object_prt 2 "<< std::endl;
#endif
    if(!ptr._obj){
    _obj = NULL;
    }else{
    _obj = (ptr._obj)->duplicate();
    }
  }

  Object_ptr::Object_ptr(Object *p)
  {
#if DEBUG
    std::cerr << "*** Object_prt 3 "<< std::endl;
#endif
    if (p) _obj = p->duplicate();
    else _obj = NULL;
  }

  void Object_ptr::set(CORBA_Object impl)
  {
    if (impl) {
      _obj = new Object(impl);
    }else{
      _obj = NULL;
    }
  }


  Object_ptr::Object_ptr(void *p) : _obj(NULL)
  {
#if DEBUG
    std::cerr << "*** Object_prt 4 "<< std::endl;
#endif
    assert(p == NULL);
  }

  CORBA_Object Object_ptr::impl()
  {
    return _obj? _obj->impl(): NULL;
  }

  Object_ptr::~Object_ptr() {
#if DEBUG
    std::cerr << "#### Call ~Object_ptr() " << std::endl;
#endif
    if(_obj){
#if DEBUG
      std::cerr << "#### Call ~Object_ptr() " << _obj << std::endl;
#endif
      if(!_obj->_release()){
#if DEBUG
        std::cerr << "#### Call ~Object_ptr() delete " << _obj << std::endl;
#endif
        delete _obj;
      }
      _obj = NULL;
    }
  }

  /* #############################################
   *
   * functions
   *
   * ############################################# */
  
  CORBA_string string_dup(const char* str)
  {
    if (!str) { return NULL; }
    return RtORB__strdup(str, "CORBA::string_dup");
  }
  
  CORBA_string string_alloc(CORBA_unsigned_long n)
  {
    if (n <= 0) { return NULL; }
    return (CORBA_string)RtORB__alloc(n+1, "CORBA::string_alloc");
  }
};

CORBA_string_var::~CORBA_string_var()
{
  if (_str) {
    RtORB__free(_str, "~CORBA_string_var");
  }
}
  

char * CORBA_string_var::alloc(const char *p) {
  if (p) {
    _str = RtORB__strdup(p, "CORBA_string_var::alloc");
  }
  return _str;
}
