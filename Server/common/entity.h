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
		//刷新entity的属性,数据一般来自数据库
		void UpdateProps(TDBID dbid, SEntityPropFromPluto* p);
		//刷新entity的属性,数据由一个table给出
		void UpdateProps(map<string, VOBJECT*>& new_data);

		//刷新一个属性
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
		bool m_bHasClient;     //是否拥有客户端
		bool m_bIsBase;     //true:base false:cell
	};

}

#endif // !__ENTITY__