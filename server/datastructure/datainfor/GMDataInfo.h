#ifndef __GMDATAINFO_H__
#define __GMDATAINFO_H__

#include <string>
#include "../../socket/ParamPool.h"

class GMRoomPlayerInfo
{
public:
	unsigned int m_playerID;
	std::string m_playerName;
};

//////////////////////////////////////////////////////////////////////////
// 和GM客户端适配的数据结构
//////////////////////////////////////////////////////////////////////////
class GMRoomInfo
{
public:
	GMRoomInfo();
	~GMRoomInfo();

public:
	unsigned short m_RoomID;
	std::string m_RoomName;
	std::string m_RoomPassword;

	unsigned int m_PlayerMainID;
	std::string m_PlayerMainName;

	std::vector<GMRoomPlayerInfo> m_PlayerList;

	unsigned short m_MusicID;
	std::string m_MusicName;
	unsigned short m_MusicMode;
	unsigned short m_MusicDiff;

public:
	void doEncode(CParamPool & IOBuff);
	void doDecode(CParamPool & IOBuff);

	void AddPlayerInfo(GMRoomPlayerInfo player);
};

class PlayerMoneyInfo
{
public:
	void doEncode(CParamPool & IOBuff);
	void doDecode(CParamPool & IOBuff);

public:
	std::string m_PlayerPUID;
	int m_nMoney;
	int m_nBindMoney;
};

class GMRechargeResult
{
public:
	void doEncode(CParamPool & IOBuff);
	void doDecode(CParamPool & IOBuff);
public:
	std::string m_PlayerPUID;
	bool m_Result;
};

class CServerPlayerCountInfo
{
public:
	int m_nLine;
	std::string m_strName;
	int m_nCurrentCount;
	int m_nLimitCount;

public:
	void doEncode(CParamPool & IOBuff);
	void doDecode(CParamPool & IOBuff);
};

#endif

