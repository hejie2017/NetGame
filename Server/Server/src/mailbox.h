
enum EFDTYPE{
	FD_TYPE_ACCEPT,
	FD_TYPE_CLIENT
};

class CMailBox{
public:
	CMailBox(EFDTYPE fdtype);
	~CMailBox();

	EFDTYPE GetFdType()
	{
		return m_fdtype;
	}

private:
	EFDTYPE m_fdtype;
};
