#ifndef __MAIL_BOX__
#define __MAIL_BOX__


#include <string>
#include <list>

#include "memory_pool.h"
#include "pluto.h"

using namespace std;

enum EFDTYPE {
	FD_TYPE_SERVER,
	FD_TYPE_ACCEPT
};

enum
{
	SEND_BUFF = 8 * 1024
};

class CPluto;

class CMailBox
{
	public:
	CMailBox(short  uid, EFDTYPE fdtype, const char* pszAddr, short  unPort);
	~CMailBox();

	int ConnectServer(int epfd);
	int SendAll();
public:
	inline bool IsConnected() const
	{
		return m_bConnected;
	}

	inline void SetConnected(bool c = true)
	{
		m_bConnected = c;
	}

	inline void SetFd(int fd)
	{
		m_fd = fd;
	}

	inline int GetFd() const
	{
		return m_fd;
	}

	inline EFDTYPE GetFdType() const
	{
		return m_fdType;
	}

	inline const string& GetServerName() const
	{
		return m_serverName;
	}

	inline short GetServerPort() const
	{
		return m_serverPort;
	}

	inline CPluto* GetRecvPluto()
	{
		return m_pluto;
	}

	inline void SetRecvPluto(CPluto* u)
	{
		m_pluto = u;
	}
	
private:
	list<CPluto*> m_tobeSend;
	CPluto* m_pluto;
	
	int m_nSendPos;

	short m_id;
	bool m_bConnected;

	int m_fd;
	EFDTYPE m_fdType;
	string m_serverName;
	short m_serverPort;

	static MemoryPool *memPool;
};

#endif