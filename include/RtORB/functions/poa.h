/*
 * Copyright (c) 2008, AIST.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */
/*
 * RtORB:
 *     poa.h
 *
 * Copyright(C) 2006, Isao Hara, AIST
 *
 * $Revision: 1.3 $
 * $Date: 2008/05/29 04:54:57 $
 * $Id: poa.h,v 1.3 2008/05/29 04:54:57 yoshi Exp $
 *
 */
/*!
 * @file poa.h
 * @if jp
 * @brief POA, POA_Manager, POA_Manager_Factoryなどに関する操作関数群。
 * @else
 * @brief Actions for POA, POA_Manager, POA_Manager_Factory, etc.
 * @endif
 * @author National Institute of Advanced Industrial Science and Technology (AIST)
 */

#ifndef __FUNC_POA_H__
#define __FUNC_POA_H__

/*!
 * @if jp
 * @brief "POA Manager Factory" を生成する。PortableServer_POAManagerFactory構造体の生成と初期化のみ。
 * @else
 * @brief create 'POA Manager Factory'.
 * @endif
 * @param orb ORB reference
 * @return POAManagerFactory reference
 */
PortableServer_POAManagerFactory PortableServer_POAManagerFactory_new(CORBA_ORB orb);

/*!
 * @if jp
 * @brief POA Manager Factoryに含まれるPOAManager全てを破棄する。
 * @else
 * @brief Destroy POA Manager Factory including each POA Managers.
 * @endif
 * @param factory POAManagerFactory reference
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void PortableServer_POAManagerFactory_destroy(PortableServer_POAManagerFactory factory, CORBA_Environment *env);

/*!
 * @if jp
 * @brief POA Manager Factoryに含まれるPOAManagerから、指定したIDと一致するものを探し、リファレンスを返す。
 * @else
 * @brief Return reference of POA Manager which is match to 'ID' from POA Manager Factory
 * @endif
 * @param factory POAManagerFactory reference
 * @param id ID for serch (=POA Manager's ID) 
 * @param env CORBA env(Exception Infomation)
 * @return 'POA Manager reference' / 'Null':Error or NotFound
 */
PortableServer_POAManager
PortableServer_POAManagerFactory_find(PortableServer_POAManagerFactory factory, char *id,
		CORBA_Environment *env);

/*!
 * @if jp
 * @brief POA Manager Facotroyに含まれるPOA Managerのリストを返す。
 * @else
 * @brief Return POA Mangar's list from POA_Manager_Factory.
 * @endif
 * @param factory POAManagerFactory reference
 * @param env CORBA env(Exception Infomation)
 * @return POA Manager's list
 */
PtrArray * PortableServer_POAManagerFactory_list(PortableServer_POAManagerFactory factory,
		CORBA_Environment *env);

/*!
 * @if jp
 * @brief "Root POA Manager"　を生成する。以下に詳細。
 *
 *        先ず、POAManagerFactory内のManagerリストに生成するManagerと同じIDのものが有るかどうかを確認する。
 *        新規であれば、PortableServer_POAManager構造体を生成し、初期化(HOLDING状態)する。
 *        自身をPOAManagerFactoryに追加登録する。
 * @else
 * @brief create 'Root POA Manager'. Check whether already exist or not by ID. Create and initialize POA Manager, and add to Factory.
 * @endif
 * @param factory POAManagerFactory reference
 * @param id ID for regist (=POA Manager's ID) 
 * @param env CORBA env(Exception Infomation)
 * @return 'POA Manager reference' / 'Null':Error or NotFound
 */
PortableServer_POAManager
PortableServer_POAManagerFactory_create_POAManager(PortableServer_POAManagerFactory factory,
		char *id, CORBA_Environment *env);

/*!
 * @if jp
 * @brief POA Managerを生成する。PortableServer_POAManager構造体を生成し、初期化(HOLDING状態)する。
 * @else
 * @brief Create POA Manager
 * @endif
 * @param id POA Manager's ID
 * @return 'POA Manager reference'
 */
PortableServer_POAManager PortableServer_POAManager_new(char *id);

/*!
 * @if jp
 * @brief "POA Manager"を活性化する。以下に詳細。
 *
 *        この関数では、登録されているすべてのPOAオブジェクトを活性化(POA_ACTIVE状態)する。RtORBでは、通信のためのイベントループにPOAのサービス用Socketポートに対して、データ受信時に呼び出される関数として、PortableServer_enqueue_request関数を設定する。
 *
 * @else
 * @brief Activate "POA Manager". Set status to 'POA_ACTIVE' and set property of 'SockProfile'.
 * @endif
 * @param poa_mgr POA Manager
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void PortableServer_POAManager_activate(PortableServer_POAManager poa_mgr,
		CORBA_Environment *env) ;

/*!
 * @if jp
 * @brief "POA Manager"を非活性化する。ステータスを'POA_INACTIVE'にし、GIOPコネクションのシャットダウン処理を行う。(新しいリクエストと未実行のリクエストは拒否される)
 * @else
 * @brief Deactivate "POA Manager". Set status to 'POA_INACTIVE' and shutdown GIOP connection, deny requests.
 * @endif
 * @param poa_mgr POA Manager
 * @param etherealize_objects (Unused)
 * @param wait_for_complation (Unused)
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void PortableServer_POAManager_deactivate(PortableServer_POAManager poa_mgr,
		CORBA_boolean etherealize_objects, CORBA_boolean wait_for_complation,
		CORBA_Environment *env) ;

/*!
 * @if jp
 * @brief "POA Manager"をdiscarding状態(リクエスト破棄状態)にする。ステータスを'POA_DISCARDING'にし、GIOPコネクションのシャットダウン処理を行う。(新しいリクエストとキュー内の未実行のリクエストは破棄される)
 * @else
 * @brief Change "POA Manager" to 'Discard' mode. Set status to 'POA_DISCARDING' and shutdown GIOP connection, destroy requests.
 * @endif
 * @param poa_mgr POA Manager
 * @param wait_for_complation (Unused)
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void PortableServer_POAManager_discars_requests(PortableServer_POAManager poa_mgr,
		CORBA_boolean wait_for_complation, CORBA_Environment *env) ;

/*!
 * @if jp
 * @brief "POA Manager"をHolding状態(リクエスト蓄積状態)にする。ステータスを'POA_HOLDING'にし、GIOPコネクションのシャットダウン処理を行う。(入ってくるリクエストはキュー内に蓄積される)
 * @else
 * @brief Change "POA Manager" to 'Holding' mode. Set status to 'POA_HOLDING' and shutdown GIOP connection, save requests to queue.
 * @endif
 * @param poa_mgr POA Manager
 * @param wait_for_complation (Unused)
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void PortableServer_POAManager_hold_requests(PortableServer_POAManager poa_mgr,
		CORBA_boolean wait_for_complation, CORBA_Environment *env) ;

/*!
 * @if jp
 * @brief POA Manager シャットダウン処理。"POA Manager"をdiscarding状態(リクエスト破棄状態)にする。ステータスを'POA_DISCARDING'にし、GIOPコネクションのシャットダウン処理を行う。(新しいリクエストとキュー内の未実行のリクエストは破棄される)
 * @else
 * @brief POA Manager shutdown action. Change "POA Manager" to 'Discard' mode. Set status to 'POA_DISCARDING' and shutdown GIOP connection, destroy requests.
 * @endif
 * @param poa_mgr POA Manager
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void PortableServer_POAManager_shutdown(PortableServer_POAManager poa_mgr, CORBA_Environment *env) ;

/*!
 * @if jp
 * @brief POA Manager 破棄処理。"POA Manager"をINACTIVE状態にし、GIOPコネクションをクローズしメモリ開放する。
 * @else
 * @brief POA Manager destroy action. Change "POA Manager" to 'INACTIVE' mode. Set status to 'POA_INACTIVE' and close GIOP connection.
 * @endif
 * @param poa_mgr POA Manager
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void PortableServer_POAManager_destory(PortableServer_POAManager poa_mgr, CORBA_Environment *env);

/*!
 * @if jp
 * @brief POA Managerのステータス値を返す。
 * @else
 * @brief Return status of POA Manager.
 * @endif
 * @param poa_mgr POA Manager
 * @param env CORBA env(Exception Infomation)
 * @return POA Manager's status (POA_HOLDING/POA_ACTIVE/POA_DISCARDING/POA_INACTIVE)
 */
uint32_t
PortableServer_POAManager_get_state(PortableServer_POAManager poa_mgr, CORBA_Environment *env);

/*!
 * @if jp
 * @brief POA ManagerのIDを返す。
 * @else
 * @brief Return ID of POA Manager.
 * @endif
 * @param poa_mgr POA Manager
 * @param env CORBA env(Exception Infomation)
 * @return POA Manager's ID
 */
char * PortableServer_POAManager_get_id(PortableServer_POAManager poa_mgr, CORBA_Environment *env);

/*!
 * @if jp
 * @brief "Root POA"　を生成する。(CORBAではORB内で唯一のRootPOAとRootPOAManagerが存在) 以下に詳細。
 *
 *        (1)PortableServer_POA_struct の生成 \n
 *        (2)生成したPortableServer_POA_struct のobject_mapを初期化する。初期値は、64アイテムが保存できるハッシュ表。\n
 *        (3)POAに対するGIOPポート用の構造体の生成。これは、GIOP_Connection__create 関数を呼び出すことで生成。
 *           この時、単に GIOP_Connection 構造体を生成し、ホストのIPアドレスのみを設定する。\n
 *        (4)GIOPのためのsocketポートの生成。GIOP_Connection__open関数を呼ぶことでGIOPの通信に必要なsocketポートの
 *           生成を行う。この関数では、ポート番号を引数で指定可能だが、'0'を指定するとシステム側で適当なポート番号を割り当てる。
 *        (5)requests スロットを０に設定し、idを設定する。\n
 *        (6)set_SocketProfile_arg関数で、socketポートに対応したSocketProfile構造体のargスロットに生成したPOAを代入する。\n
 *          このSockProfile構造体は、低レベルの通信を行うときに使用され、各ポートに接続時、切断時に呼び出す関数を設定したり、
 *          ポートにデータが来たときに呼び出す関数を設定することが可能。このargスロットはポート接続、切断時に呼び出される
 *          関数の引数となる。ただ、現在の実装では、未使用。\n
 * @else
 * @brief Create RootPOA.
 * @endif
 * @param id ID of RootPOA
 * @param port PORT number for socket 
 * @return POA reference
 */
PortableServer_POA PortableServer_POA_new(char *id, unsigned short port);

/*!
 * @if jp
 * @brief POAとPOA_Managerを生成する。内部で'PortableServer_POA_new()'と'PortableServer_POAManager_new()'を呼び生成する。
 * @else
 * @brief Create POA and POA Manager. Call 'PortableServer_POA_new()' and 'PortableServer_POAManager_new()'
.
 * @endif
 * @param ppoa POA reference
 * @param id ID of POA
 * @param mgr POA Manager
 * @param policies POA policy (Unused)
 * @param env CORBA env(Exception Infomation)
 * @return POA reference
 */
PortableServer_POA PortableServer_POA_createPOA(PortableServer_POA ppoa, char *id, 
		PortableServer_POAManager mgr, PortableServer_POA_Policy *policies, 
		CORBA_Environment *env);

/*!
 * @brief (T.B.D)
 */
int CORBA_ORB_enqueue_request(CORBA_ORB orb, int sock);

/*!
 * @brief (T.B.D)
 */
int PortableServer_POA_enqueue_request(PortableServer_POA poa, int sock);

/*!
 * @if jp
 * @brief POAアクティベート処理。PortableServer_POAManager_activate()を呼ぶのみ。
 * @else
 * @brief POA activate. Only call PortableServer_POAManager_activate().
 * @endif
 * @param poa POA
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void PortableServer_POA_activate(PortableServer_POA poa, CORBA_Environment *env) ;

/*!
 * @if jp
 * @brief POA破棄処理。PortableServer_POAManager_destory()を呼ぶのみ。
 * @else
 * @brief POA destroy. Only call PortableServer_POAManager_destory().
 * @endif
 * @param poa POA
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void PortableServer_POA_destory(PortableServer_POA poa, CORBA_Environment *env) ;

/*!
 * @if jp
 * @brief "POA Manager"を取得する。
 * @else
 * @brief get "POA Manager".
 * @endif
 * @param poa POA (RtORBではRootPOA)
 * @param env CORBA env(Exception Infomation)
 * @return POA Manager
 */
PortableServer_POAManager PortableServer_POA__get_the_POAManager(PortableServer_POA poa,
		CORBA_Environment *env);

/*!
 * @if jp
 * @brief データ受信時のリクエスト処理を行い、リクエストの有無を返す。
 * @else
 * @brief Request action at the time of getting data. Return flag.
 * @endif
 * @param h GIOP_ConnectionHandler
 * @return [1]:request is exist, [-1]:request is Null
 */
int PortableServer_enqueue_request(GIOP_ConnectionHandler *h);

/*!
 * @if jp
 * @brief リモートオブジェクトのmethodを実行する。以下に詳細。
 * 
 *        引数としてPOAオブジェクトをとり、poa->requestsに格納されたリクエスト(GIOP_enqueue_request関数内で生成)
 *        それぞれにに対して、GIOP_execute_request(poa, poa->requests)関数を実行することで、
 *        実際のリモート呼出を行っている。さらに、poaの子poaに対してもこの関数を再帰呼び出しすることで、
 *        argで引渡されたpoa以下のすべてのpoaに対するリクエストの処理を実現している。\n
 *        POA_main_loopでは、この引数として RootPOAを渡しており、これによりORBの内のすべての
 *        リクエストに対する処理を行っている。
 * @else
 * @brief Action of remote CORBA object's method.
 * @endif
 * @param arg POA object
 * @return void
 */
void PortableServer_execute_request(void *arg);

/*!
 * @if jp
 * @brief CORBAイベントループ処理。インターバルタイム:1000msecで、socketからのデータ受信後の処理を指定し、'server_loop()'を呼ぶのみ。
 * @else
 * @brief CORBA event roop action. Only call 'server_loop()'.
 * @endif
 * @param rootPOA rootPOA
 * @return void
 */
void POA_main_loop(PortableServer_POA rootPOA);

/*!
 * @brief (T.B.D)
 */
int POA_select_server_sockets(PortableServer_POA poa, fd_set *sockbits, int server_port,
		struct timeval *time_out);

/*!
 * @brief (T.B.D)
 */
void POA_server_socket_loop(PortableServer_POA poa, float time_out_float);

/*!
 * @if jp
 * @brief POAのアクティブオブジェクトマップ(AOM)に該当IDのオブジェクトを登録する。
 * @else
 * @brief Regist indicated object by ID to AOM of POA.
 * @endif
 * @param poa POA
 * @param servant PortableServer_Servant
 * @param env CORBA env(Exception Infomation)
 * @return 'ID of activated(registed) object', or 'Null':error.
 */
PortableServer_ObjectId PortableServer_POA_activate_object(PortableServer_POA poa, 
		PortableServer_Servant servant, CORBA_Environment *env);

/*!
 * @if jp
 * @brief サーバントの登録処理、IORの作成を行う。
 * @else
 * @brief Regist Servant and create IOR.
 * @endif
 * @param poa POA
 * @param servant PortableServer_Servant
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void register_PortableServer_Servant(PortableServer_POA poa,
		PortableServer_Servant servant, CORBA_Environment *env);

/*!
 * @if jp
 * @brief impl型のデータをCORBA_Objectに変換する。
 * @else
 * @brief change 'impl' type's Servant data to CORBA_Object data.
 * @endif
 * @param poa_servant impl type's POA Servant
 * @return CORBA object
 */
CORBA_Object impl_POA_ServantBase_to_Object(impl_POA_ServantBase *poa_servant);

/*!
 * @if jp
 * @brief POAオブジェクトを新規作成する。
 * @else
 * @brief Create new POA object.
 * @endif
 * @param info PortableServer_ClassInfo
 * @param servant PortableServer_Servant
 * @param finalize finalize of PortableServer_ServantBase__vepv
 * @param vepv PortableServer_ServantBase__vepv
 * @return void
 */
void RtORB_POA_Object__create(PortableServer_ClassInfo *info, PortableServer_Servant servant,
		void (*finalize)(), void *vepv);

/*!
 * @if jp
 * @brief POAのアクティブオブジェクトマップ(AOM)から該当IDのオブジェクトを削除する。
 * @else
 * @brief Delete indicated object by ID from AOM of POA.
 * @endif
 * @param poa POA
 * @param id ID for removing from AOM
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void PortableServer_POA_deactivate_object(PortableServer_POA poa, 
		PortableServer_ObjectId id, CORBA_Environment *env);

/*!
 * @if jp
 * @brief サーバントをPOAに再登録する。古いデータは事前に削除する。
 * @else
 * @brief Delete and re-regist Servant to POA.
 * @endif
 * @param poa POA 
 * @param obj CORBA object
 * @param object_key object key
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void PortableServer_POA_reinstall_object(PortableServer_POA poa,
                CORBA_Object obj, char *object_key, CORBA_Environment *env);

/*!
 * @if jp
 * @brief POAサーバントからオブジェクトリファレンスを返す。
 * @else
 * @brief Return object reference from POA servant.
 * @endif
 * @param poa POA
 * @param servant PortableServer_Servant
 * @param env CORBA env(Exception Infomation)
 * @return object reference
 */
CORBA_Object PortableServer_POA_servant_to_reference(PortableServer_POA poa,
		PortableServer_Servant servant, CORBA_Environment *env);

/*!
 * @if jp
 * @brief デフォルトPOAを返す。
 * @else
 * @brief Return default POA.
 * @endif
 * @param servant PortableServer_Servant
 * @param env CORBA env(Exception Infomation)
 * @return POA
 */
PortableServer_POA PortableServer_ServantBase__default_POA(PortableServer_Servant servant,
		CORBA_Environment *env);

/*!
 * @brief (T.B.D)
 */
CORBA_InterfaceDef * PortableServer_ServantBase__get_interface(PortableServer_Servant servant, 
		CORBA_Environment *env);

/*!
 * @if jp
 * @brief 指定したIDと一致するIDを持つサーバントがあるかどうか調べる。一致するものがあれば、'TRUE'、異なれば'FALSE'を返す。
 * @else
 * @brief Check whether the ID of Servant and indicated ID are matched or not, and return TRUE(matched) or FALSE(unmatched) as result.
 * @endif
 * @param servant PortableServer_Servant
 * @param id ID for search
 * @param env CORBA env(Exception Infomation)
 * @return True:matched, False:unmatched
 */
CORBA_boolean PortableServer_ServantBase__is_a(PortableServer_Servant servant, const char *id,
		CORBA_Environment *env);

/*!
 * @if jp
 * @brief オブジェクトのnon_existチェック。正常時はFalseを返す。(T.B.D)
 * @else
 * @brief Check whether object is not exist or not. Always return 'False' normally. (T.B.D)
 * @endif
 * @param servant PortableServer_Servant
 * @param env CORBA env(Exception Infomation)
 * @return false (T.B.D)
 */
CORBA_boolean PortableServer_ServantBase__non_existent(PortableServer_Servant servant,
		CORBA_Environment *env);

/*!
 * @brief (T.B.D)
 */
void PortableServer_ServantBase__add_ref(PortableServer_Servant servant,  CORBA_Environment *env);

/*!
 * @brief (T.B.D)
 */
void PortableServer_ServantBase__remove_ref(PortableServer_Servant servant,  CORBA_Environment *env);

/*!
 * @if jp
 * @brief サーバントベースの削除。メモリリリース。
 * @else
 * @brief Delete ServantBase and release memory.
 * @endif
 * @param servant PortableServer_Servant
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void PortableServer_ServantBase__fini(PortableServer_Servant servant, CORBA_Environment *env);

/*!
 * @if jp
 * @brief サーバントベースの初期化。vepvの初期設定。
 * @else
 * @brief Initialize ServantBase. Set init values to 'vepv'.
 * @endif
 * @param servant PortableServer_Servant
 * @param env CORBA env(Exception Infomation)
 * @return void
 */
void PortableServer_ServantBase__init(PortableServer_ServantBase *servant, CORBA_Environment *env);

/*!
 * @if jp
 * @brief 現在存在しているORBに対応したRootPOAを返す。
 * @else
 * @brief Return 'RootPOA' related current ORB.
 * @endif
 * @param ev CORBA env(Exception Infomation)
 * @return RootPOA reference
 */
PortableServer_POA PortableServer_root_POA(CORBA_Environment *ev);

#endif
