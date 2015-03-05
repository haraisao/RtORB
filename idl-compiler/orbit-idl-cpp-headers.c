#include "config.h"
#include "orbit-idl-c-backend.h"
#include <string.h>

#define DEFINE_CPP_TYPE_START3(CI, CLS, IDSTR) \
  fprintf((CI)->fh, "/* %s(%d) */\n", __FILE__, __LINE__); \
  fprintf((CI)->fh, "#ifndef %s%s_cpp_type_defined\n", CLS, IDSTR); \
  fprintf((CI)->fh, "#define %s%s_cpp_type_defined\n\n", CLS, IDSTR); \

#define DEFINE_CPP_TYPE_START2(CI, CLS) DEFINE_CPP_TYPE_START3(CI, CLS, "")

#define DEFINE_CPP_TYPE_END3(CI, CLS, IDSTR) \
  fprintf((CI)->fh, "\n#endif /* %s%s_cpp_type_defined */\n\n", CLS, IDSTR); \

#define DEFINE_CPP_TYPE_END2(CI, CLS) DEFINE_CPP_TYPE_END3(CI, CLS, "")




static void
ch_output_cpp_type_enum(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci);

static void
ch_output_cpp_interface_stub(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci);

static void
ch_output_cpp_type_dcl_without_sequence(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci);


static void
ch_output_cpp_execption(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci);

void rtorb_cbe_cpp_op_write_proto (FILE       *of, IDL_tree    op, const char *nom_prefix, gboolean    flag);

void ch_output_cpp_skel_class_protos(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci);

void
ch_output_cpp_stub_class_protos(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci);

void ch_output_cpp_global_functions(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci);

void ch_output_cpp_global_functions(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci);

void ch_output_cpp_poa(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci, const char *mod_name);

void ch_output_cpp_skel_class_protos(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci)
{
  if(!tree) return;

  switch(IDL_NODE_TYPE(tree)) {
  case IDLN_MODULE:
    {
      char *id = IDL_ns_ident_to_qstring ( IDL_IDENT_TO_NS (IDL_INTERFACE (tree).ident), "::", 0);

      fprintf(ci->fh, "namespace %s {\n", id);

      ch_output_cpp_skel_class_protos(IDL_MODULE(tree).definition_list, rinfo, ci);

      fprintf(ci->fh, "};\n");
    }

    break;
  case IDLN_LIST:
    {
      IDL_tree sub;

      for(sub = tree; sub; sub = IDL_LIST(sub).next) {
	ch_output_cpp_skel_class_protos(IDL_LIST(sub).data, rinfo, ci);
      }
    }
    break;
  case IDLN_INTERFACE:
    {
      IDL_tree sub;
      char *cid = IDL_ns_ident_to_qstring ( IDL_IDENT_TO_NS (IDL_INTERFACE (tree).ident), "_", 0);
      char *id = IDL_ns_ident_to_qstring ( IDL_IDENT_TO_NS (IDL_INTERFACE (tree).ident), "::", tree->refs);
      char *idfull = IDL_ns_ident_to_qstring ( IDL_IDENT_TO_NS (IDL_INTERFACE (tree).ident), "::", 0);

      DEFINE_CPP_TYPE_START3(ci, cid, "_impl");

      fprintf(ci->fh, "class %s;\n", id);
      fprintf(ci->fh, "class %s__impl : \n", id);
      fprintf(ci->fh, "  public virtual CORBA::Object");

      for (sub = IDL_INTERFACE(tree).inheritance_spec; sub; sub = IDL_LIST(sub).next) {
      
	IDL_tree curr = IDL_LIST(sub).data;

	char *id_ = orbit_cbe_get_typespec_str2(curr, OIDL_TYPENAME_CPP_FULL);
	fprintf(ci->fh, ", public virtual %s__impl", id_);
	g_free(id_);
      }

      fprintf (ci->fh, "\n{\n  public:\n");

      fprintf(ci->fh, "    %s__impl() {}\n", id);


      for(sub = IDL_INTERFACE(tree).body; sub; sub = IDL_LIST(sub).next) {
	IDL_tree cur;

       fprintf(ci->fh, "    ");
	cur = IDL_LIST(sub).data;

	switch(IDL_NODE_TYPE(cur))
	  {
	  case IDLN_OP_DCL:

	    orbit_idl_check_oneway_op (cur);
	    rtorb_cbe_cpp_op_write_proto(ci->fh, cur, idfull, TRUE);
	    fprintf(ci->fh, ";\n");

	    break;
	  case IDLN_ATTR_DCL:
	    {
	      OIDL_Attr_Info *ai;
	      IDL_tree curitem;

	      for(curitem = IDL_ATTR_DCL(cur).simple_declarations; curitem;
		  curitem = IDL_LIST(curitem).next) {

		ai = IDL_LIST(curitem).data->data;
	      
		rtorb_cbe_cpp_op_write_proto(ci->fh, ai->op1, idfull, TRUE);
		fprintf(ci->fh, ";\n");
	      
		if(ai->op2) {
		  rtorb_cbe_cpp_op_write_proto(ci->fh, ai->op2, idfull, TRUE);
		  fprintf(ci->fh, ";\n");
		}

	      }
	    }
	    break;
	  default:
	    break;
	  }
      }
      fprintf(ci->fh, "};\n");
      DEFINE_CPP_TYPE_END3(ci, cid, "_impl");

      g_free (cid);
      g_free (id);
    }
    break;
  case IDLN_TYPE_STRUCT:
    break;
  case IDLN_EXCEPT_DCL:
    ch_output_cpp_execption(tree, rinfo, ci);
    break;

  case IDLN_TYPE_ENUM:
    ch_output_cpp_type_enum(tree, rinfo, ci);
    break;
#if 0
  case IDLN_TYPE_UNION:
    fprintf(stderr, "UNION\n");
/*
    ch_output_type_union (tree, rinfo, ci);
*/
    break;
#endif
  default:
    break;
  }
}


void
ch_output_cpp_stub_class_protos(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci)
{
  if(!tree) return;

  switch(IDL_NODE_TYPE(tree)) {
  case IDLN_MODULE:
    {
      char *id = IDL_ns_ident_to_qstring ( IDL_IDENT_TO_NS (IDL_INTERFACE (tree).ident), "::", tree->refs);
      fprintf(ci->fh, "/*  C++ stub module */\n");
      fprintf(ci->fh, "namespace %s {\n", id);

      ch_output_cpp_stub_class_protos(IDL_MODULE(tree).definition_list, rinfo, ci);

      fprintf(ci->fh, "  }; /* end namespace */\n");
    }
    break;
  case IDLN_LIST:
    {
      IDL_tree sub;

      fprintf(ci->fh, "/*  C++ stub List */\n");
      for(sub = tree; sub; sub = IDL_LIST(sub).next) {
	ch_output_cpp_stub_class_protos(IDL_LIST(sub).data, rinfo, ci);
      }
    }
    break;
  case IDLN_INTERFACE:
    fprintf(ci->fh, "/*  C++ stub Interface */\n");
    ch_output_cpp_interface_stub(tree, rinfo, ci);
    break;
  default:
    break;
  }
}

static void
ch_output_cpp_type_struct_operators(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci)
{
  {
    char *cid = orbit_cbe_get_typespec_str2(IDL_TYPE_STRUCT(tree).ident,
					    OIDL_TYPENAME_C);
    char *id = orbit_cbe_get_typespec_str2(IDL_TYPE_STRUCT(tree).ident,
					   OIDL_TYPENAME_CPP_FULL);
    char *in_id = orbit_cbe_write_param_typespec_str3(IDL_TYPE_STRUCT(tree).ident,
						      DATA_IN,
						      OIDL_TYPENAME_CPP_FULL);

    fprintf(ci->fh, "#ifndef %s_operators_defined\n", cid);
    fprintf(ci->fh, "#define %s_operators_defined\n\n", cid);
    
    fprintf(ci->fh, "inline void operator<<=(CORBA::Any &any, %s from_) {\n", in_id);
    fprintf(ci->fh, "  CORBA_TypeCode tc = TC_%s;\n", cid);
    fprintf(ci->fh, "  CORBA::AnyStructStream<%s>::from(any, tc, from_);\n", id);
    fprintf(ci->fh, "}\n\n");
    
    fprintf(ci->fh, "inline CORBA::Boolean operator>>=(const CORBA::Any &any, %s * &to_) {\n", id);
    fprintf(ci->fh, "  CORBA_TypeCode tc = TC_%s;\n", cid);
    fprintf(ci->fh, "  return CORBA::AnyStructStream<%s>::to(any, tc, to_);\n", id);
    fprintf(ci->fh, "}\n\n");

    fprintf(ci->fh, "#endif /* %s_operators_defined */\n\n\n", cid);

    g_free(cid);
    g_free(id);
    g_free(in_id);
  }
}

void ch_output_cpp_global_functions(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci)
{
  if(!tree) return;

  switch(IDL_NODE_TYPE(tree)) {
  case IDLN_MODULE:
    /* no namespace block */
    ch_output_cpp_global_functions(IDL_MODULE(tree).definition_list, rinfo, ci);
    break;
  case IDLN_LIST:
    {
      IDL_tree sub;

      for(sub = tree; sub; sub = IDL_LIST(sub).next) {
	ch_output_cpp_global_functions(IDL_LIST(sub).data, rinfo, ci);
      }
    }
    break;
  case IDLN_TYPE_STRUCT:
    ch_output_cpp_type_struct_operators(tree, rinfo, ci);
    break;
  default:
    break;
  }
}



static void ch_output_cpp_execption(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci)
{
  IDL_tree cur;
  
  char *cid = orbit_cbe_get_typespec_str2(IDL_EXCEPT_DCL (tree).ident, OIDL_TYPENAME_C);
  char *cppid = orbit_cbe_get_typespec_str2(IDL_EXCEPT_DCL (tree).ident, OIDL_TYPENAME_CPP_SIMPLE);
  fprintf(ci->fh, "#ifndef %s_exception_defined\n", cid);
  fprintf(ci->fh, "#define %s_exception_defined\n\n", cid);


  fprintf(ci->fh, "class %s : public CORBA::UserException\n{\n", cppid);
  fprintf(ci->fh, "  public:\n");


  for(cur = IDL_TYPE_STRUCT(tree).member_list; cur; cur = IDL_LIST(cur).next) {
    IDL_tree member = IDL_LIST(cur).data;
    IDL_tree ts = IDL_MEMBER(member).type_spec;
    char *type = orbit_cbe_write_param_typespec_str3(ts, DATA_IN, OIDL_TYPENAME_CPP_FULL);
    char *vartype = orbit_cbe_get_typespec_str2(ts, OIDL_TYPENAME_CPP_FULL);

    IDL_tree tts = orbit_cbe_get_typespec(ts);
    switch(IDL_NODE_TYPE(tts)) {
    case IDLN_INTERFACE:
      {
	char *tmp = g_strdup_printf("%s_ptr", vartype);
	g_free(vartype);
	vartype = tmp;
      }
      break;
    default:
      break;
    }
    IDL_tree tname = IDL_LIST(IDL_MEMBER(member).dcls).data;

    char *name = 0;
    switch(IDL_NODE_TYPE(tname)) {
    case IDLN_IDENT:
      name = IDL_IDENT(tname).str;
      break;
    default:
      break;
    }
    if (name) {

      fprintf(ci->fh, "    ");
      fprintf(ci->fh, "%s _%s; \n", vartype, name);
      fprintf(ci->fh, "    ");
      fprintf(ci->fh, "%s %s() { return _%s; }\n", type, name, name);
    }

    g_free(vartype);
    g_free(type);
  }

  fprintf(ci->fh, "    %s() {}\n", cppid);

/*
   if arguments exist
*/
  if ((cur = IDL_TYPE_STRUCT(tree).member_list) != NULL) {
    fprintf(ci->fh, "    %s(", cppid);
  
    for(; cur; cur = IDL_LIST(cur).next) {
      IDL_tree member = IDL_LIST(cur).data;
      IDL_tree ts = IDL_MEMBER(member).type_spec;

      char *type = orbit_cbe_write_param_typespec_str3(ts, DATA_IN, OIDL_TYPENAME_CPP_FULL);
      IDL_tree tname = IDL_LIST(IDL_MEMBER(member).dcls).data;

      char *name = 0;
      switch(IDL_NODE_TYPE(tname)) {
      case IDLN_IDENT:
	name = IDL_IDENT(tname).str;
	break;
      default:
	break;
      }
      if (name) {
	fprintf(ci->fh, "%s %s_ %s", type, name, IDL_LIST(cur).next?",":"");
      }

      g_free(type);
    }
    fprintf(ci->fh, " )\n    {\n");

    for(cur = IDL_TYPE_STRUCT(tree).member_list; cur; cur = IDL_LIST(cur).next) {
      fprintf(ci->fh, "      ");
      IDL_tree member = IDL_LIST(cur).data;
      IDL_tree ts = IDL_MEMBER(member).type_spec;
      char *type = orbit_cbe_write_param_typespec_str3(ts, DATA_IN, OIDL_TYPENAME_CPP_FULL);
      IDL_tree tname = IDL_LIST(IDL_MEMBER(member).dcls).data;

      int string = 0;

      if (strcmp(type, "const CORBA_char *") == 0) {
	string = 1;
      }

      char *name = 0;
      switch(IDL_NODE_TYPE(tname)) {
      case IDLN_IDENT:
	name = IDL_IDENT(tname).str;
	break;
      default:
	break;
      }
      if (name) {
	if (string) {
	  fprintf(ci->fh, "_%s = (char*)%s_;", name, name);
	} else {
	  fprintf(ci->fh, "_%s = %s_;", name, name);
	}
      }

      g_free(type);
    }

    fprintf(ci->fh, "\n    }\n");
  }
  
  fprintf(ci->fh, "    %s(CORBA_Environment *ev) : CORBA::UserException(ev)\n    {\n", cppid);

  /*
   if arguments exist
  */
  if ((cur = IDL_TYPE_STRUCT(tree).member_list) != NULL) {
    fprintf(ci->fh, "      %s *ex = (%s*) CORBA_exception_get_value(ev);\n", cid, cid);

    for(; cur; cur = IDL_LIST(cur).next) {
      fprintf(ci->fh, "      ");
      IDL_tree member = IDL_LIST(cur).data;
      IDL_tree ts = IDL_MEMBER(member).type_spec;
      char *type = orbit_cbe_write_param_typespec_str3(ts, DATA_IN, OIDL_TYPENAME_CPP_FULL);
      IDL_tree tname = IDL_LIST(IDL_MEMBER(member).dcls).data;

      int string = 0;

      if (strcmp(type, "const CORBA_char *") == 0) {
	string = 1;
      }

      char *name = 0;
      switch(IDL_NODE_TYPE(tname)) {
      case IDLN_IDENT:
	name = IDL_IDENT(tname).str;
	break;
      default:
	break;
      }
      if (name) {
	if (string) {
	  fprintf(ci->fh, "_%s = (char*)ex->%s;", name, name);
	} else {
	  fprintf(ci->fh, "_%s = ex->%s;", name, name);
	}
      }

      g_free(type);
    }
  }
  fprintf(ci->fh, "\n    }\n");

  fprintf(ci->fh, "};\n\n");

      
  fprintf(ci->fh, "#endif /* %s_exception_defined */\n\n", cid);
  g_free(cid);
  g_free(cppid);
/*
    ch_output_cpp_type_struct(tree, rinfo, ci);
*/
}


/*
static void
rtorb_cbe_cpp_op_predeclare_class (FILE       *of,
				   IDL_tree    tree)
{
  IDL_tree  sub;
  IDL_tree  typespec;
  char *id;


  typespec = orbit_cbe_get_typespec(tree);

  switch(IDL_NODE_TYPE(typespec)) {
  case IDLN_IDENT:
    id = orbit_cbe_get_typespec_str2(typespec, OIDL_TYPENAME_CPP_SIMPLE);
    fprintf(of, "class %s;\n", id);
    g_free(id);
    break;
  default:
    break;
  }

  if (IDL_NODE_TYPE(tree) == IDLN_OP_DCL) {
    for (sub = IDL_OP_DCL (tree).parameter_dcls; sub; sub = IDL_LIST (sub).next) {
      IDL_tree parm = IDL_LIST (sub).data;

      id = 0;
      typespec = orbit_cbe_get_typespec(parm);
      switch(IDL_NODE_TYPE(typespec)) {
      case IDLN_IDENT:
	id = orbit_cbe_get_typespec_str2(typespec, OIDL_TYPENAME_CPP_SIMPLE);
	break;
      default:
	break;
      }

      if (id) {
	fprintf(of, "class %s;\n", id);
	g_free(id);
      }
	       
    }
  }
  fprintf(of, "\n");
}
*/
void
rtorb_cbe_cpp_op_write_proto5 (FILE       *of,
			       IDL_tree    op,
			       const char *nom_prefix,
			       gboolean    flag,
			       OIDL_TypeNameType tn
			       );

void
rtorb_cbe_cpp_op_write_proto (FILE       *of,
			      IDL_tree    op,
			      const char *nom_prefix,
			      gboolean    flag)
{
  rtorb_cbe_cpp_op_write_proto5(of, op, nom_prefix, flag, OIDL_TYPENAME_CPP_SIMPLE);
/*
  rtorb_cbe_cpp_op_write_proto5(of, op, nom_prefix, flag, OIDL_TYPENAME_CPP_FULL);
*/
}

void
rtorb_cbe_cpp_op_write_proto5 (FILE       *of,
			       IDL_tree    op,
			       const char *nom_prefix,
			       gboolean    flag,
			       OIDL_TypeNameType tn
			       )
{
  IDL_tree  sub;

  g_assert (IDL_NODE_TYPE(op) == IDLN_OP_DCL);

  if(flag) fprintf (of, "virtual ");
  char *ns = orbit_cbe_get_typespec_str2(IDL_OP_DCL(op).op_type_spec, OIDL_TYPENAME_CPP_MODULE);
  if(strncmp(nom_prefix, ns, strlen(ns))){
/*
    fprintf(stderr, "  %s <--> %s\n", nom_prefix, ns);
*/
    tn=OIDL_TYPENAME_CPP_FULL;
  }
  orbit_cbe_write_param_typespec3 (of, op, tn);
#if 0
  id = IDL_ns_ident_to_qstring (
				IDL_IDENT_TO_NS ( IDL_OP_DCL(op).ident), "::", 0);
  fprintf (of, " %s", id);
  g_free (id);
#else
  fprintf (of, " %s", IDL_IDENT (IDL_OP_DCL (op).ident).str);
#endif

  fprintf (of, "(");

  for (sub = IDL_OP_DCL (op).parameter_dcls; sub; sub = IDL_LIST (sub).next) {
    IDL_tree parm = IDL_LIST (sub).data;
    orbit_cbe_write_param_typespec3 (of, parm, tn);
    fprintf (of, " %s", IDL_IDENT (IDL_PARAM_DCL (parm).simple_declarator).str);
    if(IDL_LIST (sub).next) fprintf (of, ", ");
  }

  fprintf (of, ")");
  if(flag) fprintf (of, "=0");
}

static void ch_output_cpp_type_enum(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci)
{
  IDL_tree  l;
	
  char * cid = IDL_ns_ident_to_qstring (
					IDL_IDENT_TO_NS (IDL_TYPE_ENUM (tree).ident), "_", 0);

  char * cppid = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(IDL_TYPE_ENUM(tree).ident), "::", tree->refs);

  fprintf(ci->fh, "#ifndef %s_cpp_enum_defined\n", cid);
  fprintf(ci->fh, "#define %s_cpp_enum_defined\n", cid);

  fprintf(ci->fh, "  typedef %s %s;\n", cid, cppid);
  fprintf(ci->fh, "  typedef %s %s_var;\n", cid, cppid);
  fprintf(ci->fh, "  typedef CORBA_enum_helper %s_helper;\n", cppid);
  fprintf(ci->fh, "  typedef CORBA_enum_helper %s_CInArg;\n", cppid);
  fprintf(ci->fh, "  typedef CORBA_enum_helper %s_COutArg;\n", cppid);
  fprintf(ci->fh, "  typedef CORBA_enum_helper %s_CInOutArg;\n", cppid);
  fprintf(ci->fh, "  typedef CORBA_enum_helper %s_CppInArg;\n", cppid);
  fprintf(ci->fh, "  typedef CORBA_enum_helper %s_CppOutArg;\n", cppid);
  fprintf(ci->fh, "  typedef CORBA_enum_helper %s_CppInOutArg;\n", cppid);

  fprintf(ci->fh, "enum { \n");

  for (l = IDL_TYPE_ENUM (tree).enumerator_list; l; l = IDL_LIST (l).next) {
    char *id;
    char *p;

    id = IDL_ns_ident_to_qstring (IDL_IDENT_TO_NS (IDL_LIST (l).data), "_", 0);

    p = orbit_cbe_get_typespec_str2(IDL_LIST (l).data, OIDL_TYPENAME_CPP_SIMPLE);
    fprintf(ci->fh, "  %s = %s,\n", p, id);
    g_free(p);
    g_free(id);
  }

  fprintf(ci->fh, "};\n\n");
/*
  	fprintf(ci->fh, "typedef CORBA_enum_ptr<%s, %s> %s_ptr;\n", cppid, cid, cppid);
  	fprintf(ci->fh, "typedef %s_ptr %s_var;\n", cppid, cppid);
*/
  fprintf(ci->fh, "#endif /* %s_cpp_enum_defined */\n\n", cid);

  g_free(cppid);
  g_free(cid);
}



static void ch_output_cpp_interface_stub(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci)
{
  IDL_tree sub;

  char *id = orbit_cbe_get_typespec_str2(IDL_INTERFACE (tree).ident, OIDL_TYPENAME_CPP_SIMPLE);
  char *cid = orbit_cbe_get_typespec_str2(IDL_INTERFACE (tree).ident, OIDL_TYPENAME_C);
  char *mod_id = orbit_cbe_get_typespec_str2(tree, OIDL_TYPENAME_CPP_MODULE);

  if(tree->refs == 1){

    fprintf(ci->fh, "#ifndef %s_%s_def\n", mod_id, id);
    fprintf(ci->fh, "#define %s_%s_def\n\n", mod_id, id);
  }

  fprintf(ci->fh, "class %s : \n", id);
  
  if ((sub = IDL_INTERFACE(tree).inheritance_spec) != NULL ) {

    for (; sub; sub = IDL_LIST(sub).next) {
      
      IDL_tree curr = IDL_LIST(sub).data;

      char *id_ = orbit_cbe_get_typespec_str2(curr, OIDL_TYPENAME_CPP_FULL);
      fprintf(ci->fh, "  public virtual %s", id_);

      if (IDL_LIST(sub).next) {
	fprintf(ci->fh, ",\n");
      }
      g_free(id_);
    }
  } else {
    fprintf(ci->fh, "  public virtual CORBA::Object\n");
  }
  fprintf(ci->fh, "{\n");
  fprintf(ci->fh, "  public:\n");
  fprintf(ci->fh, "    typedef %s _c_type;\n", cid);
  fprintf(ci->fh, "    typedef %s_ptr _ptr_type;\n", id);
  fprintf(ci->fh, "    typedef %s_var _var_type;\n\n", id);
  fprintf(ci->fh, "    typedef %s_helper _helper_type;\n\n", id);

  fprintf(ci->fh, "  public:\n");

  if ((sub = IDL_INTERFACE(tree).inheritance_spec) != NULL ) {

/*
     for (; sub; sub = IDL_LIST(sub).next) {
*/
      
    IDL_tree curr = IDL_LIST(sub).data;

    char *id_ = orbit_cbe_get_typespec_str2(curr, OIDL_TYPENAME_CPP_FULL);
    fprintf(ci->fh, "    typedef %s _super_type;\n", id_);
    g_free(id_);
/*
        }
*/
  } else {

    fprintf(ci->fh, "    typedef CORBA::Object _super_type;\n");
  }


  fprintf(ci->fh, "  protected:\n");
  fprintf(ci->fh, "    %s() : _super_type() {}\n\n", id);
  fprintf(ci->fh, "  public:\n");
  fprintf(ci->fh, "    %s(CORBA_Object impl) : CORBA::Object(impl) {}\n\n", id);

  fprintf(ci->fh, "    %s * operator->() { return this; };\n", id);
  fprintf(ci->fh, "    %s & operator=(CORBA_Object);\n", id);

  fprintf(ci->fh, "    static %s_ptr _narrow(CORBA::Object_ptr ptr);\n", id);
  fprintf(ci->fh, "    static %s_ptr _duplicate(%s_ptr ptr) {\n      return CORBA_interface::Utils<%s>::duplicate(ptr);\n    }\n", id, id, id);


/*
        fprintf(ci->fh, "      %s(PortableServer::POA_ptr);\n", id);
*/
  for(sub = IDL_INTERFACE(tree).body; sub; sub = IDL_LIST(sub).next) {
    IDL_tree cur;

    cur = IDL_LIST(sub).data;

    switch(IDL_NODE_TYPE(cur)) {
    case IDLN_EXCEPT_DCL:
      ch_output_cpp_execption(cur, rinfo, ci);
      break;
    case IDLN_TYPE_ENUM:
      ch_output_cpp_type_enum(cur, rinfo, ci);
      break;
    case IDLN_TYPE_DCL:
      ch_output_cpp_type_dcl_without_sequence(cur, rinfo, ci);
      break;
    case IDLN_OP_DCL:
      fprintf(ci->fh, "    ");
      orbit_idl_check_oneway_op (cur);
      rtorb_cbe_cpp_op_write_proto(ci->fh, cur, mod_id, FALSE);
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

	for(curitem = IDL_ATTR_DCL(cur).simple_declarations; curitem;
	    curitem = IDL_LIST(curitem).next) {

	  ai = IDL_LIST(curitem).data->data;
	      
          fprintf(ci->fh, "    ");
	  rtorb_cbe_cpp_op_write_proto(ci->fh, ai->op1, mod_id, FALSE);
	  fprintf(ci->fh, ";\n");
	      
	  if(ai->op2) {
            fprintf(ci->fh, "    ");
	    rtorb_cbe_cpp_op_write_proto(ci->fh, ai->op2, mod_id, FALSE);
	    fprintf(ci->fh, ";\n");
	  }

	}
      }
      break;
    default:
      break;
    }
  }
  fprintf(ci->fh, "};\n\n");

  fprintf(ci->fh, "\n");

  {
    char *id_ = orbit_cbe_write_param_typespec_str3(IDL_INTERFACE (tree).ident, DATA_IN, OIDL_TYPENAME_CPP_SIMPLE);

    fprintf(ci->fh, "#ifndef %s_operators_defined\n", cid);
    fprintf(ci->fh, "#define %s_operators_defined\n\n", cid);
    
    fprintf(ci->fh, "inline void operator<<=(CORBA_any &any, %s from_)\n{\n", id_);
    fprintf(ci->fh, "  CORBA_TypeCode tc = TC_CORBA_Object;\n");
    fprintf(ci->fh, "  CORBA_Object val = from_.impl();\n");
    fprintf(ci->fh, "  CORBA::AnyObjectStream::from(any, tc, val);\n");
    fprintf(ci->fh, "}\n\n");

    fprintf(ci->fh, "#endif /* %s_operators_defined */ \n\n\n", cid);

    g_free(id_);
  }


  fprintf(ci->fh, "#endif /* %s_%s_def */\n\n", mod_id, id);
  g_free (cid);
  g_free (id);
}

void
ch_output_cpp_poa(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci, const char *mod_name)
{
  if(!tree) return;

  if ( tree->declspec & IDLF_DECLSPEC_PIDL )
    return;

  switch(IDL_NODE_TYPE(tree)) {
  case IDLN_MODULE:

    {
      char* id = IDL_ns_ident_to_qstring(IDL_IDENT_TO_NS(IDL_MODULE(tree).ident), "_", 0);
      fprintf(ci->fh, "namespace POA_%s {\n", id);

      ch_output_cpp_poa(IDL_MODULE(tree).definition_list, rinfo, ci, id);

      fprintf(ci->fh, "}\n\n");
      g_free(id);

      break;
    }
  case IDLN_LIST:
    {
      IDL_tree sub;

      for(sub = tree; sub; sub = IDL_LIST(sub).next) {
	ch_output_cpp_poa(IDL_LIST(sub).data, rinfo, ci, mod_name);
      }
    }
    break;
  case IDLN_INTERFACE:
    {
      IDL_tree sub;
      char * id;
      char * cid;
      char * idfull;
      
      if (!mod_name) { return; }
      
      id = orbit_cbe_get_typespec_str2(IDL_INTERFACE(tree).ident, OIDL_TYPENAME_CPP_SIMPLE);
      idfull = orbit_cbe_get_typespec_str2(IDL_INTERFACE(tree).ident, OIDL_TYPENAME_CPP_FULL);
      cid = orbit_cbe_get_typespec_str2(IDL_INTERFACE(tree).ident, OIDL_TYPENAME_C);

      fprintf(ci->fh, "#ifndef _defined_POA_CPP_%s_%s\n", mod_name, id);
      fprintf(ci->fh, "#define _defined_POA_CPP_%s_%s\n", mod_name, id);


      fprintf(ci->fh, "\nclass %s : \n", id);
      fprintf(ci->fh, "  public virtual %s::%s__impl", mod_name, id);

      sub = IDL_INTERFACE(tree).inheritance_spec;
      if (sub) {
	for (; sub; sub = IDL_LIST(sub).next) {
      
	  IDL_tree curr = IDL_LIST(sub).data;

	  char *id_ = orbit_cbe_get_typespec_str2(curr, OIDL_TYPENAME_CPP_FULL);
	  fprintf(ci->fh, ", public virtual POA_%s\n", id_);
	  g_free(id_);
	}
      } else {
	fprintf(ci->fh, ", public virtual PortableServer::ServantBase\n");
      }

      fprintf(ci->fh, "{\n");
      fprintf(ci->fh, "  typedef ::%s T;\n\n", idfull);
      fprintf(ci->fh, "  public:\n");
      fprintf(ci->fh, "    ::%s_ptr _this() {\n", idfull);
      fprintf(ci->fh, "      if (_t == NULL) {\n");
      fprintf(ci->fh, "        CORBA_Object impl = __this(&ev);\n");
      fprintf(ci->fh, "        if (impl != NULL) { _t = new T(impl); }\n");
      fprintf(ci->fh, "      }\n");
      fprintf(ci->fh, "      return _t;\n");
      fprintf(ci->fh, "    }\n\n");
      
      fprintf(ci->fh, "    %s() : _t(NULL) {\n", id);
      fprintf(ci->fh, "      PortableServer::POA_var poa = PortableServer::ServantBase::_root_POA();\n");
      fprintf(ci->fh, "      poa_servant = impl_%s__create_servant(poa->_impl, &ev);\n", cid);
      fprintf(ci->fh, "      register_PortableServer_Servant(poa->_impl, poa_servant, &ev);\n");
      fprintf(ci->fh, "      _impl = PortableServer_POA_servant_to_reference(poa->_impl, poa_servant, &ev);\n");
      fprintf(ci->fh, "      _impl->impl_obj = static_cast<CORBA::Object*>(this);\n");
      fprintf(ci->fh, "    }\n\n");
      fprintf(ci->fh, "    ~%s() { delete _t; _t = NULL; }\n\n", id);
      fprintf(ci->fh, "  private:\n");
      fprintf(ci->fh, "    T * _t;\n");

      fprintf(ci->fh, "};\n\n");


      fprintf(ci->fh, "#endif /* _defined_POA_CPP_%s_%s */\n\n", mod_name, id);

      g_free(id);
      g_free(cid);
      g_free(idfull);

      break;
    }
  default:
    break;
  }
}

static void
ch_output_cpp_type_dcl_without_sequence(IDL_tree tree, OIDL_Run_Info *rinfo, OIDL_C_Info *ci)
{
  IDL_tree l;
  for (l = IDL_TYPE_DCL (tree).dcls; l; l = IDL_LIST (l).next) {
    char *cid = NULL;
    char *cppid = NULL;
    IDL_tree ent = IDL_LIST (l).data;
    IDL_tree tts = orbit_cbe_get_typespec(ent);
    
    if (IDL_NODE_TYPE(tts) == IDLN_TYPE_SEQUENCE) {
      continue;
    }

    IDL_tree ident = NULL;

    switch (IDL_NODE_TYPE(ent)) {
    case IDLN_IDENT:
      ident = ent;
      break;
    case IDLN_TYPE_ARRAY:
      ident = IDL_TYPE_ARRAY (ent).ident;
      break;
    default:

      g_assert_not_reached ();
      break;

    }
    char *ts_cpp  = orbit_cbe_get_typespec_str2(IDL_TYPE_DCL(tree).type_spec, OIDL_TYPENAME_CPP_FULL);
    cid = IDL_ns_ident_to_qstring (
				   IDL_IDENT_TO_NS (ident), "_", 0);
    cppid = orbit_cbe_get_typespec_str2(ident, OIDL_TYPENAME_CPP_SIMPLE);
    fprintf (ci->fh, "#if !defined(_%s_cpp_defined)\n#define _%s_cpp_defined 1\n\n", cid, cid);
    fprintf (ci->fh, "typedef %s %s;\n\n", cid, cppid);
    fprintf (ci->fh, "typedef %s_helper %s_helper;\n\n", ts_cpp, cppid);
    fprintf (ci->fh, "typedef %s_helper %s_CInArg;\n\n", ts_cpp, cppid);
    fprintf (ci->fh, "typedef %s_helper %s_COutArg;\n\n", ts_cpp, cppid);
    fprintf (ci->fh, "typedef %s_helper %s_CInOutArg;\n\n", ts_cpp, cppid);
    fprintf (ci->fh, "typedef %s_helper %s_CppInArg;\n\n", ts_cpp, cppid);
    fprintf (ci->fh, "typedef %s_helper %s_CppOutArg;\n\n", ts_cpp, cppid);
    fprintf (ci->fh, "typedef %s_helper %s_CppInOutArg;\n\n", ts_cpp, cppid);
    fprintf (ci->fh, "#endif\n");
    g_free (ts_cpp);
    g_free (cppid);
    g_free (cid);
  }
}

