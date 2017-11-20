#include "mailbox.h"

CMailBox::CMailBox(short  uid, EFDTYPE fdtype, const char* pszAddr, short  unPort)
:m_bConnected(false), m_fd(0), m_fdType(fdtype),m_serverName(pszAddr), m_serverPort(unPort), m_id(uid)
{

}

CMailBox::~CMailBox()
{

}
