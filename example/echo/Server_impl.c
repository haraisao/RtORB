/* This is a template file generated by command */
/* rtorb-idl --skeleton-impl echo.idl */
/* User must edit this file, inserting servant  */
/* specific code between markers. */

#include "echo.h"

/*** App-specific servant structures ***/
typedef struct {
    POA_Echo servant;
    PortableServer_POA poa;

    /* ------ add private attributes here ------ */
#if __cplusplus
    CORBA::Object_var impl_obj;
#endif /* __cplusplus */
    /* ------ ---------- end ------------ ------ */

} impl_POA_Echo;


/*** Implementation stub prototypes ***/
static void impl_Echo__destroy(
  impl_POA_Echo *servant,
  CORBA_Environment *ev);

static CORBA_string
impl_Echo_echoString(
  impl_POA_Echo *servant,
  const CORBA_char * mesg,
  CORBA_Environment *ev);

static CORBA_string
impl_Echo_echoString2(
  impl_POA_Echo *servant,
  const CORBA_char * mesg,
  CORBA_string* res,
  CORBA_Environment *ev);

static CORBA_string
impl_Echo_echoString3(
  impl_POA_Echo *servant,
  CORBA_string* mesg,
  CORBA_Environment *ev);

static CORBA_long
impl_Echo_plus(
  impl_POA_Echo *servant,
  CORBA_long x,
  CORBA_long y,
  CORBA_Environment *ev);

static void
impl_Echo_plus2(
  impl_POA_Echo *servant,
  CORBA_long x,
  CORBA_long y,
  CORBA_long* ans,
  CORBA_Environment *ev);

static CORBA_float
impl_Echo_divide(
  impl_POA_Echo *servant,
  CORBA_float x,
  CORBA_float y,
  CORBA_Environment *ev);

static void
impl_Echo_divide2(
  impl_POA_Echo *servant,
  CORBA_float x,
  CORBA_float y,
  CORBA_float* ans,
  CORBA_Environment *ev);


/*** epv structures ***/
static PortableServer_ServantBase__epv impl_Echo_base_epv = {
    NULL,             /* _private data */
    (void (*)(void*, CORBA_Environment*))& impl_Echo__destroy, /* finalize routine */
    NULL,             /* default_POA routine */
};

static POA_Echo__epv impl_Echo_epv = {
    NULL, /* _private */
    (CORBA_string (*)(void *, const CORBA_char *, CORBA_Environment *))&impl_Echo_echoString,
    (CORBA_string (*)(void *, const CORBA_char *, CORBA_string*, CORBA_Environment *))&impl_Echo_echoString2,
    (CORBA_string (*)(void *, CORBA_string*, CORBA_Environment *))&impl_Echo_echoString3,
    (CORBA_long (*)(void *, CORBA_long, CORBA_long, CORBA_Environment *))&impl_Echo_plus,
    (void (*)(void *, CORBA_long, CORBA_long, CORBA_long*, CORBA_Environment *))&impl_Echo_plus2,
    (CORBA_float (*)(void *, CORBA_float, CORBA_float, CORBA_Environment *))&impl_Echo_divide,
    (void (*)(void *, CORBA_float, CORBA_float, CORBA_float*, CORBA_Environment *))&impl_Echo_divide2,
};


/*** vepv structures ***/
static POA_Echo__vepv impl_Echo_vepv = {
    &impl_Echo_base_epv,
    &impl_Echo_epv,
};


/*** Stub implementations ***/
CORBA_Echo 
impl_Echo__create(PortableServer_POA poa, CORBA_Environment *ev)
{
  CORBA_Echo retval;
  impl_POA_Echo *newservant;
  PortableServer_ObjectId objid;

  newservant = (impl_POA_Echo *)RtORB_calloc(1, sizeof(impl_POA_Echo)," create...");
  newservant->servant.vepv = &impl_Echo_vepv;
  newservant->poa = poa;
  POA_Echo__init((PortableServer_Servant)newservant, ev);
   /* Before servant is going to be activated all
    * private attributes must be initialized.  */

   /* ------ init private attributes here ------ */
   /* ------ ---------- end ------------- ------ */

  objid = PortableServer_POA_activate_object(poa, newservant, ev);
  RtORB_free(objid, " objid");
  retval = PortableServer_POA_servant_to_reference(poa, newservant, ev);

  return retval;
}

static void
impl_Echo__destroy(impl_POA_Echo *servant, CORBA_Environment *ev)
{
    CORBA_Object_release ((CORBA_Object) servant->poa, ev);

    /* No further remote method calls are delegated to 
    * servant and you may free your private attributes. */
   /* ------ free private attributes here ------ */
   /* ------ ---------- end ------------- ------ */

    POA_Echo__fini((PortableServer_Servant)servant, ev);

    RtORB_free (servant, "servant");
}



 /*** create servent **/ 
impl_POA_ServantBase * 
impl_Echo__create_servant(PortableServer_POA poa, CORBA_Environment *ev)
{
  CORBA_Echo retval;
  impl_POA_Echo *newservant;
  PortableServer_ObjectId objid;

  newservant = (impl_POA_Echo *)RtORB_calloc(1, sizeof(impl_POA_Echo), "  create_servant");
  newservant->servant.vepv = &impl_Echo_vepv;
  newservant->poa = poa;
  POA_Echo__init((PortableServer_Servant)newservant, ev);
   /* Before servant is going to be activated all
    * private attributs must be initialized.  */

   /* ------ init private attributes here ------ */
   /* ------ ---------- end ------------- ------ */


  return (impl_POA_ServantBase*)newservant;
}

static CORBA_string
impl_Echo_echoString(
  impl_POA_Echo *servant,
  const CORBA_char * mesg,
  CORBA_Environment *ev)
{
  CORBA_string retval;
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object*>(object->impl_obj);
    Echo__impl *impl = dynamic_cast<Echo__impl *>(obj_);
    CORBA_string_var var = impl->echoString(CORBA_string_CInArg(mesg ));
    retval = var._retn();
    ev->_cpp_flag = 1;
#else /*  C  */
    /* ------   insert method code here   ------ */
    fprintf(stderr, "  Echo::echoString : %s", mesg);
    retval = RtORB_strdup(mesg, "..");
    /* ------ ---------- end ------------ ------ */
#endif /* __cplusplus */ 
  return retval;
}

static CORBA_string
impl_Echo_echoString2(
  impl_POA_Echo *servant,
  const CORBA_char * mesg,
  CORBA_string* res,
  CORBA_Environment *ev)
{
  CORBA_string retval;
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object*>(object->impl_obj);
    Echo__impl *impl = dynamic_cast<Echo__impl *>(obj_);
    CORBA_string_var var = impl->echoString2(CORBA_string_CInArg(mesg ), CORBA_string_COutArg(res ));
    retval = var._retn();
    ev->_cpp_flag = 1;
#else /*  C  */
    /* ------   insert method code here   ------ */
    fprintf(stderr, "  Echo::echoString : %s", mesg);
    retval = RtORB_strdup(mesg, "..");
    res = RtORB_strdup("Echo::echoString2", "..");
    /* ------ ---------- end ------------ ------ */
#endif /* __cplusplus */ 
  return retval;
}

static CORBA_string
impl_Echo_echoString3(
  impl_POA_Echo *servant,
  CORBA_string* mesg,
  CORBA_Environment *ev)
{
  CORBA_string retval;
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object*>(object->impl_obj);
    Echo__impl *impl = dynamic_cast<Echo__impl *>(obj_);
    CORBA_string_var var = impl->echoString3(CORBA_string_CInOutArg(mesg ));
    retval = var._retn();
    ev->_cpp_flag = 1;
#else /*  C  */
    /* ------   insert method code here   ------ */
    fprintf(stderr, "  Echo::echoString : %s", mesg);
    retval = RtORB_strdup(mesg, "");
    /* ------ ---------- end ------------ ------ */
#endif /* __cplusplus */ 
  return retval;
}

static CORBA_long
impl_Echo_plus(
  impl_POA_Echo *servant,
  CORBA_long x,
  CORBA_long y,
  CORBA_Environment *ev)
{
  CORBA_long retval;
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object*>(object->impl_obj);
    Echo__impl *impl = dynamic_cast<Echo__impl *>(obj_);
    CORBA::Long_var var = impl->plus(CORBA::Long_CInArg(x ), CORBA::Long_CInArg(y ));
  /* type = 20 */
    retval = var;
    ev->_cpp_flag = 1;
#else /*  C  */
    /* ------   insert method code here   ------ */
    retval = x + y;
    /* ------ ---------- end ------------ ------ */
#endif /* __cplusplus */ 
  return retval;
}

static void
impl_Echo_plus2(
  impl_POA_Echo *servant,
  CORBA_long x,
  CORBA_long y,
  CORBA_long* ans,
  CORBA_Environment *ev)
{
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object *>(object->impl_obj);
    Echo__impl *impl = dynamic_cast<Echo__impl *>(obj_);
    impl->plus2(CORBA::Long_CInArg(x ), CORBA::Long_CInArg(y ), CORBA::Long_COutArg(ans ));
#else /* C */
    /* ------   insert method code here   ------ */
    *ans = x + y;
    /* ------ ---------- end ------------ ------ */
#endif /*__cplusplus */
}

static CORBA_float
impl_Echo_divide(
  impl_POA_Echo *servant,
  CORBA_float x,
  CORBA_float y,
  CORBA_Environment *ev)
{
  CORBA_float retval;
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object*>(object->impl_obj);
    Echo__impl *impl = dynamic_cast<Echo__impl *>(obj_);
    CORBA_float_var var = impl->divide(CORBA_float_CInArg(x ), CORBA_float_CInArg(y ));
  /* type = 21 */
    retval = var;
    ev->_cpp_flag = 1;
#else /*  C  */
    /* ------   insert method code here   ------ */
    retval = x / y;
    /* ------ ---------- end ------------ ------ */
#endif /* __cplusplus */ 
  return retval;
}

static void
impl_Echo_divide2(
  impl_POA_Echo *servant,
  CORBA_float x,
  CORBA_float y,
  CORBA_float* ans,
  CORBA_Environment *ev)
{
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object *>(object->impl_obj);
    Echo__impl *impl = dynamic_cast<Echo__impl *>(obj_);
    impl->divide2(CORBA_float_CInArg(x ), CORBA_float_CInArg(y ), CORBA_float_COutArg(ans ));
#else /* C */
    /* ------   insert method code here   ------ */
    *ans = x / y;
    /* ------ ---------- end ------------ ------ */
#endif /*__cplusplus */
}


/*** Boohoo! ***/
