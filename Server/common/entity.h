#ifndef __ENTITY__
#define __ENTITY__

#include "type_mogo.h"
#include "pluto.h"
#include "my_stl.h"
#include "defparser.h"

namespace mogo {

	class CEntityParent
	{
	public:
		CEntityParent(TENTITYTYPE etype, TENTITYID nid);
		virtual ~CEntityParent();

		bool PickleToPluto(CPluto& u) const;
		//ˢ��entity������,����һ���������ݿ�
		void UpdateProps(TDBID dbid, SEntityPropFromPluto* p);
		//ˢ��entity������,������һ��table����
		void UpdateProps(map<string, VOBJECT*>& new_data);

		//ˢ��һ������
		VOBJECT* UpdateAProp(const string& strPropName, VOBJECT* v, bool& bUpdate);
	public:
		inline TENTITYID GetId() const
		{
			return m_id;
		}
	protected:
		const SEntityDef* GetEntityDef() const;
		bool UnpickleFromPluto(CPluto& u);

	protected:
		TENTITYID m_id;
		TDBID m_dbid;
		TENTITYTYPE m_etype;
		map<string, VOBJECT*> m_data;
		CEntityMailbox m_mymb;
		bool m_bHasClient;     //�Ƿ�ӵ�пͻ���
		bool m_bIsBase;     //true:base false:cell
	};

}

#endif // !__ENTITY__