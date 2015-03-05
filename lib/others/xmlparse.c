/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 *  Simple XLM Parser
 *  Copyright(c) Isao Hara, 2004, 2005
 *
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/01/08 07:56:07 $
 *  $Id: xmlparse.c,v 1.1.1.1 2008/01/08 07:56:07 yoshi Exp $
 *
 */

#include "xmlparse.h"

#define IS_KANJI(P) (((*P) & 0x80) == 0x80)

///////////////// XML Parser
xmlTag *newTag(xmlTag *parent){
 xmlTag *ttag;
 ttag = (struct xmlTag *)malloc(sizeof(struct xmlTag));
 ttag->prev = (xmlTag *)0;
 ttag->next = (xmlTag *)0;
 ttag->children = NULL;
 ttag->parent = parent;
 ttag->name = NULL; 
 ttag->value = NULL; 
 ttag->params = 0; 
 ttag->param = NULL; 
 ttag->nchildren = 0; 
 if(parent){
   ttag->level = parent->level +1; 
   parent->nchildren++;
   if(parent->children == NULL) parent->children = ttag; 
 }
 return ttag;
}

void freeTag(xmlTag *tag){
  //fprintf(stderr, "Call freeTag %s\n", tag->name);
  while(tag->nchildren) freeTag(tag->children);
  if(tag->parent){
    (tag->parent)->nchildren--;
    if((tag->parent)->children == tag) tag->parent->children = tag->next; 
  }
  if(tag->prev) (tag->prev)->next = tag->next;
  if(tag->next) (tag->next)->prev = tag->prev;
  if(tag->name)  free(tag->name); 
  if(tag->value) free(tag->value); 
  if(tag->param) free(tag->param); 

  free(tag);
  return; 
}

char getChar(int fd){
 char ch;
 int n;
 if((n= read(fd, &ch, 1)) != 1) {
//   fprintf(stderr, "Error in getChar (%d)\n", n);
   close(fd);
   return(-1);
 }
 return ch;
}

char *skipBlank(char *str){ while(!IS_KANJI(str) && isspace(*str)) str++; return str;}

int countParams(char *str){
  int i,n,count;

  n = strlen(str);
  for(i=0,count=0;i < n; i++){
   if(str[i] == '=') count++;
  }
  return count;
}
 
TagParam *parseParams(char *tag, int *c){
  int n, i, j,count,len;
  int params;
  TagParam *param;
  int isParam = 1, isValue = 0;
  int isString = 0;

  tag = skipBlank(tag);
  len = strlen(tag);
  *c = count = countParams(tag);
  param = (TagParam *)malloc(sizeof(TagParam)*count);
  if(param == NULL) fprintf(stderr,"Fail to alloc memory:param");
  
  for(i=0,j=0,count=0,n=0; n < len+1;n++, tag++){
    if(i > MAX_LEN && j > MAX_LEN){
      fprintf(stderr, "Error: buffer overflow in parseParams\n");
      return NULL;
    }
    if(isString == 0 && (isspace(*tag) || *tag == '\0')){
       if(isParam == 0 && isValue == 1 ){
         param[i].value[j] = '\0';
         isParam = 1; isValue = 0;
	 i++; j=0; count++;
       } else if(isParam == 1 && isValue == 0){
	isParam = 0; j=0;
        param[i].id[j] = '\0';
       }
    }else{
     if (*tag == '='){
        param[i].id[j] = '\0';
	isParam = 0; j=0;
     }else{
       if(isParam){
        param[i].id[j++] = *tag;
       }else{
	isValue = 1;
        if(*tag == '"') isString = ~isString;
        else param[i].value[j++] = *tag;
       }
     }
    }
  }

  *c = count;
  return param;
}

int skipComment(int fd){
  char ch;
  int isComment = 1;
  int isaComment = 0;

  while(isComment){
    if((ch = getChar(fd)) == -1) return 0;
    if(ch == '-') isaComment++;
    else{
      if(isaComment > 1 && ch == '>') isComment = 0;
      isaComment = 0;
    }
  }
  return 1;
}

int checkHeader(int fd){
  char ch, cch;
  int i,j,k,n;
  int isHeader = 0;
  int isParam, isValue;
  int chkTag = 0;
  int count;
  char *tag,*pptr, *vptr;
  char tmptag[1024];
  TagParam *param;

  int okVersion = -10;

  i=0;
  tag = tmptag;
  while((ch = getChar(fd)) >= 0){
    if(!isHeader && ch == '<'){
      if((cch = getChar(fd)) < 0) return cch;
      if(cch == '?') isHeader = 1;
    }else if(isHeader){
      if(ch == '?'){
        if((cch = getChar(fd)) < 0) return cch;
        if(cch == '>'){ tmptag[i] = '\0'; break; }
        else return -1; // format error
      }else{
	 tmptag[i++] = ch; 
      }
    }
  }

  tag = skipBlank(tag);
  if(strncasecmp(tag, "xml",3)) return -1;
  tag = skipBlank(tag+3);

  param = parseParams(tag, &count);

  for(i=0;i<count;i++){
    pptr = param[i].id; vptr = param[i].value;
    if(!strcasecmp(pptr,"version") &&
       !strcmp(vptr,"1.0")) return 1;
  }
  return -1;
}

/*
	parse content

**/


char getCharFromString(char *str){
 if(!IS_KANJI(str) && *str == '\0') {
   return(-1);
 }
 return *str;
}

int skipCommentFromString(char *str){
  char ch;
  int isComment = 1;
  int isaComment = 0;

  while(isComment){
    if((ch = getCharFromString(str++)) == -1) return 0;
    if(ch == '-') isaComment++;
    else{
      if(isaComment > 1 && ch == '>') isComment = 0;
      isaComment = 0;
    }
  }
  return 1;
}

xmlTag *parse_xml_string(char *buf){
  int i,j,count,ii;
  char ch, cch;
  int isComment = 0;
  int err = 0;
  int isSingleTag = 0;
  char tag[MAX_LEN+1];
  char *ptag;
  char value[1024];
  int isaComment, noParam;
  int level = -1;
  xmlTag *current_tag = NULL;
  xmlTag *top_tag = NULL;
  xmlTag *prev_tag = NULL;
  TagParam *param;
  char *bb=buf;
  int isString;

  j=0;

  while((ch = getCharFromString(buf++)) != -1){

    if(ch == '<'){
      noParam=1;
      isSingleTag = 0;
      isComment = 0;
      memset(tag,0,MAX_LEN+1);
      for(i=0 ; i<MAX_LEN ; i++){ /// read tag
        if(i==3 && !strncmp(tag,"!--", 3)){ isComment = 1; break; }
        if((cch = getCharFromString(buf++)) == -1){
             fprintf(stderr, "Error in read tag \n");
             return 0;
        }
        if(isspace(cch)){ tag[i]= '\0'; noParam = 0; break; }
        if(cch == '>'){ 
	  if(tag[i-1] == '/'){
	     isSingleTag=1;
	     tag[i-1]='\0';
          }else tag[i]= '\0';
	  break; 
//        if(cch == '/'){ tag[i]= '\0'; isSingleTag = 0; }
	}
        tag[i] = cch;
      }
       // check comment
      if(isComment){
        if(skipCommentFromString(buf) == 0) return 0;
      }else{
        if(tag[0] == '/'){ // endTag
          if(current_tag == NULL){
            fprintf(stderr, "Format error : Invalid Tag found %s\n",tag);
            return NULL;
          }
	  if(strcmp(current_tag->name, &tag[1])){
            fprintf(stderr, "Format error %s -> %s\n", current_tag->name, tag);
            return NULL;
          }
          value[j] = '\0';
 	  if(strlen(value) > 0) {
            current_tag->value = (char *)malloc(strlen(value)+1);
            strcpy(current_tag->value,value);
          }
          prev_tag = current_tag;
          current_tag = current_tag->parent;
          level--; 
	  j=0;
        }else{
          current_tag = newTag(current_tag);
          current_tag->name = (char *)malloc(strlen(tag)+1);
          strcpy(current_tag->name,tag);
	  if(top_tag == NULL){ top_tag= current_tag; }
          level++;
          current_tag->level = level;

          if(!noParam){
  	    ptag = tag;
      	    isString = 0;
            for(i=0 ; i<MAX_LEN ; i++){ /// read tag
              if((cch = getCharFromString(buf++)) == -1){
		fprintf(stderr, "Error in parse param %s\n",tag);
  	         return 0;
	      }
		if(cch == '"'){ isString = ~isString; }
		if(!isString){
              	  if(cch == '>'){ tag[i]= '\0'; break; }
              	  if(cch == '/'){ tag[i]= '\0'; isSingleTag = 1; break; }
		}	

              tag[i] = cch;
            }
  	    ptag = skipBlank(ptag);
  	    param = parseParams(ptag, &count);
            current_tag->params = count;
            current_tag->param = param;
          }
          noParam=1;
	  if( prev_tag != NULL && (prev_tag->level == current_tag->level)){
            prev_tag->next = current_tag;
            current_tag->prev = prev_tag;
          }else{
	    prev_tag = NULL;
          }
	  j=0;
	  if(isSingleTag == 1){
            prev_tag = current_tag;
            current_tag = current_tag->parent;
            level--; 
	  }
        }

      }
    }

    //if(ch != '<' && ch != '\n') value[j++] = ch;
    if(ch != '<') value[j++] = ch;
  } 
  if(level == -1) return top_tag;
  else{
    fprintf(stderr, "**Error in parse_xml_string level=%d\n%s\n",level,bb);
    return NULL;
  }
}

/////////////////////////////////
xmlTag *parse_xml_file(int fd){
  int i,j,count;
  char ch, cch;
  int isComment = 0;
  int err = 0;
  int isSingleTag = 0;
  char tag[MAX_LEN+1];
  char *ptag;
  char value[1024];
  int isaComment, noParam;
  int level = -1;
  xmlTag *current_tag = NULL;
  xmlTag *top_tag = NULL;
  xmlTag *prev_tag = NULL;
  TagParam *param;
  int isString;

  if( (err = checkHeader(fd)) < 0){
    fprintf(stderr,"Error: This isn't XML file (%d)\n",err);
    return 0;
  }

  j=0;

  while((ch = getChar(fd)) != -1){

    if(ch == '<'){
      noParam=1;
      isSingleTag = 0;
      isComment = 0;
      for(i=0 ; i<MAX_LEN ; i++){ /// read tag
        if(i==3 && !strncmp(tag,"!--", 3)){ isComment = 1; break; }
        if((cch = getChar(fd)) == -1) return 0;
        if(isspace(cch)){ tag[i]= '\0'; noParam = 0; break; }
        if(cch == '>'){ 
	  if(tag[i-1] == '/'){
	     isSingleTag=1;
	     tag[i-1]='\0';
          }else tag[i]= '\0';
	  break; 
	}
       
        tag[i] = cch;
      }

       // check comment
      if(isComment){
        if(skipComment(fd) == 0){
          fprintf(stderr, "Error in parse_xml_file\n");
         return 0;
        }
      }else{
        if(tag[0] == '/'){ // endTag
          if(current_tag == NULL){
            fprintf(stderr, "Format error : Invalid Tag found %s\n",tag);
            return NULL;
          }
	  if(strcmp(current_tag->name, &tag[1])){
            fprintf(stderr, "Format error %s -> %s\n", current_tag->name, tag);
            return NULL;
          }
          value[j] = '\0';
 	  if(strlen(value) > 0) {
            current_tag->value = (char *)strdup(value);
/*
            current_tag->value = (char *)malloc(strlen(value)+1);
            strcpy(current_tag->value,value);
*/
          }
          prev_tag = current_tag;
          current_tag = current_tag->parent;
          level--; 
	  j=0;
        }else{
          current_tag = newTag(current_tag);
          current_tag->name = (char *)malloc(strlen(tag)+1);
          strcpy(current_tag->name,tag);
	  if(top_tag == NULL){ top_tag= current_tag; }
          level++;
          current_tag->level = level;

          if(!noParam){
  	    ptag = tag;
	   isString = 0;
            for(i=0 ; i<MAX_LEN ; i++){ /// read tag
              if((cch = getChar(fd)) == -1) return 0;
              if(cch == '>'){ tag[i]= '\0'; break; }
		if(cch == '"'){ isString = ~isString; }
		if(!isString){
              	  if(cch == '>'){ tag[i]= '\0'; break; }
              	  if(cch == '/'){ tag[i]= '\0'; isSingleTag = 1; break; }
		}	
              tag[i] = cch;
            }
  	    ptag = skipBlank(ptag);
  	    param = parseParams(ptag, &count);
            current_tag->params = count;
            current_tag->param = param;
          }

          noParam=1;
	  if( prev_tag != NULL && (prev_tag->level == current_tag->level)){
            prev_tag->next = current_tag;
            current_tag->prev = prev_tag;
          }else{
	    prev_tag = NULL;
          }
	  j=0;
	  if(isSingleTag == 1){
            prev_tag = current_tag;
            current_tag = current_tag->parent;
            level--; 
	  }
        }

      }
    }

//    if(ch != '<' && ch != '\n') value[j++] = ch;
    if(ch != '<') value[j++] = ch;
  } 
  if(level == -1) return top_tag;
  else{
    fprintf(stderr, "**Error in parse_xml_string level=%d\n",level);
    return NULL;
  }
}
/////////////////////////////////////

char *getParamValue(xmlTag *tag, char *param){
   int j;

   for(j=0;j < tag->params;j++){
      if(!strcmp(tag->param[j].id, param)){ return(tag->param[j].value); }
   }
   return NULL;
}

int tagNameComp(xmlTag *tag, char *name){
  return (!strcasecmp(tag->name, name));
}


void appendXmlTag(xmlTag *top, xmlTag *tag){
   xmlTag *tmp = top;

   while(tmp->next) tmp = tmp->next;
   tmp->next = tag;
   tag->prev = tmp;
}

int countXmlTag(xmlTag *top){
   int n=1;
   if(!top) return 0;
   while(top->next){ top = top->next; n++; }
   return n;
}

xmlTag *getTagInfo(xmlTag *tag, char *name){
  xmlTag *tmp, *result=NULL;

   while(tag){
     if(tagNameComp(tag, name)){
        tmp = newTag(NULL);
	tmp->name = tag->name;
        tmp->value = tag->value;
        tmp->param = tag->param;
        tmp->params = tag->params;
//        fprintf(stderr, "### %s %s %d, %d\n",tmp->name, tmp->value,
//                tmp->param, tmp->params);
	if(result)  appendXmlTag(result, tmp);
        else result = tmp;
     }
	 tag=tag->next;
   }
   return result;
}

