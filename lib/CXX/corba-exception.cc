/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*!
 * @file corba-exception.cc
 * @if jp
 * @brief CORBA exception actions for C++.
 * @else
 * @brief CORBA exception actions for C++.
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#include <RtORB/corba.hh>

void catchAndThrowDefaultException(CORBA_Environment *ev)
{
  switch(ev->_major) {
  case CORBA_NO_EXCEPTION:
    break;
  case CORBA_USER_EXCEPTION:
    throw CORBA::UserException(ev);
  case CORBA_SYSTEM_EXCEPTION:
    throw CORBA::SystemException(ev);
  default:
    fprintf(stderr, "!!!! Catch an unkown Error, errno = %d\n", (int)ev->_major);
    exit(1);
  }
}

namespace CORBA {

  Exception::Exception()
  {
    _msg[0] = 0;
  }
  Exception::Exception(const char *type)
  {
    sprintf(_msg, "%s : ", type);
  }
  
  Exception::Exception(const char *type, const char *msg)
  {
    sprintf(_msg, "%s : %s", type, msg);
  }

  Exception::Exception(const char *type, CORBA_Environment *ev)
  {
    sprintf(_msg, "%s : %s", type, ev->_repo_id? (char *)ev->_repo_id: "");
  }

  const char *Exception::msg()
  {
    return _msg[0]? _msg: NULL;
  }

  UserException::UserException() : Exception("UserException")
  {
    
  }
	
  UserException::UserException(CORBA_Environment *ev) : Exception("UserException", ev)
  {
    
  }

  SystemException::SystemException(CORBA_Environment *ev)
    : Exception("SystemException", ev) 
  {

  }
  SystemException::SystemException(const char *msg)
    : Exception("SystemException", msg)
  {
    
  }
}
