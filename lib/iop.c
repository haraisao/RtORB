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
 *  	iop.c
 *  Copyright(C) 2006, Isao Hara, AIST
 *
 *  $Revision: 1.2 $
 *  $Date: 2008/05/19 06:28:08 $
 *  $Id: iop.c,v 1.2 2008/05/19 06:28:08 yoshi Exp $
 */
/*!
 * @file iop.c
 * @if jp
 * @brief IOP(Inter-Orb-Protocol)、IOR、Parse処理などの定義。
 * @else
 * @brief Definition of IOP(Inter-Orb-Protocol), IOR, parsing, etc..
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#define _GNU_SOURCE
#include <string.h>
#include <RtORB/corba.h>
#include <RtORB/codeset.h>

const uint32_t RTORB_ORB_TYPE = 0x52544d00; /*   RTM\x00  */

/****  IOR  ****/
int Create_Tag_Internet_IOP(char *dist, int sock, char *object_key){
   int current = 0;
   uint32_t orb_type_data[] = { 1, RTORB_ORB_TYPE };
   CORBA_Sequence orb_type = { 2, 2, orb_type_data, 0, tk_long };
   uint32_t code_sets_data[] = {
	   1,
	   CodeSet_8859_1, /* 0x00010001 */
	   1,
	   CodeSet_UTF_8,  /* 0x05010001 */
	   CodeSet_UTF_16,  /* 0x00010109 */
	   1,
	   CodeSet_UTF_16  /* 0x00010109 */
   };
   CORBA_Sequence code_sets = { 7, 7, code_sets_data, 0, tk_long };

   char *hostname = (char *)get_ip_address(sock);
   if(!hostname) hostname = "127.0.0.1";
   unsigned short port = get_socket_port(sock);

   dist[current++]=1; /* little endian */
   dist[current++]=1; /* version.major */
#if 1
   dist[current++]=2; /* version.minor */
#else
   dist[current++]=0; /* version.minor */
#endif
   dist[current++]=0;

#ifdef DEBUG_CALL
  fprintf(stderr, "Call in  Create_Tag_Internet_IOP\n");
#endif
   marshalString((octet *)dist, &current, hostname);
   RtORB_free(hostname,"Create_Tag_Internet_IOP");

   marshalShort((octet *)dist, &current, port);
   marshalString((octet *)dist, &current, object_key);

    /**** Tagged Component  ****/
#if 1
   marshalLong((octet *)dist, &current, 2);   /* Number of TaggedComponent  */
   
   marshalLong((octet *)dist, &current, IOP_TAG_ORB_TYPE);    /* ORB_TYPE  */
   marshal_CORBA_Sequence((octet *)dist, &current, &orb_type);

   marshalLong((octet *)dist, &current, IOP_TAG_CODE_SETS);   /* Default CODE_SETS  */
   marshal_CORBA_Sequence((octet *)dist, &current, &code_sets);
#else
   marshalLong((octet *)dist, &current, 1);   /* Number of TaggedComponent  */
   
   marshalLong((octet *)dist, &current, IOP_TAG_ORB_TYPE);    /* ORB_TYPE  */
   marshal_CORBA_Sequence((octet *)dist, &current, &orb_type);
#endif

   return current; 
}

int Create_Default_IOR(char *buf, int sock, char *typedId, char *object_key){
   int current=0;
   char *iiop = NULL;
   int iiop_size;

   iiop = (char *)RtORB_alloc( 4096, "Create_Default_IOR");
   memset(iiop, 0, 4096);

#ifdef DEBUG_CALL
   fprintf(stderr, "Call in Create_Default_IOR\n");
#endif
   marshalString((octet *)buf, &current, typedId);	/* Typed ID */
   Address_Alignment(&current, 4);
   marshalLong((octet *)buf, &current, 1); 		/* Number of TaggedProfiles */
   marshalLong((octet *)buf, &current, IOP_TAG_INTERNET_IOP); /* Profile:TAG_INTERNET_IOP */

   iiop_size = Create_Tag_Internet_IOP(iiop, sock, object_key);
   marshal_Octet_Sequence((octet *)buf, &current, (octet *)iiop, iiop_size);

   buf[current]=0;

   if ( iiop ){
     RtORB_free( iiop,"Create_Default_IOR");
   }

   return current;
}
/*******************************************/
/****  ToString Helper functions        ****/
int Sequence_Octet_to_String(char *dist, char *src, int size){
  int i,j;
  char c;

  for(i=0,j=0;i<size;i++){
    c = src[i] >> 4;
    dist[j++]= hexdigit(c);
    dist[j++]= hexdigit(src[i]);
  }
/*  dist[j]= 0;  */
  return j;
}

int Long_to_String(char *dist, int32_t val){
   char buf[4];
   memcpy(buf, &val, 4);
   return Sequence_Octet_to_String(dist, buf, 4);
}

int CORBA_Sequence_Octet_to_String(char *dist, char *src, int size){
   int n=0;
   n += Long_to_String(dist, size);
   n += Sequence_Octet_to_String(dist+n, src, size);
   return n;
}

/***** Create default IOR string *****/
int createIOR(char *buf, int sock, char *typedId, char *object_key){
/* char ior[4096]; */
   int ior_size;
   int current=12;
   char *ior = NULL;
   ior = ( char *)RtORB_alloc( 4096,"createIOR");
   memset(ior,0,4096);

#if __CDR_ORDER__
   memcpy(buf, "IOR:01000000", 12);       /* 'IOR:' + byte_code + padding  */
#else
   memcpy(buf, "IOR:00000000", 12);       /* 'IOR:' + byte_code + padding  */
#endif
   ior_size = Create_Default_IOR(ior, sock, typedId, object_key);
   current += Sequence_Octet_to_String(&buf[current], ior, ior_size);

   buf[current]=0;

   if ( ior ){
     RtORB_free( ior, "createIOR");
   }

   return 1;
}

/****** CORBA_URL ******/
CORBA_URL *
CORBA_URL__alloc(int32_t n){
  int i;
  CORBA_URL *url = (CORBA_URL *)RtORB_alloc(sizeof(CORBA_URL) * n, "CORBA_URL__alloc");
  for(i=0;i<n;i++){
    url[i].protocol = PROTO_IIOP; /* default is IIOP */
    url[i].byte_order = 1; 	 /* Littel endian */
    url[i].version_major = 1; 
    url[i].version_minor = 0;  
    url[i].type_id = NULL; 
    url[i].hostname = NULL; 
    url[i].port = 2809;		/* default port number, usually NameService */
    url[i].object_key = NULL; 
    url[i].naming_context = NULL; 
    url[i].object_key_len = 0; 
    url[i].flags = 0; 
    url[i]._ior_string_len = 0; 
    url[i]._ior_string = NULL; 
  }

  return url;
}

void
CORBA_URL__delete(CORBA_URL *url, int32_t n){
  int i;
  for(i=0;i<n;i++){
    String__delete(url[i].type_id, "CORBA_URL__delete(type_id)");
    String__delete(url[i].hostname, "CORBA_URL__delete(hostname)");
    String__delete(url[i].object_key, "CORBA_URL__delete(object_key)");
    String__delete(url[i].naming_context, "CORBA_URL__delete(naming_context)");
    String__delete(url[i]._ior_string, "CORBA_URL__delete(_ior_string)");
  }
  return;
}

#define SET_LONG(BUF, POS, V) { int pos__ = POS; marshalLong(BUF, &pos__, V); }

/* Parsse IOR and change byte order */
int parseIOR(CORBA_URL **data, octet *ior, int *pos, int byte_order){
  uint32_t profile_len, profile_count;
  uint32_t profile_type;
  int tmp;
  int i, res = 0;
  int profile_start;
  int _start = (*pos);
  int _ior_len = 0;

  CORBA_URL *url = (CORBA_URL *)CORBA_URL__alloc(1);

  url->type_id = (char *)demarshalString((octet *)ior, pos, byte_order, &tmp);
  SET_LONG(ior, *pos - tmp - 4, tmp);

  Address_Alignment(pos, 4);
  profile_count = demarshalLong(ior, pos, byte_order);
  SET_LONG(ior, *pos-4, profile_count);

  for(i=0;i<profile_count;i++){
    profile_type = demarshalLong(ior, pos, byte_order);
    SET_LONG(ior, *pos-4, profile_type);

    profile_len = demarshalLong(ior, pos, byte_order);
    SET_LONG(ior, *pos-4, profile_len);

    profile_start =  *pos;

   switch(profile_type){
      case 0: 
	res = 1;
        url->byte_order = ior[*pos];
	*pos += 1;
        url->version_major = ior[*pos];
	*pos += 1;
        url->version_minor = ior[*pos];
	*pos += 1;

        Address_Alignment(pos, 4);

        url->hostname = demarshalString((octet *)ior, pos, url->byte_order, &tmp);
        Address_Alignment(pos, 2);
        url->port = demarshalShort(ior, pos, url->byte_order);
        Address_Alignment(pos, 4);
  
        url->object_key = demarshalString((octet *)ior, pos, url->byte_order,
			(int *)&url->object_key_len);

	*pos = profile_start + profile_len;
    /* parse Tagged Profiles
       not implemented
    */
         break;
     default:
         *data = NULL;
/*
         RtORB_free(url, "parseIOR");
*/
	 break;
   }
  }

/*
  char *buf=(char *)RtORB_alloc(1024,"parseIOR(buf)");
  memset(buf,0,1024);
  _ior_len = *pos - _start;
  sprintf(buf, "IOR:0%d000000%s", byte_order, Octet2String(ior+_start, _ior_len));

  url->_ior_string_len = _ior_len +12;
  url->_ior_string = (char *)RtORB_alloc(_ior_len+13, "parseIOR");
  memcpy(url->_ior_string, buf , url->_ior_string_len+1);
  free(buf);
*/
  _ior_len = *pos - _start;
/*
  sprintf(buf, "IOR:0%d000000%s", byte_order, Octet2String(ior+_start, _ior_len));
*/
  url->_ior_string_len = _ior_len ;
  url->_ior_string = (char *)RtORB_alloc(_ior_len, "parseIOR");
  memcpy(url->_ior_string, ior+_start , url->_ior_string_len);

  if(res){  *data = url;  }

/* We should clean up memories....  */

  return res;
}

#if DEBUG
void 
printCORBA_URL(CORBA_URL *url){
  fprintf(stderr, "protocol = %d\n", url->protocol);
  fprintf(stderr, "hostname = %s\n", url->hostname);
  fprintf(stderr, "port = %d\n", url->port);
  fprintf(stderr, "object_key = %s\n", url->object_key);
  fprintf(stderr, "name = %s\n", url->naming_context);
}
#endif

char *nextToken(char **dist, char *str, const char *seed)
{
   int i;
   char *tmp = str;

   if(!str){
      *dist = NULL;
      return NULL;
   }

   for(i=0; *tmp; i++, tmp++){
     if(index(seed, *tmp) ) break;
   }
   *dist = (char *)RtORB_strndup(str, i, "nextToken");
#if DEBUG
   fprintf(stderr, "nextToken = %s \n", *dist);
#endif
   return tmp;
}

void
parseURL(CORBA_URL *target, char *str){
  char *tmp;

  str = nextToken(&tmp, str, ":");
  if(!strcmp(tmp,"rtr")){
    target->protocol = PROTO_RIR;
    RtORB_free(tmp, "parseURL(proto)");
    return;
  }
  RtORB_free(tmp, "parseURL(proto)2");

  str = nextToken(&tmp, ++str, ":@");
  if(*str == '@'){
     sscanf(tmp, "%d.%d", (int *)(&target->version_major), (int *)(&target->version_minor));
     RtORB_free(tmp, "parseURL(version)");
     str = nextToken(&tmp, ++str, ":");
  }
  target->hostname = RtORB_strdup(tmp, "parseURL(host)");
  RtORB_free(tmp, "parseURL(host)");

  if(RtORB_strlen(str)){
    str = nextToken(&tmp, ++str, ":@");
    target->port = atoi(tmp);
    RtORB_free(tmp, "parseURL(port)3");
  }

  return;
}


char *
extractURL(char **dist, char *str, int *num, const char *delim){
  char *url, *retval = NULL;


  if( *num > MAX_URLS){
     fprintf(stderr, "Error: no more urls acceptable\n"); 
     return strchr(str, '/');
  }

  str = nextToken(&url, str, delim);
  dist[*num] = url;
  *num += 1;

  switch(*str){
      case ',':
	retval = extractURL(dist, str+1, num, delim);
	break;
      case '/':
	retval = str+1;
        break;
      case '#':
	retval = str+1;
        break;
      default:
	break;
  }
  if(retval) return RtORB_strdup(retval, "extractURL");
  else return NULL;
}

int parseCorbalocURL(CORBA_URL **data, char *str){
  int num = 0;
  int i;
  char *dist[MAX_URLS], *obj_key;

  obj_key = extractURL(dist, str, &num, ",/");
  CORBA_URL *url = (CORBA_URL *)CORBA_URL__alloc(num);

  if(!obj_key || *obj_key == '\0')
	  obj_key = RtORB_strdup("NameService", "parseCorbalicURL");

  for(i=0;i < num;i++){
     parseURL(&url[i],  dist[i]);
     url[i].object_key = RtORB_strdup(obj_key,  "parseCorbalicURL2");
     url[i].object_key_len = RtORB_strlen(obj_key);
     RtORB_free(dist[i], "nextToken");
  }

  *data = url;
  RtORB_free(obj_key, "parseCorbalicURL3");

  return num;
}

int parseCorbanameURL(CORBA_URL **data, char *str){
  int num = 0;
  int i;
  char *dist[MAX_URLS];
  char *name;

  name = extractURL(dist, str, &num, ",#");
 
  if(!name || *name == '\0'){
    fprintf(stderr, "Error: no NameContext exists.\n");
    return 0;
  }

  CORBA_URL *url = (CORBA_URL *)CORBA_URL__alloc(num);

  for(i=0;i < num;i++){
     parseURL(&url[i],  dist[i]);
     url[i].object_key = RtORB_strdup("NameService",  "parseCorbalicURL");
     url[i].object_key_len = RtORB_strlen(url[i].object_key);
     url[i].naming_context = RtORB_strdup(name, "parseCorbanameURL(naming_context)");
  }

  *data = url;

  return num;
}

int parseCorbaURL(CORBA_URL **data, char *str){
  int retval = 0;
  int len;
  int pos;
  octet *IOR;

  if(!strncasecmp(str, "IOR:", 4)){
    len = strlen(str + 4) / 2;
    IOR = String2Octet(str + 4);
/*
    retval = parseIOR(data, IOR + 4, len-4, IOR[0]);
*/
    pos = 4;
    retval = parseIOR(data, IOR, &pos,  IOR[0]);
  }else if(!strncasecmp(str, "corbaloc:", 9)){

    retval = parseCorbalocURL(data, str + 9);

  }else if(!strncasecmp(str, "corbaname:", 10)){

    retval = parseCorbanameURL(data, str + 10);

  }else{

     fprintf(stderr, "Unknown URL: %s \n", str);

  }
   
  return retval;
}

