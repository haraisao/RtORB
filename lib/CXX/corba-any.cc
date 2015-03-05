/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*!
 * @file corba-any.cc
 * @if jp
 * @brief (TODO)
 * @else
 * @brief (TODO)
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */


#include <RtORB/corba.hh>

CORBA_any::CORBA_any() : _type(NULL), _val(NULL), _release(1) 
{
}

CORBA_any::CORBA_any(const CORBA_any &o) : _type(NULL), _val(NULL), _release(1)
{
  _type = o._type;
  if (o._val) {
    copy_val(o._type, o._val);
  }
}

CORBA_any::~CORBA_any()
{
  free_();
}

void CORBA_any::free_()
{
  CORBA_any_clear(this);
}

void CORBA_any::copy_val(CORBA_TypeCode tc, CORBA_any_val *v)
{
  if (!tc) { return; }
  alloc(tc);

  switch(tc->kind) {
  case tk_char:
    _val->val_char = v->val_char; break;
  case tk_octet:
    _val->val_octet = v->val_octet; break;
  case tk_boolean:
    _val->val_bool = v->val_bool; break;
  case tk_ulong:
    _val->val_ulong = v->val_ulong; break;
  case tk_objref:
    if (v->val_obj) {
      _val->val_obj = CORBA_Object_dup(v->val_obj);
    }
    break;
  case tk_string:
    if (v->val_str) {
      _val->val_str = RtORB_strdup(v->val_str, "CORBA_any::copy_val");
    }
    break;
  case tk_except:
    _val->val_except = v->val_except; break;
  case tk_struct:
    {
      int len = v->val_encoded.len;
      char *data = v->val_encoded.data;
      _val->val_encoded.len = len;
      if (len > 0 &&  data != NULL) {
	memcpy(_val->val_encoded.data, data, len);
      }
    }

    break;
  default:
    fprintf(stderr, "CORBA_any::copy_val : typecode %d not supported\n", (int)tc->kind);
    free_();
    break;
  }
  return;
}

#if 0
/*!
 * @if jp
 * @brief  (TODO)
 * @else
 * @brief  (TODO)
 * @endif
 * @param tc (TODO)
 * @param val (TODO)
 * @return void
 */
void print_any_val(CORBA_TypeCode tc, CORBA_any_val *val)
{
  if (!tc) { return; }

  switch(tc->kind) {
  case tk_char:
     fprintf(stderr, "\ttk_char %c\n", val->val_char);
     break;
  case tk_octet:
     fprintf(stderr, "\ttk_octet %c\n", val->val_octet);
     break;
  case tk_boolean:
     fprintf(stderr, "\ttk_boolean %c\n", val->val_bool);
     break;
  case tk_ulong:
     fprintf(stderr, "\ttk_ulong %d\n", (int32_t)val->val_ulong);
     break;
  case tk_objref:
    if (val->val_obj) {
       fprintf(stderr, "\ttk_objref %x\n", val->val_obj);
    }
    break;
  case tk_string:
    if (val->val_str) {
      fprintf(stderr, "\ttk_string %s\n", val->val_str);
    }
    break;
  case tk_except:
      fprintf(stderr, "\ttk_except %x\n", val->val_except);
     break;
  case tk_struct:
    {
      fprintf(stderr, "\ttk_struct %x\n", val->val_encoded);
      break;
    }
    break;
  default:
    fprintf(stderr, "CORBA_any::print : typecode %d not supported\n", (int)tc->kind);
    break;
  }
  return;
}
#endif

void CORBA_any::set_val(CORBA_TypeCode tc, CORBA_any_val *v)
{
  if (!tc) { return; }
  alloc(tc);

  switch(tc->kind) {
  case tk_char:
    _val->val_char = v->val_char; break;
  case tk_octet:
    _val->val_octet = v->val_octet; break;
  case tk_boolean:
    _val->val_bool = v->val_bool; break;
  case tk_ulong:
    _val->val_ulong = v->val_ulong; break;
  case tk_objref:
    if (v->val_obj) {
      _val->val_obj = CORBA_Object_dup(v->val_obj);
    }
    break;
  case tk_string:
    if (v->val_str) {
  //    _val->val_str = RtORB_strdup(val->val_str, "CORBA_any::copy_val");
      _val->val_str = v->val_str;
//   fprintf(stderr, "\tCORBA_any::set_val: %s \n", v->val_str);
//   fprintf(stderr, "\tSting = %s, %x\n",v->val_str, v->val_str);
    }
    break;
  case tk_except:
    _val->val_except = v->val_except; break;
  case tk_struct:
    {
      int len = v->val_encoded.len;
      char *data = v->val_encoded.data;
      _val->val_encoded.len = len;
      _val->val_encoded.data= data;
/*
      if (len > 0 &&  data != NULL) {
	memcpy(_val->val_encoded.data, data, len);
      }
*/
    }

    break;
  default:
    fprintf(stderr, "CORBA_any::copy_val : typecode %d not supported\n", (int)tc->kind);
    free_();
    break;
  }
  _release=0;
  return;
}


CORBA_any & CORBA_any::operator=(const CORBA_any &o)
{
  free_();
  copy_val(o._type, o._val);
//  set_val(o._type, o._val);
//  this->_type=o._type;
  this->_release=1;

  return *this;
}

CORBA_any *CORBA_any::duplicate()
{
  CORBA_any *out;
  out = new CORBA_any();
  out->_type = this->_type;
  out->copy_val(this->_type, this->_val);
  return out;
}

void CORBA_any::operator<<=(from_char from)
{
  alloc(TC_CORBA_char);

  _val->val_char = from.val;
}

CORBA_boolean CORBA_any::hasData(CORBA_TypeCode tc) const
{
  if (!_type) { return FALSE; }
  if (_type->kind != tc->kind) { return FALSE; }
  if (!_val) { return FALSE; }
  return TRUE;
}

CORBA_boolean CORBA_any::operator>>=(to_char to) const
{
  if (!hasData(TC_CORBA_char)) { return FALSE; }
  to.buf = _val->val_char;
  return TRUE;
}

void CORBA_any::alloc(CORBA_TypeCode tc)
{
  CORBA_any_alloc(this, tc, 1);
}

void CORBA_any::operator<<=(from_boolean from)
{
  alloc(TC_CORBA_boolean);
  _val->val_bool = from.val;
}

CORBA_boolean CORBA_any::operator>>=(to_boolean to) const
{
  if (!hasData(TC_CORBA_boolean)) { return FALSE; }
  to.buf = _val->val_bool;
  return TRUE;
}

void CORBA_any::operator<<=(from_octet from)
{
  alloc(TC_CORBA_octet);
  _val->val_octet = from.val;
}

CORBA_boolean CORBA_any::operator>>=(to_octet to) const
{
  if (!hasData(TC_CORBA_octet)) { return FALSE; }
  to.buf = _val->val_octet;
  return TRUE;
}

void CORBA_any::operator<<=(from_string from)
{
  alloc(TC_CORBA_string);
#if 1
  _val->val_str = RtORB_strdup(from.val, "CORBA_any::operator<<=(from_string)");
#else
  _val->val_str = (char *)from.val;
#endif
}

CORBA_boolean CORBA_any::operator>>=(to_string to) const
{
  if (!hasData(TC_CORBA_string)) { return FALSE; }
  to.buf = _val->val_str;
  return TRUE;
}

CORBA_any::from_object::from_object(CORBA::Object_ptr &ptr_) : ptr(ptr_) 
{
  
}

void CORBA_any::operator<<=(from_object from)
{
  alloc(TC_CORBA_Object);
  
  CORBA_Object obj = from.ptr.impl();
  if (obj) {
    _val->val_obj = CORBA_Object_dup(obj);
  }
}

CORBA_boolean CORBA_any::operator>>=(to_object to) const
{
  if (!hasData(TC_CORBA_Object)) { return FALSE; }
  CORBA::Object* obj = new CORBA::Object(_val->val_obj);
  to.ptr = obj->duplicate();
  return TRUE;
}


void CORBA_any::operator<<=(CORBA_unsigned_long n)
{
  alloc(TC_CORBA_ulong);
  _val->val_ulong = n;
}


CORBA_boolean CORBA_any::operator>>=(CORBA_unsigned_long &n) const
{
  if (!hasData(TC_CORBA_ulong)) { return FALSE; }
  n = _val->val_ulong;
  return TRUE;
}

/*
  Any_var operators
 */

void CORBA_any::setData(struct CORBA_TypeCode_struct *tc, char *data, int len)
{
  CORBA_any_clear(this);
  CORBA_any_set_value(this, tc, data, len);
}

namespace CORBA {

  void Any_var::operator<<=(CORBA::ULong n)
  {
    if (!_ptr) {
      _ptr = new Any();
    }
    *_ptr <<= n;
  }
  void Any_var::operator<<=(const char *s)
  {
    if (!_ptr) {
      _ptr = new Any();
    }
    *_ptr <<= s;
  }

  void Any_var::operator<<=(char *s)
  {
    if (!_ptr) {
      _ptr = new Any();
    }
    *_ptr <<= s;
  }

  void AnyObjectStream::from(CORBA::Any &any, CORBA_TypeCode tc, CORBA_Object obj)
  {
    any.alloc(tc);
  
    if (obj) {
      any._val->val_obj = CORBA_Object_dup(obj);
    }
  }
}
