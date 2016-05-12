#ifndef __GAMEMSG_DUNGEON_H__
#define __GAMEMSG_DUNGEON_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"
#include <string>
#include <set>
#include "../../datastructure/DataStruct_Base.h"
#include "DungeonDefine.h"
#include "EntityDungeonComponent.h"
#include "../../datastructure/datainfor/MagicLampConfig.h"

using namespace std;
class CParamPool;
class PetComponent;
class CEntityMedalComponent;

class GameMsg_C2S_GetLanternBaseInfo : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetLanternBaseInfo);
	GameMsg_C2S_GetLanternBaseInfo();
	~GameMsg_C2S_GetLanternBaseInfo();

public:
	virtual bool doDecode(CParamPool &IOBuff);

public:
	 int				m_nOsType;
};

class GameMsg_S2C_GetLanternBaseInfoResult : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetLanternBaseInfoResult);
	GameMsg_S2C_GetLanternBaseInfoResult();
	~GameMsg_S2C_GetLanternBaseInfoResult();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	CEntityDungeonComponent *   m_pRoleDungeon;
};

class GameMsg_C2S_GetDungeonStory : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetDungeonStory);
	GameMsg_C2S_GetDungeonStory();
	~GameMsg_C2S_GetDungeonStory();

public:
	virtual bool doDecode(CParamPool &IOBuff);

public:
    bool               m_bIsMainline;
	unsigned int       m_nStoryType;
	unsigned int       m_nDungeonID;
	unsigned int       m_nParcloseID;
};

class GameMsg_S2C_GetDungeonStoryResult : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetDungeonStoryResult);
	GameMsg_S2C_GetDungeonStoryResult();
	~GameMsg_S2C_GetDungeonStoryResult();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	unsigned int       m_nStoryType;
	unsigned int       m_nDungeonID;
	unsigned int       m_nParcloseID;
	std::vector<unsigned int> m_listNpc;
	std::vector<CStoryContent> m_listStory;
};

class GameMsg_S2C_GetDungeonStoryFail : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetDungeonStoryFail);
	GameMsg_S2C_GetDungeonStoryFail();
	~GameMsg_S2C_GetDungeonStoryFail();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	unsigned int       m_nStoryType;
	unsigned int       m_nDungeonID;
	unsigned int       m_nParcloseID;
	std::string        m_strError;
};

class GameMsg_C2S_GetDungeonInfoDetail : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetDungeonInfoDetail);
	GameMsg_C2S_GetDungeonInfoDetail();
	~GameMsg_C2S_GetDungeonInfoDetail();

public:
	virtual bool doDecode(CParamPool &IOBuff);

public:
    bool               m_bIsMainline;
	unsigned int       m_nDungeonID;
};

class GameMsg_S2C_GetDungeonInfoDetailResult : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetDungeonInfoDetailResult);
	GameMsg_S2C_GetDungeonInfoDetailResult();
	~GameMsg_S2C_GetDungeonInfoDetailResult();

public:
	virtual bool doEncode(CParamPool &IOBuff);
    unsigned int GetThemeRunningIndex( unsigned int nThemeID );

public:
    unsigned int     m_nCurrentParcloseID;
    unsigned int     m_nServerActiveParcloseID;
    CDungeonConfig *  m_pDungeonConfig;
    CThemeConfig *  m_pThemeConfig;
    std::map<unsigned int,MainlineParcloseRecord>  m_DungeonRecord;
    std::map<unsigned int,ThemeParcloseRecord>  m_ThemeRecord;
    std::map<unsigned int,unsigned int> m_ThemeRunningIndex;
};

class GameMsg_S2C_GetDungeonInfoDetailFail : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetDungeonInfoDetailFail);
	GameMsg_S2C_GetDungeonInfoDetailFail();
	~GameMsg_S2C_GetDungeonInfoDetailFail();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	std::string     m_strError;
};


class GameMsg_C2S_GetParcloseInfoDetail : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetParcloseInfoDetail);
	GameMsg_C2S_GetParcloseInfoDetail();
	~GameMsg_C2S_GetParcloseInfoDetail();

public:
	virtual bool doDecode(CParamPool &IOBuff);

public:
    bool               m_bIsMainline;
    unsigned int       m_nDungeonID;
	unsigned int       m_nParcloseID;
};

class GameMsg_S2C_GetParcloseInfoDetailResult : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetParcloseInfoDetailResult);
	GameMsg_S2C_GetParcloseInfoDetailResult();
	~GameMsg_S2C_GetParcloseInfoDetailResult();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	CParcloseConfig * m_pParcloseConfig;
    CThemeParcloseConfig * m_pThemeParcloseConfig;
	unsigned int	  m_nLuckStar;
	itemtype_t        m_nSkipParcloseItem;
	unsigned short    m_nSkipParcloseItemCount;
	unsigned int      m_nSex;
	std::vector<CItem> m_MaleItems;
	std::vector<CItem> m_FemaleItems;
    std::vector<DebrisReward> m_DropDebrisInfo;
};

class GameMsg_S2C_GetParcloseInfoDetailFail : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetParcloseInfoDetailFail);
	GameMsg_S2C_GetParcloseInfoDetailFail();
	~GameMsg_S2C_GetParcloseInfoDetailFail();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	std::string      m_strError;
};

class GameMsg_C2S_GetStarReward : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetStarReward);
	GameMsg_C2S_GetStarReward();
	~GameMsg_C2S_GetStarReward();

public:
	virtual bool doDecode(CParamPool &IOBuff);

public:
	unsigned int       m_nRewardID;
};

class GameMsg_S2C_GetStarRewardSuc : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetStarRewardSuc);
	GameMsg_S2C_GetStarRewardSuc();
	~GameMsg_S2C_GetStarRewardSuc();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	unsigned int       m_nRewardID;
	std::list<CItem>   m_listReward;
    unsigned int       m_nMoney;
};

class GameMsg_S2C_GetStarRewardFail : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetStarRewardFail);
	GameMsg_S2C_GetStarRewardFail();
	~GameMsg_S2C_GetStarRewardFail();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	std::string			m_strError;

};

class GameMsg_C2S_ExchangeDustReward : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ExchangeDustReward);
	GameMsg_C2S_ExchangeDustReward();
	~GameMsg_C2S_ExchangeDustReward();

public:
	virtual bool doDecode(CParamPool &IOBuff);

};

class GameMsg_S2C_ExchangeDustRewardSuc : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_ExchangeDustRewardSuc);
	GameMsg_S2C_ExchangeDustRewardSuc();
	~GameMsg_S2C_ExchangeDustRewardSuc();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	unsigned int     m_nDustCount;
	std::list<CItem> m_listReward;
	unsigned int     m_nMoney;

};

class GameMsg_S2C_ExchangeDustRewardFail : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_ExchangeDustRewardFail);
    GameMsg_S2C_ExchangeDustRewardFail();
    ~GameMsg_S2C_ExchangeDustRewardFail();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    std::string			m_strError;
};

// class GameMsg_C2S_InviteFriend : public GameMsg_Base
// {
// public:
// 	GAMEMSG_CREATEDECLARE(GameMsg_C2S_InviteFriend);
// 	GameMsg_C2S_InviteFriend();
// 	~GameMsg_C2S_InviteFriend();
// 
// public:
// 	virtual bool doDecode(CParamPool &IOBuff);
// 
// public:
// 	unsigned int     m_nFriendID;
// 	unsigned int     m_nDungeonID;
// 	unsigned int     m_nParcloseID;
// };
// 
// class GameMsg_S2C_InviteFriendSuc : public GameMsg_Base
// {
// public:
// 	GAMEMSG_CREATEDECLARE(GameMsg_S2C_InviteFriendSuc);
// 	GameMsg_S2C_InviteFriendSuc();
// 	~GameMsg_S2C_InviteFriendSuc();
// 
// public:
// 	virtual bool doEncode(CParamPool &IOBuff);
// 
// public:
// 	unsigned int m_nDungeonID;
// 	unsigned int m_nParcloseID;
// 	unsigned int m_nInviterID;
// 	std::string  m_strInviterName;
// 	bool         m_nIsVip;
// 	unsigned int m_nVipLv;
// };
// 
// class GameMsg_S2C_InviteFriendFail : public GameMsg_Base
// {
// public:
// 	GAMEMSG_CREATEDECLARE(GameMsg_S2C_InviteFriendFail);
// 	GameMsg_S2C_InviteFriendFail();
// 	~GameMsg_S2C_InviteFriendFail();
// 
// public:
// 	virtual bool doEncode(CParamPool &IOBuff);
// 
// public:
// 	std::string   m_strError;
// };

class GameMsg_S2C_BeInviteToParclose : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_BeInviteToParclose);
	GameMsg_S2C_BeInviteToParclose();
	~GameMsg_S2C_BeInviteToParclose();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	std::string  m_strParcloseName;
    EDungeonType m_eType;
	unsigned int m_nDungeonID;
	unsigned int m_nParcloseID;
	unsigned int m_nInviterID;
	std::string  m_strInviterName;
	bool         m_nIsVip;
	unsigned int m_nVipLv;
};

class GameMsg_C2S_InviteFriendResponse : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_InviteFriendResponse);
	GameMsg_C2S_InviteFriendResponse();
	~GameMsg_C2S_InviteFriendResponse();

public:
	virtual bool doDecode(CParamPool &IOBuff);

public:
	unsigned int     m_nInviterID;
	bool		     m_bIsAccept;
    bool             m_bIsMainline;
	unsigned int     m_nDungeonID;
	unsigned int     m_nParcloseID;
	unsigned int     m_nPhoneOS;
};

class GameMsg_S2C_InviteFriendResponseResult : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_InviteFriendResponseResult);
	GameMsg_S2C_InviteFriendResponseResult();
	~GameMsg_S2C_InviteFriendResponseResult();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	std::string     m_strError;
};

class GameMsg_C2S_GetDungeonList : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetDungeonList);
	GameMsg_C2S_GetDungeonList();
	~GameMsg_C2S_GetDungeonList();

public:
	virtual bool doDecode(CParamPool &IOBuff);

public:
	unsigned int   m_nPage; // 页码

};

class GameMsg_S2C_GetDungeonListResult : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetDungeonListResult);
	GameMsg_S2C_GetDungeonListResult();
	~GameMsg_S2C_GetDungeonListResult();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public :
	unsigned int     m_nCurPage;
	unsigned int     m_nTotalCount;
	std::vector<CDungeonRank> m_RankList;
	unsigned short     m_nSelfRank;
	unsigned int     m_nTopDungeonID;
	unsigned int     m_nTopParcloseID;
    std::string      m_strTopDungeonName;
    std::string      m_strTopParcloseName;
};

class GameMsg_C2S_GetBuyCrystalInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetBuyCrystalInfo);
    GameMsg_C2S_GetBuyCrystalInfo();
    ~GameMsg_C2S_GetBuyCrystalInfo();

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
};

class GameMsg_S2C_GetBuyCrystalInfoResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetBuyCrystalInfoResult);
    GameMsg_S2C_GetBuyCrystalInfoResult();
    ~GameMsg_S2C_GetBuyCrystalInfoResult();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    std::map<int,CBuyCrystalConfig>         m_mapBuyCrystalConfig;
    unsigned int                            m_nFavorableBuyVitCount;
    unsigned int                            m_nHaveBuyVitCount;
};

class GameMsg_C2S_BuyCrystal : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_BuyCrystal);
	GameMsg_C2S_BuyCrystal();
	~GameMsg_C2S_BuyCrystal();

public:
	virtual bool doDecode(CParamPool &IOBuff);

public:
	unsigned int   m_nIndex; 
};

class GameMsg_S2C_BuyCrystalSuc : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_BuyCrystalSuc);
	GameMsg_S2C_BuyCrystalSuc();
	~GameMsg_S2C_BuyCrystalSuc();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	unsigned int    m_nCrystalCount; 
    unsigned int    m_nTodayVipBuyVitNum;
};

class GameMsg_S2C_BuyCrystalFail : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_BuyCrystalFail);
	GameMsg_S2C_BuyCrystalFail();
	~GameMsg_S2C_BuyCrystalFail();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	std::string   m_strError; 
};

class GameMsg_S2C_UpdateCrystal : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_UpdateCrystal);
	GameMsg_S2C_UpdateCrystal();
	~GameMsg_S2C_UpdateCrystal();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	unsigned int   m_nCrystal;
	float          m_nTime;
};

class GameMsg_C2S_StartParclose : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_StartParclose);
	GameMsg_C2S_StartParclose();
	~GameMsg_C2S_StartParclose();

public:
	virtual bool doDecode(CParamPool &IOBuff);

public:
    bool                        m_bIsMainline;
    unsigned int                m_nDungeonID;
	unsigned int				m_nParcloseID;
	std::vector<itemtype_t>     m_listItem;
	unsigned int                m_nFriendID;
};

class GameMsg_C2S_SkipParcose : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_SkipParcose);
	GameMsg_C2S_SkipParcose();
	~GameMsg_C2S_SkipParcose();

public:
	virtual bool doDecode(CParamPool &IOBuff);

public:
    bool                  m_bIsMainline;
    unsigned int          m_nDungeonID;
	unsigned int          m_nParcloseID;
};

class GameMsg_S2C_SkipParcloseSuc : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_SkipParcloseSuc);
	GameMsg_S2C_SkipParcloseSuc();
	~GameMsg_S2C_SkipParcloseSuc();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	unsigned int	  m_nCurrentParcloseStar;
	unsigned short	  m_nAllStartCount;
	unsigned int      m_nNextDungeonID;
    unsigned int      m_nNextParcloseID;
};

class GameMsg_S2C_SkipParcloseFail : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_SkipParcloseFail);
	GameMsg_S2C_SkipParcloseFail();
	~GameMsg_S2C_SkipParcloseFail();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	std::string     m_strError;
};


class GameMsg_S2C_StartParcloseFail : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_StartParcloseFail);
	GameMsg_S2C_StartParcloseFail();
	~GameMsg_S2C_StartParcloseFail();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	unsigned int  m_nErrFlag;
	std::string   m_strError;
};

class DungeonPlayerInfo
{
public:
    DungeonPlayerInfo();
    ~DungeonPlayerInfo();

    void doEncode(CParamPool& IOBuff);

public:
    unsigned int m_nRoleID;
    std::string  m_strRoleName;
    bool m_bIsHost;
    char m_chRoleType;
    bool m_nIsBoss;
    char m_chRolePos;
    char m_chRoleState;
    EDanceColorTeam m_eColorTeam;
    char m_nRoleSex;
    char m_nRoleSkin;
    char m_nGroupTitle;
    std::string m_strDanceGroupName;
    unsigned short m_nDanceGroupBadge;
    unsigned short m_nDanceGroupEffect;
    bool m_bIsVIP;
    unsigned short m_nVIPLevel;
    unsigned char  m_nMoveType;
    unsigned short m_nTransFormID;
    bool m_bEncodeMedal;

    // 其他信息
    CEntityItemComponent* m_pItemCOM;
    CEntityGeneComponent* m_pGeneCOM;
    PetComponent* m_pPetCOM;
    CEntityMedalComponent* m_pMedalCOM;
    std::map<EItemClothType, itemtype_t>  m_Equips;

};

typedef std::vector<DungeonPlayerInfo> DungeonPlayerInfoList;
 
class GameMsg_S2C_StartParcloseSuc : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_StartParcloseSuc);
	GameMsg_S2C_StartParcloseSuc();
	~GameMsg_S2C_StartParcloseSuc();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
    char						m_cRoomType;         // 默认
	unsigned int				m_nSceneID;
	unsigned int				m_nMusicID;
	unsigned int				m_nMode;
	unsigned int				m_nLevel;
	char						m_cSelfRoomType;
	bool						m_nIsHost;
    int                         m_cTeamMode;
	DungeonPlayerInfoList		m_PlayerList;
	std::string                 m_strCheckKey;
	std::vector<char>           m_vecStage;
	unsigned int				m_nCrystalNum;
	unsigned int                m_nDustNum;
};

class CParcloseEndTarget
{
public:
    CParcloseEndTarget();
    virtual ~CParcloseEndTarget(){};

public:
	bool        m_IsAchieve;
	std::string m_strTargetDes;
	std::string m_strPlayerTarget;
	std::string m_strNeedTarget;
};
typedef std::vector<CParcloseEndTarget> ParcloseEndTargetList;




class GameMsg_S2C_ParcloseEnd : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_ParcloseEnd);
	GameMsg_S2C_ParcloseEnd();
	~GameMsg_S2C_ParcloseEnd();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	bool				  m_bSucess;
	int                   m_nStarCount;
	int                   m_nNextDungeonID;
	int                   m_nLastParcloseID;
	std::vector<CRoomEndPlayerScoreInfo>      m_listPlayerScore;
	std::vector<int>      m_listNpcScore;
	ParcloseEndTargetList m_Targets;
	unsigned int		  m_nStarAllNum;
	bool                  m_bHaveStory;
	std::string           m_strOtherName;
    ParcloseReward        m_ParcloseReward;
};


class GameMsg_C2S_ExchangeDebrisEquip : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_ExchangeDebrisEquip);
    GameMsg_C2S_ExchangeDebrisEquip();
    ~GameMsg_C2S_ExchangeDebrisEquip();

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned int    m_nDebrisID;
};

class GameMsg_S2C_ExchangeDebrisEquipSuc : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_ExchangeDebrisEquipSuc);
    GameMsg_S2C_ExchangeDebrisEquipSuc();
    ~GameMsg_S2C_ExchangeDebrisEquipSuc();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned int    m_nDebrisID;
    unsigned int    m_nRemainCount;
    std::list<CItem> m_listExchangeEquip;
};

class GameMsg_S2C_ExchangeDebrisEquipFail : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_ExchangeDebrisEquipFail);
    GameMsg_S2C_ExchangeDebrisEquipFail();
    ~GameMsg_S2C_ExchangeDebrisEquipFail();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    char   m_cErrorFlag;
};



struct MsgDungeonInfo
{
    unsigned int nDungeonID;
    std::string strName;
    std::string strIcon;
    std::string strAtlas;
    char cState;
    float fRemindTime;
    bool bIsActived;
    bool bHasStory;

    void doEncode(CParamPool &IOBuff)
    {
        IOBuff.AddChar( (char)nDungeonID);
        IOBuff.AddStringW( strName);
        IOBuff.AddStringW(strIcon);
        IOBuff.AddStringW(strAtlas);
        IOBuff.AddChar(cState);
        IOBuff.AddFloat(fRemindTime);
        IOBuff.AddBool(bIsActived);
        IOBuff.AddBool(bHasStory);
    }

};

class GameMsg_C2S_GetMainlineInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetMainlineInfo);
    GameMsg_C2S_GetMainlineInfo();
    ~GameMsg_C2S_GetMainlineInfo();

public:
    virtual bool doDecode(CParamPool &IOBuff);

};


class GameMsg_S2C_GetMainlineInfoResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetMainlineInfoResult);
    GameMsg_S2C_GetMainlineInfoResult();
    ~GameMsg_S2C_GetMainlineInfoResult();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    std::list<MsgDungeonInfo>    m_listDungeonInfo;

};

class GameMsg_C2S_GetThemeInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetThemeInfo);
    GameMsg_C2S_GetThemeInfo();
    ~GameMsg_C2S_GetThemeInfo();

public:
    virtual bool doDecode(CParamPool &IOBuff);
};


class GameMsg_S2C_GetThemeInfoResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetThemeInfoResult);
    GameMsg_S2C_GetThemeInfoResult();
    ~GameMsg_S2C_GetThemeInfoResult();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    std::list<MsgDungeonInfo>    m_listDungeonInfo;

};

class GameMsg_S2C_UpdateInviteFriend : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_UpdateInviteFriend);
    GameMsg_S2C_UpdateInviteFriend();
    ~GameMsg_S2C_UpdateInviteFriend();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned int m_nRemainInviteFriendCount;
    std::set<unsigned int>  m_setInvitedFriends;

};

class GameMsg_C2S_GetFriendContactInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetFriendContactInfo);
    GameMsg_C2S_GetFriendContactInfo();
    ~GameMsg_C2S_GetFriendContactInfo();

public:
    virtual bool doDecode(CParamPool &IOBuff);
};

class GameMsg_S2C_GetFriendContactInfoResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetFriendContactInfoResult);
    GameMsg_S2C_GetFriendContactInfoResult();
    ~GameMsg_S2C_GetFriendContactInfoResult();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    std::map<unsigned int,FriendContactInfo>    m_mapFriendContact;
};

class GameMsg_C2S_GiveVitToFriend : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GiveVitToFriend);
    GameMsg_C2S_GiveVitToFriend();
    ~GameMsg_C2S_GiveVitToFriend();

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    std::list<unsigned int>     m_listFriendRoleID;
};

class GameMsg_S2C_GiveVitToFriendResult: public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GiveVitToFriendResult);
    GameMsg_S2C_GiveVitToFriendResult();
    ~GameMsg_S2C_GiveVitToFriendResult();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    std::list<unsigned int>     m_listSucessFriendRoleID;
    std::list<unsigned int>     m_listFailFriendRoleID;
};

class GameMsg_S2C_FriendNoticeByGiveVit : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_FriendNoticeByGiveVit);
    GameMsg_S2C_FriendNoticeByGiveVit();
    ~GameMsg_S2C_FriendNoticeByGiveVit();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGiveVitRoleID;
    std::string         m_strGiveVitRoleName;
    unsigned int        m_nGiveVitNum;
};

class GameMsg_C2S_GetVitFromFriend : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetVitFromFriend);
    GameMsg_C2S_GetVitFromFriend();
    ~GameMsg_C2S_GetVitFromFriend();

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    std::list<unsigned int>     m_listFriendRoleID;
};

class GameMsg_S2C_GetVitFromFriendResult: public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetVitFromFriendResult);
    GameMsg_S2C_GetVitFromFriendResult();
    ~GameMsg_S2C_GetVitFromFriendResult();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    std::list<unsigned int>     m_listScuessFriendRoleID;
    std::map<unsigned int,unsigned short>     m_mapFailFriend;
};

class GameMsg_C2S_QuickFinishParclose : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_QuickFinishParclose);
    GameMsg_C2S_QuickFinishParclose();
    ~GameMsg_C2S_QuickFinishParclose();

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    bool                m_bIsMainline;
    unsigned int        m_nDungeonID;
    unsigned int        m_nParcloseID;
};

class GameMsg_S2C_QuickFinishParcloseSucess : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_QuickFinishParcloseSucess);
    GameMsg_S2C_QuickFinishParcloseSucess();
    ~GameMsg_S2C_QuickFinishParcloseSucess();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    ParcloseReward          m_ParcloseReward;
};

class GameMsg_S2C_QuickFinishParcloseFail: public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_QuickFinishParcloseFail);
    GameMsg_S2C_QuickFinishParcloseFail();
    ~GameMsg_S2C_QuickFinishParcloseFail();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned int            m_nError;
};

#endif


//end file


