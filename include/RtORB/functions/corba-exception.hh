/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*!
 * @file corba-exception.hh
 * @if jp
 * @brief corba exception actions for C++.
 * @else
 * @brief corba exception actions for C++.
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __FUNC_CORBA_EXCEPTION_HH__
#define __FUNC_CORBA_EXCEPTION_HH__

#ifdef __cplusplus
/*!
 * @if jp
 * @brief デフォルトCORBA例外処理 (例外の取得と対応するメッセージ出力) [NO_EXCEPTION / USER_EXCEPTION / SYSTEM_EXCEPTION]
 * @else
 * @brief default CORBA exception actions. Getting exception and throw messages. [NO_EXCEPTION / USER_EXCEPTION / SYSTEM_EXCEPTION]
 * @endif
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void catchAndThrowDefaultException(CORBA_Environment *env);
#endif

#endif  /* __FUNC_CORBA_EXCEPTION_HH__ */

