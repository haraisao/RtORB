/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 * RtORB: util.h
 *
 * Copyright(C) 2006, Isao Hara, AIST
 *
 * $Revision: 1.1.1.1 $
 * $Date: 2008/01/08 07:56:06 $
 * $Id: util.h,v 1.1.1.1 2008/01/08 07:56:06 yoshi Exp $
 *
 */
/*!
 * @file util.h
 * @if jp
 * @brief 各種ユーティリティ用関数群
 * @else
 * @brief Various utility functions
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __UTIL_H__
#define __UTIL_H__

#ifdef __cplusplus
extern "C"
{
#endif 
/******* GIOP ************/
/*!
 * @if jp
 * @brief read()関数によるデータの読み込みを行う。
 * @else
 * @brief Read data by 'read()'.
 * @endif
 * @param fd File Handle
 * @param buf buffer for putting data
 * @param len data size
 * @return 1:Success, -1:Fail
 */
int32_t readBytes(int32_t fd, char *buf, int32_t len);

/*!
 * @if jp
 * @brief write()関数によるデータの書き込みを行う。
 * @else
 * @brief Write data by 'write()'.
 * @endif
 * @param fd File Handle
 * @param buf buffer for getting data
 * @param len data size
 * @return 1:Success, -1:Fail
 */
int32_t writeBytes(int32_t fd, char *buf, int32_t len);

/*  Object ID  */
/*!
 * @if jp
 * @brief Object IDを作成する。
 * @else
 * @brief Create Object_ID.
 * @endif
 * @return ID
 */
char *new_ObjectID();

/*****************************************/
/* DEBUG  */
/*!
 * @if jp
 * @brief 'buf'のデータを16進数書式で8文字毎の改行付きで出力する。 (デバッグ用)
 * @else
 * @brief Dump data as Hex data. (for DEBUG)
 * @endif
 * @param buf data for dumping
 * @param size size
 * @return void
 */
void  dumpMessage(unsigned char *buf, int32_t size);

/*!
 * @if jp
 * @brief 'val'データをCORBAタイプコードにより、書式を場合分けして出力する。(デバッグ用)
 * @else
 * @brief Dump data as various format related with CORBA_TypeCode. (for DEBUG)
 * @endif
 * @param val data for dumping
 * @param tc CORBA Type Code
 * @return void
 */
void dump_value_by_typecode(void *val, CORBA_TypeCode tc);

/*****************************************/
/*!
 * @if jp
 * @brief 文字に該当する[0-15]の数値を算出する。ASCII文字コード参照。
 * @else
 * @brief Calculate digital data ([0-15]) releated character. 
 * @endif
 * @param a Character
 * @return [0-15]の数値
 */
char ixdigit(char a);

/*!
 * @if jp
 * @brief Hex型の文字に該当するASCII文字コードの10進数値を算出する。
 * @else
 * @brief Calculate digital data (ASCII Code) releated hex character. 
 * @endif
 * @param c Character
 * @return digital data
 */
char hexdigit(char c);

/*!
 * @if jp
 * @brief Character文字列をOctet型(8 binary bit)に変換する。2文字を4bitずつに分けて１つのOctet型にして変換して格納。
 * @else
 * @brief Convert String to Octet.
 * @endif
 * @param str String
 * @return Octet data
 */
octet *String2Octet(char *str);

/*!
 * @if jp
 * @brief  Octet型(8 binary bit)をCharacter文字に変換し、文字列(配列)とする。
 * @else
 * @brief  Convert Octet to String.
 * @endif
 * @param str Octet data
 * @param len length of Octet data
 * @return character array (=String)
 */
char *Octet2String(octet *str, int len);

/*!
 * @if jp
 * @brief 'RtORB_free()'を用いて、メモリ領域を開放し、String文字列を削除する。
 * @else
 * @brief Delete String and release memory by using 'RtORB_free()' function.
 * @endif
 * @param str String
 * @param info message
 * @return void
 */
void String__delete(char *str, char *info);

/*!
 * @if jp
 * @brief 文字列の長さを返す。文字列が存在しない場合'0'を返す。
 * @else
 * @brief Return the length of String.
 * @endif
 * @param str String
 * @return length of String (if not exist, return '0')
 */
int32_t RtORB_strlen(char *str);

/*!
 * @if jp
 * @brief IP addressを取得する。
 * @else
 * @brief get IP address
 * @endif
 * @return IP address
 */
char *Get_IP_Address();

#ifdef __cplusplus
}
#endif 
#endif
