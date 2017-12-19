#include "world_cell.h"
#include "rpc_mogo.h"

namespace mogo {

	int CWorldCell::FromRpcCall(CPluto& u)
	{
		pluto_msgid_t msg_id = u.GetMsgId();
		if (!CheckClientRpc(u))
		{
			return -1;
		}

		T_VECTOR_OBJECT* p = m_rpc.Decode(u);
		if (p == NULL)
		{

			return -1;
		}
		if (u.GetDecodeErrIdx() > 0)
		{
			ClearTListObject(p);

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
		//case MSGID_CELLAPP_CREATE_CELL_IN_NEW_SPACE:
		//{
		//	nRet = CreateCellInNewSpace(p);
		//	break;
		//}

		//case MSGID_CELLAPP_CREATE_CELL_VIA_MYCELL:
		//{
		//	nRet = CreateCellViaMycell(p);
		//	break;
		//}
		//case MSGID_CELLAPP_ENTITY_ATTRI_SYNC:
		//{
		//	nRet = OnEntityAttriSync(p);
		//	break;
		//}
		//case MSGID_CELLAPP_DESTROY_CELLENTITY:
		//{
		//	nRet = DestroyCellEntity(p);
		//	break;
		//}
		//case MSGID_ALLAPP_SHUTDOWN_SERVER:
		//{
		//	nRet = ShutdownServer(p);
		//	break;
		//}
		//case MSGID_CELLAPP_PICKLE_CLIENT_ATTRIS:
		//{
		//	nRet = PickleClientAttris(p);
		//	break;
		//}
		//case MSGID_CELLAPP_PICKLE_AOI_ENTITIES:
		//{
		//	nRet = PickleAoiEntities(p);
		//	break;
		//}
		//case MSGID_CELLAPP_CLIENT_MOVE_REQ:
		//{
		//	nRet = OnClientMoveReq(p);
		//	break;
		//}
		//case MSGID_CELLAPP_CLIENT_OTHERS_MOVE_REQ:
		//{
		//	nRet = OnClientOthersMoveReq(p);
		//	break;
		//}
		//case MSGID_CELLAPP_LOSE_CLIENT:
		//{
		//	nRet = OnLoseClient(p);
		//	break;
		//}
		//case MSGID_CELLAPP_ON_TIME_MOVE:
		//{
		//	nRet = OnTimeMove(p);
		//	break;
		//}
		//case MSGID_CELLAPP_SET_VISIABLE:
		//{
		//	nRet = SetCellVisiable(p);
		//	break;
		//}
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

	int CWorldCell::OnTimeMove(T_VECTOR_OBJECT* p)//每0.1秒进一次
	{

		//触发进入aoi事件
		int nAoiEventCount = 0;
		map<TSPACEID, CSpace*>::iterator iter2 = m_spaces.begin();
		for (; iter2 != m_spaces.end(); ++iter2)
		{
			CSpace* sp = iter2->second;

			sp->AllEntitiesMove();

			if (sp->IsAoiDirty())
			{
				sp->AoiEvent();
				++nAoiEventCount;
				sp->AllEntitiesCheckLeaveAoi();
			}


		}

		return 0;
	}

	CSpace* CWorldCell::GetSpace(TSPACEID id)
	{
		map<TSPACEID, CSpace*>::const_iterator iter = m_spaces.find(id);
		if (iter != m_spaces.end())
		{
			return iter->second;
		}
		else
		{
			map<TSPACEID, CSpace*>::const_iterator iter1 = m_idleSpaces.find(id);
			if (iter1 != m_idleSpaces.end())
			{
				return iter1->second;
			}
			else
			{
				return NULL;
			}
		}
	}

}