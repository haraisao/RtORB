/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/* $Id: corba-var.hh,v 1.5 2008/01/30 12:22:25 yoshi Exp $ */ 
/*!
 * @file corba-var.hh
 * @if jp
 * @brief (TODO)
 * @else
 * @brief (TODO)
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */
#ifndef CORBA_var_hh
#define CORBA_var_hh

#include <RtORB/corba.h>

/*!
 * @class CORBA_var
 * @if jp
 * @brief (TODO)
 * @else
 * @brief (TODO)
 * @endif
 */ 
template <class T>
class CORBA_var {
private:
	typedef T * Ptr;
	typedef CORBA_var<T> V;
public:
        /*!
         * @brief (TODO)
         */
	CORBA_var() : _ptr(0) {;}
        /*!
         * @brief (TODO)
         */
	CORBA_var(const V &o) : _ptr(o._ptr) {;}
        /*!
         * @brief (TODO)
         */
	CORBA_var(const T &t) : _ptr(&t) {;}
        /*!
         * @brief (TODO)
         */
	CORBA_var(const T * ptr) : _ptr(const_cast<Ptr>(ptr)) {;}
        /*!
         * @brief (TODO)
         */
        ~CORBA_var() {
#if 0
	  if (_ptr) {
	    delete _ptr; _ptr = NULL;
	  }
#endif
	}
public:
        /*!
         * @brief (TODO)
         */
	Ptr _retn() { return _ptr; }
        /*!
         * @brief (TODO)
         */
	operator Ptr () { return _ptr; }
        /*!
         * @brief (TODO)
         */
	operator const Ptr () const { return _ptr; }
        /*!
         * @brief (TODO)
         */
	operator T & () { return *_ptr; }

	//	template <class S> operator * () { return _ptr; }

        /*!
         * @brief (TODO)
         */
	V &operator=(const V &o) { _ptr = o._ptr; return *this; }
	//V &operator=(const Ptr &o) { _ptr = o; }
	
        /*!
         * @brief (TODO)
         */
	V &operator=(const T &o) { _ptr = const_cast<Ptr>(&o); return *this; }
        /*!
         * @brief (TODO)
         */
	Ptr in () { return _ptr; }
private:
	Ptr _ptr;    /*!< TODO */
};

/*!
 * @class CORBA_ptrvar
 * @if jp
 * @brief (TODO)
 * @else
 * @brief (TODO)
 * @endif
 */ 
template <class T, class ImplPtr, class Ptr>
class CORBA_ptrvar {
private:
	typedef CORBA_ptrvar<T, ImplPtr, Ptr> V;
public:
        /*!
         * @brief (TODO)
         */
	CORBA_ptrvar() {;}
        /*!
         * @brief (TODO)
         */
	CORBA_ptrvar(const Ptr &ptr) : _ptr(ptr) {;}

        /*!
         * @brief (TODO)
         */
	operator T     () { return *_ptr; }
        /*!
         * @brief (TODO)
         */
	operator Ptr & () { return _ptr; }
        /*!
         * @brief (TODO)
         */
	operator ImplPtr () { return (ImplPtr)_ptr.impl(); }
        /*!
         * @brief (TODO)
         */
        bool operator!() const { return !_ptr; }
private:
	Ptr _ptr;   /*!< TODO */
};

/*!
 * @class CORBA_ptr
 * @if jp
 * @brief (TODO)
 * @else
 * @brief (TODO)
 * @endif
 */ 
template <class T>
class CORBA_ptr
{
public:
        /*!
         * @brief (TODO)
         */
	CORBA_ptr() : _ptr(0) {;}
        /*!
         * @brief (TODO)
         */
	CORBA_ptr(T *p) : _ptr(p) {;}
        /*!
         * @brief (TODO)
         */
	CORBA_ptr(const T *p) : _ptr(const_cast<T*>(p)) {;}

        /*!
         * @brief (TODO)
         */
	operator T *() { return _ptr; }
private:
	T * _ptr;    /*!< TODO */
};

/*!
 * @class CORBA_ptrptr
 * @if jp
 * @brief (TODO)
 * @else
 * @brief (TODO)
 * @endif
 */ 
template <class ObjPtr>
class CORBA_ptrptr
{
private:
	typedef CORBA_ptrptr<ObjPtr> Ptr;
public:
        /*!
         * @brief (TODO)
         */
	CORBA_ptrptr() : _objptr(0) {;}
        /*!
         * @brief (TODO)
         */
	CORBA_ptrptr(ObjPtr objptr) : _objptr(objptr) {;}
	//CORBA_ptrptr(const ObjPtr objptr) : _objptr(const_cast<ObjPtr>(objptr)) {;}

        /*!
         * @brief (TODO)
         */
	Ptr & operator=(const Ptr &p) {
		_objptr = p._objptr;
		return *this;
	}
        /*!
         * @brief (TODO)
         */
	operator ObjPtr () { return _objptr; }
        /*!
         * @brief (TODO)
         */
	operator ObjPtr * () { return _objptr; }
private:
	ObjPtr _objptr;   /*!< TODO */
};

	  
#endif // CORBA_var_hh
 

