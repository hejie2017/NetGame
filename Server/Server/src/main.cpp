#include "epoll_server.h"

int main()
{
	CEpollServer* sv = new CEpollServer();
	sv->Service("202.168.133.195",8888);


	return 0;
}
