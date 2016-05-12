#ifndef _ENTITY_QUEST_NEW_COMPONENT_H__
#define _ENTITY_QUEST_NEW_COMPONENT_H__

#include <map>
#include <set>
#include <vector>
#include "../logic/EntityComponent.h"
#include "../../nsytworld/QuestDef.h"
#include "../../nsytworld/QuestDoc.h"
#include "../../datastructure/DataStruct_Base.h"
#include "../logic/RoleComponentImpl.h"
#include "QuestListEntry.h"
#include "QuestRecord.h"

class CEntityAttributeComponent;
class CEntityNetComponent;
class CEntityItemComponent;
class CEntityVIPComponent;
class CEntityCoupleComponent;
class CEntityDanceGroupComponent;
class CEntityFriendComponent;
class CEntityTimerComponent;
class CQuestTargetSend;
class CEntityCheckInComponent;
class CEntityDungeonComponent;

struct CompleteTargetInfo
{
	unsigned int m_ID;
	unsigned short m_Count;
};

class CEntityQuestNewComponent : public CEntityComponent
{
public:
	CEntityQuestNewComponent();
	~CEntityQuestNewComponent();

private:
	CEntityAttributeComponent   * m_pAttr;
	CEntityNetComponent         * m_pNet;
	CEntityItemComponent        * m_pItem;
	CEntityVIPComponent			* m_pRoleVIP;
	CEntityCoupleComponent		* m_pCouple;
	CEntityDanceGroupComponent  * m_pDanceGroup;
	CEntityFriendComponent		* m_pFriend;
	CEntityTimerComponent		* m_pTimer;
    CEntityCheckInComponent     * m_pCheckIn;
    CEntityDungeonComponent     * m_pDungeon;

private:
    typedef std::set<unsigned short> QuestIDSet;
    QuestIDSet	m_HistroyQuest;		
	QuestIDSet	m_UnAcceptQuest;	//��δ���ܵ�����
	QuestIDSet	m_CurrentQuest;		//���ڽ����е�����
//	std::vector<unsigned short>				m_DestroyQuest;					//�����ٵ�����
    //���Ա����½ӵ����� �������ûʲô����
//    std::map<unsigned short,CQuestDocNew*>	m_CanReAcceptQuest;	
	std::map<unsigned short,QUESTNEWINFO>	m_QuestDetailInfo;	//��ϸ��������(���ݿ��Ӧ��Ϣ)
	std::map<unsigned short,QUESTNEWINFO>	m_DirtyQuest;	    //��Ҫ���µ�DB��������ϸ����
    unsigned int m_nDirtyTime;
	
	std::vector<int>						m_QuestLocationList;//���н������������£�����ֶ������Ƿ����ĳ����

    typedef std::map<unsigned short,unsigned short > MusicParamMap;
    typedef std::map<itemtype_t,unsigned short > UseItemParamMap;
    typedef std::map<int,unsigned short > FunctionParamMap;
	std::map<unsigned short, MusicParamMap> m_MusicCount;		//������ɴ������ֶκ���Ϊ������id��������id����ɴ�������
	std::map<unsigned short, UseItemParamMap> m_ItemUseCount;		//��Ʒʹ�ô������ֶκ���Ϊ������id����ʹ����Ʒid����ɴ�������
	std::map<unsigned short, FunctionParamMap> m_UseFunctionCount;	//����ʹ�ô������ֶκ���Ϊ������id����ʹ�ù������ͣ���ɴ�������

    QuestRecordManager m_recordManager;

	//unsigned short							m_GuideQuestID;
	bool									m_bSendQuestDataToClient;			//�Ƿ��Ѿ����͹������б�
    unsigned short                          m_UnRewardVipQuest;

public:
	virtual void		Start();
	void				RegComponentNetMsgMap();
	virtual void		SerializeComponent(CParamPool &IOBuff);
	virtual const char* SerializeName(){ return "QuestNew"; };
	void				OnEvent(CComponentEvent& refEvent );
	bool				CreateFromDB(ROLEINFO_DB* pRoleInfoDB);
	virtual bool		PacketToDB(ROLEINFO_DB *pRoleInforDB) const; 
    virtual bool		PacketToCache(ROLEINFO_DB *pRoleInfoCache) const;
    virtual void		OnUpdate(const unsigned long &nTimeElapsed);
	virtual void		OnLogin();
	virtual void		OnLogout();
	virtual void		SendExtraStaticData();
	virtual void		PacketBuff(std::list<BuffData>& listBuff){}

	//�ⲿ���ýӿ�
public:
	void OnAttributeChange();			//��ɫ���Ըı䣬�����ȼ������ѣ����£���飬���ܶȣ�vip���������ţ����׶ȣ��������������Σ���õ��ߣ��������
	void OnUseItem(itemtype_t itemID,int nCount);
	void OnUseItem(itemtype_t itemID);		//ʹ�õ��ߣ��ɴ����������Ҫʹ�õ��ߵ�����
	void OnMusicEnd(QUESTUPDATEINFO *pQuestUpdateInfo);			//���ֽ��������������������
	void OnOpenComponent(unsigned short nComponentID);
	void OnChangeLocation(int nCurrentLocation);
//	void OnFunction(unsigned short nFunctionID,int nParam);
	void OnItemChange();
    void OnMagiclampComplete();
    void OnMallBuyItem(itemtype_t itemID, int validTime, int nCount);
    void OnClotheEffectReplace(unsigned int count); // ִ����ħ����ע
    void OnClotheEffectLevelUp(unsigned int count); // ִ����ħ������
    void OnUseMoneyMagicArray(unsigned int count); // ִ�н�ȯħ����Ĵ���
    void OnUseCoinMagicArray(unsigned int count); // ִ��M��ħ����Ĵ���

    void OnDanceGroupChange();
    void AddTimerStartQuest();

	void SendQuestData();

	void RemoveCoupleQuest();
	void RemoveDanceQuest();

    void SendQuestVipReward();

public:
	//GM����
	bool AddQuestByCmd(unsigned short nQuestID);
    bool AddQuestByCmdPersistent (unsigned short nQuestID);
	bool CompleteQuestByCmd(unsigned short nQuestID);

private:
	bool _CanAcceptQuest(CQuestDocNew* pDoc,int nParam = 0);	
	bool _CanAcceptQuestRepeat(CQuestDocNew* pDoc);
	bool _CheckCondition(IQuestCondition* pCond,int nParam = 0);
	bool _CheckConditionHasItem(IQuestCondition* pCond);
	bool _CheckConditionItem(IQuestCondition* pCond);
	bool _CheckConditionUseItem(IQuestCondition* pCond,itemtype_t itemID);
	bool _CheckConditionPreQuest(IQuestCondition* pCond);
	void _AcceptQuest(CQuestDocNew* pDoc,/*bool bCleanDayTimes,*/bool bNotifyClient,bool bUpdateDB);
	bool _IsQuestUnAccept(CQuestDocNew* pDoc);

	void _AcceptQuestList(std::vector<unsigned short>& idList,bool bNotifyClient);

	bool _CanCompleteQuest(CQuestDocNew* pDoc,int nParam = 0);
	bool _CheckComplete(IQuestComplete* pComp,int nParam = 0);
	bool _CheckCompleteMusic(IQuestComplete* pComp,QUESTUPDATEINFO* pMusicInfo);		//������ֵ�������
	bool _CheckMusicCompleted(IQuestComplete* pComp,QUESTUPDATEINFO* pMusicInfo);		//���һ�����ֵ�������������������ɣ������ּ�������1
	bool _CheckMusicCompleted(IQuestComplete* pComp);									//���һ������������������֣������ɵ�����ȫ����ɣ�������������
	bool _CheckUseCloth(IQuestComplete* pComp);
	bool _CheckUseItem(IQuestComplete* pComp);
	bool _CheckObtainItem(IQuestComplete* pComp);
    bool _CheckBuyItem(IQuestComplete* pComp);
	bool _CheckCompleteHasItem(IQuestComplete* pCond);
	bool _CheckCompleteOpenComponent(IQuestComplete* pComp);
	void _CompleteQuest(CQuestDocNew* pDoc,bool bNotifyClient = true);
	void _NotifyCompleteQuest(CQuestDocNew* pDoc);
	void _NotifyAcceptQuest(CQuestDocNew* pDoc);
//	void _CheckAllCompleteQuest();
//	bool _CheckTimeType(char timeType);
    // ���������ɴ�������
    bool _CheckTimesLimit(QUESTNEWINFO* questNewInfo, CQuestDocNew* pDoc);

	bool _HasLover();
	bool _HasMarried();
	bool _HasDanceGroup();
	int  _GetDanceGroupContribution();
	int  _GetDanceGroupTotalContribution();
	int  _GetDanceGroupTotalHonor();
	unsigned short _GetDanceGroupLevel();

	unsigned short _AddUseItemTimes(itemtype_t itemID,int nCount,std::vector<unsigned short>& questidList);		//ʹ����Ʒ��������Ʒʹ�ô��� ������ֵΪӰ����������������Ϊ0���ʾ����ʧ��
	unsigned short _AddUseItemTimes(IQuestComplete* pComp,unsigned short nCount,itemtype_t itemID);							//����ֵΪ����ɹ�������ID
	unsigned short _AddMusicTimes(QUESTUPDATEINFO* pMusicInfo,std::vector<unsigned short>& questidList);	//�������ּ���
	unsigned short _AddMusicTimes(IQuestComplete* pComp,QUESTUPDATEINFO* pMusicInfo);
	void _AddMusicTimes(unsigned short nQuestID,unsigned short nMusicID);									//�����һ����������ӵ�������б���
    void _AddUseFunctionCount(unsigned short nQuestID, int nFunctionID);
	unsigned int _GetMusicTimes(unsigned short nQuestID,unsigned short nMusicID);							//ȡ��������ɴ���
    int  _GetUseFunctionTimes(unsigned short nQuestID, int & nParam);
    void _ParseAndInsertMusic(unsigned short nQuestID,std::string& musicInfo);
    void _ParseAndInsertUseItem(unsigned short nQuestID,std::string& itemInfo);
    void _ParseAndInsertFunctions(unsigned short nQuestID,std::string& funcInfo);
    bool _ParseItem(std::string& strInput,unsigned int& id,unsigned short& count);
    int _ParseItem(std::string& strInput,std::vector<CompleteTargetInfo>& m_Result);
    void _AddQuestMusicTarget(list<CQuestTargetSend>& target,std::map<unsigned short,MusicParamMap >& source,unsigned short questid,eTargetType type);
    void _AddQuestFunctionTarget(list<CQuestTargetSend>& target,std::map<unsigned short,FunctionParamMap>& source,unsigned short questid,eTargetType type);
    void _AddQuestUseItemTarget(list<CQuestTargetSend>& target,std::map<unsigned short,UseItemParamMap>& source,unsigned short questid,eTargetType type);

//	void _AddQuestTimer(CQuestDocNew* pDoc);																//Ϊ����ʼʱ��ͽ���ʱ����������һ����ʱ��
	void _NotifyChangeQuestState(unsigned short nQuestID,eQuestState state);

	void _AddAcceptedQuest(QUESTNEWINFO* pQuestInfo);
//	void _AddCompletedQuest(QUESTNEWINFO* pQuestInfo);
	CQuestDocNew* _GetQuestDoc(unsigned short nQuestID);

	//�������������ڴ������������һ�㲻���ڵ�����ѭ���в��������뽫��Ҫɾ�������񵥶���ѯ��������ɾ������
//	bool _DestroyQuest_internal(unsigned short nQuestID,eQuestDestroy type,bool bUpdateDB);
// 	void _DestroyQuest(bool bUpdateDB);
    
	void _AddDestroyQuestTimer(CQuestDocNew *pDoc);
//	void _AddUpdateQuestTimer(unsigned int nTime,unsigned short nQuestID);
// 	bool _DestroyDailyQuest(unsigned short nQuestID,bool bUpdateDB);
	void _CheckCanCompleteQuest();
	void _AddHistoryQuest(unsigned short questID);
	void _CheckCanAcceptQuest(int nParam = 0,bool bNotifyClient = true);
	void _DeleteQuest(unsigned short nQuestID);

//	void _SaveQuest(CQuestDocNew* pDoc);
	void _InsertQuest(CQuestDocNew* pDoc);	//�����������ݿ�
    void _UpdateQuest2DB(unsigned short nQuestID);	//��������
    void _UpdateQuest2DB(QUESTNEWINFO& pQuestInfo);	//��������
	void _InsertNewQuestData(CQuestDocNew* pDoc);
    void _UpdateQuestState(unsigned short nQuestID,unsigned short newState);
	void _UpdateQuestState(unsigned short nQuestID,unsigned short newState/*,bool addcompletetimes,bool adddaytimes*/,unsigned short destroytype);
// 	void _RefreshDailyQuest(bool bUpdateDB);
// 	void _RefreshDailyQuest_internal(CQuestDocNew* pDoc,time_t nNow,std::vector<unsigned short>& canAcceptQuest,std::vector<unsigned short>& unAcceptQuest,bool bUpdateDB);
//	void _CleanDailyQuest(CQuestDocNew* pDoc,QUESTNEWINFO& questInfo,time_t nNow,bool bUpdateDB);

	void OnGetQuestReward(GameMsg_Base &msg, CSlotPeer &slotPeer);
	void OnGetQuestList(GameMsg_Base &msg, CSlotPeer &slotPeer);
	void OnQuestRead(GameMsg_Base &msg, CSlotPeer &slotPeer);
	void OnAcceptQuest(GameMsg_Base &msg, CSlotPeer &slotPeer);
	void OnClientCompleteQuest(GameMsg_Base &msg, CSlotPeer &slotPeer);
	void OnGetQuestState(GameMsg_Base &msg, CSlotPeer &slotPeer);

    void _SendPlayerMsg(GameMsg_Base *pMsg);

	unsigned short _GetTargetNeedTimes(eTargetType type,unsigned short nQuestID,unsigned int nTargetID);

	void _SendPlayerRewardAll(CQuestDocNew* pDoc);
    void _SendPlayerReward(CQuestDocNew* pDoc,unsigned int nExp,unsigned int nMoney,unsigned int nHonor,unsigned int contribution,unsigned int vipExp,unsigned int Intimacy,std::list<CItem>& items , int bindMCoin);
    void _SendPlayerVipReward(CQuestDocNew* pDoc);
	void _SendQuestList();
	void _MakeQuestListEntry(CQuestListEntry& listEntry,unsigned short nQuestID);

	void _OnQuestComplete(unsigned short nQuestID);
    void _CleanOverdueRepeatQuest();
    // CQuestDocNew::IsReAcceptType���Ͷ����ܻ���������
    void RetsetRepeatQuest(unsigned short nQuestID,bool cleanDayTimes, bool toDB);
	void _AddCompleteTimes(QUESTNEWINFO* pInfo);

	void _PushCanCompleteQuest();
	void _SendUnRewardQuest(std::vector<unsigned short>& ids);
	//void _PushGuideQuest();
	int _GetCanRewardQuest(std::vector<unsigned short>& ids);
	std::vector<int>::iterator _FindOpenLocation(int nLoc);
// 	int _FindQuestByFunction(unsigned short nFunctionID,int nParam,std::vector<unsigned short>& questList);
	bool _ExchangeQuestItem(CQuestDocNew* pDoc);

	int	_GetQuestCompleteDayTimes(unsigned short nQuestID);
	void _LogRoleQuest(unsigned int nRoleID, unsigned int nQuestID);
    // �ж������Ƿ��ǿ��ظ�������
//    bool IsReAcceptType(char nType);                    
    // ���뱣֤doc��Ϊ�գ�����Ҳ�����Ϣ���򴴽�һ����ӽ���
    std::map<unsigned short,QUESTNEWINFO>::iterator EnsureGetDetailerInfo(CQuestDocNew *pDoc);

    // �����ڵ����񣬲����
    void CheckOverdueQuest();   
    // ��⵱ǰ��ÿ������
    void CheckCurrentDaily();
    // �����ʷ���񣬰��п��ܽ�ȡ������ȡ����������unaccept.
    void CheckRealHistroy();


    bool CheckQuestStartAndEndTime(CQuestDocNew* pDoc, time_t now);
    bool CheckQuestLastTime(CQuestDocNew* pDoc, time_t now);
    bool HasCompleteTimes(CQuestDocNew* pDoc);
    bool AcceptAble(CQuestDocNew* pDoc);


    void WriteDirtyQuest();
    void WriteDirtyQuest(const QUESTNEWINFO &questInfo);
};

typedef CRoleComponentImpl<CEntityQuestNewComponent,CGetRoleProcessorObj,CGetRoleProcessorObj> CRoleQuestNew;


#endif

