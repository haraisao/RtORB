/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*!
 * @file CosNaming-stubs.cc
 * @if jp
 * @brief STUB file for C++ from CosNaming.idl
 * @else
 * @brief STUB file for C++ from CosNaming.idl
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#include "CosNaming.h"

CosNaming::NamingContext_ptr CosNaming::NamingContext::
_narrow(CORBA::Object_ptr obj)
{
   return CORBA_Narrow < CosNaming::NamingContext >::narrow(obj);
}

CORBA::Boolean CosNaming::NamingContext::_is_a(const CORBA_char * id)
{
   CORBA_boolean
      tmp =
      CosNaming_NamingContext__is_a(this->_impl, CORBA_string_CppInArg(id),
				    &this->ev);
   return CORBA::Boolean_helper::_retnew(tmp);
}

void
CosNaming::NamingContext::bind(const Name & n, CORBA::Object_ptr obj) 
{
   CosNaming_NamingContext_bind(this->_impl, CosNaming::Name_CppInArg(n),
				CORBA::Object_CppInArg(obj), &this->ev);
   if (this->ev._major == CORBA_USER_EXCEPTION) {
	   CORBA_TypeCode tc = ev._params._type;
	   if (CORBA_TypeCode_equal(tc, 
				    TC_CosNaming_NamingContext_AlreadyBound, NULL)) {
		   throw AlreadyBound();
	   } else if (CORBA_TypeCode_equal(tc,
					   TC_CosNaming_NamingContext_InvalidName, NULL)) {
		   throw InvalidName();
	   } else if (CORBA_TypeCode_equal(tc,
					   TC_CosNaming_NamingContext_CannotProceed, NULL)) {
		   throw CannotProceed();
	   } else if (CORBA_TypeCode_equal(tc,
					   TC_CosNaming_NamingContext_NotFound, NULL)) {
		   throw NotFound();
	   } else {
		   throw CORBA::UserException();
	   }
   } 
   return;
}

void
CosNaming::NamingContext::rebind(const Name & n, CORBA::Object_ptr obj)
{
   CosNaming_NamingContext_rebind(this->_impl, CosNaming::Name_CppInArg(n),
				  CORBA::Object_CppInArg(obj), &this->ev);

   if (this->ev._major == CORBA_USER_EXCEPTION) {
	   CORBA_TypeCode tc = ev._params._type;

	   if (CORBA_TypeCode_equal(tc,
				    TC_CosNaming_NamingContext_InvalidName, NULL)) {
		   throw InvalidName();
	   } else if (CORBA_TypeCode_equal(tc,
					   TC_CosNaming_NamingContext_CannotProceed, NULL)) {
		   throw CannotProceed();
	   } else if (CORBA_TypeCode_equal(tc,
					   TC_CosNaming_NamingContext_NotFound, NULL)) {
		   throw NotFound();
	   } else {
		   throw CORBA::UserException();
	   }
   } 

   return;
}

void
CosNaming::NamingContext::bind_context(const Name & n, NamingContext_ptr nc)
{
   CosNaming_NamingContext_bind_context(this->_impl,
					CosNaming::Name_CppInArg(n),
					CosNaming::NamingContext_CppInArg(nc),
					&this->ev);
   if (this->ev._major == CORBA_USER_EXCEPTION) {
	   CORBA_TypeCode tc = ev._params._type;
	   if (CORBA_TypeCode_equal(tc, 
				    TC_CosNaming_NamingContext_AlreadyBound, NULL)) {
		   throw AlreadyBound();
	   } else if (CORBA_TypeCode_equal(tc,
					   TC_CosNaming_NamingContext_InvalidName, NULL)) {
		   throw InvalidName();
	   } else if (CORBA_TypeCode_equal(tc,
					   TC_CosNaming_NamingContext_CannotProceed, NULL)) {
		   throw CannotProceed();
	   } else if (CORBA_TypeCode_equal(tc,
					   TC_CosNaming_NamingContext_NotFound, NULL)) {
		   throw NotFound();
	   } else {
		   throw CORBA::UserException();
	   }
   } 
   return;
}

void
CosNaming::NamingContext::rebind_context(const Name & n, NamingContext_ptr nc)
{
   CosNaming_NamingContext_rebind_context(this->_impl,
					  CosNaming::Name_CppInArg(n),
					  CosNaming::
					  NamingContext_CppInArg(nc),
					  &this->ev);
   if (this->ev._major == CORBA_USER_EXCEPTION) {
	   CORBA_TypeCode tc = ev._params._type;

	   if (CORBA_TypeCode_equal(tc,
				    TC_CosNaming_NamingContext_InvalidName, NULL)) {
		   throw InvalidName();
	   } else if (CORBA_TypeCode_equal(tc,
					   TC_CosNaming_NamingContext_CannotProceed, NULL)) {
		   throw CannotProceed();
	   } else if (CORBA_TypeCode_equal(tc,
					   TC_CosNaming_NamingContext_NotFound, NULL)) {
		   throw NotFound();
	   } else {
		   throw CORBA::UserException();
	   }
   } 
   return;
}

CORBA::Object_ptr CosNaming::NamingContext::resolve(const Name & n)
{
   CORBA_Object
      tmp =
      CosNaming_NamingContext_resolve(this->_impl,
				      CosNaming::Name_CppInArg(n), &this->ev);
   if (this->ev._major == CORBA_USER_EXCEPTION) {
	   CORBA_TypeCode tc = ev._params._type;

	   if (CORBA_TypeCode_equal(tc,
				    TC_CosNaming_NamingContext_InvalidName, NULL)) {
		   throw InvalidName();
	   } else if (CORBA_TypeCode_equal(tc,
					   TC_CosNaming_NamingContext_CannotProceed, NULL)) {
		   throw CannotProceed();
	   } else if (CORBA_TypeCode_equal(tc,
					   TC_CosNaming_NamingContext_NotFound, NULL)) {
		   throw NotFound();
	   } else {
		   throw CORBA::UserException();
	   }
   } 
   
   return CORBA::Object_helper::_retnew(tmp);
}

void
CosNaming::NamingContext::unbind(const Name & n)
{
   CosNaming_NamingContext_unbind(this->_impl, CosNaming::Name_CppInArg(n),
				  &this->ev);
   if (this->ev._major == CORBA_USER_EXCEPTION) {
	   CORBA_TypeCode tc = ev._params._type;

	   if (CORBA_TypeCode_equal(tc,
				    TC_CosNaming_NamingContext_InvalidName, NULL)) {
		   throw InvalidName();
	   } else if (CORBA_TypeCode_equal(tc,
					   TC_CosNaming_NamingContext_CannotProceed, NULL)) {
		   throw CannotProceed();
	   } else if (CORBA_TypeCode_equal(tc,
					   TC_CosNaming_NamingContext_NotFound, NULL)) {
		   throw NotFound();
	   } else {
		   throw CORBA::UserException();
	   }
   } 
   
   return;
}

CosNaming::NamingContext_ptr CosNaming::NamingContext::new_context()
{
   CosNaming_NamingContext
      tmp = CosNaming_NamingContext_new_context(this->_impl, &this->ev);

   return CosNaming::NamingContext_helper::_retnew(tmp);
}

CosNaming::NamingContext_ptr
   CosNaming::NamingContext::bind_new_context(const Name & n)
{
   CosNaming_NamingContext
      tmp =
      CosNaming_NamingContext_bind_new_context(this->_impl,
					       CosNaming::Name_CppInArg(n),
					       &this->ev);
   if (this->ev._major == CORBA_USER_EXCEPTION) {
	   CORBA_TypeCode tc = ev._params._type;

	   if (CORBA_TypeCode_equal(tc,
				    TC_CosNaming_NamingContext_InvalidName, NULL)) {
		   throw InvalidName();
	   } else if (CORBA_TypeCode_equal(tc,
					   TC_CosNaming_NamingContext_CannotProceed, NULL)) {
		   throw CannotProceed();
	   } else if (CORBA_TypeCode_equal(tc,
					   TC_CosNaming_NamingContext_NotFound, NULL)) {
		   throw NotFound();
	   } else if (CORBA_TypeCode_equal(tc,
					   TC_CosNaming_NamingContext_AlreadyBound, NULL)) {
		   throw AlreadyBound();
	   } else {
		   throw CORBA::UserException();
	   }
   } 

   return CosNaming::NamingContext_helper::_retnew(tmp);
}

void
CosNaming::NamingContext::destroy()
{
   CosNaming_NamingContext_destroy(this->_impl, &this->ev);
   if (this->ev._major == CORBA_USER_EXCEPTION) {
	   CORBA_TypeCode tc = ev._params._type;

	   if (CORBA_TypeCode_equal(tc,
				    TC_CosNaming_NamingContext_NotEmpty, NULL)) {
		   throw NotEmpty();
	   } else {
		   throw CORBA::UserException();
	   }
   }
			   
   return;
}

void
CosNaming::NamingContext::list(CORBA::ULong how_many, BindingList_out bl,
			       BindingIterator_ptr bi)
{
   CosNaming_NamingContext_list(this->_impl, CORBA::ULong_CppInArg(how_many),
				CosNaming::BindingList_CppOutArg(bl),
				CosNaming::BindingIterator_CppOutArg(bi),
				&this->ev);
   return;
}

CosNaming::BindingIterator_ptr CosNaming::
   BindingIterator::_narrow(CORBA::Object_ptr obj)
{
   return CORBA_Narrow < CosNaming::BindingIterator >::narrow(obj);
}

CORBA::Boolean CosNaming::BindingIterator::next_one(Binding_out b)
{
   CORBA_boolean
      tmp =
      CosNaming_BindingIterator_next_one(this->_impl,
					 CosNaming::Binding_CppOutArg(b),
					 &this->ev);
   return CORBA::Boolean_helper::_retnew(tmp);
}

CORBA::Boolean
   CosNaming::BindingIterator::next_n(CORBA::ULong how_many,
				      BindingList_out bl)
{
   CORBA_boolean
      tmp =
      CosNaming_BindingIterator_next_n(this->_impl,
				       CORBA::ULong_CppInArg(how_many),
				       CosNaming::BindingList_CppOutArg(bl),
				       &this->ev);
   return CORBA::Boolean_helper::_retnew(tmp);
}

void
CosNaming::BindingIterator::destroy()
{
   CosNaming_BindingIterator_destroy(this->_impl, &this->ev);
   return;
}

CosNaming::NamingContextExt_ptr CosNaming::
   NamingContextExt::_narrow(CORBA::Object_ptr obj)
{
   return CORBA_Narrow < CosNaming::NamingContextExt >::narrow(obj);
}

CosNaming::NamingContextExt::StringName
   CosNaming::NamingContextExt::to_string(const Name & n)
{
   CosNaming_NamingContextExt_StringName
      tmp =
      CosNaming_NamingContextExt_to_string(this->_impl,
					   CosNaming::Name_CppInArg(n),
					   &this->ev);
   if (this->ev._major == CORBA_USER_EXCEPTION) {
	   CORBA_TypeCode tc = ev._params._type;
	   if (CORBA_TypeCode_equal(tc, 
				    TC_CosNaming_NamingContext_InvalidName, NULL)) {
		   throw CosNaming::NamingContext::InvalidName();
	   } else {
		   throw CORBA::UserException();
	   }
   }

   return CosNaming::NamingContextExt::StringName_helper::_retnew(tmp);
}

CosNaming::Name * CosNaming::NamingContextExt::to_name(const CORBA_char * sn)
{
   CosNaming_Name *
      tmp =
      CosNaming_NamingContextExt_to_name(this->_impl,
					 CosNaming::NamingContextExt::
					 StringName_CppInArg(sn), &this->ev);
   if (this->ev._major == CORBA_USER_EXCEPTION) {
	   CORBA_TypeCode tc = ev._params._type;
	   if (CORBA_TypeCode_equal(tc, 
				    TC_CosNaming_NamingContext_InvalidName, NULL)) {
		   throw CosNaming::NamingContext::InvalidName();
	   } else {
		   throw CORBA::UserException();
	   }
   }
   
   return CosNaming::Name_helper::_retnew(tmp);
}

CosNaming::NamingContextExt::URLString
   CosNaming::NamingContextExt::to_url(const CORBA_char * addr,
				       const CORBA_char * sn)
{
   CosNaming_NamingContextExt_URLString
      tmp =
      CosNaming_NamingContextExt_to_url(this->_impl,
					CosNaming::NamingContextExt::
					Address_CppInArg(addr),
					CosNaming::NamingContextExt::
					StringName_CppInArg(sn), &this->ev);
   if (this->ev._major == CORBA_USER_EXCEPTION) {
	   CORBA_TypeCode tc = ev._params._type;
	   if (CORBA_TypeCode_equal(tc, 
				    TC_CosNaming_NamingContextExt_InvalidAddress, NULL)) {
		   throw InvalidAddress();
	   } else if (CORBA_TypeCode_equal(tc,
					   TC_CosNaming_NamingContext_InvalidName, NULL)) {
		   throw CosNaming::NamingContext::InvalidName();
	   } else {
		   throw CORBA::UserException();
	   }
   }
   
   return CosNaming::NamingContextExt::URLString_helper::_retnew(tmp);
}

CORBA::Object_ptr
   CosNaming::NamingContextExt::resolve_str(const CORBA_char * n)
{
   CORBA_Object
      tmp =
      CosNaming_NamingContextExt_resolve_str(this->_impl,
					     CosNaming::NamingContextExt::
					     StringName_CppInArg(n),
					     &this->ev);
   if (this->ev._major == CORBA_USER_EXCEPTION) {
	   CORBA_TypeCode tc = ev._params._type;
	   if (CORBA_TypeCode_equal(tc, 
				    TC_CosNaming_NamingContext_AlreadyBound, NULL)) {
		   throw CosNaming::NamingContext::AlreadyBound();
	   } else if (CORBA_TypeCode_equal(tc,
					   TC_CosNaming_NamingContext_InvalidName, NULL)) {
		   throw CosNaming::NamingContext::InvalidName();
	   } else if (CORBA_TypeCode_equal(tc,
					   TC_CosNaming_NamingContext_CannotProceed, NULL)) {
		   throw CosNaming::NamingContext::CannotProceed();
	   } else if (CORBA_TypeCode_equal(tc,
					   TC_CosNaming_NamingContext_NotFound, NULL)) {
		   throw CosNaming::NamingContext::NotFound();
	   } else {
		   throw CORBA::UserException();
	   }
   } 

   return CORBA::Object_helper::_retnew(tmp);
}

CORBA_sequence_CosNaming_NameComponent::CppSeq::
   ElemRef CORBA_sequence_CosNaming_NameComponent::operator[](CORBA::ULong n)
     const
     {
	return
	CORBA_sequence_CosNaming_NameComponent::CppSeq::element(this,
								(int) n);
     }

CORBA::ULong CORBA_sequence_CosNaming_NameComponent::
length(CORBA::ULong n)
{
   return CORBA_sequence_CosNaming_NameComponent::CppSeq::length(this, n);
}

CORBA_sequence_CosNaming_NameComponent_type &
   CORBA_sequence_CosNaming_NameComponent::operator=(const CppSeq & o)
{
   o.copyTo(*this);
   return *this;
}

CORBA_sequence_CosNaming_NameComponent_type &
   CORBA_sequence_CosNaming_NameComponent::
operator=(const CORBA_sequence_CosNaming_NameComponent_type & o)
{
   CppSeq::copy(o, *this);
   return *this;
}

CORBA_sequence_CosNaming_Binding::CppSeq::
   ElemRef CORBA_sequence_CosNaming_Binding::operator[](CORBA::ULong n) {
   return CORBA_sequence_CosNaming_Binding::CppSeq::element(this, (int) n);
}

CORBA_sequence_CosNaming_Binding::CppSeq::
   ElemRef CORBA_sequence_CosNaming_Binding::operator[](CORBA::ULong n)
     const
     {
	return
	CORBA_sequence_CosNaming_Binding::CppSeq::element(this, (int) n);
     }

CORBA::ULong CORBA_sequence_CosNaming_Binding::
length(CORBA::ULong n)
{
   return CORBA_sequence_CosNaming_Binding::CppSeq::length(this, n);
}

CORBA_sequence_CosNaming_Binding_type &
   CORBA_sequence_CosNaming_Binding::operator=(const CppSeq & o)
{
   o.copyTo(*this);
   return *this;
}
