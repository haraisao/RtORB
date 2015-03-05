/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 *  Sequence:
 *
 *
 */
/*!
 * @file corba-sequence.c
 * @if jp
 * @brief CORBA Sequence関連のデータの扱い用関数群。
 * @else
 * @brief CORBA_Sequence data action.
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#include <RtORB/corba.h>

/**** CORBA_Sequence *****/
uint32_t get_octet_size(uint32_t type){
  int res = 4;
  switch(type){
     case tk_char:
     case tk_octet:
	res = 1;
	break;
     case tk_short:
     case tk_ushort:
	res = 2;
	break;
     case tk_long:
     case tk_ulong:
     case tk_float:
	res = 4;
	break;
     case tk_longlong:
     case tk_ulonglong:
     case tk_double:
	res = 8;
	break;
     case tk_longdouble:
	res = 16;
	break;
     default:
	break;
  }
  return res;
}

uint32_t sizeof_CORBA_Sequence(CORBA_Sequence *seq){
  return get_octet_size(seq->_type) * seq->_length;
}

CORBA_Sequence *new_CORBA_Sequence(uint32_t type, uint32_t len){
  CORBA_Sequence *res = (CORBA_Sequence *)RtORB_alloc(sizeof(CORBA_Sequence), 
					"new_CORBA_Sequence");
  res->_type = type;
  res->_length = res->_maximum = len;
  res->_release = 0;
  res->_buffer = (octet *)RtORB_alloc(get_octet_size(type) * len, 
				"new_CORBA_Sequence (_buffer)");
  return res;
}

int copy_CORBA_Sequence_Octet(CORBA_Sequence_Octet *dist, CORBA_Sequence_Octet *src){
   dist->_length = src->_length;
   dist->_maximum = src->_maximum;
   dist->_release = src->_release;
   dist->_buffer = (octet *)RtORB_alloc(src->_length,
		   "copy_CORBA_Sequence_Octet(buffer)");
   memcpy(dist->_buffer, src->_buffer, src->_length);
   return 1;
}

/*!
 * @if jp
 * @brief CORBA SequenceBase dataのメモリ開放処理をする。
 * @else
 * @brief release Memory. (CORBA Sequence data)
 * @endif
 * @param p pointer of CORBA Sequence data
 * @param str message
 * @return void
 */
void
CORBA_SequenceBase_Buffer_free(void *p, char str){
  RtORB_free(p, str);
  return;
}

CORBA_SequenceBase *CORBA_SequenceBase__new(int _max, int _len){
  CORBA_SequenceBase *seq =
	  (CORBA_SequenceBase *)RtORB_alloc(sizeof(CORBA_SequenceBase),
					  "CORBA_SequenceBase__new");
  seq->_length = _len;
  seq->_maximum = _max;
  seq->_release = 0;
  seq->_buffer = (void **)RtORB_calloc( sizeof(void **), _max, "CORBA_Sequence__new");
  return seq;
}

void CORBA_SequenceBase__add(CORBA_SequenceBase *seq, void *val){
 CORBA_SequenceBase__set(seq, seq->_length, val);
}

void CORBA_SequenceBase__set(CORBA_SequenceBase *seq, int idx, void *val){
 idx += 1;
  if(idx > seq->_maximum){
     seq->_maximum = idx;
     seq->_length = idx;
     seq->_buffer = (void **)RtORB_realloc(seq->_buffer, idx * sizeof(void *), "CORA_SequenceBase__set");
  }
  if(idx > seq->_length){ seq->_length = idx; }

  seq->_buffer[idx-1] = val;
  return;
}

void CORBA_SequenceBase__clear(CORBA_SequenceBase *seq, void (*_free)()){
  typedef void (*free_func_type)(void*, const char *);
  int i;
  if(!_free) _free =  (void (*)())CORBA_SequenceBase_Buffer_free;
  for(i=0; seq->_length;i++){
	  ((free_func_type)_free)(seq->_buffer[i], "CORBA_SequenceBase__clear");
  }
  seq->_length = 0;
  memset(seq->_buffer, 0, seq->_maximum);
  return ;
}


void CORBA_SequenceBase__delete(CORBA_SequenceBase *seq, void (*_free)(), int flag){
  typedef void (*free_func_type)(void*, const char *);
  int i;
  if(!_free) _free = (void (*)())CORBA_SequenceBase_Buffer_free;
  for(i=0; seq->_length;i++){
	  ((free_func_type)_free)(seq->_buffer[i], "CORBA_SequenceBase__clear");
  }
  if(seq->_maximum){
    RtORB_free(seq->_buffer, "CORBA_SequenceBase__delete(buffer)");
  }
  if (flag){
    RtORB_free(seq, "CORBA_SequenceBase__delete");
  }
}

