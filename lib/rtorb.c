/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*!
 * @file rtorb.c
 * @if jp
 * @brief CORBA_Methodの呼び出しに関連した関数群。
 * @else
 * @brief Actions related with invoking CORBA_Methods.
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#include <RtORB/corba.h>

int
Marshal_Args(octet *buf, void **argv, int i_args, CORBA_IArg *i_argv){
  int i;
  int current = 0;

  for(i=0;i<i_args;i++){
    if(i_argv[i].io == CORBA_I_ARG_IN ||
       i_argv[i].io == CORBA_I_ARG_INOUT ){
       marshal_by_typecode(buf, argv[i], i_argv[i].tc, &current);
    }
  }
    return current;
}

void actionReply(octet *buf, CORBA_Sequence_Octet *body, CORBA_Class_Method *method, CORBA_Environment *env)
{
  int reply_status;

   GIOP_MessageHeader header;
   GIOP_ReplyHeader *reply_header;
   int byte_order = 0;


   memcpy(&header, buf, SIZEOF_GIOP_HEADER);

   reply_header = (GIOP_ReplyHeader *)newReplyHeader();
   deMarshalReply(reply_header, body, buf, &header);

   if(header.version.minor < 2){
       reply_status = reply_header->_1_0.reply_status;
   }else{
       reply_status = reply_header->_1_2.reply_status;
   }
   if((header.flags & 0x01) == 0 ) header.message_size=ntohl(header.message_size);
   else byte_order = 1;

#ifdef DEBUG_GIOP
   fprintf(stderr, "actionReply : Dump Reply\n");
   dumpMessage(body->_buffer, body->_length);
#endif
   env->_major = reply_status;

   switch(reply_status){
         case GIOP_NO_EXCEPTION:
	    break;
	 case GIOP_USER_EXCEPTION:
	    break;
	 case GIOP_SYSTEM_EXCEPTION:
	    fprintf(stderr, "System Exception occured in actionReply \n");
	    break;
	 case GIOP_LOCATION_FORWARD:
	 case GIOP_LOCATION_FORWARD_PERM:
	    dumpMessage(buf + SIZEOF_GIOP_HEADER, header.message_size);
	    fprintf(stderr, "Location forward\n");
	    break;
	 case GIOP_NEEDS_ADDRESSING_MODE:
	 default:
	    dumpMessage(buf + SIZEOF_GIOP_HEADER, header.message_size);
	    break;
    }
    deleteReplyHeader(reply_header, header.version.minor);
    return;
}


void invokeMethod_via_GIOP(CORBA_Object obj, 
		CORBA_Class_Method *method,
		void **retval,
		void **args,
		CORBA_Environment *env)
{
  int len;
  int byte_order = 0;
  int fragment = -1;
  GIOP_MessageHeader header;
  CORBA_Sequence_Octet *body;
  GIOP_ConnectionHandler *h = NULL;
  octet *arg_buf = NULL;
  octet *req_buf = NULL;
  char *buf = NULL;

  arg_buf = ( octet* )RtORB_alloc(MaxMessageSize,"invokeMethod_via_GIOP(arg_buf)");
  req_buf = ( octet* )RtORB_alloc(MaxMessageSize,"invokeMethod_via_GIOP(req_buf)");
  buf = ( char* )RtORB_alloc( MaxMessageSize,"invokeMethod_via_GIOP(buf)");

  /* connect to the remote object */
  h = make_client_connection(obj->connection);
  if (h == NULL) {
      fprintf(stderr, "Can't open connection (%s, %d)\n", obj->connection->hostname, obj->connection->port);
     if(arg_buf){ RtORB_free(arg_buf,"invokeMethod_via_GIOP(arg_buf)"); }
      if(req_buf){ RtORB_free(req_buf,"invokeMethod_via_GIOP(req_buf)"); }
      if(buf){ RtORB_free(buf,"invokeMethod_via_GIOP(buf)"); }
      exit(1);
  }

  /* check location */
#if 1
  if(!obj->_url[0].location_flags){
    if(confirmLocation(h, &obj->_url[0]) != GIOP_OBJECT_HERE )
    {
      fprintf(stderr, "CORBA_ERROR: Invalid Location \n");
      env->_major = CORBA_SYSTEM_EXCEPTION;

     RtORB_free(h, "invokeMethod_via_GIOP(h)");
     if(arg_buf){ RtORB_free(arg_buf, "invokeMethod_via_GIOP(arg_buf)"); }
     if(req_buf){ RtORB_free(req_buf, "invokeMethod_via_GIOP(req_buf)"); }
     if(buf){ RtORB_free(buf, "invokeMethod_via_GIOP(buf)"); }
     return;

    }else obj->_url[0].location_flags = 1;
  }
#endif

  /*  send request */
#ifdef DEBUG_MALLOC
  fprintf(stderr, "  send request : %s\n", method->name);
#endif  
  len = Marshal_Args(arg_buf, args, method->in_argc, method->in_argv);
#ifdef DEBUG_GIOP
  fprintf(stderr, "  ====== send request data  : %d\n", len);
  dumpMessage(arg_buf, len);
  fprintf(stderr, "  ====== \n");
#endif  
  len = createRequest(req_buf, 1, 
		  obj->_url[0].object_key,
		  obj->_url[0].object_key_len,
		  method->name,
		  strlen(method->name)+1,
		  arg_buf, len, 2 );

  /* change for shmc */
  GIOP_ConnectionHandler_send(h, (char *)req_buf, len);

  memset(&header, 0, SIZEOF_GIOP_HEADER);
  memset(buf, 0, MaxMessageSize);

  /*  receive reply */
  if(receiveMessage(h, &header, (octet *)buf, MaxMessageSize) < 0){
     env->_major = CORBA_SYSTEM_EXCEPTION;
     fprintf(stderr, "--- ERROR --\n");

     RtORB_free(h, "invokeMethod_via_GIOP(h)");
    if(arg_buf){ RtORB_free(arg_buf, "invokeMethod_via_GIOP(arg_buf)"); }
    if(req_buf){ RtORB_free(req_buf, "invokeMethod_via_GIOP(req_buf)"); }
    if(buf){ RtORB_free(buf, "invokeMethod_via_GIOP(buf)"); }
    return;
  }

 if((header.flags & 0x01) == 0 ) header.message_size=ntohl(header.message_size);
 else byte_order = 1;
 if(header.version.minor > 0) fragment = header.flags & 0x02;

  body = (CORBA_Sequence_Octet *)new_CORBA_Sequence_Octet(0);
  actionReply((octet *)buf, body, method, env);

  if(env->_major == CORBA_NO_EXCEPTION){
#ifdef DEBUG_MALLOC
  fprintf(stderr, "  deMarshal_Arguments: start\n");
#endif  
    deMarshal_Arguments(retval, args, body->_buffer, method, byte_order);
  }else if(env->_major == CORBA_USER_EXCEPTION){
       int len, i;
       int pos=0;
       env->_repo_id = (unsigned char *)demarshalString((octet *)body->_buffer, &pos, byte_order, &len);

       for(i=0;i<method->n_exceptinfo;i++){
		   if(strcmp((const char *)method->exceptinfo[i]->repository_id,
			 (const char *)env->_repo_id) == 0){

	   void *value = NULL;
	   if (size_of_typecode(method->exceptinfo[i], F_DEMARSHAL) > 0) {
	     value = RtORB_typecode_alloc(method->exceptinfo[i]);
	     demarshal_by_typecode((void **)value, 
	 	method->exceptinfo[i], body->_buffer, &pos, byte_order);
	   }
	   CORBA_any_set_exception_value(&env->_params,
					 method->exceptinfo[i],
					 value,
					 FALSE);
	   break;
	 }
       }
       if(method->n_exceptinfo == i)
          fprintf(stderr, "Unknown Exception occured: %s\n", env->_repo_id);
  }

#ifdef DEBUG_MALLOC
  fprintf(stderr, "  delete Replybody...: start\n");
#endif  
  delete_CORBA_Sequence_Octet(body, 1);

#ifdef DEBUG_MALLOC
  fprintf(stderr, "  free all local values h,arg_buf,req_buf,buf : start\n");
#endif  

  RtORB_free(h, "invokeMethod_via_GIOP(h)");

  if(arg_buf){
    RtORB_free(arg_buf, "invokeMethod_via_GIOP(arg_buf)");
  }
  if(req_buf){
    RtORB_free(req_buf, "invokeMethod_via_GIOP(req_buf)");
  }
  if(buf){
    RtORB_free(buf, "invokeMethod_via_GIOP(buf)");
  }

  return;
}


int CORBA_ORB_find_object(PortableServer_POA poa, CORBA_Object obj, CORBA_Environment *env){

  if(!poa) return 0;
  if(getValue(poa->object_map, obj->object_key)) return 1;

  return 0;
}

/*!
 * @if jp
 * @brief invokeMethod()のローカルテスト用
 * @else
 * @brief Local Test for invokeMethod()
 * @endif
 * @param obj CORBA Object
 * @param method method (CORBA_Class_Method)
 * @param retval return value of Method
 * @param args arguments of Method
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void call_local_func(CORBA_Object obj, 
		CORBA_Class_Method *method,
		void **retval,
		void **args,
		CORBA_Environment *env)
{
  RtORB_POA_Object *poa_obj;
  PortableServer_ClassInfo *info;
  CORBA_Class_Method *m_data;
  void *(*impl_method)();
  void (*call_impl_func)(PortableServer_ServantBase *, void*, CORBA_Class_Method*, void**, CORBA_Environment*, void*(*method)());
  typedef void (*impl_func_type)(PortableServer_ServantBase *, void*, CORBA_Class_Method*, void**, CORBA_Environment*, void*(*method)());

  poa_obj =(RtORB_POA_Object *) getValue(obj->poa->object_map, obj->object_key);
  info = (PortableServer_ClassInfo *)poa_obj->_private;
  PortableServer_ServantBase *sb = (PortableServer_ServantBase*)poa_obj->servant;
  call_impl_func = (impl_func_type)(*info->impl_finder)(&sb->_private, method->name, &m_data, &impl_method );
  (*call_impl_func)(sb, retval, m_data, args, env, impl_method);
}

void invokeMethod(CORBA_Object obj, 
		CORBA_Class_Method *method,
		void **retval,
		void **args,
		CORBA_Environment *env)
{
  RtORB_POA_Object *poa_obj;
  PortableServer_ClassInfo *info;
  CORBA_Class_Method *m_data;
  void *(*impl_method)();
  void (*call_impl_func)(PortableServer_ServantBase *, void*, CORBA_Class_Method*, void**, CORBA_Environment*, void*(*method)());
  typedef void (*impl_func_type)(PortableServer_ServantBase *, void*, CORBA_Class_Method*, void**, CORBA_Environment*, void*(*method)());

  /* clear for Erro handler*/
  if(env->_major)  clearException(env);

  /* check wheter object is exist in Local_Host or not(=Remote_Host).  */
  if(!CORBA_ORB_find_object(obj->poa, obj, env)){
#ifdef DEBUG
    fprintf(stderr, "  connect to %s:%d\n", obj->connection->hostname, obj->connection->port);
#endif
    /* Object is in Remote_Host */
    invokeMethod_via_GIOP(obj, method, retval, args, env);
    return;
  }

  /* Object is in Local_Host */
  poa_obj =(RtORB_POA_Object *) getValue(obj->poa->object_map, obj->object_key);
  info = (PortableServer_ClassInfo *)poa_obj->_private;
  PortableServer_ServantBase *sb = (PortableServer_ServantBase*)poa_obj->servant;
  call_impl_func = (impl_func_type)(*info->impl_finder)(&sb->_private, method->name, &m_data, &impl_method );
  (*call_impl_func)(sb, retval, m_data, args, env, impl_method);
}
