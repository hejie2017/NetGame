/*----------------------------------------------------------------
 // Copyright (C) 2013 广州，爱游
 //
 // 模块名：epoll_server
 // 创建者：Steven Yang
 // 修改者列表：
 // 创建日期：2013.1.5
 // 模块描述：epoll 相关。
 //----------------------------------------------------------------*/

#include "epoll_server.h"

CEpollServer::CEpollServer() :
		m_epfd(0), m_bShutdown(false),m_fds() {

}

CEpollServer::~CEpollServer() {

}

int CEpollServer::StartServer(const char* pszAddr, uint16_t unPort) {
	m_strAddr.assign(pszAddr);
	m_unPort = unPort;

	int fd = MogoSocket();
	if (fd == -1) {
		ERROR_RETURN2("Failed to create socket");
	}

	MogoSetNonblocking(fd);

	//修改rcvbuf和sndbuf
	enum {
		_BUFF_SIZE = 174760
	};
	MogoSetBuffSize(fd, _BUFF_SIZE, _BUFF_SIZE);

	int n = MogoBind(fd, pszAddr, unPort);
	if (n != 0) {
		printf("bind fail, fd=%d;pszAddr=%s;unPort=%d\n", fd, pszAddr, unPort);
		ERROR_RETURN2("Failed to bind");
	}

	n = MogoListen(fd, 20);
	if (n != 0) {
		ERROR_RETURN2("Failed to listen");
	}

	m_epfd = epoll_create(MAX_EPOLL_SIZE);
	if (m_epfd == -1) {
		ERROR_RETURN2("Failed to epoll_create");
	}

	struct epoll_event ev;
	memset(&ev, 0, sizeof ev);
	//ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
	ev.events = EPOLLIN | EPOLLOUT;
	ev.data.fd = fd;

	if (epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		ERROR_RETURN2("Failed to epoll_ctl_add listen fd");
	}

	return 0;
}

int CEpollServer::Service(const char* pszAddr, unsigned int unPort) {
	int nRet = StartServer(pszAddr, unPort);
	if (nRet != 0) {
		return nRet;
	}

	struct epoll_event ev;
	struct epoll_event events[MAX_EPOLL_SIZE];

	enum {
		_EPOLL_TIMEOUT = 50,
	};

	while (!m_bShutdown) {
		//GetWorld()->DoConsoleLua();

//        int event_count = m_fds.size();
		int event_count = 10;
		int nfds = epoll_wait(m_epfd, events, event_count, _EPOLL_TIMEOUT);
		if (nfds == -1) {
			if (errno == EINTR) //modify-kevin-20130226
			{
				continue;
			} else {
				ERROR_RETURN2("Failed to epoll_wait");
				break;
			}
		} else if (nfds == 0) {
			//timeout
//            this->HandleTimeout();
		}

		for (int n = 0; n < nfds; ++n) {
			int fd = events[n].data.fd;
            CMailBox* mb = GetFdMailbox(fd);
            if(mb == NULL){
					continue;
				}
            EFDTYPE tfd = mb->GetFdType();


            switch(tfd)
				{
					case FD_TYPE_ACCEPT:
					{
						int _nRet = HandleNewConnection(fd);

						break;
					}
					case FD_TYPE_CLIENT:
					{
//						if(this->HandleFdEvent(fd, events[n].events, mb) != 0)
//						{
//							//--event_count;
//						}
						break;
					}

					default:
					{
						//FD_TYPE_ERROR
						break;
					}
				}

		}

	}

	OnShutdownServer();

	return 0;
}

void CEpollServer::OnShutdownServer() {
	sleep(2);
}

int CEpollServer::HandleNewConnection(int fd) {
	struct sockaddr_in their_addr;
	socklen_t their_len = sizeof(their_addr);
	int new_fd = accept(fd, (struct sockaddr *) &their_addr, &their_len);
	if (new_fd < 0) {
//        if(errno == EAGAIN)
//        {
//            ERROR_PRINT2("Failed to accept new connection,try EAGAIN\n")
//            return -1;
//        }
//        else
//        {
//            ERROR_PRINT2("Failed to accept new connection")
//            return -2;
//        }
	}

	enum {
		MAX_ACCEPT = 1024 - 20,
	};
	//一般linux设置每个进程打开文件数为1024,这个设置正好符合游戏的最大连接数
	//enum{ MAX_ACCEPT = 4000, };
//    if(m_fds.size() >= MAX_ACCEPT)
//    {
//        ::close(new_fd);
////        LogWarning("max_connection", "closed=%d", new_fd);
//        return -3;
//    }

	char* pszClientAddr = inet_ntoa(their_addr.sin_addr);
	uint16_t unClientPort = ntohs(their_addr.sin_port);

	MogoSetNonblocking(new_fd);
	struct epoll_event ev;
	memset(&ev, 0, sizeof ev);

	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = new_fd;
	if (epoll_ctl(m_epfd, EPOLL_CTL_ADD, new_fd, &ev) < 0) {
		ERROR_PRINT2("Failed to epoll_ctl_add new accepted socket");
		return -3;
	}
    this->OnNewFdAccepted(new_fd, their_addr);

	return 0;
}

int CEpollServer::OnNewFdAccepted(int new_fd, sockaddr_in& addr)
{
    char* pszClientAddr = inet_ntoa(addr.sin_addr);
    uint16_t unClientPort = ntohs(addr.sin_port);

    AddFdAndMb(new_fd, FD_TYPE_ACCEPT, pszClientAddr, unClientPort);
    //printf("on_new_fd_accepted\n");
    return 0;
}

void CEpollServer::AddFdAndMb(int fd, CMailBox* pmb)
{
    pmb->SetFd(fd);

    map<int, CMailBox*>::iterator iter = m_fds.lower_bound(fd);
    if(iter != m_fds.end() && iter->first == fd)
    {
        //异常情况,有一个老的mb未删除
        CMailBox* p2 = iter->second;
        if (p2 != pmb)
        {
            delete p2;
            iter->second = pmb;
        }

        //iter->second = pmb;
        //delete p2;

    }
    else
    {
        //正常情况
        m_fds.insert(iter, make_pair(fd, pmb));

        ////初始化每个连接上面待处理的Pluto数量
        //this->m_fd2Plutos.insert(make_pair(fd, 0));
    }

}

//来自客户端的连接会直接调用这个方法
void CEpollServer::AddFdAndMb(int fd, EFDTYPE efd, const char* pszAddr, uint16_t unPort)
{
    CMailBox* pmb = new CMailBox(0, efd, pszAddr, unPort);

//    //来自可信任客户端地址的连接,免认证
//    if(this->GetWorld()->IsTrustedClient(pmb->GetServerName()))
//    {
//        LogDebug("CEpollServer::AddFdAndMb", "client_trusted, serverName=%s", pmb->GetServerName().c_str());
//        pmb->SetAuthz(MAILBOX_CLIENT_TRUSTED);
//    }

    //设置已连接标记
    pmb->SetConnected();

    AddFdAndMb(fd, pmb);
}


//服务器主动关闭一个socket
void CEpollServer::CloseFdFromServer(int fd) {
//    this->OnFdClosed(fd);
	epoll_ctl(m_epfd, EPOLL_CTL_DEL, fd, NULL);
	::close(fd);
//    RemoveFd(fd);
}

//顶掉一个连接
void CEpollServer::KickOffFd(int fd) {
	epoll_ctl(m_epfd, EPOLL_CTL_DEL, fd, NULL);
//    RemoveFd(fd);
	::close(fd);
}

CMailBox* CEpollServer::GetFdMailbox(int fd)
{
    map<int, CMailBox*>::const_iterator iter = m_fds.find(fd);
    if(iter == m_fds.end())
    {
        return NULL;
    }
    else
    {
        return iter->second;
    }
}

//
////直接接收数据至pluto,不需要先接收到buff再copy
//int CEpollServer::HandleMessage(int fd, CMailBox* mb)
//{
//    //errno = 0;
//    int nLen = -1;
//    CPluto* u = mb->GetRecvPluto();
//    if(u == NULL)
//    {
//        //新包
//        //接收包头
//        char szHead[PLUTO_MSGLEN_HEAD];
//        nLen = ::recv(fd, szHead, PLUTO_MSGLEN_HEAD, 0);
//
//        //LogDebug("hdm_recv1", "fd=%d;want=%d;recv=%d", fd, PLUTO_MSGLEN_HEAD, nLen);
//        if(nLen > 0)
//        {
//            if(nLen == PLUTO_MSGLEN_HEAD)
//            {
//                uint32_t nMsgLen = sz_to_uint32((unsigned char*)szHead);
//
//                if (nMsgLen > 65536)
//                {
//                    LogWarning("CEpollServer::HandleMessage warning", "nMsgLen=%d;ServerName=%s;ServerType=%d",
//                                                                       nMsgLen, mb->GetServerName().c_str(), mb->GetServerMbType());
//                    PrintHex16(szHead, (size_t)nLen);
//                }
//
//                //PLUTO_HEAD_SIZE_CHECK(fd, mb, nMsgLen);
//                int result = this->CheckPlutoHeadSize(fd, mb, nMsgLen);
//                if (result < 0)
//                {
//                    return result;
//                }
//
//                u = new CPluto(nMsgLen);
//
//                //copy head
//                char* szBuff = u->GetRecvBuff();
//                memcpy(szBuff, szHead, PLUTO_MSGLEN_HEAD);
//
//                //接收包体
//                int nWanted = nMsgLen-PLUTO_MSGLEN_HEAD;
//                nLen = ::recv(fd, szBuff+PLUTO_MSGLEN_HEAD, nWanted, 0);
//                //LogDebug("hdm_recv2", "fd=%d;want=%d;recv=%d", fd, nWanted, nLen);
//                if(nLen > 0)
//                {
//                    if(nLen == nWanted)
//                    {
//                        //接收完整
//                        u->EndRecv(nMsgLen);
//                        u->SetMailbox(mb);
//                        AddRecvMsg(u);
//                        mb->SetRecvPluto(NULL); //置空
//
//                        //printf("recv all\n");
//                        //PrintHexPluto(*u);
//                        //LogDebug("CEpollServer::HandleMessage all", "msg_id=%d", u->GetMsgId());
//                        //可能还有其他包要收
//#ifdef HANDLE_MESSAGE_OLD
//                        return HandleMessage(fd, mb);
//#else
//                        return -4;
//#endif
//                    }
//                    else
//                    {
//                        //接收不完整,留到下次接着处理
//                        u->SetLen(PLUTO_MSGLEN_HEAD+nLen);
//                        mb->SetRecvPluto(u);
//
//                        //PrintHexPluto(*u);
//                        //LogDebug("CEpollServer::HandleMessage part", "msg_id=%d", u->GetMsgId());
//                        //printf("recv part11\n");
//                        return PLUTO_MSGLEN_HEAD+nLen;
//                    }
//                }
//                else if(nLen < 0)
//                {
//                    if(errno == EAGAIN || errno == EINPROGRESS)
//                    {
//                        //接收不完整,留到下次接着处理
//                        u->SetLen(PLUTO_MSGLEN_HEAD);
//                        mb->SetRecvPluto(u);
//
//                        //print_hex_pluto(*u);
//                        //printf("recv part11\n");
//                        return PLUTO_MSGLEN_HEAD;
//                    }
//                }
//
//                delete u;//这个u是前面new出来的,不用提供给其他地方处理
//                u = NULL;
//            }
//            else
//            {
//                //包头没有接收完
//                u = new CPluto(PLUTO_MSGLEN_HEAD);
//                char* szBuff = u->GetRecvBuff();
//                memcpy(szBuff, szHead, nLen);
//                u->SetLen(nLen);
//                mb->SetRecvPluto(u);
//
//                //print_hex_pluto(*u);
//                //printf("recv part22\n");
//                return nLen;
//            }
//        }
//    }
//    else
//    {
//        char* szBuff = u->GetRecvBuff();
//        int nLastLen = u->GetLen();     //上次接收到的数据长度
//        if(nLastLen < PLUTO_MSGLEN_HEAD)
//        {
//            //包头未收完
//            int nWanted = PLUTO_MSGLEN_HEAD - nLastLen;
//            nLen = ::recv(fd, szBuff+nLastLen, nWanted, 0);
//            //LogDebug("hdm_recv3", "fd=%d;want=%d;recv=%d", fd, nWanted, nLen);
//            if(nLen > 0)
//            {
//                if(nLen == nWanted)
//                {
//                    int nMsgLen = sz_to_uint32((unsigned char*)szBuff);
//                    //PLUTO_HEAD_SIZE_CHECK(fd, mb, nMsgLen);
//
//                    int result = this->CheckPlutoHeadSize(fd, mb, nMsgLen);
//                    if (result < 0)
//                    {
//                        return result;
//                    }
//
//                    CPluto* u2 = new CPluto(nMsgLen);
//                    memcpy(u2->GetRecvBuff(), szBuff, PLUTO_MSGLEN_HEAD);
//                    u2->SetLen(PLUTO_MSGLEN_HEAD);
//                    mb->SetRecvPluto(u2);
//                    delete u;
//
//                    //printf("recv all88\n");
//                    //print_hex_pluto(*u2);
//
//#ifdef HANDLE_MESSAGE_OLD
//                    return HandleMessage(fd, mb);
//#else
//                    return -4;
//#endif
//                }
//                else
//                {
//                    //printf("recv part99\n");
//                    //仍然未接收完
//                    u->SetLen(nLastLen+nLen);
//                    //print_hex_pluto(*u);
//                    return nLen;
//                }
//            }
//        }
//        else
//        {
//            int nWanted = u->GetBuffSize() - nLastLen;
//            nLen = ::recv(fd, szBuff+nLastLen, nWanted, 0);
//            //LogDebug("hdm_recv4", "fd=%d;want=%d;recv=%d", fd, nWanted, nLen);
//            if(nLen > 0)
//            {
//                if(nLen == nWanted)
//                {
//                    //接收完整
//                    u->EndRecv(nLastLen+nLen);
//                    u->SetMailbox(mb);
//                    AddRecvMsg(u);
//                    mb->SetRecvPluto(NULL); //置空
//
//                    //printf("recv part33\n");
//                    //print_hex_pluto(*u);
//                    //可能还有其他包要处理
//
//#ifdef HANDLE_MESSAGE_OLD
//                    return HandleMessage(fd, mb);
//#else
//                    return -4;
//#endif
//                }
//                else
//                {
//                    //printf("recv part44\n");
//                    //接收不完整,留到下次接着处理
//                    u->SetLen(nLastLen+nLen);
//                    //print_hex_pluto(*u);
//
//                    return nLen;
//                }
//            }
//        }
//    }
//
//    if(nLen == 0)
//    {
//        //client close
//    }
//    else
//    {
//        if(errno == EAGAIN)
//        {
//            return 0;
//        }
//        LogWarning("handle_message_err", "failed, %d,'%s'",errno, strerror(errno));
//    }
//    //LogDebug("hdm_recv_err", "fd=%d;recv=%d;err=%d", fd, nLen, errno);
//    close(fd);
//    return -1;
//}
//#endif
