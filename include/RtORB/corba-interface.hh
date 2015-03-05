/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/* $Id: corba-interface.hh,v 1.38 2008/04/18 13:38:26 yoshi Exp $ */
/*!
 * @file corba-interface.hh
 * @if jp
 * @brief (TODO)
 * @else
 * @brief (TODO)
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __CORBA_INTERFACE_HH__
#define __CORBA_INTERFACE_HH__

#include <assert.h>


namespace CORBA_interface {

  /*!
   * @brief (TODO)
   */
  template <typename T> class T_Ptr;

  //////////////////////////////////////////////////////////////////
  //
  //      T_Helper template class
  //
  //////////////////////////////////////////////////////////////////

  /*!
   * @class T_Helper 
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */ 
  template <class T, class Ptr>
  class T_Helper 
  {
  public:
    /*!
     * @brief (TODO)
     */
    T_Helper(CORBA_Object impl) : _ptr(impl) {}
    /*!
     * @brief (TODO)
     */
    T_Helper(Ptr ptr) : _ptr(ptr) {}

    /*!
     * @brief (TODO)
     */
    operator CORBA_Object () { return _ptr.impl(); }
    /*!
     * @brief (TODO)
     */
    operator Ptr () { return _ptr; }
    /*!
     * @brief (TODO)
     */
    operator CORBA_Object *() {
	    CORBA::Object *obj = _ptr.object();
	    return obj? obj->implptr(): NULL;
    }
      
  private:
    Ptr	_ptr;  /*!< TODO */
  public:

    /*!
     * @brief (TODO)
     */
    static CORBA_Object _retnew(Ptr ptr)
    {
      if (!ptr) { return NULL; }
      CORBA_Object impl = ptr->release();
      ptr.free_(true);
      return impl;
    }

    /*!
     * @brief (TODO)
     */
    static Ptr _retnew(CORBA_Object obj)
    {
      return new T(obj);
    }
  };

  /*!
   * @class ObjHelper 
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  template <class T>
  class ObjHelper
  {
  public:

    /*!
     * @brief (TODO)
     */	  
    static T * duplicate(T *p)
    {
      if (!p) { return NULL; }
      return duplicate(p->_impl, &p->ev);
    }

    /*!
     * @brief (TODO)
     */
    static T * duplicate(CORBA_Object obj)
    {
      if (!obj) { return NULL; }
      return duplicate(obj, NULL);
    }

    /*!
     * @brief (TODO)
     */
    static T * release(CORBA::Object *p) {
      if (p) {
       if(!p->_release()){
         delete p;
       }
      }
      return NULL;
    }

    /*!
     * @brief (TODO)
     */    
    static T * destroy(CORBA::Object *p) {
      if (p) {
      	delete p;
      }
      return NULL;
    }

    /*!
     * @brief (TODO)
     */    
    static T * duplicate(CORBA_Object obj, CORBA_Environment *ev)
    {
      if (!obj) { return NULL; }
       CORBA_Object_duplicate(obj, ev);

      return new T(obj);
    }
  };

  /*!
   * @class CInArg 
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  template <class T>
  class CInArg {
    typedef T_Ptr<T> Ptr;
  public:
    /*!
     * @brief (TODO)
     */ 
    CInArg(CORBA_Object impl) : _obj(NULL) {
 //     _obj = ObjHelper<T>::duplicate(impl);
      _obj = new T(impl);
    }
    ~CInArg() {
      delete _obj;
    }

    /*!
     * @brief (TODO)
     */ 
    operator Ptr () { return Ptr(_obj); }
  private:
    T * _obj;  /*!< TODO */
  };
  
  //////////////////////////////////////////////////////////////////
  //
  //      T_Ptr template class
  //
  //////////////////////////////////////////////////////////////////

  /*!
   * @class Handle 
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  class Handle
  {
  protected:
    CORBA::Object * 	_obj;  /*!< TODO */
  protected:
    Handle() : _obj(NULL) {
    }

    ~Handle() {
#if DEBUG_MALLOC
      std::cerr << "   ~Handle()" << std::endl;
#endif
//       delete  _obj;
    }

    /*!
     * @brief (TODO)
     */ 
    void delete_obj(){
      CORBA_Object_free(_obj->_impl) ;
#if DEBUG_MALLOC
      std::cerr << "   Handle::delete_obj() :" << _obj << std::endl;
#endif
    }

  public:
    /*!
     * @brief (TODO)
     */ 
    CORBA::Object *_object() const { return _obj;}

    /*!
     * @brief (TODO)
     */ 
    CORBA_Object impl() const { return _obj? _obj->impl(): NULL; }

    /*!
     * @brief (TODO)
     */ 
    operator CORBA_Object () { return impl(); }

    /*!
     * @brief (TODO)
     */ 
    operator CORBA::Object_ptr () { return CORBA::Object_ptr(_obj); }

    /*!
     * @brief (TODO)
     */ 
    inline operator CORBA::Object_ptr () const { return CORBA::Object_ptr(_obj); }

    /*!
     * @brief (TODO)
     */ 
    bool operator!() const {
      return _obj == NULL? true: false;
    }
  };
  
  /*!
   * @class T_SeqElemPtr 
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  template <class T> class T_SeqElemPtr;

  /*!
   * @class T_Var 
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */  
  template <class T> class T_Var;

  /*!
   * @class T_Var 
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */  
  template <> class T_Var<CORBA::Object> : public Handle {};

  /*!
   * @class T_Var 
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  template <class T>
  class T_Var : public T_Var<typename T::_super_type>
  {
    friend class T_Ptr<T>;
  private:
    typedef T_Var<typename T::_super_type> Super;
    typedef T_Var<T> Var;
    typedef T_Ptr<T> Ptr;
    typedef ObjHelper<T> Helper;
  private:
    /*!
     * @brief (TODO)
     */ 
    void set(CORBA_Object impl)
    {
      if (!impl) { return; }
      Super::_obj = new T(impl);
    }
  public:
    /*!
     * @brief (TODO)
     */ 
    T_Var() : Super() {}
    /*!
     * @brief (TODO)
     */ 
    T_Var(T *t) : Super(){
      Super::_obj = t;
    }
    /*!
     * @brief (TODO)
     */ 
    T_Var(CORBA_Object impl);

    /*!
     * @brief (TODO)
     */ 
    T_Var(Ptr p);
    /*!
     * @brief (TODO)
     */ 
    T_Var(const T &t);
    /*!
     * @brief (TODO)
     */ 
    T_Var(const Var &);
    virtual ~T_Var();

    /*!
     * @brief (TODO)
     */ 
    Var &operator=(T *t); 
    /*!
     * @brief (TODO)
     */ 
    Var &operator=(CORBA::Object_var);
    /*!
     * @brief (TODO)
     */ 
    Var &operator=(CORBA_Object);
    /*!
     * @brief (TODO)
     */ 
    Var &operator=(const Ptr &);
    /*!
     * @brief (TODO)
     */ 
    Var &operator=(const Var &);

    /*!
     * @brief (TODO)
     */     
    T * _retn() {
      T *tmp = object();
      Super::_obj = 0;
      return tmp;
    }

    /*!
     * @brief (TODO)
     */ 
    T * operator->() {
      return object();
    }

    /*!
     * @brief (TODO)
     */ 
    T *object() const {
      return dynamic_cast<T*>(Super::_obj);
    }

/*
    T *in() const{
      return dynamic_cast<T*>(Super::_obj);
    }

    T& out() const{
      return &(dynamic_cast<T*>(Super::_obj));
    }
*/
    /*!
     * @brief (TODO)
     */ 
    Ptr in() const{
      return Ptr(dynamic_cast<T*>(Super::_obj));
    }

    /*!
     * @brief (TODO)
     */ 
    Ptr inout() {
      return Ptr(dynamic_cast<T*>(Super::_obj));
    }

  };


  //////////////////////////////////////////////////////////////////
  //
  //      T_Ptr template class
  //
  //////////////////////////////////////////////////////////////////
  /*!
   * @class PtrHandle 
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  class PtrHandle : public Handle
  {
  private:
    typedef Handle Super;
  protected:
    bool 		_release;  /*!< TODO */
    /*!
     * @brief (TODO)
     */
    PtrHandle() : Super(), _release(false) {
    }
    ~PtrHandle() {
#if DEBUG_MALLOC
      std::cerr << "   ~PtrHandle() :" << _obj << std::endl;
#endif
    }
    /*!
     * @brief (TODO)
     */
    void delete_obj(){
     // delete  _obj;
//      std::cerr << "   ~delete_obj() :" << _obj << std::endl;
      Super::delete_obj();
    }
  };

  /*!
   * @class T_Ptr 
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  template <typename T> class T_Ptr;
  /*!
   * @class T_Ptr 
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  template <> class T_Ptr<CORBA::Object> : public PtrHandle {};

  /*!
   * @class T_Ptr 
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  template <typename T>
  class T_Ptr : public T_Ptr<typename T::_super_type>
  {
    friend class T_Var<T>;
  private:
    typedef T_Ptr<typename T::_super_type> Super;
    typedef T_Ptr<T> Ptr;
    typedef T_Var<T> Var;
  private:
    /*!
     * @brief (TODO)
     */
    void set(CORBA_Object impl)
    {
      if (!impl) { return; }
      T * obj = new T(impl);
      obj->duplicate();
      Super::_obj = obj;
      Super::_release = true;
    }
  private:
  public:

    /*!
     * @brief (TODO)
     */
    T_Ptr() : Super() { }
    /*!
     * @brief (TODO)
     */
    T_Ptr(const Ptr &o) : Super()
    {
      if (o.object() != NULL){
//        Super::_obj = (o.object())->duplicate();
        Super::_obj = o.object();
     }
    }
    
    /*!
     * @brief (TODO)
     */
    T_Ptr(T * t) : Super()
    {
      Super::_obj = t;
    }

    /*!
     * @brief (TODO)
     */
    T_Ptr(T &t) : Super() {
      Super::_obj = t;
    }

    /*!
     * @brief (TODO)
     */
    T_Ptr(CORBA_Object impl) : Super() {
      set(impl);
    }

    /*!
     * @brief (TODO)
     */
    T_Ptr(CORBA::Object_ptr ptr) {
//      Super::_obj = (ptr.operator->())->duplicate();
      Super::_obj = ptr.operator->();
    }

    /*!
     * @brief (TODO)
     */
    T_Ptr(const Var &o)
    {
      if (o._obj != NULL){
//        Super::_obj = (o._obj)->duplicate();
        Super::_obj = o._obj;
     }
    }

    /*!
     * @brief (TODO)
     */
    virtual ~T_Ptr() {
#if DEBUG_MALLOC
      std::cerr << "   ~T_Ptr()" << "   _obj:" << Super::_obj << std::endl;
#endif
      if (Super::_release) {
//	delete Super::_obj;
	delete_obj();
	Super::_obj = NULL;
	Super::_release = false;
      }

    }

    /*!
     * @brief (TODO)
     */
    void delete_obj(){
#if DEBUG_MALLOC
      std::cerr << "   Super::delete_obj() :"  << std::endl;
#endif
      Super::delete_obj();
    }

    /*!
     * @brief (TODO)
     */	  
    Ptr & operator=(T *t);
    /*!
     * @brief (TODO)
     */
    Ptr & operator=(const T &t);
    /*!
     * @brief (TODO)
     */
    Ptr & operator=(const Ptr &o);
    /*!
     * @brief (TODO)
     */
    Ptr & operator=(const Var &);

    /*!
     * @brief (TODO)
     */
    Ptr & operator=(CORBA_Object impl) {
      set(impl);
      return *this;
    }

    /*!
     * @brief (TODO)
     */
    T * operator->() {
      return object();
    }

    /*!
     * @brief (TODO)
     */    
    T * operator->() const {
      return object();
    }

    /*!
     * @brief (TODO)
     */
    operator T * () { return object(); }
    /*!
     * @brief (TODO)
     */
    operator T & () { return *Super::_obj; }

    /*!
     * @brief (TODO)
     */
    T *object() const {
      return dynamic_cast<T*>(Super::_obj);
    }

    /*!
     * @brief (TODO)
     */
    T *in() {
      return dynamic_cast<T*>(Super::_obj);
    }

    /*!
     * @brief (TODO)
     */
    void free_(bool force)
    {
      if (Super::_obj) {
        if(!Super::_obj->_release()){
          delete Super::_obj;
        }
      }
      Super::_obj = NULL;
      Super::_release = false;
    }
  };


  //////////////////////////////////////////////////////////////////
  //
  //      T_SeqElemPtr member implementation
  //
  //////////////////////////////////////////////////////////////////

  template <class T>
  class T_SeqElemPtr
  {
    typedef T_SeqElemPtr<T> This;
    typedef T_Ptr<T> Ptr;
  private:
    CORBA_Object &_impl;
    T *	_obj;
  private:
    T_SeqElemPtr() {}
  public:
    T_SeqElemPtr(CORBA_Object &impl) : _impl(impl), _obj(NULL) {}
    T_SeqElemPtr(const This &o) : _impl(o._impl), _obj(NULL) {}

    virtual ~T_SeqElemPtr() {
      if (_obj) {
	delete _obj; _obj = NULL;
      }
    }

    T * operator->() { return object(); }

    operator Ptr () { return Ptr(_impl); }
    operator CORBA::Object_ptr () { return CORBA::Object_ptr(_impl); }

    //
    // Copy operators 
    //
    //  Call free_() every time copy operator is called!!
    //
    This & operator=(CORBA_Object impl) {
      free_();
      _impl = impl;
      return *this;
    }
    
    This & operator=(Ptr ptr)
    {
      free_();
      _impl = ptr.impl();
      return *this;
    }
    This & operator=(const This &o)
    {
      free_();
      _impl = o._impl;
      return *this;
    }
    This & operator=(CORBA::Object_ptr ptr)
    {
      free_();
      _impl = ptr.impl();
      return *this;
    }
  public:
    T * object() {
      if (!_obj) {
	_obj = new T(_impl);
      }
      return _obj;
    }

    T * in() { return object(); }
  private:
    void free_()
    {
      if (_obj) {
	delete _obj; _obj = NULL;
      }
    }
  private:
    This &operator=(T *) {}
  };
  

  //////////////////////////////////////////////////////////////////
  //
  //      T_Var member implementation
  //
  //////////////////////////////////////////////////////////////////

  template <class T> T_Var<T>::T_Var(Ptr p) : Super() {
    Super::_obj = Helper::duplicate(p.object());
  }

  template <class T> T_Var<T>::T_Var(const Var &o) : Super()
  {
    Super::_obj = Helper::duplicate(o.object());
  }

  template <class T> T_Var<T>::T_Var(CORBA_Object impl) : Super() {
    Super::_obj = Helper::duplicate(impl);
  }

  template <class T> T_Var<T>::~T_Var() {
    Helper::destroy(Super::_obj);
    Super::_obj = NULL;
  }

  template <class T> T_Var<T> & T_Var<T>::operator=(T *t) {
    Helper::release(Super::_obj);
//    Super::_obj = Helper::duplicate(t);
    Super::_obj = t;
    return *this;
  }

  template <class T> T_Var<T> & T_Var<T>::operator=(CORBA_Object impl) {
    Helper::release(Super::_obj);
    Super::_obj = Helper::duplicate(impl);
    return *this;
  }
  
  template <class T> T_Var<T> & T_Var<T>::operator=(const Var &var) {
    Helper::release(Super::_obj);
    Super::_obj = Helper::duplicate(var.object());
    return *this;
  }

  template <class T> T_Var<T> & T_Var<T>::operator=(const Ptr &p) {
    Helper::release(Super::_obj);
    Super::_obj = p.object();
    return *this;
  }
  
  //////////////////////////////////////////////////////////////////
  //
  //      T_Ptr member implementation
  //
  //////////////////////////////////////////////////////////////////

  template <class T> T_Ptr<T> & T_Ptr<T>::operator=(T *t) {
    free_(false);
    Super::_obj = t;
    return *this;
  }

  template <class T> T_Ptr<T> & T_Ptr<T>::operator=(const T_Ptr<T> &o)
  {
    free_(false);
    CORBA_Object impl = o.impl();
    set(impl);
    return *this;
  }

  template <class T> T_Ptr<T> & T_Ptr<T>::operator=(const T_Var<T> &v)
  {
    free_(false);
    Super::_obj = (v._obj)->duplicate();
    return *this;
  }

  /*!
   * @class Utils 
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  template <class T>
  class Utils
  {
    typedef T_Ptr<T> Ptr;
  public:
    static Ptr duplicate(Ptr ptr) {
      if (!ptr) { return (T*) NULL; }
      CORBA_Object impl = ptr.impl();
      return new T(impl);
    }
  };
}

#endif /* __CORBA_INTERFACE_HH__ */
