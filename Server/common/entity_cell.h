#ifndef __ENTITY_CELL__
#define __ENTITY_CELL__

#include "entity.h"
#include "aoi.h"


namespace mogo
{

	class CSpace;

	class CEntityCell : public CEntityParent
	{
	public:
		CEntityCell(TENTITYTYPE etype, TENTITYID nid);
		~CEntityCell();
	public:
		//有新的entity进入aoi
		void OnEnterAoi(CSpace* sp, TENTITYID eid);
		//检查关注者是否离开了aoi
		bool CheckLeaveAoi();
		//有entity离开了aoi
		void OnLeaveAoi(TENTITYID eid, bool bIsNest);
		//清除所有的关注者
		void ClearAoiEntities();
		void PickleOtherClientToPluto(CPluto& u);
		CSpace* GetMySpace();
		CMailBox* GetBaseMailboxIfClient();
		bool BroadcastPos();
		CPluto* CEntityCell::SendOtherEntityPos(CPluto* u, TENTITYID eid, int16_t x, int16_t y, uint32_t checkFlag,
			pluto_msgid_t selfCPlutoHead, pluto_msgid_t otherCPlutoHead);
		bool OnMoveTick();

		bool AddInFollowers(TENTITYID eid);
		bool AddInObservers(TENTITYID eid);
		bool IsObserversFull();
		bool RemoveInFollower(TENTITYID eid);

	public:
		inline TSPACEID GetSpaceID() const
		{
			return m_spaceId;
		}

		inline void SetSpaceID(TSPACEID sid)
		{
			m_spaceId = sid;
		}

		int GetBaseServerId();
		
	protected:
		uint16_t m_nBaseSvrId;

		TSPACEID m_spaceId;

		//AOI模块实际计算出的关注列表
		set<TENTITYID> m_entitiesIds;

		////考虑到网络问题而裁剪出来的实际广播用的关注列表
		//set<TENTITYID> m_observersIds;

		set<TENTITYID> m_followers;    //被关注者列表，意思是被该vector里面的人关注，相当于粉丝列表(默认30人)
		set<TENTITYID> m_observers;    //关注者列表，意思是成功关注的人(默认15人)


	public:
		position_t m_pos[2];                                    //位置坐标
		uint8_t face;                                           //朝向

		list<std::pair<int16_t, int16_t>*>* m_lsPosPairs;       //路点坐标
		uint16_t m_nSpeed;                                      //速度,每一个移动tick里可以移动的距离
																//bool m_bCanMove;                                      //是否可以移动标记
		bool m_bBroadcast;                                      //本次tick是否需要广播给关注者
		bool m_bMoved;                                          //本次tick是否发生移动行为

		int lastMoveTime;                                       //上一次移动时间
		uint8_t badMoveTimes;                                   //移动不良记录的次数
	};
}

#endif // !__ENTITY_CELL__
