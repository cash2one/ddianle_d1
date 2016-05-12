#include "GameMsg_S2C_Friend.h"
#include "../share/ServerMsgDef.h"
#include "../../datastructure/Macro_Define.h"

GameMsg_S2C_FriendApplication::GameMsg_S2C_FriendApplication()
    : GameMsg_Base(MSG_S2C_FriendApplication)
    , m_bInsertMsgBox(false)
{
}

bool GameMsg_S2C_FriendApplication::doEncode(CParamPool &IOBuff)
{
    m_friendInfo.doEncode(IOBuff);
    IOBuff.AddBool(m_bInsertMsgBox);

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_AddBlackFriendFail::GameMsg_S2C_AddBlackFriendFail()
:GameMsg_Base(MSG_S2C_AddBlackFriendFail)
{

}

GameMsg_S2C_AddBlackFriendFail::~GameMsg_S2C_AddBlackFriendFail()
{

}

bool GameMsg_S2C_AddBlackFriendFail::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());
	
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_FriendApplicationSuc::GameMsg_S2C_FriendApplicationSuc()
:GameMsg_Base(MSG_S2C_FriendApplicationSuc)
{
}

GameMsg_S2C_FriendApplicationSuc::~GameMsg_S2C_FriendApplicationSuc()
{
}

bool GameMsg_S2C_FriendApplicationSuc::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW(m_strFriendName.c_str());

	return true;
}

GameMsg_S2C_FriendApplicationFail::GameMsg_S2C_FriendApplicationFail()
:GameMsg_Base(MSG_S2C_FriendApplicationFail)
,m_strError("")
{

}

GameMsg_S2C_FriendApplicationFail::~GameMsg_S2C_FriendApplicationFail()
{

}

bool GameMsg_S2C_FriendApplicationFail::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());	

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_AcceptApplicationSuc::GameMsg_S2C_AcceptApplicationSuc()
:GameMsg_Base(MSG_S2C_AcceptApplicationSuc)
{
}

GameMsg_S2C_AcceptApplicationSuc::~GameMsg_S2C_AcceptApplicationSuc()
{
}

bool GameMsg_S2C_AcceptApplicationSuc::doEncode(CParamPool &IOBuff)
{
	m_friendInfo.doEncode(IOBuff);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_AcceptApplicationFail::GameMsg_S2C_AcceptApplicationFail()
:GameMsg_Base(MSG_S2C_AcceptApplicationFail)
{

}

GameMsg_S2C_AcceptApplicationFail::~GameMsg_S2C_AcceptApplicationFail()
{

}

bool GameMsg_S2C_AcceptApplicationFail::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_RefuseApplicaionSuc::GameMsg_S2C_RefuseApplicaionSuc()
:GameMsg_Base(MSG_S2C_RefuseApplicationSuc)
,m_nRoleID(0)
{
}

GameMsg_S2C_RefuseApplicaionSuc::~GameMsg_S2C_RefuseApplicaionSuc()
{
}

bool GameMsg_S2C_RefuseApplicaionSuc::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_RefuseApplicaionFail::GameMsg_S2C_RefuseApplicaionFail()
:GameMsg_Base(MSG_S2C_RefuseApplicationFail)
,m_strError("")
{

}

GameMsg_S2C_RefuseApplicaionFail::~GameMsg_S2C_RefuseApplicaionFail()
{
}

bool GameMsg_S2C_RefuseApplicaionFail::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_RemoveFriendSuc::GameMsg_S2C_RemoveFriendSuc()
:GameMsg_Base(MSG_S2C_RemoveFriendSuc)
,m_nRoleID(0)
{

}

GameMsg_S2C_RemoveFriendSuc::~GameMsg_S2C_RemoveFriendSuc()
{
}

bool GameMsg_S2C_RemoveFriendSuc::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);

	return true;
}

GameMsg_S2C_RemoveFriendFail::GameMsg_S2C_RemoveFriendFail()
:GameMsg_Base(MSG_S2C_RemoveFriendFail)
,m_strError("")
{

}

GameMsg_S2C_RemoveFriendFail::~GameMsg_S2C_RemoveFriendFail()
{

}

bool GameMsg_S2C_RemoveFriendFail::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_AddBlackListSuc::GameMsg_S2C_AddBlackListSuc()
:GameMsg_Base(MSG_S2C_AddBlackListSuc)
{
}

GameMsg_S2C_AddBlackListSuc::~GameMsg_S2C_AddBlackListSuc()
{
}

bool GameMsg_S2C_AddBlackListSuc::doEncode(CParamPool &IOBuff)
{
	m_friendInfo.doEncode(IOBuff);
	
	return true;
}

GameMsg_S2C_AddBlackListFail::GameMsg_S2C_AddBlackListFail()
:GameMsg_Base(MSG_S2C_AddBlackListFail)
,m_strError("")
{

}

GameMsg_S2C_AddBlackListFail::~GameMsg_S2C_AddBlackListFail()
{

}

bool GameMsg_S2C_AddBlackListFail::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_RemoveBlackListSuc::GameMsg_S2C_RemoveBlackListSuc()
:GameMsg_Base(MSG_S2C_RemoveBlackListSuc)
,m_nRoleID(0)
{

}

GameMsg_S2C_RemoveBlackListSuc::~GameMsg_S2C_RemoveBlackListSuc()
{
}

bool GameMsg_S2C_RemoveBlackListSuc::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_RemoveBlackListFail::GameMsg_S2C_RemoveBlackListFail()
:GameMsg_Base(MSG_S2C_RemoveBlackListFail)
,m_strError("")
{

}

GameMsg_S2C_RemoveBlackListFail::~GameMsg_S2C_RemoveBlackListFail()
{

}

bool GameMsg_S2C_RemoveBlackListFail::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());	

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_GetFriendOnlineInfoResult::GameMsg_S2C_GetFriendOnlineInfoResult():
GameMsg_Base(MSG_S2C_GetFriendOnlineInfoReuslt)
{
}

GameMsg_S2C_GetFriendOnlineInfoResult::~GameMsg_S2C_GetFriendOnlineInfoResult()
{
}

bool GameMsg_S2C_GetFriendOnlineInfoResult::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort((unsigned short ) m_listFriend.size());
	for (list<CFriendOnlineInfo>::iterator itr = m_listFriend.begin(); itr != m_listFriend.end(); itr++) {
		itr->doEncode(IOBuff);
	}

	return true;
}

GameMsg_S2C_UpdateFriendInfo::GameMsg_S2C_UpdateFriendInfo()
:GameMsg_Base(MSG_S2C_UpdateFriendInfo)
{

}

GameMsg_S2C_UpdateFriendInfo::~GameMsg_S2C_UpdateFriendInfo()
{

}
bool GameMsg_S2C_UpdateFriendInfo::doEncode(CParamPool &IOBuff)
{
	m_friendOnlineInfo.doEncode(IOBuff);

	return true;
}
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_NotifyHasBeenAccepted::GameMsg_S2C_NotifyHasBeenAccepted():
GameMsg_Base(MSG_S2C_NotityHasBeenAccepted)
{
}

GameMsg_S2C_NotifyHasBeenAccepted::~GameMsg_S2C_NotifyHasBeenAccepted()
{
}

bool GameMsg_S2C_NotifyHasBeenAccepted::doEncode(CParamPool &IOBuff)
{
	m_friendInfo.doEncode(IOBuff);

	return true;
}

GameMsg_S2C_HasBeenRemovedFriend::GameMsg_S2C_HasBeenRemovedFriend()
:GameMsg_Base(MSG_S2C_HasBeenRemovedFriend)
,m_nSrcID(0)
{

}

GameMsg_S2C_HasBeenRemovedFriend::~GameMsg_S2C_HasBeenRemovedFriend()
{

}

bool GameMsg_S2C_HasBeenRemovedFriend::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUInt(m_nSrcID);
	
	return true;
}


GameMsg_S2C_HasBeenRemovedBlackList::GameMsg_S2C_HasBeenRemovedBlackList()
:GameMsg_Base(MSG_S2C_HasBeenRemovedBlackList)
,m_nSrcID(0)
{

}

GameMsg_S2C_HasBeenRemovedBlackList::~GameMsg_S2C_HasBeenRemovedBlackList()
{

}

bool GameMsg_S2C_HasBeenRemovedBlackList::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUInt(m_nSrcID);

	return true;
}

GameMsg_S2C_GetTempListPlayerOnlineInfoSuc::GameMsg_S2C_GetTempListPlayerOnlineInfoSuc()
:GameMsg_Base(MSG_S2C_GetTempListPlayerInfoSuc)
,m_nListType(0)
{

}

GameMsg_S2C_GetTempListPlayerOnlineInfoSuc::~GameMsg_S2C_GetTempListPlayerOnlineInfoSuc()
{

}

bool GameMsg_S2C_GetTempListPlayerOnlineInfoSuc::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUChar(m_nListType);
	IOBuff.AddUShort((unsigned short)m_listOnlineInfo.size());
	for (list<CFriendOnlineInfo>::iterator itr = m_listOnlineInfo.begin(); itr != m_listOnlineInfo.end(); ++itr) {
		itr->doEncode(IOBuff);
	}

	return true;
}

GameMsg_S2C_GetTempListPlayerOnlineInfoFail::GameMsg_S2C_GetTempListPlayerOnlineInfoFail()
:GameMsg_Base(MSG_S2C_GetTempListPlayerInfoFail)
,m_nListType(0)
,m_strError("")
{

}

GameMsg_S2C_GetTempListPlayerOnlineInfoFail::~GameMsg_S2C_GetTempListPlayerOnlineInfoFail()
{

}

bool GameMsg_S2C_GetTempListPlayerOnlineInfoFail::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUChar(m_nListType);
	IOBuff.AddStringW(m_strError.c_str());

	return true;
}

GameMsg_S2C_GetBlackListOnlineInfoSuc::GameMsg_S2C_GetBlackListOnlineInfoSuc()
:GameMsg_Base(MSG_S2C_GetBlackListOnlineInfoSuc)
{

}

GameMsg_S2C_GetBlackListOnlineInfoSuc::~GameMsg_S2C_GetBlackListOnlineInfoSuc()
{

}

bool GameMsg_S2C_GetBlackListOnlineInfoSuc::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort((unsigned short)m_listBlakList.size());		
	for (list<CFriendOnlineInfo>::iterator itr = m_listBlakList.begin(); itr != m_listBlakList.end(); ++itr) {
		itr->doEncode(IOBuff);
	}

	return true;
}

GameMsg_S2C_GetBlackListOnlineInfoFail::GameMsg_S2C_GetBlackListOnlineInfoFail()
:GameMsg_Base(MSG_S2C_GetBlackListOnlineInfoFail)
{

}

GameMsg_S2C_GetBlackListOnlineInfoFail::~GameMsg_S2C_GetBlackListOnlineInfoFail()
{

}

bool GameMsg_S2C_GetBlackListOnlineInfoFail::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());

	return true;
}


GameMsg_S2C_BlackList::GameMsg_S2C_BlackList()
:GameMsg_Base(MSG_S2C_BlackList)
{
}

GameMsg_S2C_BlackList::~GameMsg_S2C_BlackList()
{
}

bool GameMsg_S2C_BlackList::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUShort((unsigned short)m_mapBlackList.size());
	for (std::map< unsigned int, FriendInfo * >::iterator itr = m_mapBlackList.begin(); itr != m_mapBlackList.end(); ++itr) {
		if (itr->second != NULL) {
			itr->second->doEncode(IOBuff);
		}
	}
	return true;
}


///////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_FindPlayerResult::GameMsg_S2C_FindPlayerResult()
:GameMsg_Base(MSG_S2C_FindPlayerResult)
{

}

GameMsg_S2C_FindPlayerResult::~GameMsg_S2C_FindPlayerResult()
{

}
bool GameMsg_S2C_FindPlayerResult::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort((unsigned short)m_PlayerList.size());

	std::list<CFindPlayerInfo>::iterator it;
	for (it = m_PlayerList.begin(); it != m_PlayerList.end(); ++it)
	{
		it->doEncode(IOBuff);
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_MakeFriend::GameMsg_S2C_MakeFriend()
:GameMsg_Base(MSG_S2C_MakeFriend)
{
	m_nRoleID = 0;
	m_nSex = 0;
	m_bVIP = false;
	m_nVIPLevel = 1;
}

GameMsg_S2C_MakeFriend::~GameMsg_S2C_MakeFriend()
{

}

bool GameMsg_S2C_MakeFriend::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddStringW(m_strRoleName.c_str());
	IOBuff.AddUChar(m_nSex);
	IOBuff.AddBool(m_bVIP);
	IOBuff.AddUShort(m_nVIPLevel);
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_FindPlayerSetting::GameMsg_S2C_FindPlayerSetting()
: GameMsg_Base(MSG_S2C_FindPlayerSetting)
{

}

GameMsg_S2C_FindPlayerSetting::~GameMsg_S2C_FindPlayerSetting()
{

}

bool GameMsg_S2C_FindPlayerSetting::doEncode(CParamPool & IOBuff)
{
	std::map<int, CFindFriendCondition>::iterator it;
	
	IOBuff.AddUShort((unsigned short)m_FindFriendType.size());
	for (it = m_FindFriendType.begin(); it != m_FindFriendType.end(); ++it)
	{
		it->second.doEncode(IOBuff);
	}
	IOBuff.AddUShort((unsigned short)m_FindFriendLevel.size());
	for (it = m_FindFriendLevel.begin(); it != m_FindFriendLevel.end(); ++it)
	{
		it->second.doEncode(IOBuff);
	}
	IOBuff.AddUShort((unsigned short)m_FindFriendSex.size());
	for (it = m_FindFriendSex.begin(); it != m_FindFriendSex.end(); ++it)
	{
		it->second.doEncode(IOBuff);
	}
	IOBuff.AddUShort((unsigned short)m_FindFriendAge.size());
	for (it = m_FindFriendAge.begin(); it != m_FindFriendAge.end(); ++it)
	{
		it->second.doEncode(IOBuff);
	}
	IOBuff.AddUShort((unsigned short)m_FindFriendConstellation.size());
	for (it = m_FindFriendConstellation.begin(); it != m_FindFriendConstellation.end(); ++it)
	{
		it->second.doEncode(IOBuff);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_S2C_FindPlayerFail::GameMsg_S2C_FindPlayerFail()
:GameMsg_Base(MSG_S2C_FindPlayerFail)
{
	m_strError = "";
	m_nCoolDownTime = 0;
}

GameMsg_S2C_FindPlayerFail::~GameMsg_S2C_FindPlayerFail()
{
}

bool GameMsg_S2C_FindPlayerFail::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());
	IOBuff.AddInt(m_nCoolDownTime);
	return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_GetRecommendFriendsResult::GameMsg_S2C_GetRecommendFriendsResult()
    : GameMsg_Base(MSG_S2C_GetRecommendFriendsResult)
    , m_nError(ERecommendFriendsError_Success)
    , m_fCD(0.0f)
    , m_nRefreshCount(0)
{
}

bool GameMsg_S2C_GetRecommendFriendsResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nError);
    IOBuff.AddFloat(m_fCD);
    IOBuff.AddUShort(m_nRefreshCount);
    IOBuff.AddUShort((unsigned short)m_listPlayers.size() );

    for (std::list<CFindPlayerInfo>::iterator itr = m_listPlayers.begin(); 
         itr != m_listPlayers.end(); ++itr)
    {
        itr->doEncode(IOBuff);
    }

    return true;
}


