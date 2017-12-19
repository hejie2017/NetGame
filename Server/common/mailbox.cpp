#include "mailbox.h"
#include "net_util.h"

using namespace mogo;


	CMailBox::CMailBox(short  uid, EFDTYPE fdtype, const char* pszAddr, short  unPort)
		:m_bConnected(false), m_fd(0), m_fdType(fdtype), m_serverName(pszAddr), m_serverPort(unPort), m_id(uid), m_nSendPos(0)
	{

	}

	CMailBox::~CMailBox()
	{

	}

	int CMailBox::ConnectServer(int epfd)
	{

		if (m_fd > 0)
		{
#ifdef __linux__
			close(m_fd);
#else
			closesocket(m_fd);
#endif
		}

		m_fd = MogoSocket();
		MogoSetNonblocking(m_fd);

		//�޸�rcvbuf��sndbuf
		enum { _BUFF_SIZE = 174760 };
		MogoSetBuffSize(m_fd, _BUFF_SIZE, _BUFF_SIZE);

#ifdef __linux__
		struct epoll_event ev;
		memset(&ev, 0, sizeof ev);
		ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
		ev.data.fd = m_fd;

		if (epoll_ctl(epfd, EPOLL_CTL_ADD, m_fd, &ev) == -1)
		{
			ERROR_RETURN2("Failed to epoll_ctl_add connect fd");
		}
#else

#endif // __linux__

		int nRet = MogoConnect(m_fd, GetServerName().c_str(), GetServerPort());
		if (nRet != 0 && errno != EINPROGRESS)
		{
			printf("Failed to connect");
			return -1;
		}

		return 0;
	}

	int CMailBox::SendAll()
	{
		if (IsConnected()) {
			while (!m_tobeSend.empty()) {
				CPluto* u = m_tobeSend.front();
				int nSendWant = (int)u->GetLen() - m_nSendPos;		//�ڴ����͵��ֽ���
				int nSendRet = ::send(m_fd, u->GetBuff() + m_nSendPos, nSendWant, 0);

				if (nSendRet != nSendWant)
				{
					//uint16_t mbid = GetMailboxId();

					//if (mbid == 0 && GetAuthz() != MAILBOX_CLIENT_TRUSTED)
					//{
					//	//�ͻ������Ӳ��ط���,ֱ�ӹص�
					//	return -1;
					//}

					if (nSendRet >= 0)
					{
						//������,�����´μ�������                        
						m_nSendPos += nSendRet;
						return 0;
					}
					else
					{
						if (errno == EINPROGRESS || errno == EAGAIN)
						{
							//������,�����´μ�������                        
							return 0;
						}
					}

					//�ж�,����ǿͻ�����ر�,���������������,֪ͨ�������˳�
					//������Ϣ��,�ȴ��ط�
					//��ʱѡ���˳�? //todo
					return -1;
				}

				m_tobeSend.pop_front();
				delete u;
				m_nSendPos = 0;
			}
		}
		return 0;
	}

