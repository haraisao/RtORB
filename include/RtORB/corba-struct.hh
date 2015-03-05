/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/* $Id: corba-struct.hh,v 1.6 2008/04/11 07:26:40 yoshi Exp $ */ 
/*!
 * @file corba-struct.hh
 * @if jp
 * @brief (TODO)
 * @else
 * @brief (TODO)
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __CORBA_STRUCT_HH__
#define __CORBA_STRUCT_HH__

namespace CORBA_struct {

  template <class T> class T_Var;

  /*!
   * @class T_Out
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */   
  template <class T>
  class T_Out
  {
    typedef T_Out<T> Out;
    typedef T_Var<T> Var;
  private:
    T * _ref;    /*!< TODO */
  public:
    /*!
     * @brief (TODO)
     */
    T_Out(T * ref) : _ref(ref) {}
    /*!
     * @brief (TODO)
     */
    T_Out(const T_Out &o) : _ref(o._ref) {}

    /*!
     * @brief (TODO)
     */
    T * ptr() { return _ref; }

    /*!
     * @brief (TODO)
     */
    operator T & () {
      /* supposed to be NOT NULL */
      return *_ref;
    }

    /*!
     * @brief (TODO)
     */
    Out & operator=(T *t)
    {
      _ref = t;
      return *this;
    }
  };

  /*!
   * @class T_Arg
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  template <class T>
  class T_Arg
  {
  public:
    /*!
     * @brief (TODO)
     */
    T_Arg(T *p) : _ptr(p) {}
    /*!
     * @brief (TODO)
     */
    T_Arg(const T &t) : _ptr( const_cast<T*>(&t) ) {}

    /*!
     * @brief (TODO)
     */
    operator T * () { return _ptr; }
    /*!
     * @brief (TODO)
     */
    operator T ** () { return &_ptr; }
    /*!
     * @brief (TODO)
     */
    operator T & () { return *_ptr; }
  private:
    T * _ptr;   /*!< TODO */
  };

  /*!
   * @class T_COutArg
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  template <class T>
  class T_COutArg
  {
    typedef T_Out<T> Out;
  private:
    Out *_out;    /*!< TODO */
  public:
    /*!
     * @brief (TODO)
     */
    T_COutArg(T * t) : _out(NULL) {
      _out = new Out(t);
    }

    /*!
     * @brief (TODO)
     */    
    T_COutArg(T** pp) : _out(NULL) {
      T *p = *pp;
      if (!p) {
	p = new T();
	*pp = p;
      }
      _out = new Out(p);
    }
    /*!
     * @brief (TODO)
     */
    ~T_COutArg() {
      if (_out) {
	delete _out; _out = NULL;
      }
    }

    /*!
     * @brief (TODO)
     */
    operator T & () { return *(_out->ptr()); }
    /*!
     * @brief (TODO)
     */
    operator Out & () { return *_out; }
  };

  /*!
   * @class T_Helper
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  template <class T>
  class T_Helper
  {
  public:
    /*!
     * @brief (TODO)
     */
    static T * _retnew(T &t) {
/*
      T *p = new T();
      *p = t;
      return p;
*/
      return &t;
    }
    /*!
     * @brief (TODO)
     */
    static T * _retnew(T *t) {
/*
      T *p = new T();
      p = t;
      return p;
*/
      return t;
    }
  };

  /*!
   * @class T_Ptr
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  template <class T>
  class T_Ptr
  {
  public:
    /*!
     * @brief (TODO)
     */
    T_Ptr(T * p) : _ptr(p) {;}
    /*!
     * @brief (TODO)
     */
    T_Ptr(const T & o) : _ptr(const_cast<T*>(&o)) {;}

    /*!
     * @brief (TODO)
     */
    operator T *() { return _ptr; }
    /*!
     * @brief (TODO)
     */
    operator T &() { return _ptr; }

    /*!
     * @brief (TODO)
     */
    T * operator->() { return _ptr; }
  private:
    T * _ptr;    /*!< TODO */
  };

  /*!
   * @class T_Var
   * @if jp
   * @brief (TODO)
   * @else
   * @brief (TODO)
   * @endif
   */
  template <class T>
  class T_Var
  {
    typedef T_Out<T> Out;
    typedef T_Var<T> Var;
    typedef T_Ptr<T> Ptr;
  public:
    /*!
     * @brief (TODO)
     */
    T_Var() : _ptr(NULL), _out(NULL) {;}
    /*!
     * @brief (TODO)
     */
    T_Var(T *t) : _ptr(t), _out(NULL) {;}
    /*!
     * @brief (TODO)
     */
    ~T_Var() {
      free_();
    }

    /*!
     * @brief (TODO)
     */
    operator T& () { return *_ptr; }
    /*!
     * @brief (TODO)
     */
    operator Ptr () { return Ptr(_ptr); }

    /*!
     * @brief (TODO)
     */
    Var &operator=(T *t) {
#if DEBUG_MALLOC
      std::cerr << "   == Call Operator= ==" << std::endl;
#endif
      free_();
      _ptr = t;
      return *this;
    }

    /*!
     * @brief (TODO)
     */
    T * operator->() { return _ptr; }

    /*!
     * @brief (TODO)
     */  
    T *_retn() {
      T *tmp = _ptr;
      _ptr = NULL;

      free_();
      
      return tmp;
    }

    /*!
     * @brief (TODO)
     */
    T &inout() { return *_ptr; }
    /*!
     * @brief (TODO)
     */
    T & in() { return *_ptr; }
    /*!
     * @brief (TODO)
     */
    Out & out() {
      if (!_out) {
	_out = new Out(_ptr);
      }
      return *_out;
    }
  private:
    /*!
     * @brief (TODO)
     */
    void free_() {
      if (_out) {
#if DEBUG
	fprintf(stderr, "DELETE strcut_var _out (0x%x)\n", _out);
#endif
	delete _out; _out = NULL;
      }
      if (_ptr) {
#if DEBUG
	fprintf(stderr, "DELETE strcut_var _ptr (0x%x)\n", _ptr);
#endif
	delete _ptr; _ptr = NULL;
      }
    }
  private:
    T *_ptr;       /*!< TODO */
    Out *_out;     /*!< TODO */
  };


}

#endif // __CORBA_STRUCT_HH__
 
