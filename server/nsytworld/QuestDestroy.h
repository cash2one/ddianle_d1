#ifndef __QUEST_DESTROY_H__
#define __QUEST_DESTROY_H__

#include "QuestTrigger.h"
#include "../datastructure/Macro_Define.h"
#include <vector>
#include "QuestDoc.h"

class QuestDestroyLevel : public IQuestDestroy
{
public:
    QuestDestroyLevel();
    virtual ~QuestDestroyLevel();

public:
    bool Load(CFileStream* pFs);
    virtual EQuest_Destroy_Type GetDestroyType();
    virtual bool CheckCondition(int nValue,int nParam  = 0 );

private:
    unsigned short m_Level;
};

class QuestDestroyDivorce :public IQuestDestroy
{
public:
    QuestDestroyDivorce();
    virtual ~QuestDestroyDivorce();

public:
    bool Load(CFileStream* pFs);
    virtual EQuest_Destroy_Type GetDestroyType();

private:
    bool m_bEnable;
};

class QuestDestroyLeaveDanceGroup :public IQuestDestroy
{
public:
    QuestDestroyLeaveDanceGroup();
    virtual ~QuestDestroyLeaveDanceGroup();

public:
    bool Load(CFileStream* pFs);
    virtual EQuest_Destroy_Type GetDestroyType();

private:
    bool m_bEnable;
};

#endif

