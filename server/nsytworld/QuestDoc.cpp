#include "QuestDoc.h"
#include <time.h>
#include "QuestReadWriteTool.h"
#include "../datastructure/DataStruct_Base.h"
#include "QuestComplete.h"
#include "QuestReward.h"
#include "../socket/Log.h"


CQusetBaseInfo::CQusetBaseInfo()
{
	m_nGroupID = 0;
	m_nQuestID = 0;
	m_strQustName = "";
	m_nType = 0;
}

CQusetBaseInfo::~CQusetBaseInfo()
{
}

void CQusetBaseInfo::ReadDoc(CFileStream* pFs)
{
	pFs->ReadUShort(m_nGroupID);
	pFs->ReadUShort(m_nQuestID);
	pFs->ReadString2(m_strQustName);
	pFs->ReadUChar(m_nType);
}

void CQusetBaseInfo::WriteDoc(CFileStream* pFs)
{
	pFs->WriteUShort(m_nGroupID);
	pFs->WriteUShort(m_nQuestID);
	pFs->WriteString2(m_strQustName);
	pFs->WriteUChar(m_nType);
}

CQuestRequirement::CQuestRequirement()
{
	m_nLevel = 0;
	m_nSex = 0;;
	m_nHotValue = 0;
	m_nTitleID = 0;
	m_nPartyLevel = 0;
	m_nGroupLevel = 0;
	m_nMarriageLevel = 0;
	m_nWeekDay = 0;
	m_nBeginTime = 0;
	m_nEndTime = 0;
	m_nPreviousQusetID = 0;
	m_nExclusiveQustID = 0;
	m_nTriggerType = 0;	
	m_nLimitedTime = 0;
}

CQuestRequirement::~CQuestRequirement()
{
}

void CQuestRequirement::ReadDoc(CFileStream* pFs)
{
	if (pFs == NULL)
	{
		return;
	}

	pFs->ReadUShort(m_nLevel);
	pFs->ReadUShort(m_nEndLevel);
	unsigned short nSex = 0;
	pFs->ReadUShort(nSex);
	m_nSex = (unsigned char)nSex;
	pFs->ReadUInt(m_nHotValue);
	pFs->ReadUChar(m_nTitleID);
	pFs->ReadUChar(m_nPartyLevel);
	pFs->ReadUChar(m_nGroupLevel);
	pFs->ReadUChar(m_nMarriageLevel);
	pFs->ReadUChar(m_nWeekDay);
	pFs->ReadUInt(m_nBeginTime);
	pFs->ReadUInt(m_nEndTime);
	pFs->ReadUShort(m_nPreviousQusetID);
	pFs->ReadUShort(m_nExclusiveQustID);
	pFs->ReadUInt(m_nLimitedTime);
	pFs->ReadUChar(m_nTriggerType);	
	CQusetReadWriteTool::ReadItemToMap(m_mapRequredItem, pFs);
}
void CQuestRequirement::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nLevel);
	IOBuff.AddUShort(m_nEndLevel);
	IOBuff.AddUChar(m_nSex);
	IOBuff.AddUInt(m_nHotValue);
	IOBuff.AddUChar(m_nTitleID);
	IOBuff.AddUChar(m_nPartyLevel);
	IOBuff.AddUChar(m_nGroupLevel);
	IOBuff.AddUChar(m_nMarriageLevel);
	IOBuff.AddUChar(m_nWeekDay);
	IOBuff.AddUInt(m_nBeginTime);
	IOBuff.AddUInt(m_nEndTime);
	IOBuff.AddUShort(m_nPreviousQusetID);
	IOBuff.AddUShort(m_nExclusiveQustID);
	IOBuff.AddUChar(m_nTriggerType);
	IOBuff.AddUChar((unsigned char)m_mapRequredItem.size());
	if (m_mapRequredItem.size() > 0)
	{
		map<itemtype_t, CQuestItem *>::iterator it;
		it = m_mapRequredItem.begin();
		for (; it != m_mapRequredItem.end(); it++)
		{
			if (it->second != NULL)
			{
				it->second->doEncode(IOBuff);
			}
		}
	}
	IOBuff.AddUInt(m_nLimitedTime);
}
void CQuestRequirement::WriteDoc(CFileStream* pFs)
{
	if (pFs == NULL)
	{
		return;
	}

	pFs->WriteUShort(m_nLevel);
	unsigned short nSex = m_nSex;
	pFs->WriteUShort(nSex);
	m_nSex = (unsigned char)nSex;
	pFs->WriteUInt(m_nHotValue);
	pFs->WriteUChar(m_nTitleID);
	pFs->WriteUChar(m_nPartyLevel);
	pFs->WriteUChar(m_nGroupLevel);
	pFs->WriteUChar(m_nMarriageLevel);
	pFs->WriteUChar(m_nWeekDay);
	pFs->WriteUInt(m_nBeginTime);
	pFs->WriteUInt(m_nEndTime);
	pFs->WriteUShort(m_nPreviousQusetID);
	pFs->WriteUShort(m_nExclusiveQustID);
	pFs->WriteUInt(m_nLimitedTime);
	pFs->WriteUChar(m_nTriggerType);	
	CQusetReadWriteTool::WriteItemToMap(m_mapRequredItem, pFs);
	pFs->WriteUInt(m_nLimitedTime);
}

CQuestItem::CQuestItem()
{
	m_nItemID = 0;
	m_nItemCount = 0;
	m_nValidTime = 0;
}

CQuestItem::~CQuestItem()
{
}

void CQuestItem::ReadDoc(CFileStream* pFs)
{
	pFs->ReadUInt(m_nItemID);
	pFs->ReadUShort(m_nItemCount);
	pFs->ReadInt(m_nValidTime);
}

void CQuestItem::WriteDoc(CFileStream* pFs)
{
	pFs->WriteUInt(m_nItemID);
	pFs->WriteUShort(m_nItemCount);
	pFs->WriteInt(m_nValidTime);
}

CQuestMusic::CQuestMusic()
{
	m_nMusicID = 0;
	m_nModelID = 0;
	m_nSceneID = 0;
	m_nDifficulty = 0;
	m_nMinPlayerNum = 0;
	m_nMinRank = 0;
	m_nMinScore = 0;
	m_nMinPerfect = 0;
	m_nMinConPerfect = 0;
	m_nMaxFail = 0;
	m_nMinCombo = 0;
	m_bConRequired = 0;
	m_nRequiredMatchNum = 0;
}

CQuestMusic::~CQuestMusic()
{
}

void CQuestMusic::ReadDoc(CFileStream* pFs)
{
	pFs->ReadUShort(m_nMusicID);
	pFs->ReadUChar(m_nModelID);
	pFs->ReadUChar(m_nSceneID);
	pFs->ReadUChar(m_nDifficulty);
	pFs->ReadUChar(m_nMinPlayerNum);
	pFs->ReadUChar(m_nMinRank);
	pFs->ReadUInt(m_nMinScore);
	pFs->ReadUShort(m_nMinPerfect);
	pFs->ReadUShort(m_nMinConPerfect);
	pFs->ReadUShort(m_nMaxFail);
	pFs->ReadUShort(m_nMinCombo);
	pFs->ReadBool(m_bConRequired);
	pFs->ReadUShort(m_nRequiredMatchNum);
	
	CQusetReadWriteTool::ReadItemToMap(m_mapCloth, pFs);
}

void CQuestMusic::WriteDoc(CFileStream* pFs)
{
	pFs->WriteUShort(m_nMusicID);
	pFs->WriteUChar(m_nModelID);
	pFs->WriteUChar(m_nSceneID);
	pFs->WriteUChar(m_nDifficulty);
	pFs->WriteUChar(m_nMinPlayerNum);
	pFs->WriteUChar(m_nMinRank);
	pFs->WriteUInt(m_nMinScore);
	pFs->WriteUShort(m_nMinPerfect);
	pFs->WriteUShort(m_nMinConPerfect);
	pFs->WriteUShort(m_nMaxFail);
	pFs->WriteUShort(m_nMinCombo);
	pFs->WriteBool(m_bConRequired);
	pFs->WriteUShort(m_nRequiredMatchNum);
}
void CQuestMusic::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nMusicID);
	IOBuff.AddUChar(m_nModelID);
	IOBuff.AddUChar(m_nSceneID);
	IOBuff.AddUChar(m_nDifficulty);
	IOBuff.AddUChar(m_nMinPlayerNum);
	IOBuff.AddUChar(m_nMinRank);
	IOBuff.AddUInt(m_nMinScore);
	IOBuff.AddUShort(m_nMinPerfect);
	IOBuff.AddUShort(m_nMinConPerfect);
	IOBuff.AddUShort(m_nMaxFail);
	IOBuff.AddUShort(m_nMinCombo);
	IOBuff.AddBool(m_bConRequired);
	IOBuff.AddUShort(m_nRequiredMatchNum);
}

CAttrTarget::CAttrTarget()
{
	m_nTargetLevel = 1;				// 等级
	m_nTargetHotValue = 0;			// 人气
	m_nTargetFriendNum = 0;			// 好友数
	m_nTargetMarriageLevel = 0;		// 婚姻等级
	m_nTargetGroupLevel = 0;		// 舞团
	m_nTargetSchool = 0;			// 门派
	m_nTargetEx1 = 0;
	m_nTargetEx2 = 0;
	m_nTargetEx3 = 0;
	m_nPayCount = 0;
	m_nTargetTitle = 0;
}

CAttrTarget::~CAttrTarget()
{
}

void CAttrTarget::ReadDoc(CFileStream* pFs)
{
	pFs->ReadUShort(m_nTargetLevel);
	pFs->ReadUInt(m_nTargetHotValue);
	pFs->ReadUShort(m_nTargetFriendNum);
	pFs->ReadUChar(m_nTargetMarriageLevel);
	pFs->ReadUChar(m_nTargetGroupLevel);
	pFs->ReadUChar(m_nTargetSchool);
	pFs->ReadInt(m_nPayCount);
	pFs->ReadUChar(m_nTargetTitle);
	pFs->ReadInt(m_nTargetEx1);
	pFs->ReadInt(m_nTargetEx2);
	pFs->ReadInt(m_nTargetEx3);
}

void CAttrTarget::WriteDoc(CFileStream* pFs)
{
	pFs->WriteUShort(m_nTargetLevel);
	pFs->WriteUInt(m_nTargetHotValue);
	pFs->WriteUShort(m_nTargetFriendNum);
	pFs->WriteUChar(m_nTargetMarriageLevel);
	pFs->WriteUChar(m_nTargetGroupLevel);
	pFs->WriteInt(m_nPayCount);
	pFs->WriteUChar(m_nTargetTitle);
	pFs->WriteUChar(m_nTargetSchool);
	pFs->WriteInt(m_nTargetEx1);
	pFs->WriteInt(m_nTargetEx2);
	pFs->WriteInt(m_nTargetEx3);
}

void CAttrTarget::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nTargetLevel);
	IOBuff.AddUInt(m_nTargetHotValue);
	IOBuff.AddUShort(m_nTargetFriendNum);
	IOBuff.AddUChar(m_nTargetMarriageLevel);
	IOBuff.AddUChar(m_nTargetGroupLevel);
	IOBuff.AddUChar(m_nTargetSchool);
	IOBuff.AddInt(m_nTargetEx1);
	IOBuff.AddInt(m_nTargetEx2);
	IOBuff.AddInt(m_nTargetEx3);
}

CQuestReward::CQuestReward()
{
	m_nRewardExp = 0;
	m_nRewardMoney = 0;
}

CQuestReward::~CQuestReward()
{
    std::map<itemtype_t, CQuestItem *>::iterator itr;
    for ( itr = m_mapRewardItemMale.begin(); itr != m_mapRewardItemMale.end(); ++itr )
    {
        delete itr->second, itr->second = NULL;
    }
    m_mapRewardItemMale.clear();

    for ( itr = m_mapRewardItemFemale.begin(); itr != m_mapRewardItemFemale.begin(); ++itr )
    {
        delete itr->second, itr->second = NULL;
    }
    m_mapRewardItemFemale.clear();

    for ( itr = m_mapRewardVipItemMale.begin(); itr != m_mapRewardVipItemMale.end(); ++itr )
    {
        delete itr->second, itr->second = NULL;
    }
    m_mapRewardVipItemMale.clear();

    for ( itr = m_mapRewardVipItemFemale.begin(); itr != m_mapRewardVipItemFemale.end(); ++itr )
    {
        delete itr->second, itr->second = NULL;
    }
    m_mapRewardVipItemFemale.clear();
}

void CQuestReward::ReadDoc(CFileStream* pFs)
{
	pFs->ReadUInt(m_nRewardExp);
	pFs->ReadUInt(m_nRewardMoney);
	CQusetReadWriteTool::ReadItemToMap(m_mapRewardItemMale, pFs);
	CQusetReadWriteTool::ReadItemToMap(m_mapRewardItemFemale, pFs);
}

void CQuestReward::WriteDoc(CFileStream* pFs)
{
	pFs->WriteUInt(m_nRewardExp);
	pFs->WriteUInt(m_nRewardMoney);
	CQusetReadWriteTool::WriteItemToMap(m_mapRewardItemMale, pFs);
	CQusetReadWriteTool::WriteItemToMap(m_mapRewardItemFemale, pFs);
}
void CQuestReward::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRewardExp);
	IOBuff.AddUInt(m_nRewardMoney);
	IOBuff.AddUChar((unsigned char)m_mapRewardItemMale.size());
	if (m_mapRewardItemMale.size() > 0)
	{
		map<itemtype_t, CQuestItem *>::iterator it;
		it = m_mapRewardItemMale.begin();
		for (; it != m_mapRewardItemMale.end(); it++)
		{
			if (it->second != NULL)
			{
				it->second->doEncode(IOBuff);
			}
		}
	}
	IOBuff.AddUChar((unsigned char)m_mapRewardItemFemale.size());
	if (m_mapRewardItemFemale.size() > 0)
	{
		map<itemtype_t, CQuestItem *>::iterator it;
		it = m_mapRewardItemFemale.begin();
		for (; it != m_mapRewardItemFemale.end(); it++)
		{
			if (it->second != NULL)
			{
				it->second->doEncode(IOBuff);
			}
		}
	}
}
CQuestDoc::CQuestDoc()
{
}

CQuestDoc::~CQuestDoc()
{
    for ( std::map<unsigned short, CQuestMusic *>::iterator itr = m_mapMusicTarget.begin();
        itr != m_mapMusicTarget.end(); ++itr )
    {
        delete itr->second, itr->second = NULL;
    }
    m_mapMusicTarget.clear();
}

void CQuestDoc::ReadDoc(CFileStream* pFs)
{
	if (pFs != NULL)
	{
		m_CQusetBaseInfo.ReadDoc(pFs);
		m_CQusetRequirement.ReadDoc(pFs);
		CQusetReadWriteTool::ReadMusicToMap(m_mapMusicTarget, pFs);
		CQusetReadWriteTool::ReadItemToMap(m_mapItemUse, pFs);
		CQusetReadWriteTool::ReadItemToMap(m_mapItemCollection, pFs);
		m_attrTarget.ReadDoc(pFs);
		m_nRewrad.ReadDoc(pFs);
		pFs->ReadString2(m_strDescribe);
		pFs->ReadString2(m_strPrompts);
		pFs->ReadString2(m_strFinalWords);
	}
}
void CQuestDoc::WriteDoc(CFileStream* pFs)
{
	if (pFs != NULL)
	{
		m_CQusetBaseInfo.WriteDoc(pFs);
		m_CQusetRequirement.WriteDoc(pFs);
		CQusetReadWriteTool::WriteMusicToMap(m_mapMusicTarget, pFs);
		CQusetReadWriteTool::WriteItemToMap(m_mapItemUse, pFs);
		CQusetReadWriteTool::WriteItemToMap(m_mapItemCollection, pFs);
		m_attrTarget.WriteDoc(pFs);
		m_nRewrad.WriteDoc(pFs);
		pFs->WriteString2(m_strDescribe);
		pFs->WriteString2(m_strPrompts);
		pFs->WriteString2(m_strFinalWords);
	}
}

void CQuestDoc::doEncode(CParamPool &IOBuff)
{
	m_CQusetBaseInfo.doEncode(IOBuff);
	m_CQusetRequirement.doEncode(IOBuff);
	IOBuff.AddUChar((unsigned char)m_mapMusicTarget.size());	
	if (m_mapMusicTarget.size() > 0)
	{
		map<unsigned short, CQuestMusic *>::iterator it;
		it = m_mapMusicTarget.begin();
		for (; it != m_mapMusicTarget.end(); it++)
		{
			if (it->second != NULL)
			{
				it->second->doEncode(IOBuff);
			}
		}
	}

	IOBuff.AddUChar((unsigned char) m_mapItemUse.size());
	if (m_mapItemUse.size() > 0)
	{
		map<itemtype_t, CQuestItem *>::iterator it;
		it = m_mapItemUse.begin();
		for (; it != m_mapItemUse.end(); it++)
		{
			if (it->second != NULL)
			{
				it->second->doEncode(IOBuff);
			}
		}
	}
		
	IOBuff.AddUChar((unsigned char) m_mapItemCollection.size());
	if (m_mapItemCollection.size() > 0)
	{
		map<itemtype_t, CQuestItem *>::iterator it;
		it = m_mapItemCollection.begin();
		for (; it != m_mapItemCollection.end(); it++)
		{
			if (it->second != NULL)
			{
				it->second->doEncode(IOBuff);
			}
		}
	}

	m_attrTarget.doEncode(IOBuff);
	m_nRewrad.doEncode(IOBuff);
	IOBuff.AddString(m_strDescribe.c_str());
	IOBuff.AddString(m_strPrompts.c_str());
	IOBuff.AddString(m_strFinalWords.c_str());
}

unsigned short CQuestDoc::GetID() 
{ 
	return m_CQusetBaseInfo.m_nQuestID; 
}

unsigned char  CQuestDoc::GetType()const 
{ 
	return m_CQusetBaseInfo.m_nType; 
}

const char *   CQuestDoc::GetQuestName()const 
{ 
	return m_CQusetBaseInfo.m_strQustName.c_str(); 
}

unsigned char  CQuestDoc::GetWeekDay()
{
	return m_CQusetRequirement.m_nWeekDay;
}

unsigned int   CQuestDoc::GetLimitedTime() const 
{ 
	return m_CQusetRequirement.m_nLimitedTime; 
}

unsigned int   CQuestDoc::GetStartTime()const 
{ 
	return m_CQusetRequirement.m_nBeginTime; 
}

unsigned int   CQuestDoc::GetEndTime()const 
{ 
	return m_CQusetRequirement.m_nEndTime; 
}

const char   * CQuestDoc::GetDescription()const 
{ 
	return m_strDescribe.c_str(); 
}

CQuestReward & CQuestDoc::GetQuestReward() 
{ 
	return m_nRewrad; 
}

map<itemtype_t, CQuestItem *> * CQuestDoc::GetQuestMusicRequiredCloth(unsigned short nIndex)const
{
	map<unsigned short, CQuestMusic *>::const_iterator itr = m_mapMusicTarget.find(nIndex);
	if (itr != m_mapMusicTarget.end())
	{
		return &(itr->second->m_mapCloth);
	}
	
	return NULL;
}

unsigned short CQuestDoc::GetExclusiveQuestID()
{
	return m_CQusetRequirement.m_nExclusiveQustID;
}

CQuestMusic * CQuestDoc::GetQuestMusic(unsigned short nIndex)
{
	map<unsigned short, CQuestMusic *>::const_iterator itr = m_mapMusicTarget.find(nIndex);
	if (itr != m_mapMusicTarget.end())
	{
		return &(*itr->second);
	}

	return NULL;
}

bool CQuestDoc::IsDailyQuest()const
{ 
	return m_CQusetBaseInfo.m_nType == Quest_Daily;
}

bool CQuestDoc::IsGrowQuest()const 
{ 
	return m_CQusetBaseInfo.m_nType == Quest_Grow; 
}

bool CQuestDoc::IsActivityQuest()const 
{ 
	return m_CQusetBaseInfo.m_nType == Quest_Activity; 
}

bool CQuestDoc::IsPartyQuest()const 
{ 
	return m_CQusetBaseInfo.m_nType == Quest_Party; 
}

bool CQuestDoc::IsDanceGroupQuest()const 
{ 
	return m_CQusetBaseInfo.m_nType == Quest_DanceGroup; 
}

bool CQuestDoc::IsLoversQuest()const 
{ 
	return m_CQusetBaseInfo.m_nType == Quest_Lovers; 
}

bool CQuestDoc::IsRepeatedQuest()const 
{ 
	return m_CQusetBaseInfo.m_nType == Quest_Daily; 
}



bool CQuestDoc::IsValidTime()
{
	unsigned int nTimeNow = (unsigned int)time(NULL);
	if(m_CQusetRequirement.m_nBeginTime > 0 && nTimeNow < m_CQusetRequirement.m_nBeginTime){
		return false;
	}
	if(m_CQusetRequirement.m_nEndTime > 0 && nTimeNow > m_CQusetRequirement.m_nEndTime){
		return false;
	}
	return true;
}

CQuestDocNew::CQuestDocNew()
{

}

CQuestDocNew::~CQuestDocNew()
{
	std::map<EQuest_Condition_Type,IQuestCondition*>::iterator itc = m_Condition.begin();
	for (; itc != m_Condition.end() ; itc++)
	{
		IQuestCondition* pCond = itc->second;
		delete pCond,pCond = NULL;
	}
	std::map<EQuest_Complete_Type,IQuestComplete*>::iterator itp = m_Complete.begin();
	for (;itp != m_Complete.end() ; itp++)
	{
		IQuestComplete* pComp = itp->second;
		delete pComp,pComp = NULL;
	}
	std::map<EQuest_Destroy_Type,IQuestDestroy*>::iterator itd = m_Destroy.begin();
	for (;itd != m_Destroy.end() ; itd++)
	{
		IQuestDestroy* pDectroy = itd->second;
		delete pDectroy , pDectroy = NULL;
	}
	std::map<EQuest_Reward_Type,IQuestReward*>::iterator itr = m_Reward.begin();
	for (;itr != m_Reward.end() ; itr++)
	{
		IQuestReward* pReward = itr->second;
		delete pReward,pReward = NULL;
	}
}

bool	CQuestDocNew::ReadDoc(CFileStream* pFs)
{
	//基本任务信息
	m_BaseInfo.ReadDoc(pFs);
	//读取条件
	unsigned short nCondCount = 0;
	pFs->ReadUShort(nCondCount);
	for (unsigned short i = 0 ; i < nCondCount ; i++)
	{
		unsigned short condType = 0;
		pFs->ReadUShort(condType);
		IQuestCondition* pCond = IQuestCondition::Create((EQuest_Condition_Type)condType);
		if (pCond == NULL)
		{
			WriteLog(LOGLEVEL_ERROR, "CreateCondition faild, Invalid CondType: condType = %d .\n",condType);
			return false;
		}
		pCond->Load(pFs);
		pCond->SetQuestID(m_BaseInfo.ID);
		m_Condition.insert(make_pair(pCond->GetConditionType(),pCond));
	}
	unsigned short nCompCount = 0;
	pFs->ReadUShort(nCompCount);
	for (unsigned short i = 0 ; i < nCompCount ; i++)
	{
		unsigned short compType = 0;
		pFs->ReadUShort(compType);
		IQuestComplete* pComp = IQuestComplete::Create((EQuest_Complete_Type)compType);
		if (pComp == NULL)
		{
			WriteLog(LOGLEVEL_ERROR, "CreateComplete faild, Invalid CompType: compType = %d .\n",compType);
			return false;
		}
		pComp->Load(pFs);
		pComp->SetQuestID(m_BaseInfo.ID);
		m_Complete.insert(make_pair(pComp->GetCompleteType(),pComp));
	}
	unsigned short nRewardCount = 0;
	pFs->ReadUShort(nRewardCount);
	for (unsigned short i = 0 ; i < nRewardCount ; i++)
	{
		unsigned short rewardType = 0;
		pFs->ReadUShort(rewardType);
		IQuestReward* pReward = IQuestReward::Create((EQuest_Reward_Type)rewardType);
		if (pReward == NULL)
		{
			WriteLog(LOGLEVEL_ERROR, "CreateReward faild, Invalid Reward: rewardType = %d .\n",rewardType);
			return false;
		}
		pReward->Load(pFs);
		pReward->SetQuestID(m_BaseInfo.ID);
		m_Reward.insert(make_pair(pReward->GetRewardType(),pReward));
	}
	unsigned short nDestroyType = 0;
	pFs->ReadUShort(nDestroyType);
	for (unsigned short i = 0 ; i < nDestroyType ; i++)
	{
		unsigned short destroyType = 0;
		pFs->ReadUShort(destroyType);
		IQuestDestroy* pDestroy = IQuestDestroy::Create((EQuest_Destroy_Type)destroyType);
		if (pDestroy == NULL)
		{
			WriteLog(LOGLEVEL_ERROR, "CreateDestroy faild, Invalid Destroy: destroyType = %d .\n",destroyType);
			return false;
		}
		pDestroy->Load(pFs);
		pDestroy->SetQuestID(m_BaseInfo.ID);
		m_Destroy.insert(make_pair(pDestroy->GetDestroyType(),pDestroy));
	}
	if (m_BaseInfo.Type == eQuestTypeNew_Guide)
	{
		//只在客户端用到
		CQuestDocGuide guide;
		guide.ReadDoc(pFs);
	}
	return true;
}

CQuestNewBaseInfo::CQuestNewBaseInfo()
{
	ID = 0;
	GroupID = 0;
	CompleteTimes = 0;
	QuestTarget = 0;
	TimeType = 0;
	StartTime = 0;
	EndTime = 0;
	LastTime = 0;
	Type = 0;
    statisticsType = eQuestStaticTypeNew_Invalid;
	Level = 0;
	DayTimes = 0;
}

bool CQuestNewBaseInfo::ReadDoc(CFileStream* pFs)
{
	pFs->ReadUShort(ID);
	pFs->ReadString2(Name);
	pFs->ReadUShort(GroupID);
	pFs->ReadUInt(CompleteTimes);
	pFs->ReadShort(QuestTarget);
	pFs->ReadChar(TimeType);
	pFs->ReadUInt(StartTime);
	pFs->ReadUInt(EndTime);
	pFs->ReadInt(LastTime);
	pFs->ReadChar(Type);
    pFs->ReadChar(statisticsType);
	pFs->ReadChar(Level);
	pFs->ReadUShort(DayTimes);
	pFs->ReadString2(Description);
	return true;
}

bool CQuestDocNew::CanDestroyByLevel(unsigned short level)
{
	map<EQuest_Destroy_Type,IQuestDestroy*>::iterator it = m_Destroy.begin();
	for (;it != m_Destroy.end() ; it++)
	{
		if (it->second->GetDestroyType() == EQuest_Destroy_Level)
		{
			return it->second->CheckCondition(level);
		}
	}
	return false;
}

unsigned short CQuestDocNew::GetMusicCount(unsigned short musicID)
{
	map<EQuest_Complete_Type,IQuestComplete*>::iterator it = m_Complete.find(EQuest_Complete_UseMusic);
	if (it == m_Complete.end())
	{
		return 0;
	}
	QuestCompleteUseMusic* pComp = (QuestCompleteUseMusic*)it->second;
	return pComp->GetMusicCount(musicID);
}

unsigned short CQuestDocNew::GetUseItemCount(itemtype_t itemID)
{
	map<EQuest_Complete_Type,IQuestComplete*>::iterator it = m_Complete.find(EQuest_Complete_UseItem);
	if (it == m_Complete.end())
	{
		return 0;
	}
	QuestCompleteUseItem* pComp = (QuestCompleteUseItem*)it->second;
	return pComp->GetItemCount(itemID);
}

unsigned short CQuestDocNew::GetUseFunctionCount(unsigned short nFuncID)
{
	return 0;
}

bool CQuestDocNew::GetReward(int nSex,unsigned int& nExp,unsigned int& nMoney,unsigned int& nHonor,unsigned int& contribution,
	unsigned int& vipExp,unsigned int& Intimacy,std::list<CItem>& items , int& bindMCoin)
{
	nExp = 0;
	nMoney = 0;
	nHonor = 0;
	contribution = 0;
	vipExp = 0;
	Intimacy = 0;
	bindMCoin = 0;
	items.clear();

	bool bRet = true;

	map<EQuest_Reward_Type,IQuestReward*>::iterator it = m_Reward.begin();
	for ( ; it != m_Reward.end() ; it++)
	{
		switch(it->first)
		{
		case EQuest_Reward_Exp:
			{
				QuestRewardExp* pReward = (QuestRewardExp*)it->second;
				nExp = pReward->GetExp();
			}
			break;
		case EQuest_Reward_Contribution:
			{
				QuestRewardContribution* pReward = (QuestRewardContribution*)it->second;
				contribution = pReward->GetContribution();
			}
			break;
		case EQuest_Reward_Honor:
			{
				QuestRewardHonor* pReward = (QuestRewardHonor*)it->second;
				nHonor = pReward->GetHonor();
			}
			break;
		case EQuest_Reward_Intimacy:
			{
				QuestRewardIntimacy* pReward = (QuestRewardIntimacy*)it->second;
				Intimacy = pReward->GetIntimacy();
			}
			break;
		case EQuest_Reward_Item:
			{
				QuestRewardItem* pReward = (QuestRewardItem*)it->second;
				bRet = pReward->GetRewardItem(nSex,items);
			}
			break;
		case EQuest_Reward_Money:
			{
				QuestRewardMoney* pReward = (QuestRewardMoney*)it->second;
				nMoney = pReward->GetMoney();
			}
			break;
		case EQuest_Reward_VipValue:
			{
				QuestRewardVipValue* pReward = (QuestRewardVipValue*)it->second;
				vipExp = pReward->GetValue();
			}
			break;
		case EQuest_Reward_BindMCoin:
			{
				QuestRewardBingMCoin* pReward = (QuestRewardBingMCoin*)it->second;
				bindMCoin = pReward->GetBindMCoin();
			}
			break;
        case EQuest_VipReward_Exp:
        case EQuest_VipReward_Money:
        case EQuest_VipReward_Honor:
        case EQuest_VipReward_Contribution:
        case EQuest_VipReward_VipValue:
        case EQuest_VipReward_Intimacy:
        case EQuest_VipReward_Item:
        case EQuest_VipReward_BindMCoin:
            // do nothing...
            break;
        case EQuest_Reward_None:
            WriteLog(LOGLEVEL_ERROR, "error quest[%d] reward none type.", (int)QuestID());
            break;

		default:
			break;
		}
	}
	return bRet;
}

bool CQuestDocNew::GetVipReward(int nSex,unsigned int& nExp,unsigned int& nMoney,unsigned int& nHonor,unsigned int& contribution,
    unsigned int& vipExp,unsigned int& Intimacy,std::list<CItem>& items , int& bindMCoin)
{
    nExp = 0;
    nMoney = 0;
    nHonor = 0;
    contribution = 0;
    vipExp = 0;
    Intimacy = 0;
    bindMCoin = 0;
    items.clear();

    bool bRet = true;

    map<EQuest_Reward_Type,IQuestReward*>::iterator it = m_Reward.begin();
    for ( ; it != m_Reward.end() ; it++)
    {
        switch(it->first)
        {
        case EQuest_VipReward_Exp:
            {
                QuestVipRewardExp* pReward = (QuestVipRewardExp*)it->second;
                nExp = pReward->GetExp();
            }
            break;
        case EQuest_VipReward_Contribution:
            {
                QuestVipRewardContribution* pReward = (QuestVipRewardContribution*)it->second;
                contribution = pReward->GetContribution();
            }
            break;
        case EQuest_VipReward_Honor:
            {
                QuestVipRewardHonor* pReward = (QuestVipRewardHonor*)it->second;
                nHonor = pReward->GetHonor();
            }
            break;
        case EQuest_VipReward_Intimacy:
            {
                QuestVipRewardIntimacy* pReward = (QuestVipRewardIntimacy*)it->second;
                Intimacy = pReward->GetIntimacy();
            }
            break;
        case EQuest_VipReward_Item:
            {
                QuestVipRewardItem* pReward = (QuestVipRewardItem*)it->second;
                bRet = pReward->GetRewardItem(nSex,items);
            }
            break;
        case EQuest_VipReward_Money:
            {
                QuestVipRewardMoney* pReward = (QuestVipRewardMoney*)it->second;
                nMoney = pReward->GetMoney();
            }
            break;
        case EQuest_VipReward_VipValue:
            {
                QuestVipRewardVipValue* pReward = (QuestVipRewardVipValue*)it->second;
                vipExp = pReward->GetVipValue();
            }
            break;
        case EQuest_VipReward_BindMCoin:
            {
                QuestVipRewardBindMCoin* pReward = (QuestVipRewardBindMCoin*)it->second;
                bindMCoin = pReward->GetBindMCoin();
            }
            break;
        case EQuest_Reward_Exp:
        case EQuest_Reward_Contribution:
        case EQuest_Reward_Honor:
        case EQuest_Reward_Intimacy:
        case EQuest_Reward_Item:
        case EQuest_Reward_Money:
        case EQuest_Reward_VipValue:
        case EQuest_Reward_BindMCoin:
            // do nothing...
            break;
        case EQuest_Reward_None:
            WriteLog(LOGLEVEL_ERROR, "error quest[%d] reward none type.", (int)QuestID());
            break;

        default:
            break;
        }
    }
    return bRet;
}

IQuestComplete* CQuestDocNew::GetComplete(EQuest_Complete_Type type)
{
    map<EQuest_Complete_Type,IQuestComplete*>::iterator it = m_Complete.find(type);
    if (it == m_Complete.end())
    {
        return NULL;
    }
    return it->second;
}

bool CQuestDocNew::IsExchangeQuest()
{
    return GetComplete(EQuest_Complete_ObtainItem) != NULL;
}

void CQuestDocNew::GetExchangeItem(std::vector<CQuestItem>& items)
{
    QuestCompleteObtainItem* pComp = (QuestCompleteObtainItem*)GetComplete(EQuest_Complete_ObtainItem);
    if (pComp != NULL)
    {
        pComp->GetQuestItems(items);
    }
}

bool CQuestDocGuide::ReadDoc(CFileStream* pFs)
{
    pFs->ReadUShort(m_nAction);
    pFs->ReadUShort(m_nStep);
    pFs->ReadInt(m_nParam);
    pFs->ReadString2(m_strParam2);
    pFs->ReadBool(m_bForce);
    pFs->ReadString2(m_strExtendText);

	// 服务器不需要这四个string.
	std::string strTemp;
	pFs->ReadString2(strTemp);
	pFs->ReadString2(strTemp);
	pFs->ReadString2(strTemp);
	pFs->ReadString2(strTemp);


    return true;
}

bool CQuestDocNew::IsDivorceDestroyQuest()
{
    map<EQuest_Destroy_Type,IQuestDestroy*>::iterator it = m_Destroy.find(EQuest_Destroy_Divorce);
    return it != m_Destroy.end();
}

bool CQuestDocNew::IsDanceGroupDestroyQuest()
{
    map<EQuest_Destroy_Type,IQuestDestroy*>::iterator it = m_Destroy.find(EQuest_Destroy_LeaveDanceGroup);
    return it != m_Destroy.end();
}

bool CQuestDocNew::CheckTimeType(unsigned int nDay)
{
    switch ((EQuestTimeType)m_BaseInfo.TimeType)
    {
    case EQuestTimeType_Monday:
        return nDay == MONDAY;
    case EQuestTimeType_Tuesday:
        return nDay == TUESDAY;
    case EQuestTimeType_Wednesday:
        return nDay == WEDNESDAY;
    case EQuestTimeType_Thursday:
        return nDay == THURSDAY;
    case EQuestTimeType_Friday:
        return nDay == FRIDAY;
    case EQuestTimeType_Saturday:
        return nDay == SATURDAY;
    case EQuestTimeType_Sunday:
        return nDay == SUNDAY;
    case EQuestTimeType_EveryWeek:
    case EQuestTimeType_EveryDay:
        return true;
    default:
        return true;
    }
}

bool CQuestDocNew::IsReAcceptType()
{
    return (eQuestTypeNew_Activity == m_BaseInfo.Type
        || eQuestTypeNew_Daily == m_BaseInfo.Type);

}

//end file


