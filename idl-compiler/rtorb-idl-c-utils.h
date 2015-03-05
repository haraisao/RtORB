


char *rtorb_cbe_get_typespec_str(IDL_tree tree);
char * rtorb_cbe_get_typecode_name (IDL_tree tree);

gboolean rtorb_cbe_type_is_builtin(IDL_tree tree);

char * rtorb_cbe_get_typespec_str2(IDL_tree tree, OIDL_TypeNameType tn);
char * rtorb_cbe_get_typespec_str(IDL_tree tree);
void rtorb_cbe_write_typespec(FILE *of, IDL_tree tree);
char * rtorb_cbe_write_param_typespec_str3(IDL_tree ts, IDL_ParamRole role, OIDL_TypeNameType otype);
char * rtorb_cbe_write_param_typespec_str(IDL_tree ts, IDL_ParamRole role);

void rtorb_cbe_write_param_typespec3(FILE *of, IDL_tree tree, OIDL_TypeNameType tn) ;
void rtorb_cbe_write_param_typespec(FILE *of, IDL_tree tree) ;
char * rtorb_cbe_get_param_typespec_str3(IDL_tree ts, IDL_ParamRole role, OIDL_TypeNameType otype); 
char * rtorb_cbe_get_param_typespec_str2(IDL_tree tree, OIDL_TypeNameType tn) ;
char * rtorb_cbe_get_param_typespec_str(IDL_tree tree) ;
void rtorb_cbe_op_write_proto (FILE       *of, IDL_tree    op, const char *nom_prefix, gboolean    for_epv);

void rtorb_cbe_write_const(FILE *of, IDL_tree tree);
void rtorb_cbe_id_define_hack(FILE *fh, const char *def_prefix, const char *def_name, const char *def_value);
void rtorb_cbe_id_cond_hack(FILE *fh, const char *def_prefix, const char *def_name, const char *def_value);

#define BASE_TYPES \
	     IDLN_TYPE_INTEGER: \
	case IDLN_TYPE_FLOAT: \
	case IDLN_TYPE_ENUM: \
        case IDLN_TYPE_BOOLEAN: \
	case IDLN_TYPE_CHAR: \
	case IDLN_TYPE_WIDE_CHAR: \
	case IDLN_TYPE_OCTET

#define STRING_TYPES \
	     IDLN_TYPE_STRING: \
	case IDLN_TYPE_WIDE_STRING

#define OBJREF_TYPES \
	     IDLN_TYPE_OBJECT: \
	case IDLN_INTERFACE: \
	case IDLN_FORWARD_DCL

void rtorb_cbe_flatten_args (IDL_tree tree, FILE *of, const char *name);
char * rtorb_cbe_unflatten_ref (IDL_ParamRole role, IDL_tree typespec); 
void rtorb_cbe_unflatten_args (IDL_tree tree, FILE *of, const char *name);

int rtorb_cbe_get_param_typespec(IDL_tree tree);
