/**************************************************************************
 libpopt compatible functions
***************************************************************************/
#ifndef __MY_OPTIONS_H__
#define __MY_OPTIONS_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define OPT_ARG_NONE 		1
#define OPT_ARG_INT		1<<2
#define OPT_ARG_STRING		1<<3
#define OPT_ARG_INCLUDE_TABLE 	1<<4
#define OPT_ARG_CALLBACK 	1<<5
#define OPT_ARGFLAG_ONEDASH 	1<<6

struct myOption{
 char *longopt;
 char shortopt;
 unsigned int type;
 void *val;
 int n;
 char *optdesc;
 char *optarg;
};

extern char cmdname[];

int checkLongOptions(char *lopt, struct myOption *options, const char *arg);

void mk_option_usage(struct myOption *options);
void show_usage(struct myOption *options);
int parseOptions(int argc, const char *argv[],struct myOption *options);

#endif
