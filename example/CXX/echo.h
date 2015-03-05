/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 * This file was generated by orbit-idl-2 - DO NOT EDIT!
 */

#ifndef echo_H
#define echo_H 1
#define ORBIT_IDL_SERIAL 20
#include <RtORB/corba.h>

#ifdef __cplusplus
#include <RtORB/corba.hh>
extern "C"
{
#endif				/* __cplusplus */

/** typedefs **/
#if !defined(ORBIT_DECL_Echo) && !defined(_Echo_defined)
#define ORBIT_DECL_Echo 1
#define _Echo_defined 1
#define Echo__freekids CORBA_Object__freekids
   typedef CORBA_Object CORBA_Echo;
#ifndef __cplusplus
   typedef CORBA_Object Echo;
#endif				// __cplusplus
   extern CORBA_unsigned_long Echo__classid;
#if !defined(TC_IMPL_TC_Echo_0)
#define TC_IMPL_TC_Echo_0 'e'
#define TC_IMPL_TC_Echo_1 'c'
#define TC_IMPL_TC_Echo_2 'h'
#define TC_IMPL_TC_Echo_3 'o'
#ifdef ORBIT_IDL_C_IMODULE_echo
   static
#else
   extern
#endif
   const struct CORBA_TypeCode_struct TC_Echo_struct;
#define TC_Echo ((CORBA_TypeCode)&TC_Echo_struct)
#endif
#endif

/** POA structures **/
#ifndef _defined_POA_Echo
#define _defined_POA_Echo 1
   typedef struct
   {
      void *_private;
       CORBA_string(*echoString) (PortableServer_Servant _servant,
				  const CORBA_char * mesg,
				  CORBA_Environment * ev);
       CORBA_string(*echoString2) (PortableServer_Servant _servant,
				   const CORBA_char * mesg,
				   CORBA_string * res,
				   CORBA_Environment * ev);
       CORBA_string(*echoString3) (PortableServer_Servant _servant,
				   CORBA_string * mesg,
				   CORBA_Environment * ev);
       CORBA_long(*plus) (PortableServer_Servant _servant, const CORBA_long x,
			  const CORBA_long y, CORBA_Environment * ev);
      void (*plus2) (PortableServer_Servant _servant, const CORBA_long x,
		     const CORBA_long y, CORBA_long * ans,
		     CORBA_Environment * ev);
       CORBA_float(*divide) (PortableServer_Servant _servant,
			     const CORBA_float x, const CORBA_float y,
			     CORBA_Environment * ev);
      void (*divide2) (PortableServer_Servant _servant, const CORBA_float x,
		       const CORBA_float y, CORBA_float * ans,
		       CORBA_Environment * ev);
   } POA_Echo__epv;
   typedef struct
   {
      PortableServer_ServantBase__epv *_base_epv;
      POA_Echo__epv *Echo_epv;
   } POA_Echo__vepv;
   typedef struct
   {
      void *_private;
      POA_Echo__vepv *vepv;
   } POA_Echo;
   extern void POA_Echo__init(PortableServer_Servant servant,
			      CORBA_Environment * ev);
   extern void POA_Echo__fini(PortableServer_Servant servant,
			      CORBA_Environment * ev);
#endif				/* _defined_POA_Echo */

/** skel prototypes **/
   void _RtORB_impl_Echo_echoString(CORBA_Echo _obj, void **result,
				    CORBA_Class_Method * _m, void **argv,
				    CORBA_Environment * ev,
				    CORBA_string(*_impl_echoString)
				    (PortableServer_Servant _servant,
				     const CORBA_char * mesg,
				     CORBA_Environment * ev));
   void _RtORB_impl_Echo_echoString2(CORBA_Echo _obj, void **result,
				     CORBA_Class_Method * _m, void **argv,
				     CORBA_Environment * ev,
				     CORBA_string(*_impl_echoString2)
				     (PortableServer_Servant _servant,
				      const CORBA_char * mesg,
				      CORBA_string * res,
				      CORBA_Environment * ev));
   void _RtORB_impl_Echo_echoString3(CORBA_Echo _obj, void **result,
				     CORBA_Class_Method * _m, void **argv,
				     CORBA_Environment * ev,
				     CORBA_string(*_impl_echoString3)
				     (PortableServer_Servant _servant,
				      CORBA_string * mesg,
				      CORBA_Environment * ev));
   void _RtORB_impl_Echo_plus(CORBA_Echo _obj, void **result,
			      CORBA_Class_Method * _m, void **argv,
			      CORBA_Environment * ev,
			      CORBA_long(*_impl_plus) (PortableServer_Servant
						       _servant,
						       const CORBA_long x,
						       const CORBA_long y,
						       CORBA_Environment *
						       ev));
   void _RtORB_impl_Echo_plus2(CORBA_Echo _obj, void **result,
			       CORBA_Class_Method * _m, void **argv,
			       CORBA_Environment * ev,
			       void (*_impl_plus2) (PortableServer_Servant
						    _servant,
						    const CORBA_long x,
						    const CORBA_long y,
						    CORBA_long * ans,
						    CORBA_Environment * ev));
   void _RtORB_impl_Echo_divide(CORBA_Echo _obj, void **result,
				CORBA_Class_Method * _m, void **argv,
				CORBA_Environment * ev,
				CORBA_float(*_impl_divide)
				(PortableServer_Servant _servant,
				 const CORBA_float x, const CORBA_float y,
				 CORBA_Environment * ev));
   void _RtORB_impl_Echo_divide2(CORBA_Echo _obj, void **result,
				 CORBA_Class_Method * _m, void **argv,
				 CORBA_Environment * ev,
				 void (*_impl_divide2) (PortableServer_Servant
							_servant,
							const CORBA_float x,
							const CORBA_float y,
							CORBA_float * ans,
							CORBA_Environment *
							ev));

/** stub prototypes **/
   CORBA_string Echo_echoString(CORBA_Echo _obj, const CORBA_char * mesg,
				CORBA_Environment * ev);
   CORBA_string Echo_echoString2(CORBA_Echo _obj, const CORBA_char * mesg,
				 CORBA_string * res, CORBA_Environment * ev);
   CORBA_string Echo_echoString3(CORBA_Echo _obj, CORBA_string * mesg,
				 CORBA_Environment * ev);
   CORBA_long Echo_plus(CORBA_Echo _obj, const CORBA_long x,
			const CORBA_long y, CORBA_Environment * ev);
   void Echo_plus2(CORBA_Echo _obj, const CORBA_long x, const CORBA_long y,
		   CORBA_long * ans, CORBA_Environment * ev);
   CORBA_float Echo_divide(CORBA_Echo _obj, const CORBA_float x,
			   const CORBA_float y, CORBA_Environment * ev);
   void Echo_divide2(CORBA_Echo _obj, const CORBA_float x,
		     const CORBA_float y, CORBA_float * ans,
		     CORBA_Environment * ev);
#ifdef ORBIT_IDL_C_IMODULE_echo
   static
#else
   extern
#endif
   CORBA_Class_Impl Echo__imp;
#define Echo_IMETHODS_LEN 7
#ifdef ORBIT_IDL_C_IMODULE_echo
   static
#else
   extern
#endif
   CORBA_Class_Method Echo__methods[Echo_IMETHODS_LEN];

 /** for C++ **/
#ifdef __cplusplus
}

/** C++ class definition(skels)  **/
class Echo_impl:public
   CORBA::Object
{
 public:
   Echo_impl(PortableServer::POA_ptr);
   virtual CORBA_string
   echoString(const CORBA_char * mesg) = 0;
   virtual CORBA_string
   echoString2(const CORBA_char * mesg, CORBA_string * res) = 0;
   virtual CORBA_string
   echoString3(CORBA_string * mesg) = 0;
   virtual CORBA_long
   plus(const CORBA_long x, const CORBA_long y) = 0;
   virtual void
   plus2(const CORBA_long x, const CORBA_long y, CORBA_long * ans) = 0;
   virtual CORBA_float
   divide(const CORBA_float x, const CORBA_float y) = 0;
   virtual void
   divide2(const CORBA_float x, const CORBA_float y, CORBA_float * ans) = 0;
};

/** C++ class definition(stubs)  **/
class
   Echo;

typedef CORBA_interface::T_Ptr<Echo> Echo_ptr;
typedef CORBA_interface::T_Var<Echo> Echo_var;

class
   Echo:
   public
   CORBA::Object
{
 public:
  Echo(CORBA_Object impl) : CORBA::Object(impl) {
  }
   CORBA_string
   echoString(const CORBA_char * mesg);
   CORBA_string
   echoString2(const CORBA_char * mesg, CORBA_string * res);
   CORBA_string
   echoString3(CORBA_string * mesg);
   CORBA_long
   plus(const CORBA_long x, const CORBA_long y);
   void
   plus2(const CORBA_long x, const CORBA_long y, CORBA_long * ans);
   CORBA_float
   divide(const CORBA_float x, const CORBA_float y);
   void
   divide2(const CORBA_float x, const CORBA_float y, CORBA_float * ans);
	
   static Echo_ptr _narrow(CORBA::Object_ptr);

  typedef CORBA::Object _super_type;
  typedef CORBA_interface::T_Ptr<Echo> _ptr_type;
  typedef CORBA_interface::T_Helper<Echo, _ptr_type> _helper_type;
  typedef CORBA_Echo _c_type;
  
};
#endif /* __cplusplus */

#endif
#undef ORBIT_IDL_SERIAL
