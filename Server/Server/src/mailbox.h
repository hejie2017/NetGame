#include <string>
using namespace std;

enum EFDTYPE{
	FD_TYPE_ACCEPT,
	FD_TYPE_CLIENT
};

class CMailBox{
public:
	CMailBox(short  uid, EFDTYPE fdtype, const char* pszAddr, short  unPort);
	~CMailBox();

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

private:
	short m_id;
	bool m_bConnected;

	int m_fd;
	EFDTYPE m_fdType;
	string m_serverName;
	short m_serverPort;
};
