#ifndef __QUEST_CONDITION_H__
#define __QUEST_CONDITION_H__

#include "QuestTrigger.h"
#include "../datastructure/Macro_Define.h"
#include <vector>
#include "QuestDoc.h"

//////////////////////////////////////////////////////////////////////////
///具体的触发条件
//////////////////////////////////////////////////////////////////////////

class QuestConditionLevelMin : public IQuestCondition
{
public:
    QuestConditionLevelMin();
    virtual ~QuestConditionLevelMin();

public:
    virtual EQuest_Condition_Type GetConditionType();
    virtual bool CheckCondition(int nValue,int nParam = 0);
    virtual bool Load(CFileStream* pFs);

private:
    unsigned short m_nMinLevel;
};

class QuestConditionVipLevel : public IQuestCondition
{
public:
    QuestConditionVipLevel();
    virtual ~QuestConditionVipLevel();
public:
    virtual EQuest_Condition_Type GetConditionType();
    virtual bool CheckCondition(int nValue,int nParam = 0);
    virtual bool Load(CFileStream* pFs);

private:
    unsigned short m_nVipLevel;
};

class QuestConditionSex : public IQuestCondition
{
public:
    QuestConditionSex();
    virtual ~QuestConditionSex();
public:
    virtual EQuest_Condition_Type GetConditionType();
    virtual bool CheckCondition(int nValue,int nParam = 0);
    virtual bool Load(CFileStream* pFs);

private:
    ESexType m_nSex;
};

class QuestConditionXingZuo : public IQuestCondition
{
public:
    QuestConditionXingZuo();
    virtual ~QuestConditionXingZuo();
public:
    virtual EQuest_Condition_Type GetConditionType();
    virtual bool CheckCondition(int nValue,int nParam = 0);
    virtual bool Load(CFileStream* pFs);

private:
    eConstellation m_nXingZuo;
};

class QuestConditionHasLover : public IQuestCondition
{
public:
    QuestConditionHasLover();
    virtual ~QuestConditionHasLover();

public:
    virtual EQuest_Condition_Type GetConditionType();
    virtual bool CheckCondition(int nValue,int nParam = 0);
    virtual bool Load(CFileStream* pFs);

private:
    bool m_bHasLover;
};

class QuestConditionHasMarried : public IQuestCondition
{
public:
    QuestConditionHasMarried();
    virtual ~QuestConditionHasMarried();
public:
    virtual EQuest_Condition_Type GetConditionType();
    virtual bool CheckCondition(int nValue,int nParam = 0);
    virtual bool Load(CFileStream* pFs);

private:
    bool m_bHasMarried;
};

class QuestConditionIntimacy : public IQuestCondition
{
public:
    QuestConditionIntimacy();
    virtual ~QuestConditionIntimacy();
public:
    virtual EQuest_Condition_Type GetConditionType();
    virtual bool CheckCondition(int nValue,int nParam = 0);
    virtual bool Load(CFileStream* pFs);
private:
    int m_nIntimacy;
};

class QuestConditionPreQuest : public IQuestCondition
{
public:
    QuestConditionPreQuest();
    virtual ~QuestConditionPreQuest();
public:
    virtual EQuest_Condition_Type GetConditionType();
    virtual bool CheckCondition(int nValue,int nParam = 0);
    virtual bool Load(CFileStream* pFs);
public:
    unsigned short m_nPreQuestID;
};

class QuestConditionObtainItem : public IQuestCondition
{
public:
    QuestConditionObtainItem();
    virtual ~QuestConditionObtainItem();
public:
    virtual EQuest_Condition_Type GetConditionType();
    virtual bool CheckCondition(int nValue,int nParam = 0);
    virtual bool Load(CFileStream* pFs);
    virtual void SetQuestID(unsigned short id);
public:
    std::vector<IQuestCondition* > m_SubCondition;
};

class QuestConditionUseItem : public IQuestCondition
{
public:
    QuestConditionUseItem();
    virtual ~QuestConditionUseItem();
public:
    virtual EQuest_Condition_Type GetConditionType();
    virtual bool CheckCondition(int nValue,int nParam = 0);
    virtual bool Load(CFileStream* pFs);
    virtual void SetQuestID(unsigned short id);
public:
    std::vector<IQuestCondition* > m_SubCondition;
};

class QuestConditionHasDanceGroup : public IQuestCondition
{
public:
    QuestConditionHasDanceGroup();
    virtual ~QuestConditionHasDanceGroup();
public:
    virtual EQuest_Condition_Type GetConditionType();
    virtual bool CheckCondition(int nValue,int nParam = 0);
    virtual bool Load(CFileStream* pFs);

private:
    bool m_bHasDanceGroup;
};

class QuestConditionDanceGroupContribution : public IQuestCondition
{
public:
    QuestConditionDanceGroupContribution();
    virtual ~QuestConditionDanceGroupContribution();
public:
    virtual EQuest_Condition_Type GetConditionType();
    virtual bool CheckCondition(int nValue,int nParam = 0);
    virtual bool Load(CFileStream* pFs);

private:
    int m_nCondition;
};

class QuestConditionDanceGroupLevel : public IQuestCondition
{
public:
    QuestConditionDanceGroupLevel();
    virtual ~QuestConditionDanceGroupLevel();
public:
    virtual EQuest_Condition_Type GetConditionType();
    virtual bool CheckCondition(int nValue,int nParam = 0);
    virtual bool Load(CFileStream* pFs);

private:
    unsigned short m_nLevel;
};

class QuestConditionItem : public IQuestCondition
{
public:
    QuestConditionItem();
    virtual ~QuestConditionItem();

public:
    virtual EQuest_Condition_Type GetConditionType();
    virtual bool CheckCondition(int nValue,int nParam = 0);
    virtual bool Load(CFileStream* pFs);

public:
    CQuestItem	m_Item;
};

class QuestConditionChargeVersion : public IQuestCondition
{
public:
    QuestConditionChargeVersion();
    virtual ~QuestConditionChargeVersion();

public:
    virtual EQuest_Condition_Type GetConditionType();
    virtual bool CheckCondition(int nValue,int nParam  = 0 );
    virtual bool Load(CFileStream* pFs);

public:
    bool m_Value;
};

#endif

