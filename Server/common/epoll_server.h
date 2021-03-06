#ifndef __EPOLL_SERVER_HEAD__
#define __EPOLL_SERVER_HEAD__

#include "net_util.h"
#include <string>
#include <map>
#include "mailbox.h"
#include "world.h"

using namespace std;
using namespace mogo;

enum
{
    MAX_EPOLL_SIZE = 9999,
    MAXBUF = 1024,
    CLIENT_TIMEOUT = 20,
    OTHERSERVER_TIMEOUT = 5,
};

namespace mogo {
	class world;
}

class CEpollServer
{
    public:
        CEpollServer();
        virtual ~CEpollServer();

    public:
        int StartServer(const char* pszAddr, uint16_t unPort);
        int Service(const char* pszAddr, unsigned int unPort);
		int ConnectMailboxs(const char* pszCfgFile);
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
		void HandlePluto();
		world* GetWorld();
		void SetWorld(world* w);
		int HandleSendPluto();
		CMailBox* GetClientMailbox(int32_t fd);
	public:
		inline void SetMailboxId(uint16_t mid)
		{
			m_unMailboxId = mid;
		}

		inline uint16_t GetMailboxId() const
		{
			return m_unMailboxId;
		}

		CMailBox* GetServerMailbox(uint16_t nServerId)
		{
			//printf("get_server_mailbox:%d_%d\n", nServerId, m_serverMbs.size());
			if (nServerId < m_serverMbs.size())
			{
				return m_serverMbs[nServerId];
			}
			else
			{
				return NULL;
			}
		}

    protected:
        string m_strAddr;
        uint16_t m_unPort;

        bool m_bShutdown;
        int m_epfd;

        map<int, CMailBox*> m_fds;
		list<CPluto*> m_recvMsgs;
		map<int, uint32_t> m_fd2Plutos;
		vector<CMailBox*> m_serverMbs;    //服务器组件的mailbox

		world* the_world;
		uint16_t m_unMailboxId;
#ifdef WIN32
		int g_iTotalConn;
		SOCKET g_CliSocketArr[FD_SETSIZE];// 总的套接字集合 
		//fd_set allSockSet; // 总的套接字集合 
		fd_set readSet; // 可读套接字集合 
		fd_set writeSet; // 可写套接字集合
		int sListen;
#endif // WIN32

};


#endif

