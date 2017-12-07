/*----------------------------------------------------------------
// Copyright (C) 2013 ���ݣ�����
//
// ģ������defparse
// �����ߣ�Steven Yang
// �޸����б�
// �������ڣ�2013.1.5
// ģ��������def �ļ�������
//----------------------------------------------------------------*/
//
#include "defparser.h"
#include "pluto.h"
//#include "logger.h"

#ifndef _WIN32
#include <openssl/md5.h>
#endif


const string EMPTY_STRING = "";

namespace mogo
{


	CStr2IntMap::CStr2IntMap() : m_nNextId(0)
	{
	}

	CStr2IntMap::~CStr2IntMap()
	{
	}

	void CStr2IntMap::AddNewString(const string& s)
	{
		m_str2Int.insert(make_pair(s, m_nNextId));
		m_int2Str.push_back(s);
		++m_nNextId;
	}

	//Ԥ�ȷ���n��ȱʡֵ
	void CStr2IntMap::Assign(uint32_t n)
	{
		for (uint32_t i = 0; i < n; ++i)
		{
			AddNewString("");
		}
	}


	//ֱ��ָ��һ��id��string��ӳ��
	void CStr2IntMap::SetNewString(uint32_t uid, const string& s)
	{
		m_str2Int.insert(make_pair(s, uid));
		m_int2Str[uid] = s;
	}


	const string& CStr2IntMap::GetStrByInt(uint32_t n) const
	{
		if (n < m_nNextId)
		{
			return m_int2Str[n];
		}
		return EMPTY_STRING;
	}

	int32_t CStr2IntMap::GetIntByStr(const string& s) const
	{
		map<string, uint32_t>::const_iterator iter = m_str2Int.find(s);
		if (iter == m_str2Int.end())
		{
			return -1;
		}
		return iter->second;
	}


	//////////////////////////////////////////////////////////////////////////////////////////

	_SEntityDefProperties::_SEntityDefProperties()
	{

	}

	_SEntityDefProperties::_SEntityDefProperties(const _SEntityDefProperties& other)
	{
		this->m_name = other.m_name;
		this->m_nType = other.m_nType;
		this->m_nFlags = other.m_nFlags;
		this->m_bSaveDb = other.m_bSaveDb;
		this->m_defaultValue = other.m_defaultValue;
	}

	_SEntityDefMethods::_SEntityDefMethods()
	{

	}

	_SEntityDefMethods::_SEntityDefMethods(const _SEntityDefMethods& other)
	{
		this->m_nServerId = other.m_nServerId;
		this->m_bExposed = other.m_bExposed;
		this->m_funcName = other.m_funcName;

		list<VTYPE>::const_iterator iter = other.m_argsType.begin();
		for (; iter != other.m_argsType.end(); ++iter)
		{
			this->m_argsType.push_back(*iter);
		}
	}

	SEntityDef::~SEntityDef()
	{
		ClearMap(m_properties);
		ClearMap(m_clientMethods);
		ClearMap(m_baseMethods);
		ClearMap(m_cellMethods);
	}


	CDefParser::CDefParser() :m_maxtype(0), m_typeInt2Str(NULL)
	{
		InitData();
	}

	CDefParser::~CDefParser()
	{
		delete[] m_typeInt2Str;
		ClearMap(m_entityDefs);
	}

	void CDefParser::InitData()
	{
		m_propTypeStr2Int.insert(make_pair("LUA_TABLE", V_LUATABLE));
		m_propTypeStr2Int.insert(make_pair("INT8", V_INT8));
		m_propTypeStr2Int.insert(make_pair("UINT8", V_UINT8));
		m_propTypeStr2Int.insert(make_pair("INT16", V_INT16));
		m_propTypeStr2Int.insert(make_pair("UINT16", V_UINT16));
		m_propTypeStr2Int.insert(make_pair("INT32", V_INT32));
		m_propTypeStr2Int.insert(make_pair("UINT32", V_UINT32));
		m_propTypeStr2Int.insert(make_pair("INT64", V_INT64));
		m_propTypeStr2Int.insert(make_pair("UINT64", V_UINT64));
		m_propTypeStr2Int.insert(make_pair("FLOAT", V_FLOAT32));
		m_propTypeStr2Int.insert(make_pair("FLOAT64", V_FLOAT64));
		m_propTypeStr2Int.insert(make_pair("STRING", V_STR));
		m_propTypeStr2Int.insert(make_pair("BLOB", V_BLOB));
		m_propTypeStr2Int.insert(make_pair("LUA_OBJECT", V_LUA_OBJECT));
		m_propTypeStr2Int.insert(make_pair("REDIS_HASH", V_REDIS_HASH));
		//m_propTypeStr2Int.insert(make_pair("MAILBOX", V_ENTITYMB));

	}

	//���������ļ��е��ַ���������Ӧ�����Ͷ���
	VTYPE CDefParser::GetVTypeByStr(const string& s)
	{
		map<string, VTYPE>::const_iterator iter = m_propTypeStr2Int.find(s);
		if (iter != m_propTypeStr2Int.end())
		{
			return iter->second;
		}
		else
		{
			return V_TYPE_ERR;
		}
	}
	//����entities.xml�ļ���ȡ������ʵ���������,��ŵ�ʵ�����ƺ�������ɵ�ӳ��ṹ��
	//Ȼ�����ʵ��������ʵ����������ļ����ݣ�������ת��Ϊʵ�嶨�����
	void CDefParser::init(const char* pszDefFilePath)
	{
		/*{
			ostringstream oss;
			oss << pszDefFilePath << g_cPathSplit << "entities.xml";
			ParseEntitiesXml(oss.str().c_str());
		}
		LogDebug("CDefParser::init", "==========");
		m_md5.reset();
		map<string, TENTITYTYPE>::const_iterator iter = m_typeStr2Int.begin();
		for (; iter != m_typeStr2Int.end(); ++iter)
		{
			ostringstream oss;
			oss << pszDefFilePath << g_cPathSplit << iter->first << ".xml";
			SEntityDef* p = ParseDef(oss.str().c_str());
			m_entityDefs.insert(make_pair(iter->first, p));
			CalcMd5(iter->first, oss.str().c_str());
		}
		m_EntityDefMd5 = m_md5.toString();
		LogDebug("CDefParser::init", "m_EntityDefMd5 = %s", m_EntityDefMd5.c_str());
		ParseParent();*/
	}

	void CDefParser::CalcMd5(const string entityName, const char* pszFile)
	{
		////ֻ��Account �� Avatar��MD5
		//if (m_typeStr2CalcMd5.find(entityName) != m_typeStr2CalcMd5.end())
		//{
		//	ifstream in(pszFile);
		//	m_md5.update(in);
		//	//LogDebug("CDefParser::CalcMd5", entityName.c_str());
		//}
	}

	TENTITYTYPE CDefParser::GetTypeId(const string& tname)
	{
		map<string, TENTITYTYPE>::const_iterator iter = m_typeStr2Int.find(tname);
		if (iter != m_typeStr2Int.end())
		{
			return iter->second;
		}
		else
		{
			return ENTITY_TYPE_NONE;
		}
	}


	const string& CDefParser::GetTypeName(TENTITYTYPE tid)
	{
		if (tid <= m_maxtype)
		{
			return m_typeInt2Str[tid];
		}
		else
		{
			return EMPTY_STRING;
		}
	}


	//����entities.xml
	void CDefParser::ParseEntitiesXml(const char* pszFileName)
	{
		//TiXmlDocument doc;
		//if (!doc.LoadFile(pszFileName))
		//{
		//	ThrowException(-1, "Failed to parse file: %s .", pszFileName);
		//}

		//TiXmlElement* root = doc.RootElement();
		//TiXmlElement* ety = root->FirstChildElement();
		////����entity�������ƺͱ�ŵĶ�Ӧ��ϵ
		//TENTITYTYPE nEntityTypeNo = 0;
		//for (; ety != NULL; ety = ety->NextSiblingElement())
		//{
		//	m_typeStr2Int.insert(make_pair(ety->Value(), ++nEntityTypeNo));

		//	TiXmlNode* p = ety->FirstChild();
		//	if (p)
		//	{

		//		int tmp = atoi(p->Value());
		//		m_typeStr2CalcMd5.insert(make_pair(ety->Value(), tmp));
		//	}
		//}

		//m_maxtype = (uint16_t)(1 + m_typeStr2Int.size());
		//m_typeInt2Str = new string[m_maxtype];
		//m_typeInt2Str[0] = EMPTY_STRING;

		////entity���ͱ�ź����ƵĶ�Ӧ��ϵ
		//map<string, TENTITYTYPE>::const_iterator iter = m_typeStr2Int.begin();
		//for (; iter != m_typeStr2Int.end(); ++iter)
		//{
		//	m_typeInt2Str[iter->second] = iter->first;
		//}
	}

	SEntityDef* CDefParser::ParseDef(const char* pszDefFn)
	{
		//TiXmlDocument doc;
		//if (!doc.LoadFile(pszDefFn))
		//{
		//	ThrowException(-1, "Failed to parse def file: %s.", pszDefFn);
		//}

		//TiXmlElement* root = doc.RootElement();
		//SEntityDef* pDef = new SEntityDef;
		//pDef->m_bHasCellClient = false;

		////parent
		//TiXmlElement* parent = root->FirstChildElement("parent");
		//if (parent)
		//{
		//	pDef->m_parent.assign(parent->GetText());
		//}
		//else
		//{
		//	pDef->m_parent = "";
		//}

		////Properties
		//TiXmlElement* props = root->FirstChildElement("Properties");
		//if (props)
		//{
		//	TiXmlElement* field = props->FirstChildElement();
		//	for (; field != NULL; field = field->NextSiblingElement())
		//	{
		//		_SEntityDefProperties* pProp = ReadProperty(pDef, field);
		//		//todo,��������ظ����ֶ���
		//		pDef->m_properties.insert(make_pair(pProp->m_name, pProp));
		//		pDef->m_propertiesList.push_back(pProp);
		//		pDef->m_propertiesMap.AddNewString(field->Value());
		//	}
		//}

		////ClientMethods
		//TiXmlElement* methods = root->FirstChildElement("ClientMethods");
		//if (methods)
		//{
		//	TiXmlElement* m = methods->FirstChildElement();
		//	for (; m != NULL; m = m->NextSiblingElement())
		//	{
		//		_SEntityDefMethods* p = ReadMethod(m);
		//		p->m_nServerId = SERVER_CLIENT;
		//		pDef->m_clientMethods.insert(make_pair(p->m_funcName, p));
		//		pDef->m_clientMethodsMap.AddNewString(p->m_funcName);

		//		//LogDebug("CDefParser::ParseDef", "pszDefFn=%s;p->m_funcName=%s", pszDefFn, p->m_funcName.c_str());
		//	}
		//}

		////BaseMethods
		//methods = root->FirstChildElement("BaseMethods");
		//if (methods)
		//{
		//	TiXmlElement* m = methods->FirstChildElement();
		//	for (; m != NULL; m = m->NextSiblingElement())
		//	{
		//		_SEntityDefMethods* p = ReadMethod(m);
		//		p->m_nServerId = SERVER_BASEAPP;
		//		pDef->m_baseMethods.insert(make_pair(p->m_funcName, p));
		//		pDef->m_baseMethodsMap.AddNewString(p->m_funcName);
		//	}
		//}

		////CellMethods
		//methods = root->FirstChildElement("CellMethods");
		//if (methods)
		//{
		//	TiXmlElement* m = methods->FirstChildElement();
		//	for (; m != NULL; m = m->NextSiblingElement())
		//	{
		//		_SEntityDefMethods* p = ReadMethod(m);
		//		p->m_nServerId = SERVER_CELLAPP;
		//		pDef->m_cellMethods.insert(make_pair(p->m_funcName, p));
		//		pDef->m_cellMethodsMap.AddNewString(p->m_funcName);
		//	}
		//}

		//return pDef;
		return NULL;
	}


	//����parent��ϵ
	void CDefParser::ParseParent()
	{
		map<string, SEntityDef*>::iterator iter = m_entityDefs.begin();
		for (; iter != m_entityDefs.end(); ++iter)
		{
			const string& strEntity = iter->first;
			SEntityDef* pDef = iter->second;

			//����л���
			if (!pDef->m_parent.empty())
			{
				ParseEntityParent(strEntity, pDef);
			}
		}
	}

	//����һ��entity������parent��ϵ
	void CDefParser::ParseEntityParent(const string& strEntity, SEntityDef* pDef)
	{
		//set<string> parents;

		//string strChild = strEntity;
		//const SEntityDef* pChild = pDef;
		//for (; !pChild->m_parent.empty();)
		//{
		//	//�Ƿ���ѭ���ĸ������
		//	set<string>::const_iterator iter = parents.find(pChild->m_parent);
		//	if (iter != parents.end())
		//	{
		//		ThrowException(-1, "%s has circle parent '%s' ", strChild.c_str(), pChild->m_parent.c_str());
		//	}

		//	//����δ����
		//	const SEntityDef* pParent = GetEntityDefByName(pChild->m_parent);
		//	if (pParent == NULL)
		//	{
		//		ThrowException(-1, "%s' parent '%s' not define!", strChild.c_str(), pChild->m_parent.c_str());
		//	}

		//	//���Ƹ���(����߲�εĸ���)�����Ե�entity
		//	CopyParentToEntity(pDef, pParent);

		//	//����������ĸ���
		//	parents.insert(pChild->m_parent);
		//	strChild = pChild->m_parent;
		//	pChild = pParent;
		//}

	}

	//����һ��������������ݵ�һ��entity
	void CDefParser::CopyParentToEntity(SEntityDef* pChild, const SEntityDef* pParent)
	{
		//properties
		{
			map<string, _SEntityDefProperties*>::const_iterator iter = pParent->m_properties.begin();
			for (; iter != pParent->m_properties.end(); ++iter)
			{
				const string& strPropName = iter->first;
				if (pChild->m_properties.find(strPropName) == pChild->m_properties.end())
				{
					//�����Ѿ������������,�������Ϊ׼;����δ����,ʹ�ø���Ķ���
					_SEntityDefProperties* pCopy = new _SEntityDefProperties(*(iter->second));
					pChild->m_properties.insert(make_pair(strPropName, pCopy));
					pChild->m_propertiesList.push_back(pCopy);
					pChild->m_propertiesMap.AddNewString(strPropName);
				}
			}
		}
		//client methods���ü̳�
		//base methods
		{
			map<string, _SEntityDefMethods*>::const_iterator iter = pParent->m_baseMethods.begin();
			for (; iter != pParent->m_baseMethods.end(); ++iter)
			{
				const string& strMethodName = iter->first;
				if (pChild->m_baseMethods.find(strMethodName) == pChild->m_baseMethods.end())
				{
					_SEntityDefMethods* pCopy = new _SEntityDefMethods(*(iter->second));
					pChild->m_baseMethods.insert(make_pair(strMethodName, pCopy));
					pChild->m_baseMethodsMap.AddNewString(strMethodName);
				}
			}
		}
		//cell methods
		{
			map<string, _SEntityDefMethods*>::const_iterator iter = pParent->m_cellMethods.begin();
			for (; iter != pParent->m_cellMethods.end(); ++iter)
			{
				const string& strMethodName = iter->first;
				if (pChild->m_cellMethods.find(strMethodName) == pChild->m_cellMethods.end())
				{
					_SEntityDefMethods* pCopy = new _SEntityDefMethods(*(iter->second));
					pChild->m_cellMethods.insert(make_pair(strMethodName, pCopy));
					pChild->m_cellMethodsMap.AddNewString(strMethodName);
				}
			}
		}
	}

	//_SEntityDefProperties* CDefParser::ReadProperty(SEntityDef* pDef, TiXmlElement* node)
	//{
	//	_SEntityDefProperties* pProp = new _SEntityDefProperties;
	//	pProp->m_name.assign(node->Value());

	//	TiXmlElement* field_attri = node->FirstChildElement("Type");
	//	if (field_attri)
	//	{
	//		const char* szTypeName = field_attri->GetText();
	//		VTYPE vt = GetVTypeByStr(szTypeName);
	//		if (vt != V_TYPE_ERR)
	//		{
	//			pProp->m_nType = vt;
	//		}
	//		//else if(strcmp(szTypeName, "ARRAY") == 0)
	//		//{
	//		//    //array of ***���⴦��
	//		//}
	//		else
	//		{
	//			//δ���������
	//			ThrowException(-1, "Property '%s' has unsupported type: '%s'.",
	//				pProp->m_name.c_str(), szTypeName);
	//		}
	//	}

	//	//Flags
	//	pProp->m_nFlags = 0;
	//	field_attri = node->FirstChildElement("Flags");
	//	if (field_attri)
	//	{
	//		const char* _szFlag = field_attri->GetText();
	//		if (strcasecmp(_szFlag, "BASE") == 0)
	//		{
	//			SetBaseFlag(pProp->m_nFlags);
	//		}
	//		else if (strcasecmp(_szFlag, "BASE_AND_CLIENT") == 0)
	//		{
	//			SetBaseFlag(pProp->m_nFlags);
	//			SetClientFlag(pProp->m_nFlags);
	//		}
	//		else if (strcasecmp(_szFlag, "CELL") == 0)
	//		{
	//			SetCellFlag(pProp->m_nFlags);
	//		}
	//		else if (strcasecmp(_szFlag, "CELL_AND_CLIENT") == 0)
	//		{
	//			SetCellFlag(pProp->m_nFlags);
	//			SetClientFlag(pProp->m_nFlags);
	//			pDef->m_bHasCellClient = true;
	//		}
	//		else if (strcasecmp(_szFlag, "ALL_CLIENTS") == 0)
	//		{
	//			SetCellFlag(pProp->m_nFlags);
	//			SetClientFlag(pProp->m_nFlags);
	//			SetOtherClientsFlag(pProp->m_nFlags);
	//			pDef->m_bHasCellClient = true;
	//		}
	//		else if (strcasecmp(_szFlag, "OTHER_CLIENTS") == 0)
	//		{
	//			//ע���all_clients������
	//			SetCellFlag(pProp->m_nFlags);
	//			//setClientFlag(pProp->m_nFlags);
	//			SetOtherClientsFlag(pProp->m_nFlags);
	//			//pDef->m_bHasCellClient = false;
	//		}
	//		else
	//		{
	//			ThrowException(-1, "unknown properties flag:%s", _szFlag);
	//		}
	//	}

	//	//���ֶ��Ƿ�base��cell���е��ֶ�
	//	field_attri = node->FirstChildElement("BaseAndCell");
	//	if (field_attri)
	//	{
	//		if (strcasecmp(field_attri->GetText(), "TRUE") == 0)
	//		{
	//			SetCellFlag(pProp->m_nFlags);
	//			SetBaseFlag(pProp->m_nFlags);
	//		}
	//	}

	//	//���ֶ��Ƿ��cellApp
	//	field_attri = node->FirstChildElement("CellShare");
	//	if (field_attri)
	//	{
	//		if (strcasecmp(field_attri->GetText(), "TRUE") == 0)
	//		{
	//			pProp->m_bCellData = true;
	//		}
	//		else
	//		{
	//			pProp->m_bCellData = false;
	//		}
	//	}
	//	else
	//	{
	//		pProp->m_bCellData = false;
	//	}

	//	//���ֶ��Ƿ񱣴浽���ݿ�
	//	field_attri = node->FirstChildElement("Persistent");
	//	if (field_attri)
	//	{
	//		if (strcasecmp(field_attri->GetText(), "TRUE") == 0)
	//		{
	//			pProp->m_bSaveDb = true;

	//			//cell���Բ����д��̱��(������cell��base���е�����)
	//			if (IsCellFlag(pProp->m_nFlags) && !IsBaseFlag(pProp->m_nFlags))
	//			{
	//				ThrowException(-1, "CELL(but Base) properties cant set Persistent to TRUE");
	//			}
	//		}
	//		else
	//		{
	//			pProp->m_bSaveDb = false;
	//		}
	//	}
	//	else
	//	{
	//		pProp->m_bSaveDb = false;
	//	}

	//	//���ֶ��Ƿ���ȱʡֵ
	//	field_attri = node->FirstChildElement("Default");
	//	if (field_attri)
	//	{
	//		pProp->m_defaultValue.assign(field_attri->GetText());
	//	}
	//	else
	//	{
	//		pProp->m_defaultValue.assign("");
	//	}

	//	//���ֶ��Ƿ񴴽�Ψһ����
	//	field_attri = node->FirstChildElement("UniqueIndex");
	//	if (field_attri)
	//	{
	//		if (pDef->m_strUniqueIndex.empty())
	//		{
	//			pDef->m_strUniqueIndex.assign(node->Value());
	//		}
	//		else
	//		{
	//			//����ֶ�������Ψһ����
	//			ThrowException(-1, "At least two properties set unique_index, '%s' vs '%s'", \
	//				pDef->m_strUniqueIndex.c_str(), node->Value());
	//		}
	//	}

	//	return pProp;
	//}

	//_SEntityDefMethods* CDefParser::ReadMethod(TiXmlElement* node)
	//{
	//	_SEntityDefMethods* p = new _SEntityDefMethods;
	//	p->m_funcName.assign(node->Value());

	//	TiXmlElement* e = node->FirstChildElement("Exposed");
	//	if (e)
	//	{
	//		p->m_bExposed = true;
	//	}
	//	else
	//	{
	//		p->m_bExposed = false;
	//	}

	//	e = node->FirstChildElement("Arg");
	//	for (; e != NULL; e = e->NextSiblingElement("Arg"))
	//	{
	//		const char* s = e->GetText();
	//		VTYPE vt = GetVTypeByStr(s);
	//		if (vt != V_TYPE_ERR)
	//		{
	//			p->m_argsType.push_back(vt);
	//		}
	//		else
	//		{
	//			ThrowException(-1, "Method '%s' has a error Arg type '%s'.",
	//				p->m_funcName.c_str(), s);
	//		}
	//	}

	//	return p;
	//}

	//����entity���ͻ�ȡ�䶨��
	const SEntityDef* CDefParser::GetEntityDefByName(const string& strEntityName)
	{
		map<string, SEntityDef*>::const_iterator iter = m_entityDefs.find(strEntityName);
		if (iter == m_entityDefs.end())
		{
			return NULL;
		}
		else
		{
			return iter->second;
		}
	}

	const SEntityDef* CDefParser::GetEntityDefByType(TENTITYTYPE t)
	{
		const string& strEntityName = this->GetTypeName(t);
		const SEntityDef* pDef = this->GetEntityDefByName(strEntityName);
		return pDef;
	}

	//����entity���ͺ���������ȡ���ԵĶ���
	const _SEntityDefProperties* CDefParser::GetEntityPropDef(const SEntityDef* p,
		const string& strPropName)
	{
		map<string, _SEntityDefProperties*>::const_iterator iter = p->m_properties.find(strPropName);
		if (iter == p->m_properties.end())
		{
			return NULL;
		}
		else
		{
			return iter->second;
		}
	}

	const _SEntityDefProperties* CDefParser::GetEntityPropDef(const string& strEntityName,
		const string& strPropName)
	{
		const SEntityDef* p = GetEntityDefByName(strEntityName);
		if (p)
		{
			return GetEntityPropDef(p, strPropName);
		}

		return NULL;
	}

	/*void CDefParser::ReadDbCfg(CCfgReader* r)
	{
		m_dbCfg.m_strHost = r->GetValue("db", "host");
		m_dbCfg.m_strUser = r->GetValue("db", "user");
		m_dbCfg.m_strPasswd = r->GetValue("db", "passwd");
		m_dbCfg.m_unPort = atoi(r->GetValue("db", "port").c_str());
		m_dbCfg.m_strDbName = r->GetValue("db", "db");

		m_logDbCfg.m_strHost = r->GetValue("log_db", "host");
		m_logDbCfg.m_strUser = r->GetValue("log_db", "user");
		m_logDbCfg.m_strPasswd = r->GetValue("log_db", "passwd");
		m_logDbCfg.m_unPort = atoi(r->GetValue("log_db", "port").c_str());
		m_logDbCfg.m_strDbName = r->GetValue("log_db", "db");

		m_redisCfg.m_unPort = atoi(r->GetValue("redis", "port").c_str());
		m_redisCfg.m_unDbId = atoi(r->GetValue("redis", "database_id").c_str());
	}*/

//	//��ѯһ��entity��ĳ������������
//	const string& CDefParser::GetMethodNameById(TENTITYTYPE t, int32_t nFuncId)
//	{
//		const SEntityDef* p = GetEntityDefByType(t);
//		if (p)
//		{
//			return p->m_baseMethodsMap.GetStrByInt(nFuncId);
//		}
//
//		return EMPTY_STRING;
//	}
//
//	//��ѯһ��entity��ĳ��cell����������
//	const string& CDefParser::GetCellMethodNameById(TENTITYTYPE t, int32_t nFuncId)
//	{
//		const SEntityDef* p = GetEntityDefByType(t);
//		if (p)
//		{
//			return p->m_cellMethodsMap.GetStrByInt(nFuncId);
//		}
//
//		return EMPTY_STRING;
//	}
//
//	//��������def�ļ��Ļ��ǩ��
//	string CDefParser::CalcMd5OfAll(const char* pszDefFilePath)
//	{
//		ostringstream oss;
//		for (int i = 1; i < m_maxtype; ++i)
//		{
//			const string& strEntityName = m_typeInt2Str[i];
//			//��ȡÿ��def�����ݵ�һ������ڴ����
//			ostringstream ossDefFileName;
//			ossDefFileName << pszDefFilePath << g_cPathSplit << strEntityName << ".xml";
//			//printf("add %d %s to md5_buff\n", i, ossDefFileName.str().c_str());
//			ifstream deff(ossDefFileName.str(), ios::in);
//			char szLine[256];
//			while (!deff.eof() && deff.getline(szLine, sizeof(szLine), 0x0A))
//			{
//				oss << szLine;
//			}
//		}
//
//#ifdef _WIN32
//		return "";
//#else
//		enum { SIZE16 = 16, };
//		unsigned char szMd5[SIZE16];
//		const string& strOss = oss.str();
//		MD5((unsigned char*)strOss.c_str(), strOss.size(), szMd5);
//
//		char szKey[64];
//		memset(szKey, 0, sizeof(szKey));
//		for (int i = 0; i<SIZE16; ++i)
//		{
//			char_to_sz(szMd5[i], szKey + 2 * i);
//		}
//
//		return string(szKey);
//#endif
//	}

}


