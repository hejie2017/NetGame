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
		int OnAvatarPosSync(T_VECTOR_OBJECT* p);

		CEntityBase* GetEntityByFd(int fd);
		bool AddEntity(CEntityBase* p);
		CEntityParent* GetEntity(TENTITYID id);
		int ClientLogin(T_VECTOR_OBJECT* p,int fd);
	private:
		CEntityMgr<CEntityBase> m_enMgr;
		map<int, TENTITYID> m_fd2Entity;
	};

}
#endif