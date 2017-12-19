/*----------------------------------------------------------------
// Copyright (C) 2013 广州，爱游
//
// 模块名：net_util
// 创建者：Steven Yang
// 修改者列表：
// 创建日期：2013.1.5
// 模块描述：对socket 的简单包装
//----------------------------------------------------------------*/

#include "net_util.h"

namespace mogo
{

    bool MogoSetNonblocking(int sockfd)
    {
#ifdef __linux__
        return fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) != -1;
#else
		//return ioctlsocket(sockfd, FIONBIO, (unsigned long *)1);
		return 0;
#endif
    }

    int MogoSocket()
    {
        return socket(PF_INET, SOCK_STREAM, 0);
    }

    int MogoBind(int sockfd, const char* pszAddr, unsigned int unPort)
    {
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = PF_INET;
        addr.sin_port = htons(unPort);

        if(pszAddr == NULL || strcmp(pszAddr, "") == 0)
        {
            addr.sin_addr.s_addr = INADDR_ANY;
        }
        else
        {
#ifdef __linux__
			addr.sin_addr.s_addr = inet_addr(pszAddr);
#else // DEBUG
			struct in_addr s;
			inet_pton(PF_INET, pszAddr, (void *)&s);
			addr.sin_addr.s_addr = s.s_addr;
#endif
        }

        int flag = 1;
        int len = sizeof(int);
#ifdef __linux__
		setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, len);
#else // DEBUG
        //setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&flag, len);
#endif
        return bind(sockfd, (struct sockaddr*)&addr, sizeof(addr) );
    }

    int MogoListen(int sockfd, int backlog/* = 5*/)
    {
        return listen(sockfd, backlog);
    }

    int MogoConnect(int fd, const char* pszAddr, unsigned int unPort)
    {
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = PF_INET;
        addr.sin_port = htons(unPort);
        //addr.sin_addr.s_addr = inet_addr(pszAddr);

        return connect(fd, (sockaddr*)&addr, sizeof(addr));
    }

	void MogoSetBuffSize(int fd, int nRcvBuf, int nSndBuf)
	{	
#ifdef __linux__
		setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (const int*)&nRcvBuf, sizeof(int));
		setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (const int*)&nSndBuf, sizeof(int));
#else // DEBUG
		setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (const char*)&nRcvBuf, sizeof(int));	
		setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (const char*)&nSndBuf, sizeof(int));
#endif
	}

	void MogoGetBuffSize(int fd)
	{
#ifdef __linux__
		int n1 = 0,n2 = 0;
		socklen_t nn1 = sizeof(n1),nn2=sizeof(n2);
		getsockopt(fd, SOL_SOCKET, SO_RCVBUF, (int*)&n1, &nn1);
		getsockopt(fd, SOL_SOCKET, SO_SNDBUF, (int*)&n2, &nn2);
		//printf("222fd=%d;rcv=%d;snd=%d\n", fd, n1, n2);
#else

#endif
	}

    int MogoAsyncRead(int sockfd, void* buf, size_t bufsize, int nTimeout)
    {
#ifdef __linux__
        fd_set rfds;       
        FD_ZERO(&rfds);
        FD_SET(sockfd, &rfds);

        struct timeval tv;
        tv.tv_sec = nTimeout;
        tv.tv_usec = 0;

        int ret = select(sockfd+1, &rfds, NULL, NULL, &tv);
        if(ret > 0)
        {
            return ::read(sockfd, buf, bufsize);
        }
        
        return ret;
#endif
		return 0;
    }
};
