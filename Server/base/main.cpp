#include "base_server.h"
#include "world_base.h"
#ifdef __linux__
const char* ipstr = "202.168.133.195";
#else
const char* ipstr = "202.168.133.150";
#endif

world* g_pTheWorld = new CWorldBase;
int main()
{
	CBaseServer* server = new CBaseServer;

	server->SetWorld(g_pTheWorld);
	server->Service(ipstr, 8889);

	return 0;
}
