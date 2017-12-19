#ifndef __WORLD_SELECT_HEAD__
#define __WORLD_SELECT_HEAD__

#include "../base/world_base.h"
#include "../cell/world_cell.h"

extern world* g_pTheWorld;

inline world* GetWorld()
{
	return g_pTheWorld;
}

inline CWorldBase& GetWorldbase()
{
	return (CWorldBase&)*g_pTheWorld;
}

inline CWorldCell& GetWorldcell()
{
	return (CWorldCell&)*g_pTheWorld;
}


#endif
