/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*!
 * @file poa.cc
 * @if jp
 * @brief (TODO)
 * @else
 * @brief (TODO)
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#include <RtORB/corba.hh>


namespace PortableServer{

//// Class POA
  POA::POA() :  _impl(NULL) { //, refs(0) 
     memset(&ev, 0, sizeof(CORBA_Environment));
  }

  POA::POA(PortableServer_POA poa) : _impl(poa) { // , refs(0) 
    //refs++;
  }

  POA::~POA(){
    /*
    refs--;
    if(_impl && refs < 0) {
      PortableServer_POA_destory(_impl, &ev);
      _impl = NULL;
    }
    */
  }

  POA::POA(const POA& other){
     _impl = other._impl;
     //refs++;
  }

  POA& POA::operator=(const POA& other){
     _impl = other._impl;
     // refs++;
     return *this;
  }

  POA_ptr POA::_nil() {
    return NULL;
  }

  POA_ptr POA::_duplicate(POA_ptr ptr)
  {
    if (!ptr) { return NULL; }
    //ptr->refs++;
    return new POA(*ptr);
  }

  POA_ptr POA::_narrow(CORBA::Object_ptr ptr){
    PortableServer_POA impl = (PortableServer_POA) ptr->impl()->poa;
    if (!impl) { return NULL; }
    return new POA(impl);
  }

  ObjectId_ptr POA::activate_object(ServantBase *sb) throw (CORBA::Exception) {
     PortableServer_ObjectId objid = PortableServer_POA_activate_object(_impl, sb->getServantBase(), &ev);
     catchAndThrowDefaultException(&ev);
     
     return new ObjectId(objid);
  }

  void POA::deactivate_object(const ObjectId &id)
  {
    if (!id._impl) { return; }
    PortableServer_POA_deactivate_object(_impl, id._impl, &ev);
  }


  POAManager_ptr POA::the_POAManager(){

    PortableServer_POAManager mgr = PortableServer_POA__get_the_POAManager(_impl, &ev);
    if (!mgr) { return NULL; }

    return new POAManager(mgr);
  }

  void POA::destroy(CORBA::Boolean etherealize, CORBA::Boolean wait_for_completion)
  {
    PortableServer_POA_destory(_impl, &ev);
  }

  ObjectId_ptr POA::servant_to_id(ServantBase *sb) throw (CORBA::Exception)
  {
    if (!sb) {
      throw CORBA::SystemException("POA::servant_to_id : ServantBase is NULL");
    }
    PortableServer_ServantBase * sb_impl = sb->getServantBase();
    assert(sb_impl);

    
    RtORB_POA_Object *poa_obj = (RtORB_POA_Object *)(sb_impl->_private);

    if(!poa_obj) {
      throw CORBA::SystemException("PortableServer::sevant_to_id : RtORB_POA_Object is NULL");
    }
    return new ObjectId(poa_obj->objectId);

  }
      
  CORBA::Object_ptr POA::id_to_reference(const ObjectId &id) throw (CORBA::Exception)
  {
    if (! (id._impl) || ! (id._impl->_this) ) {
      throw CORBA::SystemException("POA::id_to_reference : CORBA_Object is NULL");
    }
    return id._impl->_this;
  }

  CORBA::Object_ptr POA::servant_to_reference(ServantBase *sb) throw (CORBA::Exception)
  {
    if (!_impl) {
      throw CORBA::SystemException("POA::servant_to_reference : POA is NULL");
    }
    PortableServer_ServantBase *sb_ = NULL;
    if (!sb || (sb_ = sb->getServantBase()) == NULL) {
      throw CORBA::SystemException("POA::servant_to_reference : ServantBase is NULL");
    }
    CORBA_Object obj = PortableServer_POA_servant_to_reference(_impl, sb_, &ev);
    catchAndThrowDefaultException(&ev);
    if (!obj) { return CORBA::Object_ptr(); }
    return CORBA::Object_ptr(obj);
  }

  POA_var::POA_var() : _ptr(NULL) {}
  
  POA_var::POA_var(POA * ptr) : _ptr(ptr) {}
  POA_var::POA_var(const POA_var &var) : _ptr(NULL)
  {
    if (var._ptr) {
      _ptr = new POA(*_ptr);
    }
  }

  POA_var::~POA_var()
  {
    if (_ptr) {
      delete _ptr; _ptr = NULL;
    }
  }
  

//// Class ObjectId
  ObjectId::ObjectId(){
  }

  ObjectId::ObjectId(PortableServer_ObjectId id) : _impl(id) {
  }
//// Class ServantBase
  ServantBase::ServantBase() : poa_servant(NULL), refs(0) {
    memset(&_ev, 0, sizeof(CORBA_Environment));
  }

  ServantBase::~ServantBase(){
    refs--;
    //if(_obj && refs < 0) RtORB_free(_obj, "~ServantBase");
  }


  CORBA_Object ServantBase::__this(CORBA_Environment *ev) throw (CORBA::Exception) {
    PortableServer_ServantBase *sb = getServantBase();
    if (!sb) { return CORBA::Object_ptr(); }
    RtORB_POA_Object *poa_obj = (RtORB_POA_Object *)(sb->_private);

    if(!poa_obj) {
      throw CORBA::SystemException("PortableServer::_this : RtORB_POA_Object is NULL");
    }

    return poa_obj->obj;
  }

  CORBA_Object ServantBase::__this() throw (CORBA::Exception) {
    PortableServer_ServantBase *sb = getServantBase();
    if (!sb) { return CORBA::Object_ptr(); }
    RtORB_POA_Object *poa_obj = (RtORB_POA_Object *)(sb->_private);

    if(!poa_obj) {
      throw CORBA::SystemException("PortableServer::_this : RtORB_POA_Object is NULL");
    }

    return poa_obj->obj;
  }

  POA_ptr ServantBase::_default_POA()
  {
    PortableServer_ServantBase *sb = getServantBase();
    PortableServer_POA poa = PortableServer_ServantBase__default_POA(sb, &_ev);
    catchDefaultException(&_ev);
    if (!poa) { return NULL; }
    return new POA(poa);
  }

  POA_ptr ServantBase::_root_POA() throw (CORBA::Exception)
  {
    CORBA_Environment ev;
    memset(&ev, 0x00, sizeof(ev));
    PortableServer_POA poa = PortableServer_root_POA(&ev);
    catchAndThrowDefaultException(&ev);
    if (!poa) { return NULL; }
    return new POA(poa);

  }


  POAManager::POAManager(PortableServer_POAManager mgr) : ServantBase(), _mgr(mgr)
  {

  }

  void POAManager::activate() throw (CORBA::Exception) {
    PortableServer_POAManager_activate(_mgr, &_ev);
    catchAndThrowDefaultException(&_ev);
  }

  void POAManager::deactivate(CORBA::Boolean etherealize, CORBA::Boolean wait_for_completion) throw (CORBA::Exception)
  {
	  PortableServer_POAManager_deactivate(_mgr,
					       etherealize,
					       wait_for_completion,
					       &_ev);
	  catchAndThrowDefaultException(&_ev);
  }

  ObjectId string_to_ObjectId(const CORBA_string oid){
    CORBA::Object_ptr ptr = CORBA::_root_ORB_->string_to_object(oid);
     
    ObjectId objid(PortableServer_ObjectId(ptr.impl()));
    return objid; 
  }

  void POA::activate_object_with_id(ObjectId_ptr id, ServantBase *sb) throw (CORBA::Exception){

     return;

  }


};
