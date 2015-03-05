/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 * Utilities for CORBA_any
 *
 */
/*!
 * @file corba-any.c
 * @if jp
 * @brief CORBA_any型関連のデータを扱う関数群
 * @else
 * @brief CORBA_any data actions.
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#include <RtORB/corba.h>

/*
void *
any_to_value(void **ptr, CORBA_TypeCode tc){
    SKIP_ALIAS(tc);

    switch(tc->kind){
      case tk_null:
      case tk_void:
	      fprintf(stderr, "null or void value found\n");
	      return NULL;
      case tk_octet:
      case tk_boolean:
      case tk_char:
	      fprintf(stderr, "char = %d\n", (int)(*((char **)ptr)) );
	      break; 
      case tk_short:
      case tk_ushort:
	      fprintf(stderr, "short = %d\n", (int)(*((short **)ptr)) );
	      break;
      case tk_long:
      case tk_ulong:
      case tk_enum:
	      fprintf(stderr, "long = %d\n", (int)(*((int32_t **)ptr)) );
	      break;
      case tk_sequence:
	      fprintf(stderr, "sequence = %x\n", (int)ptr );
	      break;
      default :
	      fprintf(stderr, "Unknown Type = %d\n", (int)tc->kind);
	      break;

    }

    return (void *)ptr;
}
*/

/*
void *
get_value_by_name(CORBA_any *val, char *label){
  int i;
  int skip = 0;
  //void **ptr = (void **)val->_value;
  void **ptr = (void**) CORBA_any_get_value(val);
  void *retval;
  CORBA_TypeCode tc = val->_type;

  for(i=0; i<tc->member_count;i++){
   if(strcmp(tc->member_name[i], label) == 0) break;
   skip = size_of_typecode(tc->member_type[i]);
  }
  if(i == tc->member_count){
	  fprintf(stderr, "Label %s not found\n", label);
	  return NULL;
  }
  ptr = (void **)((char *)ptr + skip);

  retval = any_to_value(ptr, tc->member_type[i]);

  return retval;
}
*/

/*!
 * @if jp
 * @brief CORBA_any型データ用にメモリを確保し、TypeCodeとreleaseフラグをセットする。
 * @else
 * @brief allocate memory for CORBA_any data, and set TypeCode data and Release_Flag.
 * @endif
 * @param any CORBA_any data
 * @param tc CORBA_TypeCode data
 * @param clear clear Flag
 * @return void
 */
void CORBA_any_alloc(CORBA_any *any, CORBA_TypeCode tc, int32_t clear)
{
  if (clear) { CORBA_any_clear(any); }
  any->_type = tc;
  any->_val = (CORBA_any_val *) RtORB_alloc(sizeof(CORBA_any_val), "CORBA_any_alloc");
  any->_release=1;
}

/*!
 * @if jp
 * @brief CORBA_any型データ内に、引数のコンテンツデータをセットする。
 * @else
 * @brief set contents data to CORBA_any data array.
 * @endif
 * @param any CORBA_any data
 * @param tc CORBA_TypeCode data
 * @param data contents data
 * @param len length of array
 * @return void
 */
void CORBA_any_set_value_duplicate(CORBA_any *any, struct CORBA_TypeCode_struct *tc, char *data, int len)
{
  CORBA_any_alloc(any, tc, 0);
  if (len > 0) {
    char *buf = (char*)RtORB_alloc(len, "CORBA_any::setData");
    memcpy(buf, data, len);
    any->_val->val_encoded.data = buf;
  } else {
    any->_val->val_encoded.data = NULL;
  }
  any->_val->val_encoded.len = len;
}

void CORBA_any_set_value(CORBA_any *any, struct CORBA_TypeCode_struct *tc, char *data, int32_t len)
{
  CORBA_any_alloc(any, tc, 0);
  if (len > 0) {
    char *buf = (char*)RtORB_alloc(len, "CORBA_any::setData");
    memcpy(buf, data, len);
    any->_val->val_encoded.data = buf;
  } else {
    any->_val->val_encoded.data = NULL;
  }
  any->_val->val_encoded.len = len;
}

void CORBA_any_set_exception_value(CORBA_any *any, struct CORBA_TypeCode_struct * tc, void *value, CORBA_boolean release)
{
  if (tc) {
#ifdef USE_THREAD
    CORBA_any_alloc(any, tc, 0);
#else
    CORBA_any_alloc(any, tc, 1);
#endif
    any->_val->val_except = value;
  }
}

void CORBA_any_set_exception(CORBA_any *any, CORBA_any *src)
{
  void *v = NULL;
  if (!src || !src->_type) { return; }
  if (src->_type->kind != tk_except) { return; }

  v = src->_val? src->_val->val_except: NULL;
  CORBA_any_set_exception_value(any, src->_type, v, FALSE);
}

void * CORBA_any_get_value(CORBA_any* any)
{
  CORBA_any_val *val = NULL;
  if (!any || !any->_val) {
    fprintf(stderr, "CORBA_any_get_value : CORBA_any data is NULL");
    return NULL;
  }
  if (!any->_type) {
    fprintf(stderr, "CORBA_any_get_value : CORBA_any typecode is NULL");
    return NULL;
  }

  val = any->_val;
  switch(any->_type->kind) {
  case tk_null:
  case tk_void:
    return NULL;
  case tk_char:
    return &val->val_char;
  case tk_octet:
    return &val->val_octet;
  case tk_boolean:
    return &val->val_bool;
  case tk_ulong:
    return &val->val_ulong;
  case tk_string:
    return &val->val_str;
  case tk_objref:
    return &val->val_obj;
  case tk_except:
    return &val->val_except;
  case tk_struct:
    do_assert(0, "CORBA_any_get_value : struct has to be marshaled");
    break;
  default:
    fprintf(stderr, "CORBA_any_get_value : typecode %d is not supported", (int)any->_type->kind);
    break;
  }
  return NULL;
}

void CORBA_any_clear(CORBA_any *any)
{
  if (!any) { return; }
  
  if (any->_type && any->_release > 0 && any->_val) {
//    CORBA_any_val *val = any->_val;

    switch(any->_type->kind) {
    case tk_string:
      if (any->_val->val_str) {
	RtORB_free(any->_val->val_str, "CORBA_any_clear::string");
      }
      break;
    case tk_objref:
      if (any->_val->val_obj) { CORBA_Object_free(any->_val->val_obj); }
      break;
    case tk_struct:
      if (any->_val->val_encoded.len > 0) {
	RtORB_free(any->_val->val_encoded.data, "~CORBA_any_clear:struct (_data)");
      }
      break;
    default:
//      fprintf(stderr, "~CORBA_any_clear: type = %d\n", any->_type->kind);
      break;
    }
    void *val = any->_val;
    any->_val = NULL;
    //RtORB_free(any->_val, "~CORBA_any_clear(any->_val)");
    RtORB_free(val, "~CORBA_any_clear(any->_val)");

  }else{
    if (any->_release == 0){
       void *val = any->_val;
       any->_val = NULL;
       RtORB_free(val, "~CORBA_any_clear2(any->_val)");
    }
  }
//  any->_val = NULL;
  any->_type = NULL;
/*
  any->_release = 1;
*/
}

char * CORBA_any_get_encoded(CORBA_any *any, int32_t *len)
{
  if (!any || !any->_type) { return NULL; }

  if (any->_type->kind != tk_struct) { return NULL; }
  CORBA_any_val *val = any->_val;
  if (!val) { return NULL; }

  *len = val->val_encoded.len;
  if (*len <= 0) { return NULL; }
  return val->val_encoded.data;
}
  
  
