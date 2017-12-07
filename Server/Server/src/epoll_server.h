#ifndef __EPOLL_SERVER_HEAD__
#define __EPOLL_SERVER_HEAD__

#include "net_util.h"
#include <string>
#include <map>
#include "mailbox.h"

using namespace std;
using namespace mogo;

enum
{
    MAX_EPOLL_SIZE = 9999,
    MAXBUF = 1024,
    CLIENT_TIMEOUT = 20,
    OTHERSERVER_TIMEOUT = 5,
};

class CEpollServer
{
    public:
        CEpollServer();
        virtual ~CEpollServer();

    public:
        int StartServer(const char* pszAddr, uint16_t unPort);
        void Shutdown();

    public:
        int Service(const char* pszAddr, unsigned int unPort);

    public:
        //服务器主动关闭一个socket
        void CloseFdFromServer(int fd);
        //顶掉一个连接
        void KickOffFd(int fd);
    public:
        void OnShutdownServer();
        int HandleNewConnection(int fd);
        int OnNewFdAccepted(int new_fd, sockaddr_in& addr);
        CMailBox* GetFdMailbox(int fd);
        void AddFdAndMb(int fd, CMailBox* pmb);
        void AddFdAndMb(int fd, EFDTYPE efd, const char* pszAddr, uint16_t unPort);
        int HandleMessage(int fd, CMailBox* mb);
		int CheckPlutoHeadSize(int fd, CMailBox* mb, uint32_t nMsgLen);
		void AddRecvMsg(CPluto* u);

    protected:
        string m_strAddr;
        uint16_t m_unPort;

        bool m_bShutdown;
        int m_epfd;

        map<int, CMailBox*> m_fds;
		list<CPluto*> m_recvMsgs;
		map<int, uint32_t> m_fd2Plutos;

#ifdef WIN32
		fd_set allSockSet; // 总的套接字集合 
		fd_set readSet; // 可读套接字集合 
		fd_set writeSet; // 可写套接字集合
		int sListen;
#endif // WIN32

};


#endif

