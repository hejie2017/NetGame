#include "entity_cell.h"
#include "world_select.h"
#include "rpc_mogo.h"
#include "pluto.h"


inline void FormatPos(float x, int16_t& x2)
{
	if (x < 0.0f)
	{
		x2 = 0;
	}
	else
	{
		x2 = (int16_t)x;
	}
}

float _move(int16_t x1, int16_t y1, int16_t x2, int16_t y2, float speed, int16_t& x3, int16_t& y3)
{
	int32_t dx = (int32_t)x2 - (int32_t)x1;
	int32_t dy = (int32_t)y2 - (int32_t)y1;

	int32_t d = dx*dx + dy*dy;
	float speed_square = speed * speed;
	if (speed_square > d)
	{
		//�ٶȴ��ڼ������,����Ҫ����һ��·��
		x3 = x2;
		y3 = y2;
		return speed - sqrt((float)d);
	}

	float sloop = sqrt((float)d);
	//FormatPos(x1 + (dx/sloop)*speed, x3);kevin modified
	//FormatPos(y1 + (dy/sloop)*speed, y3);kevin modified
	x3 = x1 + (dx / sloop)*speed;
	y3 = y1 + (dy / sloop)*speed;

	return 0.0f;
}


CEntityCell::CEntityCell(TENTITYTYPE etype, TENTITYID nid)
	: CEntityParent(etype, nid), m_spaceId(0)

	{
		m_pos[0] = 0;
		m_pos[1] = 0;
	}


CEntityCell::~CEntityCell()
{

}

//��ȡbase��server_id
int CEntityCell::GetBaseServerId()
{
	if (m_nBaseSvrId == 0)
	{
		return -2;
	}

	return m_nBaseSvrId;
}

//���µ�entity����aoi
void CEntityCell::OnEnterAoi(CSpace* sp, TENTITYID eid)
{
	//LogDebug("CEntityCell::OnEnterAoi", "eid=%d;m_id=%d", eid, this->m_id);
	CEntityCell* p2 = sp->GetEntity(eid);
	if (p2)
	{
		set<TENTITYID>::iterator iter = m_entitiesIds.lower_bound(eid);
		if (iter != m_entitiesIds.end() && *iter == eid)
		{
			//�Ѿ�������
			//LogWarning("OnEnterAoi", "dup=%u", p2->GetId());
		}
		else
		{
			m_entitiesIds.insert(iter, eid);

			if (m_bHasClient)
			{
				//����пͻ���,���½���entity��other_CLIENTS���Է����ͻ���
				int nBaseId = GetBaseServerId();
				if (nBaseId > 0)
				{
					CMailBox* mb = GetWorld()->GetServerMailbox(nBaseId);
					if (mb)
					{
						//�½���entity������
						CPluto* u = new CPluto;
						CPluto& uu = *u;
						uu.Encode(MSGID_BASEAPP_CLIENT_MSG_VIA_BASE) << m_id << (uint16_t)MSGID_CLIENT_AOI_NEW_ENTITY;

						uint32_t idx1 = uu.GetLen();
						uu << (uint16_t)0;                                    //����ռλ

						p2->PickleOtherClientToPluto(uu);                //�������

						uint32_t idx2 = uu.GetLen();
						uu.ReplaceField<uint16_t>(idx1, idx2 - idx1 - 2);        //�滻Ϊ�����ĳ���
						uu << EndPluto;

						//LogDebug("CEntityCell::OnEnterAoi", "m_id=%d;eid=%d;u->GetLen()=%d;", m_id, eid, u->GetLen());

						mb->PushPluto(u);

					}
				}

			}
		}
	}

}

//����other_clients��ǵ����Դ����pluto
void CEntityCell::PickleOtherClientToPluto(CPluto& u)
{
	//���entity����������һ��client�ɼ���cell����
	const SEntityDef* pDef = this->GetEntityDef();
	//if(pDef && pDef->m_bHasCellClient)
	if (pDef)
	{

#ifdef __FACE
		u << m_etype << m_id << face << m_pos[0] << m_pos[1];
#else
		u << m_etype << m_id << m_pos[0] << m_pos[1];
#endif

		//������entity������client�ɼ���cell����
		map<string, _SEntityDefProperties*>::const_iterator iter = pDef->m_properties.begin();
		for (; iter != pDef->m_properties.end(); ++iter)
		{
			_SEntityDefProperties* p = iter->second;
			if (IsOtherClientsFlag(p->m_nFlags))
			{
				const string& strEntityName = iter->first;
				map<string, VOBJECT*>::const_iterator iter2 = m_data.find(strEntityName);
				if (iter2 == m_data.end())
				{
					//todo warning...
					continue;
				}

				u << (uint16_t)(pDef->m_propertiesMap.GetIntByStr(iter2->first));
				u.FillPluto(*(iter2->second));
			}
		}
	}
}

CSpace* CEntityCell::GetMySpace()
{
	return GetWorldcell().GetSpace(m_spaceId);
}

//����ע���Ƿ��뿪��aoi
bool CEntityCell::CheckLeaveAoi()
{
	if (!m_bMoved)
	{
		//�����ƶ���
		return false;
	}

	m_bMoved = false;

	CSpace* sp = GetMySpace();
	if (sp == NULL)
	{
		return false;
	}

	bool bLeave = false;
	list<TENTITYID> left_ids;    //�Ѿ��뿪�Ĺ�ע��
	set<TENTITYID>::iterator iter = m_entitiesIds.begin();
	for (; iter != m_entitiesIds.end(); ++iter)
	{
		TENTITYID eid = *iter;
		CEntityCell* p2 = sp->GetEntity(eid);
		if (p2 == NULL)
		{
			//�Ѿ��뿪�˱�space
			this->OnLeaveAoi(eid, true);
			left_ids.push_back(eid);
			bLeave = true;
		}
		else
		{
			if (IsOutOfAoi(m_pos, p2->m_pos))
			{
				//�뿪��aoi
				this->OnLeaveAoi(eid, true);
				p2->OnLeaveAoi(m_id, false);
				left_ids.push_back(eid);
				bLeave = true;
			}
		}
	}

	while (!left_ids.empty())
	{
		TENTITYID eid2 = left_ids.front();
		m_entitiesIds.erase(eid2);
		left_ids.pop_front();
	}

	return bLeave;
}

bool CEntityCell::AddInFollowers(TENTITYID eid)
{
	//uint16_t MaxFollowerCount = atoi(GetWorld()->GetCfgReader()->GetOptValue("params", "max_follower_count", "30").c_str());

	set<TENTITYID>::iterator iter = this->m_followers.find(eid);
	if ((this->m_followers.size() >= GetWorldcell().GetMaxFollowerCount()) || (iter != this->m_followers.end()))
	{
		return false;
	}
	this->m_followers.insert(iter, eid);
	return true;
}

bool CEntityCell::AddInObservers(TENTITYID eid)
{
	set<TENTITYID>::iterator iter = this->m_observers.find(eid);

	//uint16_t MaxObserverCount = atoi(GetWorld()->GetCfgReader()->GetOptValue("params", "max_observer_count", "15").c_str());

	if ((this->m_observers.size() >= GetWorldcell().GetMaxObserverCount()) || (iter != this->m_observers.end()))
	{
		return false;
	}
	this->m_observers.insert(iter, eid);
	return true;
}

bool CEntityCell::IsObserversFull()
{
	//uint16_t MaxObserverCount = atoi(GetWorld()->GetCfgReader()->GetOptValue("params", "max_observer_count", "15").c_str());

	return this->m_observers.size() >= GetWorldcell().GetMaxObserverCount();
}

bool CEntityCell::RemoveInFollower(TENTITYID eid)
{
	set<TENTITYID>::iterator iter = this->m_followers.find(eid);
	if (iter != this->m_followers.end())
	{
		this->m_followers.erase(iter);
		return true;
	}
	return false;
}

//��entity�뿪��aoi;bIsNestΪtrue��ʾǶ�׵�����,����erase id
void CEntityCell::OnLeaveAoi(TENTITYID eid, bool bIsNest)
{
	bool bFound = false;
	if (bIsNest)
	{
		bFound = true;
	}
	else
	{
		set<TENTITYID>::iterator iter = m_entitiesIds.find(eid);
		if (iter != m_entitiesIds.end())
		{
			bFound = true;
			m_entitiesIds.erase(iter);
		}
	}

	CSpace *sp = this->GetMySpace();
	if (!sp)
	{
		return;
	}

	set<TENTITYID>::iterator iter2 = m_observers.find(eid);
	if (iter2 != m_observers.end())
	{
		//������Ϣ���ͻ���
		//printf("%u say: %u leave aoi\n", m_id, eid);
		//LogDebug("OnLeaveAoi", "watcher=%u;leave=%u", m_id, eid);

		CMailBox* mb = GetBaseMailboxIfClient();
		if (mb)
		{
			CPluto* u = new CPluto;
			u->Encode(MSGID_BASEAPP_CLIENT_MSG_VIA_BASE) << m_id << (uint16_t)MSGID_CLIENT_AOI_DEL_ENTITY
				<< (uint16_t)sizeof(eid) << eid << EndPluto;

			//LogDebug("CEntityCell::OnLeaveAoi", "u.GenLen()=%d", u->GetLen());

			mb->PushPluto(u);
		}

		m_observers.erase(iter2);

		if (this->IsObserversFull())
		{
			return;
		}

		set<TENTITYID>::iterator iter3 = m_entitiesIds.begin();
		for (; iter3 != m_entitiesIds.end(); ++iter3)
		{
			CEntityCell* p2 = sp->GetEntity(*iter3);

			if (p2 && p2->AddInFollowers(m_id) && this->AddInObservers(*iter3))
			{
				if (m_bHasClient)
				{
					//����пͻ���,���½���entity��other_CLIENTS���Է����ͻ���
					int nBaseId = GetBaseServerId();
					if (nBaseId > 0)
					{
						CMailBox* mb = GetWorld()->GetServerMailbox(nBaseId);
						if (mb)
						{
							//�½���entity������
							CPluto* u = new CPluto;
							CPluto& uu = *u;
							uu.Encode(MSGID_BASEAPP_CLIENT_MSG_VIA_BASE) << m_id << (uint16_t)MSGID_CLIENT_AOI_NEW_ENTITY;

							uint32_t idx1 = uu.GetLen();
							uu << (uint16_t)0;                                    //����ռλ

							p2->PickleOtherClientToPluto(uu);                //�������

							uint32_t idx2 = uu.GetLen();
							uu.ReplaceField<uint16_t>(idx1, idx2 - idx1 - 2);        //�滻Ϊ�����ĳ���
							uu << EndPluto;

							//LogDebug("CEntityCell::OnLeaveAoi Add", "m_id=%d;id=%d", m_id, *iter3);

							mb->PushPluto(u);


							////�½���entity��·����Ϣ
							//if(p2->GetLeftPosPairs())
							//{
							//    SendOtherClientMoveReq(NULL, p2->GetId(), *(p2->GetLeftPosPairs()) );
							//}
						}
					}

				}
				return;
			}
		}
	}

}

//����пͻ������ȡ������base��mailbox
CMailBox* CEntityCell::GetBaseMailboxIfClient()
{
	if (!m_bHasClient)
	{
		return NULL;
	}

	int nBaseId = GetBaseServerId();
	if (nBaseId <= 0)
	{
		return NULL;
	}

	CMailBox* mb = GetWorld()->GetServerMailbox(nBaseId);
	return mb;
}

//������еĹ�ע��
void CEntityCell::ClearAoiEntities()
{
	CSpace* sp = GetMySpace();
	if (sp)
	{
		//֪ͨ��ע�����뿪��;�����֪ͨ,����ע��û���ƶ�ʱ,�޷���⵽�ҵ��뿪
		set<TENTITYID>::iterator iter = m_entitiesIds.begin();

#ifdef __AOI_PRUNING
		for (; iter != m_entitiesIds.end(); ++iter)
		{
			CEntityCell* p2 = sp->GetEntity(*iter);
			if (p2)
			{
				p2->RemoveInFollower(m_id);
			}
		}

		iter = m_entitiesIds.begin();
#endif

		for (; iter != m_entitiesIds.end(); ++iter)
		{
			CEntityCell* p2 = sp->GetEntity(*iter);
			if (p2)
			{
				p2->OnLeaveAoi(m_id, false);
			}
		}
	}

	//m_observersIds.clear();

	//������еĹ�ע��
	m_entitiesIds.clear();

#ifdef __AOI_PRUNING
	m_followers.clear();
	m_observers.clear();
#endif

}

bool CEntityCell::BroadcastPos()
{
	if (!m_bBroadcast)
	{
		return false;
	}

	m_bBroadcast = false;

	//ͬ�������aoi��Χ���������

	CSpace* sp = GetMySpace();
	if (sp)
	{
		CPluto* u = NULL;
		CEntityCell* p2 = NULL;
		CPluto* u2 = NULL;

#ifdef __AOI_PRUNING
		set<TENTITYID>::iterator iter = m_followers.begin();
		for (; iter != m_followers.end(); ++iter)
#else
		set<TENTITYID>::iterator iter = m_entitiesIds.begin();
		for (; iter != m_entitiesIds.end(); ++iter)
#endif

		{
			p2 = sp->GetEntity(*iter);
			if (p2)
			{

#ifdef __FACE
				u2 = p2->SendOtherEntityPos(u, m_id, face, this->m_pos[0], this->m_pos[1],
					MSGID_CLIENT_ENTITY_POS_SYNC, MSGID_CLIENT_OTHER_ENTITY_POS_SYNC);
#else
				u2 = p2->SendOtherEntityPos(u, m_id, this->m_pos[0], this->m_pos[1], 0,
					MSGID_CLIENT_ENTITY_POS_SYNC, MSGID_CLIENT_OTHER_ENTITY_POS_SYNC);
#endif

				if (u2)
				{
					u = u2;
				}
			}
		}

		//ˢ���Լ��������aoi������
		sp->OnPosMove(this);
	}

	CMailBox* mb = GetBaseMailboxIfClient();
	if (mb)
	{
		CPluto *u = new CPluto;
#ifdef __FACE
		u->Encode(MSGID_BASEAPP_AVATAR_POS_SYNC) << (uint16_t)MSGID_CLIENT_ENTITY_POS_SYNC << m_id << face << this->m_pos[0] << this->m_pos[1] << (uint8_t)0 << EndPluto;
#else
		u->Encode(MSGID_BASEAPP_AVATAR_POS_SYNC) << (uint16_t)MSGID_CLIENT_ENTITY_POS_SYNC << m_id << this->m_pos[0] << this->m_pos[1] << (uint8_t)0 << EndPluto;
#endif

		mb->PushPluto(u);
	}


	return true;

}

//��entity������ͬ����aoi��Χ�ڵ����
CPluto* CEntityCell::SendOtherEntityPos(CPluto* u, TENTITYID eid, int16_t x, int16_t y, uint32_t checkFlag,
	pluto_msgid_t selfCPlutoHead, pluto_msgid_t otherCPlutoHead)

{
	CMailBox* mb = GetBaseMailboxIfClient();
	if (mb)
	{
		if (eid == m_id)
		{
			//�����Լ��Ŀͻ���,���⴦��,���Դ����u(����ʱ��NULL)
			u = new CPluto;

			u->Encode(MSGID_BASEAPP_AVATAR_POS_SYNC) << (uint16_t)selfCPlutoHead << m_id << x << y << (uint8_t)1 << EndPluto;

			mb->PushPluto(u);

			return NULL;
		}

		if (u == NULL)
		{
			//��һ�����,�����ĸ���
			u = new CPluto;

			u->Encode(MSGID_BASEAPP_CLIENT_MSG_VIA_BASE) << m_id << (uint16_t)otherCPlutoHead
				<< (uint16_t)(sizeof(eid) + sizeof(x) + sizeof(y) + sizeof(checkFlag))        //����Ԥ֪����
				<< eid << x << y << checkFlag                                  //��������
				<< EndPluto;

			//LogDebug("CEntityCell::SendOtherEntityPos 2", "u.GenLen()=%d;msg_id=%d;face=%d;x=%d;y=%d",
			//                                               u->GetLen(), otherCPlutoHead, newFace, x, y);

			mb->PushPluto(u);

			return u;
		}
		else
		{
			CPluto* u2 = new CPluto(u->GetBuff(), u->GetLen());
			u2->ReplaceField<uint32_t>(8, m_id);

			//LogDebug("CEntityCell::SendOtherEntityPos 3", "u2.GenLen()=%d;msg_id=%d;face=%d;x=%d;y=%d",
			//                                               u2->GetLen(), otherCPlutoHead, newFace, x, y);

			mb->PushPluto(u2);

			return NULL;
		}
	}

	return NULL;
}

//����true��ʾ�ƶ���
bool CEntityCell::OnMoveTick()
{
	//����������²����������
	//        printf("OnMoveTick %d\n", m_id);
	if (m_lsPosPairs == NULL || m_lsPosPairs->empty())
	{
		return false;
	}

	//��ǰ����
	int16_t x1 = m_pos[0];
	int16_t y1 = m_pos[1];
	//�����ƶ����ߵ���Ŀ������
	int16_t x3;
	int16_t y3;
	//ʣ���ٶ�ֵ
	float speed = (float)m_nSpeed;

	while (!(m_lsPosPairs->empty()))
	{
		pair<int16_t, int16_t>* pNextPos = m_lsPosPairs->front();

		speed = _move(x1, y1, pNextPos->first, pNextPos->second, speed, x3, y3);
		if (speed > 0)
		{
			//��ǰ·���Ѿ�����,ɾ��
			m_lsPosPairs->pop_front();
			delete pNextPos;

			//����Ҫ����һ��·��
			x1 = x3;
			y1 = y3;

			continue;
		}
		else
		{
			//��ǰ·��û������,��ɾ��,�´ν�����
			break;
		}
	}

	//�����µ������
	m_pos[0] = x3;
	m_pos[1] = y3;
	//   printf("new pos, %d-%d\n", m_pos[0], m_pos[1]);


	//ͬ�������aoi��Χ���������
	CSpace* sp = GetMySpace();
	if (sp && sp->GetEntity(this->GetId()) == this)
	{
		//ˢ���Լ��������aoi������
		sp->OnPosMove(this);
	}

	m_bMoved = true;        //�����ƶ����˵ı��
	return true;
}
