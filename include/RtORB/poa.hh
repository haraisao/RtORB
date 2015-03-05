/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*!
 * @file poa.hh
 * @if jp
 * @brief (TODO)
 * @else
 * @brief (TODO)
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */
#ifndef __POA_HH__
#define __POA_HH__
#include <RtORB/corba-defs.hh>
#include <RtORB/corba-var.hh>

namespace PortableServer{
  class POA;
  typedef POA * POA_ptr;

  class ObjectId;
  typedef ObjectId * ObjectId_ptr;
  typedef CORBA_var<ObjectId> ObjectId_var;
  //typedef ObjectId * ObjectId_var;

  class RefCountServantBase;

  class POAManager;
  typedef POAManager * POAManager_ptr;
  typedef POAManager * POAManager_var;

  class ServantBase;

  /*!
   * @brief Abstract (TODO)
   * @param oid (TODO)
   * @return (TODO)
   */
  ObjectId string_to_ObjectId(const CORBA_string oid);

 /*
  *  Class POA
  */
  /*!
   * @class POA
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */ 
  class POA : public CORBA::Object
  {
  public:
    PortableServer_POA _impl;   /*!< TODO */
    // int32_t refs;

  public:
    POA();
    virtual ~POA();
    /*!
     * @brief (TODO)
     */
    POA(PortableServer_POA poa);
    /*!
     * @brief (TODO)
     */
    POA(const POA& other);
    /*!
     * @brief (TODO)
     */
    POA& operator=(const POA& other);

    /*!
     * @brief (TODO)
     */
    ObjectId_ptr activate_object(ServantBase *sb) throw (CORBA::Exception);
    /*!
     * @brief (TODO)
     */
    void activate_object_with_id(ObjectId_ptr id, ServantBase *sb) throw (CORBA::Exception);
    /*!
     * @brief (TODO)
     */
    void deactivate_object(const ObjectId &);

    /*!
     * @brief (TODO)
     */
    void destroy(CORBA::Boolean etherealize, CORBA::Boolean wait_for_completion);

    /*!
     * @brief (TODO)
     */
    ObjectId_ptr servant_to_id(ServantBase *sb) throw (CORBA::Exception);
    /*!
     * @brief (TODO)
     */
    CORBA::Object_ptr id_to_reference(const ObjectId &) throw (CORBA::Exception);
    /*!
     * @brief (TODO)
     */
    CORBA::Object_ptr servant_to_reference(ServantBase *) throw (CORBA::Exception);

    /*!
     * @brief (TODO)
     */
    POAManager_ptr the_POAManager();
    /*!
     * @brief (TODO)
     */
    static POA_ptr _narrow(CORBA::Object_ptr obj);
    /*!
     * @brief (TODO)
     */
    static POA_ptr _nil();
    /*!
     * @brief (TODO)
     */
    static POA_ptr _duplicate(POA_ptr);

    /*!
     * @brief (TODO)
     */
    class ObjectNotActive: ::CORBA::UserException {
    };
    /*!
     * @brief (TODO)
     */
    class ServantAlreadyActive: ::CORBA::UserException {
    };
    /*!
     * @brief (TODO)
     */
    class ObjectAlreadyActive: ::CORBA::UserException {
    };
    /*!
     * @brief (TODO)
     */
    class ServantNotActive: ::CORBA::UserException {
      public:
        /*!
         * @brief (TODO)
         */
        char *_name() { return _msg; }
    };
    /*!
     * @brief (TODO)
     */
    class WrongPolicy: ::CORBA::UserException {
      public:
        /*!
         * @brief (TODO)
         */
        char *_name() { return _msg; }
    };
  };

  /*!
   * @class POA_var
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */ 
  class POA_var
  {
  public:
    POA_var();
    /*!
     * @brief (TODO)
     */
    POA_var(POA_ptr ptr);
    /*!
     * @brief (TODO)
     */
    POA_var(const POA_var &var);
	  
    ~POA_var();
    /*!
     * @brief (TODO)
     */
    POA_ptr operator->() { return _ptr; }

    /*!
     * @brief (TODO)
     */
    operator POA_ptr () { return _ptr; }
    /*!
     * @brief (TODO)
     */
    operator CORBA::Object_ptr () { return CORBA::Object_ptr(_ptr); }

    /*!
     * @brief (TODO)
     */
    POA_var & operator=(POA_ptr ptr) {
      _ptr = ptr;
      return *this;
    }
	  
  private:
    POA * _ptr;   /*!< TODO */
  };
	

 /*
  *  Class ObjectId
  */
  /*!
   * @class ObjectId
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */ 
  class ObjectId 
  {
     public:
       PortableServer_ObjectId _impl;    /*!< TODO */

     public:
       ObjectId();
       /*!
        * @brief (TODO)
        */
       ObjectId(PortableServer_ObjectId);
  };

 /*
  *  Class ServantBase
  */
  /*!
   * @class ServantBase
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */ 
  class ServantBase : public virtual CORBA::Object
  {
  public:
    CORBA_Environment _ev;                /*!< TODO */
    impl_POA_ServantBase * poa_servant;   /*!< TODO */
    int32_t refs;                           /*!< TODO */
  public:
    ServantBase();
    ~ServantBase();
    /*!
     * @brief (TODO)
     */
    void _add_ref()
    {
      refs++;
    }
    /*!
     * @brief (TODO)
     */
    void _remove_ref()
    {
      refs--;
      if (refs < 0) {
	fprintf(stderr, "ServantBase: negative reference count\n");
	return;
      } else if (refs == 0) {
	delete this;
      }
    }

    /*!
     * @brief (TODO)
     */
    PortableServer_ServantBase * getServantBase()
    {
      return poa_servant != NULL? &poa_servant->servant: NULL;
    }
    POA_ptr _default_POA();

    //static POA_ptr _root_POA(CORBA_Environment*);
    /*!
     * @brief (TODO)
     */
    static POA_ptr _root_POA() throw (CORBA::Exception);

    /*!
     * @brief (TODO)
     */
    CORBA_Object __this(CORBA_Environment *) throw (CORBA::Exception);
    CORBA_Object __this() throw (CORBA::Exception);
  };

 /*
  *  Class RefCountServantBase
  */
  /*!
   * @class RefCountServantBase
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  class RefCountServantBase : public virtual ServantBase
  {
  public:
    PortableServer_POA _poa;  /*!< TODO */
	
  };

 /*
  *  Class POAManager
  */
  /*!
   * @class POAManager
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  class POAManager : public ServantBase
  {
  public:
    PortableServer_POAManager _mgr;  /*!< TODO */
    /*!
     * @brief (TODO)
     */
    POAManager(PortableServer_POAManager mgr);
  public:
    /*!
     * @brief (TODO)
     */
    void activate() throw (CORBA::Exception);
    /*!
     * @brief (TODO)
     */
    void deactivate(CORBA::Boolean etherealize,
		    CORBA::Boolean wait_for_completion) throw (CORBA::Exception);

  };

  /*!
   * @class Servant
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  class Servant{
    public:
     POA_var poa;                      /*!< TODO */
     CORBA::Object_var obj;            /*!< TODO */
     PortableServer_Servant _servant;  /*!< TODO */

    public:
     Servant(){}
     /*!
      * @brief (TODO)
      */
     Servant(PortableServer_ServantBase *sb){
//        poa = new POA(sb->poa);
//	obj = new CORBA::Object(sb->obj);
     }
  };

};

#endif
