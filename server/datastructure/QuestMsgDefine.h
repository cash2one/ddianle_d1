/********************************************************************
	created:	2010/12/03
	filename: 	DataStructure\questmsgdefine.h
	author:		meijiuhua
	
	purpose:	任务消息
*********************************************************************/

#ifndef __QuestMsgDefine_H__
#define __QuestMsgDefine_H__

#include "gamemsgdef.h"
#include "../nsytworld/questdoc.h"

#pragma pack(1)

///////////////////////////////////////////////////////////////////////////////////////
//所有0x0d00 - 0x0dff之间的消息值保留给任务使用
//All messages between 0x0d00 - 0x0dff are reserved for quests, please do not use them !!!
///////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// quest messages

/*
struct GAMMSG_C2S_AskAllShowQuests : public GAMEMSG_BASE
{
	GAMMSG_C2S_AskAllShowQuests()
	{
		nMsg = MSG_C2S_AskAllShowQuests;
		nSize = sizeof(GAMMSG_C2S_AskAllShowQuests);
	}
};

struct GAMEMSG_C2S_AskQuestInfo : public GAMEMSG_BASE
{
	unsigned int nQuestID;

	//QT_BASEINFO = 0,		//基本信息，包含任务id、名字和类型
	//QT_CANACEPT,			//能接任务详细信息
	//QT_ACEPTED,			//已接任务详细信息
	//QT_COMPLETE,			//已完成任务详细信息
	//QT_GONGGAO,			//不能接的类型，公告
	//QT_CHONGZHISONGLI		//不能接的类型, 充值送礼领取
	unsigned char nType;

	GAMEMSG_C2S_AskQuestInfo()
	{
		nQuestID = 0;
		nType = QT_ACEPTED;
		nMsg = MSG_C2S_AskQuestInfo;
		nSize = sizeof(GAMEMSG_C2S_AskQuestInfo);
	}
};

struct GAMEMSG_S2C_AskQuestInfo : public GAMEMSG_BASE
{
	unsigned int nQuestID;
	
	//QT_BASEINFO = 0,		//基本信息，包含任务id、名字和类型
	//QT_CANACEPT,			//能接任务详细信息
	//QT_ACEPTED,			//已接任务详细信息
	//QT_COMPLETE,			//已完成任务详细信息
	//QT_GONGGAO,			//不能接的类型，公告
	//QT_CHONGZHISONGLI		//不能接的类型, 充值送礼领取
	unsigned char nType;

	char data[1];
	GAMEMSG_S2C_AskQuestInfo()
	{
		nQuestID = 0;
		nType = QT_ACEPTED;
		nMsg = MSG_S2C_AskQuestInfo;
		nSize = sizeof(GAMEMSG_S2C_AskQuestInfo);
	}
};

struct GAMEMSG_C2S_AcceptQuest : public GAMEMSG_BASE
{
	unsigned int uQuestID;

	GAMEMSG_C2S_AcceptQuest()
	{
		uQuestID = 0;
		nMsg = MSG_C2S_AcceptQuest;
		nSize = sizeof(GAMEMSG_C2S_AcceptQuest);
	}
};

//struct GAMEMSG_S2C_AcceptQuest : public GAMEMSG_BASE
//{
//	//包含任务id、名称和类型，以及状态
//	char data[1];
//
//	GAMEMSG_S2C_AcceptQuest()
//	{
//		nMsg = MSG_S2C_AcceptQuest;
//		nSize = sizeof(GAMEMSG_S2C_AcceptQuest);
//	}
//};

struct GAMEMSG_C2S_CompleteQuest : public GAMEMSG_BASE
{
	unsigned int uQuestID;

	GAMEMSG_C2S_CompleteQuest()
	{
		uQuestID = 0;
		nMsg = MSG_C2S_CompleteQuest;
		nSize = sizeof(GAMEMSG_C2S_CompleteQuest);
	}
};

struct GAMEMSG_S2C_CompleteQuest : public GAMEMSG_BASE
{
	unsigned int uQuestID;

	//完成任务的描述，如果为空，默认提示“任务完成”即可
	char data[1];

	GAMEMSG_S2C_CompleteQuest()
	{
		uQuestID = 0;
		nMsg = MSG_S2C_CompleteQuest;
		nSize = sizeof(GAMEMSG_S2C_CompleteQuest);
	}
};

struct GAMEMSG_S2C_QuestHasMeetAllTargets : public GAMEMSG_BASE
{
	unsigned int uQuestID;

	GAMEMSG_S2C_QuestHasMeetAllTargets()
	{
		uQuestID = 0;
		nMsg = MSG_S2C_QuestHasMeetAllTargets;
		nSize = sizeof(GAMEMSG_S2C_QuestHasMeetAllTargets);
	}
};

struct GAMEMSG_S2C_QuestHasLostTarget : public GAMEMSG_BASE
{
	unsigned int uQuestID;

	GAMEMSG_S2C_QuestHasLostTarget()
	{
		uQuestID = 0;
		nMsg = MSG_S2C_QuestHasLostTarget;
		nSize = sizeof(GAMEMSG_S2C_QuestHasLostTarget);
	}
};

struct GAMEMSG_S2C_QuestFailed : public GAMEMSG_BASE
{
	unsigned int uQuestID;
	char reason;

	GAMEMSG_S2C_QuestFailed()
	{
		uQuestID = 0;
		reason = 0;
		nMsg = MSG_S2C_QuestFailed;
		nSize = sizeof(GAMEMSG_S2C_QuestFailed);
	}
};

struct GAMEMSG_S2C_AceptQuestFailed : public GAMEMSG_BASE
{
	unsigned int uQuestID;
	char reason;

	GAMEMSG_S2C_AceptQuestFailed()
	{
		uQuestID = 0;
		reason = 0;
		nMsg = MSG_S2C_AceptQuestFailed;
		nSize = sizeof(GAMEMSG_S2C_AceptQuestFailed);
	}
};

struct GAMEMSG_S2C_QuestRewardFailed: public GAMEMSG_BASE
{
	short nReason;
	GAMEMSG_S2C_QuestRewardFailed()
	{
		nReason = 0;
		nMsg = MSG_S2C_QuestRewardFailed;
		nSize = sizeof(GAMEMSG_S2C_QuestRewardFailed);
	}
};

struct GAMEMSG_S2C_SendQuest: public GAMEMSG_BASE
{
	int m_nQuestSize;
	char m_pQuestInfo[1];

	GAMEMSG_S2C_SendQuest()
	{
		nMsg = MSG_S2C_SendQuest;
		m_nQuestSize = 0;
	}
};

struct GAMEMSG_S2C_RemoveQuest : public GAMEMSG_BASE
{
	unsigned int uQuestID;

	GAMEMSG_S2C_RemoveQuest()
	{
		uQuestID = 0;
		nMsg = MSG_S2C_RemoveQuest;
		nSize = sizeof(GAMEMSG_S2C_RemoveQuest);
	}
};

struct GAMEMSG_C2S_AbandonQuest : public GAMEMSG_BASE
{
	unsigned int uQuestID;

	GAMEMSG_C2S_AbandonQuest()
	{
		uQuestID = 0;
		nMsg = MSG_C2S_AbandonQuest;
		nSize = sizeof(GAMEMSG_C2S_AbandonQuest);
	}
};

struct GAMEMSG_S2C_AbandonQuest : public GAMEMSG_BASE
{
	unsigned int uQuestID;

	GAMEMSG_S2C_AbandonQuest()
	{
		uQuestID = 0;
		nMsg = MSG_S2C_AbandonQuest;
		nSize = sizeof(GAMEMSG_S2C_AbandonQuest);
	}
};


struct GAMEMSG_S2C_TellRewardItem : public GAMEMSG_BASE
{
	int nItemType;
	char num;

	GAMEMSG_S2C_TellRewardItem()
	{
		nItemType = 0;
		num = 0;
		nMsg = MSG_S2C_TellRewardItem;
		nSize = sizeof(GAMEMSG_S2C_TellRewardItem);
	}
};

struct GAMEMSG_S2C_UpdateQuestData: public GAMEMSG_BASE
{
	unsigned int uQuestID;

	//更新模块的索引，这里很重要，有疑问请当面询问
	int nIndex;
	//更新模块的目标id
	unsigned int nTargetID;
	short nUpdateNum;

	GAMEMSG_S2C_UpdateQuestData()
	{
		uQuestID = 0;
		nIndex = 0;
		nTargetID = 0;
		nUpdateNum = 0;
		nMsg = MSG_S2C_UpdateQuestData;
		nSize = sizeof(GAMEMSG_S2C_UpdateQuestData);
	}
};


struct GAMEMSG_C2S_AcquireQuestDesc : public GAMEMSG_BASE
{
	unsigned int uQuestID;
	char cFlag; // 0 desc 1 prompt 2 final

	GAMEMSG_C2S_AcquireQuestDesc()
	{
		uQuestID = 0;
		cFlag = 0;
		nMsg = MSG_C2S_AcquireQuestDesc;
		nSize = sizeof(GAMEMSG_C2S_AcquireQuestDesc);
	}
};

struct GAMEMSG_S2C_UpdateQuestDesc : public GAMEMSG_BASE
{
	unsigned int uQuestID;
	char cFlag; // 0 desc 1 prompt 2 final
	char szDesc[1];

	GAMEMSG_S2C_UpdateQuestDesc()
	{
		uQuestID = 0;
		cFlag = 0;
		nMsg = MSG_S2C_UpdateQuestDesc;
		nSize = sizeof(MSG_S2C_UpdateQuestDesc);
	}
};
*/
#pragma pack()

#endif

