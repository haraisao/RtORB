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

static void cc_type_code_decl(IDL_tree tree, OIDL_C_Info *ci);
void rtorb_idl_output_c_decls (IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci);

/*
  Output TypeCode prep of Sequence   
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
//  case IDLN_TYPE_DCL:
  case IDLN_TYPE_ENUM:
  case IDLN_TYPE_FIXED:
    if (tfd->step)
      cc_type_code_decl(tree, ci);
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

static void
cc_type_code_decl(IDL_tree tree, OIDL_C_Info *ci)
{
  char *ctmp;

  ctmp = orbit_cbe_get_typespec_str(tree);

  if ( ci->do_impl_hack ) {
    fprintf(ci->fh, "/***************************/\n");
    fprintf(ci->fh, "#ifndef  TC_DEF_TC_%s\n", ctmp);
    fprintf(ci->fh, "#  define  TC_DEF_TC_%s	1\n", ctmp);
    fprintf(ci->fh, "#  ifdef __cplusplus\n");
    fprintf(ci->fh, "#    define  Defined_%s	1\n", ctmp);
    fprintf(ci->fh, "#  endif /* __cplusplus */\n");
    fprintf(ci->fh, "#endif\n");
    fprintf(ci->fh, "/***************************/\n");
  }
  g_free(ctmp);
}

/*
  Output common file
  Main function to output XX-common.c

*/
void
rtorb_idl_output_c_decls (IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci)
{
  fprintf (ci->fh, OIDL_C_WARNING);

  fprintf(ci->fh, "#ifndef %s%s_H\n", rinfo->header_guard_prefix, ci->c_base_name);
  fprintf(ci->fh, "#define %s%s_H 1\n", rinfo->header_guard_prefix, ci->c_base_name);
  fprintf (ci->fh, "\n/** TypeCode definitions */\n");
  cc_output_typecodes (tree, ci);
  fprintf(ci->fh, "#endif /*  %s%s_H */\n", rinfo->header_guard_prefix, ci->c_base_name);
}
