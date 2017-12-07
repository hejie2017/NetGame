#include "epoll_server.h"
#ifdef __linux__
const char* ipstr = "202.168.133.195";
#else
const char* ipstr = "202.168.133.150";
#endif

int main()
{
	CEpollServer sv;

	sv.Service(ipstr,8888);

	return 0;
}
