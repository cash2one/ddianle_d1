#include "GameMsg_C2S_Friend.h"
#include "../share/ServerMsgDef.h"
#include "../../socket/Utility.h"


GameMsg_C2S_FriendApplication::GameMsg_C2S_FriendApplication()
    : GameMsg_Base(MSG_C2S_FriendApplication)
    , m_nFriendRoleID(0)
    , m_nFriendCallBy(0)
    , m_nFriendCallTo(0)
    , m_bInsertMsgBox(false)
{
}

bool GameMsg_C2S_FriendApplication::doDecode(CParamPool &IOBuff)
{
    m_nFriendRoleID = IOBuff.GetUInt();
    m_nFriendCallBy = IOBuff.GetUChar();
    m_nFriendCallTo = IOBuff.GetUChar();
    m_bInsertMsgBox = IOBuff.GetBool();

    return true;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
GameMsg_C2S_AddBlackFriend::GameMsg_C2S_AddBlackFriend()
:GameMsg_Base(MSG_C2S_AddBlackFriend)
,m_nRoleID(0)
{

}

GameMsg_C2S_AddBlackFriend::~GameMsg_C2S_AddBlackFriend()
{

}

bool GameMsg_C2S_AddBlackFriend::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();

	return true;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
GameMsg_C2S_AcceptApplication::GameMsg_C2S_AcceptApplication()
:GameMsg_Base(MSG_C2S_AcceptApplication)
,m_nFriendRoleID(0)
,m_nFriendCallBy(0)
,m_nFriendCallTo(0)
{

}

GameMsg_C2S_AcceptApplication::~GameMsg_C2S_AcceptApplication()
{

}

bool GameMsg_C2S_AcceptApplication::doDecode( CParamPool &IOBuff )
{
	m_nFriendRoleID = IOBuff.GetUInt();
	m_nFriendCallBy = IOBuff.GetUChar();
	m_nFriendCallTo = IOBuff.GetUChar();

	return true;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
GameMsg_C2S_RefuseApplication::GameMsg_C2S_RefuseApplication()
:GameMsg_Base(MSG_C2S_RefuseApplication)
,m_nFriendCallBy(0)
,m_nFriendCallTo(0)
{
}

GameMsg_C2S_RefuseApplication::~GameMsg_C2S_RefuseApplication()
{
}

bool GameMsg_C2S_RefuseApplication::doDecode(CParamPool &IOBuff)
{
	m_nFriendRoleID = IOBuff.GetUInt();
	m_nFriendCallBy = IOBuff.GetUChar();
	m_nFriendCallTo = IOBuff.GetUChar();

	return true;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
GameMsg_C2S_RemoveFriend::GameMsg_C2S_RemoveFriend():
GameMsg_Base(MSG_C2S_RemoveFriend)
{
}

GameMsg_C2S_RemoveFriend::GameMsg_C2S_RemoveFriend(unsigned int nFriendRoleID)
    : GameMsg_Base(MSG_C2S_RemoveFriend)
{
	m_nFriendRoleID = nFriendRoleID;	
}

GameMsg_C2S_RemoveFriend::~GameMsg_C2S_RemoveFriend()
{
}
bool GameMsg_C2S_RemoveFriend::doDecode(CParamPool &IOBuff)
{
	m_nFriendRoleID = IOBuff.GetUInt();
	return true;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
GameMsg_C2S_AddBlackList::GameMsg_C2S_AddBlackList()
:GameMsg_Base(MSG_C2S_AddBlackList)
,m_nRoleID(0)
{

}

GameMsg_C2S_AddBlackList::~GameMsg_C2S_AddBlackList()
{
}

bool GameMsg_C2S_AddBlackList::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();

	return true;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
GameMsg_C2S_RemoveBlackList::GameMsg_C2S_RemoveBlackList():
GameMsg_Base(MSG_C2S_RemoveBlackList)
{
}

GameMsg_C2S_RemoveBlackList::GameMsg_C2S_RemoveBlackList(unsigned int nFriendRoleID)
    : GameMsg_Base(MSG_C2S_RemoveFriend)
{
	m_nBlackRoleID = nFriendRoleID;
}

GameMsg_C2S_RemoveBlackList::~GameMsg_C2S_RemoveBlackList()
{
}
bool GameMsg_C2S_RemoveBlackList::doDecode(CParamPool &IOBuff)
{
	m_nBlackRoleID = IOBuff.GetUInt();

	return true;
}
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//GameMsg_C2S_GetFriendOnlineInfo::GameMsg_C2S_GetFriendOnlineInfo():
//GameMsg_Base(MSG_C2S_GetFriendOnlineInfo)
//{
//
//}
//
//GameMsg_C2S_GetFriendOnlineInfo::~GameMsg_C2S_GetFriendOnlineInfo()
//{
//}
//
//bool GameMsg_C2S_GetFriendOnlineInfo::doDecode(CParamPool &IOBuff)
//{
//	return true;
//}
/////////////////////////////////////////////////////////////////////////
GameMsg_C2S_GetTempListPlayerOnlineInfo::GameMsg_C2S_GetTempListPlayerOnlineInfo()
:GameMsg_Base(MSG_C2S_GetTempListPlayerOnlineInfo)
,m_nListType(0)
{

}

GameMsg_C2S_GetTempListPlayerOnlineInfo::~GameMsg_C2S_GetTempListPlayerOnlineInfo()
{

}

bool GameMsg_C2S_GetTempListPlayerOnlineInfo::doDecode( CParamPool & IOBUff )
{
	m_nListType = IOBUff.GetUChar();

	return true;
}

GameMsg_C2S_GetBlackListOnlineInfo::GameMsg_C2S_GetBlackListOnlineInfo()
:GameMsg_Base(MSG_C2S_GetBlackListOnlineInfo)
{

}

GameMsg_C2S_GetBlackListOnlineInfo::~GameMsg_C2S_GetBlackListOnlineInfo()
{

}

bool GameMsg_C2S_GetBlackListOnlineInfo::doDecode( CParamPool & IOBuff )
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_FindPlayer::GameMsg_C2S_FindPlayer()
:GameMsg_Base(MSG_C2S_FindPlayer)
{
	m_nFindFriendTypeIndex = 0;
	m_nFindRoleID = 0;
	m_strFindRoleName = "";
	m_nFindLevelIndex = 0;
	m_nFindSexIndex = 0;
	m_nFindAgeIndex = 0;
	m_nFindConstellationIndex = 0;
}
GameMsg_C2S_FindPlayer::~GameMsg_C2S_FindPlayer()
{
}
bool GameMsg_C2S_FindPlayer::doDecode( CParamPool & IOBUff )
{
	m_nFindFriendTypeIndex = IOBUff.GetUChar();
	m_nFindRoleID = IOBUff.GetUInt();
	IOBUff.GetStringW(m_strFindRoleName);
	m_nFindLevelIndex = IOBUff.GetUChar();
	m_nFindSexIndex = IOBUff.GetUChar();
	m_nFindAgeIndex = IOBUff.GetUChar();
	m_nFindConstellationIndex = IOBUff.GetUChar();

	return true;
}

GameMsg_C2S_ReworkFrindNotes::GameMsg_C2S_ReworkFrindNotes()
:GameMsg_Base(MSG_C2S_ReworkFriendNotes)
,m_nSelectRoleID( 0 )
,m_strNotes( "" )
{
}

GameMsg_C2S_ReworkFrindNotes::~GameMsg_C2S_ReworkFrindNotes()
{

}

bool GameMsg_C2S_ReworkFrindNotes::doDecode( CParamPool & IOBuff )
{
	m_nSelectRoleID = IOBuff.GetUInt();
	IOBuff.GetStringW( m_strNotes );
	return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_GetRecommendFriends::GameMsg_C2S_GetRecommendFriends()
    : GameMsg_Base(MSG_C2S_GetRecommendFriends)
    , m_nRecommendType(ERecommendFriendsType_None)
    , m_nMaxCount(0)
{
}

bool GameMsg_C2S_GetRecommendFriends::doDecode(CParamPool &IOBuff)
{
    m_nRecommendType = IOBuff.GetUChar();
    m_nMaxCount = IOBuff.GetUInt();

    return true;
}

