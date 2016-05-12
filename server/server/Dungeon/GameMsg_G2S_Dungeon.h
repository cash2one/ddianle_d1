#ifndef	__GAMEMSG_G2S_DUNGEON_H__
#define	__GAMEMSG_G2S_DUNGEON_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"
#include <list>
class CParamPool;

class GameMsg_G2S_GetRankListResult : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE( GameMsg_G2S_GetRankListResult );
	GameMsg_G2S_GetRankListResult();
	~GameMsg_G2S_GetRankListResult(){};

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	unsigned int	m_nCurrentPage;
	std::list<DungeonRankInfo>  m_RankList;
	int             m_nSelfRank;
};

class GameMsg_G2S_SyncDungeonInviteFriendInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE( GameMsg_G2S_SyncDungeonInviteFriendInfo );
    GameMsg_G2S_SyncDungeonInviteFriendInfo();
    ~GameMsg_G2S_SyncDungeonInviteFriendInfo(){};

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    CDungeonInviteInfo  m_DungeonInviteInfo;;
};

class GameMsg_G2S_GiveVitToFriend : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE( GameMsg_G2S_GiveVitToFriend );
    GameMsg_G2S_GiveVitToFriend();
    ~GameMsg_G2S_GiveVitToFriend(){};

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int        m_nFriendRoleID;
    std::string         m_strFriendRoleName;
};


#endif
// end of file


