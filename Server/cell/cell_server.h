#ifndef __CELL_SERVER__
#define __CELL_SERVER__

#include "../common/epoll_server.h"

class CCellServer:public CEpollServer  {
public:
	CCellServer();
	~CCellServer();
};

#endif // !__BASE_SERVER__
