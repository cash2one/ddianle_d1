/************************************************************************/
/* 
**	数据库的历史任务表会使用index + __int64，用位来保存历史任务
**  是否完成。
**	另外，因为一些可重复的任务需要记录当日是否已经做过，所以另外
**	使用repeated_quest_done表来记录每日任务的完成情况，该表不会删除。
**	只会记录完成的时间，当天是否已经完成可以此来判断。
*/  
/************************************************************************/

#ifndef	__ENTITYQUESTCOMPONENT_H__
#define __ENTITYQUESTCOMPONENT_H__
#include "../logic/RoleComponentImpl.h"
#include "../logic/EntityComponent.h"
#include "QuestDataManager.h"
#include <map>
#include <set>
#include <list>

class CEntityAttributeComponent;
class CEntityNetComponent;
class CEntityChatComponent;
class CEntityItemComponent;
class CEntityAchievementComponent;
class CEntityFriendComponent;
class CEntityTimerComponent;
class CRoomComponent;
class CEntityDanceGroupComponent;
class CEntityCoupleComponent;
class CEntityVIPComponent;

class CQuestDoc;
class QUESTINFO;

class CEntityQuestComponent :
	public CEntityComponent
{
private:
	CEntityAttributeComponent   * m_pAttr;
	CEntityNetComponent         * m_pNet;
	CEntityItemComponent        * m_pItem;
	CEntityChatComponent		* m_pChat;
	CEntityAchievementComponent * m_pAch;
	CEntityFriendComponent       * m_pFriend;
	CEntityTimerComponent       * m_pTimer;
	CEntityDanceGroupComponent  * m_pGroup;
	CEntityCoupleComponent      * m_pCouple;
    CEntityVIPComponent         * m_pVip;

public:
	CEntityQuestComponent();
	~CEntityQuestComponent();
public:
	virtual void		Start();
	void				RegComponentNetMsgMap();
	virtual void		SerializeComponent(CParamPool &IOBuff);
	virtual const char* SerializeName(){ return "Quest"; };
	void				OnEvent(CComponentEvent& refEvent );
	bool				CreateFromDB(ROLEINFO_DB* pRoleInfoDB);
	virtual bool		PacketToDB(ROLEINFO_DB *pRoleInforDB) const; 
	virtual void		OnUpdate(const unsigned long &nTimeElapsed);
	virtual void		OnLogin();
	virtual void		OnLogout();
	virtual void		SendExtraStaticData();
	virtual void		PacketBuff(std::list<BuffData>& listBuff){}
public:
	void FinQuest(unsigned short nQuestID);	//for cmd
	void PushPlayerQuest(bool bPushTimer = false);

	void OnGetQuestList(GameMsg_Base &msg, CSlotPeer & slotPeer);
	void OnQuestRead(GameMsg_Base &msg, CSlotPeer & slotPeer);
	void OnAcceptQuest(GameMsg_Base &msg, CSlotPeer & slotPeer);
	void OnGetQuestReward(GameMsg_Base &msg, CSlotPeer & slotPeer);	// 领奖之后才算是完成该任务
	void OnGetFresherQuestReward(GameMsg_Base &msg, CSlotPeer & slotPeer);

	void OnBuyItem();
	void OnGetFresherQuestList(GameMsg_Base &msg, CSlotPeer & slotPeer);

	void OnUseItem(itemtype_t nItemType, unsigned short nCount);
	void OnItemCountChange(itemtype_t nItemType, int nCount);
	void MusicMatchChange(QUESTUPDATEINFO *pQuestUpdateInfo);
	void MusicGradeChange(int nGrade);
	void OnRoleAttrChange();
	void MusicEnd(CRoomComponent& rRoleRoomCOM); 

	unsigned int GetCompletedQuestCountByType(unsigned short nType);// 某类型任务的完成数
	unsigned int GetCompletedAllQuestCount();


	void PushFresherQuestStateChange();
    void GetFresherVIPExtraReward();
private:
	void _SetQuestState(QUESTINFO *pQuestInfo,unsigned char nQuestState);
	QUESTINFO * _GetPlayerQuest(unsigned short nQuestID);

	bool _HasAccept(unsigned short nQuestID);   // 已接受任务,进行中，但未完成
	bool _HasComplete(unsigned short nQuestID); // 已完成，可领奖，但未领奖
	bool _HasDone(unsigned short nQuestID);     // 已完成，在历史任务列表中
	
	void _AddQuest(CQuestDoc *pQuestDoc);
	void _AddQuestHistory(unsigned short nQuestID);
	void _AddQuestHistoryToDB(unsigned short nQuestID);
	void _UpdateQuestHistoryToDB(unsigned short nQuestID);
	void _AddRepeatedQuestDoneToDB(CRepeatedHistoryQuest &info);
	void _UpdateRepeatedQuestToDB(CRepeatedHistoryQuest & info);

	bool _CanAcceptQuest(CQuestDoc *pQuestDoc);
	bool _CanCompleteQuest(QUESTINFO *pQuestInfo);
	bool _CanCompleteMusicTarget(QUESTINFO *pQuestInfo);
	bool _CanCompleteItemUseTarget(QUESTINFO *pQuestInfo);
	bool _CanCompleteItemCollectionTarget(QUESTINFO *pQuestInfo);

	void _RemoveQuest(unsigned short nQuestID);
	void _RemoveQuestToDB(unsigned short nQuestID);
	void _RemoveExpiredQuest();	// 删除到期的的任务
		
	void _UpdateCollectionTarget(QUESTINFO &questInfo);

	bool _UpdateMusicTarget(CQuestDoc * pQuestDoc, CQuestMusic * pQuestMusic,QUESTUPDATEINFO *pQuestUpdateInfo);
	void _UpdateTargetToDB(unsigned short nQuestID,TargetInfo * pTarget);

	void _AddQuestToDBCallBack(QueryBase &query);
	void _AddQuestHistoryToDBCallBack(QueryBase &query);
	void _UpdateQuestInfoCallBack(QueryBase &query);
	void _RemoveQuestToDBCallBack(QueryBase &query);
	void _SendFresherQuestAndReward();

	void _AddQuestTimer(QUESTINFO *pQuestInfo);
	void _DelExclusiveQuest(unsigned short nQuestID);

private:
	bool _IsInQuestList(unsigned short nQuestID);
	void _SendPlayerMsg(GameMsg_Base *pMsg);
	unsigned int _GetItemCount(itemtype_t nItemID);
	bool _GetEnoughItem(map<itemtype_t, CQuestItem *>& mapItem);
	bool _CanAddItem(map<itemtype_t, CQuestItem *> &mapItemPlus);
	void _GetReward(unsigned short nQuestID, int nMoney, int nExp, map<itemtype_t, CQuestItem *> &mapItem);
	void _AddRewardItem(map<itemtype_t, CQuestItem *> &mapItem, unsigned short nQuestID);
	void _RemoveCollectionItem(unsigned short nQuestID, map<itemtype_t, CQuestItem *> &mapItemCollection);
	bool _GetEnoughRequiredEquipedCloth(map<itemtype_t, CQuestItem *> &mapQuestMusicCloth)const;
	void _NotifyPlayerQuestStateChange(unsigned short nQuestID, unsigned char nState);
	void _ProcessNewQuest(CQuestDoc *pQuestDoc);
	bool _AcceptQuest(unsigned short nQuestID, bool bSendMsg);
	bool _CanAddDailyQuest(CQuestDoc & questDoc);
	__int64 _GetHistoryQuestBlob(unsigned short nQuestID);
	void _UpdateQuestTotalCount();
	void _AddRepeatedQuestDone(CRepeatedHistoryQuest &info);
	CRepeatedHistoryQuest * _GetRepeatedQuestDoneBy(unsigned short nQuestID);
	bool _HasDoneRepeatedQuest(unsigned short nQuestID);
	bool _HasDoneRepeatedQuestToday(unsigned short nQuestID);

	bool _IsRepeatedQuest(unsigned short nType);
	bool _CanReachSpecailCond(unsigned int nQuestID) const;

	//完成任务日志
	void _LogRoleQuest(unsigned int nRoleID, unsigned int nQuestID);
	void _SendQuestStaticBinData();

	// 新手七日目标
	void _SendFresherQuest();
	void _SendFresherUnReceivedReward();
	bool CanCompleteFresherQuest(CFresherQuestDoc *pDoc);
	void SendPlayerFresherQuestReward(CFresherQuestDoc *pDoc, bool bSendMail, bool bIsVipReward);
	unsigned char FresherDayToEventID(int nDay);
	bool HasReceivedFresherQuestReward(int nQuestID);
    void _GetFresherQuestReward(int nQuestID, bool bIsExtraReward);
private:
	list< QUESTINFO > m_listQuest;	// 当前任务列表
	unsigned int m_nNewQuestCount;	// 新增任务数量
	unsigned int m_nNewCompleteQuestCount;	// 新近完成的任务数量

	list< CRepeatedHistoryQuest > m_listRepeatedQuest;	//	只读取已经完成或者曾经过的日常任务，每次完成一个日常任务则插进数据库，如果已经存在则更新时间,完成次数+1；
	map<short, __int64> m_mapQuestHistory;				//	完成的任务记录对应的位置为1,该字段包含所有的任务，包括日常任务。
	unsigned int m_nLastStop;							//	记录上一次时间点
	unsigned int m_nLastAutoPushTime;					//  上一次自动领取任务的时间
	
	std::set<int>   m_setFresherQuestReward;			//新手七天目标奖励领取标志,里面存放questID
	int m_nFresherDay;
	int m_nMallBuyCount;
	int m_nFresherBestMusicGrade;						//新手期间最好成绩（A/B/C/D/S/SS级判定）
    unsigned int m_nFresherLastRewardQuest;             //新手七天目标最后领取的任务ID
};

typedef CRoleComponentImpl<CEntityQuestComponent,CGetRoleProcessorObj,CGetRoleProcessorObj> CRoleQuest;

#endif


//file end




