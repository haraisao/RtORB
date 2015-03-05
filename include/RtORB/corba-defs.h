/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*

 Simple CORBA Implementation for x86 (Big Endian)
  
*/

/*!
 * @file corba-defs.h
 * @if jp
 * @brief CORBA definition.
 * @else
 * @brief CORBA definition.
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
*/

#ifndef __CORBA_DEFS_H__
#define __CORBA_DEFS_H__

#include <stdint.h>

#if USE_THREAD
#include <pthread.h>
#endif
#include <RtORB/list.h>
#include <RtORB/array.h>
#include <RtORB/hashtable.h>
#include <RtORB/typecode.h>


#ifdef __cplusplus
namespace CORBA {
  class Any_var;
  class Object;
  class Object_ptr;
}

class cdrStream;
#endif

#ifdef __cplusplus
extern "C"
{
#endif 

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* for experimental use */
#ifndef CORBA_VPVID
#define CORBA_VPVID	0xfffff
#endif

#define CORBA_I_ARG_IN		0
#define CORBA_I_ARG_OUT		1
#define CORBA_I_ARG_INOUT	2

#define CORBA_OBJECT_NIL	NULL

/* Alignments */
#define CORBA_OCTET_ALIGN	1
#define CORBA_WCHAR_ALIGN	2
#define CORBA_SHORT_ALIGN	2
#define CORBA_LONG_ALIGN	4
#define CORBA_FLOAT_ALIGN	4
#define CORBA_DOUBLE_ALIGN	8
#define CORBA_LONG_LONG_ALIGN	8
#define CORBA_LONG_DOUBLE_ALIGN	16
#define CORBA_OBJECT_ALIGN	4
/* SIZE_OF */
#define CORBA_OCTET_SIZE	1
#define CORBA_WCHAR_SIZE	2
#define CORBA_SHORT_SIZE	2
#define CORBA_LONG_SIZE		4
#define CORBA_FLOAT_SIZE	4
#define CORBA_DOUBLE_SIZE	8
#define CORBA_LONG_LONG_SIZE	8
#define CORBA_LONG_DOUBLE_SIZE	16
#define CORBA_OBJECT_SIZE	4

/* Exception */
  /*
enum CORBA_System_Exceptions{
  CORBA_BAD_PARAM=1, CORBA_INVALID_VALUE, CORBA_UNKNOWN_VALUE, END_EXCEPTION
};
  */

/*! enum IOP_ServiceId (正常終了/ユーザ例外/システム例外) */
enum CORBA_ReplyStatusType{
   CORBA_NO_EXCEPTION, CORBA_USER_EXCEPTION, CORBA_SYSTEM_EXCEPTION, 
   CORBA_LOCATION_FORWARD,CORBA_LOCATION_FORWARD_PERM, NEEDS_ADDRESSING_MODE
};


/***  Basic Data Type **/
typedef char   CORBA_char;
typedef int16_t  CORBA_short;
typedef int32_t  CORBA_long;
typedef int64_t  CORBA_long_long;
typedef unsigned char  CORBA_unsigned_char;
typedef uint16_t CORBA_unsigned_short;
typedef uint32_t CORBA_unsigned_long;
typedef uint64_t CORBA_unsigned_long_long;
typedef float    CORBA_float;
typedef double   CORBA_double;
typedef long double CORBA_long_double;
typedef char     CORBA_octet;
typedef short    CORBA_wchar;
typedef unsigned char CORBA_boolean;
typedef char *   CORBA_string;
typedef short *  CORBA_wstring;

typedef unsigned char boolean;

extern const uint32_t RTORB_ORB_TYPE;

#define CORBA_ANY_MAXBUF MaxMessageSize  


struct CORBA_Object_struct;

/*!
 * @union CORBA_any_val
 * @brief CORBA_any_val (TODO)
 * @param val_bool (TODO)
 * @param val_octet (TODO)
 * @param val_char (TODO)
 * @param val_ulong (TODO)
 * @param val_str (TODO)
 * @param val_obj (TODO)
 * @param val_except (TODO)
 * @param val_encoded (TODO)
 */
typedef union
{
  CORBA_boolean val_bool;
  CORBA_octet val_octet;
  CORBA_char  val_char;
  CORBA_unsigned_long val_ulong;

  char * val_str;
  struct CORBA_Object_struct * val_obj;
  void * val_except;

  /* tk_struct */
  struct val_t {
    int32_t   len;
    char * data;
  } val_encoded;
} CORBA_any_val;

/*!
 * @struct CORBA_any
 * @brief CORBA_any CORBA Any type data
 * @param _type Identified ID (CORBA_Type_Code)
 * @param _val adress of data
 * @param _release data release flag (TRUE:release)
 */
typedef struct CORBA_any {
  struct CORBA_TypeCode_struct *_type;
  CORBA_any_val *_val;
/*
  CORBA_boolean _release;
*/
  CORBA_unsigned_long _release;

#ifdef __cplusplus
  CORBA_any();
/*!
 * @brief (TODO)
 */
  CORBA_any(const CORBA_any &o);
  ~CORBA_any();

/*!
 * @brief (TODO)
 */
  CORBA_boolean hasData(struct CORBA_TypeCode_struct *) const;

/*!
 * @brief (TODO)
 */
  void setData(struct CORBA_TypeCode_struct *tc, char *data, int len);
/*!
 * @brief (TODO)
 */
  void copy_val(CORBA_TypeCode_struct *, CORBA_any_val *);
/*!
 * @brief (TODO)
 */
  void set_val(CORBA_TypeCode_struct *, CORBA_any_val *);

/*!
 * @brief (TODO)
 */	
  void alloc(CORBA_TypeCode_struct *);
/*!
 * @brief (TODO)
 */
  void free_();

/*!
 * @brief (TODO)
 */  
  CORBA_any & operator=(const CORBA_any &o);

/*!
 * @brief (TODO)
 */
  CORBA_any * duplicate();

/*!
 * @brief (TODO)
 */
  CORBA_TypeCode_struct * type() { return _type; }

/*!
 * @brief (TODO)
 */		
  struct from_char {
    from_char(CORBA_char v) : val(v) {};
    CORBA_char val;
  };

/*!
 * @brief (TODO)
 */
  struct from_boolean {
    from_boolean(CORBA_boolean v) : val(v) {};
    CORBA_boolean val;
  };

/*!
 * @brief (TODO)
 */
  struct from_octet {
    from_octet(CORBA_octet v) : val(v) {};
    CORBA_octet val;
  };

/*!
 * @brief (TODO)
 */
  struct from_string {
    from_string(const char *v) : val(v) {}
    const char *val;
  };

/*!
 * @brief (TODO)
 */
  struct from_object {
    from_object(CORBA::Object_ptr &ptr_);
    CORBA::Object_ptr &ptr;
  };

/*!
 * @brief (TODO)
 */  
  struct from_any {
    from_any(CORBA_any *v) : val(v) {}
    CORBA_any * val;
  };

/*!
 * @brief (TODO)
 */
  void operator<<=(from_char);
/*!
 * @brief (TODO)
 */
  void operator<<=(from_boolean);
/*!
 * @brief (TODO)
 */
  void operator<<=(from_octet);
/*!
 * @brief (TODO)
 */
  void operator<<=(from_string);
/*!
 * @brief (TODO)
 */
  void operator<<=(from_object);
/*!
 * @brief (TODO)
 */
  void operator<<=(from_any);
/*!
 * @brief (TODO)
 */
  void operator<<=(CORBA_unsigned_long);

/*!
 * @brief (TODO)
 */
  struct to_char {
    to_char(CORBA_char &buf_) : buf(buf_){}
    CORBA_char &buf;
  };

/*!
 * @brief (TODO)
 */
  struct to_octet {
    to_octet(CORBA_octet &buf_) : buf(buf_){}
    CORBA_octet &buf;
  };

/*!
 * @brief (TODO)
 */
  struct to_boolean {
    to_boolean(CORBA_boolean &buf_) : buf(buf_){}
    CORBA_boolean &buf;
  };

/*!
 * @brief (TODO)
 */
  struct to_string {
    to_string(char *& buf_) : buf(buf_){}
    to_string(const char *& buf_) : buf((char *&)buf_){}
    char * & buf;
  };

/*!
 * @brief (TODO)
 */
  struct to_object {
    to_object(CORBA::Object_ptr &ptr_) : ptr(ptr_) {}
    CORBA::Object_ptr &ptr;
  };

/*!
 * @brief (TODO)
 */	
  CORBA_boolean operator>>=(to_char o) const;

/*!
 * @brief (TODO)
 */
  CORBA_boolean operator>>=(to_boolean o) const;

/*!
 * @brief (TODO)
 */
  CORBA_boolean operator>>=(to_octet o) const;

/*!
 * @brief (TODO)
 */
  CORBA_boolean operator>>=(to_string o) const;

/*!
 * @brief (TODO)
 */
  CORBA_boolean operator>>=(to_object o) const;

/*!
 * @brief (TODO)
 */
  CORBA_boolean operator>>=(CORBA_any &o) const;

/*!
 * @brief (TODO)
 */
  CORBA_boolean operator>>=(CORBA_unsigned_long &n) const;

#endif
} CORBA_any ;

/*!
 * @if jp
 * @brief CORBA_any型データ用にメモリを確保し、TypeCodeとreleaseフラグをセットする。
 * @else
 * @brief allocate memory for CORBA_any data, and set TypeCode data and Release_Flag.
 * @endif
 * @param clear clear Flag
 * @return void
 */
void 	CORBA_any_alloc(CORBA_any *, struct CORBA_TypeCode_struct *, int32_t clear);

/*!
 * @if jp
 * @brief CORBA_any型データ内のコンテンツを全てクリアする。
 * @else
 * @brief clear all contents in CORBA_any data array.
 * @endif
 * @param any CORBA_any data
 * @return void
 */
void 	CORBA_any_clear(CORBA_any *any);

/*!
 * @if jp
 * @brief CORBA_any型データ内に、引数のコンテンツデータをセットする。
 * @else
 * @brief set contents data to CORBA_any data array.
 * @endif
 * @param data contents data
 * @param len length of array
 * @return void
 */
void    CORBA_any_set_value(CORBA_any*, struct CORBA_TypeCode_struct *, char *data, int32_t len);

/*!
 * @if jp
 * @brief TypeCodeに応じた、CORBA_any型データ内のコンテンツデータを返す。
 * @else
 * @brief return contents data releated with TypeCode from CORBA_any data array. 
 * @endif
 * @return reference of contents data
 */
void * 	CORBA_any_get_value(CORBA_any*);
	
/*!
 * @if jp
 * @brief CORBA_any型データ内のエンコード済みのCORBA_any_val型コンテンツデータを返す。また、その配列長をセットする。
 * @else
 * @brief return encoded contents data in CORBA_any data array.
 * @endif
 * @param any CORBA_any data
 * @param len length of CORBA_any_val data array
 * @return encoded data
 */
char * 	CORBA_any_get_encoded(CORBA_any *any, int32_t *len);

/*!
 * @if jp
 * @brief CORBA_any型データ内のコンテンツに例外情報のデータをセットする。
 * @else
 * @brief set contents exception data to CORBA_any data array.
 * @endif
 * @param any CORBA_any data
 * @param tc CORBA_TypeCode data
 * @param value contents exception data
 * @param release release Flag
 * @return void
 */
void 	CORBA_any_set_exception_value(CORBA_any *any, struct CORBA_TypeCode_struct * tc, void *value, CORBA_boolean release);

/*!
 * @if jp
 * @brief CORBA_any型データ内のコンテンツに例外情報のデータをセットする。
 * @else
 * @brief set contents exception data to CORBA_any data array.
 * @endif
 * @param any target of CORBA_any data
 * @param src buffer of CORBA_any data
 * @return void
 */
void 	CORBA_any_set_exception(CORBA_any *any, CORBA_any *src);

#ifdef __cplusplus

typedef CORBA_boolean         CORBA_boolean_var;
typedef CORBA_double          CORBA_double_var;
typedef CORBA_float           CORBA_float_var;
typedef CORBA_long            CORBA_long_var;
typedef CORBA_char            CORBA_char_var;
typedef CORBA_unsigned_long   CORBA_unsigned_long_var;
typedef CORBA_octet           CORBA_octet_var;
typedef CORBA_short           CORBA_short_var;
typedef CORBA_unsigned_short  CORBA_unsigned_short_var;


typedef CORBA_string CORBA_string_ptr;

/*!
 * @class CORBA_string_var
 * @if jp
 * @brief (TODO)
 * @else
 * @brief (TODO)
 * @endif
*/
class CORBA_string_var {
public:

/*!
 * @brief (TODO)
 */
  CORBA_string_var() : _str(0) {}

/*!
 * @brief (TODO)
 */
  CORBA_string_var(unsigned char *p) : _str(0)
  {
    _str = alloc((const char*)p);
  }

/*!
 * @brief (TODO)
 */    
  CORBA_string_var(char *p) : _str(0)
  {
    _str = alloc(p);
  }

/*!
 * @brief (TODO)
 */
  CORBA_string_var(const char *p) : _str(0)
  {
    _str = alloc(p);
  }
  ~CORBA_string_var();

/*!
 * @brief (TODO)
 */
  operator CORBA_string () { return _str; }

/*!
 * @brief (TODO)
 */
  CORBA_string _retn() {
    char *tmp = _str;
    _str = NULL;
    return tmp;
  }

/*!
 * @brief (TODO)
 */
  const char * in() {
    return _str;
  }

private:
  char *_str;  /*!< TODO */
  
private:
/*!
 * @brief (TODO)
 */
  char * alloc(const char *p);
};


typedef CORBA_wstring CORBA_wstring_ptr;

/*!
 * @class CORBA_wstring_var
 * @if jp
 * @brief (TODO)
 * @else
 * @brief (TODO)
 * @endif
*/
class CORBA_wstring_var {
public:

/*!
 * @brief (TODO)
 */
  CORBA_wstring_var() : _str(0) {}

/*!
 * @brief (TODO)
 */
  CORBA_wstring_var(unsigned char *p) : _str(0)
  {
    _str = alloc((const int16_t*)p);
  }
    
/*!
 * @brief (TODO)
 */
  CORBA_wstring_var(int16_t *p) : _str(0)
  {
    _str = alloc(p);
  }

/*!
 * @brief (TODO)
 */
  CORBA_wstring_var(const int16_t *p) : _str(0)
  {
    _str = alloc(p);
  }
  ~CORBA_wstring_var();

/*!
 * @brief (TODO)
 */
  operator CORBA_wstring () { return (short *)_str; }

/*!
 * @brief (TODO)
 */
  CORBA_wstring _retn() {
    short *tmp = (short *)_str;
    _str = NULL;
    return tmp;
  }

private:
  int16_t *_str;  /*!< TODO */
  
private:
/*!
 * @brief (TODO)
 */
  int16_t * alloc(const int16_t *p);
};

#endif

/*  Sequence */
/*!
 * @struct CORBA_SequenceBase
 * @brief CORBA_SequenceBase structure
 * @param _maximum max count of sequence array
 * @param _length count of sequence array
 * @param _buffer address of data
 * @param _release data release flag (TRUE:release)
 */
typedef struct{
  uint32_t _maximum;
  uint32_t _length;
  void * *_buffer;
/*
  boolean  _release;
*/
  uint32_t  _release;
} CORBA_SequenceBase;

/*!
 * @struct CORBA_Sequence
 * @brief CORBA_Sequence structure
 * @param _length count of sequence array
 * @param _maximum max count of sequence array
 * @param _buffer address of data
 * @param _release data release flag (TRUE:release)
 * @param _type data type
 */
typedef struct{
  uint32_t _length;
  uint32_t _maximum;
  void *_buffer;
/*
  boolean  _release;
*/
  uint32_t  _release;
  uint32_t _type;
} CORBA_Sequence;

/*!
 * @struct CORBA_Sequence_Octet
 * @brief CORBA_Sequence_Octet structure
 * @param _length count of sequence array
 * @param _maximum max count of sequence array
 * @param _buffer address of data
 * @param _release data release flag (TRUE:release)
 */
typedef struct {
  uint32_t _length;
  uint32_t _maximum;
  unsigned char *_buffer;
/*
  boolean  _release;
*/
  uint32_t  _release;
} CORBA_Sequence_Octet;

typedef CORBA_Sequence_Octet CORBA_String;

/*!
 * @struct CORBA_Environment
 * @brief CORBA_Environment (Exception Information)
 * @param _major Exception type (ex: CORBA_NO_EXCEPTION, CORBA_USER_EXCEPTION, CORBA_SYSTEM_EXCEPTIO ...)
 * @param _repo_id Exception Report ID
 * @param _params Exception parameter
 * @param _cpp_flag flag for C++ Interface
 */
typedef struct CORBA_Environment{
  uint32_t _major; /* Exception type */
  unsigned char *_repo_id;
  CORBA_any _params; 
  uint32_t _cpp_flag; /* flag for C++ Interface */
} CORBA_Environment;

/*-----------------------------------------------------------------*/
/*!
 * @struct CORBA_Ref
 * @brief CORBA reference
 * @param key object key
 * @param url CORBA url
 */
typedef struct CORBA_Ref{
  char *key;
  char *url;
} CORBA_Ref;

/*!
 * @struct CORBA_Config
 * @brief CORBA_Config (TODO)
 * @param server_id (TODO)
 * @param orb_id (TODO)
 * @param listenEndpoints (TODO)
 * @param flag_propietary_activation (TODO)
 * @param init_ref (TODO)
 * @param default_init_ref (TODO)
 * @param port (TODO)
 */
typedef struct CORBA_Config {
   char *server_id;
   char *orb_id;
   char *listenEndpoints;
   uint32_t flag_propietary_activation;
   PtrArray  *init_ref;
   char *default_init_ref;
   uint16_t port;
}CORBA_Config;

/*-----------------------------------------------------------------*/
/*!
 * @struct CORBA_InterfaceDef
 * @brief CORBA_InterfaceDef (TODO)
 * @param dummy (TODO)
 */
typedef struct{
	uint32_t dummy;
}CORBA_InterfaceDef;

/*! enum IOP_Profile_ID (TODO) */
enum CORBA_SetOverrideType{
	CORBA_SET_OVERRIDE,CORBA_ADD_OVERRIDE
};

/* Policy */
typedef uint32_t CORBA_PolicyType;

/*!
 * @struct CORBA_Policy_struct
 * @brief CORBA_Policy_struct (TODO)
 * @param policy_type (TODO)
 * @param state (TODO)
 */
typedef struct{
  CORBA_PolicyType policy_type;  /* readonly */
  CORBA_any state;
}CORBA_Policy_struct;

typedef CORBA_Policy_struct  * CORBA_Policy;

/*!
 * @struct CORBA_PolicyList
 * @brief CORBA_PolicyList (TODO)
 * @param _maximum (TODO)
 * @param _length (TODO)
 * @param _release data release flag (TRUE:release)
 * @param _policies (TODO)
 */
typedef struct{
  uint32_t _maximum;
  uint32_t _length;
  uint32_t _release;
  CORBA_Policy *_policies;
}CORBA_PolicyList;

/*!
 * @struct CORBA_PolicyTypeSeq
 * @brief CORBA_PolicyTypeSeq (TODO)
 * @param _maximum (TODO)
 * @param _length (TODO)
 * @param _release data release flag (TRUE:release)
 * @param _types (TODO)
 */
typedef struct{
  uint32_t _maximum;
  uint32_t _length;
  uint32_t _release;
  CORBA_PolicyType *_types;
}CORBA_PolicyTypeSeq;

typedef int32_t CORBA_PolicyErrorCode;

extern const CORBA_PolicyErrorCode BAD_POLICY;
extern const CORBA_PolicyErrorCode UNSUPPORTED_POLICY;
extern const CORBA_PolicyErrorCode BAD_POLICY_TYPE;
extern const CORBA_PolicyErrorCode BAD_POLICY_VALUE;
extern const CORBA_PolicyErrorCode UNSUPPORTED_POLICY_VALUE;

/*  Domain Manager */
/*!
 * @struct CORBA_DomainManager
 * @brief CORBA_DomainManager (TODO)
 * @param _id (TODO)
 */
typedef struct{
  uint32_t _id;
}CORBA_DomainManager;

/*!
 * @struct CORBA_DomainManagerList
 * @brief CORBA_DomainManagerList (TODO)
 * @param _maximum (TODO)
 * @param _length (TODO)
 * @param _release data release flag (TRUE:release)
 * @param _types (TODO)
 */
typedef struct{
  uint32_t _maximum;
  uint32_t _length;
  uint32_t _release;
  CORBA_DomainManager *_types;
}CORBA_DomainManagerList;

/*   ORB  */
/*!
 * @struct CORBA_ORB_struct
 * @brief ORB structure in RtORB
 * @param _id (Unused)
 * @param _threads (Unused) (for Multi Thread)
 * @param _adapters ORB内のオブジェクトアダプタのList(servantのlist)
 * @param _object_table ORB内のオブジェクトテーブル
 * @param _policies (Unused)
 * @param hostname ORBを起動したホスト名(IP address)
 * @param request_id (Unused)
 * @param poa_mgr_factory RootPOAManager
 * @param cfg 初期設定のコマンド引数などの処理用
 */
typedef struct CORBA_ORB_struct{
  unsigned char *_id;
  PtrArray *_threads;
  PtrArray *_adapters;
  hashtable *_object_table;
  CORBA_PolicyList *_policies;

  char *hostname;

  uint32_t request_id;

  struct PortableServer_POAManagerFactory_struct *poa_mgr_factory;

  CORBA_Config cfg;

} CORBA_ORB_struct;

typedef CORBA_ORB_struct * CORBA_ORB;

typedef  int32_t  CORBA_TCKind;
typedef  char *CORBA_RepositoryId;
typedef  char *CORBA_Identifier;
typedef  char *CORBA_Visibility;
typedef  char *CORBA_ValueModifier;

/*!
 * @struct CORBA_TypeCode_struct
 * @brief CORBA_TypeCode_struct (TODO)
 * @param parents (TODO)
 * @param kind (TODO)
 * @param repository_id (TODO)
 * @param identifier (TODO)
 * @param member_count (TODO)
 * @param member_name (TODO)
 * @param member_type (TODO)
 * @param member_label (TODO)
 * @param discriminator (TODO)
 * @param default_index (TODO)
 * @param length the number of elements count
 * @param content_type (TODO)
 * @param fixed_digits (TODO)
 * @param fixed_scale (TODO)
 * @param member_visibility (TODO)
 * @param type_modifier (TODO)
 * @param concrete_base_type (TODO)
 * @param size (TODO)
 * @param alignment (TODO)
 */
typedef struct CORBA_TypeCode_struct{
  void *			parents;
  CORBA_TCKind			kind;
  CORBA_RepositoryId		repository_id;
  CORBA_Identifier		identifier; 
  uint32_t		member_count; 	/* struct, union, enum, value, exception, event */
  CORBA_Identifier  		*member_name; 	/* struct, union, enum, value, exception, event */
  struct CORBA_TypeCode_struct 	**member_type; 	/* struct, union, value, exception, event */
  int32_t 			*member_label; 	/* union */
  struct CORBA_TypeCode_struct 	*discriminator; /* union */
  int32_t   			default_index;  /* union */
  uint32_t   		length; 	/* string, wstring, sequence, array */ 
  struct CORBA_TypeCode_struct 	*content_type;  /* sequence, array, value_box, alias */ 
  uint16_t		fixed_digits;   /* fixed */
  int16_t				fixed_scale;    /* fixed */
  CORBA_Visibility		*member_visibility;  	/* value, event */
  CORBA_ValueModifier		type_modifier;  	/* value, event */
  struct CORBA_TypeCode_struct 	*concrete_base_type;  	/* value, event */
  int16_t 			size;
  int16_t				alignment;

#ifdef __cplusplus
  const char *id() const { return repository_id; }
  const char *name() const { return identifier; }
#endif
	
}CORBA_TypeCode_struct;

typedef CORBA_TypeCode_struct *CORBA_TypeCode ;

/*!
 * @struct CORBA_IArg
 * @brief CORBA_IArg structure
 * @param tc CORBA Type Code
 * @param io flag (IN/OUT/INOUT)
 * @param name 
 */
typedef struct CORBA_IArg {
  CORBA_TypeCode tc;
  unsigned char  io;
  char *name;
}CORBA_IArg;

/*!
 * @struct CORBA_Class_Method
 * @brief CORBA_Class_Method (TODO)
 * @param name (TODO)
 * @param retval (TODO)
 * @param in_argc (TODO)
 * @param in_argv (TODO)
 * @param n_exceptinfo (TODO)
 * @param exceptinfo (TODO)
 */
typedef struct CORBA_Class_Method{
  char *name;
  CORBA_TypeCode retval;
/*
  void (*func)();
*/
  int32_t in_argc;
  CORBA_IArg *in_argv;
  int32_t n_exceptinfo;
  CORBA_TypeCode *exceptinfo;
}CORBA_Class_Method;

/*!
 * @struct CORBA_Class_Impl
 * @brief CORBA_Class_Impl (TODO)
 * @param tc (TODO)
 * @param name (TODO)
 * @param n_methods (TODO)
 * @param methods (TODO)
 * @param n_base_types (TODO)
 * @param base_types (TODO)
 */
typedef struct{
  CORBA_TypeCode tc;
  char *name;
  int32_t n_methods; 
  struct CORBA_Class_Method *methods;
  int32_t n_base_types;
  char **base_types;
} CORBA_Class_Impl;

extern const CORBA_TypeCode_struct TC_null_struct;
extern const CORBA_TypeCode_struct TC_void_struct;
extern const CORBA_TypeCode_struct TC_short_struct;
extern const CORBA_TypeCode_struct TC_long_struct;
extern const CORBA_TypeCode_struct TC_longlong_struct;
extern const CORBA_TypeCode_struct TC_ushort_struct;
extern const CORBA_TypeCode_struct TC_ulong_struct;
extern const CORBA_TypeCode_struct TC_ulonglong_struct;
extern const CORBA_TypeCode_struct TC_float_struct;
extern const CORBA_TypeCode_struct TC_double_struct;
extern const CORBA_TypeCode_struct TC_longdouble_struct;
extern const CORBA_TypeCode_struct TC_boolean_struct;
extern const CORBA_TypeCode_struct TC_char_struct;
extern const CORBA_TypeCode_struct TC_wchar_struct;
extern const CORBA_TypeCode_struct TC_octet_struct;
extern const CORBA_TypeCode_struct TC_any_struct;
extern const CORBA_TypeCode_struct TC_TypeCode_struct;
extern const CORBA_TypeCode_struct TC_Object_struct;
extern const CORBA_TypeCode_struct TC_string_struct;
extern const CORBA_TypeCode_struct TC_wstring_struct;
extern const CORBA_TypeCode_struct TC_ValueBase_struct;
extern const CORBA_TypeCode_struct TC_Component_struct;
extern const CORBA_TypeCode_struct TC_Home_struct;
extern const CORBA_TypeCode_struct TC_EventBase_struct;

#define TC_null		 ((CORBA_TypeCode)&TC_null_struct)
#define TC_void   	 ((CORBA_TypeCode)&TC_void_struct)
#define TC_CORBA_short	 ((CORBA_TypeCode)&TC_short_struct)
#define TC_CORBA_long		 ((CORBA_TypeCode)&TC_long_struct)
#define TC_CORBA_long_long	 ((CORBA_TypeCode)&TC_longlong_struct)
#define TC_CORBA_ushort	 ((CORBA_TypeCode)&TC_ushort_struct)
#define TC_CORBA_unsigned_short	 ((CORBA_TypeCode)&TC_ushort_struct)
#define TC_CORBA_ulong	 ((CORBA_TypeCode)&TC_ulong_struct)
#define TC_CORBA_unsigned_long	 ((CORBA_TypeCode)&TC_ulong_struct)
#define TC_CORBA_ulonglong	 ((CORBA_TypeCode)&TC_ulonglong_struct)
#define TC_CORBA_unsigned_long_long	 ((CORBA_TypeCode)&TC_ulonglong_struct)
#define TC_CORBA_float	 ((CORBA_TypeCode)&TC_float_struct)
#define TC_CORBA_double	 ((CORBA_TypeCode)&TC_double_struct)
#define TC_CORBA_long_double	 ((CORBA_TypeCode)&TC_longdouble_struct)
#define TC_CORBA_boolean	 ((CORBA_TypeCode)&TC_boolean_struct)
#define TC_CORBA_char		 ((CORBA_TypeCode)&TC_char_struct)
#define TC_CORBA_wchar	 ((CORBA_TypeCode)&TC_wchar_struct)
#define TC_CORBA_octet	 ((CORBA_TypeCode)&TC_octet_struct)
#define TC_CORBA_any		 ((CORBA_TypeCode)&TC_any_struct)
#define TC_CORBA_TypeCode	 ((CORBA_TypeCode)&TC_TypeCode_struct)
#define TC_CORBA_Object	 ((CORBA_TypeCode)&TC_Object_struct)
#define TC_CORBA_string	 ((CORBA_TypeCode)&TC_string_struct)
#define TC_CORBA_wstring	 ((CORBA_TypeCode)&TC_wstring_struct)
#define TC_CORBA_ValueBase	 ((CORBA_TypeCode)&TC_ValueBase_struct)
#define TC_CORBA_Component	 ((CORBA_TypeCode)&TC_Component_struct)
#define TC_CORBA_Home		 ((CORBA_TypeCode)&TC_Home_struct)
#define TC_CORBA_EventBase	 ((CORBA_TypeCode)&TC_EventBase_struct)

/*  For compatible to ORBit2 */
#define TC_CORBA_char_struct	 TC_char_struct
#define TC_CORBA_wchar_struct	 TC_wchar_struct
#define TC_CORBA_boolean_struct	 TC_boolean_struct
#define TC_CORBA_octet_struct	 TC_octet_struct
#define TC_CORBA_short_struct	 TC_short_struct
#define TC_CORBA_unsigned_short_struct	 TC_ushort_struct
#define TC_CORBA_long_struct	 TC_long_struct
#define TC_CORBA_unsigned_long_struct	 TC_ulong_struct
#define TC_CORBA_float_struct	 TC_float_struct
#define TC_CORBA_double_struct	 TC_double_struct
#define TC_CORBA_string_struct	 TC_string_struct
#define TC_CORBA_wstring_struct	 TC_wstring_struct
#define TC_CORBA_TypeCode_struct	 TC_TypeCode_struct
#define TC_CORBA_Object_struct	 TC_Object_struct
#define TC_CORBA_any_struct	 TC_any_struct

/*! enum IOP_ServiceId (SIMPLE:have only 1 param, COMLEX:have some params) */
enum { PLT_NOT_SUPPORTED = -1, PLT_NONE, PLT_SIMPLE, PLT_COMPLEX,};

int PARAM_LIST_TYPE(unsigned int kind);

#ifdef __cplusplus
}
#endif 


#endif
