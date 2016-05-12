#ifndef	__GAMEMSG_S2G_DUNGEON_H__
#define	__GAMEMSG_S2G_DUNGEON_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"

class CParamPool;
class GameMsg_S2G_GetRankList : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_GetRankList);
	GameMsg_S2G_GetRankList();
	virtual ~GameMsg_S2G_GetRankList();

public:
	bool doDecode(CParamPool &IOBuff);
	bool doEncode(CParamPool &IOBuff);

public:
	unsigned int            m_nRoleID;
	unsigned int            m_nCurrentPage;
};

class GameMsg_S2G_UpdateParcloseInfo : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE( GameMsg_S2G_UpdateParcloseInfo );
	GameMsg_S2G_UpdateParcloseInfo();
	virtual ~GameMsg_S2G_UpdateParcloseInfo(){};

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	unsigned int	m_nRoleID;
	unsigned int    m_nParcloseID;
	unsigned int    m_nStarCount;
};

class GameMsg_S2G_ClearDungeonInviteFriendInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE( GameMsg_S2G_ClearDungeonInviteFriendInfo );
    GameMsg_S2G_ClearDungeonInviteFriendInfo();
    virtual ~GameMsg_S2G_ClearDungeonInviteFriendInfo(){};

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int	m_nRoleID;
};

class GameMsg_S2G_AddDungeonInviteFriendInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE( GameMsg_S2G_AddDungeonInviteFriendInfo );
    GameMsg_S2G_AddDungeonInviteFriendInfo();
    virtual ~GameMsg_S2G_AddDungeonInviteFriendInfo(){};

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int    m_nRoleID;
    unsigned int    m_nFriendRoleID;
    unsigned int    m_nInviteTime;
};

class GameMsg_S2G_RemoveDungeonInviteInfo : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE( GameMsg_S2G_RemoveDungeonInviteInfo );
	GameMsg_S2G_RemoveDungeonInviteInfo();
	virtual ~GameMsg_S2G_RemoveDungeonInviteInfo(){};

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	unsigned int    m_nRoleID;
	unsigned int	m_nDeleteRoleID;
};

class GameMsg_S2G_GiveVitToFriend : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE( GameMsg_S2G_GiveVitToFriend );
    GameMsg_S2G_GiveVitToFriend();
    virtual ~GameMsg_S2G_GiveVitToFriend(){};

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int                    m_nGiveVitRoleID;
    std::string                     m_strGiveVitRoleName;
    unsigned int                    m_nVitNum;
    std::list<unsigned int>         m_listFriendRoleID;
};


#endif
// end of file


