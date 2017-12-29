#ifndef __ENTITY_BASE_HEAD__
#define __ENTITY_BASE_HEAD__

#include "entity.h"
#include "memory_pool.h"

namespace mogo
{

	enum ECELLSTATE
	{
		E_HASNT_CELL = 0,     //no cell
		E_CELL_IN_CREATING = 1,     //cell is  creating
		E_CELL_CREATED = 2,     //cell has created
		E_CELL_IN_DESTROYING = 3,   //cell is destroying
	};
	class CEntityBase : public CEntityParent
	{
	public:
		CEntityBase(TENTITYTYPE etype, TENTITYID nid);
		~CEntityBase();

		int GetCellServerId();
		int GetClientFd();

	public:
		inline void SetClientFd(int32_t fd)
		{
			this->m_ClientFd = fd;
		}
	private:
		uint16_t m_nCellSvrId;
		ECELLSTATE m_nCellState;

		//base的entity，当它没有cell部分时，会把cell的属性存到这里
		map<string, VOBJECT*> m_cellData;

		int32_t m_ClientFd;
	};
}

#endif