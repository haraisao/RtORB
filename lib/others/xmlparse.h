/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 * Simple XLM Parser
 *  Copyright(C)  2004-2006, Isao Hara, AIST
 *
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/01/08 07:56:07 $
 *  $Id: xmlparse.h,v 1.1.1.1 2008/01/08 07:56:07 yoshi Exp $
 */

#ifndef __XML_PARSER_H__
#define __XML_PARSER_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_LEN 256

typedef struct TagParam{
 char id[MAX_LEN+1];
 char value[MAX_LEN+1]; 
} TagParam;

typedef struct xmlTag{
  struct xmlTag *prev;
  struct xmlTag *next;
  char *name;
  char *value;
  int params;
  TagParam *param;
  struct xmlTag *parent;
  struct xmlTag *children;
  int nchildren;
  int level;
} xmlTag;

///////////////// XML Parser
xmlTag *newTag(xmlTag *parent);
void freeTag(xmlTag *tag);
char getChar(int fd);
char *skipBlank(char *str);
int countParams(char *str);
TagParam *parseParams(char *tag, int *c);
int skipComment(int fd);
int checkHeader(int fd);
xmlTag *parse_xml_file(int fd);
xmlTag *parse_xml_string(char *buf);

char *getParamValue(xmlTag *tag, char *param);
int tagNameComp(xmlTag *tag, char *name);

void appendXmlTag(xmlTag *top, xmlTag *tag);
int countXmlTag(xmlTag *top);
xmlTag *getTagInfo(xmlTag *tag, char *name);

#endif
