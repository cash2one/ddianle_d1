/************************************************************************/
/* 
**	���ݿ����ʷ������ʹ��index + __int64����λ��������ʷ����
**  �Ƿ���ɡ�
**	���⣬��ΪһЩ���ظ���������Ҫ��¼�����Ƿ��Ѿ���������������
**	ʹ��repeated_quest_done������¼ÿ����������������ñ���ɾ����
**	ֻ���¼��ɵ�ʱ�䣬�����Ƿ��Ѿ���ɿ��Դ����жϡ�
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
	void OnGetQuestReward(GameMsg_Base &msg, CSlotPeer & slotPeer);	// �콱֮���������ɸ�����
	void OnGetFresherQuestReward(GameMsg_Base &msg, CSlotPeer & slotPeer);

	void OnBuyItem();
	void OnGetFresherQuestList(GameMsg_Base &msg, CSlotPeer & slotPeer);

	void OnUseItem(itemtype_t nItemType, unsigned short nCount);
	void OnItemCountChange(itemtype_t nItemType, int nCount);
	void MusicMatchChange(QUESTUPDATEINFO *pQuestUpdateInfo);
	void MusicGradeChange(int nGrade);
	void OnRoleAttrChange();
	void MusicEnd(CRoomComponent& rRoleRoomCOM); 

	unsigned int GetCompletedQuestCountByType(unsigned short nType);// ĳ��������������
	unsigned int GetCompletedAllQuestCount();


	void PushFresherQuestStateChange();
    void GetFresherVIPExtraReward();
private:
	void _SetQuestState(QUESTINFO *pQuestInfo,unsigned char nQuestState);
	QUESTINFO * _GetPlayerQuest(unsigned short nQuestID);

	bool _HasAccept(unsigned short nQuestID);   // �ѽ�������,�����У���δ���
	bool _HasComplete(unsigned short nQuestID); // ����ɣ����콱����δ�콱
	bool _HasDone(unsigned short nQuestID);     // ����ɣ�����ʷ�����б���
	
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
	void _RemoveExpiredQuest();	// ɾ�����ڵĵ�����
		
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

	//���������־
	void _LogRoleQuest(unsigned int nRoleID, unsigned int nQuestID);
	void _SendQuestStaticBinData();

	// ��������Ŀ��
	void _SendFresherQuest();
	void _SendFresherUnReceivedReward();
	bool CanCompleteFresherQuest(CFresherQuestDoc *pDoc);
	void SendPlayerFresherQuestReward(CFresherQuestDoc *pDoc, bool bSendMail, bool bIsVipReward);
	unsigned char FresherDayToEventID(int nDay);
	bool HasReceivedFresherQuestReward(int nQuestID);
    void _GetFresherQuestReward(int nQuestID, bool bIsExtraReward);
private:
	list< QUESTINFO > m_listQuest;	// ��ǰ�����б�
	unsigned int m_nNewQuestCount;	// ������������
	unsigned int m_nNewCompleteQuestCount;	// �½���ɵ���������

	list< CRepeatedHistoryQuest > m_listRepeatedQuest;	//	ֻ��ȡ�Ѿ���ɻ������������ճ�����ÿ�����һ���ճ������������ݿ⣬����Ѿ����������ʱ��,��ɴ���+1��
	map<short, __int64> m_mapQuestHistory;				//	��ɵ������¼��Ӧ��λ��Ϊ1,���ֶΰ������е����񣬰����ճ�����
	unsigned int m_nLastStop;							//	��¼��һ��ʱ���
	unsigned int m_nLastAutoPushTime;					//  ��һ���Զ���ȡ�����ʱ��
	
	std::set<int>   m_setFresherQuestReward;			//��������Ŀ�꽱����ȡ��־,������questID
	int m_nFresherDay;
	int m_nMallBuyCount;
	int m_nFresherBestMusicGrade;						//�����ڼ���óɼ���A/B/C/D/S/SS���ж���
    unsigned int m_nFresherLastRewardQuest;             //��������Ŀ�������ȡ������ID
};

typedef CRoleComponentImpl<CEntityQuestComponent,CGetRoleProcessorObj,CGetRoleProcessorObj> CRoleQuest;

#endif


//file end




