#ifndef __QUEST_REWARD_H__
#define __QUEST_REWARD_H__

#include "QuestTrigger.h"
#include "../datastructure/Macro_Define.h"
#include <vector>
#include "QuestDoc.h"

//  ¾­Ñé½±Àø
class QuestRewardExp :public IQuestReward
{
public:
    QuestRewardExp();
    virtual ~QuestRewardExp();

public:
    bool Load(CFileStream* pFs);
    virtual EQuest_Reward_Type GetRewardType();
    unsigned int GetExp();

private:
    unsigned int m_Exp;
};

//  ½ðÇ®½±Àø
class QuestRewardMoney :public IQuestReward
{
public:
    QuestRewardMoney();
    virtual ~QuestRewardMoney();

public:
    bool Load(CFileStream* pFs);
    virtual EQuest_Reward_Type GetRewardType();
    unsigned int GetMoney();

private:
    unsigned int m_Money;
};

// ÎèÍÅÈÙÓþ½±Àø
class QuestRewardHonor :public IQuestReward
{
public:
    QuestRewardHonor();
    virtual ~QuestRewardHonor();

public:
    bool Load(CFileStream* pFs);
    virtual EQuest_Reward_Type GetRewardType();
    unsigned int GetHonor();

private:
    unsigned int m_Honor;
};

// ÎèÍÅ¹±Ï×¶È½±Àø
class QuestRewardContribution :public IQuestReward
{
public:
    QuestRewardContribution();
    virtual ~QuestRewardContribution();

public:
    bool Load(CFileStream* pFs);
    virtual EQuest_Reward_Type GetRewardType();
    unsigned int GetContribution();

private:
    unsigned int m_Contribution;
};

// vip½±Àø
class QuestRewardVipValue :public IQuestReward
{
public:
    QuestRewardVipValue();
    virtual ~QuestRewardVipValue();

public:
    bool Load(CFileStream* pFs);
    virtual EQuest_Reward_Type GetRewardType();
    unsigned int GetValue();

private:
    unsigned int m_VipValue;
};

// ÇéÂÂÇ×ÃÜ¶È½±Àø
class QuestRewardIntimacy :public IQuestReward
{
public:
    QuestRewardIntimacy();
    virtual ~QuestRewardIntimacy();

public:
    bool Load(CFileStream* pFs);
    virtual EQuest_Reward_Type GetRewardType();
    unsigned int GetIntimacy();

private:
    unsigned int m_Intimacy;
};

// ÎïÆ·½±Àø
class QuestRewardItem :public IQuestReward
{
public:
    QuestRewardItem();
    virtual ~QuestRewardItem();

public:
    bool Load(CFileStream* pFs);
    virtual EQuest_Reward_Type GetRewardType();
    bool GetRewardItem(int nSex,std::list<CItem>& items);

private:
    std::vector<CQuestItem> m_MaleItem;
    std::vector<CQuestItem> m_FemaleItem;
};

// °ó¶¨M±Ò½±Àø
class QuestRewardBingMCoin : public IQuestReward
{
public:
    QuestRewardBingMCoin();
    virtual ~QuestRewardBingMCoin();

public:
    bool Load(CFileStream* pFs);
    virtual EQuest_Reward_Type GetRewardType();
    int GetBindMCoin();

private:
    int m_BindMCoin;
};

// vip¶îÍâ¾­Ñé½±Àø
class QuestVipRewardExp : public IQuestReward
{
public:
    QuestVipRewardExp();
    virtual ~QuestVipRewardExp();

public:
    bool Load(CFileStream* pFs);
    virtual EQuest_Reward_Type GetRewardType();
    int GetExp();

private:
    int m_nExp;
};

// vip¶îÍâÎèÍÅÈÙÓþ½±Àø
class QuestVipRewardHonor : public IQuestReward
{
public:
    QuestVipRewardHonor();
    virtual ~QuestVipRewardHonor();

public:
    bool Load(CFileStream* pFs);
    virtual EQuest_Reward_Type GetRewardType();
    int GetHonor();

private:
    int m_nHonor;
};

// vip¶îÍâÇ×ÃÜ¶È½±Àø
class QuestVipRewardIntimacy : public IQuestReward
{
public:
    QuestVipRewardIntimacy();
    virtual ~QuestVipRewardIntimacy();

public:
    bool Load(CFileStream* pFs);
    virtual EQuest_Reward_Type GetRewardType();
    int GetIntimacy();

private:
    int m_nIntimacy;
};

// vip¶îÍâ½ðÈ¯½±Àø
class QuestVipRewardMoney : public IQuestReward
{
public:
    QuestVipRewardMoney();
    virtual ~QuestVipRewardMoney();

public:
    bool Load(CFileStream* pFs);
    virtual EQuest_Reward_Type GetRewardType();
    int GetMoney();

private:
    int m_nMoney;
};

// vip¶îÍâvip½±Àø
class QuestVipRewardVipValue : public IQuestReward
{
public:
    QuestVipRewardVipValue();
    virtual ~QuestVipRewardVipValue();

public:
    bool Load(CFileStream* pFs);
    virtual EQuest_Reward_Type GetRewardType();
    int GetVipValue() ;

private:
    int m_nVipValue;
};

// vip¶îÍâ¹±Ï×¶È½±Àø
class QuestVipRewardContribution : public IQuestReward
{
public:
    QuestVipRewardContribution();
    virtual ~QuestVipRewardContribution();

public:
    bool Load(CFileStream* pFs);
    virtual EQuest_Reward_Type GetRewardType();
    int GetContribution() ;

private:
    int m_nContribution;
};

// vip¶îÍâ°ó¶¨M±Ò½±Àø
class QuestVipRewardBindMCoin : public IQuestReward
{
public:
    QuestVipRewardBindMCoin();
    virtual ~QuestVipRewardBindMCoin();

public:
    bool Load(CFileStream* pFs);
    virtual EQuest_Reward_Type GetRewardType() ;
    int GetBindMCoin();

private:
    int m_nBindMCoin;
};

// vip¶îÍâÎïÆ·½±Àø
class QuestVipRewardItem : public IQuestReward
{
public:
    QuestVipRewardItem();
    virtual ~QuestVipRewardItem();

public:
    bool Load(CFileStream* pFs);
    virtual EQuest_Reward_Type GetRewardType();
    bool GetRewardItem(int nSex,std::list<CItem>& items);

private:
    std::vector<CQuestItem> m_MaleItem;
    std::vector<CQuestItem> m_FemaleItem;
};

#endif

