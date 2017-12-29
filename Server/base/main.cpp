#include "base_server.h"
#include "world_base.h"
#ifdef __linux__
const char* ipstr = "202.168.133.195";
#else
const char* ipstr = "202.168.133.150";
#endif

world* g_pTheWorld = new CWorldBase;
int main(int argc, char* argv[])
{
	//first args[1] is configure file name
	const char* pszEtcFn = argv[1];
	//second args[2] is server id for self
	uint16_t nServerId = (uint16_t)atoi(argv[2]);

	
	g_pTheWorld->init(pszEtcFn);

	CBaseServer* server = new CBaseServer;
	server->SetWorld(g_pTheWorld);
	server->SetMailboxId(nServerId);
	g_pTheWorld->SetServer(server);

	server->Service(ipstr, 8001);
	return 0;
}
