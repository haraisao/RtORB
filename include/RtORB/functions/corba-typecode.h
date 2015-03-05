/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 *   RtORB: TypeCode.c
 *
 *   Copyright(C) 2006, Isao Hara, AIST
 *
 *   $Revision: 1.3 $
 *   $Date: 2008/04/16 11:40:38 $
 *   $Id: corba-typecode.h,v 1.3 2008/04/16 11:40:38 yoshi Exp $
 */
/*!
 * @file corba-typecode.h
 * @if jp
 * @brief CORBA_TypeCode関連データを扱う関数群
 * @else
 * @brief CORBA_Type_Code data actions.
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __FUNC_CORBA_TYPECODE_H__
#define __FUNC_CORBA_TYPECODE_H__

/*!
 * @if jp
 * @brief (T.B.D)
 * @else
 * @brief (T.B.D)
 * @endif
 * @param ev (T.B.D)
 * @param msg (T.B.D)
 * @return (T.B.D)
 */
void raises(CORBA_Environment *ev,char *msg );

/*!
 * @if jp
 * @brief TypeCodeが一致するかどうか比較をする。中身まで詳細な比較。
 * @else
 * @brief Compare wheter TypeCode datas are matched or not.
 * @endif
 * @param code TypeCode data
 * @param tc TypeCode reference for Comparing
 * @param ev CORBA env(Exception Infomation)
 * @return NG:FALSE, OK:TRUE
 */
boolean CORBA_TypeCode_equal(CORBA_TypeCode code, CORBA_TypeCode tc, CORBA_Environment *ev);

/*!
 * @if jp
 * @brief TypeCodeの'kind'が一致するかどうか比較する。
 * @else
 * @brief Compare wheter 'kind' of TypeCode data is matched or not.
 * @endif
 * @param code TypeCode data
 * @param tc TypeCode reference for Comparing
 * @param ev CORBA env(Exception Infomation)
 * @return NG:FALSE, OK:TRUE
 */
boolean CORBA_TypeCode_equivalent(CORBA_TypeCode code, CORBA_TypeCode tc, CORBA_Environment *ev);

/*!
 * @if jp
 * @brief TypeCodeのkind値を返す。
 * @else
 * @brief return 'kind' value of TypeCode data.
 * @endif
 * @param code TypeCode data
 * @param ev CORBA env(Exception Infomation)
 * @return kind value
 */
CORBA_TCKind CORBA_TypeCode_kind(CORBA_TypeCode code, CORBA_Environment *ev);

/*!
 * @if jp
 * @brief TypeCodeのリポジトリIDを返す。
 * @else
 * @brief return 'repoid' value of TypeCode data.
 * @endif
 * @param code TypeCode data
 * @param ev CORBA env(Exception Infomation)
 * @return repository ID
 */
CORBA_RepositoryId CORBA_TypeCode_id(CORBA_TypeCode code, CORBA_Environment *ev);

/*!
 * @if jp
 * @brief TypeCodeの名前(identifier)を返す。
 * @else
 * @brief return 'identifier' value of TypeCode data.
 * @endif
 * @param code TypeCode data
 * @param ev CORBA env(Exception Infomation)
 * @return identifier
 */
CORBA_Identifier CORBA_TypeCode_name(CORBA_TypeCode code, CORBA_Environment *ev);

/*!
 * @if jp
 * @brief 指定したkindの'member_count'を返す。
 * @else
 * @brief return counts of members of indicated 'kind'.
 * @endif
 * @param code TypeCode data
 * @param ev CORBA env(Exception Infomation)
 * @return member_count
 */
uint32_t CORBA_TypeCode_member_count(CORBA_TypeCode code, CORBA_Environment *ev);

/*!
 * @if jp
 * @brief 指定したkindの'member_name'を返す。
 * @else
 * @brief return name of indicated 'kind'.
 * @endif
 * @param code TypeCode data
 * @param index index
 * @param ev CORBA env(Exception Infomation)
 * @return member_name
 */
CORBA_Identifier CORBA_TypeCode_member_name(CORBA_TypeCode code,uint32_t index,  CORBA_Environment *ev);

/*!
 * @if jp
 * @brief 指定したkindのタイプを返す。
 * @else
 * @brief return member_type of indicated 'lind'.
 * @endif
 * @param code TypeCode data
 * @param index index
 * @param ev CORBA env(Exception Infomation)
 * @return member_type
 */
CORBA_TypeCode CORBA_TypeCode_member_type(CORBA_TypeCode code,uint32_t index,  CORBA_Environment *ev);

/*!
 * @if jp
 * @brief 指定したkindのラベルを返す。
 * @else
 * @brief return member_label of indicated 'kind'.
 * @endif
 * @param code TypeCode data
 * @param index index
 * @param ev CORBA env(Exception Infomation)
 * @return member_label
 */
void * * CORBA_TypeCode_member_label(CORBA_TypeCode code,uint32_t index,  CORBA_Environment *ev);

/*!
 * @if jp
 * @brief TypeCodeデータのdiscriminatorを返す。
 * @else
 * @brief return 'discriminator' in CORBA_Type_Code.
 * @endif
 * @param code TypeCode data
 * @param ev CORBA env(Exception Infomation)
 * @return discriminator
 */
CORBA_TypeCode CORBA_TypeCode_discriminator_type(CORBA_TypeCode code,  CORBA_Environment *ev);

/*!
 * @if jp
 * @brief TypeCodeデータのデフォルトインデックスを返す。
 * @else
 * @brief return default index in CORBA_Type_Code.
 * @endif
 * @param code TypeCode data
 * @param ev CORBA env(Exception Infomation)
 * @return default_index
 */
int32_t CORBA_TypeCode_default_index(CORBA_TypeCode code,  CORBA_Environment *ev);

/*!
 * @if jp
 * @brief TypeCodeデータの要素数を返す。
 * @else
 * @brief return count of elements in CORBA_Type_Code.
 * @endif
 * @param code TypeCode data
 * @param ev CORBA env(Exception Infomation)
 * @return length
 */
uint32_t CORBA_TypeCode_length(CORBA_TypeCode code,  CORBA_Environment *ev);

/*!
 * @if jp
 * @brief 指定したkindの'content_type'を返す。
 * @else
 * @brief return 'content_type' of indicated 'kind'.
 * @endif
 * @param code TypeCode data
 * @param ev CORBA env(Exception Infomation)
 * @return content_type
 */
CORBA_TypeCode CORBA_TypeCode_content_type(CORBA_TypeCode code,  CORBA_Environment *ev);

/*!
 * @if jp
 * @brief 指定したkindの'fixed_digits'を返す。
 * @else
 * @brief return 'fixed_digits' of indicated 'kind'.
 * @endif
 * @param code TypeCode data
 * @param ev CORBA env(Exception Infomation)
 * @return fixed_digits
 */
unsigned short CORBA_TypeCode_fixed_digits(CORBA_TypeCode code,  CORBA_Environment *ev);

/*!
 * @if jp
 * @brief 指定したkindの'fixed_scale'を返す。
 * @else
 * @brief return 'fixed_scale' of indicated 'kind'.
 * @endif
 * @param code TypeCode data
 * @param ev CORBA env(Exception Infomation)
 * @return fixed_scale
 */
short CORBA_TypeCode_fixed_scale(CORBA_TypeCode code,  CORBA_Environment *ev);

/*!
 * @if jp
 * @brief 指定したkindの'member_visibility'を返す。
 * @else
 * @brief return 'member_visibility' of indicated 'kind'.
 * @endif
 * @param code TypeCode data
 * @param index index of 'member_visibility'
 * @param ev CORBA env(Exception Infomation)
 * @return member_visibility
 */
CORBA_Visibility
CORBA_TypeCode_member_visibility(CORBA_TypeCode code, uint32_t index,  CORBA_Environment *ev);

/*!
 * @if jp
 * @brief 指定したkindの'type_modifier'を返す。
 * @else
 * @brief return 'type_modifier' of indicated 'kind'.
 * @endif
 * @param code TypeCode data
 * @param ev CORBA env(Exception Infomation)
 * @return type_modifier
 */
CORBA_ValueModifier
CORBA_TypeCode_type_modifier(CORBA_TypeCode code, CORBA_Environment *ev);

/*!
 * @if jp
 * @brief 指定したkindの'concrete_base_type'を返す。
 * @else
 * @brief return 'concrete_base_type' of indicated 'kind'.
 * @endif
 * @param code TypeCode data
 * @param ev CORBA env(Exception Infomation)
 * @return concrete_base_type
 */
CORBA_TypeCode
CORBA_TypeCode_concrete_base_type(CORBA_TypeCode code, CORBA_Environment *ev);

/*!
 * @if jp
 * @brief IDLコンパイル用処理(Orbit2準拠)
 * @else
 * @brief See orbit_cbe_type_is_fixed_length functions (idl-compile/orbit-idl-utils.c)
 * @endif
 * @param tc TypeCode data
 * @return 1:??/0:??
 */
int CORBA_TypeCode_is_fixed_size(CORBA_TypeCode tc);

/*!
 * @if jp
 * @brief TypeCode一覧情報を返す。
 * @else
 * @brief return info list of Type_Code.
 * @endif
 * @param kind 'kind' of TypeCode data
 * @return TypeCode data
 */
CORBA_TypeCode CORBA_TypeCode_get(CORBA_TCKind kind);

/*!
 * @if jp
 * @brief TypeCodeデータ登録 (マーシャル時)
 * @else
 * @brief register items of TypeCode data.
 * @endif
 * @param tc TypeCode data
 * @return '0'
 */
int
CORBA_TypeCode_add_dynamic(CORBA_TypeCode tc);

/*!
 * @if jp
 * @brief TypeCodeデータの取得(アンマーシャル時)
 * @else
 * @brief get TypeCode data.
 * @endif
 * @param kind 'kind' of TypeCode data
 * @param repoid repository ID
 * @param id   'identifier' data 
 * @return TypeCode data
 */
CORBA_TypeCode CORBA_TypeCode_get_dynamic(CORBA_TCKind kind, const char *repoid, const char *id);

#endif

