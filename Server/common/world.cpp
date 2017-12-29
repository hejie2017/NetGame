#include "world.h"


namespace mogo {

	CMailBoxManager::CMailBoxManager()
	{

	}

	CMailBoxManager::~CMailBoxManager()
	{
		//ClearContainer(m_mbs);
	}

	bool CMailBoxManager::init(CCfgReader& cfg)
	{
		//int nServerCount = atoi(cfg.GetValue("server_all", "server_count").c_str()) + 1;
		int nServerCount = SERVER_MAILBOX_RESERVE_SIZE;     //不读配置文件了,直接取这个最大值
		for (int i = 1; i < nServerCount; ++i)
		{
			char szServer[16];
			memset(szServer, 0, sizeof(szServer));
			snprintf(szServer, sizeof(szServer), "server_%d", i);

			const string& strServerType = cfg.GetOptValue(szServer, "type", "");
			if (strServerType.empty())
			{
				//LogWarning("init_cfg", "warning:missed section:%s", szServer);
				continue;
			}

			int nServerType;
			if (strServerType.compare("cwmd") == 0)
			{
				nServerType = SERVER_BASEAPPMGR;
			}
			else if (strServerType.compare("baseapp") == 0)
			{
				nServerType = SERVER_BASEAPP;
			}
			else if (strServerType.compare("cellapp") == 0)
			{
				nServerType = SERVER_CELLAPP;
			}
			else if (strServerType.compare("dbmgr") == 0)
			{
				nServerType = SERVER_DBMGR;
			}
			else if (strServerType.compare("timerd") == 0)
			{
				nServerType = SERVER_TIMERD;
			}
			else if (strServerType.compare("loginapp") == 0)
			{
				nServerType = SERVER_LOGINAPP;
			}
			else if (strServerType.compare("logapp") == 0)
			{
				nServerType = SERVER_LOG;
			}
			else if (strServerType.compare("client") == 0)
			{
				nServerType = SERVER_CLIENT;
			}
			else if (strServerType.compare("crossclient") == 0)
			{
				nServerType = SERVER_CROSSCLIENT;
			}
			else if (strServerType.compare("crossserver") == 0)
			{
				nServerType = SERVER_CROSSSERVER;
			}
			else
			{
				//ThrowException(-1, "unknown server type:%s", strServerType.c_str());
			}

			if (nServerType != SERVER_BASEAPP && nServerType != SERVER_CELLAPP)
			{
				//baseapp和cellapp可以配置多个,其他类型的服务器id必须和服务器类型编号一致
				if (nServerType != i)
				{
					//ThrowException(-1, "%s, server_id error,need=%d,set=%d", strServerType.c_str(), nServerType, i);
				}
			}
			else
			{
				//baseapp/cellapp的编号必须大于最小编号
				if (i < SERVER_MULTI_MIN_ID)
				{
					//ThrowException(-1, "%s, server_id=%d < min=%d", strServerType.c_str(), i, SERVER_MULTI_MIN_ID);
				}
			}

			CMailBox* mb = new CMailBox(i, FD_TYPE_MAILBOX, cfg.GetValue(szServer, "ip").c_str(), \
				atoi(cfg.GetValue(szServer, "port").c_str()));
			mb->SetServerMbType(nServerType);
			m_mbs.push_back(mb);
		}

		return true;
	}

	////////////////////

	world::world()
	{

	}

	world::~world()
	{

	}

	int world::init(const char* pszEtcFile)
	{
		m_cfg = new CCfgReader(pszEtcFile);
		InitMailboxMgr();
		return 0;
	}

	void world::InitMailboxMgr()
	{
		m_mbMgr.init(*m_cfg);
	}

	int world::OnTimerdTick(T_VECTOR_OBJECT* p)
	{
		if (p->size() != 1)
		{
			return -1;
		}

		uint32_t unTick = VOBJECT_GET_U32((*p)[0]);
		//m_timer.OnTick(m_L, *this, unTick);

		//LogDebug("world::OnTimerdTick", "");

		return 0;
	}

	CEpollServer* world::GetServer()
	{
		return the_server;
	}

	void world::SetServer(CEpollServer* server) 
	{
		the_server = server;
	}

	CMailBox* world::GetServerMailbox(uint16_t nServerId)
	{
		CEpollServer* s = GetServer();
		if (s)
		{
			return s->GetServerMailbox(nServerId);
		}

		return NULL;
	}

	//检查一个rpc调用是否合法
	bool world::CheckClientRpc(CPluto& u)
	{
		CMailBox* mb = u.GetMailbox();
		if (!mb)
		{
			//如果没有mb,是从本进程发来的包
			return true;
		}
		if (mb->IsDelete())
		{
			//已标记del的mb,其所有的包不再处理
			return false;
		}
		uint8_t authz = mb->GetAuthz();
		if (authz == MAILBOX_CLIENT_TRUSTED)
		{
			return true;
		}
		else if (authz == MAILBOX_CLIENT_AUTHZ)
		{
			pluto_msgid_t msg_id = u.GetMsgId();
			return msg_id == MSGID_BASEAPP_CLIENT_RPCALL ||
				msg_id == MSGID_BASEAPP_CLIENT_MOVE_REQ ||
				msg_id == MSGID_BASEAPP_CLIENT_RPC2CELL_VIA_BASE ||
				msg_id == MSGID_BASEAPP_CLIENT_OTHERS_MOVE_REQ;
		}
		else if (authz == MAILBOX_CLIENT_UNAUTHZ)
		{
			pluto_msgid_t msg_id = u.GetMsgId();
#ifdef __RELOGIN
			return msg_id == MSGID_LOGINAPP_LOGIN || msg_id == MSGID_BASEAPP_CLIENT_LOGIN || msg_id == MSGID_BASEAPP_CLIENT_RELOGIN;
#else
			return msg_id == MSGID_LOGINAPP_LOGIN || msg_id == MSGID_BASEAPP_CLIENT_LOGIN;
#endif
		}
		else
		{
			return false;
		}
	}

}