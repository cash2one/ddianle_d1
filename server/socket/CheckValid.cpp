#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <string>
using namespace std;

#include "Windefine.h"
#include "Md5.h"
#include "Crypt.h"
#include "Log.h"

#ifdef WIN32
#pragma warning(disable :4996)
#endif // WIN32

extern string g_sPath;
bool IsValid()
{
	char szDataFile[7][30]=
	{
		"data/bytes/quest.bytes",
		"data/bytes/iteminfo.bytes",
		"data/bytes/musicinfo.bytes",
		"data/logicparam.ini",
		"data/bytes/achievement.bytes",
		"data/bytes/modelinfo.bytes",
		"etc/servermap.conf"
	};
	unsigned char szMD5[7][16];
	char szOriKey[] = "ixuanqu.hotdance.com";

	std::string strFilename;
	strFilename.assign(g_sPath), strFilename.append("etc/servermap.conf");
	unsigned char szMap[16];
	MDFile(strFilename.c_str(), szMap);

	strFilename.assign(g_sPath), strFilename.append("etc/version.key");
	FILE* fp = fopen(strFilename.c_str(), "r");
	char szMapBak[16] = {0};
	char szDate[16];
	char szKey[16];
	char szMDDataFileBak[16];
	if(fp != NULL)
	{
		fread(szMapBak, 16, 1, fp);
		fread(szDate, 16, 1, fp);
		fread(szKey, 16, 1, fp);
		fread(szMDDataFileBak, 16, 1, fp);
		fclose(fp);
	}
	else
	{
		WriteLog(LOGLEVEL_ERROR, "Open file failed!\n");
		return false;
	}

	CCryptDes des;
	des.Des_Go((char*)szMDDataFileBak, (char*)szMDDataFileBak, 16, (char*)szKey, 16, _OP_DECRYPT);
	des.Des_Go(szDate, szDate, 16, szMapBak, 16, _OP_DECRYPT);
	des.Des_Go(szKey, szKey, 16, szOriKey, 16, _OP_DECRYPT);
	des.Des_Go(szMapBak, szMapBak, 16, (char*)szKey, 16, _OP_DECRYPT );
	if( 0 != memcmp(szMap, szMapBak, 16))
	{
		WriteLog(LOGLEVEL_ERROR, "servermap.conf modified!\n");
		return false;
	}
	time_t now = time(NULL);
	time_t date;
	memcpy(&date, szDate, sizeof(int));

	//如果时间超过两个月，则失效，服务不能启动
	if(now > date + 60*24*60*60)
	{
		WriteLog(LOGLEVEL_ERROR, "timeout!\n");
		return false;
	}

	memset(szMD5, 1, 7 * 16);
	for(int i = 0; i < 7; i++)
	{
		strFilename.assign(g_sPath), strFilename.append(szDataFile[i]);
		MDFile(strFilename.c_str(), szMD5[i]);
	}
	unsigned char szMDDataFile[16];
	MDData((char*)szMD5, 7 * 16, szMDDataFile);
	unsigned char szDataKey[16];
	MDData(szOriKey+1, 16, szDataKey);
	des.Des_Go((char*)szMDDataFile, (char*)szMDDataFile, 16, (char*)szDataKey, 16, _OP_ENCRYPT);
	if(0 != memcmp(szMDDataFile, szMDDataFileBak, 16))
	{
		WriteLog(LOGLEVEL_ERROR, "Data file modified!\n");
		return false;
	}
	return true;
}

//file end

