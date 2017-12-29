#include "cell_server.h"
#include "../common/world_select.h"

#ifdef __linux__
const char* ipstr = "202.168.133.195";
#else
const char* ipstr = "202.168.133.150";
#endif

world* g_pTheWorld = new CWorldCell;
int main(int argc, char* argv[])
{
	//first args[1] is configure file name
	const char* pszEtcFn = argv[1];
	//second args[2] is server id for self
	uint16_t nServerId = (uint16_t)atoi(argv[2]);


	g_pTheWorld->init(pszEtcFn);

	CCellServer* server = new CCellServer;
	server->SetWorld(g_pTheWorld);
	g_pTheWorld->SetServer(server);
	server->SetMailboxId(nServerId);


	server->Service(ipstr, 8002);


	return 0;

}