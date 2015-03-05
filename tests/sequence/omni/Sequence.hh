// This file is generated by omniidl (C++ backend)- omniORB_4_0. Do not edit.
#ifndef __Sequence_hh__
#define __Sequence_hh__

#ifndef USE_omniORB_logStream
#define USE_omniORB_logStream
#endif

#ifndef __CORBA_H_EXTERNAL_GUARD__
#include <omniORB4/CORBA.h>
#endif

#ifndef  USE_core_stub_in_nt_dll
# define USE_core_stub_in_nt_dll_NOT_DEFINED_Sequence
#endif
#ifndef  USE_dyn_stub_in_nt_dll
# define USE_dyn_stub_in_nt_dll_NOT_DEFINED_Sequence
#endif






#ifdef USE_stub_in_nt_dll
#ifndef USE_core_stub_in_nt_dll
#define USE_core_stub_in_nt_dll
#endif
#ifndef USE_dyn_stub_in_nt_dll
#define USE_dyn_stub_in_nt_dll
#endif
#endif

#ifdef _core_attr
# error "A local CPP macro _core_attr has already been defined."
#else
# ifdef  USE_core_stub_in_nt_dll
#  define _core_attr _OMNIORB_NTDLL_IMPORT
# else
#  define _core_attr
# endif
#endif

#ifdef _dyn_attr
# error "A local CPP macro _dyn_attr has already been defined."
#else
# ifdef  USE_dyn_stub_in_nt_dll
#  define _dyn_attr _OMNIORB_NTDLL_IMPORT
# else
#  define _dyn_attr
# endif
#endif





_CORBA_MODULE SequenceTest

_CORBA_MODULE_BEG

  class LongSeq_var;

  class LongSeq : public _CORBA_Unbounded_Sequence_w_FixSizeElement< CORBA::Long, 4, 4 >  {
  public:
    typedef LongSeq_var _var_type;
    inline LongSeq() {}
    inline LongSeq(const LongSeq& _s)
      : _CORBA_Unbounded_Sequence_w_FixSizeElement< CORBA::Long, 4, 4 > (_s) {}

    inline LongSeq(_CORBA_ULong _max)
      : _CORBA_Unbounded_Sequence_w_FixSizeElement< CORBA::Long, 4, 4 > (_max) {}
    inline LongSeq(_CORBA_ULong _max, _CORBA_ULong _len, CORBA::Long* _val, _CORBA_Boolean _rel=0)
      : _CORBA_Unbounded_Sequence_w_FixSizeElement< CORBA::Long, 4, 4 > (_max, _len, _val, _rel) {}

  

    inline LongSeq& operator = (const LongSeq& _s) {
      _CORBA_Unbounded_Sequence_w_FixSizeElement< CORBA::Long, 4, 4 > ::operator=(_s);
      return *this;
    }
  };

  class LongSeq_out;

  class LongSeq_var {
  public:
    inline LongSeq_var() : _pd_seq(0) {}
    inline LongSeq_var(LongSeq* _s) : _pd_seq(_s) {}
    inline LongSeq_var(const LongSeq_var& _s) {
      if( _s._pd_seq )  _pd_seq = new LongSeq(*_s._pd_seq);
      else              _pd_seq = 0;
    }
    inline ~LongSeq_var() { if( _pd_seq )  delete _pd_seq; }
      
    inline LongSeq_var& operator = (LongSeq* _s) {
      if( _pd_seq )  delete _pd_seq;
      _pd_seq = _s;
      return *this;
    }
    inline LongSeq_var& operator = (const LongSeq_var& _s) {
      if( _s._pd_seq ) {
        if( !_pd_seq )  _pd_seq = new LongSeq;
        *_pd_seq = *_s._pd_seq;
      } else if( _pd_seq ) {
        delete _pd_seq;
        _pd_seq = 0;
      }
      return *this;
    }
    inline CORBA::Long& operator [] (_CORBA_ULong _s) {
      return (*_pd_seq)[_s];
    }

  

    inline LongSeq* operator -> () { return _pd_seq; }
#if defined(__GNUG__)
    inline operator LongSeq& () const { return *_pd_seq; }
#else
    inline operator const LongSeq& () const { return *_pd_seq; }
    inline operator LongSeq& () { return *_pd_seq; }
#endif
      
    inline const LongSeq& in() const { return *_pd_seq; }
    inline LongSeq&       inout()    { return *_pd_seq; }
    inline LongSeq*&      out() {
      if( _pd_seq ) { delete _pd_seq; _pd_seq = 0; }
      return _pd_seq;
    }
    inline LongSeq* _retn() { LongSeq* tmp = _pd_seq; _pd_seq = 0; return tmp; }
      
    friend class LongSeq_out;
    
  private:
    LongSeq* _pd_seq;
  };

  class LongSeq_out {
  public:
    inline LongSeq_out(LongSeq*& _s) : _data(_s) { _data = 0; }
    inline LongSeq_out(LongSeq_var& _s)
      : _data(_s._pd_seq) { _s = (LongSeq*) 0; }
    inline LongSeq_out(const LongSeq_out& _s) : _data(_s._data) {}
    inline LongSeq_out& operator = (const LongSeq_out& _s) {
      _data = _s._data;
      return *this;
    }
    inline LongSeq_out& operator = (LongSeq* _s) {
      _data = _s;
      return *this;
    }
    inline operator LongSeq*&()  { return _data; }
    inline LongSeq*& ptr()       { return _data; }
    inline LongSeq* operator->() { return _data; }

    inline CORBA::Long& operator [] (_CORBA_ULong _i) {
      return (*_data)[_i];
    }

  

    LongSeq*& _data;

  private:
    LongSeq_out();
    LongSeq_out& operator=(const LongSeq_var&);
  };

#ifndef __SequenceTest_mTester__
#define __SequenceTest_mTester__

  class Tester;
  class _objref_Tester;
  class _impl_Tester;
  
  typedef _objref_Tester* Tester_ptr;
  typedef Tester_ptr TesterRef;

  class Tester_Helper {
  public:
    typedef Tester_ptr _ptr_type;

    static _ptr_type _nil();
    static _CORBA_Boolean is_nil(_ptr_type);
    static void release(_ptr_type);
    static void duplicate(_ptr_type);
    static void marshalObjRef(_ptr_type, cdrStream&);
    static _ptr_type unmarshalObjRef(cdrStream&);
  };

  typedef _CORBA_ObjRef_Var<_objref_Tester, Tester_Helper> Tester_var;
  typedef _CORBA_ObjRef_OUT_arg<_objref_Tester,Tester_Helper > Tester_out;

#endif

  class Tester {
  public:
    // Declarations for this interface type.
    typedef Tester_ptr _ptr_type;
    typedef Tester_var _var_type;

    static _ptr_type _duplicate(_ptr_type);
    static _ptr_type _narrow(CORBA::Object_ptr);
    static _ptr_type _unchecked_narrow(CORBA::Object_ptr);
    static _ptr_type _nil();

    static inline void _marshalObjRef(_ptr_type, cdrStream&);

    static inline _ptr_type _unmarshalObjRef(cdrStream& s) {
      omniObjRef* o = omniObjRef::_unMarshal(_PD_repoId,s);
      if (o)
        return (_ptr_type) o->_ptrToObjRef(_PD_repoId);
      else
        return _nil();
    }

    static _core_attr const char* _PD_repoId;

    // Other IDL defined within this scope.
    
  };

  class _objref_Tester :
    public virtual CORBA::Object, public virtual omniObjRef
  {
  public:
    LongSeq* ret();
    void get(LongSeq_out p);
    void set(const LongSeq& p);
    void update(LongSeq& p);

    inline _objref_Tester()  { _PR_setobj(0); }  // nil
    _objref_Tester(omniIOR*, omniIdentity*);

  protected:
    virtual ~_objref_Tester();

    
  private:
    virtual void* _ptrToObjRef(const char*);

    _objref_Tester(const _objref_Tester&);
    _objref_Tester& operator = (const _objref_Tester&);
    // not implemented

    friend class Tester;
  };

  class _pof_Tester : public _OMNI_NS(proxyObjectFactory) {
  public:
    inline _pof_Tester() : _OMNI_NS(proxyObjectFactory)(Tester::_PD_repoId) {}
    virtual ~_pof_Tester();

    virtual omniObjRef* newObjRef(omniIOR*,omniIdentity*);
    virtual _CORBA_Boolean is_a(const char*) const;
  };

  class _impl_Tester :
    public virtual omniServant
  {
  public:
    virtual ~_impl_Tester();

    virtual LongSeq* ret() = 0;
    virtual void get(LongSeq_out p) = 0;
    virtual void set(const LongSeq& p) = 0;
    virtual void update(LongSeq& p) = 0;
    
  public:  // Really protected, workaround for xlC
    virtual _CORBA_Boolean _dispatch(omniCallHandle&);

  private:
    virtual void* _ptrToInterface(const char*);
    virtual const char* _mostDerivedRepoId();
  };


_CORBA_MODULE_END



_CORBA_MODULE POA_SequenceTest
_CORBA_MODULE_BEG

  class Tester :
    public virtual SequenceTest::_impl_Tester,
    public virtual PortableServer::ServantBase
  {
  public:
    virtual ~Tester();

    inline ::SequenceTest::Tester_ptr _this() {
      return (::SequenceTest::Tester_ptr) _do_this(::SequenceTest::Tester::_PD_repoId);
    }
  };

_CORBA_MODULE_END





#undef _core_attr
#undef _dyn_attr



inline void
SequenceTest::Tester::_marshalObjRef(::SequenceTest::Tester_ptr obj, cdrStream& s) {
  omniObjRef::_marshal(obj->_PR_getobj(),s);
}




#ifdef   USE_core_stub_in_nt_dll_NOT_DEFINED_Sequence
# undef  USE_core_stub_in_nt_dll
# undef  USE_core_stub_in_nt_dll_NOT_DEFINED_Sequence
#endif
#ifdef   USE_dyn_stub_in_nt_dll_NOT_DEFINED_Sequence
# undef  USE_dyn_stub_in_nt_dll
# undef  USE_dyn_stub_in_nt_dll_NOT_DEFINED_Sequence
#endif

#endif  // __Sequence_hh__

