//AntiHack.cpp

#include "antiHack.h"
#include "md5.h"
#include "windefine.h"
#include <stdio.h>
#pragma warning(disable:4313)

bool GetModuleMD5(HMODULE hMod, unsigned char digest[16])
{
	//HMODULE hMod = GetModuleHandle(NULL);
	char szFileName[MAX_PATH];
	if(0 < GetModuleFileName(hMod, szFileName, MAX_PATH))
	{
		HANDLE hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile != INVALID_HANDLE_VALUE)
		{
			HANDLE hMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, GetFileSize(hFile, NULL), NULL);
			LPBYTE pData = (LPBYTE)MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, GetFileSize(hFile, NULL));
			MD5_CTX md5ctx;
			MD5Init(&md5ctx);
			MD5Update(&md5ctx, pData, GetFileSize(hFile, NULL));
			MD5Final(digest, &md5ctx);
			UnmapViewOfFile(pData);
			CloseHandle(hMap);
			CloseHandle(hFile);
			return true;
		}
		return false;
	}

	return false;
}

//#include "psapi.h"
//#pragma comment(lib, "Psapi.lib")

void PrintModules( DWORD processID )
{
	HMODULE hMods[1024];
	HANDLE hProcess;
	DWORD cbNeeded;
	unsigned int i;

	// Print the process identifier.

	printf( "\nProcess ID: %u\n", processID );

	// Get a list of all the modules in this process.

	hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID );
	if (NULL == hProcess)
		return;

	if( EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		for ( i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
		{
			char szModName[MAX_PATH];

			// Get the full path to the module's file.

			if ( GetModuleFileNameEx( hProcess, hMods[i], szModName,
				sizeof(szModName)))
			{
				// Print the module name and handle value.

				printf("\t%s (0x%08X)\n", szModName, hMods[i] );
			}
		}
	}

	CloseHandle( hProcess );
}

