/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 *  RtORB:
 *  	iop.h
 *  Copyright(C) 2006, Isao Hara, AIST
 *
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/01/08 07:56:06 $
 *  $Id: iop.h,v 1.1.1.1 2008/01/08 07:56:06 yoshi Exp $
 */
/*!
 * @file iop.h
 * @if jp
 * @brief IOP(Inter-Orb-Protocol)、IOR、Parse処理などの定義。
 * @else
 * @brief Definition of IOP(Inter-Orb-Protocol), IOR, parsing, etc..
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __FUNC_IOP_H__
#define __FUNC_IOP_H__

/*!
 * @if jp
 * @brief Internet_IOPタグを生成する。ORBタイプ、Codeセットデータ、HostのIPアドレス、socketのPORT番号、オブジェクトkeyをマーシャリングしてセットする。
 * @else
 * @brief Create Internet_IOP tag data. Marshal and Set Orb-type, CodeSet data, IP address of remote host, PORT number of assigned to Socketm object key, ... etc.
 * @endif
 * @param dist buffer for setting Internet_IOP tag data here
 * @param sock socket file descriptor
 * @param object_key Object Key
 * @return size of IIOP tag data
 */
int Create_Tag_Internet_IOP(char *dist, int sock, char *object_key);

/*!
 * @if jp
 * @brief デフォルトIORの作成。4Kbyteのメモリを確保し、Internet_IOPタグを生成する。
 * @else
 * @brief Create default IOR data. Allocate 4096 byte's memory, and create Internet_IOP tag data.
 * @endif
 * @param buf buffer for setting IOR data here
 * @param sock socket file descriptor
 * @param typedId Object Typed ID
 * @param object_key Object Key
 * @return size of IOR
 */
int Create_Default_IOR(char *buf, int sock, char *typedId, char *object_key);

/*
int deMarshal_IOR(IIOP_IOR *ior_data, octet *ior, int len, int byte_order);
*/

/*!
 * @if jp
 * @brief Sequence_Octet型データを上位4bit下位4bitの順で分割し、かつSequence_String(Char_Array)データに変換する。
 * @else
 * @brief Convert Sequence_Octet data to Sequence_String(Char_Array) data.
 * @endif
 * @param dist Sequence_String data
 * @param src Sequence_Octet data
 * @param size array size of Sequence_Octet data
 * @return array size of Sequence_String data
 */
int Sequence_Octet_to_String(char *dist, char *src, int size);

/*!
 * @if jp
 * @brief Long型データをString(Char)型データに変換する。
 * @else
 * @brief Convert Long data to Strig(Char) data.
 * @endif
 * @param dist String data
 * @param val Long data
 * @return array size of String data aray
 */
int Long_to_String(char *dist, int32_t val);

/*!
 * @if jp
 * @brief CORBA_Sequence_Octet型データをSequence_String(Char_Array)データに変換する。
 * @else
 * @brief Convert CORBA_Sequence_Octet data to Sequence_String(Char_Array) data.
 * @endif
 * @param dist String data
 * @param src Sequence_Octet data
 * @param size array size of Sequence_Octet data
 * @return size of String data aray
 */
int CORBA_Sequence_Octet_to_String(char *dist, char *src, int size);

/*!
 * @if jp
 * @brief IORを新規作成メイン処理。4Kbyteのメモリを確保し、接頭辞(IOR:)+Bytecode(8char)のセット、デフォルトIOR値のセットする。
 * @else
 * @brief maing action of Creating IOR(Inter) data. Allocate 4096 byte's memory, and set header('IOR:00000000'), and set default IOR data.
 * @endif
 * @param buf buffer for setting IOR data here
 * @param sock socket file descriptor
 * @param typedId Object Typed ID
 * @param object_key Object Key
 * @return '1' (Unused)
 */
int createIOR(char *buf, int sock, char *typedId, char *object_key);

/*
int parseIOR_From_String(IIOP_IOR *ior_data, char *ior);
*/

/*!
 * @if jp
 * @brief CORBA_URL型データを指定したサイズの配列としてメモリ確保し、各要素の初期値をセットする。
 * @else
 * @brief Allocate memory for CORBA_URL struct array data by indicated size of array, and initialize each elments data.
 * @endif
 * @param n size of CORBA_URL array data
 * @return poiter of CORBA_URL array data
 */
CORBA_URL * CORBA_URL__alloc(int32_t n);

/*!
 * @if jp
 * @brief CORBA_URL型データを破棄し、メモリ開放する。
 * @else
 * @brief Destroy CORBA_URL data, release memory, delete data.
 * @endif
 * @param url poiter of CORBA_URL array data
 * @param n size of CORBA_URL array data
 * @return void
 */
void CORBA_URL__delete(CORBA_URL *url, int32_t n);

/*!
 * @if jp
 * @brief IORデータのparseを行う。
 * @else
 * @brief Parsing IOR data.
 * @endif
 * @param data CORBA URL data
 * @param ior IOR data (octet type)
 * @param pos position of data for next operation (address)
 * @param byte_order Byte Order for using in demarshal function as a flag for judgement network-byte-order or host-byte-order. 
 * @return number of CORBA_URL data
 */
int parseIOR(CORBA_URL **data, octet *ior, int *pos, int byte_order);

/*!
 * @if jp
 * @brief 入力値の中で区切り文字により、次のトークンを探しそのアドレスをセットする。
 * @else
 * @brief Search next token by delimiter from input data, and set that pointer. 
 * @endif
 * @param dist next token data
 * @param str input data
 * @param seed delimiter for seraching next token.
 * @return input data
 */
char *nextToken(char **dist, char *str, const char *seed);

/*!
 * @if jp
 * @brief 入力のstrデータを各syntax(Corbaloc,Corbaname)に従いparse(解析)し、CORBA_URL型のtargetデータ内のパラメータにセットする。
 * @else
 * @brief Parsing input data 'str' by syntax of Corbaloc or Corbaname, and set each data to CORBA_URL data 'target'.
 * @endif
 * @param target CORBA URL data
 * @param str Token data including '_url' by extractURL()
 * @return void
 */
void parseURL(CORBA_URL *target, char *str);

/*!
 * @if jp
 * @brief 入力文字列から、区切り文字を用いて、URLデータを抽出する。
 * @else
 * @brief Extract URL data using delimiter character from input string.
 * @endif
 * @param dist Token data including '_url'
 * @param str string data for parsing (URL schema data. type is IOR, Corbaloc, Corbaname, other.)
 * @param num number of CORBA_URL data
 * @param delim delimiter for seraching next token.
 * @return pointer of strings after delimiter (It means object_key, NameContext, ...)
 */
char * extractURL(char **dist, char *str, int *num, const char *delim);

/*!
 * @if jp
 * @brief Corbaloc形式のURLスキーマデータをCORBA_URL型のデータにparseし、dataに格納し、CORBA_URLデータ数を返す。
 * @else
 * @brief Parsing 'Corbaloc' type's URL schema data to CORBA_URL data, and set each elements to 'data' and return the 'number' of CORBA_URL data.
 * @endif
 * @param data CORBA URL data
 * @param str Corbaloc URL data for parsing
 * @return number of CORBA_URL data
 */
int parseCorbalocURL(CORBA_URL **data, char *str);

/*!
 * @if jp
 * @brief Corbaname形式のURLスキーマデータをCORBA_URL型のデータにparseし、dataに格納し、CORBA_URLデータ数を返す。
 * @else
 * @brief Parsing 'Corbaname' type's URL schema data to CORBA_URL data, and set each elements to 'data' and return the 'number' of CORBA_URL data.
 * @endif
 * @param data CORBA URL data
 * @param str Corbaname URL data for parsing
 * @return number of CORBA_URL data
 */
int parseCorbanameURL(CORBA_URL **data, char *str);

/*!
 * @if jp
 * @brief URLスキーマとしての入力stringデータを、IOR/Corbaloc/Corbaname各々のタイプにより場合分けして、CORBA_URLデータにParse(解析/分解)するメイン処理。
 * @else
 * @brief Main action of parsing URL schema(IOR/Corbaloc/Corbaname) data to CORBA_URL data.
 * @endif
 * @param data CORBA URL data
 * @param str string data for parsing (URL schema data. type is IOR, Corbaloc, Corbaname, other.)
 * @return number of CORBA_URL data
 */
int parseCorbaURL(CORBA_URL **data, char *str);

#endif
