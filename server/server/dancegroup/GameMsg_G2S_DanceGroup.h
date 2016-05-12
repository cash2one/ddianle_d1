#ifndef	__GAMEMSG_G2S_DANCE_GROUP_H__
#define	__GAMEMSG_G2S_DANCE_GROUP_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DanceGroup.h"

class CParamPool;
class CDanceGroupInfo;

class GameMsg_G2S_UpdateDanceGroupBaseInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_UpdateDanceGroupBaseInfo);
    GameMsg_G2S_UpdateDanceGroupBaseInfo();
    ~GameMsg_G2S_UpdateDanceGroupBaseInfo() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short      m_nUpdateType;
    CDanceGroupBaseInfo m_DGBaseInfo;       // 基本信息
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_G2S_AddDanceGroupMemberInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_AddDanceGroupMemberInfo);
    GameMsg_G2S_AddDanceGroupMemberInfo();
    ~GameMsg_G2S_AddDanceGroupMemberInfo() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;         // 舞团ID
    CDanceGroupMember   m_MemberInfo;       // 成员信息
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_G2S_UpdateDanceGroupMemberInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_UpdateDanceGroupMemberInfo);
    GameMsg_G2S_UpdateDanceGroupMemberInfo();
    ~GameMsg_G2S_UpdateDanceGroupMemberInfo() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

    unsigned int        m_nGroupID;         // 舞团ID
    CDanceGroupMember   m_MemberInfo;       // 成员信息
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_G2S_RemoveDanceGroupMemberInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_RemoveDanceGroupMemberInfo);
    GameMsg_G2S_RemoveDanceGroupMemberInfo();
    ~GameMsg_G2S_RemoveDanceGroupMemberInfo() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;
    unsigned int        m_nRoleID;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_G2S_AddRequestDanceGroupInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_AddRequestDanceGroupInfo);
    GameMsg_G2S_AddRequestDanceGroupInfo();
    ~GameMsg_G2S_AddRequestDanceGroupInfo() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int           m_nGroupID;      // 舞团ID
    CRequestDanceGroupInfo m_RequestInfo;   // 申请信息
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_G2S_RemoveRequestDanceGroupInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_RemoveRequestDanceGroupInfo);
    GameMsg_G2S_RemoveRequestDanceGroupInfo();
    ~GameMsg_G2S_RemoveRequestDanceGroupInfo() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;         // 舞团ID
    unsigned int        m_nRoleID;          // 角色ID
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_G2S_CreateDanceGroupResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_CreateDanceGroupResult);
    GameMsg_G2S_CreateDanceGroupResult();
    ~GameMsg_G2S_CreateDanceGroupResult();

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short      m_nResult;
    unsigned int        m_nCreateCD;
    CDanceGroupInfo     *m_pDGInfo;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_G2S_EnterDanceGroupSuccess : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_EnterDanceGroupSuccess);
    GameMsg_G2S_EnterDanceGroupSuccess();
    ~GameMsg_G2S_EnterDanceGroupSuccess();

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    CDanceGroupInfo     *m_pDGInfo;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_G2S_DonateDanceGroupResourceResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_DonateDanceGroupResourceResult);
    GameMsg_G2S_DonateDanceGroupResourceResult();
    ~GameMsg_G2S_DonateDanceGroupResourceResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short      m_nResult;
    unsigned int        m_nResA;
    unsigned int        m_nResB;
    unsigned int        m_nResC;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_G2S_UpdateDanceGroupActivityStarState : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_UpdateDanceGroupActivityStarState);
    GameMsg_G2S_UpdateDanceGroupActivityStarState();
    ~GameMsg_G2S_UpdateDanceGroupActivityStarState(){}

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;
    unsigned short      m_nStarLevel;
    unsigned int        m_nCurrentPower;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_G2S_UpdateDanceGroupActivityInfo : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_UpdateDanceGroupActivityInfo);
	GameMsg_G2S_UpdateDanceGroupActivityInfo();
	~GameMsg_G2S_UpdateDanceGroupActivityInfo() {}

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	unsigned int m_nGroupID;
	CDanceGroupActivityInfo m_nActivityInfo;

};


//重新获取数据
class GameMsg_G2S_DanceGroupActivityReset : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE( GameMsg_G2S_DanceGroupActivityReset );
	GameMsg_G2S_DanceGroupActivityReset();
	~GameMsg_G2S_DanceGroupActivityReset() {}

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	unsigned int m_nGroupID;
	unsigned int m_nRoleID;

};

class GameMsg_G2S_DanceGroupAddRecordResult : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE( GameMsg_G2S_DanceGroupAddRecordResult );
	GameMsg_G2S_DanceGroupAddRecordResult();
	~GameMsg_G2S_DanceGroupAddRecordResult() {}

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	CDanceGroupRecordsInfo m_recordInfo;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_G2S_DanceGroupShopGetInfoResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_DanceGroupShopGetInfoResult);
    GameMsg_G2S_DanceGroupShopGetInfoResult();
    ~GameMsg_G2S_DanceGroupShopGetInfoResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short      m_nErrorCode;
    std::map<unsigned short, unsigned short> m_mapUseCount;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_G2S_GetMyDanceGroupInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_GetMyDanceGroupInfo);
    GameMsg_G2S_GetMyDanceGroupInfo();
    ~GameMsg_G2S_GetMyDanceGroupInfo();

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short      m_nClickedPlace;
    CDanceGroupInfo     *m_pDGInfo;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_G2S_AcceptDanceGroupMemberInvitationResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_AcceptDanceGroupMemberInvitationResult);
    GameMsg_G2S_AcceptDanceGroupMemberInvitationResult();
    ~GameMsg_G2S_AcceptDanceGroupMemberInvitationResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nResult;
};

// MSG_G2S_BroadcastMemberKicked
class GameMsg_G2S_BroadcastMemberKicked : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_BroadcastMemberKicked);
    GameMsg_G2S_BroadcastMemberKicked();
    ~GameMsg_G2S_BroadcastMemberKicked();

public: 
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int m_nKickedRoleID;
    unsigned int m_nGroupID; // 被提出的公会ID
};

class GameMsg_G2S_BroadcastMemberQuit : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_BroadcastMemberQuit);
    GameMsg_G2S_BroadcastMemberQuit();
    ~GameMsg_G2S_BroadcastMemberQuit();

public: 
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int m_nQuitRoleID;
    unsigned int m_nGroupID; // 被提出的公会ID
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_G2S_UpdateDanceGroupChallengeActive : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_G2S_UpdateDanceGroupChallengeActive)

public:
    bool doEncode( CParamPool& IOBuff );
    bool doDecode( CParamPool& IOBuff );

public:
    unsigned int m_nGroupID;
    unsigned int m_nActive;
};

#endif
// end of file


