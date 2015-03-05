/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/* $Id: corba-object-defs.hh,v 1.27 2008/06/06 08:01:40 yoshi Exp $ */
/*!
 * @file corba-object-defs.hh
 * @if jp
 * @brief (TODO)
 * @else
 * @brief (TODO)
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __CORBA_OBJECT_DEFS_HH__
#define __CORBA_OBJECT_DEFS_HH__

extern "C"{
#include <RtORB/corba-defs.h>
}

#include <RtORB/corba-object-defs.h>

namespace CORBA {

  class Object;
  class Object_var;

  /*!
   * @class Object_ptr
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */ 
  class Object_ptr
  {
  public:
    /*!
     * @brief (TODO)
     */
    void set(CORBA_Object);
  public:
    /*!
     * @brief (TODO)
     */
    Object_ptr() : _obj(NULL) {}
    /*!
     * @brief (TODO)
     */
    Object_ptr(void *);
    /*!
     * @brief (TODO)
     */
    Object_ptr(CORBA_Object impl);
    /*!
     * @brief (TODO)
     */
    Object_ptr(Object *p);
    /*!
     * @brief (TODO)
     */
    Object_ptr(const Object_ptr& ptr);

    virtual ~Object_ptr();

    /*!
     * @brief (TODO)
     */
    bool operator!() {
      return _obj == NULL? true: false;
    }
    /*!
     * @brief (TODO)
     */
    Object * operator->() { return _obj; }

    /*!
     * @brief (TODO)
     */
    operator CORBA_Object () { return impl(); }

    /*!
     * @brief (TODO)
     */	  
    Object *object() { return _obj;}

    /*!
     * @brief (TODO)
     */
    CORBA_Object impl();
  private:
    Object *_obj;  /*!< TODO */
  };

  /*!
   * @class Object
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  class Object
  {
    friend struct CORBA_any; /*!< TODO */
  public:
    typedef Object_ptr _ptr_type;
    typedef Object_var _var_type;
	  
  private:
    int32_t _ref;  /*!< TODO */

  public:
    void *_private;       /*!< TODO */
    CORBA_Environment ev; /*!< TODO */
    CORBA_Object  _impl;  /*!< TODO */

  public:
    Object();
    virtual ~Object();

    /*!
     * @brief (TODO)
     */
    Object(Object * obj);
    /*!
     * @brief (TODO)
     */
    Object(CORBA_Object obj);
    /*!
     * @brief (TODO)
     */
    Object(const Object& other);

    /*!
     * @brief (TODO)
     */
    Object& operator=(const Object &other);

    /*!
     * @brief (TODO)
     */
    Object* duplicate();
    /*!
     * @brief (TODO)
     */
    bool _release();

    /*!
     * @brief (TODO)
     */
    operator Object_ptr () { return Object_ptr(this); }

    /*!
     * @brief (TODO)
     */
    CORBA_boolean _is_equivalent(Object_ptr othor); 
    /*!
     * @brief (TODO)
     */
    static Object_ptr _duplicate(Object_ptr ptr);
    /*!
     * @brief (TODO)
     */
    static Object_var _duplicate(Object_var var);

    /*!
     * @brief (TODO)
     */
    CORBA_Object release();
    // CORBA_ptr _nil() causes the ambiguous type matching of copy operator
    // Use this instead.
    /*!
     * @brief (TODO)
     */
    static CORBA_Object _nil() { return NULL; }
    /*!
     * @brief (TODO)
     */
    CORBA_Object impl() const { return _impl; }
    /*!
     * @brief (TODO)
     */
    CORBA_Object * implptr() { return &_impl; }
    /*!
     * @brief (TODO)
     */
    CORBA_boolean _non_existent();
    
  };


#if 0
#define COD_DUMP(MSG) printf MSG; printf("\n")
#else
#define COD_DUMP(MSG) 
#endif

  /*!
   * @class Object_var
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  class Object_var
  {
    typedef Object_var Var;
  private:
    Object * _obj;  /*!< TODO */
  private:
    /*!
     * @brief (TODO)
     */
    CORBA_Environment * getEnv(CORBA_Object impl);
  private:
    /*!
     * @brief (TODO)
     */
    void set(CORBA_Object impl);
    /*!
     * @brief (TODO)
     */
    void free_();
  public:
    Object_var();
    /*!
     * @brief (TODO)
     */
    Object_var(const Var &o);
    /*!
     * @brief (TODO)
     */
    Object_var(Object *p);
    /*!
     * @brief (TODO)
     */
    Object_var(CORBA_Object impl);
    /*!
     * @brief (TODO)
     */
    Object_var(Object_ptr ptr);
    
    ~Object_var();

    /*!
     * @brief (TODO)
     */
    operator Object_ptr ();

    /*!
     * @brief (TODO)
     */    
    Object * operator->();
    /*!
     * @brief (TODO)
     */
    bool     operator!();
    /*!
     * @brief (TODO)
     */
    Var &    operator=(CORBA_Object impl);
    /*!
     * @brief (TODO)
     */
    Var &    operator=(Object_ptr);

    /*!
     * @brief (TODO)
     */    
    Object * object();
    /*!
     * @brief (TODO)
     */
    Object * in() const {
      std::cerr << "===== Call Object_var.in() "<< _obj << std::endl;
      return _obj;
    }
    /*!
     * @brief (TODO)
     */
    Object * inout() { return _obj;}
    /*!
     * @brief (TODO)
     */
    Object_ptr & out(){
      Object_ptr *_ptr = new Object_ptr(_obj);
      return *_ptr;
    }

    /*!
     * @brief (TODO)
     */
    CORBA_Object impl() { return _obj->impl();}

  };

  /*!
   * @class Object_helper
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  class Object_helper {
  public:
    /*!
     * @brief (TODO)
     */
    static CORBA_Object _retnew(CORBA_Object obj) {
      return obj;
    }
  };

  /*!
   * @class Object_arg
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  class Object_arg {
  public:
    /*!
     * @brief (TODO)
     */
    Object_arg(Object_ptr ptr) : _ptr(ptr) {;}
    /*!
     * @brief (TODO)
     */
    operator CORBA_Object() { return _ptr.impl(); }
    /*!
     * @brief (TODO)
     */
    operator Object_ptr() { return _ptr.impl(); }
  private:
    Object_ptr _ptr;  /*!< TODO */
  };
  typedef Object_arg Object_CInArg;
  typedef Object_arg Object_COutArg;
  typedef Object_arg Object_CInOutArg;
  typedef Object_arg Object_CppInArg;
  typedef Object_arg Object_CppOutArg;
  typedef Object_arg Object_CppInOutArg;
}

  /*!
   * @class CORBA_Narrow
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
template <class T>
class CORBA_Narrow
{
public:
  /*!
   * @brief (TODO)
   */
  static T * narrow(CORBA::Object_ptr ptr) {
    if (!ptr) { return NULL; }
    CORBA_Object impl = ptr.impl();
    return new T(impl);
  }
};

#endif /* __CORBA_OBJECT_DEFS_HH__ */
