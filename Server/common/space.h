#ifndef __SPACE_HEAD__
#define __SPACE_HEAD__


#include "type_mogo.h"
#include "my_stl.h"
#include "entity_cell.h"
#include "entity_mgr.h"
#include "aoi.h"
//#include "event.h"

namespace mogo
{


	class CSpace
	{
	public:
		CSpace(TSPACEID id);
		~CSpace();

	public:
		bool AddEntity(position_t x, position_t y, CEntityCell* p);
		bool DelEntity(CEntityCell* p);

	public:
		//����aoi�¼�
		void AoiEvent();

		void OnPosMove(CEntityCell* p);

		//space������entity�ƶ�
		void AllEntitiesMove();

		//space������entity�����뿪�¼�
		void AllEntitiesCheckLeaveAoi();

	public:
		void TelePortLocally(CEntityCell* p, position_t x1, position_t y1);

	public:
		inline TSPACEID GetId() const
		{
			return m_id;
		}
		inline uint32_t Size() const
		{
			return (uint32_t)m_enMgr.Size();
		}

		inline CEntityCell* GetEntity(TENTITYID eid)
		{
			return m_enMgr.GetEntity(eid);
		}

		inline bool IsAoiDirty() const
		{
			return m_bAoiDirty;
		}

	private:
		TSPACEID m_id;
		CEntityMgr<CEntityCell> m_enMgr;
		aoi_space* m_aoiMgr;
		bool m_bAoiDirty;           //�Ƿ���Ҫ����aoi�¼�������

		CSpace(const CSpace&);
		CSpace& operator=(const CSpace&);

#ifdef __TEST
		uint64_t broadcastPosNum;
		uint8_t moveTicks;
#endif


	};



}





#endif

