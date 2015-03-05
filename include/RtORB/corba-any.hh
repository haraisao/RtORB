/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*!
 * @file corba-any.hh
 * @if jp
 * @brief (TODO)
 * @else
 * @brief (TODO)
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */
#ifndef __CORBA_ANY_HH__
#define __CORBA_ANY_HH__

namespace CORBA {
	
  typedef CORBA_any Any;
  typedef Any * Any_ptr;

  /*!
   * @class Any_out
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */ 
  class Any_out {
    Any ** _pp;  /*!< TODO */
  public:
    /*!
     * @brief (TODO)
     */
    Any_out() : _pp(NULL) {}
    /*!
     * @brief (TODO)
     */
    Any_out(Any **pp) : _pp(pp) {}
    /*!
     * @brief (TODO)
     */
    Any * ptr() { return *_pp; }

    /*!
     * @brief (TODO)
     */
    Any_out & operator=(Any *p) {
      *_pp = p;
      return *this;
    }

    /*!
     * @brief (TODO)
     */
    operator Any ** () { return _pp; }
  };

  /*!
   * @class Any_var
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */ 
  class Any_var {
  public:
    /*!
     * @brief (TODO)
     */
    Any_var() : _ptr(NULL), _out(NULL) {}
    /*!
     * @brief (TODO)
     */
    Any_var(Any *p) : _ptr(p), _out(NULL) {}

    /*!
     * @brief (TODO)
     */
    Any_var & operator=(Any *p) {
      _ptr = p;
      return *this;
    }
    /*!
     * @brief (TODO)
     */
    ~Any_var() {
      if (_ptr) { delete _ptr; _ptr = NULL;}
      if (_out) { delete _out; _out = NULL; }
    }
  public:
    /*!
     * @brief (TODO)
     */
    operator Any &() { return *_ptr; }
    /*!
     * @brief (TODO)
     */
    operator Any *() { return _ptr; }

    /*!
     * @brief (TODO)
     */    
    Any * _retn() {
      Any *tmp = _ptr;
      _ptr = 0;
      return tmp;
    }

    /*!
     * @brief (TODO)
     */
    Any & inout() { return *_ptr; }
    /*!
     * @brief (TODO)
     */
    Any & in() { return *_ptr; }
    /*!
     * @brief (TODO)
     */
    Any_out & out() {
      if (!_out) {
	_out = new Any_out(&_ptr);
      }
      return *_out;
    }
    
    /*!
     * @brief (TODO)
     */
    void operator<<=(ULong n);
    /*!
     * @brief (TODO)
     */
    void operator<<=(const char *);
    /*!
     * @brief (TODO)
     */
    void operator<<=(char *);

  private:
    Any * _ptr;     /*!< TODO */
    Any_out *_out;  /*!< TODO */
  };

  /*!
   * @class Any_Arg
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */ 
  class Any_Arg {
  private:
    CORBA_any * _ptr;  /*!< TODO */
  public:
    /*!
     * @brief (TODO)
     */
    Any_Arg(CORBA_any * p) : _ptr(p) {}

    /*!
     * @brief (TODO)
     */
    Any_Arg(const CORBA_any &obj) : _ptr(NULL)
    {
      _ptr = const_cast<CORBA_any*>(&obj);
    }
    
    /*!
     * @brief (TODO)
     */
    operator CORBA_any *() { return _ptr; }
    /*!
     * @brief (TODO)
     */
    operator CORBA_any & () { return *_ptr; }
  public:
  };

  typedef Any_Arg Any_CInArg;
  typedef Any_out Any_COutArg;
  typedef Any_Arg Any_CInOutArg;
  typedef Any_Arg Any_CppInArg;
  typedef Any_out Any_CppOutArg;
  typedef Any_Arg Any_CppInOutArg;

  /*!
   * @class Any_helper
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */ 
  class Any_helper {
  public:
    /*!
     * @brief (TODO)
     */
    static Any * _retnew(Any *p)
    {
      return p;
    }
  };
};

#endif /* __CORBA_ANY_HH__ */
