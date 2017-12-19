#include "world.h"


namespace mogo {

	world::world()
	{

	}

	world::~world()
	{

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