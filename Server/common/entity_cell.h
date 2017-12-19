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
		//���µ�entity����aoi
		void OnEnterAoi(CSpace* sp, TENTITYID eid);
		//����ע���Ƿ��뿪��aoi
		bool CheckLeaveAoi();
		//��entity�뿪��aoi
		void OnLeaveAoi(TENTITYID eid, bool bIsNest);
		//������еĹ�ע��
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

		//AOIģ��ʵ�ʼ�����Ĺ�ע�б�
		set<TENTITYID> m_entitiesIds;

		////���ǵ�����������ü�������ʵ�ʹ㲥�õĹ�ע�б�
		//set<TENTITYID> m_observersIds;

		set<TENTITYID> m_followers;    //����ע���б���˼�Ǳ���vector������˹�ע���൱�ڷ�˿�б�(Ĭ��30��)
		set<TENTITYID> m_observers;    //��ע���б���˼�ǳɹ���ע����(Ĭ��15��)


	public:
		position_t m_pos[2];                                    //λ������
		uint8_t face;                                           //����

		list<std::pair<int16_t, int16_t>*>* m_lsPosPairs;       //·������
		uint16_t m_nSpeed;                                      //�ٶ�,ÿһ���ƶ�tick������ƶ��ľ���
																//bool m_bCanMove;                                      //�Ƿ�����ƶ����
		bool m_bBroadcast;                                      //����tick�Ƿ���Ҫ�㲥����ע��
		bool m_bMoved;                                          //����tick�Ƿ����ƶ���Ϊ

		int lastMoveTime;                                       //��һ���ƶ�ʱ��
		uint8_t badMoveTimes;                                   //�ƶ�������¼�Ĵ���
	};
}

#endif // !__ENTITY_CELL__
