#ifndef __WORLD_BASE__
#define __WORLD_BASE__

#include "world.h"
#include "entity_base.h"
#include "entity_mgr.h"

namespace mogo {

	class CWorldBase :public world {
	public:
		CWorldBase();
		~CWorldBase();

		int FromRpcCall(CPluto& u);

		int OnClientMoveReq(CPluto& u);

		CEntityBase* GetEntityByFd(int fd);
		CEntityParent* GetEntity(TENTITYID id);
	private:
		CEntityMgr<CEntityBase> m_enMgr;
		map<int, TENTITYID> m_fd2Entity;
	};

}
#endif