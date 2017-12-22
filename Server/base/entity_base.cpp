#include "entity_base.h"

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

}