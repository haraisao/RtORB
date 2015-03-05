/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*!
 * @file corba-object-defs.h
 * @if jp
 * @brief CORBA OBJECTの定義
 * @else
 * @brief Definition of CORBA OBJECT
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __CORBA_OBJECT_DEFS__
#define __CORBA_OBJECT_DEFS__

#include <RtORB/corba-defs.h>
#include <RtORB/iop-defs.h>
#include <RtORB/list.h>
#include <RtORB/array.h>
#include <RtORB/hashtable.h>

#ifdef __cplusplus
extern "C"
{
#endif 
/*------------------------------------------------*/
/*!
 * @struct GIOP_Connection
 * @brief GIOP_Connection structure
 * @param sock Socket Descriptor
 * @param activate FALSE:INACTIVE, TRUE:ACTIVE
 * @param port Socket Port number
 * @param hostname Host Name (IP address)
 */
typedef struct GIOP_Connection{
  int32_t sock;
  boolean activate;
  uint16_t port;
  unsigned char *hostname;

#if USE_THREAD
  pthread_t thread;
#endif
} GIOP_Connection;

/*   Object */
/*!
 * @struct CORBA_Object_struct
 * @brief CORBA Object structure
 *
 *        RtORBでは、CORBA_Objectは、ServantへのポインタとIOR等の情報の保持、
 *        Stubとしての機能を管理することにる。
 *        実装側では、Servantとimplがあるはずで、Stub側(クライアン側)では、
 *        URLとGIOPコネクションが最低でも必要となる。
 *        この表現では、非常に冗長になる部分があり、RtORBの内部でメモリーリーク、
 *        または、セグメント化違反になるコードが存在している可能性もある。
 * @param servant Servantへのポインタ
 * @param poa POAへのポインタ(通常はRootPOA)
 * @param orb ORBへのポインタ
 * @param typedId オブジェクトのtypedID(narrow時に使用)
 * @param object_key オブジェクトID(同じORBの中では一意の必要あり)
 * @param _url CORBA＿URL
 * @param num_urls CORBA_URLの数
 * @param _ior_string このオブジェクトに対するIOR(octet sequence?)
 * @param connection クライアント用のGIOPコネクション
 * @param interface CORBA_InterfaceDefはDummyのみ要素があり(Unusedと同じ)
 * @param repository_id リポジトリID
 * @param ref オブジェクトのリファレンスカウンタ
 * @param policy (Unused)
 * @param _policies (Unused)
 * @param impl 実際のオブジェクトへのポインタ
 * @param impl_obj C++オブジェクトへのポインタ
 * @param release (Unused)
 */
typedef struct CORBA_Object_struct{
  void *servant;
  struct PortableServer_POA_struct *poa;
  CORBA_ORB orb;
  unsigned char *typedId;
  unsigned char *object_key;
  CORBA_URL *_url;
  unsigned char num_urls;
/*
  uint32_t _length_ior_string;
*/
  unsigned char *_ior_string;
/*
  GIOP_Connection connection;
*/
  GIOP_Connection *connection;
  CORBA_InterfaceDef *interface;
  uint32_t *repository_id;
  uint32_t ref;
  CORBA_Policy *policy;
  CORBA_PolicyList *_policies;
  void *impl;
  void *impl_obj;
  int32_t release;
#ifdef __cplusplus
  CORBA_boolean _is_equivalent(CORBA_Object_struct*);
  CORBA_Object_struct & operator=(const CORBA_Object_struct &o);
#endif

}CORBA_Object_struct;

typedef CORBA_Object_struct * CORBA_Object; /*!< pointer type of CORBA_Object_struct */

/*!
 * @struct CORBA_ServiceInforamtion
 * @brief CORBA_ServiceInforamtion
 */
typedef struct {
}CORBA_ServiceInforamtion;

/*!
 * @struct CORBA_Context
 * @brief CORBA_Context
 */
typedef struct {
}CORBA_Context;

#ifdef __cplusplus
}
#endif 
#endif
