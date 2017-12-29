#include "world_base.h"
#include "world_select.h"
#include "entity_base.h"

namespace mogo {

	CWorldBase::CWorldBase() :world() {

	}
	CWorldBase::~CWorldBase() {

	}

	bool CWorldBase::AddEntity(CEntityBase* p)
	{
		return m_enMgr.AddEntity(p);
	}

	CEntityParent* CWorldBase::GetEntity(TENTITYID id)
	{
		return (CEntityParent*)m_enMgr.GetEntity(id);
	}

	int CWorldBase::FromRpcCall(CPluto& u)
	{
		pluto_msgid_t msg_id = u.GetMsgId();
		//if (!CheckClientRpc(u))
		//{
		//	//LogWarning("from_rpc_call", "invalid rpcall error.unknown msgid:%d\n", msg_id);
		//	return -1;
		//}

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
			case MSGID_LOGINAPP_LOGIN:
			{
				int fd = u.GetMailbox()->GetFd();
				AddClientFdToVObjectList(fd, p);
				nRet = ClientLogin(p,fd);
				if (nRet != 0)
				{
					GetServer()->CloseFdFromServer(fd);
				}
				break;
			}
			case MSGID_BASEAPP_AVATAR_POS_SYNC:
			{
				nRet = OnAvatarPosSync(p);
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
		/*CEntityBase* p = GetEntityByFd(fd);
		if (p == NULL)
		{
			return -2;
		}

		int nCellId = p->GetCellServerId();*/
		int nCellId = 2;
		if (nCellId > 0)
		{
			CMailBox* mb = GetServerMailbox(nCellId);
			if (mb)
			{
				CPluto* u2 = new CPluto;
				//(*u2).Encode(MSGID_CELLAPP_CLIENT_MOVE_REQ) << p->GetId();
				(*u2).Encode(MSGID_CELLAPP_CLIENT_MOVE_REQ) << 2;
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

int CWorldBase::OnAvatarPosSync(T_VECTOR_OBJECT* p)
{
#ifdef __FACE
	if (p->size() != 6)
	{
		return -1;
	}
#else
	if (p->size() != 5)
	{
		return -1;
	}
#endif

	pluto_msgid_t msg_id = VOBJECT_GET_U16((*p)[0]);
	TENTITYID eid = VOBJECT_GET_U32((*p)[1]);
#ifdef __FACE
	uint8_t face = VOBJECT_GET_U8((*p)[2]);
	int16_t x = VOBJECT_GET_I16((*p)[3]);
	int16_t y = VOBJECT_GET_I16((*p)[4]);
	uint8_t notifyToClient = VOBJECT_GET_U8((*p)[5]);
#else
	int16_t x = VOBJECT_GET_I16((*p)[2]);
	int16_t y = VOBJECT_GET_I16((*p)[3]);
	uint8_t notifyToClient = VOBJECT_GET_U8((*p)[4]);
#endif

	CEntityBase* pBase = (CEntityBase*)GetEntity(1);
	if (pBase)
	{
		//LogDebug("CWorldBase::OnAvatarPosSync", "eid=%d;msg_id=%d;face=%d;x=%d;y=%d", eid, msg_id, face, x, y);

		//x,y字段设置到entity里
		//pBase->SetMapXY(x, y);

		if (notifyToClient == 1)
		{
			//同步给客户端
			CMailBox* mb = GetServer()->GetClientMailbox(pBase->GetClientFd());
			if (mb)
			{
				CPluto* u = new CPluto;
#ifdef __FACE
				(*u).Encode(msg_id) << face << x << y << EndPluto;
#else
				(*u).Encode(msg_id) << x << y << EndPluto;
				printf("%d ", x);
#endif
				mb->PushPluto(u);
			}
		}

	}

	return 0;
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

//将客户端的socket fd附加到pluto解包出来的数据结构上
void world::AddClientFdToVObjectList(int fd, T_VECTOR_OBJECT* p)
{
	VOBJECT* v = new VOBJECT;
	v->vt = V_INT32;
	v->vv.i32 = (int32_t)fd;
	p->push_back(v);
}

int CWorldBase::ClientLogin(T_VECTOR_OBJECT* p,int fd)
{

	//CEntityBase* pBase = (CEntityBase*)GetEntity(1);
	
	CEntityBase* pEntitybase = new CEntityBase(1, 1);
	GetWorldbase().AddEntity(pEntitybase);
	pEntitybase->SetClientFd(fd);
	////加入map
	//m_fd2Entity.insert(make_pair(fd, pBase->GetId()));


	return 0;
}