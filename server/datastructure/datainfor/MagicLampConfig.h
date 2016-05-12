/********************************************************************
    created:    2015-07-13
    filename:   DataStructure\datainfor\MagicLampConfig.h
    author:     

    purpose:   MagicLamp.bytes配置中相对应的数据结构
*********************************************************************/

#ifndef __AIDATAINFO_H__
#define __AIDATAINFO_H__

class CFileStream;

#define AIDataInfoName  "/data/bytes/MagicLamp.bytes"
#include "ItemType.h"
#include "../DataStruct_Base.h"

enum EDungeonType
{
    EDungeonType_Mainline,   // 主线
    EDungeonType_Theme,      // 主题
};

enum ETargetFlag
{
    ETargetFlag_None,
    ETargetFlag_Mark,                   // 总分达到多少
    ETargetFlag_ComboCount,             // Combo个数
    ETargetFlag_Judge,                  // 判断为X以上(D=1,C=2,B=3,A=4,S=5,SS=6)
    ETargetFlag_InviteFriend,           // 邀请好友
    ETargetFlag_InviteCouple,           // 邀请情侣
};

// boss配置
class CBossConfig
{
public:
    CBossConfig();
    ~CBossConfig(){};

public:
    bool Load(CFileStream&file);
    bool Save(CFileStream&file);

public:
    int         m_nBossID;
    std::string m_strName;
    char        m_cSex;
    int         m_cSKinColor;
    std::map<EItemClothType, itemtype_t>   m_Equips;
};

// 主线章节
class CDungeonConfig
{
public:
    CDungeonConfig();
    virtual ~CDungeonConfig(){};

public:
    bool Load(CFileStream&file);
    bool Save(CFileStream&file);

public:
    unsigned int    m_nDungeonID;
    std::string		m_strDungeonName;
    std::string     m_strIconName;
    std::string     m_strIconAtlas;
    unsigned int    m_nStoryID;
    std::vector<unsigned int> m_ParcloseList;
};
typedef std::map<unsigned int,CDungeonConfig*> CMainlineConfigTable;

// boss分数数据结构
class CBossScoreRange
{
public:
    CBossScoreRange();
    virtual ~CBossScoreRange(){};

public:
    unsigned int m_nMinScore;
    unsigned int m_nMaxScore;
};

// 掉落物品信息
class CDropItemInfo
{
public:
    CDropItemInfo();
    virtual ~CDropItemInfo(){};

    bool operator == (const CDropItemInfo& rItem) const
    {
        return m_itemInfo.m_nItemType == rItem.m_itemInfo.m_nItemType;
    }

public:
    CItem m_itemInfo;
    unsigned int m_nRate;
};

// 碎片掉落信息
class CDebrisDropInfo
{
public:
    CDebrisDropInfo();
    virtual ~CDebrisDropInfo(){};

    bool operator == (const CDebrisDropInfo& rDebris) const
    {
        return m_nDebrisID == rDebris.m_nDebrisID;
    }

public:
    unsigned int m_nDebrisID;
    unsigned int m_nDebrisCount;
    unsigned int m_nDropRate;
};

// 主线掉落
class CMainlineDropConfig
{
public:
    CMainlineDropConfig();
    virtual ~CMainlineDropConfig(){};

public:
    bool Load(CFileStream&file);
    bool Save(CFileStream&file);

public:
    unsigned int m_nParcloseID;
    unsigned int m_nMustDropNum;
    std::vector<CDropItemInfo> m_MustMaleDrop;
    std::vector<CDropItemInfo> m_MustFemaleDrop;
    std::vector<CDebrisDropInfo> m_MustDebrisDrop;
    unsigned int m_nOptionalRate;
    unsigned int m_nOptionalNum;
    std::vector<CDropItemInfo> m_OptionalMaleDrop;
    std::vector<CDropItemInfo> m_OptionalFemaleDrop;
    std::vector<CDebrisDropInfo> m_OptionalDebrisDrop;
};

// 主题掉落
class CThemeDropConfig : public CMainlineDropConfig
{
public:
    CThemeDropConfig();
    virtual ~CThemeDropConfig(){};

public:
    bool Load(CFileStream&file);
    bool Save(CFileStream&file);

public:
    unsigned int m_nThemeID;
};

// 垃圾掉落
class CRubbishDropConfig
{
public:
    CRubbishDropConfig();
    virtual ~CRubbishDropConfig(){};

public:
    bool Load(CFileStream&file);
    bool Save(CFileStream&file);

public:
    unsigned int m_nRubbishDropID;
    std::vector<CDropItemInfo> m_MaleDrop;
    std::vector<CDropItemInfo> m_FemaleDrop;
};

// 碎片配置
class CDebrisConfig
{
public:
    CDebrisConfig();
    virtual ~CDebrisConfig(){};

public:
    bool Load(CFileStream&file);
    bool Save(CFileStream&file);

public:
    unsigned int m_nDebrisID;
    std::string m_strName;
    std::string m_strIconName;
    std::string m_strAtlasName;
    unsigned int m_nNeedNum;
    unsigned int m_nCurrencyType;
    unsigned int m_nCurrencyCount;
    std::vector<CItem> m_MaleExchangeItem;
    std::vector<CItem> m_FemaleExchangeItem;
};

// 主题故事
class CThemeConfig
{
public:
    CThemeConfig();
    virtual ~CThemeConfig();

public:
    bool Load(CFileStream & file);
    bool Save(CFileStream & file);

public:
    unsigned int m_nThemeID;
    std::string m_strName;
    unsigned int m_nOpenTime;
    unsigned int m_nEndTime;
    std::string m_strThemeIcon;
    std::string m_strThemeAtlas;
    std::string m_strSceneIcon;
    std::string m_strSceneAtlas;
    unsigned int m_nStoryID;
};

// 关卡配置
class CParcloseConfig
{
public:
    CParcloseConfig();
    virtual ~CParcloseConfig(){};

public:
    bool Load(CFileStream&file);
    bool Save(CFileStream&file);
    bool IsBossParclose();

public:
    unsigned int  m_nParcloseID;
    std::string   m_strParcloseName;
    unsigned int  m_nBeginStoryID;
    unsigned int  m_nEndStoryID;
    unsigned int  m_nCostCrystal;
    unsigned int  m_nSceneID;
    unsigned int  m_nMusicID;
    std::string   m_strMusicName;
    unsigned int  m_nMusicMode;
    unsigned int  m_nMusicLevel;
    float m_fPosX;
    float m_fPosY;
    unsigned int  m_nDailyQuickFinishNum;
    std::vector<unsigned int> m_Targets;
    std::vector<unsigned int> m_BossIDList;
    std::vector<CBossScoreRange> m_BossScoreList;
};

// 主题关卡
class CThemeParcloseConfig : public CParcloseConfig
{
public:
    CThemeParcloseConfig();
    virtual ~CThemeParcloseConfig();

public:
    bool Load(CFileStream & file);
    bool Save(CFileStream & file);

public:
    unsigned int  m_nThemeID;
};

// 剧情内容
class CStoryContent
{
public:
    CStoryContent();
    virtual ~CStoryContent(){};

public:
    unsigned int m_nNPCID;
    std::string m_strContent;
};

// 剧情配置
class CStoryConfig
{
public:
    CStoryConfig(){};
    virtual ~CStoryConfig(){};

public:
    bool Load(CFileStream&file);
    bool Save(CFileStream&file);

public:
    unsigned int m_nStoryID;
    std::vector<unsigned int> m_NPCList;
    std::vector<CStoryContent> m_ContentList;
};

// 目标配置
class CTargetConfig
{
public:
    CTargetConfig(){};
    virtual ~CTargetConfig(){};

public:
    bool Load(CFileStream&file);
    bool Save(CFileStream&file);

    // 获取星数
    unsigned int GetStarCount(unsigned int nRelatedValue);

public:
    unsigned int                            m_nTargetID;
    std::string                             m_strDesc;
    unsigned int                            m_nFlag;
    std::string                             m_strValue;
    std::map<unsigned short,unsigned int>   m_mapStarValue;
};

#endif

