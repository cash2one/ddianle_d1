#ifndef		__ENTITYMARRIAGECOMPONENT_H__
#define		__ENTITYMARRIAGECOMPONENT_H__

#include "../logic/RoleComponentImpl.h"
#include "../logic/EntityComponent.h"
#include "CoupleMacroDef.h"
#include "DivorceAgreement.h"
#include "GameMsg_S2C_Couple.h"

class CEntityAttributeComponent;
class CEntityNetComponent;
class CEntityItemComponent;
class CEntityMailComponent;
class CEntityChatComponent;
class CEntityDanceGroupComponent;
class CEntityGeneComponent;
class CEntityVIPComponent;
class CRoomComponent;
class CPlayerInfoComponent;
class CEntityFriendComponent;
class CEntityVIPComponent;
class CEntityQuestComponent;
class CEntityQuestNewComponent;
class CEntityMallComponent;
class CEntityDungeonComponent;
class PetComponent;
class CEntityMedalComponent;
class CEntityHorseComponent;

class CCoupleRequest;
class CWeddingRoom;
class CWeddingRoomPlayerInfo;
class CEntityPhotoRoomComponent;
class CAmuseRoomComponent;

class CEntityCoupleComponent :
    public CEntityComponent
{
public:
    CEntityCoupleComponent();
    ~CEntityCoupleComponent();
public:
    virtual void Start();
    virtual void OnEvent(CComponentEvent& refEvent);
    virtual bool CreateFromDB(ROLEINFO_DB* pRoleInforDB);
    virtual bool PacketToDB(ROLEINFO_DB *pRoleInforDB) const;
    virtual void OnUpdate(const unsigned long &nTimeElapsed);
    virtual void SerializeComponent(CParamPool &IOBuff);
    virtual const char* SerializeName();
    virtual void OnLogin();
    virtual void OnLogout();
    void RegComponentNetMsgMap();
    virtual void SendExtraStaticData(){};
    virtual void PacketBuff(std::list<BuffData>& listBuff) {}

    CEntityAttributeComponent	* RoleAttr();
    CEntityItemComponent		* RoleItem();
    CEntityMailComponent		* RoleMail();
    CEntityChatComponent		* RoleChat();
    CRoomComponent				* RoleStageRoom();
    CEntityPhotoRoomComponent	* RolePhotoRoom();
    CAmuseRoomComponent			* RoleAmuseRoom();
    CEntityDanceGroupComponent  * RoleDanceGroup();

    unsigned int RoleID();
    const CCoupleInfo & CoupleInfo();
    unsigned int PairID();
    unsigned int CoupleID();
    const std::string & CoupleName();
    unsigned int Intimacy();
    unsigned int CoupleDate();
    unsigned int MarriageDate();
    unsigned int CoupleSmallRoleID(){
        return m_coupleInfo.m_nSmallRoleID;
    }

    bool GetCoupleRoleID(unsigned int &rMaleRoleID, unsigned int &rFemaleRoleID);

    bool IsSingle();
    bool CanHaveNewCouple(unsigned int nTargetID);
    bool IsDivorceing(unsigned int nTargetID);

    // 是否结过婚
    bool IsMarried();

    // 重置夫妻信息
    void ResetCoupleData();

    // 获取
    CWeddingRoom* WeddingRoom();

    // 获取/设置教堂类型
    void SetRoomRoleType(EWeddingRoomRoleType eRoomRoleType);
    EWeddingRoomRoleType RoomRoleType();

    // 是否房间主人
    bool IsHost() const;

    // 进入
    void InWeddingRoom(CWeddingRoom *pRoom, bool bHost);

    // 离开
    void OutWeddingRoom();

    void SetRoomPlayerInfo(CWeddingRoomPlayerInfo & rRoomPlayer);

    // 开始婚礼
    bool StartWedding(int & nRes, std::string & strError);

    // 自动开始婚礼
    bool AutoStartWedding(int & nRes, std::string & strError);

    void GetMarried();

    void ChangeIntimacy(int nValue, EChangeIntimacyCause eCause, int nPara);

    void SendMarriedItem();
    void DeleteMarriedItem();

    void SendPlayerMsg(GameMsg_Base *pMsg);

    void CoupleTest(int nPara1, int nPara2, int nPara3);

    // 获取结婚后祝福值
    unsigned int GetBlessingValue();

    // 获取/设置纪念日礼物时间
    void SetMarriageGiftDate(unsigned int nDate);
    unsigned int GetMarriageGiftDate(){
        return m_coupleInfo.m_nMarriageGiftDate;
    }
    void SendMarriageDateGift(unsigned int nDate);

    // 检查情侣任务
    void CheckCoupleQuest();

private:
    void _OnSendCoupleRequest(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void _OnAcceptCoupleRequest(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void _OnRefuseCoupleRequest(GameMsg_Base &msg, CSlotPeer & slotPeer);
    void _OnCoupleSeparateRequest(GameMsg_Base &msg, CSlotPeer & slotPeer);
    void _OnNotifyHasBeenSeparated(GameMsg_Base &msg, CSlotPeer & slotPeer);
    void _OnRcvWeddingStartingRoomNofication(GameMsg_Base &msg, CSlotPeer & slotPeer);
    void _OnRcvAutoStartWeddingRoomFail(GameMsg_Base &msg, CSlotPeer & slotPeer);
    void _OnRequestMarry(GameMsg_Base &rMsg, CSlotPeer & slotPeer);
    void _OnRequestDivorce(GameMsg_Base &msg, CSlotPeer & slotPeer);
    void _NotifyHasBeenDivorced(GameMsg_Base & msg, CSlotPeer &slotPeer);

    void _OnTyrEnterWeddingRoom(GameMsg_Base & rMsg, CSlotPeer &slotPeer);
    void _OnEnterWeddingRoom(GameMsg_Base & rMsg, CSlotPeer &slotPeer);
    void _OnEnterMyWeddingRoom(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    void _OnKickWeddingRoomPlayer(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    void _OnSetWeddingPlayerType(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    void _OnChangeWeddingRoomPwd(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    void _OnRoleNameChanged(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    void _OnAddCoupleCallBack(QueryBase & query);
    void _OnAddDivorceAgreementCallBack(QueryBase & query);

    bool _CanStartWedding(int & nRes, std::string & strError);
    bool _HasReceiveCoupleRequest(unsigned int nSenderID);
    void _AddCoupleRequest(CCoupleRequest * pCoupleRequest);
    void _RemoveCoupleRequest(unsigned int nSenderID);

    // 情侣、结婚性别判断
    bool CheckCoupleSex(char cSelfSex, char cOtherSex);

public:
    // 获取教堂列表
    void _OnGetWeddingRoomList(GameMsg_Base &rMsg, CSlotPeer & slotPeer);

    // 离开教堂房间
    void _OnExitWeddingRoom(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // 改变亲密值
    void _OnChangeIntimacy(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // 创建婚房
    void _OnCreateWeddingRoom(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // 获取排行信息
    void OnGetBlessingRankList(GameMsg_Base & rMsg, CSlotPeer &slotPeer);
    void _OnGetBlessingRankList(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // 玩家的祝福
    void OnBlessingCouple(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // 玩家红包
    void OnRedEnvelope(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // 获取coupleInfo信息
    void OnGetCoupleInfo(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // 获取coupleInfo信息
    void _OnGetCoupleInfo(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // 获取婚礼动态列表
    void OnGetWeddingRecordList(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // 获取婚礼动态列表(从group来的)
    void _OnGetWeddingRecordList(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // 获取婚礼动态的祝福值
    void OnGetBlessingValue(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // 获取婚礼动态的祝福值(从group来的)
    void _OnGetBlessingValue(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // 获取形象
    void OnGetPersonInfo(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // 获取形象(从group来的)
    void _OnGetPersonInfo(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // 获取夫妇祝福奖励信息
    void OnGetCoupleRedEnvelopeGiftInfo(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    void _OnDelRedEnvelopeMoney(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    void _OnSendBlessingErrorMsg(GameMsg_Base & rMsg, CSlotPeer &slotPeer);
    void _OnSendBlessingCoupleSucMsg(GameMsg_Base & rMsg, CSlotPeer &slotPeer);
    void _OnSendRedEnvelopeSucMsg(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    void _OnWeddingNotice(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    void RoleCoupleLog(unsigned int nMaleID, unsigned int nFemaleID, int type, int nIntimacy, int nBlessingValue);

private:
    // 广播祝福消息
    void BroadcastCreateWeddingRoomMsg();

    // 广播情侣双方在线好友开启婚礼消息
    void BroadcastMarriageToCoupleFriends(bool isSendCurrentLine = false);
    void BroadcastOtherLineFriends(unsigned int nRoleID, std::string strMaleName, std::string strFemaleName);

public:
    // 获取离婚协议书
    CDivorceAgreement & GetDivorceAgreement(){
        return m_DivorceAgreement;
    }

    //获取协议书ID
    unsigned int GetDivorceAgreementID(){
        return m_DivorceAgreement.GetAgreementID();
    }

    // 获取自己协议书ID
    unsigned int GetSelfDivorceAgreementID(){
        return m_nDivorceAgreementID;
    }
    void SetSelfDivorceAgreementID(unsigned int nAgreementID){
        m_nDivorceAgreementID = nAgreementID;
    }

    // 基本信息
    void SerializeDivorceComponent(CParamPool &IOBuff);

    // 申请离婚
    void OnDivorceRequest(GameMsg_Base &msg, CSlotPeer & slotPeer);
    int DivorceRequest();

    // 取消离婚
    int DivorceCancel();
    void OnDivorceCancel(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // 强制离婚
    void OnDivorceMandatory(GameMsg_Base & rMsg, CSlotPeer &slotPeer);
    int DivorceMandatory();

    // 同意离婚
    void OnDivorceAgree(GameMsg_Base & rMsg, CSlotPeer &slotPeer);
    int DivorecAgree();

    // 处理离婚信息
    int DivorceHandle();

    // 离婚聊天
    void OnDivorceChat(GameMsg_Base & rMsg, CSlotPeer &slotPeer);
    int DivorceChat();

    // 离婚送礼
    void OnDivorceGift(GameMsg_Base & rMsg, CSlotPeer &slotPeer);
    int DivorceGift();

    // 愿望物品
    void OnDivorceWishItem(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // 离婚MV
    void OnDivorceAnimation(GameMsg_Base & rMsg, CSlotPeer &slotPeer);
    int DivorceAnimation();
    void SetDivorcePlayerMsg(DivorcePlayerMsg & playermsg);
    void SetDivorceOfflinePlayerMsg(unsigned int nRoleID, RoleImageOfflineData & OffLineData, DivorcePlayerMsg & playermsg);
    void _GetDivorceCouplePersonInfo(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // 有了新情侣
    void HaveNewCouple(char cThreeSex);

    // 协议书对方ID
    unsigned int GetDivorecAgreementOtherRoleID();

    // 行为通知
    void _OnRefreshDivorceAction(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

public:
    // in room attribute
    CWeddingRoom *              m_pWeddingRoom;
    bool                        m_bHost;
    EWeddingRoomRoleType        m_eRoomRoleType;
    time_t                      m_nLastSendCoupleRequestTime;
    CCoupleInfo	                m_coupleInfo;
    std::list<CCoupleRequest *> m_listCoupleRequest;
    unsigned int		m_nDivorceAgreementID;
    CDivorceAgreement m_DivorceAgreement;

public:
    CEntityAttributeComponent	* m_pRoleAttr;
    CEntityNetComponent			* m_pRoleNet;
    CEntityItemComponent		* m_pRoleItem;
    CEntityMailComponent		* m_pRoleMail;
    CEntityChatComponent		* m_pRoleChat;
    CEntityDanceGroupComponent	* m_pRoleDanceGroup;
    CEntityGeneComponent		* m_pRoleGene;
    CRoomComponent				* m_pRoleRoom;
    CPlayerInfoComponent		* m_pRoleInfo;
    CEntityPhotoRoomComponent	* m_pRolePhotoRoom;
    CAmuseRoomComponent			* m_pRoleAmuseRoom;
    CEntityFriendComponent       * m_pRoleFriend;
    CEntityVIPComponent         * m_pRoleVip;
    CEntityQuestComponent		* m_pRoleQuest;
    CEntityQuestNewComponent	* m_pRoleQuestNew;
    CEntityMallComponent		* m_pRoleMall;
    CEntityDungeonComponent		* m_pRoleDungeon;
    PetComponent                * m_pRolePet;
    CEntityMedalComponent       *m_pRoleMedal;
    CEntityHorseComponent       * m_pRoleHorse;

};

typedef CRoleComponentImpl<CEntityCoupleComponent, CGetRoleProcessorObj, CGetRoleProcessorObj> CRoleCouple;

#endif

