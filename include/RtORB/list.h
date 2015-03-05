/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 *
 *  list.h
 *
 */
/*!
 * @file list.h
 * @if jp
 * @brief 単純な可変長のポインタリスト構造と操作用の関数群を提供。
 *
 *        リストできる要素については、特に制限はありませんが、各要素を削除する時の
 *        処理関数を定義しておく必要があります。デフォルトでは、削除処理関数は　
 *        NULLに設定されますので、削除時にメモリ解放が行われません。
 * @else
 * @brief General-purpose valiable List Structure
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __LIST_H__
#define __LIST_H__

#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif 

/*!
 * @struct PtrList
 * @brief 可変長の汎用リスト構造体(相互連結型のポインタList)。
 *        主に、[GIOPの Request Queue][POAに対するRequest Queue]に使用。
 *        動的にリサイズ可能で必要なメモリしか消費しない。
 * @param length length of List
 * @param item item pointer
 * @param prev link to prevous item
 * @param next link to next item
 * @param free_op function pointer for releasing elements
 * @param released release flag(0:not released,1:released[queue is done])
 */
typedef struct PtrList_struct{
  uint32_t length;
/*
  uint32_t *item;
*/
  void **item;
  struct PtrList_struct *prev, *next;
  void (*free_op)(void**);
  int32_t released;
} PtrList;

/*
   List
*/
/*!
 * @if jp
 * @brief 新しい汎用リストの要素を生成する
 * @else
 * @brief Create New List
 * @endif
 * @param value item for registration
 * @param free_op function pointer for releasing elements
 * @return List pointer
 */
PtrList * PtrList_new_item(void *value, void (*free_op)(void**));

/*!
 * @if jp
 * @brief lstで表されたリストの末尾に要素を追加し、lstを返す。
 *        lstは、通常先頭の要素であるが、リスト内のどこの要素でも良いが、
 *        返値は、lstになるので注意。もしlstがNULLの場合には、
 *        新しく要素を作成してそのポインタを返す。
 * @else
 * @brief append new elements to List
 * @endif
 * @param lst List pointer
 * @param value item for registration
 * @param free_op function pointer for releasing elements
 * @return List pointer
 */
PtrList * PtrList_append(PtrList *lst, void *value, void (*free_op)(void**));

/*!
 * @if jp
 * @brief lstから末尾まので、汎用リストの要素数を返す。
 * @else
 * @brief get the number of all elements in List
 * @endif
 * @param lst List pointer
 * @return the number of all elements
 */
int32_t PtrList_length(PtrList *lst);

/*!
 * @if jp
 * @brief lstを先頭とし、index番目の要素を返す。
 * @else
 * @brief get elements releated 'index' from List
 * @endif
 * @param lst List pointer
 * @param index index
 * @return element pointer 
 */
void * PtrList_get_item(PtrList *lst, int32_t index);

/*!
 * @if jp
 * @brief lstを先頭とし、index番目の汎用リスト構造体を返す。
 * @else
 * @brief get List pointer releated "index"
 * @endif
 * @param lst List pointer
 * @param index index
 * @return List pointer releated "index"
 */
PtrList * PtrList_get(PtrList *lst, int32_t index);

/*!
 * @if jp
 * @brief lstの汎用リスト構造体を開放する。
 *        free_op がNULLでない場合には、要素itemをfree_opを用いて開放する。
 *        ここで連結されたリストのnext, prevには、変更が加えられないので注意。
 *        通常は、PtrList_remove関数で、要素の削除を行うべきである。
 * @else
 * @brief Remove List
 * @endif
 * @param lst List pointer
 * @return void
 */
void PtrList_free(PtrList *lst);

/*!
 * @if jp
 * @brief lstを先頭とし、index番目の要素を削除する。
 * @else
 * @brief Remove elements releated 'index'.
 * @endif
 * @param lst List pointer
 * @param index index
 * @return List pointer
 */
PtrList * PtrList_remove(PtrList *lst, int32_t index);

/*!
 * @if jp
 * @brief lstを先頭とし、index番目の汎用リスト構造体の要素を設定する。
 *        このとき、予め設定されている要素は、開放されないので注意。
 * @else
 * @brief set values as 'item' to List releated 'index'.
 * @endif
 * @param lst List pointer
 * @param index index
 * @param value List item
 * @return List pointer
 */
PtrList * PtrList_set_item(PtrList *lst, int32_t index, void *value);

/*!
 * @if jp
 * @brief lstの要素を削除する。これは、PtrList_remove(lst, 0); と同じとなる。
 * @else
 * @brief Remove elements releated 'lst'. ( PtrList_remove(lst, 0); )
 * @endif
 * @param lst List pointer
 * @return Head List pointer 
 */
PtrList * PtrList_remove_item(PtrList *lst);

/*!
 * @if jp
 * @brief lstを先頭とし、それ以降のリストをすべて解放する。
 *        この関数では、lstの前に要素があっても、その前のnextの値を書き換えないので注意する。
 * @else
 * @brief Remove elements releated (index > 'lst').
 * @endif
 * @param lst List pointer
 * @return void
 */
void PtrList_remove_item_all(PtrList *lst);

/*!
 * @if jp
 * @brief lstを先頭とし、汎用リストの要素のreleaseが0でないものを開放する。
 * @else
 * @brief Remove elements releated {(index > 'lst') && ('released' != 0)|
 * @endif
 * @param lst List pointer
 * @return Head List pointer
 */
PtrList *PtrList_remove_released_items(PtrList *lst);

/*!
 * @if jp
 * @brief lstが含まれるリストの先頭の汎用リスト構造体を返す。
 * @else
 * @brief get Head List pointer related 'lst' List
 * @endif
 * @param lst List pointer
 * @return Head List pointer
 */
PtrList *PtrList_first(PtrList *lst);

/*!
 * @if jp
 * @brief lstが含まれるリストの最後の汎用リスト構造体を返す。
 * @else
 * @brief get Last List pointer related 'lst' List
 * @endif
 * @param lst List pointer
 * @return Head List pointer
 */
PtrList *PtrList_last(PtrList *lst);

#ifdef __cplusplus
}
#endif 

#endif
