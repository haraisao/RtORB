/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 *   RtORB:
 *     orb.h
 *
 *   Copyright(C) 2006, Isao Hara, AIST
 *
 *   $Revision: 1.1.1.1 $
 *   $Date: 2008/01/08 07:56:06 $
 *   $Id: orb.h,v 1.1.1.1 2008/01/08 07:56:06 yoshi Exp $
 *
 */
/*!
 * @file orb.h
 * @if jp
 * @brief CORBA_ORB関連処理群
 * @else
 * @brief CORBA ORB actions.
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __FUNC_ORB_H__
#define __FUNC_ORB_H__

/*
#ifndef CORBA_POLICY_ERROR_CODE
#define CORBA_POLICY_ERROR_CODE	1
const CORBA_PolicyErrorCode BAD_POLICY = 0;
const CORBA_PolicyErrorCode UNSUPPORTED_POLICY = 1;
const CORBA_PolicyErrorCode BAD_POLICY_TYPE = 2;
const CORBA_PolicyErrorCode BAD_POLICY_VALUE = 3;
const CORBA_PolicyErrorCode UNSUPPORTED_POLICY_VALUE = 4;
#endif
hashtable *CORBA_Object_Table;
*/

/*!
 * @if jp
 * @brief CORBA exceptionの初期化。envのメモリ確保のみ。
 * @else
 * @brief Initialize CORBA exception. Allocate memory for 'env'. 
 * @endif
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void CORBA_exception_init(CORBA_Environment *env);

/*!
 * @if jp
 * @brief _repo_id(例外レポート番号)を返す。 
 * @else
 * @brief return ID of Exception Report.
 * @endif
 * @param ev CORBA env(Exception Infomation)
 * @return repo_id(Exception Report ID)
 */
char * CORBA_exception_id(CORBA_Environment *ev);

/*!
 * @if jp
 * @brief _params(例外識別パラメータ)を返す。
 * @else
 * @brief return Exception Parameter. 
 * @endif
 * @param ev CORBA env(Exception Infomation)
 * @return _params(Exception parameter) 
 */
CORBA_any  CORBA_exception_value(CORBA_Environment *ev);

/*!
 * @if jp
 * @brief CORBA exceptionのメモリ開放
 * @else
 * @brief Free action of CORBA exception's memory.
 * @endif
 * @param ev CORBA env(Exception Infomation)
 * @return void
 */
void CORBA_exception_free(CORBA_Environment *ev);

/*!
 * @if jp
 * @brief CORBA exceptionのセット
 * @else
 * @brief Set CORBA exception.
 * @endif
 * @param ev CORBA env(Exception Infomation)
 * @param id exception type
 * @param report Exception Report ID
 * @return void
 */
void set_CORBA_exception(CORBA_Environment *ev, uint32_t id, char *report);

/**************************************************/
/*!
 * @if jp
 * @brief ServerIDを標準エラー出力に出力 (ORB オプション)
 * @else
 * @brief output ServerID (ORB option)
 * @endif
 * @param cfg (Unused)
 * @param argv ServerID
 * @return void
 */
void setServiceID(CORBA_Config *cfg, char *argv[]);

/*!
 * @if jp
 * @brief ORBidを標準エラー出力に出力 (ORB オプション)
 * @else
 * @brief output ORBid (ORB option)
 * @endif
 * @param cfg (Unused)
 * @param argv ORBid
 * @return void
 */
void setORBid(CORBA_Config *cfg, char *argv[]);

/*!
 * @if jp
 * @brief ListenEndpointsを標準エラー出力に出力 (ORB オプション)
 * @else
 * @brief output ListenEndpoint (ORB option)
 * @endif
 * @param cfg (Unused)
 * @param argv ListenEndpoints
 * @return void
 */
void setListenEndpoints(CORBA_Config *cfg, char *argv[]);

/*!
 * @if jp
 * @brief ProprietayActivationを標準エラー出力に出力 (ORB オプション)
 * @else
 * @brief output ProprietayActivation (ORB option)
 * @endif
 * @param cfg (Unused)
 * @param argv ProprietayActivation
 * @return void
 */
void setProprietaryActivation(CORBA_Config *cfg, char *argv[]);

/*!
 * @if jp
 * @brief 'InitRef'をORBのコンフィグにセットする。
 * @else
 * @brief set 'InitRef' to ORB's Config.
 * @endif
 * @param cfg ORB Config
 * @param argv InitRef
 * @return void
 */
void setInitRef(CORBA_Config *cfg, char *argv[]);

/*!
 * @if jp
 * @brief 'InitRef'をORBのコンフィグにデフォルトセットする。
 * @else
 * @brief set 'InitRef' to ORB's Config as default value.
 * @endif
 * @param cfg ORB Config
 * @param argv InitRef
 * @return void
 */
void setDefaultInitRef(CORBA_Config *cfg, char *argv[]);

/*!
 * @if jp
 * @brief PORT番号をORBのコンフィグにデフォルトセットする。
 * @else
 * @brief set PORT number to ORB's Config.
 * @endif
 * @param cfg ORB Config
 * @param argv ServerPort
 * @return void
 */
void setServerPort(CORBA_Config *cfg, char *argv[]);

/*!
 * @if jp
 * @brief 引数のオプションの中身チェック
 * @else
 * @brief check arguments (ORB option)
 * @endif
 * @param argv options
 * @param n number of arguments
 * @return 1:OK, 0:NG
 */
int checkArgParams(char *argv[], int n);

/*!
 * @if jp
 * @brief 引数のオプションチェック
 * @else
 * @brief check arguments and do it. (ORB option)
 * @endif
 * @param cfg ORB config
 * @param argc argc
 * @param argv options
 * @param i index
 * @param arg_name argments name
 * @param func pointer of function
 * @param requires the number of required argments
 * @return 1:matched, 0:not matched
 */
int checkArgs(CORBA_Config *cfg, int *argc, char *argv[], int i,
		char *arg_name, void (*func)(), int requires);

/*!
 * @if jp
 * @brief ORBの初期化と初期設定、RootPOA, POAManager, POAManagerFactoryの生成、ORB起動オプション引数チェック
 * @else
 * @brief Initialize ORB, create RootPOA, POAManager, POAManagerFactory and check ORB options 
 * @endif
 * @param argc number of arguments
 * @param argv arguments (ORB options)
 * @param orb_id ORB ID(Unused)
 * @param env CORBA env(Exception Infomation)
 * @return Reference of ORB 
 */
CORBA_ORB CORBA_ORB_init(int *argc, char **argv, char orb_id, CORBA_Environment *env);

/*!
 * @if jp
 * @brief id(ObjectKey)からCORBAオブジェクトのリファレンスを取得する。異常終了時には、例外情報がセットされる。 
 *
 *        ORB起動時に"-ORBInitiRef"オプションで設定したオブジェクトリファレンスとORBの"_object_table"に格納されている、
 *        CORBA_Objectの中からobject_keyが同じものを検索して返す。検索する順番は"-ORBInitiRef"で指定したものを最優先
 *        としており、もし一致するObjectが存在しない場合には、CORBA_Environmentに"InvalidName"のシステムエラーを設定する。
 *        なお、"-ORBInitiRef"に登録しているオブジェクトリファレンスは、CORBA_ORB_string_to_object関数でCORBA_Objectに変換される。
 * @else
 * @brief get Object reference by id(ObjectKey) 
 * @endif
 * @param orb ORB reference
 * @param id Object Key
 * @param env CORBA env(Exception Infomation)
 * @return Reference of CORBA Object
 */
CORBA_Object CORBA_ORB_resolve_initial_references(CORBA_ORB orb, char *id,  CORBA_Environment *env);

/*!
 * @if jp
 * @brief CORBAオブジェクトを新規作成する。
 *
 *        単に一意のobject_keyを設定しているのみです。実際には、XXX.idlからidlコンパイラによって生成されたXXX-skelimpl.c
 *        に具体的なオブジェクトの実装を記述する必要があり。
 * @else
 * @brief Create CORBA Object
 * @endif
 * @param obj_key Object Key
 * @return Reference of CORBA Object
 */
CORBA_Object new_CORBA_Object(char *obj_key);

/*!
 * @if jp
 * @brief CORBAオブジェクトの開放処理。リファレンスカウンタのデクリメントと削除処理を行う。内部で'delete_CORBA_Object()'呼ぶ。
 * @else
 * @brief Free action of CORBA Object. reduce Counter and delete Object's contens. Call 'delete_CORBA_Object()'.
 * @endif
 * @param obj CORBA Object
 * @return reference counter of CORBA Object
 */
int CORBA_Object_free(CORBA_Object obj);

/*!
 * @if jp
 * @brief CORBA Objectの削除処理。 内部の要素の削除、メモリリリース。
 * @else
 * @brief Delete CORBA Object. 
 * @endif
 * @param obj CORBA Object
 * @return void
 */
void delete_CORBA_Object(CORBA_Object obj);

/*!
 * @if jp
 * @brief コピー作製処理。'リファレンスカウンタ'を１つ増やすのみ。
 * @else
 * @brief Copy action. Only increment 'Refernce Counter'. 
 * @endif
 * @param obj CORBA Object
 * @return CORBA Object
 */
CORBA_Object CORBA_Object_dup(CORBA_Object obj);

/*!
 * @if jp
 * @brief CORBAオブジェクトのKeyをセットする。
 * @else
 * @brief set new Object Key.
 * @endif
 * @param obj CORBA Object
 * @param key Object Key
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void CORBA_Object__set_object_key(CORBA_Object obj, char *key, CORBA_Environment *env);

/*!
 * @if jp
 * @brief narrow action
 * @else
 * @brief narrow action
 * @endif
 * @param obj CORBA Object
 * @param tc CORBA TypeCode
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void CORBA_Object__narrow(CORBA_Object obj, CORBA_TypeCode tc, CORBA_Environment *env);

/*!
 * @if jp
 * @brief ORB IDを返す
 * @else
 * @brief return ORB_ID.
 * @endif
 * @param orb ORB reference
 * @param env CORBA env(Exception Infomation)
 * @return ORB ID
 */
unsigned char * CORBA_ORB_id(CORBA_ORB orb, CORBA_Environment *env);

/*!
 * @if jp
 * @brief Objectリファレンスを文字列に変換する。
 * @else
 * @brief Convert an object reference to String.
 * @endif
 * @param obj CORBA Object
 * @param env CORBA env(Exception Infomation)
 * @return A stringifed Internet Object Reference(IOR)
 */
char *CORBA_Object__to_string(CORBA_Object obj, CORBA_Environment *env);

/*!
 * @if jp
 * @brief Objectリファレンスを文字列(IOR)に変換する。
 * @else
 * @brief Convert an object reference to String.
 * @endif
 * @param orb ORB reference
 * @param obj CORBA Object
 * @param env CORBA env(Exception Infomation)
 * @return A stringifed Internet Object Reference(IOR)
 */
unsigned char * CORBA_ORB_object_to_string(CORBA_ORB orb, CORBA_Object obj,  CORBA_Environment *env);

/*!
 * @if jp
 * @brief 文字列(IOR)をObjectリファレンスに変換する。
 *
 *        CORBAのURL表現(corbaloc://...)からCORBA_Objectを生成する。CORBAのURLのパージングは、parseCorbaURL関数で
 *        実施され、object->_urlに格納されるようになっている。このパージング後のURLは、動作速度を少し上げる以上の
 *        効果は無いので（CORBA_Connectionやobject_keyがあれば再構成可能なため）、COBRA_Objectに持たせる必要は
 *        ないかもしれず、この辺りは、実装が予告なく変更される可能性がある。(T.B.D)
 * @else
 * @brief Convert String to an object reference.
 * @endif
 * @param orb ORB reference
 * @param str IOR
 * @param env CORBA env(Exception Infomation)
 * @return Object reference
 */
CORBA_Object CORBA_ORB_string_to_object(CORBA_ORB orb, unsigned char *str,  CORBA_Environment *env);

/*!
 * @if jp
 * @brief (T.B.D)
 * @else
 * @brief (T.B.D)
 * @endif
 * @param orb ORB reference
 * @param service_type (T.B.D)
 * @param service_information (T.B.D)
 * @param env CORBA env(Exception Infomation)
 * @return (T.B.D)
 */
boolean CORBA_ORB_get_service_information(CORBA_ORB orb, unsigned short service_type,
		CORBA_ServiceInforamtion *service_information, CORBA_Environment *env) ;

/*!
 * @if jp
 * @brief デフォルトContextオブジェクトのリファレンスを取得する。
 * @else
 * @brief get default Context Object Reference.
 * @endif
 * @param orb ORB reference
 * @param ctx Context Object Reference
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void CORBA_ORB_get_default_context(CORBA_ORB orb, CORBA_Context *ctx, CORBA_Environment *env) ;

/*!
 * @if jp
 * @brief (T.B.D)
 * @else
 * @brief (T.B.D)
 * @endif
 * @param orb ORB reference
 * @param env CORBA env(Exception Infomation)
 * @return (T.B.D)
 */
boolean CORBA_ORB_work_pending(CORBA_ORB orb, CORBA_Environment *env);

/*!
 * @if jp
 * @brief (T.B.D)
 * @else
 * @brief (T.B.D)
 * @endif
 * @param orb ORB reference
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void CORBA_ORB_perform_work(CORBA_ORB orb, CORBA_Environment *env);

/*!
 * @if jp
 * @brief CORBAイベントループ処理。RootPOAの参照を指定し、POA_main_loop()を呼ぶのみ。(selectシステムコールで入力待ちする無限ループ)
 * @else
 * @brief CORBA event roop action. Only call POA_main_loop().
 * @endif
 * @param orb ORB reference
 * @param env CORBA env(Exception Infomation)(Unused)
 * @return void
 */
void CORBA_ORB_run(CORBA_ORB orb,  CORBA_Environment *env) ;

/*!
 * @if jp
 * @brief (T.B.D)
 * @else
 * @brief (T.B.D)
 * @endif
 * @param orb ORB reference
 * @param wait_for_completion (T.B.D)
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void CORBA_ORB_shutdown(CORBA_ORB orb, boolean wait_for_completion, CORBA_Environment *env) ;

/*!
 * @if jp
 * @brief ORB,RootPOA,POAManager,POAManagerFactoryを破棄する。
 * @else
 * @brief destroy ORB,RootPOA,POAManager,POAManagerFactory.
 * @endif
 * @param orb ORB reference
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void CORBA_ORB_destroy(CORBA_ORB orb, CORBA_Environment *env);

/*!
 * @if jp
 * @brief 次のRequest_IDを返す
 * @else
 * @brief return next Request_ID.
 * @endif
 * @param orb ORB reference
 * @param env CORBA env(Exception Infomation)
 * @return next request ID
 */
uint32_t CORBA_ORB_get_next_request_id(CORBA_ORB orb, CORBA_Environment *env);


/*!
 * @if jp
 * @brief interfaceを返す
 * @else
 * @brief return 'interface' of CORBA Object
 * @endif
 * @param object CORBA Object
 * @param env CORBA env(Exception Infomation) (Unused)
 * @return Interface
 */
CORBA_InterfaceDef * CORBA_Object_get_interface(CORBA_Object object, CORBA_Environment *env);

/*!
 * @if jp
 * @brief repository_idを返す
 * @else
 * @brief return 'repository_id' of CORBA Object
 * @endif
 * @param object CORBA Object
 * @param env CORBA env(Exception Infomation)
 * @return repository_id
 */
uint32_t * CORBA_Object_repository_id(CORBA_Object object, CORBA_Environment *env);

/*!
 * @if jp
 * @brief コピー作製処理。'リファレンスカウンタ'を１つ増やすのみ。 
 * @else
 * @brief Copy action. Only increment 'Refernce Counter'. 
 * @endif
 * @param object CORBA Object
 * @param env CORBA env(Exception Infomation) (Unused)
 * @return CORBA Object
 */
CORBA_Object CORBA_Object_duplicate(CORBA_Object object, CORBA_Environment *env);

/*!
 * @if jp
 * @brief 現在使用しているオブジェクトリファレンスを解放する。
 * @else
 * @brief Release object reference.
 * @endif
 * @param object CORBA Object
 * @param env CORBA env(Exception Infomation) (Unused)
 * @return void
 */
void CORBA_Object_release(CORBA_Object object, CORBA_Environment *env);

/*!
 * @if jp
 * @brief CORBAオブジェクトがNILかどうかの判定をする。
 * @else
 * @brief Check whether the given object is 'NIL' or not.
 * @endif
 * @param object CORBA Object
 * @param env CORBA env(Exception Infomation) (Unused)
 * @return TRUE: nil, FALSE: exist
 */
boolean CORBA_Object_is_nil(CORBA_Object object, CORBA_Environment *env);

/*!
 * @if jp
 * @brief CORBAオブジェクトがnarrowによる特定のインタフェースを実装したものかどうかをチェックする。(リポジトリIDの比較)
 * @else
 * @brief To find out if this object can be narrowed to the object(implemented interface). (Comapre repository_ID)
 * @endif
 * @param object CORBA Object
 * @param logical_type_id Logical Type ID (repository ID)
 * @param env CORBA env(Exception Infomation) (Unused)
 * @return FALSE:matched, TRUE:not matched
 */
boolean CORBA_Object_is_a(CORBA_Object object, unsigned char *logical_type_id,
		CORBA_Environment *env);

/*!
 * @if jp
 * @brief 対象のオブジェクトのオブジェクトリファレンスがすでに破棄されているかどうかを確認する。 
 * @else
 * @brief Check whether the given object is existent or not.
 * @endif
 * @param object CORBA Object
 * @param env CORBA env(Exception Infomation) (Unused)
 * @return TRUE: exist, FALSE: non exist
 */
boolean CORBA_Object_non_existent(CORBA_Object object, CORBA_Environment *env);

/*!
 * @if jp
 * @brief elfhash()によるハッシュ値の算出をし、結果をmaximumにて割り算した剰余をvalとして返す。
 * @else
 * @brief Calc Hash value by 'elfhash()' and adjust by max value. Return output value.
 * @endif
 * @param object CORBA Object
 * @param maximum max value
 * @param env CORBA env(Exception Infomation)
 * @return adjusted Hash value
 */
uint32_t CORBA_Object_hash(CORBA_Object object, uint32_t maximum, CORBA_Environment *env);

/*!
 * @if jp
 * @brief 2つのオブジェクトの参照が等価かどうかの判定する。 
 *
 *        First, compare typedId if exist. They might be empty.\n
 *        Second, compare object_key.
 * @else
 * @brief Checks whether the given Object is equivalent to other Object. 
 *
 *        First, compare typedId if exist. They might be empty.\n
 *        Second, compare object_key.
 * @endif
 * @param object CORBA Object
 * @param other_object CORBA Object
 * @param env CORBA env(Exception Infomation) (Unused)
 * @return FALSE:unmatched ,TRUE:matched
 */
boolean CORBA_Object_is_equivalent(CORBA_Object object, CORBA_Object other_object,
		CORBA_Environment *env);


/*!
 * @if jp
 * @brief (T.B.D)
 * @else
 * @brief (T.B.D)
 * @endif
 * @param object (T.B.D)
 * @param policy_type (T.B.D)
 * @param env (T.B.D)
 * @return (T.B.D)
 */
CORBA_Policy * CORBA_Object_get_policy(CORBA_Object object, CORBA_PolicyType policy_type,
		CORBA_Environment *env);

/*!
 * @if jp
 * @brief (T.B.D)
 * @else
 * @brief (T.B.D)
 * @endif
 * @param object (T.B.D)
 * @param type (T.B.D)
 * @param env (T.B.D)
 * @return (T.B.D)
 */
CORBA_Policy * CORBA_Object_get_client_policy(CORBA_Object object, CORBA_PolicyType type,
		CORBA_Environment *env);

/*!
 * @if jp
 * @brief (T.B.D)
 * @else
 * @brief (T.B.D)
 * @endif
 * @param object (T.B.D)
 * @param types (T.B.D)
 * @param env (T.B.D)
 * @return (T.B.D)
 */
CORBA_PolicyList * CORBA_Object_get_policy_overrides(CORBA_Object object,
		CORBA_PolicyTypeSeq types, CORBA_Environment *env);

/*!
 * @if jp
 * @brief (T.B.D)
 * @else
 * @brief (T.B.D)
 * @endif
 * @param object CORBA Object
 * @param policies (T.B.D)
 * @param set_add (T.B.D)
 * @param env CORBA env(Exception Infomation)
 * @return (T.B.D)
 */
CORBA_Object CORBA_Object_set_policy_overrides(CORBA_Object object, CORBA_PolicyList policies,
		uint32_t set_add, CORBA_Environment *env);

/*!
 * @if jp
 * @brief (T.B.D)
 * @else
 * @brief (T.B.D)
 * @endif
 * @param object CORBA Object
 * @param inconsistent_policies (T.B.D)
 * @param env CORBA env(Exception Infomation)
 * @return (T.B.D)
 */
boolean CORBA_Object_validate_connection(CORBA_Object object,
		CORBA_PolicyList inconsistent_policies, CORBA_Environment *env);

/*!
 * @if jp
 * @brief (T.B.D)
 * @else
 * @brief (T.B.D)
 * @endif
 * @param object (T.B.D)
 * @param env (T.B.D)
 * @return (T.B.D)
 */
CORBA_DomainManagerList * CORBA_Object_get_domain_managers(CORBA_Object object,
		CORBA_Environment *env);

/*!
 * @if jp
 * @brief (T.B.D)
 * @else
 * @brief (T.B.D)
 * @endif
 * @param object (T.B.D)
 * @param env (T.B.D)
 * @return (T.B.D)
 */
CORBA_Object CORBA_Object_get_component( CORBA_Object object, CORBA_Environment *env);

/*!
 * @if jp
 * @brief ORBへのポインタを返す
 * @else
 * @brief return 'ORB pointer'
 * @endif
 * @param object CORBA Object
 * @param env CORBA env(Exception Infomation) (Unused)
 * @return ORB pointer
 */
CORBA_ORB CORBA_Object_get_orb( CORBA_Object object, CORBA_Environment *env);

/*!
 * @if jp
 * @brief CORBAポリシー用の領域を確保し生成する。
 * @else
 * @brief create CORBA policy
 * @endif
 * @return CORBA policy
 */
CORBA_Policy new_CORBA_Policy();

/*!
 * @if jp
 * @brief CORBAポリシーをコピーする。
 * @else
 * @brief copy CORBA policy to other.
 * @endif
 * @param policy CORBA policy
 * @param env CORBA env(Exception Infomation)
 * @return copied ORBA policy
 */
CORBA_Policy CORBA_Policy_copy(CORBA_Policy policy, CORBA_Environment *env);

/*!
 * @if jp
 * @brief CORBAポリシー用の領域を開放し削除する。
 * @else
 * @brief delete CORBA policy
 * @endif
 * @param policy CORBA policy
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void CORBA_Policy_destroy(CORBA_Policy policy, CORBA_Environment *env);

/*!
 * @if jp
 * @brief CORBAポリシーを設定する。
 * @else
 * @brief set  CORBA policy
 * @endif
 * @param orb ORB reference
 * @param type Policy Type
 * @param val Policy Statement
 * @param env CORBA env(Exception Infomation)
 * @return CORBA policy
 */
CORBA_Policy CORBA_ORB_create_policy(CORBA_ORB orb, CORBA_PolicyType type,
		CORBA_any val, CORBA_Environment *env);

#endif
