/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 *  RtORB Allocater
 *
 *
 */
/*!
 * @file allocater.h
 * @if jp
 * @brief RtORB Allocater functions.
 * @else
 * @brief RtORB Allocater functions.
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __FUNC_ALLOCATER_H__
#define __FUNC_ALLOCATER_H__

/*!
 * @if jp
 * @brief 指定したサイズでメモリを確保し、0で初期化する。デバッグモードの場合、infoをメッセージとして出力する。(malloc,memset)
 * @else
 * @brief Allocate memory by specified size, and initialize as '0' value. If DEBUG mode, output messages. (malloc,memset)
 * @endif
 * @param size memory size
 * @param info message
 * @return memory address
 */
void * RtORB__alloc(int32_t size, const char *info);

/*!
 * @if jp
 * @brief 指定したサイズと要素数でメモリを確保し、0で初期化する。デバッグモードの場合、infoをメッセージとして出力する。(calloc)
 * @else
 * @brief Allocate memory by specified size and element count, and initialize as '0' value. If DEBUG mode, output messages. (calloc)
 * @endif
 * @param size memory size
 * @param n element count
 * @param info message
 * @return memory address
 */
void * RtORB__calloc(int32_t size, int32_t n, const char *info);

/*!
 * @if jp
 * @brief 指定したサイズで確保しているメモリサイズを変更する。デバッグモードの場合、infoをメッセージとして出力する。(realloc)
 * @else
 * @brief Reallocate memory by specified size. If DEBUG mode, output messages. (realloc)
 * @endif
 * @param ptr memory address for reallocation
 * @param size memory size
 * @param info message
 * @return memory address
 */
void * RtORB__realloc(void *ptr, int32_t size, const char *info);

/*!
 * @if jp
 * @brief CORBAタイプコードの種類に応じたメモリ確保処理を行う。
 * @else
 * @brief Various allocation releated CORBA_Typecode.  
 * @endif
 * @param tc CORBA Type Code
 * @param len element count
 * @param info message
 * @return memory address (or NULL)
 */
void * RtORB_alloc_by_typecode(CORBA_TypeCode tc, int32_t len, const char *info);

/*!
 * @if jp
 * @brief 指定したサイズと要素数でメモリを確保し、0で初期化するために、内部で'RtORB_calloc()'をcallするのみ。
 * @else
 * @brief Only call 'RtORB_calloc()'.  
 * @endif
 * @param tc CORBA Type Code
 * @return memory address 
 */
void * RtORB_typecode_alloc(CORBA_TypeCode tc);

/*!
 * @if jp
 * @brief CORBAタイプコードの種類に応じたメモリ確保処理を行うために、内部で'RtORB_alloc_by_typecode()'をcallするのみ。
 * @else
 * @brief Only call 'RtORB_alloc_by_typecode()'.  
 * @endif
 * @param tc CORBA Type Code
 * @param length element count
 * @return memory address (or NULL)
 */
void * RtORB__allocbuf(CORBA_TypeCode tc, int32_t length);

/*!
 * @if jp
 * @brief CORBAタイプコードの種類に応じたメモリ再確保処理を行う、内部で'RtORB_realloc_by_typecode()'をcallするのみ。
 * @else
 * @brief Only call 'RtORB_realloc_by_typecode()'.  
 * @endif
 * @param ptr memory address for reallocation
 * @param tc CORBA Type Code
 * @param length element count
 * @return memory address (or NULL)
 */
void * RtORB__reallocbuf(void * ptr, CORBA_TypeCode tc, int32_t length);

/*!
 * @if jp
 * @brief CORBAタイプコードの種類に応じたメモリ確保処理を行うために、内部で'RtORB_alloc_by_typecode()'をcallするのみ。
 * @else
 * @brief Only call 'RtORB_alloc_by_typecode()'.  
 * @endif
 * @param tc CORBA Type Code
 * @param length element count
 * @return memory address (or NULL)
 */
void * RtORB_sequence_allocbuf(CORBA_TypeCode tc, int32_t length);

/*!
 * @if jp
 * @brief strdupにより文字列strをメモリ領域に確保する(文字列のコピー)。デバッグモードの場合、infoをメッセージとして出力する。
 * @else
 * @brief Allocate memory and set 'str' to it. If DEBUG mode, output messages. (strdup)
 * @endif
 * @param str Strings
 * @param info message
 * @return memory address
 */
char * RtORB__strdup(const char *str, const char *info);

char * RtORB__strndup(const char *str, int32_t n, const char *info);
/*!
 * @if jp
 * @brief 確保したメモリ領域を開放し、デバッグモードの場合、infoをメッセージとして出力する。
 * @else
 * @brief Release allocated memory. If DEBUG mode, output messages.
 * @endif
 * @param val target memory address for release
 * @param info message
 * @return void
 */
void RtORB__free(void *val, const char *info);

/*!
 * @if jp
 * @brief CORBAタイプコードの種類に応じたメモリ開放処理を行う。
 * @else
 * @brief Various releasing memory action releated CORBA_Typecode.  
 * @endif
 * @param tc CORBA Type Code
 * @param val target memory address for release
 * @param flag Flag(TRUE:release/FALSE:none)
 * @return void
 */
void RtORB_free_by_typecode(CORBA_TypeCode tc, void *val, int32_t flag);

/*!
 * @if jp
 * @brief CORBAタイプコードの種類に応じたメモリ開放処理を行う。(C++)
 * @else
 * @brief Various releasing memory action releated CORBA_Typecode.  (C++)
 * @endif
 * @param tc CORBA Type Code
 * @param val target memory address for release
 * @param flag Flag(TRUE:release/FALSE:none)
 * @return void
 */
void RtORB_free_by_typecode_cpp(CORBA_TypeCode tc, void *val, int32_t flag);

/*!
 * @if jp
 * @brief 確保したメモリ領域を開放する。 See deMarshal_Arg function(giop-marshal.c)
 * @else
 * @brief Release allocated memory. See deMarshal_Arg function(giop-marshal.c)
 * @endif
 * @param argv argv
 * @param in_argc argc(CORBA_Class_Method)
 * @param in_argv argv(CORBA_Class_Method)
 * @return void
 */
void RtORB_Arguments_free(void **argv, int32_t in_argc, CORBA_IArg *in_argv);

/*!
 * @if jp
 * @brief CORBAタイプコードの種類に応じたメモリ開放処理を行うために、内部で'RtORB_free_by_typecode()'をcallするのみ。
 * @else
 * @brief Only call 'RtORB_free_by_typecode()'.  
 * @endif
 * @param tc CORBA Type Code
 * @param p pointer (target memory address for release)
 * @param d T.B.D (Unused)
 * @return void
 */
void RtORB__freekids(CORBA_TypeCode tc, void *p, void *d);


#if defined(Darwin)
char *strndup(const char *str, size_t n);
#endif


void RtORB__free(void *val, const char *info);
void RtORB_Result__free(CORBA_TypeCode tc, void **result);
void RtORB_Result__free_cpp(CORBA_TypeCode tc, void **result);


#endif

