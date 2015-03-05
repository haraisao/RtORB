/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 *   RtORB: corba-exception.c
 *
 *   Copyright(C) 2006, Isao Hara, AIST
 *
 *   $Revision: 1.4 $
 *   $Date: 2008/05/19 06:33:28 $
 *   $Id: corba-exception.c,v 1.4 2008/05/19 06:33:28 yoshi Exp $
 *
 */
/*!
 * @file corba-exception.c
 * @if jp
 * @brief CORBAの例外情報を扱う機能群。
 * @else
 * @brief corba exception actions.
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#include <RtORB/corba.h>

void
CORBA_exception_set(CORBA_Environment *ev, uint32_t major, char *repo_id,
		CORBA_any *params){
  if(major != CORBA_NO_EXCEPTION){
    ev->_major = major;
    ev->_repo_id = (unsigned char *)repo_id;
    if(params){
      CORBA_any_set_exception(&ev->_params, params);
    }
  }
}

void
CORBA_user_exception_set(CORBA_Environment *ev, char *repo_id, CORBA_TypeCode tc, void *value){

  ev->_major = CORBA_USER_EXCEPTION;
  ev->_repo_id = (unsigned char *)repo_id;

  if(tc && value){
    CORBA_any_set_exception_value(&ev->_params, tc, value, TRUE);
  }
}

void * CORBA_exception_get_value(CORBA_Environment *ev)
{
  if (!ev) { return NULL; }
  if (!ev->_params._type) { return NULL; }
  if (ev->_params._type->kind != tk_except) { return NULL; }
  if (!ev->_params._val) { return NULL; }
  return ev->_params._val->val_except;
}


void CORBA_system_exception(CORBA_Environment *ev, char *msg){
  CORBA_exception_set(ev, CORBA_SYSTEM_EXCEPTION, msg, 0);
}

int catchDefaultException(CORBA_Environment *env){
  switch(env->_major){
    case CORBA_NO_EXCEPTION:
      break;
    case CORBA_USER_EXCEPTION:
      fprintf(stderr, "Catch user_exception: %s\n", env->_repo_id);
      break;
    case CORBA_SYSTEM_EXCEPTION:
      fprintf(stderr, "!!!! System exception occured. !!!!\n");
      exit(0);
      break;
    default:
      fprintf(stderr, "!!!! Catch an unknown error, errno=%d\n",
		      (int)env->_major);
      exit(1);
  }
  return env->_major;
}

int catchException(CORBA_Environment *env, char *repo_id){
  if(env->_major == CORBA_NO_EXCEPTION) return 0;
  if(!repo_id){ return env->_major;}
  if (!strcmp(repo_id, "SystemException") &&
		  env->_major == CORBA_SYSTEM_EXCEPTION) return 1;
  if(env->_repo_id && !strcmp((const char *)env->_repo_id, (const char *)repo_id) &&
		  env->_major == CORBA_USER_EXCEPTION) return 1;
  else return 0;
}

void clearException(CORBA_Environment *env){
  /*
   if(env->_params._type){
     RtORB_free_by_typecode(env->_params._type, env->_params._vaue, 1); 
   }
  */
  CORBA_any_clear(&env->_params);
  memset(env, 0, sizeof(CORBA_Environment));
}


/*
CORBA_any *
CORBA_user_exception_alloc(CORBA_TypeCode tc){
 CORBA_any *param = (CORBA_any *)RtORB_alloc(sizeof(CORBA_any),
	                              "CORBA_user_exception_alloc");
  param->_type = tc;
  param->_value = RtORB_alloc_by_typecode(tc, 1, "CORBA_user_exception_alloc");
  param->_release = 0;
  return param;
}
*/
