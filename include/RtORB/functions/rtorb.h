/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 *  rtorb.h
 *
 *
 */
/*!
 * @file rtorb.h
 * @if jp
 * @brief CORBA_Methodの呼び出しに関連した関数群。
 * @else
 * @brief Actions related with invoking CORBA_Methods.
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __FUNC_RTORB_H__
#define __FUNC_RTORB_H__

/*!
 * @if jp
 * @brief TypeCodeを元に、リモート呼び出し用メソッドの引数データのCDR符号化(マーシャリング)を行う。
 * @else
 * @brief Marshaling(convert to CDR) arg datas of method for remote call by typecode.
 * @endif
 * @param buf buffer
 * @param argv arguments of Method
 * @param i_args i_args (CORBA_Class_Method)
 * @param i_argv i_argv (CORBA_Class_Method)
 * @return length of data
 */
int Marshal_Args(octet *buf, void **argv, int i_args, CORBA_IArg *i_argv);

/*!
 * @if jp
 * @brief 送信したリクエストのReplyメッセージの取得とアンマーシャルして内容を解析する。
 * @else
 * @brief Get reply for sent request, and demarshal messages.
 * @endif
 * @param buf buffer
 * @param body body part of GIOP messaage
 * @param method Method(CORBA_Class_Method) (Unused)
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void actionReply(octet *buf, CORBA_Sequence_Octet *body,
		CORBA_Class_Method *method,
		CORBA_Environment *env);

/*!
 * @if jp
 * @brief GIOP経由でリモートオブジェクトにアクセスする。
 *
 *        make_client_connection関数を呼びGIOP_ConnectionHandlerを取得する。 オブジェクトの _url[0].location_flagが真でない場合には、その場所にリモートオブジェクトが存在するかをconfirmLocation関数を呼び出して、確認する。リモート呼び出しに必要な引数を Marshal_Args関数を使って　CDRの符号化する。createRequest関数を使って、GIOPリクエストを生成する。GIOP_ConnectionHandler_send関数を実行し、リモートオブジェクトにGIOPリクエストを送る。リモートオブジェクトからの返り値等が入るバッファを初期化する。リモートオブジェクトからの実行結果を　receiveMessage関数で得る。このとき返事がくるまでブロックされることになる。actionReply関数により返ってきたメッセージに応じた処理をする。env->_major が　CORBA_NO_EXCEPTION　であれば、エラーがなかったので、 deMarshal_Arguments関数を呼び出して、リプライメッセージから引数と返り値を復号化する。env->_major が　CORBA_USER_EXCEPTIONの場合には、エラー情報が返ってきているので、それを復号化する。使用した、通信用バッファを開放する。
 * @else
 * @brief Connect to the remote object, check location, marshar data and send request, receive reply and demarshal, etc. Call method of CORBA_Object via GIOP.      
 * @endif
 * @param obj CORBA Object
 * @param method Method(CORBA_Class_Method)
 * @param retval return value of Method
 * @param args arguments of Method
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void invokeMethod_via_GIOP(CORBA_Object obj, CORBA_Class_Method *method,
		void **retval, void **args, CORBA_Environment *env);

/*!
 * @if jp
 * @brief POAのオブジェクトマップから、Keyにマッチするオブジェクトが存在するかどうかを確認し、ある:1/なし:0を返す。
 * @else
 * @brief Check whehter CORBA object is exist or not using object key and POA object map. 
 * @endif
 * @param poa POA
 * @param obj CORBA Object
 * @param env CORBA env(Exception Infomation)
 * @return 1:Exist, 0:NoExist
 */
int CORBA_ORB_find_object(PortableServer_POA poa, CORBA_Object obj, CORBA_Environment *env);

/*!
 * @if jp
 * @brief オブジェクトのメソッドを呼び出す。
 *
 *        エラーハンドラのためにCORBA_Environmentをクリア、呼び出すオブジェクトがリモートなのかローカルなのかをチェック('CORBA_ORB_find_object()')し、リモートであれば'invokeMethod_via_GIOP()'をcallし、ローカルであれば、getValue関数でobject keyをもつオブジェクトをTableから呼び出し、対象となるオブジェクトから、呼び出す実装関数を見つけて、その実装関数を実行させる。
 * @else
 * @brief Call method of object. 
 *
 *        Clear 'env'. Check wheter object is exist in Local_Host or not(=Remote_Host) by 'CORBA_ORB_find_object()'. If in Remote_Host, call 'invokeMethod_via_GIOP()'. If in Local_Hosts, get object from object_map by object_key, find method, and execute it.
 * @endif
 * @param obj CORBA Object
 * @param method Method(CORBA_Class_Method) 
 * @param retval return value of Method
 * @param args arguments of Method
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void invokeMethod(CORBA_Object obj, CORBA_Class_Method *method,
		void **retval, void **args, CORBA_Environment *env);
#endif
