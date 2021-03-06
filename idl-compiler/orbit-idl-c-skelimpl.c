#include "config.h"

#include "orbit-idl-c-backend.h"
#include "rtorb-idl-c-utils.h"

/* This file copied from the old IDL compiler orbit-c-skelimpl.c, with minimal changes. */

static void orbit_cbe_write_skelimpl(FILE *outfile, IDL_tree tree, const char *hdrname);
static OIDL_Run_Info *info=0;

void
orbit_idl_output_c_skelimpl(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci)
{
  info = rinfo;
  orbit_cbe_write_skelimpl(ci->fh, tree, ci->base_name);
}

#include <ctype.h>
#include <glib.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* Abbreviations used here:
   "cbe" stands for "C backend"
   "hdr" -> "header" (duh :)
   "of" -> "output file"
   "ns" -> "name space"
*/

typedef struct {
  FILE *of;
  IDL_tree tree;
  enum { PASS_SERVANTS, PASS_PROTOS, PASS_EPVS, PASS_VEPVS,
	 PASS_IMPLSTUBS, PASS_CPP, PASS_LAST } pass;
} CBESkelImplInfo;

const char *passnames[] = {
  "App-specific servant structures",
  "Implementation stub prototypes",
  "epv structures",
  "vepv structures",
  "Stub implementations",
  "Boohoo!"
};

static void orbit_cbe_ski_process_piece(CBESkelImplInfo *ski);
static void cbe_ski_do_list(CBESkelImplInfo *ski);
static void cbe_ski_do_inherited_attr_dcl(CBESkelImplInfo *ski, IDL_tree current_interface);
static void cbe_ski_do_attr_dcl(CBESkelImplInfo *ski);
static void cbe_ski_do_inherited_op_dcl(CBESkelImplInfo *ski, IDL_tree current_interface);
static void cbe_ski_do_op_dcl(CBESkelImplInfo *ski);
static void cbe_ski_do_param_dcl(CBESkelImplInfo *ski);
static void cbe_ski_do_interface(CBESkelImplInfo *ski);
static void cbe_ski_do_module(CBESkelImplInfo *ski);

static void cbe_ski_do_param_dcl2(CBESkelImplInfo *ski);
static void cbe_ski_do_param_dcl3(CBESkelImplInfo *ski);

static void
orbit_cbe_write_skelimpl(FILE *outfile, IDL_tree tree, const char *hdrname)
{
  CBESkelImplInfo ski = {NULL, NULL, PASS_SERVANTS};

  ski.of = outfile;
  ski.tree = tree;

  g_return_if_fail(IDL_NODE_TYPE(tree) == IDLN_LIST);

  fprintf(outfile, "/* This is a template file generated by command */\n");
#if 0
  fprintf(outfile, "/* orbit-idl-2 --skeleton-impl %s.idl */\n", hdrname);
#endif
  fprintf(outfile, "/* rtorb-idl --skeleton-impl %s.idl */\n", hdrname);
  fprintf(outfile, "/* User must edit this file, inserting servant  */\n");
  fprintf(outfile, "/* specific code between markers. */\n\n");

  fprintf(outfile, "#include \"%s.h\"\n", hdrname);

  for(ski.pass = PASS_SERVANTS; ski.pass < PASS_LAST; ski.pass++) {
    fprintf(outfile, "\n/*** %s ***/\n", passnames[ski.pass]);
    orbit_cbe_ski_process_piece(&ski);
  }
}

static void
orbit_cbe_ski_process_piece(CBESkelImplInfo *ski)
{
  /* I'm not implementing this as an array of function pointers
     because we may want to do special logic for particular cases in
     the future. Hope this is clear enough. -ECL */

  switch(IDL_NODE_TYPE(ski->tree)) {
  case IDLN_ATTR_DCL:
    cbe_ski_do_attr_dcl(ski);
    break;
  case IDLN_INTERFACE:
    cbe_ski_do_interface(ski);
    break;
  case IDLN_LIST:
    cbe_ski_do_list(ski);
    break;
  case IDLN_MODULE:
    cbe_ski_do_module(ski);
    break;
  case IDLN_OP_DCL:
    cbe_ski_do_op_dcl(ski);
    break;
  case IDLN_PARAM_DCL:
    cbe_ski_do_param_dcl(ski);
    break;
  default:
    break;
  }
}

static void
cbe_ski_do_module(CBESkelImplInfo *ski)
{
  CBESkelImplInfo subski = *ski;
  subski.tree = IDL_MODULE(ski->tree).definition_list;
  cbe_ski_do_list(&subski);
}

static void
cbe_ski_do_list(CBESkelImplInfo *ski)
{
  CBESkelImplInfo subski = *ski;
  IDL_tree curitem;

  for(curitem = ski->tree; curitem; curitem = IDL_LIST(curitem).next) {
    subski.tree = IDL_LIST(curitem).data;
    orbit_cbe_ski_process_piece(&subski);
  }
}

static void cbe_ski_do_attr_dcl_internal(CBESkelImplInfo *ski, IDL_tree current_interface, gboolean inherited)
{
  IDL_tree curop, curitem;
  GString *attrname = g_string_new(NULL);
  CBESkelImplInfo subski = *ski;

  if(ski->pass == PASS_SERVANTS) {
    for(curitem = IDL_ATTR_DCL(ski->tree).simple_declarations; curitem;
	curitem = IDL_LIST(curitem).next) {
      orbit_cbe_write_typespec(ski->of, IDL_ATTR_DCL(ski->tree).param_type_spec);
      fprintf(ski->of, " attr_%s;\n", IDL_IDENT(IDL_LIST(curitem).data).str);
    }
  }

  for(curitem = IDL_ATTR_DCL(ski->tree).simple_declarations;
      curitem; curitem = IDL_LIST(curitem).next) {

    /* Fake the attribute get/set methods as operation declarations */
    IDL_tree ident, ns_data_save;
    int i;

    for (i = 0; i < 2; ++i) {
	    if (i && IDL_ATTR_DCL(ski->tree).f_readonly)
		    break;
	    /* Output the operation on this attribute */
	    g_string_printf(attrname, i ? "_set_%s" : "_get_%s",
			     IDL_IDENT(IDL_LIST(curitem).data).str);
	    ident = IDL_ident_new(g_strdup(attrname->str));
	    
	    /* Tell the ident where our namespace node is, and request a return value
	       if this is the _get operation */
	    IDL_IDENT_TO_NS(ident) = IDL_IDENT_TO_NS(IDL_LIST(curitem).data);
	    curop = IDL_op_dcl_new(0, i == 0 ?
				   IDL_ATTR_DCL(ski->tree).param_type_spec : NULL,
				   ident, NULL, NULL, NULL);
	    
	    curop->up = ski->tree->up;
	    subski.tree = curop;
	    
	    /* Save the namespace ident (IDL_GENTREE data) reference, assign
	       back to the temporary tree, output the operation, then restore
	       the namespace ident link */
	    ns_data_save = IDL_GENTREE(IDL_IDENT_TO_NS(IDL_LIST(curitem).data)).data;
	    IDL_GENTREE(IDL_IDENT_TO_NS(IDL_LIST(curitem).data)).data = ident;

	    if (i) {
		    /* The set routine also needs the value, so we
		       temporarily add that to the operation
		       declaration */
		    IDL_OP_DCL(curop).parameter_dcls = IDL_list_new(
			    IDL_param_dcl_new(IDL_PARAM_IN,
					      IDL_ATTR_DCL(ski->tree).param_type_spec,
					      IDL_ident_new(g_strdup("value"))));
	    }
	    
	    if(inherited==TRUE)
	      cbe_ski_do_inherited_op_dcl(&subski, current_interface);
	    else
	      orbit_cbe_ski_process_piece(&subski);

	    /* Restore the fake link to the original in the namespace */
	    IDL_GENTREE(IDL_IDENT_TO_NS(IDL_LIST(curitem).data)).data = ns_data_save;

	    if (i) {
		    /* Free only what we've created for the fake node, so remove 
		       the attribute node element and then free the rest */
		    IDL_PARAM_DCL(IDL_LIST(
			    IDL_OP_DCL(curop).parameter_dcls).data).param_type_spec = NULL;
	    }
	    
	    /* Remove what we've "borrowed" from ATTR_DCL from the
	       fake curop node then free the rest */
	    IDL_OP_DCL(curop).op_type_spec = NULL;
	    IDL_tree_free(curop);
    }
  }

  g_string_free(attrname, TRUE);
}

static void cbe_ski_do_attr_dcl(CBESkelImplInfo *ski)
{
  cbe_ski_do_attr_dcl_internal(ski, NULL, FALSE);
}

void
cbe_ski_do_inherited_attr_dcl(CBESkelImplInfo *ski, IDL_tree current_interface)
{
  cbe_ski_do_attr_dcl_internal(ski, current_interface, TRUE);
}

static void cbe_ski_do_pass_epvs(CBESkelImplInfo *ski)
{
  CBESkelImplInfo subski = *ski;
  
  IDL_tree curitem, op;
  char *id = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(IDL_OP_DCL(ski->tree).ident),
			       "_", 0);

  fprintf(ski->of, "    (");
  orbit_cbe_write_param_typespec(ski->of, ski->tree);

  fprintf(ski->of, " (*)(void *, ");
    
  op = ski->tree;
  for(curitem = IDL_OP_DCL(ski->tree).parameter_dcls;
      curitem; curitem = IDL_LIST(curitem).next) {
    subski.tree = IDL_LIST(curitem).data;
    if(IDL_NODE_TYPE(subski.tree) == IDLN_PARAM_DCL) {
      cbe_ski_do_param_dcl3(&subski);
    }
  }

  fprintf(ski->of, "CORBA_Environment *))&impl_%s,\n", id);
  g_free(id);
}

static void
cbe_ski_do_op_dcl(CBESkelImplInfo *ski)
{
  /* If you fix anything here, please also fix it in
     cbe_ski_interface_print_epv_for_op(), which is almost a
     cut-and-paste of this routine */

  char *id, *id2, *id3, *func;
  IDL_tree curitem, op, op1;
  int level;
  CBESkelImplInfo subski = *ski;

  switch(ski->pass) {
  case PASS_PROTOS:
  case PASS_IMPLSTUBS:
#if 0
    fprintf(ski->of, "static ");
#endif
    orbit_cbe_write_param_typespec(ski->of, ski->tree);
    
    id = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(IDL_OP_DCL(ski->tree).ident), "_", 0);
    
    curitem = IDL_get_parent_node(ski->tree, IDLN_INTERFACE, &level);

    g_assert(curitem);
    id2 = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(IDL_INTERFACE(curitem).ident), "_", 0);

    id3 = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(IDL_INTERFACE(curitem).ident), "::", 0);
 
    func = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(IDL_OP_DCL(ski->tree).ident), "_", ski->tree->refs);

    fprintf(ski->of, "\nimpl_%s(\n  impl_POA_%s *servant,\n", id, id2);
#if 0
    g_free(id); g_free(id2);
#endif
    
    op = ski->tree;
    for(curitem = IDL_OP_DCL(ski->tree).parameter_dcls; curitem; curitem = IDL_LIST(curitem).next) {
      fprintf(ski->of, "  ");
      subski.tree = IDL_LIST(curitem).data;
      orbit_cbe_ski_process_piece(&subski);
    }


    if(IDL_OP_DCL(op).context_expr){
      fprintf(ski->of, "  CORBA_Context ctx,\n");
    }

    fprintf(ski->of, "  CORBA_Environment *ev)");

    if(ski->pass == PASS_IMPLSTUBS) {
      fprintf(ski->of, "\n{\n");

      if(IDL_OP_DCL(op).op_type_spec) {
	fprintf(ski->of, "  ");
	orbit_cbe_write_param_typespec(ski->of, ski->tree);

	fprintf(ski->of, " retval;\n");

        if(info->cpp_defs){
	  fprintf(ski->of, "  memset(ev, 0, sizeof(CORBA_Environment));\n");
          fprintf(ski->of, "#if __cplusplus\n");
	  fprintf(ski->of, "    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);\n" );
	  fprintf(ski->of, "    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object*>(object->impl_obj);\n");
	  fprintf(ski->of, "    %s__impl *impl = dynamic_cast<%s__impl *>(obj_);\n",id3, id3 );

	  char *typestr = orbit_cbe_get_typespec_str2(IDL_OP_DCL(op).op_type_spec, OIDL_TYPENAME_CPP_FULL);
	  fprintf(ski->of, "    %s_var var = impl->%s(", typestr, func);
	  g_free(typestr);

#if 1
          op1 = ski->tree;
          for(curitem = IDL_OP_DCL(ski->tree).parameter_dcls; curitem; curitem = IDL_LIST(curitem).next) {
            subski.tree = IDL_LIST(curitem).data;
            if(IDL_NODE_TYPE(subski.tree) == IDLN_PARAM_DCL) {
	      char  * str = orbit_cbe_get_typespec_str2(subski.tree, OIDL_TYPENAME_CPP_FULL);
	      fprintf(ski->of, "%s_", str); 
	      switch(IDL_PARAM_DCL(subski.tree).attr){
	        case IDL_PARAM_IN:
	          fprintf(ski->of, "CInArg");
                  break;
	        case IDL_PARAM_OUT:
	          fprintf(ski->of, "COutArg");
                  break;
	        case IDL_PARAM_INOUT:
	          fprintf(ski->of, "CInOutArg");
                  break;
	        default:
	          g_assert(0);
                  break;
	      }
	      fprintf(ski->of, "(");
	      cbe_ski_do_param_dcl2(&subski);
	      fprintf(ski->of, ")");
	      g_free(str);
            }
            if( IDL_LIST(curitem).next){  fprintf(ski->of, ", "); }
          }
#endif
          char * typespec_str = rtorb_cbe_get_param_typespec_str(ski->tree);

	  fprintf(ski->of, ");\n");

	  if(!strcmp(typespec_str,"CORBA_Object")){
	    fprintf(ski->of, "    retval = var->_impl;\n");
	  }else if(!strcmp(typespec_str,"CORBA_string")){
	    fprintf(ski->of, "    retval = var._retn();\n");
	  }else if( rtorb_cbe_get_param_typespec(ski->tree) == IDLN_INTERFACE){
	    fprintf(ski->of, "    retval = var.impl();\n");
	  }else if( rtorb_cbe_get_param_typespec(ski->tree) == IDLN_TYPE_STRUCT){
	    fprintf(ski->of, "    retval = var._retn();\n");
	  }else if( rtorb_cbe_get_param_typespec(ski->tree) == IDLN_TYPE_SEQUENCE){
	    fprintf(ski->of, "    retval = var->_retn();\n");
          }else{
#if 1
	    fprintf(ski->of, "  /* type = %d */\n", rtorb_cbe_get_param_typespec(ski->tree));
#endif
	    fprintf(ski->of, "    retval = var;\n");

            if(IDL_NODE_TYPE(ski->tree) == IDLN_OP_DCL){
              IDL_tree ts = IDL_OP_DCL(ski->tree).op_type_spec;
              IDL_tree typedef_spec = orbit_cbe_get_typespec (ts);
              if(IDL_NODE_TYPE(typedef_spec) == IDLN_TYPE_SEQUENCE) {
		;
#if 0
	        fprintf(ski->of, "RtORB_free(&var[0], \"retval Seq clear\");\n");
#endif
              }
            }
          }
	  fprintf(ski->of, "    ev->_cpp_flag = 1;\n");
	  fprintf(ski->of, "#else /*  C  */\n");
	  fprintf(ski->of, "    /* ------   insert method code here   ------ */\n");
	  fprintf(ski->of, "    /* ------ ---------- end ------------ ------ */\n");
          fprintf(ski->of, "#endif /* __cplusplus */ ");
 	}else{
	fprintf(ski->of, "    /* ------   insert method code here   ------ */\n");
	fprintf(ski->of, "    /* ------ ---------- end ------------ ------ */\n");
	}
	fprintf(ski->of, "\n  return retval;\n");
      }
      else
      {	
        if(info->cpp_defs){
  	  fprintf(ski->of, "#if __cplusplus\n");
	  fprintf(ski->of, "    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);\n" );
	  fprintf(ski->of, "    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object *>(object->impl_obj);\n");
	  fprintf(ski->of, "    %s__impl *impl = dynamic_cast<%s__impl *>(obj_);\n",id3, id3 );
#if 0
	  fprintf(ski->of, "    obj->%s(\n", strchr(id,'_') + 1);
#endif
	  fprintf(ski->of, "    impl->%s(", func);
#if 1
          op1 = ski->tree;
          for(curitem = IDL_OP_DCL(ski->tree).parameter_dcls; curitem; curitem = IDL_LIST(curitem).next) {
            subski.tree = IDL_LIST(curitem).data;
            if(IDL_NODE_TYPE(subski.tree) == IDLN_PARAM_DCL) {
	      char *str = orbit_cbe_get_typespec_str2(subski.tree, OIDL_TYPENAME_CPP_FULL);
	
	      fprintf(ski->of, "%s_", str); 
	      switch(IDL_PARAM_DCL(subski.tree).attr){
	        case IDL_PARAM_IN:
	          fprintf(ski->of, "CInArg"); break;
	       case IDL_PARAM_OUT:
	          fprintf(ski->of, "COutArg"); break;
	       case IDL_PARAM_INOUT:
	          fprintf(ski->of, "CInOutArg"); break;
	       default:
	          g_assert(0); break;
	      }
	      fprintf(ski->of, "(");
	      cbe_ski_do_param_dcl2(&subski);
	      fprintf(ski->of, ")");
	      g_free(str);
            }
            if( IDL_LIST(curitem).next) fprintf(ski->of, ", ");
          }
#endif
  	  fprintf(ski->of, ");\n");
	  fprintf(ski->of, "#else /* C */\n");
	  fprintf(ski->of, "    /* ------   insert method code here   ------ */\n");
	  fprintf(ski->of, "    /* ------ ---------- end ------------ ------ */\n");
	  fprintf(ski->of, "#endif /*__cplusplus */\n");
        }else{
	  fprintf(ski->of, "    /* ------   insert method code here   ------ */\n");
	  fprintf(ski->of, "    /* ------ ---------- end ------------ ------ */\n");
        }

      }
      fprintf(ski->of, "}\n\n");

    } else /* PASS_PROTOS */
      fprintf(ski->of, ";\n\n");

    g_free(id); g_free(id2); g_free(id3);
    g_free(func);

    break; /* End PASS_PROTOS | PASS_IMPLSTUBS */

  case PASS_EPVS:
#if 0
    {
      IDL_tree curitem, op;
      id = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(IDL_OP_DCL(ski->tree).ident),
				 "_", 0);

      fprintf(ski->of, "  (");
      orbit_cbe_write_param_typespec(ski->of, ski->tree);

      fprintf(ski->of, "(*)(void *, ");
    
      op = ski->tree;
      for(curitem = IDL_OP_DCL(ski->tree).parameter_dcls; curitem; curitem = IDL_LIST(curitem).next) {
        subski.tree = IDL_LIST(curitem).data;
        if(IDL_NODE_TYPE(subski.tree) == IDLN_PARAM_DCL) {
          cbe_ski_do_param_dcl3(&subski);
        }
      }

      fprintf(ski->of, "CORBA_Environment *))&impl_%s,\n", id);
      g_free(id);
    }
#endif 
    cbe_ski_do_pass_epvs(ski);
    break;
  default:
    break;
  }
}

static void
cbe_ski_do_inherited_op_dcl(CBESkelImplInfo *ski, IDL_tree current_interface)
{
  char *id, *id2, *id3, *func;
  IDL_tree ident, curitem, intf, op, op1;
  int level;
  CBESkelImplInfo subski = *ski;

  id = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(IDL_INTERFACE(current_interface).ident), "_", 0);
  intf = IDL_get_parent_node(ski->tree, IDLN_INTERFACE, NULL);
  id2 = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(IDL_INTERFACE(intf).ident), "_", 0);

  id3 = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(IDL_INTERFACE(intf).ident), "::", 0);
  func = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(IDL_OP_DCL(ski->tree).ident), "_", ski->tree->refs);

  ident=IDL_OP_DCL(ski->tree).ident;
  g_assert(ident);

  switch(ski->pass) {
  case PASS_PROTOS:
  case PASS_IMPLSTUBS:
#if 0
    fprintf(ski->of, "static ");
#endif
    orbit_cbe_write_param_typespec(ski->of, ski->tree);
    
    curitem = IDL_get_parent_node(ski->tree, IDLN_INTERFACE, &level);
    g_assert(curitem);

    fprintf(ski->of, "\nimpl_%s_%s(\n    impl_POA_%s *servant,\n", id, IDL_IDENT(ident).str, id);
    
    op = ski->tree;
    for(curitem = IDL_OP_DCL(ski->tree).parameter_dcls; curitem; curitem = IDL_LIST(curitem).next) {
      subski.tree = IDL_LIST(curitem).data;
      fprintf(ski->of, "    ");
      orbit_cbe_ski_process_piece(&subski);
    }

    if(IDL_OP_DCL(op).context_expr){
      fprintf(ski->of, "    CORBA_Context ctx,\n");
    }

    fprintf(ski->of, "    CORBA_Environment *ev)");

    if(ski->pass == PASS_IMPLSTUBS) {
      fprintf(ski->of, "\n{\n");
      fprintf(ski->of, "  memset(ev, 0, sizeof(CORBA_Environment));\n");
      if(IDL_OP_DCL(op).op_type_spec) {
	fprintf(ski->of, "  ");
	orbit_cbe_write_param_typespec(ski->of, ski->tree);
	fprintf(ski->of, " retval;\n");
        if(info->cpp_defs){
          fprintf(ski->of, "#if __cplusplus\n");
 	  fprintf(ski->of, "    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);\n" );
	  fprintf(ski->of, "    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object*>(object->impl_obj);\n");
	  fprintf(ski->of, "    %s__impl *impl = dynamic_cast<%s__impl *>(obj_);\n",id3, id3 );

	  char *typestr = orbit_cbe_get_typespec_str2(IDL_OP_DCL(op).op_type_spec, OIDL_TYPENAME_CPP_FULL);
	  fprintf(ski->of, "    %s_var var = impl->%s(", typestr, func);
	  g_free(typestr);

#if 1
          op1 = ski->tree;
          for(curitem = IDL_OP_DCL(ski->tree).parameter_dcls; curitem; curitem = IDL_LIST(curitem).next) {
            subski.tree = IDL_LIST(curitem).data;
            if(IDL_NODE_TYPE(subski.tree) == IDLN_PARAM_DCL) {
 	      char  * str = orbit_cbe_get_typespec_str2(subski.tree, OIDL_TYPENAME_CPP_FULL);
	      fprintf(ski->of, "%s_", str); 

	      switch(IDL_PARAM_DCL(subski.tree).attr){
	        case IDL_PARAM_IN:
	          fprintf(ski->of, "CInArg"); break;
	        case IDL_PARAM_OUT:
	          fprintf(ski->of, "COutArg"); break;
	        case IDL_PARAM_INOUT:
	          fprintf(ski->of, "CInOutArg"); break;
	        default:
	          g_assert(0); break;
	      }
	      fprintf(ski->of, "(");
	      cbe_ski_do_param_dcl2(&subski);
	      fprintf(ski->of, ")");
	      g_free(str);
            }
            if( IDL_LIST(curitem).next) fprintf(ski->of, ", ");
          }
#endif
          char * typespec_str = rtorb_cbe_get_param_typespec_str(ski->tree);

	  fprintf(ski->of, " );\n");

	  if(!strcmp(typespec_str,"CORBA_Object")){
	    fprintf(ski->of, "  retval = var->_impl;\n");
	  }else if(!strcmp(typespec_str,"CORBA_string")){
	    fprintf(ski->of, "  retval = var._retn();\n");
	  }else if(rtorb_cbe_get_param_typespec(ski->tree) == IDLN_INTERFACE){
	    fprintf(ski->of, "  retval = var.impl();\n");
	  }else if(rtorb_cbe_get_param_typespec(ski->tree) == IDLN_TYPE_STRUCT){
	    fprintf(ski->of, "  retval = var._retn();\n");
	  }else if(rtorb_cbe_get_param_typespec(ski->tree) == IDLN_TYPE_SEQUENCE){
	    fprintf(ski->of, "  retval = var->_retn();\n");
          }else{
	    fprintf(ski->of, "  retval = var;\n");

            if(IDL_NODE_TYPE(ski->tree) == IDLN_OP_DCL){
              IDL_tree ts = IDL_OP_DCL(ski->tree).op_type_spec;
              IDL_tree typedef_spec = orbit_cbe_get_typespec (ts);
              if(IDL_NODE_TYPE(typedef_spec) == IDLN_TYPE_SEQUENCE) {
		  ;
#if 0
	          fprintf(ski->of, "  RtORB_free(&var[0], \"retval Seq clear\");\n");
#endif
              }
            }
          }
	  fprintf(ski->of, "  ev->_cpp_flag = 1;\n");
	  fprintf(ski->of, "#else /*  C  */\n");
	  fprintf(ski->of, "    /* ------   insert method code here   ------ */\n");
	  fprintf(ski->of, "    /* ------ ---------- end ------------ ------ */\n");
          fprintf(ski->of, "#endif /* __cplusplus */ ");
	  fprintf(ski->of, "\n  return retval;\n");
        }else{
	  fprintf(ski->of, "    /* ------   insert method code here   ------ */\n");
	  fprintf(ski->of, "    /* ------ ---------- end ------------ ------ */\n");
        }
      } else {	
	fprintf(ski->of, "    /* ------   insert method code here   ------ */\n");
	fprintf(ski->of, "    /* ------ ---------- end ------------ ------ */\n");
      }
      fprintf(ski->of, "}\n\n");
    } else { /* PASS_PROTOS */
      fprintf(ski->of, ";\n\n");
    }
    break; /* End PASS_PROTOS | PASS_IMPLSTUBS */

  case PASS_EPVS:
    cbe_ski_do_pass_epvs(ski);
#if 0
    ident=IDL_OP_DCL(ski->tree).ident;
    g_assert(ident);

    fprintf(ski->of, "(void *)&impl_%s_%s,\n", id, IDL_IDENT(ident).str);
    /*
    fprintf(ski->of, "(void (*)(void *, " );
    fprintf(ski->of, "CORBA_Environment*))&impl_%s_%s,\n", id, IDL_IDENT(ident).str);
    */
#endif
    break;
  default:
    break;
  }

  g_free(id);
  g_free(id2);
}

static void
cbe_ski_do_param_dcl(CBESkelImplInfo *ski)
{
  orbit_cbe_write_param_typespec(ski->of, ski->tree);
  fprintf(ski->of, " %s,\n", IDL_IDENT(IDL_PARAM_DCL(ski->tree).simple_declarator).str);
}

static void
cbe_ski_do_param_dcl2(CBESkelImplInfo *ski)
{
/*
  orbit_cbe_write_param_typespec(ski->of, ski->tree);
*/
  fprintf(ski->of, "%s ", IDL_IDENT(IDL_PARAM_DCL(ski->tree).simple_declarator).str);
}

static void
cbe_ski_do_param_dcl3(CBESkelImplInfo *ski)
{
  orbit_cbe_write_param_typespec(ski->of, ski->tree);
  fprintf(ski->of, ", ");
}
static void
cbe_ski_do_interface_vepv_entry(IDL_tree interface, CBESkelImplInfo *ski)
{
  char *id, *inherit_id;

  if(interface==ski->tree) {
    id = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(IDL_INTERFACE(ski->tree).ident), "_", 0);
    fprintf(ski->of, "    &impl_%s_epv,\n", id);
    g_free(id);
    return;
  }

  id = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(IDL_INTERFACE(ski->tree).ident), "_", 0);
  inherit_id = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(IDL_INTERFACE(interface).ident),
			       "_", 0);
  fprintf(ski->of, "    &impl_%s_%s_epv,\n", id, inherit_id);

  g_free(id);
  g_free(inherit_id);
}

static void
cbe_ski_do_inherited_methods(IDL_tree interface, CBESkelImplInfo *ski)
{
  CBESkelImplInfo subski= *ski;
  IDL_tree curitem;
  char *id = NULL, *inherit_id = NULL; /* Quiet gcc */

  if(interface==ski->tree)
    return;

  if(ski->pass==PASS_EPVS) {
    id = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(IDL_INTERFACE(ski->tree).ident),
      "_", 0);
    inherit_id = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(IDL_INTERFACE(interface).ident),
      "_", 0);
    fprintf(ski->of, "static POA_%s__epv impl_%s_%s_epv = \n{\n     NULL, /* _private */\n",
      inherit_id, id, inherit_id);
  }

  for(curitem = IDL_INTERFACE(interface).body; curitem; curitem=IDL_LIST(curitem).next) {
    subski.tree=IDL_LIST(curitem).data;

    switch(IDL_NODE_TYPE(IDL_LIST(curitem).data)) {
	case IDLN_OP_DCL:
		cbe_ski_do_inherited_op_dcl(&subski, ski->tree);
		break;
	case IDLN_ATTR_DCL:
		cbe_ski_do_inherited_attr_dcl(&subski, ski->tree);
		break;
	default:
	  break;
    }
  }

  if(ski->pass==PASS_EPVS) {
    fprintf(ski->of, "};\n\n");

    g_free(id);
    g_free(inherit_id);
  }
}

static void
cbe_ski_do_interface(CBESkelImplInfo *ski)
{
  char *id, *id2;
  CBESkelImplInfo subski = *ski;

  id = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(IDL_INTERFACE(ski->tree).ident), "_", 0);
  id2 = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(IDL_INTERFACE(ski->tree).ident), "::", 0);

  switch(ski->pass) {
  case PASS_SERVANTS:
    fprintf(ski->of, "typedef struct {\n    POA_%s servant;\n    PortableServer_POA poa;\n", id);
    subski.tree = IDL_INTERFACE(ski->tree).body;
    cbe_ski_do_list(&subski);

    IDL_tree_traverse_parents(ski->tree, (GFunc)&cbe_ski_do_inherited_methods,
    				ski);
    fprintf(ski->of, "\n    /* ------ add private attributes here ------ */\n");
    if(info->cpp_defs){
      fprintf(ski->of, "#if __cplusplus\n");
      fprintf(ski->of, "    CORBA::Object_var impl_obj;\n");
      fprintf(ski->of, "#endif /* __cplusplus */\n");
    }
    fprintf(ski->of, "    /* ------ ---------- end ------------ ------ */\n\n");
    fprintf(ski->of, "} impl_POA_%s;\n\n", id);
    break;
  case PASS_EPVS:
    fprintf(ski->of,
	    "static PortableServer_ServantBase__epv impl_%s_base_epv = {\n",
	    id);
    fprintf(ski->of, "    NULL,             /* _private data */\n");
    fprintf(ski->of, "    (void (*)(void*, CORBA_Environment*))& impl_%s__destroy, /* finalize routine */\n", id);
    fprintf(ski->of, "    NULL,             /* default_POA routine */\n");
    fprintf(ski->of, "};\n\n");

    fprintf(ski->of, "static POA_%s__epv impl_%s_epv = {\n    NULL, /* _private */\n", id, id);
    subski.tree = IDL_INTERFACE(ski->tree).body;
    cbe_ski_do_list(&subski);
    fprintf(ski->of, "};\n\n");
    IDL_tree_traverse_parents(ski->tree, (GFunc)&cbe_ski_do_inherited_methods,
    				ski);
    break;
  case PASS_VEPVS:
    fprintf(ski->of, "static POA_%s__vepv impl_%s_vepv = {\n", id, id);
    fprintf(ski->of, "    &impl_%s_base_epv,\n", id);
    IDL_tree_traverse_parents(ski->tree, (GFunc)&cbe_ski_do_interface_vepv_entry,
			      ski);
    fprintf(ski->of, "};\n\n");
    break;
  case PASS_IMPLSTUBS:

    /*
       creat Servant
     080205 weak or static?
    */
#if GNU_WEAK
    fprintf(ski->of, "__attribute__((weak)) CORBA_%s \nimpl_%s__create(PortableServer_POA poa, CORBA_Environment *ev)\n", id, id);
#else
    fprintf(ski->of, "CORBA_%s \nimpl_%s__create(PortableServer_POA poa, CORBA_Environment *ev)\n", id, id);
#endif
    fprintf(ski->of, "{\n  CORBA_%s retval;\n  impl_POA_%s *newservant;\n  PortableServer_ObjectId objid;\n\n", id, id);
    fprintf(ski->of, "  newservant = (impl_POA_%s *)RtORB_calloc(1, sizeof(impl_POA_%s),\" create...\");\n", id, id);
    fprintf(ski->of, "  newservant->servant.vepv = &impl_%s_vepv;\n", id);
    fprintf(ski->of, "  newservant->poa = poa;\n");

    fprintf(ski->of, "  POA_%s__init((PortableServer_Servant)newservant, ev);\n", id);
    
    fprintf(ski->of, "   /* Before servant is going to be activated all\n");
    fprintf(ski->of, "    * private attributes must be initialized.  */\n"); 
    fprintf(ski->of, "\n");
    fprintf(ski->of, "   /* ------ init private attributes here ------ */\n");
    fprintf(ski->of, "   /* ------ ---------- end ------------- ------ */\n");
    fprintf(ski->of, "\n");
    fprintf(ski->of, "  objid = PortableServer_POA_activate_object(poa, newservant, ev);\n");
    fprintf(ski->of, "  RtORB_free(objid, \" objid\");\n");
    fprintf(ski->of, "  retval = PortableServer_POA_servant_to_reference(poa, newservant, ev);\n");
    fprintf(ski->of, "\n  return retval;\n}\n\n");

    fprintf(ski->of, "static void\nimpl_%s__destroy(impl_POA_%s *servant, CORBA_Environment *ev)\n{\n", id, id);
    fprintf(ski->of, "    CORBA_Object_release ((CORBA_Object) servant->poa, ev);\n\n");
    fprintf(ski->of, "    /* No further remote method calls are delegated to \n");
    fprintf(ski->of, "    * servant and you may free your private attributes. */\n");

    fprintf(ski->of, "   /* ------ free private attributes here ------ */\n");
    fprintf(ski->of, "   /* ------ ---------- end ------------- ------ */\n");
    fprintf(ski->of, "\n    POA_%s__fini((PortableServer_Servant)servant, ev);\n", id);
    fprintf(ski->of, "\n    RtORB_free (servant, \"servant\");\n");
    fprintf(ski->of, "}\n\n");

    /*
     creat Servant
    */
    fprintf(ski->of, "\n\n /*** create servent **/ \n");
    /*
      080205 weak or static?
    */
#if GNU_WEAK
    fprintf(ski->of, "__attribute__((weak)) impl_POA_ServantBase * \nimpl_%s__create_servant(PortableServer_POA poa, CORBA_Environment *ev)\n", id);
#else
    fprintf(ski->of, "impl_POA_ServantBase * \nimpl_%s__create_servant(PortableServer_POA poa, CORBA_Environment *ev)\n", id);
#endif
    fprintf(ski->of, "{\n");
#if 0
    fprintf(ski->of, "  CORBA_%s retval;\n", id);
#endif
    fprintf(ski->of, "  impl_POA_%s *newservant;\n", id);
#if 0
    fprintf(ski->of, "  PortableServer_ObjectId objid;\n");
#endif
    fprintf(ski->of, "\n");

    fprintf(ski->of, "  newservant = (impl_POA_%s *)RtORB_calloc(1, sizeof(impl_POA_%s), \"  create_servant\");\n", id, id);
    fprintf(ski->of, "  newservant->servant.vepv = &impl_%s_vepv;\n", id);
    fprintf(ski->of, "  newservant->poa = poa;\n");

    fprintf(ski->of, "  POA_%s__init((PortableServer_Servant)newservant, ev);\n", id);
    
    fprintf(ski->of, "   /* Before servant is going to be activated all\n");
    fprintf(ski->of, "    * private attributs must be initialized.  */\n"); 
    fprintf(ski->of, "\n");
    fprintf(ski->of, "   /* ------ init private attributes here ------ */\n");
    fprintf(ski->of, "   /* ------ ---------- end ------------- ------ */\n");
    fprintf(ski->of, "\n");
    fprintf(ski->of, "\n  return (impl_POA_ServantBase*)newservant;\n}\n\n");

    subski.tree = IDL_INTERFACE(ski->tree).body;
    cbe_ski_do_list(&subski);
    IDL_tree_traverse_parents(ski->tree, (GFunc)&cbe_ski_do_inherited_methods,
    				ski);
    break;
  case PASS_PROTOS:
    fprintf(ski->of, "static void impl_%s__destroy(\n  impl_POA_%s *servant,\n  CORBA_Environment *ev);\n\n", id, id);
    subski.tree = IDL_INTERFACE(ski->tree).body;
    cbe_ski_do_list(&subski);
    IDL_tree_traverse_parents(ski->tree, (GFunc)&cbe_ski_do_inherited_methods,
    				ski);
    break;
  case PASS_CPP:
    {
#if 0
    fprintf(ski->of, " class impl_%s_base_epv = \n{\n", id);
    fprintf(ski->of, "    NULL,             /* _private data */\n");
    fprintf(ski->of, "    (void (*)(void*, CORBA_Environment*))& impl_%s__destroy, /* finalize routine */\n", id);
    fprintf(ski->of, "    NULL,             /* default_POA routine */\n");
    fprintf(ski->of, "};\n\n");

    fprintf(ski->of, "static POA_%s__epv impl_%s_epv = \n{\n    NULL, /* _private */\n", id, id);
    subski.tree = IDL_INTERFACE(ski->tree).body;
    cbe_ski_do_list(&subski);
    fprintf(ski->of, "};\n");
    IDL_tree_traverse_parents(ski->tree, (GFunc)&cbe_ski_do_inherited_methods, ski);
#endif
    }
    break;
  default:
    break;
  }

  g_free(id);
  g_free(id2);
}
