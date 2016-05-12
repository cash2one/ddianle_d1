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

    // �Ƿ�����
    bool IsMarried();

    // ���÷�����Ϣ
    void ResetCoupleData();

    // ��ȡ
    CWeddingRoom* WeddingRoom();

    // ��ȡ/���ý�������
    void SetRoomRoleType(EWeddingRoomRoleType eRoomRoleType);
    EWeddingRoomRoleType RoomRoleType();

    // �Ƿ񷿼�����
    bool IsHost() const;

    // ����
    void InWeddingRoom(CWeddingRoom *pRoom, bool bHost);

    // �뿪
    void OutWeddingRoom();

    void SetRoomPlayerInfo(CWeddingRoomPlayerInfo & rRoomPlayer);

    // ��ʼ����
    bool StartWedding(int & nRes, std::string & strError);

    // �Զ���ʼ����
    bool AutoStartWedding(int & nRes, std::string & strError);

    void GetMarried();

    void ChangeIntimacy(int nValue, EChangeIntimacyCause eCause, int nPara);

    void SendMarriedItem();
    void DeleteMarriedItem();

    void SendPlayerMsg(GameMsg_Base *pMsg);

    void CoupleTest(int nPara1, int nPara2, int nPara3);

    // ��ȡ����ף��ֵ
    unsigned int GetBlessingValue();

    // ��ȡ/���ü���������ʱ��
    void SetMarriageGiftDate(unsigned int nDate);
    unsigned int GetMarriageGiftDate(){
        return m_coupleInfo.m_nMarriageGiftDate;
    }
    void SendMarriageDateGift(unsigned int nDate);

    // �����������
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

    // ���¡�����Ա��ж�
    bool CheckCoupleSex(char cSelfSex, char cOtherSex);

public:
    // ��ȡ�����б�
    void _OnGetWeddingRoomList(GameMsg_Base &rMsg, CSlotPeer & slotPeer);

    // �뿪���÷���
    void _OnExitWeddingRoom(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // �ı�����ֵ
    void _OnChangeIntimacy(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // �����鷿
    void _OnCreateWeddingRoom(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // ��ȡ������Ϣ
    void OnGetBlessingRankList(GameMsg_Base & rMsg, CSlotPeer &slotPeer);
    void _OnGetBlessingRankList(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // ��ҵ�ף��
    void OnBlessingCouple(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // ��Һ��
    void OnRedEnvelope(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // ��ȡcoupleInfo��Ϣ
    void OnGetCoupleInfo(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // ��ȡcoupleInfo��Ϣ
    void _OnGetCoupleInfo(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // ��ȡ����̬�б�
    void OnGetWeddingRecordList(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // ��ȡ����̬�б�(��group����)
    void _OnGetWeddingRecordList(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // ��ȡ����̬��ף��ֵ
    void OnGetBlessingValue(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // ��ȡ����̬��ף��ֵ(��group����)
    void _OnGetBlessingValue(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // ��ȡ����
    void OnGetPersonInfo(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // ��ȡ����(��group����)
    void _OnGetPersonInfo(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // ��ȡ��ף��������Ϣ
    void OnGetCoupleRedEnvelopeGiftInfo(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    void _OnDelRedEnvelopeMoney(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    void _OnSendBlessingErrorMsg(GameMsg_Base & rMsg, CSlotPeer &slotPeer);
    void _OnSendBlessingCoupleSucMsg(GameMsg_Base & rMsg, CSlotPeer &slotPeer);
    void _OnSendRedEnvelopeSucMsg(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    void _OnWeddingNotice(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    void RoleCoupleLog(unsigned int nMaleID, unsigned int nFemaleID, int type, int nIntimacy, int nBlessingValue);

private:
    // �㲥ף����Ϣ
    void BroadcastCreateWeddingRoomMsg();

    // �㲥����˫�����ߺ��ѿ���������Ϣ
    void BroadcastMarriageToCoupleFriends(bool isSendCurrentLine = false);
    void BroadcastOtherLineFriends(unsigned int nRoleID, std::string strMaleName, std::string strFemaleName);

public:
    // ��ȡ���Э����
    CDivorceAgreement & GetDivorceAgreement(){
        return m_DivorceAgreement;
    }

    //��ȡЭ����ID
    unsigned int GetDivorceAgreementID(){
        return m_DivorceAgreement.GetAgreementID();
    }

    // ��ȡ�Լ�Э����ID
    unsigned int GetSelfDivorceAgreementID(){
        return m_nDivorceAgreementID;
    }
    void SetSelfDivorceAgreementID(unsigned int nAgreementID){
        m_nDivorceAgreementID = nAgreementID;
    }

    // ������Ϣ
    void SerializeDivorceComponent(CParamPool &IOBuff);

    // �������
    void OnDivorceRequest(GameMsg_Base &msg, CSlotPeer & slotPeer);
    int DivorceRequest();

    // ȡ�����
    int DivorceCancel();
    void OnDivorceCancel(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // ǿ�����
    void OnDivorceMandatory(GameMsg_Base & rMsg, CSlotPeer &slotPeer);
    int DivorceMandatory();

    // ͬ�����
    void OnDivorceAgree(GameMsg_Base & rMsg, CSlotPeer &slotPeer);
    int DivorecAgree();

    // ���������Ϣ
    int DivorceHandle();

    // �������
    void OnDivorceChat(GameMsg_Base & rMsg, CSlotPeer &slotPeer);
    int DivorceChat();

    // �������
    void OnDivorceGift(GameMsg_Base & rMsg, CSlotPeer &slotPeer);
    int DivorceGift();

    // Ը����Ʒ
    void OnDivorceWishItem(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // ���MV
    void OnDivorceAnimation(GameMsg_Base & rMsg, CSlotPeer &slotPeer);
    int DivorceAnimation();
    void SetDivorcePlayerMsg(DivorcePlayerMsg & playermsg);
    void SetDivorceOfflinePlayerMsg(unsigned int nRoleID, RoleImageOfflineData & OffLineData, DivorcePlayerMsg & playermsg);
    void _GetDivorceCouplePersonInfo(GameMsg_Base & rMsg, CSlotPeer &slotPeer);

    // ����������
    void HaveNewCouple(char cThreeSex);

    // Э����Է�ID
    unsigned int GetDivorecAgreementOtherRoleID();

    // ��Ϊ֪ͨ
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

