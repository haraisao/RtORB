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
 * @file giop-defs.h
 * @if jp
 * @brief GIOP定義
 * @else
 * @brief GIOP Definition
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __GIOP_DEFS_H__
#define __GIOP_DEFS_H__

#include <RtORB/corba-defs.h>
#include <RtORB/corba-object-defs.h>
#include <RtORB/iop-defs.h>


#ifdef __cplusplus
extern "C"
{
#endif 
#define SIZEOF_GIOP_HEADER 12  
/*
#define MaxMessageSize 262144
*/
#define MaxMessageSize 4096  

/*
 * GIOP Message Type
 */
/*! enum GIOP_MsgType */
enum GIOP_MsgType{
  GIOP_Request,          /*!< for calling remote message. */
  GIOP_Reply,            /*!< for response to Request message. */
  GIOP_CancelRequest,    /*!< for canceling send request. (means no wait to Reply.) */
  GIOP_LocateRequest,    /*!< for checking whether server suppot the remote object or not. */
  GIOP_LocateReply,      /*!< for response messages to LocateRequest. */
  GIOP_CloseConnection,  /*!< for closing connection from server. */
  GIOP_MessageError,     /*!< for response invalid message. */
  GIOP_Fragment,         /*!< for flag following message because long messages are divided. */
  GIOP_MsgType_END       /*!< for Message type end */
};

/*
 * GIOP Message Type
 */
/*! enum GIOP_TargetAddress_Type (TODO) */
enum GIOP_TargetAddress_Type{
  GIOP_KeyAddr, GIOP_ProfileAddr, GIOP_ReferenceAddr
};

/*
 * GIOP ReplyStatus
 */
/*! enum ReplyStatusType (TODO) */
enum ReplyStatusType{
  GIOP_NO_EXCEPTION,          /*!< TODO */
  GIOP_USER_EXCEPTION,        /*!< TODO */
  GIOP_SYSTEM_EXCEPTION,      /*!< TODO */
  GIOP_LOCATION_FORWARD,      /*!< TODO */
  GIOP_LOCATION_FORWARD_PERM, /*!< > GIOP 1.2 */
  GIOP_NEEDS_ADDRESSING_MODE, /*!< > GIOP 1.2 */
  GIOP_ReplyStatusType_END    /*!< TODO */
};

/*
 * GIOP LocationStatus
 */
/*! enum LocatieStatusType (TODO) */
enum LocatieStatusType{
  GIOP_UNKNOWN_OBJECT,             /*!< TODO */
  GIOP_OBJECT_HERE,                /*!< TODO */
  GIOP_OBJECT_FORWARD,             /*!< TODO */
  GIOP_OBJECT_FORWARD_PERM,	   /*!< > GIOP 1.2 */
  GIOP_LOC_SYSTEM_EXCEPTION,	   /*!< > GIOP 1.2 */
  GIOP_LOC_NEEDS_ADDRESSING_MODE,  /*!< > GIOP 1.2 */
  GIOP_LocationStatusType_END      /*!< TODO */
};

typedef int16_t AddressingDisposition;

/*!
 * @union GIOP_TargetAddress
 * @brief GIOP_TargetAddress (TODO)
 * @param object_key ... when KeyAddr
 * @param profile ... when ProfileAddr 
 * @param ior ... when ReafercnceAddr
 */
typedef union {
  CORBA_Sequence_Octet object_key; 
  IOP_TaggedProfile profile; 
  IORAddressingInfo ior;     
} GIOP_TargetAddress;

/******* GIOP ************/
/*
 * GIOP Message Header
 */
/*!
 * @struct GIOP_Version
 * @brief GIOP_Version for GIOP_Message_Header
 * @param major major version
 * @param minor minor version
 */
typedef struct {
  octet major;
  octet minor;
} GIOP_Version;

/*!
 * @struct GIOP_MessageHeader
 * @brief GIOP Message Header structure
 * @param magic magic number (='GIOP')
 * @param flags byte order for CDR marshal/unmarshal
 * @param version GIOP version
 * @param message_type type of message
 * @param message_size size of message
 */
typedef struct{
  char magic[4];
  GIOP_Version version;
  octet flags;  	/* GIOP1.0 --> byte_code */
  octet message_type;
  uint32_t message_size;
} GIOP_MessageHeader;

/*
 * GIOP Request Header
 */
/*!
 * @struct GIOP_RequestHeader_1_0
 * @brief GIOP_RequestHeader structur [ver1.0]
 * @param service_context (TODO)
 * @param request_id request ID
 * @param response_expected (TODO)
 * @param object_key object key
 * @param operation (TODO)
 * @param requesting_principal (TODO)
 */
typedef struct {
  IOP_ServiceContextList service_context;
  uint32_t request_id;
  boolean  response_expected;
  CORBA_Sequence_Octet object_key;
  CORBA_String operation;
  CORBA_Sequence_Octet requesting_principal;
} GIOP_RequestHeader_1_0;

/*!
 * @struct GIOP_RequestHeader_1_1
 * @brief GIOP_RequestHeader structur [ver1.1]
 * @param service_context (TODO)
 * @param request_id request ID
 * @param response_expected (TODO)
 * @param reserved (TODO)
 * @param object_key object key
 * @param operation (TODO)
 * @param requesting_principal (TODO)
 */
typedef struct{
  IOP_ServiceContextList service_context;
  uint32_t request_id;
  boolean  response_expected;
  unsigned char reserved[3];
  CORBA_Sequence_Octet object_key;
  CORBA_String operation;
  CORBA_Sequence_Octet requesting_principal;
} GIOP_RequestHeader_1_1;

/*!
 * @struct GIOP_RequestHeader_1_2
 * @brief GIOP_RequestHeader structur [ver1.2]
 * @param request_id request ID
 * @param response_flags (TODO)
 * @param reserved (TODO)
 * @param target_type (TODO)
 * @param target (TODO)
 * @param operation (TODO)
 * @param service_context (TODO)
 * @param requesting_principal (TODO)
 */
typedef struct {
  uint32_t request_id;
  unsigned char response_flags;
  unsigned char reserved[3];
  uint16_t target_type;
  GIOP_TargetAddress target;
  CORBA_String operation;
  IOP_ServiceContextList service_context;
} GIOP_RequestHeader_1_2;

typedef GIOP_RequestHeader_1_2 GIOP_RequestHeader_1_3;

/*!
 * @union GIOP_RequestHeader
 * @brief GIOP_RequestHeader union
 * @param _1_0 GIOP_RequestHeader structure [ver1.0]
 * @param _1_1 GIOP_RequestHeader structure [ver1.1]
 * @param _1_2 GIOP_RequestHeader structure [ver1.2]
 */
typedef union{
  GIOP_RequestHeader_1_0  _1_0;
  GIOP_RequestHeader_1_1  _1_1;
  GIOP_RequestHeader_1_2  _1_2;
} GIOP_RequestHeader;

/*
 * GIOP Reply Header
 */
/*!
 * @struct GIOP_ReplyHeader_1_0
 * @brief GIOP_Reply Header structur [ver1.0]
 * @param service_context (TODO)
 * @param request_id request ID for response
 * @param reply_status number of 'ReplyStatusType'
 */
typedef struct {
  IOP_ServiceContextList service_context;
  uint32_t request_id;
  uint32_t reply_status;
} GIOP_ReplyHeader_1_0;

typedef GIOP_ReplyHeader_1_0 GIOP_ReplyHeader_1_1;

/*!
 * @struct GIOP_ReplyHeader_1_2
 * @brief GIOP_ReplyHeader structur [ver1.2]
 * @param request_id request ID for response
 * @param reply_status number of 'ReplyStatusType'
 * @param service_context (TODO)
 */
typedef struct {
  uint32_t request_id;
  uint32_t reply_status;
  IOP_ServiceContextList service_context;
} GIOP_ReplyHeader_1_2;

typedef GIOP_ReplyHeader_1_2 GIOP_ReplyHeader_1_3;

/*!
 * @union GIOP_ReplyHeader
 * @brief GIOP_ReplyHeader union
 * @param _1_0 ReplyHeader of GIOP ver1.0 and ver1.1
 * @param _1_2 ReplyHeader of GIOP ver1.2 and ver1.3
 */
typedef union{
  GIOP_ReplyHeader_1_0  _1_0;
  GIOP_ReplyHeader_1_2  _1_2;
} GIOP_ReplyHeader;

/*!
 * @struct GIOP_SystemExceptionReplyBody
 * @brief GIOP_SystemExceptionReplyBody structure
 * @param exception_id Exception ID
 * @param minor_code_value minor code of Exception
 * @param completion_status (TODO)
 */
typedef struct {
  CORBA_String  exception_id;
  uint32_t minor_code_value;
  uint32_t completion_status;
} GIOP_SystemExceptionReplyBody;

/*
 * GIOP Cancel Request Header
 */
/*!
 * @struct GIOP_CancelRequestHeader
 * @brief GIOP Cancel Request Header structure
 * @param request_id request ID
 */
typedef struct {
  uint32_t request_id;
} GIOP_CancelRequestHeader;

/*
 * GIOP Locate Request Header
 */
/*!
 * @struct GIOP_LocateRequestHeader_1_0
 * @brief GIOP Locate Request Header structure [ver1.0]
 * @param request_id request ID
 * @param object_key object key
 */
typedef struct {
  uint32_t request_id;
  CORBA_Sequence_Octet object_key;
} GIOP_LocateRequestHeader_1_0;

typedef GIOP_LocateRequestHeader_1_0 GIOP_LocateRequestHeader_1_1;

/*!
 * @struct GIOP_LocateRequestHeader_1_2
 * @brief GIOP Locate Request Header structure [ver1.2]
 * @param request_id request ID
 * @param target_key (TODO)
 * @param target (TODO)
 */
typedef struct {
  uint32_t request_id;
  uint16_t target_type;
  GIOP_TargetAddress target;
} GIOP_LocateRequestHeader_1_2;

typedef GIOP_LocateRequestHeader_1_2 GIOP_LocateRequestHeader_1_3;

/*!
 * @union GIOP_LocateRequestHeader
 * @brief GIOP_LocateRequestHeader union
 * @param _1_0 GIOP_LocateRequestHeader structure [ver1.0/ver1.1]
 * @param _1_2 GIOP_LocateRequestHeader structure [ver1.2/ver1.3]
 */
typedef union {
   GIOP_LocateRequestHeader_1_0 _1_0;
   GIOP_LocateRequestHeader_1_2 _1_2;
} GIOP_LocateRequestHeader;


/*
 * GIOP Locate Reply Header
 */
/*!
 * @struct GIOP_LocateReplyHeader_1_0
 * @brief GIOP Locate Reply Header structure [ver1.0]
 * @param request_id request ID
 * @param locate_status locate_status (releated with 'LocatieStatusType')
 */
typedef struct {
  uint32_t request_id;
  uint32_t locate_status;
} GIOP_LocateReplyHeader_1_0;

typedef GIOP_LocateReplyHeader_1_0 GIOP_LocateReplyHeader_1_1;
typedef GIOP_LocateReplyHeader_1_0 GIOP_LocateReplyHeader_1_2;
typedef GIOP_LocateReplyHeader_1_0 GIOP_LocateReplyHeader_1_3;

typedef GIOP_LocateReplyHeader_1_0 GIOP_LocateReplyHeader;

/*!
 * @struct GIOP_ReplyBody
 * @brief GIOP_ReplyBody structure
 * @param status (TODO)
 * @param body_size (TODO)
 * @param body (TODO)
 * @param exception (TODO)
 */
typedef struct{
  uint32_t status;
  uint32_t body_size;
  octet *body;
  GIOP_SystemExceptionReplyBody exception;
} GIOP_ReplyBody;

/*
 * GIOP CloseConnection Header
 */

/*
 * GIOP MessageError Header
 */

/*
 * GIOP Fragment Request Header
 */

/*!
 * @struct GIOP_FragmentHeader_1_2
 * @brief GIOP_FragmentHeader (TODO)
 * @param request_id (TODO)
 */
typedef struct {
  uint32_t request_id;
} GIOP_FragmentHeader_1_2;

typedef GIOP_FragmentHeader_1_2 GIOP_FragmentHeader_1_3;

/*! enum GIOP_ConnectionType (Socket/ShareMemory) */
typedef enum {
  CONNECTION_TYPE_SOCKET = 0,  /*!< socket */
} GIOP_ConnectionType;
  
/*!
 * @struct GIOP_ConnectionHandler
 * @brief GIOP_ConnectionHandler (TODO)
 * @param type (Socket/ShareMemory)
 * @param sock (TODO)
 */
typedef struct
{
  GIOP_ConnectionType type;
  int	sock;
} GIOP_ConnectionHandler;

/*!
 * @if jp
 * @brief Socket,Shared_Memoryいずれかのコネクションタイプによる場合分けで、Socket,Shared_Memoryいずれかからの入力データの読み込みを行い、'buf'に書き込む。
 * @else
 * @brief Read data from Socket or Shared_Moemory and write to 'buf' considering connection types(Socket/SharedMemory).
 * @endif
 * @param h GIOP_ConnectionHandler
 * @param buf buffer for putting data 
 * @param len data size
 * @return 1:Success, -1:Fail 
 */
int32_t GIOP_ConnectionHandler_recv(GIOP_ConnectionHandler *h, char *buf, int32_t len);

/*!
 * @if jp
 * @brief Socket,Shared_Memoryいずれかのコネクションタイプによる場合分けで、'buf'のデータを読み込み、Socket,Shared_Memoryいずれかに書き込む。
 * @else
 * @brief Write data from 'buf' to Socket or Shared_Moemory considering connection types(Socket/SharedMemory).
 * @endif
 * @param h GIOP_ConnectionHandler
 * @param buf buffer for reading data 
 * @param len data size
 * @return size of written data, '-1':Fail to write
 */
int32_t GIOP_ConnectionHandler_send(GIOP_ConnectionHandler *h, char *buf, int32_t len);

/*!
 * @if jp
 * @brief Socket,Shared_Memoryいずれかのコネクションタイプによる場合分けで、コネクションをクローズする。
 * @else
 * @brief Close connection considering connection types(Socket/SharedMemory).
 * @endif
 * @param h GIOP_ConnectionHandler
 * @return void
 */
void GIOP_ConnectionHandler_close(GIOP_ConnectionHandler *h);	

/*!
 * @if jp
 * @brief Socket Profile構造体よりPOAオブジェクトを取得する。
 * @else
 * @brief get POA object from Socket Profile structure.
 * @endif
 * @param h GIOP_ConnectionHandler
 * @return POA Object
 */
void * GIOP_ConnectionHandler_get_arg(GIOP_ConnectionHandler *h);

/*!
 * @struct GIOP_Request_struct
 * @brief GIOP_Request_struct (TODO)
 * @param connh (TODO)
 * @param buf (TODO)
 * @param object (TODO)
 */
typedef struct GIOP_Request_struct {
  GIOP_ConnectionHandler *connh;
  char *buf;
  void *object;
} GIOP_Request_struct;

typedef GIOP_Request_struct * GIOP_Request_Item;

#ifdef __cplusplus


}
#endif 
#endif
