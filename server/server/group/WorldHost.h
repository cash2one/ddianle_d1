#ifndef __WORLD_HOST_H__
#define __WORLD_HOST_H__

#include "../share/ServerDataStruct.h"
#include "../../socket/inc/SocketLib.h"
#include <map>
#include <string>
using namespace std;

class CWorldHost
{
public:
	CWorldHost(){};
	~CWorldHost();
	//初始化
	bool Initialize();
	//处理来自各游戏服务器的消息
	void OnServerMsg(LPGameMsg_Base pMsg, char nSvr);
	//响应时间流逝
	void OnUpdate(int& timeElapse);

    void SetClosed();

    void OnGameServerConnected(unsigned short nSlot);
protected:
	std::map< string, int> m_mapSilence;
	int m_nSilenceDelay;
private:
};


#endif//__WORLD_HOST_H__


//file end

