#include "base_server.h"
#ifdef __linux__
const char* ipstr = "202.168.133.195";
#else
const char* ipstr = "202.168.133.150";
#endif

int main()
{
	//CBaseServer* server = new CBaseServer;

	//server->Service(ipstr, 8888);

	return 0;
}