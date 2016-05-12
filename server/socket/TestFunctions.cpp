#include "stdio.h"
#include "conio.h"
#include <iostream>
using namespace std;
#include <windows.h>

struct bit
{
	public:
		int a:3;
		int b:2;
		int c:3;
};

//env	CPU		P4 2.4G
//		MEM		1G
//		OS		WinXP

int main(void)
{
	char szBuff[100];
	char szBuff1[100];

	int arr[100];
	int* ptr = arr +10;
	int count = arr + 100 -ptr;

	sprintf(szBuff, "count = %d\n", count);
	OutputDebugString(szBuff);

	CRITICAL_SECTION cs;
	InitializeCriticalSection(&cs);
	DWORD dwTick = GetTickCount();
	for(int i = 0; i < 1000000; i++)
	{
		EnterCriticalSection(&cs);
		LeaveCriticalSection(&cs);
	}
	//EnterCriticalSection and LeaveCriticalSection 1,000,000 times need 156ms
	//So EnterCriticalSection and LeaveCriticalSection need 156ns
	sprintf(szBuff, "Ticks = %d\n", GetTickCount() - dwTick);
	OutputDebugString(szBuff);
	DeleteCriticalSection(&cs);

	dwTick = GetTickCount();
	for(i = 0; i < 1000000; i++)
	{
		memcpy(szBuff, szBuff1, 100);
		memcpy(szBuff1, szBuff, 100);
	}
	//memcpy 100 bytes 1,000,000 * 2 times need 94ms
	//So memcpy 100 bytes need 47ns
	sprintf(szBuff, "Ticks = %d\n", GetTickCount() - dwTick);
	OutputDebugString(szBuff);

	dwTick = GetTickCount();
	for(i = 0; i < 1000000; i++)
	{
		char* ptr = new char[100];
		delete ptr;
	}
	//new char[100] and delete char[100] 1,000,000 need 2031ms
	//So new and delete need 2us
	sprintf(szBuff, "Ticks = %d\n", GetTickCount() - dwTick);
	OutputDebugString(szBuff);

	bool bRes = true;
	dwTick = GetTickCount();
	for(i = 0; i < 10000000; i++)
	{
		if(bRes)bRes = false;
		else bRes = true;
	}
	//10,000,000 times need 32 ms
	//So 1 time need 3ns
	sprintf(szBuff, "Ticks = %d\n", GetTickCount() - dwTick);
	OutputDebugString(szBuff);

	bit s;
	char* c = (char*)&s;
	*c = 0x99;
	cout << s.a <<endl<<s.b<<endl<<s.c<<endl;
	getch();
	return 0;
}


