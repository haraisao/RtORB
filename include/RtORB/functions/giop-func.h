/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 *  RtORB: giop-func.h
 *   IIOP-GIOP
 *
 *  Copyright(C) 2006, Isao Hara, AIST
 *
 *  $Revision: 1.2 $
 *  $Date: 2008/05/29 04:54:57 $
 *  $Id: giop.h,v 1.2 2008/05/29 04:54:57 yoshi Exp $
 *
 */
/*!
 * @file giop-func.h
 * @if jp
 * @brief GIOP関連処理
 * @else
 * @brief GIOP actions
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __FUNC_GIOP_H__
#define __FUNC_GIOP_H__

/*** GIOP_Connection ***/
/*!
 * @if jp
 * @brief GIOPコネクタを新規作成する。GIOP型のデータサイズをメモリに確保し、IPアドレスの取得、各中身のパラメータの初期セットを行う。コネクタのポインタを返す。
 * @else
 * @brief Create GIOP connector. Allocate Memory by GIOP data size, get Host(IP address), set each parameters, and return pointer of connector.
 * @endif
 * @return pointer of connector (GIOP_Connection data) 
 */
GIOP_Connection * GIOP_Connection__create();

/*!
 * @if jp
 * @brief 指定したコネクタとPORT番号でコネクションをオープンする。内部で'make_server_connection()'をcallする。
 * @else
 * @brief Open connection using indicated connector and PORT number. Call 'make_server_connection()' function. 
 * @endif
 * @param conn connector of GIOP_Connection data 
 * @param port PORT number
 * @return void
 */
void GIOP_Connection__open(GIOP_Connection *conn, short port);

/*!
 * @if jp
 * @brief コネクションをシャットダウンする。内部の'activate'フラグを'False'にするのみ。
 * @else
 * @brief Shutdown connection. Only set 'activate' flag's value to 'False'.
 * @endif
 * @param conn connector of GIOP_Connection data 
 * @return void
 */
void GIOP_Connection__shutdown(GIOP_Connection *conn);

/*!
 * @if jp
 * @brief コネクションをクローズする。GIOP_Connectionデータの中身のパラメータを全てクリアする。内部で'GIOP_Connection__shutdown()'を呼ぶ。
 * @else
 * @brief Close connection. Clear all of GIOP_Connection data. Call 'GIOP_Connection__shutdown()' function.
 * @endif
 * @param conn connector of GIOP_Connection data 
 * @return void
 */
void GIOP_Connection__close(GIOP_Connection *conn);

/*!
 * @if jp
 * @brief [Unused(T.B.D)] GIOPコネクションによるループを開始する。内部で'server_socket_loop()'をcallする。(multi-Thread用)
 * @else
 * @brief [Unused(T.B.D)] Start roop action of GIOP connection. Call 'server_socket_loop()' function. (For multi-Thread)
 * @endif
 * @param conn connector of GIOP_Connection data
 * @return void
 */
void *GIOP_Connection__run(GIOP_Connection *conn);

/*!
 * @if jp
 * @brief コネクションのクローズ処理とメモリの開放処理を行う。内部で'GIOP_Connection__close()'を呼ぶ。
 * @else
 * @brief Release memory for GIOP connection, and close connection. Call 'GIOP_Connection__close()' function.
 * @endif
 * @param conn connector of GIOP_Connection data
 * @return void
 */
void GIOP_Connection__free(GIOP_Connection *conn);

#if 0
GIOP_Connection * newGIOP_ServerConnection(short port);
void shutdownGIOP_ServerConnection(GIOP_Connection *conn);
void deleteGIOP_Connection(GIOP_Connection *conn);
void runGIOP_Server_Connection(GIOP_Connection *conn);
#endif

/*!
 * @if jp
 * @brief GIOP_MessageHeader型のヘッダデータを作成する。メモリの確保を行い、magic(='GIOP'という文字)やversion、バイトオーダー用のフラグをセットする。
 * @else
 * @brief Create GIOP_MessageHeader data, allocate memory and set magic, version, flag as byte_order.
 * @endif
 * @param order byte_order (0:BigEndian,1:LittleEndian ... refer to define parts in 'giop-msg.h')
 * @param version minor version of GIOP
 * @return pointer of GIOP_MessageHeader data
 */
GIOP_MessageHeader *GIOP_Create_MessageHeader(int order, int version);

/*!
 * @if jp
 * @brief GIOPコネクションハンドラーのソケットで取得したデータを指定したサイズで'buf'に一旦格納し、GIOP_MessageHeader型データにコピーする。
 * @else
 * @brief Receive messages data from GIOP_ConnectionHandler, and set to 'buf' as octet data, and copy to GIOP_MessageHeader type's data.
 * @endif
 * @param h GIOP_ConnectionHandler data
 * @param header GIOP_MessageHeader data for setting data from 'buf'
 * @param buf buffer for putting data from 'h'
 * @param maxMsg max size of buffer for message (releated with defined values 'MaxMessageSize' or 'RECV_BUF_SIZE' ...)
 * @return size of header message (or '-1':error)
 */
int receiveMessage(GIOP_ConnectionHandler *h, GIOP_MessageHeader *header, octet *buf, int maxMsg);

/*!
 * @if jp
 * @brief 受信したGIOP_Requestメッセージと内部で作成した対応するGIOP_Replyメッセージの情報を元に、送信するGIOP_ReplyのHeader部を作成し、Body部と合わせてマーシャリングを行い、GIOP_Replyメッセージ送信(socketへの書き込み)を行う。
 * @else
 * @brief Make GIOP_Reply header parts using GIOP_Request messages and GIOP_Reply body part, marshal GIOP_Reply header and body parts, and send(write to socket) data as GIOP_Reply messages.
 * @endif
 * @param h GIOP_ConnectionHandler data
 * @param request_header Header of GIOP_Request messages
 * @param reply_body Body of GIOP_Reply messages
 * @param version minor version of GIOP
 * @return 1:Success, -1:Fail
 */
int reply_Message(GIOP_ConnectionHandler *h, GIOP_RequestHeader *request_header,
		GIOP_ReplyBody *reply_body, int version);

/*!
 * @if jp
 * @brief GIOP_Requestにより指定したサーバントを呼び出し、メソッドを実行する。レスポンスのためにGIOP_Replyメッセージを作成し、そのBODY部のデータを返す。
 * @else
 * @brief Call servant and remote method indicated by GIOP_Request message, and execute method. Create GIOP_Reply message for response. And return body part of GIOP_Reply message.
 * @endif
 * @param poa POA
 * @param header input data (GIOP_Request header part buffer) 
 * @param env CORBA env(Exception Infomation)
 * @param arg_buf input data (GIOP_Request body part buffer)
 * @param version minor version of GIOP
 * @param order byte_order(LittleEndian/BigEndian)
 * @return GIOP_ReplyBody data
 */
GIOP_ReplyBody *invokeServant(PortableServer_POA poa,
		GIOP_RequestHeader *header, 
		CORBA_Environment *env, octet *arg_buf, int version, int order);

/*!
 * @if jp
 * @brief keyで指定したオブジェクトが、指定したPOAのオブジェクトマップに登録されているかどうかチェックする。登録されていれば、'GIOP_OBJECT_HERE'を、されていなければ'GIOP_UNKNOWN_OBJECT'を返す。
 * @else
 * @brief Check whether object indicated 'key' is exist or not in object_map of indicated POA. Return following values, 'GIOP_OBJECT_HERE' as 'found' or 'GIOP_UNKNOWN_OBJECT' as 'not found'
 * @endif
 * @param poa POA
 * @param key object_key
 * @param ior (Unused)
 * @return 'GIOP_OBJECT_HERE':found / 'GIOP_UNKNOWN_OBJECT': not found
 */
int find_object(PortableServer_POA poa, char *key, char *ior);

/*!
 * @if jp
 * @brief GIOP_LocateRequestリクエストに対するGIOP_LocateReplyリプライをする。アンマーシャル済みのGIOP_LocateRequestHeader型メッセージデータから、ヘッダ情報や、オブジェクトの状態'locate_status'を取得し、GIOP_Reply型データを作成し、そのHeader部にデータをマーシャリングしてセットし、リクエストに対するリプライとして送信する。
 * @else
 * @brief Reply messages as GIOP_LocateReply for request of GIOP_LocateRequest. Get header info from unmarshaled data of GIOP_LocateRequestHeader, get 'locate_status', create GIOP_Reply type's data and marshal and set each parameters to it, and send messages as GIOP_LocateReply message.
 * @endif
 * @param poa POA
 * @param h GIOP_ConnectionHandler data
 * @param locate_request_header unmarshaled GIOP_LocateRequestHeader type's data
 * @param version minor version of GIOP
 * @return '1':Success, '-1':Fail
 */
int reply_locateMessage(PortableServer_POA poa, GIOP_ConnectionHandler *h,
		GIOP_LocateRequestHeader *locate_request_header, int version);

/*!
 * @if jp
 * @brief GIOP_Request型データを作成する。引数で与えられたHeader情報とBody部のデータを'MarshalRequest()'関数によりマーシャリングを行い、出力用のバッファ'buf'にセットする。セットしたGIOP_Request型データのサイズを返す。
 * @else
 * @brief Create GIOP_Request type's data. Get Header and Body data from arguments of this function, and marshal  by 'MarshalRequest()' function , and set them to 'buf', and return size of written GIOP_Request type's data.
 * @endif
 * @param buf target buffer for putting data and for sending GIOP_LocateRequest data
 * @param response 'response_expected' or 'response_flags' of GIOP_RequestHeader data for setting
 * @param object_key 'object_key' of GIOP_RequestHeader data for setting
 * @param object_key_len length of 'object_key'
 * @param operation 'operation' of GIOP_RequestHeader data for setting
 * @param operation_len length of 'operation'
 * @param args GIOP_Request body part buffer for setting to 'buf' 
 * @param args_len size of GIOP_Request body part('args') 
 * @param version minor version of GIOP
 * @return size of GIOP_Request data
 */
uint32_t createRequest(octet *buf, int response, char *object_key,
		int object_key_len, char *operation, int operation_len,
		octet *args, int args_len, int version);

/*!
 * @if jp
 * @brief メッセージタイプが'GIOP_CancelRequest'のGIOP_MessageHeader型データを作成する。データを'buf'にセットし、'request_id'を付与する。その[header+4byte]サイズを返す。
 * @else
 * @brief Create GIOP_MessageHeader type's data which type is 'GIOP_CancelRequest'. Set data to 'buf' and add 'request_id' to 'buf' and return size of header data + 4byte.
 * @endif
 * @param buf buffer for GIOP message header
 * @param request_id Request ID
 * @return size of GIOP header + 4byte('request_id')
 */
uint32_t createCancelRequest(octet *buf, uint32_t request_id);

/*!
 * @if jp
 * @brief 'object_key'で指定したオブジェクトKeyを持つオブジェクトの状態('locate_status')を問い合わせるための、GIOP_LocateRequest型メッセージデータを新規作成(マーシャル)し、そのデータサイズを返す。
 * @else
 * @brief Create GIOP_LocateRequest type's data for confirming the status('locate_status') of object which has 'object_key' and return size of it.
 * @endif
 * @param buf target buffer for putting data and for sending GIOP_LocateRequest data
 * @param object_key Object Key
 * @param len length of Object Key
 * @return size of GIOP_LocateRequest type's data
 */
int createLocateRequest(octet *buf, char *object_key, int len);

/*!
 * @if jp
 * @brief メッセージタイプが'GIOP_CloseConnection'のGIOP_MessageHeader型データを作成する。データを'buf'にセットし、そのheaderサイズを返す。
 * @else
 * @brief  Create GIOP_MessageHeader type's data which type is 'GIOP_CloseConnection'. Set data to 'buf' and return size of header data.
 * @endif
 * @param buf buffer for GIOP message header
 * @return size of GIOP header
 */
uint32_t createCloseConnectionMessage(octet *buf);

/*!
 * @if jp
 * @brief メッセージタイプが'GIOP_MessageError'のGIOP_MessageHeader型データを作成する。データを'buf'にセットし、そのheaderサイズを返す。
 * @else
 * @brief Create GIOP_MessageHeader type's data which type is 'GIOP_MessageError'. Set data to 'buf' and return size of header data.
 * @endif
 * @param buf buffer for GIOP message header
 * @return size of GIOP header
 */
uint32_t createMessageErrorMessage(octet *buf);

/*!
 * @if jp
 * @brief 'ior'で指定したオブジェクトの状態('locate_status')を問い合わせるための、GIOP_LocateRequest型メッセージデータをマーシャルして作成('createLocateRequest()'関数)し、リクエストを送信する。
 * @else
 * @brief Create GIOP_LocateRequest type's data and marshal data by 'createLocateRequest()' functuion, and send request, for confirming Location status('locate_status') of object indicated by 'ior'.
 * @endif
 * @param h GIOP_ConnectionHandler data
 * @param ior CORBA_URL type's IOR data
 * @return size of written request data, '-1':Fail to write
 */
int requestLocation(GIOP_ConnectionHandler *h, CORBA_URL *ior);

/*!
 * @if jp
 * @brief 'ior'で指定したオブジェクトの状態('locate_status')を問い合わせる。まず、GIOP_LocateRequest型メッセージデータをマーシャルして作成('createLocateRequest()'関数)し、リクエストを送信する。次に、GIOPメッセージデータを受信('receiveMessage()'関数)し、GIOP_LocateReply型メッセージとしてアンマーシャル('deMarshalLocateReply()'関数)して、オブジェクトの状態('locate_status')を取得する。取得したを'locate_status'返す。
 * @else
 * @brief Confirm Location status('locate_status') of object indicated by 'ior'. First, create GIOP_LocateRequest type's data and marshal data by 'createLocateRequest()' functuion, and send request. Next, receive GIOP message data by 'receiveMessage()' function, and unmarshal data and get 'locate_status' by 'deMarshalLocateReply()' function. Last, return 'locate_status' of object indicated by 'ior'.
 * @endif
 * @param h GIOP_ConnectionHandler data
 * @param ior CORBA_URL type's IOR data
 * @return locate_status in GIOP_LocateReplyHeader type (GIOP_UNKNOWN_OBJECT/GIOP_OBJECT_HERE/GIOP_OBJECT_FORWARD/GIOP_OBJECT_FORWARD_PERM/GIOP_LOC_SYSTEM_EXCEPTION/GIOP_LOC_NEEDS_ADDRESSING_MODE/GIOP_LocationStatusType_END)
 */
int confirmLocation(GIOP_ConnectionHandler *h, CORBA_URL *ior);

/*!
 * @if jp
 * @brief GIOP Communication(T.B.D)
 * @else
 * @brief GIOP Communication(T.B.D)
 * @endif
 * @param h GIOP_ConnectionHandler data
 * @return 1:OK or Active, -1:Fail or Closed
 */
int RecvMessage(GIOP_ConnectionHandler *h);

/*!
 * @if jp
 * @brief GIOP_Request_Item型のデータのメモリ開放する。
 * @else
 * @brief Release memory of GIOP_Request_Item type's data.
 * @endif
 * @param arg arg
 * @return void
 */
void free_request(void *arg);

/*!
 * @if jp
 * @brief リクエストの追加処理を行う。
 * 
 *          呼び出し時にGIOPメッセージ受信用バッファを確保しデータを受信する。POAオブジェクトのrequestsに、
 *        リクエストを追加するために、GIOP_Request構造体 request を新たに生成する。\n
 *          GIOPのメッセージ本体を受信するために、"request->buf"に受信データ保存用のバッファを確保し、
 *        GIOPメッセージをコピーする。その後に、requestを　poa->requestsの末尾に追加し、最初に確保した
 *        データ受信用バッファを削除する。注意点は、この関数は、リモートからの呼び出しメッセージを受信し、
 *        Queueに入れることしかしない。実際の関数呼び出しは、リクエストのあったすべてのSocketポートから
 *        メッセージを受信し終わった後に、逐次的に関数呼び出しが行われる。このことは、関数呼び出しの実効時に、
 *        別のリモートオブジェクトに関数を呼び出し、待ち状態に入ってしまうと、他からのリクエスト処理でできない
 *        ことを意味する。このことは、Aが実装側、Bが呼び出し側の場合に、BがAのmethodを呼び出している処理内で、
 *        AがBにリモート呼出をしてしまうとデッドロック状態に陥ってしまう。そのため、関数呼び出しは、直ちに返る必要がある。
 * @else
 * @brief Receive request and add to enqueue.
 * @endif
 * @param h GIOP_ConnectionHandler
 * @param lst list of POA requests
 * @return list of POA requests(added)
 */
PtrList *GIOP_enqueue_request(GIOP_ConnectionHandler *h, PtrList *lst);

/*!
 * @if jp
 * @brief 各POAのリストに格納された全リクエストに対する処理を行う。取得したGIOPメッセージタイプを解釈し、各々のタイプ毎の実行用の関数をCallする。スレッド使用の場合は、Mutextのロック/アンロックも行う。１つのリクエストが終了したら、'release'フラグを終了を意味する'1'にセットして次のリクエスト処理に移る。最後にメモリリリースと、リクエスト用のリスト(queue)から終了済みのリクエスト項目を削除する。
 * @else
 * @brief Execute action of GIOP request from Client in POA. Analyze GIOP messages's type, and call each type  functions releated with GIOP message's type. If use THREAD, do action of MUTEX(rock/unrock). After doing 1 request, set 'release' flag to '1'(it means done.), and move to next request. Last, release memory and delete item of request list which 'release' is '1'. 
 * @endif
 * @param poa POA
 * @param lst requests list in POA (poa->requests)
 * @return rest of requests list in POA after execution
 */
PtrList *GIOP_execute_request(PortableServer_POA poa, PtrList *lst);

#endif
