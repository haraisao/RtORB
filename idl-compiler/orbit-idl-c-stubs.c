#include "config.h"

#include "orbit-idl-c-backend.h"

#include <string.h>
#include <assert.h>

void orbit_cbe_op_write_proto5 (FILE       *of, IDL_tree    op, const char *nom_prefix, gboolean    for_epv, gboolean arg_helper, OIDL_TypeNameType otype);

void orbit_cbe_op_write_proto2 (FILE       *of, IDL_tree    op, const char *nom_prefix, gboolean    for_epv);

void orbit_cbe_cpp_op_write_proto (FILE       *of, IDL_tree    op, const char *nom_prefix, gboolean    for_epv);


/*
  Output Stub
  Called by cs_output_stubs
*/
static void
cs_output_stub (IDL_tree     tree,
    OIDL_C_Info *ci,
    int         *idx)
{
  FILE     *of = ci->fh;
  char     *iface_id;
  char     *opname;
  gboolean  has_retval, has_args;

  g_return_if_fail (idx != NULL);

  iface_id = IDL_ns_ident_to_qstring (
      IDL_IDENT_TO_NS (IDL_INTERFACE (
        IDL_get_parent_node (tree, IDLN_INTERFACE, NULL)
          ).ident), "_", 0);
  opname = IDL_ns_ident_to_qstring (IDL_IDENT_TO_NS (IDL_OP_DCL (tree).ident), "_", 0);

  has_retval = IDL_OP_DCL (tree).op_type_spec != NULL;
  has_args   = IDL_OP_DCL (tree).parameter_dcls != NULL;

  orbit_cbe_op_write_proto (of, tree, "", FALSE);

  fprintf (of, "\n{\n");

  if (has_retval) {
    fprintf (of, "  ");
    orbit_cbe_write_param_typespec (of, tree);
    fprintf (of, " " ORBIT_RETVAL_VAR_NAME ";\n");
//    fprintf (of, "  void *_retval;\n");
  }

  /* remote invocation part */
  if (has_args){
    orbit_cbe_flatten_args (tree, of, "_args");
  }

/*
  if (has_retval) {
    fprintf (of, "  _retval = (void *)" ORBIT_RETVAL_VAR_NAME ";\n");
  }
*/
  fprintf (of, "  invokeMethod (_obj, "
     "&%s__imp.methods[%d], ", iface_id, *idx);

  if (has_retval)
    fprintf (of, "(void **)&_ORBIT_retval, ");
  else
    fprintf (of, "NULL, ");

  if (has_args)
    fprintf (of, "_args, ");
  else
    fprintf (of, "NULL, ");
#if 0
  if (IDL_OP_DCL (tree).context_expr)
    fprintf (ci->fh, "_ctx, ");
  else
    fprintf (ci->fh, "NULL, ");
#endif    
  fprintf (of, "ev );\n");

#if 0
  fprintf (of, "%s__classid, G_STRUCT_OFFSET (POA_%s__epv, %s),\n",
     iface_id, iface_id, IDL_IDENT (IDL_OP_DCL (tree).ident).str);
  fprintf (of, "(ORBitSmallSkeleton) _ORBIT_skel_small_%s);\n\n", opname);
#endif

  if (has_retval)
    fprintf (of, "  return " ORBIT_RETVAL_VAR_NAME ";\n");

  fprintf (of, "}\n");

  g_free (iface_id);

  (*idx)++;
}

/*
   Outout Stubs

*/
static void
cs_output_stubs (IDL_tree     tree,
     OIDL_C_Info *ci,
     int         *idx)
{
  if (!tree)
    return;


  switch (IDL_NODE_TYPE (tree)) {
    case IDLN_MODULE:
      cs_output_stubs (IDL_MODULE (tree).definition_list, ci, idx);
      break;
    case IDLN_LIST:
      {
        IDL_tree sub;

        for (sub = tree; sub; sub = IDL_LIST (sub).next){
          cs_output_stubs (IDL_LIST (sub).data, ci, idx);
        }
      }
      break;
    case IDLN_ATTR_DCL:
      {
        IDL_tree node;
      
        fprintf (ci->fh, "/*  Attributes  */\n");
        for (node = IDL_ATTR_DCL (tree).simple_declarations; node; node = IDL_LIST (node).next) {
          OIDL_Attr_Info *ai;
          ai = IDL_LIST (node).data->data;
  
          cs_output_stubs (ai->op1, ci, idx);

          if (ai->op2){
            cs_output_stubs (ai->op2, ci, idx);
          }
        }
        fprintf (ci->fh, "/*  End of Attributes  */\n");
      }
      break;
    case IDLN_INTERFACE:
      {
        int real_idx = 0;

        fprintf (ci->fh, "/*  Interface  */\n");
        cs_output_stubs (IDL_INTERFACE (tree).body, ci, &real_idx);
        fprintf (ci->fh, "/*  End of Interface  */\n");
      }
      break;
    case IDLN_OP_DCL:
      cs_output_stub (tree, ci, idx);
      break;
    default:
      break;
  }
}

/**
 for C++
**/

void
orbit_cbe_op_write_proto5 (FILE       *of,
        IDL_tree    op,
        const char *nom_prefix,
        gboolean    for_epv, gboolean arg_helper, OIDL_TypeNameType otype)
{
  IDL_tree  sub;
  char     *id;

  g_assert (IDL_NODE_TYPE(op) == IDLN_OP_DCL);

  id = IDL_ns_ident_to_qstring (
    IDL_IDENT_TO_NS (IDL_INTERFACE (
      IDL_get_parent_node (op, IDLN_INTERFACE, NULL)).ident), "_", 0);

  if (for_epv)
    fprintf (of, " (*%s%s)", nom_prefix ? nom_prefix : "",
       IDL_IDENT(IDL_OP_DCL(op).ident).str);
  else 
    fprintf (of, " %s%s_%s", nom_prefix ? nom_prefix : "",
       id, IDL_IDENT (IDL_OP_DCL (op).ident).str);

  g_free (id);
  fprintf (of, "(this->_impl,");

  for (sub = IDL_OP_DCL (op).parameter_dcls; sub; sub = IDL_LIST (sub).next) {
    IDL_tree parm = IDL_LIST (sub).data;

    char *arg = IDL_IDENT (IDL_PARAM_DCL (parm).simple_declarator).str;

    if (arg_helper) {
      char * typestr = orbit_cbe_get_typespec_str2(parm, otype);

      fprintf(of, "%s_", typestr);
      switch(IDL_PARAM_DCL(parm).attr) {
      case IDL_PARAM_IN:
        fprintf(of, "CppInArg"); break;
      case IDL_PARAM_OUT:
        fprintf(of, "CppOutArg"); break;
      case IDL_PARAM_INOUT:
        fprintf(of, "CppInOutArg"); break;
      default:
        assert(0); break;
      }
      fprintf(of, "(%s), ", arg);
        
      g_free(typestr);
    } else {
      fprintf (of, "%s, ", arg);
    }
  }
  fprintf (of, "&this->ev);\n");
}

void
orbit_cbe_op_write_proto2 (FILE       *of,
        IDL_tree    op,
        const char *nom_prefix,
        gboolean    for_epv)
{
     orbit_cbe_op_write_proto5(of, op, nom_prefix, for_epv, FALSE, OIDL_TYPENAME_C);  
}


void
orbit_cbe_cpp_op_write_proto (FILE       *of,
        IDL_tree    op,
        const char *nom_prefix,
        gboolean    for_epv)
{
  IDL_tree  sub;
  char     *id;

  g_assert (IDL_NODE_TYPE(op) == IDLN_OP_DCL);

  orbit_cbe_write_param_typespec3 (of, op, OIDL_TYPENAME_CPP_FULL);

  id = IDL_ns_ident_to_qstring (
    IDL_IDENT_TO_NS (IDL_INTERFACE (
      IDL_get_parent_node (op, IDLN_INTERFACE, NULL)).ident), "::", 0);

  fprintf (of, "\n %s%s::%s", nom_prefix ? nom_prefix : "",
       id, IDL_IDENT (IDL_OP_DCL (op).ident).str);

  g_free (id);
  fprintf (of, "(");


  for (sub = IDL_OP_DCL (op).parameter_dcls; sub; sub = IDL_LIST (sub).next) {
    IDL_tree parm = IDL_LIST (sub).data;
    orbit_cbe_write_param_typespec3 (of, parm, OIDL_TYPENAME_CPP_SIMPLE);
    fprintf (of, " %s", IDL_IDENT (IDL_PARAM_DCL (parm).simple_declarator).str);
    if(IDL_LIST (sub).next)fprintf (of, ", ");
  }

  fprintf (of, ")");
}

/*
   Output stub for C++

*/
static void
cs_output_cpp_stub (IDL_tree     tree,
    OIDL_C_Info *ci,
    int         *idx)
{
  FILE     *of = ci->fh;
  char     *iface_id;
  char     *opname;
  gboolean  has_retval, has_args;

  g_return_if_fail (idx != NULL);

  iface_id = IDL_ns_ident_to_qstring (
      IDL_IDENT_TO_NS (IDL_INTERFACE (
        IDL_get_parent_node (tree, IDLN_INTERFACE, NULL)
          ).ident), "::", 0);
  opname = IDL_ns_ident_to_qstring (IDL_IDENT_TO_NS (IDL_OP_DCL (tree).ident), "::", 0);

  has_retval = IDL_OP_DCL (tree).op_type_spec != NULL;
  has_args   = IDL_OP_DCL (tree).parameter_dcls != NULL;

  fprintf (of, "\n");
  orbit_cbe_cpp_op_write_proto (of, tree, "", FALSE);

  fprintf (of, "\n{\n");

  /* remote invocation part */

  IDL_tree ts = NULL;
  if (has_retval){
       ts = IDL_OP_DCL(tree).op_type_spec;

       char *str1 = orbit_cbe_write_param_typespec_str3(ts, DATA_RETURN, OIDL_TYPENAME_C);
       fprintf(of, "  %s tmp = ", str1);
  }
  orbit_cbe_op_write_proto5 (of, tree, "", FALSE, TRUE, OIDL_TYPENAME_CPP_FULL);
  if (has_retval) {
    char *typestr = orbit_cbe_get_typespec_str2(ts, OIDL_TYPENAME_CPP_FULL);
    fprintf (of, "  return %s_helper::_retnew(tmp);\n", typestr);
    g_free(typestr);
  } else {
    fprintf (of, "  return;\n ");
  } 

  fprintf (of, "}\n");

  g_free (iface_id);

  (*idx)++;
}

static void
cs_output_cpp_stubs (IDL_tree     tree,
     OIDL_C_Info *ci,
     int         *idx)
{
  if (!tree)
    return;

  switch (IDL_NODE_TYPE (tree))
  {
    case IDLN_MODULE:
      cs_output_cpp_stubs (IDL_MODULE (tree).definition_list, ci, idx);
      break;
    case IDLN_LIST:
      {
        IDL_tree sub;

        for (sub = tree; sub; sub = IDL_LIST (sub).next){
          cs_output_cpp_stubs (IDL_LIST (sub).data, ci, idx);
        }
      }
      break;
    case IDLN_ATTR_DCL:
      {
        IDL_tree node;
      
        for (node = IDL_ATTR_DCL (tree).simple_declarations; node; node = IDL_LIST (node).next) {
          OIDL_Attr_Info *ai;

          ai = IDL_LIST (node).data->data;
  
          cs_output_cpp_stubs (ai->op1, ci, idx);

          if (ai->op2){
            cs_output_cpp_stubs (ai->op2, ci, idx);
          }
        }
      }
      break;
    case IDLN_INTERFACE:
      {
        int real_idx = 0;

        char *id = IDL_ns_ident_to_qstring ( IDL_IDENT_TO_NS (IDL_INTERFACE (
              IDL_get_parent_node (tree, IDLN_INTERFACE, NULL)
               ).ident), "::", 0);

#if GNU_WEAK
        fprintf(ci->fh, "__attribute__((weak)) %s_ptr\n%s::_narrow(CORBA::Object_ptr obj)\n{\n",id, id);
#else
        fprintf(ci->fh, "%s_ptr\n%s::_narrow(CORBA::Object_ptr obj)\n{\n",id, id);
#endif
        fprintf(ci->fh, "  return CORBA_Narrow<%s>::narrow(obj);\n", id);
        fprintf(ci->fh, "}\n");

        cs_output_cpp_stubs (IDL_INTERFACE (tree).body, ci, &real_idx);

        g_free(id);
    
      }
      break;
    case IDLN_OP_DCL:
      cs_output_cpp_stub (tree, ci, idx);
      break;
    default:
      break;
  }
}

/*
   Output Stub (main)

*/
void
orbit_idl_output_c_stubs (IDL_tree       tree,
        OIDL_Run_Info *rinfo,
        OIDL_C_Info   *ci)
{
  fprintf (ci->fh, OIDL_C_WARNING);
  fprintf (ci->fh, "#include <string.h>\n");
  fprintf (ci->fh, "#define ORBIT2_STUBS_API\n");
  fprintf (ci->fh, "#include \"%s.h\"\n\n", ci->base_name);

  cs_output_stubs (tree, ci, NULL);

  if(rinfo->cpp_defs){
    fprintf (ci->fh, "\n\n#if __cplusplus\n");
    cs_output_cpp_stubs (tree, ci, NULL);
    fprintf (ci->fh, "\n#endif /* __cplusplus */\n");
  }
}
