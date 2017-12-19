#ifndef __RPC__MOGO__HEAD__
#define __RPC__MOGO__HEAD__

#include "type_mogo.h"
#include "pluto.h"
#include "defparser.h"
//#include "logger.h"


namespace mogo
{

	enum MSGID_ENUM_TYPE
	{
	//�����������ͻ��˵İ�
	MSGID_CLIENT_LOGIN_RESP = 1,                                  //�����������ͻ��˵��˺ŵ�¼���
	MSGID_CLIENT_NOTIFY_ATTACH_BASEAPP = 2,                                  //����baseapp֪ͨ
	MSGID_CLIENT_ENTITY_ATTACHED = 3,                                  //֪ͨ�ͻ����Ѿ�attach��һ��������entity,ͬʱˢ����
	MSGID_CLIENT_AVATAR_ATTRI_SYNC = 4,                                  //AVATAR��������޸�ͬ��
	MSGID_CLIENT_RPC_RESP = 5,                                  //�����������ͻ��˵�rpc
	MSGID_CLIENT_ENTITY_POS_SYNC = 6,                                  //���������߿ͻ�������仯(move)
	//MSGID_CLIENT_ENTITY_SPACE_CHANGE        = 7,                                  //���������߿ͻ��˳����仯
	MSGID_CLIENT_AOI_ENTITIES = 8,                                  //���������߿ͻ���aoi��Χ�ڵ�entity
	MSGID_CLIENT_AOI_NEW_ENTITY = 9,                                  //���������߿ͻ���aoi��Χ��������entity
	MSGID_CLIENT_ENTITY_CELL_ATTACHED = 10,                                 //������������ͻ��˵�cell_client����
	MSGID_CLIENT_OTHER_ENTITY_ATTRI_SYNC = 11,                                 //����entity���Ա仯ͬ��
	MSGID_CLIENT_OTHER_ENTITY_POS_SYNC = 12,                                 //����entity����仯ͬ��(move)
	MSGID_CLIENT_OTHER_ENTITY_MOVE_REQ = 13,                                 //������ת��������entity���ƶ�����
	//MSGID_CLIENT_OTHER_RPC_RESP             = 14,                                 //�������ͻ���entity��rpc
	MSGID_CLIENT_AOI_DEL_ENTITY = 15,                                 //��entity�뿪��aoi

	MSGID_CLIENT_ENTITY_POS_PULL = 16,                                 //���������߿ͻ�������仯(����)
	MSGID_CLIENT_OTHER_ENTITY_POS_PULL = 17,                                 //������ת��������entity���ƶ�����(����)
	MSGID_CLIENT_ENTITY_POS_TELEPORT = 18,                                 //���������߿ͻ�������仯(teleport)
	MSGID_CLIENT_OTHER_ENTITY_TELEPORT = 19,                                 //������ת��������entity���ƶ�����(teleport)
	MSGID_CLIENT_CHECK_RESP = 20,

	#ifdef __RELOGIN
	MSGID_CLIENT_RELOGIN_KEY_NOT_EXIT = 21,                                 //��������ʱ��������֪ͨ�ͻ���key�Ѿ�������
	#endif
	MSGID_CLIENT_NOTIFY_MULTILOGIN = 22,                                 //������֪ͨ�µĿͻ��˶Ͽ�����


	//�ͻ��˷����������İ�
	MSGID_LOGINAPP_CHECK = 30,                                 //�ͻ��˰汾У��
	MSGID_LOGINAPP_LOGIN = 31,                                 //�ͻ��������ʻ���/������е�¼��֤
	MSGID_BASEAPP_CLIENT_LOGIN = 32,
	MSGID_BASEAPP_CLIENT_RPCALL = 33,                                 //�ͻ��˷����Զ�̵���
	MSGID_BASEAPP_CLIENT_MOVE_REQ = 34,                                 //�ͻ��˷�����ƶ�
	MSGID_BASEAPP_CLIENT_RPC2CELL_VIA_BASE = 35,                                 //�ͻ��˷����(;��base��)cell��������
	MSGID_BASEAPP_CLIENT_OTHERS_MOVE_REQ = 36,                                 //�ͻ��˷��������entity���ƶ�(�����Ӷ����)


	MSGID_BASEAPP_CLIENT_RELOGIN = 37,                                 //�ͻ��˶�������

	//�ݶ�50���µ��ǿͻ��˺ͷ������Ľ�����,��Ҫ����
	MAX_CLIENT_SERVER_MSGID = 50,

	MSGID_ALLAPP_ONTICK = 101,                                //������Ϣ
	MSGID_ALLAPP_SETTIME = 102,                                //ͬ��ʱ����Ϣ
	MSGID_ALLAPP_SHUTDOWN_SERVER = 103,                                //�رշ�����֪ͨ

	//MSGID_LOGINAPP_LOGIN                    = MSGTYPE_LOGINAPP + 1,             //�ͻ��������ʻ���/������е�¼��֤
	MSGID_LOGINAPP_MODIFY_LOGIN_FLAG = MSGTYPE_LOGINAPP + 6,               //�޸ķ������Ƿ���Ե�¼���
	MSGID_LOGINAPP_SELECT_ACCOUNT_CALLBACK = MSGTYPE_LOGINAPP + 7,
	MSGID_LOGINAPP_NOTIFY_CLIENT_TO_ATTACH = MSGTYPE_LOGINAPP + 8,
	MSGID_LOGINAPP_FORBIDLOGIN = MSGTYPE_LOGINAPP + 9,               //��ֹ��½
	MSGID_LOGINAPP_MODIFY_ONLINE_COUNT = MSGTYPE_LOGINAPP + 10,              //�޸ķ�������ǰ��������
	MSGID_LOGINAPP_LOGIN_VERIFY_CALLBACK = MSGTYPE_LOGINAPP + 11,              //sdk��������֤����
	MSGID_LOGINAPP_FORBID_IP_UNTIL_TIME = MSGTYPE_LOGINAPP + 12,               //��ֹip��½����ֹʱ��
	MSGID_LOGINAPP_FORBID_ACCOUNT_UNTIL_TIME = MSGTYPE_LOGINAPP + 13,               //��ֹ�˺ŵ�½����ֹʱ��
	MSGID_LOGINAPP_NOTIFY_CLIENT_MULTILOGIN = MSGTYPE_LOGINAPP + 14,               //֪ͨ�µĿͻ��˶Ͽ�����
	MSGID_LOGINAPP_WEBLOGIN = MSGTYPE_LOGINAPP + 15,               //�����Դ�web�������ĵ�½

	MSGID_BASEAPPMGR_REGISTERGLOBALLY = MSGTYPE_BASEAPPMGR + 1,
	MSGID_BASEAPPMGR_CREATEBASE_ANYWHERE = MSGTYPE_BASEAPPMGR + 2,               //
	MSGID_BASEAPPMGR_CREATEBASE_FROM_DBID_ANYWHERE = MSGTYPE_BASEAPPMGR + 3,        //
	MSGID_BASEAPPMGR_CREATEBASE_FROM_NAME_ANYWHERE = MSGTYPE_BASEAPPMGR + 4,        //
	MSGID_BASEAPPMGR_CREATE_CELL_IN_NEW_SPACE = MSGTYPE_BASEAPPMGR + 5,        //
	MSGID_BASEAPPMGR_SHUTDOWN_SERVERS = MSGTYPE_BASEAPPMGR + 6,               //ֹͣ������
	MSGID_BASEAPPMGR_ON_SERVER_SHUTDOWN = MSGTYPE_BASEAPPMGR + 7,               //ĳ������������ֹͣ��Ļص�����
	MSGID_BASEAPPMGR_CREATEBASE_FROM_NAME = MSGTYPE_BASEAPPMGR + 8,               //
	MSGID_BASEAPPMGR_SHUTDOWN_SERVERS_CALLBACK = MSGTYPE_BASEAPPMGR + 9,            //�رշ������Ļ�Ӧ

	MSGID_BASEAPP_REGISTERGLOBALLY_CALLBACK = MSGTYPE_BASEAPP + 1,
	MSGID_BASEAPP_ADD_GLOBALBASE = MSGTYPE_BASEAPP + 2,
	MSGID_BASEAPP_DEL_GLOBALBASE = MSGTYPE_BASEAPP + 3,
	MSGID_BASEAPP_INSERT_ENTITY_CALLBACK = MSGTYPE_BASEAPP + 4,
	MSGID_BASEAPP_SELECT_ENTITY_CALLBACK = MSGTYPE_BASEAPP + 5,
	MSGID_BASEAPP_UPDATE_ENTITY_CALLBACK = MSGTYPE_BASEAPP + 6,
	MSGID_BASEAPP_LOOKUP_ENTITY_CALLBACK = MSGTYPE_BASEAPP + 7,
	MSGID_BASEAPP_ENTITY_MULTILOGIN = MSGTYPE_BASEAPP + 8,                //����ͻ��˵�¼��һ��entity
	MSGID_BASEAPP_LOAD_ALL_AVATAR = MSGTYPE_BASEAPP + 9,                //�ű������load����Avatar
	MSGID_BASEAPP_ON_GET_CELL = MSGTYPE_BASEAPP + 10,               //get cell
	MSGID_BASEAPP_CREATE_CELL_VIA_MYCELL = MSGTYPE_BASEAPP + 11,               //ͨ��һ����cell��base����������һ��base��cell
	MSGID_BASEAPP_CREATE_CELL_FAILED = MSGTYPE_BASEAPP + 12,               //����cellʧ�ܵĻص�����
	MSGID_BASEAPP_LOAD_ENTITIES_OF_TYPE = MSGTYPE_BASEAPP + 13,               //�ű������loadһ�����͵�����entity
	MSGID_BASEAPP_LOAD_ENTITIES_END_MSG = MSGTYPE_BASEAPP + 14,               //load entities�Ľ�����Ϣ��
	MSGID_BASEAPP_ON_LOSE_CELL = MSGTYPE_BASEAPP + 15,               //cell destroy֮��Ļص�����
	MSGID_BASEAPP_CREATE_BASE_ANYWHERE = MSGTYPE_BASEAPP + 16,               //
	MSGID_BASEAPP_SET_BASE_DATA = MSGTYPE_BASEAPP + 17,               //����ȫbase����
	MSGID_BASEAPP_DEL_BASE_DATA = MSGTYPE_BASEAPP + 18,               //ɾ��ȫbase����
	MSGID_BASEAPP_ON_REDIS_HASH_LOAD = MSGTYPE_BASEAPP + 19,               //redis_hash����load֮��Ļص�����
	MSGID_BASEAPP_CELL_ATTRI_SYNC = MSGTYPE_BASEAPP + 20,               //cell_and_client����ͨ��baseͬ�����ͻ���
	MSGID_BASEAPP_AVATAR_POS_SYNC = MSGTYPE_BASEAPP + 21,               //avatar�Լ���cell�ϵ�����ͬ����base
	MSGID_BASEAPP_DEL_ACCOUNT_CACHE_CALLBACK = MSGTYPE_BASEAPP + 22,               //֪ͨbasedbmgr�ϵ�account�����Ѿ�ɾ��

	MSGID_BASEAPP_ENTITY_RPC = MSGTYPE_BASEAPP + 80,               //�������˽��̷���Ļ���entity��rpc
	MSGID_BASEAPP_LUA_DEBUG = MSGTYPE_BASEAPP + 90,               //����lua�ű�
	MSGID_BASEAPP_ENTITY_ATTRI_SYNC = MSGTYPE_BASEAPP + 91,
	MSGID_BASEAPP_CLIENT_RPC_VIA_BASE = MSGTYPE_BASEAPP + 92,               //ͨ��baseת����client rpc����
	MSGID_BASEAPP_TIME_SAVE = MSGTYPE_BASEAPP + 93,               //��ʱ����
	MSGID_BASEAPP_CLIENT_MSG_VIA_BASE = MSGTYPE_BASEAPP + 94,               //ͨ��baseת����client��Ϣ��,ע����92������
	MSGID_BASEAPP_ITEMS_LOADING_CALLBACK = MSGTYPE_BASEAPP + 95,               //ָ��avatar��������װ�ػص�����
	MSGID_BASEAPP_UPDATE_ITEMS_CALLBACK = MSGTYPE_BASEAPP + 96,               //udpate items data �ص�����
	MSGID_BASEAPP_TABLE_SELECT_CALLBACK = MSGTYPE_BASEAPP + 97,               //TableSelect�Ļص�
	MSGID_BASEAPP_BROADCAST_CLIENT_PRC = MSGTYPE_BASEAPP + 98,               //ȫ���㲥��client rpc����
	MSGID_BASEAPP_TABLE_UPDATE_BATCH_CB = MSGTYPE_BASEAPP + 99,               //TableUpdateBatch�Ļص�
	MSGID_BASEAPP_TABLE_INSERT_CALLBACK = MSGTYPE_BASEAPP + 100,              //TableInsert�Ļص�
	//MSGID_BASEAPP_CLIENT_LOGIN              = MSGTYPE_BASEAPP + 100,
	//MSGID_BASEAPP_CLIENT_RPCALL             = MSGTYPE_BASEAPP + 101,              //�ͻ��˷����Զ�̵���
	MSGID_BASEAPP_TABLE_EXCUTE_CALLBACK = MSGTYPE_BASEAPP + 102,              //TableInsert�Ļص�
	MSGID_BASEAPP_INSERT_ITEMS_CALLBACK = MSGTYPE_BASEAPP + 103,              //insert item call back
	MSGID_BASEAPP_CROSSCLIENT_BROADCAST = MSGTYPE_BASEAPP + 104,                //broadcast from crossserver
	MSGID_BASEAPP_TABLE2EXCUTE_RESP = MSGTYPE_BASEAPP + 105,
	MSGID_BASEAPP_TABLE2SELECT_RESP = MSGTYPE_BASEAPP + 106,
	MSGID_BASEAPP_TABLE2INSERT_RESP = MSGTYPE_BASEAPP + 107,

	MSGID_CELLAPP_CREATE_CELL_IN_NEW_SPACE = MSGTYPE_CELLAPP + 1,
	MSGID_CELLAPP_ENTITY_RPC = MSGTYPE_CELLAPP + 2,
	MSGID_CELLAPP_CREATE_CELL_VIA_MYCELL = MSGTYPE_CELLAPP + 3,
	MSGID_CELLAPP_DESTROY_CELLENTITY = MSGTYPE_CELLAPP + 4,
	MSGID_CELLAPP_PICKLE_CLIENT_ATTRIS = MSGTYPE_CELLAPP + 5,                //BASE֪ͨcell�������client����
	MSGID_CELLAPP_PICKLE_AOI_ENTITIES = MSGTYPE_CELLAPP + 6,                //base֪ͨcell���aoi������entity
	MSGID_CELLAPP_CLIENT_MOVE_REQ = MSGTYPE_CELLAPP + 7,                //�ͻ��˷�����ƶ�
	MSGID_CELLAPP_LOSE_CLIENT = MSGTYPE_CELLAPP + 8,
	MSGID_CELLAPP_ON_TIME_MOVE = MSGTYPE_CELLAPP + 9,                //entity time move
	MSGID_CELLAPP_SET_VISIABLE = MSGTYPE_CELLAPP + 10,               //
	MSGID_CELLAPP_CLIENT_OTHERS_MOVE_REQ = MSGTYPE_CELLAPP + 11,               //�ͻ��˷��������ʵ���ƶ�(�����Ӷ����)


	MSGID_CELLAPP_LUA_DEBUG = MSGTYPE_CELLAPP + 90,               //����lua�ű�
	MSGID_CELLAPP_ENTITY_ATTRI_SYNC = MSGTYPE_CELLAPP + 91,


	MSGID_DBMGR_INSERT_ENTITY = MSGTYPE_DBMGR + 1,
	MSGID_DBMGR_UPDATE_ENTITY = MSGTYPE_DBMGR + 2,
	MSGID_DBMGR_DELETE_ENTITY = MSGTYPE_DBMGR + 3,
	MSGID_DBMGR_SELECT_ENTITY = MSGTYPE_DBMGR + 4,
	MSGID_DBMGR_SELECT_ACCOUNT = MSGTYPE_DBMGR + 5,
	MSGID_DBMGR_RAW_SELECT = MSGTYPE_DBMGR + 6,                    //ָ��sql����select
	MSGID_DBMGR_RAW_MODIFY = MSGTYPE_DBMGR + 7,                    //ָ��sql����DELETE/UPDATE��
	MSGID_DBMGR_RAW_MODIFY_NORESP = MSGTYPE_DBMGR + 8,                    //(����Ҫ���ؽ��)ָ��sql����DELETE/UPDATE��
	MSGID_DBMGR_CREATEBASE = MSGTYPE_DBMGR + 9,                    //
	MSGID_DBMGR_CREATEBASE_FROM_DBID = MSGTYPE_DBMGR + 10,                   //
	MSGID_DBMGR_CREATEBASE_FROM_NAME = MSGTYPE_DBMGR + 11,                   //
	MSGID_DBMGR_LOAD_ALL_AVATAR = MSGTYPE_DBMGR + 12,
	MSGID_DBMGR_LOAD_ENTITIES_OF_TYPE = MSGTYPE_DBMGR + 13,
	MSGID_DBMGR_SHUTDOWN_SERVER = MSGTYPE_DBMGR + 14,
	MSGID_DBMGR_REDIS_HASH_LOAD = MSGTYPE_DBMGR + 15,
	MSGID_DBMGR_REDIS_HASH_SET = MSGTYPE_DBMGR + 16,
	MSGID_DBMGR_REDIS_HASH_DEL = MSGTYPE_DBMGR + 17,
	MSGID_DBMGR_UPDATE_ENTITY_REDIS = MSGTYPE_DBMGR + 18,
	MSGID_DBMGR_DEL_ACCOUNT_CACHE = MSGTYPE_DBMGR + 19,                   //ɾ���˺Ż��棺������Ϣ
	MSGID_DBMGR_UPDATE_ITEMS = MSGTYPE_DBMGR + 20,                   //�����������ݸ���
	MSGID_DBMGR_LOADING_ITEMS = MSGTYPE_DBMGR + 21,                   //��������װ��
	MSGID_DBMGR_TABLE_SELECT = MSGTYPE_DBMGR + 22,                   //��ѯĳ�ű�
	MSGID_DBMGR_UPDATE_BATCH = MSGTYPE_DBMGR + 23,                   //����ĳ���ֶ��������±�����
	MSGID_DBMGR_TABLE_INSERT = MSGTYPE_DBMGR + 24,                   //����ĳ�ű�
	MSGID_DBMGR_TABLE_EXCUTE = MSGTYPE_DBMGR + 25,                   //ִ��ĳ��sql
	MSGID_DBMGR_INCREMENTAL_UPDATE_ITEMS = MSGTYPE_DBMGR + 26,                   //ִ�е��߸��²���
	MSGID_DBMGR_TABLE2_SELECT = MSGTYPE_DBMGR + 27,
	MSGID_DBMGR_TABLE2_INSERT = MSGTYPE_DBMGR + 28,
	MSGID_DBMGR_TABLE2_EXCUTE = MSGTYPE_DBMGR + 29,


	MSGID_LOG_INSERT = MSGTYPE_LOG + 1,  //��־����
	MSGID_OTHER_HTTP_REQ = MSGTYPE_LOG + 2,  //http����
	MSGID_OTHER_ADD_GLOBALBASE = MSGTYPE_LOG + 3,  //��global��mailboxע�ᵽother����
	MSGID_OTHER_YUNYING_API = MSGTYPE_LOG + 4,  //��Ӫ֧�Žӿ�
	MSGID_OTHER_CLIENT_RESPONSE = MSGTYPE_LOG + 5,  //��Ӫ֧�Žӿڷ���

	MSGID_OTHER_LOGIN_VERIFY = MSGTYPE_LOG + 10, //sdk��������֤
	MSGID_OTHER_PLAT_API = MSGTYPE_LOG + 11, //ƽ̨�ӿ�

	MSGID_CROSSCLIENT_SERVER_RPC_PROXY = MSGTYPE_CROSSCLIENT + 1,              //�ڲ����̷���crossclientת����serverRpc
	MSGID_CROSSCLIENT_RPC = MSGTYPE_CROSSCLIENT + 2,
	MSGID_CROSSCLIENT_RESP = MSGTYPE_CROSSCLIENT + 3,
	MSGID_CROSSCLIENT_BROADCAST = MSGTYPE_CROSSCLIENT + 4,

	MSGID_CROSSSERVER_RPC = MSGTYPE_CROSSSERVER + 1,
	MSGID_CROSSSERVER_CLIENT_RPC_PROXY = MSGTYPE_CROSSSERVER + 2,
	MSGID_CROSSSERVER_REGISTER_SERVER = MSGTYPE_CROSSSERVER + 3,
	MSGID_CROSSSERVER_CLIENT_RESP_PROXY = MSGTYPE_CROSSSERVER + 4,
	MSGID_CROSSSERVER_CLIENT_BC_PROXY = MSGTYPE_CROSSSERVER + 5,
	MSGID_CROSSSERVER_CHECK_MD5 = MSGTYPE_CROSSSERVER + 6,

	};


	enum
	{
		MAILBOX_CLIENT_UNAUTHZ = 0,         //�����ڿͻ��˵�����,δ��֤
		MAILBOX_CLIENT_AUTHZ = 1,           //�����ڿͻ��˵�����,����֤
		MAILBOX_CLIENT_TRUSTED = 0xf,       //�����ڷ������˵Ŀ���������
	};

	enum EFDTYPE
	{
		FD_TYPE_ERROR = 0,
		FD_TYPE_SERVER = 1,
		FD_TYPE_MAILBOX = 2,
		FD_TYPE_ACCEPT = 3,
	};

	enum ERPCERR
	{
		ERR_RPC_UNKNOWN_MSGID = -99,   //δ֪msgid
		ERR_RPC_DECODE = -98,   //�������
		ERR_RPC_LOGIC = -97,   //�߼�����
	};

	//���rpc����Ƿ����
#define CHECK_RPC_DECODE_ERR(u) \
        {\
            if(u.GetDecodeErrIdx()>0)\
            {\
                return ERR_RPC_DECODE;\
            }\
        }

	//��鲢��ȡrpc�е�һ���ֶ�
#define CHECK_AND_GET_RPC_FIELD(u, field_var, field_type) \
        field_type field_var;\
        u >> field_var;\
        if(u.GetDecodeErrIdx()>0)\
        {\
            return ERR_RPC_DECODE;\
        }


	//��鲢��ȡrpc�е�һ��c_str�ֶ�
#define CHECK_AND_GET_RPC_FIELD_CSTR(u, field_var) \
        string _tmp_##field_var;\
        u >> _tmp_##field_var;\
        if(u.GetDecodeErrIdx()>0)\
        {\
            return ERR_RPC_DECODE;\
        }\
        const char* field_var = _tmp_##field_var.c_str();


	//��鲢��ȡrpc�е�һ��entity���Դ���ֶ�
#define CHECK_AND_GET_RPC_FIELD_PROPS(u, field_var) \
        VOBJECT _tmp_##field_var;\
        _tmp_##field_var.vt = V_ENTITY;\
        _tmp_##field_var.vv.p = new SEntityPropFromPluto;\
        u.UnpickleEntity(_tmp_##field_var);\
        CHECK_RPC_DECODE_ERR(u);\
        SEntityPropFromPluto* field_var = (SEntityPropFromPluto*)(_tmp_##field_var.vv.p);



	class CRpcUtil
	{
	public:
		CRpcUtil();
		~CRpcUtil();

	private:
		//��ʼ����Ƕ(���Զ���)�ķ���
		void InitInnerMethods();

	public:
		template<typename T1>
		void Encode(CPluto& u, pluto_msgid_t msg_id, const T1& p1);
		template<typename T1, typename T2>
		void Encode(CPluto& u, pluto_msgid_t msg_id, const T1& p1, const T2& p2);
		template<typename T1, typename T2, typename T3>
		void Encode(CPluto& u, pluto_msgid_t msg_id, const T1& p1, const T2& p2, const T3& p3);
		template<typename T1, typename T2, typename T3, typename T4>
		void Encode(CPluto& u, pluto_msgid_t msg_id, const T1& p1, const T2& p2, const T3& p3, const T4& p4);
		template<typename T1, typename T2, typename T3, typename T4, typename T5>
		void Encode(CPluto& u, pluto_msgid_t msg_id, const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5);
		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
		void Encode(CPluto& u, pluto_msgid_t msg_id, const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5, const T6& p6);

	public:
		_SEntityDefMethods* GetDef(pluto_msgid_t msgid);

	public:
		T_VECTOR_OBJECT* Decode(CPluto& u);

	private:
		//����ͨ�ø�ʽ(����������m_method��)��rpc����
		T_VECTOR_OBJECT* DecodeGeneralRpc(CPluto& u);
		T_VECTOR_OBJECT* DecodeBaseEntityRpc(CPluto& u);
		T_VECTOR_OBJECT* DecodeCellEntityRpc(CPluto& u);
		T_VECTOR_OBJECT* DecodeBaseClientRpc(CPluto& u);
		T_VECTOR_OBJECT* DecodeEntityAttriSync(CPluto& u);
		T_VECTOR_OBJECT* DecodeClientMoveReq(CPluto& u);                 //�����ͻ����ƶ���
		T_VECTOR_OBJECT* DecodeBroadcastClientRpc(CPluto& u);
		T_VECTOR_OBJECT* DecodeClientOthersMoveReq(CPluto& u);           //�����ͻ��˶�����ʵ����ƶ���

	public:
		//��pluto��������entity�����ֶ��б�
		bool UnpickleEntityDataFromPluto(CPluto& u, TDBID& dbid, map<string, VOBJECT*> data);

	private:
		map<pluto_msgid_t, _SEntityDefMethods*> m_methods;
		string m_strFuncNameNotfound;

	};

	template<typename T1>
	void CRpcUtil::Encode(CPluto& u, pluto_msgid_t msg_id, const T1& p1)
	{
		//map<pluto_msgid_t, _SEntityDefMethods*>::const_iterator iter = m_methods.find(msg_id);
		//if(iter == m_methods.end())
		//{
		//    LogError("CRpcUtil::encode error", "msg_id=%d", msg_id);
		//    return;
		//}

		u.Encode(msg_id) << p1 << EndPluto;
	}

	template<typename T1, typename T2>
	void CRpcUtil::Encode(CPluto& u, pluto_msgid_t msg_id, const T1& p1, const T2& p2)
	{
		//map<pluto_msgid_t, _SEntityDefMethods*>::const_iterator iter = m_methods.find(msg_id);
		//if(iter == m_methods.end())
		//{
		//    LogError("CRpcUtil::encode error", "msg_id=%d", msg_id);
		//    return;
		//}

		u.Encode(msg_id) << p1 << p2 << EndPluto;
	}

	template<typename T1, typename T2, typename T3>
	void CRpcUtil::Encode(CPluto& u, pluto_msgid_t msg_id, const T1& p1, const T2& p2, const T3& p3)
	{
		//map<pluto_msgid_t, _SEntityDefMethods*>::const_iterator iter = m_methods.find(msg_id);
		//if(iter == m_methods.end())
		//{
		//    LogError("CRpcUtil::encode error", "msg_id=%d", msg_id);
		//    return;
		//}

		u.Encode(msg_id) << p1 << p2 << p3 << EndPluto;
	}

	template<typename T1, typename T2, typename T3, typename T4>
	void CRpcUtil::Encode(CPluto& u, pluto_msgid_t msg_id, const T1& p1, const T2& p2, const T3& p3, const T4& p4)
	{
		//map<pluto_msgid_t, _SEntityDefMethods*>::const_iterator iter = m_methods.find(msg_id);
		//if(iter == m_methods.end())
		//{
		//    LogError("CRpcUtil::encode error", "msg_id=%d", msg_id);
		//    return;
		//}

		u.Encode(msg_id) << p1 << p2 << p3 << p4 << EndPluto;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5>
	void CRpcUtil::Encode(CPluto& u, pluto_msgid_t msg_id, const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5)
	{
		//map<pluto_msgid_t, _SEntityDefMethods*>::const_iterator iter = m_methods.find(msg_id);
		//if(iter == m_methods.end())
		//{
		//    LogError("CRpcUtil::encode error", "msg_id=%d", msg_id);
		//    return;
		//}

		u.Encode(msg_id) << p1 << p2 << p3 << p4 << p5 << EndPluto;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	void CRpcUtil::Encode(CPluto& u, pluto_msgid_t msg_id, const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5, const T6& p6)
	{
		//map<pluto_msgid_t, _SEntityDefMethods*>::const_iterator iter = m_methods.find(msg_id);
		//if(iter == m_methods.end())
		//{
		//    LogError("CRpcUtil::encode error", "msg_id=%d", msg_id);
		//    return;
		//}

		u.Encode(msg_id) << p1 << p2 << p3 << p4 << p5 << p6 << EndPluto;
	}

}



#endif

