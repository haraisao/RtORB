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
 *   Array Operations
 *
 * Copyright(C) 2006, Isao Hara, AIST
 *
 * $Revision: 1.2 $
 * $Date: 2008/03/06 12:43:48 $
 * $Id: array.c,v 1.2 2008/03/06 12:43:48 yoshi Exp $
 *
 */
/*!
 * @file array.c
 * @if jp
 * @brief 汎用配列用function。
 * @else
 * @brief Functions of Array (Simple List Structure).
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#include <RtORB/corba.h>
#include <RtORB/array.h>

PtrArray *
new_PtrArray(){

   PtrArray *res = (PtrArray *)RtORB_alloc(sizeof(PtrArray), "new_PrtArray");

   if (res == NULL) exit(1);
   memset(res, 0, sizeof(PtrArray));
   return res;
}

void destroy_PtrArray(PtrArray *array){
  if(!array) return;
  if(array->array) {
    RtORB_free(array->array, "destroy_PtrArray0");
  }
  RtORB_free(array, "destroy_PtrArray1");
  return;
}

uint32_t PtrArray_add (PtrArray *array, void *new_data){
  if(!array) return 0;

  if(array->maximum == 0) {

    int newsize = 5;
    array->maximum = newsize;
    array->array = (long *)RtORB_calloc(newsize, sizeof(long), "PtrArray_add");

  } else if(array->length +1 > array->maximum){
	  array->array = (long *)RtORB_realloc(array->array, array->length * 2, "PtrArray_add" );
	  if(array->array == NULL){
		  fprintf(stderr, "Memory alloc error\n");
		  exit(1);
	  }
	  array->maximum = array->length * 2;
  }
  array->array[array->length] = (long)new_data;
  array->length += 1;
  return array->length;
}

int32_t PtrArray_delete(PtrArray *array, int32_t index){

  if(!array) return 0;

  if (index >= array->length) return -1;
  if(array->length > 1){
    if(array->free_op) (*array->free_op)(array->array[index]);
    array->array[index] = array->array[array->length -1]; 
  }

  array->length -= 1;
  array->array[array->length] = 0;

  return array->length;
}

void *PtrArray_get(PtrArray *array, int32_t index){
  if (index >= array->length ) return (void *)-1;
  return (void *)array->array[index];
}

int32_t PtrArray_find(PtrArray *array, void *val, int32_t (*cmp)(void*, long) ){
  int i;

  if(!array) return -1;

  for(i=0; i<array->length;i++){
     if( cmp(val, array->array[i]) ) return i;
  }
  return -1;
}

void PtrArray_foreach(PtrArray *array, void *(*func)(long) ){
  int i;

  if(!array) return;

  for(i=0; i<array->length;i++){
     func(array->array[i]);
  }
  return;
}

