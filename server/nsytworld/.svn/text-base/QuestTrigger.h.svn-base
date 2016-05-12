#ifndef __QUEST_TRIGGER_H__
#define __QUEST_TRIGGER_H__

#include "QuestDef.h"
#include <vector>

#include "../datastructure/datainfor/FileStream.h"

//////////////////////////////////////////////////////////////////////////
///触发器基类，触发条件，完成条件，销毁条件都从这个类继承
//////////////////////////////////////////////////////////////////////////
class IQuestTrigger
{
public:
    IQuestTrigger(){}
    virtual ~IQuestTrigger(){}

public:
    //触发器类型
    virtual EQuest_Trigger_Type GetTriggerType(){return EQuest_Trigger_None;}

    //条件检测，nValue为当前值，nParam为检测参数，比如物品id，歌曲id
    virtual bool CheckCondition(int nValue,int nParam = 0){return false;}

    //取得子条件，通常物品条件和音乐条件会包含子条件
    virtual int  GetSubConditions(std::vector<IQuestTrigger>& cond){return 0;}

    virtual bool Load(CFileStream* pFs){return false;}

    virtual void SetQuestID(unsigned short id) { m_QuestID = id; }

    virtual unsigned short GetQuestID() { return m_QuestID ;}

private:
    unsigned short m_QuestID;
};

//////////////////////////////////////////////////////////////////////////
///触发条件
//////////////////////////////////////////////////////////////////////////
class IQuestCondition : public IQuestTrigger
{
public:
    IQuestCondition(){}
    virtual ~IQuestCondition(){}

public:
    virtual EQuest_Trigger_Type GetTriggerType() {return EQuest_Trigger_Condition;}

    virtual EQuest_Condition_Type GetConditionType() {return EQuest_Condition_None;}

    static IQuestCondition* Create(EQuest_Condition_Type type);

};

//////////////////////////////////////////////////////////////////////////
///完成条件
//////////////////////////////////////////////////////////////////////////
class IQuestComplete : public IQuestTrigger
{
public:
    IQuestComplete(){}
    virtual ~IQuestComplete(){}

public:
    virtual EQuest_Trigger_Type GetTriggerType(){return EQuest_Trigger_None;}
    virtual EQuest_Complete_Type GetCompleteType(){return EQuest_Complete_None;}

    static  IQuestComplete* Create(EQuest_Complete_Type type);
};

//////////////////////////////////////////////////////////////////////////
///销毁条件
//////////////////////////////////////////////////////////////////////////
class IQuestDestroy : public IQuestTrigger
{
public:
    IQuestDestroy(){}
    virtual ~IQuestDestroy(){}

public:
    virtual EQuest_Trigger_Type GetTriggerType(){return EQuest_Trigger_Destroy;}
    virtual EQuest_Destroy_Type GetDestroyType(){return EQuest_Destroy_None;}

    static IQuestDestroy* Create(EQuest_Destroy_Type type);
};

//////////////////////////////////////////////////////////////////////////
///任务奖励
//////////////////////////////////////////////////////////////////////////
class IQuestReward : public IQuestTrigger
{
public:
    IQuestReward(){}
    virtual ~IQuestReward(){}

public:
    virtual EQuest_Trigger_Type GetTriggerType() {return EQuest_Trigger_Reward;}
    virtual EQuest_Reward_Type GetRewardType(){return EQuest_Reward_None;}

    static IQuestReward* Create(EQuest_Reward_Type type);
};


#endif

