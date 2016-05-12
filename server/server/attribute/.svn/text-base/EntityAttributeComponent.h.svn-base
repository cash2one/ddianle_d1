/*
*2011-11-9
*by wangshijun
*把entity组件化。
*/
#ifndef __ENTITYATTRIBUTE_COMPONENT_H__
#define __ENTITYATTRIBUTE_COMPONENT_H__
#include "../logic/RoleComponentImpl.h"
#include "../logic/EntityComponent.h"
#include "../mall/MallDefine.h"
#include "../mall/CurrencyType.h"

#define ATTRIBUTE_UPDATE_DELAY	3000	//必须大于1000毫秒，并且为1000的整数倍


class GameMsg_S2C_CreateRoleSuc;
class CEntityQuestComponent;
class CEntityNetComponent;
class CEntityAchievementComponent;
class CRoomComponent;
class CEntityItemComponent;
class CEntityMailComponent;
class CEntityFriendComponent;
class CAmuseRoomComponent;
class CEntityPhotoRoomComponent;
class CEntityDanceGroupComponent;
class CEntityCoupleComponent;
class CEntityVIPComponent;
class CNotificationComponent;
class CEntityAntiAddictonComponent;
class CEntityQuestNewComponent;
class FirstPaymentInfo;
class CEntityActivityComponent;
class CRankComponent;
class CLevelRewardInfo;

#define BUILD_CURRENCY_MAP(currencyMap, MCoin, bindMCoin, money)\
    CurrencyMap currencyMap; \
if (0 != MCoin) currencyMap.insert(std::make_pair(EMallCurrencyType_MCoin, (MCoin))); \
if (0 != bindMCoin) currencyMap.insert(std::make_pair(EMallCurrencyType_BindMCoin, (bindMCoin))); \
if (0 != money) currencyMap.insert(std::make_pair(EMallCurrencyType_Money, (money)))


#define ADDTO_CURRENCY_MAP(currencyMap, coinType, coin)\
if (0 != coin && coinType > EMallCurrencyType_None && coinType < EMallCurrencyType_Max)\
    currencyMap.insert(std::make_pair((unsigned char)(coinType), (unsigned int)(coin)))


#define BUILD_CURRENCY_MAP_ONETYPE(currencyMap, coinType, coin)\
    CurrencyMap currencyMap; \
    ADDTO_CURRENCY_MAP(currencyMap, coinType, coin)

class CEntityAttributeComponent :
    public CEntityComponent
{
public:
    unsigned short		m_nPID;														// 平台ID
    unsigned int        m_nAccount;													// 账号id
    unsigned int        m_nRoleID;													// 角色id
    char                m_szRoleName[MAX_NAME_LEN];									// 角色名称

    unsigned char       m_nSex;														// 身体类型,即性别
    int					m_nMoney;													// 金钱
    unsigned int        m_nExp;														// 经验
    unsigned short      m_nLevel;													// 级别
    unsigned int        m_nHot;														// 人气

    unsigned char       m_nConstellation;											// 星座
    unsigned char       m_nSkinColor;												// 皮肤颜色

    unsigned int        m_nPriv;													// 角色权限

    unsigned int		m_nPre;														// 声望
    unsigned int        m_nRoleCreateTime;											// 角色创建时间
    unsigned int        m_nLastLoginTime;											// 上次登录时间
    unsigned int        m_nLastLogoutTime;											// 上次登出时间
    bool                m_bTodayFirstLogin;											// 是否是今天第一次登录
    unsigned int        m_nTimeOnline;     											// 在线时长

    unsigned char       m_anBeiBaoKuozhan[BEIBAOKUOZHAN_MAX_ITEM_TYPE_NUM];			// 背包扩展
    unsigned char       m_nBadgeGridNum;
    char                m_szSignature[MAX_SIGNATURE_LEN];							// 签名
    char				m_szInvitationCode[MAX_INVITATION_CODE_LEN];				// 邀请码
    unsigned char		m_nClothGroupId;											// 服饰搭配id
    int					m_nTransformId;												// 变身id
    bool                m_bKeyDataModified;											// 关键数据更新标志

    BILLINFO            m_Bi;
    unsigned short      m_nMoneyGain;                    							// 金钱获得的比率
    unsigned short      m_nExpGain;	                    							// 经验值获得的比率
    unsigned short      m_nPreGain;	                    							// 声望值获得的比率
    unsigned short      m_nIgnoreMis;                    							// 忽略错误数

    int					m_nRoleLocation;                							// 角色位置信息
    unsigned int		m_nQuestDone;												// 完成任务总数
    //事件
    bool				m_bCompleteTeachGuide;										// 完成新手引导
    char				m_nCompleteProcGuide;										// 完成新手引导
    bool				m_bCompleteAnimation;										// 完成新手动画
    bool                m_bCompleteFirstPaymentTag;									// 完成首次充值
    bool                m_bCanSendFirstPaymentReward;								// 已完成首次充值，但没发送礼品
    bool				m_bReceivedInvitationReward;								// 已经领取邀请奖励
    bool				m_bReceivedProcGuideReward;									// 已经领取新手引导奖励
    unsigned int		m_guideData;												// 新手引导二进制数据，
    // 	typedef std::set<unsigned int> FixedCharges;
    typedef std::map<unsigned int, unsigned int> FixedCharges; // <charge, timestamp>
    FixedCharges		m_fixedCharges;												// 定额首冲完成的额度

    unsigned int        m_nLastChangeBirthdayTime;                                  // 上次更改生日时间戳

    unsigned int        m_nLastRstMoneyTime;
    unsigned int        m_nMallLastMoneyRstRemindtime;
    unsigned int        m_nMailLastMoneyRstRemindTime;
    unsigned int		m_nAccountCreateTime;

private:
    unsigned long		m_nUpdateDelay;
    FirstPaymentInfo*   m_pFirstPaymentInfoLastNotify;								// 上次首充结构体
    std::string         m_strBirthday;                                              // 生日, 格式为: "19900601"
    unsigned int        m_nNotGetLevelUpVIPRewardLevel;                             // 升级奖励未领取VIP奖励等级

public:
    CEntityAttributeComponent(void);
    ~CEntityAttributeComponent(void);
public:
    virtual void Start();
    void RegComponentNetMsgMap();
    virtual void SendExtraStaticData(){};
    virtual void PacketBuff(std::list<BuffData>& listBuff){}
public:
    bool CanBeVIPKick();
    unsigned short PID() const;

    void SetAccount(int nAccount){
        m_nAccount = nAccount;
    };
    unsigned int GetAccount() const {
        return m_nAccount;
    };
    void SetRoleID(unsigned int nRoleID){
        m_nRoleID = nRoleID;
    };
    unsigned int GetRoleID() const {
        return m_nRoleID;
    };

    unsigned char GetColor() const {
        return m_nSkinColor;
    }
    unsigned char GetSex() const {
        return m_nSex;
    };

    bool IsMale() const {
        return m_nSex == ESexType_Male ? true : false;
    }

    const char *GetRoleName() const;
    void SetRoleName(const char *pNewName);
    void OnNameChanged(const char* pNewName);

    short GetAge() const;

    void SetBirthday(const std::string &rBirthday);
    const std::string& GetBirthday() const {
        return m_strBirthday;
    }

    void SetInvitationCode(const char *pszInvitationCode);
    const char *GetInvitationCode();

    unsigned int GetPoint() const {
        return m_Bi.nPoint;
    };
    unsigned int GetTotalPoint() const {
        return m_Bi.nTotalPoint;
    }
    unsigned int GetBindPoint() const {
        return m_Bi.nBindPoint;
    };

    int GetMoney() const {
        return m_nMoney;
    };
    void SetMoney(int nValue);

    void ChangeMoney(int nValue, EChangeMoneyCause eChangeMoneyCause, int nAffixData, bool bSaveToDB = true, bool bChecAntiAddiction = true);

    unsigned short GetLevel() const;
    void SetLevel(unsigned short nValue, int nCauseType, int nAffixData);
    void OnLevelUp(unsigned short nOldLevel);

    unsigned int GetExp() const {
        return m_nExp;
    };
    int GetLevelExp() const;

    unsigned int AddAbsExp(unsigned int nValue, CRoleExpLog::EAddExpCause eAddExpCause, int nAffixData);
    unsigned int AddRateExp(unsigned int nValue, CRoleExpLog::EAddExpCause eAddExpCause, int nAffixData);

    void SetPre(unsigned int nValue, int nCauseType, int nAffixData);
    void ModifyPre(int nValue, int nCauseType, int nAffixData);
    unsigned int GetPre(){
        return m_nPre;
    }

    unsigned int GetLastLoginTime() const {
        return m_nLastLoginTime;
    };
    unsigned int GetLastLogoutTime() const {
        return m_nLastLogoutTime;
    };
    void SetTodayFirstLoginTag(bool b) {
        m_bTodayFirstLogin = b;
    };
    bool GetTodayFirstLoginTag() const {
        return m_bTodayFirstLogin;
    };

    const BILLINFO & GetBillInfo() const {
        return m_Bi;
    };

    void SetHot(unsigned int nValue, int nCauseType, int nAffixData);
    unsigned int GetHot()const{
        return m_nHot;
    }
    void ModifyHot(int nValue, int nCauseType, int nAffixData);

    void SetLocation(int nLocation);
    int GetLocation();

    unsigned int GetOnlineTime(){
        return m_nTimeOnline;
    }
    void SetOnlineTime(unsigned int nOnlineTime);

    unsigned char GetBadgeGridNum(){
        return m_nBadgeGridNum;
    };
    void          SetBadgeGridNum(unsigned char nNum);
    unsigned short GetMoneyGain() const {
        return m_nMoneyGain;
    };
    unsigned short GetExpGain() const {
        return m_nExpGain;
    };
    unsigned short GetPreGain() const {
        return m_nPreGain;
    };
    unsigned short GetIgnoreMis() const {
        return m_nIgnoreMis;
    };

    void SetMoneyGain(unsigned short nValue);
    void SetExpGain(unsigned short nValue);
    void SetPreGain(unsigned short nValue);
    void SetIgnoreMis(unsigned short nValue);

    void ModifyMoneyGain(int nValue);
    void ModifyExpGain(int nValue);
    void ModifyPreGain(int nValue);
    void ModifyIgnoreMis(int nValue);

    unsigned char GetClothGroupID() const {
        return m_nClothGroupId;
    }
    void SetClothGroupID(unsigned char id) {
        m_nClothGroupId = id; m_bKeyDataModified = true;
    }

    int GetTransformId() const;
    void SetTransformIdAndNotify(int val);

    FirstPaymentInfo* GetFirstPaymentInfoLastNotify() const {
        return m_pFirstPaymentInfoLastNotify;
    }
    void SetFirstPaymentInfoLastNotify(FirstPaymentInfo* val) {
        m_pFirstPaymentInfoLastNotify = val;
    }

    bool IsMarried();

    void ResendLevelUpVIPExtraReward(unsigned char nOpenVipFromType);
    // 设置固定首冲完成标记
    void SetFixedChargeRewardDone(unsigned int amount);
    void SendFixedChargeMsg(bool bIsFiexedCharge, unsigned int nGiftBindPoint);
public:
    //权限
    unsigned int GetPriv()const {
        return m_nPriv;
    };
    bool HasPriv(unsigned int nPriv)const {
        return (((m_nPriv&nPriv) == nPriv) ? true : false);
    }
    bool IsNormalPlayer() {
        return m_nPriv == 0;
    };
    bool IsDebugPlayer() {
        return m_nPriv == 0xffffffff;
    };
    bool IsGM() {
        return m_nPriv != 0;
    };
public:
    bool CreateFromDB(ROLEINFO_DB* pRoleInforDB);
    bool PacketToDB(ROLEINFO_DB*pRoleInforDB) const;
    virtual void SerializeComponent(CParamPool &IOBuff);
    virtual const char* SerializeName(){
        return "Attribute";
    };
    virtual void OnLogin();
    virtual void OnLogout();
    virtual void OnUpdate(const unsigned long & nTimeElapsed);

public:
    bool CanBill(CurrencyMap &mapCurrency);
    void Bill(CurrencyMap &mapCurrency, EChangeBillCause eChangeBillCause, EChangeBindBillCause eChangeBindBillCause, EChangeMoneyCause eChangeMoneyCause);
    //改变M币
    unsigned int ChangeBill(int nPoint, EChangeBillCause eChangeBillCause, bool bSaveToDB = true, bool bAddCumulativeSpend = true, bool bAddVipExp = true);

    //改变和绑定M币
    unsigned int ChangeBindBill(int nBindPoint, EChangeBindBillCause eCause, bool bSaveToDB = true, bool bAddCumulativeSpend = true);

    ////nType   0表示游戏活动赠送  1表示游戏内任务奖励 2表示使用抵用券购物
    //unsigned int AddChange(int nPoint,unsigned char nType);

    // 货币是否够
    bool IsEnoughCurrency(EMallCurrencyType eType, int nMoney);
    void DeductCurrency(EMallCurrencyType eType, int nMoney, const std::vector<int> & vecCause);

public:
    void TodayFirstLogin();//第一次登入完
    virtual void OnEvent(CComponentEvent& refEvent);
    void SendPlayerMsg(GameMsg_Base *pMsg);
    void SendGroupMsg(GameMsg_Base *pMsg);
public:
    //GM命令添加M币
    void AddBindMCoinByCmd(unsigned int nValue);
    void AddMCoinByCmd(unsigned int nValue);

    //角色改变金券日志
    static void LogRoleChangeMoney(unsigned int nRoleID, int nChangeValue, EChangeMoneyCause eChangeMoneyCause, int nNewMoney);

private:
    void SetGroupUpdateAttrInfo(CWorldRoleUpdateAttrInfo &updateAttrInfo);
    void SendGroupUpdateAttrInfo(CWorldRoleUpdateAttrInfo &updateAttrInfo);
    void UpdateGroupAttrInfo();
    void NotifyGroupRoleNameChange();
    void NotifyGroupLevelChange();
    void NotifyGroupMoneyChange();
    void NotifyGroupRoleAgeChange();
    void _SendLevelUpReward(unsigned short nOldLevel, unsigned short nNewLevel);
    void _SendLevelUpReward(const CLevelRewardInfo &rRewardInfo);
    void _SendUpgradeInstallerMessage();

    //角色添加经验日志记录
    void _LogAddExp(unsigned int nRoleID, CRoleExpLog::EAddExpCause eAddExpCause, unsigned int nAddExp, unsigned int nNewLevel, unsigned int nNewExp);

    //角色改变M币日志
    void _LogRoleChangeBill(unsigned int nRoleID, int nChangeValue, EChangeBillCause eChangeBillCause, unsigned int nNewPT);
    void _LogRoleChangeBindBill(unsigned int nRoleID, int nChangeValue, EChangeBindBillCause eCause, unsigned int nNewBindPT);

    // 角色属性改变日志
    void LogRoleAttrChange(unsigned int nRoleID, const std::string &strOldAttrValue, ERoleAttrChangeCause eChangeCause, const std::string &strNewAttrValue);

    //更新首充
    void _UpdateFirstPayment(unsigned int nTimeNow);
    void SetRoleName(const char *pNewName, bool bNotice);

public:
    bool IsGuideFinish(EGuideBit eGuide);
    void SetGuideFinish(EGuideBit eGuide);
    void FlushGuidData();


private:
    void OnChangeName(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnChangeSignature(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnCompleteTeachGuide(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnCompleteProcGuide(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnSelectGuideFace(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnCompleteAnimation(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnChangeBirthday(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnReciveGuideExperienceCard(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnReciveGuideFinish(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnFixedChargeReq(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnChangeRoleNameCallBack(QueryBase & query);
    void OnChangeRoleSignatureCallBack(QueryBase & query);
    void OnChargeCallBack(QueryBase & query);
    void OnCompleteTeachGuideCallBack(QueryBase &query);
    void OnCompleteProcGuideCallBack(QueryBase &query);
    void OnCompleteProcAnimationCallBack(QueryBase& query);
    void OnClearFirstPaymentInfoCallBack(QueryBase& query);

private:
    CEntityNetComponent		* m_pRoleNet;
    CEntityQuestComponent   * m_pRoleQuest;
    CEntityQuestNewComponent* m_pRoleQuestNew;
    CEntityAchievementComponent *m_pRoleAch;
    CEntityItemComponent	* m_pRoleItem;
    CEntityMailComponent	* m_pRoleMail;
    CEntityFriendComponent	* m_pRoleFriend;
    CRoomComponent			* m_pRoleRoom;
    CEntityDanceGroupComponent * m_pDanceGroup;
    CEntityCoupleComponent	* m_pRoleCouple;
    CEntityVIPComponent		* m_pRoleVIP;
    CNotificationComponent  * m_pNotification;
    CEntityAntiAddictonComponent* m_pRoleAntiAddiction;
    CEntityPhotoRoomComponent * m_pPhotoRoomCOM;
    CAmuseRoomComponent       * m_pAmuseRoomCOM;
    CEntityActivityComponent * m_pRoleActivity;
    CRankComponent          *m_pRankComponent;
};

typedef CRoleComponentImpl<CEntityAttributeComponent, CGetRoleProcessorObj, CGetRoleProcessorObj> CRoleAttribute;

#endif
