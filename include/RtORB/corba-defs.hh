/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*!
 * @file corba-defs.hh
 * @if jp
 * @brief (TODO)
 * @else
 * @brief (TODO)
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __CORBA_HH__
#define __CORBA_HH__
extern "C"{
#include <RtORB/corba.h>
}

#include "RtORB/corba-object-defs.hh"


namespace CORBA{

  class ORB;

  typedef CORBA_string String;
  typedef CORBA_string_ptr String_ptr;
  typedef CORBA_string_var String_var;

 extern ORB *_root_ORB_; /*!< TODO */

  /*!
   * @class Exception
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */ 
  class Exception
  {
  private:
    CORBA_Environment *ev;  /*!< TODO */
  public:
    char _msg[256];  /*!< TODO */

    const char *msg();
  protected:
    Exception();
    /*!
     * @brief (TODO)
     */
    Exception(const char *type);
    /*!
     * @brief (TODO)
     */
    Exception(const char *type, const char *msg);
    /*!
     * @brief (TODO)
     */
    Exception(const char *type, CORBA_Environment *ev);
  public:
    virtual ~Exception() {}
  };

  /*!
   * @class UserException
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */ 
  class UserException : public Exception
  {
  public:
    UserException();
   /*!
   * @brief (TODO)
   */ 
   UserException(CORBA_Environment *ev);
    virtual ~UserException() {}
  };

  /*!
   * @class SystemException
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */ 
  class SystemException : public Exception
  {
  public:
    uint32_t minor_v;  /*!< TODO */

    /*!
     * @brief (TODO)
     */
    SystemException(CORBA_Environment *ev);
    /*!
     * @brief (TODO)
     */
    SystemException(const char *msg);
    /*!
     * @brief (TODO)
     */
    void set_minor(CORBA_unsigned_long minor_val){ minor_v = minor_val; }
    /*!
     * @brief (TODO)
     */
    CORBA_unsigned_long get_minor(){ return minor_v; }
  };

  /*!
   * @class COMM_FAILURE
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */ 
  class COMM_FAILURE : public Exception {};

  /*!
   * @class INV_OBJREF
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */ 
  class INV_OBJREF : public Exception {};
  /*!
   * @class OBJECT_NOT_EXIST
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */ 
  class OBJECT_NOT_EXIST : public Exception {};
  /*!
   * @class OBJ_ADAPTER
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */ 
  class OBJ_ADAPTER : public Exception {};

  //  Class ORB
  //
  class ORB;
  
  //
  // Class Object
  //
  //class Object
  /*!
   * @class ORB
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */ 
  class ORB : public Object {
  public:
    class InvalidName {};
    typedef ORB * Ptr;


    public:
      CORBA_ORB _impl;        /*!< TODO */
      CORBA_Environment ev;   /*!< TODO */

    public:
      ORB();
      /*!
       * @brief (TODO)
       */
      ORB(CORBA_ORB);
      /*!
       * @brief (TODO)
       */
      ORB(const ORB& other);
      ~ORB();

      /*!
       * @brief (TODO)
       */
      void __set_impl(CORBA_ORB orb);
      /*!
       * @brief (TODO)
       */
      CORBA_boolean __is_nil();

      /*!
       * @brief (TODO)
       */	  
      Object_ptr resolve_initial_references(char *name) throw (Exception);
      /*!
       * @brief (TODO)
       */
      String_ptr object_to_string(Object_ptr obj) throw (Exception);
      /*!
       * @brief (TODO)
       */
      String_ptr object_to_string2(CORBA_Object obj) throw (Exception);
      /*!
       * @brief (TODO)
       */
      void run();
      /*!
       * @brief C++ Wrapper. Shutdown CORBA ORB. only call CORBA_ORB_shutdown()
       */
      void shutdown(boolean flag);
      /*!
       * @brief C++ Wrapper. destroy CORBA ORB. only call CORBA_ORB_destroy()
       */
      void destroy();
      /*!
       * @brief (TODO)
       */
      static Ptr _duplicate(Ptr ptr) { return ptr->_duplicate(); }
      /*!
       * @brief (TODO)
       */
      Ptr _duplicate();
	  
    /*!
     * @brief (TODO)
     */
    static Ptr _nil();

    /*!
     * @brief (TODO)
     */
    Object_ptr string_to_object(const char*) throw (Exception);

    /*!
     * @brief (TODO)
     */
    CORBA_boolean work_pending();
    /*!
     * @brief (TODO)
     */
    void perform_work();

    /*!
     * @brief (TODO)
     */
    inline CORBA_ORB impl() { return _impl; }
    /*!
     * @brief (TODO)
     */
    inline CORBA_ORB in() { return _impl; }
  };
  typedef ORB* ORB_ptr;

  /*!
   * @class ORB_var
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */ 
  class ORB_var
  {
  private:
    ORB * _ptr;  /*!< TODO */
  public:
    /*!
     * @brief (TODO)
     */
    ORB_var() : _ptr(NULL) {};
    /*!
     * @brief (TODO)
     */
    ORB_var(ORB *obj) : _ptr(obj) {};
    ~ORB_var();

    /*!
     * @brief (TODO)
     */    
    ORB_ptr operator->() { return _ptr; }
    /*!
     * @brief (TODO)
     */
    operator ORB_ptr () { return _ptr; }
    /*!
     * @brief (TODO)
     */
    operator Object_ptr () { return Object_ptr(_ptr); }

//    ORB_var & operator=(ORB *ptr);
    /*!
     * @brief (TODO)
     */
    ORB_var & operator=(ORB_ptr ptr);

    /*!
     * @brief (TODO)
     */
    ORB * in(){ return _ptr; }
  };

  /*!
   * @brief (TODO)
   */
  ORB_ptr ORB_init(int& argc, char **argv);

  /*!
   * @brief (TODO)
   */
  CORBA_string string_dup(const char *);
  /*!
   * @brief (TODO)
   */
  CORBA_string string_alloc(CORBA_unsigned_long);
  /*!
   * @brief (TODO)
   */
  CORBA_boolean is_nil(Object_ptr ptr);
/*
  {
    if(!ptr){
      return true;
    }else{
      if (ptr.object()){
        return false;
      }else{
        return true;
      }
    }
//    return !ptr? true: (!ptr.object() ? true : false);
  }
*/

  typedef CORBA_boolean 	Boolean;
  typedef CORBA_double 		Double;
  typedef CORBA_float 		Float;
  typedef CORBA_long 		Long;
  typedef CORBA_char 		Char;
  typedef CORBA_unsigned_long 	ULong;
  typedef CORBA_octet		Octet;
  typedef CORBA_short		Short;
  typedef CORBA_unsigned_short  UShort;

  typedef Double 		Double_var;
  typedef Float			Float_var;
  typedef Boolean		Boolean_var;

  /*!
   * @class ValueType_helper
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */ 
  template <typename T>
  class ValueType_helper
  {
  private:
    T _v;   /*!< TODO */
  public:
    /*!
     * @brief (TODO)
     */
    ValueType_helper(T v) : _v(v) {}
    /*!
     * @brief (TODO)
     */
    operator T () { return _v; }
  public:
    /*!
     * @brief (TODO)
     */
    static T _retnew(T v) { return v; }
  };

#define DECLARE_VALUETYPE_HELPERS(CLS) \
typedef ValueType_helper<CLS> CLS##_helper; \
typedef ValueType_helper<CLS> CLS##_CInArg; \
typedef ValueType_helper<CLS> CLS##_COutArg; \
typedef ValueType_helper<CLS> CLS##_CInOutArg; \
typedef ValueType_helper<CLS> CLS##_CppInArg; \
typedef ValueType_helper<CLS> CLS##_CppOutArg; \
typedef ValueType_helper<CLS> CLS##_CppInOutArg; \

  DECLARE_VALUETYPE_HELPERS(Long);
  DECLARE_VALUETYPE_HELPERS(ULong);
  DECLARE_VALUETYPE_HELPERS(Boolean);
  DECLARE_VALUETYPE_HELPERS(Double);

#undef DECLARE_VALUETYPE_HELPERS

};

/*!
 * @class CORBA_string_helper
 * @if jp
 * @brief (TODO)
 * @else
 * @brief (TODO)
 * @endif
 */
class CORBA_string_helper
{
public:
  /*!
   * @brief (TODO)
   */
  CORBA_string_helper(const char * s) : _s(s) {;}

  /*!
   * @brief (TODO)
   */
  operator const char *() { return _s; }
private:
  const char *_s;  /*!< TODO */
public:
  /*!
   * @brief (TODO)
   */
  static CORBA_string _retnew(CORBA_string s) { return s; }
};

typedef CORBA_string_helper CORBA_string_CInArg;
typedef CORBA_string_helper CORBA_string_COutArg;
typedef CORBA_string_helper CORBA_string_CInOutArg;
typedef CORBA_string_helper CORBA_string_CppInArg;
typedef CORBA_string_helper CORBA_string_CppOutArg;
typedef CORBA_string_helper CORBA_string_CppInOutArg;

/*!
 * @class CORBA_enum_helper
 * @if jp
 * @brief (TODO)
 * @else
 * @brief (TODO)
 * @endif
 */
class CORBA_enum_helper
{
public:
  /*!
   * @brief (TODO)
   */
  CORBA_enum_helper(int32_t n) : _n(n) {}
  /*!
   * @brief (TODO)
   */
  operator int () { return _n; }
private:
  int32_t _n;   /*!< TODO */
public:
  /*!
   * @brief (TODO)
   */
  static int32_t _retnew(int32_t n) { return n; }
};


#define DECLARE_VALUETYPE_HELPERS(CLS) \
typedef CORBA::ValueType_helper<CLS> CLS##_helper; \
typedef CORBA::ValueType_helper<CLS> CLS##_CInArg; \
typedef CORBA::ValueType_helper<CLS> CLS##_COutArg; \
typedef CORBA::ValueType_helper<CLS> CLS##_CInOutArg; \
typedef CORBA::ValueType_helper<CLS> CLS##_CppInArg; \
typedef CORBA::ValueType_helper<CLS> CLS##_CppOutArg; \
typedef CORBA::ValueType_helper<CLS> CLS##_CppInOutArg; \

  DECLARE_VALUETYPE_HELPERS(CORBA_float);

#undef DECLARE_VALUETYPE_HELPERS

#endif
