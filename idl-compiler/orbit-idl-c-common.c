/*
  IDL Compiler 

*/
#include "config.h"

#include "orbit-idl-c-backend.h"

#include <string.h>

typedef struct {
  IDL_tree tree;
  GSList  *methods; /* IDLN_OP_DCLs */
} Interface;

typedef struct {
  FILE     *of;
  IDL_tree cur_node; /* Current Interface */
  char     *cur_id;
  guint    parents;
} CCSmallInterfaceTraverseInfo;

/* TypeCodes */

static void cc_output_typecodes (IDL_tree     tree,
         OIDL_C_Info *ci);

/*
  Output TypeCode prep of Sequence   
  Called by cc_output_tc_walker
*/
static void
cc_typecode_prep_sequence (IDL_tree tree, OIDL_C_Info *ci)
{
  IDL_tree  seq_type;
  IDL_tree  fake_seq_type = NULL;
  char     *type_str;
  char     *seq_type_str;

  seq_type = orbit_cbe_get_typespec (IDL_TYPE_SEQUENCE (tree).simple_type_spec);

  if (IDL_NODE_TYPE (seq_type) != IDLN_INTERFACE)
    seq_type_str = orbit_cbe_get_typespec_str (seq_type);
  else {
    seq_type_str = g_strdup ("CORBA_Object");
    fake_seq_type = IDL_type_object_new ();
  }

  type_str = orbit_cbe_get_typespec_str (IDL_TYPE_SEQUENCE (tree).simple_type_spec);

  if (strcmp (type_str, seq_type_str)) {
    IDL_tree fake_seq;

    fake_seq = IDL_type_sequence_new (
        fake_seq_type ? fake_seq_type : seq_type,
        NULL);
    IDL_NODE_UP (fake_seq) = IDL_NODE_UP (tree);

    cc_output_typecodes (fake_seq, ci);

    IDL_TYPE_SEQUENCE (fake_seq).simple_type_spec = NULL;
    IDL_tree_free (fake_seq);
  }

  if (fake_seq_type)
    IDL_tree_free (fake_seq_type);

  g_free (type_str);
  g_free (seq_type_str);
}
 
/*
  Walker function, I supporse this function called by IDL_tree_walk2.
*/
static gboolean
cc_output_tc_walker (IDL_tree_func_data *tfd, OIDL_C_Info *ci)
{
  IDL_tree tree = tfd->tree;

  switch(IDL_NODE_TYPE (tree)) {
  case IDLN_CONST_DCL:
  case IDLN_ATTR_DCL:
  case IDLN_OP_DCL:
    return FALSE; /* dont recurse into these */

  case IDLN_TYPE_SEQUENCE:
    if (!tfd->step) {
      cc_typecode_prep_sequence (tree, ci);
      break;
    }

    /* drop through */
  case IDLN_INTERFACE:
  case IDLN_EXCEPT_DCL:
  case IDLN_TYPE_STRUCT:
  case IDLN_TYPE_UNION:
  case IDLN_TYPE_DCL:
  case IDLN_TYPE_ENUM:
  case IDLN_TYPE_FIXED:
    if (tfd->step)
      orbit_output_typecode (ci, tree);
    break;
  default:
    break;
  }
  return TRUE; /* continue walking */
}

/*
  Outpit TypeCode definitions
  2nd called by orbit_idl_output_c_common.

*/
static void
cc_output_typecodes (IDL_tree     tree, OIDL_C_Info *ci)
{
  IDL_tree_walk2 (tree, 0, IDL_WalkF_TypespecOnly,
      (IDL_tree_func) cc_output_tc_walker,
      (IDL_tree_func) cc_output_tc_walker,
      ci);
}

/* class ids */
/*
    Output Class ID
    3rd called by orbit_idl_output_c_common 

*/
static void
cc_output_class_id (IDL_tree       tree,
         OIDL_Run_Info *rinfo,
         OIDL_C_Info   *ci)
{
  char *iface_id;

  iface_id = IDL_ns_ident_to_qstring (
    IDL_IDENT_TO_NS (IDL_INTERFACE (tree).ident), "_", 0);

  fprintf (ci->fh, "\n#ifndef ORBIT_IDL_C_IMODULE_%s\n",ci->c_base_name);
  fprintf (ci->fh, "CORBA_unsigned_long %s__classid = 0;\n", iface_id);
  fprintf (ci->fh, "#endif\n");

  g_free (iface_id);
}

static void
cc_output_class_ids (IDL_tree       tree,
         OIDL_Run_Info *rinfo,
         OIDL_C_Info   *ci)
{
  if (!tree || (tree->declspec & IDLF_DECLSPEC_PIDL))
    return;

  switch (IDL_NODE_TYPE (tree)) {
  case IDLN_MODULE:
    cc_output_class_ids (IDL_MODULE (tree).definition_list, rinfo, ci);
    break;
  case IDLN_LIST: {
    IDL_tree node;

    for (node = tree; node; node = IDL_LIST (node).next)
      cc_output_class_ids (IDL_LIST (node).data, rinfo, ci);
    break;
    }
  case IDLN_INTERFACE:
    cc_output_class_id (tree, rinfo, ci);
    break;
  default:
    break;
  }
}

/* IInterfaces */

static void
cc_output_iargs (FILE *of, const char *method, IDL_tree tree)
{
  IDL_tree sub;
  int      arg_count = 0;

  /* Build a list of IArgs */
  for (sub = IDL_OP_DCL (tree).parameter_dcls; sub;
       sub = IDL_LIST (sub).next) {
    IDL_tree parm;
    char    *tc;

    if (!arg_count)
      fprintf (of, "static CORBA_IArg %s__arginfo [] = {\n", method);

    parm = IDL_LIST(sub).data;

    fprintf (of, "\t{ ");

    /* TypeCode tc */
    tc = orbit_cbe_get_typecode_name (
      IDL_PARAM_DCL (parm).param_type_spec);
    if (!tc) {
      g_warning ("Can't get typecode");
      tc = g_strdup ("NULL /* no typecode */");
    }
    fprintf (of, "%s, ", tc);

    /* IArgFlag flags */
    switch (IDL_PARAM_DCL (parm).attr) {
    case IDL_PARAM_IN:
      fprintf (of, " CORBA_I_ARG_IN ");
      break;
    case IDL_PARAM_OUT:
      fprintf (of, " CORBA_I_ARG_OUT ");
      break;
    case IDL_PARAM_INOUT:
      fprintf (of, " CORBA_I_ARG_INOUT ");
      break;
    }

    fprintf (of, ", ");

    /* string name */
    fprintf (of, "(char *)\"%s\"", IDL_IDENT (IDL_PARAM_DCL (
      IDL_LIST (sub).data).simple_declarator).str);

    fprintf (of, " }%s\n", IDL_LIST (sub).next ? "," : "");

    g_free (tc);
    arg_count++;
  }

  if (arg_count)
    fprintf (of, "};\n");
}

static void
cc_output_contexts (FILE *of, const char *method, IDL_tree tree)
{
  /* Build a list of contest names */
  if (IDL_OP_DCL (tree).context_expr) {
    IDL_tree curitem;

    fprintf (of, "/* Exceptions */\n");
    fprintf (of, "static CORBA_string %s__contextinfo [] = {\n",
       method);

    for (curitem = IDL_OP_DCL (tree).context_expr; curitem;
         curitem = IDL_LIST (curitem).next) {
      fprintf (of, "\"%s\"%c", 
         IDL_STRING (IDL_LIST (curitem).data).value,
         IDL_LIST (curitem).next ? ',' : ' ');
    }

    fprintf (of, "};\n");
  }
}

static void
cc_output_exceptinfo (FILE *of, const char *method, IDL_tree tree)
{
  /* Build a list of exception typecodes */
  if (IDL_OP_DCL (tree).raises_expr) {
    IDL_tree curitem;

    fprintf (of, "/* Exceptions */\n");
    fprintf (of, "static CORBA_TypeCode %s__exceptinfo [] = {\n",
       method);
    
    for (curitem = IDL_OP_DCL (tree).raises_expr; curitem;
         curitem = IDL_LIST(curitem).next) {
      char *type_id;
      IDL_tree curnode = IDL_LIST(curitem).data;
      
      type_id = orbit_cbe_get_typecode_name (curnode);
      fprintf (of, "\t%s,\n", type_id);
      g_free (type_id);
    }
    fprintf (of, "\tNULL\n};\n");
  }
}

static void
cc_output_method_bits (IDL_tree tree, const char *id, OIDL_C_Info *ci)
{
  FILE *of = ci->fh;
  char *fullname;

  fullname = g_strconcat (id, "_", IDL_IDENT (
    IDL_OP_DCL (tree).ident).str, NULL);

  cc_output_iargs (of, fullname, tree);

  cc_output_contexts (of, fullname, tree);

  cc_output_exceptinfo (of, fullname, tree);

  g_free (fullname);
}

/*
   Output Method

*/
static void
cc_output_method (FILE *of, IDL_tree tree, const char *id)
{
  int arg_count;
  int except_count;
  int context_count;
  const char *method;
  char       *fullname;

  fullname = g_strconcat (id, "_", IDL_IDENT (
    IDL_OP_DCL (tree).ident).str, NULL);

  arg_count = IDL_list_length (IDL_OP_DCL (tree).parameter_dcls);
  except_count = IDL_list_length (IDL_OP_DCL (tree).raises_expr);
  context_count = IDL_list_length (IDL_OP_DCL (tree).context_expr);
  
  fprintf (of, "\t{\n");
  /* string name, long name_len */
  method = IDL_IDENT (IDL_OP_DCL (tree).ident).str;
/*
  fprintf (of, "\"%s\", %d,\n", method, strlen (method));
*/
  fprintf (of, "\t\t(char *)\"%s\",\n", method);

  /* TypeCode ret */
  if (IDL_OP_DCL (tree).op_type_spec) {
    char *type_id;

    type_id = orbit_cbe_get_typespec_str (
      IDL_OP_DCL (tree).op_type_spec);
    fprintf (of, "\t\tTC_%s, \n", type_id);
    g_free (type_id);
  } else
    fprintf (of, "TC_void, \n");
/*
  fprintf (of, "\t\t _RtORB_impl_%s,\n", fullname);
*/
  /* IArgs arguments */
  if (arg_count)
    fprintf (of, "\t\t %d, %s__arginfo,  \n", arg_count, fullname);
  else
    fprintf (of, "\t\t 0, NULL, \n");

#if 0
  /* IArgs arguments */
  if (arg_count)
    fprintf (of, "\t\t{ %d, %d, %s__arginfo, FALSE },\n",
       arg_count, arg_count, fullname);
  else
  /* IContexts contexts */
  if (context_count)
    fprintf (of, "\t\t{ %d, %d, %s__contextinfo, FALSE },\n",
       context_count, context_count, fullname);
  else
    fprintf (of, "\t\t{ 0, 0, NULL, FALSE },\n");
    
#endif
  /* ITypes exceptions */
  if (IDL_OP_DCL (tree).raises_expr)
    fprintf (of, "\t\t %d, %s__exceptinfo \n",
        except_count, fullname);
  else
    fprintf (of, "\t\t 0, NULL\n");

#if 0
  /* IMethodFlags flags */
  fprintf (of, "\t\t0");

  if (IDL_OP_DCL(tree).f_oneway)
    fprintf (of, " | ORBit_I_METHOD_1_WAY");
/* FIXME: re-scan for no_out */
/*  if (no_out)
  fprintf (of, " | ORBit_I_METHOD_NO_OUT");*/

  if (IDL_OP_DCL (tree).op_type_spec &&
      orbit_cbe_type_is_fixed_length (
        IDL_OP_DCL (tree).op_type_spec))
    fprintf (of, "| ORBit_I_COMMON_FIXED_SIZE");

  if (IDL_OP_DCL(tree).context_expr)
    fprintf (of, "| ORBit_I_METHOD_HAS_CONTEXT");
#endif

  fprintf (of, "\n\t}");

  g_free (fullname);
}

static void
cc_output_base_itypes(IDL_tree node, CCSmallInterfaceTraverseInfo *iti)
{
  if (iti->cur_node == node)
    return;

  fprintf (iti->of, "\t(char *)\"%s\",\n",
     IDL_IDENT(IDL_INTERFACE(node).ident).repo_id);

  iti->parents++;
}

static void
cc_output_itypes (GSList *list, OIDL_C_Info *ci)
{
  GSList *l;
  FILE   *of = ci->fh;

  for (l = list; l; l = l->next) {
    CCSmallInterfaceTraverseInfo iti;
    Interface *i = l->data;
    char      *id;
    GSList    *m;

    id = IDL_ns_ident_to_qstring (IDL_IDENT_TO_NS (
      IDL_INTERFACE (i->tree).ident), "_", 0);

    for (m = i->methods; m; m = m->next)
      cc_output_method_bits (m->data, id, ci);

    if (i->methods) {
      
      /* 080205 weak or static?*/
#if GNU_WEAK
      fprintf (of, "CORBA_Class_Method %s__methods [] __attribute__((weak)) = {\n", id);
#else
      fprintf (of, "CORBA_Class_Method %s__methods [] = {\n", id);
#endif

      if (!(m = i->methods))
        fprintf (of, "  {{0}}");

      else for (; m; m = m->next) {
        cc_output_method (of, m->data, id);
        if (m->next)
          fprintf(of, ",");
      }

      fprintf (of, "\n};");
    }

    fprintf (of, "\nstatic char * %s__base_itypes[] = {\n", id);

    iti.of = of;
    iti.cur_node = i->tree;
    iti.cur_id = id;
    iti.parents = 0;
    IDL_tree_traverse_parents(i->tree, (GFunc)cc_output_base_itypes, &iti);

    fprintf (of, "\t(char *)\"IDL:omg.org/CORBA/Object:1.0\"\n};");

    fprintf (of, "\n#ifdef ORBIT_IDL_C_IMODULE_%s\n",
       ci->c_base_name);
    fprintf (of, "static\n");
    fprintf (of, "#endif\n");
#if GNU_WEAK
    fprintf (of, "CORBA_Class_Impl %s__imp __attribute__((weak)) = {\n", id);
#else
    fprintf (of, "CORBA_Class_Impl %s__imp = {\n", id);
#endif
    fprintf (of, "    TC_%s,\n", id);
    fprintf (of, "    (char *)\"%s\",\n", id);
    fprintf (of, "    %d,\n    %s__methods,\n", g_slist_length (i->methods), id);

#if 0
    fprintf (of, "{%d, %d, %s__base_itypes, FALSE}\n", 
       iti.parents + 1, iti.parents + 1, id);
#else
    fprintf (of, "    %d, (char **) %s__base_itypes\n", iti.parents + 1, id);
#endif
    fprintf (of, "};\n\n");

    g_free (id);
  }

  for (l = list; l; l = l->next) {
    g_slist_free (((Interface *)l->data)->methods);
    g_free (l->data);
  }

  g_slist_free (list);
}

/*
   Output Interfaces
    4th called by orbit_idl_output_c_common 
*/
static GSList *
cc_build_interfaces (GSList *list, IDL_tree tree)
{
  if (!tree)
    return list;

  switch (IDL_NODE_TYPE (tree)) {
  case IDLN_MODULE:
    list = cc_build_interfaces (
      list, IDL_MODULE (tree).definition_list);
    break;
  case IDLN_LIST: {
    IDL_tree sub;
    for (sub = tree; sub; sub = IDL_LIST (sub).next)
      list = cc_build_interfaces (
        list, IDL_LIST (sub).data);
    break;
  }
  case IDLN_ATTR_DCL: {
    IDL_tree curitem;
      
    for (curitem = IDL_ATTR_DCL (tree).simple_declarations;
         curitem; curitem = IDL_LIST (curitem).next) {
      OIDL_Attr_Info *ai = IDL_LIST (curitem).data->data;
  
      list = cc_build_interfaces (list, ai->op1);
      if (ai->op2)
        list = cc_build_interfaces (list, ai->op2);
    }
    break;
  }
  case IDLN_INTERFACE: {
    Interface *i = g_new0 (Interface, 1);

    i->tree = tree;

    list = g_slist_append (list, i);

    list = cc_build_interfaces (list, IDL_INTERFACE(tree).body);

    break;
  }
  case IDLN_OP_DCL: {
    Interface *i;

    g_return_val_if_fail (list != NULL, NULL);

    i = ( g_slist_last(list) )->data;
    i->methods = g_slist_append (i->methods, tree);
    break;
  }
  case IDLN_EXCEPT_DCL:
    break;
  default:
    break;
  }

  return list;
}

/*
  Output skeletons
  This function called by cc_output_skels

*/
static void
cc_output_skel (IDL_tree     tree,
    OIDL_C_Info *ci,
    int         *idx)
{
  IDL_tree  intf;
  gboolean  has_args;
  gboolean  has_retval;
  char     *opname;
  char     *ifname;

  g_return_if_fail (idx != NULL);

  intf = IDL_get_parent_node (tree, IDLN_INTERFACE, NULL);

  has_args   = IDL_OP_DCL (tree).parameter_dcls != NULL;
  has_retval = IDL_OP_DCL (tree).op_type_spec != NULL;

  opname = IDL_ns_ident_to_qstring (IDL_IDENT_TO_NS (IDL_OP_DCL (tree).ident), "_", 0);
  ifname = IDL_ns_ident_to_qstring (IDL_IDENT_TO_NS (IDL_INTERFACE (intf).ident), "_", 0);

#if 0
  fprintf (ci->fh, "void _ORBIT_skel_small_%s("
        "POA_%s             *_o_servant, "
        "gpointer            _o_retval,"
        "gpointer           *_o_args,"
        "CORBA_Context       _o_ctx,"
        "CORBA_Environment  *_o_ev,\n", opname, ifname);

  orbit_cbe_op_write_proto (ci->fh, tree, "_impl_", TRUE);
#else
  /*
  fprintf (ci->fh, "void *_RtORB_impl_%s("
        "CORBA_%s    obj, "
        "octet      *buf,"
        "CORBA_Class_Method  *m,"
        "GIOP_ReplyBody    *reply,"
        "CORBA_Environment  *env \n", opname, ifname);
*/
/*
  fprintf (ci->fh, "void _RtORB_impl_%s("
        "CORBA_%s            obj, "
        "CORBA_Any          *result,"
        "CORBA_Class_Method  *m,"
        "CORBA_Any          *argv,"
        "CORBA_Environment  *env,\n", opname, ifname);
*/
/*
  fprintf (ci->fh, "void _RtORB_impl_%s("
        "CORBA_%s            obj, "
        "void *          *result,"
        "CORBA_Class_Method  *m,"
        "void *          *argv,"
        "CORBA_Environment  *env,\n", opname, ifname);
  orbit_cbe_op_write_proto (ci->fh, tree, "_impl_", TRUE);
*/

  fprintf (ci->fh, "void _RtORB_impl_%s("
                   "CORBA_%s obj,\n"
                   "    void **result, "
                   "CORBA_Class_Method *m, "
                   "void **argv, "
                   "CORBA_Environment  *env,\n", opname, ifname);
  fprintf (ci->fh, "    ");
  orbit_cbe_op_write_proto (ci->fh, tree, "_impl_", TRUE);

#endif
  fprintf (ci->fh, ")\n{\n");

#if 0
 /** previous definition, but we should use this section for C-language **/
  if (has_retval) {
    fprintf (ci->fh, "  *(");
    orbit_cbe_write_param_typespec (ci->fh, tree);
    fprintf (ci->fh, " *)_o_retval = ");
  }
  fprintf (ci->fh, "_impl_%s (_o_servant, ", IDL_IDENT (IDL_OP_DCL (tree).ident).str);
  
  orbit_cbe_unflatten_args (tree, ci->fh, "_o_args");

  if (IDL_OP_DCL (tree).context_expr)
    fprintf (ci->fh, "_o_ctx, ");

  fprintf (ci->fh, "_o_ev);\n");
#else
/*
  fprintf (ci->fh, "  CORBA_Any *argv; \n"
       "  CORBA_Any result; \n"
       " \n"
       "  argv = (CORBA_Any *)deMarshal_Arg(buf, m->in_argc, m->in_argv); \n"
       "\n"
      );
*/
/*
  if (has_retval) {
    char *repl_type = rtorb_cbe_get_typespec_str(IDL_OP_DCL(tree).op_type_spec);
    fprintf (ci->fh, "  result->%s = ", repl_type);
  }
*/

  fprintf (ci->fh, "  ");
  if (has_retval) {
    fprintf (ci->fh, "*(");
    orbit_cbe_write_param_typespec (ci->fh, tree);
    fprintf (ci->fh, " *)result = ");
  }

/*
  fprintf (ci->fh, "impl_%s (obj, ", opname);
*/
  fprintf (ci->fh, "_impl_%s (obj, ", IDL_IDENT (IDL_OP_DCL (tree).ident).str);
  
  orbit_cbe_unflatten_args (tree, ci->fh, "argv");
/* 
  rtorb_cbe_unflatten_args (tree, ci->fh, "argv");
*/

/*
  if (IDL_OP_DCL (tree).context_expr)
    fprintf (ci->fh, "_o_ctx, ");
*/
  fprintf (ci->fh, "env);\n");

/*
  fprintf (ci->fh, "\n"
   " Marshal_Reply_Arguments(reply, &result, argv, m); \n"
   "\n"
   "Free_Arguments(argv, m->in_argc, m->in_argv); \n"
   "free(argv); \n"
   "return; \n"
   );
*/
#endif
  fprintf (ci->fh, "}\n");

  g_free (opname);
  g_free (ifname);

  (*idx)++;
}

/*
  Output skels
   1st called by orbit_idl_output_c_common
*/
static void
cc_output_skels (IDL_tree       tree,
     OIDL_Run_Info *rinfo,
     OIDL_C_Info   *ci,
     int           *idx)
{

  if (!tree || (tree->declspec & IDLF_DECLSPEC_PIDL))
    return;

  switch (IDL_NODE_TYPE (tree)) 
  {
    case IDLN_MODULE:
      fprintf(ci->fh, "/* Module (Skels) */\n");
      cc_output_skels (IDL_MODULE (tree).definition_list, rinfo, ci, idx);
      fprintf(ci->fh, "/* End of Module (Skels) */\n");
      break;
    case IDLN_LIST:
      {
        IDL_tree node;

        fprintf(ci->fh, "/* List (Skels) */\n");
        for (node = tree; node; node = IDL_LIST (node).next){
          cc_output_skels (IDL_LIST (node).data, rinfo, ci, idx);
        }
        fprintf(ci->fh, "/* End of List (Skels) */\n");
      }
      break;
    case IDLN_ATTR_DCL:
      {
        OIDL_Attr_Info *ai = tree->data;
        IDL_tree        node;
      
        fprintf(ci->fh, "/* Attribute declarations (Skels) */\n");
        for (node = IDL_ATTR_DCL (tree).simple_declarations; node; node = IDL_LIST (node).next)
        {
          ai = IDL_LIST (node).data->data;
          cc_output_skels (ai->op1, rinfo, ci, idx);

          if (ai->op2){
            cc_output_skels (ai->op2, rinfo, ci, idx);
          }
        }
        fprintf(ci->fh, "/* End of Attribute declarations (Skels) */\n");
      }
      break;
    case IDLN_INTERFACE:
      {
        fprintf(ci->fh, "/* Interface declarations (Skels) */\n");
        int real_idx = 0;
        cc_output_skels (IDL_INTERFACE (tree).body, rinfo, ci, &real_idx);
        fprintf(ci->fh, "/* End of Interface declarations (Skels) */\n");
      }
      break;
    case IDLN_OP_DCL:
      fprintf(ci->fh, "/* Option declarations (Skels) */\n");
      cc_output_skel (tree, ci, idx);
      fprintf(ci->fh, "/* End of Option declarations (Skels) */\n");
      break;
    default:
      break;
  }
}

static void cc_output_methods(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci)
{
  if (!tree) { return; }

  switch (IDL_NODE_TYPE (tree)) {
  case IDLN_MODULE:
    cc_output_methods (IDL_MODULE (tree).definition_list, rinfo, ci);
    break;
  case IDLN_LIST: {
    IDL_tree node;

    for (node = tree; node; node = IDL_LIST (node).next)
      cc_output_methods (IDL_LIST (node).data, rinfo, ci);
    break;
    }

  case IDLN_TYPE_DCL:
    cc_output_methods(IDL_TYPE_DCL (tree).type_spec, rinfo, ci);
    break;
   case IDLN_TYPE_SEQUENCE:
     {
       IDL_tree tts = orbit_cbe_get_typespec(IDL_TYPE_SEQUENCE(tree).simple_type_spec);
       char *ctmp = orbit_cbe_get_typespec_str(IDL_TYPE_SEQUENCE(tree).simple_type_spec);
       char *ctmp2 = orbit_cbe_get_typespec_str(tts);
       gboolean separate_defs = strcmp(ctmp, ctmp2);
       
       if (!separate_defs) {
         char * str = orbit_cbe_get_typespec_str(tree);
        /* operator [] */

         fprintf(ci->fh, "#ifndef Defined_%s\n",str);
         fprintf(ci->fh, "#define Defined_%s TRUE\n",str);
#if GNU_WEAK
         fprintf(ci->fh, "__attribute__((weak)) %s::CppSeq::ElemRef \n%s::operator[](CORBA::ULong n)\n{\n", str, str);
         fprintf(ci->fh, "  return %s::CppSeq::element(this, (int)n);\n}\n\n", str);

         fprintf(ci->fh, "__attribute__((weak)) %s::CppSeq::ElemRef \n%s::operator[](CORBA::ULong n) const\n{\n", str, str);
         fprintf(ci->fh, "  return %s::CppSeq::element(this, (int)n );\n}\n\n", str);

        /* length() */
        fprintf(ci->fh, "__attribute__((weak)) CORBA::ULong \n%s::length(CORBA::ULong n)\n{\n", str);
        fprintf(ci->fh, "  return %s::CppSeq::length(this, n);\n}\n\n", str);
        /* copy operator */
        fprintf(ci->fh, "__attribute__((weak)) %s_type & \n%s::operator=(const CppSeq &o)\n{\n", str, str);
        fprintf(ci->fh, "  o.copyTo(*this);\n  return *this;\n}\n\n");
        fprintf(ci->fh, "__attribute__((weak)) %s_type & \n%s::operator=(const %s_type &o)\n{\n", str,str, str, str);
        fprintf(ci->fh, "  CppSeq::copy(o, *this);\n  return *this;\n}\n\n");
        fprintf(ci->fh, "#endif /* Defined_%s */\n",str);
#else
         fprintf(ci->fh, " %s::CppSeq::ElemRef \n%s::operator[](CORBA::ULong n)\n{\n", str, str);
         fprintf(ci->fh, "  return %s::CppSeq::element(this, (int)n);\n}\n\n", str);

         fprintf(ci->fh, " %s::CppSeq::ElemRef \n%s::operator[](CORBA::ULong n) const\n{\n", str, str);
         fprintf(ci->fh, "  return %s::CppSeq::element(this, (int)n );\n}\n\n", str);

        /* length() */
        fprintf(ci->fh, " CORBA::ULong \n%s::length(CORBA::ULong n)\n{\n", str);
        fprintf(ci->fh, "  return %s::CppSeq::length(this, n);\n}\n\n", str);
        /* copy operator */
        fprintf(ci->fh, " %s_type & \n%s::operator=(const CppSeq &o)\n{\n", str, str);
        fprintf(ci->fh, "  o.copyTo(*this);\n  return *this;\n}\n\n");
        fprintf(ci->fh, " %s_type & \n%s::operator=(const %s_type &o)\n{\n", str,str, str);
        fprintf(ci->fh, "  CppSeq::copy(o, *this);\n  return *this;\n}\n\n");
        fprintf(ci->fh, "#endif /* Defined_%s */\n",str);
#endif
         g_free(str);
       }
       g_free(ctmp);
       g_free(ctmp2);
     }
    break;
  case IDLN_TYPE_STRUCT:
    {
      char * str = orbit_cbe_get_typespec_str(tree);
                        IDL_tree cur, curmem;

      fprintf(ci->fh, "#ifndef Defined_cdr_%s\n",str);
       fprintf(ci->fh, "#define Defined_cdr_%s TRUE\n",str);
#if GNU_WEAK
      fprintf(ci->fh, "__attribute__((weak)) void %s::operator>>=(cdrStream &_n) CONST \n{\n", str);
#else
      fprintf(ci->fh, " void %s::operator>>=(cdrStream &_n) CONST \n{\n", str);
#endif

                        for(cur = IDL_TYPE_STRUCT(tree).member_list; cur;
                                                       cur = IDL_LIST(cur).next) {
                          for(curmem = IDL_MEMBER(IDL_LIST(cur).data).dcls;
                                curmem; curmem = IDL_LIST(curmem).next) {
          fprintf(ci->fh, "  %s >>= _n;\n",IDL_IDENT(IDL_LIST(curmem).data).str);
                          }
                        }

      fprintf(ci->fh, "}\n");
#if GNU_WEAK
      fprintf(ci->fh, "__attribute__((weak)) void %s::operator<<=(cdrStream &_n) {\n", str);
#else
      fprintf(ci->fh, " void %s::operator<<=(cdrStream &_n) {\n", str);
#endif
                        for(cur = IDL_TYPE_STRUCT(tree).member_list; cur;
                                                       cur = IDL_LIST(cur).next) {
                          for(curmem = IDL_MEMBER(IDL_LIST(cur).data).dcls;
                                curmem; curmem = IDL_LIST(curmem).next) {
          fprintf(ci->fh, "  %s <<= _n;\n",IDL_IDENT(IDL_LIST(curmem).data).str);
                          }
                        }
      fprintf(ci->fh, "}\n");
      fprintf(ci->fh, "#endif /* Defined_cdr_%s */\n",str);
/*
      IDL_tree cur;
*/
      for(cur = IDL_TYPE_STRUCT(tree).member_list; cur; cur = IDL_LIST(cur).next) {
        IDL_tree ts;
        ts = IDL_MEMBER(IDL_LIST(cur).data).type_spec;
        cc_output_methods(ts, rinfo, ci);
      }
    }
    break;
  default:
    break;
  }

}

/*
  Output common file
  Main function to output XX-common.c

*/
void
orbit_idl_output_c_common (IDL_tree       tree,
         OIDL_Run_Info *rinfo,
         OIDL_C_Info   *ci)
{
  fprintf (ci->fh, OIDL_C_WARNING);

  fprintf (ci->fh, "#include <string.h>\n");
  fprintf (ci->fh, "#include <RtORB/cdrStream.h>\n");
  fprintf (ci->fh, "#define CONST const\n");
  fprintf (ci->fh, "#define %s_COMMON\n", ci->c_base_name);

  fprintf (ci->fh, "#include \"%s.h\"\n\n", ci->base_name);

  /* FIXME: this is slightly nasty, but we need these in common,
     and this fixes an internal build issue */

  if (rinfo->enabled_passes & OUTPUT_SKELS ||
      rinfo->enabled_passes & OUTPUT_STUBS) {
    fprintf (ci->fh, "/** Interface Module definitions in %s */\n",ci->c_base_name);
    fprintf (ci->fh, "\n#ifndef ORBIT_IDL_C_IMODULE_%s\n",ci->c_base_name);
    cc_output_skels (tree, rinfo, ci, NULL);
    fprintf (ci->fh, "\n#endif\n");
  }

  fprintf (ci->fh, "\n/** TypeCode definitions in %s */\n",ci->c_base_name);
  cc_output_typecodes (tree, ci);

  fprintf (ci->fh, "\n/** Class ID definitions in %s */\n",ci->c_base_name);
  cc_output_class_ids (tree, rinfo, ci);

  if (rinfo->idata) {
    GSList *list = NULL;

    fprintf (ci->fh, "\n/* Interface type data */\n\n");
    list = cc_build_interfaces (list, tree);
    cc_output_itypes (list, ci);
  }

  /** C++ **/
  if(rinfo->cpp_defs){
    fprintf(ci->fh, "/* C++ Methods */\n");
    fprintf(ci->fh, "#ifdef __cplusplus /* cc_output_methods */\n");
    cc_output_methods(tree, rinfo, ci);
    fprintf(ci->fh, "#endif /* __cplusplus */\n");
  }
}
