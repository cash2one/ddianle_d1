#include "socketmgr.h"
using namespace ZeroSocket;
#include "utility.h"

#define PACKET_TIMES		6

//#define TEST_GETTIMEOFDAY

#define TEST_ADJTIMEX
#include <sys/timex.h>

int main()
{
#ifdef TEST_ADJTIMEX
	struct timex tx;
	for(int i = 0; i < 10; i++)
	{
		tx.modes = 0;
		adjtimex(&tx);
		printf("%d\n", tx.time.tv_sec);
		sleep(1);
	}
	return 0;
#endif

#ifdef TEST_GETTIMEOFDAY
#include <sys/time.h>
	struct timeval tv;
	struct timezone tz;
	for(int i = 0; i < 10; i++)
	{
		gettimeofday(&tv, &tz);
		printf("%d\n", tv.tv_sec);
		sleep(1);
	}
	return 0;
#endif

	unsigned int addr;
	s_Host.GetHostAddr(&addr);
	CSocketMgr mgr;
	mgr.Init(8043, 1024, 16, 4);
	GAMEMSG_BASE*  pMsg;
	unsigned short nSlot;
	unsigned int nPacket, nTotalSize;
	nPacket = nTotalSize = 0;
	int nFlag = 0;
	for(;;)
	{
		while(mgr.GetGameMsg(pMsg, nSlot))
		{
			//fprintf(stdout, "RecvMsg on slot %d, 0x%04X, %d\n", nSlot, pMsg->nMsg, pMsg->nSize);	
			nPacket++;
			nTotalSize += pMsg->nSize;
			switch(pMsg->nMsg) {
			case MSG_SYSTEM_Ping:
				mgr.SendGameMsg(pMsg, nSlot);
				break;
			case MSG_SYSTEM_Test:
				for(int i = 0; i < PACKET_TIMES; i++)//回复相当倍数的封包
				{
					bool bRes = mgr.SendGameMsg(pMsg, nSlot);
					ASSERT(bRes);
				}
				break;
			case MSG_SYSTEM_Disconnect:
				fprintf(stdout, "Client %d disconnect.\n", nSlot);
				break;
			default:
				break;
			}
			delete pMsg;
		}
		if(nFlag++%10 == 0)
		{
			fprintf(stdout, "%08d client Count %03d, RecvMsg %08d, total size %09d\r", nFlag/10, mgr.GetCurClientCount(), nPacket, nTotalSize);
		}
		//millisecondSleep(5);
	}
	return 0;
}


