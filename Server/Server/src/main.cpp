#include "epoll_server.h"

int main()
{
	CEpollServer sv;

	sv.Service("202.168.133.150",8888);

	return 0;
}
