/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 *  RtORB: endian.h
 *    Simple GIOP Implementation for x86 (Little Endian)
 *
 *  Copyright(C) 2006, Isao Hara, AIST
 *
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/01/08 07:56:06 $
 *  $Id: endian.h,v 1.1.1.1 2008/01/08 07:56:06 yoshi Exp $
 *  
 */
/*!
 * @file endian.h
 * @if jp
 * @brief ByteOrder(Endian)の入れ替え(逆順)処理を行う。1Byte単位での入れ替えでBig<-->Littleで変換可能。
 * @else
 * @brief ByteOrder(Endian) action.
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

/*
#include <RtORB/corba.h>
*/
#ifndef __FUNC_ENDIAN_H__
#define __FUNC_ENDIAN_H__

/******* ByteOrder ************/
/*!
 * @if jp
 * @brief ByteOrder(Endian)ルールに基づき、1Byteずつデータをswapする(逆順に入れ替える)。 Big<-->Littleどちらにも対応。 (refer giop-marshal.c)
 * @else
 * @brief Swaping data by ByteOrder(Endian) rules.  (refer giop-marshal.c)
 * @endif
 * @param dist head pointer of Post-Swap data
 * @param source head pointer of Pre-Swap data
 * @param size Size by data_type
 * @return void
 */
void swapByteOrder(unsigned char *dist, unsigned char *source, int size);

/*!
 * @if jp
 * @brief Octet型データを1Byte単位で逆順にswapし、変換後のデータを返す。 (Unused)
 * @else
 * @brief Swaping octet-type-data and return post-data. (Unused)
 * @endif
 * @param buf target data (both pre/post)
 * @param size Size by data_type
 * @return void
 */
void swapByteOrderInOctet(char *buf, int size);

/*!
 * @if jp
 * @brief Short型データを1Byte単位で逆順にswapし、変換後のデータを返す。 (Unused)
 * @else
 * @brief Swaping short-type-data and return post-data. (Unused)
 * @endif
 * @param v Pre-Data
 * @return Post-Data
 */
short reverseOrderShort(short v);

/*!
 * @if jp
 * @brief Long型データを1Byte単位で逆順にswapし、変換後のデータを返す。(Unused)
 * @else
 * @brief Swaping long-type-data and return post-data. (Unused)
 * @endif
 * @param v Pre-Data
 * @return Post-Data
 */
int32_t reverseOrderLong(int32_t v);

/*!
 * @if jp
 * @brief LongLong型データを1Byte単位で逆順にswapし、変換後のデータを返す。(Unused)
 * @else
 * @brief Swaping longlong-type-data and return post-data. (Unused)
 * @endif
 * @param v Pre-Data
 * @return Post-Data
 */
int64_t  reverseOrderLongLong(int64_t  v);

/*!
 * @if jp
 * @brief Float型データを1Byte単位で逆順にswapし、変換後のデータを返す。(Unused)
 * @else
 * @brief Swaping float-type-data and return post-data. (Unused)
 * @endif
 * @param v Pre-Data
 * @return Post-Data
 */
float reverseOrderFloat(float v);

/*!
 * @if jp
 * @brief Double型データを1Byte単位で逆順にswapし、変換後のデータを返す。(Unused)
 * @else
 * @brief Swaping double-type-data and return post-data. (Unused)
 * @endif
 * @param v Pre-Data
 * @return Post-Data
 */
double reverseOrderDouble(double v);

/*!
 * @if jp
 * @brief LongDouble型データを1Byte単位で逆順にswapし、変換後のデータを返す。(Unused)
 * @else
 * @brief Swaping longdouble-type-data and return post-data. (Unused)
 * @endif
 * @param v Pre-Data
 * @return Post-Data
 */
long double reverseOrderLongDouble(long double v);

/*********************************/

#endif
