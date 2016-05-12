#include "QuestDef.h"
#include "QuestTrigger.h"


#include "QuestCondition.h"
#include "QuestComplete.h"
#include "QuestDestroy.h"
#include "QuestReward.h"


IQuestCondition* IQuestCondition::Create(EQuest_Condition_Type type)
{
    IQuestCondition* pCondition = NULL;
    switch(type)
    {
    case  EQuest_Condition_LevelMin:
        pCondition = new QuestConditionLevelMin();
        break;
    case EQuest_Condition_VipLevel:
        pCondition = new QuestConditionVipLevel();
        break;
    case EQuest_Condition_Sex:
        pCondition = new QuestConditionSex();
        break;
    case EQuest_Condition_XingZuo:
        pCondition = new QuestConditionXingZuo();
        break;
    case EQuest_Condition_HasLover:
        pCondition = new QuestConditionHasLover();
        break;
    case EQuest_Condition_HasMarried:
        pCondition = new QuestConditionHasMarried();
        break;
    case EQuest_Condition_Intimacy:
        pCondition = new QuestConditionIntimacy();
        break;
    case EQuest_Condition_PreQuest:
        pCondition = new QuestConditionPreQuest();
        break;
    case EQuest_Condition_ObtainItem:
        pCondition = new QuestConditionObtainItem();
        break;
    case EQuest_Condition_UseItem:
        pCondition = new QuestConditionUseItem();
        break;
    case EQuest_Condition_HasDanceGroup:
        pCondition = new QuestConditionHasDanceGroup();
        break;
    case EQuest_Condition_DanceGroupContribution:
        pCondition = new QuestConditionDanceGroupContribution();
        break;
    case EQuest_Condition_DanceGroupLevel:
        pCondition = new QuestConditionDanceGroupLevel();
        break;
    case EQuest_Condition_Item:
        pCondition = new QuestConditionItem();
        break;
    case EQuest_Condition_ChargeVersion:
        pCondition = new QuestConditionChargeVersion();
        break;
    case EQuest_Condition_None:
        break;
    default:
        break;
    }
    return pCondition;
}

IQuestComplete* IQuestComplete::Create(EQuest_Complete_Type type)
{
    IQuestComplete* pComp = NULL;
    switch(type)
    {
    case EQuest_Complete_Level:
        pComp = new QuestCompleteLevel();
        break;
    case EQuest_Complete_FriendCount:
        pComp = new QuestCompleteFriendCount();
        break;
    case EQuest_Complete_VipLevel:
        pComp = new QuestCompleteVipLevel();
        break;
    case EQuest_Complete_HasLover:
        pComp = new QuestCompleteHasLover();
        break;
    case EQuest_Complete_HasMarried:
        pComp = new QuestCompleteHasMarried();
        break;
    case EQuest_Complete_Intimacy:
        pComp = new QuestCompleteIntimacy();
        break;
    case EQuest_Complete_HasDanceGroup:
        pComp = new QuestCompleteHasDanceGroup();
        break;
    case EQuest_Complete_DanceGroupContribution:
        pComp = new QuestCompleteDanceGroupContribution();
        break;
    case EQuest_Complete_DanceGroupTotalContribution:
        pComp = new QuestCompleteDanceGroupTotalContribution();
        break;
    case EQuest_Complete_DanceGroupTotalHonor:
        pComp = new QuestCompleteDanceGroupTotalHonor();
        break;
    case EQuest_Complete_UseCloth:
        pComp = new QuestCompleteUseCloth();
        break;
    case EQuest_Complete_UseItem:
        pComp = new QuestCompleteUseItem();
        break;
    case EQuest_Complete_ObtainItem:
        pComp = new QuestCompleteObtainItem();
        break;
    case EQuest_Complete_OpenWeb:
        pComp = new QuestCompleteOpenWeb();
        break;
    case EQuest_Complete_UseFunction:
        pComp = new QuestCompleteUseFunction();
        break;
    case EQuest_Complete_UseMusic:
        pComp = new QuestCompleteUseMusic();
        break;
    case EQuest_Complete_Item:
        pComp = new QuestCompleteItem();
        break;
    case EQuest_Complete_OpenComponent:
        pComp = new QuestCompleteOpenComponent();
        break;
    case EQuest_Complete_MatchMusicEnd:
        pComp = new QuestCompleteMatchMusicEnd();
        break;
    case EQuest_Complete_Magiclamp:
        pComp = new QuestCompleteMagiclamp();
        break;
    case EQuest_Complete_BuyItem:
        pComp = new QuestCompleteBuyItem();
        break;
    case EQuest_Complete_ClotheEffectReplace:
        pComp = new QuestCompleteClotheEffectReplace();
        break;
    case EQuest_Complete_ClotheEffectLevelUp:
        pComp = new QuestCompleteClotheEffectLevelUp();
        break;
    case EQuest_Complete_MoneyMagicArray:
        pComp = new QuestCompleteMoneyMagicArray();
        break;
    case EQuest_Complete_CoinMagicArray:
        pComp = new QuestCompleteCoinMagicArray();
        break;

    case EMusic_Complete_None:
        break;
    case EMusic_Complete_Scene:
        pComp = new MusicCompleteScene();
        break;
    case EMusic_Complete_Diff:
        pComp = new MusicCompleteDiff();
        break;
    case EMusic_Complete_SceneHuman:
        pComp = new MusicCompleteSceneHuman();
        break;
    case EMusic_Complete_SceneFriend:
        pComp = new MusicCompleteSceneFriend();
        break;
    case EMusic_Complete_HasLover:
        pComp = new MusicCompleteHasLover();
        break;
    case EMusic_Complete_DanceGroup:
        pComp = new MusicCompleteDanceGroup();
        break;
    case EMusic_Complete_Rank:
        pComp = new MusicCompleteRank();
        break;
    case EMusic_Complete_EvalLevel:
        pComp = new MusicCompleteEvalLevel();
        break;
    case EMusic_Complete_Free:
        pComp = new MusicCompleteFree();
        break;
    case EMusic_Complete_Team:
        pComp = new MusicCompleteTeam();
        break;
    case EMusic_Complete_Score:
        pComp = new MusicCompleteScore();
        break;
    case EMusic_Complete_Perfect:
        pComp = new MusicCompletePerfect();
        break;
    case EMusic_Complete_ConPerfect:
        pComp = new MusicCompleteConPerfect();
        break;
    case EMusic_Complete_Combo:
        pComp = new MusicCompleteCombo();
        break;
    case EMusic_Complete_MissMax:
        pComp = new MusicCompleteMissMax();
        break;
    case EMusic_Complete_Model:
        pComp = new MusicCompleteModel();
        break;
    case EQuest_Complete_None:
        break;
    default:
        break;
    }
    return pComp;
}

IQuestDestroy* IQuestDestroy::Create(EQuest_Destroy_Type type)
{
    IQuestDestroy* pDestroy = NULL;
    switch(type)
    {
    case EQuest_Destroy_Level:
        pDestroy = new QuestDestroyLevel();
        break;
    case EQuest_Destroy_Divorce:
        pDestroy = new QuestDestroyDivorce();
        break;
    case EQuest_Destroy_LeaveDanceGroup:
        pDestroy = new QuestDestroyLeaveDanceGroup();
        break;
    case EQuest_Destroy_None:
        break;
    default:
        break;
    }
    return pDestroy;
}

IQuestReward* IQuestReward::Create(EQuest_Reward_Type type)
{
    IQuestReward* pReward = NULL;
    switch(type)
    {
    case EQuest_Reward_Exp:
        pReward = new QuestRewardExp();
        break;
    case EQuest_Reward_Contribution:
        pReward = new QuestRewardContribution();
        break;
    case EQuest_Reward_Honor:
        pReward = new QuestRewardHonor();
        break;
    case EQuest_Reward_Intimacy:
        pReward = new QuestRewardIntimacy();
        break;
    case EQuest_Reward_Item:
        pReward = new QuestRewardItem();
        break;
    case EQuest_Reward_Money:
        pReward = new QuestRewardMoney();
        break;
    case EQuest_Reward_VipValue:
        pReward = new QuestRewardVipValue();
        break;
    case EQuest_Reward_BindMCoin:
        pReward = new QuestRewardBingMCoin();
        break;
    case EQuest_VipReward_BindMCoin:
        pReward = new QuestVipRewardBindMCoin();
        break;
    case EQuest_VipReward_Contribution:
        pReward = new QuestVipRewardContribution();
        break;
    case EQuest_VipReward_Exp:
        pReward = new QuestVipRewardExp();
        break;
    case EQuest_VipReward_Honor:
        pReward = new QuestVipRewardHonor();
        break;
    case EQuest_VipReward_Intimacy:
        pReward = new QuestVipRewardIntimacy();
        break;
    case EQuest_VipReward_Item:
        pReward = new QuestVipRewardItem();
        break;
    case EQuest_VipReward_Money:
        pReward = new QuestVipRewardMoney();
        break;
    case EQuest_VipReward_VipValue:
        pReward = new QuestVipRewardVipValue();
        break;
    case EQuest_Reward_None:
        break;
    default:
        break;
    }
    return pReward;
}

