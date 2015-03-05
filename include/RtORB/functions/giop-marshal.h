/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 * RtORB: giop-marshal.h
 *   IIOP-GIOP
 *
 *  Copyright(C) 2006, Isao Hara, AIST
 *
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/01/08 07:56:06 $
 *  $Id: giop-marshal.h,v 1.1.1.1 2008/01/08 07:56:06 yoshi Exp $
 */
/*!
 * @file giop-marshal.h
 * @if jp
 * @brief GIOP関連データのマーシャリング/アンマーシャリング処理。
 * @else
 * @brief GIOP Marshaling and deMarshaling actions.
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __FUNC_GIOP_MARSHAL_H__
#define __FUNC_GIOP_MARSHAL_H__

/*!
 * @if jp
 * @brief メモリのアライメント処理を行う。'base'で指定したサイズの倍数に現在の位置'current'が来るようにオフセットさせる。
 * @else
 * @brief Action of memory alignment. Move 'current' position by adding offset value calculated by 'base' value.
 * @endif
 * @param current current position of buffer. (address) (next writable)
 * @param base size for alignment
 * @return void
 */
void Address_Alignment(int *current, int base);

/*!
 * @if jp
 * @brief ポインタアドレスのアライメント処理を行う。
 * @else
 * @brief Action of pointer address alignment. 
 * @endif
 * @param val current position of pointer address 
 * @param base size for alignment
 * @return aligned position of pointer address  
 */
void *Align_Pointer_Address(void *val, uint32_t base);

/*!
 * @if jp
 * @brief CORBA_TypeCode型データの種類に応じたサイズを返す。
 * @else
 * @brief Return each kind of size of CORBA_TypeCode data. 
 * @endif
 * @param tc CORBA_TypeCode data 
 * @return size of CORBA_TypeCode data
 */
uint32_t  size_of_typecode(CORBA_TypeCode tc, int flag);

/*!
 * @if jp
 * @brief CORBA_TypeCode型データの種類に応じたAlignmentを返す。
 * @else
 * @brief Return each kind of alignment of CORBA_TypeCode data. 
 * @endif
 * @param tc CORBA_TypeCode data 
 * @return alignment of CORBA_TypeCode data
 */
uint32_t align_of_typecode(CORBA_TypeCode tc, int flag);

/*!
 * @if jp
 * @brief Octet型データを'size'分だけコピーする。
 * @else
 * @brief Copy octet type data.
 * @endif
 * @param dist target buffer for coping data (after copy)
 * @param buf reference buffer for coping (before copy)
 * @param size size of copied data
 * @param current current position of buffer.
 * @return void
 */
int copy_octet(void *dist, octet *buf, int size, int *current);

/*
*
*  marshal
*/
/*!
 * @if jp
 * @brief 'val'のデータ(1byte)をバッファ'buf'の'pos'の位置のアドレスに書き込む。さらに、'pos'位置を次回のために1byte分進める。
 * @else
 * @brief Write 1byte data of 'val' to 'buf' which position is 'pos', and add 1 to 'pos' for next.
 * @endif
 * @param buf target buffer for putting data (octet)
 * @param pos next writable position of buffer (address)
 * @param val data for writing to 'buf'
 * @return void
 */
int marshalOctet(octet *buf, int *pos, octet val);

/*!
 * @if jp
 * @brief 'val'のデータを先頭から2byteだけバッファ'buf'の'pos'の位置のアドレスに書き込む。さらに、'pos'位置を次回のために2byte分進める。
 * @else
 * @brief Write 2byte data of 'val' to 'buf' which position is 'pos', and add 2 to 'pos' for next.
 * @endif
 * @param buf target buffer for putting data (octet)
 * @param pos next writable position of buffer (address)
 * @param val data for writing to 'buf'
 * @return void
 */
int marshalShort(octet *buf, int *pos, short val);

/*!
 * @if jp
 * @brief 'val'のデータを先頭から4byteだけバッファ'buf'の'pos'の位置のアドレスに書き込む。さらに、'pos'位置を次回のために4byte分進める。
 * @else
 * @brief Write 4byte data of 'val' to 'buf' which position is 'pos', and add 4 to 'pos' for next.
 * @endif
 * @param buf target buffer for putting data (octet)
 * @param pos next writable position of buffer (address)
 * @param val data for writing to 'buf'
 * @return void
 */
int marshalLong(octet *buf, int *pos, int32_t val);

/*!
 * @if jp
 * @brief 'val'のデータを先頭から4byteだけバッファ'buf'の'pos'の位置のアドレスに書き込む。さらに、'pos'位置を次回のために4byte分進める。
 * @else
 * @brief Write 4byte data of 'val' to 'buf' which position is 'pos', and add 4 to 'pos' for next.
 * @endif
 * @param buf target buffer for putting data (octet)
 * @param pos next writable position of buffer (address)
 * @param val data for writing to 'buf'
 * @return void
 */
int marshalFloat(octet *buf, int *pos, float val);

/*!
 * @if jp
 * @brief 'val'のデータを先頭から8byteだけバッファ'buf'の'pos'の位置のアドレスに書き込む。さらに、'pos'位置を次回のために8byte分進める。
 * @else
 * @brief Write 8byte data of 'val' to 'buf' which position is 'pos', and add 8 to 'pos' for next.
 * @endif
 * @param buf target buffer for putting data (octet)
 * @param pos next writable position of buffer (address)
 * @param val data for writing to 'buf'
 * @return void
 */
int marshalDouble(octet *buf, int *pos, double val);

/*!
 * @if jp
 * @brief まず文字列'str'データの長さを取得する。文字列長の値を4byteで、文字列'str'データを文字列サイズ分だけ、バッファ'dist'の'pos'の位置のアドレスに各々書き込む。さらに、'pos'位置を次回のために[4+文字列サイズ]byte分進める。
 * @else
 * @brief First, get length(size) of 'str'. Next, Write 4byte data of string_length value and 'string_length'byte data of 'str' to 'buf' which position is 'pos', and add [4 + size of 'str'] to 'pos' for next.
 * @endif
 * @param dist target buffer for putting data (octet)
 * @param pos next writable position of buffer (address)
 * @param str string data for writing to 'dist'
 * @return next writable position of buffer (address)
 */
int marshalString(octet *dist, int *pos, char *str);

/*!
 * @if jp
 * @brief 'str->_legth'の値を4byteで、文字列'str'データを'str->_legth'サイズ分だけ、バッファ'dist'の'pos'の位置のアドレスに各々書き込む。さらに、'pos'位置を次回のために[4+'str->_legth'サイズ]byte分進める。
 * @else
 * @brief Write 4byte data of 'str->_legth' value and 'str->_legth'byte data of 'str' to 'buf' which position is 'pos', and add [4 + 'str->_legth'] to 'pos' for next.
 * @endif
 * @param dist target buffer for putting data (octet)
 * @param pos next writable position of buffer (address)
 * @param str CORBA_String(CORBA_Sequence_Octet) string data for writing to 'dist'
 * @return next writable position of buffer (address))
 */
int marshal_CORBA_String(octet *dist, int *pos, CORBA_String *str);

/*!
 * @if jp
 * @brief 'size'の値を4byteで、octet型'data'データを'size'サイズ分だけ、バッファ'buf'の'pos'の位置のアドレスに各々書き込む。さらに、'pos'位置を次回のために[4+'size'サイズ]byte分進める。
 * @else
 * @brief Write 4byte data of 'size' value and 'size'byte data of 'data' to 'buf' which position is 'pos', and add [4 + 'size'] to 'pos' for next.
 * @endif
 * @param buf target buffer for putting data (octet)
 * @param pos next writable position of buffer (address)
 * @param data data for writing to 'buf' 
 * @param size size of 'data'
 * @return void
 */
int marshal_Octet_Sequence(octet *buf, int *pos, octet *data, int size);

/*!
 * @if jp
 * @brief まず'seq'データの全体サイズを取得する。全体サイズの値を4byteで、'seq'データをTypeCodeに対応したサイズとそのseq長を掛けたサイズ分(=size)だけ、バッファ'dist'の'pos'の位置のアドレスに各々書き込む。さらに、'pos'位置を次回のために[4+size]byte分進める。
 * @else
 * @brief First, get total_size of 'seq'. Next, Write 4byte data of total_size value and 'size(legth*sizeByTypecode)'byte data of 'seq' to 'buf' which position is 'pos', and add [4 + 'size'] to 'pos' for next.
 * @endif
 * @param buf target buffer for putting data (octet)
 * @param pos next writable position of buffer (address)
 * @param seq CORBA_Sequence type data for writing to 'buf' 
 * @return void
 */
int marshal_CORBA_Sequence(octet *buf, int *pos, CORBA_Sequence *seq);

/*!
 * @if jp
 * @brief List内の'service_context'の数値numを4byteで、'service_context'データの中身の'context_id'と'context_data'をペアとして、バッファ'dist'の'pos'の位置のアドレスに各々num回のループ分書き込む。さらに、内部で'pos'位置を次回のために適宜進める。
 * @else
 * @brief Write 4byte data of 'service_context->num' value and each 'context_id' and 'context_data' in 'service_context' to 'buf' which position is 'pos'.
 * @endif
 * @param service_context service_context data of IOP_ServiceContextList type for writing to 'buf' 
 * @param buf target buffer for putting data (octet)
 * @param current next writable position of buffer (address)
 * @return void
 */
int marshal_IOP_SerivceContextList(IOP_ServiceContextList *service_context, octet *buf, int *current);

/* demarshal */
/*!
 * @if jp
 * @brief バッファ'buf'の'pos'位置にある1byte分のデータを読み取り、返す。さらに、'pos'位置を次回のために1byte分進める。
 * @else
 * @brief Read 1byte data from 'buf' which position is 'pos', and return it. And add 1 to 'pos' for next.
 * @endif
 * @param buf reference buffer for getting data
 * @param pos current readable position of buffer (address)
 * @return demarshled Octet type data
 */
octet demarshalOctet(char *buf, int *pos);

/*!
 * @if jp
 * @brief バッファ'buf'の'pos'位置にある2byte分のデータを読み取り、'byte_order'を見て、必要あればホストバイトオーダーにendianを変換してから、その値を返す。さらに、'pos'位置を次回のために2byte分進める。
 * @else
 * @brief Read 2byte data from 'buf' which position is 'pos', and check 'byte_order', convert endian, return it. And add 2 to 'pos' for next.
 * @endif
 * @param buf reference buffer for getting data
 * @param pos current readable position of buffer (address)
 * @param order byte_order(LittleEndian/BigEndian)
 * @return demarshled Short type data
 */
short demarshalShort(octet *buf, int *pos, int order);

/*!
 * @if jp
 * @brief バッファ'buf'の'pos'位置にある4byte分のデータを読み取り、'byte_order'を見て、必要あればホストバイトオーダーにendianを変換してから、その値を返す。さらに、'pos'位置を次回のために4byte分進める。
 * @else
 * @brief Read 4byte data from 'buf' which position is 'pos', and check 'byte_order', convert endian, return it. And add 4 to 'pos' for next.
 * @endif
 * @param buf reference buffer for getting data
 * @param pos current readable position of buffer (address)
 * @param order byte_order(LittleEndian/BigEndian)
 * @return demarshled Long type data
 */
int32_t demarshalLong(octet *buf, int *pos, int order);

/*!
 * @if jp
 * @brief 'byte_order'に応じた手順で、バッファ'buf'の'pos'位置にある4byte分のデータを読み取り、その値を返す。さらに、'pos'位置を次回のために4byte分進める。
 * @else
 * @brief Read 4byte data from 'buf' which position is 'pos', and check 'byte_order', return it. And add 4 to 'pos' for next.
 * @endif
 * @param buf reference buffer for getting data
 * @param pos current readable position of buffer (address)
 * @param order byte_order(LittleEndian/BigEndian)
 * @return demarshled Float type data
 */
float demarshalFloat(octet *buf, int *pos, int order);

/*!
 * @if jp
 * @brief 'byte_order'に応じた手順で、バッファ'buf'の'pos'位置にある8byte分のデータを読み取り、その値を返す。さらに、'pos'位置を次回のために8byte分進める。
 * @else
 * @brief Read 8byte data from 'buf' which position is 'pos', and check 'byte_order', return it. And add 8 to 'pos' for next.
 * @endif
 * @param buf reference buffer for getting data
 * @param pos current readable position of buffer (address)
 * @param order byte_order(LittleEndian/BigEndian)
 * @return demarshled Double type data
 */
double demarshalDouble(octet *buf, int *pos, int order);

/*!
 * @if jp
 * @brief まず、stringのサイズ[size]を'src'の先頭4byteから取得し、dmarshal用にメモリをその[size]で確保する。バッファ'src'の'pos+4'位置にある[size]byte分のデータを読み取り、その値を返す。また、引数のsize値のセット、'pos'位置を次回のために<4+size>byte分進める。
 * @else
 * @brief First read 4byte data as <string_size>(byte) value from 'src' which position is 'pos'. Next, alocate memory for <string_size>(byte) and set string data there and return it. And add <4+string_size> to 'pos' for next. And set <string_size> to 'size'.
 * @endif
 * @param src reference buffer for getting data
 * @param pos current readable position of buffer (address)
 * @param byte_order byte_order(LittleEndian/BigEndian)
 * @param size size of string
 * @return demarshled string data
 */
char *demarshalString(octet *src, int *pos, int byte_order, int *size);

/*!
 * @if jp
 * @brief まず、stringのサイズ[size]を'src'の先頭4byteから取得し、CORBA_String型データの'_length','_maximum'にセットする。 dmarshal用にメモリをその[size]で'_buffer'に確保する。バッファ'src'の'pos+4'位置にある[size]byte分のデータを読み取り'_buffer'にセットし、その値を返す。さらに、'pos'位置を次回のために<4+size>byte分進める。
 * @else
 * @brief First read 4byte data as <string_size>(byte) value from 'src' which position is 'pos' and set it to  '_length' and '_maximum' in CORBA_String data. Next, alocate memory for <string_size>(byte) and set string data to '_buffer' in CORBA_String data and return it. And add <4+string_size> to 'pos' for next. And set <string_size> to 'size'.
 * @endif
 * @param str CORBA_String data for setting data from 'src' (after)
 * @param src reference buffer for getting data (before)
 * @param pos current readable position of buffer (address)
 * @param order byte_order(LittleEndian/BigEndian)
 * @return Always '1' (T.B.D)
 */
void demarshal_CORBA_String(CORBA_String *str, octet *src, int *pos, int order);

/*!
 * @if jp
 * @brief 'context'データの中身の'context_id'と'context_data'を各々取得し、4byteでメモリのアライメントをする。
 * @else
 * @brief Get 'context_id' and 'context_data' in 'context' data , and align memory by 4byte.
 * @endif
 * @param context context data of IOP_ServiceContext type for reading from 'buf' 
 * @param buf reference buffer for getting data
 * @param current current readable position of buffer (address)
 * @param order byte_order(LittleEndian/BigEndian)
 * @return void
 */
void demarshal_IOP_SerivceContext(IOP_ServiceContext *context,
		octet *buf, int *current, int order);

/*!
 * @if jp
 * @brief List内の'service_context'の数値numを4byteで、'service_context'データの中身の'context_id'と'context_data'をペアとして、バッファ'dist'の'pos'の位置のアドレスに各々num回のループ分書き込む。さらに、内部で'pos'位置を次回のために適宜進める。
 * @else
 * @brief Write 4byte data of 'service_context->num' value and each 'context_id' and 'context_data' in 'service_context' to 'buf' which position is 'pos'.
 * @endif
 * @param service_context service_context data of IOP_ServiceContextList type for writing to 'buf' 
 * @param buf target buffer for putting data (octet)
 * @param current next writable position of buffer (address)
 * @return void
 */
/*!
 * @if jp
 * @brief まず、'buf'の先頭4byteからList内の'service_context'の数値numを取得する。 num回のループ分、バッファ'buf'の'pos+4'の位置のアドレスから開始して、'service_context'データの中身の'context_id'と'context_data'の取得処理を行う。
 * @else
 * @brief First get 'service_context->num' value from first 4byte data. Next get each 'context_id' and 'context_data' in 'service_context' 
 * @endif
 * @param service_context service_context data of IOP_ServiceContextList type for reading from 'buf' 
 * @param buf reference buffer for getting data
 * @param current current readable position of buffer (address)
 * @param order byte_order(LittleEndian/BigEndian)
 * @return void
 */
void demarshal_IOP_SerivceContextList(IOP_ServiceContextList *service_context,
		octet *buf, int *current, int order);

/*****************************/
/*!
 * @if jp
 * @brief 入力データ'buf'に対し、各TypeCodeに応じたアンマーシャリングを行い、結果を'dist'にセットする。
 * @else
 * @brief Unmarshal input data 'buf' considering several kinds of TypeCode, and set unmarshaled data to 'dist'.
 * @endif
 * @param dist target buffer for setting data (result)
 * @param tc CORBA_TypeCode data
 * @param buf reference buffer for getting data (input)
 * @param current current readable position of buffer (address)
 * @param order byte_order(LittleEndian/BigEndian)
 * @return size of CORBA_TypeCode data
 */
uint32_t demarshal_by_typecode(void **dist, CORBA_TypeCode tc, octet *buf, int *current, int order);

/*!
 * @if jp
 * @brief サーバーサイドにおける、CORBAオペレーション宣言のパラメータタイプ(in/out/inout)に応じたアンマーシャリング処理。
 * @else
 * @brief Demarshal arguments(in/out/inout) on server side considering parameter types of CORBA operation definition. This function called from 'invokeServant()'. 
 * @endif
 * @param buf reference buffer for getting data (input)
 * @param i_args counts of arg
 * @param i_argv CORBA_IArg data
 * @param order byte_order(LittleEndian/BigEndian)
 * @return pointer of unmarshaled data (output)
 */
void ** deMarshal_Arg(octet *buf, int i_args, CORBA_IArg *i_argv, int order);

/*!
 * @if jp
 * @brief クライアントサイドにおいて、まず、入力データ'buf'の先頭から、各TypeCodeに応じたアンマーシャリングを行い、結果を'retval'にセットし、次に、'buf'の続きにあるデータに対して、CORBAオペレーション宣言のパラメータタイプに応じたアンマーシャリング処理を行い、結果を'args'にセットする。
 * @else
 * @brief Demarshal result and argumnets on client side. This function called from 'invokeMethod_via_GIOP()'.
 * @endif
 * @param retval unmarshaled data (result)
 * @param args arguments of Method   (result)
 * @param buf reference buffer for getting data (input)
 * @param method Method(CORBA_Class_Method)
 * @param order byte_order(LittleEndian/BigEndian)
 * @return void
 */
void deMarshal_Arguments(void **retval, void **args, octet *buf, CORBA_Class_Method *method, int order);

/*****************************/
/*!
 * @if jp
 * @brief 入力データ'argv'をCORBA_TypeCode毎に場合分けして、マーシャリングし、出力用バッファ'buf'に対し書き込む。
 * @else
 * @brief Marshal input data 'argv' considering several kinds of CORBA_TypeCode, and set marshaled data to 'buf'.
 * @endif
 * @param buf target buffer for putting data (octet)
 * @param argv data for writing to 'buf' 
 * @param tc CORBA_TypeCode data for marshaling
 * @param current current writable position of buffer (address) 
 * @return 'current' value / '0':means none or Null or void
 */
int marshal_by_typecode(octet *buf, void *argv, CORBA_TypeCode tc, int *current);

/*!
 * @if jp
 * @brief CORBAオペレーション宣言のパラメータタイプ(in/out/inout)に応じたマーシャリング処理。入力データ'argv'を'buf'にセットし、そのデータサイズを返す。(GIOP_Replyメッセージ本体用)
 * @else
 * @brief Marshal arguments(in/out/inout) considering parameter types of CORBA operation definition. Set marshaled 'argv' data to 'buf', and return size of it. (For writing to body of GIOP_Reply messages)
 * @endif
 * @param buf target buffer for putting data (octet)
 * @param argv data for writing to 'buf' 
 * @param i_args counts of arg
 * @param i_argv CORBA_IArg data
 * @return data size of marshaled 'argv' data 
 */
int Marshal_Arg(octet *buf, void * *argv, int i_args, CORBA_IArg *i_argv);

/*!
 * @if jp
 * @brief 入力データ'result'をCORBA_TypeCode毎に場合分けして、マーシャリングし、出力用バッファ'reply_buf'に対し書き込む。(GIOP_Replyメッセージ本体用)
 * @else
 * @brief Marshal input data 'result' considering several kinds of CORBA_TypeCode, and set marshaled data to 'reply_buf'. (For writing to body of GIOP_Reply messages)
 * @endif
 * @param reply_buf target buffer for putting data
 * @param result data for writing to 'reply_buf' 
 * @param tc CORBA_TypeCode data for marshaling
 * @param size data size(= current readable position of buffer (address))
 * @return void
 */
void Marshal_Result(char *reply_buf, void * *result, CORBA_TypeCode tc, int *size);

/*!
 * @if jp
 * @brief サーバーサイドにおいて、入力データをマーシャリングして、GIOP_Replyメッセージ本体を作成する。必要なメモリを確保した後、内部から'Marshal_Result()'と'Marshal_Arg()'関数を呼び、必要データをマーシャリングしてGIOP_Replyメッセージ用データをセットする。
 * @else
 * @brief On server side, marshal input data and create GIOP_Reply message's body data. First, allocate memory by maxsize for GIOP message, next, call 'Marshal_Result()' and 'Marshal_Arg()' functions for marshaling input data and set result to buffer. 
 * @endif
 * @param reply GIOP_ReplyBody type's data for putting marshaled data 
 * @param result data for writing to 'reply' in 'Marshal_Result()' function
 * @param argv data for writing to 'reply' in 'Marshal_Arg()' function
 * @param m CORBA_Class_Method data for writing each 'argv' elements
 * @return void
 */
void Marshal_Reply_Arguments(GIOP_ReplyBody *reply, void * *result,
			void * *argv, CORBA_Class_Method *m);



#endif
