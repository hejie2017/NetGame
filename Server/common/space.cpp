/*----------------------------------------------------------------
// Copyright (C) 2013 ���ݣ�����
//
// ģ������space
// �����ߣ�Steven Yang
// �޸����б�
// �������ڣ�2013.1.14
// ģ��������������ط�װ
//----------------------------------------------------------------*/

#include <utility>
#include <algorithm>
#include "space.h"
#include "my_stl.h"
#include "../cell/world_cell.h"
#include "world_select.h"



namespace mogo
{

	struct alloc_cookie
	{
		int count;
		int max;
		int current;
	};

	static void *
		MyAlloc(void * ud, void *ptr, size_t sz)
	{
		if (ptr == NULL)
		{
			char* p = new char[sz];
			return p;
		}

		delete[](char*)ptr;
		return NULL;
	}


	CSpace::CSpace(TSPACEID id) : m_id(id), m_enMgr(), m_bAoiDirty(false)

#ifdef __TEST
		, broadcastPosNum(0), moveTicks(0)
#endif

	{
		//struct alloc_cookie cookie = { 0,0,0 };
		m_aoiMgr = AoiCreate(MyAlloc, (void*)0);
	}

	CSpace::~CSpace()
	{
		AoiRelease(m_aoiMgr);
	}

	bool CSpace::AddEntity(position_t x, position_t y, CEntityCell* p)
	{
		bool b = m_enMgr.AddEntity(p);
		if (b)
		{
			p->SetSpaceID(m_id);
			p->m_pos[0] = x;
			p->m_pos[1] = y;

			//printf("addEntity,%u,%u,%u\n", p->GetId(), p->m_pos[0], p->m_pos[1]);
			AoiUpdate(m_aoiMgr, p->GetId(), "wm", p->m_pos);
			m_bAoiDirty = true;
		}

		return b;
	}

	bool CSpace::DelEntity(CEntityCell* p)
	{
		m_enMgr.DelEntity(p);

		//�����ע��
		p->ClearAoiEntities();

		//֪ͨaoi������,�������ϴ���aoiEvent
		AoiUpdate(m_aoiMgr, p->GetId(), "d", p->m_pos);
		m_bAoiDirty = true;

		return true;
	}

	static void OnEnterAoi(void* ud, uint32_t watcher, uint32_t marker)
	{
		CSpace* sp = (CSpace*)ud;
		CEntityCell* p1 = sp->GetEntity(watcher);
		if (p1)
		{
			p1->OnEnterAoi(sp, marker);
		}
	}

	//����aoi�¼�
	void CSpace::AoiEvent()
	{
		AoiMessage(m_aoiMgr, OnEnterAoi, this);
		m_bAoiDirty = false;
	}

	void CSpace::OnPosMove(CEntityCell* p)
	{
		AoiUpdate(m_aoiMgr, p->GetId(), "wm", p->m_pos);
		m_bAoiDirty = true;
	}

	//����������ת
	void CSpace::TelePortLocally(CEntityCell* p, position_t x1, position_t y1)
	{
		p->m_pos[0] = x1;
		p->m_pos[1] = y1;

		OnPosMove(p);
	}

	void CSpace::AllEntitiesMove()
	{
#ifdef __TEST
		this->moveTicks++;
#endif
		map<TENTITYID, CEntityCell*>& entities = m_enMgr.Entities();
		map<TENTITYID, CEntityCell*>::iterator iter = entities.begin();
		for (; iter != entities.end(); ++iter)
		{

#ifdef __TEST

			if (iter->second->BroadcastPos())
			{
				this->broadcastPosNum++;
			}

			if (this->moveTicks == 100)
			{
				LogDebug("CSpace::AllEntitiesMove", "broadcastPosNum=%d", (this->broadcastPosNum / 100));
				this->broadcastPosNum = 0;
				this->moveTicks = 0;
			}
#else
			iter->second->BroadcastPos();
#endif

			iter->second->OnMoveTick();
		}
	}

	void CSpace::AllEntitiesCheckLeaveAoi()
	{
		map<TENTITYID, CEntityCell*>& entities = m_enMgr.Entities();
		map<TENTITYID, CEntityCell*>::iterator iter = entities.begin();
		//iter = entities.begin();
		for (; iter != entities.end(); ++iter)
		{
			iter->second->CheckLeaveAoi();
		}
	}

}

