#include "config.h"
#include "orbit-idl-c-backend.h"

#include <string.h>
#include <ctype.h>


/* ch = C header */
static void ch_output_types(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci);
static void ch_output_poa(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci);
static void ch_output_itypes (IDL_tree tree, OIDL_C_Info *ci);
static void ch_output_stub_protos(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci);
static void ch_output_skel_protos(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci);

/**  external */
void ch_output_cpp_poa(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci, const char *mod_name);
void ch_output_cpp_skel_class_protos(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci);
void ch_output_cpp_stub_class_protos(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci);

void ch_output_cpp_global_functions(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci);

/* internal */
static void ch_output_interface(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci);
static void ch_output_type_struct(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci, int defcpp);
static void ch_output_type_enum(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci);
static void ch_output_type_dcl(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci);
static void ch_output_native(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci);
static void ch_output_type_union(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci);
static void ch_output_codefrag(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci);
static void ch_output_const_dcl(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci);
static void ch_prep(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci);
static void ch_type_alloc_and_tc(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci, gboolean do_alloc);

/*** helper function **/
static char *getNodeName(IDL_tree tname);

/*
  Output header file for C-language
*/
void
orbit_idl_output_c_headers (IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci)
{
  fprintf (ci->fh, OIDL_C_WARNING); // Top comment
  fprintf(ci->fh, "#ifndef %s%s_H\n", rinfo->header_guard_prefix, ci->c_base_name);
  fprintf(ci->fh, "#define %s%s_H 1\n", rinfo->header_guard_prefix, ci->c_base_name);

#if 0
  fprintf(ci->fh, "#define ORBIT_IDL_SERIAL %d\n", ORBIT_CONFIG_SERIAL);  // version ?
#endif
  fprintf(ci->fh, "#include <RtORB/corba.h>\n\n");


  if(rinfo->cpp_defs){
    /* for C++ */
    fprintf(ci->fh, "#ifdef __cplusplus\n");
    fprintf(ci->fh, "#include <RtORB/corba.hh>\n");
    fprintf(ci->fh, "extern \"C\" {\n");
    fprintf(ci->fh, "#endif /* __cplusplus */\n\n");
  }

  /* Do all the typedefs, etc. */
  fprintf(ci->fh, "\n/** typedefs **/\n");
  ch_output_types(tree, rinfo, ci);
  
  if ( ci->do_skel_defs ) {
    /* Do all the POA structures, etc. */
    fprintf(ci->fh, "\n/** POA structures **/\n");
    ch_output_poa(tree, rinfo, ci);

    fprintf(ci->fh, "\n/** skel prototypes **/\n");
    ch_output_skel_protos(tree, rinfo, ci);
  }

  fprintf(ci->fh, "\n/** stub prototypes **/\n");
  ch_output_stub_protos(tree, rinfo, ci);

  if ( ci->ext_dcls && ci->ext_dcls->str )
    fputs( ci->ext_dcls->str, ci->fh);	/* this may be huge! */

  if (rinfo->idata) { /* FIXME: hackish ? */
    ch_output_itypes(tree, ci);
  }


  if(rinfo->cpp_defs){
    fprintf(ci->fh, "\n /** for C++ **/\n");
    fprintf(ci->fh, "#ifdef __cplusplus\n");
    fprintf(ci->fh, "}\n");

    fprintf(ci->fh, "\n/** C++ class definition(skels)  **/\n");
    ch_output_cpp_skel_class_protos(tree, rinfo, ci);

    fprintf(ci->fh, "\n/** C++ class definition(stubs)  **/\n");
    ch_output_cpp_stub_class_protos(tree, rinfo, ci);

    fprintf(ci->fh, "\n/** C++ class definition(POA)  **/\n");
    ch_output_cpp_poa(tree, rinfo, ci, (const char*)0);

    fprintf(ci->fh, "\n/** C++ class definition(global fumctions)  **/\n");
    ch_output_cpp_global_functions(tree, rinfo, ci);

    fprintf(ci->fh, "#endif /* __cplusplus */\n\n");
  }

  fprintf(ci->fh, "#endif\n");

#if 0
  fprintf(ci->fh, "#undef ORBIT_IDL_SERIAL\n"); 
#endif
}

/*
  Output var definition
  Called by
     -ch_output_type_union
     -ch_output_type_struct
 */
static void
ch_output_var(IDL_tree val,
              IDL_tree name,
              OIDL_C_Info *ci)
{
  fprintf(ci->fh, "\n/* Var type definitions  */\n");
  orbit_cbe_write_typespec(ci->fh, val);

  fprintf(ci->fh, " ");
  switch(IDL_NODE_TYPE(name)) {
    case IDLN_IDENT:
      fprintf(ci->fh, "%s", IDL_IDENT(name).str);
      break;
    case IDLN_TYPE_ARRAY:
      {
        IDL_tree curitem;

        fprintf(ci->fh, "%s", IDL_IDENT(IDL_TYPE_ARRAY(name).ident).str);
        for(curitem = IDL_TYPE_ARRAY(name).size_list; curitem; curitem = IDL_LIST(curitem).next) {
          fprintf(ci->fh, "[%" IDL_LL "d]", (long long)IDL_INTEGER(IDL_LIST(curitem).data).value);
        }
      }
      break;
    default:
      g_error("Weird varname - %s", IDL_tree_type_names[IDL_NODE_TYPE(name)]);
      break;
    }
    fprintf(ci->fh, ";\n");
}


/*
  Output C_Types
    Called by:
      -orbit_idl_output_c_headers
      -itself
*/
static void
ch_output_types (IDL_tree       tree,
		 OIDL_Run_Info *rinfo,
		 OIDL_C_Info   *ci)
{
  if (!tree) { return; }

  switch (IDL_NODE_TYPE (tree)) 
  {
    case IDLN_EXCEPT_DCL: /* declare exceptions */
      {
        char *id;

        id = IDL_ns_ident_to_qstring ( IDL_IDENT_TO_NS(IDL_EXCEPT_DCL(tree).ident), "_", 0);

        fprintf (ci->fh, "#ifndef ex_%s\n", id);
        fprintf (ci->fh, "#define ex_%s \"%s\"\n", id, IDL_IDENT (IDL_EXCEPT_DCL (tree).ident).repo_id);

        g_free (id);

        ch_output_type_struct (tree, rinfo, ci, 0);
        fprintf (ci->fh, "#endif /* ex_%s */\n", id);
      }
      break;

    case IDLN_FORWARD_DCL: 
    case IDLN_INTERFACE:/* interface */
      ch_output_interface (tree, rinfo, ci);
      break;

    case IDLN_TYPE_STRUCT: /* struct */
      ch_output_type_struct (tree, rinfo, ci, 1);
      break;

    case IDLN_TYPE_ENUM: /* enum */
      ch_output_type_enum (tree, rinfo, ci);
      break;

    case IDLN_TYPE_DCL: /* declare type */
      ch_output_type_dcl (tree, rinfo, ci);
      break;

    case IDLN_TYPE_UNION: /* union */
      ch_output_type_union (tree, rinfo, ci);
      break;

    case IDLN_CODEFRAG: /* codefrag */
      ch_output_codefrag (tree, rinfo, ci);
      break;

    case IDLN_SRCFILE: /* include source file */
      {
        if (rinfo->onlytop) {
          char *idlfn = IDL_SRCFILE (tree).filename;

          if (!IDL_SRCFILE (tree).seenCnt &&
	      !IDL_SRCFILE(tree).isTop    &&
	      !IDL_SRCFILE(tree).wasInhibit) {
	    gchar *hfn, *htail;

	    hfn   = g_path_get_basename (idlfn);
	    htail = strrchr (hfn,'.');

	    g_assert (htail && strlen (htail) >= 2);

	    htail [1] = 'h';
	    htail [2] = 0;

	    fprintf (ci->fh, "#include \"%s\"\n", hfn);

	    g_free (hfn);
          }

          fprintf (ci->fh, "/* from IDL source file \"%s\" "
	                  "(seen %d, isTop %d, wasInhibit %d) */ \n", 
	                 idlfn,
	                 IDL_SRCFILE (tree).seenCnt,
	                 IDL_SRCFILE (tree).isTop,
	                 IDL_SRCFILE (tree).wasInhibit);
        }
      }
      break;
    case IDLN_CONST_DCL:  /* declare constant */
      ch_output_const_dcl (tree, rinfo, ci);
      break;

    case IDLN_NATIVE: /* native type ...*/
      ch_output_native (tree, rinfo, ci);
      break;

    default:
      break;
  }

  /***** recursive call ****/
  switch (IDL_NODE_TYPE (tree)) 
  {
    case IDLN_MODULE:  /** module */
      fprintf (ci->fh, "\n/* Module (Recursive) */\n");
      ch_output_types (IDL_MODULE (tree).definition_list, rinfo, ci);
      fprintf (ci->fh, "\n/* End of Module (Recursive) */\n");
      break;

    case IDLN_LIST: /* list */
      {
        IDL_tree sub;

        fprintf (ci->fh, "\n/* List (Recursive) */\n");
        for (sub = tree; sub; sub = IDL_LIST (sub).next)
        {
          ch_output_types (IDL_LIST (sub).data, rinfo, ci);
        }
        fprintf (ci->fh, "\n/* End of List (Recursive) */\n");
      }
      break;

    case IDLN_INTERFACE: /* interface */
      fprintf (ci->fh, "\n/* Interface (Recursive) */\n");
      ch_output_types (IDL_INTERFACE (tree).body, rinfo, ci);
      fprintf (ci->fh, "\n/* End of Interface (Recursive) */\n");
      break;

    default:
      break;
  }
}

/*
  Output an interface definitions
   Called by
    - ch_output_types
*/
static void
ch_output_interface(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci)
{
  char * cid = orbit_cbe_get_typespec_str(tree);

  fprintf(ci->fh, "/** Interface (%s)   */\n", cid);
  fprintf(ci->fh, "#if !defined(ORBIT_DECL_%s) && !defined(_%s_defined)\n", cid, cid);
  fprintf(ci->fh, "# define ORBIT_DECL_%s 1\n", cid);
  fprintf(ci->fh, "# define _%s_defined 1\n", cid);

  if ( tree->declspec & IDLF_DECLSPEC_PIDL ) {
    /* PIDL interfaces are not normal CORBA Objects */
    fprintf(ci->fh, "  typedef struct %s_type *%s;\n", cid, cid);

    fprintf(ci->fh, "#ifndef TC_%s\n", cid);
    fprintf(ci->fh, "#  define TC_%s TC_CORBA_Object\n", cid);
    fprintf(ci->fh, "#endif\n");
  } else {

    char *cppid = orbit_cbe_get_typespec_str2(tree, OIDL_TYPENAME_CPP_SIMPLE);
    char *modname = orbit_cbe_get_typespec_str2(tree, OIDL_TYPENAME_CPP_MODULE);
					     
//    fprintf(ci->fh, "#define %s__freekids CORBA_Object__freekids\n", cid);

    fprintf(ci->fh, "typedef CORBA_Object CORBA_%s;\n", cid);
    fprintf(ci->fh, "typedef CORBA_Object %s;\n", cid);

    
    if(rinfo->cpp_defs){ /** C++ **/
      fprintf(ci->fh, "\n#ifdef __cplusplus\n");
      fprintf(ci->fh, "namespace %s {;\n", modname);
      fprintf(ci->fh, "  class %s;\n", cppid);
      fprintf(ci->fh, "  typedef CORBA_interface::T_Ptr<%s> %s_ptr;\n", cppid, cppid);
      fprintf(ci->fh, "  typedef CORBA_interface::T_Var<%s> %s_var;\n", cppid, cppid);
      fprintf(ci->fh, "  typedef CORBA_interface::T_Helper<%s, %s_ptr> %s_helper;\n", cppid, cppid, cppid);
      fprintf(ci->fh, "  typedef CORBA_interface::CInArg<%s> %s_CInArg;\n", cppid,  cppid);
      fprintf(ci->fh, "  typedef %s_helper %s_COutArg;\n", cppid,  cppid);
      fprintf(ci->fh, "  typedef %s_helper %s_CInOutArg;\n", cppid,  cppid);
      fprintf(ci->fh, "  typedef %s_helper %s_CppInArg;\n", cppid,  cppid);
      fprintf(ci->fh, "  typedef %s_helper %s_CppOutArg;\n", cppid,  cppid);
      fprintf(ci->fh, "  typedef %s_helper %s_CppInOutArg;\n", cppid,  cppid);

      fprintf(ci->fh, "}\n#endif  /* __cplusplus */\n");
    }

    /* 080205 weak or static? */
#if GNU_WEAK
    fprintf(ci->fh, "extern CORBA_unsigned_long %s__classid __attribute__((weak));\n", cid);
#else
    fprintf(ci->fh, "extern CORBA_unsigned_long %s__classid;\n", cid);
#endif
    fprintf(ci->fh, "/*  TypeCode and alloc for %s  */\n", cid);
    ch_type_alloc_and_tc(tree, rinfo, ci, FALSE);

    g_free(cppid);
    g_free(modname);
  }

  fprintf(ci->fh, "#endif  /** Interface (%s)   */\n\n", cid);
  g_free(cid);
}

/*
  Output enum definitions
   Called by
    -ch_output_types
    -ch_output_type_union
    -ch_prep
*/
static void
ch_output_type_enum (IDL_tree       tree,
		     OIDL_Run_Info *rinfo,
		     OIDL_C_Info   *ci)
{
  IDL_tree  l;
  char     *enumid;

  /* CORBA spec says to do
   * typedef unsigned int enum_name;
   * and then #defines for each enumerator.
   * This works just as well and seems cleaner.
   */

  enumid = IDL_ns_ident_to_qstring (IDL_IDENT_TO_NS (IDL_TYPE_ENUM (tree).ident), "_", 0);

  fprintf(ci->fh, "/** Enumeration Type (%s)   */\n", enumid);
  fprintf (ci->fh, "#if !defined(_%s_defined)\n", enumid);
  fprintf (ci->fh, "#define _%s_defined 1\n", enumid);

  fprintf (ci->fh, "enum {\n");

  for (l = IDL_TYPE_ENUM (tree).enumerator_list; l; l = IDL_LIST (l).next) {
    char *id;
    id = IDL_ns_ident_to_qstring (IDL_IDENT_TO_NS (IDL_LIST (l).data), "_", 0);
    fprintf (ci->fh, "  %s%s\n", id, IDL_LIST (l).next ? "," : "");
    g_free (id);
  }

  fprintf (ci->fh, "} ;\n");
  fprintf (ci->fh, "typedef int %s;\n", enumid);

  fprintf(ci->fh, "/** TypeCode and alloc */\n");
  ch_type_alloc_and_tc (tree, rinfo, ci, FALSE);

  fprintf (ci->fh, "#endif\n");
  fprintf(ci->fh, "/** End of Enumeration Type (%s)   */\n\n", enumid);

  g_free (enumid);
}

/*
  Output declare type
  Called by
    -ch_output_types
*/
static void
ch_output_type_dcl(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci)
{
  IDL_tree  l;

  fprintf(ci->fh, "/** Declare Type  */\n");
  ch_prep (IDL_TYPE_DCL (tree).type_spec, rinfo, ci);

  for (l = IDL_TYPE_DCL (tree).dcls; l; l = IDL_LIST (l).next) 
  {
    char *ctmp = NULL;

    IDL_tree ent = IDL_LIST (l).data;

    /** setup identifier **/
    switch (IDL_NODE_TYPE(ent))
    {
      case IDLN_IDENT:
        ctmp = IDL_ns_ident_to_qstring ( IDL_IDENT_TO_NS (ent), "_", 0);
        break;
      case IDLN_TYPE_ARRAY:
        ctmp = IDL_ns_ident_to_qstring ( IDL_IDENT_TO_NS (IDL_TYPE_ARRAY (ent).ident), "_", 0);
        break;
      default:
        g_assert_not_reached ();
        break;
    }
 
    fprintf (ci->fh, "#if !defined(_%s_defined)\n", ctmp);
    fprintf (ci->fh, "#define _%s_defined 1\n", ctmp);

    fprintf (ci->fh, "typedef ");
    orbit_cbe_write_typespec (ci->fh, IDL_TYPE_DCL (tree).type_spec);
    fprintf (ci->fh, " %s;\n\n", ctmp);

    IDL_tree tts = orbit_cbe_get_typespec(ent);

    if(rinfo->cpp_defs)
    {
      char *cppid = orbit_cbe_get_typespec_str2( ent, OIDL_TYPENAME_CPP_SIMPLE);
      char *modname = orbit_cbe_get_typespec_str2( ent, OIDL_TYPENAME_CPP_MODULE);

      fprintf (ci->fh, "#ifdef __cplusplus\n");
      fprintf(ci->fh, "namespace %s {\n", modname);
		  
      if (IDL_NODE_TYPE(tts) == IDLN_TYPE_SEQUENCE) {
#if 0
	fprintf(ci->fh, "  typedef CORBA_sequence::T_Sequence<%s, %s::Elem, %s::Elem_ptr> %s;\n", ctmp, ctmp, ctmp, cppid);
#endif
	fprintf(ci->fh, "  typedef %s::CppSeq %s;\n", ctmp, cppid);
	fprintf(ci->fh, "  typedef CORBA_sequence::T_Helper<%s, %s> %s_helper;\n", ctmp, cppid, cppid);
	fprintf(ci->fh, "  typedef CORBA_sequence::T_CInArg<%s, %s> %s_CInArg;\n", ctmp, cppid, cppid);
	fprintf(ci->fh, "  typedef CORBA_sequence::T_COutArg<%s, %s> %s_COutArg;\n", ctmp, cppid, cppid);
	fprintf(ci->fh, "  typedef CORBA_sequence::T_CInOutArg<%s, %s> %s_CInOutArg;\n", ctmp, cppid, cppid);
	fprintf(ci->fh, "  typedef CORBA_sequence::T_CppInArg<%s, %s> %s_CppInArg;\n", ctmp, cppid, cppid);
	fprintf(ci->fh, "  typedef CORBA_sequence::T_CppOutArg<%s, %s> %s_CppOutArg;\n", ctmp, cppid, cppid);
	fprintf(ci->fh, "  typedef CORBA_sequence::T_CppInOutArg<%s, %s> %s_CppInOutArg;\n", ctmp, cppid, cppid);
	fprintf(ci->fh, "  typedef CORBA_sequence::T_Ptr<%s> %s_ptr;\n", cppid, cppid);
	fprintf(ci->fh, "  typedef CORBA_sequence::T_Var<%s> %s_var;\n", cppid, cppid);
	fprintf(ci->fh, "  typedef CORBA_sequence::T_SeqOut<%s> %s_out;\n", cppid, cppid);
      } else {
	char * typestr = orbit_cbe_get_typespec_str2 (IDL_TYPE_DCL (tree).type_spec, OIDL_TYPENAME_C);
	char * typestr_cpp = orbit_cbe_get_typespec_str2 (IDL_TYPE_DCL (tree).type_spec, OIDL_TYPENAME_CPP_FULL);
	fprintf(ci->fh, "  typedef %s %s;\n\n", typestr, cppid);
	fprintf(ci->fh, "  typedef %s %s_var;\n\n", typestr, cppid);
	fprintf(ci->fh, "  typedef %s_helper %s_helper;\n", typestr_cpp, cppid);
	fprintf(ci->fh, "  typedef %s_helper %s_CInArg;\n", typestr_cpp, cppid);
	fprintf(ci->fh, "  typedef %s_helper %s_COutArg;\n", typestr_cpp, cppid);
	fprintf(ci->fh, "  typedef %s_helper %s_CInOutArg;\n", typestr_cpp, cppid);
	fprintf(ci->fh, "  typedef %s_helper %s_CppInArg;\n", typestr_cpp, cppid);
	fprintf(ci->fh, "  typedef %s_helper %s_CppOutArg;\n", typestr_cpp, cppid);
	fprintf(ci->fh, "  typedef %s_helper %s_CppInOutArg;\n", typestr_cpp, cppid);
	g_free(typestr_cpp);
	g_free(typestr);
      }
      fprintf (ci->fh, "}\n#endif /* __cplusplus */\n");
      g_free(cppid);
      g_free(modname);
    }

    /* Probably I don't need follows, because I won't support array type */
    switch (IDL_NODE_TYPE (ent))
    {
      case IDLN_IDENT: /* */
/* comment out 2010/11/17
        fprintf (ci->fh, "#define %s_marshal(x,y,z) ", ctmp);
        orbit_cbe_write_typespec (ci->fh, IDL_TYPE_DCL (tree).type_spec);
        fprintf (ci->fh, "_marshal((x),(y),(z))\n");

        fprintf (ci->fh, "#define %s_demarshal(x,y,z,i) ", ctmp);
        orbit_cbe_write_typespec (ci->fh, IDL_TYPE_DCL (tree).type_spec);
        fprintf (ci->fh, "_demarshal((x),(y),(z),(i))\n");
*/
      break;

      case IDLN_TYPE_ARRAY: /** array type, but I need not support ? */
      {
        IDL_tree sub;

        for (sub = IDL_TYPE_ARRAY (ent).size_list; sub; sub = IDL_LIST (sub).next){
          fprintf (ci->fh, "[%" IDL_LL "d]", (long long)IDL_INTEGER (IDL_LIST (sub).data).value);
        }

        fprintf (ci->fh, ";\n");
        fprintf (ci->fh, "typedef ");

        orbit_cbe_write_typespec (ci->fh, IDL_TYPE_DCL (tree).type_spec);
        fprintf (ci->fh, " %s_slice", ctmp);

        for (sub = IDL_LIST (IDL_TYPE_ARRAY (ent).size_list).next; sub; sub = IDL_LIST (sub).next){
	  fprintf (ci->fh, "[%" IDL_LL "d]", (long long)IDL_INTEGER (IDL_LIST (sub).data).value);
        }
        fprintf(ci->fh, ";\n");
      }
      break;
    default:
      break;
    }

    
    fprintf(ci->fh, "/** TypeCode and allocator */\n");
    ch_type_alloc_and_tc (ent, rinfo, ci, TRUE);
    fprintf (ci->fh, "#endif\n");
    fprintf(ci->fh, "/** End of Declare Type  */\n\n");
    g_free (ctmp);
  }
}

/*
  Output native ?
  Called by
    -ch_output_types 
*/
static void
ch_output_native(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci)
{
  char *ctmp;
  IDL_tree id = IDL_NATIVE(tree).ident;
  ctmp = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(id), "_", 0);

  fprintf(ci->fh, "/** Native Type  (%s)*/\n", ctmp);
  fprintf(ci->fh, "#if !defined(_%s_defined)\n",  ctmp);
  fprintf(ci->fh, "#define _%s_defined 1\n", ctmp);
  fprintf(ci->fh, "typedef struct %s_type *%s;\n", ctmp, ctmp);

  /* Dont even think about emitting a typecode. */
  fprintf(ci->fh, "#endif\n");
  fprintf(ci->fh, "/** End of Native Type  */\n\n");
  g_free(ctmp);
}

/*
  Output cpp structure
  Called by
    -ch_output_type_struct
    -ch_output_type_union
*/
static void 
ch_output_cpp_type_struct(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci)
{
  if(rinfo->cpp_defs){
    char *modname = orbit_cbe_get_typespec_str2(IDL_TYPE_STRUCT(tree).ident, OIDL_TYPENAME_CPP_MODULE);
    char *id = orbit_cbe_get_typespec_str2(IDL_TYPE_STRUCT(tree).ident, OIDL_TYPENAME_CPP_SIMPLE);

    char *cid = orbit_cbe_get_typespec_str2(IDL_TYPE_STRUCT(tree).ident, OIDL_TYPENAME_C);

    fprintf(ci->fh, "#ifdef __cplusplus\n");
    fprintf(ci->fh, "namespace %s {\n", modname);
    fprintf(ci->fh, "  typedef %s %s;\n", cid, id);
    fprintf(ci->fh, "  typedef CORBA_struct::T_Helper<%s> %s_helper;\n", id, id);
    fprintf(ci->fh, "  typedef CORBA_struct::T_Arg<%s> %s_CInArg;\n", id, id);
    fprintf(ci->fh, "  typedef CORBA_struct::T_COutArg<%s> %s_COutArg;\n", id, id);
    fprintf(ci->fh, "  typedef CORBA_struct::T_Arg<%s> %s_CInOutArg;\n", id, id);
    fprintf(ci->fh, "  typedef CORBA_struct::T_Arg<%s> %s_CppInArg;\n", id, id);
    fprintf(ci->fh, "  typedef CORBA_struct::T_Arg<%s> %s_CppOutArg;\n", id, id);
    fprintf(ci->fh, "  typedef CORBA_struct::T_Arg<%s> %s_CppInOutArg;\n", id, id);
    fprintf(ci->fh, "  typedef CORBA_struct::T_Ptr<%s> %s_ptr;\n", id, id);
    fprintf(ci->fh, "  typedef CORBA_struct::T_Var<%s> %s_var;\n", id, id);

    if (orbit_cbe_type_is_fixed_length(tree)) {
      fprintf(ci->fh, "  typedef %s & %s_out;\n", id, id);
    } else {
      fprintf(ci->fh, "  typedef CORBA_struct::T_Out<%s> %s_out;\n", id, id);
    }
  
    fprintf(ci->fh, "};\n");
  
    fprintf(ci->fh, "#endif /* __cplusplus */\n");

    g_free(modname);
    g_free(id);
    g_free(cid);
  }  

}

/*
   Output structure declarations
   Called by
    -ch_output_types
    -ch_prep

*/
static void
ch_output_type_struct(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci, int defcpp)
{
  char *id;
  IDL_tree cur, curmem;

  id = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(IDL_TYPE_STRUCT(tree).ident), "_", 0);

  fprintf(ci->fh, "/*  Structure %s */\n", id);
  fprintf(ci->fh, "#if !defined(_%s_defined)\n", id);
  fprintf(ci->fh, "#define _%s_defined 1\n", id);

  /* define type code */
  fprintf(ci->fh, "/*  TypeCode and allocator %s */\n", id);
  ch_type_alloc_and_tc(tree, rinfo, ci, TRUE);


  /* put typedef out first for recursive seq */
  fprintf(ci->fh, "typedef struct %s_type %s;\n", id, id);

  /* Scan for any nested decls */
  for(cur = IDL_TYPE_STRUCT(tree).member_list; cur; cur = IDL_LIST(cur).next) {
    IDL_tree ts;
    ts = IDL_MEMBER(IDL_LIST(cur).data).type_spec;
    ch_prep(ts, rinfo, ci);
  }

  fprintf(ci->fh, "\nstruct %s_type {\n", id);

  for(cur = IDL_TYPE_STRUCT(tree).member_list; cur; cur = IDL_LIST(cur).next) {
    for(curmem = IDL_MEMBER(IDL_LIST(cur).data).dcls; curmem; curmem = IDL_LIST(curmem).next) {
      ch_output_var(IDL_MEMBER(IDL_LIST(cur).data).type_spec, IDL_LIST(curmem).data, ci);
    }
  }
  if(!IDL_TYPE_STRUCT(tree).member_list)
    fprintf(ci->fh, "int dummy;\n");

  if(rinfo->cpp_defs){
    fprintf(ci->fh, "#ifdef __cplusplus\n");
    fprintf(ci->fh, "  %s_type() {\n", id);
    fprintf(ci->fh, "    memset(this, 0, sizeof(struct %s_type)); \n", id);
   
    if(!IDL_TYPE_STRUCT(tree).member_list){
      fprintf(ci->fh, "    dummy=0;\n");
    }

    fprintf(ci->fh, "  }\n");
    fprintf(ci->fh, "  CORBA_TypeCode _type_code() { return TC_%s; }\n", id);
    fprintf(ci->fh, "  ~%s_type() { RtORB_free_by_typecode_cpp(TC_%s, this, 0);}\n", id, id);
    fprintf(ci->fh, "  void operator>>=(cdrStream &) const;\n");
    fprintf(ci->fh, "  void operator<<=(cdrStream &);\n");
    fprintf(ci->fh, "#endif /* __cplusplus */\n");
  }

  fprintf(ci->fh, "};\n\n");

  if (defcpp) {
    ch_output_cpp_type_struct(tree, rinfo, ci);
  }

  fprintf(ci->fh, "#endif\n");
  fprintf(ci->fh, "/*  End of Structure %s */\n", id);

  g_free(id);
}


/*
   Output union declarations
   Called by
    -ch_output_types
*/
static void
ch_output_type_union(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci)
{
  char *id;
  IDL_tree curitem;

  if (IDL_NODE_TYPE (IDL_TYPE_UNION (tree).switch_type_spec) == IDLN_TYPE_ENUM){
    ch_output_type_enum (IDL_TYPE_UNION (tree).switch_type_spec, rinfo, ci);
  }
  id = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(IDL_TYPE_UNION(tree).ident), "_", 0);

  fprintf(ci->fh, "/*  Union %s */\n", id);
  fprintf(ci->fh, "#if !defined(_%s_defined)\n", id);
  fprintf(ci->fh, "#define _%s_defined 1\n", id);

  fprintf(ci->fh, "/*  TypeCode and allocator %s */\n", id);
  ch_type_alloc_and_tc(tree, rinfo, ci, TRUE);

  fprintf(ci->fh, "typedef struct %s_type %s;\n", id, id);

  /* Scan for any nested decls */
  for(curitem = IDL_TYPE_UNION(tree).switch_body; curitem; curitem = IDL_LIST(curitem).next) {
    IDL_tree member = IDL_CASE_STMT(IDL_LIST(curitem).data).element_spec;
    ch_prep(IDL_MEMBER(member).type_spec, rinfo, ci);
  }

  fprintf(ci->fh, "\nstruct %s_type {\n", id);
  
  orbit_cbe_write_typespec(ci->fh, IDL_TYPE_UNION(tree).switch_type_spec);
  fprintf(ci->fh, "  __d;\n");
  fprintf(ci->fh, "  struct {\n");

  for(curitem = IDL_TYPE_UNION(tree).switch_body; curitem; curitem = IDL_LIST(curitem).next) {
    IDL_tree member;
 
    member = IDL_CASE_STMT(IDL_LIST(curitem).data).element_spec;
    fprintf(ci->fh, "  ");
    ch_output_var(IDL_MEMBER(member).type_spec,
		  IDL_LIST(IDL_MEMBER(member).dcls).data,
		  ci);
  }

  fprintf(ci->fh, "  } _u;\n");

  if(rinfo->cpp_defs){ /** C++ **/
    fprintf(ci->fh, "#ifdef __cplusplus\n");
    fprintf(ci->fh, "  %s_type() : __d(0) {}\n", id);
    fprintf(ci->fh, "  %s_type(const %s_type &o) {\n", id, id);
    fprintf(ci->fh, "    __d = o.__d;\n");

    for(curitem = IDL_TYPE_UNION(tree).switch_body; curitem; curitem = IDL_LIST(curitem).next) {
      IDL_tree member = IDL_CASE_STMT(IDL_LIST(curitem).data).element_spec;
      IDL_tree tname = IDL_LIST(IDL_MEMBER(member).dcls).data;

      char *name = getNodeName(tname);
      if (name) {
        fprintf(ci->fh, "    _u.%s = o._u.%s;\n", name, name);
      }

    }
    fprintf(ci->fh, "  }\n");

    fprintf(ci->fh, "  CORBA_short _d() const { return __d; };\n");

    int cnt = 1;
    for(curitem = IDL_TYPE_UNION(tree).switch_body; curitem; curitem = IDL_LIST(curitem).next) {

      IDL_tree member = IDL_CASE_STMT(IDL_LIST(curitem).data).element_spec;
      IDL_tree ts = IDL_MEMBER(member).type_spec;
      char *type = orbit_cbe_get_typespec_str2(ts, OIDL_TYPENAME_CPP_FULL);
      char *rettype = orbit_cbe_write_param_typespec_str3(ts, DATA_RETURN, OIDL_TYPENAME_CPP_FULL);

      IDL_tree tname = IDL_LIST(IDL_MEMBER(member).dcls).data;

      char *name = getNodeName(tname);

      if (name) {
#if 0
        fprintf(ci->fh, "%s %s() const { return _u.%s; }\n", rettype, name, name);
        fprintf(ci->fh, "void %s(%s %s_) { __d = %d; _u.%s = %s_; }\n", name, type, name, cnt, name, name);
#endif
        fprintf(ci->fh, "  %s %s() const;\n", rettype, name);
        fprintf(ci->fh, "  void %s(%s %s_);", name, type, name);
        cnt++;
      }

      g_free(rettype);
      g_free(type);
    
    }
    fprintf(ci->fh, "  CORBA_TypeCode _type_code() { return TC_%s; }\n", id);
  
    fprintf(ci->fh, "  inline void operator>>=(cdrStream &s) const{ /* marshal union */\n");
    fprintf(ci->fh, "    __d >>= s;\n");
    fprintf(ci->fh, "    switch(__d){\n");
    int xx=0;
    for(curitem = IDL_TYPE_UNION(tree).switch_body; curitem; curitem = IDL_LIST(curitem).next) {
      IDL_tree member = IDL_CASE_STMT(IDL_LIST(curitem).data).element_spec;
      IDL_tree tname = IDL_LIST(IDL_MEMBER(member).dcls).data;

      char *name = getNodeName(tname);

      if (name) {
        fprintf(ci->fh, "      case %d:\n", xx);
        fprintf(ci->fh, "        _u.%s >>= s;\n", name);
        fprintf(ci->fh, "        break;\n");
        xx++;
      }
    }
    fprintf(ci->fh, "    }\n");
    fprintf(ci->fh, "  }\n");
    fprintf(ci->fh, "  inline void operator<<=(cdrStream &s){ /* cdr for union */ \n");
    fprintf(ci->fh, "    __d <<= s;\n");
    fprintf(ci->fh, "    switch(__d){\n");
    xx=0;
    for(curitem = IDL_TYPE_UNION(tree).switch_body; curitem; curitem = IDL_LIST(curitem).next) {
      IDL_tree member = IDL_CASE_STMT(IDL_LIST(curitem).data).element_spec;
      IDL_tree tname = IDL_LIST(IDL_MEMBER(member).dcls).data;

      char *name = 0;
      switch(IDL_NODE_TYPE(tname))
      {
        case IDLN_IDENT:
          name = IDL_IDENT(tname).str;
          break;
        default:
          break;
      }
      if (name) {
        fprintf(ci->fh, "      case %d:\n", xx);
        fprintf(ci->fh, "        _u.%s <<= s;\n", name);
        fprintf(ci->fh, "        break;\n");
        xx++;
      }
    }
    fprintf(ci->fh, "    }\n");
    fprintf(ci->fh, "  }\n");

    fprintf(ci->fh, "#endif /* __cplusplus */\n");
  }

  fprintf(ci->fh, "};\n\n");

  if(rinfo->cpp_defs){ /** C++ **/
    ch_output_cpp_type_struct(tree, rinfo, ci);
  }

  fprintf(ci->fh, "#endif\n");
  fprintf(ci->fh, "/* End of Union %s */\n\n", id);

  g_free(id);
}

/*
   Output codeflag ?
   Called by
    -ch_output_types
*/
static void
ch_output_codefrag(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci)
{
  GSList *list;

  fprintf(ci->fh, "/*** Codeflag ****/\n");

  for(list = IDL_CODEFRAG(tree).lines; list; list = g_slist_next(list)) {
    if(!strncmp(list->data, "#pragma include_defs", sizeof("#pragma include_defs")-1)) {
      char *ctmp, *cte;
      ctmp = ((char *)list->data) + sizeof("#pragma include_defs");
      while(*ctmp && (isspace((int)*ctmp) || *ctmp == '"')) ctmp++;
      cte = ctmp;
      while(*cte && !isspace((int)*cte) && *cte != '"') cte++;
      *cte = '\0';
      fprintf(ci->fh, "#include <%s>\n", ctmp);
    } else{
      fprintf(ci->fh, "%s\n", (char *)list->data);
    }
  }
  fprintf(ci->fh, "/*** End of Codeflag ****/\n");
}

/*
   Output constant declarations
   Called by
    -ch_output_types
*/
static void
ch_output_const_dcl(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci)
{
  char    *id;
  IDL_tree ident;
  IDL_tree typespec;

  ident = IDL_CONST_DCL (tree).ident;
  id = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS (ident), "_", 0);

  fprintf(ci->fh, "/*   Constant  %s  */\n", id);

  fprintf(ci->fh, "#ifndef %s\n", id);
  fprintf(ci->fh, "#define %s ", id);

  orbit_cbe_write_const(ci->fh, IDL_CONST_DCL(tree).const_exp);

  typespec = orbit_cbe_get_typespec (IDL_CONST_DCL(tree).const_type);
  if (IDL_NODE_TYPE (typespec) == IDLN_TYPE_INTEGER &&
      !IDL_TYPE_INTEGER (typespec).f_signed)
    fprintf(ci->fh, "U");

  fprintf(ci->fh, "\n");
  fprintf(ci->fh, "#endif /* !%s */\n\n", id);
  fprintf(ci->fh, "/* End of Constant  %s  */\n", id);

  g_free(id);
}

/*
   Output fix declarations, but I don't need it
   Called by
    -ch_output_types
*/
static void
ch_prep_fixed(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci)
{
  char *ctmp;

  ctmp = orbit_cbe_get_typespec_str(tree);
  fprintf(ci->fh, "/* Fixed  %s  */\n", ctmp);
  fprintf(ci->fh,
    "typedef struct {\n  CORBA_unsigned_short _digits;\n  CORBA_short _scale;\n  CORBA_char _value[%d];\n} %s;\n",
	  (int) (IDL_INTEGER(IDL_TYPE_FIXED(tree).positive_int_const).value + 2)/2, ctmp);
  g_free(ctmp);

  fprintf(ci->fh, "/* TypeCode and allocator: %s  */\n", ctmp);
  ch_type_alloc_and_tc(tree, rinfo, ci, TRUE);
  fprintf(ci->fh, "/* End of Fixed  %s  */\n", ctmp);
}

/*
   Output Prep Sequence 
   Called by
    -ch_prep
    -ch_prep_sequence
*/
static void
ch_prep_sequence(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci)
{
  char *ctmp, *fullname, *fullname_def, *ctmp2;
  IDL_tree tts;
  gboolean separate_defs, fake_if;
  IDL_tree fake_seq = NULL;

  tts = orbit_cbe_get_typespec(IDL_TYPE_SEQUENCE(tree).simple_type_spec);
  ctmp = orbit_cbe_get_typespec_str(IDL_TYPE_SEQUENCE(tree).simple_type_spec);
  ctmp2 = orbit_cbe_get_typespec_str(tts);
  fake_if = (IDL_NODE_TYPE(tts) == IDLN_INTERFACE);

#if 0
  if(fake_if)
    {
      g_free(ctmp2);
      ctmp2 = g_strdup("CORBA_Object");
    }
#endif

  separate_defs = strcmp(ctmp, ctmp2);
  fullname = orbit_cbe_get_typespec_str(tree);

  if(separate_defs)
  {
    if(fake_if){
      tts = IDL_type_object_new();
    }
    fake_seq = IDL_type_sequence_new(tts, NULL);
    IDL_NODE_UP(fake_seq) = IDL_NODE_UP(tree);
    ch_prep_sequence(fake_seq, rinfo, ci);

    fullname_def = g_strdup_printf("CORBA_sequence_%s", ctmp2);

    if(!fake_if){
      IDL_TYPE_SEQUENCE(fake_seq).simple_type_spec = NULL;
    }
  } else {
    fullname_def = g_strdup(fullname);
  }

  if(IDL_NODE_TYPE(IDL_TYPE_SEQUENCE(tree).simple_type_spec) == IDLN_TYPE_SEQUENCE){
    ch_prep_sequence(IDL_TYPE_SEQUENCE(tree).simple_type_spec, rinfo, ci);
  }

  /* NOTE: ORBIT_DECL_%s protects redef of everything (struct,TC,externs)
   * while _%s_defined protects only the struct */

  fprintf(ci->fh, "#if !defined(ORBIT_DECL_%s)\n", fullname);
  fprintf(ci->fh, "#define ORBIT_DECL_%s 1\n", fullname);


  if(separate_defs)
  {
    fprintf(ci->fh, "#if !defined(_%s_defined)\n", fullname);
    fprintf(ci->fh, "#define _%s_defined 1\n", fullname);

    if(!strcmp(ctmp, "CORBA_RepositoryId")){
      fprintf(ci->fh, "/* CRACKHEADS */\n");
    }
    fprintf(ci->fh, "typedef %s %s;\n", fullname_def, fullname);

    if(rinfo->cpp_defs){
      fprintf(ci->fh, "#ifdef __cplusplus\n");
      fprintf(ci->fh, "  typedef CORBA_var<%s> %s_var;\n", fullname_def, fullname);
      fprintf(ci->fh, "#endif /* __cplusplus */\n");
    }

    fprintf(ci->fh, "#endif\n");

    fprintf(ci->fh, "/* Typecode and allocator (%s)*/\n", fullname);
    ch_type_alloc_and_tc(tree, rinfo, ci, FALSE);

    fprintf(ci->fh, "#define %s__alloc %s__alloc\n", fullname, fullname_def);

    fprintf(ci->fh, "#define CORBA_sequence_%s_allocbuf CORBA_sequence_%s_allocbuf\n", ctmp, ctmp2);

    IDL_tree_free(fake_seq);

  } else {

    IDL_tree elemtype;
    char *tc, *member_type;
    char * typespec_str;

    fprintf(ci->fh, "#if !defined(_%s_defined)\n", fullname);
    fprintf(ci->fh, "#define _%s_defined 1\n", fullname);

    fprintf(ci->fh, "/* Typecode and allocator (%s)*/\n", fullname);
    ch_type_alloc_and_tc(tree, rinfo, ci, TRUE);

    typespec_str = orbit_cbe_get_typespec_str(tree);
    /* for _var_type declaration */

    fprintf(ci->fh, "typedef struct %s_type {\n", typespec_str);
      fprintf(ci->fh, "  CORBA_unsigned_long _maximum;\n");
      fprintf(ci->fh, "  CORBA_unsigned_long _length;\n  ");
      elemtype = orbit_cbe_get_typespec(IDL_TYPE_SEQUENCE(tree).simple_type_spec);
      orbit_cbe_write_typespec(ci->fh, elemtype);
      fprintf(ci->fh, "* _buffer;\n");
      fprintf(ci->fh, "  CORBA_unsigned_long _release;\n\n");


    if(rinfo->cpp_defs){
      fprintf(ci->fh, "#ifdef __cplusplus\n");

      fprintf(ci->fh, "  %s_type () {\n", typespec_str);
      fprintf(ci->fh, "    _length = 0;\n");
      fprintf(ci->fh, "    _maximum = 0;\n");
      fprintf(ci->fh, "    _buffer = NULL;\n");
      fprintf(ci->fh, "    _release = 0;\n");
      fprintf(ci->fh, "  }\n");
      //fprintf(ci->fh, "  %s_type (const %s_type &o) : _length(0), _maximum(0) , _buffer(NULL), _release(0) { CppSeq::copy(o, *this); }\n", typespec_str, typespec_str);
      fprintf(ci->fh, "  %s_type (const %s_type &o) {\n", typespec_str,typespec_str);
      fprintf(ci->fh, "    _length = 0;\n");
      fprintf(ci->fh, "    _maximum = 0;\n");
      fprintf(ci->fh, "    _buffer = NULL;\n");
      fprintf(ci->fh, "    _release = 0;\n");
      fprintf(ci->fh, "    CppSeq::copy(o, *this); }\n");

      fprintf(ci->fh, "  ~%s_type () {\n", typespec_str);
      fprintf(ci->fh, "    if (_buffer) { RtORB_free_by_typecode_cpp(_type_code(),this,0); }\n");

      fprintf(ci->fh, "  }\n");
      
      switch(IDL_NODE_TYPE(elemtype))
      {
        case IDLN_INTERFACE:
          {
	    char *elem_cpp = orbit_cbe_get_typespec_str2(elemtype, OIDL_TYPENAME_CPP_FULL);
	    fprintf(ci->fh, "  typedef CORBA_Object Elem;\n");
	    fprintf(ci->fh, "  typedef CORBA_interface::T_SeqElemPtr<%s> ElemPtr;\n", elem_cpp);
	    fprintf(ci->fh, "  typedef ElemPtr ElemRef;\n");
	    fprintf(ci->fh, "  typedef CORBA_sequence::T_ElemCOHelper ElemHelper;\n");
	    fprintf(ci->fh, "  typedef CORBA_sequence::T_PtrSeq<%s_type, Elem, ElemPtr, ElemHelper> CppSeq;\n", typespec_str);
	    g_free(elem_cpp);
	  }
	  break;
        case IDLN_TYPE_STRUCT:
        default:
          {
	    char *elem_cpp = orbit_cbe_get_typespec_str2(elemtype, OIDL_TYPENAME_CPP_FULL);
	    char *elem_ret = orbit_cbe_write_param_typespec_str3(elemtype, DATA_RETURN, OIDL_TYPENAME_CPP_FULL);
	    fprintf(ci->fh, "  typedef %s Elem;\n", elem_cpp);
	    fprintf(ci->fh, "  typedef Elem & ElemRef;\n");
	    fprintf(ci->fh, "  typedef CORBA_sequence::T_ElemRefHelper<Elem> ElemHelper;\n");
	    fprintf(ci->fh, "  typedef CORBA_sequence::T_RefSeq<%s_type, Elem, ElemHelper> CppSeq;\n", typespec_str);

            g_free(elem_cpp);
            g_free(elem_ret);
	}
	break;
      }

      fprintf(ci->fh, "  ElemRef operator[](CORBA::ULong n);\n");
      fprintf(ci->fh, "  CppSeq::ElemRef operator[](CORBA::ULong n) const;\n");

      fprintf(ci->fh, "  CORBA::ULong length() const { return _length; }\n");
      fprintf(ci->fh, "  CORBA::ULong length(CORBA::ULong n);\n");

      fprintf(ci->fh, "  %s_type & operator=(const CppSeq &o);\n", typespec_str);
      fprintf(ci->fh, "  %s_type & operator=(const %s_type &o);\n", typespec_str, typespec_str);

      fprintf(ci->fh, " static CORBA_TypeCode_struct * _type_code() { return const_cast<CORBA_TypeCode_struct*>(&TC_%s_struct); };\n", typespec_str);

      fprintf(ci->fh, "  void operator>>=(cdrStream &s) const{\n");
      fprintf(ci->fh, "    s.marshal_sequence(_buffer, _length, _type_code());\n");
      fprintf(ci->fh, "  }\n");
      fprintf(ci->fh, "  void operator<<=(cdrStream &s){\n");
      fprintf(ci->fh, "    if(this->_length > 0){\n");
      fprintf(ci->fh, "      RtORB_free_by_typecode_cpp(_type_code(), this, 0);\n");
      fprintf(ci->fh, "    }\n ");
      fprintf(ci->fh, "    if(this->_buffer){\n");
      fprintf(ci->fh, "      RtORB_free(this->_buffer, \" CORBA_sequence: operator <<=\");\n");
      fprintf(ci->fh, "    }\n ");
      fprintf(ci->fh, "    _length <<= s;\n");
      fprintf(ci->fh, "    _buffer = ");
      fprintf(ci->fh, "(");
      elemtype = orbit_cbe_get_typespec(IDL_TYPE_SEQUENCE(tree).simple_type_spec);
      orbit_cbe_write_typespec(ci->fh, elemtype);
      fprintf(ci->fh, " *)");
      fprintf(ci->fh, "s.unmarshal_sequence(_length, _type_code());\n");
      fprintf(ci->fh, "    _maximum = _length;\n");
      fprintf(ci->fh, "    _release = 0;\n");
      fprintf(ci->fh, "  }\n");


      fprintf(ci->fh, "#endif /* __cplusplus */\n");
    }

    fprintf(ci->fh, "}");

    orbit_cbe_write_typespec(ci->fh, tree);
    fprintf(ci->fh, ";\n");

    fprintf(ci->fh, "#endif\n\n");

    tc = orbit_cbe_get_typecode_name (orbit_cbe_get_typespec (tree));
    member_type = orbit_cbe_type_is_builtin (IDL_TYPE_SEQUENCE (tree).simple_type_spec) ?
    ctmp + strlen ("CORBA_") : ctmp;

#if 0
      fprintf (ci->fh, "#define CORBA_sequence_%s_allocbuf(l) "
	       "((%s*)ORBit_small_allocbuf (%s, (l)))\n",
	       member_type, member_type, tc);
#else
#if 0 
	fprintf (ci->fh, "#define CORBA_sequence_%s_allocbuf(l) "
	"((%s*)RtORB_sequence_allocbuf (%s, (l)))\n",
	member_type, member_type, tc);
#endif
#endif

      g_free (tc);
    }

  fprintf(ci->fh, "#endif\n");

  g_free(ctmp2);
  g_free(ctmp);
  g_free(fullname);
  g_free(fullname_def);
}

/*
  Preprocessor definition for each type declarations
  Call by
    -ch_output_type_dcl
    -ch_output_type_struct
    -ch_output_type_union
*/
static
void ch_prep (IDL_tree       tree,
	      OIDL_Run_Info *rinfo,
	      OIDL_C_Info   *ci)
{
  switch (IDL_NODE_TYPE (tree))
  {
    case IDLN_TYPE_SEQUENCE:
      ch_prep_sequence (tree, rinfo, ci);
      break;
    case IDLN_TYPE_FIXED:
      ch_prep_fixed (tree, rinfo, ci);
      break;
    case IDLN_TYPE_STRUCT:
      ch_output_type_struct (tree, rinfo, ci, 1);
      break;
    case IDLN_TYPE_ENUM:
      ch_output_type_enum (tree, rinfo, ci);
      break;
    default:
      break;
  }
}

/*
   Output Typecode and allocators

*/
static void
ch_type_alloc_and_tc(IDL_tree tree, OIDL_Run_Info *rinfo,
		     OIDL_C_Info *ci, gboolean do_alloc)
{
  char *ctmp;
  IDL_tree tts;

  ctmp = orbit_cbe_get_typespec_str(tree);

  if ( ci->do_impl_hack ) {
#if 0
    fprintf(ci->fh, "#if !defined(TC_IMPL_TC_%s_0)\n", ctmp);
    orbit_cbe_id_define_hack(ci->fh, "TC_IMPL_TC", ctmp, ci->c_base_name);
#else
    // delete 2010/11/18
    fprintf(ci->fh, "#if !defined(TC_IMPL_TC_%s)\n", ctmp);
    fprintf(ci->fh, "#define  TC_IMPL_TC_%s	1\n", ctmp);
#endif
  }

#if 0
  fprintf (ci->fh, "#ifdef ORBIT_IDL_C_IMODULE_%s\n", ci->c_base_name);
  fprintf (ci->fh, "  static\n");
  fprintf (ci->fh, "#else\n");
  fprintf (ci->fh, "  extern\n");
  fprintf (ci->fh, "#endif\n");
#else
  fprintf (ci->fh, "  extern ");
#endif

  fprintf (ci->fh, "struct CORBA_TypeCode_struct TC_%s_struct;\n", ctmp);
  fprintf (ci->fh, "#define TC_%s ((CORBA_TypeCode)&TC_%s_struct)\n", ctmp, ctmp);

  if (ci->do_impl_hack){
    fprintf (ci->fh, "#endif\n");
  }

  if(do_alloc) {
    char *tc;

    tts = orbit_cbe_get_typespec(tree);
    tc = orbit_cbe_get_typecode_name (tts);

#if 0
    fprintf (ci->fh, "#define %s__alloc() ((%s%s *)ORBit_small_alloc (%s))\n",
	     ctmp, ctmp, (IDL_NODE_TYPE(tree) == IDLN_TYPE_ARRAY)?"_slice":"", tc);

    fprintf (ci->fh, "#define %s__freekids(m,d) ORBit_small_freekids (%s,(m),(d))\n", ctmp, tc);
#else
    fprintf (ci->fh, "#define %s__alloc() ((%s%s *)RtORB_typecode_alloc (%s))\n",
	     ctmp, ctmp, (IDL_NODE_TYPE(tree) == IDLN_TYPE_ARRAY)?"_slice":"", tc);

//    fprintf (ci->fh, "#define %s__freekids(m,d) RtORB__freekids (%s,(m),(d))\n", ctmp, tc);
#endif

    if ( IDL_NODE_TYPE(tts) == IDLN_TYPE_SEQUENCE )
    {
      char *member_type = orbit_cbe_get_typespec_str(IDL_TYPE_SEQUENCE(tts).simple_type_spec);
      char *member_name = orbit_cbe_type_is_builtin (IDL_TYPE_SEQUENCE (tts).simple_type_spec) ?
      member_type + strlen ("CORBA_") : member_type;
	
#if 0
      fprintf (ci->fh, "#define %s_allocbuf(l) "
		 "((%s*)ORBit_small_allocbuf (%s, (l)))\n",
		 ctmp, member_name, tc);
#else
      fprintf (ci->fh, "#define %s_allocbuf(l) ((%s*)RtORB__allocbuf (%s, (l)))\n",
		 ctmp, member_name, tc);
#endif
      g_free (member_type);
    }
    g_free (tc);
  }
  g_free(ctmp);
}

/************************/
/*
  Output vepr declaration of interface 
  Called by
    -ch_output_poa
*/
static void
cbe_header_interface_print_vepv(IDL_tree node, FILE *of)
{
  char *id = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(IDL_INTERFACE(node).ident),
				     "_", 0);
  fprintf(of, "  POA_%s__epv *%s_epv;\n", id, id);
  g_free(id);

}

/*
   Output POA
   Called by
     -orbit_idl_output_c_headers
     -ch_output_poa
*/
static void
ch_output_poa(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci)
{
  if(!tree){ return; }

  if ( tree->declspec & IDLF_DECLSPEC_PIDL ){ return; }

  switch(IDL_NODE_TYPE(tree))
  {
    case IDLN_MODULE:
      ch_output_poa(IDL_MODULE(tree).definition_list, rinfo, ci);
      break;
    case IDLN_LIST:
      {
        IDL_tree sub;

        for(sub = tree; sub; sub = IDL_LIST(sub).next) {
          ch_output_poa(IDL_LIST(sub).data, rinfo, ci);
        }
      }
      break;
    case IDLN_INTERFACE:
      {
        IDL_tree sub;
        char *id;

        id = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(IDL_INTERFACE(tree).ident), "_", 0);
        fprintf(ci->fh, "#ifndef _defined_POA_%s\n", id);
        fprintf(ci->fh, "#define _defined_POA_%s 1\n", id);

        /* First, do epv for this interface, then vepv, then servant */
        fprintf(ci->fh, "typedef struct {\n");
        fprintf(ci->fh, "  void *_private;\n");
        for(sub = IDL_INTERFACE(tree).body; sub; sub = IDL_LIST(sub).next) {
          IDL_tree cur;
	  cur = IDL_LIST(sub).data;

	  switch(IDL_NODE_TYPE(cur)) 
          {
	    case IDLN_OP_DCL:
              fprintf(ci->fh, "  ");
	      orbit_cbe_op_write_proto(ci->fh, cur, "", TRUE);
	      fprintf(ci->fh, ";\n");
	      break;
	    case IDLN_ATTR_DCL:
	      {
	        OIDL_Attr_Info *ai;
	        IDL_tree curitem;

                for(curitem = IDL_ATTR_DCL(cur).simple_declarations; curitem; curitem = IDL_LIST(curitem).next) {
	          ai = IDL_LIST(curitem).data->data;
	      
                  fprintf(ci->fh, "  ");
	          orbit_cbe_op_write_proto(ci->fh, ai->op1, "", TRUE);
	          fprintf(ci->fh, ";\n");
	      
	          if(ai->op2) {
                    fprintf(ci->fh, "  ");
		    orbit_cbe_op_write_proto(ci->fh, ai->op2, "", TRUE);
		    fprintf(ci->fh, ";\n");
	          }
	       }
	    }
	    break;
	  default:
	    break;
	}
      }
      fprintf(ci->fh, "} POA_%s__epv;\n\n", id);

      fprintf(ci->fh, "typedef struct {\n");
      fprintf(ci->fh, "  PortableServer_ServantBase__epv *_base_epv;\n");
      IDL_tree_traverse_parents(tree, (GFunc)cbe_header_interface_print_vepv, ci->fh);
      fprintf(ci->fh, "} POA_%s__vepv;\n\n", id);

      fprintf(ci->fh, "typedef struct {\n");
      fprintf(ci->fh, "  void *_private;\n");
      fprintf(ci->fh, "  POA_%s__vepv *vepv;\n", id);
      fprintf(ci->fh, "} POA_%s;\n\n", id);

      /* 080205 weak or static? */
      fprintf(ci->fh,
	      "extern CORBA_%s impl_%s__create(PortableServer_POA servant, CORBA_Environment *ev);\n", id, id);
      fprintf(ci->fh, "extern impl_POA_ServantBase * impl_%s__create_servant(PortableServer_POA servant, CORBA_Environment *ev);\n", id);
#if GNU_WEAK
      fprintf(ci->fh,
	      "extern void POA_%s__init(PortableServer_Servant servant, CORBA_Environment *ev) __attribute__((weak)) ;\n", id);
      fprintf(ci->fh,
	      "extern void POA_%s__fini(PortableServer_Servant servant, CORBA_Environment *ev) __attribute__((weak)) ;\n", id);
#else
      fprintf(ci->fh, "extern void POA_%s__init(PortableServer_Servant servant, CORBA_Environment *ev);\n", id);
      fprintf(ci->fh, "extern void POA_%s__fini(PortableServer_Servant servant, CORBA_Environment *ev);\n", id);
#endif

      fprintf(ci->fh, "#endif /* _defined_POA_%s */\n\n", id);

      g_free(id);
    }
    break;
  default:
    break;
  }
}

/************************/

typedef struct {
  FILE *of;
  IDL_tree realif;
} InheritedOutputInfo;

static void ch_output_inherited_protos(IDL_tree curif, InheritedOutputInfo *ioi);

/*
  Output prototypes for stub
  Called by
    -orbit_idl_output_c_headers
    -ch_output_stub_protos
*/
static void
ch_output_stub_protos(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci)
{
  if(!tree) return;

  switch(IDL_NODE_TYPE(tree))
  {
    case IDLN_MODULE:
      fprintf(ci->fh, "/*   Module  */\n");
      ch_output_stub_protos(IDL_MODULE(tree).definition_list, rinfo, ci);
      fprintf(ci->fh, "/*   End of Module  */\n\n");
      break;
    case IDLN_LIST:
      {
        IDL_tree sub;

        fprintf(ci->fh, "/*   List  */\n");
        for(sub = tree; sub; sub = IDL_LIST(sub).next) {
          ch_output_stub_protos(IDL_LIST(sub).data, rinfo, ci);
        }
        fprintf(ci->fh, "/*   End of List  */\n\n");
      }
      break;
    case IDLN_INTERFACE:
      {
        IDL_tree sub;

        fprintf(ci->fh, "/*   Interface  */\n");
        if(IDL_INTERFACE(tree).inheritance_spec) {
          InheritedOutputInfo ioi;
	  ioi.of = ci->fh;
	  ioi.realif = tree;
	  IDL_tree_traverse_parents(IDL_INTERFACE(tree).inheritance_spec, (GFunc)ch_output_inherited_protos, &ioi);
        }

        for(sub = IDL_INTERFACE(tree).body; sub; sub = IDL_LIST(sub).next) {
          IDL_tree cur;

	  cur = IDL_LIST(sub).data;

          switch(IDL_NODE_TYPE(cur))
          {
	     case IDLN_OP_DCL:
	       orbit_idl_check_oneway_op (cur); /* RtORB doesn't support 'oneway'*/
	       orbit_cbe_op_write_proto(ci->fh, cur, "", FALSE);
#if GNU_WEAK
	       fprintf(ci->fh, " __attribute__((weak));\n");
#else
	       fprintf(ci->fh, " ;\n");
#endif
	       break;
             case IDLN_ATTR_DCL:
	      {
	        OIDL_Attr_Info *ai;
	        IDL_tree curitem;

	        for(curitem = IDL_ATTR_DCL(cur).simple_declarations; curitem; curitem = IDL_LIST(curitem).next) {
	          ai = IDL_LIST(curitem).data->data;
	      
	          orbit_cbe_op_write_proto(ci->fh, ai->op1, "", FALSE);
	          fprintf(ci->fh, ";\n");
	      
	          if(ai->op2) {
		    orbit_cbe_op_write_proto(ci->fh, ai->op2, "", FALSE);
		    fprintf(ci->fh, ";\n");
	          }
	        }
	      }
	      break;
	    default:
	      break;
	}
      }
    }
    break;
  default:
    break;
  }
}

/*
   Output inherited prototypes ?
   Called by
     -ch_output_stub_protos
*/
static void
ch_output_inherited_protos(IDL_tree curif, InheritedOutputInfo *ioi)
{
  char *id, *realid;
  IDL_tree curitem;

  if(curif == ioi->realif)
    return;

  realid = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(IDL_INTERFACE(ioi->realif).ident), "_", 0);
  id = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(IDL_INTERFACE(curif).ident), "_", 0);

  for(curitem = IDL_INTERFACE(curif).body; curitem; curitem = IDL_LIST(curitem).next) {
    IDL_tree curop = IDL_LIST(curitem).data;

    switch(IDL_NODE_TYPE(curop)) {
    case IDLN_OP_DCL:
      fprintf(ioi->of, "#define %s_%s %s_%s\n",
	      realid, IDL_IDENT(IDL_OP_DCL(curop).ident).str,
	      id, IDL_IDENT(IDL_OP_DCL(curop).ident).str);
      break;
    case IDLN_ATTR_DCL:
      {
	IDL_tree curitem;

	/* We don't use OIDL_Attr_Info here because inherited ops may go back into trees that are output-inhibited
	   and therefore don't have the OIDL_Attr_Info generated on them */

	for(curitem = IDL_ATTR_DCL(curop).simple_declarations; curitem; curitem = IDL_LIST(curitem).next) {
	  IDL_tree ident;

	  ident = IDL_LIST(curitem).data;
	  
	  fprintf(ioi->of, "#define %s__get_%s %s__get_%s\n",
		  realid, IDL_IDENT(ident).str,
		  id, IDL_IDENT(ident).str);

	  if(!IDL_ATTR_DCL(curop).f_readonly)
	    fprintf(ioi->of, "#define %s__set_%s %s__set_%s\n",
		    realid, IDL_IDENT(ident).str,
		    id, IDL_IDENT(ident).str);
	}
      }
      break;
    default:
      break;
    }
  }

  g_free(id);
  g_free(realid);
}

/*
   Skeleton
   Called by
     -ch_output_skel_protos
*/
static void
doskel(IDL_tree cur, OIDL_Run_Info *rinfo, char *ifid, OIDL_C_Info *ci)
{
  char *id;

  id = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(IDL_OP_DCL(cur).ident), "_", 0);
#if 0
  /*  By Hara?
    fprintf(ci->fh, "void *_RtORB_impl_%s("
    "CORBA_%s _obj, "
    "octet * _buf, "
    "CORBA_Class_Method *_m, "
    "GIOP_ReplyBody *reply,"
    "CORBA_Environment *ev ", id, ifid);
  */
  /* By Suri ?
    fprintf(ci->fh, "void _RtORB_impl_%s("
    "CORBA_%s _obj, "
    "CORBA_Any *result, "
    "CORBA_Class_Method *_m, "
    "CORBA_Any *argv,"
    "CORBA_Environment *ev, ", id, ifid);
  */
#endif

  fprintf(ci->fh, "void _RtORB_impl_%s("
	  "CORBA_%s _obj, "
	  "void * *result, "
	  "CORBA_Class_Method *_m, "
	  "void * *argv,"
	  "CORBA_Environment *ev, ", id, ifid);

  orbit_cbe_op_write_proto(ci->fh, cur, "_impl_", TRUE);
  /* 080205 weak or static? */
#if GNU_WEAK
  fprintf(ci->fh, ") __attribute__((weak));\n");
#else
  fprintf(ci->fh, ");\n");
#endif

  g_free(id);
}

/*
   Output prototypes for skeleton
   Called by
     -orbit_idl_output_c_headers
     -ch_output_skel_protos(Recursive call)
*/
static void
ch_output_skel_protos(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci)
{
  if(!tree) return;

  if ( tree->declspec & IDLF_DECLSPEC_PIDL )
    return;

  switch(IDL_NODE_TYPE(tree)) {
    case IDLN_MODULE:
      fprintf(ci->fh, "/*   Module */\n");
      ch_output_skel_protos(IDL_MODULE(tree).definition_list, rinfo, ci);
      fprintf(ci->fh, "/*   End of Module */\n");
      break;
    case IDLN_LIST:
      {
        IDL_tree sub;

        fprintf(ci->fh, "/*   List */\n");
        for(sub = tree; sub; sub = IDL_LIST(sub).next) {
          ch_output_skel_protos(IDL_LIST(sub).data, rinfo, ci);
        }
        fprintf(ci->fh, "/*   End of List */\n");
      }
      break;
    case IDLN_INTERFACE:
      {
        IDL_tree sub;
        char *ifid;

        ifid = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(IDL_INTERFACE(tree).ident), "_", 0);

        for(sub = IDL_INTERFACE(tree).body; sub; sub = IDL_LIST(sub).next) {
          IDL_tree cur;

	  cur = IDL_LIST(sub).data;

	  switch(IDL_NODE_TYPE(cur))
          {
	    case IDLN_OP_DCL:
	      doskel(cur, rinfo, ifid, ci);
	      break;
	    case IDLN_ATTR_DCL:
	      {
	        OIDL_Attr_Info *ai = cur->data;
	        IDL_tree curitem;

	        for(curitem = IDL_ATTR_DCL(cur).simple_declarations; curitem; curitem = IDL_LIST(curitem).next) {
	          ai = IDL_LIST(curitem).data->data;
	      
	          doskel(ai->op1, rinfo, ifid, ci);
	          if(ai->op2){
		    doskel(ai->op2, rinfo, ifid, ci);
                  }
	        }
	      }
	      break;
	    default:
	      break;
	  }
        }
        g_free(ifid);
      }
      break;
    default:
      break;
  }
}

/*
  Output itypes ?
  Called by
    -orbit_idl_output_c_headers
*/
static void
ch_output_itypes (IDL_tree tree, OIDL_C_Info *ci)
{
  static int num_methods = 0;

  if (!tree)
    return;

  switch(IDL_NODE_TYPE(tree))
  {
    case IDLN_MODULE:
      ch_output_itypes (IDL_MODULE(tree).definition_list, ci);
      break;
    case IDLN_LIST:
      {
        IDL_tree sub;
        for (sub = tree; sub; sub = IDL_LIST(sub).next){
          ch_output_itypes (IDL_LIST(sub).data, ci);
        }
      }
      break;
    case IDLN_ATTR_DCL:
      {
        OIDL_Attr_Info *ai = tree->data;

        IDL_tree curitem;
      
        for(curitem = IDL_ATTR_DCL(tree).simple_declarations; curitem; curitem = IDL_LIST(curitem).next) {
          ai = IDL_LIST(curitem).data->data;
	
          ch_output_itypes (ai->op1, ci);
          if(ai->op2){
	    ch_output_itypes (ai->op2, ci);
          }
        }
      }
      break;

    case IDLN_INTERFACE:
      {
        char  *id;

        id = IDL_ns_ident_to_qstring (IDL_IDENT_TO_NS ( IDL_INTERFACE (tree).ident), "_", 0);

        ch_output_itypes (IDL_INTERFACE(tree).body, ci);
#if 0
        fprintf (ci->fh, "#ifdef ORBIT_IDL_C_IMODULE_%s\n", ci->c_base_name);
        fprintf (ci->fh, "static \n");
        fprintf (ci->fh, "#else\n");
        fprintf (ci->fh, "extern \n");
        fprintf (ci->fh, "#endif\n");
#else
        fprintf (ci->fh, "extern ");
#endif
        fprintf (ci->fh, "CORBA_Class_Impl %s__imp;\n", id);

        fprintf (ci->fh, "#define %s_IMETHODS_LEN %d\n", id, num_methods);

        if (num_methods == 0){
          fprintf (ci->fh, "#define %s__methods (CORBA_Class_Method*) NULL\n", id);
        } else {
#if 0
          fprintf (ci->fh, "#ifdef ORBIT_IDL_C_IMODULE_%s\n", ci->c_base_name);
          fprintf (ci->fh, "static \n");
          fprintf (ci->fh, "#else\n");
          fprintf (ci->fh, "extern \n");
          fprintf (ci->fh, "#endif\n");
#else
          fprintf (ci->fh, "extern ");
#endif
          fprintf (ci->fh, "CORBA_Class_Method %s__methods[%s_IMETHODS_LEN];\n", id, id);
        }
        num_methods = 0;
      }
      break;

    case IDLN_OP_DCL:
      num_methods++;
      break;
    default:
      break;
  }
}

/*


*/
static char *getNodeName(IDL_tree tname){
  char *name = 0;
  switch(IDL_NODE_TYPE(tname))
  {
     case IDLN_IDENT:
       name = IDL_IDENT(tname).str;
       break;
     default:
       break;
  }
  return name;
}
