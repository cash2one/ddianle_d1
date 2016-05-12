//  [2003-4-10]
// Ini.h
// class for read ini file

#ifndef __INI_H__
#define __INI_H__

#ifdef WIN32
#pragma warning(disable:4267 4786)
#endif

#include <string>
#include <list>
#include <map>
using namespace std;

typedef map< string, map< string, string > >	_tMapIni;
typedef map< string, string > _tMapIniSection;

class CIni
{
public:
	bool Load(const char* szFileName);
	int GetInt(const char* szSection, const char* szKey, int nDefault);
	int GetString(const char* szSection, const char* szKey, char* szBuff, const unsigned int cbLen, const char* szDefault);
	int GetString(const char* szSection, const char* szKey, string& strBuff, const char* szDefault);
	void SetString(const char* szSection, const char* szKey, const char* szNewValue);
	void SetInt(const char* szSection, const char* szKey, int nNewValue);
	void Clear();
	bool Save(const char* szFileName);
private:
	 _tMapIni m_mapOption;
};

void TrimLeft(string &str, char c = ' ');
void TrimRight(string &str, char c = ' ');
void TrimCR(string &str);
bool DepartString(string &str, string &str1, string &str2);
bool IsSection( string &str );
bool LoadIni( _tMapIni &mapOption, const char* szFileName);
bool CryptIni( const char* szSource, const char* szDest, const char* szKeyString);
bool SaveIni( _tMapIni &mapOption, const char* szFileName);

#endif//__INI_H__


//file end

