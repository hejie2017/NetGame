#include "entity.h"
#include "world_select.h"

namespace mogo {


	CEntityParent::CEntityParent(TENTITYTYPE etype, TENTITYID nid)
		: m_etype(etype), m_id(nid), m_mymb(), m_dbid(0)
	{
		//cout << "CEntity::CEntity: " << nid << endl;
	}

	CEntityParent::~CEntityParent()
	{
		//cout << "CEntity::~CEntity: " << m_id << endl;
		ClearMap(m_data);
	}

	bool CEntityParent::PickleToPluto(CPluto& u) const
	{
		const SEntityDef* pDef = this->GetEntityDef();
		if (pDef)
		{
			//u << m_mymb << m_dbid;   //不需要包含mb和dbid,这两个字段要分离出去
			u << m_etype;              //必须包含entity_type,以便接包时查询def数据
			map<string, _SEntityDefProperties*>::const_iterator iter = pDef->m_properties.begin();
			for (; iter != pDef->m_properties.end(); ++iter)
			{
				_SEntityDefProperties* p = iter->second;
				if (!p->m_bSaveDb)
				{
					//不需要存盘
					continue;
				}

				const string& strEntityName = iter->first;
				map<string, VOBJECT*>::const_iterator iter2 = m_data.find(strEntityName);
				if (iter2 == m_data.end())
				{
					//todo warning...
					continue;
				}

				//u << iter2->first.c_str();
				u << (uint16_t)(pDef->m_propertiesMap.GetIntByStr(iter2->first));
				u.FillPluto(*(iter2->second));
			}
			//print_hex_pluto(u);
		}

		return pDef != NULL;
	}

	const SEntityDef* CEntityParent::GetEntityDef() const
	{
		return GetWorld()->GetDefParser().GetEntityDefByType(m_etype);
	}

	bool CEntityParent::UnpickleFromPluto(CPluto& u)
	{
		VOBJECT v;
		v.vt = V_ENTITY;
		bool b = u.UnpickleEntity(v);
		return b;
	}

	//刷新entity的属性,数据一般来自数据库
	void CEntityParent::UpdateProps(TDBID dbid, SEntityPropFromPluto* p)
	{
		m_dbid = dbid;

		map<string, VOBJECT*>& new_data = p->data;
		map<string, VOBJECT*>::iterator iter = new_data.begin();
		for (; iter != new_data.end(); ++iter)
		{
			map<string, VOBJECT*>::iterator iter2 = m_data.find(iter->first);
			if (iter2 != m_data.end())
			{
				//交换指针
				VOBJECT* ptmp = iter2->second;
				iter2->second = iter->second;
				iter->second = ptmp;
			}
		}
	}

	//刷新entity的属性,数据由一个table给出
	void CEntityParent::UpdateProps(map<string, VOBJECT*>& new_data)
	{
		map<string, VOBJECT*>::iterator iter = new_data.begin();
		for (; iter != new_data.end(); ++iter)
		{
			map<string, VOBJECT*>::iterator iter2 = m_data.find(iter->first);
			if (iter2 != m_data.end())
			{
				//交换指针
				VOBJECT* ptmp = iter2->second;
				iter2->second = iter->second;
				iter->second = ptmp;
			}
		}
	}


	//刷新一个属性
	VOBJECT* CEntityParent::UpdateAProp(const string& strPropName, VOBJECT* v, bool& bUpdate)
	{
		map<string, VOBJECT*>::iterator iter = m_data.find(strPropName);
		if (iter == m_data.end())
		{
			bUpdate = false;
			return v;
		}

		VOBJECT* v2 = iter->second;
		iter->second = v;
		bUpdate = true;
		return v2;
	}


}