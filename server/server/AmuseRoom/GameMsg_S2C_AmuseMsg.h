#ifndef __GAMEMSG_S2C_AMUSEROOM_MSG_H__
#define __GAMEMSG_S2C_AMUSEROOM_MSG_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"
#include "../item/EntityItemComponent.h"
#include "../gene/EntityGeneComponent.h"
#include "../Pet/PetComponent.h"
#include <list>

#include "AmuseParam.h"

class CEntityMedalComponent;
class CAmuseActionParam_Base;
class CAmuseEventParam_Base;
class CEntityHorseComponent;

///////////////////////////////////////////////////////
class CAmuseRoomPlayerInfo
{
public:
	CAmuseRoomPlayerInfo();
	~CAmuseRoomPlayerInfo();

	void doEncode(CParamPool& IOBuff);

public:
	unsigned int m_nRoleID;
	std::string m_strRoleName;
	bool m_bIsHost;
	char m_nRoleSex;
	char m_nRoleSkin;
	char m_nDanceGroupTitle;
	std::string m_strDanceGroupName;
	unsigned short m_nDanceGroupBadgeId;
	unsigned short m_nDanceGroupEffectId;
	bool m_bIsVIP;
	unsigned short m_nVIPLevel;
	unsigned char m_nMoveType;
	unsigned short m_nTransFormID;

	float m_fOrient;
	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;

	CEntityItemComponent* m_pItemCOM;
	CEntityGeneComponent* m_pGeneCOM;
	PetComponent*         m_pPetCOM;
    CEntityMedalComponent   *m_pMedalCOM;
    CEntityHorseComponent   *m_pHorseCOM;
};


///////////////////////////////////////////////////////
class CAmuseRoomDeviceInfo
{
public:
	CAmuseRoomDeviceInfo();
	~CAmuseRoomDeviceInfo();

	void doEncode(CParamPool& IOBuff);

public:
	char m_nDeviceID;
	char m_nInviteType;
};


///////////////////////////////////////////////////////
class CAmuseRoomBriefInfo
{
public:
	CAmuseRoomBriefInfo();
	~CAmuseRoomBriefInfo();

	void doEncode(CParamPool& IOBuff);

public:
	unsigned int m_nRoomID;
	std::string m_strRoomName;
	bool m_bHasPwd;
	char m_chRoomTheme;
	char m_chRoomScene;
	char m_chRoomColor;
	bool m_bIsTop;
	char m_szPlayerSex[MAX_AMUSEROOM_PLAYER_COUNT];
};

enum eAmuseRoomType
{
	eAmuseRoomType_Invalid = 0,
	eAmuseRoomType_AmuseRoom,
	eAmuseRoomType_ConstellationRoom,
};

///////////////////////////////////////////////////////
class CAmuseRoomWholeInfo
{
public:
	CAmuseRoomWholeInfo();
	~CAmuseRoomWholeInfo();

	void doEncode(CParamPool& IOBuff);

public:
	unsigned int m_nRoomID;
	std::string m_strRoomName;
	std::string m_strRoomPwd;
	bool m_bHasPwd;
	bool m_bIsHost;
	char m_chRoomTheme;
	char m_chRoomScene;
	char m_chRoomCapacity;
	char m_chRoomType;

	std::list<CAmuseRoomPlayerInfo> m_lstAmusePlayer;
	std::list<CAmuseRoomDeviceInfo> m_lstAmuseDevice;
	std::list<CAmuseActionParam_Base*> m_lstPlayerAction;
	std::list<CAmuseEventParam_Base*> m_lstRoomEvent;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_GetAmuseRoomListRes : public GameMsg_Base
{
public:
	GameMsg_S2C_GetAmuseRoomListRes(void);
	~GameMsg_S2C_GetAmuseRoomListRes(void);

	bool virtual doEncode(CParamPool& IOBuff);

public:
	unsigned short m_nCurPage;
	unsigned short m_nTotalCount;

	std::vector<CAmuseRoomBriefInfo> m_vecRoomBriefInfo;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_CreateAmuseRoomSuc : public GameMsg_Base
{
public:
	GameMsg_S2C_CreateAmuseRoomSuc(void);
	~GameMsg_S2C_CreateAmuseRoomSuc(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	CAmuseRoomWholeInfo m_RoomWholeInfo;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_CreateAmuseRoomFail : public GameMsg_Base
{
public:
	GameMsg_S2C_CreateAmuseRoomFail(void);
	~GameMsg_S2C_CreateAmuseRoomFail(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::string m_strError;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_AmuseRoomRequriePwd : public GameMsg_Base
{
public:
	GameMsg_S2C_AmuseRoomRequriePwd();
	~GameMsg_S2C_AmuseRoomRequriePwd();

	virtual bool doEncode(CParamPool &IOBuff);

public:
	unsigned int m_nRoomID;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_EnterAmuseRoomFail : public GameMsg_Base
{
public:
	GameMsg_S2C_EnterAmuseRoomFail(void);
	~GameMsg_S2C_EnterAmuseRoomFail(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::string m_strError;
};

enum eAmuseRoomQuitTo
{
    eAmuseRoomQuitTo_Unknown = 0,
    eAmuseRoomQuitTo_World,
    eAmuseRoomQuitTo_AmuseLobby,
    eAmuseRoomQuitTo_ConstellationLobby,
    eAmuseRoomQuitTo_Dungeon,
};

///////////////////////////////////////////////////////
class GameMsg_S2C_QuitAmuseRoomSuc : public GameMsg_Base
{
public:
	GameMsg_S2C_QuitAmuseRoomSuc(void);
	~GameMsg_S2C_QuitAmuseRoomSuc(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	char m_chQuitType;
	unsigned int m_nKickerID;
	unsigned char m_nQuitTo;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_QuitAmuseRoomFail : public GameMsg_Base
{
public:
	GameMsg_S2C_QuitAmuseRoomFail(void);
	~GameMsg_S2C_QuitAmuseRoomFail(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::string m_strError;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_ChangeAmuseRoomInfoSuc : public GameMsg_Base
{
public:
	GameMsg_S2C_ChangeAmuseRoomInfoSuc(void);
	~GameMsg_S2C_ChangeAmuseRoomInfoSuc(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::string m_strRoomName;
	std::string m_strRoomPwd;
	bool m_bHasPwd;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_ChangeAmuseRoomInfoFail : public GameMsg_Base
{
public:
	GameMsg_S2C_ChangeAmuseRoomInfoFail(void);
	~GameMsg_S2C_ChangeAmuseRoomInfoFail(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::string m_strError;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_AmuseKickPlayerFail : public GameMsg_Base
{
public:
	GameMsg_S2C_AmuseKickPlayerFail(void);
	~GameMsg_S2C_AmuseKickPlayerFail(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::string m_strError;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_AmuseEnterPlayer : public GameMsg_Base
{
public:
	GameMsg_S2C_AmuseEnterPlayer(void);
	~GameMsg_S2C_AmuseEnterPlayer(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	CAmuseRoomPlayerInfo m_PlayerInfo;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_AmuseQuitPlayer : public GameMsg_Base
{
public:
	GameMsg_S2C_AmuseQuitPlayer(void);
	~GameMsg_S2C_AmuseQuitPlayer(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	char m_chQuitType;
	unsigned int m_nQuitID;
	unsigned int m_nNewHostID;
	std::string m_strRoomPwd;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_AmusePromoteHostSuc : public GameMsg_Base
{
public:
	GameMsg_S2C_AmusePromoteHostSuc();
	~GameMsg_S2C_AmusePromoteHostSuc();

	virtual bool doEncode(CParamPool & IOBuff);

public: 
	unsigned int m_nOldHostID;
	unsigned int m_nNewHostID;

	std::string m_strRoomPwd;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_AmusePromoteHostFail : public GameMsg_Base
{
public:
	GameMsg_S2C_AmusePromoteHostFail();
	~GameMsg_S2C_AmusePromoteHostFail();

	virtual bool doEncode(CParamPool & IOBuff);

public:
	std::string m_strError;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_AmuseInvitePlayerSuc : public GameMsg_Base
{
public:
	GameMsg_S2C_AmuseInvitePlayerSuc(void);
	~GameMsg_S2C_AmuseInvitePlayerSuc(void);

	virtual bool doEncode(CParamPool& IOBuff);
};


///////////////////////////////////////////////////////
class GameMsg_S2C_AmuseInvitePlayerFail : public GameMsg_Base
{
public:
	GameMsg_S2C_AmuseInvitePlayerFail(void);
	~GameMsg_S2C_AmuseInvitePlayerFail(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::string m_strError;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_AmuseInviteeNotice : public GameMsg_Base
{
public:
	GameMsg_S2C_AmuseInviteeNotice(void);
	~GameMsg_S2C_AmuseInviteeNotice(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	unsigned int m_nRoomID;

	unsigned int m_nInviterRoleID;
	std::string m_strInviterName;
	bool m_bIsInviterVIP;
	unsigned short m_nInviterVIPLevel;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_AmuseInviteeFail : public GameMsg_Base
{
public:
	GameMsg_S2C_AmuseInviteeFail(void);
	~GameMsg_S2C_AmuseInviteeFail(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::string m_strError;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_DoAmuseActionSuc : public GameMsg_Base
{
public:
	GameMsg_S2C_DoAmuseActionSuc(void);
	~GameMsg_S2C_DoAmuseActionSuc(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	CAmuseActionParam_Base* m_pActionParam;		//heap memory, assigned outside
};


///////////////////////////////////////////////////////
class GameMsg_S2C_DoAmuseActionFail : public GameMsg_Base
{
public:
	GameMsg_S2C_DoAmuseActionFail(void);
	~GameMsg_S2C_DoAmuseActionFail(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::string m_strError;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_AmuseEventNotice : public GameMsg_Base
{
public:
	GameMsg_S2C_AmuseEventNotice(void);
	~GameMsg_S2C_AmuseEventNotice(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	CAmuseEventParam_Base* m_pEventParam;	//heap memory, assigned outside
};

///////////////////////////////////////////////////////
class GameMsg_S2C_PlayMakerBroadcast : public GameMsg_Base
{
public:
	GameMsg_S2C_PlayMakerBroadcast(void);
	~GameMsg_S2C_PlayMakerBroadcast(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	PlayMakerInfo m_playerMakerInfo;
};

#endif
// end of file


