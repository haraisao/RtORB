/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 * array.h
 *
 */
/*!
 * @file array.h
 * @if jp
 * @brief 単純な固定長汎用配列(ポインタの配列を持っているだけ)、同一型のリスト構造と操作関数を提供。
 * @else
 * @brief Simple List Structure
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif 

/*!
 * @struct PtrArray
 * @brief 単純な固定長汎用配列。 
 *        汎用配列は、"list.h"の汎用リスト構造体の簡易実装版で、非常に限定的な場合の使用を想定したリスト構造体であり、
 *        メモリ使用量が比較的少なく済むように実装。
 *        格納するポインタには、開放するための関数を別途定義し、free_opに設定することが必要。
 *        CORBA_Sequenceに構造が似ており、基本的にリサイズしない。
 *        主に、ORB内の[ORB Initial Reference][ObjectAdapter(POA)][Thread][POA Manager]の管理に使用。
 * @param length 配列長
 * @param maximum MAX値
 * @param array 汎用配列ポインタ
 * @param free_op 要素を開放するための関数
 */
typedef struct {
  uint32_t length;
  uint32_t maximum;
  long *array;

  void (*free_op)(long);
} PtrArray;


/*
 Array
*/
/*!
 * @if jp
 * @brief 新しく配列を格納するPtrArrayを生成する。
 * @else
 * @brief create new array.
 * @endif
 * @return Array pointer
 */
PtrArray * new_PtrArray();

/*!
 * @if jp
 * @brief PrtArrayを破壊する。このとき各要素は開放されないため、予め開放しておく必要あり。
 * @else
 * @brief destroy PtrArray.
 * @endif
 * @param array Array pointer
 * @return void
 */
void destroy_PtrArray(PtrArray *array);

/*!
 * @if jp
 * @brief PrtArrayに新しい要素を末尾に追加する。
 *        PtrArrayの要素が全く無い場合、5つの配列格納領域を確保する。
 *        確保された配列格納領域がfullの場合には、2倍の領域をreallocで再確保する。
 *        (最初は5つの配列格納領域を確保し、6つめのデータを追加する際に格納領域が10に変更される。)
 * @else
 * @brief add new elements to PtrArray
 * @endif
 * @param array Array pointer
 * @param new_data new elements
 * @return length of array
 */
uint32_t PtrArray_add (PtrArray *array, void *new_data);

/*!
 * @if jp
 * @brief index番目のデータfree_op関数を使って削除する。
 *        削除された場所は、最後尾のデータに置き換えられ、配列の順番が変更される。
 * @else
 * @brief delete elements of Array and change the order.
 * @endif
 * @param array Array pointer
 * @param index index
 * @return length of array
 */
int32_t PtrArray_delete(PtrArray *array, int32_t index);

/*!
 * @if jp
 * @brief index番目のデータを獲得する。
 *        indexが配列の長さ以上の場合には、データが無いため"-1"が返る。（NULLにすべきか検討）
 * @else
 * @brief Get elements of array which was indicated by 'index'. 
 * @endif
 * @param array Array pointer
 * @param index index
 * @return elements of array
 */
void *PtrArray_get(PtrArray *array, int32_t index);

/*!
 * @if jp
 * @brief 配列の中で　cmp関数を評価関数として各要素とvalを比較し、同じ要素と判断されたindexを返す。
 *        見つからない場合には、"-1"を返す。
 * @else
 * @brief Comapring elements of Array. match or not. return 'index' or '-1' as none.
 * @endif
 * @param array Array pointer
 * @param val element for comaparing
 * @param cmp function for comparing
 * @return index of Array which was matched as same elements by comparing action.
 */
int32_t PtrArray_find(PtrArray *array, void *val, int32_t (*cmp)(void*, long) );

/*!
 * @if jp
 * @brief 配列の各要素に対して、func関数を適用する。
 * @else
 * @brief Roop action to each elements of array. 
 * @endif
 * @param array Array pointer
 * @param func function pointer
 * @return void
 */
void PtrArray_foreach(PtrArray *array, void *(*func)(long) );

#ifdef __cplusplus
}
#endif 

#endif
