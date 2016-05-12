#include "GameMsg_S2G_Dungeon.h"
#include "../../socket/ParamPool.h"
#include "../share/ServerMsgDef.h"

////////////////////////////////////////////////////////
GameMsg_S2G_GetRankList::GameMsg_S2G_GetRankList()
:GameMsg_Base( MSG_S2G_GetRankList )
,m_nRoleID( 0 )
,m_nCurrentPage(0)
{
};

GameMsg_S2G_GetRankList::~GameMsg_S2G_GetRankList()
{
};

bool GameMsg_S2G_GetRankList::doDecode( CParamPool &IOBuff )
{
	m_nRoleID = IOBuff.GetUInt();
	m_nCurrentPage = IOBuff.GetUInt();
	return true;
}

bool GameMsg_S2G_GetRankList::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt( m_nRoleID );
	IOBuff.AddUInt( m_nCurrentPage );
	return true;
}

///////////////////////////////////////////////////////////////////
GameMsg_S2G_UpdateParcloseInfo::GameMsg_S2G_UpdateParcloseInfo()
:GameMsg_Base( MSG_S2G_UpdateParcloseInfo )
,m_nRoleID(0)
,m_nParcloseID(0)
,m_nStarCount(0)
{

}

bool GameMsg_S2G_UpdateParcloseInfo::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt( m_nRoleID );
	IOBuff.AddUInt( m_nParcloseID );
	IOBuff.AddUInt( m_nStarCount );

	return true;
}

bool GameMsg_S2G_UpdateParcloseInfo::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();
	m_nParcloseID = IOBuff.GetUInt();
	m_nStarCount = IOBuff.GetUInt();

	return true;
}

///////////////////////////////////////////////////////////////////
GameMsg_S2G_ClearDungeonInviteFriendInfo::GameMsg_S2G_ClearDungeonInviteFriendInfo()
    :GameMsg_Base( MSG_S2G_ClearInviteFriendInfo )
    ,m_nRoleID(0)
{

}

bool GameMsg_S2G_ClearDungeonInviteFriendInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt( m_nRoleID );

    return true;
}

bool GameMsg_S2G_ClearDungeonInviteFriendInfo::doDecode(CParamPool &IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();

    return true;
}

///////////////////////////////////////////////////////////////////
GameMsg_S2G_AddDungeonInviteFriendInfo::GameMsg_S2G_AddDungeonInviteFriendInfo()
    :GameMsg_Base( MSG_S2G_AddInviteFriendInfo )
    ,m_nRoleID(0)
    ,m_nFriendRoleID(0)
    ,m_nInviteTime(0)
{
}

bool GameMsg_S2G_AddDungeonInviteFriendInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt( m_nRoleID );
    IOBuff.AddUInt( m_nFriendRoleID );
    IOBuff.AddUInt( m_nInviteTime );

    return true;
}

bool GameMsg_S2G_AddDungeonInviteFriendInfo::doDecode(CParamPool &IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    m_nFriendRoleID = IOBuff.GetUInt();
    m_nInviteTime = IOBuff.GetUInt();

    return true;
}

///////////////////////////////////////////////////////////////////
GameMsg_S2G_RemoveDungeonInviteInfo::GameMsg_S2G_RemoveDungeonInviteInfo()
	:GameMsg_Base( MSG_S2G_DelInviteFriendInfo )
	,m_nRoleID(0)
	,m_nDeleteRoleID(0)
{


}

bool GameMsg_S2G_RemoveDungeonInviteInfo::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt( m_nRoleID );
	IOBuff.AddUInt( m_nDeleteRoleID ); 
	return true;
}

bool GameMsg_S2G_RemoveDungeonInviteInfo::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();
	m_nDeleteRoleID = IOBuff.GetUInt();
	return true;
}

///////////////////////////////////////////////////////////////////
GameMsg_S2G_GiveVitToFriend::GameMsg_S2G_GiveVitToFriend()
    :GameMsg_Base( MSG_S2G_GiveVitToFriend )
    ,m_nGiveVitRoleID(0)
    ,m_strGiveVitRoleName("")
    ,m_nVitNum(0)
{
}

bool GameMsg_S2G_GiveVitToFriend::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGiveVitRoleID);
    IOBuff.AddStringW(m_strGiveVitRoleName);
    IOBuff.AddUInt(m_nVitNum);
    IOBuff.AddUShort((unsigned short)m_listFriendRoleID.size());
    std::list<unsigned int>::iterator it = m_listFriendRoleID.begin();
    for (; it != m_listFriendRoleID.end(); ++it)
    {
        IOBuff.AddUInt(*it);
    }
    return true;
}

bool GameMsg_S2G_GiveVitToFriend::doDecode(CParamPool &IOBuff)
{
    m_nGiveVitRoleID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strGiveVitRoleName);
    m_nVitNum = IOBuff.GetUInt();
    unsigned short nItemCount = IOBuff.GetUShort();
    for (unsigned short i = 0; i < nItemCount; ++i)
    {
        m_listFriendRoleID.push_back(IOBuff.GetUInt());
    }
    return true;
}

