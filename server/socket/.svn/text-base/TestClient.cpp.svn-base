#include "clientsocketmgr_w.h"
using namespace ZeroSocket;

#include "antiHack.h"
#define TEST_NETWORK_LOAD	//测试网络负载能力
//#define TEST_CHECK_INTEGRALITY	//测试完整性


#define CLIENT_COUNT		200

int main(int argc, char* argv[])
{
#ifdef TEST_CHECK_INTEGRALITY
	PrintModules(GetCurrentProcessId());
	unsigned char szMD5[16];
	if( GetModuleMD5(GetModuleHandle(NULL), szMD5) )
	{
		FILE* fp = fopen("./md5.dig", "w+");
		if(fp != NULL)
		{
			fwrite(szMD5, 16, 1, fp);
			fclose(fp);
		}
	}
	
	getchar();
#endif//TEST_CHECK_INTEGRALITY

#ifdef TEST_NETWORK_LOAD
	if(argc < 2)
	{
		printf("Please input ip address in command line\n");
		return 0;
	}
	CClientSocketMgr *mgr = new CClientSocketMgr[CLIENT_COUNT];
	unsigned int *nSizeTotal = new unsigned int[CLIENT_COUNT];
	int *nState = new int[CLIENT_COUNT];
	int i;
	GAMEMSG_BASE* pMsg;
	GAMEMSG_SYSTEM_Ping msgPing;
	msgPing.nDirection = 1;
	GAMEMSG_SYSTEM_Test msgTest;
	char szContent[40]={"1234567890abcdefghij1234567890123456789"};
	memcpy(msgTest.szBuff,szContent, 40);

	for(i = 0; i < CLIENT_COUNT; i++)
	{
		nSizeTotal[i] = 0;
		nState[i] = 0;

		mgr[i].Init();
		if( mgr[i].Connect(NULL, argv[1], 8043) )
		{
			nState[i] = 1;
		}
		millisecondSleep(10);
	}
	for(int __nLoop=0;;__nLoop++)
	{
		millisecondSleep(200);

		int nCount = 0;

		for(i = 0; i < CLIENT_COUNT; i++)
		{
			if(nState[i] > 0)
			{
				nCount++;
				while ((pMsg = mgr[i].GetGameMsg()) != NULL )
				{
					switch(pMsg->nMsg) 
					{
					case MSG_SYSTEM_Disconnect:
						nState[i] = 0;//连接断开
						break;
					case MSG_SYSTEM_Slot:
					case MSG_SYSTEM_Connect:
						nState[i] = 2;//连接完成，可以开始发消息
						break;
					case MSG_SYSTEM_Ping:
						fprintf(stdout, "client %03d, ping : %04d\r", i, ((LPGAMEMSG_SYSTEM_Ping)pMsg)->nTickCount);
						break;
					case MSG_SYSTEM_Test:
						ASSERT( 0 == memcmp(((LPGAMEMSG_SYSTEM_Test)pMsg)->szBuff, szContent, 40));
						//nSizeTotal[i] += pMsg->nSize;
						//break;
					default:
						nSizeTotal[i] += pMsg->nSize;
						break;
					}
					delete pMsg;
				}
				if(nState[i] > 1)mgr[i].SendGameMsg(&msgTest);
			}
		}
		int nSlot = rand()%CLIENT_COUNT;
		if(nState[nSlot] > 1)
		{
			msgPing.nTickCount = GetTickCount();
			mgr[nSlot].SendGameMsg(&msgPing);
		}
		//fprintf(stdout, "%08d Current Client Count = %d\n", __nLoop, nCount);

		//for(i = 0; i < CLIENT_COUNT; i++)
		//	fprintf(stdout, "%02d RecvMsg Total size %d\n", i, nSizeTotal[i]);


	}
#endif //TEST_NETWORK_LOAD
}


