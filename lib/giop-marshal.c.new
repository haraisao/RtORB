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
 *   IIOP-GIOP
 *
 *  Copyright(C) 2006, Isao Hara, AIST
 *
 *  $Revision: 1.17 $
 *  $Date: 2008/06/04 15:08:03 $
 *  $Id: giop-marshal.c,v 1.17 2008/06/04 15:08:03 yoshi Exp $
 */
/*!
 * @file giop-marshal.c
 * @if jp
 * @brief GIOP関連データのマーシャリング/アンマーシャリング処理。
 * @else
 * @brief GIOP Marshaling and deMarshaling actions.
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#include <RtORB/corba.h>
/*
#include <RtORB/giop.h>
#include <RtORB/hashtable.h>
#include <RtORB/util.h>
#include <RtORB/functions/giop-marshal.h>
#include <RtORB/functions/typecode-func.h>
*/


/*!
 * @if jp
 * @brief 入力データ'tc'から、各要素をマーシャリングして、出力用バッファ'buf'に対し書き込む。まずは、kind値を書き込む。以降に、Param_List_Type値とtypecode種類を考慮した場合分けを行い、CORBA_TypeCode型データの各成分を書き込む。
 * @else
 * @brief Marshal CORBA_TypeCode data 'tc' and write to buffer 'buf'. First, write 'kind' value. Next, write each parameter considering Param_List_Type related with 'kind' value.
 * @endif
 * @param buf target buffer for putting data
 * @param current current writable position of buffer (address) 
 * @param tc CORBA_TypeCode data
 * @return '0' (Always)
 */
static int marshal_typecode(octet *buf, int *current, CORBA_TypeCode tc);
/*!
 * @if jp
 * @brief 入力データ'buf'に対し、アンマーシャリングを行い、TypeCodeの'kind'値を取得し、Param_List_Typeを考慮し、対応するCORBA_TypeCodeデータを返す。
 * @else
 * @brief Unmarshal input data 'buf' considering several kinds of TypeCode, get 'kind' value,  return CORBA_TypeCode data considering Param_List_Type related with 'kind' value.
 * @endif
 * @param buf reference buffer for getting data
 * @param current current readable position of buffer (address) 
 * @param order byte_order(LittleEndian/BigEndian)
 * @return unmarshaled CORBA_TypeCode data
 */
static CORBA_TypeCode demarshal_typecode(octet *buf, int *current, int order);

/*
   Shift current position with an alignment of data.
*/
void Address_Alignment(int *current, int base)
{
  int pad;

  if (base <= 0) return;
  if((pad = *current % base)) *current += base - pad;
  return;
}

/*
   Shift current pointer with an alignment of data.
*/
void *Align_Pointer_Address(void *val, uint32_t base)
{
  uint32_t pad;
  char *tmpval;

  if (!base) return val;
  tmpval = (char *)val;
  if((pad = (unsigned long)tmpval % base) != 0){
    tmpval += base - pad;
  }
  return (void *)tmpval;
}

/*
  Return a size of data based on the COBRA_TypeCode.
  if 'flag' euqals to zero  then for a native data structure, other for CORBA's.
*/
uint32_t size_of_typecode(CORBA_TypeCode tc, int flag)
{
  int i;
  int size;

  SKIP_ALIAS(tc);

  switch(tc->kind){
    case tk_null:
    case tk_void:
       return 0;
    case tk_octet:
    case tk_char:
    case tk_boolean:
       return 1;

    case tk_short:
    case tk_ushort:
       return 2;
	
    case tk_long:
    case tk_ulong:
    case tk_float:
       return 4;

    case tk_double:
       return 8;

    case tk_any:
       return sizeof(CORBA_any);

    case tk_sequence:
       return sizeof(CORBA_SequenceBase);

    case tk_except:
    case tk_struct:
       for(i=0, size=0; i<tc->member_count;i++){
           Address_Alignment(&size, align_of_typecode(tc->member_type[i], flag));
           size += size_of_typecode(tc->member_type[i], flag);
           if (i == (tc->member_count - 1)) {
               Address_Alignment(&size, align_of_typecode(tc, flag)); /* Last padding */
           }
       }
       return size;
    case tk_union:
      /* union type in IDL is converted to struct type */
       for(i=0, size=0; i<tc->member_count;i++){
           Address_Alignment(&size, align_of_typecode(tc->member_type[i], flag));
           size += size_of_typecode(tc->member_type[i], flag);
       }
       size += size_of_typecode(tc->discriminator, flag);
       return size;
    case tk_objref:
       return sizeof(CORBA_Object);

    case tk_string:
       if(flag == F_MARSHAL) return 4;
       return sizeof(void *);

    default:
      if (tc->size) return tc->size;
      else return sizeof(void *);
  }
}

/*
  Return an alignment of data based on CORBA_TypeCode.
  A meaning of 'flag' is same as 'size_of_typecode' function.
*/
uint32_t align_of_typecode(CORBA_TypeCode tc, int flag)
{
  int i;
  uint32_t align, max_align;

  SKIP_ALIAS(tc);

  switch(tc->kind){
    case tk_null:
    case tk_void:
       return 1;
    case tk_sequence:
       return 4;
    case tk_struct:  /* Modified to get Max_Align. */
    case tk_union:
       for(i=0, max_align=0; i<tc->member_count;i++){
	   if((align = align_of_typecode(tc->member_type[i], flag)) > max_align) max_align = align;
       }
       return max_align;
    case tk_string:
       if(flag == F_MARSHAL) return 4;
       return sizeof(void *);
    default:
      if (tc->alignment) return tc->alignment;
      else return 1;
  }
  return tc->alignment;
}

/*
  This function similar to memory copy...
*/
int copy_octet(void *dist, octet *buf, int size, int *current)
{
   memcpy(dist, buf + (*current), size);
   *current += size;
   return *current;
}

/****************/
/*!
 * @if jp
 * @brief UnionデータからTypeCodeに対応した、格納されたデータ型の種類を表すindex(UnionSwitch)を返す。
 * @else
 * @brief Return UnionSwitch(index which indicate type of stored data) by TypeCode from input union data.
 * @endif
 * @param tc CORBA_TypeCode data  
 * @param val void pointer of union type's data
 * @return index which indicate type of stored data (union)
 */
int32_t get_union_switch(CORBA_TypeCode tc, void *val)
{
  int32_t retval = 0;
  SKIP_ALIAS (tc);

  switch (tc->kind) {
    case tk_ulong:
    case tk_long:
    case tk_enum: 
      {
	int32_t tmp;
        memcpy (&tmp, val, sizeof (int32_t));
  	retval = tmp;
        break;
      }
    case tk_ushort:
    case tk_short:
     {
        short tmp;
	memcpy (&tmp, val, sizeof (short));
        retval = (int32_t) tmp;
        break;
      }
    case tk_char:
    case tk_boolean:
    case tk_octet:
         retval = *(octet *)val;
         break;
    default:
	 fprintf(stderr, "Error in get_union_switch(%d)\n", (int)tc->kind);
         break;
   }
    return retval;
}

/*************************************
  Marshaling data to an octed buffer.
**************************************/
/*
    Marshal a byte of integer data.
*/
int marshalOctet(octet *buf, int *pos, octet val)
{
  buf[*pos] = val;
  *pos += 1;
  return *pos;
}

/*
    Marshal two bytes of integer data.
*/
int marshalShort(octet *buf, int *pos, short val)
{
  Address_Alignment(pos, 2);
  memcpy(buf+(*pos), &val, 2);
  *pos += 2;
  return *pos;
}

/*
    Marshal four bytes of integer data.
*/
int marshalLong(octet *buf, int *pos, int32_t val)
{
  Address_Alignment(pos, 4);
  memcpy(buf + (*pos), &val, 4);
  *pos += 4;
  return *pos;
}
/*
    Marshal float-typed data.
*/
int marshalFloat(octet *buf, int *pos, float val)
{
  Address_Alignment(pos, 4);
  memcpy(buf + (*pos), &val, 4);
  *pos += 4;
  return *pos;
}

/*
    Marshal double-typed data.
*/
int marshalDouble(octet *buf, int *pos, double val)
{
  Address_Alignment(pos, 8);
  memcpy(buf+(*pos), &val, 8);
  *pos += 8;
  return *pos;
}

/*
    Marshal string data.
*/
int marshalString(octet *dist, int *pos, char *str)
{
  int len = 0;

  if(!str){
    marshalLong(dist, pos, len);
    fprintf(stderr, "  marshalString NULL (%x)\n",
	 (unsigned int)((long)dist+(*pos)));
  }else{
    len = strlen(str)+1;
    marshalLong(dist, pos, len);
    memcpy(dist + (*pos), str, len);
    *pos += len;
  }
  return *pos;
}

/*
  Marshal CORBA_string.
*/
int marshal_CORBA_String(octet *dist, int *pos, CORBA_String *str)
{
  marshalLong(dist, pos, str->_length);
  memcpy(&dist[*pos], str->_buffer, str->_length);
  *pos += str->_length;
  return *pos;
}

/*
  Marshal an octed sequence.
*/
int marshal_Octet_Sequence(octet *buf, int *pos, octet *data, int size)
{
  marshalLong((octet *)buf, pos, size);
  memcpy(buf + (*pos), data, size);
  *pos += size;

  return *pos;
}

/*
  Marshal an CORBA_Sequence. I support only a basic data type....
*/
int marshal_CORBA_Sequence(octet *buf, int *pos, CORBA_SequenceBase *seq, int type)
{
  uint32_t len = sizeof_CORBA_Sequence(seq, type);

  marshalLong((octet *)buf, pos, len);
  switch(type){
    case tk_char:
    case tk_octet:
      memcpy(buf+(*pos), seq->_buffer, seq->_length);
      *pos += seq->_length;
      break;
    case tk_short:
    case tk_ushort:
      memcpy(buf+(*pos), seq->_buffer, seq->_length*2);
      *pos += seq->_length*2;
      break;
    case tk_long:
    case tk_ulong:
    case tk_enum:
    case tk_float:
      memcpy(buf+(*pos), seq->_buffer, seq->_length*4);
      *pos += seq->_length*4;
      break;
    case tk_longlong:
    case tk_double:
      memcpy(buf+(*pos), seq->_buffer, seq->_length*8);
      *pos += seq->_length*8;
      break;
    default:
      fprintf(stderr, "ERROR in marshal_CORBA_Sequence, sorry we dont support this type(%d)\n", type); 
      break;
   }
   return *pos;
}

/*
  Marshal an CORBA_Sequence.
*/
int marshal_IOP_SerivceContextList(IOP_ServiceContextList *service_context, octet *buf, int *current)
{
  int i;
  marshalLong(buf, current, service_context->num);
  if(service_context->num){
    for(i=0; i<service_context->num; i++){
      IOP_ServiceContext *context =  &service_context->data[i];
       marshalLong(buf, current, context->context_id);
       marshal_CORBA_String(buf, current, &context->context_data);
    }
  }
  return *current;
}

/*****
  unmarshaling a octet sequence to native data.
 *****/
/*
  ummarshal to octet data.
*/
octet demarshalOctet(char *buf, int *pos)
{
   octet res = buf[*pos];
   *pos += 1;

   return res;
}

/*
  ummarshal to two bytes integer.
  if 'order' equals to zero, swap byte-order.
*/
short demarshalShort(octet *buf, int *pos, int order)
{
  short val;
  char buf_f[2];

  Address_Alignment(pos, 2);
  if(order == 0) swapByteOrder((octet *)buf_f, (buf + (*pos)), 2);
  else memcpy(buf_f, buf + (*pos) , 2);
  *pos += 2;

  memcpy(&val, buf_f, 2);
  return val;
}

/*
  ummarshal to four bytes integer.
  if 'order' equals to zero, swap byte-order.
*/
int32_t demarshalLong(octet *buf, int *pos, int order)
{
  int32_t val;
  char buf_f[4];

  Address_Alignment(pos, 4);
  if(order == 0) swapByteOrder((octet *)buf_f, (buf + (*pos)), 4);
  else memcpy(buf_f, buf + (*pos) , 4);
  *pos += 4;

  memcpy(&val, buf_f, 4);
  return val;
}

/*
  ummarshal to float data.
  if 'order' equals to zero, swap byte-order.
*/
float demarshalFloat(octet *buf, int *pos, int order)
{
  float val;
  char buf_f[4];

  Address_Alignment(pos, 4);
  if(order == 0) swapByteOrder((octet *)buf_f, (buf + (*pos)), 4);
  else memcpy(buf_f, buf + (*pos) , 4);
  *pos += 4;
  memcpy(&val, buf_f, 4);

  return val;
}

/*
  ummarshal to double data.
  if 'order' equals to zero, swap byte-order.
*/
double demarshalDouble(octet *buf, int *pos, int order)
{
  double val;
  char buf_d[8];

  Address_Alignment(pos, 8);
  if(order == 0) swapByteOrder((octet *)buf_d, (buf + (*pos)), 8);
  else memcpy(buf_d, buf + (*pos) , 8);
  *pos += 8;
  memcpy(&val, buf_d, 8);
  return val;
}

/*
  ummarshal to string data.
  if 'order' equals to zero, swap byte-order.
*/
char *demarshalString(octet *src, int *pos, int byte_order, int *size)
{
  char *res;
  int32_t len;

  len = demarshalLong(src, pos, byte_order);
  res = (char *)RtORB_alloc(len, "demarshalString");
  memcpy(res, &src[*pos], len);
  *pos += len;
  *size = len;
   return res;
}

/*
  ummarshal to CORBA_String data.
  if 'order' equals to zero, swap byte-order.
*/
void demarshal_CORBA_String(CORBA_String *str, octet *src, int *pos, int order)
{
  str->_maximum = str->_length = demarshalLong(src, pos, order);
  if(str->_length){
     str->_buffer = (unsigned char *)RtORB_alloc(str->_length+1, "demarshal_CORBA_String");
     memcpy(str->_buffer, &src[*pos], str->_length);
     str->_buffer[str->_length]='\0';
     *pos += str->_length;
  }
  return;
}

/*
  ummarshal to IOP_ServiceContext data.
  if 'order' equals to zero, swap byte-order.
*/
void demarshal_IOP_SerivceContext(IOP_ServiceContext *context, octet *buf, int *current, int order)
{
   context->context_id = demarshalLong(buf, current, order);
   demarshal_CORBA_String(&context->context_data, buf, current, order);
   Address_Alignment(current, 4);
   return;
}

/*
  ummarshal to IOP_ServiceContextList data.
  if 'order' equals to zero, swap byte-order.
*/
void demarshal_IOP_SerivceContextList(IOP_ServiceContextList *service_context,
		octet *buf, int *current, int order)
{
  int i;
  service_context->num = demarshalLong(buf, current, order);
  if(service_context->num > 0)
  {
    service_context->data= (IOP_ServiceContext *)RtORB_alloc(sizeof(IOP_ServiceContext)*service_context->num,
					 "demarshal_IOP_SerivceContextList");
    for(i=0; i < service_context->num; i++)
    {
      demarshal_IOP_SerivceContext(&service_context->data[i], buf, current, order);
    }
  }
  return;
}

/****************/
/*
  ummarshal to data by CORBA_TypeCode.
  if 'order' equals to zero, swap byte-order.
*/
uint32_t
demarshal_by_typecode(void **dist, CORBA_TypeCode tc, octet *buf, int *current, int order)
{
  int len;

  ASSERT(tc != 0, "demarshal_by_typecode");

  SKIP_ALIAS(tc);

#ifdef DEBUG_MARSHAL
     fprintf(stderr, "  >>> %s : %d<<< (%d)\n", tc->repository_id,tc->kind, *current);
#endif

  switch(tc->kind)
  {
    case tk_null:
    case tk_void:
      return 0;

    case tk_char:
    case tk_octet:
    case tk_boolean:
      {
	char v = demarshalOctet((char *)buf, current);
	if (dist) { *((char *)dist) = v; }
      }
      break;
      
    case tk_short:
    case tk_ushort:
      {
	short v = demarshalShort(buf, current, order);
	if (dist) { *((short *)dist) = v; }
      }
      break;

    case tk_long:
    case tk_ulong:
    case tk_enum:
      {
	int32_t v = demarshalLong(buf, current, order);
	if (dist) { *((int32_t *)dist) = v; }
      }
      break ;
      
    case tk_float:
      {
	float v = demarshalFloat(buf, current, order);
	if (dist) { *((float *)dist) = v; }
      }
      break ;

    case tk_double:
      {
        double v = demarshalDouble(buf, current, order);
        if (dist) { *((double *)dist) = v; }
      }
      break;

    case tk_string:
      {
        len = demarshalLong(buf, current, order);
        if (dist) {
#if 1  /* Free() (But, need to comment out in [C++_Wrapper ver]...) */
          if(*dist) {
            RtORB_free(*dist, "Free {demarshal_by_typecode(string)}");
            *dist = NULL;
          }
#endif
          *dist = (void *)RtORB_alloc(len, "demarshal_by_typecode(string)");
	  copy_octet(*dist, buf, len, current);
        } else {
          *current += len;
        }
      }
      break;

    case tk_sequence:
      {
        int i;
        int32_t len, offset;
        void **s_tmp = NULL;
        CORBA_SequenceBase *sb = NULL;

        int MaxSize = 2000000;  /* Max SequeceData Size (For alloc) */
        int32_t buf_size = 0;
        void **s_tmp_base;

	len = demarshalLong(buf, current, order);

        if (dist) {
          sb = (CORBA_SequenceBase *)dist;
          if (sb->_maximum > 0) {
             /* Just free buffer. DO NOT free CORBA_Sequence object. */
            RtORB__free(sb->_buffer, "demarshal_type_typecode : tk_sequence");
            sb->_buffer = NULL;
          }
          sb->_length = sb->_maximum = len;
        }

	if (len > 0) {
          if (sb != NULL) {
            s_tmp = s_tmp_base = (void**)RtORB_alloc(MaxSize, "demarshal_by_typecode(sequence) (maxsize)");  /* Allocate by MaxSize as Buffer */
            if (!s_tmp_base) return 0;

	    for(i=0;i<len;i++){
              offset = demarshal_by_typecode(s_tmp, tc->member_type[0], buf, current, order);  

              if (s_tmp) {
#if DEBUG
                dump_value_by_typecode(s_tmp, tc->member_type[0]);
#endif
                s_tmp = (void **)((char*)s_tmp + offset); 
	      }
	    }
            buf_size = (char*)s_tmp - (char*)s_tmp_base; /* Get needed buffer size */
            sb->_buffer = (void**)RtORB_alloc(buf_size, "demarshal_by_typecode(sequence) (buffer)"); /* Allocate buffer */
            if (!sb->_buffer) {
                RtORB__free(s_tmp_base, "demarshal_by_typecode(sequence) (free)");
                s_tmp_base = NULL;
                return 0;
            } else {
                memcpy(sb->_buffer, s_tmp_base, buf_size);  /* Set unmarshaled data */
                RtORB__free(s_tmp_base, "demarshal_by_typecode(sequence) (free)");
                s_tmp_base = NULL;
            }
          } /* if(sb) */
	} /* if(len) */

       }
       break;

     case tk_except:
     case tk_struct:
       {
          int i;
          uint32_t tmp;
          void **args = dist;

          Address_Alignment(current, align_of_typecode(tc,F_DEMARSHAL)); /* Align by MaxSize*/
          /* Shift for Struct Padding (case: sequence<struct>) */
          args = (void**)Align_Pointer_Address((void*)args, align_of_typecode(tc, F_DEMARSHAL));

  	  for(i=0;i<tc->member_count;i++){
            /* Shift for Members Padding */
            args = (void**)Align_Pointer_Address((void*)args, align_of_typecode(tc->member_type[i], F_DEMARSHAL));  /* Each members align*/

            tmp = demarshal_by_typecode(args, tc->member_type[i],buf, current, order);  

            if (args) {
#if DEBUG
               dump_value_by_typecode(args,  tc->member_type[i]);
#endif
               args = (void **)((char *)args + tmp);

               /* Add padding after last member by aligment of Struct(max alignment of members) */
               if (i == (tc->member_count - 1)) {
                 args = (void**)Align_Pointer_Address((void*)args, align_of_typecode(tc, F_DEMARSHAL));  /* Last align */
               }
             }
          } /* for() */
        }
        break;

      case tk_union:
	{
	  int i;
	  int n;
          void **        discrim;
          void **args = dist;
	  int 		idx;
	  char   tmp[128];

	  discrim = args? args: (void **)tmp;
          Address_Alignment(current, align_of_typecode(tc->discriminator, F_DEMARSHAL));
          demarshal_by_typecode (discrim, tc->discriminator, buf, current, order);

	  idx = get_union_switch(tc->discriminator, discrim);
	  if (args) {
	    n = size_of_typecode(tc->discriminator,F_DEMARSHAL);
	    Address_Alignment(&n, 4); /* Consider struct alignment!! */
	    args = (void **)((char *)args + n);
	  }

	  if (idx != tc->default_index) {
	    Address_Alignment(current, align_of_typecode(tc, F_DEMARSHAL));
	    for (i=0; i<tc->member_count; i++) {
	      CORBA_TypeCode ctc = tc->member_type[i];
	      if (tc->member_label[i] == idx) {
		demarshal_by_typecode (args, ctc, buf, current, order);
	      }
	      if (args) {
		n = size_of_typecode(ctc,F_DEMARSHAL);
		Address_Alignment(&n, 4);
		args = (void **)((char *)args + n);
	      }
	    }
	  }
	}
        break;

      case tk_objref:
	{
	    CORBA_Object obj = NULL;
	    unsigned char num_urls = 0;
	    CORBA_URL *url;
	    int ior_start = *current;
	    int ior_len = 0;

            Address_Alignment(current, 4);
            if (buf != NULL) {
              num_urls = parseIOR(&url, buf, current, order);
            }

	    if (num_urls > 0 && dist) {
	      obj = new_CORBA_Object(NULL);
	      obj->num_urls = num_urls;
	      obj->_url = url;
	      ior_len = *current - ior_start;

#ifdef DEBUG_MARSHAL
              fprintf(stderr, "\n\tCall GIOP_Connection__create in demarshal_by_typecode( %s:%d )\n",
                                       obj->_url[0].hostname, obj->_url[0].port);
#endif
	      obj->connection = GIOP_Connection__create();
	      RtORB_free(obj->connection->hostname, "demarshal_by_typecode");
	      obj->connection->hostname = (unsigned char *)RtORB_strdup(obj->_url[0].hostname,
						       "demarshal");
	      obj->connection->port = obj->_url[0].port;

	      RtORB_free(obj->object_key, "demarshal_by_typecode(objref)");
	      obj->object_key = (unsigned char *)RtORB_strdup(obj->_url[0].object_key,
					     "demarshal_by_typecode(objref)");
	      *dist = obj;
#if DEBUG_MARSHAL
              fprintf(stderr, "Check objct reference ref=%d\n",obj->ref);
#endif
	    } 
	 }
        break;

      case tk_any:
	{
	  void **args = NULL;

	  CORBA_TypeCode tc_;
	  CORBA_any * any = (CORBA_any*)dist;

	  Address_Alignment(current, 4);

	  tc_ = demarshal_typecode(buf, current, order);
	  if (!tc_) {
	    fprintf(stderr, "cannot demarshal typecode");
	  }
	  /* Just calculate data length. Any demarshal it later. */

	  if (tc_ && tc_->kind == tk_struct) {

	    /*
	       Struct type hold binary data because there's no function to copy
	       struct data deeply. So when struct data copy occurs, just copy bi
	       nary data.
	       
	       Considering byte order difference, demarshal and marshal again.
	    */
	    void **ptr;
	    Address_Alignment(current, 4);
	    ptr = (void **)RtORB_alloc_by_typecode(tc_, 1, "demarshal_by_typecode(any)");
	    demarshal_by_typecode((void **)ptr, tc_, buf, current, order);
	    {
              char *buf_ = NULL;

              buf_ = (char *)RtORB_alloc( 4096, "demarshal_by_typecode:tk_any" );
              memset(buf, 0, 4096);

	      int len = 0;
	      marshal_by_typecode((octet *)buf_, ptr, tc_, &len);
	      CORBA_any_set_value(any, tc_, buf_, len);

              if ( buf_ ){ RtORB_free(buf_, "demarshal_by_typecode:tk_any"); }
	    }

	    RtORB_free_by_typecode(tc_, ptr, 1, 0);
#if 0
            CORBA_any_clear(any);
#endif
	  } else {
	    CORBA_any_alloc(any, tc_, 0);
	    args = (void**) CORBA_any_get_value(any);
	    if (args) {
	      demarshal_by_typecode(args, tc_, buf, current, order);
	    }
	  }
	    
	  break;
	}
      default:
        fprintf(stderr, "demarshal_by_typecode: %d is not supported\n",
			(int)tc->kind );
        return 0;
  }

  return size_of_typecode(tc,F_DEMARSHAL);
}

/*
 * deMarshal_Arg:
 *   demarshal arguments on server side.
 *   This function called from 'invokeServant2'
 */
void **
deMarshal_Arg(octet *buf, int i_args, CORBA_IArg *i_argv, int order){
  int i;
  int current = 0;
  void **argv;

  if (i_args <= 0) { return (void**) NULL; }
  
  argv = (void **)RtORB_alloc(sizeof(void *) * i_args, "deMarshal_Arg(argv)" );

  for(i=0;i<i_args;i++){
    /* Memory of the in ant in-out argment is allocated.  */
    /* That of the out argment is NOT.    */
    if(i_argv[i].io == CORBA_I_ARG_IN || i_argv[i].io == CORBA_I_ARG_INOUT )
    {
      argv[i] = RtORB_alloc_by_typecode(i_argv[i].tc, 1, "deMarshal_Arg");
      demarshal_by_typecode((void **)argv[i], i_argv[i].tc, buf, &current, order);
    } else {
      CORBA_TypeCode tc = i_argv[i].tc;
      SKIP_ALIAS(tc);
      switch(tc->kind) {
      case tk_sequence:
      case tk_union:
	argv[i] = RtORB_alloc_by_typecode(i_argv[i].tc, 1, "deMarshal_Arg(out)");

	break;
      default:
	argv[i] = (void *)RtORB_alloc(sizeof(void *), "deMarshal_Args2(argv[i])" );
	*(void **)argv[i] = (void *)RtORB_alloc_by_typecode(i_argv[i].tc, 1, "deMarshal_Arg(out)");
	break;
      }
    }
  }
  return argv;
}

/*!
 * @if jp
 * @brief 入力データ'buf'に対し、各TypeCodeに応じたアンマーシャリングを行った結果の'retval'と、取得したデータサイズ'size'を取得する。
 * @else
 * @brief Get unmarshaled data 'retval' and data size of it by unmarshal input data 'buf' considering several kinds of TypeCode.
 * @endif
 * @param retval unmarshaled data
 * @param tc CORBA_TypeCode data
 * @param buf reference buffer for getting data (input)
 * @param size data size(= current readable position of buffer (address))
 * @param order byte_order(LittleEndian/BigEndian)
 * @return void
 */
void
deMarshal_Result(void **retval, CORBA_TypeCode tc, octet *buf,
		int *size, int order){
   SKIP_ALIAS(tc);

   switch(tc->kind){
     case tk_union:
       if (CORBA_TypeCode_is_fixed_size(tc)) {
	 /* Returned type is value. */
	 demarshal_by_typecode(retval, tc, buf, size, order);
	 break;
       } /* else go thru */
     case tk_struct:
     case tk_sequence:
     case tk_any:
       {
	 /* Returned type is pointer. */
	 void **val = (void **)RtORB_typecode_alloc(tc);
         demarshal_by_typecode(val, tc, buf, size, order);
          *retval = val;
       }
       break;
     default:
       /* Returned type is value. */
       demarshal_by_typecode(retval, tc, buf, size, order);
       break;
   }
   return;
}
/*
 * deMarshal_Arguments:
 *     demarchal result and argumnets on client side
 *     This function called from 'invokeMethod_via_GIOP'.
 */
void 
deMarshal_Arguments(void **retval, void **args, octet *buf,
		CORBA_Class_Method *method, int order){
  int i;
  int current = 0;
  int size = 0;

  if (retval){
     deMarshal_Result(retval, method->retval, buf, &size, order);
     buf += size;
  }

  for(i=0;i<method->in_argc;i++){
    if(method->in_argv[i].io == CORBA_I_ARG_OUT) {
      demarshal_by_typecode((void**)*(void**)args[i], method->in_argv[i].tc,
		      buf , &current, order);
    } else if (method->in_argv[i].io == CORBA_I_ARG_INOUT ){
      demarshal_by_typecode((void**)args[i], method->in_argv[i].tc,
		      buf , &current, order);
    }
  }
  return ;
}

/*
 *  marshal_by_typecode:
 *     marshal values by CORBA_TypeCode.
 *     this function called from 'Marshal_Argments2' and 'Marshal_Arg'
 */
int marshal_by_typecode(octet *buf, void *argv, CORBA_TypeCode tc, int *current)
{
  if(!tc) return 0;
  CORBA_TypeCode_add_dynamic(tc);

#ifdef DEBUG_MARSHAL
  fprintf(stderr, "  [[[ %s ]]](%d)\n", tc->repository_id, *current);
#endif
  
  SKIP_ALIAS(tc);

  switch(tc->kind){
    case tk_null:
    case tk_void:
      return *current;
    case tk_char:
    case tk_octet:
    case tk_boolean:
      marshalOctet(buf, current, *((char *)argv));
      break;
    case tk_short:
      marshalShort(buf, current, *((short *)argv));
      break;
    case tk_ushort:
      marshalShort(buf, current, *((unsigned short *)argv));
      break;
    case tk_long:
      marshalLong(buf, current, *((int32_t *)argv));
      break;
    case tk_ulong:
    case tk_enum:
      marshalLong(buf, current, *((uint32_t *)argv));
      break;
    case tk_float:
      marshalFloat(buf, current, *((float *)argv));
      break;
    case tk_double:
      marshalDouble(buf, current, *((double *)argv));
      break;
    case tk_string:
      marshalString(buf, current, *((char **)argv));
      break;
    case tk_sequence:
      {
        int i;
        char *_buffer;
        int cpos=0;
        int align_base;

        int skip =  size_of_typecode(tc->member_type[0], F_DEMARSHAL);
        CORBA_SequenceBase *sb = (CORBA_SequenceBase *)argv;

        marshalLong(buf, current, sb->_length);

        align_base = align_of_typecode(tc->member_type[0],F_DEMARSHAL);
        for(i=0;i<sb->_length;i++){
          Address_Alignment(&cpos, align_base);
          _buffer = (char *)sb->_buffer + cpos;
          marshal_by_typecode(buf, (void **)_buffer, tc->member_type[0], current);
          cpos += skip;
        }
      }
      break;
    case tk_except:
    case tk_struct:
      {
        int i;
        int cpos=0;
        void **_buffer;
        CORBA_TypeCode ctc;

        Address_Alignment(current, align_of_typecode(tc,F_MARSHAL));

        for(i=0;i<tc->member_count;i++){
          ctc = tc->member_type[i];
          Address_Alignment(&cpos, align_of_typecode(ctc,F_DEMARSHAL));
          _buffer = (void **)((char *)argv + cpos);

          marshal_by_typecode(buf, _buffer, ctc, current);
          cpos = cpos + size_of_typecode(ctc, F_DEMARSHAL);
        }
      }
      break;

    case tk_union:
      {
        int i;
        int n;
        int idx;
        void **args = (void **)argv;
        void **discrim = (void **)argv;

        Address_Alignment(current, align_of_typecode(tc, F_MARSHAL));

        marshal_by_typecode (buf, args, tc->discriminator, current);
        idx = get_union_switch(tc->discriminator, discrim);
        n = size_of_typecode(tc->discriminator, F_DEMARSHAL);
        Address_Alignment(&n, sizeof(void *)); /* Consider struct alignment */
        args = (void **)((char *)args + n);

        if (idx != tc->default_index) {
          for(i=0;i<tc->member_count;i++){
            CORBA_TypeCode ctc = tc->member_type[i];
            if ((int32_t)tc->member_label[i] == idx) {
              marshal_by_typecode(buf, args, ctc, current);
            }
            n = size_of_typecode(ctc, F_DEMARSHAL);
            Address_Alignment(&n, sizeof(void *));
            args = (void **)((char *)args + n);
          }
	}
      }
      break;

    case tk_objref:
      {
        CORBA_Object obj = *(CORBA_Object *)argv;
        char *ior;
        char *ior_byte;
        int len;
        CORBA_Environment env;

        Address_Alignment(current, 4);

        if(!obj){
          marshalLong(buf, current, 1);
          marshalOctet(buf, current, '\0');
          marshalLong(buf, current, 0);
        } else if (obj->_url){
          memcpy(buf + (*current), obj->_url[0]._ior_string, obj->_url[0]._ior_string_len);
          *current += obj->_url[0]._ior_string_len;
        }else{
          if(!obj->_ior_string){
            CORBA_Object__to_string(obj, &env);
          }
          ior = (char *)obj->_ior_string;
          ior_byte = (char *)String2Octet(ior + 12); 
          len = strlen(ior+12) / 2;
	  memcpy(buf + (*current), ior_byte, len);
	  RtORB_free(ior_byte, "marshal_by_typecode:objref");
          *current += len;
       }
     }
     break;
   case tk_any:
     {
       CORBA_any *any = (CORBA_any *)argv;
       CORBA_TypeCode tc_ = any->_type;

       if (tc_ == NULL) { tc_ = CORBA_TypeCode_get(tk_null); }
          Address_Alignment(current, 4);
	  marshal_typecode(buf, current, tc_);
         int32_t len;
         char *v = CORBA_any_get_encoded(any, &len);
         if (v) {
           memcpy(&buf[*current], v, len); *current += len;
         } else {
           void *args = CORBA_any_get_value(any);
           if (args) {
#ifdef DEBUG_MARSHAL
	     fprintf(stderr, "  === marshal any type : args %d\n", tc_? tc_->kind: tk_END);
#endif
              marshal_by_typecode(buf, (void **)args, tc_, current);
#ifdef DEBUG_MARSHAL
              fprintf(stderr, "  === End of marshal any type : args %d\n", tc_? tc_->kind: tk_END);
#endif
            }
         }
      }
      break;

    default:
      fprintf(stderr,"marshal_by_typecode: Unknown type(%d)\n", (int)tc->kind);
      break;
  }
  return *current;
}

/*
 * Marshal_Arg:
 *    marchal out or inout arguments, called from 'Marshal_Reply_Arguments'
 *
 * In GIOP reply message, Argument data is needed when there's in argument.
 *
 */
int
Marshal_Arg(octet *buf, void * *argv, int i_args, CORBA_IArg *i_argv){
  int i;
  int current = 0;
  int out = 0;

  for(i=0;i<i_args;i++){
    if(i_argv[i].io == CORBA_I_ARG_OUT ||
       i_argv[i].io == CORBA_I_ARG_INOUT) {
      out = 1;
    }
  }
  /* no in argument */
  if (!out) { return 0; }

  for(i=0;i<i_args;i++){
    if(i_argv[i].io == CORBA_I_ARG_OUT) {
      marshal_by_typecode(buf, *(void**)argv[i], i_argv[i].tc, &current);
    } else if (i_argv[i].io == CORBA_I_ARG_INOUT ){
      marshal_by_typecode(buf, argv[i], i_argv[i].tc, &current);
    }
  }
  return current;
}

/*
 * Marshal_Reply_Arguments function
 *
 *   Called only from Marshal_Reply_Arguments function.
 */
void
Marshal_Result(char *reply_buf, void * *result,
			CORBA_TypeCode tc, int *size)
{
   SKIP_ALIAS(tc);
   switch(tc->kind){
     case tk_struct:
     case tk_sequence:
       marshal_by_typecode((octet *)reply_buf, *result, tc, size);
       break;
     default:
       marshal_by_typecode((octet *)reply_buf, result, tc, size);
       break;
   }
   return;
}
/*
 * Marshal_Reply_Arguments:
 *    marshal reply values on server side.
 *    This function called from 'invokeServant2'.
 */
void
Marshal_Reply_Arguments(GIOP_ReplyBody *reply, 
			void * *result,
			void * *argv,
			CORBA_Class_Method *m)
{
   char *reply_buf = NULL;
   int size = 0;
   int MaxSize = 2000000;

   reply_buf = (char *)RtORB_alloc( MaxSize,"Marshal_Reply_Arguments");
   if(! reply_buf ){ 
     fprintf(stderr, "Error in Marshal_Reply_Arguments: Fail to allocate buffer...\n");
    return;
   }
   memset(reply_buf, 0, MaxSize);

   Marshal_Result(reply_buf, result, m->retval, &size);
#ifdef DEBUG_MARSHAL
   fprintf(stderr, "  ==== End of Marshal_Result\n");
#endif
   size += Marshal_Arg((octet *)(reply_buf+ size), argv, m->in_argc, m->in_argv);

   reply->body_size = size;
   if (size <= 0) {
     if ( reply_buf ){ RtORB_free( reply_buf, "Marshal_Reply_Arguments"); }
     return;
   }
   
   if (reply->body){
      reply->body = (octet *) RtORB_realloc(reply->body, reply->body_size,
      			"Marshal_Reply_Argument2(Body, realloc)");
   }else{
      reply->body = (octet *) RtORB_alloc(reply->body_size,
		      "Marshal_Reply_Argument2(Body, malloc)");
   }

   memcpy(reply->body, reply_buf, reply->body_size);
   RtORB_free( reply_buf, "Marshal_Reply_Arguments");

   return;
}

/*
   marshal CORBA_TypeCode

*/
static int marshal_typecode(octet *buf, int *current, CORBA_TypeCode tc)
{
  if (!tc) {
    marshalLong(buf, current, tk_END);
    return 0;
  }
	
  SKIP_ALIAS(tc);

  marshalLong(buf, current, tc->kind);
  
  switch( PARAM_LIST_TYPE(tc->kind) ) {
  case PLT_NONE:
    return 0;
  case PLT_SIMPLE:
    marshalLong(buf, current, 0);
#ifdef DEBUG_MARSHAL
    fprintf(stderr, "  marshal_type_code(type = %d) : PLT_SIMPLE\n", tc->kind);
#endif
    return 0;
  case PLT_COMPLEX:
    {
      int len_ = 0;
      octet *buf_ = NULL;

      buf_ = ( octet* )RtORB_alloc( 4096, "marshal_typecode(PLT_COMPLEX)");
      memset(buf_, 0, 4096); 
	  
      switch(tc->kind) {
      case tk_string:
	break;
      case tk_struct:
	{
	  int i;
	  
	  marshalOctet(buf_, &len_, RTORB_BYTE_ORDER);
	  marshalString(buf_, &len_, tc->repository_id);
	  marshalString(buf_, &len_, tc->identifier);
	  marshalLong(buf_, &len_, tc->member_count); /* member_count*/

	  for (i=0; i<tc->member_count; i++) {
	    CORBA_TypeCode ctc = tc->member_type[i];
	    
	    marshalString(buf_, &len_, tc->member_name[i]);
	    Address_Alignment(&len_, 4);
	    marshal_typecode(buf_, &len_, ctc);
	    Address_Alignment(&len_, 4);
	  }
	}
	break;
      case tk_objref:
	marshalOctet(buf_, &len_, RTORB_BYTE_ORDER);
	marshalString(buf_, &len_, tc->repository_id);
	marshalString(buf_, &len_, tc->identifier);
	break;
      case tk_sequence:
	marshalOctet(buf_, &len_, RTORB_BYTE_ORDER);
	Address_Alignment(&len_, 4);
	{
	    CORBA_TypeCode ctc = tc->member_type[0];
	    
	    marshal_typecode(buf_, &len_, ctc);
	    Address_Alignment(&len_, 4);
	    marshalLong(buf_, &len_, tc->length);
	  }
	break;
      default:
	fprintf(stderr, "marshal_type_code(type = %d) : not supported type",
		 (int)tc->kind);
	break;
      }
      /* datalen */
      marshalLong(buf, current, len_); 
      if (len_ > 0) {
	memcpy(&buf[*current], buf_, len_);
	*current += len_;
      } else {
#if DEBUG
	fprintf(stderr, "marshal_type_code(type = %d) : data size is ZERO", (int)tc->kind);
#endif
      }
      if( buf_ ){ RtORB_free( buf_, "marshal_typecode:PLT_COMPREX" ); }
    }
    break;
      /* go thru */
  default:
    fprintf(stderr, "marshal_type_code(type = %d):not supported type", 
		(int)tc->kind);
    break;
  }

  return 0;
}

/*
  unmarshal CORBA_TypeCode
*/
static CORBA_TypeCode demarshal_typecode(octet *buf, int *current, int order)
{
  CORBA_TCKind kind;

  kind = demarshalLong(buf, current, order);
  if (kind == tk_END) { return NULL; }
  
  switch( PARAM_LIST_TYPE(kind) ){
  case PLT_NONE:
    return CORBA_TypeCode_get(kind);
  case PLT_COMPLEX:
    {
      CORBA_TypeCode tc = NULL;
      int32_t sz = demarshalLong(buf, current, order);
      
#ifdef DEBUG_MARSHAL
      fprintf(stderr, "  dmarshal_type_code(type = %d) : PLT_COMPLEX size = %d\n", (int)kind, sz);
#endif
      switch(kind) {
      case tk_struct:
	{
	  int slen;
	  octet ordr;
	  int pos = *current;
	  ordr = demarshalOctet((char *)buf, &pos); /* Byte-order */
	  char *repoid = demarshalString((octet *)buf, &pos, order, &slen);
	  char *id = demarshalString((octet *)buf, &pos, order, &slen);

	  tc = CORBA_TypeCode_get_dynamic(tk_struct, repoid, id);
	  RtORB_free(repoid, "demarshal_typecode(repoid)");
	  RtORB_free(id, "demarshal_typecode(id)");
	}
	break; 
      case tk_objref:
	{
	  int pos = *current;
	  int len;
	  CORBA_boolean obj_order = demarshalOctet((char *)buf, &pos);
	  char *repoid = demarshalString((octet *)buf, &pos, obj_order, &len);
	  char *id = demarshalString((octet *)buf, &pos, obj_order, &len);
	  tc = CORBA_TypeCode_get_dynamic(tk_objref, repoid, id);
	  if (!tc) {
	    fprintf(stderr, "cannot get dynamic typecode : %s\n", repoid);
	  }
	  RtORB_free(repoid, "demarshal_typecode");
	  RtORB_free(id, "demarshal_typecode");
	}
	break;
      default:
	fprintf(stderr, "demarshal_typecode(type = %d) : not implemented\n", (int)kind);
	break;
      }
      *current += sz;
      return tc;
    }
  case PLT_SIMPLE:
    switch(kind) {
    case tk_string:
      {
	int32_t sz;
	sz = demarshalLong(buf, current, order);
#ifdef DEBUG_MARSHAL
	fprintf(stderr, "  demarshal_type_code(type = %d) : PLT_SIMPL size = %d\n", (int)kind, sz);
#endif
      }
      return CORBA_TypeCode_get(kind);
    default:
      break;
    }
    /* go thru */
  case PLT_NOT_SUPPORTED:
  default:
    fprintf(stderr, "demarshal_type_code(type = %d) : not supported type\n", (int)kind);
    break;
  }
  return NULL;
}

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

