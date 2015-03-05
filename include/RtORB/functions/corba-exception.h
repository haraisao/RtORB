/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 *   RtORB: corba-exception.h
 *
 *   Copyright(C) 2006, Isao Hara, AIST
 *
 *   $Revision: 1.2 $
 *   $Date: 2008/05/19 06:09:41 $
 *   $Id: corba-exception.h,v 1.2 2008/05/19 06:09:41 yoshi Exp $
 *
 */
/*!
 * @file corba-exception.h
 * @if jp
 * @brief CORBAの例外情報を扱う機能群。
 * @else
 * @brief corba exception actions.
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __FUNC_CORBA_EXCEPTION_H__
#define __FUNC_CORBA_EXCEPTION_H__

/*!
 * @if jp
 * @brief CORBA例外情報をセットする。
 * @else
 * @brief set CORBA exception's information.
 * @endif
 * @param ev CORBA env(Exception Infomation)
 * @param major Exception type (ex: CORBA_NO_EXCEPTION, CORBA_USER_EXCEPTION, CORBA_SYSTEM_EXCEPTIO ...) 
 * @param repo_id Exception Report ID
 * @param params Exception parameter 
 * @return void
 */
void
CORBA_exception_set(CORBA_Environment *ev, uint32_t major,
		char *repo_id, CORBA_any *params);

/*!
 * @if jp
 * @brief CORBA例外情報をセットする。(USER_EXCEPTION)
 * @else
 * @brief set CORBA exception's information. (USER_EXCEPTION)
 * @endif
 * @param ev CORBA env(Exception Infomation)
 * @param repo_id Exception Report ID
 * @param value Exception data
 * @param tc CORBA Type Code
 * @return void
 */
void 
CORBA_user_exception_set(CORBA_Environment *ev, char *repo_id, CORBA_TypeCode tc, void *value);

/*!
 * @if jp
 * @brief CORBA例外情報をセットする。(SYSTEM_EXCEPTION)
 * @else
 * @brief set CORBA exception's information. (SYSTEM_EXCEPTION)
 * @endif
 * @param ev CORBA env(Exception Infomation)
 * @param msg message(Exception Report ID)
 * @return void
 */
void CORBA_system_exception(CORBA_Environment *ev, char *msg);

/*!
 * @if jp
 * @brief デフォルト例外情報をキャッチし、内部で判別し例外タイプを返す。
 * @else
 * @brief catch default Exception info, anf return Exception type.
 * @endif
 * @param env CORBA env(Exception Infomation)
 * @return Exception type (ex: CORBA_NO_EXCEPTION, CORBA_USER_EXCEPTION, CORBA_SYSTEM_EXCEPTIO ...) 
 */
int catchDefaultException(CORBA_Environment *env);

/*!
 * @if jp
 * @brief 例外情報の有無を判別する。
 * @else
 * @brief Check wheter Exception info are or not.
 * @endif
 * @param env CORBA env(Exception Infomation)
 * @param repo_id Exception Report ID
 * @return 1:Exception, 0:NoException, or Exception_Type
 */
int catchException(CORBA_Environment *env, char *repo_id);

/*!
 * @if jp
 * @brief 例外情報をクリアする。
 * @else
 * @brief clear Exception info.
 * @endif
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void clearException(CORBA_Environment *env);

/*!
 * @if jp
 * @brief 例外情報の中身のデータを取得する。
 * @else
 * @brief get contents from Exception info data.
 * @endif
 * @param ev CORBA env(Exception Infomation)
 * @return Exception data
 */
void * CORBA_exception_get_value(CORBA_Environment *ev);

#endif
