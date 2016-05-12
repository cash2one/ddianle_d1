#ifndef __GAMEMSG_LBS_H__
#define __GAMEMSG_LBS_H__

#include "../../socket/GameMsg_Base.h"

//更新玩家经纬度
class GameMsg_C2S_UpdateLBSPositon:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_UpdateLBSPositon);
	GameMsg_C2S_UpdateLBSPositon();
	~GameMsg_C2S_UpdateLBSPositon();
public:
	virtual bool doEncode(CParamPool &IOBuff);
	virtual bool doDecode(CParamPool &IOBuff);
public:
	float m_fX;//经度
	float m_fY;//纬度
};

class GameMsg_S2G_UpdateLBSPositon:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_UpdateLBSPositon);
	GameMsg_S2G_UpdateLBSPositon();
	~GameMsg_S2G_UpdateLBSPositon();
public:
	virtual bool doEncode(CParamPool &IOBuff);
	virtual bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID;
	float m_fX;
	float m_fY;
};

class GameMsg_C2S_GetNearbyPlayers:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetNearbyPlayers);
	GameMsg_C2S_GetNearbyPlayers();
	~GameMsg_C2S_GetNearbyPlayers();
public:
	virtual bool doEncode(CParamPool &IOBuff);
	virtual bool doDecode(CParamPool &IOBuff);
public:
	unsigned char m_cFlag;//原样返回客户端
};

class GameMsg_S2G_GetNearbyPlayers:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_GetNearbyPlayers);
	GameMsg_S2G_GetNearbyPlayers();
	~GameMsg_S2G_GetNearbyPlayers();
public:
	virtual bool doEncode(CParamPool &IOBuff);
	virtual bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID;
	unsigned char m_cFlag;
};

class NearbyPlayer
{
public:
	NearbyPlayer();
	~NearbyPlayer();
public:
	bool doEncode(CParamPool &IOBuff) const;
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID;
	stringw m_strRoleName;
	unsigned short m_nLevel;
	unsigned short m_nVIPLevel;
	unsigned char m_nSex;
	int m_nLine;
	float m_fDist;
	float m_fX;
	float m_fY;
	unsigned char m_nCallTo;
};

class GameMsg_G2S_NearbyPlayers:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_NearbyPlayers);
	GameMsg_G2S_NearbyPlayers();
	~GameMsg_G2S_NearbyPlayers();
public:
	virtual bool doEncode(CParamPool &IOBuff);
	virtual bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID;
	unsigned char m_cFlag;
	vector<NearbyPlayer> m_listNearbyPlayer;
};

class GameMsg_S2C_NearbyPlayers:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_NearbyPlayers);
	GameMsg_S2C_NearbyPlayers();
	~GameMsg_S2C_NearbyPlayers();
public:
	virtual bool doEncode(CParamPool &IOBuff);
	virtual bool doDecode(CParamPool &IOBuff);
public:
	unsigned char m_cFlag;
	vector<NearbyPlayer> m_listNearbyPlayer;
};
class GameMsg_S2G_CloseLBS:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_CloseLBS);
	GameMsg_S2G_CloseLBS();
	~GameMsg_S2G_CloseLBS();
public:
	virtual bool doEncode(CParamPool &IOBuff);
	virtual bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID;
};
#endif

