/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/* $Id: corba-seq.hh,v 1.48 2008/06/06 08:01:40 yoshi Exp $ */ 
/*!
 * @file corba-seq.hh
 * @if jp
 * @brief (TODO)
 * @else
 * @brief (TODO)
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __CORBA_SEQ_HH__
#define __CORBA_SEQ_HH__

#include <RtORB/corba-defs.hh>

namespace CORBA_sequence {


  /*
  template <class T>
  class T_ElemPtrHelper
  {
  public:
    static T * nil() { return 0; }
    static T * duplicate(T *t) {
      return new T(t);
    }
    static void release(T *p) { delete p; }
  };
  */

  /*!
   * @class T_ElemRefHelper
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */ 
  template <class T>
  class T_ElemRefHelper
  {
  public:
    static void nil(T &t) {
      //t = T();
      //t.T::T();
    }
    static T & duplicate(T & t) { return t; }
    static void release(T &t) {
//      fprintf(stderr, "class T_ElemRefHelper buffer=%x \n",&t);
//      delete &t;
    }
  };

  /*!
   * @class T_ElemCOHelper
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */ 
  class T_ElemCOHelper
  {
  public:
    /*!
     * @brief (TODO)
     */
    static void nil(CORBA_Object &obj)
    {
      obj = NULL;
    }
    /*!
     * @brief (TODO)
     */
    static CORBA_Object duplicate(CORBA_Object obj) {
      if (obj) {
	CORBA_Object_duplicate(obj, NULL);
      }
      return obj;
    }
    /*!
     * @brief (TODO)
     */
    static void release(CORBA_Object obj) {
      if (obj) {
	CORBA_Object_release(obj, NULL);
      }
    }
  };
  
  template <class T> class T_Ptr;
  template <class T> class T_SeqOut;

  /*!
   * @class T_Seq
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  template <class CType, class Elem, class ElemHelper>
  class T_Seq
  {
    typedef T_Seq<CType, Elem, ElemHelper> Seq;
  protected:
    /*!
     * @brief (TODO)
     */
    T_Seq(int n) : _impl(NULL), _tc( CType::_type_code() ), _release(true) 
    {
      alloc();
      length((CORBA::ULong)n);
    }
    /*!
     * @brief (TODO)
     */
    T_Seq(bool release) : _impl(NULL), _tc( CType::_type_code() ), _release(release)
    {
      alloc();
    }

    /*!
     * @brief (TODO)
     */    
    T_Seq(CType *impl, bool release) : _impl(impl), _tc( CType::_type_code() ), _release(release) {}
    /*!
     * @brief (TODO)
     */
    T_Seq(const CType *impl) : _impl( const_cast<CType*>(impl) ), _tc( CType::_type_code() ), _release(false) {}
    /*!
     * @brief (TODO)
     */
    T_Seq(const CType &o) : _impl(NULL), _tc ( CType::_type_code() ), _release(true) 
    {
      alloc();
      copy(o, *this->_impl);
    }
    /*!
     * @brief (TODO)
     */
    T_Seq(const Seq &o) : _impl(NULL), _tc ( CType::_type_code() ), _release(true)
    {
      alloc();
      copy(*o._impl, *this->_impl);
    }
      
    /*!
     * @brief (TODO)
     */
    void free_()
    {
      if (_release && _impl) {
	if (_impl->_length > 0) {
	  for (CORBA::ULong i=0; i<_impl->_length; i++) {
            RtORB_free_by_typecode_cpp(_tc->member_type[0], (void *)&_impl->_buffer[i], 0);
/*
	    ElemHelper::release(_impl->_buffer[i]);
*/
	  }
	  RtORB__free(_impl->_buffer, "~T_Seq(buffer)");
	}
	RtORB__free(_impl, "~T_Seq");
	_impl = NULL;
      }
    }
  public:
    virtual ~T_Seq() {
      free_();
    }

    /*!
     * @brief (TODO)
     */
    CORBA::ULong length() const {
      return _impl != NULL? _impl->_length:0;
    }

    /*!
     * @brief (TODO)
     */
    CORBA::ULong length(CORBA::ULong n)
    {
      return length(this->_impl, n);
    }

    /*!
     * @brief (TODO)
     */
    void copyTo(CType &to) const
    {

      if (!_impl) {
	return;
      }
      copy(*_impl, to);
    }
/*
    void setTo(CType &to) const
    {
      if (!_impl) {
	return;
      }

      set_val(*_impl, to);
    }
*/
  public:
    /*!
     * @brief (TODO)
     */
    static void copy(const CType &from, CType &to) {
      CORBA::ULong n = from.length();
      to.length(n);
      for (CORBA::ULong i=0; i<n; i++) {
	to._buffer[i] = ElemHelper::duplicate(from._buffer[i]);
      }
    }
/*
    static void set_val(const CType &from, CType &to) {
      to._length=from._length;
      to._maximum=from._maximum;
      to._buffer = from._buffer;
    }
*/
    /*!
     * @brief (TODO)
     */
    static CORBA::ULong length(CType *impl, CORBA::ULong n)
    {
      CORBA::ULong i;
      if (n <= 0) { return 0; }
      
      if (impl->_buffer == NULL) {
	Elem *newbuf= (Elem *)RtORB_calloc(sizeof(Elem), n,  "CORBA_sequenct::T_Seq::length");
	for (i=0; i<n; i++) {
	  ElemHelper::nil(newbuf[i]);
	}
	impl->_buffer = newbuf;
	impl->_maximum = impl->_length = n;
      } else if (n > impl->_length) {

	size_t oldsize = impl->_length * sizeof(Elem);
	size_t newsize = n * sizeof(Elem);
	Elem * newbuf = (Elem*)RtORB_realloc(impl->_buffer, newsize, "CORBA_sequenct::T_Seq::length");
	memset(newbuf + impl->_length, 0x00, newsize - oldsize);
	for (i=impl->_length; i<n; i++) {
	  ElemHelper::nil(newbuf[i]);
	}
	impl->_buffer = newbuf;
	impl->_maximum = impl->_length = n;
      }
      return n;
    }

  protected:
    /*!
     * @brief (TODO)
     */
    void alloc()
    {
//      if(_impl) return;
      _impl = (CType *)RtORB_alloc(sizeof(CType)*1, "T_Seq::alloc");
      _impl->_length = _impl->_maximum = 0;
      _impl->_buffer = NULL;
    }

    
  public:
    /*!
     * @brief (TODO)
     */
    CType * _retn()  {
       CType *tmp = _impl;
       _impl = NULL;
       return tmp; 
    }
    /*!
     * @brief (TODO)
     */
    CType * cobj() const { return _impl; }
    /*!
     * @brief (TODO)
     */
    CType ** cptr() { return & _impl; }
    /*!
     * @brief (TODO)
     */
    CType * release() {
      CType *tmp = _impl;
      _impl = NULL;
      return tmp;
    }

  protected:
    CType *_impl;         /*!< TODO */
    CORBA_TypeCode _tc;   /*!< TODO */
    bool _release;       /*!< TODO */
  };

  template <class CType, class CppType>	 class T_CInArg;
  template <class CType, class CppType>	 class T_COutArg;
  template <class CType, class CppType>	 class T_CInOutArg;
  template <class CType, class CppType>	 class T_CppInArg;
  template <class CType, class CppType>	 class T_CppOutArg;
  template <class CType, class CppType>	 class T_CppInOutArg;

  /*!
   * @class T_RefSeq
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  template <class CType_, class Elem, class ElemHelper>
  class T_RefSeq : public T_Seq<CType_, Elem, ElemHelper>
  {
  public:
    typedef T_RefSeq<CType_, Elem, ElemHelper> Seq;
    typedef T_Seq<CType_, Elem, ElemHelper> Super;
    typedef CType_ CType;
    typedef T_Ptr<Seq> Ptr;
    typedef Elem & ElemRef;
  public:
    typedef T_CInArg<CType_, Seq> CInArg;
    typedef T_COutArg<CType_, Seq> COutArg;
    typedef T_CInOutArg<CType_, Seq> CInOutArg;
    typedef T_CppInArg<CType_, Seq> CppInArg;
    typedef T_CppOutArg<CType_, Seq> CppOutArg;
    typedef T_CppInOutArg<CType_, Seq> CppInOutArg;
  private:
  public:
    /*!
     * @brief (TODO)
     */
    T_RefSeq(int n) : Super(n) {}
    /*!
     * @brief (TODO)
     */
    T_RefSeq(CORBA::ULong n) : Super((int)n) {}
    /*!
     * @brief (TODO)
     */
    T_RefSeq(bool release) : Super(release) {}
    /*!
     * @brief (TODO)
     */
    T_RefSeq() : Super(0) {}
    /*!
     * @brief (TODO)
     */
    T_RefSeq(CType *impl, bool release = true) : Super(impl, release) {}
    /*!
     * @brief (TODO)
     */
    T_RefSeq(const CType *impl) : Super((const CType *) impl) {}
    /*!
     * @brief (TODO)
     */
    T_RefSeq(const Seq &o) : Super((int)o.length()) 
    {
      o.copyTo(*this->_impl);
    }
    /*!
     * @brief (TODO)
     */
    T_RefSeq(const CType &o) : Super(o)
    {

    }

    ~T_RefSeq() {
    }
    /*!
     * @brief (TODO)
     */    
    Elem & operator[] (int n)
    {
      return element(this->_impl, n);
    }
#if defined(Cygwin) && ( __GNUC__ < 4)
    /*!
     * @brief (TODO)
     */
    Elem & operator[] (CORBA::Long n)
    {
      return element(this->_impl, (int)n);
    }
#endif
    /*!
     * @brief (TODO)
     */    
    Elem & operator[] (CORBA::ULong n)
    {
      return element(this->_impl, (int)n);
    }
    /*!
     * @brief (TODO)
     */
    Elem & operator[] (CORBA::ULong n) const
    {
      return element(this->_impl, (int)n);
    }
    /*!
     * @brief (TODO)
     */
    Seq & operator=(const Seq &o)
    {
      o.copyTo(*this->_impl);
      return *this;
    }
    /*!
     * @brief (TODO)
     */
#if defined(Cygwin) && ( __GNUC__ < 4)
    Elem & elementAt(int n){
      CORBA_TypeCode tc = CType::_type_code();
//      int swap = tc->size / sizeof(void*);
      int swap = size_of_typecode(tc,F_DEMARSHAL) / sizeof(void*);
      Elem &t = this->_impl->_buffer[n * swap];
    }
#endif
    /*!
     * @brief (TODO)
     */
    static Elem & element(const CType *impl, int n)
    {
      Elem &t = impl->_buffer[n];
      return t;
    }
    /*!
     * @brief (TODO)
     */
    static Elem & element(CType *impl, int n)
    {
//      CORBA_TypeCode tc = CType::_type_code();
//      int swap = tc->size / sizeof(void*);
//      Elem &t = impl->_buffer[n * swap];
      Elem &t = impl->_buffer[n];
      return t;
    }
  };

  /*!
   * @class T_PtrSeq
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  template <class CType_, class Elem, class ElemPtr, class ElemHelper>
  class T_PtrSeq : public T_Seq<CType_, Elem, ElemHelper>
  {
  public:
    typedef T_PtrSeq<CType_, Elem, ElemPtr, ElemHelper> Seq;
    typedef T_Seq<CType_, Elem, ElemHelper> Super;
    typedef CType_ CType;
    typedef T_Ptr<Seq> Ptr;
    
    typedef ElemPtr ElemRef;
  public:
    /*!
     * @brief (TODO)
     */
    T_PtrSeq(int n) : Super(n) {}
    /*!
     * @brief (TODO)
     */
    T_PtrSeq(CORBA::ULong n) : Super((int)n) {}
    /*!
     * @brief (TODO)
     */
    T_PtrSeq(bool release = true) : Super(release) {}
    /*!
     * @brief (TODO)
     */
    T_PtrSeq(CType *impl, bool release=true) : Super(impl, release) {}
    /*!
     * @brief (TODO)
     */
    T_PtrSeq(const CType *impl) : Super(impl) {}
    /*!
     * @brief (TODO)
     */
    T_PtrSeq(const CType &impl) : Super(impl) {}
    /*!
     * @brief (TODO)
     */
    T_PtrSeq(const Seq &o) : Super(o) {}

#if defined(Cygwin) && ( __GNUC__ < 4)
    /*!
     * @brief (TODO)
     */
    ElemPtr operator[] (int n)
    {
      return element(this->_impl, n);
    }
#endif
    /*!
     * @brief (TODO)
     */    
    ElemPtr operator[] (CORBA::Long n)
    {
      return element(this->_impl, (int)n);
    }
    /*!
     * @brief (TODO)
     */    
    ElemPtr operator[] (CORBA::ULong n)
    {
      return element(this->_impl, (int)n);
    }
    /*!
     * @brief (TODO)
     */
    ElemPtr operator[] (CORBA::ULong n) const
    {
      return element(this->_impl, (int)n);
    }
    /*!
     * @brief (TODO)
     */
    Seq & operator=(const Seq &o)
    {
      o.copyTo(*this->_impl);
      return *this;
    }
  public:
    /*!
     * @brief (TODO)
     */
    static ElemPtr element(const CType *impl, int i)
    {
      return ElemPtr(impl->_buffer[i]);
    }
    /*!
     * @brief (TODO)
     */
    static ElemPtr element(CType *impl, int i)
    {
      return ElemPtr(impl->_buffer[i]);
    }
  };

  /*!
   * @class T_PtrVarBase
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  template <class T, bool RELEASE>
  class T_PtrVarBase
  {
    friend class T_SeqOut<T>;
  public:
    typedef typename T::ElemRef ElemRef;
    typedef T_PtrVarBase<T, RELEASE> This;
  public:
    /*!
     * @brief (TODO)
     */
    T_PtrVarBase() : _obj(NULL), _release(RELEASE) {}
    /*!
     * @brief (TODO)
     */
    T_PtrVarBase(T *p) : _obj(p), _release(RELEASE) {}
    /*!
     * @brief (TODO)
     */
    T_PtrVarBase(const T &o) : _obj(NULL), _release(RELEASE)
    {
      _obj = new T(o);
    } 
    /*!
     * @brief (TODO)
     */
    virtual ~T_PtrVarBase() {
      release();
    }

#if defined(Cygwin) && ( __GNUC__ < 4)
    /*!
     * @brief (TODO)
     */      
    ElemRef operator[] (int n)
    {
      return _obj->operator[](n);
    }
#endif
    /*!
     * @brief (TODO)
     */    
    ElemRef operator[] (CORBA::Long n)
    {
      return _obj->operator[](n);

    }
    /*!
     * @brief (TODO)
     */    
    ElemRef operator[] (CORBA::ULong n)
    {
      return _obj->operator[](n);
    }
    /*!
     * @brief (TODO)
     */      
    ElemRef operator[] (CORBA::ULong n) const
    {
      return _obj->operator[](n);
    }
    /*!
     * @brief (TODO)
     */
    CORBA::ULong length(CORBA::ULong n) {
      if (!_obj) { return 0; }
      return _obj->length(n);
    }
    /*!
     * @brief (TODO)
     */
    CORBA::ULong length() const
    {
      return _obj? _obj->length(): 0;
    }
    /*!
     * @brief (TODO)
     */
    T & operator*() { return *_obj; }
    /*!
     * @brief (TODO)
     */
    T * operator->() { return _obj; }
    /*!
     * @brief (TODO)
     */
    operator T &() { return *_obj; }
    /*!
     * @brief (TODO)
     */
    operator T *() { return _obj; }

  public:
    /*!
     * @brief (TODO)
     */
    T * _retn() {
      T *tmp = _obj;
      _obj = 0;
      return tmp;
    }

  public:
    /*!
     * @brief (TODO)
     */
    T & inout() { return *_obj; }
    /*!
     * @brief (TODO)
     */
    T * in() const { return _obj; }

  protected:
    T * _obj;          /*!< TODO */
    bool _release;    /*!< TODO */
  protected:
    /*!
     * @brief (TODO)
     */
    void release() {
      if (_release && _obj != NULL) {
	delete _obj; _obj = NULL;
      }
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
  template <class T>
  class T_Ptr : public T_PtrVarBase<T, false>
  {
    typedef T_PtrVarBase<T, false> Super;
    typedef T_Ptr<T> Ptr;
    typedef typename T::CType CType;
  public:
    /*!
     * @brief (TODO)
     */
    T_Ptr(CType &o) : Super(new T(const_cast<CType*>(&o), false))
    {
      Super::_release = true;
    }
    /*!
     * @brief (TODO)
     */
    T_Ptr(const T &o) : Super(o) {}
    /*!
     * @brief (TODO)
     */
    T_Ptr(CType *impl) : Super(new T(impl, false)) {}
    /*!
     * @brief (TODO)
     */	  
    T_Ptr(T* t) : Super(t) {}
    /*!
     * @brief (TODO)
     */
    T_Ptr(const Ptr &o) {
      Super::_obj = o._obj;
      Super::_release = false;
    }
  };

  /*!
   * @class T_Var
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  template <class T>
  class T_Var : public T_PtrVarBase<T, true>
  {
    typedef T_PtrVarBase<T, true> Super;
    typedef T_Var<T> This;
  public:
    /*!
     * @brief (TODO)
     */
    T_Var() : Super() {}
    /*!
     * @brief (TODO)
     */
    T_Var(T * p) : Super(p) {}
    /*!
     * @brief (TODO)
     */
    T_Var(const T &o) : Super(o) {}

    /*!
     * @brief (TODO)
     */
    This &operator=(T *p)
    {
      Super::release();
      Super::_obj = p;
      return *this;
    }
    /*!
     * @brief (TODO)
     */
    This &operator=(const T &o)
    {
      Super::release();
      Super::_obj = new T(o);
      return *this;
    }
    /*!
     * @brief (TODO)
     */
    T * &out(){
      return Super::_obj;
    }
    /*!
     * @brief (TODO)
     */
    T &inout(){
      return *Super::_obj;
    }
  };

  /*!
   * @class T_SeqOut
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  template <class T>
  class T_SeqOut
  {
    typedef typename T::CType CType;

    typedef T_SeqOut<T> This;
    typedef T_Var<T> Var;
    typedef T_Ptr<T> Ptr;
  private:
    T * & _ref;   /*!< TODO */
  public:
    /*!
     * @brief (TODO)
     */
    T_SeqOut(T * &ref) : _ref(ref)
    {
      //_ref = NULL;
    };
    /*!
     * @brief (TODO)
     */    
    T_SeqOut(Var & var) : _ref(var._obj)
    {
      //_ref = NULL;
    };
    /*!
     * @brief (TODO)
     */
    This & operator=(T *p)
    {
      _ref = p;
      return *this;
    }
    /*!
     * @brief (TODO)
     */
    operator CType **() {
      if (!_ref) {
	_ref = new T(true);
      }
      return _ref->cptr();
    }
    /*!
     * @brief (TODO)
     */    
    Ptr ptr() {
      return Ptr(_ref);
    }
    /*!
     * @brief (TODO)
     */
    Ptr operator->() { return Ptr(_ref); }
    
  };

  /*!
   * @class T_CInArg
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  template <class CType, class CppType>	
  class T_CInArg
  {
  private:
    CppType *_obj;  /*!< TODO */
  public:
    /*!
     * @brief (TODO)
     */
    T_CInArg(CType *p) : _obj(NULL) {
      _obj = new CppType(p, false);
    }
    ~T_CInArg() {
      delete _obj;
    }
    /*!
     * @brief (TODO)
     */
    operator CppType & ()
    {
      return *_obj;
    }
  };

  /*!
   * @class T_COutArg
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  template <class CType, class CppType>
  class T_COutArg
  {
    typedef T_SeqOut<CppType> SeqOut;
  private:
    CType **_ref;  /*!< TODO */
    CppType *_obj; /*!< TODO */
  public:
    /*!
     * @brief (TODO)
     */
    T_COutArg(CType **ref) : _ref(ref), _obj(NULL)
    {
    }
    ~T_COutArg() {
      if (_obj) {
	*_ref = _obj->release();
	delete _obj;
      }
    }
    /*!
     * @brief (TODO)
     */      
    operator SeqOut ()
    {
      if (!_obj) {
	_obj = new CppType(true);
      }

      return SeqOut(_obj);
    }
  };

  /*!
   * @class T_CInOutArg
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  template <class CType, class CppType>
  class T_CInOutArg
  {
  private:
    CppType *_obj;  /*!< TODO */
  public:
    /*!
     * @brief (TODO)
     */
    T_CInOutArg(CType *impl) {
      _obj = new CppType(impl, false);
    }
    /*!
     * @brief (TODO)
     */
    T_CInOutArg() {
      delete _obj;
    }
    /*!
     * @brief (TODO)
     */
    operator CppType &() { return *_obj; }
  };

  /*!
   * @class T_CppInArg
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  template <class CType, class CppType>
  class T_CppInArg {
    typedef T_SeqOut<CppType> Out;
  private:
    const CppType &_obj;  /*!< TODO */
  public:
    /*!
     * @brief (TODO)
     */
    T_CppInArg(const CppType &obj) : _obj(obj) {}
    /*!
     * @brief (TODO)
     */
    operator CType *() { return _obj.cobj(); }
  };

  /*!
   * @class T_CppOutArg
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  template <class CType, class CppType>
  class T_CppOutArg {
    typedef T_SeqOut<CppType> Out;
  private:
    Out &m_out;  /*!< TODO */
  public:
    /*!
     * @brief (TODO)
     */
    T_CppOutArg(Out &out) : m_out(out) {}
    /*!
     * @brief (TODO)
     */
    operator CType **() { return (CType **) m_out; }
  };

  /*!
   * @class T_CppInOutArg
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  template <class CType, class CppType>
  class T_CppInOutArg {
  private:
    CppType &_obj;  /*!< TODO */
  public:
    /*!
     * @brief (TODO)
     */
    T_CppInOutArg(CppType &obj) : _obj(obj) {}
    /*!
     * @brief (TODO)
     */
    operator CType *() { return _obj.cobj(); }
  };
  
  /*!
   * @class T_Helper
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  template <class CType, class CppType>
  class T_Helper {
  public:
    /*!
     * @brief (TODO)
     */
    static CType * _retnew(CppType *p)
    {
      if (!p) { return NULL; }
      return p->cobj();
    }
    /*!
     * @brief (TODO)
     */
    static CppType* _retnew(CType &t)
    {
      CppType *p = new CppType(t);
      return p;
    }
    /*!
     * @brief (TODO)
     */
    static CppType* _retnew(CType *t)
    {
      CppType *p = new CppType(*t);
      return p;
    }
  };
}

#endif // __CORBA_SEQ_HH__
 
