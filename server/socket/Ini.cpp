#include <stdio.h>
#include <stdlib.h>
#include "Ini.h"
#include "Crypt.h"
#include "Md5.h"
#include "Formatter.h"

//#define _TEST_INI
#ifdef _TEST_INI
#define _DISPLAY_LIST
#endif
#ifdef WIN32
#pragma warning(disable:4996)
#endif
bool CIni::Load(const char* szFileName)
{
	return LoadIni(m_mapOption, szFileName);
}
bool CIni::Save(const char* szFileName)
{
	return SaveIni(m_mapOption, szFileName);
}

void CIni::Clear()
{
	m_mapOption.clear();
}

int CIni::GetInt(const char* szSection, const char* szKey, int nDefault)
{
	_tMapIni::iterator it = m_mapOption.find(szSection);
	if(it != m_mapOption.end())
	{
		_tMapIniSection::iterator itValue = it->second.find(szKey);
		if(itValue != it->second.end())
		{
			return atoi(itValue->second.c_str());
		}
	}
	return nDefault;
}


#ifndef MAX_STRING
#define MAX_STRING	256
#endif
int CIni::GetString(const char* szSection, const char* szKey, string& strBuff, const char* szDefault)
{
	char szBuff[MAX_STRING] = "";
	int nRes = GetString(szSection, szKey, szBuff, MAX_STRING, szDefault);
	strBuff = szBuff;
	return nRes;
}

int CIni::GetString(const char* szSection, const char* szKey, char* szBuff, const unsigned int cbLen, const char* szDefault)
{
	_tMapIni::iterator it = m_mapOption.find(szSection);
	if(it != m_mapOption.end())
	{
		_tMapIniSection::iterator itValue = it->second.find(szKey);

		if( itValue != it->second.end() )
		{
			if(cbLen <= itValue->second.size())
				return 0;
			else
			{
				strcpy(szBuff, itValue->second.c_str());
				return itValue->second.size();
			}
		}
	}
	if(szDefault != NULL && cbLen > strlen(szDefault))
	{
		strcpy(szBuff, szDefault);
		return strlen(szDefault);
	}
	else
	{
		return 0;
	}
}

void CIni::SetInt(const char* szSection, const char* szKey, int nNewValue)
{
	std::string strBuff;
	Formatter(strBuff) << nNewValue;
	SetString(szSection, szKey, strBuff.c_str());
}

void CIni::SetString(const char* szSection, const char* szKey, const char* szNewValue)
{
	string str = szNewValue;
	string strKey = szKey;
	string strSec = szSection;

	_tMapIni::iterator it = m_mapOption.find(szSection);
	if(it != m_mapOption.end())
	{
		it->second[strKey] = str;
	}
	else
	{
		_tMapIniSection sec;
		sec[strKey] = str;
		m_mapOption[strSec] = sec;
	}
}

void TrimLeft( string &str, char c)
{
    if ( str.empty() )
        return;

    std::string::size_type nFirst = str.find_first_not_of( c );

    if ( nFirst == std::string::npos )
        str.clear();
    else if ( 0 < nFirst && nFirst < str.size() )
        str.erase( 0, nFirst );
}

void TrimRight( string &str, char c )
{
    if ( str.empty() )
        return;

    std::string::size_type nLast = str.find_last_not_of( c );

    if ( nLast == std::string::npos )
        str.clear();
    else
        str.erase( nLast + 1 );
}

void TrimCR( string &str)
{
	while(str.size() > 0 && 
		(str[str.size()-1] == '\r' || str[str.size()-1] == '\n')  )
	{
		str = str.substr(0, str.size() - 1);
	}
}

bool DepartString( string &str, string &str1, string &str2)
{
	TrimLeft(str);TrimLeft(str, '\t');TrimLeft(str);
	TrimRight(str);TrimRight(str, '\t');TrimRight(str);
	if(str.size() > 0)
	{
		int nEqu = str.find_first_of('=');
		if(nEqu > 0 && (unsigned)nEqu < str.size() - 1)
		{
			str1 = str.substr(0, nEqu);
			TrimRight(str1);TrimRight(str1, '\t');
			str2 = str.substr(nEqu + 1, str.size() - nEqu -1);
			TrimLeft(str2);TrimLeft(str2, '\t');
			return true;
		}
	}
	return false;
}

bool IsSection( string &str )
{
	TrimCR(str);
	TrimLeft(str);TrimRight(str);
	TrimLeft(str, '\t');TrimRight(str, '\t');

	//空字符串，下面的str[0]会触发string内部的Error，VS2010环境下
	if( !str.empty() && str[0] == '[' && str[str.size()-1] == ']' )
	{
		str = str.substr(1, str.size()-2);
		return true;
	}
	else
		return false;
}

#define BUFF_LEN  200

bool LoadIni(_tMapIni &mapOption, const char* szFileName)
{
	char szBuff[BUFF_LEN];
	string str, str1, str2;
	pair< string, _tMapIniSection >section;
	FILE* fp;
	if( (fp = fopen(szFileName, "r") ) != NULL )
	{
		if(fgetc(fp) != 0)
		{
			fseek(fp, SEEK_SET, 0);
			while ( fgets(szBuff, BUFF_LEN, fp) != NULL)
			{
				str = szBuff;
				if (IsSection(str)) 
				{
					section.first = str;
					mapOption.insert( section );
				}
				else if( DepartString(str, str1, str2))
				{
					mapOption[section.first][str1] = str2;
				}
			}
		}
		else
		{
			char szKey[16];
			char szKeyKey[16];
			fread(szKey, 16, 1, fp);
			fread(szKeyKey, 16, 1, fp);
			int nLen;
			CCryptDes des;
			while( !feof(fp) && 0 != (nLen = fgetc(fp)))
			{
				fread(szBuff, nLen, 1, fp);
				des.Des_Go(szKey, szKey, 16, szKeyKey, 16, _OP_ENCRYPT);
				des.Des_Go(szBuff, szBuff, nLen, szKey, 16, _OP_DECRYPT);

				str = szBuff;
				if (IsSection(str)) 
				{
					section.first = str;
					mapOption.insert( section );
				}
				else if( DepartString(str, str1, str2))
				{
					mapOption[section.first][str1] = str2;
				}
			}
			
		}
		fclose(fp);

#ifdef _DISPLAY_LIST
		//display
		_tMapIni::iterator it;
		for(it = mapOption.begin(); it != mapOption.end(); it++)
		{
			printf("[%s]\n",(*it).first.c_str());
			_tMapIniSection::iterator itValue;
			for( itValue = it->second.begin(); itValue != it->second.end(); itValue++ )
			{
				printf("%s=%s\n", itValue->first.c_str(), itValue->second.c_str() );
			}
		}
#endif //_DISPLAY_LIST
		return true;
	}

	return false;
}

bool SaveIni(_tMapIni &mapOption, const char* szFileName)
{
	FILE* fp = fopen(szFileName, "w");
	if(fp == NULL)
		return false;
	
	for(_tMapIni::iterator it = mapOption.begin(); it != mapOption.end(); it++)
	{
		
		fprintf(fp, "[%s]\r\n", it->first.c_str());
		for( _tMapIniSection::iterator itSec = it->second.begin(); itSec != it->second.end(); itSec++)
		{
			fprintf(fp, "%s=%s\r\n", itSec->first.c_str(), itSec->second.c_str());
		}
		fprintf(fp, "\r\n");
	}
	fclose(fp);
	return true;
}

bool CryptIni(const char* szSource, const char* szDest, const char* szKeyString)
{
	if(szKeyString == NULL || strlen(szKeyString) < 2)
		return false;

	char szBuff[BUFF_LEN];
	char szKey[16];
	char szKeyKey[16];
	MDString((char*)szKeyString, (unsigned char*)szKey);
	MDString((char*)szKeyString + 1, (unsigned char*)szKeyKey);
	
	FILE* fpSrc = fopen(szSource, "r");

	bool bRes = false;
	if(fpSrc != NULL)
	{
		FILE* fpDst = fopen(szDest, "w");
		if(fpDst != NULL)
		{
			fputc(0, fpDst);
			fwrite(szKey, 16, 1, fpDst);
			fwrite(szKeyKey, 16, 1, fpDst);
			CCryptDes des;
			while ( fgets(szBuff, BUFF_LEN, fpSrc) != NULL)
			{
				int nLen = strlen(szBuff);
				nLen = (nLen+8) & 0x000000F8;
				fputc( nLen, fpDst);
				des.Des_Go(szKey, szKey, 16, szKeyKey, 16, _OP_ENCRYPT);
				des.Des_Go(szBuff, szBuff, nLen, szKey, 16, _OP_ENCRYPT);
				fwrite(szBuff, nLen, 1, fpDst);
			}
			fputc(0, fpDst);
			fclose(fpDst);
			bRes = true;
		}

		fclose(fpSrc);
	}

	return bRes;
}

#undef BUFF_LEN


#ifdef _TEST_INI

int main(int argc, char* argv[])
{
  _tMapIni lstOption;
  LoadIni(lstOption, "config.ini");

  SaveIni(lstOption, "cc.ini");

  lstOption.clear();

  CryptIni("config.ini", "config.ini.1", "test");

  LoadIni(lstOption, "config.ini.1");

  return 0;
}

#endif



//EOF

