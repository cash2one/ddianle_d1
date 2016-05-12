#ifndef __QUEST_COMPLETE_H__
#define __QUEST_COMPLETE_H__

#include "QuestTrigger.h"
#include "../datastructure/Macro_Define.h"
#include <vector>
#include <string>
#include <map>
#include "QuestDoc.h"

//////////////////////////////////////////////////////////////////////////
///具体的完成条件
//////////////////////////////////////////////////////////////////////////

class QuestCompleteLevel : public IQuestComplete
{
public:
    QuestCompleteLevel();
    virtual ~QuestCompleteLevel();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EQuest_Complete_Level;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0);

private:
    unsigned short m_Level;
};

class QuestCompleteFriendCount : public IQuestComplete
{
public:
    QuestCompleteFriendCount();
    virtual ~QuestCompleteFriendCount();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EQuest_Complete_FriendCount;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0);

private:
    unsigned short m_FriendCount;
};

class QuestCompleteVipLevel : public IQuestComplete
{
public:
    QuestCompleteVipLevel();
    virtual ~QuestCompleteVipLevel();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EQuest_Complete_VipLevel;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0);

private:
    unsigned short m_VipLevel;
};

class QuestCompleteHasLover : public IQuestComplete
{
public:
    QuestCompleteHasLover();
    virtual ~QuestCompleteHasLover();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EQuest_Complete_HasLover;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0);

private:
    bool m_HasLover;
};

class QuestCompleteHasMarried : public IQuestComplete
{
public:
    QuestCompleteHasMarried();
    virtual ~QuestCompleteHasMarried();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EQuest_Complete_HasMarried;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0);

private:
    bool m_HasMarried;
};

class QuestCompleteIntimacy : public IQuestComplete
{
public:
    QuestCompleteIntimacy();
    virtual ~QuestCompleteIntimacy();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EQuest_Complete_Intimacy;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0);

private:
    int m_nIntimacy;
};

class QuestCompleteHasDanceGroup : public IQuestComplete
{
public:
    QuestCompleteHasDanceGroup();
    virtual ~QuestCompleteHasDanceGroup();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EQuest_Complete_HasDanceGroup;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0);

private:
    bool m_bHasDanceGroup;
};

class QuestCompleteDanceGroupContribution : public IQuestComplete
{
public:
    QuestCompleteDanceGroupContribution();
    virtual ~QuestCompleteDanceGroupContribution();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EQuest_Complete_DanceGroupContribution;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0);

private:
    int m_Condition;
};

class QuestCompleteDanceGroupTotalContribution : public IQuestComplete
{
public:
    QuestCompleteDanceGroupTotalContribution();
    virtual ~QuestCompleteDanceGroupTotalContribution();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EQuest_Complete_DanceGroupTotalContribution;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0);

private:
    int m_Condition;
};

class QuestCompleteDanceGroupTotalHonor : public IQuestComplete
{
public:
    QuestCompleteDanceGroupTotalHonor();
    virtual ~QuestCompleteDanceGroupTotalHonor();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EQuest_Complete_DanceGroupTotalHonor;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0);

private:
    int m_nHonor;
};

class QuestCompleteUseCloth : public IQuestComplete
{
public:
    QuestCompleteUseCloth();
    virtual ~QuestCompleteUseCloth();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EQuest_Complete_UseCloth;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0);
    virtual void SetQuestID(unsigned short id);

public:
    std::vector<IQuestComplete* > m_SubComplete;
};

class QuestCompleteUseItem : public IQuestComplete
{
public:
    QuestCompleteUseItem();
    virtual ~QuestCompleteUseItem();
public:
    virtual EQuest_Complete_Type GetCompleteType() {return EQuest_Complete_UseItem;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0);
    virtual void SetQuestID(unsigned short id);
    unsigned short GetItemCount(itemtype_t itemID);

public:
    std::vector<IQuestComplete* > m_SubComplete;
};

class QuestCompleteObtainItem : public IQuestComplete
{
public:
    QuestCompleteObtainItem();
    virtual ~QuestCompleteObtainItem();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EQuest_Complete_ObtainItem;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0);
    virtual void SetQuestID(unsigned short id);
    void GetQuestItems(std::vector<CQuestItem>& items);

public:
    std::vector<IQuestComplete* > m_SubComplete;
};

class QuestCompleteOpenWeb : public IQuestComplete
{
public:
    QuestCompleteOpenWeb();
    virtual ~QuestCompleteOpenWeb();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EQuest_Complete_OpenWeb;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0);
public:
    std::string m_strWeb;
};

class QuestCompleteUseFunction : public IQuestComplete
{
public:
    QuestCompleteUseFunction();
    virtual ~QuestCompleteUseFunction();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EQuest_Complete_UseFunction;}

    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0);

    ECompleteUseFunction GetFunction(){return m_Function;}

private:
    unsigned short m_Times;
    int m_Param;
    ECompleteUseFunction m_Function;
};

class CQuestMusicNew
{
public:
    CQuestMusicNew();
    ~CQuestMusicNew();

public:
    bool Load(CFileStream* pFs);
    void SetQuestID(unsigned short id);

public:
    unsigned short m_MusicID;
    unsigned short m_Count;
    std::map<EQuest_Complete_Type,IQuestComplete* > m_MusicQuest;

};


class QuestCompleteUseMusic : public IQuestComplete
{
public:
    QuestCompleteUseMusic();
    virtual ~QuestCompleteUseMusic();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EQuest_Complete_UseMusic;}

    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0 );

    CQuestMusicNew* GetMusicComplete(unsigned short musicID);
    virtual void SetQuestID(unsigned short id);

    unsigned short GetMusicCount(unsigned short musicID);

public:
    std::vector<CQuestMusicNew*>	m_MusicList;
};

class QuestCompleteItem : public IQuestComplete
{
public:
    QuestCompleteItem();
    virtual ~QuestCompleteItem();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EQuest_Complete_Item;}
    virtual bool Load(CFileStream* pFs);

public:
    itemtype_t m_ItemID;
    unsigned short m_Count;
    int m_ValidTime;
};

class QuestCompleteOpenComponent : public IQuestComplete
{
public:
    QuestCompleteOpenComponent();
    virtual ~QuestCompleteOpenComponent();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EQuest_Complete_OpenComponent;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0 );
    unsigned int GetComponent(){return m_ComponentID;}
private:
    unsigned int m_ComponentID;
};

// 完成魔法神灯关卡
class QuestCompleteMagiclamp : public IQuestComplete
{
public:
    QuestCompleteMagiclamp();
    virtual ~QuestCompleteMagiclamp();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EQuest_Complete_Magiclamp;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0 );
public:
    unsigned int m_nStageID;    // 魔法神灯的关卡
    unsigned short m_nStar;     // 需要的星数
    std::string m_strCharpter;  // 章节
    std::string m_strProclose;  // 关卡
};


// 商城购买指定数量物品
class QuestCompleteBuyItem : public IQuestComplete
{
public:
    QuestCompleteBuyItem();
    virtual ~QuestCompleteBuyItem();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EQuest_Complete_BuyItem;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0 ) {return false;}
    bool ValidItem(itemtype_t itemID, int validTime);

public:
    std::vector<QuestCompleteItem*> m_SubComplete;
};

class QuestCompleteClotheEffectReplace : public IQuestComplete
{
public:
    QuestCompleteClotheEffectReplace();
    virtual ~QuestCompleteClotheEffectReplace();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EQuest_Complete_ClotheEffectReplace;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0 );
public:
    unsigned int m_num;
};

class QuestCompleteClotheEffectLevelUp : public IQuestComplete
{
public:
    QuestCompleteClotheEffectLevelUp();
    virtual ~QuestCompleteClotheEffectLevelUp();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EQuest_Complete_ClotheEffectLevelUp;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0 );
public:
    unsigned int m_num;
};

class QuestCompleteMoneyMagicArray : public IQuestComplete
{
public:
    QuestCompleteMoneyMagicArray();
    virtual ~QuestCompleteMoneyMagicArray();

public:
    virtual EQuest_Complete_Type GetCompleteType() { return EQuest_Complete_MoneyMagicArray; }
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue, int nParam = 0);
public:
    unsigned int m_num;
};

class QuestCompleteCoinMagicArray : public IQuestComplete
{
public:
    QuestCompleteCoinMagicArray();
    virtual ~QuestCompleteCoinMagicArray();

public:
    virtual EQuest_Complete_Type GetCompleteType() { return EQuest_Complete_CoinMagicArray; }
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue, int nParam = 0);
public:
    unsigned int m_num;
};


class MusicCompleteScene : public IQuestComplete
{
public:
    MusicCompleteScene();
    virtual ~MusicCompleteScene();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EMusic_Complete_Scene;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0 );

private:
    unsigned short m_SceneID;
};

class MusicCompleteDiff : public IQuestComplete
{
public:
    MusicCompleteDiff();
    virtual ~MusicCompleteDiff();
public:
    virtual EQuest_Complete_Type GetCompleteType() {return EMusic_Complete_Diff;}

    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0 );

private:
    unsigned short m_Diff;
};

class MusicCompleteSceneHuman : public IQuestComplete
{
public:
    MusicCompleteSceneHuman();
    virtual ~MusicCompleteSceneHuman();
public:
    virtual EQuest_Complete_Type GetCompleteType() {return EMusic_Complete_SceneHuman;}

    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0 );

private:
    unsigned short m_Count;
};

class MusicCompleteSceneFriend : public IQuestComplete
{
public:
    MusicCompleteSceneFriend();
    virtual ~MusicCompleteSceneFriend();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EMusic_Complete_SceneFriend;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0 );

private:
    unsigned short m_Count;
};

class MusicCompleteHasLover : public IQuestComplete
{
public:
    MusicCompleteHasLover();
    virtual ~MusicCompleteHasLover();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EMusic_Complete_HasLover;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0 );

private:
    bool m_bHasLover;
};

class MusicCompleteDanceGroup : public IQuestComplete
{
public:
    MusicCompleteDanceGroup();
    virtual ~MusicCompleteDanceGroup();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EMusic_Complete_DanceGroup;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0 );

private:
    unsigned short m_Count;
};

class MusicCompleteRank : public IQuestComplete
{
public:
    MusicCompleteRank();
    virtual ~MusicCompleteRank();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EMusic_Complete_Rank;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0 );

private:
    char m_Rank;
};

class MusicCompleteEvalLevel : public IQuestComplete
{
public:
    MusicCompleteEvalLevel();
    virtual ~MusicCompleteEvalLevel();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EMusic_Complete_EvalLevel;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0 );

private:
    unsigned short m_Level;
};

class MusicCompleteFree : public IQuestComplete
{
public:
    MusicCompleteFree();
    virtual ~MusicCompleteFree();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EMusic_Complete_Free;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0 );

private:
    bool m_bFree;
};

class MusicCompleteTeam : public IQuestComplete
{
public:
    MusicCompleteTeam();
    virtual ~MusicCompleteTeam();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EMusic_Complete_Team;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0 );

private:
    bool m_bTeam;
};

class MusicCompleteScore : public IQuestComplete
{
public:
    MusicCompleteScore();
    virtual ~MusicCompleteScore();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EMusic_Complete_Score;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0 );

private:
    unsigned int m_nScore;
};

class MusicCompletePerfect : public IQuestComplete
{
public:
    MusicCompletePerfect();
    virtual ~MusicCompletePerfect();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EMusic_Complete_Perfect;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0 );

private:
    int m_Perfect;
};

class MusicCompleteConPerfect : public IQuestComplete
{
public:
    MusicCompleteConPerfect();
    virtual ~MusicCompleteConPerfect();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EMusic_Complete_ConPerfect;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0 );

private:
    unsigned short m_ConPerfect;
};

class MusicCompleteCombo : public IQuestComplete
{
public:
    MusicCompleteCombo();
    virtual ~MusicCompleteCombo();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EMusic_Complete_Combo;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0 );

private:
    unsigned short m_nCombo;
};

class MusicCompleteMissMax : public IQuestComplete
{
public:
    MusicCompleteMissMax();
    virtual ~MusicCompleteMissMax();

public:
    virtual EQuest_Complete_Type GetCompleteType() {return EMusic_Complete_MissMax;}
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0 );

private:
    unsigned short m_MissMax;
};

class MusicCompleteModel : public IQuestComplete
{
public:
    MusicCompleteModel();
    virtual ~MusicCompleteModel();

public:
    virtual EQuest_Complete_Type GetCompleteType(){ return EMusic_Complete_Model; }
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0);

private:
    unsigned short m_Model;
};

class QuestCompleteMatchMusicEnd : public IQuestComplete
{
public:
    QuestCompleteMatchMusicEnd();
    virtual ~QuestCompleteMatchMusicEnd();

public:
    virtual EQuest_Complete_Type GetCompleteType(){ return  EQuest_Complete_MatchMusicEnd; }
    virtual bool Load(CFileStream* pFs);
    virtual bool CheckCondition(int nValue,int nParam = 0);

private:
    unsigned int m_Times;
};

#endif

