#include "entity_base.h"
#include "world_select.h"

namespace mogo {

	CEntityBase::CEntityBase(TENTITYTYPE etype, TENTITYID nid)
		: CEntityParent(etype, nid), m_nCellState(E_HASNT_CELL), m_nCellSvrId(0), m_ClientFd(-1)
	{
		m_bIsBase = true;
		//LogInfo("CEntityBase::CEntityBase", "id=%d", nid);
	}

	CEntityBase::~CEntityBase()
	{
	}

	int CEntityBase::GetCellServerId()
	{
		if (m_nCellState != E_CELL_CREATED)
		{
			return -1;
		}

		if (m_nCellSvrId == 0)
		{
			return -2;
		}

		return m_nCellSvrId;
	}

	int CEntityBase::GetClientFd()
	{
		//map<string, VOBJECT*>::const_iterator iter = m_data.find("client");
		//if (iter == m_data.end())
		//{
		//	return -1;
		//}

		//return m_ClientFd;

		//CWorldBase the_world = GetWorldbase();
		//the_world.GetServer()->GetClientMailbox();

		//int nRef = iter->second->vv.i32;
		//world* the_world = GetWorld();
		//lua_State* L = the_world->GetLuaState();
		//int m = the_world->GetLuaTables().GetObj(L, nRef);

		//lua_rawgeti(L, -1, g_nMailBoxServerIdKey);

		//if(lua_isnumber(L, -1))
		//{
		//    int nServerId = (int)lua_tonumber(L, -1);
		//    lua_pop(L, m + 1);
		//    return nServerId;
		//}

		//lua_pop(L, m + 1);
		//return -3;

		return m_ClientFd;
	}

}