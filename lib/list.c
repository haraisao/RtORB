/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 * RtORB:
 *   List Operations
 *
 * Copyright(C) 2006, Isao Hara, AIST
 *
 * $Revision: 1.1.1.1 $
 * $Date: 2008/01/08 07:56:06 $
 * $Id: list.c,v 1.1.1.1 2008/01/08 07:56:06 yoshi Exp $
 *
 */
/*!
 * @file list.c
 * @if jp
 * @brief 汎用リスト構造体を操作する関数群。
 * @else
 * @brief Functions of General-purpose valiable List Structure.
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#include <RtORB/corba.h>
#include <RtORB/list.h>

/***  List  ****/

PtrList *
PtrList_new_item(void *value, void (*free_op)(void**)){
   PtrList *res = (PtrList *)RtORB_alloc(sizeof(PtrList), "PtrList_new_item");
   if (res == NULL) exit(1);
   memset(res, 0, sizeof(PtrList));

   res->item = (void**)value;
   res->free_op = free_op;
   res->prev = res->next = NULL;
   res->released = res->length =0;
   return res;
}

PtrList *
PtrList_append(PtrList *lst, void *value, void (*free_op)(void**)){
   PtrList *tmp;

   if (!lst){
     lst = PtrList_new_item(value, free_op);
   }else{
     for(tmp=lst; tmp->next ; tmp=tmp->next);
     tmp->next = PtrList_new_item(value, free_op);
     tmp->next->prev = tmp;
   }
   return lst;
}

int32_t
PtrList_length(PtrList *lst){
  int i;
   PtrList *tmp;
  if(!lst) return 0;

  for(i=0, tmp=lst; tmp->next ; i++,tmp=tmp->next);
  return i+1;
}

void *
PtrList_get_item(PtrList *lst, int32_t index){
  int i;
   PtrList *tmp;
  if(!lst) return (void *)0;

  for(i=0, tmp=lst; i < index && tmp->next ; i++,tmp=tmp->next);
  return tmp->item;
}

PtrList *
PtrList_get(PtrList *lst, int32_t index){
  int i;
   PtrList *tmp;
  if(!lst) return (PtrList *)0;

  for(i=0, tmp=lst; i < index && tmp->next ; i++,tmp=tmp->next);
  return tmp;
}

void
PtrList_free(PtrList *lst){
  if(lst->free_op){ (*lst->free_op)(lst->item); }
  RtORB_free(lst, "PtrList_free");
  return;
}

PtrList *
PtrList_remove(PtrList *lst, int32_t index){
  int i;
   PtrList *tmp;
  if(!lst) return (PtrList *)0;

  for(i=0, tmp=lst; i < index && tmp->next ; i++,tmp=tmp->next);
  if (tmp->next) tmp->next->prev = tmp->prev;
  if (tmp->prev) tmp->prev->next = tmp->next;
  else lst = tmp->next;

  PtrList_free(tmp);
  return lst ;
}


PtrList *
PtrList_set_item(PtrList *lst, int32_t index, void *value){
  int i;
   PtrList *tmp;
  if(!lst) return (PtrList *)0;

  for(i=0, tmp=lst; i < index && tmp->next ; i++,tmp=tmp->next);
  tmp->item = (void**)value;
  return lst;
}

PtrList *
PtrList_remove_item(PtrList *lst){
   PtrList *tmp = lst;
   PtrList *retval = (PtrList *)0;

  if(!lst) return (PtrList *)0;

  if (tmp->next){
    retval = tmp->next;
    tmp->next->prev = tmp->prev;
  }
  if (tmp->prev){
     tmp->prev->next = tmp->next;
     if(retval == 0){ retval = tmp->prev; }
  }
  PtrList_free(tmp);

  return retval ;
}

PtrList *
PtrList_first(PtrList *lst){
  PtrList *tmp = lst;
  if(!lst) return NULL;

  while(tmp->prev) tmp = tmp->prev;

  return tmp;
}

PtrList *
PtrList_last(PtrList *lst){
  PtrList *tmp = lst;
  if(!lst) return NULL;

  while(tmp->next) tmp = tmp->next;

  return tmp;
}

void
PtrList_remove_item_all(PtrList *lst){
  PtrList *tmp = lst; 
  while(tmp) tmp=PtrList_remove_item(tmp);

  return;
}

PtrList *
PtrList_remove_released_items(PtrList *lst){
  PtrList *tmp = lst; 
  PtrList *retval = NULL; 

  if(!lst) return NULL;
  
  while(tmp){
    if(tmp->released)
	  tmp = PtrList_remove_item(tmp);
    else {
      retval = tmp;
      tmp=tmp->next;
    }
  }

  if(retval) return PtrList_first(retval);
  return retval;
}
