#include "world_cell.h"
#include "rpc_mogo.h"
#include "world_select.h"

namespace mogo {

	CWorldCell::CWorldCell() :world() {
		CreateCellInNewSpace(NULL);
	}
	CWorldCell::~CWorldCell() {

	}

	int CWorldCell::FromRpcCall(CPluto& u)
	{
		pluto_msgid_t msg_id = u.GetMsgId();
		/*if (!CheckClientRpc(u))
		{
			return -1;
		}*/

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
		case MSGID_CELLAPP_CREATE_CELL_IN_NEW_SPACE:
		{
			nRet = CreateCellInNewSpace(p);
			break;
		}

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
		case MSGID_CELLAPP_CLIENT_MOVE_REQ:
		{
			nRet = OnClientMoveReq(p);
			break;
		}
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

	int CWorldCell::OnClientMoveReq(T_VECTOR_OBJECT* p)
	{
		//printf("CWorldCell::on_client_move_req\n");
#ifdef __FACE
		if (p->size() < 4)
		{
			return -1;
		}
#else
		if (p->size() < 3)
		{
			return -1;
		}
#endif
		TENTITYID eid = VOBJECT_GET_U32((*p)[0]);
		CEntityCell* pCell = (CEntityCell*)GetEntity(eid);
		if (pCell)
		{

#ifdef __FACE
			uint8_t face = VOBJECT_GET_U8((*p)[1]);
			int16_t x = VOBJECT_GET_I16((*p)[2]);
			int16_t y = VOBJECT_GET_I16((*p)[3]);
#else
			int16_t x = VOBJECT_GET_I16((*p)[1]);
			int16_t y = VOBJECT_GET_I16((*p)[2]);
#endif


#ifdef __FACE
			pCell->OnClientMoveReq(face, x, y);
#else
			printf("%d ", x);
			pCell->OnClientMoveReq(x, y);
#endif

		}

		return 0;
	}

	bool CWorldCell::AddEntity(CEntityCell* p)
	{
		return m_enMgr.AddEntity(p);
	}

	CEntityParent* CWorldCell::GetEntity(TENTITYID id)
	{
		return (CEntityParent*)m_enMgr.GetEntity(id);
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

	int CWorldCell::CreateCellInNewSpace(T_VECTOR_OBJECT* p)
	{
		//TENTITYTYPE entity_type = GetWorld()->GetDefParser().GetTypeId(pszTypeName);

		CEntityCell* e = new CEntityCell(2,2);
		this->AddEntity(e);

		return 0;
	}

}