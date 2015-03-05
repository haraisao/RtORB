/**************************************************************************

    orbit-idl-main.c (Driver program for the IDL parser & backend)

    Copyright (C) 1999 Elliot Lee

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    $Id: orbit-idl-main.c,v 1.1.1.1 2008/01/08 07:56:07 yoshi Exp $

***************************************************************************/
#include "config.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <glib.h>
#include <libIDL/IDL.h>

#include "myOptions.h"


/* Settings made from the command line (prefaced with cl_) */
static gboolean cl_disable_stubs = FALSE,
  cl_disable_skels = FALSE,
  cl_disable_common = FALSE,
  cl_disable_headers = FALSE,
  cl_disable_decls = FALSE,
  cl_enable_cppif = FALSE,
  cl_enable_skeleton_impl = FALSE;
static int cl_idlwarnlevel = 2;
static int cl_debuglevel = 0;
static int cl_is_pidl = 0;
static int cl_output_version = 0;
static int cl_disable_idata = 0;
static int cl_enable_imodule = 0;
static int cl_add_imodule = 0;
static gboolean cl_disable_defs_skels = FALSE;
static gboolean cl_showcpperrors = TRUE;
static char *cl_output_lang = "c";
static char *cl_header_guard_prefix = "";
static char *cl_backend_dir = NULL;
static gboolean cl_onlytop = FALSE;
static char *cl_deps_file = NULL;

static gboolean cl_nocppdefs = FALSE;

#define BASE_CPP_ARGS "-D__ORBIT_IDL__ "
static GString *cl_cpp_args;

static char *c_output_formatter = INDENT_COMMAND;

char cmdname[] = "rtorb-idl";

static void
cl_libIDL_version_callback( const struct myOption *opt, char *arg, void *data)
{
  g_print("libIDL %s (CORBA %s)",
	  IDL_get_libver_string(),
	  IDL_get_IDLver_string());

  exit(0);
}

static void
cl_cpp_callback(const struct myOption *opt, char *arg, void *data)
{
  g_assert(opt!=NULL);

  if(opt->shortopt=='D')
    g_string_append(cl_cpp_args, "-D");
  else if(opt->shortopt=='I')
    g_string_append(cl_cpp_args, "-I");

  g_string_append(cl_cpp_args, arg);
  g_string_append_c(cl_cpp_args, ' ');
}

static const struct myOption cl_libIDL_callback_options[] = {
  {NULL, '\0', OPT_ARG_CALLBACK, (void *)cl_libIDL_version_callback, 0, NULL, NULL},
  {"libIDL-version", '\0', OPT_ARG_NONE, NULL, 0, "Show version of libIDL used.", NULL},
  {NULL, '\0', 0, NULL, 0, NULL, NULL}
};

static const struct myOption cl_cpp_callback_options[] = {
  {NULL, '\0', OPT_ARG_CALLBACK, (void *)cl_cpp_callback, 0, NULL, NULL},
  {"define", 'D', OPT_ARGFLAG_ONEDASH | OPT_ARG_STRING, NULL, 0, "Define value in preprocessor", NULL},
  {"include", 'I', OPT_ARGFLAG_ONEDASH | OPT_ARG_STRING, NULL, 0, "Add search path for include files", NULL},
  {NULL, '\0', 0, NULL, 0, NULL, NULL}
};

struct myOption options[] = {
  {NULL, '\0',OPT_ARG_INCLUDE_TABLE, &cl_cpp_callback_options, 0, NULL, NULL},
  {NULL, '\0', OPT_ARG_INCLUDE_TABLE, &cl_libIDL_callback_options, 0, NULL, NULL},
  {"version", 'v', OPT_ARG_NONE, &cl_output_version, 0, "Output compiler version and serial", NULL},
  {"lang", 'l', OPT_ARG_STRING, &cl_output_lang, 0, "Output language (default is C)", NULL},
  {"debug", 'd', OPT_ARG_INT, &cl_debuglevel, 0, "Debug level 0 to 4", NULL},
  {"idlwarnlevel", '\0', OPT_ARG_INT, &cl_idlwarnlevel, 0, "IDL warning level 0 to 4, default is 2", NULL},
  {"showcpperrors", '\0',OPT_ARG_NONE, &cl_showcpperrors, 0, "Show CPP errors", NULL},
  {"nostubs", '\0', OPT_ARG_NONE, &cl_disable_stubs, 0, "Don't output stubs", NULL},
  {"noskels", '\0', OPT_ARG_NONE, &cl_disable_skels, 0, "Don't output skels", NULL},
  {"nocommon", '\0', OPT_ARG_NONE, &cl_disable_common, 0, "Don't output common", NULL},
  {"noheaders", '\0', OPT_ARG_NONE, &cl_disable_headers, 0, "Don't output headers", NULL},
  {"nodelcs", '\0', OPT_ARG_NONE, &cl_disable_decls, 0, "Don't output declaration header", NULL},
  {"noidata", '\0', OPT_ARG_NONE, &cl_disable_idata, 0, "Don't generate Interface type data", NULL},
  {"imodule", 'i', OPT_ARG_NONE, &cl_enable_imodule, 0, "Output only an imodule file", NULL},
  {"add-imodule", '\0', OPT_ARG_NONE, &cl_add_imodule, 0, "Output an imodule file", NULL},
  {"skeleton-impl", '\0', OPT_ARG_NONE, &cl_enable_skeleton_impl, 0, "Output skeleton implementation", NULL},
  {"cpp-interface", '\0', OPT_ARG_NONE, &cl_enable_cppif, 0, "Output cpp-interface", NULL},
  {"nocpp-defs", '\0', OPT_ARG_NONE, &cl_nocppdefs, 0, "Don't output cpp definitions", NULL},

  {"backenddir", '\0', OPT_ARG_STRING, &cl_backend_dir, 0, "Override IDL backend library directory", "DIR"},
  {"c-output-formatter", '\0', OPT_ARG_STRING, &c_output_formatter, 0, "Program to use to format output (normally, indent)", "PROGRAM"},
  {"onlytop", '\0', OPT_ARG_NONE, &cl_onlytop, 0, "Inhibit includes", NULL},
  {"pidl", '\0', OPT_ARG_NONE, &cl_is_pidl, 0, "Treat as Pseudo IDL", NULL},
  {"nodefskels", '\0', OPT_ARG_NONE, &cl_disable_defs_skels, 0, "Don't output defs for skels in header", NULL},
  {"deps", '\0', OPT_ARG_STRING, &cl_deps_file, 0, "Generate dependency info suitable for inclusion in Makefile", "FILENAME"},
  {"headerguardprefix", '\0', OPT_ARG_STRING, &cl_header_guard_prefix, 0, "Prefix for #ifdef header guards. Sometimes useful to avoid conflicts.", NULL},
  {NULL, '\0', 0, NULL, 0, NULL, NULL}
};

/********** main routines **********/
int main(int argc, const char *argv[])
{

  int cp = 1;

  int retval = 0;
  const char *arg;
  OIDL_Run_Info rinfo;

  /* Argument parsing, etc. */
  cl_cpp_args = g_string_new("-D__ORBIT_IDL__ ");

  if(getenv("C_OUTPUT_FORMATTER"))
    c_output_formatter = getenv("C_OUTPUT_FORMATTER");

  if((cp = parseOptions(argc, argv, options)) < 0){
    exit (0);
  }

  if (cl_output_version) {
	  printf ("%s %s - serial %d\n\n", cmdname,
		  VERSION, ORBIT_CONFIG_SERIAL);
	  exit (0);
  }

  /* Prep our run info for the backend */
  rinfo.cpp_args = cl_cpp_args->str;
  rinfo.debug_level = cl_debuglevel;
  rinfo.idl_warn_level = cl_idlwarnlevel;
  rinfo.show_cpp_errors = cl_showcpperrors;
  rinfo.is_pidl = cl_is_pidl;
  rinfo.do_skel_defs = !cl_disable_defs_skels;
  rinfo.enabled_passes =
     (cl_disable_stubs?0:OUTPUT_STUBS)
    |(cl_disable_skels?0:OUTPUT_SKELS)
    |(cl_disable_common?0:OUTPUT_COMMON)
    |(cl_disable_headers?0:OUTPUT_HEADERS)
    |(cl_disable_decls?0:OUTPUT_DECLS)
    |(cl_enable_skeleton_impl?OUTPUT_SKELIMPL:0)
    |(cl_enable_cppif?OUTPUT_CPPIF:0)
#if 0
    |(cl_add_imodule?OUTPUT_IMODULE:0)
    |(cl_deps_file != NULL?OUTPUT_DEPS:0)
#endif
    ;

  rinfo.deps_file = cl_deps_file;

#if 0
  if (cl_enable_imodule) /* clobber */
    rinfo.enabled_passes =
      OUTPUT_COMMON | OUTPUT_HEADERS | OUTPUT_IMODULE;

#endif

  rinfo.output_formatter = c_output_formatter;
  rinfo.output_language = cl_output_lang;
  rinfo.header_guard_prefix = cl_header_guard_prefix;
  rinfo.backend_directory = cl_backend_dir;
  rinfo.onlytop = cl_onlytop;
  rinfo.idata = !cl_disable_idata;
  rinfo.cpp_defs = !cl_nocppdefs;
  
  printf ("%s" VERSION " compiling\n", cmdname);
  printf (" %s mode, %s preprocessor errors, passes: %s%s%s%s%s%s\n\n",
	  rinfo.is_pidl ? "pidl" : "",
	  rinfo.show_cpp_errors ? "show" : "hide",
	  cl_disable_stubs ? "" : "stubs ",
	  cl_disable_skels ? "" : "skels ",
	  cl_disable_common ? "" : "common ",
	  cl_disable_headers ? "" : "headers ",
	  cl_disable_decls ? "" : "decls ",
	  cl_enable_skeleton_impl ? "skel_impl" : " "
#if 0
	  cl_enable_cppif ? "cpp" : " ",
	  cl_enable_imodule ? "imodule" : ""
#endif
        );
	   
  printf ("Debug level=%d\n", cl_debuglevel);
  printf ("lang = %s\n", cl_output_lang);
  printf ("cpp_args = %s\n", cl_cpp_args->str);

  /* Do it */
  int i;
  
  for(i=cp; i< argc;i++){
    rinfo.input_filename = g_strdup (argv[i]);
    printf("Compiling %s, %s\n", argv[i], rinfo.input_filename);

    if (!orbit_idl_to_backend(argv[i], &rinfo)) {
      g_warning("%s compilation failed", arg);
      retval = 1;
    }

    g_free(rinfo.input_filename);
  }

  return retval;
}
