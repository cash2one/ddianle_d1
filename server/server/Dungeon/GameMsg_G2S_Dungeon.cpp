#include "GameMsg_G2S_Dungeon.h"
#include "../share/ServerMsgDef.h"
#include "../../socket/ParamPool.h"

//////////////////////////////////////////////////////////////////////////////
GameMsg_G2S_GetRankListResult::GameMsg_G2S_GetRankListResult()
:GameMsg_Base( MSG_G2S_GetRankListResult )
,m_nCurrentPage(0)
,m_nSelfRank(0)
{

}

bool GameMsg_G2S_GetRankListResult::doDecode(CParamPool &IOBuff)
{
	m_nCurrentPage = IOBuff.GetUInt();
	unsigned short nCount = IOBuff.GetUShort();
	for ( unsigned short i = 0;  i < nCount; ++i )
	{
		DungeonRankInfo info;
		info.nRoleID = IOBuff.GetUInt();
		IOBuff.GetStringW(info.strRoleName);
		info.nParcloseID = IOBuff.GetUInt();
		info.nStarCount = IOBuff.GetUInt();

		m_RankList.push_back( info );
	}
	m_nSelfRank = IOBuff.GetInt();

	return true;
}

bool GameMsg_G2S_GetRankListResult::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt( m_nCurrentPage );
	IOBuff.AddUShort( (unsigned short)m_RankList.size() );
	std::list<DungeonRankInfo>::iterator it = m_RankList.begin();
	for ( ; it != m_RankList.end(); ++it )
	{
		IOBuff.AddUInt( it->nRoleID );
		IOBuff.AddStringW( it->strRoleName );
		IOBuff.AddUInt( it->nParcloseID );
		IOBuff.AddUInt( it->nStarCount );
	}
	IOBuff.AddInt( m_nSelfRank );

	return true;
}

//////////////////////////////////////////////////////////////////////////////
GameMsg_G2S_SyncDungeonInviteFriendInfo::GameMsg_G2S_SyncDungeonInviteFriendInfo()
    :GameMsg_Base( MSG_G2S_SyncInviteFriendInfo )
{

}

bool GameMsg_G2S_SyncDungeonInviteFriendInfo::doDecode(CParamPool &IOBuff)
{
    m_DungeonInviteInfo.doDecode( IOBuff );
    return true;
}

bool GameMsg_G2S_SyncDungeonInviteFriendInfo::doEncode(CParamPool &IOBuff)
{
    m_DungeonInviteInfo.doEncode( IOBuff );
    return true;
}

//////////////////////////////////////////////////////////////////////////////
GameMsg_G2S_GiveVitToFriend::GameMsg_G2S_GiveVitToFriend()
    :GameMsg_Base( MSG_G2S_GiveVitToFriend )
    ,m_nFriendRoleID(0)
    ,m_strFriendRoleName("")
{

}

bool GameMsg_G2S_GiveVitToFriend::doDecode(CParamPool &IOBuff)
{
    m_nFriendRoleID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strFriendRoleName);
    return true;
}

bool GameMsg_G2S_GiveVitToFriend::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nFriendRoleID);
    IOBuff.AddStringW(m_strFriendRoleName);
    return true;
}


