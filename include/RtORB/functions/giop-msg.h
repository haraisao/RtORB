/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 * RtORB: giop-msg.h
 *   IIOP-GIOP
 *
 *  Copyright(C) 2006, Isao Hara, AIST
 *
 *  $Revision: 1.2 $
 *  $Date: 2008/05/20 05:46:29 $
 *  $Id: giop-msg.h,v 1.2 2008/05/20 05:46:29 yoshi Exp $
 */
/*!
 * @file giop-msg.h
 * @if jp
 * @brief GIOP関連処理
 * @else
 * @brief GIOP actions
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __FUNC_GIOP_MSG_H__
#define __FUNC_GIOP_MSG_H__
/************************************/

#define delete_CORBA_String(str, flag)  delete_CORBA_Sequence_Octet(str, flag)

#if 0
/******* Global Variable ************/
GIOP_Version GIOP_1_0 = {1,0};
GIOP_Version GIOP_1_1 = {1,1};
GIOP_Version GIOP_1_2 = {1,2};
GIOP_Version GIOP_1_3 = {1,3};

uint32_t request_counter = 0;
#endif

/************************************/
/*!
 * @if jp
 * @brief マーシャリングされたGIOP_SystemExceptionReplyBodyデータを作成する。
 * @else
 * @brief Set each parameters to GIOP_SystemExceptionReplyBody data and marshal. 
 * @endif
 * @param e_body GIOP_SystemExceptionReplyBody data
 * @param id ID for 'exception_id'
 * @param code CODE for 'minor_code_value'
 * @param state STATE for 'completion_status'
 * @return Marshaled 'GIOP_SystemExceptionReplyBody' data
 */
octet *setSystemException( GIOP_SystemExceptionReplyBody *e_body, char *id,  int code, int state);

/*!
 * @if jp
 * @brief CORBA_Sequence_Octet型のメモリリリースを行いデータを削除する。
 * @else
 * @brief Release memory and delete CORBA_Sequence_Octet type's data.
 * @endif
 * @param seq CORBA_Sequence_Octet data
 * @param flag 1:delete,  else:no_delete
 * @return void
 */
void delete_CORBA_Sequence_Octet(CORBA_Sequence_Octet *seq, int flag);

/*!
 * @if jp
 * @brief ServiceContextList型のメモリリリースを行いデータを削除する。
 * @else
 * @brief Release memory and delete ServiceContextList type's data.
 * @endif
 * @param list IOP_ServiceContextList type's data
 * @param flag 1:delete,  else:no_delete
 * @return void
 */
void delete_IOP_ServiceContextList(IOP_ServiceContextList *list, int flag);

/*!
 * @if jp
 * @brief GIOP_TargetAddress型のメモリリリースを行いデータを削除する。
 * @else
 * @brief Release memory and delete GIOP_TargetAddress type's data.
 * @endif
 * @param target GIOP_TargetAddress type's data
 * @param type GIOP_TargetAddress type(GIOP_KeyAddr/GIOP_ProfileAddr/GIOP_ReferenceAddr)
 * @param flag 1:delete,  else:no_delete
 * @return void
 */
void delete_CORBA_TargetAddress(GIOP_TargetAddress *target, unsigned short type, int flag);

/*!
 * @if jp
 * @brief GIOP_RequestHeaderデータ用のメモリ確保を行い、中身の初期化をして、そのポインタを返す。
 * @else
 * @brief Allocate memory for GIOP_RequestHeader data, initialize, and return it.
 * @endif
 * @return initialized GIOP_RequestHeader data 
 */
GIOP_RequestHeader *newRequestHeader();

/*!
 * @if jp
 * @brief GIOP_ReplyHeaderデータ用のメモリ確保を行い、中身の初期化をして、そのポインタを返す。
 * @else
 * @brief Allocate memory for GIOP_ReplyHeader data, initialize, and return it.
 * @endif
 * @return initialized GIOP_ReplyHeader data 
 */
GIOP_ReplyHeader *newReplyHeader();

/*!
 * @if jp
 * @brief GIOP_CancelRequestHeaderデータ用のメモリ確保を行い、中身の初期化をして、そのポインタを返す。
 * @else
 * @brief Allocate memory for GIOP_CancelRequestHeader data, initialize, and return it.
 * @endif
 * @return initialized GIOP_CancelRequestHeader data 
 */
GIOP_CancelRequestHeader *newCancelRequestHeader();

/*!
 * @if jp
 * @brief GIOP_LocateRequestHeaderデータ用のメモリ確保を行い、中身の初期化をして、そのポインタを返す。
 * @else
 * @brief Allocate memory for GIOP_LocateRequestHeader data, initialize, and return it.
 * @endif
 * @return initialized GIOP_LocateRequestHeader data 
 */
GIOP_LocateRequestHeader *newLocateRequestHeader();

/*!
 * @if jp
 * @brief GIOP_LocateReplyHeaderデータ用のメモリ確保を行い、中身の初期化をして、そのポインタを返す。
 * @else
 * @brief Allocate memory for GIOP_LocateReplyHeader data, initialize, and return it.
 * @endif
 * @return initialized GIOP_LocateReplyHeader data 
 */
GIOP_LocateReplyHeader *newLocateReplyHeader();

/*!
 * @if jp
 * @brief sizeで指定した領域のメモリをCORBA_Sequence_Octet型データ用に確保し、中身の各パラメータを初期値セットし、ポインタを返す。
 * @else
 * @brief Allocate memory by indicated size 'size' for CORBA_Sequence_Octet type's data, initialize contents parameter, and return pointer of this data. 
 * @endif
 * @param size data size for memory allocation
 * @return initialized CORBA_Sequence_Octet data
 */
CORBA_Sequence_Octet *new_CORBA_Sequence_Octet(int size);

/*!
 * @if jp
 * @brief sizeで指定した領域のメモリをCORBA_Sequence_Octet型データ用に確保し、中身の各パラメータを初期値セットし、ポインタを返す。
 * @else
 * @brief Allocate memory by indicated size 'size' for CORBA_Sequence_Octet type's data, initialize contents parameter, and return pointer of this data. 
 * @endif
 * @param size data size for memory allocation
 * @return initialized CORBA_Sequence_Octet data
 */
CORBA_Sequence_Octet *new_CORBA_Sequence_Octet2(int size);

/*!
 * @if jp
 * @brief CORBA_Sequence_Octet型のメモリリリースを行いデータを削除する。
 * @else
 * @brief Release memory and delete CORBA_Sequence_Octet type's data.
 * @endif
 * @param seq CORBA_Sequence_Octet data
 * @param flag 1:delete,  else:no_delete
 * @return void
 */
void delete_CORBA_Sequence_Octet2(CORBA_Sequence_Octet *seq, int flag);

/*!
 * @if jp
 * @brief 入力データ'val'をCORBA_Sequence_Octet型データとしてセットする。サイズのリサイズも行う。
 * @else
 * @brief Set 'val' data as CORBA_Sequence_Octet data. Resize data size.
 * @endif
 * @param seq CORBA_Sequence_Octet data
 * @param val data for setting as CORBA_Sequence_Octet data
 * @param size data size of 'val'
 * @return void
 */
void set_CORBA_Sequence_Octet(CORBA_Sequence_Octet *seq, octet *val, int size);

/*!
 * @if jp
 * @brief CORBA_Sequence_Octet型データをクリアする。内部のサイズを'0'にするのみ。
 * @else
 * @brief Clear CORBA_Sequence_Octet data. Only set size to '0'.
 * @endif
 * @param seq CORBA_Sequence_Octet data
 * @return void
 */
void clear_CORBA_Sequence_Octet(CORBA_Sequence_Octet *seq);

/*!
 * @if jp
 * @brief GIOP_RequestHeader型のメモリリリースを行いデータを削除する。
 * @else
 * @brief Release memory and delete GIOP_RequestHeader type's data.
 * @endif
 * @param header GIOP_RequestHeader data
 * @param version minor version of GIOP
 * @return void
 */
void deleteRequestHeader(GIOP_RequestHeader *header, int version);

/*!
 * @if jp
 * @brief GIOP_SystemExceptionReplyBodyのメモリリリースを行いデータを削除する。
 * @else
 * @brief Release memory and delete GIOP_SystemExceptionReplyBody type's data.
 * @endif
 * @param exp GIOP_SystemExceptionReplyBody data
 * @return void
 */
void deleteSystemExceptionReplyBody(GIOP_SystemExceptionReplyBody *exp);

/*!
 * @if jp
 * @brief GIOP_ReplyBodyのメモリリリースを行いデータを削除する。
 * @else
 * @brief Release memory and delete GIOP_ReplyBody type's data.
 * @endif
 * @param body GIOP_ReplyBody data
 * @return void
 */
void deleteReplyBody(GIOP_ReplyBody *body);

/*!
 * @if jp
 * @brief GIOP_ReplyHeaderのメモリリリースを行いデータを削除する。
 * @else
 * @brief Release memory and delete GIOP_ReplyHeader type's data.
 * @endif
 * @param header GIOP_ReplyHeader data
 * @param version minor version of GIOP
 * @return void
 */
void deleteReplyHeader(GIOP_ReplyHeader *header, int version);

/*!
 * @if jp
 * @brief GIOP_LocateRequestHeaderのメモリリリースを行いデータを削除する。
 * @else
 * @brief Release memory and delete GIOP_LocateRequestHeader type's data.
 * @endif
 * @param header GIOP_LocateRequestHeader data
 * @param version minor version of GIOP
 * @return void
 */
void deleteLocateRequestHeader(GIOP_LocateRequestHeader *header, int version);

/*
   demarshal Messages
*/
/*!
 * @if jp
 * @brief 各GIOPバージョンで場合分けし、'buf'からGIOP_Requestタイプメッセージのアンマーシャリングを行い、'header'と'body'にセットする。
 * @else
 * @brief Get GIOP_Request data from 'buf', demarshal considering GIOP version, set each data to 'header' and 'body'.  
 * @endif
 * @param header GIOP_Request header part buffer for setting from 'buf'
 * @param body GIOP_Request body part buffer for setting from 'buf'
 * @param buf buffer of received data for getting GIOP_Request data
 * @param m_header received GIOP_MessageHeader data
 * @return 1:Success, -1:Fail
 */
int deMarshalRequest(GIOP_RequestHeader *header, CORBA_Sequence_Octet *body,
		char *buf, GIOP_MessageHeader *m_header);

/*!
 * @if jp
 * @brief GIOP_ver1.0,_ver1.1において、'buf'からGIOP_Requestタイプメッセージのアンマーシャリングを行い、'header'と'body'にセットする。
 * @else
 * @brief Get GIOP_Request data from 'buf', demarshal considering GIOP version, set each data to 'header' and 'body'.  (GIOP_ver1.0, _ver1.1)
 * @endif
 * @param Header GIOP_Request header part buffer for setting from 'buf'
 * @param body  GIOP_Request body part buffer for setting from 'buf'
 * @param buf buffer of received data for getting GIOP_Request data
 * @param m_header received GIOP_MessageHeader data
 * @return 1:Success, -1:Fail
 */
int deMarshalRequest_1_0(GIOP_RequestHeader *Header, CORBA_Sequence_Octet *body,
		char *buf, GIOP_MessageHeader *m_header);

/*!
 * @if jp
 * @brief GIOP_ver1.2,_ver1.3において、'buf'からGIOP_Requestタイプメッセージのアンマーシャリングを行い、'header'と'body'にセットする。
 * @else
 * @brief Get GIOP_Request data from 'buf', demarshal considering GIOP version, set each data to 'header' and 'body'.  (GIOP_ver1.2, _ver1.3)
 * @endif
 * @param Header GIOP_Request header part buffer for setting from 'buf'
 * @param body GIOP_Request body part buffer for setting from 'buf'
 * @param buf buffer of received data for getting GIOP_Request data
 * @param m_header received GIOP_MessageHeader data
 * @return 1:Success, -1:Fail
 */
int deMarshalRequest_1_2(GIOP_RequestHeader *Header, CORBA_Sequence_Octet *body,
		char *buf, GIOP_MessageHeader *m_header);

/*!
 * @if jp
 * @brief 各GIOPバージョンで場合分けし、'buf'からGIOP_Replyタイプメッセージのアンマーシャリングを行い、'header'と'body'にセットする。
 * @else
 * @brief Get GIOP_Reply data from 'buf', demarshal considering GIOP version, set each data to 'header' and 'body'.  
 * @endif
 * @param header GIOP_Reply header part buffer for setting from 'buf'
 * @param body GIOP_Reply body part buffer for setting from 'buf'
 * @param buf buffer of received data for getting GIOP_Reply data
 * @param m_header received GIOP_MessageHeader data
 * @return 1:Success, -1:Fail (Always '1')
 */
int deMarshalReply(GIOP_ReplyHeader *header, CORBA_Sequence_Octet *body,
		unsigned char *buf, GIOP_MessageHeader *m_header);

/*!
 * @if jp
 * @brief 'buf'のアンマーシャリングを行い、GIOP_CancelRequestHeader型データの'request_id'を取得しセットする。
 * @else
 * @brief Get GIOP_CancelRequest data from 'buf', demarshal, set data to 'request_id'.  
 * @endif
 * @param header GIOP_CancelRequest header part buffer for setting from 'buf'
 * @param buf buffer of received data for getting GIOP_CancelRequest data
 * @param m_header received GIOP_MessageHeader data
 * @return 1:Success, -1:Fail (Always '1')
 */
int deMarshalCancelRequest(GIOP_CancelRequestHeader *header,
		unsigned char *buf, GIOP_MessageHeader *m_header);

/*!
 * @if jp
 * @brief 各GIOPバージョンで場合分けし、'buf'からGIOP_LocateRequestHeaderタイプメッセージのアンマーシャリングを行い、'header'にセットする。
 * @else
 * @brief Get GIOP_LocateRequest data from 'buf', demarshal considering GIOP version, set each data to 'header'.  
 * @endif
 * @param header GIOP_LocateRequest header part buffer for setting from 'buf'
 * @param buf buffer of received data for getting GIOP_LocateRequest data
 * @param m_header received GIOP_MessageHeader data
 * @return 1:Success, -1:Fail (Always '1')
 */
int deMarshalLocateRequest(GIOP_LocateRequestHeader *header,
		unsigned char *buf, GIOP_MessageHeader *m_header);

/*!
 * @if jp
 * @brief 各'locate_status'で場合分けし、'buf'からGIOP_LocateReplyタイプメッセージのアンマーシャリングを行い、'header'と'body'にセットする。
 * @else
 * @brief Get GIOP_LocateReply data from 'buf', demarshal considering 'locate_status' types, set each data to 'header' and 'body'.  
 * @endif
 * @param header GIOP_LocateReply header part buffer for setting from 'buf'
 * @param body GIOP_LocateReply body part buffer for setting from 'buf'
 * @param buf buffer of received data for getting GIOP_LocateReply data
 * @param m_header received GIOP_MessageHeader data
 * @return 1:Success, -1:Fail (Always '1')
 */
int deMarshalLocateReply(GIOP_LocateReplyHeader *header,
		CORBA_Sequence_Octet *body, 
		unsigned char *buf, GIOP_MessageHeader *m_header);

/*
 *   Marshal Request & Reply
 */
/*!
 * @if jp
 * @brief 各GIOPバージョンで場合分けし、GIOP_Requestタイプメッセージの'header'と'body'部のデータをマーシャリングし、'buf'にセットする。全体のデータサイズを返す。
 * @else
 * @brief Marshal 'header' and 'body' data of GIOP_Request type considering GIOP version, and set them to 'buf' as GIOP_Request data. And return total size of GIOP_Request data.
 * @endif
 * @param buf target buffer for putting data and for sending GIOP_Request data
 * @param header GIOP_Request header part buffer for setting to 'buf'
 * @param body GIOP_Request body part buffer for setting to 'buf'
 * @param body_size size of GIOP_Request body part('body')
 * @param version minor version of GIOP
 * @return size of GIOP_Request data, or '-1':Fail
 */
int MarshalRequest(char *buf, GIOP_RequestHeader *header, octet *body,
		int body_size, int version);

/*!
 * @if jp
 * @brief GIOP_ver1.0,_ver1.1において、GIOP_Requestタイプメッセージの'Header'と'args(body)'部のデータをマーシャリングし、'buf'にセットする。全体のデータサイズを返す。
 * @else
 * @brief Marshal 'Header' and 'args' data of GIOP_Request type considering GIOP version, and set them to 'buf' as GIOP_Request data. And return total size of GIOP_Request data. (GIOP_ver1.0, _ver1.1)
 * @endif
 * @param buf target buffer for putting data and for sending GIOP_Request data
 * @param Header GIOP_Request header part buffer for setting to 'buf'
 * @param args GIOP_Request body part buffer for setting to 'buf'
 * @param size size of GIOP_Request body part('args')
 * @param version minor version of GIOP
 * @return size of GIOP_Request data, or '-1':Fail
 */
int MarshalRequest_1_0(char *buf, GIOP_RequestHeader *Header, octet *args,
		int size, int version);

/*!
 * @if jp
 * @brief GIOP_ver1.2,_ver1.3において、GIOP_Requestタイプメッセージの'Header'と'args(body)'部のデータをマーシャリングし、'buf'にセットする。全体のデータサイズを返す。
 * @else
 * @brief Marshal 'Header' and 'args' data of GIOP_Request type considering GIOP version, and set them to 'buf' as GIOP_Request data. And return total size of GIOP_Request data. (GIOP_ver1.2, _ver1.3)
 * @endif
 * @param buf target buffer for putting data and for sending GIOP_Request data
 * @param Header GIOP_Request header part buffer for setting to 'buf'
 * @param args GIOP_Request body part buffer for setting to 'buf'
 * @param size size of GIOP_Request body part('args')
 * @param version minor version of GIOP
 * @return size of GIOP_Request data, or '-1':Fail
 */
int MarshalRequest_1_2(char *buf, GIOP_RequestHeader *Header, octet *args,
		int size, int version);

/*!
 * @if jp
 * @brief GIOP_Replyメッセージ用にheader部body部を'buf'にマーシャリングして格納し、全体サイズを算出して返す。
 * @else
 * @brief Marshal header and body parts of GIOP_Reply data and set them to 'buf', calculate total data size, and return it.
 * @endif
 * @param buf target buffer for putting data and for sending GIOP_Reply data
 * @param size size of GIOP_Reply total data 
 * @param header GIOP_Reply header part buffer for setting to 'buf'
 * @param body GIOP_Reply body part buffer for setting to 'buf'
 * @param body_size size of GIOP_Reply body part
 * @param version minor version of GIOP
 * @return size of GIOP_Reply total data 
 */
int MarshalReply(unsigned char *buf, int *size, GIOP_ReplyHeader *header,
		octet *body, int body_size, int version);

/*!
 * @if jp
 * @brief 各GIOPバージョンで場合分けし、GIOP_LocateRequestメッセージ用にheader部を'buf'にマーシャリングして格納し、全体サイズを算出して返す。
 * @else
 * @brief Marshal GIOP_LocateRequest header data considering GIOP version and set them to 'buf', calculate total data size, and return it.
 * @endif
 * @param buf target buffer for putting data and for sending GIOP_LocateRequest data
 * @param header GIOP_LocateRequest header part buffer for setting to 'buf'
 * @param version minor version of GIOP
 * @return size of GIOP_Request data, or '-1':Fail
 */
int MarshalLocateRequest(char *buf, GIOP_LocateRequestHeader *header,
		int version);

/*!
 * @if jp
 * @brief GIOP_Messages_Headerを作成し、その後、各'locate_status'で場合分けし、GIOP_LocateReplyメッセージ用にheader部を'buf'にマーシャリングして格納し、全体サイズを算出して返す。
 * @else
 * @brief Create GIOP_Messages_Header data, and marshal GIOP_LocateReply header data considering 'locate_status' types, and set them to 'buf', calculate total data size, and return it.
 * @endif
 * @param buf target buffer for putting data and for sending GIOP_LocateReply data
 * @param size size of GIOP_LocateReply
 * @param header GIOP_LocateReply header part buffer for setting to 'buf'
 * @param ior IOR
 * @param version minor version of GIOP
 * @return size of GIOP_LocateReply data
 */
int MarshalLocateReply(unsigned char *buf, int *size,
		GIOP_LocateReplyHeader *header, char *ior, int version);

/*!
 * @if jp
 * @brief リクエスト用カウンタ'request_counter'をインクリメントさせるのみ。
 * @else
 * @brief Only increment counter for request (request_counter)
 * @endif
 * @return incremented counter for request (request_counter)
 */
uint32_t next_request_id();

#define RTORB_LITTLE_ENDIAN (1)
#define RTORB_BIG_ENDIAN    (0)

#define RTORB_BYTE_ORDER RTORB_LITTLE_ENDIAN

#endif
