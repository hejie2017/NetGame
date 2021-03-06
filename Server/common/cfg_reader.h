#ifndef __CFG_READER__HEAD__
#define __CFG_READER__HEAD__

#ifdef _WIN32
#pragma warning (disable:4786)
#pragma warning (disable:4503)
#endif

#include "my_stl.h"
#include "util.h"
namespace mogo
{
	//�Զ����������
	class CCfgReader
	{
	public:
		CCfgReader(const string& strFile);
		~CCfgReader();

		bool HasValue(const char* szSection, const char* szName);
		string GetValue(const char* szSection, const char* szName);
		string GetOptValue(const char* szSection, const char* szName, const string& strDefault);

	private:
		int ScanCfgFile();

	private:
		string m_strFile;
		map<string, map<string, string>*> m_CfgDict;
		bool m_bScan;

	};

};

#endif
