#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Md5.h"

#ifdef WIN32
#pragma warning(disable:4996)
#endif // WIN32


int main(int argc, char* argv[])
{
	FILE* fp = fopen("../etc/validclient.md5", "w");
	if(fp != NULL)
	{
		unsigned char szVer[16];
		unsigned char md5value[4][16];
		memset(md5value, 0, 4*16);
		char szFileName[260] = {0};

		strcpy(szFileName, "../data/bytes/quest.bytes");
		MDFile(szFileName, md5value[1]);
		strcpy(szFileName, "../data/bytes/iteminfo.bytes");
		MDFile(szFileName, md5value[2]);
		strcpy(szFileName, "../data/bytes/musicinfo.bytes");
		MDFile(szFileName, md5value[3]);
		strcpy(szFileName, "../data/bytes/modelinfo.bytes");
		MDFile(szFileName, md5value[0]);

		MDData((char*)md5value, 4 * 16, szVer);

		fwrite(szVer, 16, 1, fp);

		fclose(fp);

		printf("Done!\n");
	}
	else
	{
		printf("Failed!\n");
		return -1;
	}

	return 0;
}

//


