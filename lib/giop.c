/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 *  RtORB:
 *   IIOP-GIOP
 *
 *  Copyright(C) 2006, Isao Hara, AIST
 *
 *  $Revision: 1.9 $
 *  $Date: 2008/06/04 15:10:23 $
 *  $Id: giop.c,v 1.9 2008/06/04 15:10:23 yoshi Exp $
 *
 */
/*!
 * @file giop.c
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
#include <RtORB/sockport.h>

#include <sys/types.h>
#include <unistd.h>
#if !defined(Cygwin)
#include <ifaddrs.h>
#endif
#include <sys/select.h>

#define RECV_BUF_SIZE  1024 * 2048

extern CORBA_ORB _ORB_;
/***********************************/
GIOP_Connection *
GIOP_Connection__create(){
  GIOP_Connection *conn=(GIOP_Connection *)RtORB_alloc(sizeof(GIOP_Connection),
		 "GIOP_Connection__alloc" );
  memset(conn, 0, sizeof(GIOP_Connection));

  if(_ORB_){
    conn->hostname = (unsigned char *)RtORB_strdup(_ORB_->hostname, "GIOP_Connection__create");
  }else{ 
    conn->hostname = (unsigned char *)Get_IP_Address();
  }
  conn->activate = FALSE;

#if USE_THREAD
  conn->thread = (pthread_t)NULL;
#endif

  return conn;
}


void
GIOP_Connection__open(GIOP_Connection *conn, short port){
  extern int make_server_connection(GIOP_Connection *conn, int port);
  if(!conn) return;

  make_server_connection(conn, port);
  
  return;
}

void
GIOP_Connection__shutdown(GIOP_Connection *conn){
#if USE_THREAD
  void *result; 
#endif
  if(!conn) return;
  if(conn->activate){ conn->activate = FALSE; }
#if USE_THREAD
/*
   wait for shutdown thread
*/
  pthread_join(conn->thread, &result);
#endif
  return;
}

void
GIOP_Connection__close(GIOP_Connection *conn){
  if(!conn) return;
  if(conn->activate) GIOP_Connection__shutdown(conn);
  if(conn->sock) close(conn->sock);

  conn->port = 0;
  conn->sock = 0;
  
  return;
}

/* For multi thread */
void *
GIOP_Connection__run(GIOP_Connection *conn){
  if(!conn) return NULL;
#if 0
  if(conn->type != 0) return;
#endif
  if(conn->sock && conn->activate == FALSE){
     conn->activate = TRUE;
     server_socket_loop(conn->sock, (float)1000.0, NULL, NULL, RecvMessage);
  }
  return NULL;
}

void
GIOP_Connection__free(GIOP_Connection *conn){
  if(!conn) return;
  if(conn->sock) GIOP_Connection__close(conn);

  if(conn->hostname){
    RtORB_free(conn->hostname, "GIOP_Connection__free(hostname)");
  }
  RtORB_free(conn, "GIOP_Connection__free");
}


/***************************************************/
GIOP_MessageHeader *GIOP_Create_MessageHeader(int order, int version){
  GIOP_MessageHeader *header = (GIOP_MessageHeader *)RtORB_alloc(SIZEOF_GIOP_HEADER, "GIOP_Create_MessageHeader");

  memcpy(header->magic, "GIOP", 4);
  header->version.major = 1;
  header->version.minor = version;
  header->flags = order;

  return header;
}

/***************************************************/
/***
 *
 */
int receiveMessage(GIOP_ConnectionHandler *h, GIOP_MessageHeader *header, octet *buf, int maxMsg){
  int recv_res = GIOP_ConnectionHandler_recv(h, (char *)buf, SIZEOF_GIOP_HEADER);
  if (recv_res < 0) {
    //fprintf(stderr, "Bad GIOP message\n");
    return -1;
  }
  memcpy(header, buf, SIZEOF_GIOP_HEADER);
  if((header->flags & 0x01) == 0 ){
     header->message_size=ntohl(header->message_size);
  }

  int msgLen = header->message_size;

  if(msgLen > maxMsg){
    fprintf(stderr, "Buffer overflow(size = %#x)!! %d  maxMsg=%d\n",
         (int)header->message_size, (int)header->message_size, maxMsg);
    return -1;
  }

  if (GIOP_ConnectionHandler_recv(h, (char *)(buf+SIZEOF_GIOP_HEADER), msgLen) < 0) {
    fprintf(stderr, "Bad Reply message\n");
    return -1;
  }

  return header->message_size;
}


/***************************************************/

int reply_Message(GIOP_ConnectionHandler *h, GIOP_RequestHeader *request_header,
		GIOP_ReplyBody *reply_body,
		int version)
{
  unsigned char *msgbuf = NULL;
  int len=0;
  int res=1;
  GIOP_ReplyHeader header;

  int alloc_size = reply_body->body_size+SIZEOF_GIOP_HEADER+1024;
  msgbuf = (unsigned char *)RtORB_alloc( alloc_size, "reply_Message");
  if ( !msgbuf ) {
    fprintf(stderr, "ERROR in reply_Message: Fail to allocate buffer(%d)\n", alloc_size);
    return -1;
  }
  memset(msgbuf, 0, alloc_size);

  if (version < 2){
    header._1_0.service_context.num = 0;
    header._1_0.request_id = request_header->_1_0.request_id;
    header._1_0.reply_status = reply_body->status;
  }else{
    header._1_2.request_id = request_header->_1_2.request_id;
    header._1_2.reply_status = reply_body->status;
    header._1_2.service_context.num = 0; /* Not Implemented for service_context */
  }

  switch(reply_body->status){
    case GIOP_SYSTEM_EXCEPTION:
         {
	 fprintf(stderr, "System Exception!!!\n");
           int id_len = reply_body->exception.exception_id._length +1;
           int exception_size = id_len+16;
           int current=0;
           octet *ebuf;
           ebuf=(octet *)RtORB_alloc(exception_size, "reply_Message:GIOP_SYSTEM_EXCEPTION");

           marshalLong(ebuf, &current, reply_body->status);
           marshal_Octet_Sequence(ebuf, &current, 
                          reply_body->exception.exception_id._buffer, id_len);
           marshalLong(ebuf, &current, reply_body->exception.minor_code_value);
           marshalLong(ebuf, &current, reply_body->exception.completion_status);

           MarshalReply(msgbuf, &len, &header, ebuf, exception_size, version);
           RtORB_free(ebuf, "reply_Message:GIOP_SYSTEM_EXCEPTION");
	 }
	 break;
    case GIOP_USER_EXCEPTION:
	 fprintf(stderr, "User Exception!!!\n");
    case GIOP_NO_EXCEPTION:
         MarshalReply(msgbuf, &len, &header, reply_body->body,
			 reply_body->body_size, version);
	 break;
    default:
	 break;
  }

#ifdef DEBUG_GIOP
   fprintf(stderr, "===== GIOP Reply (%d) = status=%d ====\n", len, reply_body->status);
   dumpMessage(msgbuf, len);
#endif

 /* Send Message */

  if (GIOP_ConnectionHandler_send(h, (char *)msgbuf, len) < 0){
    fprintf(stderr, "Fail to send reply message\n");
    res = -1;
  }
  RtORB_free(msgbuf, "reply_Message: finish"); 
  return res;
}


/*
 * invoke
 *
 */
/*!
 * @if jp
 * @brief stringとstring_arrayの比較処理。
 * @else
 * @brief Comparing 'string' data and 'string_array' data.
 * @endif
 * @param str string
 * @param str_array string array
 * @param n array index
 * @return 0:matched, 1:unmatched
 */
int check_strings(char *str, char **str_array, int n){
  int i;
  for (i=0;i<n;i++){
    if(!strcmp(str, str_array[i])) return 0;
  }
  return 1;
}


GIOP_ReplyBody *invokeServant(PortableServer_POA poa,
		GIOP_RequestHeader *header,
  		CORBA_Environment *env,
		octet *arg_buf, int version, int order)
{
  char *function;
  char *obj_key;
  RtORB_POA_Object *poa_obj;
  PortableServer_ClassInfo *info;
  CORBA_Class_Method *m_data;
  void *(*impl_method)();
  void (*call_impl_func)(PortableServer_ServantBase *, void*, CORBA_Class_Method*, void**, CORBA_Environment*, void*(*method)());
  void * *argv;
  void * result;
  typedef void (*impl_func_type)(PortableServer_ServantBase *, void*, CORBA_Class_Method*, void**, CORBA_Environment*, void*(*method)());

  void RtORB_Result_free(CORBA_TypeCode tc, void **result);

  hashtable *tbl; 

  GIOP_ReplyBody *reply = (GIOP_ReplyBody *)RtORB_alloc(sizeof(GIOP_ReplyBody),
		  "ReplyBody in invokeServant");

  reply->status = GIOP_NO_EXCEPTION ;
  reply->body = NULL;
  reply->body_size = 0;

  if(poa) tbl = poa->object_map;
  else tbl = _ORB_->_object_table;
  
  if (version > 1){
    obj_key = (char *)header->_1_2.target.object_key._buffer;
    function = (char *)header->_1_2.operation._buffer;
  }else{
    obj_key = (char *)header->_1_0.object_key._buffer;
    function = (char *)header->_1_0.operation._buffer;
  }

  if(!obj_key) obj_key = (char *)"NameService";
  poa_obj = (RtORB_POA_Object *)getValue(tbl, obj_key);


#ifdef DEBUG_OLD
    if (version > 1){
      fprintf(stderr, "POA Object %s, function: %s\n",
		      header->_1_2.target.object_key._buffer, function);
    }else{
      fprintf(stderr, "POA Object %s, function: %s\n",
		      header->_1_0.object_key._buffer, function);
    }
#endif

  if(poa_obj == NULL){
    reply->status = GIOP_SYSTEM_EXCEPTION;
    reply->exception.exception_id._buffer= (unsigned char *)"Unknown Object";
    reply->exception.exception_id._length= 14;
    reply->exception.minor_code_value = 0;
    reply->exception.completion_status = 0;
    return reply;

  }else{
    extern void** Result_alloc(CORBA_TypeCode tc);
    info = (PortableServer_ClassInfo *)poa_obj->_private;
    PortableServer_ServantBase *sb = (PortableServer_ServantBase*)poa_obj->servant;

    call_impl_func = (impl_func_type)(*info->impl_finder)(&sb->_private, function, &m_data, &impl_method );

#if DEBUG_MALLOC
    fprintf(stderr, "    invokeServant obj_key=%s, func=%s\n", obj_key, function);
#endif

    if (!call_impl_func){
      if(!strcmp(function, "_is_a") && sb->vepv && sb->vepv[0]->is_a ){
	  int pos, len;
	  pos = 0;
	  char *id = demarshalString((octet *)arg_buf, &pos, order, &len);
          CORBA_boolean retval = (sb->vepv[0]->is_a)(&sb->_private, id , env );
          RtORB__free(id, "_is_a");
	  reply->body_size = 1;
          reply->body = (octet *) RtORB_alloc(reply->body_size,
			   "Reply->body(_is_a) in invokeServant");
	   memcpy(reply->body, &retval , 1);
	   return reply;
      }else if(!strcmp(function, "_non_existent") && sb->vepv && sb->vepv[0]->non_existent ){
	  int pos;
	  pos = 0;
          CORBA_boolean retval = (sb->vepv[0]->non_existent)(&sb->_private, env );
	  reply->body_size = 1;
          reply->body = (octet *) RtORB_alloc(reply->body_size,
			   "Reply->body(_non_exietent) in invokeServant");
	   memcpy(reply->body, &retval , 1);
	   return reply;
      }else{
        fprintf(stderr, "No such a Function: %s\n", function);
        reply->status = GIOP_SYSTEM_EXCEPTION;
        reply->exception.exception_id._buffer= (unsigned char *)"Unknown Function";
        reply->exception.exception_id._length= 14;
        reply->exception.minor_code_value = 0;
        reply->exception.completion_status = 0;
        return reply;
      }
    }else{
    }

    argv = (void * *)deMarshal_Arg(arg_buf, m_data->in_argc,
		    m_data->in_argv, order);

    (*call_impl_func)(sb, &result, m_data, argv, env, impl_method);

    switch(env->_major){
      case CORBA_NO_EXCEPTION:
        Marshal_Reply_Arguments(reply, (void**)result, argv, m_data);
        break;
      case CORBA_USER_EXCEPTION:
	{
           char *reply_buf = NULL;
	   int size = 0;
           reply_buf = (char *)RtORB_alloc( MaxMessageSize, "invokeServant(USER_EXCEPTION)");
           memset(reply_buf, 0, MaxMessageSize );

	   fprintf(stderr, "User exception(%s): %s\n",
			   function, env->_repo_id);
	   marshalString((octet *)reply_buf, &size, (char *)env->_repo_id);
           marshal_by_typecode((octet *)reply_buf, &env->_params, 
			    TC_CORBA_any, &size);
	   reply->body_size = size;
	   reply->body = (octet *) RtORB_alloc(reply->body_size,
			   "Reply->body(USER_EXCEPTION) in invokeServant");
	   memcpy(reply->body, reply_buf, size);

           reply->status = GIOP_USER_EXCEPTION;

           if ( reply_buf ){
              RtORB_free( reply_buf, "invokeServant:USER_EXCEPTION");
           }
	}
	break;
      case CORBA_SYSTEM_EXCEPTION:
        fprintf(stderr, "System exception occured in %s: %s\n",
			function, env->_repo_id);
        reply->status = GIOP_SYSTEM_EXCEPTION;
        reply->exception.exception_id._buffer=env->_repo_id;
        reply->exception.exception_id._length=strlen((const char *)env->_repo_id);
        reply->exception.minor_code_value = 0;
        reply->exception.completion_status = 0;
	break;
      default:
	break;
    }

    RtORB_Arguments_free(argv, m_data->in_argc, m_data->in_argv);

    if(env->_cpp_flag){
      RtORB_Result__free_cpp(m_data->retval, result);
    }else{
      RtORB_Result__free(m_data->retval, result);
    }
    RtORB_free(argv, "deMarshal_Arg in invokeServant");
  }

  return reply;
}

int find_object(PortableServer_POA poa, char *key, char *ior){
  hashtable *tbl;

  if(poa) tbl = poa->object_map;
  else tbl = _ORB_->_object_table;

  if ( getValue(tbl, key) == NULL) return  GIOP_UNKNOWN_OBJECT;
  return  GIOP_OBJECT_HERE;
}

int reply_locateMessage(PortableServer_POA poa, GIOP_ConnectionHandler *h,
		GIOP_LocateRequestHeader *locate_request_header, int version)
{
  unsigned char *buf = NULL;
  int len=0;
  char *object_key;
  char *ior = NULL;   /* Not use */
  
  buf = (unsigned char *)RtORB_alloc( MaxMessageSize, "reply_locateMessage");
  memset(buf, 0, MaxMessageSize);
  
  GIOP_LocateReplyHeader header;
  if (version < 2){
    header.request_id = locate_request_header->_1_0.request_id;
    object_key = (char *)locate_request_header->_1_0.object_key._buffer;
  }else{
    header.request_id = locate_request_header->_1_2.request_id;
    object_key = (char *)locate_request_header->_1_2.target.object_key._buffer;
  }

  header.locate_status = find_object(poa, object_key, ior);
  MarshalLocateReply(buf, &len, &header, ior, version);

  if (GIOP_ConnectionHandler_send(h, (char *)buf, len) < 0){
    if ( buf ) { RtORB_free( buf, "reply_locateMessage:GIOP_ConnectionHandler_send"); }

    return -1;
  }
  if ( buf ) { RtORB_free( buf, "reply_locateMessage: finish"); }
  return 1;
}

/********************************************************/
/*   Create Message
*/
uint32_t createRequest(octet *buf, int response, 
		char *object_key, int object_key_len,
		char *operation, int operation_len,
		octet *args, int args_len, 
		int version)
{
  int len;

  GIOP_RequestHeader *header = (GIOP_RequestHeader *)newRequestHeader();

  memset(buf, 0, MaxMessageSize);

  GIOP_MessageHeader *MsgHeader = GIOP_Create_MessageHeader(RTORB_BYTE_ORDER, version);
  MsgHeader->message_type = GIOP_Request ;

  if(version < 2){
   header->_1_0.request_id = next_request_id();
   header->_1_0.response_expected = response;
   set_CORBA_Sequence_Octet(&header->_1_0.object_key, (octet *)object_key, object_key_len);
   set_CORBA_Sequence_Octet(&header->_1_0.operation, (octet *)operation, operation_len);
  }else{
   header->_1_2.request_id = next_request_id();
   header->_1_2.response_flags = response;
   header->_1_2.target_type = GIOP_KeyAddr;
   set_CORBA_Sequence_Octet(&header->_1_2.target.object_key, (octet *)object_key, object_key_len);
   set_CORBA_Sequence_Octet(&header->_1_2.operation, (octet *)operation, operation_len);
  }
  len = MarshalRequest((char *)buf, header, args, args_len, version);

  MsgHeader->message_size = len - SIZEOF_GIOP_HEADER;
  memcpy(buf, MsgHeader, SIZEOF_GIOP_HEADER);

  deleteRequestHeader(header, version);
  RtORB_free(MsgHeader, "createRequest");

  return len;
}

/*
 * Cancel Request
 */
uint32_t createCancelRequest(octet *buf, uint32_t request_id)
{
  uint32_t version = 2;
  int current = SIZEOF_GIOP_HEADER;

  memset(buf, 0, MaxMessageSize);

  GIOP_MessageHeader *MsgHeader = GIOP_Create_MessageHeader(RTORB_BYTE_ORDER, version);
  MsgHeader->message_type = GIOP_CancelRequest ;
  MsgHeader->message_size = 4;
  memcpy(buf, MsgHeader, SIZEOF_GIOP_HEADER);

  marshalLong(buf, &current, request_id);
  RtORB_free(MsgHeader,"createCancelRequest");

  return SIZEOF_GIOP_HEADER + 4; 
}


int createLocateRequest(octet *buf, char *object_key, int len){
  int size;
  int version = 2;
  GIOP_LocateRequestHeader Header;

  memset(buf, 0, MaxMessageSize);

  GIOP_MessageHeader *MsgHeader = GIOP_Create_MessageHeader(RTORB_BYTE_ORDER, version);
  MsgHeader->message_type = GIOP_LocateRequest ;

  if (version < 2){
    Header._1_0.request_id = next_request_id();
    Header._1_0.object_key._length = len;
    Header._1_0.object_key._buffer = (unsigned char *)object_key;
  }else{
    Header._1_2.request_id = next_request_id();
    Header._1_2.target_type = GIOP_KeyAddr;
    Header._1_2.target.object_key._length = len;
    Header._1_2.target.object_key._buffer = (unsigned char *)object_key;
  }

  size = MarshalLocateRequest((char *)buf, &Header, version);
  if (size < 0) return -1;

  MsgHeader->message_size = size - SIZEOF_GIOP_HEADER ;
  memcpy(buf, MsgHeader, SIZEOF_GIOP_HEADER);

  RtORB_free(MsgHeader, "createLocateRequest");
  return size ;
}

/*
 * CloseConnection Message
 */
uint32_t createCloseConnectionMessage(octet *buf)
{
  uint32_t version = 2;

  memset(buf, 0, MaxMessageSize);

  GIOP_MessageHeader *MsgHeader = GIOP_Create_MessageHeader(RTORB_BYTE_ORDER, version);
  MsgHeader->message_type = GIOP_CloseConnection ;
  MsgHeader->message_size = 0;
  memcpy(buf, MsgHeader, SIZEOF_GIOP_HEADER);

  RtORB_free(MsgHeader,"createCloseConnectionRequest");
  return SIZEOF_GIOP_HEADER; 
}

/*
 * MessageError Message
 */
uint32_t createMessageErrorMessage(octet *buf){
  uint32_t version = 0;

  memset(buf, 0, MaxMessageSize);

  GIOP_MessageHeader *MsgHeader = GIOP_Create_MessageHeader(RTORB_BYTE_ORDER, version);
  MsgHeader->message_type = GIOP_MessageError ;
  MsgHeader->message_size = 0;
  memcpy(buf, MsgHeader, SIZEOF_GIOP_HEADER);

  RtORB_free(MsgHeader,"createMessageErrorMessage");
  return SIZEOF_GIOP_HEADER; 
}

/*
 *
 *
 */
int requestLocation(GIOP_ConnectionHandler *h, CORBA_URL *ior){
  char *buf = NULL;
  int len;
  int ret;

  buf = (char *)RtORB_alloc( MaxMessageSize, "requestLocation");
  if ( !buf ) {
    fprintf(stderr, "ERROR in requestLocation: Fail to allocate buffer..\n");
    return -1;
  }
  memset(buf, 0, MaxMessageSize);

  len = createLocateRequest((octet *)buf, ior->object_key, ior->object_key_len);
  ret = GIOP_ConnectionHandler_send(h, buf, len);

  RtORB_free( buf, "requestLocation" );
  return ret;
}
/*
 *
 * confirm Location
 */
int confirmLocation(GIOP_ConnectionHandler *h, CORBA_URL *ior){
  char *buf;
  int len;
  GIOP_MessageHeader header;
  GIOP_LocateReplyHeader *locate_reply_header;
  int result;
  CORBA_Sequence_Octet *body;

  buf = (char *)RtORB_alloc( MaxMessageSize,"confirmLocation");
  if ( !buf ) {
    fprintf(stderr, "ERROR in confirmLocation: Fail to allocate buffer..\n");
    return -1;
  }
  memset(buf, 0, MaxMessageSize);

  len = createLocateRequest((octet *)buf, ior->object_key, ior->object_key_len);
  GIOP_ConnectionHandler_send(h, buf, len);

  memset(buf, 0, MaxMessageSize);

  if(receiveMessage(h, &header, (octet *)buf, MaxMessageSize) < 0){
    RtORB_free( buf , "confirmLocation:receiveMessage");
    return (-1);
  }

  body = (CORBA_Sequence_Octet *)new_CORBA_Sequence_Octet(0);
  locate_reply_header = (GIOP_LocateReplyHeader *)newLocateReplyHeader();
  deMarshalLocateReply(locate_reply_header, body, (unsigned char *)buf, &header);
  result =locate_reply_header->locate_status;
  delete_CORBA_Sequence_Octet(body, 1);
  RtORB_free(locate_reply_header,"confirmLocation(locate_reply_header)");
  RtORB_free( buf, "confirmLocation:finish" );

  return result;
}

/*
 *
 *  GIOP Request Procedure
 */
/*!
 * @if jp
 * @brief GIOPリクエスト用プロシージャ。GIOP_Requestメッセージを受信し、その中身を解析('deMarshalRequest()')する。解析したGIOP_Requestメッセージの中身を元に、対応するGIOP_Replyメッセージを作成('invokeServant()')する。GIOP_Replyメッセージの送信(socketへの書き込み('reply_Message()'))を行う。使用したメモリのリリースを行う。
 * @else
 * @brief GIOP Request Procedure. Analyze received GIOP_Request messages by 'deMarshalRequest()'. Make GIOP_Reply header parts using GIOP_Request messages and GIOP_Reply body part by 'invokeServant()'. And send(write to socket) data as GIOP_Reply messages by 'reply_Message()'. Release used temporary memory.
 * @endif
 * @param h GIOP_ConnectionHandler data
 * @param buf buffer of received GIOP messages data
 * @param poa POA
 * @param header GIOP message header for setting data
 * @param body GIOP message body for setting data
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void GIOP_Request_perform(GIOP_ConnectionHandler *h, octet *buf,
  PortableServer_POA poa, GIOP_MessageHeader *header,
  CORBA_Sequence_Octet *body, CORBA_Environment *env)
{
  GIOP_RequestHeader *request_header;
  GIOP_ReplyBody *reply_body;
  int byte_order = 0;

  if((header->flags & 0x01) != 0 ) byte_order = 1;

#ifdef DEBUG_GIOP
  fprintf(stderr, "==== GIOP Message ====\n");
  dumpMessage(buf, header->message_size + SIZEOF_GIOP_HEADER);
#endif

  request_header = (GIOP_RequestHeader *)newRequestHeader();
  deMarshalRequest(request_header, body, (char *)buf, header);

  reply_body = invokeServant(poa, request_header, env, body->_buffer,
		  header->version.minor, byte_order);

  reply_Message(h, request_header, reply_body, header->version.minor);

  deleteReplyBody(reply_body);
  deleteRequestHeader(request_header, header->version.minor);
  return;
}

/*
 * GIOP Reply 
 *
 */
/*!
 * @if jp
 * @brief GIOPリプライ用プロシージャ。各GIOPバージョンで場合分けし、'buf'からGIOP_Replyタイプメッセージのアンマーシャリングを行い、'header'と'body'にセット('deMarshalReply()')する。'reply_status'に応じてメッセージ出力を行う。メモリリリースを行う。
 * @else
 * @brief GIOP Reply Procedure. Get GIOP_Reply data from 'buf', demarshal considering GIOP version, set each data to 'header' and 'body' by 'deMarshalReply()'. Output message releated with 'reply_status'. Release memory.
 * @endif
 * @param h GIOP_ConnectionHandler data
 * @param buf buffer of received GIOP messages data
 * @param poa POA
 * @param header GIOP message header for setting data
 * @param body GIOP message body for setting data
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void GIOP_Reply_perform(GIOP_ConnectionHandler *h, octet *buf, PortableServer_POA poa,
  	GIOP_MessageHeader *header, CORBA_Sequence_Octet *body,
	CORBA_Environment *env)
{
  GIOP_ReplyHeader *reply_header;
  int reply_status;
   
   reply_header = (GIOP_ReplyHeader *)newReplyHeader();
   deMarshalReply(reply_header, body, buf, header);

   if(header->version.minor < 2){
     reply_status = reply_header->_1_0.reply_status;
   }else{
     reply_status = reply_header->_1_2.reply_status;
   }

   switch(reply_status){
     case GIOP_NO_EXCEPTION:
       break;
     case GIOP_USER_EXCEPTION:
       fprintf(stderr, "User Exception occured!!!\n");
       break;
     case GIOP_SYSTEM_EXCEPTION:
       fprintf(stderr, "System Exception occured in GIOP_Reply_perform\n");
       break;
     case GIOP_LOCATION_FORWARD:
     case GIOP_LOCATION_FORWARD_PERM:
       dumpMessage(buf + SIZEOF_GIOP_HEADER, header->message_size);
       fprintf(stderr, "Location forward\n");
       break;
     case GIOP_NEEDS_ADDRESSING_MODE:
     default:
       dumpMessage(buf + SIZEOF_GIOP_HEADER, header->message_size);
     break;
   }
   deleteReplyHeader(reply_header, header->version.minor);
   
   return;
}

/*
 * GIOP Cancel Request 
 *
 */
/*!
 * @if jp
 * @brief GIOP Cancel_Request用プロシージャ。'buf'のアンマーシャリングを行い、GIOP_CancelRequestHeader型データの'request_id'を取得しセット('deMarshalCancelRequest()')する。
 * @else
 * @brief GIOP Cancel Request Procedure. Get GIOP_CancelRequest data from 'buf', demarshal, set data to 'request_id' by 'deMarshalCancelRequest()'.  
 * @endif
 * @param h GIOP_ConnectionHandler data
 * @param buf buffer of received GIOP messages data
 * @param poa POA
 * @param header GIOP message header for setting data
 * @param body GIOP message body for setting data
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void GIOP_CancelRequest_perform(GIOP_ConnectionHandler *h, octet *buf, PortableServer_POA poa,
  	GIOP_MessageHeader *header, CORBA_Sequence_Octet *body,
	CORBA_Environment *env)
{
  GIOP_CancelRequestHeader *cancel_request_header;

  cancel_request_header = (GIOP_CancelRequestHeader *)newCancelRequestHeader();
  deMarshalCancelRequest(cancel_request_header, buf, header);
  fprintf(stderr, "Call Cancel %d\n", (int)cancel_request_header->request_id);
  RtORB_free(cancel_request_header, "GIOP_CancelRequest_perform");
  return;
}

/*
 * GIOP Location Request 
 *
 */
/*!
 * @if jp
 * @brief GIOP Locationリクエスト用プロシージャ。 GIOP_LocateRequestメッセージを受信し、その中身を解析('deMarshalLocateRequest()')する。解析したGIOP_LocateRequestメッセージの中身を元に、対応するGIOP_LocateReplyメッセージを作成し、送信(socketへの書き込み('reply_locateMessage()'))を行う。使用したメモリのリリースを行う。
 * @else
 * @brief GIOP Location Request Procedure. Analyze received GIOP_LocateRequest messages by 'deMarshalLocateRequest()'. Make GIOP_Reply header parts using GIOP_LocateRequest messages, and send(write to socket) data as GIOP_LocateReply messages by 'reply_locateMessage()'. Release used temporary memory.
 * @endif
 * @param h GIOP_ConnectionHandler data
 * @param buf buffer of received GIOP messages data
 * @param poa POA
 * @param header GIOP message header for setting data
 * @param body GIOP message body for setting data
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void GIOP_LocationRequest_perform(GIOP_ConnectionHandler *h, octet *buf, PortableServer_POA poa,
  	GIOP_MessageHeader *header, CORBA_Sequence_Octet *body,
	CORBA_Environment *env)
{
  GIOP_LocateRequestHeader *locate_request_header;

  locate_request_header = (GIOP_LocateRequestHeader *)newLocateRequestHeader();
  deMarshalLocateRequest(locate_request_header, buf, header);

  reply_locateMessage(poa, h, locate_request_header, header->version.minor);

  deleteLocateRequestHeader(locate_request_header, header->version.minor);
  return;
}

/*
 * GIOP Location Reply 
 *
 */
/*!
 * @if jp
 * @brief GIOP Locationリプライ用プロシージャ。各GIOPバージョンで場合分けし、'buf'からGIOP_LocationReplyタイプメッセージのアンマーシャリングを行い、'header'と'body'にセット('deMarshalLocateReply()')する。'reply_status'に応じてメッセージ出力を行う。メモリリリースを行う。
 * @else
 * @brief GIOP Location Reply Procedure. Get GIOP_LocationReply data from 'buf', demarshal considering GIOP version, set each data to 'header' and 'body' by 'deMarshalLocateReply()'. Output message releated with 'reply_status'. Release memory.
 * @endif
 * @param h GIOP_ConnectionHandler data
 * @param buf buffer of received GIOP messages data
 * @param poa POA
 * @param header GIOP message header for setting data
 * @param body GIOP message body for setting data
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void GIOP_LocationReply_perform(GIOP_ConnectionHandler *h, octet *buf, PortableServer_POA poa,
  	GIOP_MessageHeader *header, CORBA_Sequence_Octet *body,
	CORBA_Environment *env)
{
  GIOP_LocateReplyHeader *locate_reply_header;

  locate_reply_header = (GIOP_LocateReplyHeader *)newLocateReplyHeader();
  deMarshalLocateReply(locate_reply_header, body, buf, header);

  switch(locate_reply_header->locate_status){
     case GIOP_UNKNOWN_OBJECT:
	fprintf(stderr, "Unknown Object\n");
	break;
     case GIOP_OBJECT_HERE:
	 break;
     case GIOP_OBJECT_FORWARD:
     case GIOP_OBJECT_FORWARD_PERM:
	 fprintf(stderr, "Object forward\n");
         dumpMessage(buf + SIZEOF_GIOP_HEADER, header->message_size);
	 break;
     case GIOP_LOC_SYSTEM_EXCEPTION:
	 fprintf(stderr, "Locate System Exception\n");
         dumpMessage(buf + SIZEOF_GIOP_HEADER, header->message_size);
	 break;
     case GIOP_LOC_NEEDS_ADDRESSING_MODE:
	 fprintf(stderr, "Not Implemented\n");
         dumpMessage(buf + SIZEOF_GIOP_HEADER, header->message_size);
	 break;
     default:
	 fprintf(stderr, " *[ %d ]*\n", (int)locate_reply_header->locate_status);
	 break;
  }
  RtORB_free(locate_reply_header,"GIOP_LocateReply_perform");
  return;
}

/*
 * free_request
 */
void free_request(void *arg){
  GIOP_Request_Item req = (GIOP_Request_Item)arg;
  if(!req) return;

  RtORB_free(req->buf, "free_request(buf)");
  RtORB_free(req->connh, "free_request(connh)");
  RtORB_free(req, "free_request");
  return;
}

/*
 * GIOP enqueu request
 */
PtrList *GIOP_enqueue_request(GIOP_ConnectionHandler *h, PtrList *lst){
  char *buf = NULL;
  
  GIOP_MessageHeader header;
  GIOP_Request_Item request;

#ifdef DEBUG_OLD
  fprintf(stderr, "\n=======================\n");
#endif

  buf = ( char* )RtORB_alloc( RECV_BUF_SIZE, "GIOP_enqueue_request");
  memset(buf, 0, RECV_BUF_SIZE);


  if(receiveMessage(h, &header, (octet *)buf, RECV_BUF_SIZE) < 0){
//    fprintf(stderr, "Error in GIOP_enqueue_request: Fail to receive data...\n");  
    RtORB_free( buf, "GIOP_enqueue_request" );
    return NULL;
  }

  request = (GIOP_Request_Item)RtORB_alloc(sizeof(GIOP_Request_struct),
		  "GIOP_enqueue_request 2");

  request->connh = (GIOP_ConnectionHandler *)RtORB_alloc(sizeof(GIOP_ConnectionHandler), "GIOP_enqueue_request");
  request->connh->type = h->type;
  request->connh->sock = h->sock;

  request->buf = (char *)RtORB_alloc(header.message_size + SIZEOF_GIOP_HEADER, "GIOP_enqueue_request (buf)");

  memcpy(request->buf, buf, header.message_size + SIZEOF_GIOP_HEADER);

  lst = (PtrList *)PtrList_append(lst, request, (void (*)(void **))free_request);
  RtORB_free( buf, "GIOP_enqueue_request:finish" );

  return lst;
}

/*
 *   GIOP Communication
 *
 */
int RecvMessage(GIOP_ConnectionHandler *h){
  int byte_order = 0 ;
  int fragment = -1 ;
  char *recvbuf = NULL;
  int res = 1;
  GIOP_MessageHeader header;
  int recvBufSize = RECV_BUF_SIZE;

  CORBA_Sequence_Octet *body;
  CORBA_Environment env;

  if((recvbuf = (char *)RtORB_alloc( recvBufSize, "RecvMessage(buf)")) == NULL){
    fprintf(stderr, "Error in RecvMesssage: Fail to allocate buffer...\n");  
    return -1;
  }

  memset(&header, 0, SIZEOF_GIOP_HEADER);
  memset(recvbuf, 0, recvBufSize);

  body = (CORBA_Sequence_Octet *)new_CORBA_Sequence_Octet(0);

  if(receiveMessage(h, &header, (unsigned char *)recvbuf, recvBufSize) < 0){
    fprintf(stderr, "Error in RecvMesssage: Fail to receive data...\n");  
    RtORB_free( recvbuf, "RecvMessage(buf)");
    return -1;
  }

  if((header.flags & 0x01) == 0 );    /*header.message_size=ntohl(header.message_size); -> in receiveMessage */
  else byte_order = 1;

  if(header.version.minor > 0) fragment = header.flags & 0x02;

#ifdef DEBUG_GIOP
  fprintf(stderr, "[%d]\n", header.message_type);
#endif

  switch(header.message_type){
    case GIOP_Request:   /* Sample Implementation only  */
         GIOP_Request_perform(h, (octet *)recvbuf, NULL, &header, body, &env);
         break;
    case GIOP_Reply:     /* This is for a client, not implemented  */
         GIOP_Reply_perform(h, (octet *)recvbuf, NULL, &header, body, &env);
         break;
    case GIOP_CancelRequest: 
         GIOP_CancelRequest_perform(h, (octet *)recvbuf, NULL, &header, body, &env);
         break;
    case GIOP_LocateRequest:
         GIOP_LocationRequest_perform(h, (octet *)recvbuf, NULL, &header, body, &env);
         break;
    case GIOP_LocateReply:  /* This is for a client, not complete  */
         GIOP_LocationReply_perform(h, (octet *)recvbuf, NULL, &header, body, &env);
         break;
    case GIOP_CloseConnection:
         GIOP_ConnectionHandler_close(h);
         res = -1;
	 break;
    case GIOP_MessageError: 
	 fprintf(stderr, "Message Error occured\n");
         break;
    case GIOP_Fragment: /* Not implemented  */
	 fprintf(stderr, "FragmentMessage have not implemented.....\n");
         break;
    default:
	 fprintf(stderr, "Invalid message type (%d)\n", header.message_type);
         res=-1;
  }

  RtORB_free( recvbuf, "RecvMessage(buf)");

  return res;
}

#if USE_THREAD

struct GIOP_Request_Arg{
  GIOP_ConnectionHandler h;
  char *buf;
  PortableServer_POA poa;
};

void GIOP_Request_perform_thread(void *arg)
{
  struct GIOP_Request_Arg *args = (struct GIOP_Request_Arg *)arg;
  GIOP_MessageHeader *header;
  CORBA_Sequence_Octet *body;

  CORBA_Environment env;

  body = (CORBA_Sequence_Octet *)new_CORBA_Sequence_Octet2(0);

  header = (GIOP_MessageHeader *)args->buf;

  GIOP_Request_perform(&args->h, (octet *)args->buf, args->poa, header, body, &env);
  free(args->buf);
  delete_CORBA_Sequence_Octet2(body, 1);
  free(arg);
}
#endif

PtrList *GIOP_execute_request(PortableServer_POA poa, PtrList *lst){
  GIOP_ConnectionHandler *h;
  int version ;
  int byte_order = 0 ;
  int fragment = -1 ;
  char *buf;
  GIOP_Request_Item request;
  PtrList *current_request, *retval;
  GIOP_MessageHeader *header;
  CORBA_Sequence_Octet *body;
  CORBA_Environment env;

#if 0
  CORBA_MUTEX_LOCK();
#endif

  body = (CORBA_Sequence_Octet *)new_CORBA_Sequence_Octet2(0);

  current_request = lst;

#if 0
  CORBA_MUTEX_UNLOCK();
#endif

  while(current_request){

#if 0
    CORBA_MUTEX_LOCK();
#endif

    request = (GIOP_Request_Item)current_request->item;
    header = (GIOP_MessageHeader *)request->buf;
    buf = request->buf;
    h = request->connh;

    poa = (PortableServer_POA) GIOP_ConnectionHandler_get_arg(h);

    version = header->version.minor;

    if((header->flags & 0x01) == 0 ) header->message_size=ntohl(header->message_size);
    else byte_order = 1;

    if(header->version.minor > 0) fragment = header->flags & 0x02;

    memset(&env, 0, sizeof(CORBA_Environment));

    switch(header->message_type){
      case GIOP_Request: /* Sample Implementation only */
#if USE_THREAD
        {
         pthread_t thr;
         struct GIOP_Request_Arg *arg = (struct GIOP_Request_Arg *)RtORB_alloc(sizeof(struct GIOP_Request_Arg), "GIOP_execute_request (buf)");
         int buf_size = header->message_size + SIZEOF_GIOP_HEADER;

         arg->buf = (char *)RtORB_alloc(buf_size, "GIOP_execute_request (buf)");
         memcpy(&arg->h, request->connh, sizeof(GIOP_ConnectionHandler));
         memcpy(arg->buf, request->buf,  buf_size);
         arg->poa = poa;

         RunThread(&thr, GIOP_Request_perform_thread, (void *)arg, 1);
        }
#else
         GIOP_Request_perform(h, (octet *)buf, poa, header, body, &env);
#endif
         break;
      case GIOP_Reply:  /* This is for a client, not implemented */
         GIOP_Reply_perform(h, (octet *)buf, poa, header, body, &env);
         break;
      case GIOP_CancelRequest:
         GIOP_CancelRequest_perform(h, (octet *)buf, poa, header, body, &env);
         break;
      case GIOP_LocateRequest:
	 GIOP_LocationRequest_perform(h, (octet *)buf, poa, header, body, &env);
         break;
      case GIOP_LocateReply:  /* This is for a client, not complete */
	 GIOP_LocationRequest_perform(h, (octet *)buf, poa, header, body, &env);
         break;

      case GIOP_CloseConnection:
	 break;
      case GIOP_MessageError: 
	 fprintf(stderr, "Message Error occured\n");
         break;
      case GIOP_Fragment: /* Not implemented */
	 fprintf(stderr, "FragmentMessage have not implemented.....\n");
         break;
      default:
	 fprintf(stderr, "Invalid message type (%d)\n", header->message_type);
         break;
    }

     current_request->released = 1;  
     current_request = current_request->next;
  }

  delete_CORBA_Sequence_Octet2(body, 1);
  retval =  (PtrList *)PtrList_remove_released_items(lst);

  return retval;
}


int32_t GIOP_ConnectionHandler_send(GIOP_ConnectionHandler *h, char *buf, int32_t len)
{
  int res=0;

  switch(h->type) {
    case CONNECTION_TYPE_SOCKET:
#if 0
      n = h->sock +1;

      FD_ZERO(&w_sock);
      FD_SET(h->sock, &w_sock);
      int stat = select(n, &w_sock, 0, 0, &tv);
      if(stat>0){
        fprintf(stderr, "Ready to read data (%d)\n", h->sock);
        read(h->sock, rbuf, 1024);
      }

      fprintf(stderr, "No data present in socket (%d)\n", h->sock);
      FD_ZERO(&w_sock);
      FD_SET(h->sock, &w_sock);
      select(n, 0, &w_sock, 0, 0);

      res=write(h->sock, buf, len);
      fprintf(stderr, "Send Data %d (%d)\n", h->sock, res);
#else
      res = writeBytes(h->sock, buf, 12);
      res += writeBytes(h->sock, buf+12, len-12);
#endif
      return res;

    default:
      do_assert(0, "GIOP_ConnectionHandler_send : bad type");
      return -1;
  }
}

int32_t GIOP_ConnectionHandler_recv(GIOP_ConnectionHandler *h, char *buf, int32_t len)
{

  switch(h->type) {
  case CONNECTION_TYPE_SOCKET:
    return readBytes(h->sock, buf, len);
  default:
    do_assert(0, "GIOP_ConnectionHandler_recv : bad type");
    return -1;
  }
}

void * GIOP_ConnectionHandler_get_arg(GIOP_ConnectionHandler *h)
{
  switch(h->type) {
  case CONNECTION_TYPE_SOCKET:
    return get_SockProfile_arg(h->sock);
  default:
    do_assert(0, "GIOP_ConnectionHandler_get_arg");
    break;
  }
  return NULL;
}

void GIOP_ConnectionHandler_close(GIOP_ConnectionHandler *h)
{
  switch(h->type) {
  case CONNECTION_TYPE_SOCKET:
    close(h->sock);
    break;
  default:
    break;
  }
}
