/**************************************************************************

 myOptions.h

***************************************************************************/
#include "myOptions.h"

int checkLongOptions(char *lopt, struct myOption *options, const char *arg){
  int i=0;
  int res=-1;
  int *v;
  char **sv;
  struct myOption *subtable;
  void (*callbackfunc)();
  int shift=1;
  int check_longopt=1;
  int check_shortopt=1;

  callbackfunc = NULL;
  while( options[i].type > 0){
    if( options[i].type == OPT_ARG_INCLUDE_TABLE){
      subtable = options[i].val;
      res = checkLongOptions(lopt, subtable, arg);
      if(res >= 0) return res;
      i++;
      continue;
    }

    if( options[i].type == OPT_ARG_CALLBACK){
      callbackfunc = options[i].val;
      i++;
      continue;
    }

    if(options[i].type & OPT_ARGFLAG_ONEDASH){
       shift=0;
    }

    check_longopt=strncmp(lopt+shift, options[i].longopt,strlen(options[i].longopt)); 
    if(options[i].shortopt){
      check_shortopt=strncmp(lopt, &options[i].shortopt, 1);
    }

    if(check_longopt == 0 || check_shortopt == 0){
      res=0;
      if(callbackfunc){
        char *args;
        if(index(lopt, '=')) args = index(lopt,'=') +1;
        else args = lopt+1;

        (*callbackfunc)(&options[i], args, NULL);
      }else{
        switch(options[i].type){
          case OPT_ARG_NONE:
            v = options[i].val;
            *v = 1;
            break;
          case OPT_ARG_INT:
            v = options[i].val;
            *v = atoi(arg);
            res=1;
            break;
          case OPT_ARG_STRING:
            sv = options[i].val;
            if(check_longopt == 0){
              *sv = index(lopt, '=')+1;
            }else if(check_shortopt == 0){
              *sv = strdup(arg);
              res=1;
            }
            break;
          default:
            break;
        }
      }
      return res;
    }
    i++;
  }
  return -1;
}

void mk_option_usage(struct myOption *options){
  int i=0;
  struct myOption *subtable;
  char *dash = "--"; 
  char aa[10]; 

  while( options[i].type > 0){
    if(options[i].type == OPT_ARG_INCLUDE_TABLE){
      subtable = options[i].val;
      mk_option_usage(subtable);
    }else{
      if(options[i].type != OPT_ARG_CALLBACK){
        if(options[i].type & OPT_ARGFLAG_ONEDASH){
          dash = "-"; 
        }
        if(options[i].shortopt){
          sprintf((char *)aa, "  -%c, %s", options[i].shortopt , dash);
        }else{
          sprintf((char *)aa, "      %s", dash);
        }
        if(options[i].optarg){
          fprintf(stderr, "%s%s=%s \t\t %s\n",aa, options[i].longopt, options[i].optarg, options[i].optdesc);
        }else{
          if(options[i].type & OPT_ARG_STRING){
            fprintf(stderr, "%s%s=STRING \t\t %s\n",aa,options[i].longopt, options[i].optdesc);
          }else{
            fprintf(stderr, "%s%s \t\t %s\n",aa,options[i].longopt, options[i].optdesc);
          }
        }
      }
    }
    i++;

  }
}

void show_usage(struct myOption *options){
  fprintf(stderr, "Usage: %s [Options] file1 file2 ...\n", cmdname);
  mk_option_usage(options);
  fprintf(stderr, "\n\n");
}

int parseOptions(int argc, const char *argv[],struct myOption *options){
  int i, cp = 1;
  char *val;
  char *Opt;

  for(i=cp; i<argc;i++){
    val =  (char *)argv[i];
    if(*val ==  '-'){
      Opt = val+1;
      int vv;
      if((vv=checkLongOptions(Opt, options, argv[i+1])) < 0){
       show_usage(options);
       return -1;
      }
      i += vv;
    }else{
      break;
    }
  }
  return i;
}
