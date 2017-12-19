#ifndef __WORLD_CELL__
#define __WORLD_CELL__

#include "world.h"
#include "space.h"

namespace mogo {
	class CWorldCell :public world {
	public:
		CWorldCell() {}
		~CWorldCell() {}

		int FromRpcCall(CPluto& u);

		int OnTimeMove(T_VECTOR_OBJECT* p);
		CSpace* GetSpace(TSPACEID id);

	public:
		inline uint16_t GetMaxObserverCount()
		{
			return this->m_nMaxObserverCount;
		}

		inline uint16_t GetMaxFollowerCount()
		{
			return this->m_nMaxFollowerCount;
		}

	private:
		map<TSPACEID, CSpace*> m_spaces;

		map<TSPACEID, CSpace*> m_idleSpaces;

		CEntityMgr<CEntityCell> m_enMgr;
		TSPACEID m_nSpaceSeq;

		uint8_t moveTimes;

		uint16_t m_nMaxObserverCount;
		uint16_t m_nMaxFollowerCount;
	};

}
#endif