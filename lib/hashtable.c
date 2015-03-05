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
 *
 *  $Revision: 1.2 $
 *  $Date: 2008/04/17 07:26:48 $
 *  $Id: hashtable.c,v 1.2 2008/04/17 07:26:48 yoshi Exp $
 *
 */
/*
 *  Hash table
 */
/*!
 * @file hashtable.c
 * @if jp
 * @brief Hash Table Functions.
 * @else
 * @brief Hash Table Functions.
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#include <stdint.h>
#include <RtORB/corba.h>
#include <RtORB/hashtable.h>

/**** HASH   ***/
/*
 * HASH function
 */
uint32_t elfhash(const unsigned char *name) {
   uint32_t h = 0, g;
   while (*name) {
       h = (h << 4) + *name++;
       g = h & 0xF0000000L;
       if (g) h ^= g >> 24;
       h &= ~g;
   }
   return h;
 }

int cmp_string(char *str1, char *str2){
  return (int)(!strcmp(str1, str2));
}

/*
 * Create Hash table
 *
 */
hashtable *createHashtable(int32_t size){
  hashtable *tbl;
  tbl = (hashtable *)RtORB_alloc(sizeof(hashtable), "createHashtable");
  tbl->items = (hashtableItem *)RtORB_alloc(sizeof(hashtableItem) * size,
		  "createHashtable(items)");
  memset(tbl->items, 0, sizeof(hashtableItem) * size);
/*
  tbl->deleted = 0;
*/
  tbl->size=size;
  tbl->n_items=0;
  tbl->cmp_func = (int32_t (*)(char*, char*))cmp_string;
  tbl->hash_func = elfhash;
  tbl->del_func = 0;

  return tbl;
}

/*!
 * @if jp
 * @brief  set function pointer for "cmp_func". (Unused)
 * @else
 * @brief  set function pointer for "cmp_func". (Unused)
 * @endif
 * @param tbl Hash Table
 * @param cmp_func function pointer for comparing elements
 * @return void
 */
void set_cmp_func(hashtable *tbl, int (*cmp_func)(char*, char*)){
  tbl->cmp_func = (int32_t (*)(char*, char*))cmp_func;
}

/*!
 * @if jp
 * @brief  set function pointer for "hash_func". (Unused)
 * @else
 * @brief  set function pointer for "hash_func". (Unused)
 * @endif
 * @param tbl Hash Table
 * @param hash_func Hash function pointer
 * @return void
 */
void set_hash_func(hashtable *tbl, uint32_t (*hash_func)(const unsigned char*)){
  tbl->hash_func = hash_func;
}

/*!
 * @if jp
 * @brief  set function pointer for "del_func". (Unused)
 * @else
 * @brief  set function pointer for "del_func". (Unused)
 * @endif
 * @param tbl Hash Table
 * @param del_func function pointer for deleting elements
 * @return void
 */
void set_delete_func(hashtable *tbl, void (*del_func)(hashtableItem)){
  tbl->del_func = del_func;
}

/*
 * resize hashtable
 */
hashtable *resizeHashtable(hashtable *tbl, int size){
  hashtableItem *item;
  int i,j;
  int key;

  if(tbl == NULL) return createHashtable(size);

  if((item = (hashtableItem *)RtORB_alloc(sizeof(hashtableItem) * size,
				  "resizeHashtable")) == NULL) return NULL;
  memset(item, 0, sizeof(hashtableItem) * size);

  for(i=0, j=0;i<tbl->size && j < size;i++){
    if(!tbl->items[i].key) break;
    key = getNewKey(tbl, tbl->items[i].key);
    item[key].key=tbl->items[i].key;
    item[key].value=tbl->items[i].value;
    j++; 
  }  
  RtORB_free(tbl->items,"resizeHashtable(items)");
  tbl->items = item;
  tbl->size=size;
  tbl->n_items = j;

  return tbl;
}


void rehash_table(hashtable *tbl){
  hashtableItem *item;
  int key;
  int i,j;

  if((item = (hashtableItem *)RtORB_alloc(sizeof(hashtableItem) * tbl->size,
				 "rehash_table" )) == NULL) return;
  memset(item, 0, sizeof(hashtableItem) * tbl->size);

  for(i=0, j=0;i<tbl->size;i++){
    if(!tbl->items[i].key) break;
    key = getNewKey(tbl, tbl->items[i].key);
    item[key].key=tbl->items[i].key;
    item[key].value=tbl->items[i].value;
    j++; 
  }  
  RtORB_free(tbl->items, "rehash_table(items)");
  tbl->items = item;
  tbl->n_items = j;
  return;
}


/*
 * Destory Hash table
 *
 */
void destoryHashTable(hashtable *tbl){
  int i;

  if(!tbl) return;
  if(tbl->del_func){
    for(i=0;i<tbl->size;i++){
      (*tbl->del_func)(tbl->items[i]);
    }
  }
  RtORB_free(tbl->items, "destoryHashTable(items)");
  RtORB_free(tbl,"destoryHashTable");
}

/*
 *  get a new key
 *
 */
int32_t getNewKey(hashtable *tbl, void *k){
  int size;
  int key;

   size = tbl->size;
   key = (*tbl->hash_func)((const unsigned char*)k) % tbl->size;

  
  while(size){
	  if(tbl->items[key].key == 0|| (*tbl->cmp_func)((char*)tbl->items[key].key, (char*)k)) return key;
     else key = hashtableNextKey(tbl, key);
     size--;
  }

  fprintf(stderr, "No space left.....\n");
  return -1;
}

int32_t hashtableNextKey(hashtable *tbl, int32_t k){
  int key = (k+1) % tbl->size;
  return key;
}

/*
 *
 * get an item<key, value> from a hash table by a key
 *
 */
hashtableItem *getItemByKey(hashtable *tbl, void *k){
   int key;
   int size = tbl->size;
   int n_items = tbl->n_items;

   key = (*tbl->hash_func)((const unsigned char*)k) % tbl->size;

/*   if(!tbl->items[key].key) return NULL;   */
   while(n_items && size ){

     if(!tbl->items[key].key){
	     size--;
	     continue;
     }
     n_items--;


     if((*tbl->cmp_func)((char*)tbl->items[key].key, (char*)k)){
        return &tbl->items[key];
     }
     else key = hashtableNextKey(tbl, key);

/*     if(tbl->deleted && !tbl->items[key].key) return NULL;  */
   }
   return NULL;
}

int getIndexByKey(hashtable *tbl, void *k){
   int key;
   int n_items = tbl->n_items;

   key = (*tbl->hash_func)((const unsigned char*)k) % tbl->size;

   while(n_items){
     if(!tbl->items[key].key) continue;
     n_items--;

     if((*tbl->cmp_func)((char*)tbl->items[key].key, (char*)k)) return key;
     else key = hashtableNextKey(tbl, key);

   }
   return -1;
}

/*
 *
 * get a value from a hash table by a key
 *
 */
void *getValue(hashtable *tbl, void *key){
#if HASH_DEBUG
  fprintf(stderr, "getValue : %s in %x\n", (char *)key, tbl);
#endif  
  hashtableItem *item = getItemByKey(tbl, key);
#if HASH_DEBUG
  fprintf(stderr, "item : %x, ", item);
  if (item) fprintf(stderr, "item->value :%x\n", item->value);
  else fprintf(stderr, "\n");
#endif  
  if(item && item->value) return item->value;
  return NULL;
}

/*
 *
 * register an item<key, value> into a hash table
 *
 */
int32_t registerItem(hashtable *tbl, void *k, void *val){
   int key = getNewKey(tbl, k);

   if (key < 0){
     resizeHashtable(tbl, tbl->size * 2);
     key = getNewKey(tbl, k);
     if (key < 0){ return -1; }
   }

   if(tbl->items[key].key == 0){
     tbl->items[key].key = k;
     tbl->n_items += 1;
   }
   tbl->items[key].value = val;
#if HASH_DEBUG
   fprintf(stderr, "registerItem(%x):key=%s, ",tbl, (char*)k);
   fprintf(stderr, " val=%x\n", val);

#endif   
   return key;
}

/*
 *
 * delete item<key, value> from a hash table
 *
 */
void deleteItem(hashtable *tbl, void *k){
   int idx = getIndexByKey(tbl, k);
   if(idx < 0) return;

   if(tbl->del_func) (*tbl->del_func)(tbl->items[idx]);
   tbl->items[idx].key = 0;
   tbl->items[idx].value = 0;

   tbl->n_items -= 1;
}

