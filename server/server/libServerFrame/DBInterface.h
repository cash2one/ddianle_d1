/********************************************************************
created:	2010/10/13
author:		mei jiuhua
FileName:   DBInterface.h
Description:project database interface for mysql
*********************************************************************/

#ifndef __DATABASE_INTERFACE_H__
#define __DATABASE_INTERFACE_H__

#ifndef OUT
#define OUT
#endif

#include "../../socket/DBIMysql.h"
#include "../../socket/Utility.h"
#include "../../datastructure/DataStruct_DB.h"
#include "../../datastructure/DataStruct_DanceGroup.h"
#include "../../datastructure/DataStruct_Couple.h"
#include "../../datastructure/DataStruct_Rank.h"
#include "../../datastructure/LogStructDef.h"
#include "../../datastructure/datainfor/GMDataInfo.h"
#include "../CeremonyRoom/ceremonyRoomHead.h"
#include "../activity/RedEnvelope/RedEnvelope.h"
#include "../dancegroup/DataStruct_BigMama.h"

#include <map>
#include <set>
using namespace std;

class Pet;
class PetEgg;
class CMedalScore;

//查询返回值的含义
const int Query_Res_OK = 0;//查询成功
const int Query_Res_Fail = -1;//失败
const int Query_Res_Empty = 1;//没有查到符合条件的结果
const int Query_Res_AlreadyExist = 2;//已经存在
const int Query_Res_InvalidParam = 3;//非预期的参数
const int Query_Res_RoleNameAlreadyExist = 4;//非预期的参数

struct InfoCenterItem;
typedef std::map<unsigned int, InfoCenterItem*> InfoCenterPtrMap;

class CDBInterface : public CDBIMysql
{
public:
    CDBInterface();
    ~CDBInterface();
public:
    CFormatString strFmt;
public:
    //得到注册角色数量
    int GetRegistedRoleCount(int& nCount);
    //得到注册玩家数量
    int GetRegistedPlayerCount(int& nCount);
    // 得到一批64位ID值
    int GetBatchID64(__int64* pID);

    //创建账号
    int CheckAndCreateAccount(LPACCOUNTINFO pInfo);
    int ExistAccountName(LPACCOUNTINFO pInfo); // 不存在返回Query_Res_OK，否则返回Query_Res_Fail
    int CreateAccount(LPACCOUNTINFO pInfo);
    int CreateAccountBill(LPACCOUNTINFO pInfo);
    // 得到账号信息,如果账号不存在，并且是平台账号，则创建该账户
    int GetAccountInfo(LPACCOUNTINFO pInfo, bool &canCreate);
    // 获得账号ID
    int GetAccountID(unsigned short nPID, std::string strPUID, unsigned int & rnAccountID);
    //更新玩家的电话
    int UpdatePlayerPhone(unsigned int nAccount, std::string strNewPhone, std::string strOldPhone);
    //更新账号状态
    int UpdateAccountState(LPACCOUNTINFO pInfo);
    //更新账号锁定状态
    int UpdateAccountLockState(LockAccountInfo* pInfo);
    //重置账号状态，nAccount为０时重置所有账号
    int ResetAccountState(unsigned int nAccount = 0, int nStateLogin = 0);
    //账号登出
    int AccountLogout(unsigned int nAccount, unsigned int nLoginTime);

    int CreateRole(CreateRoleInfo *pInfo, int &rError);
    int CheckRoleName(string &strRoleName);	// 角色昵称是否已经存在

    //读取预注册的可用的快速账号
    int LoadAutoAccout(list<CAutoAccount *> & listAutoAccount);
    int UpdateAutoAccoutState(unsigned int nAccount, int nState);
    int CreateNewRandomAccount(list<CAutoAccount *> &listAutoAccount, int nCount);

    //读取激活码
    int LoadActivatedCode(map<unsigned int, unsigned char > & mapActivateCode);
    int ActivateCode(unsigned int nCode, AccountInfo *pInfo);

    //通过账户名称得到账号id
    unsigned int GetAccountByAccountName(char *strAccountName, int nPID);

    //保存和得到角色的数据,nMode的bit
    static const int MODE_RoleAll = 0x00000000;
    static const int MODE_RoleAttr = 0x00000001;
    static const int MODE_RoleFriend = 0x00000008;
    static const int MODE_RoleMusic = 0x00000010;
    static const int MODE_RoleSetting = 0x00000040;
    static const int MODE_RoleScene = 0x00000100;
    static const int MODE_RoleQuestReward = 0x00000200;
    static const int MODE_RoleRelation = 0x00000400;
    static const int MODE_RoleAlbum = 0x00000800;
    static const int MODE_RoleVIP = 0x00004000;
    static const int MODE_RoleEvent = 0x00008000;
    static const int MODE_MoneyReset = 0x00020000;
    static const int MODE_Mail = 0x00040000;
    static const int MODE_Couple = 0x00100000;
    static const int MODE_Dungeon = 0x00800000;
    static const int MODE_SpecialMall = 0x04000000;
    static const int MODE_LongactingRecharge = 0x20000000;

    int UpdateRole(LPROLEINFO_DB pRole, int nMode = MODE_RoleAll);
    int GetRoleInfo(LPROLEINFO_DB pRole, int nMode = MODE_RoleAll);
    int UpdateRolenName(unsigned int nRoleid, char * newrolename);
    int UpdateRoleMoney(unsigned int nRoleID, int nMoney);
    int UpdateRoleMoneyByAccountId(unsigned int nAccountId, int nMoney, unsigned int& nRoleID, unsigned int& nOldMoney);
    int UpdateRoleBadgeNum(unsigned int nRoleID, unsigned char nNum);
    int UpdateRoleSignature(unsigned int nRoleID, const char *szWords);
    int GetRoleIDByAccountID(unsigned int nAccount, unsigned int& nRoleID);
    int UpdateRoleBirthday(unsigned int nRoleID, const std::string *pStrNewBirthday);
    int GetRoleOfflineTime(unsigned int nRoleID, unsigned int * pOfflineTime);

    //item
    int LoadRoleItem(LPROLEINFO_DB pRole);

    int UpdateBagExpand(unsigned int nRoleID, char* BagExpand);
    int LoadRoleOfflineData(unsigned int nRoleID, RoleImageOfflineData * offLineData);
    int LoadRoleOfflineData(const std::set<unsigned int> & setRoleID, const std::map<unsigned int, RoleImageOfflineData*> & mapOfflineData);
    int AddRoleItem(unsigned int nRoleID, DBItem* pItem);
    int UpdateRoleItem(unsigned int nRoleID, DBItem* pItem);
    int RemoveRoleItem(__int64 nItemID, unsigned int nDeleteTime);
    int RemoveRoleItemByType(unsigned int nRoleID, unsigned int nTypeID, int nCol);
    int RemoveRoleDynamicBoxRelated(unsigned int nRoleID, __int64 nitemID);
    int UpdateItemCount(unsigned short nCount, __int64 nItemID);
    int UpdateItemPosition(ITEMINFO_Position* pItem);
    int UpdateItemPosition2(ITEMINFO_Position2* pItem);
    int UpdateItemOwner(unsigned int nRoleID, __int64 nItemID);
    int GetActivateGiftInfo(CActivateGiftInfo *pGiftInfo);
    int ActivateGift(unsigned int nRoleID, CActivateGiftInfo *pInfo);
    int AddRoleClothGroup( unsigned int nRoleID, RoleClothGroup *pRoleClothGroup );
    int UpdateRoleClothGroup( unsigned int nRoleID, RoleClothGroup* pRoleClothGroup, bool bUpdateItems );

    // 衣服特效
    int LoadClothEffect(LPROLEINFO_DB pRole);
    int LoadRoleEffect(LPROLEINFO_DB pRole);
    int LoadHandbookProgress(LPROLEINFO_DB pRole);
    int LoadColorProgress(LPROLEINFO_DB pRole);
    int InsertClothEffect(unsigned int nRoleID, itemid_t nItemID, unsigned int nEffectID);
    int UpdateClothEffect(unsigned int nRoleID, itemid_t nItemID, unsigned int nEffectID);
    int InsertRoleEffect(unsigned int nRoleID, unsigned int nEffectID);
    int InsertHandbookProgress(unsigned int nRoleID, ClothEffectHandbookDB * pHandbookProgress);
    int UpdateHandbookProgress(unsigned int nRoleID, ClothEffectHandbookDB * pHandbookProgress);
    int InsertColorProgress(unsigned int nRoleID, ClothEffectHandbookDB * pColorProgress);
    int UpdateColorProgress(unsigned int nRoleID, ClothEffectHandbookDB * pColorProgress);

    // 仓库
    int LoadStorageCapacity(LPROLEINFO_DB pRole);
    int InsertStorageCapacity(unsigned int nRoleID, const StorageCapacityRecord & record);
    int UpdateStorageCapacity(unsigned int nRoleID, const StorageCapacityRecord & record);

    //cooldown
    int UpdateCoolDown(unsigned int nRoleID, CoolDownData & CoolDown);
    int SaveRoleCooldown(LPROLEINFO_DB pRole);
    int LoadRoleCooldown(LPROLEINFO_DB pRole);

    //gene
    int LoadRoleGene(LPROLEINFO_DB pRole);
    int AddRoleGene(unsigned int nRoleID, GeneData* pGeneData);
    int DeleteRoleGene(unsigned int nRoleID, GeneData* pGeneData);
    int UpdateRoleGene(unsigned int nRoleID, GeneData* pGeneData);

    //命令
    int LoadCommands(list< CMDINFO > *pCmdList);

    //计费相关
    int LoadBillInfo(BILLINFO* pBillInfo);//读取计费信息
    int UpdateBillPt(ChongZhiInfo * pChongZhiInfo);//充值
    //int LoadLastLogoutTime(unsigned int nAccount, unsigned int & nLastLogoutTime);

    int UpdateBillPt(unsigned int nAccount, int nPt);        //更改M币
    int UpdateBillBindPt(unsigned int nAccount, int nBindPt);//更改绑定M币
    int UpdateBillChange(unsigned int nAccount, int nPoint);
    int SaveChargeAction(const ChargeAction & action);

    //商城
    int LoadMallInfo(list<MallGood*>* goodslist, int &nVersion);
    int SaveMallLog(MALL_LOG* log);
    int SaveSingleItemLog(const std::vector<MallSigleItemLog> & malllog);
    int SaveMallDesireNum(unsigned int nType, unsigned int nAddNum);
    char * GetSQL(){
        return sql;
    };

    // 体验卡：
    int LoadExperiencedProduct(LPROLEINFO_DB pRole);
    int InsertExperiencedProduct(CExperiencedCardProduct &prod);
    int LoadExperiencedTime(LPROLEINFO_DB pRole);
    int UpdateExperiencedTime(CExperiencedCardUseNum &useNum);

    // Friends
    int GetBeAppliedPlayerInfo(FriendInfo & beAppliedRole);
    int AddBlackListFriend(unsigned int nRoleID, FriendInfo & friendInfo);
    int AddRoleFriend(unsigned int nRoleID, unsigned int nFriendID, unsigned int nType);
    int RemoveFriend(unsigned int nRoleID, unsigned int nFriendID);
    int AddBlackList(unsigned int nRoleID, FriendInfo &friendInfo);
    int RemoveBlackList(unsigned int nRoleID, unsigned int nFriendID);
    int _BecomeFriend(unsigned int selfID, unsigned int nFriendID, int nRelationType, int nCallBy, int nCallTo);
    int BeceomFriend(unsigned int nSelfID, unsigned int nFriendID, FriendInfo *pFriendInfo);
    int RemoveRoleFriend(unsigned int nRoleID, unsigned int nFriendID);
    int UpdateRoleFriend(unsigned int nRoleID, unsigned int nFriendID, unsigned char nFriendType, int nFriendCalling);
    void LoadRoleFriend(LPROLEINFO_DB pRole);
    int LoadRoleCompensation(LPROLEINFO_DB pRole);
    int LoadRoleQuestNew(LPROLEINFO_DB pRole);
    int ReworkFriendNotes(unsigned int nRoleID, unsigned int nFriendID, const std::string & notes);
    //int LoadFriendInfo(list<FriendInfo>& listFriendInfo);
    int LoadRoleClothGroup(LPROLEINFO_DB pRole);
    int LoadFriendOfflineTime(unsigned int nRoleID, std::map<unsigned int, unsigned int*> * pFriendInfo);

    //任务
    //int LoadRoleQuest(LPROLEINFO_DB pRole);
    //int LoadRepeatedQuest(unsigned int nRoleID, list<CRepeatedHistoryQuest> &listHistory);
    //int LoadQuestHistory(unsigned int nRoleID, map<short, __int64> & mapQuestHistory);

    //	int LoadQuestTarget(QUESTINFO *pQuest, unsigned int nRoleID, unsigned short nQeustID);

    int AddRoleQuest(unsigned int nRoleID, QUESTINFO* pQuest);
    int AddQuestTarget(unsigned int nRoleID, unsigned short nQuestID, TargetInfo &target);

    int UpdateQuestInfo(unsigned int nRoleID, QUESTINFO* pQuest);										              // timeleft可以不记录，验证完成与否靠最终验证来确定，该字段暂时保留
    int UpdateTargetInfo(unsigned int nRoleID, unsigned short nQuestID, map<unsigned char, list<TargetInfo> > &mapTarget);  // 可以再细分update各种不同目标
    int UpdateQuestTargetEntry(unsigned int nRoleID, unsigned short nQuestID, TargetInfo *pTarget);
    int UpdateRoleQuest(unsigned int nRoleID, QUESTINFO* pQuest);
    int RemoveRoleQuest(unsigned int nRoleID, unsigned short nQuestID);												  // 连同target一同删除，删除任务的时机是???

    int AddRoleQuestHistory(unsigned int nRoleID, short nIndex, __int64 nRecord);
    int UpdateRoleQuestHistory(unsigned int nRoleID, short nIndex, __int64 nRecord);
    int RemoveRoleQuestHistory(unsigned int nRoleID, unsigned short nQuestID);

    int AddRepeatedQuestDone(unsigned int nRoleID, CRepeatedHistoryQuest & info);
    int UpdateRepeatedQuestDone(unsigned nRoleID, CRepeatedHistoryQuest & info);

    //mail
    int LoadRoleMail(unsigned int nRoleID, mapXQMail * pXQMapIn);
    int LoadRoleMail(LPROLEINFO_DB pRole);
    //int LoadMailItem(MailInfo* pInfo);
    int GetAllMailItem(__int64 nMailID);
    int GetMailMoney(__int64 nMailID);
    int AddMail(MailInfo * pMail, unsigned int nReceiverID);
    int ReadMail(__int64 nMailID);
    int DeleteMail(__int64 nMailID, unsigned int nDeleteTime);
    int MailGetRoleID(MailInfoDB * pMail_db);
    int LoadMailStrangerSetting(std::vector<unsigned int> vecNotReceiveRole);
    int UpdateMailStrangerSetting(unsigned int nRoleID, bool bReceive);


    // 事件
    // 完成新手引导
    //void LoadEvent(LPROLEINFO_DB pRole);
    int LoadCompleteTeachGuide(unsigned int nRoleID, bool &bCompleteTeachGuide);
    int LoadCompleteProcGuide(unsigned int nRoleID, bool &bCompleteProcGuide);

    int LoadFinishFirstPaymentInfo(unsigned int nRoleID, bool &bFristPaymentTag, bool &bCanSendFirstPaymentReward, int nVersionType);
    int LoadFirstPaymentInfo(unsigned int nRoleID, int & nCount, int & rRechargeTime, int nVersionType);
    int LoadFirstPaymentRewardInfo(unsigned int nRoleID, int & nCount, int & rRechargeTime, int nVersionType);
    int LoadFirstPaymentRewardInfoByAccountId(RechargeRewardInfo& info);
    int AddFirstPaymentInfo(unsigned int nAccount, unsigned int nCount);
    int AddFirstPaymentInfoFeed(unsigned int nAccount, unsigned int nCount);
    int AddSendFirstPaymentReward(unsigned int nAccount, unsigned int nCount);
    int AddSendFirstPaymentRewardFeed(unsigned int nAccount, unsigned int nCount);
    int LoadEventRecord(EventInfoDB& eve);
    int LoadFixedChargeReward(unsigned int account, unsigned int amount, RechargeRewardInfo* pRechargeRewardInfo);
    int UpdateFixedChargeReward(unsigned int account, unsigned int amount, unsigned int stamp);
    // 加载一个玩家的所有固定首冲数据
    int LoadFixedChargeRewardAll(unsigned int account, FixedChargeRecord &record);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //									  成就模块            													   //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void UpdateAchievementData(LPROLEINFO_DB pRole);
    // 称号
    int AddTitle(unsigned int nRoleID, CTitleInfo * pTitle);
    // 成就
    int AddAchievement(unsigned int nRoleID, CAchievementInfo *pAchievement);
    int LoadAchievement(LPROLEINFO_DB	pRole);
    int UpdateAchievementState(unsigned int nRoleID, unsigned int nAchievementID, unsigned char nState);
    // 战斗成绩
    void UpdateRoleFightResult(ROLEINFO_DB * pRole);
    int AddFightResult(unsigned int nRoleID, CFightResult &fightResult);
    int UpdateFightResult(unsigned int nRoleID, CFightResult& result);

    // 事件相关
    int AddEventEntry(unsigned int nRoleID, unsigned char nEventID, unsigned int nCount);
    int UpdateEventEntry(unsigned nRoleID, unsigned char nEventID, unsigned int nCount);
    int UpdateEventEntry4(unsigned nRoleID, unsigned char nEventID, unsigned nCount, unsigned int nTime);

    // 信息中心
    int InfoCenterLoadAllInfo(InfoCenterPtrMap &ptrMap);
    int InfoCenterAddInfo(InfoCenterItem &item); // 
    int InfoCenterUpdateInfo(InfoCenterItem &item); // 
    int InfoCenterRemoveInfo(unsigned int id); // 
    int InfoCenterUpdateRoleReadInfo(unsigned int roleID, unsigned int infoID, unsigned int uptime); // 
    int InfoCenterLoadRoleReaded(unsigned int roleID, std::vector<std::pair<unsigned int, unsigned int> > &vecReaded);


    // 大V上线全服公告
    int UpdateVIPOnlineAnnounce(unsigned int nRoleID, unsigned int nTime);

    // 太鼓模式下累计全连任意歌曲的次数
    int AddHistoryTaikoComboMusicCountEntry(unsigned int nRoleID, unsigned int nCount);
    int UpdateHistoryTaikoComboMusicCountEntry(unsigned int nRoleID, unsigned int nCount);
    // 愿望
    int AddHistoryWishItemCountEntry(unsigned int nRoleID, unsigned int nCount);
    int UpdateHistoryWishItemCountEntry(unsigned int nRoleID, unsigned int nCount);
    // 实现愿望
    int AddHistoryGainWishItemCountEntry(unsigned int nRoleID, unsigned int nCount);
    int UpdateHistoryGainWishItemCountEntry(unsigned int nRoleID, unsigned int nCount);
    // 使用花束的数量
    int AddHistoryUseBouquetCountEntry(unsigned int nRoleID, unsigned int nCount);
    int UpdateHistoryUseBouquetCountEntry(unsigned int nRoleID, unsigned int nCount);
    // 累计拥有任意服饰的数量
    int AddHistoryClothCount(unsigned int nRoleID, unsigned int nCount);
    int UpdateHistoryClothCount(unsigned nRoleID, unsigned int nCount);
    // 累计拥有的徽章数量
    int AddHistoryBadageCount(unsigned nRoleID, unsigned int nCount);
    int UpdateHistoryBadageCount(unsigned nRoleID, unsigned int nCount);
    // 与异性通过快速匹配游戏的异性人数
    int AddHistoryAutoMatchWithOppositeSexCountEntry(unsigned int nRoleID, unsigned int nCount);
    int UpdateHistoryAutoMatchWithOppositeSexCountEntry(unsigned int nRoleID, unsigned int nCount);
    // 周相关的数据
    int AddHistoryWeeklyEventEntry(unsigned int nRoleID, unsigned char nEventID);
    int UpdateHistoryWeeklyEventEntry(unsigned int nRoleID, unsigned char nEventID, unsigned int nCount, unsigned int nTime);
    // 一周内使用的小喇叭数量	
    int AddHistorySmallSpeakersUseCountInSevenDays(unsigned int nRoleID);
    int UpdateHistorySmallSpeakersUseCountInSevenDays(unsigned int nRoleID, unsigned int nCount, unsigned int nTime);
    // 一周内使用的大喇叭数量
    int AddHistoryBigSpeakersUseCountInSevenDays(unsigned int nRoleID);
    int UpdateHistoryBigSpeakersUseCountInSevenDays(unsigned int nRoleID, unsigned int nCount, unsigned int nTime);
    // 一周内鲜花的次数
    int AddHistorySendFlowerCountInSevenDays(unsigned int nRoleID);
    int UpdaetHistorySendFlowerCountInSevenDays(unsigned int nRoleID, unsigned int nCount, unsigned int nTime);
    // 事件相关数据
    int LoadEvent(LPROLEINFO_DB pRole);
    //int LoadHistoryClothCount(LPROLEINFO_DB pRole);
    //int LoadHistoryWishItemCount(LPROLEINFO_DB pRole);
    //int LoadGainWishItemCount(LPROLEINFO_DB pRole);
    //int LoadHistoryUseBouquetCount(LPROLEINFO_DB pRole);
    //int LoadHistoryBadageCount(LPROLEINFO_DB pRole);
    //// 周相关
    //int LoadSmallSpeakersInSevenDays(LPROLEINFO_DB pRole);
    //int LoadBigSpeakersInSevenDays(LPROLEINFO_DB pRole);
    //int LoadFlowersSendInSevenDays(LPROLEINFO_DB pRole);
    // 成就与战斗相关
    //int LoadFightResult(LPROLEINFO_DB pRole);
    //int LoadHistoryTaikoComboMusicCount(LPROLEINFO_DB pRole);		// 太鼓模式下累计全连任意歌曲的次数
    int LoadHistoryFightResult(LPROLEINFO_DB pRole);
    //int LoadHistoryAutoMatchWithOppositeSexCount(LPROLEINFO_DB pRole);
    // 称号
    int LoadTitle(LPROLEINFO_DB pRole);
    // 签到
    int LoadChekIn(LPROLEINFO_DB pRole);
    int AddCheckInGift(unsigned int nRoleID, unsigned char nPacketType);
    int RemoveCheckInGift(unsigned int nRoleID, unsigned char nPacketType);
    //int LoadCheckInGift(LPROLEINFO_DB pRole);
    int InsertCheckIn(unsigned int nRoleID, CCheckInInfo *pInfo);
    int UpdateCheckIn(unsigned int nRoleID, CCheckInInfo *pInfo);

    // VIP 
    int SaveVIPInfo(unsigned int nRoleID, CRoleVIPInfo &VIPInfo);
    int LoadVIPInfo(unsigned int nRoleID, CRoleVIPInfo &VIPInfo);
    int SaveVIPConsumedPt(unsigned int nRoleID, int nConsumedPt);

    void LoadVIP(LPROLEINFO_DB pRole);

    // money
    int LoadMoneyResetInfo(LPROLEINFO_DB pRole);
    int ResetMoney(unsigned int nRoleID, unsigned int nResetTime);
    int UpdateMoneyResetTime(unsigned int nRoleID, unsigned int nResetTime);
    int UpdateMoneyMailRemindTime(unsigned int nRoleID, unsigned int nRemindTime);
    int UpdateMoneyMallRemindTime(unsigned int nRoleID, unsigned int nRemindTime);

    //在线人数
    int AddOnlineState(int num);

    // keep online reward
    int LoadKeepOnlineRewardInfo(LPROLEINFO_DB pRole);
    int UpdateKeepOnlineRewardInfo(unsigned int nRoleID, CKeepOnlineReward & info);

    // GM
    int GetRcvGMSysRoleInfo(list<CRcvSysMailRoleInfo> & listRoleInfo);
    int QueryGiftByTimeRange(unsigned short nGiftID, int & nCount, string & strStartTime, string & strEndTime);
    int GetGiftCodesState(list<CQueryGiftState> & listQueryCodes);
    int GMPrivSendMCoin(std::vector<PlayerMoneyInfo>* pPlayerMoneyInfo, std::vector<GMRechargeResult>* pPlayerResultList);

    //log
    int LogSQLLog(ISQLLog & rSQLLog);
    //gift
    int LoadGift(int nAccountID, std::map< int, GiftDB > & rGiftMap);
    int GetTheGiftReward(int nIndexID);

    //累计充值奖励
    int SaveCumulativeRecharge(unsigned int nRoleID, unsigned int nLastReceivedTime, std::vector<int> & rvecReward);
    int LoadCumulativeRechargeInfo(unsigned int nAccount, CCumulativeRechargeDB & rRechargeInfo);
    int LoadCumulativeRechargeReward(unsigned int nRoleID, unsigned int nRechargeStartTime, unsigned int nRechargeStopTime, std::vector<int> & rVecReward);

    //累计消费
    int LoadCumulativeSpendReward(unsigned int nRoleID, std::vector<unsigned int> & rewardvec);
    int LoadCumulativeSpendInfo(unsigned int nRoleID, CCumulativeSpendDB & spendinfo);
    int UpdateCumulativeSpendReward(unsigned int nRoleID, int nRewardIndex);
    int InsertCumulativeSpendReward(unsigned int nRoleID, int nRewardIndex);
    int InsertCumulativeSpendBill(unsigned int nRoleID, int nValue);
    int InsertCumulativeSpendBindBill(unsigned int nRoleID, int nValue);

    // 累计消费获取勋章
    int LoadCumulativeSpendGotMedalReward(const CumulativeSpendGotMedalDB &rInfo, std::vector<bool> &rGotReward);
    int InsertCumulativeSpendGotMedalReward(unsigned int nRoleID, unsigned char nIndex);

    // 购买物品获取勋章
    int LoadBuyItemGotMedalRewardedInfo(const BuyItemGotMedalDB &rInfo, BuyItemGotMedalRewardedInfoMap &rRewardedInfo);
    int SetBuyItemGotMedalRewarded(const BuyItemGotMedalDB &rInfo);
    int SaveBuyItemGotMedalInfo(bool bInsert, const BuyItemGotMedalDB &rInfo);

    //圣诞活动-免费领取
    int SaveXmasFreeRewardInfo(unsigned int nRoleID, unsigned int nLastGetRewardTime, std::vector<int> & rvecReward);
    int LoadXmaxFreeRewardInfo(CXmasFreeRewardDB &rXmasFreeRewardInfo);

    //生日许愿活动
    int SaveFestivalWishInfo(CFestivalWishInfoDB & rWishInfoDB);
    int LoadFestivalWishInfo(CFestivalWishInfoDB & rWishInfoDB);

    //节日祝福活动
    int AddBless(CFestivalBlessInfoDB & blessEntryDB);
    int SaveRoleBlessActivityInfo(unsigned int nRoleID, vector<int> &vecReward, unsigned int nLastGetRewardTime);
    int LoadRoleBlessActivityInfo(CRoleBlessActivityDB *pBlessInfo);

    //加载所有的舞团信息
    int LoadAllDanceGroup(CDanceGroupManager & rDGMgr);
    //创建舞团
    int CreateDanceGroup(CDanceGroupInfo & rDGInfo, int & rnResult);
    //移除舞团
    int RemoveDanceGroup(unsigned int nGroupID);
    //更新舞团基本信息
    int UpdateDanceGroupInfo(CDanceGroupBaseInfo & rDGBaseInfo);
    //添加舞团成员
    int AddDanceGroupMember(unsigned int nGroupID, unsigned int nRoleID, unsigned char nTitle, unsigned char nProperty, unsigned int nContribution, unsigned int nInTime);
    //更新舞团成员信息
    int UpdateDanceGroupMember(unsigned int nGoupId, unsigned int nRoleId, CDanceGroupMember *pMember);
    int UpdateDanceGroupMemberWithTime(unsigned int nGoupId, unsigned int nRoleId, CDanceGroupMember *pMember);
    //删除舞团成员
    int DelDanceGroupMember(unsigned int nGroupID, unsigned int nRoleID);
    //更新舞团申请信息
    int UpdateRequestDanceGroup(unsigned int nGroupID, unsigned int nRoleID, unsigned int nExpiredTime);
    //删除舞团申请信息
    int RemoveRequestDanceGroup(unsigned int nGroupID, const std::list<unsigned int> *pListRoleID);
    //修改舞团名称
    int ChangeDanceGroupName(unsigned int nGroupID, std::string & rstrNewName, unsigned int nChangeDanceGroupNameTime, int & rnResult);
    //更新舞团活动信息
    int UpdateDanceGroupActivityInfo(unsigned int nGroupId, CDanceGroupActivityInfo& rDGActivityInfo);
    //添加舞团记录
    int AddDanceGroupRecord(unsigned int nGoupid, CDanceGroupRecordsInfo& rRecordInfo);
    // 退团信息写入：
    int UpdateDanceGroupQuitInfo(unsigned int nRoleId, unsigned int nDanceGroupID, unsigned int stamp);
    // 入团仪式加载日志
    int CeremonyLoadLog(DanceGroupCeremonyLogMap &logMap);
    int CeremonyLoadPraiseInfo(unsigned int logID, DanceGroupCeremonyLog &roomLog);
    // 入团仪式写入日志,记得要填充ID。
    int CeremonyWriteLog(DanceGroupCeremonyLog &groupLog, unsigned int nGroupID);
    // 舞团大妈Group数据
    int LoadBigMamaGroupInfo(DanceGroupBigMamaMap &bigMamaMap);
    int UpdateOrInsertBigMamaGroupInfo(const DanceGroupBigMamaInfo &dbmi);
    int LoadBigMamaRoleInfo(unsigned int nRoleID, unsigned int &nPrizeStamp, unsigned int &nJoinStamp);
    int UpdateOrInsertBigMamaRoleInfo(unsigned int nRoleID, unsigned int nPrizeStamp, unsigned int nJoinStamp);

    // 舞团试炼
    int DanceGroupChallengeLoadRanking(unsigned int nRoleID, unsigned int nStartTime, std::map<int, DanceGroupChallengeRanking> &rRanking);
    int DanceGroupChallengeUpdateRankingState(unsigned int nRoleID, unsigned int nStartTime, std::map<int, DanceGroupChallengeRanking> &rRanking);
    int DanceGroupChallengeRecordRanking(const DanceGroupChallengeRecordRankingInfo &rRanking);
    int DanceGroupChallengeLoadAllInfo(unsigned int nStartTime, std::map<unsigned int, DanceGroupChallengeInfo *> &rmapInfo);
    int DanceGroupChallengeUpdateActive(unsigned int nGroupID, unsigned int nActive);
    int DanceGroupChallengeUpdateContribution(const DanceGroupChallengeUpdateRoleContribution &rInfo);
    int DanceGroupChallengeOpenStorey(unsigned int nGroupID, unsigned int nStartTime, const DanceGroupChallengeOpendStoreyInfo &rOpendInfo);
    int DanceGroupChallengeUpdateStoreyScore(unsigned int nGroupID, unsigned int nStartTime, const DanceGroupChallengeOpendStoreyInfo &rOpendInfo);
    //舞团秘境
    int DanceGroupFairyLandLoadAllInfo(std::map<int, DanceGroupFairyLandGroupInfo>& rGinfo, 
        std::map<int, std::map<int, DanceGroupFairyLandPersonalInfo > >& rGPinfo,
        time_t &newDayClearStamp, time_t &passRewardStamp, time_t &storgeRewardStamp);
    int DanceGroupFairyLandNewDay(int nDefalutChapter);
    int DanceGroupFairyLandUpdateStampInfo(time_t nTimeStamp);
    int DanceGroupFairyLandUpdateStampInfo(int nType, time_t nTimeStamp);
    int DanceGroupFairyLandDelGroupInfo(int nGroupId);
    int DanceGroupFairyLandAddGroupInfo(int nGroupId, DanceGroupFairyLandGroupInfo& rGinfo);
    int DanceGroupFairyLandDelPersonalInfo(int nGroupId, int nRoleId);
    int DanceGroupFairyLandAddPersonalInfo(int nGroupId, int nRoleId);
    int DanceGroupFairyLandUpdateGroupChapter(int nGroupId, int nChapter);
    int DanceGroupFairyLandUpdateBlassRank(int nGroupId, std::string &rRankJsonFmt);
    int DanceGroupFairyLandUpdateTodayRank(int nGroupId, std::string &rRankJsonFmt);
    int DanceGroupFairyLandUpdateBlassEndTime(int nGroupId, int nEndTime, unsigned int nLastRoleID, const std::string &strRoleName);
    int DanceGroupFairyLandUpdateGroupScore(int nGroupId, int nScore);
    int DanceGroupFairyLandUpdateBlessEndTime(int nGroupId, int nEndTime);

    int DanceGroupFairyLandUpdatePersonalTotalScore(int nRoleId, int nTotalScore);
    int DanceGroupFairyLandClearYesterdayScore(int nRoleId);
    int DanceGroupFairyLandUpdatePersonaBlassTimes(int nRoleId,int nTotalTimes);
    int DanceGroupFairyLandUpdateChallengeTimes(int nRoleId, int nLastChapterLev, int nChallengeTimes);
    int DanceGroupFairyLandUpdateLastChallengeLvl(int nRoleid, int nChapterlvl);
    int DanceGroupFairyLandUpdateJoinRewardFlag(int nRoleId, std::string & str);
    int DanceGroupFairyLandUpdatePassedRewardFlag(int nRoleId, std::string & str);
    int DanceGroupFairyLandUpdatePassedRewardRecord(int nRoleId, std::string & strRewardRecord);
    int DanceGroupFairyLandUpdateStotageBox(int nRoleId, std::string & strStorageBox);

    int DanceGroupFairyLandClearAllPassedReward();
    int DanceGroupFairyLandClearAllStorageBoxReward();

    std::string LoadSystemState(int nID);
    int UpdateSystemState(int nID, int nValue);
    int UpdateSystemState(int nID, std::string strVale);

    //外部发送邮件
    int ExternalSendMail(ExternalSendMailInfo & rESMInfo);
    //外部发送Gift邮件
    int ExternalSendMailGift(ExternalSendMailGiftInfo & rESMGInfo);

    //玩家自定义配置
    int LoadPlayerSetting(ROLEINFO_DB *pRole);
    int SavePlayerSetting(unsigned int nRoleID, CPlayerSetting & rPlayerSetting, int nOption);

    //情侣婚姻
    int AddCoupleInfo(CCoupleInfoDB & coupleInfo, int & nAddRes);
    int DelCoupleInfo(unsigned int nPairID);
    int LoadCoupleInfo(ROLEINFO_DB *pRole);
    int UpdateCoupleMarriageDate(CCoupleInfo & coupleInfo);
    int UpdateCoupleIntimacy(unsigned int nPairID, int nValue);
    int UpdateCoupleBlessingValue(unsigned int nPairID, int nValue);
    int UpdateCoupleDivorceAgreementID(unsigned int nRoleID, unsigned int nDivorceAgreementID);
    int AddBlessingRankInfo(const CBlessingValueRankInfo & rankInfo);
    int DelBlessingRankInfo(unsigned int nPairID);
    int UpdateBlessingRankValue(unsigned int nPairID, int nBlessingValue);
    int LoadAllBlessingValueRank(std::vector<CBlessingValueRankInfo> * rankList);
    int LoadofflineCoupleInfo(std::map<unsigned int, COfflineCoupleInfo> * pOfflineCoupleInfo);
    int LoadRankCoupleInfo(std::map<unsigned int, CCoupleInfo> * pcoupletable);
    int UpdateMarriageGiftDate(unsigned int nPairID, int nGiftDate);

    //离婚协议书
    int AddDivorceAgreementInfo(CDivorceAgreementInfo & agreementinfo, int & nAddRes);
    int AddDivorceAction(const CDivorceAction & action);
    int LoadDivorceAgreementInfo(ROLEINFO_DB *pRole);
    int LoadDivorceAction(ROLEINFO_DB *pRole);
    int UpdateDivorceAgreementState(unsigned int nAgreementID, int nState);
    int UpdateDivorceAgreement(unsigned int nAgreementID, CDivorceAgreementInfo & agreement);

    // 魔法神灯
    int LoadMagicLamp(ROLEINFO_DB *pRole);
    int LoadMainlineParcloseList(ROLEINFO_DB *pRole);
    int LoadThemeParcloseList(ROLEINFO_DB *pRole);
    int LoadDungeonStarReward(ROLEINFO_DB *pRole);
    int LoadPlayerDebris(ROLEINFO_DB *pRole);
    int LoadFriendContactInfo(ROLEINFO_DB *pRole);

    // 福利系统
    int LoadLongactingRecharge(ROLEINFO_DB *pRole);
    int UpdateLongActingRechargeIndex(unsigned int nRoleID, unsigned int nRewardIndex);

    // 幸运魔法阵
    int LoadMagicArrayRecord(LPROLEINFO_DB pRole);
    int InsertMagicArrayRecord(unsigned int nRoleID, const MagicArrayRecord & magic);
    int UpdateMagicArrayRecord(unsigned int nRoleID, const MagicArrayRecord & magic);
    int LoadMagicArrayExchange(LPROLEINFO_DB pRole);
    int InsertMagicArrayExchange(unsigned int nRoleID, const MagicArrayExchangeRecord & exchangeRoecord);
    int UpdateMagicArrayExchange(unsigned int nRoleID, const MagicArrayExchangeRecord & exchangeRoecord);

    //拼图活动
    int LoadRolePuzzleOnLogin(ROLEINFO_DB *pRole);
    int LoadRolePuzzle(CRoleAllPuzzleDB& rRolePuzzle);
    int InsertRolePuzzle(unsigned int nRoleID, CRolePuzzleActivityDB& rPuzzleInfo);
    int UpdateRolePuzzle(unsigned int nRoleID, int nOriTime, CRolePuzzleActivityDB& rPuzzleInfo);

    //新玩家招募活动
    int UpdateRoleInvitationCode(unsigned int nRoleID, const char *pszInvitationCode);
    int CheckInvitationCode(const char* pszInvitationCode, int& isValid);

    int GetInvitationBindCount(int nValidLevel, unsigned int nStartTime, unsigned int nEndTime, const char *pszInvitationCode, unsigned int& nBindCount);
    int AddInvitation(unsigned int nRoleID, unsigned int nTime, const char *pszInvitationCode);

    int AddInviterReward(unsigned int nRoleID, int nLevel);
    int LoadInviterReward(CRoleAllInvitationInfoDB* pRewards, unsigned int nStartTime, unsigned int nEndTime);

    int AddAcceptorReward(unsigned int nRoleID);

    int GetInvitationInfo(int nValidLevel, CRoleAllInvitationInfoDB* pInvitationInfo, unsigned int nStartTime, unsigned int nEndTime);
    // 直播房间
    int GetDancerName(CLiveRoomDancerDB *pDancer);
    int UpdateDancerName(CLiveRoomDancerDB *pDancer);
    int LoadLiveRoomPointAndHot(unsigned int nStartTime, unsigned int nEndTime, CAllLiveRoomScoreDB *pAllScore);
    int	GetLiveRoomPointAndHot(unsigned int nStartTime, unsigned int nEndTime, CLiveRoomScoreDB *pScore);
    int UpdateAuditionPoint(unsigned int nRoleID, int nPoint, unsigned int nStartTime, unsigned int nEndTime);
    int UpdateDancerHot(unsigned int nRoleID, int nHot, unsigned int nStartTime, unsigned int nEndTime);

    //清除首充状态: 多次开启首充活动
    int ClearFirstPaymentInfo(unsigned int nRoleID);

    // 评论消息推送
    int UpdateNoPushRatingToday(unsigned int nRoleID, unsigned int nTime);	// 今天不再提醒
    int UpdatePushRatedLevelUp(unsigned int nRoleID, int nLevel);	// 升级提醒
    int UpdatePushRatedOnlineTime(unsigned int nRoleID, unsigned int nTime); // 在线时间提醒
    int UpdatePushRatedDailyQuest(unsigned int nRoleID, unsigned int nTime); // 完成第一个任务
    int UpdatePushRatedDailyShoping(unsigned int nRoleID, unsigned int nTime); // 完成第一次商城消费
    int UpdatePushRatedCount(unsigned int nRoleID, int nCount, unsigned int nTime); // 推送次数

    // 账号安全
    int LoadAccountSecurityQuiz(unsigned int nRoleID, SecurityQuiz_DB *quiz);
    int SetAccountSecurityQuiz(unsigned int nRoleID, const SecurityQuiz_DB *quiz);

    //新手七天乐活动
    int AddFresherReward(unsigned int nRoleID, int nDay);
    //新手七天目标
    int AddFresherQuestReward(unsigned int nRoleID, unsigned char nEventID, unsigned short nQuestID);
    int AddFresherQuestBestMusicGrade(unsigned int nRoleID, unsigned char nGrade);

    int	LoadCompensationInfo(CompensationInfoDB* pInfoDB);
    int InsertCompensationInfo(CompensationInfo* pInfoDB);
    int InsertRoleCompensation(int nRoleID, int nCompensationID);

    // 舞团红包
    int UpdateRedEnvelopeAnoymityEvent(int nRoleID, int nAnonymity);
    int LoadDanceGroupRedEnvelope(std::map<unsigned int, RedEnvelope*> & mapRedEnvelope, int & nMaxid);
    int InsertIntoRedEnvelope(RedEnvelope & redenvelope);
    int UpdateRedEnvelope(RedEnvelope & redenvelope);
    int LoadRedEnvelopeSenderInfo(RedEnvelopeSenderInfo & sender);

    // 增加商城购买次数
    int AddMallBuyCount(unsigned int nRoleID, int nCount);
    //平台特权
    int AddPlatformPrivilegeReward(unsigned int nRoleID, unsigned short  nRewardLevel);

    int AddQuestNew(QUESTNEWINFO* pQuestInfo);
    int UpdateQuestNew(QUESTNEWINFO* pQuestInfo);
    //时光蛋
    int CreateTimeEgg(CreateTimeEggEntry* pCreateTimeEggEntry);
    int GetTimeEggsByRoleID(QueryRoleTimeEggEntry* pInfo);
    int OpenTimeEgg(OpenTimeEggEntry* pInfo);
    int GetTimeEggs(std::map<unsigned int, TimeEggEntry*>* pInfo, unsigned int nEggID);
    // 星恋魔法阵
    int AddEnterConstellationLobbyEvent(unsigned int nRoleID);
    int UpdateConstellationCardInfo(unsigned int nRoleID, unsigned short nIndex, unsigned char nConstellation, unsigned int nCoolDownEndTime);
    int UpdateConstellationMatchCount(unsigned int nRoleID, int nMatchCount);
    int UpdateConstellationRoom(unsigned int nRoleID, unsigned int nRoomID, unsigned int nCreateTime, int nLine);

    // 魔法神灯
    int UpateMagicLampInfo(unsigned int nRoleID, const MagicLamp * pMagicLamp);
    int InsertMagicLampInfo(unsigned int nRoleID, const MagicLamp * pMagicLamp);
    int LoadFriendDungeonInfo(unsigned int nRoleID, std::vector<DungeonRankInfo> * pRankPlayers);
    int UpdateParcloseInfo(unsigned int nRoleID, const MainlineParcloseRecord * pRecord);
    int InsertParcloseInfo(unsigned int nRoleID, const MainlineParcloseRecord * pRecord);
    int UpdateThemeParcloseInfo(unsigned int nRoleID, const ThemeParcloseRecord * pRecord);
    int InsertThemeParcloseInfo(unsigned int nRoleID, const ThemeParcloseRecord * pRecord);
    int InsertStarRewardIndex(unsigned int nRoleID, unsigned int nIndex);
    int UpdateDebrisInfo(unsigned int nRoleID, unsigned int nDebrisID, unsigned int nCount);
    int InsertFriendContactInfo(unsigned int nRoleID, const FriendContactInfo * pFriendContactInfo);
    int UpdateFriendContactInfo(unsigned int nRoleID, const FriendContactInfo * pFriendContactInfo);
    int BatchUpdateFriendContactInfo(unsigned int nRoleID);
    int ClearMainlineQuickFinishNum(unsigned int nRoleID);
    int ClearThemeQuickFinishNum(unsigned int nRoleID);
    int UpdateOfflineFriendContactInfo(unsigned int nRoleID, unsigned int nFriendRoleID, unsigned int nVitNum);

    //舞团活动首次登录奖励
    int AddDanceGroupReward(unsigned int nRoleId, unsigned int nTime);
    int DelDanceGroupReward(unsigned int nRoleId);

    //社交分享
    int LoadSocialShare(LPROLEINFO_DB pRole);
    int SaveSocialShare(unsigned int nRoleId, int nUI, unsigned int nTime);

    //月卡
    int LoadMonthCard(LPROLEINFO_DB pRole);
    int UpdateMonthCardInfo(unsigned int nRoleID, const MonthCardDB * pMonthCardInfo);
    int InsertMonthCardInfo(unsigned int nRoleID, const MonthCardDB * pMonthCardInfo);

    // 限量销售
    int LoadLimitedEdition(LPROLEINFO_DB pRole);
    int LoadAuctionNotGiveInfo(LPROLEINFO_DB pRole);
    int SaveSpecialMall(CSpecialMallInfoDB *pInfo);
    int SaveAuctionBid(CAuctionBidInfoDB *pInfo);
    int SaveGroupBuyingCount(CSaveGroupBuyingCount *pInfo);
    int ChangeAuctionState(long long nLogID, unsigned int nRoleID, unsigned short nIdx, unsigned int nEndTime, EAuctionBidState eState, unsigned int nPrice, bool &bSuccess);
    int GetGroupBuyingCount(std::list<CGetLimitedSalesCount> *pInfo);
    int GetLimitedEditionCount(std::list<CGetLimitedSalesCount> *pInfo);
    int GetAuctionBidInfo(std::map<unsigned short, CGetAuctionBidInfo> *pInfo);

    // 排行榜
    int GetRankRoleData(RankGetRoleDataDB *pRoleDataDB);
    int GetRankAllDatas(RankGetAllDatasDB *pAllDatasDB);
    int SaveRankData(RankSaveDataDB *pSaveDataDB);
    int UpdateRankDataState(CRankUpdateDataStateDB *pUpdateDataStateDB);
    int SaveRankHistory( RankSaveHistoryDB *pSaveHistoryDB );

    // 充值设备黑名单
    int LoadBlackDevice(std::list<std::string> &blackList);
    int InsertBlackDevice(std::vector<std::string>* pDeviceList, std::string* strGM);
    int DeleteBlackDevice(std::vector<std::string>* pDeviceList);

    // 修改玩家密码
    int ChangeUserPassword(std::string* pAccount, std::string* pPassword);

    // 推荐好友
    int UpdateRecommendCount(unsigned int nRoleID, unsigned int nCount);
    int UpdateRecommendResetTime(unsigned int nRoleID, unsigned int nCount);

    // 活跃度系统
    int LoadRoleActiveness(unsigned int nRoleID, RoleActivenessDB &activeness);
    int UpdateActivenessScore(unsigned int nRoleID, int nScore);
    int UpdateActivenessTargetProgress(unsigned int nRoleID, const std::string &strTargetProgress, const std::string &strDanceMode);
    int UpdateActivenessReward(unsigned int nRoleID, const std::string &strScoreList);
    int UpdateOrInsertActiveness(unsigned int nRoleID, RoleActivenessDB *pActiveness);

    // 宠物系统
    int LoadPetIncubatorInfo(unsigned int nRoleID, unsigned short &rLevel);
    int UpdatePetIncubatorLevel(unsigned int nRoleID, unsigned short nLevel);
    int LoadPetEggInfo(unsigned int nRoleID, PetEgg &rPetEgg);
    int CreatePetEggInfo(unsigned int nRoleID, PetEgg &rPetEgg);
    int UpdatePetEggState(unsigned int nEggID, unsigned char nState);
    int LoadPetExtendGridCount(unsigned int nRoleID, unsigned short &rCount);
    int UpdatePetExtendGridCount(unsigned int nRoleID, unsigned short nCount);
    int LoadPetsInfo(unsigned int nRoleID, std::map<unsigned int, PetDB, std::greater<unsigned int> > &rPets);
    int CreatePetInfo(unsigned int nRoleID, Pet &rPet);
    int ChangePetName(unsigned int nPetID, const std::string &rNewName);
    int UpdatePetState(unsigned int nPetID, unsigned char nState);
    int UpdatePetQuality(unsigned int nPetID, unsigned char nQualityType);
    int UpdatePetEvolution(unsigned int nPetID, unsigned char nEvolutionTimes);
    int UpdatePetEndurance(unsigned int nPetID, unsigned short nEndurance, unsigned int nRecoveryTime);
    int UpdatePetLevelAndExp(unsigned int nPetID, unsigned short nLevel, unsigned int nExp);
    int UpdatePetSkill(Pet &rPet);
    int ResetPetSkill(Pet &rPet);

    int CheckRebindInfo(RebindAccountInfo &rebindInfo);
    int UpdateAccountByTrancode(RebindAccountInfo &rebindInfo);

    // 勋章系统
    int CreateMedal(unsigned int nRoleID, MedalUnitDB& unit);
    int LoadMedalMap(unsigned int nRoleID, std::map<int, MedalUnitDB> &mapCMedalUint);
    int UpdateMedal(unsigned int nRoleID, MedalUnitDB& unit);

    int CreateMedalCondition(unsigned int nRoleID, MedalConditionDB& unit);
    int LoadMedalCondition(unsigned int nRoleID, std::map<int, MedalConditionDB> &mapCMedalUint);
    int UpdateMedalCondition(unsigned int nRoleID, MedalConditionDB& condition);
    int DeleteMedalCondition(unsigned int nRoleID, MedalConditionDB& condition);
    
    int LoadMedalScore(unsigned int nRoleID, int& nMedalWearScore, int& nMedalCumulativeScore);
    int UpdateMedalScore(unsigned int nRoleID, MedalScoreDB& score);

    int DelVersionMedal(std::list<int>& list);
    int SaveHistoryVersionsMedal(std::list<int>& list);
    int doCheckAddMedalByGm(unsigned int nRoleID, std::list<int>* pListneedCheckMedalID);
    int AddMedalByGm(unsigned int nRoleID, int nMedalID, std::list<int>* pListneedCheckMedalID);

    //图鉴系统
    int LoadHandbookPcInfo(unsigned int nRoleID, int nType, std::map<int, std::string > &PersonalInfo);
    int LoadHandbookGlobalInfo(int nType, std::map<int, CHandbookUnit > &GlobalInfo);
    int UpdateHandbookGlobalInfo(int nType, std::map<int, CHandbookUnit > &unit);

    int UpdateHandbookPcInfo(unsigned roleid, const std::map<int, std::string > &PersonalInfo);
    int InsertHandbookPcInfo(unsigned roleid, int nHandbookType, const std::map<int, std::string > &PersonalInfo);

//     void HandbookDecode(OUT std::list<CHandbookPersonalInfo *> &infolist, char* src);
//     void HandbookEncode(const std::list<CHandbookPersonalInfo *> &infolist, OUT std::string &src);

    // 愿望
    int LoadDesiredItem(LPROLEINFO_DB pRole);
    int InsertDesiredItem(unsigned int nRoleID, itemtype_t nItemType);
    int DeleteDesiredItem(unsigned int nRoleID, itemtype_t nItemType);

protected:
    void LoadItemInfoFromRow(DBItem *pItemInfo);
    void LoadQuestInfoFromRow(LPQUESTINFO pQuest);

    void InsertTargetInto(QUESTINFO *pQuest, TargetInfo *pInfo);
    bool IsValidCheckPacketType(unsigned char nPacketType);

    char *pStrStop;
};

#endif//__DATABASE_INTERFACE_H__

