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
 *  $Revision: 1.3 $
 *  $Date: 2008/05/20 05:33:03 $
 *  $Id: giop-msg.c,v 1.3 2008/05/20 05:33:03 yoshi Exp $
 */
/*!
 * @file giop-msg.c
 * @if jp
 * @brief GIOP関連処理
 * @else
 * @brief GIOP actions
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#include <RtORB/corba.h>
#include <RtORB/giop.h>
#include <RtORB/hashtable.h>

/*  GIOP_MessageHeader *GIOP_Create_MessageHeader(); */

/******* GIOP ************/
GIOP_Version GIOP_1_0 = {1,0};
GIOP_Version GIOP_1_1 = {1,1};
GIOP_Version GIOP_1_2 = {1,2};
GIOP_Version GIOP_1_3 = {1,3};


/************************/
uint32_t request_counter = 0;

uint32_t next_request_id(){
  return ++request_counter;
}

/************************/
octet *setSystemException( GIOP_SystemExceptionReplyBody *e_body, char *id,  int code, int state){
  int pos=0;

  octet *rep = (octet *)RtORB_alloc( strlen(id) + 1 + 8, "setSystemException"); 
  memset(rep, 0, strlen(id) + 1 +8); 
  e_body->exception_id._length = strlen(id) + 1;
  e_body->exception_id._buffer = (unsigned char *)RtORB_strdup(id, "setSystemException");

  e_body->minor_code_value = code;
  e_body->completion_status = state;

  marshal_CORBA_String(rep, &pos, &e_body->exception_id);
  memcpy(rep+ e_body->exception_id._length, e_body, 8);
  return rep;
}

/*
 *
 */
void delete_CORBA_Sequence_Octet(CORBA_Sequence_Octet *seq, int flag){
  if(seq->_maximum){
    RtORB_free(seq->_buffer, "delete_CORBA_Sequence_Octet(buffer)");
  }
  if (flag){
    RtORB_free(seq, "delete_CORBA_Sequence_Octet");
  }
}

void delete_CORBA_Sequence_Octet2(CORBA_Sequence_Octet *seq, int flag){
  if(seq->_maximum){
    RtORB_free(seq->_buffer, "delete_CORBA_Sequence_Octet(buffer)");
  }
  if (flag){
    RtORB_free(seq, "delete_CORBA_Sequence_Octet(seq)");
  }
}

#define delete_CORBA_String(str, flag)  delete_CORBA_Sequence_Octet(str, flag)

void delete_IOP_ServiceContextList(IOP_ServiceContextList *list, int flag){
  int i;
  for(i=0; i< list->num; i++){
    IOP_ServiceContext *context = &list->data[i];
    delete_CORBA_Sequence_Octet(&context->context_data, flag);
  }
  RtORB_free(list->data, "delete_IOP_ServiceContextList");
}

void delete_CORBA_TargetAddress(GIOP_TargetAddress *target, unsigned short type, int flag){
  switch(type){
    case GIOP_KeyAddr:
     delete_CORBA_Sequence_Octet(&target->object_key, flag);
     return;
    case GIOP_ProfileAddr:
    case GIOP_ReferenceAddr:
    default:
      return;
  }
}

GIOP_RequestHeader *newRequestHeader(){
  GIOP_RequestHeader *header = (GIOP_RequestHeader *)RtORB_alloc(sizeof(GIOP_RequestHeader), "newRequestHeader");
  memset(header, 0, sizeof(GIOP_RequestHeader));
  return header;
}

GIOP_ReplyHeader *newReplyHeader(){
  GIOP_ReplyHeader *header = (GIOP_ReplyHeader *)RtORB_alloc(sizeof(GIOP_ReplyHeader), "newReplyHeader");
  memset(header, 0, sizeof(GIOP_ReplyHeader));
  return header;
}

GIOP_CancelRequestHeader *newCancelRequestHeader(){
  GIOP_CancelRequestHeader *header = (GIOP_CancelRequestHeader *)RtORB_alloc(sizeof(GIOP_CancelRequestHeader), "newCancelRequestHeader");
  memset(header, 0, sizeof(GIOP_CancelRequestHeader));
  return header;
}

GIOP_LocateRequestHeader *newLocateRequestHeader(){
  GIOP_LocateRequestHeader *header = (GIOP_LocateRequestHeader *)RtORB_alloc(sizeof(GIOP_LocateRequestHeader), "newLocateRequestHeader");
  memset(header, 0, sizeof(GIOP_LocateRequestHeader));
  return header;
}

GIOP_LocateReplyHeader *newLocateReplyHeader(){
  GIOP_LocateReplyHeader *header = (GIOP_LocateReplyHeader *)RtORB_alloc(sizeof(GIOP_LocateReplyHeader), "newLocateReplyHeader");
  memset(header, 0, sizeof(GIOP_LocateReplyHeader));
  return header;
}

CORBA_Sequence_Octet *new_CORBA_Sequence_Octet(int size){
  CORBA_Sequence_Octet *seq = (CORBA_Sequence_Octet *)RtORB_alloc(sizeof(CORBA_Sequence_Octet), "new_CORBA_Sequence_Octet");
  seq->_length = size;
  seq->_maximum = size;
  seq->_release = 0;
  if(size > 0){
    seq->_buffer = (octet *)RtORB_alloc(size, "new_CORBA_Sequence_Octet(buffer)");
    memset(seq->_buffer, 0, size);
  }else{
    seq->_buffer = (octet *)NULL;
  }
  return seq;
}

CORBA_Sequence_Octet *new_CORBA_Sequence_Octet2(int size){
  CORBA_Sequence_Octet *seq = (CORBA_Sequence_Octet *)RtORB_alloc(sizeof(CORBA_Sequence_Octet), "new_CORBA_Sequence_Octet2");

  seq->_length = size;
  seq->_maximum = size;
  seq->_release = 0;
  if(size > 0){
    seq->_buffer = (octet *)RtORB_alloc(size, "new_CORBA_Sequence_Octet(buffer)");
    memset(seq->_buffer, 0, size);
  }else{
    seq->_buffer = (octet *)NULL;
  }
  return seq;
}

void set_CORBA_Sequence_Octet(CORBA_Sequence_Octet *seq, octet *val, int size){

  if(size > seq->_maximum){
     if(seq->_buffer){
	   RtORB_free(seq->_buffer, "set_CORBA_Sequence_Octet(buffer)");
     }
     seq->_buffer = (octet *)RtORB_alloc(size,"set_CORBA_Sequence_Octet(buffer)");
     seq->_maximum = size;
  }
  seq->_length = size;
  memcpy(seq->_buffer, val, size);

  return;
}

void clear_CORBA_Sequence_Octet(CORBA_Sequence_Octet *seq){
  seq->_length = 0;
  return ;
}

void deleteRequestHeader(GIOP_RequestHeader *header, int version){
  if(!header) return;
  if(version < 2){
    delete_IOP_ServiceContextList(&header->_1_0.service_context, 0);
    delete_CORBA_Sequence_Octet(&header->_1_0.object_key, 0);
    delete_CORBA_String(&header->_1_0.operation, 0);
    delete_CORBA_Sequence_Octet(&header->_1_0.requesting_principal, 0);
/*
    if (version == 1) free(&header->_1_1.reserved);
*/
  }else{
    delete_IOP_ServiceContextList(&header->_1_2.service_context, 0);
    delete_CORBA_TargetAddress(&header->_1_2.target, header->_1_2.target_type, 0);
    delete_CORBA_String(&header->_1_2.operation, 0);
/*
    free(&header->_1_2.reserved);
*/
  }
	 
  RtORB_free(header,"deleteRequestHeader");

  return ;
}

void deleteSystemExceptionReplyBody(GIOP_SystemExceptionReplyBody *exp){
  if(!exp) return;
  delete_CORBA_String(&exp->exception_id, 0);
  RtORB_free(exp, "deleteSystemExcepionReplyBody");
}

void deleteReplyBody(GIOP_ReplyBody *body){
  if(!body) return;
  if(body->body){
    RtORB_free(body->body, "deleteReplyBody(body)");
  }
/*
  deleteSystemExceptionReplyBody(&body->exception);
*/
  RtORB_free(body, "deleteReplyBody");
}

void deleteReplyHeader(GIOP_ReplyHeader *header, int version){
  if(!header) return;
  if(version < 2){
    delete_IOP_ServiceContextList(&header->_1_0.service_context, 0);
  }else{
    delete_IOP_ServiceContextList(&header->_1_2.service_context, 0);
  }

  RtORB_free(header, "deleteReplyHeader");
  return ;
}

void deleteLocateRequestHeader(GIOP_LocateRequestHeader *header, int version){
  if(!header) return;
  if(version < 2){
    delete_CORBA_Sequence_Octet(&header->_1_0.object_key, 0);
  }else{
    delete_CORBA_TargetAddress(&header->_1_2.target, header->_1_2.target_type, 0);
  }

  RtORB_free(header, "deleteLocateRequestHeader");

  return ;
}
/***************************************/
/*    demarshal Messages               */
/*
 *
 * Request Header
 */
int deMarshalRequest(GIOP_RequestHeader *header,
		CORBA_Sequence_Octet *body,
		char *buf, GIOP_MessageHeader *m_header){

  int version = m_header->version.minor;

  switch(version){
    case 0: case 1:
       if(deMarshalRequest_1_0(header, body, buf, m_header) < 0 ){
	 fprintf(stderr, "Error: Invalid RequestHeader\n");
         deleteRequestHeader(header, version);
         return -1;
       }
       return 1;

    case 2: case 3:
       if(deMarshalRequest_1_2(header, body, buf, m_header) < 0 ){
	 fprintf(stderr, "Error: Invalid RequestHeader\n");
         deleteRequestHeader(header, version);
         return -1;
       }
       return 1;
    default:
       fprintf(stderr, "Error: Invalid RequestHeader\n");
       deleteRequestHeader(header, version);
       return -1;
  }
}


/*      GIOP 1.0 & 1.1   */
int deMarshalRequest_1_0(GIOP_RequestHeader *Header,
		CORBA_Sequence_Octet *body,
		char *buf, GIOP_MessageHeader *m_header){

  int current = SIZEOF_GIOP_HEADER;
  int version, size, order;

  version = m_header->version.minor;
  order   = m_header->flags & 0x01;
  size    = m_header->message_size;

  GIOP_RequestHeader_1_0 *header = &(Header->_1_0);

  if (version > 1) return -1;

  demarshal_IOP_SerivceContextList(&header->service_context, (octet *)buf, &current, order);

  Address_Alignment(&current, 4);
  header->request_id        = demarshalLong((octet *)buf, &current, order);
  header->response_expected = demarshalOctet(buf, &current);

  Address_Alignment(&current, 4);
  demarshal_CORBA_String(&header->object_key, (octet *)buf, &current, order);
  Address_Alignment(&current, 4);
  demarshal_CORBA_String(&header->operation, (octet *)buf, &current, order);
  Address_Alignment(&current, 4);
  demarshal_CORBA_String(&header->requesting_principal, (octet *)buf, &current, order);

  /**** Request body;  */
  if (current < size+SIZEOF_GIOP_HEADER) {
    set_CORBA_Sequence_Octet(body, (octet *)(buf+current),
		     	size - current + SIZEOF_GIOP_HEADER);
  }

  return 1; 
}

/***** GIOP 1.2 & 1.3  ****/
int deMarshalRequest_1_2(GIOP_RequestHeader *Header,
		CORBA_Sequence_Octet *body, 
		char *buf, GIOP_MessageHeader *m_header){

  int current = SIZEOF_GIOP_HEADER;
  int version, size, order;

  version = m_header->version.minor;
  order   = m_header->flags & 0x01;
  size    = m_header->message_size;

  GIOP_RequestHeader_1_2 *header = &(Header->_1_2);

  if (version < 2) return -1;

  header->request_id     = demarshalLong((octet *)buf, &current, order);
  header->response_flags = demarshalOctet(buf, &current);

  Address_Alignment(&current, 4);
  header->target_type    = demarshalShort((octet *)buf, &current, order);

  switch(header->target_type){
    case GIOP_KeyAddr: /* KeyAddr */
      Address_Alignment(&current, 4);
      demarshal_CORBA_String(&header->target.object_key, (octet *)buf, &current, order);
      break;
    case GIOP_ProfileAddr:
      return -1;
      /*  break;  */
    case GIOP_ReferenceAddr:
      return -1;
      /*  break;  */
    default:
      return -1;
  }

  Address_Alignment(&current, 4);
  demarshal_CORBA_String(&header->operation, (octet *)buf, &current, order);

  Address_Alignment(&current, 4);
  demarshal_IOP_SerivceContextList(&header->service_context, (octet *)buf, &current, order);

  /* Request body;  */
  if (current < size+SIZEOF_GIOP_HEADER) {
     Address_Alignment(&current, 8);
     size -= current - SIZEOF_GIOP_HEADER;
     set_CORBA_Sequence_Octet(body, (octet *)(buf+current), size);
  }
  return 1; 
}

/*
 * Reply Message
 *
 */

int deMarshalReply(GIOP_ReplyHeader *header,
		CORBA_Sequence_Octet *body,
		unsigned char *buf, GIOP_MessageHeader *m_header)
{
  int i;
  int version;
  int order;
  int size;
  int current = SIZEOF_GIOP_HEADER;

  version = m_header->version.minor;
  order = m_header->flags & 0x01;
  size = m_header->message_size;

  if (version < 2){
    GIOP_ReplyHeader_1_0 *h = &header->_1_0;

    h->service_context.num = demarshalLong(buf, &current, order);
    for(i=0;i<h->service_context.num;i++){
       IOP_ServiceContext *context = &h->service_context.data[i];

       context->context_id = demarshalLong(buf, &current, order);
       demarshal_CORBA_String(&context->context_data,(octet *)buf, &current, order);
    }

    Address_Alignment(&current, 4);
    h->request_id   = demarshalLong((octet *)buf, &current, order);
    h->reply_status = demarshalLong((octet *)buf, &current, order);

  }else{
    GIOP_ReplyHeader_1_2 *h = &header->_1_2;

    h->request_id   = demarshalLong((octet *)buf, &current, order);
    h->reply_status = demarshalLong((octet *)buf, &current, order);

    Address_Alignment(&current, 4);
    h->service_context.num = demarshalLong(buf, &current, order);
    for(i=0;i<h->service_context.num;i++){
       IOP_ServiceContext *context = &h->service_context.data[i];

       context->context_id = demarshalLong(buf, &current, order);
       demarshal_CORBA_String(&context->context_data,(octet *)buf, &current, order);
    }

    if(size > current - SIZEOF_GIOP_HEADER){
       Address_Alignment(&current, 8);
    }
  }

  if(size > current - SIZEOF_GIOP_HEADER){
    set_CORBA_Sequence_Octet(body, buf+current, 
		    size - current + SIZEOF_GIOP_HEADER);
  }

  return 1;
}

/*
 *
 * Cancel Request
 */
int deMarshalCancelRequest(GIOP_CancelRequestHeader *header,
		unsigned char *buf, GIOP_MessageHeader *m_header)
{
  int current = SIZEOF_GIOP_HEADER;
  int order = m_header->flags & 0x01;

  header->request_id = demarshalLong(buf, &current, order);

  return 1; 
}


/*
 * LocateRequest 
 *
 */
int deMarshalLocateRequest(GIOP_LocateRequestHeader *header,
		unsigned char *buf, GIOP_MessageHeader *m_header)
{
  int version;
  int order;
  int size;
  int current = SIZEOF_GIOP_HEADER;

  version = m_header->version.minor;
  order = m_header->flags & 0x01;
  size = m_header->message_size;

  if(version < 2){ /* GIOP 1.0 or 1.1  */
    header->_1_0.request_id = demarshalLong(buf, &current, order);
    demarshal_CORBA_String(&header->_1_0.object_key, buf, &current, order);
  }else{
    header->_1_2.request_id  = demarshalLong(buf, &current, order);
    header->_1_2.target_type = demarshalShort(buf, &current, order);
    switch(header->_1_2.target_type){
      case 0: /* KeyAddr: */
  	   Address_Alignment(&current, 4);
           demarshal_CORBA_String(&header->_1_2.target.object_key, buf, &current, order);
	   break;
      case 1: /* ProfileAddr: */
	   break;
      case 2: /* ReferenceAddr: */
	   break;
    }
  }

  return 1; 
}

/*
 * LocateReply
 *
 */
int deMarshalLocateReply(GIOP_LocateReplyHeader *header,
		CORBA_Sequence_Octet *body, 
		unsigned char *buf, GIOP_MessageHeader *m_header)
{
  int len, size;
  int order;
  int current = SIZEOF_GIOP_HEADER;

  order = m_header->flags & 0x01;
  size = m_header->message_size;

  header->request_id    = demarshalLong(buf, &current, order);
  header->locate_status = demarshalLong(buf, &current, order);

  switch(header->locate_status){
    case GIOP_OBJECT_FORWARD:
    case GIOP_OBJECT_FORWARD_PERM:
	 len = demarshalLong(buf, &current, order);
	 set_CORBA_Sequence_Octet(body, buf+current, len);

	 break;
    case GIOP_LOC_SYSTEM_EXCEPTION:
	 set_CORBA_Sequence_Octet(body, buf+current, 
			 size - current + SIZEOF_GIOP_HEADER);
	 break;
    case GIOP_LOC_NEEDS_ADDRESSING_MODE:
	 /*   Not Implemented */
	 return -1;
    default:
	 break;
  }

  return 1;
}

/*******************************/
/*   Marshal Request & Reply   */
/*******************************/
/*
 *  Request Message
 *
 */

int MarshalRequest(char *buf, GIOP_RequestHeader *header,
		octet *body, int body_size, int version)
{
  int request_size ;
  switch(version){
    case 0: case 1:
       if((request_size = MarshalRequest_1_0(buf, header, 
				       body, body_size, version)) < 0 ){
	 fprintf(stderr, "Error: Invalid RequestHeader %d\n",version);
         deleteRequestHeader(header, version);
         return -1;
       }
       return request_size;

    case 2: case 3:
       if((request_size = MarshalRequest_1_2(buf, header, 
				       body, body_size, version)) < 0 ){
	 fprintf(stderr, "Error: Invalid RequestHeader %d\n",version);
         deleteRequestHeader(header, version);
         return -1;
       }
       return request_size;
    default:
       fprintf(stderr, "Error: Invalid RequestHeader %d\n",version);
       deleteRequestHeader(header, version);
       return -1;
  }
}

/*   GIOP 1.0 & 1.1  */
int MarshalRequest_1_0(char *buf, GIOP_RequestHeader *Header,
		octet *args, int size,
		int version)
{
  int current = SIZEOF_GIOP_HEADER;

  GIOP_RequestHeader_1_0 *header = &(Header->_1_0);

  if (version > 1) return -1;

  marshal_IOP_SerivceContextList(&header->service_context, (octet *)buf, &current);

  Address_Alignment(&current, 4);
  marshalLong((octet *)buf, &current, header->request_id);
  marshalLong((octet *)buf, &current, header->response_expected);

  Address_Alignment(&current, 4);
  marshal_CORBA_String((octet *)buf, &current, &header->object_key);

  Address_Alignment(&current, 4);
  marshal_CORBA_String((octet *)buf, &current, &header->operation);

  Address_Alignment(&current, 4);
  marshal_CORBA_String((octet *)buf, &current, &header->requesting_principal);

  if ( size > 0) {
    memcpy(buf+current, args, size);
    current += size ;
  }

  return current; 
}

/*   GIOP 1.2 & 1.3  */
int MarshalRequest_1_2(char *buf, GIOP_RequestHeader *Header,
		octet *args, int size,
		int version)
{
  int current = SIZEOF_GIOP_HEADER;

  GIOP_RequestHeader_1_2 *header = &(Header->_1_2);

  if (version < 2) return -1;

  marshalLong((octet *)buf, &current, header->request_id);
  marshalLong((octet *)buf, &current, header->response_flags);

  Address_Alignment(&current, 4);
  marshalLong((octet *)buf, &current, header->target_type);

  switch(header->target_type){
    case GIOP_KeyAddr: /* KeyAddr */
      Address_Alignment(&current, 4);
      marshal_CORBA_String((octet *)buf, &current, &header->target.object_key);
      break;
    case GIOP_ProfileAddr: /* Not Implemented */
      return -1;
    case GIOP_ReferenceAddr: /* Not Implemented */
      return -1;
      /* break; */
    default:
      return -1;
  }

  Address_Alignment(&current, 4);

  if(!header->operation._length) return -1;
  marshal_CORBA_String((octet *)buf, &current, &header->operation);

  Address_Alignment(&current, 4);
  marshal_IOP_SerivceContextList(&header->service_context, (octet *)buf, &current);

  /* Request body; */
  if ( size > 0) {
    Address_Alignment(&current, 8);
    memcpy(buf+current, args, size);
    current += size ;
  }
  return current; 
}

/*
 * Reply Message
 *
 */
int MarshalReply(unsigned char *buf, int *size, GIOP_ReplyHeader *header,
		octet *body, int body_size,
		int version)
{
  int i;
  *size = SIZEOF_GIOP_HEADER;

  GIOP_MessageHeader *MsgHeader = GIOP_Create_MessageHeader(RTORB_BYTE_ORDER, version); /* Little Endian */

  MsgHeader->message_type = GIOP_Reply ;

  if (version < 2){
    GIOP_ReplyHeader_1_0 *h = &header->_1_0;

    marshalLong((octet *)buf, size, h->service_context.num);
    for(i=0;i<h->service_context.num;i++){
       IOP_ServiceContext *context = &h->service_context.data[i];
       marshalLong((octet *)buf, size, context->context_id);
       marshal_CORBA_String((octet *)buf, size, &context->context_data);
    }

    Address_Alignment(size, 4);
    marshalLong((octet *)buf, size, h->request_id);
    marshalLong((octet *)buf, size, h->reply_status);

  }else{
    GIOP_ReplyHeader_1_2 *h = &header->_1_2;

    marshalLong((octet *)buf, size, h->request_id);
    marshalLong((octet *)buf, size, h->reply_status);

    marshalLong((octet *)buf, size, h->service_context.num);
    for(i=0;i<h->service_context.num;i++){
       IOP_ServiceContext *context = &h->service_context.data[i];
       marshalLong((octet *)buf, size, context->context_id);
       marshal_CORBA_String((octet *)buf, size, &context->context_data);
    }
    if(body_size > 0){ Address_Alignment(size, 8); }
  }

  if(body_size > 0){
    memcpy(buf+(*size), body, body_size);
    *size += body_size;
  }
#ifdef DEBUG_OLD
  fprintf(stderr, "    MarshalReply  %d, body_size=%d\n", *size, body_size);
#endif

  MsgHeader->message_size = *size - SIZEOF_GIOP_HEADER ; /* only reply header  */
  memcpy(buf, MsgHeader, SIZEOF_GIOP_HEADER);

  RtORB_free(MsgHeader, "MarshalReply:");
  return *size;
}


/*
 * LocateRequest 
 *
 */
int MarshalLocateRequest(char *buf, GIOP_LocateRequestHeader *header,
		int version)
{
  int current = SIZEOF_GIOP_HEADER;

  if(version < 2){ /* GIOP 1.0 or 1.1 */
    marshalLong((octet *)buf, &current, header->_1_0.request_id);
    marshal_CORBA_String((octet *)buf, &current, &header->_1_0.object_key);
  }else{
    marshalLong((octet *)buf, &current, header->_1_2.request_id);
    marshalLong((octet *)buf, &current, header->_1_2.target_type);
    switch(header->_1_2.target_type){
      case 0: /* KeyAddr: */
  	   Address_Alignment(&current, 4);
           marshal_CORBA_String((octet *)buf, &current, &header->_1_2.target.object_key);
	   break;
      case 1: /* ProfileAddr: */
	   return -1;
	   break;
      case 2: /* ReferenceAddr: */
	   return -1;
    }
  }

  return current; 
}

/*
 * LocateReply
 *
 */
int MarshalLocateReply(unsigned char *buf, int *size,
		GIOP_LocateReplyHeader *header, char *ior, int version)
{
  int len = 0;
  GIOP_SystemExceptionReplyBody e_body;
  GIOP_MessageHeader *MsgHeader = GIOP_Create_MessageHeader(RTORB_BYTE_ORDER, version); /* Little Endian */
  MsgHeader->message_type = GIOP_LocateReply ;
  MsgHeader->message_size = 8 ; /* only reply header */
  memcpy(buf, MsgHeader, SIZEOF_GIOP_HEADER);
  *size += SIZEOF_GIOP_HEADER;
  memcpy(buf + SIZEOF_GIOP_HEADER, header, 8);
  *size += 8;

  switch(header->locate_status){
    case GIOP_OBJECT_FORWARD:
    case GIOP_OBJECT_FORWARD_PERM:
         marshal_Octet_Sequence(buf, size, (octet *)ior, len);
	 break;
    case GIOP_LOC_SYSTEM_EXCEPTION:
         setSystemException(&e_body, "Location System exceptio", 0 , 0);
	 /* Not Implemented  */
	 break;
    case GIOP_LOC_NEEDS_ADDRESSING_MODE:
	 /* Not Implemented  */
	 break;
    default:
	 break;
  }

  RtORB_free(MsgHeader, "MarshalLocateReply");
  return *size;
}


