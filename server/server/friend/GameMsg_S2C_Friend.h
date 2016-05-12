#ifndef __GAMEMSG_S2C_FRIEND_H__
#define __GAMEMSG_S2C_FRIEND_H__

#include <string>
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/datainfor/ItemType.h"
#include "../../datastructure/Macro_Define.h"
#include "../../datastructure/DataStruct_DB.h"
#include "../../datastructure/FriendDataManager.h"


using namespace std;
class CParamPool;

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_FriendApplication : public GameMsg_Base
{
public:
    GameMsg_S2C_FriendApplication();
    ~GameMsg_S2C_FriendApplication() {}

public:
    bool doEncode(CParamPool &IOBuff);

public:
    FriendInfo  m_friendInfo;
    bool        m_bInsertMsgBox;
};

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_AddBlackFriendFail
:public GameMsg_Base
{
public:
	GameMsg_S2C_AddBlackFriendFail();
	~GameMsg_S2C_AddBlackFriendFail();
public:
	bool doEncode(CParamPool & IOBuff);
public:
	string m_strError;
};


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_FriendApplicationSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_FriendApplicationSuc();
	~GameMsg_S2C_FriendApplicationSuc();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	string m_strFriendName;
};

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_FriendApplicationFail:
	public GameMsg_Base
{
public:
	GameMsg_S2C_FriendApplicationFail();
	~GameMsg_S2C_FriendApplicationFail();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	string m_strError;
};

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_AcceptApplicationSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_AcceptApplicationSuc();
	~GameMsg_S2C_AcceptApplicationSuc();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	FriendInfo m_friendInfo;
};

class GameMsg_S2C_AcceptApplicationFail:
	public GameMsg_Base
{
public:
	GameMsg_S2C_AcceptApplicationFail();
	~GameMsg_S2C_AcceptApplicationFail();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	string m_strError;
};

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_RefuseApplicaionSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_RefuseApplicaionSuc();
	~GameMsg_S2C_RefuseApplicaionSuc();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID;
};

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_RefuseApplicaionFail:
	public GameMsg_Base
{
public:
	GameMsg_S2C_RefuseApplicaionFail();
	~GameMsg_S2C_RefuseApplicaionFail();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	string m_strError;
};

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_RemoveFriendSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_RemoveFriendSuc();
	~GameMsg_S2C_RemoveFriendSuc();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID;
};

class GameMsg_S2C_RemoveFriendFail:
	public GameMsg_Base
{
public:
	GameMsg_S2C_RemoveFriendFail();
	~GameMsg_S2C_RemoveFriendFail();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	string m_strError;
};

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_AddBlackListSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_AddBlackListSuc();
	~GameMsg_S2C_AddBlackListSuc();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	FriendInfo m_friendInfo;
};

class GameMsg_S2C_AddBlackListFail:
	public GameMsg_Base
{
public:
	GameMsg_S2C_AddBlackListFail();
	~GameMsg_S2C_AddBlackListFail();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	string m_strError;
};


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_RemoveBlackListSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_RemoveBlackListSuc();
	~GameMsg_S2C_RemoveBlackListSuc();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID; 
};

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_RemoveBlackListFail:
	public GameMsg_Base
{
public:
	GameMsg_S2C_RemoveBlackListFail();
	~GameMsg_S2C_RemoveBlackListFail();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	string m_strError;
};

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_GetFriendOnlineInfoResult:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetFriendOnlineInfoResult();
	~GameMsg_S2C_GetFriendOnlineInfoResult();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	list<CFriendOnlineInfo> m_listFriend;
};
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_UpdateFriendInfo:
	public GameMsg_Base
{
public:
	GameMsg_S2C_UpdateFriendInfo();
	~GameMsg_S2C_UpdateFriendInfo();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	CFriendOnlineInfo m_friendOnlineInfo;		
};

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_NotifyHasBeenAccepted:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_NotifyHasBeenAccepted);
	GameMsg_S2C_NotifyHasBeenAccepted();
	~GameMsg_S2C_NotifyHasBeenAccepted();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	FriendInfo m_friendInfo;
};


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_HasBeenRemovedFriend:
	public GameMsg_Base
{
public:
	GameMsg_S2C_HasBeenRemovedFriend();
	~GameMsg_S2C_HasBeenRemovedFriend();
public:
	bool doEncode(CParamPool & IOBuff);
public:
	unsigned int m_nSrcID;
};

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_HasBeenRemovedBlackList:
	public GameMsg_Base
{
public:
	GameMsg_S2C_HasBeenRemovedBlackList();
	~GameMsg_S2C_HasBeenRemovedBlackList();
public:
	bool doEncode(CParamPool & IOBuff);
public:
	unsigned int m_nSrcID;
};

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_GetTempListPlayerOnlineInfoSuc :
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetTempListPlayerOnlineInfoSuc();
	~GameMsg_S2C_GetTempListPlayerOnlineInfoSuc();
public:
	bool doEncode(CParamPool & IOBuff);
public:
	unsigned char m_nListType;
	list<CFriendOnlineInfo> m_listOnlineInfo;
};

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_GetTempListPlayerOnlineInfoFail : 
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetTempListPlayerOnlineInfoFail();
	~GameMsg_S2C_GetTempListPlayerOnlineInfoFail();
public:
	bool doEncode(CParamPool & IOBuff);
public:			
	unsigned char m_nListType;
	string m_strError;
};

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_GetBlackListOnlineInfoSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetBlackListOnlineInfoSuc();
	~GameMsg_S2C_GetBlackListOnlineInfoSuc();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	list<CFriendOnlineInfo> m_listBlakList;
};

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_GetBlackListOnlineInfoFail : 
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetBlackListOnlineInfoFail();
	~GameMsg_S2C_GetBlackListOnlineInfoFail();
public:
	bool doEncode(CParamPool & IOBuff);
public:
	string m_strError;
};

///////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_BlackList : 
	public GameMsg_Base
{
public:
	GameMsg_S2C_BlackList();
	~GameMsg_S2C_BlackList();
public:
	bool doEncode(CParamPool & IOBuff);
public:
	map< unsigned int, FriendInfo * > m_mapBlackList;
};
///////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_MakeFriend :
	public GameMsg_Base
{
public:
	GameMsg_S2C_MakeFriend();
	~GameMsg_S2C_MakeFriend();
public:
	bool doEncode(CParamPool & IOBuff);
public:
	unsigned int m_nRoleID;
	string m_strRoleName;
	unsigned char m_nSex;
	bool m_bVIP;
	unsigned short m_nVIPLevel;
};

///////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_FindPlayerResult :
	public GameMsg_Base
{
public:
	GameMsg_S2C_FindPlayerResult();
	~GameMsg_S2C_FindPlayerResult();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	std::list<CFindPlayerInfo> m_PlayerList;
};

///////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_FindPlayerSetting :
	public GameMsg_Base
{
public:
	GameMsg_S2C_FindPlayerSetting();
	~GameMsg_S2C_FindPlayerSetting();
public:
	bool doEncode(CParamPool & IOBuff);
public:
	std::map<int, CFindFriendCondition> m_FindFriendType;
	std::map<int, CFindFriendCondition> m_FindFriendLevel;
	std::map<int, CFindFriendCondition> m_FindFriendSex;
	std::map<int, CFindFriendCondition> m_FindFriendAge;
	std::map<int, CFindFriendCondition> m_FindFriendConstellation;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_FindPlayerFail :
	public GameMsg_Base
{
public:
	GameMsg_S2C_FindPlayerFail();
	~GameMsg_S2C_FindPlayerFail();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	std::string m_strError;
	int m_nCoolDownTime;
};

//////////////////////////////////////////////////////////////////////////

enum ERecommendFriendsError
{
    ERecommendFriendsError_Success,
    ERecommendFriendsError_InCD,            // CD中
    ERecommendFriendsError_InvalidType,     // ERecommendFriendsType无效
    ERecommendFriendsError_NotReset,        // 未重置
};

class GameMsg_S2C_GetRecommendFriendsResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetRecommendFriendsResult);
    GameMsg_S2C_GetRecommendFriendsResult();
    ~GameMsg_S2C_GetRecommendFriendsResult() {}

public:
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned char               m_nError;
    float                       m_fCD;
    unsigned short              m_nRefreshCount;
    std::list<CFindPlayerInfo>  m_listPlayers;
};

#endif

