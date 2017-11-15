#ifndef __EPOLL_SERVER_HEAD__
#define __EPOLL_SERVER_HEAD__


#include <sys/epoll.h>

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

    protected:
        virtual int HandlePluto();
        virtual int HandleSendPluto();

    protected:
        string m_strAddr;
        uint16_t m_unPort;

};



#endif

