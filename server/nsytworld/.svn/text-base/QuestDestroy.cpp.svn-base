#include "QuestTrigger.h"
#include "QuestDestroy.h"

QuestDestroyLevel::QuestDestroyLevel()
{
    m_Level = 0;
}
QuestDestroyLevel::~QuestDestroyLevel()
{
    m_Level = 0;
}
bool QuestDestroyLevel::Load(CFileStream* pFs)
{
    pFs->ReadUShort(m_Level);
    return true;
}

EQuest_Destroy_Type QuestDestroyLevel::GetDestroyType()
{
    return EQuest_Destroy_Level;
}

QuestDestroyDivorce::QuestDestroyDivorce()
{
    m_bEnable = false;
}
QuestDestroyDivorce::~QuestDestroyDivorce()
{
    m_bEnable = false;
}
bool QuestDestroyDivorce::Load(CFileStream* pFs)
{
    pFs->ReadBool(m_bEnable);
    return true;
}

EQuest_Destroy_Type QuestDestroyDivorce::GetDestroyType()
{
    return EQuest_Destroy_Divorce;
}

QuestDestroyLeaveDanceGroup::QuestDestroyLeaveDanceGroup()
{
    m_bEnable = false;
}
QuestDestroyLeaveDanceGroup::~QuestDestroyLeaveDanceGroup()
{
    m_bEnable = false;
}

bool QuestDestroyLeaveDanceGroup::Load(CFileStream* pFs)
{
    pFs->ReadBool(m_bEnable);
    return true;
}

EQuest_Destroy_Type QuestDestroyLeaveDanceGroup::GetDestroyType()
{
    return EQuest_Destroy_LeaveDanceGroup;
}

bool QuestDestroyLevel::CheckCondition(int nValue,int nParam /* = 0 */ )
{
    return nValue >= m_Level;
}
