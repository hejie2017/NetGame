#include "cfg_reader.h"
namespace mogo
{

	CCfgReader::CCfgReader(const string& strFile)
		:m_strFile(strFile), m_bScan(false)
	{
	}

	CCfgReader::~CCfgReader()
	{
		map<string, map<string, string>*>::iterator iter = m_CfgDict.begin();
		for (; iter != m_CfgDict.end(); ++iter)
		{
			delete iter->second;
			iter->second = NULL;
		}
	}

	bool CCfgReader::HasValue(const char* szSection, const char* szName)
	{
		if (!m_bScan)
		{
			ScanCfgFile();
		}

		if (!szSection || !szName)
		{
			return false;
		}

		map<string, map<string, string>*>::const_iterator iter = m_CfgDict.find(szSection);
		if (iter == m_CfgDict.end())
		{
			return false;
		}

		return iter->second->find(szName) != iter->second->end();
	}

	string CCfgReader::GetValue(const char* szSection, const char* szName)
	{
		if (!m_bScan)
		{
			ScanCfgFile();
		}

		map<string, map<string, string>*>::const_iterator iter = m_CfgDict.find(szSection);
		if (iter != m_CfgDict.end())
		{
			map<string, string>& tmp = *(iter->second);
			map<string, string>::const_iterator iter2 = tmp.find(szName);
			if (iter2 != tmp.end())
			{
				//printf("get_value, section=[%s], %s=%s\n", szSection, szName, iter2->second.c_str());
				return iter2->second;
			}
		}

		static string strEmpty("");
		return strEmpty;
	}

	string CCfgReader::GetOptValue(const char* szSection, const char* szName,
		const string& strDefault)
	{
		//try
		//{
			if (HasValue(szSection, szName))
			{
				return GetValue(szSection, szName);
			}
			else
			{
				return strDefault;
			}
		//}
		//catch (const CException&)
		//{
		//	return strDefault;
		//}
	}


	int CCfgReader::ScanCfgFile()
	{
		ifstream file(m_strFile.c_str(), ios::in);
		if (!file.is_open())
		{
			return 1;
		}

		string strSection;
		char szLine[128];
		while (!file.eof() && file.getline(szLine, sizeof(szLine), 0x0A))
		{
			//window文件格式换行是0x0D0A,UNIX/LINUX是0x0A
			string sLine = Trim(szLine);
			if (sLine.empty() || sLine[0] == '#')
			{
				continue;
			}

			if (sLine[0] == '[' && sLine[sLine.size() - 1] == ']')
			{
				strSection = sLine.substr(1, sLine.size() - 2);
				continue;
			}

			string::size_type pos = sLine.find("=");
			if (pos == string::npos)
			{
				continue;
			}

			if (strSection.empty())
			{
				//ThrowException(-1, "item '%s' has not section", sLine.c_str());
			}

			string strTemp = sLine.substr(0, pos);
			string s1 = Trim(strTemp);
			strTemp = sLine.substr(pos + 1, sLine.size() - pos - 1);
			string s2 = Trim(strTemp);

			map<string, map<string, string>*>::iterator iter = m_CfgDict.lower_bound(strSection);
			if (iter != m_CfgDict.end() && strSection == iter->first)
			{
				iter->second->insert(make_pair(s1, s2));
			}
			else
			{
				map<string, string>* tmp = new map<string, string>;
				tmp->insert(make_pair(s1, s2));
				m_CfgDict.insert(iter, make_pair(strSection, tmp));
			}
		}
		m_bScan = true;
	}

};
