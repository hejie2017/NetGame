#ifndef __BASE_SERVER__
#define __BASE_SERVER__

#include "epoll_server.h"

class CBaseServer:public CEpollServer
{
public:
	CBaseServer();
	~CBaseServer();
};

#endif // !__BASE_SERVER__
