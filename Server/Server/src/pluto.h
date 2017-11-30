#pragma once
#include "util.h"
#include "mailbox.h"

class CPluto
{
public:
	CPluto();
	~CPluto();

public:
	inline const char* GetBuff() const
	{
		return m_szBuff;
	}

	inline char* GetRecvBuff()
	{
		return m_szBuff;
	}

	inline void SetLen(uint32_t n)
	{
		m_unLen = n;
	}
	inline void SetMaxLen(uint32_t n)
	{
		m_unMaxLen = n;
	}

	inline void EndRecv(uint32_t n)
	{
		m_unLen = n;
		m_unMaxLen = n;
	}

	inline uint32_t GetBuffSize() const
	{
		return m_unBuffSize;
	}

	inline uint32_t GetLen() const
	{
		return m_unLen;
	}

	inline uint32_t GetMaxLen() const
	{
		return m_unMaxLen;
	}

	inline CMailBox* GetMailbox()
	{
		return m_mb;
	}

	inline void SetMailbox(CMailBox* mb)
	{
		m_mb = mb;
	}

	inline bool IsEnd() const
	{
		return m_unLen >= m_unMaxLen;
	}

	inline uint32_t GetDecodeErrIdx() const
	{
		return m_nDecodeErrIdx;
	}

	inline bool IsEncodeErr() const
	{
		return m_bEncodeErr;
	}

	inline void SetEncodeErr()
	{
		m_bEncodeErr = true;
	}

private:
	static MemoryPool *memPool;
	static void expandMemoryPool();
	//static MyLock m_lock;

	char* m_szBuff;
	uint32_t m_unBuffSize;
	uint32_t m_unLen;
	uint32_t m_unMaxLen;
	uint32_t m_nDecodeErrIdx;
	CMailBox* m_mb;
	bool m_bEncodeErr;  //±àÂëÊÇ·ñ³ö´í

};
