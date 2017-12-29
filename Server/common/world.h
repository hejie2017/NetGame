#ifndef __WORLD__
#define __WORLD__

#include "pluto.h"
#include "defparser.h"
#include "cfg_reader.h"
#include "rpc_mogo.h"
#include "epoll_server.h"

class CEpollServer;

namespace mogo {

	class CMailBoxManager
	{
	public:
		CMailBoxManager();
		~CMailBoxManager();

	public:
		bool init(CCfgReader& cfg);

	public:
		inline list<CMailBox*>& GetMailboxs()
		{
			return m_mbs;
		}

	private:
		list<CMailBox*> m_mbs;

	};

	class world
	{
	public:
		world();
		virtual ~world();

		int init(const char* pszEtcFile);

		void InitMailboxMgr();

		virtual int FromRpcCall(CPluto& u) = 0;

		int OnTimerdTick(T_VECTOR_OBJECT* p);

		CMailBox* GetServerMailbox(uint16_t nServerId);

		void SetServer(CEpollServer* server);
		CEpollServer* GetServer();

		bool CheckClientRpc(CPluto& u);
		void AddClientFdToVObjectList(int fd, T_VECTOR_OBJECT* p);

	public:
		template<typename T1>
		void RpcCall(uint16_t nServerId, pluto_msgid_t msg_id, const T1& p1);

		template<typename T1, typename T2>
		void RpcCall(uint16_t nServerId, pluto_msgid_t msg_id, const T1& p1, const T2& p2);

		template<typename T1, typename T2, typename T3>
		void RpcCall(uint16_t nServerId, pluto_msgid_t msg_id, const T1& p1, const T2& p2, const T3& p3);

		template<typename T1, typename T2, typename T3, typename T4>
		void RpcCall(uint16_t nServerId, pluto_msgid_t msg_id, const T1& p1, const T2& p2, const T3& p3, const T4& p4);

		template<typename T1, typename T2, typename T3, typename T4, typename T5>
		void RpcCall(uint16_t nServerId, pluto_msgid_t msg_id, const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5);

		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
		void RpcCall(uint16_t nServerId, pluto_msgid_t msg_id, const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5, const T6& p6);

	public:
		//使用于dbmgr/logapp多线程的rpcall
		//inline bool SyncRpcCall(CPlutoList& pl, uint16_t nServerId, CPluto* u)
		//{
		//	CEpollServer* s = this->GetServer();
		//	CMailBox* mb = s->GetServerMailbox(nServerId);
		//	if (mb)
		//	{
		//		u->SetMailbox(mb);
		//		pl.PushPluto(u);

		//		return true;
		//	}
		//	return false;
		//}

		template<typename T1>
		void SyncRpcCall(CPlutoList& pl, uint16_t nServerId, pluto_msgid_t msg_id, const T1& p1);

		template<typename T1, typename T2>
		void SyncRpcCall(CPlutoList& pl, uint16_t nServerId, pluto_msgid_t msg_id, const T1& p1, const T2& p2);

		template<typename T1, typename T2, typename T3>
		void SyncRpcCall(CPlutoList& pl, uint16_t nServerId, pluto_msgid_t msg_id, const T1& p1, const T2& p2, const T3& p3);

		template<typename T1, typename T2, typename T3, typename T4>
		void SyncRpcCall(CPlutoList& pl, uint16_t nServerId, pluto_msgid_t msg_id, const T1& p1, const T2& p2, const T3& p3, const T4& p4);

		template<typename T1, typename T2, typename T3, typename T4, typename T5>
		void SyncRpcCall(CPlutoList& pl, uint16_t nServerId, pluto_msgid_t msg_id, const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5);

		template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
		void SyncRpcCall(CPlutoList& pl, uint16_t nServerId, pluto_msgid_t msg_id, const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5, const T6& p6);

	public:
		inline CDefParser& GetDefParser()
		{
			return m_defParser;
		}

		inline bool IsDelete() const
		{
			return m_bDeleteFlag;
		}
		inline CMailBoxManager& GetMbMgr()
		{
			return m_mbMgr;
		}
	protected:

		CCfgReader* m_cfg;
		CDefParser m_defParser;
		CEpollServer* the_server;
		
		CRpcUtil m_rpc;
		bool m_bDeleteFlag;        //标记之后,pluto包不再处理

		CMailBoxManager m_mbMgr;
	};


	//从VOBJECT*中读取字段
#define VOBJECT_GET_SSTR(p) *(p->vv.s)
#define VOBJECT_GET_STR(p) p->vv.s->c_str()
#define VOBJECT_GET_U8(p) p->vv.u8
#define VOBJECT_GET_U16(p) p->vv.u16
#define VOBJECT_GET_U32(p) p->vv.u32
#define VOBJECT_GET_U64(p) p->vv.u64
#define VOBJECT_GET_I8(p) p->vv.i8
#define VOBJECT_GET_I16(p) p->vv.i16
#define VOBJECT_GET_I32(p) p->vv.i32
#define VOBJECT_GET_I64(p) p->vv.i64
#define VOBJECT_GET_F32(p) p->vv.f32
//#define VOBJECT_GET_F64(p) p->vv.f64
#define VOBJECT_GET_EMB(p) p->vv.emb
#define VOBJECT_GET_BLOB(x) (x->vv.p)   //p->vv.p


	template<typename T1>
	void world::RpcCall(uint16_t nServerId, pluto_msgid_t msg_id, const T1& p1)
	{
		if (nServerId == GetMailboxId())
		{
			//本进程
			CPluto* u = new CPluto;
			m_rpc.Encode(*u, msg_id, p1);
			GetServer()->AddLocalRpcPluto(u);
		}
		else
		{
			CMailBox* mb = GetServerMailbox(nServerId);
			if (mb)
			{
				mb->RpcCall(m_rpc, msg_id, p1);
			}
			else
			{
				LogWarning("world.rpc_call.error", "server_id=%d", nServerId);
			}
		}
	}

	template<typename T1, typename T2>
	void world::RpcCall(uint16_t nServerId, pluto_msgid_t msg_id, const T1& p1, const T2& p2)
	{
		if (nServerId == GetMailboxId())
		{
			//本进程
			CPluto* u = new CPluto;
			m_rpc.Encode(*u, msg_id, p1, p2);
			GetServer()->AddLocalRpcPluto(u);
		}
		else
		{
			CMailBox* mb = GetServerMailbox(nServerId);
			if (mb)
			{
				mb->RpcCall(m_rpc, msg_id, p1, p2);
			}
			else
			{
				LogWarning("world.rpc_call.error", "server_id=%d", nServerId);
			}
		}
	}

	template<typename T1, typename T2, typename T3>
	void world::RpcCall(uint16_t nServerId, pluto_msgid_t msg_id, const T1& p1, const T2& p2, const T3& p3)
	{
		if (nServerId == GetMailboxId())
		{
			//本进程
			CPluto* u = new CPluto;
			m_rpc.Encode(*u, msg_id, p1, p2, p3);
			GetServer()->AddLocalRpcPluto(u);
		}
		else
		{
			CMailBox* mb = GetServerMailbox(nServerId);
			if (mb)
			{
				mb->RpcCall(m_rpc, msg_id, p1, p2, p3);
			}
			else
			{
				LogWarning("world.rpc_call.error", "server_id=%d", nServerId);
			}
		}
	}

	template<typename T1, typename T2, typename T3, typename T4>
	void world::RpcCall(uint16_t nServerId, pluto_msgid_t msg_id, const T1& p1, const T2& p2, const T3& p3, const T4& p4)
	{
		if (nServerId == GetMailboxId())
		{
			//本进程
			CPluto* u = new CPluto;
			m_rpc.Encode(*u, msg_id, p1, p2, p3, p4);
			GetServer()->AddLocalRpcPluto(u);
		}
		else
		{
			CMailBox* mb = GetServerMailbox(nServerId);
			if (mb)
			{
				mb->RpcCall(m_rpc, msg_id, p1, p2, p3, p4);
			}
			else
			{
				LogWarning("world.rpc_call.error", "server_id=%d", nServerId);
			}
		}
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5>
	void world::RpcCall(uint16_t nServerId, pluto_msgid_t msg_id, const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5)
	{
		if (nServerId == GetMailboxId())
		{
			//本进程
			CPluto* u = new CPluto;
			m_rpc.Encode(*u, msg_id, p1, p2, p3, p4, p5);
			GetServer()->AddLocalRpcPluto(u);
		}
		else
		{
			CMailBox* mb = GetServerMailbox(nServerId);
			if (mb)
			{
				mb->RpcCall(m_rpc, msg_id, p1, p2, p3, p4, p5);
			}
			else
			{
				LogWarning("world.rpc_call.error", "server_id=%d", nServerId);
			}
		}
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	void world::RpcCall(uint16_t nServerId, pluto_msgid_t msg_id, const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5, const T6& p6)
	{
		if (nServerId == GetMailboxId())
		{
			//本进程
			CPluto* u = new CPluto;
			m_rpc.Encode(*u, msg_id, p1, p2, p3, p4, p5, p6);
			GetServer()->AddLocalRpcPluto(u);
		}
		else
		{
			CMailBox* mb = GetServerMailbox(nServerId);
			if (mb)
			{
				mb->RpcCall(m_rpc, msg_id, p1, p2, p3, p4, p5, p6);
			}
			else
			{
				LogWarning("world.rpc_call.error", "server_id=%d", nServerId);
			}
		}
	}

	//使用于dbmgr/logapp多线程的rpcall
	template<typename T1>
	void world::SyncRpcCall(CPlutoList& pl, uint16_t nServerId, pluto_msgid_t msg_id, const T1& p1)
	{
		CEpollServer* s = this->GetServer();
		CMailBox* mb = s->GetServerMailbox(nServerId);
		if (mb)
		{
			CPluto* u = new CPluto;
			m_rpc.Encode(*u, msg_id, p1);
			u->SetMailbox(mb);
			pl.PushPluto(u);
		}
	}

	template<typename T1, typename T2>
	void world::SyncRpcCall(CPlutoList& pl, uint16_t nServerId, pluto_msgid_t msg_id, const T1& p1, const T2& p2)
	{
		CEpollServer* s = this->GetServer();
		CMailBox* mb = s->GetServerMailbox(nServerId);
		if (mb)
		{
			CPluto* u = new CPluto;
			m_rpc.Encode(*u, msg_id, p1, p2);
			u->SetMailbox(mb);
			pl.PushPluto(u);
		}
	}

	template<typename T1, typename T2, typename T3>
	void world::SyncRpcCall(CPlutoList& pl, uint16_t nServerId, pluto_msgid_t msg_id, const T1& p1, const T2& p2, const T3& p3)
	{
		CEpollServer* s = this->GetServer();
		CMailBox* mb = s->GetServerMailbox(nServerId);
		if (mb)
		{
			CPluto* u = new CPluto;
			m_rpc.Encode(*u, msg_id, p1, p2, p3);
			u->SetMailbox(mb);
			pl.PushPluto(u);
		}
	}

	template<typename T1, typename T2, typename T3, typename T4>
	void world::SyncRpcCall(CPlutoList& pl, uint16_t nServerId, pluto_msgid_t msg_id, const T1& p1, const T2& p2, const T3& p3, const T4& p4)
	{
		CEpollServer* s = this->GetServer();
		CMailBox* mb = s->GetServerMailbox(nServerId);
		if (mb)
		{
			CPluto* u = new CPluto;
			m_rpc.Encode(*u, msg_id, p1, p2, p3, p4);
			u->SetMailbox(mb);
			pl.PushPluto(u);
		}
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5>
	void world::SyncRpcCall(CPlutoList& pl, uint16_t nServerId, pluto_msgid_t msg_id, const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5)
	{
		CEpollServer* s = this->GetServer();
		CMailBox* mb = s->GetServerMailbox(nServerId);
		if (mb)
		{
			CPluto* u = new CPluto;
			m_rpc.Encode(*u, msg_id, p1, p2, p3, p4, p5);
			u->SetMailbox(mb);
			pl.PushPluto(u);
		}
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	void world::SyncRpcCall(CPlutoList& pl, uint16_t nServerId, pluto_msgid_t msg_id, const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5, const T6& p6)
	{
		CEpollServer* s = this->GetServer();
		CMailBox* mb = s->GetServerMailbox(nServerId);
		if (mb)
		{
			CPluto* u = new CPluto;
			m_rpc.Encode(*u, msg_id, p1, p2, p3, p4, p6);
			u->SetMailbox(mb);
			pl.PushPluto(u);
		}
	}

}

#endif // !__WORLD__

