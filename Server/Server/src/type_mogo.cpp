/*----------------------------------------------------------------
// Copyright (C) 2013 广州，爱游
//
// 模块名：type_mogo
// 创建者：Steven Yang
// 修改者列表：
// 创建日期：2013.1.11
// 模块描述：def相关数据类型定义。
//----------------------------------------------------------------*/

#ifdef _WIN32
#include <winsock.h>
#endif
#include "type_mogo.h"
#include "pluto.h"
#include "util.h"


#ifdef _PLUTO_POOL
mogo::MemoryPool* mogo::VOBJECT::memPool = NULL;
mogo::MyLock mogo::VOBJECT::m_lock;
#endif

namespace mogo
{

	VOBJECT::~VOBJECT()
	{
		this->Clear();
	}

	void VOBJECT::Clear()
	{
		switch (vt)
		{
		case V_STR:
		{
			delete vv.s;
			break;
		}
		case V_BLOB:
		{
			delete (charArrayDummy*)vv.p;
			break;
		}
		case V_ENTITY:
		{
			delete (SEntityPropFromPluto*)vv.p;
			break;
		}

		}

		vt = V_UINT8;		//修改类型为没有清理操作的类型,以防重复清理
	}

#ifdef _PLUTO_POOL
	void * VOBJECT::operator new(size_t size)
	{

		m_lock.Lock();

		if (NULL == memPool)
		{
			expandMemoryPool();
		}

		MemoryPool *head = memPool;
		memPool = head->next;

		m_lock.Unlock();

		//LogDebug("VOBJECT new", "size=%d", size);

		return head;
	}

	void VOBJECT::operator delete(void* p, size_t size)
	{
		m_lock.Lock();

		MemoryPool *head = (MemoryPool *)p;
		head->next = memPool;
		memPool = head;

		m_lock.Unlock();

		//LogDebug("VOBJECT delete", "size=%d", size);
	}

	void VOBJECT::expandMemoryPool()
	{
		size_t size = (sizeof(VOBJECT) > sizeof(MemoryPool *)) ? sizeof(VOBJECT) : sizeof(MemoryPool *);

		MemoryPool *runner = (MemoryPool *) new char[size];
		memPool = runner;

		enum { EXPAND_SIZE = 32 };
		for (int i = 0; i<EXPAND_SIZE; i++)
		{
			runner->next = (MemoryPool *) new char[size];
			runner = runner->next;
		}

		runner->next = NULL;
	}
#endif


	void FillVObjectInitValue(const CEntityParent* pEntity, const string& strAttri, VOBJECT& v)
	{
		switch (v.vt)
		{
		case V_UINT8:
		{
			v.vv.u8 = (uint8_t)0;
			break;
		}
		case V_INT8:
		{
			v.vv.i8 = (int8_t)0;
			break;
		}
		case V_UINT16:
		{
			v.vv.u16 = (uint16_t)0;
			break;
		}
		case V_INT16:
		{
			v.vv.i16 = (int16_t)0;
			break;
		}
		case V_UINT32:
		{
			v.vv.u32 = (uint32_t)0;
			break;
		}
		case V_INT32:
		{
			v.vv.i32 = (int32_t)0;
			break;
		}
		case V_UINT64:
		{
			v.vv.u64 = (uint64_t)0;
			break;
		}
		case V_INT64:
		{
			v.vv.i64 = (int64_t)0;
			break;
		}
		case V_FLOAT32:
		{
			v.vv.f32 = (float32_t)0;
			break;
		}
		case V_FLOAT64:
		{
			v.vv.f64 = (float64_t)0;
			break;
		}
		case V_STR:
		{
			v.vv.s = new string("");
			break;
		}
		case V_BLOB:
		{
			v.vv.p = new charArrayDummy;
			break;
		}
		
		case V_LUA_OBJECT:
		{
			v.vv.i32 = -1;
			break;
		}
		//case V_REDIS_HASH:
		//{
		//	lua_State* L = GetWorld()->GetLuaState();
		//	CRedisHash* p = CreateRedisHash(L);
		//	p->SetEntity(pEntity, strAttri);
		//	v.vv.p = p;
		//	break;
		//}
		default:
		{
			//nothing to do
		}
		}
	}

	void FillVObjectDefaultValue(const CEntityParent* pEntity, const string& strAttri, VOBJECT& v, const string& strDefault)
	{
		if (strDefault.empty())
		{
			FillVObjectInitValue(pEntity, strAttri, v);
			return;
		}

		switch (v.vt)
		{
		case V_UINT8:
		{
			v.vv.u8 = (uint8_t)atoi(strDefault.c_str());
			break;
		}
		case V_INT8:
		{
			v.vv.i8 = (int8_t)atoi(strDefault.c_str());
			break;
		}
		case V_UINT16:
		{
			v.vv.u16 = (uint16_t)atoi(strDefault.c_str());
			break;
		}
		case V_INT16:
		{
			v.vv.i16 = (int16_t)atoi(strDefault.c_str());
			break;
		}
		case V_UINT32:
		{
			v.vv.u32 = (uint32_t)atoll(strDefault.c_str());
			break;
		}
		case V_INT32:
		{
			v.vv.i32 = (int32_t)atoll(strDefault.c_str());
			break;
		}
		case V_UINT64:
		{
			v.vv.u64 = (uint64_t)atoll(strDefault.c_str());
			break;
		}
		case V_INT64:
		{
			v.vv.i64 = (int64_t)atoll(strDefault.c_str());
			break;
		}
		case V_FLOAT32:
		{
			v.vv.f32 = (float32_t)atof(strDefault.c_str());
			break;
		}
		case V_FLOAT64:
		{
			v.vv.f64 = (float64_t)atof(strDefault.c_str());
			break;
		}
		case V_STR:
		{
			v.vv.s = new string(strDefault);
			break;
		}
		case V_BLOB:
		{
			v.vv.p = new charArrayDummy;
			break;
		}

		/*case V_REDIS_HASH:
		{
			lua_State* L = GetWorld()->GetLuaState();
			CRedisHash* p = CreateRedisHash(L);
			p->SetEntity(pEntity, strAttri);
			v.vv.p = p;
			break;
		}*/
		default:
		{
			//nothing to do
		}
		}

	}

	
};






