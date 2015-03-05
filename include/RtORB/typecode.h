/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 * TypeCode for RtORB
 */
/*!
 * @file typecode.h
 * @if jp
 * @brief CORBA Type Code definition.
 * @else
 * @brief CORBA Type Code definition.
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __TYPECODE_H__
#define __TYPECODE_H__

#ifdef __cplusplus
extern "C"
{
#endif 

/*! enum TypeCode */
enum TypeCode{
  tk_null,			/* '0' */
  tk_void,			/* '1' */
  tk_short,			/* '2' */
  tk_long,			/* '3' */
  tk_ushort,			/* '4' */
  tk_ulong,			/* '5' */
  tk_float,			/* '6' */
  tk_double,			/* '7' */
  tk_boolean,			/* '8' */
  tk_char,			/* '9' */
  tk_octet,			/* '10' */
  tk_any,			/* '11' */
  tk_TypeCode,			/* '12' */
  tk_Principal,			/* '13' */
  tk_objref,			/* '14' */
  tk_struct,			/* '15' */
  tk_union,			/* '16' */
  tk_enum,			/* '17' */
  tk_string,			/* '18' */
  tk_sequence,			/* '19' */
  tk_array,			/* '20' */
  tk_alias,			/* '21' */
  tk_except,			/* '22' */
  tk_longlong,			/* '23' */
  tk_ulonglong,			/* '24' */
  tk_longdouble,		/* '25' */
  tk_wchar,			/* '26' */
  tk_wstring,			/* '27' */
  tk_fixed,			/* '28' */
  tk_value,			/* '29' */
  tk_value_box,			/* '30' */
  tk_native,			/* '31' */
  tk_abstract_interface,	/* '32' */
  tk_local_interface,		/* '33' */
  tk_component,			/* '34' */
  tk_home,			/* '35' */
  tk_event,			/* '36' */

  tk_END = 0xffffff		/* 'huge number' */
};


typedef unsigned char octet;

typedef unsigned long       POINTER;

typedef uint8_t       uchar;
/*
typedef uint16_t      ushort;
typedef uint32_t       ulong;
*/
typedef uint64_t  ulonglong;
typedef long double         longdouble;

#ifdef __cplusplus
}
#endif 
#endif
