#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Md5.h"
#include "Crypt.h"

#ifdef WIN32
#pragma warning(disable:4996)
#endif

#define SERVERMAP_ORI_FILE	"../etc/servermap.conf"
#define SERVERMAP_BAK_FILE	"../etc/version.key"

int main(int argc, char* argv[])
{
	if(argc == 3 && strcmp(argv[2], "hotdance") == 0)
	{

		char szDataFile[7][50]=
		{
			"../data/bytes/quest.bytes",
			"../data/bytes/iteminfo.bytes",
			"../data/bytes/musicinfo.bytes",
			"../data/logicparam.ini",
			"../data/bytes/achievement.bytes",
			"../data/bytes/modelinfo.bytes",
			"../etc/servermap.conf"
		};
		unsigned char szMD5[7][16];
		char szOriKey[] = "ixuanqu.hotdance.com";

		char szKey[16];
		MDString(argv[1], (unsigned char*)szKey);
		char szMap[16];
		MDFile(SERVERMAP_ORI_FILE, (unsigned char*)szMap);

		CCryptDes des;
		des.Des_Go(szMap, szMap, 16, szKey, 16, _OP_ENCRYPT);

		time_t date = time(NULL);
		char szDate[16];
		memcpy(szDate, &date, sizeof(int));
		srand((unsigned int)date);
		date = rand();
		memcpy(szDate+sizeof(int), &date, sizeof(int));
		date = rand();
		memcpy(szDate+sizeof(int), &date, sizeof(int));
		date = rand();
		memcpy(szDate+sizeof(int), &date, sizeof(int));

		des.Des_Go(szDate, szDate, 16, szMap, 16,_OP_ENCRYPT);

		des.Des_Go(szKey, szKey, 16, szOriKey, 16, _OP_ENCRYPT);

		memset(szMD5, 0, 7 * 16);
		for(int i = 0; i < 7; i++)
		{
			MDFile(szDataFile[i], szMD5[i]);
		}

		unsigned char szMDDataFile[16];
		MDData((char*)szMD5, 7 * 16, szMDDataFile);

		unsigned char szDataKey[16];
		MDData(szOriKey+1, 16, szDataKey);
		des.Des_Go((char*)szMDDataFile, (char*)szMDDataFile, 16, (char*)szDataKey, 16, _OP_ENCRYPT);
		des.Des_Go((char*)szMDDataFile, (char*)szMDDataFile, 16, (char*)szKey, 16, _OP_ENCRYPT);

		FILE *fp = fopen(SERVERMAP_BAK_FILE, "w");
		if( fp != NULL)
		{
			fwrite(szMap, 16, 1, fp);
			fwrite(szDate, 16, 1, fp);
			fwrite(szKey, 16, 1, fp);
			fwrite(szMDDataFile, 16, 1, fp);
			fclose(fp);
			printf("Key file created!\n");
		}
	}
	else
	{
	}

	return 0;
}


//file end

