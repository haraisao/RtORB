/*
  cdrMemoryStream 
*/
/*!
 * @file cdrStream.cpp
 * @if jp
 * @brief CDR Memory Stream Function for C++. (Marshal,UnMarshal,etc..)
 * @else
 * @brief CDR Memory Stream Function for C++. (Marshal,UnMarshal,etc..)
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */
#ifdef __cplusplus
#include <iostream>
#include <RtORB/cdrStream.h>

#if defined(Cygwin) && ( __GNUC__ < 4)
/*! array of Parameter List Type(PLT) and enum TypeCodes */
static int32_t paramListType[] =  {
  PLT_NONE,             /* tk_null                      // 0 */
  PLT_NONE,             /* tk_void                      // 1 */
  PLT_NONE,             /* tk_short                     // 2 */
  PLT_NONE,             /* tk_long                      // 3 */
  PLT_NONE,             /* tk_ushort                    // 4 */
  PLT_NONE,             /* tk_ulong                     // 5 */
  PLT_NONE,             /* tk_float                     // 6 */
  PLT_NONE,             /* tk_double                    // 7 */
  PLT_NONE,             /* tk_boolean                   // 8 */
  PLT_NONE,             /* tk_char                      // 9 */
  PLT_NONE,             /* tk_octet                     // 10 */
  PLT_NONE,             /* tk_any                       // 11 */
  PLT_NONE,             /* tk_TypeCode                  // 12 */
  PLT_NONE,             /* tk_Principal                 // 13 */
  PLT_COMPLEX,          /* tk_objref,                   // 14 */
  PLT_COMPLEX,          /* tk_struct,                   // 15 */
  PLT_COMPLEX,          /* tk_union,                    // 16 */
  PLT_COMPLEX,          /* tk_enum,                     // 17 */
  PLT_SIMPLE,           /* tk_string,                   // 18 */
  PLT_COMPLEX,          /* tk_sequence,                 // 19 */
  PLT_COMPLEX,          /* tk_array,                    // 20 */
  PLT_COMPLEX,          /* tk_alias,                    // 21 */
  PLT_COMPLEX,          /* tk_except,                   // 22 */
  PLT_NONE,             /* tk_longlong,                 // 23 */
  PLT_NONE,             /* tk_ulonglong,                // 24 */
  PLT_NONE,             /* tk_longdouble,               // 25 */
  PLT_NONE,             /* tk_wchar,                    // 26 */
  PLT_SIMPLE,           /* tk_wstring,                  // 27 */
  PLT_NOT_SUPPORTED,    /* tk_fixed,                    // 28 */
  PLT_NOT_SUPPORTED,    /* tk_value,                    // 29 */
  PLT_NOT_SUPPORTED,    /* tk_value_box,                // 30 */
  PLT_NOT_SUPPORTED,    /* tk_native,                   // 31 */
  PLT_NOT_SUPPORTED,    /* tk_abstract_interface,       // 32 */
  PLT_NOT_SUPPORTED,    /* tk_local_interface,          // 33 */
  PLT_NOT_SUPPORTED,    /* tk_component,                // 34 */
  PLT_NOT_SUPPORTED,    /* tk_home,                     // 35 */
  PLT_NOT_SUPPORTED,    /* tk_event,                    // 36 */

  tk_END
};

int
PARAM_LIST_TYPE(unsigned int kind){
 return paramListType[kind];
}

#endif

/*
  cdrStream
*/

cdrStream::cdrStream(): out_buf(0),out_len(0),out_c(0),byte_order(0){
}

cdrStream::~cdrStream(){
  if(out_buf) RtORB_free(out_buf, "~cdrStream");
}

void cdrStream::marshal_sequence(void *buf, int32_t size, CORBA_TypeCode tc){
  octet *_buf;
  char *_ptr;
  int len=0;
  void *val;
  CORBA_TypeCode _tc=tc->member_type[0]; 

  size >>= *this;
  /*std::cout << "=== Call marshal_sequence " << std::endl;*/

  _ptr = (char *)buf;
  _buf = (octet *)RtORB_alloc(1024,"marshal_sequence");
  for(int i=0 ; i < size ; i++){
    len=0;
    marshal_by_typecode(_buf, _ptr, _tc, &len);
    put_octet_array((char *)_buf, len, _tc->alignment);
    _ptr = _ptr + _tc->size;
  }
  RtORB_free(_buf,"marshal_sequence");
  return;
}

void *cdrStream::unmarshal_sequence(int32_t size, CORBA_TypeCode tc){
  void *res ;
  char *_ptr;
  int len, i;
  CORBA_TypeCode _tc=tc->member_type[0]; 

  res = RtORB_alloc(_tc->size * size, "unmarshal_sequence");
  memset(res, 0, _tc->size * size);

  _ptr = (char *)res;
  len=12;
  Address_Alignment(&len, _tc->alignment);
  for(i=0;i<size;i++){
    demarshal_by_typecode((void **)_ptr, _tc, (octet *)out_buf, &len, byte_order);
    _ptr +=  _tc->size;
  }
  return res;
}

void cdrStream::marshal_CORBA_any(CORBA_any any){
  int32_t len;

  CORBA_TypeCode tc_ = any._type;
  if (tc_ == NULL) { tc_ = CORBA_TypeCode_get(tk_null); }

  tc_ >>= *this;
 
  char * v = CORBA_any_get_encoded(&any, &len);
  if (v) {
    len >>= *this;
    put_octet_array(v, len, 1);
  } else {
     CORBA_any_val *val;
     do{
       if (!any._val) {
         fprintf(stderr, "marshal_CORBA_any : CORBA_any data is NULL");
         break;
       }
       if (!any._type) {
         fprintf(stderr, "marshal_CORBA_any : CORBA_any typecode is NULL");
         break;
       }
       val = any._val;
       switch(any._type->kind) {
         case tk_null:
         case tk_void:
           break;
         case tk_char:
           val->val_char >>= *this;
           break;
         case tk_octet:
           val->val_octet >>= *this;
           break;
         case tk_boolean:
           val->val_bool >>= *this;
           break;
         case tk_ulong:
           val->val_ulong >>= *this;
           break;
         case tk_string:
           val->val_str >>= *this;
           break;
         case tk_objref:
           val->val_obj >>= *this;
           break;
         case tk_except:
         case tk_struct:
           put_octet_array((char *)val->val_encoded.data, val->val_encoded.len, 4);
           /*do_assert(0, "CORBA_any_get_value : struct has to be marshaled");*/
           break;
         default:
           fprintf(stderr, "CORBA_any_get_value : typecode %d is not supported",
			 (int)any._type->kind);
           break;
         }
     }while(0);

  }
  return;
}

CORBA_any cdrStream::unmarshal_CORBA_any(){
  CORBA_any *any=new CORBA_any;
  any->_type <<= *this;
  if(!any->_type){
    fprintf(stderr, "Error in unmarshal_CORBA_any:cannot unmarshal typecoce\n");
  }
  switch(any->_type->kind) {
    case tk_null:
    case tk_void:
      break;
    case tk_char:
      any->_val->val_char <<= *this;
      break;
    case tk_octet:
      any->_val->val_octet <<= *this;
      break;
    case tk_boolean:
      any->_val->val_bool <<= *this;
      break;
    case tk_ulong:
      any->_val->val_ulong <<= *this;
      break;
    case tk_string:
      any->_val->val_str <<= *this;
      break;
    case tk_objref:
      any->_val->val_obj <<= *this;
      break;
    case tk_except:
    case tk_struct:
       get_octet_array((char *)any->_val->val_encoded.data, any->_type->size, 4);
       any->_val->val_encoded.len = any->_type->size;
       break;
    default:
      fprintf(stderr, "unmarshal_CORBA_any : typecode %d is not supported",
			 (int)any->_type->kind);
      break;
    }
  return *any;
}

void cdrStream::marshal_CORBA_TypeCode(CORBA_TypeCode tc){
  int32_t val;
  int i;
  cdrStream ss;

  if(!tc){
    val=tk_END;
    val >>= *this;
    return;
  }

  SKIP_ALIAS(tc);

  tc->kind  >>= *this;
  
  switch( PARAM_LIST_TYPE(tc->kind) ) {
  case PLT_NONE:
    return;
  case PLT_SIMPLE:
    val=0;
    val >>= *this;
    return;
  case PLT_COMPLEX:
    {
      switch(tc->kind) {
      case tk_string:
	break;
      case tk_struct:
	{
          char val_oct = RTORB_BYTE_ORDER;
          val_oct >>= ss;
	  tc->repository_id >>= ss;
	  tc->identifier >>= ss;
	  tc->member_count >>= ss; 

	  for (i=0; i<tc->member_count; i++) {
	    CORBA_TypeCode ctc = tc->member_type[i];
	    
	    tc->member_name[i] >>= ss;
	    ctc >>= ss;
	  }
	}
	break;
      case tk_objref:
        {
          char val_oct = RTORB_BYTE_ORDER;
          val_oct >>= ss;
	  tc->repository_id >>= ss;
	  tc->identifier >>= ss;
        }
	break;
      case tk_sequence:
	{
          char val_oct = RTORB_BYTE_ORDER;
          val_oct >>= ss;
	  CORBA_TypeCode ctc = tc->member_type[0];
	    
	  ctc >>= ss;
	  tc->length >>= ss;
	}
	break;
      default:
	fprintf(stderr, "marshal_CORBA_TypeCode(type = %d) : not supported type",
		 (int)tc->kind);
	break;
      }
      int32_t len = ss.out_len;
      len >>= *this; 
      put_octet_array((char *)ss.out_buf, len, 1);
    }
    break;
  default:
    fprintf(stderr, "marshal_CORBA_TypeCode(type = %d) : not supported type", 
		(int)tc->kind);
    break;
  }

  return;
}

CORBA_TypeCode cdrStream::unmarshal_CORBA_TypeCode(){
  CORBA_TypeCode tc;
  CORBA_TCKind kind;

  kind <<=  *this;
  if (kind == tk_END) { return NULL; }
  
  switch( PARAM_LIST_TYPE(kind) ){
    case PLT_NONE:
      return CORBA_TypeCode_get(kind);
    case PLT_COMPLEX:
      {
        int sz;
        sz <<= *this;
      
        switch(kind) {
          case tk_struct:
	  {
	    int slen;
	    char order;
	    char *repoid, *id;
  
            order <<= *this;
	    repoid <<=  *this;
	    id <<= *this;

	    tc = CORBA_TypeCode_get_dynamic(tk_struct, repoid, id);
	    if (tc) {
	      fprintf(stderr, "unmarshal_TypeCode : struct %s\n", tc->repository_id);
	    } else {
              fprintf(stderr, "unmarshal_typecode(type = %d) : cannot find struct %s(%s)\n", tc->kind, id, repoid);
	    }
	    RtORB_free(repoid, "unmarshal_TypeCode(repoid)");
	    RtORB_free(id, "unmarshal_TypeCode(id)");
	  }
	  break; 
          case tk_objref:
	    {
	      int len;
	      CORBA_boolean obj_order;
	      char *repoid, *id;
	      obj_order <<= *this;
	      repoid <<= *this;
	      id <<= *this;
	      tc = CORBA_TypeCode_get_dynamic(tk_objref, repoid, id);
	      if (!tc) {
	        fprintf(stderr, "cannot get dynamic typecode : %s\n", repoid);
	      }
	      RtORB_free(repoid, "unmarshal_typecode");
	      RtORB_free(id, "unmarshal_typecode");
	    }
	    break;
          default:
	    fprintf(stderr, "unmarshal_typecode(type = %d) : not implemented\n", (int)kind);
	    break;
          }
          return tc;
        }
    case PLT_SIMPLE:
      switch(kind) {
        case tk_string:
          {
  	    int32_t sz;
  	    sz <<= *this;
          }
          return CORBA_TypeCode_get(kind);
        default:
          break;
       }
    case PLT_NOT_SUPPORTED:
    default:
      fprintf(stderr, "unmarshal_TypeCode(type = %d) : not supported type\n", (int)kind);
      break;
  }
  return tc;
}

void cdrStream::marshal_CORBA_Object(CORBA_Object obj){
  char *ior;
  char *ior_byte;
  int len;
  CORBA_Environment env;

  if(!obj){
    int l=1;
    char val='\0';
    l >>= *this;
    val >>= *this;
    l=0;
    l >>= *this;
  } else if (obj->_url){
    obj->_url[0]._ior_string_len >>= *this;
    obj->_url[0]._ior_string >>= *this;
    put_octet_array(obj->_url[0]._ior_string, obj->_url[0]._ior_string_len, 1); 
  }else{
    if(!obj->_ior_string){ CORBA_Object__to_string(obj, &env); }
    ior = (char *)obj->_ior_string;
    ior_byte = (char *)String2Octet(ior + 12); 
    len = strlen(ior+12) / 2;
    len >>= *this;
    ior_byte >>= *this;
    put_octet_array(ior_byte, len, 1);
    RtORB_free(ior_byte, "marshal_by_typecode:objref");
  }
  return;
}

CORBA_Object cdrStream::unmarshal_CORBA_Object(){
  CORBA_Object obj = NULL;
  unsigned char num_urls = 0;
  CORBA_URL *url;
  int ior_start = this->out_c;
  int ior_len = 0;

  Address_Alignment((int *)&this->out_c, 4);
  if (this->out_buf != NULL) {
     num_urls = parseIOR(&url, (octet *)this->out_buf, (int *)&this->out_c, this->byte_order);
  }
  if (num_urls > 0) {
    obj = new_CORBA_Object(NULL);
    obj->num_urls = num_urls;
    obj->_url = url;
    ior_len = this->out_c - ior_start;
    obj->orb = NULL;
    std::cerr << std::endl << "GIOP_Connection__create:cdrStream::unmarshal_CORBA_Object()" << std::endl;
    obj->connection = GIOP_Connection__create();
    obj->connection->hostname = (unsigned char *)RtORB_strdup(obj->_url[0].hostname,
				       "demarshal");
    obj->connection->port = obj->_url[0].port;
    obj->poa = 0;
    //	    if(obj->_url[0].type_id) obj->typedId = obj->_url[0].type_id;

    RtORB_free(obj->object_key, "demarshal_by_typecode(objref)");
    obj->object_key = (unsigned char *)RtORB_strdup(obj->_url[0].object_key,
		     "demarshal_by_typecode(objref)");
    { 
      CORBA_Environment ev;
      memset(&ev, 0x00, sizeof(ev));
      PortableServer_POA poa = PortableServer_root_POA(&ev);
      if (CORBA_ORB_find_object(poa, obj, &ev)) {
        obj->poa = poa;
      }
    }
  } 
  return obj;
}

////////////////////////////
// cdrMemoryStream

cdrMemoryStream::cdrMemoryStream(int32_t initBufsize, int32_t clsMemory)
{
  out_c=0;
  byte_order=0;

  if(initBufsize){
    out_buf = RtORB_alloc(initBufsize, "cdrMemoryStream");
    out_len= initBufsize;
  }else{
    out_buf=0;
    out_len=0;
  }
  if(clsMemory){
    memset(out_buf, 0, out_len);
  }
}

cdrMemoryStream::cdrMemoryStream(void *databuffer, int32_t maxLen)
{
  out_c=0;
  byte_order=0;

  out_buf = RtORB_alloc(maxLen, "cdrMemoryStream");
  memcpy(out_buf, databuffer,maxLen);
  out_len= maxLen;
}

cdrMemoryStream::cdrMemoryStream(const cdrMemoryStream& obj)
{
  out_c=0;
  byte_order=obj.byte_order;
  out_len= obj.out_len;

  out_buf = RtORB_alloc(out_len,"cdrMemoryStream");
  memcpy(out_buf, obj.out_buf, out_len);
}

/*
void cdrMemoryStream::rewindInputPtr(){
  return;
}
*/

void cdrMemoryStream::rewindPtrs(){
  out_c = 0;
  return;
}

uint32_t cdrMemoryStream::bufSize() const{
  return out_len;
}

void *cdrMemoryStream::bufPtr() const{
  return out_buf;
}

void cdrMemoryStream::setByteSwapFlag(int32_t order){
  byte_order = order;
  return;
}

int32_t cdrMemoryStream::readOnly(){
  return 0;
}

/*
void* cdrMemoryStream::ptrToClass(int *cptr){
  if(cptr == &cdrMemoryStream::_classid) return (cdrMemoryStream*)this;
  if(cptr == &cdrStream::_classid) return (cdrStream*)this;
  return 0;
}
*/

void cdrStream::get_octet_array(char *octet, int32_t size, int32_t align){
  int pad;
  int pos = out_c; 
  if((pad = pos % align)){
    pos += align - pad;
    if (pos+size > out_len) return;
  }
  memcpy(octet, (void *)((char *)out_buf+pos), size);
  out_c = pos+size;
  return;
}

void cdrStream::put_octet_array(char *octet, int32_t size, int32_t align){
  int pad;
  int pos = out_c; 

  if((pad = pos % align)) pos += align - pad;
  if(pos+size > out_len){
    if(out_buf) out_buf = RtORB_realloc(out_buf, size+pos, "put_octet_array");
    else out_buf=RtORB_alloc(size+pos, "put_octet_array");
    out_len=size+pos;
  }
  memcpy((void *)((char *)out_buf+pos), octet, size);
  out_c = pos+size;
  return;
}

void cdrMemoryStream::skipInput(uint32_t size){
  if(out_c + size > out_len){
    out_buf = RtORB_realloc(out_buf, out_c+size, "skipInput");
    out_len = out_c+size;
  }
  out_c += size;
  return;
}

/*
int  cdrMemoryStream::checkInputOverrun(uint32_t itemSize, uint32_t nItems, int align){
  int pos = out_c; 
  int pad;

  if((pad=pos % align)) pos += align - pad;
  if(pos+itemSize*nItems > out_len) return 1;
  
  return 0;
}
*/

int32_t cdrMemoryStream::checkOutputOverrun(uint32_t itemSize, uint32_t nItems, int32_t align){

  int pad;
  int pos = out_c; 

  if((pad=pos % align)) pos += align - pad;
  if(pos+itemSize*nItems > out_len) return 1;

  return 0;
}

/*
void  cdrMemoryStream::copy_to(cdrStream &cs, int size, int align=1){
  return;
}

void  cdrMemoryStream::fetchInputData(int align, int size){
  return;
}

int  cdrMemoryStream::reseveOutputSpaceForPrimitiveType(int align, int size){
  return 0;
}

int  cdrMemoryStream::maybeReseveOutputSpace(int align, int size){
  return 0;
}


uint32_t cdrMemoryStream::currentInputPtr() const{
  
  return 0;
}
*/

uint32_t cdrMemoryStream::currentOutputPtr() const{
  return (uint32_t)((long)out_buf + out_c);
}

cdrMemoryStream& cdrMemoryStream::operator=(const cdrMemoryStream &cms){
  out_len = cms.out_len;
  byte_order = cms.byte_order;
  if(out_buf) RtORB_free(out_buf, "cdrStream:operator=");
  if(cms.out_len > 0){
    out_buf = RtORB_alloc(cms.out_len, "cdrStream:operator=");
    memcpy(out_buf, cms.out_buf, cms.out_len);
  }
  out_c = 0;
  return *this;
}

#endif //__cplusplus__
