/* This is a template file generated by command */
/* rtorb-idl --skeleton-impl CosNaming.idl */
/* User must edit this file, inserting servant  */
/* specific code between markers. */

#include "CosNaming.h"

/*** App-specific servant structures ***/
typedef struct {
    POA_CosNaming_NamingContext servant;
    PortableServer_POA poa;

    /* ------ add private attributes here ------ */
#if __cplusplus
    CORBA::Object_var impl_obj;
#endif /* __cplusplus */
    /* ------ ---------- end ------------ ------ */

} impl_POA_CosNaming_NamingContext;

typedef struct {
    POA_CosNaming_BindingIterator servant;
    PortableServer_POA poa;

    /* ------ add private attributes here ------ */
#if __cplusplus
    CORBA::Object_var impl_obj;
#endif /* __cplusplus */
    /* ------ ---------- end ------------ ------ */

} impl_POA_CosNaming_BindingIterator;

typedef struct {
    POA_CosNaming_NamingContextExt servant;
    PortableServer_POA poa;

    /* ------ add private attributes here ------ */
#if __cplusplus
    CORBA::Object_var impl_obj;
#endif /* __cplusplus */
    /* ------ ---------- end ------------ ------ */

} impl_POA_CosNaming_NamingContextExt;


/*** Implementation stub prototypes ***/
static void impl_CosNaming_NamingContext__destroy(
  impl_POA_CosNaming_NamingContext *servant,
  CORBA_Environment *ev);

CORBA_boolean
impl_CosNaming_NamingContext__is_a(
  impl_POA_CosNaming_NamingContext *servant,
  const CORBA_char * id,
  CORBA_Environment *ev);

void
impl_CosNaming_NamingContext_bind(
  impl_POA_CosNaming_NamingContext *servant,
  CosNaming_Name* n,
  CORBA_Object obj,
  CORBA_Environment *ev);

void
impl_CosNaming_NamingContext_rebind(
  impl_POA_CosNaming_NamingContext *servant,
  CosNaming_Name* n,
  CORBA_Object obj,
  CORBA_Environment *ev);

void
impl_CosNaming_NamingContext_bind_context(
  impl_POA_CosNaming_NamingContext *servant,
  CosNaming_Name* n,
  CosNaming_NamingContext nc,
  CORBA_Environment *ev);

void
impl_CosNaming_NamingContext_rebind_context(
  impl_POA_CosNaming_NamingContext *servant,
  CosNaming_Name* n,
  CosNaming_NamingContext nc,
  CORBA_Environment *ev);

CORBA_Object
impl_CosNaming_NamingContext_resolve(
  impl_POA_CosNaming_NamingContext *servant,
  CosNaming_Name* n,
  CORBA_Environment *ev);

void
impl_CosNaming_NamingContext_unbind(
  impl_POA_CosNaming_NamingContext *servant,
  CosNaming_Name* n,
  CORBA_Environment *ev);

CosNaming_NamingContext
impl_CosNaming_NamingContext_new_context(
  impl_POA_CosNaming_NamingContext *servant,
  CORBA_Environment *ev);

CosNaming_NamingContext
impl_CosNaming_NamingContext_bind_new_context(
  impl_POA_CosNaming_NamingContext *servant,
  CosNaming_Name* n,
  CORBA_Environment *ev);

void
impl_CosNaming_NamingContext_destroy(
  impl_POA_CosNaming_NamingContext *servant,
  CORBA_Environment *ev);

void
impl_CosNaming_NamingContext_list(
  impl_POA_CosNaming_NamingContext *servant,
  CORBA_unsigned_long how_many,
  CosNaming_BindingList** bl,
  CosNaming_BindingIterator* bi,
  CORBA_Environment *ev);

static void impl_CosNaming_BindingIterator__destroy(
  impl_POA_CosNaming_BindingIterator *servant,
  CORBA_Environment *ev);

CORBA_boolean
impl_CosNaming_BindingIterator_next_one(
  impl_POA_CosNaming_BindingIterator *servant,
  CosNaming_Binding** b,
  CORBA_Environment *ev);

CORBA_boolean
impl_CosNaming_BindingIterator_next_n(
  impl_POA_CosNaming_BindingIterator *servant,
  CORBA_unsigned_long how_many,
  CosNaming_BindingList** bl,
  CORBA_Environment *ev);

void
impl_CosNaming_BindingIterator_destroy(
  impl_POA_CosNaming_BindingIterator *servant,
  CORBA_Environment *ev);

static void impl_CosNaming_NamingContextExt__destroy(
  impl_POA_CosNaming_NamingContextExt *servant,
  CORBA_Environment *ev);

CosNaming_NamingContextExt_StringName
impl_CosNaming_NamingContextExt_to_string(
  impl_POA_CosNaming_NamingContextExt *servant,
  CosNaming_Name* n,
  CORBA_Environment *ev);

CosNaming_Name*
impl_CosNaming_NamingContextExt_to_name(
  impl_POA_CosNaming_NamingContextExt *servant,
  const CORBA_char * sn,
  CORBA_Environment *ev);

CosNaming_NamingContextExt_URLString
impl_CosNaming_NamingContextExt_to_url(
  impl_POA_CosNaming_NamingContextExt *servant,
  const CORBA_char * addr,
  const CORBA_char * sn,
  CORBA_Environment *ev);

CORBA_Object
impl_CosNaming_NamingContextExt_resolve_str(
  impl_POA_CosNaming_NamingContextExt *servant,
  const CORBA_char * n,
  CORBA_Environment *ev);

CORBA_boolean
impl_CosNaming_NamingContextExt__is_a(
    impl_POA_CosNaming_NamingContextExt *servant,
    const CORBA_char * id,
    CORBA_Environment *ev);

void
impl_CosNaming_NamingContextExt_bind(
    impl_POA_CosNaming_NamingContextExt *servant,
    CosNaming_Name* n,
    CORBA_Object obj,
    CORBA_Environment *ev);

void
impl_CosNaming_NamingContextExt_rebind(
    impl_POA_CosNaming_NamingContextExt *servant,
    CosNaming_Name* n,
    CORBA_Object obj,
    CORBA_Environment *ev);

void
impl_CosNaming_NamingContextExt_bind_context(
    impl_POA_CosNaming_NamingContextExt *servant,
    CosNaming_Name* n,
    CosNaming_NamingContext nc,
    CORBA_Environment *ev);

void
impl_CosNaming_NamingContextExt_rebind_context(
    impl_POA_CosNaming_NamingContextExt *servant,
    CosNaming_Name* n,
    CosNaming_NamingContext nc,
    CORBA_Environment *ev);

CORBA_Object
impl_CosNaming_NamingContextExt_resolve(
    impl_POA_CosNaming_NamingContextExt *servant,
    CosNaming_Name* n,
    CORBA_Environment *ev);

void
impl_CosNaming_NamingContextExt_unbind(
    impl_POA_CosNaming_NamingContextExt *servant,
    CosNaming_Name* n,
    CORBA_Environment *ev);

CosNaming_NamingContext
impl_CosNaming_NamingContextExt_new_context(
    impl_POA_CosNaming_NamingContextExt *servant,
    CORBA_Environment *ev);

CosNaming_NamingContext
impl_CosNaming_NamingContextExt_bind_new_context(
    impl_POA_CosNaming_NamingContextExt *servant,
    CosNaming_Name* n,
    CORBA_Environment *ev);

void
impl_CosNaming_NamingContextExt_destroy(
    impl_POA_CosNaming_NamingContextExt *servant,
    CORBA_Environment *ev);

void
impl_CosNaming_NamingContextExt_list(
    impl_POA_CosNaming_NamingContextExt *servant,
    CORBA_unsigned_long how_many,
    CosNaming_BindingList** bl,
    CosNaming_BindingIterator* bi,
    CORBA_Environment *ev);


/*** epv structures ***/
static PortableServer_ServantBase__epv impl_CosNaming_NamingContext_base_epv = {
    NULL,             /* _private data */
    (void (*)(void*, CORBA_Environment*))& impl_CosNaming_NamingContext__destroy, /* finalize routine */
    NULL,             /* default_POA routine */
};

static POA_CosNaming_NamingContext__epv impl_CosNaming_NamingContext_epv = {
    NULL, /* _private */
    (CORBA_boolean (*)(void *, const CORBA_char *, CORBA_Environment *))&impl_CosNaming_NamingContext__is_a,
    (void (*)(void *, CosNaming_Name*, CORBA_Object, CORBA_Environment *))&impl_CosNaming_NamingContext_bind,
    (void (*)(void *, CosNaming_Name*, CORBA_Object, CORBA_Environment *))&impl_CosNaming_NamingContext_rebind,
    (void (*)(void *, CosNaming_Name*, CosNaming_NamingContext, CORBA_Environment *))&impl_CosNaming_NamingContext_bind_context,
    (void (*)(void *, CosNaming_Name*, CosNaming_NamingContext, CORBA_Environment *))&impl_CosNaming_NamingContext_rebind_context,
    (CORBA_Object (*)(void *, CosNaming_Name*, CORBA_Environment *))&impl_CosNaming_NamingContext_resolve,
    (void (*)(void *, CosNaming_Name*, CORBA_Environment *))&impl_CosNaming_NamingContext_unbind,
    (CosNaming_NamingContext (*)(void *, CORBA_Environment *))&impl_CosNaming_NamingContext_new_context,
    (CosNaming_NamingContext (*)(void *, CosNaming_Name*, CORBA_Environment *))&impl_CosNaming_NamingContext_bind_new_context,
    (void (*)(void *, CORBA_Environment *))&impl_CosNaming_NamingContext_destroy,
    (void (*)(void *, CORBA_unsigned_long, CosNaming_BindingList**, CosNaming_BindingIterator*, CORBA_Environment *))&impl_CosNaming_NamingContext_list,
};

static PortableServer_ServantBase__epv impl_CosNaming_BindingIterator_base_epv = {
    NULL,             /* _private data */
    (void (*)(void*, CORBA_Environment*))& impl_CosNaming_BindingIterator__destroy, /* finalize routine */
    NULL,             /* default_POA routine */
};

static POA_CosNaming_BindingIterator__epv impl_CosNaming_BindingIterator_epv = {
    NULL, /* _private */
    (CORBA_boolean (*)(void *, CosNaming_Binding**, CORBA_Environment *))&impl_CosNaming_BindingIterator_next_one,
    (CORBA_boolean (*)(void *, CORBA_unsigned_long, CosNaming_BindingList**, CORBA_Environment *))&impl_CosNaming_BindingIterator_next_n,
    (void (*)(void *, CORBA_Environment *))&impl_CosNaming_BindingIterator_destroy,
};

static PortableServer_ServantBase__epv impl_CosNaming_NamingContextExt_base_epv = {
    NULL,             /* _private data */
    (void (*)(void*, CORBA_Environment*))& impl_CosNaming_NamingContextExt__destroy, /* finalize routine */
    NULL,             /* default_POA routine */
};

static POA_CosNaming_NamingContextExt__epv impl_CosNaming_NamingContextExt_epv = {
    NULL, /* _private */
    (CosNaming_NamingContextExt_StringName (*)(void *, CosNaming_Name*, CORBA_Environment *))&impl_CosNaming_NamingContextExt_to_string,
    (CosNaming_Name* (*)(void *, const CORBA_char *, CORBA_Environment *))&impl_CosNaming_NamingContextExt_to_name,
    (CosNaming_NamingContextExt_URLString (*)(void *, const CORBA_char *, const CORBA_char *, CORBA_Environment *))&impl_CosNaming_NamingContextExt_to_url,
    (CORBA_Object (*)(void *, const CORBA_char *, CORBA_Environment *))&impl_CosNaming_NamingContextExt_resolve_str,
};

static POA_CosNaming_NamingContext__epv impl_CosNaming_NamingContextExt_CosNaming_NamingContext_epv = 
{
     NULL, /* _private */
    (CORBA_boolean (*)(void *, const CORBA_char *, CORBA_Environment *))&impl_CosNaming_NamingContext__is_a,
    (void (*)(void *, CosNaming_Name*, CORBA_Object, CORBA_Environment *))&impl_CosNaming_NamingContext_bind,
    (void (*)(void *, CosNaming_Name*, CORBA_Object, CORBA_Environment *))&impl_CosNaming_NamingContext_rebind,
    (void (*)(void *, CosNaming_Name*, CosNaming_NamingContext, CORBA_Environment *))&impl_CosNaming_NamingContext_bind_context,
    (void (*)(void *, CosNaming_Name*, CosNaming_NamingContext, CORBA_Environment *))&impl_CosNaming_NamingContext_rebind_context,
    (CORBA_Object (*)(void *, CosNaming_Name*, CORBA_Environment *))&impl_CosNaming_NamingContext_resolve,
    (void (*)(void *, CosNaming_Name*, CORBA_Environment *))&impl_CosNaming_NamingContext_unbind,
    (CosNaming_NamingContext (*)(void *, CORBA_Environment *))&impl_CosNaming_NamingContext_new_context,
    (CosNaming_NamingContext (*)(void *, CosNaming_Name*, CORBA_Environment *))&impl_CosNaming_NamingContext_bind_new_context,
    (void (*)(void *, CORBA_Environment *))&impl_CosNaming_NamingContext_destroy,
    (void (*)(void *, CORBA_unsigned_long, CosNaming_BindingList**, CosNaming_BindingIterator*, CORBA_Environment *))&impl_CosNaming_NamingContext_list,
};


/*** vepv structures ***/
static POA_CosNaming_NamingContext__vepv impl_CosNaming_NamingContext_vepv = {
    &impl_CosNaming_NamingContext_base_epv,
    &impl_CosNaming_NamingContext_epv,
};

static POA_CosNaming_BindingIterator__vepv impl_CosNaming_BindingIterator_vepv = {
    &impl_CosNaming_BindingIterator_base_epv,
    &impl_CosNaming_BindingIterator_epv,
};

static POA_CosNaming_NamingContextExt__vepv impl_CosNaming_NamingContextExt_vepv = {
    &impl_CosNaming_NamingContextExt_base_epv,
    &impl_CosNaming_NamingContextExt_CosNaming_NamingContext_epv,
    &impl_CosNaming_NamingContextExt_epv,
};


/*** Stub implementations ***/
CORBA_CosNaming_NamingContext 
impl_CosNaming_NamingContext__create(PortableServer_POA poa, CORBA_Environment *ev)
{
  CORBA_CosNaming_NamingContext retval;
  impl_POA_CosNaming_NamingContext *newservant;
  PortableServer_ObjectId objid;

  newservant = (impl_POA_CosNaming_NamingContext *)RtORB_calloc(1, sizeof(impl_POA_CosNaming_NamingContext)," create...");
  newservant->servant.vepv = &impl_CosNaming_NamingContext_vepv;
  newservant->poa = poa;
  POA_CosNaming_NamingContext__init((PortableServer_Servant)newservant, ev);
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
impl_CosNaming_NamingContext__destroy(impl_POA_CosNaming_NamingContext *servant, CORBA_Environment *ev)
{
    CORBA_Object_release ((CORBA_Object) servant->poa, ev);

    /* No further remote method calls are delegated to 
    * servant and you may free your private attributes. */
   /* ------ free private attributes here ------ */
   /* ------ ---------- end ------------- ------ */

    POA_CosNaming_NamingContext__fini((PortableServer_Servant)servant, ev);

    RtORB_free (servant, "servant");
}



 /*** create servent **/ 
impl_POA_ServantBase * 
impl_CosNaming_NamingContext__create_servant(PortableServer_POA poa, CORBA_Environment *ev)
{
  impl_POA_CosNaming_NamingContext *newservant;

  newservant = (impl_POA_CosNaming_NamingContext *)RtORB_calloc(1, sizeof(impl_POA_CosNaming_NamingContext), "  create_servant");
  newservant->servant.vepv = &impl_CosNaming_NamingContext_vepv;
  newservant->poa = poa;
  POA_CosNaming_NamingContext__init((PortableServer_Servant)newservant, ev);
   /* Before servant is going to be activated all
    * private attributs must be initialized.  */

   /* ------ init private attributes here ------ */
   /* ------ ---------- end ------------- ------ */


  return (impl_POA_ServantBase*)newservant;
}

CORBA_boolean
impl_CosNaming_NamingContext__is_a(
  impl_POA_CosNaming_NamingContext *servant,
  const CORBA_char * id,
  CORBA_Environment *ev)
{
  CORBA_boolean retval;
  memset(ev, 0, sizeof(CORBA_Environment));
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object*>(object->impl_obj);
    CosNaming::NamingContext__impl *impl = dynamic_cast<CosNaming::NamingContext__impl *>(obj_);
    CORBA::Boolean_var var = impl->_is_a(CORBA_string_CInArg(id ));
  /* type = 27 */
    retval = var;
    ev->_cpp_flag = 1;
#else /*  C  */
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
#endif /* __cplusplus */ 
  return retval;
}

void
impl_CosNaming_NamingContext_bind(
  impl_POA_CosNaming_NamingContext *servant,
  CosNaming_Name* n,
  CORBA_Object obj,
  CORBA_Environment *ev)
{
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object *>(object->impl_obj);
    CosNaming::NamingContext__impl *impl = dynamic_cast<CosNaming::NamingContext__impl *>(obj_);
    impl->bind(CosNaming::Name_CInArg(n ), CORBA::Object_CInArg(obj ));
#else /* C */
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
#endif /*__cplusplus */
}

void
impl_CosNaming_NamingContext_rebind(
  impl_POA_CosNaming_NamingContext *servant,
  CosNaming_Name* n,
  CORBA_Object obj,
  CORBA_Environment *ev)
{
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object *>(object->impl_obj);
    CosNaming::NamingContext__impl *impl = dynamic_cast<CosNaming::NamingContext__impl *>(obj_);
    impl->rebind(CosNaming::Name_CInArg(n ), CORBA::Object_CInArg(obj ));
#else /* C */
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
#endif /*__cplusplus */
}

void
impl_CosNaming_NamingContext_bind_context(
  impl_POA_CosNaming_NamingContext *servant,
  CosNaming_Name* n,
  CosNaming_NamingContext nc,
  CORBA_Environment *ev)
{
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object *>(object->impl_obj);
    CosNaming::NamingContext__impl *impl = dynamic_cast<CosNaming::NamingContext__impl *>(obj_);
    impl->bind_context(CosNaming::Name_CInArg(n ), CosNaming::NamingContext_CInArg(nc ));
#else /* C */
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
#endif /*__cplusplus */
}

void
impl_CosNaming_NamingContext_rebind_context(
  impl_POA_CosNaming_NamingContext *servant,
  CosNaming_Name* n,
  CosNaming_NamingContext nc,
  CORBA_Environment *ev)
{
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object *>(object->impl_obj);
    CosNaming::NamingContext__impl *impl = dynamic_cast<CosNaming::NamingContext__impl *>(obj_);
    impl->rebind_context(CosNaming::Name_CInArg(n ), CosNaming::NamingContext_CInArg(nc ));
#else /* C */
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
#endif /*__cplusplus */
}

CORBA_Object
impl_CosNaming_NamingContext_resolve(
  impl_POA_CosNaming_NamingContext *servant,
  CosNaming_Name* n,
  CORBA_Environment *ev)
{
  CORBA_Object retval;
  memset(ev, 0, sizeof(CORBA_Environment));
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object*>(object->impl_obj);
    CosNaming::NamingContext__impl *impl = dynamic_cast<CosNaming::NamingContext__impl *>(obj_);
    CORBA::Object_var var = impl->resolve(CosNaming::Name_CInArg(n ));
    retval = var->_impl;
    ev->_cpp_flag = 1;
#else /*  C  */
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
#endif /* __cplusplus */ 
  return retval;
}

void
impl_CosNaming_NamingContext_unbind(
  impl_POA_CosNaming_NamingContext *servant,
  CosNaming_Name* n,
  CORBA_Environment *ev)
{
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object *>(object->impl_obj);
    CosNaming::NamingContext__impl *impl = dynamic_cast<CosNaming::NamingContext__impl *>(obj_);
    impl->unbind(CosNaming::Name_CInArg(n ));
#else /* C */
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
#endif /*__cplusplus */
}

CosNaming_NamingContext
impl_CosNaming_NamingContext_new_context(
  impl_POA_CosNaming_NamingContext *servant,
  CORBA_Environment *ev)
{
  CosNaming_NamingContext retval;
  memset(ev, 0, sizeof(CORBA_Environment));
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object*>(object->impl_obj);
    CosNaming::NamingContext__impl *impl = dynamic_cast<CosNaming::NamingContext__impl *>(obj_);
    CosNaming::NamingContext_var var = impl->new_context();
    retval = var.impl();
    ev->_cpp_flag = 1;
#else /*  C  */
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
#endif /* __cplusplus */ 
  return retval;
}

CosNaming_NamingContext
impl_CosNaming_NamingContext_bind_new_context(
  impl_POA_CosNaming_NamingContext *servant,
  CosNaming_Name* n,
  CORBA_Environment *ev)
{
  CosNaming_NamingContext retval;
  memset(ev, 0, sizeof(CORBA_Environment));
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object*>(object->impl_obj);
    CosNaming::NamingContext__impl *impl = dynamic_cast<CosNaming::NamingContext__impl *>(obj_);
    CosNaming::NamingContext_var var = impl->bind_new_context(CosNaming::Name_CInArg(n ));
    retval = var.impl();
    ev->_cpp_flag = 1;
#else /*  C  */
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
#endif /* __cplusplus */ 
  return retval;
}

void
impl_CosNaming_NamingContext_destroy(
  impl_POA_CosNaming_NamingContext *servant,
  CORBA_Environment *ev)
{
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object *>(object->impl_obj);
    CosNaming::NamingContext__impl *impl = dynamic_cast<CosNaming::NamingContext__impl *>(obj_);
    impl->destroy();
#else /* C */
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
#endif /*__cplusplus */
}

void
impl_CosNaming_NamingContext_list(
  impl_POA_CosNaming_NamingContext *servant,
  CORBA_unsigned_long how_many,
  CosNaming_BindingList** bl,
  CosNaming_BindingIterator* bi,
  CORBA_Environment *ev)
{
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object *>(object->impl_obj);
    CosNaming::NamingContext__impl *impl = dynamic_cast<CosNaming::NamingContext__impl *>(obj_);
    impl->list(CORBA::ULong_CInArg(how_many ), CosNaming::BindingList_COutArg(bl ), CosNaming::BindingIterator_COutArg(bi ));
#else /* C */
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
#endif /*__cplusplus */
}

CORBA_CosNaming_BindingIterator 
impl_CosNaming_BindingIterator__create(PortableServer_POA poa, CORBA_Environment *ev)
{
  CORBA_CosNaming_BindingIterator retval;
  impl_POA_CosNaming_BindingIterator *newservant;
  PortableServer_ObjectId objid;

  newservant = (impl_POA_CosNaming_BindingIterator *)RtORB_calloc(1, sizeof(impl_POA_CosNaming_BindingIterator)," create...");
  newservant->servant.vepv = &impl_CosNaming_BindingIterator_vepv;
  newservant->poa = poa;
  POA_CosNaming_BindingIterator__init((PortableServer_Servant)newservant, ev);
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
impl_CosNaming_BindingIterator__destroy(impl_POA_CosNaming_BindingIterator *servant, CORBA_Environment *ev)
{
    CORBA_Object_release ((CORBA_Object) servant->poa, ev);

    /* No further remote method calls are delegated to 
    * servant and you may free your private attributes. */
   /* ------ free private attributes here ------ */
   /* ------ ---------- end ------------- ------ */

    POA_CosNaming_BindingIterator__fini((PortableServer_Servant)servant, ev);

    RtORB_free (servant, "servant");
}



 /*** create servent **/ 
impl_POA_ServantBase * 
impl_CosNaming_BindingIterator__create_servant(PortableServer_POA poa, CORBA_Environment *ev)
{
  impl_POA_CosNaming_BindingIterator *newservant;

  newservant = (impl_POA_CosNaming_BindingIterator *)RtORB_calloc(1, sizeof(impl_POA_CosNaming_BindingIterator), "  create_servant");
  newservant->servant.vepv = &impl_CosNaming_BindingIterator_vepv;
  newservant->poa = poa;
  POA_CosNaming_BindingIterator__init((PortableServer_Servant)newservant, ev);
   /* Before servant is going to be activated all
    * private attributs must be initialized.  */

   /* ------ init private attributes here ------ */
   /* ------ ---------- end ------------- ------ */


  return (impl_POA_ServantBase*)newservant;
}

CORBA_boolean
impl_CosNaming_BindingIterator_next_one(
  impl_POA_CosNaming_BindingIterator *servant,
  CosNaming_Binding** b,
  CORBA_Environment *ev)
{
  CORBA_boolean retval;
  memset(ev, 0, sizeof(CORBA_Environment));
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object*>(object->impl_obj);
    CosNaming::BindingIterator__impl *impl = dynamic_cast<CosNaming::BindingIterator__impl *>(obj_);
    CORBA::Boolean_var var = impl->next_one(CosNaming::Binding_COutArg(b ));
  /* type = 27 */
    retval = var;
    ev->_cpp_flag = 1;
#else /*  C  */
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
#endif /* __cplusplus */ 
  return retval;
}

CORBA_boolean
impl_CosNaming_BindingIterator_next_n(
  impl_POA_CosNaming_BindingIterator *servant,
  CORBA_unsigned_long how_many,
  CosNaming_BindingList** bl,
  CORBA_Environment *ev)
{
  CORBA_boolean retval;
  memset(ev, 0, sizeof(CORBA_Environment));
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object*>(object->impl_obj);
    CosNaming::BindingIterator__impl *impl = dynamic_cast<CosNaming::BindingIterator__impl *>(obj_);
    CORBA::Boolean_var var = impl->next_n(CORBA::ULong_CInArg(how_many ), CosNaming::BindingList_COutArg(bl ));
  /* type = 27 */
    retval = var;
    ev->_cpp_flag = 1;
#else /*  C  */
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
#endif /* __cplusplus */ 
  return retval;
}

void
impl_CosNaming_BindingIterator_destroy(
  impl_POA_CosNaming_BindingIterator *servant,
  CORBA_Environment *ev)
{
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object *>(object->impl_obj);
    CosNaming::BindingIterator__impl *impl = dynamic_cast<CosNaming::BindingIterator__impl *>(obj_);
    impl->destroy();
#else /* C */
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
#endif /*__cplusplus */
}

CORBA_CosNaming_NamingContextExt 
impl_CosNaming_NamingContextExt__create(PortableServer_POA poa, CORBA_Environment *ev)
{
  CORBA_CosNaming_NamingContextExt retval;
  impl_POA_CosNaming_NamingContextExt *newservant;
  PortableServer_ObjectId objid;

  newservant = (impl_POA_CosNaming_NamingContextExt *)RtORB_calloc(1, sizeof(impl_POA_CosNaming_NamingContextExt)," create...");
  newservant->servant.vepv = &impl_CosNaming_NamingContextExt_vepv;
  newservant->poa = poa;
  POA_CosNaming_NamingContextExt__init((PortableServer_Servant)newservant, ev);
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
impl_CosNaming_NamingContextExt__destroy(impl_POA_CosNaming_NamingContextExt *servant, CORBA_Environment *ev)
{
    CORBA_Object_release ((CORBA_Object) servant->poa, ev);

    /* No further remote method calls are delegated to 
    * servant and you may free your private attributes. */
   /* ------ free private attributes here ------ */
   /* ------ ---------- end ------------- ------ */

    POA_CosNaming_NamingContextExt__fini((PortableServer_Servant)servant, ev);

    RtORB_free (servant, "servant");
}



 /*** create servent **/ 
impl_POA_ServantBase * 
impl_CosNaming_NamingContextExt__create_servant(PortableServer_POA poa, CORBA_Environment *ev)
{
  impl_POA_CosNaming_NamingContextExt *newservant;

  newservant = (impl_POA_CosNaming_NamingContextExt *)RtORB_calloc(1, sizeof(impl_POA_CosNaming_NamingContextExt), "  create_servant");
  newservant->servant.vepv = &impl_CosNaming_NamingContextExt_vepv;
  newservant->poa = poa;
  POA_CosNaming_NamingContextExt__init((PortableServer_Servant)newservant, ev);
   /* Before servant is going to be activated all
    * private attributs must be initialized.  */

   /* ------ init private attributes here ------ */
   /* ------ ---------- end ------------- ------ */


  return (impl_POA_ServantBase*)newservant;
}

CosNaming_NamingContextExt_StringName
impl_CosNaming_NamingContextExt_to_string(
  impl_POA_CosNaming_NamingContextExt *servant,
  CosNaming_Name* n,
  CORBA_Environment *ev)
{
  CosNaming_NamingContextExt_StringName retval;
  memset(ev, 0, sizeof(CORBA_Environment));
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object*>(object->impl_obj);
    CosNaming::NamingContextExt__impl *impl = dynamic_cast<CosNaming::NamingContextExt__impl *>(obj_);
    CosNaming::NamingContextExt::StringName_var var = impl->to_string(CosNaming::Name_CInArg(n ));
  /* type = 25 */
    retval = var;
    ev->_cpp_flag = 1;
#else /*  C  */
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
#endif /* __cplusplus */ 
  return retval;
}

CosNaming_Name*
impl_CosNaming_NamingContextExt_to_name(
  impl_POA_CosNaming_NamingContextExt *servant,
  const CORBA_char * sn,
  CORBA_Environment *ev)
{
  CosNaming_Name* retval;
  memset(ev, 0, sizeof(CORBA_Environment));
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object*>(object->impl_obj);
    CosNaming::NamingContextExt__impl *impl = dynamic_cast<CosNaming::NamingContextExt__impl *>(obj_);
    CosNaming::Name_var var = impl->to_name(CosNaming::NamingContextExt::StringName_CInArg(sn ));
    retval = var->_retn();
    ev->_cpp_flag = 1;
#else /*  C  */
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
#endif /* __cplusplus */ 
  return retval;
}

CosNaming_NamingContextExt_URLString
impl_CosNaming_NamingContextExt_to_url(
  impl_POA_CosNaming_NamingContextExt *servant,
  const CORBA_char * addr,
  const CORBA_char * sn,
  CORBA_Environment *ev)
{
  CosNaming_NamingContextExt_URLString retval;
  memset(ev, 0, sizeof(CORBA_Environment));
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object*>(object->impl_obj);
    CosNaming::NamingContextExt__impl *impl = dynamic_cast<CosNaming::NamingContextExt__impl *>(obj_);
    CosNaming::NamingContextExt::URLString_var var = impl->to_url(CosNaming::NamingContextExt::Address_CInArg(addr ), CosNaming::NamingContextExt::StringName_CInArg(sn ));
  /* type = 25 */
    retval = var;
    ev->_cpp_flag = 1;
#else /*  C  */
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
#endif /* __cplusplus */ 
  return retval;
}

CORBA_Object
impl_CosNaming_NamingContextExt_resolve_str(
  impl_POA_CosNaming_NamingContextExt *servant,
  const CORBA_char * n,
  CORBA_Environment *ev)
{
  CORBA_Object retval;
  memset(ev, 0, sizeof(CORBA_Environment));
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object*>(object->impl_obj);
    CosNaming::NamingContextExt__impl *impl = dynamic_cast<CosNaming::NamingContextExt__impl *>(obj_);
    CORBA::Object_var var = impl->resolve_str(CosNaming::NamingContextExt::StringName_CInArg(n ));
    retval = var->_impl;
    ev->_cpp_flag = 1;
#else /*  C  */
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
#endif /* __cplusplus */ 
  return retval;
}

CORBA_boolean
impl_CosNaming_NamingContextExt__is_a(
    impl_POA_CosNaming_NamingContextExt *servant,
    const CORBA_char * id,
    CORBA_Environment *ev)
{
  memset(ev, 0, sizeof(CORBA_Environment));
  CORBA_boolean retval;
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object*>(object->impl_obj);
    CosNaming::NamingContext__impl *impl = dynamic_cast<CosNaming::NamingContext__impl *>(obj_);
    CORBA::Boolean_var var = impl->_is_a(CORBA_string_CInArg(id ) );
  retval = var;
  ev->_cpp_flag = 1;
#else /*  C  */
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
#endif /* __cplusplus */ 
  return retval;
}

void
impl_CosNaming_NamingContextExt_bind(
    impl_POA_CosNaming_NamingContextExt *servant,
    CosNaming_Name* n,
    CORBA_Object obj,
    CORBA_Environment *ev)
{
  memset(ev, 0, sizeof(CORBA_Environment));
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
}

void
impl_CosNaming_NamingContextExt_rebind(
    impl_POA_CosNaming_NamingContextExt *servant,
    CosNaming_Name* n,
    CORBA_Object obj,
    CORBA_Environment *ev)
{
  memset(ev, 0, sizeof(CORBA_Environment));
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
}

void
impl_CosNaming_NamingContextExt_bind_context(
    impl_POA_CosNaming_NamingContextExt *servant,
    CosNaming_Name* n,
    CosNaming_NamingContext nc,
    CORBA_Environment *ev)
{
  memset(ev, 0, sizeof(CORBA_Environment));
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
}

void
impl_CosNaming_NamingContextExt_rebind_context(
    impl_POA_CosNaming_NamingContextExt *servant,
    CosNaming_Name* n,
    CosNaming_NamingContext nc,
    CORBA_Environment *ev)
{
  memset(ev, 0, sizeof(CORBA_Environment));
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
}

CORBA_Object
impl_CosNaming_NamingContextExt_resolve(
    impl_POA_CosNaming_NamingContextExt *servant,
    CosNaming_Name* n,
    CORBA_Environment *ev)
{
  memset(ev, 0, sizeof(CORBA_Environment));
  CORBA_Object retval;
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object*>(object->impl_obj);
    CosNaming::NamingContext__impl *impl = dynamic_cast<CosNaming::NamingContext__impl *>(obj_);
    CORBA::Object_var var = impl->resolve(CosNaming::Name_CInArg(n ) );
  retval = var->_impl;
  ev->_cpp_flag = 1;
#else /*  C  */
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
#endif /* __cplusplus */ 
  return retval;
}

void
impl_CosNaming_NamingContextExt_unbind(
    impl_POA_CosNaming_NamingContextExt *servant,
    CosNaming_Name* n,
    CORBA_Environment *ev)
{
  memset(ev, 0, sizeof(CORBA_Environment));
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
}

CosNaming_NamingContext
impl_CosNaming_NamingContextExt_new_context(
    impl_POA_CosNaming_NamingContextExt *servant,
    CORBA_Environment *ev)
{
  memset(ev, 0, sizeof(CORBA_Environment));
  CosNaming_NamingContext retval;
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object*>(object->impl_obj);
    CosNaming::NamingContext__impl *impl = dynamic_cast<CosNaming::NamingContext__impl *>(obj_);
    CosNaming::NamingContext_var var = impl->new_context( );
  retval = var.impl();
  ev->_cpp_flag = 1;
#else /*  C  */
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
#endif /* __cplusplus */ 
  return retval;
}

CosNaming_NamingContext
impl_CosNaming_NamingContextExt_bind_new_context(
    impl_POA_CosNaming_NamingContextExt *servant,
    CosNaming_Name* n,
    CORBA_Environment *ev)
{
  memset(ev, 0, sizeof(CORBA_Environment));
  CosNaming_NamingContext retval;
#if __cplusplus
    CORBA_Object object = impl_POA_ServantBase_to_Object((impl_POA_ServantBase *)servant);
    CORBA::Object *obj_ = reinterpret_cast<CORBA::Object*>(object->impl_obj);
    CosNaming::NamingContext__impl *impl = dynamic_cast<CosNaming::NamingContext__impl *>(obj_);
    CosNaming::NamingContext_var var = impl->bind_new_context(CosNaming::Name_CInArg(n ) );
  retval = var.impl();
  ev->_cpp_flag = 1;
#else /*  C  */
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
#endif /* __cplusplus */ 
  return retval;
}

void
impl_CosNaming_NamingContextExt_destroy(
    impl_POA_CosNaming_NamingContextExt *servant,
    CORBA_Environment *ev)
{
  memset(ev, 0, sizeof(CORBA_Environment));
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
}

void
impl_CosNaming_NamingContextExt_list(
    impl_POA_CosNaming_NamingContextExt *servant,
    CORBA_unsigned_long how_many,
    CosNaming_BindingList** bl,
    CosNaming_BindingIterator* bi,
    CORBA_Environment *ev)
{
  memset(ev, 0, sizeof(CORBA_Environment));
    /* ------   insert method code here   ------ */
    /* ------ ---------- end ------------ ------ */
}


/*** Boohoo! ***/
