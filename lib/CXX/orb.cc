/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*!
 * @file orb.cc
 * @if jp
 * @brief (TODO)
 * @else
 * @brief (TODO)
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#include <RtORB/corba.hh>


namespace CORBA
{
  ORB *_root_ORB_=NULL;
  int _root_ORB_ref_count;    /*!< TODO */


   ORB_ptr ORB_init(int& argc, char **argv){
     _root_ORB_ref_count = 0;
     ORB_ptr orb =  new ORB();
     CORBA_ORB impl = CORBA_ORB_init(&argc, argv, 0, &orb->ev);
     if(impl) orb->__set_impl(impl);
     _root_ORB_ = orb;
     return orb;
   }

   
//// Class ORB
  ORB::ORB(){
    _impl = NULL;
    memset(&ev, 0, sizeof(CORBA_Environment));
  }

  ORB::ORB(CORBA_ORB orb){
    _root_ORB_ref_count++;
    _impl = orb;
    memset(&ev, 0, sizeof(CORBA_Environment));
  }

  ORB::~ORB(){
//    std::cout << "****Call deConstructor "<< std::endl;
     _root_ORB_ref_count--;

    if (_root_ORB_ref_count <= 0 && _impl) {
      CORBA_ORB_shutdown(_impl, 0,  &ev);
      CORBA_ORB_destroy(_impl, &ev);
      _impl = NULL;
    }

  }

  ORB::ORB(const ORB& obj){
//    std::cout << "****Call Copy Constructor "<< std::endl;
    _root_ORB_ref_count++;
  }

  void ORB::__set_impl(CORBA_ORB orb){
    _impl = orb;
    _root_ORB_ref_count++;
  }


  Object_ptr ORB::resolve_initial_references(char *name) throw (Exception) {
    CORBA_Object obj;
    if(!_impl) return Object_ptr();
    memset(&ev, 0, sizeof(CORBA_Environment));

    obj = CORBA_ORB_resolve_initial_references(_impl, name, &ev);
    catchAndThrowDefaultException(&ev);

    if (obj) { return Object_ptr(new Object(obj)); }

//    std::cerr << "Error in resolve_initial_references." << std::endl; 
    return Object_ptr();
  }

  boolean ORB::__is_nil(){
    if(_impl == NULL) return TRUE;
    return FALSE;
  }

  String_ptr ORB::object_to_string(Object_ptr obj) throw (Exception) {
    memset(&ev, 0, sizeof(CORBA_Environment));
    unsigned char * str = CORBA_ORB_object_to_string(_impl,  obj.impl(), &ev);
    catchAndThrowDefaultException(&ev);
    return (String_ptr)str;
  }

  String_ptr ORB::object_to_string2(CORBA_Object obj) throw (Exception) {
    memset(&ev, 0, sizeof(CORBA_Environment));
    unsigned char * str = CORBA_ORB_object_to_string(_impl,  obj, &ev);
    catchAndThrowDefaultException(&ev);
    return (String_ptr)str;
  }

  Object_ptr ORB::string_to_object(const char* str) throw (Exception)
  {
    char *p = const_cast<char*>(str);
    ev._major = 0;
    CORBA_Object obj = CORBA_ORB_string_to_object(_impl, (unsigned char*)str, &ev);

    catchAndThrowDefaultException(&ev);
    return Object_ptr(obj);
  }

  void ORB::run() {
    CORBA_ORB_run(_impl, &ev);
    return;
  }

  CORBA::Boolean ORB::work_pending()
  {
    return CORBA_ORB_work_pending(_impl, &ev);
  }
  
  void ORB::perform_work()
  {
    return CORBA_ORB_perform_work(_impl, &ev);
  }

  
  ORB * ORB::_duplicate()
  {
    return new ORB(*this);
  }
  
  ORB::Ptr ORB::_nil()
  {
    return NULL;
  }
  

  void ORB::shutdown(boolean flag){
    CORBA_ORB_shutdown(_impl, flag,  &ev);
    return;
  }

  void ORB::destroy(){
    if (_impl) {
      CORBA_ORB_destroy(_impl, &ev);
      _impl = NULL;
    }
    return;
  }

  /* ##########################
   *
   *  ORB_var
   *
   * ########################## */

  ORB_var::~ORB_var() {

    if (_ptr) {
      delete _ptr; _ptr = NULL;
    }

  }

  ORB_var & ORB_var::operator=(ORB_ptr ptr) {
    _root_ORB_ref_count++;
    if (_ptr) {
      delete _ptr; _ptr = NULL;
    }
    _ptr = ptr;
    return *this;
  }
}
