/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 *  Common Data Representation
 *
 *
 */
/*!
 * @file CDR-defs.h
 * @if jp
 * @brief CDR(Common Data Representation)の定義。送受信用のデータの表現情報の定義。
 * @else
 * @brief definition of Common Data Representation(CDR)
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __CDR_H__
#define __CDR_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define MAX(A,B)	(A>B ? A:B)  /*!< return Max value */

#define Order_BE	0  /*!< Big Endian */
#define Order_LE	1  /*!< Little Endian */


/*   Alignment requirements for OMG IDL primitive data types */
#define Align_char 	1  /*!< Alignment(char) */
#define Align_octet 	1  /*!< Alignment(octet) */
#define Align_short 	2  /*!< Alignment(short) */
#define Align_ushort 	2  /*!< Alignment(unsigned short) */
#define Align_long 	4  /*!< Alignment(long) */
#define Align_ulong 	4  /*!< Alignment(unsigned long) */
#define Align_longlong 	8  /*!< Alignment(long long) */
#define Align_ulonglong	8  /*!< Alignment(unsigned long long) */
#define Align_float	4  /*!< Alignment(float) */
#define Align_double	8  /*!< Alignment(double) */
#define Align_longdouble	8  /*!< Alignment(long double) */
#define Align_boolean	1  /*!< Alignment(boolean) */
#define Align_enum	4  /*!< Alignment(enum) */
#define Align_wchar 	1  /*!< Alignment(wchar) for GIOP 1.2 or 1.3 (for GIOP 1.1 wchar alignment is 1, 2 or 4)  */

#define Align_pointer	4  /*!< Alignment(Pointer) */
#define Align_union	4  /*!< Alignment(Union) */
#define Align_sequence	4  /*!< Alignment(Sequence) */
#define Align_any	4  /*!< Alignment(Any) */

/* Datat Length for each data type  */
#define DataLen_char 	1  /*!< Data Length(char) */
#define DataLen_octet 	1  /*!< Data Length(octet) */
#define DataLen_short 	2  /*!< Data Length(short) */
#define DataLen_ushort 	2  /*!< Data Length(unsigned short) */
#define DataLen_long 	4  /*!< Data Length(long) */
#define DataLen_ulong 	4  /*!< Data Length(unsigned long) */
#define DataLen_longlong 	8  /*!< Data Length(long long) */
#define DataLen_ulonglong	8  /*!< Data Length(unsigned long long) */
#define DataLen_float	4  /*!< Data Length(float) */
#define DataLen_double	8  /*!< Data Length(double) */
#define DataLen_longdouble	16  /*!< Data Length(long double) */
#define DataLen_boolean	1  /*!< Data Length(boolean) */
#define DataLen_enum	4  /*!< Data Length(enum) */
#define DataLen_wchar	2  /*!< Data Length(wchar) */

#ifdef __cplusplus
}
#endif                          /* __cplusplus */


#endif
