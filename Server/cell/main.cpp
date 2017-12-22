#include "cell_server.h"
#include "../common/world_select.h"

#ifdef __linux__
const char* ipstr = "202.168.133.195";
#else
const char* ipstr = "202.168.133.150";
#endif

world* g_pTheWorld = new CWorldCell;
int main()
{
	CCellServer* server = new CCellServer;
	
	server->SetWorld(g_pTheWorld);
	server->Service(ipstr, 8888);
	
	delete server;
	return 0;
}