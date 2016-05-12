#ifndef __GAMEMSG_C2S_FRIEND_H__
#define __GAMEMSG_C2S_FRIEND_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"
#include <string>

class CParamPool;
using namespace std;


enum TempPlayerList
{
	TempPlayerList_RecentlyPlayMate,	// 最近玩伴
	TempPlayerList_RecentlyChatMate,	// 最近聊天
};


class GameMsg_C2S_FriendApplication : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_FriendApplication);
    GameMsg_C2S_FriendApplication();
    ~GameMsg_C2S_FriendApplication() {}

public:
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int    m_nFriendRoleID;
    unsigned char   m_nFriendCallBy;    // 被申请人的称呼
    unsigned char   m_nFriendCallTo;    // 申请人的称呼
    bool            m_bInsertMsgBox;    // 是否放入消息盒
};
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_AddBlackFriend:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_AddBlackFriend);
	GameMsg_C2S_AddBlackFriend();
	~GameMsg_C2S_AddBlackFriend();
public:
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID;	
};

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_AcceptApplication:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_AcceptApplication);
	GameMsg_C2S_AcceptApplication();
	~GameMsg_C2S_AcceptApplication();
public:
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nFriendRoleID;
	unsigned char m_nFriendCallBy;
	unsigned char m_nFriendCallTo;
};

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_RefuseApplication:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_RefuseApplication);
	GameMsg_C2S_RefuseApplication();
	~GameMsg_C2S_RefuseApplication();
public:
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nFriendRoleID;
	unsigned char m_nFriendCallBy;
	unsigned char m_nFriendCallTo;
};

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_RemoveFriend:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_RemoveFriend);
	GameMsg_C2S_RemoveFriend();
	GameMsg_C2S_RemoveFriend(unsigned int nnFriend);
	~GameMsg_C2S_RemoveFriend();
public:
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nFriendRoleID;
};


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_AddBlackList:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_AddBlackList);
	GameMsg_C2S_AddBlackList();
	~GameMsg_C2S_AddBlackList();
public:
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID;
};

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_RemoveBlackList:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_RemoveBlackList);
	GameMsg_C2S_RemoveBlackList();
	GameMsg_C2S_RemoveBlackList(unsigned int nFriendRoleID);
	~GameMsg_C2S_RemoveBlackList();
public:
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nBlackRoleID;
};

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//class GameMsg_C2S_GetFriendOnlineInfo:
//	public GameMsg_Base
//{
//public:
//	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetFriendOnlineInfo);
//	GameMsg_C2S_GetFriendOnlineInfo();
//	~GameMsg_C2S_GetFriendOnlineInfo();
//public:
//	bool doDecode(CParamPool &IOBuff);
//public:
//};

////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_GetTempListPlayerOnlineInfo :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetTempListPlayerOnlineInfo);
	GameMsg_C2S_GetTempListPlayerOnlineInfo();
	~GameMsg_C2S_GetTempListPlayerOnlineInfo();
public:
	bool doDecode( CParamPool & IOBUff );
public:
	unsigned char m_nListType;	// 0 最近玩伴 1 最近聊天	
};


class GameMsg_C2S_GetBlackListOnlineInfo : 
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetBlackListOnlineInfo);
	GameMsg_C2S_GetBlackListOnlineInfo ();
	~GameMsg_C2S_GetBlackListOnlineInfo();
public:
	bool doDecode(CParamPool & IOBuff);
};

////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_FindPlayer :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_FindPlayer);
	GameMsg_C2S_FindPlayer();
	~GameMsg_C2S_FindPlayer();
public:
	bool doDecode( CParamPool & IOBUff );
public:
	unsigned char m_nFindFriendTypeIndex;
	unsigned int m_nFindRoleID;
	std::string m_strFindRoleName;
	unsigned char m_nFindLevelIndex;
	unsigned char m_nFindSexIndex;
	unsigned char m_nFindAgeIndex;
	unsigned char m_nFindConstellationIndex;
};

class GameMsg_C2S_ReworkFrindNotes : 
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ReworkFrindNotes);
	GameMsg_C2S_ReworkFrindNotes ();
	~GameMsg_C2S_ReworkFrindNotes();
public:
	bool doDecode(CParamPool & IOBuff);

public :
	unsigned int         m_nSelectRoleID;
	std::string          m_strNotes;
};

//////////////////////////////////////////////////////////////////////////

enum ERecommendFriendsType
{
    ERecommendFriendsType_None = 0,
    ERecommendFriendsType_Male,
    ERecommendFriendsType_Female,

    ERecommendFriendsType_Max
};

class GameMsg_C2S_GetRecommendFriends : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetRecommendFriends);
    GameMsg_C2S_GetRecommendFriends();
    ~GameMsg_C2S_GetRecommendFriends() {}

public:
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned char       m_nRecommendType;
    unsigned int        m_nMaxCount;
};


#endif

