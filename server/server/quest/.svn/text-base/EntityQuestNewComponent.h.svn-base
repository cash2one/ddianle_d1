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
	QuestIDSet	m_UnAcceptQuest;	//还未接受的任务
	QuestIDSet	m_CurrentQuest;		//正在进行中的任务
//	std::vector<unsigned short>				m_DestroyQuest;					//被销毁的任务
    //可以被重新接的任务 这个变量没什么卵用
//    std::map<unsigned short,CQuestDocNew*>	m_CanReAcceptQuest;	
	std::map<unsigned short,QUESTNEWINFO>	m_QuestDetailInfo;	//详细任务数据(数据库对应信息)
	std::map<unsigned short,QUESTNEWINFO>	m_DirtyQuest;	    //需要更新到DB的任务详细数据
    unsigned int m_nDirtyTime;
	
	std::vector<int>						m_QuestLocationList;//在有界面任务的情况下，这个字段描述是否打开了某界面

    typedef std::map<unsigned short,unsigned short > MusicParamMap;
    typedef std::map<itemtype_t,unsigned short > UseItemParamMap;
    typedef std::map<int,unsigned short > FunctionParamMap;
	std::map<unsigned short, MusicParamMap> m_MusicCount;		//舞曲完成次数，字段含义为（任务id，（音乐id，完成次数））
	std::map<unsigned short, UseItemParamMap> m_ItemUseCount;		//物品使用次数，字段含义为（任务id，（使用物品id，完成次数））
	std::map<unsigned short, FunctionParamMap> m_UseFunctionCount;	//功能使用次数，字段含义为（任务id，（使用功能类型，完成次数））

    QuestRecordManager m_recordManager;

	//unsigned short							m_GuideQuestID;
	bool									m_bSendQuestDataToClient;			//是否已经发送过任务列表
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

	//外部调用接口
public:
	void OnAttributeChange();			//角色属性改变，包括等级，好友，情侣，结婚，亲密度，vip，加入舞团，贡献度，荣誉，穿戴服饰，获得道具，任务完成
	void OnUseItem(itemtype_t itemID,int nCount);
	void OnUseItem(itemtype_t itemID);		//使用道具，可触发或完成需要使用道具的任务
	void OnMusicEnd(QUESTUPDATEINFO *pQuestUpdateInfo);			//音乐结束，可完成舞曲类任务
	void OnOpenComponent(unsigned short nComponentID);
	void OnChangeLocation(int nCurrentLocation);
//	void OnFunction(unsigned short nFunctionID,int nParam);
	void OnItemChange();
    void OnMagiclampComplete();
    void OnMallBuyItem(itemtype_t itemID, int validTime, int nCount);
    void OnClotheEffectReplace(unsigned int count); // 执行了魔法灌注
    void OnClotheEffectLevelUp(unsigned int count); // 执行了魔法升级
    void OnUseMoneyMagicArray(unsigned int count); // 执行金券魔法阵的次数
    void OnUseCoinMagicArray(unsigned int count); // 执行M币魔法阵的次数

    void OnDanceGroupChange();
    void AddTimerStartQuest();

	void SendQuestData();

	void RemoveCoupleQuest();
	void RemoveDanceQuest();

    void SendQuestVipReward();

public:
	//GM命令
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
	bool _CheckCompleteMusic(IQuestComplete* pComp,QUESTUPDATEINFO* pMusicInfo);		//检查音乐的子条件
	bool _CheckMusicCompleted(IQuestComplete* pComp,QUESTUPDATEINFO* pMusicInfo);		//检查一首音乐的所有完成条件，如果完成，则音乐计数器加1
	bool _CheckMusicCompleted(IQuestComplete* pComp);									//检查一个任务包含的所有音乐，如果达成的数量全部完成，则可以完成任务
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
    // 检测任务完成次数限制
    bool _CheckTimesLimit(QUESTNEWINFO* questNewInfo, CQuestDocNew* pDoc);

	bool _HasLover();
	bool _HasMarried();
	bool _HasDanceGroup();
	int  _GetDanceGroupContribution();
	int  _GetDanceGroupTotalContribution();
	int  _GetDanceGroupTotalHonor();
	unsigned short _GetDanceGroupLevel();

	unsigned short _AddUseItemTimes(itemtype_t itemID,int nCount,std::vector<unsigned short>& questidList);		//使用物品后增加物品使用次数 ，返回值为影响的任务数量，如果为0则表示插入失败
	unsigned short _AddUseItemTimes(IQuestComplete* pComp,unsigned short nCount,itemtype_t itemID);							//返回值为插入成功的任务ID
	unsigned short _AddMusicTimes(QUESTUPDATEINFO* pMusicInfo,std::vector<unsigned short>& questidList);	//增加音乐计数
	unsigned short _AddMusicTimes(IQuestComplete* pComp,QUESTUPDATEINFO* pMusicInfo);
	void _AddMusicTimes(unsigned short nQuestID,unsigned short nMusicID);									//完成了一次舞曲，添加到已完成列表中
    void _AddUseFunctionCount(unsigned short nQuestID, int nFunctionID);
	unsigned int _GetMusicTimes(unsigned short nQuestID,unsigned short nMusicID);							//取得舞曲完成次数
    int  _GetUseFunctionTimes(unsigned short nQuestID, int & nParam);
    void _ParseAndInsertMusic(unsigned short nQuestID,std::string& musicInfo);
    void _ParseAndInsertUseItem(unsigned short nQuestID,std::string& itemInfo);
    void _ParseAndInsertFunctions(unsigned short nQuestID,std::string& funcInfo);
    bool _ParseItem(std::string& strInput,unsigned int& id,unsigned short& count);
    int _ParseItem(std::string& strInput,std::vector<CompleteTargetInfo>& m_Result);
    void _AddQuestMusicTarget(list<CQuestTargetSend>& target,std::map<unsigned short,MusicParamMap >& source,unsigned short questid,eTargetType type);
    void _AddQuestFunctionTarget(list<CQuestTargetSend>& target,std::map<unsigned short,FunctionParamMap>& source,unsigned short questid,eTargetType type);
    void _AddQuestUseItemTarget(list<CQuestTargetSend>& target,std::map<unsigned short,UseItemParamMap>& source,unsigned short questid,eTargetType type);

//	void _AddQuestTimer(CQuestDocNew* pDoc);																//为有起始时间和结束时间的任务添加一个计时器
	void _NotifyChangeQuestState(unsigned short nQuestID,eQuestState state);

	void _AddAcceptedQuest(QUESTNEWINFO* pQuestInfo);
//	void _AddCompletedQuest(QUESTNEWINFO* pQuestInfo);
	CQuestDocNew* _GetQuestDoc(unsigned short nQuestID);

	//这个函数会操作内存迭代器，所以一般不能在迭代器循环中操作，必须将需要删除的任务单独查询出来再做删除处理
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
	void _InsertQuest(CQuestDocNew* pDoc);	//保存任务到数据库
    void _UpdateQuest2DB(unsigned short nQuestID);	//更新任务
    void _UpdateQuest2DB(QUESTNEWINFO& pQuestInfo);	//更新任务
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
    // CQuestDocNew::IsReAcceptType类型都可能会清理数据
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
    // 判断类型是否是可重复接类型
//    bool IsReAcceptType(char nType);                    
    // 必须保证doc不为空，如果找不到信息，则创建一个添加进入
    std::map<unsigned short,QUESTNEWINFO>::iterator EnsureGetDetailerInfo(CQuestDocNew *pDoc);

    // 检测过期的任务，并清除
    void CheckOverdueQuest();   
    // 检测当前的每日任务：
    void CheckCurrentDaily();
    // 检测历史任务，把有可能接取的任务取出来，放入unaccept.
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

