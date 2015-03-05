/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 * Speech Event - Action Translator ver.1.0
 *
 *   Copyright(c) 2005    Isao Hara, AIST
 */
/*
 *  Hash table
 */
/*!
 * @file hashtable.h
 * @if jp
 * @brief Hash table(ハッシュ関数は、デフォルトでは文字列をキーにして、ELFhash関数を使用)
 * @else
 * @brief Hash table(default Hash function : "ELFhash")
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */


#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif 
/* Define: struct hashtableItem */
/*!
 * @struct hashtableItem
 * @brief hashtableItem (Key & Value)
 * @param key Hash Key pointer
 * @param value Hash Value pointer
 */
typedef struct hashtableItem{
  void *key;
  void *value;
} hashtableItem;

/*!
 * @struct hashtable
 * @brief hashtable。
 *
 *        主に、POA,ORB内の[CORBAオブジェクトの管理]に使用。
 *        可変長でリサイズ可能。これはCORBAオブジェクトの最大値が不明なため。
 *        リサイズ時は原則としてサイズを倍にする。
 *        デフォルトでは、キーは文字列にするため、それ以外のものをキーとして使いたい場合
 *        （例えば、2つの文字列のペアとか）には、キーの比較関数であるcmp_funcやhash_funcを
 *        定義する必要がある。また、値に関しては特に制限はしていないが、
 *        del_func で定義された要素の削除関数を定義しておく事を推奨する。
 *        そうでない場合には、簡単にメモリーリークを発生させる可能性があるため。
 * @param items hashtableItem
 * @param size size of hash table
 * @param n_items number of items
 * @param cmp_func function pointer for comparing elements
 * @param hash_func Hash function pointer
 * @param del_func function pointer for deleting elements
 */
typedef struct hashtable{
  hashtableItem *items;
  int32_t size;
/*
  int32_t deleted;
*/
  int32_t n_items;
  int32_t (*cmp_func)(char*, char*);
  uint32_t (*hash_func)(const unsigned char*);
  void  (*del_func)(hashtableItem);
} hashtable;


/*!
 * @if jp
 * @brief デフォルトのハッシュ関数　"ELFhash"
 * @else
 * @brief default Hash function "ELFhash"
 * @endif
 * @param name Hash key
 * @return Hash value
 */
uint32_t elfhash(const unsigned char *name);

/*!
 * @if jp
 * @brief ハッシュ表を生成する。ハッシュ表の容量は、"size"で指定する。
 * @else
 * @brief create Hash Table.
 * @endif
 * @param size size of hash table
 * @return hash table
 */
hashtable *createHashtable(int32_t size);

/*!
 * @if jp
 * @brief ハッシュ表を破壊する。登録されている要素も同時に開放する。
 * @else
 * @brief delete Hash Table and items.
 * @endif
 * @param tbl Hash table
 * @return void
 */
void destoryHashTable(hashtable *tbl);

/*!
 * @if jp
 * @brief "k"の次のインデックスを得る。"k"が末尾であれば、先頭(0)が返ってくる。
 * @else
 * @brief get 'index' next of 'k'.
 * @endif
 * @param tbl Hash Table
 * @param k Hash Key 
 * @return next 'index' 
 */
int32_t hashtableNextKey(hashtable *tbl, int32_t k);

/*!
 * @if jp
 * @brief キー "k" が表すハッシュ表のインデックスを返す。
 *        すでにハッシュ関数で求めた場所に要素があった場合には、1つずつずらして、空いている場所を返す。
 *        ハッシュ表がすべて要素で埋め尽くされていれば、"-1" を返す。
 * @else
 * @brief get 'index' of Hash table releated 'key'.
 * @endif
 * @param tbl Hash Table
 * @param k Hash Key 
 * @return index of Hash
 */
int32_t getNewKey(hashtable *tbl, void *k);

/*!
 * @if jp
 * @brief キー "k" に基づきハッシュ表から要素とキーのペアであるhashtableItemを返す。
 *        もしキーに対応する要素がない場合には、NULLを返す。
 * @else
 * @brief get HashTableItem reletated "k".
 * @endif
 * @param tbl Hash Table
 * @param k Hash Key 
 * @return pointer of HashTableItem reletated "k"
 */
hashtableItem *getItemByKey(hashtable *tbl, void *k);

/*!
 * @if jp
 * @brief キー "k"として要素"val"をハッシュ表に登録し、そのインデックスを返す。
 *        既にハッシュ表が要素でfullの場合には、ハッシュ表のサイズを2倍に拡張して登録する。
 *        それでも失敗する場合には、"-1"を返す。
 * @else
 * @brief register key and value to Hash table.
 * @endif
 * @param tbl Hash Table
 * @param k Hash Key 
 * @param val value for registration
 * @return index of Hash Table releated registerd key 
 */
int32_t registerItem(hashtable *tbl, void *k, void *val);

/*!
 * @if jp
 * @brief キー "k"として見つかった要素"val"をハッシュ表から削除する。
 * @else
 * @brief delete key and value from Hash table.
 * @endif
 * @param tbl Hash Table
 * @param k Hash Key 
 * @return void
 */
void deleteItem(hashtable *tbl, void *k );

/*!
 * @if jp
 * @brief キー　"key" に基づきハッシュ表内に登録されている要素を返す。
 *        これは、getItemByKeyと似ているが、キーと要素のペアではなく、"要素"のみを返す。
 *        失敗すれば、NULLを返す。
 * @else
 * @brief get only value from Hash table releated "key".
 * @endif
 * @param tbl Hash Table
 * @param key Hash Key 
 * @return pointer of value
 */
void *getValue(hashtable *tbl, void *key);

#ifdef __cplusplus
}
#endif 
#endif
