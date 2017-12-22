#include "world_base.h"

namespace mogo {

	CWorldBase::CWorldBase() :world() {

	}
	CWorldBase::~CWorldBase() {

	}

	CEntityParent* CWorldBase::GetEntity(TENTITYID id)
	{
		return (CEntityParent*)m_enMgr.GetEntity(id);
	}

	int CWorldBase::FromRpcCall(CPluto& u)
	{
		pluto_msgid_t msg_id = u.GetMsgId();
		if (!CheckClientRpc(u))
		{
			//LogWarning("from_rpc_call", "invalid rpcall error.unknown msgid:%d\n", msg_id);
			return -1;
		}

		if (msg_id == MSGID_BASEAPP_CLIENT_MOVE_REQ)
		{
			//转发给cell的客户端移动方法
			return OnClientMoveReq(u);
		}
		else if (msg_id == MSGID_BASEAPP_CLIENT_RPC2CELL_VIA_BASE)
		{
			//通过base转发的到cell的rpc调用
			//return FromClientRpc2CellViaBase(u);
		}

		T_VECTOR_OBJECT* p = m_rpc.Decode(u);
		if (p == NULL)
		{
			//LogWarning("from_rpc_call", "rpc Decode error.unknown msgid:%d\n", msg_id);
			return -1;
		}
		if (u.GetDecodeErrIdx() > 0)
		{
			ClearTListObject(p);
			//LogWarning("from_rpc_call", "rpc Decode error.msgid:%d;pluto err idx=%d\n", msg_id, u.GetDecodeErrIdx());
			return -2;
		}

		int nRet = -1;
		switch (msg_id)
		{
			case MSGID_ALLAPP_ONTICK:
			{
				nRet = OnTimerdTick(p);
				break;
			}
			default:
			{
				//LogWarning("from_rpc_call", "unknown msgid:%d\n", msg_id);
			}
		}

		if (nRet != 0)
		{
			//LogWarning("from_rpc_call", "rpc error.msg_id=%d;ret=%d\n", msg_id, nRet);
		}

		ClearTListObject(p);

		return 0;
	}

	//处理和转发客户端的移动请求
	int CWorldBase::OnClientMoveReq(CPluto& u)
	{
		CMailBox* mb = u.GetMailbox();
		if (mb == NULL)
		{
			return -1;
		}
		int fd = mb->GetFd();
		CEntityBase* p = GetEntityByFd(fd);
		if (p == NULL)
		{
			return -2;
		}

		int nCellId = p->GetCellServerId();
		if (nCellId > 0)
		{
			CMailBox* mb = GetServerMailbox(nCellId);
			if (mb)
			{
				CPluto* u2 = new CPluto;
				(*u2).Encode(MSGID_CELLAPP_CLIENT_MOVE_REQ) << p->GetId();

				u.Decode();
				int nCopyLen = (int)(u.GetMaxLen() - u.GetLen());
				if (nCopyLen > 0)
				{
					u2->FillBuff(u.GetBuff() + u.GetLen(), nCopyLen);
					(*u2) << EndPluto;
					mb->PushPluto(u2);

					return 0;
				}
				else
				{
					delete u2;
				}
			}
		}

		return -3;
	}
}

//获取一个fd上关联的entity
CEntityBase* CWorldBase::GetEntityByFd(int fd)
{
	map<int, TENTITYID>::iterator iter = m_fd2Entity.find(fd);
	if (iter != m_fd2Entity.end())
	{
		return (CEntityBase*)GetEntity(iter->second);
	}

	return NULL;
}