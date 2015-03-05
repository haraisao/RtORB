/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 *   RtORB: TypeCode.c
 *
 *   Copyright(C) 2006, Isao Hara, AIST
 *
 *   $Revision: 1.8 $
 *   $Date: 2008/05/19 06:11:39 $
 *   $Id: corba-typecode.c,v 1.8 2008/05/19 06:11:39 yoshi Exp $
 */
/*!
 * @file corba-typecode.c
 * @if jp
 * @brief CORBA_TypeCode関連データを扱う関数群
 * @else
 * @brief CORBA_Type_Code data actions.
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#include <RtORB/corba.h>

#ifndef __CORBA_VALUE_MODIFIER__
#define __CORBA_VALUE_MODIFIER__
typedef short ValueModifier;
const ValueModifier VM_NONE=0;         
const ValueModifier VM_CUSTOM=1;       
const ValueModifier VM_ABSTRACT=2;     
const ValueModifier VM_TRUNCATABLE=3;  
#endif

/*
  exception CORBA_TypeCode_Bounds{};
  exception CORBA_TypeCode_BadKind{};
*/

#ifndef __TC_CONST__
#define __TC_CONST__
const CORBA_TypeCode_struct TC_null_struct = {
	NULL, tk_null, "IDL::omg.org/CORBA/Null:1.0", "null",  0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0
};
const CORBA_TypeCode_struct TC_void_struct = {
	NULL, tk_void, "IDL::omg.org/CORBA/void:1.0", "void",  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
const CORBA_TypeCode_struct TC_short_struct ={
	NULL, tk_short, "IDL::omg.org/CORBA/short:1.0", "short",  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2
};
const CORBA_TypeCode_struct TC_long_struct = {
	NULL, tk_long, "IDL::omg.org/CORBA/long:1.0", "long",  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4
};
const CORBA_TypeCode_struct TC_longlong_struct ={
	NULL, tk_longlong, "IDL::omg.org/CORBA/long_long:1.0", "long_long",  0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 8, 8
};
const CORBA_TypeCode_struct TC_ushort_struct = {
	NULL, tk_ushort, "IDL::omg.org/CORBA/unsigned_short:1.0", "short",  0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 2, 2
};
const CORBA_TypeCode_struct TC_ulong_struct = {
	NULL, tk_ulong, "IDL::omg.org/CORBA/unsigned_long:1.0", "unsigned_long",  0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 4, 4
};
const CORBA_TypeCode_struct TC_ulonglong_struct = {
	NULL, tk_ulonglong, "IDL::omg.org/CORBA/unsigned_long_long:1.0", "unsigned_long_long",  0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 8, 8
};
const CORBA_TypeCode_struct TC_float_struct = {
	NULL, tk_float, "IDL::omg.org/CORBA/float:1.0", "float",  0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 4, 4
};
const CORBA_TypeCode_struct TC_double_struct = {
	NULL, tk_double, "IDL::omg.org/CORBA/double:1.0", "double",  0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 8, 8
};
const CORBA_TypeCode_struct TC_longdouble_struct = {
	NULL, tk_longdouble, "IDL::omg.org/CORBA/long_double:1.0", "long_double",  0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 16, 16
};
const CORBA_TypeCode_struct TC_boolean_struct = {
	NULL, tk_boolean, "IDL::omg.org/CORBA/boolean:1.0", "boolean", 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 1, 1
};
const CORBA_TypeCode_struct TC_char_struct = {
	NULL, tk_char, "IDL::omg.org/CORBA/char:1.0", "char",  0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 1, 1
};
const CORBA_TypeCode_struct TC_wchar_struct = {
	NULL, tk_wchar, "IDL::omg.org/CORBA/wchar:1.0", "wchar",  0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 2, 2
};
const CORBA_TypeCode_struct TC_octet_struct = {
	NULL, tk_octet, "IDL::omg.org/CORBA/octet:1.0", "octet", 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 1, 1
};
const CORBA_TypeCode_struct TC_any_struct = {
	NULL, tk_any, "IDL::omg.org/CORBA/any:1.0", "any",  0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 12, 4
};
const CORBA_TypeCode_struct TC_TypeCode_struct = {
	NULL, tk_TypeCode, "IDL::omg.org/CORBA/TypeCode:1.0", "TypeCode",  0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 4, 4
};
const CORBA_TypeCode_struct TC_Object_struct = {
	NULL, tk_objref, "IDL:omg.org/CORBA/Object:1.0", "Object",0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4
};
const CORBA_TypeCode_struct TC_string_struct = {
	NULL, tk_string, "IDL::omg.org/CORBA/string:1.0", "string",  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4
};
const CORBA_TypeCode_struct TC_wstring_struct = {
	NULL, tk_wstring, "IDL::omg.org/CORBA/wstring:1.0", "wstring",  0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 4, 4
};
const CORBA_TypeCode_struct TC_ValueBase_struct = {
	NULL, tk_value, "IDL:omg.org/CORBA/ValueBase:1.0", "ValueBase", 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0
};
const CORBA_TypeCode_struct TC_Component_struct = {
	NULL, tk_component, "IDL:omg.org/Components/CCMObject:1.0", "CCMObject",  0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0
};
const CORBA_TypeCode_struct TC_Home_struct = {
	NULL, tk_home, "IDL:omg.org/Components/CCMHome:1.0", "CCMHome",  0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0
};
const CORBA_TypeCode_struct TC_EventBase_struct = {
	NULL, tk_event, "IDL:omg.org/Components/EventBase:1.0", "EventBase",  0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* dummy struct TypeCode*/
const CORBA_TypeCode_struct TC_struct_struct = {
	NULL, tk_struct, "IDL:omg.org/CORBA/struct:1.0", "struct",  0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0
};  /*!< dummy struct TypeCode */

#endif

static CORBA_TypeCode tc_table[] = {
  TC_null,				/* 0  */
  TC_void,				/* 1  */
  TC_CORBA_short,			/* 2  */
  TC_CORBA_long,			/* 3  */
  TC_CORBA_ushort,			/* 4  */
  TC_CORBA_ulong,			/* 5  */
  TC_CORBA_float,			/* 6  */
  TC_CORBA_double,			/* 7  */
  TC_CORBA_boolean,			/* 8  */
  TC_CORBA_char,			/* 9  */
  TC_CORBA_octet,			/* 10 */
  TC_CORBA_any,				/* 11 */
  TC_CORBA_TypeCode,			/* 12 */
  NULL, /* TC_CORBA_Principal,*/        /* 13 */
  TC_CORBA_Object,			/* 14 */
  (CORBA_TypeCode)&TC_struct_struct,			/* 15 */
  NULL, /*TC_CORBA_union, */		/* 16 */
  NULL, /*TC_CORBA_enum, */    		/* 17 */
  TC_CORBA_string,			/* 18 */
  NULL, /*TC_CORBA_sequence, */		/* 19 */
  NULL, /*TC_CORBA_array, */  		/* 20 */
  NULL, /*TC_CORBA_alias, */		/* 21 */
  NULL, /*TC_CORBA_except,*/		/* 22 */
  TC_CORBA_long_long,			/* 23 */
  TC_CORBA_ulonglong,			/* 24 */
  TC_CORBA_long_double,			/* 25 */
  TC_CORBA_wchar,			/* 26 */
  TC_CORBA_wstring,			/* 27 */
  NULL, /*TC_CORBA_fixed, */		/* 28 */
  NULL, /* TC_CORBA_value, */		/* 29 */
  NULL, /* TC_CORBA_value_box, */ 	/* 30 */
  NULL, /*TC_CORBA_native, */		/* 31 */
  NULL, /* TC_CORBA_abstract_interface,	*/ /* 32 */
  NULL, /* TC_CORBA_local_interface, */	/* 33 */
  NULL, /* TC_CORBA_component, */	/* 34 */
  NULL, /* TC_CORBA_home, */		/* 35 */
  NULL, /*TC_CORBA_event, */		/* 36 */
};  /*!< CORBA_Type_Code table */

#define ARY_SIZE(ARY) (sizeof(ARY)/sizeof(ARY[0]))

CORBA_TypeCode CORBA_TypeCode_get(CORBA_TCKind kind)
{
  CORBA_TypeCode tc = NULL;
  if (0 <= kind || kind < sizeof(tc_table)/sizeof(tc_table[0])) {
    tc = tc_table[kind];
  }
#if 0
  if (tc == NULL) {
    fprintf(stderr, "CORBA_TypeCode_get : typecode %ld not supported\n", kind);
  }
#endif
  return tc;
}

/*!
 * @if jp
 * @brief 与えられたrepositoryIDに一致する箇所のTypeCodeを返す。
 * @else
 * @brief return TypeCode releated with repository_ID.
 * @endif
 * @param repoid repository ID
 * @return TypeCode
 */
static CORBA_TypeCode CORBA_TypeCode_get_by_repoid(const char *repoid)
{
  int i;
  for (i=0; i<ARY_SIZE(tc_table); i++) {
    if (!tc_table[i]) { continue; }
    if (strcmp(tc_table[i]->repository_id, repoid) == 0) {
      return tc_table[i];
    }
  }
  return NULL;
}

boolean
CORBA_TypeCode_equal(CORBA_TypeCode code, CORBA_TypeCode tc, CORBA_Environment *ev){
   int i;

  if (code->kind != tc->kind) return FALSE;

  switch(code->kind){
	  case tk_sequence:
	  case tk_array:
  		if (code->length != tc->length) return FALSE;
		return CORBA_TypeCode_equal(code->member_type[0], tc->member_type[0], ev);
	  case tk_string:
	  case tk_wstring:
  		if (code->length != tc->length) return FALSE;
		break;
	  case tk_value_box:
		return CORBA_TypeCode_equal(code->content_type, tc->content_type, ev);

	  case tk_component:
	  case tk_home:
	  case tk_objref:
		return !strcmp(code->repository_id, tc->repository_id);

	  case tk_alias:
		if( strcmp(code->repository_id, tc->repository_id) ) return FALSE;
		break;

	  case tk_struct:
	  case tk_except:
		if( code->member_count != tc->member_count ) return FALSE;
		if( strcmp(code->repository_id, tc->repository_id) ) return FALSE;
		for(i=0;i<code->member_count;i++)
		  if(!  CORBA_TypeCode_equal(code->member_type[i], tc->member_type[i], ev) ) return FALSE;
		break;

	  case tk_union:
		if( code->member_count != tc->member_count ) return FALSE;
		if( code->default_index != tc->default_index ) return FALSE;
		if( strcmp(code->repository_id, tc->repository_id) ) return FALSE;
		for(i=0;i<code->member_count;i++)
		  if(!  CORBA_TypeCode_equal(code->member_type[i], tc->member_type[i], ev) ) return FALSE;
		break;

	  case tk_enum:
		if( code->member_count != tc->member_count ) return FALSE;
		if( strcmp(code->repository_id, tc->repository_id) ) return FALSE;
		break;

	  case tk_value:
	  case tk_event:

	  case tk_fixed:
		return (code->fixed_digits == tc->fixed_digits  && code->fixed_scale == tc->fixed_scale);
	  case tk_native:
	  case tk_abstract_interface:
	  case tk_local_interface:
	  default:
		break;

  }

  return TRUE;
}

boolean
CORBA_TypeCode_equivalent(CORBA_TypeCode code, CORBA_TypeCode tc, CORBA_Environment *ev){
  if (code->kind != tc->kind) return FALSE;

  return TRUE;
}

CORBA_TCKind
CORBA_TypeCode_kind(CORBA_TypeCode code, CORBA_Environment *ev){
  return code->kind;
}

CORBA_RepositoryId
CORBA_TypeCode_id(CORBA_TypeCode code, CORBA_Environment *ev){
  switch(code->kind){
     case tk_objref: case tk_struct: case tk_union:
     case tk_enum: case tk_alias: case tk_value:
     case tk_value_box: case tk_native: case tk_abstract_interface:
     case tk_local_interface: case tk_except:
     case tk_component: case tk_home: case tk_event:
       return code->repository_id;
     default:
       CORBA_system_exception(ev,"BadKind");
       return NULL;
  }
}

CORBA_Identifier
CORBA_TypeCode_name(CORBA_TypeCode code, CORBA_Environment *ev){
  switch(code->kind){
     case tk_objref: case tk_struct: case tk_union:
     case tk_enum: case tk_alias: case tk_value:
     case tk_value_box: case tk_native: case tk_abstract_interface:
     case tk_local_interface: case tk_except:
     case tk_component: case tk_home: case tk_event:
       return code->identifier;
     default:
       CORBA_system_exception(ev,"BadKind");
       return NULL;
  }
}

uint32_t
CORBA_TypeCode_member_count(CORBA_TypeCode code, CORBA_Environment *ev){
  switch(code->kind){
     case tk_struct: case tk_union: case tk_enum:
     case tk_value: case tk_except: case tk_event:
       return code->member_count;
     default:
       CORBA_system_exception(ev,"BadKind");
       return (uint32_t)0;
  }
}

CORBA_Identifier
CORBA_TypeCode_member_name(CORBA_TypeCode code,uint32_t index,  CORBA_Environment *ev){
  switch(code->kind){
     case tk_struct: case tk_union: case tk_enum:
     case tk_value: case tk_except: case tk_event:
       if(code->member_count > index ){
	  CORBA_system_exception(ev,"Bounds");
	  break;
	}      
       return code->member_name[index];
     default:
       CORBA_system_exception(ev,"BadKind");
       break;
  }
  return NULL;
}

CORBA_TypeCode
CORBA_TypeCode_member_type(CORBA_TypeCode code,uint32_t index,  CORBA_Environment *ev){
  switch(code->kind){
     case tk_struct: case tk_union:
     case tk_value: case tk_except: case tk_event:
       if(code->member_count > index ){
	  CORBA_system_exception(ev,"Bounds");
	  break;
	}      
       return code->member_type[index];
     default:
       CORBA_system_exception(ev,"BadKind");
       break;
  }
  return NULL;
}

void * *
CORBA_TypeCode_member_label(CORBA_TypeCode code,uint32_t index,  CORBA_Environment *ev){
  if(code->kind != tk_union){
    CORBA_system_exception(ev,"BadKind");
    return (void * *)NULL;
  }
  if(code->member_count > index ){
    CORBA_system_exception(ev,"Bounds");
    return (void * *)NULL;
  }

  return (void **)&code->member_label[index] ;
}

CORBA_TypeCode
CORBA_TypeCode_discriminator_type(CORBA_TypeCode code,  CORBA_Environment *ev){
  if(code->kind != tk_union){
    CORBA_system_exception(ev,"BadKind");
    return NULL;
  }
  return code->discriminator;
}

int32_t 
CORBA_TypeCode_default_index(CORBA_TypeCode code,  CORBA_Environment *ev){
  if(code->kind != tk_union){
    CORBA_system_exception(ev,"BadKind");
    return (int32_t)0;
  }
  return code->default_index;
}

uint32_t 
CORBA_TypeCode_length(CORBA_TypeCode code,  CORBA_Environment *ev){
  switch(code->kind){
     case tk_string: case tk_wstring:
     case tk_sequence: case tk_array:
       return code->length;
     default:
       CORBA_system_exception(ev,"BadKind");
       break;
  }
  return (uint32_t)0;
}

CORBA_TypeCode
CORBA_TypeCode_content_type(CORBA_TypeCode code,  CORBA_Environment *ev){
  switch(code->kind){
     case tk_sequence: case tk_array:
     case tk_value_box: case tk_alias:
       return code->content_type;
     default:
       CORBA_system_exception(ev,"BadKind");
       break;
  }
  return (CORBA_TypeCode)NULL;
}

unsigned short
CORBA_TypeCode_fixed_digits(CORBA_TypeCode code,  CORBA_Environment *ev){
  switch(code->kind){
     case tk_fixed:
       return code->fixed_digits;
     default:
       CORBA_system_exception(ev,"BadKind");
       break;
  }
  return (unsigned short)0;
}

short
CORBA_TypeCode_fixed_scale(CORBA_TypeCode code,  CORBA_Environment *ev){
  switch(code->kind){
     case tk_fixed:
       return code->fixed_scale;
     default:
       CORBA_system_exception(ev,"BadKind");
       break;
  }
  return (short)0;
}

CORBA_Visibility
CORBA_TypeCode_member_visibility(CORBA_TypeCode code, uint32_t index,  CORBA_Environment *ev){
  switch(code->kind){
     case tk_value: case tk_event:
       if(code->member_count > index ){
	  CORBA_system_exception(ev,"Bounds");
	  break;
	}      
       return code->member_visibility[index];
     default:
       CORBA_system_exception(ev,"BadKind");
       break;
  }
  return NULL;
}

CORBA_ValueModifier
CORBA_TypeCode_type_modifier(CORBA_TypeCode code, CORBA_Environment *ev){
  switch(code->kind){
     case tk_value: case tk_event:
       return code->type_modifier;
     default:
       CORBA_system_exception(ev,"BadKind");
       break;
  }
  return NULL;
}

CORBA_TypeCode
CORBA_TypeCode_concrete_base_type(CORBA_TypeCode code, CORBA_Environment *ev){
  switch(code->kind){
     case tk_value: case tk_event:
       return code->concrete_base_type;
     default:
       CORBA_system_exception(ev,"BadKind");
       break;
  }
  return NULL;
}

/*
 *
 * See orbit_cbe_type_is_fixed_length functions (idl-compile/orbit-idl-utils.c)
 *
 **/
int CORBA_TypeCode_is_fixed_size(CORBA_TypeCode tc)
{
  SKIP_ALIAS(tc);
  
  switch(tc->kind) {
  case tk_struct:
  case tk_union:
  case tk_except:
    {
      int i;
      for (i=0; i<tc->member_count; i++) {
	if (!CORBA_TypeCode_is_fixed_size(tc->member_type[i])) { return 0; }
      }
      return 1;
    }
  case tk_short:
  case tk_long:
  case tk_ushort:
  case tk_ulong:
  case tk_float:
  case tk_double:
  case tk_boolean:
  case tk_char:
  case tk_octet:
  case tk_enum:
  case tk_longlong:
  case tk_ulonglong:
  case tk_longdouble:
  case tk_wchar:
    return 1;
  case tk_any:
  case tk_TypeCode:
  case tk_Principal:
  case tk_objref:
  case tk_string:
  case tk_wstring:
  case tk_sequence:
  case tk_native:
    return 0;
  case tk_alias: /* skip */
  case tk_null:
  case tk_void:
  case tk_array:
  case tk_fixed:
  case tk_value:
  case tk_value_box:
  case tk_abstract_interface:
  case tk_local_interface:
  case tk_component:
  case tk_home:
  case tk_event:
    ASSERT(0, "CORBA_TypeCode_is_fixed_size");
    return 0;
  default:
    ASSERT(0, "CORBA_TypeCode_is_fixed_size");
    return 0;
  };
}

static hashtable *dynamic_tcs = NULL;   

int CORBA_TypeCode_add_dynamic(CORBA_TypeCode tc)
{
  CORBA_TypeCode static_tc;
  
  if (!tc || !tc->repository_id) { return 0; }


  if (!dynamic_tcs) {
    dynamic_tcs = createHashtable(64);
  }

  if (getValue(dynamic_tcs, tc->repository_id)) { return 0; }

  static_tc = CORBA_TypeCode_get(tc->kind);
  if (static_tc && strcmp(static_tc->repository_id, tc->repository_id) == 0) {
    return 0;
  }


  registerItem(dynamic_tcs, tc->repository_id, tc);
#ifdef DEBUG
  fprintf(stderr, "dynamic tc : %s registerd\n", tc->repository_id);
#endif
  return 0;
}

/*!
 * @if jp
 * @brief 与えられた引数をセットし、新規にCORBA_TypeCode型データセットを作成する。
 * @else
 * @brief create CORBA_TypeCode data-set by setting each parameters.
 * @endif
 * @param kind 'kind' data (CORBA_TypeCode)
 * @param repoid 'repository ID' data (CORBA_TypeCode)
 * @param id 'identifier' data (CORBA_TypeCode)
 * @return CORBA_TypeCode data
 */
static CORBA_TypeCode create_type_code(CORBA_TCKind kind, const char *repoid, const char *id)
{
    CORBA_TypeCode tc = (CORBA_TypeCode)RtORB_alloc(sizeof(struct CORBA_TypeCode_struct), "CORBA_TypeCode_get_dynamic");
    tc->kind = tk_objref;
    tc->repository_id = RtORB_strdup(repoid, "CORBA_TypeCode_get_dynamic : repository_id");
    tc->identifier = RtORB_strdup(id, "CORBA_TypeCode_get_dynamic : identifier");
    tc->discriminator = CORBA_OBJECT_NIL;
    tc->default_index = -1;
    return tc;
}

CORBA_TypeCode CORBA_TypeCode_get_dynamic(CORBA_TCKind kind, const char *repoid, const char *id)
{
  CORBA_TypeCode tc = NULL;
  void *v;
  if (dynamic_tcs) {
	  v = getValue(dynamic_tcs, (void *)repoid);
    if (v) {
      tc = (CORBA_TypeCode)v;
    }
  }

  if (!tc) {
    tc = CORBA_TypeCode_get_by_repoid(repoid);
  }

  if (!tc && kind == tk_objref) {
    tc = create_type_code(tk_objref, repoid, id);
    if (!dynamic_tcs) {
      dynamic_tcs = createHashtable(64);
    }
    registerItem(dynamic_tcs, tc->repository_id, tc);
  }
  
  return tc;
}

