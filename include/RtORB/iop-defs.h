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
 * @file iop-defs.h
 * @if jp
 * @brief IOP定義
 * @else
 * @brief IOP Definition
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __IOP_DEFS_H__
#define __IOP_DEFS_H__

#include <RtORB/corba-defs.h>

#ifdef __cplusplus
extern "C"
{
#endif 
/*
 * Standaer Service Contexts
 */
/*! enum IOP_ServiceId (ref: CORBA3.1 Interoperability)  */
enum IOP_ServiceId {
  IOP_TransactionService,
  IOP_CodeSets,
  IOP_ChainBypassChunk,
  IOP_ChainBypassInfo,
  IOP_LogicalThreadId,
  IOP_BI_DIR_IIOP,
  IOP_SendingContextRunTime,
  IOP_INVOCATION_POLICIES,
  IOP_FORWARDED_IDENTITY,
  IOP_UnknownExceptionInfo,
  IOP_RTCorbaPriority,
  IOP_RTCorbaPriorityRange,
  IOP_FT_GROUP_VERSION,
  IOP_FT_REQUEST,
  IOP_ExceptionDetailMessage,
  IOP_SecurityAttributeService,
  IOP_ActivityService,
  IOP_ServiceId_END
};

/*! enum IOP_Profile_ID (ref: CORBA3.1 Interoperability) */
enum IOP_Profile_ID {
  IOP_TAG_INTERNET_IOP,
  IOP_TAG_MULTIPLE_COMPONENTS,
  IOP_TAG_SCCP_IOP,
  IOP_TAG_PROFILE_END
};

/*! enum IOP_Component_ID (ref: CORBA3.1 Interoperability) */
enum IOP_Component_ID {
  IOP_TAG_ORB_TYPE,
  IOP_TAG_CODE_SETS,
  IOP_TAG_POLICIES,
  IOP_TAG_ALTERNATE_IIOP_ADDRESS,
  IOP_TAG_COMPLETE_OBJECT_KEY,
  IOP_TAG_ENDPOINT_ID_POSITION,
  IOP_TAG_LOCATION_POLICY = 12,
  IOP_TAG_ASSOCIATION_OPTIONS,
  IOP_TAG_SPKM_1_SEC_MECH,
  IOP_TAG_SPKM_2_SEC_MECH,
  IOP_TAG_KERBEROSV5_SEC_MACH,
  IOP_TAG_CSI_ECMA_SECRET_SEC_MECH,
  IOP_TAG_CSI_ECMA_HYBRID_SEC_MECH,
  IOP_TAG_SSL_SEC_TRANS,
  IOP_TAG_CSI_ECMA_PUBLIC_SEC_MECH,
  IOP_TAG_GENERIC_SEC_MECH,
  IOP_TAG_FIREWALL_TRANS,
  IOP_TAG_SCCP_CONTACT_INFO,
  IOP_TAG_JAVA_CODEBSE,
  IOP_TAG_DCE_STRING_BINDING=100,
  IOP_TAG_DCE_BINDING_NAME,
  IOP_TAG_DCE_NO_PIPES,
  IOP_TAG_DCE_SEC_MECH,
  IOP_TAG_INET_SEC_MECH=123,
  IOP_TAG_GROUP=90001,
  IOP_TAG_PRIMARY,
  IOP_TAG_HEARTBEAT_ENABLED,

  IOP_TAG_COMPONENT_END
};


/******* IOP ************/
/*!
 * @struct IOP_ServiceContext
 * @brief IOP_ServiceContext (TODO)
 * @param context_id (TODO)
 * @param context_data (TODO)
 */
typedef struct {
  uint32_t  context_id;
  CORBA_Sequence_Octet context_data;
} IOP_ServiceContext;

/*!
 * @struct IOP_ServiceContextList
 * @brief IOP_ServiceContextList (TODO)
 * @param num (TODO)
 * @param data (TODO)
 */
typedef  struct{
  uint32_t num;
  IOP_ServiceContext *data;
} IOP_ServiceContextList;

/*!
 * @struct IOP_TaggedProfile
 * @brief IOP_TaggedProfile (TODO)
 * @param tag (TODO)
 * @param profile_data (TODO)
 */
typedef struct {
  uint32_t tag;
  CORBA_Sequence_Octet profile_data;
} IOP_TaggedProfile;

/*!
 * @struct CORBA_Sequence_IOP_TaggedProfile
 * @brief CORBA_Sequence_IOP_TaggedProfile (TODO)
 * @param size (TODO)
 * @param profile (TODO)
 */
typedef struct {
   uint32_t size;
   IOP_TaggedProfile *profile;
} CORBA_Sequence_IOP_TaggedProfile;

/*!
 * @struct IOP_IOR
 * @brief IOP_IOR (TODO)
 * @param type_id (TODO)
 * @param profile (TODO)
 */
typedef struct{
  CORBA_String type_id;
  CORBA_Sequence_IOP_TaggedProfile profiles;
} IOP_IOR;

/*!
 * @struct IORAddressingInfo
 * @brief IORAddressingInfo (TODO)
 * @param selected_profile_index (TODO)
 * @param ior (TODO)
 */
typedef struct{
  uint32_t selected_profile_index;
  IOP_IOR ior;
} IORAddressingInfo;


typedef uint32_t IOP_ComponentId;

/*!
 * @struct IOP_TaggedComponent
 * @brief IOP_TaggedComponent (TODO)
 * @param tag (TODO)
 * @param component_data (TODO)
 */
typedef struct {
  IOP_ComponentId tag;
  CORBA_Sequence_Octet component_data;
} IOP_TaggedComponent;

/*!
 * @struct CORBA_Sequence_IOP_TaggedComponent
 * @brief CORBA_Sequence_IOP_TaggedComponent (TODO)
 * @param size (TODO)
 * @param data (TODO)
 */
typedef struct {
  uint32_t size;
  IOP_TaggedComponent **data;
} CORBA_Sequence_IOP_TaggedComponent;


/**
 *  IIOP
 *
 */
/*!
 * @struct IIOP_Version
 * @brief IIOP Version struct
 * @param major major version
 * @param minor minor version
 */
typedef struct {
  octet  major;
  octet  minor;
} IIOP_Version;

/*!
 * @struct IIOP_ProfileBody_1_0
 * @brief IIOP_ProfileBody_1_0 (TODO)
 * @param version IIOP version
 * @param dummy (TODO)
 * @param host host (IP address)
 * @param port PORT number
 * @param object_key object_key
 */
typedef struct {
  IIOP_Version version;
  octet dummy[2];
  CORBA_String host;
  uint16_t port;
  CORBA_Sequence_Octet object_key;
} IIOP_ProfileBody_1_0;

/*!
 * @struct IIOP_ProfileBody_1_1
 * @brief IIOP_ProfileBody_1_1 (TODO)
 * @param version (TODO)
 * @param dummy (TODO)
 * @param host host (IP address)
 * @param port PORT number
 * @param object_key object_key
 * @param components (TODO)
 */
typedef struct {
  IIOP_Version version;
  octet dummy[2];
  CORBA_String host;
  uint16_t port;
  CORBA_Sequence_Octet object_key;
  CORBA_Sequence_IOP_TaggedComponent components;
} IIOP_ProfileBody_1_1;

/*!
 * @union IIOP_ProfileBody
 * @brief IIOP_ProfileBody (TODO)
 * @param _1_0 (TODO)
 * @param _1_1 (TODO)
 */
typedef union {
  IIOP_ProfileBody_1_0 _1_0;
  IIOP_ProfileBody_1_1 _1_1;
} IIOP_ProfileBody;

/*!
 * @struct IIOP_ListenPoint
 * @brief IIOP_ProfileBody (TODO)
 * @param host host (IP address)
 * @param port PORT number
 */
typedef struct {
  CORBA_String host;
  uint16_t port;
} IIOP_ListenPoint;

/*!
 * @struct CORBA_Sequence_IIOP_ListenPoint
 * @brief CORBA_Sequence_IIOP_ListenPoint (TODO)
 * @param size (TODO)
 * @param data (TODO)
 */
typedef struct {
  uint32_t size;
  IIOP_ListenPoint *data;
} CORBA_Sequence_IIOP_ListenPoint;

typedef CORBA_Sequence_IIOP_ListenPoint IIOP_ListenPointList;

/*!
 * @struct BiDir_IIOP_ServiceContext
 * @brief BiDir_IIOP_ServiceContext (TODO)
 * @param listen_points (TODO)
 */
typedef struct {
  IIOP_ListenPointList  listen_points;
} BiDir_IIOP_ServiceContext;


/*

*/
#define MAX_URLS        10
enum{
	  PROTO_RIR, PROTO_IIOP, PROTO_OTHER
};

/*!
 * @struct CORBA_URL
 * @brief CORBA_URL (TODO)
 * @param protocol (TODO)
 * @param byte_order byte order (0x00:big endian)
 * @param version_major major version
 * @param version_minor minor version
 * @param type_id type_id
 * @param hostname host (IP address)
 * @param port PORT number
 * @param object_key object_key
 * @param naming_context (TODO)
 * @param object_key_len (TODO)
 * @param flags (TODO)
 * @param location_flags (TODO)
 * @param _ior_string (TODO)
 * @param _ior_string_len (TODO)
 */
typedef struct {
  int32_t protocol;
  int32_t byte_order;
  int32_t version_major;
  int32_t version_minor;
  char *type_id;
  char *hostname;
  uint16_t port;
  char *object_key;
  char *naming_context;
  uint32_t object_key_len;
  uint32_t flags;
  uint32_t location_flags;
  char *_ior_string;
  int32_t _ior_string_len;
} CORBA_URL;


#ifdef __cplusplus
}
#endif 
#endif
