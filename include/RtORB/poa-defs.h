/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */

/*

 Simple GIOP Implementation for x86 (Big Endian)
 
*/
/*!
 * @file poa-defs.h
 * @if jp
 * @brief POA定義
 * @else
 * @brief POA Definition
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __POA_DEFS_H__
#define __POA_DEFS_H__

#if USE_THREAD
#include <pthread.h>
#endif
#include <RtORB/array.h>
#include <RtORB/list.h>
#include <RtORB/hashtable.h>
#include <RtORB/typecode.h>

#ifdef __cplusplus
extern "C"
{
#endif 
/*    POA   */
#define POA_HOLDING 	1  /*!< HOLDING Status of POA Manager    (request --> Hold to Queue) */
#define POA_ACTIVE  	2  /*!< ACTIVE Status of POA Manager     (request --> Accept) */
#define POA_DISCARDING 	3  /*!< DISCARDING Status of POA Manager (request --> Destroy) */
#define POA_INACTIVE  	4  /*!< INACTIVE Status of POA Manager   (request --> Deny) */

/* POA_Policy */
/*!
 * @struct PortableServer_POA_Policy_struct
 * @brief POA Policy (Unused)
 * @param type type
 * @param id id
 */
typedef struct PortableServer_POA_Policy_struct{
  uint32_t  type;
  uint32_t  id;
} PortableServer_POA_Policy_struct;

typedef PortableServer_POA_Policy_struct * PortableServer_POA_Policy;

/* POAManagerFactory */
/*!
 * @struct PortableServer_POAManagerFactory_struct
 * @brief POAManagerFactory structure
 * @param orb CORBA ORB
 * @param id POAManager's ID
 * @param poa_mgr POA Manager's array
 */
typedef struct PortableServer_POAManagerFactory_struct{
  struct CORBA_ORB_struct *orb;
  char *id;
  PtrArray *poa_mgr;
} PortableServer_POAManagerFactory_struct;

typedef PortableServer_POAManagerFactory_struct * PortableServer_POAManagerFactory;

/* POAManager */
/*!
 * @struct PortableServer_POAManager_struct
 * @brief POAManager structure
 * @param orb CORBA ORB
 * @param id POA's ID
 * @param poa POA's array
 * @param status POA status (POA_HOLDING/POA_ACTIVE/POA_DISCARDING/POA_INACTIVE)
 */
typedef struct PortableServer_POAManager_struct{
  struct CORBA_ORB_struct *orb;
  char *id;
  PtrArray *poa;

  uint32_t status;
#if USE_THREAD
  pthread_t _thread[FD_SETSIZE];
#endif
} PortableServer_POAManager_struct;

typedef PortableServer_POAManager_struct * PortableServer_POAManager;

/* POA   */
/*!
 * @struct PortableServer_POA_struct
 * @brief PortableServer_POA_struct structure
 * @param orb ORB
 * @param id ORB内での一のID(RootPOAなど)
 * @param _server POAサービスのためのGIOPポート
 * @param mgr_factory POA Manager Factory
 * @param manager POA Manager
 * @param children (TODO)
 * @param servant_mgr (TODO)
 * @param object_map POA内のオブジェクトアダプタ登録用table. 初期値64で生成.
 * @param requests Request Queue from client to Servant
 */
typedef struct PortableServer_POA_struct{
  struct CORBA_ORB_struct *orb;
  char *id;

  struct GIOP_Connection *_server;

  PortableServer_POAManagerFactory mgr_factory;
  PortableServer_POAManager manager;

/*  struct PortableServer_POA_struct *children;   */
  PtrArray *children;
  struct PortableServer_ServantManager_struct *servant_mgr;

/*  PtrArray *servants;   */
  hashtable *object_map;

  PtrList *requests;

/*  uint32_t status;  */

}PortableServer_POA_struct;

typedef PortableServer_POA_struct * PortableServer_POA;

/* ServantManager   */
/*!
 * @struct PortableServer_ServantManager_struct
 * @brief ServantManager structure
 * @param id (TODO)
 * @param poa (TODO)
 * @param status (TODO)
 */
typedef struct PortableServer_ServantManager_struct{
  char *id;

  PortableServer_POA poa;

  uint32_t status;
}PortableServer_ServantManager_struct;

typedef PortableServer_ServantManager_struct * PortableServer_ServantManager;


/* ObjectId   */
/*!
 * @struct PortableServer_ObjectId_struct
 * @brief  PortableServer_ObjectId_struct structure
 * @param _this pointer of CORBA_Object_struct
 */
typedef struct PortableServer_ObjectId_struct{
  
  struct CORBA_Object_struct *_this;

}PortableServer_ObjectId_struct;

typedef PortableServer_ObjectId_struct * PortableServer_ObjectId;

/*   Servant   */
typedef void *PortableServer_Servant;

/*!
 * @struct PortableServer_ServantBase__epv
 * @brief PortableServer_ServantBase__epv structure
 * @param finalize (TODO)
 * @param default_POA (TODO)
 * @param add_ref (TODO)
 * @param remove_ref (TODO)
 * @param get_interface (TODO)
 * @param is_a (TODO)
 * @param non_existent (TODO)
 */
typedef struct {
  void *_private;

  void (*finalize)(PortableServer_Servant , CORBA_Environment *);
  PortableServer_POA (*default_POA)(PortableServer_Servant, CORBA_Environment *);
  void (*add_ref)(PortableServer_Servant, CORBA_Environment *);
  void (*remove_ref)(PortableServer_Servant, CORBA_Environment *);
  CORBA_InterfaceDef *(*get_interface)(PortableServer_Servant, CORBA_Environment *);
  CORBA_boolean (*is_a)(PortableServer_Servant, const char *, CORBA_Environment *);
  CORBA_boolean (*non_existent)(PortableServer_Servant, CORBA_Environment *);
} PortableServer_ServantBase__epv ;

typedef PortableServer_ServantBase__epv *PortableServer_ServantBase__vepv;

/*!
 * @struct PortableServer_ServantBase
 * @brief PortableServer_ServantBase structure
 * @param _private (TODO)
 * @param vepv (TODO)
 */
typedef struct {
  void *_private;
  PortableServer_ServantBase__vepv *vepv;
} PortableServer_ServantBase;

/*!
 * @struct impl_POA_ServantBase
 * @brief impl_POA_ServantBase structure
 * @param servant (TODO)
 * @param poa (TODO)
 * @param _private (TODO)
 */
typedef struct{
  PortableServer_ServantBase servant;
  PortableServer_POA poa;
  void *_private;
}impl_POA_ServantBase;

/*!
 * @struct PortableServer_ClassInfo
 * @brief PortableServer_ClassInfo structure
 * @param dummy (TODO)
 * @param impl_finder (TODO)
 * @param class_name (TODO)
 * @param class_id (TODO)
 * @param class_impl (TODO)
 */
typedef struct {
  void (*dummy)();
/*
  void *(*impl_finder)(void*, char*, CORBA_Class_Method**, void *(**)());
*/
  void *(*impl_finder)();
  const char *class_name;
  uint32_t *class_id;
  CORBA_Class_Impl *class_impl;
}PortableServer_ClassInfo;

/*   RtORB POA Object  */
/*!
 * @struct RtORB_POA_Object
 * @brief RtORB_POA_Object structure
 * @param _private (TODO)
 * @param obj (TODO)
 * @param poa (TODO)
 * @param servant (TODO)
 * @param objectId (TODO)
 * @param _reserve (TODO)
 */
typedef struct{

  void *_private;

  CORBA_Object obj;

  PortableServer_POA poa;
  PortableServer_Servant servant;
  PortableServer_ObjectId objectId;

  void *_reserve;
}RtORB_POA_Object;

/*!
 * @brief return POA releated Object (Unused)(T.B.D)
 */
PortableServer_POA PortableServer_POA__narrow(CORBA_Object);

#ifdef __cplusplus
}
#endif 
#endif

