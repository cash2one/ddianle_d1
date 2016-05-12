#include <algorithm>

#include "CoupleMsgDef.h"
#include "EntityCoupleComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../mail/EntityMailComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../gene/EntityGeneComponent.h"
#include "../vip/EntityVIPComponent.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../quest/EntityQuestComponent.h"
#include "../player/PlayerInfoComponent.h"
#include "../room/RoomComponent.h"
#include "../PhotoRoom/EntityPhotoRoomComponent.h"
#include "../AmuseRoom/AmuseRoomComponent.h"
#include "../quest/EntityQuestNewComponent.h"
#include "../Pet/PetComponent.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../../datastructure/Localization.h"
#include "../../datastructure/DataStruct_Couple.h"
#include "../friend/EntityFriendComponent.h"
#include "../mall/EntityMallComponent.h"
#include "../mail/MailMgr.h"
#include "CoupleDataStructrue.h"
#include "WeddingRoom.h"
#include "WeddingRoomManager.h"
#include "../item/ItemProcess.h"
#include "../chat/ChatManager.h"

#include "../../socket/GameMsg_Map.h"
#include "../../socket/Formatter.h"
#include "GameMsg_C2S_Couple.h"
#include "GameMsg_S2C_Couple.h"
#include "GameMsg_S2G_Couple.h"
#include "GameMsg_G2S_Couple.h"
#include "../chat/GameMsg_S2C_Chat.h"
#include "../logic/LogicCirculator.h"
#include "../../datastructure/LogStructDef.h"
#include "DivorceAgreement.h"
#include "../mall/MallDefine.h"
#include "../SystemSetting/SystemSettingMgr.h"
#include "../Dungeon/EntityDungeonComponent.h"
#include "../logic/EntityComponentEventID.h"
#include "../Pet/PetService.h"
#include "../Medal/EntityMedalComponent.h"
#include "../Horse/EntityHorseComponent.h"

extern void SendMsg2GroupServer(GameMsg_Base *pMsg);
extern CLogicCirculator* g_pCirculator;

extern unsigned short g_nCoupleRequestLevel;

extern int g_nMinIntimacyMarriageRequired;

extern int  g_nLine;

const int C_BUFF_SIZE = 512;
const int C_SEND_COUPLE_REQUEST_INTERVAL = 2;


CEntityCoupleComponent::CEntityCoupleComponent()
: CEntityComponent(true, true)
, m_pWeddingRoom(NULL)
, m_bHost(false)
, m_eRoomRoleType(EWeddingRoomRoleType_None)
, m_nLastSendCoupleRequestTime(0)
, m_nDivorceAgreementID(0)
, m_pRoleAttr(NULL)
, m_pRoleNet(NULL)
, m_pRoleItem(NULL)
, m_pRoleMail(NULL)
, m_pRoleChat(NULL)
, m_pRoleDanceGroup(NULL)
, m_pRoleGene(NULL)
, m_pRoleRoom(NULL)
, m_pRoleInfo(NULL)
, m_pRolePhotoRoom(NULL)
, m_pRoleAmuseRoom(NULL)
, m_pRoleFriend(NULL)
, m_pRoleVip(NULL)
, m_pRoleQuest(NULL)
, m_pRoleQuestNew(NULL)
, m_pRoleMall(NULL)
, m_pRoleDungeon(NULL)
, m_pRolePet(NULL)
, m_pRoleMedal(NULL)
, m_pRoleHorse(NULL)
{
}

CEntityCoupleComponent::~CEntityCoupleComponent()
{
    while (!m_listCoupleRequest.empty())
    {
        std::list<CCoupleRequest *>::iterator it = m_listCoupleRequest.begin();
        CCoupleRequest *request = *it;
        delete request;
        m_listCoupleRequest.erase(it);
    }
    m_listCoupleRequest.clear();
}

void CEntityCoupleComponent::Start()
{
    m_pRoleAttr = GetComponent<CRoleAttribute>();
    m_pRoleNet = GetComponent<CRoleNet>();
    m_pRoleItem = GetComponent<CRoleItem>();
    m_pRoleMail = GetComponent<CRoleMail>();
    m_pRoleChat = GetComponent<CRoleChat>();
    m_pRoleDanceGroup = GetComponent<CRoleDanceGroup>();
    m_pRoleGene = GetComponent<CRoleGene>();
    m_pRoleRoom = GetComponent<CRoleRoom>();
    m_pRoleInfo = GetComponent<CPlayerInfo>();
    m_pRolePhotoRoom = GetComponent<CRolePhotoRoom>();
    m_pRoleAmuseRoom = GetComponent<CRoleAmuseRoom>();
    m_pRoleFriend = GetComponent<CRoleFriend>();
    m_pRoleQuest = GetComponent<CRoleQuest>();
    m_pRoleVip = GetComponent<CRoleVIP>();
    m_pRoleQuestNew = GetComponent<CRoleQuestNew>();
    m_pRoleMall = GetComponent<CRoleMall>();
    m_pRoleDungeon = GetComponent<CRoleDungeon>();
    m_pRolePet = GetComponent<CRolePet>();
    m_pRoleMedal = GetComponent<CRoleMedal>();
    m_pRoleHorse = GetComponent<CRoleHorse>();
}

void CEntityCoupleComponent::OnEvent(CComponentEvent& refEvent)
{
    if (refEvent.nID == eComponentEvent_CoupleRequestReplay)
    {
        CCoupleRequestReplay *pReplay = (CCoupleRequestReplay *)refEvent.m_pData;

        if (pReplay == NULL)
            return;

        GameMsg_S2C_CoupleRequestReplay replayMsg;
        replayMsg.m_requestReplay = *pReplay;

        SendPlayerMsg(&replayMsg);

        if (pReplay->m_bAgree)
        {
            m_coupleInfo.m_nPairID = pReplay->m_nPairID;
            m_coupleInfo.m_nCoupleID = pReplay->m_nTargetID;
            m_coupleInfo.m_strCoupleName = pReplay->m_strTargetName;
            m_coupleInfo.m_nCoupleDate = pReplay->m_nCoupleDate;
            m_coupleInfo.m_nSmallRoleID = pReplay->m_nSmallRoleID;

            if (m_pRoleDungeon != NULL)
                m_pRoleDungeon->RemoveInviteFriendInfo(m_coupleInfo.m_nCoupleID);

            CheckCoupleQuest();
        }
    }
    else if (refEvent.nID == eComponentEvent_CoupleHasBeenSeparated)
    {
        if (m_coupleInfo.m_nPairID == 0)
            return;

        GameMsg_S2C_NotifydHasBeenSeparated notifyMsg;
        notifyMsg.m_nSeparatedBy = m_coupleInfo.m_nCoupleID;
        notifyMsg.m_strSeparatedBy = m_coupleInfo.m_strCoupleName;

        ResetCoupleData();

        SendPlayerMsg(&notifyMsg);

        CheckCoupleQuest();
    }
}

bool CEntityCoupleComponent::CreateFromDB(ROLEINFO_DB* pRoleInforDB)
{
    m_coupleInfo = pRoleInforDB->m_coupleInfo;
    if (m_coupleInfo.GetIntimacy() < 0)
    {
        m_coupleInfo.SetIntimacy(0);
    }
    else if (m_coupleInfo.GetIntimacy() > MAX_COUPLE_INTIMACY)
    {
        m_coupleInfo.SetIntimacy(MAX_COUPLE_INTIMACY);
    }

    m_nDivorceAgreementID = pRoleInforDB->m_nDivorceAgreementID;
    m_DivorceAgreement.UpdateDivorceAgreementInfo(pRoleInforDB->m_DivorceAgreementInfo);
    m_DivorceAgreement.LoadDivorceAction(pRoleInforDB->m_DivorceAction);
    return true;
}

bool CEntityCoupleComponent::PacketToDB(ROLEINFO_DB *pRoleInforDB) const
{
    return true;
}

void CEntityCoupleComponent::OnUpdate(const unsigned long &nTimeElapsed)
{
}

void CEntityCoupleComponent::SerializeComponent(CParamPool &IOBuff)
{
    m_coupleInfo.Serialize(IOBuff);
    SerializeDivorceComponent(IOBuff);
}

void CEntityCoupleComponent::SerializeDivorceComponent(CParamPool &IOBuff)
{
    IOBuff.AddChar((char)m_DivorceAgreement.GetAgreementState());
    std::string strDesc = CDivorceConfigDataMgr::Instance().GetDivorceDesc();
    IOBuff.AddStringW(strDesc);

    const CItem & divrceitem = CDivorceConfigDataMgr::Instance().GetDivorceItem();
    IOBuff.AddUInt(divrceitem.m_nItemType);

    const CItem & compulsoryitem = CDivorceConfigDataMgr::Instance().GetCompulsoryItem();
    IOBuff.AddUInt(compulsoryitem.m_nItemType);

    IOBuff.AddInt(CDivorceConfigDataMgr::Instance().GetRemarryIntimacyRate());
    IOBuff.AddUInt(m_DivorceAgreement.GetProposerID());

    IOBuff.AddUInt(m_DivorceAgreement.m_DivorceAgreementInfo.m_nSmallRoleID);
    if (m_nDivorceAgreementID > 0)
    {
        if (m_DivorceAgreement.m_DivorceAgreementInfo.m_nMaleRoleID == m_pRoleAttr->GetRoleID())
        {
            IOBuff.AddUInt(m_DivorceAgreement.m_DivorceAgreementInfo.m_nFemaleRoleID);
            IOBuff.AddStringW(m_DivorceAgreement.m_DivorceAgreementInfo.m_strFemaleRoleName);
        }
        else
        {
            IOBuff.AddUInt(m_DivorceAgreement.m_DivorceAgreementInfo.m_nMaleRoleID);
            IOBuff.AddStringW(m_DivorceAgreement.m_DivorceAgreementInfo.m_strMaleRoleName);
        }
    }
    else
    {
        IOBuff.AddUInt(0);
        IOBuff.AddStringW("");
    }

    IOBuff.AddBool(m_DivorceAgreement.GetMaleIsAgree());
    IOBuff.AddBool(m_DivorceAgreement.GetFemaleIsAgree());

    // 分手礼物
    const std::list<itemtype_t> & malegiftlist = CDivorceConfigDataMgr::Instance().GetMaleGiftList();
    IOBuff.AddUShort((unsigned short)malegiftlist.size());
    std::list<itemtype_t>::const_iterator itmale = malegiftlist.begin();
    for (; itmale != malegiftlist.end(); ++itmale)
    {
        IOBuff.AddUInt(*itmale);
    }

    const std::list<itemtype_t> & femalegiftlist = CDivorceConfigDataMgr::Instance().GetFemaleGiftList();
    IOBuff.AddUShort((unsigned short)femalegiftlist.size());
    std::list<itemtype_t>::const_iterator itfemale = femalegiftlist.begin();
    for (; itfemale != femalegiftlist.end(); ++itfemale)
    {
        IOBuff.AddUInt(*itfemale);
    }

    // 音乐列表
    const std::list<string> & musiclist = CDivorceConfigDataMgr::Instance().GetMusicList();
    IOBuff.AddUShort((unsigned short)musiclist.size());
    std::list<string>::const_iterator it = musiclist.begin();
    for (; it != musiclist.end(); ++it)
    {
        IOBuff.AddStringW(*it);
    }

    // 行为
    IOBuff.AddUShort((unsigned short)m_DivorceAgreement.m_DivorceAction.size());
    std::list<CDivorceAction>::iterator iter = m_DivorceAgreement.m_DivorceAction.begin();
    for (; iter != m_DivorceAgreement.m_DivorceAction.end(); ++iter)
    {
        IOBuff.AddUInt(iter->m_nTime);
        IOBuff.AddUInt(iter->m_nProposerID);


        if (iter->m_nProposerID == m_DivorceAgreement.m_DivorceAgreementInfo.m_nMaleRoleID)
        {
            if (m_DivorceAgreement.m_DivorceAgreementInfo.m_nSmallRoleID > 0)
            {
                if (m_pRoleAttr->GetSex() == ESexType_Male)
                {
                    IOBuff.AddChar(ESexType_Male);
                    IOBuff.AddStringW(m_DivorceAgreement.m_DivorceAgreementInfo.m_strMaleRoleName);
                    IOBuff.AddUInt(m_DivorceAgreement.m_DivorceAgreementInfo.m_nFemaleRoleID);
                    IOBuff.AddChar(ESexType_Male);
                    IOBuff.AddStringW(m_DivorceAgreement.m_DivorceAgreementInfo.m_strFemaleRoleName);
                }
                else
                {
                    IOBuff.AddChar(ESexType_Female);
                    IOBuff.AddStringW(m_DivorceAgreement.m_DivorceAgreementInfo.m_strMaleRoleName);
                    IOBuff.AddUInt(m_DivorceAgreement.m_DivorceAgreementInfo.m_nFemaleRoleID);
                    IOBuff.AddChar(ESexType_Female);
                    IOBuff.AddStringW(m_DivorceAgreement.m_DivorceAgreementInfo.m_strFemaleRoleName);
                }
            }
            else
            {
                if (m_pRoleAttr->GetSex() == ESexType_Male)
                {
                    IOBuff.AddChar(ESexType_Male);
                    IOBuff.AddStringW(m_DivorceAgreement.m_DivorceAgreementInfo.m_strMaleRoleName);
                    IOBuff.AddUInt(m_DivorceAgreement.m_DivorceAgreementInfo.m_nFemaleRoleID);
                    IOBuff.AddChar(ESexType_Female);
                    IOBuff.AddStringW(m_DivorceAgreement.m_DivorceAgreementInfo.m_strFemaleRoleName);
                }
                else
                {
                    IOBuff.AddChar(ESexType_Female);
                    IOBuff.AddStringW(m_DivorceAgreement.m_DivorceAgreementInfo.m_strMaleRoleName);
                    IOBuff.AddUInt(m_DivorceAgreement.m_DivorceAgreementInfo.m_nFemaleRoleID);
                    IOBuff.AddChar(ESexType_Male);
                    IOBuff.AddStringW(m_DivorceAgreement.m_DivorceAgreementInfo.m_strFemaleRoleName);
                }
            }
        }
        else
        {
            if (m_DivorceAgreement.m_DivorceAgreementInfo.m_nSmallRoleID > 0)
            {
                if (m_pRoleAttr->GetSex() == ESexType_Male)
                {
                    IOBuff.AddChar(ESexType_Male);
                    IOBuff.AddStringW(m_DivorceAgreement.m_DivorceAgreementInfo.m_strFemaleRoleName);
                    IOBuff.AddUInt(m_DivorceAgreement.m_DivorceAgreementInfo.m_nMaleRoleID);
                    IOBuff.AddChar(ESexType_Male);
                    IOBuff.AddStringW(m_DivorceAgreement.m_DivorceAgreementInfo.m_strMaleRoleName);
                }
                else
                {
                    IOBuff.AddChar(ESexType_Female);
                    IOBuff.AddStringW(m_DivorceAgreement.m_DivorceAgreementInfo.m_strFemaleRoleName);
                    IOBuff.AddUInt(m_DivorceAgreement.m_DivorceAgreementInfo.m_nMaleRoleID);
                    IOBuff.AddChar(ESexType_Female);
                    IOBuff.AddStringW(m_DivorceAgreement.m_DivorceAgreementInfo.m_strMaleRoleName);
                }
            }
            else
            {
                if (m_pRoleAttr->GetSex() == ESexType_Male)
                {
                    IOBuff.AddChar(ESexType_Male);
                    IOBuff.AddStringW(m_DivorceAgreement.m_DivorceAgreementInfo.m_strFemaleRoleName);
                    IOBuff.AddUInt(m_DivorceAgreement.m_DivorceAgreementInfo.m_nMaleRoleID);
                    IOBuff.AddChar(ESexType_Female);
                    IOBuff.AddStringW(m_DivorceAgreement.m_DivorceAgreementInfo.m_strMaleRoleName);
                }
                else
                {
                    IOBuff.AddChar(ESexType_Female);
                    IOBuff.AddStringW(m_DivorceAgreement.m_DivorceAgreementInfo.m_strFemaleRoleName);
                    IOBuff.AddUInt(m_DivorceAgreement.m_DivorceAgreementInfo.m_nMaleRoleID);
                    IOBuff.AddChar(ESexType_Male);
                    IOBuff.AddStringW(m_DivorceAgreement.m_DivorceAgreementInfo.m_strMaleRoleName);
                }
            }
        }

        IOBuff.AddChar(iter->m_eType);
        IOBuff.AddStringW(iter->m_strChatContent);
        iter->m_SendItem.doEncode(IOBuff);
    }
}

const char* CEntityCoupleComponent::SerializeName()
{
    return "Couple";
}

void CEntityCoupleComponent::OnLogin()
{
    if (!IsMarried())
    {
        DeleteMarriedItem();
    }
}

void CEntityCoupleComponent::OnLogout()
{
    CWeddingRoomManager::Instance().PlayerLogout(this);
}

void CEntityCoupleComponent::RegComponentNetMsgMap()
{
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_SendCoupleRequest);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_AcceptCoupleRequest);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RefuseCoupleRequest);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_CoupleSeparateRequest);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_NotifyHasBeenSeparated);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_NotifyHasBeenDivorced);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_CreateWeddingRoom);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_TryEnterWeddingRoom);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_EnterWeddingRoom);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_EnterMyWeddingRoom);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ExitWeddingRoom);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RequestMarry);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RequestDivorce);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetWeddingRoomList);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_KickWeddingRoomPlayer);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_SetWeddingPlayerType);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ChangeWeddingRoomPwd);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_BlessingCouple);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetBlessingRankList);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_MoneyBlessingPair);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetCoupleInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetWeddingRecordList);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetBlessingValue);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetPersonInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetCoupleRedEnvelopeGiftInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_CancelDivorce);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_CompulsoryDivorce);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_AgreeDivorce);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_DivorceChat);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_DivorceGift);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_DivorceWish);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_DivorceAnimation);



    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_UpdateCoupleIntimacy);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_WeddingRoomStartingNofication);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_CoupleNameChanged);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_AutoStartWeddingFail);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_GetBlessingRankList);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_GetCoupleInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_GetWeddingRecordList);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_GetBlessingValue);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_GetPersonInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_BlessingError);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_BlessingCoupleScuess);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_RedEnvelopeScuess);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_WeddingNotice);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_RedEnvelopeDelMoney);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_RefreshDivorceAction);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_GetDivorceeAnimationPersonInfo);



    RegMsgProcessor(MSG_C2S_SendCoupleRequest, &CEntityCoupleComponent::_OnSendCoupleRequest);
    RegMsgProcessor(MSG_C2S_AcceptCoupleRequest, &CEntityCoupleComponent::_OnAcceptCoupleRequest);
    RegMsgProcessor(MSG_C2S_RefuseCoupleRequest, &CEntityCoupleComponent::_OnRefuseCoupleRequest);
    RegMsgProcessor(MSG_C2S_CoupleSeparateRequest, &CEntityCoupleComponent::_OnCoupleSeparateRequest);
    RegMsgProcessor(MSG_G2S_NotifyHasBeenSeparated, &CEntityCoupleComponent::_OnNotifyHasBeenSeparated);
    RegMsgProcessor(MSG_G2S_NofiyHasBeenDivorced, &CEntityCoupleComponent::_NotifyHasBeenDivorced);
    RegMsgProcessor(MSG_C2S_CreateWeddingRoom, &CEntityCoupleComponent::_OnCreateWeddingRoom);
    RegMsgProcessor(MSG_C2S_TryEnterWeddingRoom, &CEntityCoupleComponent::_OnTyrEnterWeddingRoom);
    RegMsgProcessor(MSG_C2S_EnterWeddingRoom, &CEntityCoupleComponent::_OnEnterWeddingRoom);
    RegMsgProcessor(MSG_C2S_RequestEnterMyWeddingRoom, &CEntityCoupleComponent::_OnEnterMyWeddingRoom);
    RegMsgProcessor(MSG_C2S_ExitWeddingRoom, &CEntityCoupleComponent::_OnExitWeddingRoom);
    RegMsgProcessor(MSG_C2S_RequestMarry, &CEntityCoupleComponent::_OnRequestMarry);
    RegMsgProcessor(MSG_C2S_GetWeddingRoomList, &CEntityCoupleComponent::_OnGetWeddingRoomList);
    RegMsgProcessor(MSG_C2S_KickWeddingRoomPlayer, &CEntityCoupleComponent::_OnKickWeddingRoomPlayer);
    RegMsgProcessor(MSG_C2S_SetWeddingPlayerType, &CEntityCoupleComponent::_OnSetWeddingPlayerType);
    RegMsgProcessor(MSG_C2S_ChangeWeddingRoomPwd, &CEntityCoupleComponent::_OnChangeWeddingRoomPwd);
    RegMsgProcessor(MSG_C2S_BlessingPair, &CEntityCoupleComponent::OnBlessingCouple);
    RegMsgProcessor(MSG_C2S_GetBlessingRankList, &CEntityCoupleComponent::OnGetBlessingRankList);
    RegMsgProcessor(MSG_C2S_MoneyBlessingPair, &CEntityCoupleComponent::OnRedEnvelope);
    RegMsgProcessor(MSG_C2S_GetCoupleInfo, &CEntityCoupleComponent::OnGetCoupleInfo);
    RegMsgProcessor(MSG_C2S_GetWeddingRecordList, &CEntityCoupleComponent::OnGetWeddingRecordList);
    RegMsgProcessor(MSG_C2S_GetBlessingValue, &CEntityCoupleComponent::OnGetBlessingValue);
    RegMsgProcessor(MSG_C2S_GetPersonInfo, &CEntityCoupleComponent::OnGetPersonInfo);
    RegMsgProcessor(MSG_C2S_GetCoupleRedEnvelopeGiftInfo, &CEntityCoupleComponent::OnGetCoupleRedEnvelopeGiftInfo);
    RegMsgProcessor(MSG_C2S_RequestCancelDivorce, &CEntityCoupleComponent::OnDivorceCancel);
    RegMsgProcessor(MSG_C2S_RequestCompulsoryDivorce, &CEntityCoupleComponent::OnDivorceMandatory);
    RegMsgProcessor(MSG_C2S_AgreeDivorce, &CEntityCoupleComponent::OnDivorceAgree);
    RegMsgProcessor(MSG_C2S_DivorceChat, &CEntityCoupleComponent::OnDivorceChat);
    RegMsgProcessor(MSG_C2S_DivorceSendGift, &CEntityCoupleComponent::OnDivorceGift);
    RegMsgProcessor(MSG_C2S_RequestCoupleWish, &CEntityCoupleComponent::OnDivorceWishItem);
    RegMsgProcessor(MSG_C2S_RequestEnterDivorceAnimation, &CEntityCoupleComponent::OnDivorceAnimation);
    RegMsgProcessor(MSG_C2S_RequestDivorce, &CEntityCoupleComponent::OnDivorceRequest);


    RegMsgProcessor(MSG_G2S_UpdateCoupleIntimacy, &CEntityCoupleComponent::_OnChangeIntimacy);
    RegMsgProcessor(MSG_G2S_WeddingRoomStaringNofication, &CEntityCoupleComponent::_OnRcvWeddingStartingRoomNofication);
    RegMsgProcessor(MSG_G2S_CoupleNameChanged, &CEntityCoupleComponent::_OnRoleNameChanged);
    RegMsgProcessor(MSG_G2S_AutoStartWeddingRoomFail, &CEntityCoupleComponent::_OnRcvAutoStartWeddingRoomFail);
    //RegMsgProcessor(MSG_G2S_BlessingPair, &CEntityCoupleComponent::_OnBlessingPair);
    RegMsgProcessor(MSG_G2S_GetBlessingRankList, &CEntityCoupleComponent::_OnGetBlessingRankList);
    RegMsgProcessor(MSG_G2S_GetCoupleInfo, &CEntityCoupleComponent::_OnGetCoupleInfo);
    RegMsgProcessor(MSG_G2S_GetWeddingRecordList, &CEntityCoupleComponent::_OnGetWeddingRecordList);
    RegMsgProcessor(MSG_G2S_GetBlessingValue, &CEntityCoupleComponent::_OnGetBlessingValue);
    RegMsgProcessor(MSG_G2S_GetPersonInfo, &CEntityCoupleComponent::_OnGetPersonInfo);
    RegMsgProcessor(MSG_G2S_BlessingError, &CEntityCoupleComponent::_OnSendBlessingErrorMsg);
    RegMsgProcessor(MSG_G2S_MoneyBlessingPairSuc, &CEntityCoupleComponent::_OnSendRedEnvelopeSucMsg);
    RegMsgProcessor(MSG_G2S_BlessingPairSuc, &CEntityCoupleComponent::_OnSendBlessingCoupleSucMsg);
    RegMsgProcessor(MSG_G2S_WeddingNotice, &CEntityCoupleComponent::_OnWeddingNotice);
    RegMsgProcessor(MSG_G2S_RedEnvelopeDelMoney, &CEntityCoupleComponent::_OnDelRedEnvelopeMoney);
    RegMsgProcessor(MSG_G2S_RefreshDivorceAction, &CEntityCoupleComponent::OnDivorceAnimation);
    RegMsgProcessor(MSG_G2S_GetDivorceAnimationPersonInfo, &CEntityCoupleComponent::_GetDivorceCouplePersonInfo);


    RegQueryProcessor(QUERY_Couple_AddCouple, &CEntityCoupleComponent::_OnAddCoupleCallBack);
    RegQueryProcessor(QUERY_Couple_AddDivorceAgreement, &CEntityCoupleComponent::_OnAddDivorceAgreementCallBack);
}

CEntityAttributeComponent * CEntityCoupleComponent::RoleAttr()
{
    return m_pRoleAttr;
}

CEntityItemComponent * CEntityCoupleComponent::RoleItem()
{
    return m_pRoleItem;
}

CEntityMailComponent * CEntityCoupleComponent::RoleMail()
{
    return m_pRoleMail;
}

CEntityChatComponent * CEntityCoupleComponent::RoleChat()
{
    return m_pRoleChat;
}

CRoomComponent * CEntityCoupleComponent::RoleStageRoom()
{
    return m_pRoleRoom;
}

CEntityPhotoRoomComponent * CEntityCoupleComponent::RolePhotoRoom()
{
    return m_pRolePhotoRoom;
}

CAmuseRoomComponent * CEntityCoupleComponent::RoleAmuseRoom()
{
    return m_pRoleAmuseRoom;
}

CEntityDanceGroupComponent  * CEntityCoupleComponent::RoleDanceGroup()
{
    return m_pRoleDanceGroup;
}


void CEntityCoupleComponent::_OnSendCoupleRequest(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_C2S_SendCoupleRequest & msgRequest = (GameMsg_C2S_SendCoupleRequest &)msg;

    time_t nNow = time(NULL);
    int nError = ESendCoupleRequest_Suc;
    std::string strError = "";
    CRoleEntity *pTarget = NULL;

    if (m_pRoleAttr == NULL || m_pRoleItem == NULL)
    {
        nError = ESendCoupleRequest_Unknown;
        strError = "ESendCoupleRequest_Unknown";
    }
    else if (m_pRoleAttr->GetRoleID() == msgRequest.m_nTargetID)
    {
        nError = ESendCoupleRequest_CanNotAddSelf;
        strError = "ESendCoupleRequest_CanNotAddSelf";
    }
    else if (m_nLastSendCoupleRequestTime != 0 && m_nLastSendCoupleRequestTime - nNow < C_SEND_COUPLE_REQUEST_INTERVAL)
    {
        nError = ESendCoupleRequest_RequestTooFrequent;
        strError = "ESendCoupleRequest_RequestTooFrequent";
    }
    else if (m_pRoleAttr->GetLevel() < g_nCoupleRequestLevel)
    {
        nError = ESendCoupleRequest_LackOfLevel;
        strError = "ESendCoupleRequest_LackOfLevel";
    }
    else if (IsDivorceing(msgRequest.m_nTargetID))
    {
        nError = ESendCoupleRequest_IsDivorceing;
        strError = "ESendCoupleRequest_IsDivorceing";
    }
    else if (!CanHaveNewCouple(msgRequest.m_nTargetID))
    {
        nError = ESendCoupleRequest_NotSingle;
        strError = "ESendCoupleRequest_NotSingle";
    }
    else if (m_pRoleItem->SearchItem(C_COUPLE_REQUEST_ITEM) == NULL)
    {
        nError = ESendCoupleRequest_LackOfRequiredItem;
        strError = "ESendCoupleRequest_LackOfRequiredItem";
    }
    else if ((pTarget = CPlayerManager::Instance().GetEntityByRoleID(msgRequest.m_nTargetID)) == NULL)
    {
        nError = ESendCoupleRequest_RcverNotOnlineOrNotOnCurLine;
        strError = "ESendCoupleRequest_RcverNotOnlineOrNotOnCurLine";
    }
    else if (!pTarget->GetRoleCouple()->CanHaveNewCouple(m_pRoleAttr->GetRoleID()))
    {
        nError = ESendCoupleRequest_RcverNotSingle;
        strError = "ESendCoupleRequest_RcverNotSingle";
    }
    else if (pTarget->GetLevel() < g_nCoupleRequestLevel)
    {
        nError = ESendCoupleRequest_RcverLackOfLevel;
        strError = "ESendCoupleRequest_RcverLackOfLevel";
    }
    // 	else if (m_pRoleAttr->GetSex() == SexType_No || pTarget->GetSex() == SexType_No || m_pRoleAttr->GetSex() == pTarget->GetSex()) 
    // 	{
    // 		nError = ESendCoupleRequest_SexError;
    // 		strError = "ESendCoupleRequest_SexError";
    // 	}
    else if (!CheckCoupleSex(m_pRoleAttr->GetSex(), pTarget->GetSex()))
    {
        nError = ESendCoupleRequest_SexError;
        strError = "ESendCoupleRequest_SexError";
    }
    else
    {
        if (!m_pRoleItem->RemoveItemByType(C_COUPLE_REQUEST_ITEM, 1, EItemAction_Del_SendCoupleRequest))
        {
            nError = ESendCoupleRequest_Unknown;
            strError = "ESendCoupleRequest_Unknown";
        }
        else
        {
            GameMsg_S2C_SendCoupleRequestSuc msgSuc;
            SendPlayerMsg(&msgSuc);

            CRoleCouple *pRoleCouple = pTarget->GetComponent<CRoleCouple>();
            if (pRoleCouple != NULL)
            {
                CCoupleRequest *pCoupleRequest = new CCoupleRequest;
                pCoupleRequest->m_nRoleID = m_pRoleAttr->m_nRoleID;
                pCoupleRequest->m_strName = m_pRoleAttr->GetRoleName();
                pRoleCouple->_AddCoupleRequest(pCoupleRequest);
            }

            GameMsg_S2C_ReceiveCoupleRequest msgRecieve;
            msgRecieve.m_nSenderID = m_pRoleAttr->GetRoleID();
            msgRecieve.m_strSenderName = m_pRoleAttr->GetRoleName();
            pTarget->SendPlayerMsg(&msgRecieve);
        }
    }

    if (nError != ESendCoupleRequest_Suc)
    {
        GameMsg_S2C_SendCoupleRequestFail msgFail;
        msgFail.m_nError = (unsigned char)nError;
        msgFail.m_strError = strError;
        SendPlayerMsg(&msgFail);
    }
}

void CEntityCoupleComponent::_OnAcceptCoupleRequest(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_C2S_AcceptCoupleRequest & msgAccept = (GameMsg_C2S_AcceptCoupleRequest &)msg;

    std::string strError = "";
    std::string strErrorForOther;
    CRoleEntity *pTarget = NULL;
    if (m_pRoleAttr == NULL)
    {
        strError = "EAcceptCoupleRequest_DataError";
    }
    else if (!_HasReceiveCoupleRequest(msgAccept.m_nTargetID))
    {
        strError = "EAcceptCoupleRequest_RequestHasProcessed";
    }
    else if (m_pRoleAttr->GetLevel() < g_nCoupleRequestLevel)
    {
        strError = "EAcceptCoupleRequest_SelfLackOfLevel";
        strErrorForOther = "EAcceptCoupleRequest_OtherLackOfLevel";
    }
    else if (IsDivorceing(msgAccept.m_nTargetID))
    {
        strError = "ESendCoupleRequest_IsDivorceing";
        strErrorForOther = "ESendCoupleRequest_IsDivorceing";
    }
    else if (!CanHaveNewCouple(msgAccept.m_nTargetID))
    {
        strError = "EAcceptCoupleRequest_SelfNotSingle";
        strErrorForOther = "EAcceptCoupleRequest_OtherNotSingle";
    }
    else if ((pTarget = CPlayerManager::Instance().GetEntityByRoleID(msgAccept.m_nTargetID)) == NULL)
    {
        strError = "EAcceptCoupleRequest_OtherNotOnlineOrNotOnCurLine";
    }
    else if (pTarget->GetRoleCouple()->IsDivorceing(m_pRoleAttr->GetRoleID()))
    {
        strError = "ESendCoupleRequest_IsDivorceing";
        strErrorForOther = "ESendCoupleRequest_IsDivorceing";
    }
    else if (!pTarget->GetRoleCouple()->CanHaveNewCouple(m_pRoleAttr->GetRoleID()))
    {
        strError = "EAcceptCoupleRequest_OtherNotSingle";
        strErrorForOther = "EAcceptCoupleRequest_SelfNotSingle";
    }
    else if (pTarget->GetLevel() < g_nCoupleRequestLevel)
    {
        strError = "EAcceptCoupleRequest_OtherLackOfLevel";
        strErrorForOther = "EAcceptCoupleRequest_SelfLackOfLevel";
    }
    //     else if (m_pRoleAttr->GetSex() == SexType_No || pTarget->GetSex() == SexType_No || m_pRoleAttr->GetSex() == pTarget->GetSex()) 
    //     {
    // 		strError = "EAcceptCoupleRequest_Sex_Error";
    // 		strErrorForOther = "EAcceptCoupleRequest_Sex_Error";
    // 	}
    else if (!CheckCoupleSex(m_pRoleAttr->GetSex(), pTarget->GetSex()))
    {
        strError = "EAcceptCoupleRequest_Sex_Error";
        strErrorForOther = "EAcceptCoupleRequest_Sex_Error";
    }
    else
    {
        if (CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_NewDivorce))
        {
            if (m_nDivorceAgreementID > 0)
            {
                HaveNewCouple(pTarget->GetSex());
            }

            if (pTarget->GetRoleCouple()->GetDivorceAgreementID() > 0)
            {
                pTarget->GetRoleCouple()->HaveNewCouple(m_pRoleAttr->GetSex());
            }
        }

        unsigned int nTimeNow = (unsigned int)time(NULL);
        CCoupleInfoDB *pCoupleInfo = new CCoupleInfoDB;
        if (pCoupleInfo != NULL)
        {
            if (CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_Homosexual))
            {
                if (pTarget->GetSex() == m_pRoleAttr->GetSex())
                {
                    pCoupleInfo->m_nMaleID = pTarget->GetRoleID();
                    pCoupleInfo->m_nFemaleID = m_pRoleAttr->GetRoleID();
                    pCoupleInfo->m_nSmallRoleID = m_pRoleAttr->GetRoleID();
                }
                else
                {
                    pCoupleInfo->m_nMaleID = m_pRoleAttr->GetSex() == ESexType_Male ? m_pRoleAttr->GetRoleID() : pTarget->GetRoleID();
                    pCoupleInfo->m_nFemaleID = m_pRoleAttr->GetSex() == ESexType_Female ? m_pRoleAttr->GetRoleID() : pTarget->GetRoleID();
                }
            }
            else
            {
                pCoupleInfo->m_nMaleID = m_pRoleAttr->GetSex() == ESexType_Male ? m_pRoleAttr->GetRoleID() : pTarget->GetRoleID();
                pCoupleInfo->m_nFemaleID = m_pRoleAttr->GetSex() == ESexType_Female ? m_pRoleAttr->GetRoleID() : pTarget->GetRoleID();
            }

            pCoupleInfo->m_nCoupleDate = nTimeNow;

            CDelHelper delHelper(pCoupleInfo);
            AddQuery(QUERY_Couple_AddCouple, 0, pCoupleInfo, msgAccept.m_nTargetID, NULL, &delHelper);
        }
    }

    if (strError != "")
    {
        GameMsg_S2C_AcceptCoupleRequestFail msgFail;
        msgFail.m_strError = strError;
        SendPlayerMsg(&msgFail);
    }

    _RemoveCoupleRequest(msgAccept.m_nTargetID);
}

void CEntityCoupleComponent::_OnRefuseCoupleRequest(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
    GameMsg_C2S_RefuseCoupleRequest &msgRefuse = dynamic_cast<GameMsg_C2S_RefuseCoupleRequest &>(msg);

    if (_HasReceiveCoupleRequest(msgRefuse.m_nTargetID))
    {
        _RemoveCoupleRequest(msgRefuse.m_nTargetID);

        CRoleEntity *pRequestor = CPlayerManager::Instance().GetEntityByRoleID(msgRefuse.m_nTargetID);

        if (pRequestor != NULL && m_pRoleAttr != NULL)
        {
            CCoupleRequestReplay replay;
            replay.m_nPairID = 0;
            replay.m_nTargetID = m_pRoleAttr->GetRoleID();
            replay.m_strTargetName = m_pRoleAttr->GetRoleName();
            replay.m_nCoupleDate = 0;
            replay.m_bAgree = false;
            replay.m_strReason = msgRefuse.m_strReason;
            replay.m_nSmallRoleID = 0;

            pRequestor->SendEvent(eComponentEvent_CoupleRequestReplay, &replay);
        }
    }
}

void CEntityCoupleComponent::_OnCoupleSeparateRequest(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
    CWeddingRoomManager::Instance().OnCoupleSeparated(msg, *this);
}

void CEntityCoupleComponent::_OnNotifyHasBeenSeparated(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
    if (!IsSingle()) {
        GameMsg_G2S_NotifyHasBeenSeparated & msgSeparated = (GameMsg_G2S_NotifyHasBeenSeparated &)msg;
        m_coupleInfo.m_nPairID = 0;
        m_coupleInfo.m_nCoupleID = 0;
        m_coupleInfo.m_nCoupleDate = 0;

        GameMsg_S2C_NotifydHasBeenSeparated msgNotify;
        msgNotify.m_nSeparatedBy = msgSeparated.m_nSeparatedBy;
        msgNotify.m_strSeparatedBy = msgSeparated.m_strSeparatedBy;
        SendPlayerMsg(&msgNotify);
    }
}

void CEntityCoupleComponent::_OnRcvWeddingStartingRoomNofication(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
    GameMsg_G2S_WeddingRoomStartingNofication & msgReceived = (GameMsg_G2S_WeddingRoomStartingNofication &)msg;

    GameMsg_S2C_WeddingRoomStartingNofication msgNofication;
    msgNofication.m_nLine = msgReceived.m_nLine;
    msgNofication.m_strLineName = msgReceived.m_strLineName;
    msgNofication.m_nRoomID = msgReceived.m_nRoomID;
    msgNofication.m_nStartTime = msgReceived.m_nStartTime;

    SendPlayerMsg(&msgNofication);
}

void CEntityCoupleComponent::_OnRcvAutoStartWeddingRoomFail(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
    GameMsg_G2S_AutoStartWeddingFail & msgReceived = (GameMsg_G2S_AutoStartWeddingFail &)msg;

    GameMsg_S2C_AutoStartWeddingRoomFail msgNofication;
    msgNofication.m_nError = msgReceived.m_nError;
    msgNofication.m_strError = msgReceived.m_strError;

    SendPlayerMsg(&msgNofication);
}

void CEntityCoupleComponent::_OnGetWeddingRoomList(GameMsg_Base &rMsg, CSlotPeer & slotPeer)
{
    CWeddingRoomManager::Instance().OnGetWeddingRoomList(rMsg, *this);
}

void CEntityCoupleComponent::_OnRequestMarry(GameMsg_Base &rMsg, CSlotPeer & slotPeer)
{
    CWeddingRoomManager::Instance().OnStartWedding(rMsg, *this);
}

int CEntityCoupleComponent::DivorceRequest()
{
    if (!CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_NewDivorce))
    {
        return EDivorceMsgError_FunctionNotOpen;
    }

    // 是否结婚
    if (!IsMarried())
    {
        return EDivorceMsgError_NotMarried;
    }

    // 消耗物品
    const CItem & costitem = CDivorceConfigDataMgr::Instance().GetDivorceItem();
    if (m_pRoleItem->SearchItem(costitem.m_nItemType) == NULL)
    {
        return EDivorceMsgError_NotDivorceItem;
    }

    // 生成协议书
    CDivorceAgreementInfo agreementinfo;
    if (!CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_Homosexual) || m_coupleInfo.m_nSmallRoleID == 0)
    {
        if (m_pRoleAttr->GetSex() == ESexType_Male)
        {
            agreementinfo.m_nMaleRoleID = m_pRoleAttr->GetRoleID();
            agreementinfo.m_strMaleRoleName = m_pRoleAttr->GetRoleName();
            agreementinfo.m_nFemaleRoleID = m_coupleInfo.m_nCoupleID;
            agreementinfo.m_strFemaleRoleName = m_coupleInfo.m_strCoupleName;
            agreementinfo.m_bMaleIsAgree = true;
        }
        else
        {
            agreementinfo.m_nFemaleRoleID = m_pRoleAttr->GetRoleID();
            agreementinfo.m_strFemaleRoleName = m_pRoleAttr->GetRoleName();
            agreementinfo.m_nMaleRoleID = m_coupleInfo.m_nCoupleID;
            agreementinfo.m_strMaleRoleName = m_coupleInfo.m_strCoupleName;
            agreementinfo.m_bFemaleIsAgree = true;
        }
    }
    else
    {
        if (m_coupleInfo.m_nSmallRoleID == m_pRoleAttr->GetRoleID())
        {
            agreementinfo.m_nMaleRoleID = m_coupleInfo.m_nCoupleID;
            agreementinfo.m_strMaleRoleName = m_coupleInfo.m_strCoupleName;
            agreementinfo.m_nFemaleRoleID = m_pRoleAttr->GetRoleID();
            agreementinfo.m_strFemaleRoleName = m_pRoleAttr->GetRoleName();
            agreementinfo.m_bFemaleIsAgree = true;
        }
        else
        {
            agreementinfo.m_nMaleRoleID = m_pRoleAttr->GetRoleID();
            agreementinfo.m_strMaleRoleName = m_pRoleAttr->GetRoleName();
            agreementinfo.m_nFemaleRoleID = m_coupleInfo.m_nCoupleID;
            agreementinfo.m_strFemaleRoleName = m_coupleInfo.m_strCoupleName;
            agreementinfo.m_bMaleIsAgree = true;
        }
    }

    agreementinfo.m_nSmallRoleID = m_coupleInfo.m_nSmallRoleID;
    agreementinfo.m_nProposerID = m_pRoleAttr->GetRoleID();
    agreementinfo.m_eState = eDivorceState_Running;

    CDivorceAgreementInfo * pagreementinfo = new CDivorceAgreementInfo(agreementinfo);
    AddQuery(QUERY_Couple_AddDivorceAgreement, 0, pagreementinfo);

    return EDivorceMsgError_Suc;
}

void CEntityCoupleComponent::OnDivorceCancel(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    int nError = DivorceCancel();
    if (nError != EDivorceMsgError_Suc)
    {
        GameMsg_S2C_CancelDivorceFail failmsg;
        failmsg.m_cErrorType = (char)nError;
        SendPlayerMsg(&failmsg);
    }
    else
    {
        GameMsg_S2C_CancelDivorceSuc sucmsg;
        SendPlayerMsg(&sucmsg);
    }
}

int CEntityCoupleComponent::DivorceCancel()
{
    if (!CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_NewDivorce))
    {
        return EDivorceMsgError_FunctionNotOpen;
    }

    // 协议书不存在
    if (m_nDivorceAgreementID == 0 || m_DivorceAgreement.m_DivorceAgreementInfo.GetAgreementID() == 0)
    {
        return EDivorceMsgError_AgreementNot;
    }

    // 协议书状态不对
    if (m_DivorceAgreement.m_DivorceAgreementInfo.GetState() != eDivorceState_Running)
    {
        return EDivorceMsgError_AgreementStateErr;
    }

    if (m_DivorceAgreement.GetProposerID() != m_pRoleAttr->GetRoleID())
    {
        return EDivorceMsgError_NotRightPeople;
    }

    // 销毁自己的协议书
    m_DivorceAgreement.UpdateDivorceAgreementState(eDivorceState_End);
    int nDelIntimacy = (int)(m_coupleInfo.GetIntimacy()*(0.2));
    ChangeIntimacy(-nDelIntimacy, EChangeIntimacyCause_Remarry, 0);

    // 行为
    CDivorceAction action;
    action.m_eType = eDivorceAction_CancelaApply;
    action.m_nAgreementID = m_DivorceAgreement.GetAgreementID();
    action.m_nProposerID = m_pRoleAttr->GetRoleID();
    action.m_nTime = (unsigned int)time(NULL);
    m_DivorceAgreement.AddDivorceAction(action);

    // 处理对方
    CRoleEntity * pCoupleEntity = CPlayerManager::Instance().GetEntityByRoleID(m_coupleInfo.m_nCoupleID);
    if (pCoupleEntity != NULL)
    {
        CDivorceAgreement & agreement = pCoupleEntity->GetRoleCouple()->GetDivorceAgreement();
        agreement.UpdateDivorceAgreementState(eDivorceState_End);
        agreement.AddDivorceAction(action, false, false, false);
    }

    // 更新状态DB
    AddQuery(QUERY_Couple_UpdateDivorceAgreementState, m_DivorceAgreement.GetAgreementID(), NULL, eDivorceState_End);

    return EDivorceMsgError_Suc;
}

void CEntityCoupleComponent::OnDivorceMandatory(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    int nError = DivorceMandatory();
    if (nError != EDivorceMsgError_Suc)
    {
        GameMsg_S2C_MandatoryDivorceFail failmsg;
        failmsg.m_cErrorType = (char)nError;
        SendPlayerMsg(&failmsg);
    }
    else
    {
        GameMsg_S2C_MandatoryDivorceSuc sucmsg;
        SendPlayerMsg(&sucmsg);
    }
}

int CEntityCoupleComponent::DivorceMandatory()
{
    if (!CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_NewDivorce))
    {
        return EDivorceMsgError_FunctionNotOpen;
    }

    if (!IsMarried())
    {
        return EDivorceMsgError_NotMarried;
    }

    // 协议书不存在
    if (m_nDivorceAgreementID == 0 || m_DivorceAgreement.m_DivorceAgreementInfo.GetAgreementID() == 0)
    {
        return EDivorceMsgError_AgreementNot;
    }

    // 	if ( m_DivorceAgreement.GetProposerID() != m_pRoleAttr->GetRoleID() )
    // 	{
    // 		return EDivorceMsgError_NotRightPeople;
    // 	}

    // 协议书状态不对
    if (m_DivorceAgreement.m_DivorceAgreementInfo.GetState() != eDivorceState_Running)
    {
        return EDivorceMsgError_AgreementStateErr;
    }

    // 道具判断
    const CItem & costitem = CDivorceConfigDataMgr::Instance().GetCompulsoryItem();
    if (costitem.m_nItemType == 0 || costitem.m_nItemCount == 0)
    {
        return EDivorceMsgError_NotMandatoryDivorceItem;
    }

    if (m_pRoleItem->SearchItem(costitem.m_nItemType) == NULL)
    {
        return EDivorceMsgError_NotMandatoryDivorceItem;
    }

    // 销毁自己的协议书
    m_nDivorceAgreementID = 0;
    m_DivorceAgreement.UpdateDivorceAgreementState(eDivorceState_End);
    AddQuery(QUERY_Couple_UpdateCoupleAgreementID, m_pRoleAttr->GetRoleID(), NULL, 0);

    // 扣除道具
    m_pRoleItem->RemoveItemByType(costitem.m_nItemType, 1, EItemAction_Del_MandatoryDivorce);

    // 行为
    CDivorceAction action;
    action.m_eType = eDivorceAction_Compulsory;
    action.m_nProposerID = m_pRoleAttr->GetRoleID();
    action.m_nTime = (unsigned int)time(NULL);
    action.m_nAgreementID = m_DivorceAgreement.GetAgreementID();
    m_DivorceAgreement.AddDivorceAction(action);

    // 处理对方
    CRoleEntity * pCoupleEntity = CPlayerManager::Instance().GetEntityByRoleID(m_coupleInfo.m_nCoupleID);
    if (pCoupleEntity != NULL)
    {
        //pCoupleEntity->GetRoleCouple()->SetSelfDivorceAgreementID(0);
        CDivorceAgreement & agreement = pCoupleEntity->GetRoleCouple()->GetDivorceAgreement();
        agreement.UpdateDivorceAgreementState(eDivorceState_End);
        agreement.AddDivorceAction(action, false, false, false);
    }

    // 更新状态DB
    AddQuery(QUERY_Couple_UpdateDivorceAgreementState, m_DivorceAgreement.GetAgreementID(), NULL, eDivorceState_End);

    // 离婚处理
    DivorceHandle();

    return EDivorceMsgError_Suc;
}

void CEntityCoupleComponent::OnDivorceAgree(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    int nError = DivorecAgree();
    if (nError != EDivorceMsgError_Suc)
    {
        GameMsg_S2C_AgreeDivorceFail failmsg;
        failmsg.m_cErrorType = (char)nError;
        SendPlayerMsg(&failmsg);
    }
    else
    {
        GameMsg_S2C_AgreeDivorceSuc sucmsg;
        SendPlayerMsg(&sucmsg);
    }
}

int CEntityCoupleComponent::DivorecAgree()
{
    if (!CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_NewDivorce))
    {
        return EDivorceMsgError_FunctionNotOpen;
    }

    // 协议书不存在
    if (m_nDivorceAgreementID == 0 || m_DivorceAgreement.m_DivorceAgreementInfo.GetAgreementID() == 0)
    {
        return EDivorceMsgError_AgreementNot;
    }

    // 协议书状态不对
    if (m_DivorceAgreement.m_DivorceAgreementInfo.GetState() != eDivorceState_Running)
    {
        return EDivorceMsgError_AgreementStateErr;
    }

    if (m_DivorceAgreement.GetProposerID() == m_pRoleAttr->GetRoleID())
    {
        return EDivorceMsgError_NotRightPeople;
    }

    // 是否已经按过手印
    if (m_DivorceAgreement.m_DivorceAgreementInfo.m_nMaleRoleID == m_pRoleAttr->GetRoleID())
    {
        if (m_DivorceAgreement.m_DivorceAgreementInfo.IsMaleAgree())
        {
            return EDivorceMsgError_HaveAgree;
        }

        // 同意
        m_DivorceAgreement.m_DivorceAgreementInfo.SetMaleAgree(true);
    }
    else
    {
        if (m_DivorceAgreement.m_DivorceAgreementInfo.IsFemaleAgree())
        {
            return EDivorceMsgError_HaveAgree;
        }

        // 同意
        m_DivorceAgreement.m_DivorceAgreementInfo.SetFemaleAgree(true);
    }
    m_DivorceAgreement.UpdateDivorceAgreementState(eDivorceState_End);

    // 行为
    CDivorceAction action;
    action.m_eType = eDivorceAction_Agree;
    action.m_nAgreementID = m_DivorceAgreement.GetAgreementID();
    action.m_nProposerID = m_pRoleAttr->GetRoleID();
    action.m_nTime = (unsigned int)time(NULL);
    m_DivorceAgreement.AddDivorceAction(action);

    // 处理对方
    CRoleEntity * pCoupleEntity = CPlayerManager::Instance().GetEntityByRoleID(m_coupleInfo.m_nCoupleID);
    if (pCoupleEntity != NULL)
    {
        CDivorceAgreement & agreement = pCoupleEntity->GetRoleCouple()->GetDivorceAgreement();
        agreement.UpdateDivorceAgreementState(eDivorceState_End);
        agreement.AddDivorceAction(action, false, false, false);

        if (m_DivorceAgreement.m_DivorceAgreementInfo.m_nMaleRoleID == pCoupleEntity->GetRoleID())
        {
            agreement.SetFemaleIsAgree(true);
        }
        else
        {
            agreement.SetMaleIsAgree(true);
        }
    }

    // 更新状态DB
    CDivorceAgreementInfo * pDivorceAgreementInfo = new CDivorceAgreementInfo(m_DivorceAgreement.GetDivorceAgreementInfo());
    AddQuery(QUERY_Couple_UpdateDivorceAgreement, m_DivorceAgreement.GetAgreementID(), pDivorceAgreementInfo);

    // 离婚处理
    DivorceHandle();

    return EDivorceMsgError_Suc;
}

int CEntityCoupleComponent::DivorceHandle()
{
    unsigned int nPairID = m_coupleInfo.m_nPairID;

    // DB
    AddQuery(QUERY_Couple_DelCouple, nPairID);

    // 离婚邮件
    std::string strMailTitle = CLocalization::Instance().GetString("Couple_Divorce_Mail_Title");
    std::string strMailContent;
    if (m_pRoleAttr != NULL)
    {
        SafeFormat(strMailContent, CLocalization::Instance().GetString("Couple_Divorce_Mail_Content"), m_pRoleAttr->GetRoleName());
    }
    CMailMgr::Instance().SendSystemMailToPlayer( m_coupleInfo.m_nCoupleID, EMailType_CoupleDivorced, strMailTitle, strMailContent );

    // 对方处理
    CRoleEntity *pTarget = CPlayerManager::Instance().GetEntityByRoleID(m_coupleInfo.m_nCoupleID);
    if (pTarget != NULL && pTarget->IsMarried())
    {
        if (m_pRoleAttr != NULL)
        {
            pTarget->SysChat(CLocalization::Instance().GetString("Couple_Divorce_System_Notification").c_str(), m_pRoleAttr->GetRoleName());
        }

        if (pTarget->GetComponent<CRoleCouple>() != NULL)
        {
            pTarget->GetComponent<CRoleCouple>()->DeleteMarriedItem();
        }
        pTarget->ResetCoupleData();

        // 删除结婚任务
        pTarget->GetComponent<CRoleQuestNew>()->RemoveCoupleQuest();

        GameMsg_S2C_NotifyHasBeenDivorced msgNotifyDivorced;
        if (m_pRoleAttr != NULL)
        {
            msgNotifyDivorced.m_nDivorcedBy = m_pRoleAttr->GetRoleID();
            msgNotifyDivorced.m_strDivorcedBy = m_pRoleAttr->GetRoleName();
        }
        pTarget->SendPlayerMsg(&msgNotifyDivorced);
    }

    // 同步到group
    GameMsg_S2G_Divorced msgDivorce;
    msgDivorce.m_nDivorceID = m_pRoleAttr->GetRoleID();
    msgDivorce.m_nPairID = m_coupleInfo.m_nPairID;
    msgDivorce.m_nBeDivorcedID = m_coupleInfo.m_nCoupleID;
    msgDivorce.m_bNeedNotify = (pTarget == NULL);
    ::SendMsg2GroupServer(&msgDivorce);

    // 处理自己
    m_pRoleQuestNew->RemoveCoupleQuest();
    DeleteMarriedItem();
    ResetCoupleData();

    // 清理房间相关信息
    CWeddingRoom * pWeddingRoom = CWeddingRoomManager::Instance().FindWeddingRoomByPairID(nPairID);
    if (pWeddingRoom != NULL)
    {
        pWeddingRoom->SetValid(false);
        pWeddingRoom->OnStateChange(false);

        unsigned int nRoomID = pWeddingRoom->RoomID();
        pWeddingRoom->SendGiftToBlessinger();
        CWeddingRoomManager::Instance().RemoveAllPlayer(pWeddingRoom);
        CWeddingRoomManager::Instance().DestroyRoom(nRoomID, CRoleCoupleLog::ECoupleDestoryCase_OutCoupleHandle);
    }

    return EDivorceMsgError_Suc;
}

int CEntityCoupleComponent::DivorceChat()
{

    return EDivorceMsgError_Suc;
}

void CEntityCoupleComponent::OnDivorceChat(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_DivorceChat &charmsg = (GameMsg_C2S_DivorceChat&)rMsg;


    CDivorceAction action;
    action.m_eType = eDivorceAction_Chat;
    action.m_nAgreementID = m_DivorceAgreement.GetAgreementID();
    action.m_nProposerID = m_pRoleAttr->GetRoleID();
    action.m_nTime = (unsigned int)time(NULL);
    action.m_strChatContent = charmsg.m_strChatContent;
    m_DivorceAgreement.AddDivorceAction(action);

    // 处理对方
    CRoleEntity * pCoupleEntity = CPlayerManager::Instance().GetEntityByRoleID(m_coupleInfo.m_nCoupleID);
    if (pCoupleEntity != NULL)
    {
        CDivorceAgreement & agreement = pCoupleEntity->GetRoleCouple()->GetDivorceAgreement();
        agreement.AddDivorceAction(action, false, false, false);
    }
}

void CEntityCoupleComponent::OnDivorceGift(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_DivorceGift & giftmsg = (GameMsg_C2S_DivorceGift&)rMsg;
    unsigned char cDestSex = 0;
    if (m_coupleInfo.m_nSmallRoleID > 0)
    {
        cDestSex = m_pRoleAttr->GetSex();
    }
    else
    {
        cDestSex = m_pRoleAttr->GetSex() == ESexType_Male ? ESexType_Female : ESexType_Male;
    }

    int nError = (int)m_pRoleMall->OrderSendFromDivorec(giftmsg.vecGoods, giftmsg.m_nDestRoleID, cDestSex);
    if (nError != 0)
    {
        GameMsg_S2C_GiftDivorceFail failmsg;
        SendPlayerMsg(&failmsg);
    }
    else
    {
        // 行为
        CDivorceAction action;
        action.m_nAgreementID = m_DivorceAgreement.GetAgreementID();
        action.m_eType = eDivorceAction_Gift;
        action.m_nProposerID = m_pRoleAttr->GetRoleID();
        action.m_nTime = (unsigned int)time(NULL);
        VecPeddleGoods::iterator it = giftmsg.vecGoods.begin();
        if (it != giftmsg.vecGoods.end())
        {
            action.m_SendItem.m_nItemType = it->m_nType;
            action.m_SendItem.m_nItemCount = 1;
            action.m_SendItem.m_nValidTime = m_pRoleMall->GetValidTimeByGoodSelIndex(it->m_nType, it->m_nSelIndex);
        }
        m_DivorceAgreement.AddDivorceAction(action);


        // 处理对方
        CRoleEntity * pCoupleEntity = CPlayerManager::Instance().GetEntityByRoleID(m_coupleInfo.m_nCoupleID);
        if (pCoupleEntity != NULL)
        {
            CDivorceAgreement & agreement = pCoupleEntity->GetRoleCouple()->GetDivorceAgreement();
            agreement.AddDivorceAction(action, false, false, false);
        }

        GameMsg_S2C_GiftDivorceSuc sucmsg;
        SendPlayerMsg(&sucmsg);
    }
}

int CEntityCoupleComponent::DivorceGift()
{
    return EDivorceMsgError_Suc;
}

void CEntityCoupleComponent::OnDivorceWishItem(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_DivorceWish & wishmsg = (GameMsg_C2S_DivorceWish&)rMsg;

    GameMsg_S2C_DivorceWishItem resultmsg;
    CRoleEntity * pTargetEntity = CPlayerManager::Instance().GetEntityByRoleID(wishmsg.m_nDestRoleID);
    if (pTargetEntity != NULL)
    {
        std::list<itemtype_t> listItemType;
        m_pRoleMall->GetDesireList(listItemType);

        resultmsg.m_WishItem.insert(resultmsg.m_WishItem.end(), listItemType.begin(), listItemType.end());
    }

    SendPlayerMsg(&resultmsg);
}

unsigned int CEntityCoupleComponent::GetDivorecAgreementOtherRoleID()
{
    if (m_nDivorceAgreementID == 0)
    {
        return 0;
    }

    return m_pRoleAttr->GetRoleID() == m_DivorceAgreement.GetFemaleRoleID() ? m_DivorceAgreement.GetMaleRoleID() : m_DivorceAgreement.GetFemaleRoleID();
}

void CEntityCoupleComponent::HaveNewCouple(char cThreeSex)
{
    if (m_nDivorceAgreementID <= 0 || m_DivorceAgreement.GetAgreementID() <= 0)
    {
        return;
    }

    // 销毁自己的协议书
    m_nDivorceAgreementID = 0;
    AddQuery(QUERY_Couple_UpdateCoupleAgreementID, m_pRoleAttr->GetRoleID(), NULL, 0);
    if (m_DivorceAgreement.GetAgreementState() != eDivorceState_End)
    {
        m_DivorceAgreement.UpdateDivorceAgreementState(eDivorceState_End);

    }

    // 行为
    CDivorceAction action;
    if (m_pRoleAttr->GetSex() == cThreeSex)
    {
        action.m_eType = eDivorceAction_HaveNewHomosexual;
    }
    else
    {
        action.m_eType = eDivorceAction_HaveNew;
    }
    action.m_nProposerID = m_pRoleAttr->GetRoleID();
    action.m_nTime = (unsigned int)time(NULL);
    action.m_nAgreementID = m_DivorceAgreement.GetAgreementID();

    CRoleEntity * pCoupleEntity = CPlayerManager::Instance().GetEntityByRoleID(m_coupleInfo.m_nCoupleID);
    bool bMaleSendMsg = false;
    bool bFemaleSendMsg = false;
    if (pCoupleEntity != NULL)
    {
        bMaleSendMsg = m_pRoleAttr->GetRoleID() == m_DivorceAgreement.GetMaleRoleID() ? true : pCoupleEntity->GetRoleCouple()->GetSelfDivorceAgreementID() > 0;
        bFemaleSendMsg = m_pRoleAttr->GetRoleID() == m_DivorceAgreement.GetFemaleRoleID() ? true : pCoupleEntity->GetRoleCouple()->GetSelfDivorceAgreementID() > 0;
    }
    else
    {
        bMaleSendMsg = m_pRoleAttr->GetRoleID() == m_DivorceAgreement.GetMaleRoleID() ? true : false;
        bFemaleSendMsg = m_pRoleAttr->GetRoleID() == m_DivorceAgreement.GetFemaleRoleID() ? true : false;
    }

    m_DivorceAgreement.AddDivorceAction(action, true, bMaleSendMsg, bFemaleSendMsg);

    // 处理对方
    if (pCoupleEntity != NULL)
    {
        CDivorceAgreement & agreement = pCoupleEntity->GetRoleCouple()->GetDivorceAgreement();
        agreement.UpdateDivorceAgreementState(eDivorceState_End);
        agreement.AddDivorceAction(action, false, false, false);
    }

    // 更新状态DB
    AddQuery(QUERY_Couple_UpdateDivorceAgreementState, m_DivorceAgreement.GetAgreementID(), NULL, eDivorceState_End);

    // 离婚处理
    DivorceHandle();

    GameMsg_S2C_DivorceHaveNewCouple newcouplemsg;
    SendPlayerMsg(&newcouplemsg);
}

void CEntityCoupleComponent::_OnRefreshDivorceAction(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_RefreshDivorceAction & g2smsg = (GameMsg_G2S_RefreshDivorceAction &)rMsg;

    GameMsg_S2C_RefreshAction s2cmsg;
    s2cmsg.m_DivorceAgreementInfo = g2smsg.m_DivorceAgreementInfo;
    s2cmsg.m_action = g2smsg.m_Action;
    SendPlayerMsg(&s2cmsg);
}

int CEntityCoupleComponent::DivorceAnimation()
{
    if (!CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_NewDivorce))
    {
        return EDivorceMsgError_FunctionNotOpen;
    }

    if (m_pRoleRoom->Room() != NULL)
    {
        return EDivorceMsgError_InRoom;
    }

    if (WeddingRoom() != NULL)
    {
        return EDivorceMsgError_InRoom;
    }

    if (m_pRolePhotoRoom->PhotoRoom() != NULL)
    {
        return EDivorceMsgError_InRoom;
    }

    if (m_pRoleAmuseRoom->AmuseRoom() != NULL)
    {
        return EDivorceMsgError_InRoom;
    }

    return EDivorceMsgError_Suc;
}

void CEntityCoupleComponent::OnDivorceAnimation(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    int nError = DivorceAnimation();
    if (nError != EDivorceMsgError_Suc)
    {
        GameMsg_S2C_DivorceAnimationFail failmsg;
        failmsg.m_cErrorType = (char)nError;
        SendPlayerMsg(&failmsg);
    }
    else
    {
        unsigned int nOtherRoleID = m_pRoleAttr->GetRoleID() == m_DivorceAgreement.GetMaleRoleID() ? m_DivorceAgreement.GetFemaleRoleID() : m_DivorceAgreement.GetMaleRoleID();
        CRoleEntity * pOtherEntity = CPlayerManager::Instance().GetEntityByRoleID(nOtherRoleID);
        if (pOtherEntity != NULL)
        {
            GameMsg_S2C_DivorceAnimationSuc sucmsg;
            if (m_DivorceAgreement.m_DivorceAgreementInfo.m_nSmallRoleID > 0)
            {
                if (m_pRoleAttr->GetRoleID() == m_DivorceAgreement.GetFemaleRoleID())
                {
                    SetDivorcePlayerMsg(sucmsg.m_FemaleMsg);
                    pOtherEntity->GetRoleCouple()->SetDivorcePlayerMsg(sucmsg.m_MaleMsg);
                }
                else
                {
                    SetDivorcePlayerMsg(sucmsg.m_MaleMsg);
                    pOtherEntity->GetRoleCouple()->SetDivorcePlayerMsg(sucmsg.m_FemaleMsg);
                }
            }
            else
            {
                if (m_pRoleAttr->GetSex() == ESexType_Male)
                {
                    SetDivorcePlayerMsg(sucmsg.m_MaleMsg);
                    pOtherEntity->GetRoleCouple()->SetDivorcePlayerMsg(sucmsg.m_FemaleMsg);
                }
                else
                {
                    SetDivorcePlayerMsg(sucmsg.m_FemaleMsg);
                    pOtherEntity->GetRoleCouple()->SetDivorcePlayerMsg(sucmsg.m_MaleMsg);
                }
            }


            sucmsg.m_Actions = m_DivorceAgreement.m_DivorceAction;
            sucmsg.m_DivorceAgreementInfo = m_DivorceAgreement.m_DivorceAgreementInfo;

            SendPlayerMsg(&sucmsg);
        }
        else
        {
            GameMsg_S2G_GetDivorceAnimationPersonInfo s2gMsg;
            s2gMsg.m_nSelfRoleID = m_pRoleAttr->GetRoleID();
            s2gMsg.m_nCoupleRoleID = nOtherRoleID;
            ::SendMsg2GroupServer(&s2gMsg);
        }
    }
}

void CEntityCoupleComponent::_GetDivorceCouplePersonInfo(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    int nError = DivorceAnimation();
    if (nError != EDivorceMsgError_Suc)
    {
        GameMsg_S2C_DivorceAnimationFail failmsg;
        failmsg.m_cErrorType = (char)nError;
        SendPlayerMsg(&failmsg);
    }
    else
    {
        GameMsg_G2S_GetDivorceeAnimationPersonInfo & g2sMsg = (GameMsg_G2S_GetDivorceeAnimationPersonInfo &)rMsg;

        GameMsg_S2C_DivorceAnimationSuc sucmsg;

        if (m_DivorceAgreement.m_DivorceAgreementInfo.m_nSmallRoleID > 0)
        {
            if (m_DivorceAgreement.GetFemaleRoleID() == m_pRoleAttr->GetRoleID())
            {
                SetDivorcePlayerMsg(sucmsg.m_FemaleMsg);
                SetDivorceOfflinePlayerMsg(m_DivorceAgreement.GetMaleRoleID(), g2sMsg.m_OffLineData, sucmsg.m_MaleMsg);
                sucmsg.m_MaleMsg.OffLineData = g2sMsg.m_OffLineData;
            }
            else
            {
                SetDivorcePlayerMsg(sucmsg.m_MaleMsg);
                SetDivorceOfflinePlayerMsg(m_DivorceAgreement.GetFemaleRoleID(), g2sMsg.m_OffLineData, sucmsg.m_FemaleMsg);
                sucmsg.m_FemaleMsg.OffLineData = g2sMsg.m_OffLineData;
            }
        }
        else
        {
            if (m_pRoleAttr->GetSex() == ESexType_Male)
            {
                SetDivorcePlayerMsg(sucmsg.m_MaleMsg);
                SetDivorceOfflinePlayerMsg(m_DivorceAgreement.GetFemaleRoleID(), g2sMsg.m_OffLineData, sucmsg.m_FemaleMsg);
                sucmsg.m_FemaleMsg.OffLineData = g2sMsg.m_OffLineData;
            }
            else
            {
                SetDivorcePlayerMsg(sucmsg.m_FemaleMsg);
                SetDivorceOfflinePlayerMsg(m_DivorceAgreement.GetMaleRoleID(), g2sMsg.m_OffLineData, sucmsg.m_MaleMsg);
                sucmsg.m_MaleMsg.OffLineData = g2sMsg.m_OffLineData;
            }
        }

        sucmsg.m_Actions = m_DivorceAgreement.m_DivorceAction;
        sucmsg.m_DivorceAgreementInfo = m_DivorceAgreement.m_DivorceAgreementInfo;

        SendPlayerMsg(&sucmsg);
    }
}

void CEntityCoupleComponent::SetDivorcePlayerMsg(DivorcePlayerMsg & playermsg)
{
    playermsg.nRoleID = m_pRoleAttr->GetRoleID();
    playermsg.strRoleName = m_pRoleAttr->GetRoleName();
    playermsg.cRoleSex = m_pRoleAttr->GetSex();
    playermsg.cRoleSkin = m_pRoleAttr->GetColor();

    CDanceGroupInfo *pDanceGroup = m_pRoleDanceGroup->GetDanceGroupInfo();
    if (pDanceGroup != NULL)
    {
        playermsg.strDanceGroupName = pDanceGroup->m_DGBaseInfo.m_strGroupName;
        playermsg.nDanceGroupBadgeID = pDanceGroup->m_DGBaseInfo.m_nBadge;
        playermsg.nDanceGroupEffectID = pDanceGroup->m_DGBaseInfo.m_nEffect;

        CDanceGroupMember * pDGMember = pDanceGroup->GetGroupMember(m_pRoleAttr->GetRoleID());
        if (pDGMember != NULL)
        {
            playermsg.cDanceGroupPos = (char)pDGMember->m_nTitle;
        }
    }

    playermsg.bIsVip = m_pRoleVip->IsVIP();
    playermsg.nVipLevel = (unsigned short)m_pRoleVip->VIPLevel();
    playermsg.pRoleItem = m_pRoleItem;
    playermsg.pRoleGene = m_pRoleGene;
}

void CEntityCoupleComponent::SetDivorceOfflinePlayerMsg(unsigned int nRoleID, RoleImageOfflineData & OffLineData, DivorcePlayerMsg & playermsg)
{
    playermsg.nRoleID = nRoleID;
    playermsg.strRoleName = OffLineData.m_strRoleName;
    playermsg.cRoleSex = OffLineData.m_nSex;
    playermsg.cRoleSkin = OffLineData.m_nSkin;
}

void CEntityCoupleComponent::OnDivorceRequest(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
    int nError = DivorceRequest();
    if (nError == EDivorceMsgError_FunctionNotOpen)
    {
        _OnRequestDivorce(msg, slotPeer);
    }
    else
    {
        if (EDivorceMsgError_Suc != nError)
        {
            GameMsg_S2C_RequestDivorceFail msgFail;
            msgFail.m_nError = (unsigned char)nError;

            SendPlayerMsg(&msgFail);
        }
    }
}

void CEntityCoupleComponent::_OnRequestDivorce(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
    //GameMsg_C2S_RequestDivorce & msgDivorce = (GameMsg_C2S_RequestDivorce &)msg;

    ERequestDivorceRes eRes = ERequestDivorce_Suc;
    std::string strError = "";

    if (!IsMarried()) {
        eRes = ERequestDivorce_NotMarriedNot;
        strError = "ERequestDivorce_NotMarriedNot";
    }
    else if (m_pRoleItem == NULL || m_pRoleItem->SearchItem(C_DIVORCE_ITEM) == NULL) {
        eRes = ERequestDivorce_LackOfDivorceItem;
        strError = "ERequestDivorce_LackOfDivorceItem";
    }
    else if (!m_pRoleItem->RemoveItemByType(C_DIVORCE_ITEM, 1, EItemAction_Del_Divorce)) {
        eRes = ERequestDivorce_Unknown;
        strError = "ERequestDivorce_Unknown";
    }
    else {
        AddQuery(QUERY_Couple_DelCouple, m_coupleInfo.m_nPairID);
        GameMsg_S2C_RequestDivorceSuc msgSuc;
        SendPlayerMsg(&msgSuc);

        std::string strMailTitle = CLocalization::Instance().GetString("Couple_Divorce_Mail_Title");
        std::string strMailContent;
        if (m_pRoleAttr != NULL)
        {
            SafeFormat(strMailContent, CLocalization::Instance().GetString("Couple_Divorce_Mail_Content"),
                m_pRoleAttr->GetRoleName());
        }
        CMailMgr::Instance().SendSystemMailToPlayer( m_coupleInfo.m_nCoupleID, EMailType_CoupleDivorced, strMailTitle, strMailContent );

        CRoleEntity *pTarget = CPlayerManager::Instance().GetEntityByRoleID(m_coupleInfo.m_nCoupleID);
        if (pTarget != NULL && pTarget->IsMarried()) {
            if (m_pRoleAttr != NULL) {
                pTarget->SysChat(CLocalization::Instance().GetString("Couple_Divorce_System_Notification").c_str(), m_pRoleAttr->GetRoleName());
            }

            if (pTarget->GetComponent<CRoleCouple>() != NULL) {
                pTarget->GetComponent<CRoleCouple>()->DeleteMarriedItem();
            }
            pTarget->ResetCoupleData();

            // 删除结婚任务
            if (pTarget->GetComponent<CRoleQuestNew>() != NULL)
            {
                pTarget->GetComponent<CRoleQuestNew>()->RemoveCoupleQuest();
            }

            GameMsg_S2C_NotifyHasBeenDivorced msgNotifyDivorced;
            if (m_pRoleAttr != NULL) {
                msgNotifyDivorced.m_nDivorcedBy = m_pRoleAttr->GetRoleID();
                msgNotifyDivorced.m_strDivorcedBy = m_pRoleAttr->GetRoleName();
            }
            pTarget->SendPlayerMsg(&msgNotifyDivorced);
        }

        GameMsg_S2G_Divorced msgDivorce;
        if (m_pRoleAttr != NULL) {
            msgDivorce.m_nDivorceID = m_pRoleAttr->GetRoleID();
        }
        msgDivorce.m_nPairID = m_coupleInfo.m_nPairID;
        msgDivorce.m_nBeDivorcedID = m_coupleInfo.m_nCoupleID;
        msgDivorce.m_bNeedNotify = (pTarget == NULL);
        ::SendMsg2GroupServer(&msgDivorce);

        // 清理房间相关信息
        if (m_pWeddingRoom != NULL)
        {
            m_pWeddingRoom->SetValid(false);
            m_pWeddingRoom->OnStateChange(false);

            unsigned int nRoomID = m_pWeddingRoom->RoomID();
            m_pWeddingRoom->SendGiftToBlessinger();
            CWeddingRoomManager::Instance().RemoveAllPlayer(m_pWeddingRoom);
            CWeddingRoomManager::Instance().DestroyRoom(nRoomID, CRoleCoupleLog::ECoupleDestoryCase_OutCoupleRequest);
        }

        DeleteMarriedItem();
        ResetCoupleData();

        m_pRoleQuestNew->RemoveCoupleQuest();

        //log
        unsigned int nMaleID = 0;
        unsigned int nFemaleID = 0;
        if (m_pRoleAttr->GetSex() == ESexType_Male)
        {
            nMaleID = m_pRoleAttr->GetRoleID();
            nFemaleID = m_coupleInfo.m_nCoupleID;
        }
        else
        {
            nFemaleID = m_pRoleAttr->GetRoleID();
            nMaleID = m_coupleInfo.m_nCoupleID;
        }

        RoleCoupleLog(nMaleID, nFemaleID, CRoleCoupleLog::ERoleCoupleCause_OutCouple, m_coupleInfo.GetIntimacy(), m_coupleInfo.m_nBlessingValue);
    }

    if (strError != "") {
        GameMsg_S2C_RequestDivorceFail msgFail;
        msgFail.m_nError = (unsigned char)eRes;
        SendPlayerMsg(&msgFail);
    }
}

void CEntityCoupleComponent::_NotifyHasBeenDivorced(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_NotifyHasBeenDivorced &msgBeDivorced = (GameMsg_G2S_NotifyHasBeenDivorced &)msg;
    if (IsMarried() && m_coupleInfo.m_nCoupleID == msgBeDivorced.m_nDivorcedBy) {
        if (m_pRoleChat != NULL) {
            m_pRoleChat->SysChat(CLocalization::Instance().GetString("Couple_Divorce_System_Notification").c_str(), msgBeDivorced.m_strDivorcedBy.c_str());
        }

        GameMsg_S2C_NotifyHasBeenDivorced msgNotifyDivorced;
        msgNotifyDivorced.m_nDivorcedBy = msgBeDivorced.m_nDivorcedBy;
        msgNotifyDivorced.m_strDivorcedBy = msgBeDivorced.m_strDivorcedBy;
        SendPlayerMsg(&msgNotifyDivorced);

        DeleteMarriedItem();
        ResetCoupleData();
    }
}

void CEntityCoupleComponent::_OnCreateWeddingRoom(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    int nResult = CWeddingRoomManager::Instance().OnCreateWeddingRoom(rMsg, *this);
    if (nResult != ECreateWeddingRoom_Suc)
    {
        return;
    }

    // 更新结婚记录到group
    GameMsg_S2G_AddWeddingRecord record;
    record.m_Record.m_nPairID = m_coupleInfo.m_nPairID;
    record.m_Record.m_nIsRunning = 1;
    record.m_Record.m_nMaleID = m_pWeddingRoom->BrideID();
    record.m_Record.m_strMaleName = m_pRoleAttr->GetRoleName();
    record.m_Record.m_nFemaleID = m_pWeddingRoom->GroomID();
    record.m_Record.m_strFemaleName = m_coupleInfo.m_strCoupleName;
    record.m_Record.m_nLine = g_nLine;
    record.m_Record.m_nBlessingValue = 0;
    record.m_Record.m_nSmallRoleID = m_coupleInfo.m_nSmallRoleID;
    ::SendMsg2GroupServer(&record);

    //广播祝福消息
    //BroadcastCreateWeddingRoomMsg();
    // 广播给双方在线好友
    BroadcastMarriageToCoupleFriends(true);

    // 创建log
    RoleCoupleLog(m_pWeddingRoom->GroomID(), m_pWeddingRoom->BrideID(), CRoleCoupleLog::ERoleCoupleCause_CreateWeddingRoom, 0, 0);
}

void CEntityCoupleComponent::OnGetBlessingRankList(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_GetBlessingRankList & c2sMsg = (GameMsg_C2S_GetBlessingRankList&)rMsg;

    GameMsg_S2G_GetBlessingRankList s2gMsg;
    s2gMsg.m_nCurrentPage = c2sMsg.m_nCurrentPage;
    s2gMsg.m_nPairID = m_coupleInfo.m_nPairID;
    s2gMsg.m_nRoleID = m_pRoleAttr->GetRoleID();
    s2gMsg.m_nPageCount = c2sMsg.m_nPageCount;
    ::SendMsg2GroupServer(&s2gMsg);
}

void CEntityCoupleComponent::BroadcastOtherLineFriends(unsigned int nRoleID, std::string strMaleName, std::string strFemaleName)
{
    GameMsg_S2G_WeddingNotice s2gMsg;
    s2gMsg.m_nRoleID = nRoleID;
    s2gMsg.m_strMaleRoleName = strMaleName;
    s2gMsg.m_strFeMaleRoleName = strFemaleName;

    ::SendMsg2GroupServer(&s2gMsg);
}

void CEntityCoupleComponent::BroadcastMarriageToCoupleFriends(bool isSendCurrentLine)
{
    GameMsg_S2C_BlessingNotify  notify;
    notify.m_strManName = m_pWeddingRoom->GetGroomName();
    notify.m_strWomanName = m_pWeddingRoom->GetBrideName();
    notify.m_nPointIndex = g_nLine;

    CRoleEntity *pCouple = CPlayerManager::Instance().GetEntityByRoleID(m_coupleInfo.m_nCoupleID);
    std::vector<unsigned int> coupleList;
    if (pCouple != NULL)
    {
        pCouple->GetFriendList(coupleList);
    }

    std::vector<unsigned int> roomList;
    m_pRoleFriend->GetFriendList(roomList);

    roomList.insert(roomList.end(), coupleList.begin(), coupleList.end());
    sort(roomList.begin(), roomList.end());
    std::vector<unsigned int>::iterator itDel = unique(roomList.begin(), roomList.end());
    if (itDel != roomList.end())
    {
        roomList.erase(itDel, roomList.end());
    }

    std::vector<unsigned int>::iterator it = roomList.begin();
    for (; it != roomList.end(); ++it)
    {
        if (*it == m_coupleInfo.m_nCoupleID || *it == m_pRoleAttr->GetRoleID())
        {
            continue;
        }

        if (isSendCurrentLine)
        {
            CRoleEntity *pFriendRole = CPlayerManager::Instance().GetEntityByRoleID(*it);
            if (pFriendRole != NULL)
            {
                pFriendRole->SendPlayerMsg(&notify);
            }
            else
            {
                BroadcastOtherLineFriends(*it, m_pWeddingRoom->GetGroomName(), m_pWeddingRoom->GetBrideName());
            }
        }
        else
        {
            BroadcastOtherLineFriends(*it, m_pWeddingRoom->GetGroomName(), m_pWeddingRoom->GetBrideName());
        }
    }
}

void CEntityCoupleComponent::BroadcastCreateWeddingRoomMsg()
{
    // 广播祝福消息
    if (m_pWeddingRoom != NULL)
    {
        switch (m_pWeddingRoom->RoomType())
        {
        case  EWeddingRoom_PRIMARY:
        {
                                      // 广播给双方在线好友
                                      BroadcastMarriageToCoupleFriends(true);
        }
            break;
        case  EWeddingRoom_MIDDLE:
        {
                                     // 广播本线玩家
                                     GameMsg_S2C_BlessingNotify  notify;
                                     notify.m_strManName = m_pWeddingRoom->GetGroomName();
                                     notify.m_strWomanName = m_pWeddingRoom->GetBrideName();
                                     notify.m_nPointIndex = g_nLine;

                                     std::list< CRoleEntity * > roleList;
                                     CPlayerManager::Instance().GetAllPlayers(roleList);

                                     std::list<CRoleEntity*>::iterator it = roleList.begin();
                                     for (; it != roleList.end(); ++it)
                                     {
                                         if ((*it)->GetRoleID() != m_pWeddingRoom->BrideID() && (*it)->GetRoleID() != m_pWeddingRoom->GroomID())
                                         {
                                             (*it)->SendPlayerMsg(&notify);
                                         }
                                     }

                                     // 广播给非本线好友
                                     BroadcastMarriageToCoupleFriends();
        }
            break;
        case EWeddingRoom_ADVANCED:
        {
                                      // 广播所有线玩家
                                      GameMsg_S2G_BlessingNotify notify;
                                      notify.m_nMaleRoleID = m_pWeddingRoom->GroomID();
                                      notify.m_strManName = m_pWeddingRoom->GetGroomName();
                                      notify.m_strWomanName = m_pWeddingRoom->GetBrideName();
                                      notify.m_nFemaleRoleID = m_pWeddingRoom->BrideID();
                                      notify.m_nPointIndex = g_nLine;
                                      ::SendMsg2GroupServer(&notify);
        }
            break;
        case EWeddingRoom_Max:
        case EWeddingRoom_None:
        default:
            break;
        }
    }
}

void  CEntityCoupleComponent::OnBlessingCouple(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_BlessingCouple & c2sMsg = (GameMsg_C2S_BlessingCouple &)rMsg;

    CWeddingRoom * pWeddingRoom = CWeddingRoomManager::Instance().FindWeddingRoomByPairID(c2sMsg.m_nPairID);
    if (c2sMsg.m_nPairID == m_coupleInfo.m_nPairID)
    {
        GameMsg_S2C_BlessingError errormsg;
        errormsg.m_strError = "ECoupleMsgError_CanNotBlessingSelf";

        SendPlayerMsg(&errormsg);
    }
    else if (pWeddingRoom != NULL)
    {
        if (pWeddingRoom->IsHaveBlessing(m_pRoleAttr->GetRoleID()))
        {
            GameMsg_S2C_BlessingError errormsg;
            errormsg.m_strError = "ECoupleMsgError_HaveBlessing";

            SendPlayerMsg(&errormsg);
        }
        else if (!pWeddingRoom->Valid())
        {
            GameMsg_S2C_BlessingError errormsg;
            errormsg.m_strError = "ECoupleMsgError_BlessingPairNot";

            SendPlayerMsg(&errormsg);
        }
        else
        {
            pWeddingRoom->BlessingCouple(m_pRoleAttr->GetRoleID(), m_pRoleAttr->GetSex());
        }
    }
    else
    {
        GameMsg_S2G_BlessingCouple s2gMsg;
        s2gMsg.m_nRoleID = m_pRoleAttr->GetRoleID();
        s2gMsg.m_nPairID = c2sMsg.m_nPairID;
        s2gMsg.m_nSex = m_pRoleAttr->GetSex();

        ::SendMsg2GroupServer(&s2gMsg);
    }
}

void CEntityCoupleComponent::OnRedEnvelope(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_MoneyBlessingPair & c2sMsg = (GameMsg_C2S_MoneyBlessingPair &)rMsg;

    if (m_coupleInfo.m_nPairID == c2sMsg.m_nPairID)
    {
        GameMsg_S2C_BlessingError errormsg;
        errormsg.m_strError = "ECoupleMsgError_CanNotBlessingSelf";
        SendPlayerMsg(&errormsg);
        return;
    }

    // 判断自己的钱
    unsigned int nNeedMoney = CCoupleConfigDataMgr::Instance().GetRedEnvelopeValue()*c2sMsg.m_nRedEnvelopeCount;
    if (m_pRoleAttr->GetPoint() < nNeedMoney)
    {
        GameMsg_S2C_BlessingError errormsg;
        errormsg.m_strError = "ECoupleMsgError_MoneyNotEnough";
        SendPlayerMsg(&errormsg);
        return;
    }

    CWeddingRoom * pWeddingRoom = CWeddingRoomManager::Instance().FindWeddingRoomByPairID(c2sMsg.m_nPairID);
    if (pWeddingRoom != NULL)
    {
        if (!pWeddingRoom->Valid())
        {
            GameMsg_S2C_BlessingError errormsg;
            errormsg.m_strError = "ECoupleMsgError_BlessingPairNot";

            SendPlayerMsg(&errormsg);
        }
        else
        {
            std::string strRoleName = m_pRoleAttr->GetRoleName();
            pWeddingRoom->RedEnvelope(m_pRoleAttr->GetRoleID(), strRoleName, c2sMsg.m_nRedEnvelopeCount);
        }
    }
    else
    {
        // 转发到group
        GameMsg_S2G_RedEnvelope s2gMsg;
        s2gMsg.m_nRoleID = m_pRoleAttr->GetRoleID();
        s2gMsg.m_nPairID = c2sMsg.m_nPairID;
        s2gMsg.m_nRedEnvelopeCount = c2sMsg.m_nRedEnvelopeCount;

        ::SendMsg2GroupServer(&s2gMsg);
    }
}

void CEntityCoupleComponent::_OnGetBlessingRankList(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_GetBlessingRankList & g2sMsg = (GameMsg_G2S_GetBlessingRankList &)rMsg;

    GameMsg_S2C_GetBlessingRankListResult s2cMsg;
    s2cMsg.m_nCurrentPage = (unsigned short)g2sMsg.m_nCurrentPage;
    s2cMsg.m_nRankTotalCount = (unsigned short)g2sMsg.m_nRankTotalCount;
    s2cMsg.m_BlessingRankVec = g2sMsg.m_BlessingValueRankVec;
    s2cMsg.m_nPageCount = g2sMsg.m_nPageCount;
    this->SendPlayerMsg(&s2cMsg);
}

void CEntityCoupleComponent::OnGetCoupleInfo(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_GetCoupleInfo & c2sMsg = (GameMsg_C2S_GetCoupleInfo &)rMsg;

    GameMsg_S2G_GetCoupleInfo s2gMsg;
    s2gMsg.m_nPairID = c2sMsg.m_nPairID;
    s2gMsg.m_nRoleID = m_pRoleAttr->GetRoleID();
    ::SendMsg2GroupServer(&s2gMsg);
}

void CEntityCoupleComponent::OnGetWeddingRecordList(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_GetWeddingRecordList s2gMsg;
    s2gMsg.m_nRoleID = m_pRoleAttr->GetRoleID();
    ::SendMsg2GroupServer(&s2gMsg);
}

void CEntityCoupleComponent::OnGetBlessingValue(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_GetBlessingValue & c2sMsg = (GameMsg_C2S_GetBlessingValue &)rMsg;

    GameMsg_S2G_GetBlessingValue s2gMsg;
    s2gMsg.m_nRoleID = m_pRoleAttr->GetRoleID();
    s2gMsg.m_nPairID = c2sMsg.m_nPairID;
    ::SendMsg2GroupServer(&s2gMsg);
}

void CEntityCoupleComponent::_OnGetBlessingValue(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_GetBlessingValue & g2sMsg = (GameMsg_G2S_GetBlessingValue &)rMsg;

    GameMsg_S2C_GetBlessingValueResult s2cMsg;
    s2cMsg.m_nBlessingValue = g2sMsg.m_nBlessingValue;
    s2cMsg.m_nLine = g2sMsg.m_nLine;
    SendPlayerMsg(&s2cMsg);
}

void CEntityCoupleComponent::OnGetPersonInfo(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_GetPersonInfo &c2sMsg = (GameMsg_C2S_GetPersonInfo &)rMsg;

    CRoleEntity *pRole = CPlayerManager::Instance().GetEntityByRoleID(c2sMsg.m_nRoleID);
    if (pRole != NULL)
    {
        GameMsg_S2C_GetPersonInfoResult s2cMsg;
        s2cMsg.m_OffLineData.m_nRoleID = pRole->GetRoleID();
        s2cMsg.m_OffLineData.m_strRoleName = pRole->GetRoleName();
        s2cMsg.m_OffLineData.m_nSex = pRole->GetSex();
        s2cMsg.m_OffLineData.m_nSkin = pRole->GetColor();
        s2cMsg.m_pRoleItem = pRole->GetItemComponent();
        s2cMsg.m_pRoleGene = pRole->GetRoleGene();
        s2cMsg.m_pRolePet = pRole->GetComponent<CRolePet>();

        SendPlayerMsg(&s2cMsg);
    }
    else
    {
        GameMsg_S2G_GetPersonInfo s2gMsg;
        s2gMsg.m_nSelfRoleID = m_pRoleAttr->GetRoleID();
        s2gMsg.m_nRoleID = c2sMsg.m_nRoleID;
        ::SendMsg2GroupServer(&s2gMsg);
    }
}

void CEntityCoupleComponent::_OnGetPersonInfo(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_GetPersonInfo & g2sMsg = (GameMsg_G2S_GetPersonInfo &)rMsg;

    GameMsg_S2C_GetPersonInfoResult s2cMsg;
    s2cMsg.m_OffLineData = g2sMsg.m_OffLineData;
    std::vector<DBItem>::iterator it = s2cMsg.m_OffLineData.m_vecRoleItem.begin();
    for (; it != s2cMsg.m_OffLineData.m_vecRoleItem.end(); ++it)
    {
        ItemConfig* pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(it->m_nTypeID);
        if (pItemInfo != NULL)
        {
            it->m_nIndex = pItemInfo->GetClothPos();
        }
    }

    SendPlayerMsg(&s2cMsg);
}

void CEntityCoupleComponent::OnGetCoupleRedEnvelopeGiftInfo(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_GetCoupleRedEnvelopeGiftInfo & requestMsg = (GameMsg_C2S_GetCoupleRedEnvelopeGiftInfo&)rMsg;

    GameMsg_S2C_GetCoupleRedEnvelopeGiftInfoResult resultMsg;
    CWeddingRoom * pWeddingRoom = CWeddingRoomManager::Instance().FindWeddingRoomByPairID(requestMsg.m_nPairID);
    if (pWeddingRoom != NULL)
    {
        resultMsg.m_nRedEnvelopeCount = pWeddingRoom->GetRedEnvelopeAllCount();
    }
    else
    {
        resultMsg.m_nRedEnvelopeCount = 0;
    }

    CCoupleConfigDataMgr::Instance().GetRedEnvelopeGift(resultMsg.m_RedEnvelopeInfo);
    std::map<unsigned int, itemtype_t>::iterator it = resultMsg.m_RedEnvelopeInfo.begin();
    for (; it != resultMsg.m_RedEnvelopeInfo.end(); ++it)
    {
        std::list<CItem> templist;
        std::map<int, int> mapCurrency;
        m_pRoleItem->GetPacketItemInfo(m_pRoleNet->GetPlayerLogicIndex(), it->second, templist, mapCurrency);
        resultMsg.m_PacketInfo[it->second] = templist;
    }

    SendPlayerMsg(&resultMsg);
}

void CEntityCoupleComponent::_OnDelRedEnvelopeMoney(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_RedEnvelopeDelMoney & g2sMsg = (GameMsg_G2S_RedEnvelopeDelMoney &)rMsg;
    int nCostMoney = g2sMsg.m_nRedEnvelopeCount*CCoupleConfigDataMgr::Instance().GetRedEnvelopeValue();

    m_pRoleAttr->ChangeBill(-nCostMoney, EChangeBillCause_Del_RedEnvelope);

    // 返回group 增加祝福值
    GameMsg_S2G_AddRedEnvelopeBlessingValue s2gMsg;
    s2gMsg.m_nBlessingRoleID = m_pRoleAttr->GetRoleID();
    s2gMsg.m_strBlessingRoleName = m_pRoleAttr->GetRoleName();
    s2gMsg.m_nPairID = g2sMsg.m_nPairID;
    s2gMsg.m_nRedEnvelopeCount = g2sMsg.m_nRedEnvelopeCount;

    ::SendMsg2GroupServer(&s2gMsg);
}

void CEntityCoupleComponent::_OnSendBlessingErrorMsg(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_BlessingError & g2sMsg = (GameMsg_G2S_BlessingError&)rMsg;

    GameMsg_S2C_BlessingError errormsg;
    errormsg.m_strError = g2sMsg.m_strError;
    SendPlayerMsg(&errormsg);
}

void CEntityCoupleComponent::_OnSendBlessingCoupleSucMsg(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    GameMsg_S2C_BlessingCoupleSuc msg;
    SendPlayerMsg(&msg);
}

void CEntityCoupleComponent::_OnSendRedEnvelopeSucMsg(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    GameMsg_S2C_RedEnvelopeSuc msg;
    SendPlayerMsg(&msg);
}

void CEntityCoupleComponent::_OnWeddingNotice(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_WeddingNotice & g2sMsg = (GameMsg_G2S_WeddingNotice&)rMsg;

    GameMsg_S2C_BlessingNotify  notify;
    notify.m_strManName = g2sMsg.m_strMaleRoleName;
    notify.m_strWomanName = g2sMsg.m_strFemaleRoleName;
    notify.m_nPointIndex = g_nLine;

    SendPlayerMsg(&notify);
}

void CEntityCoupleComponent::RoleCoupleLog(unsigned int nMaleID, unsigned int nFemaleID, int type, int nIntimacy, int nBlessingValue)
{
    //log
    CRoleCoupleLog * pLog = new CRoleCoupleLog();
    pLog->m_nMaleRoleID = nMaleID;
    pLog->m_nFemaleRoleID = nFemaleID;
    pLog->m_nIntimacy = nIntimacy;
    pLog->m_nBlessingValue = nBlessingValue;
    pLog->m_eCoupleCause = (CRoleCoupleLog::ERoleCoupleCause)type;
    g_pQueryMgr->AddQuery(QUERY_LogRoleCouple, 0, pLog);
}

void CEntityCoupleComponent::_OnGetWeddingRecordList(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_GetWeddingRecordList & g2sMsg = (GameMsg_G2S_GetWeddingRecordList &)rMsg;

    GameMsg_S2C_GetWeddingRecordListResult s2cMsg;
    s2cMsg.m_nRoleID = m_pRoleAttr->GetRoleID();
    s2cMsg.m_WeddingRecord = g2sMsg.m_RecordList;

    SendPlayerMsg(&s2cMsg);
}

void CEntityCoupleComponent::_OnGetCoupleInfo(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_GetCoupleInfo & g2sMsg = (GameMsg_G2S_GetCoupleInfo &)rMsg;

    GameMsg_S2C_GetCoupleInfoResult s2cMsg;
    s2cMsg.m_nRank = g2sMsg.m_nRank;
    s2cMsg.m_CoupleInfo = g2sMsg.m_CoupleInfo;
    this->SendPlayerMsg(&s2cMsg);
}


void CEntityCoupleComponent::_OnTyrEnterWeddingRoom(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    CWeddingRoomManager::Instance().OnTryEnterWeddingRoom(rMsg, *this);
}

void CEntityCoupleComponent::_OnEnterWeddingRoom(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    CWeddingRoomManager::Instance().OnEnterWeddingRoom(rMsg, *this);
}

void CEntityCoupleComponent::_OnEnterMyWeddingRoom(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    CWeddingRoomManager::Instance().OnEnterMyWeddingRoom(rMsg, *this);
}

void CEntityCoupleComponent::_OnExitWeddingRoom(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    CWeddingRoomManager::Instance().OnExitRoom(rMsg, *this);
}

void CEntityCoupleComponent::_OnKickWeddingRoomPlayer(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    CWeddingRoomManager::Instance().OnKickWeddingRoomPlayer(rMsg, *this);
}

void CEntityCoupleComponent::_OnSetWeddingPlayerType(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    CWeddingRoomManager::Instance().OnSetWeddingPlayerType(rMsg, *this);
}

void CEntityCoupleComponent::_OnChangeWeddingRoomPwd(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    CWeddingRoomManager::Instance().OnChangeWeddingRoomPwd(rMsg, *this);
}

void CEntityCoupleComponent::_OnChangeIntimacy(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_UpdateCoupleIntimacy & msgUpdate = (GameMsg_G2S_UpdateCoupleIntimacy &)rMsg;
    if (msgUpdate.m_nChangedValue != 0) {
        __int64 nResult = m_coupleInfo.GetIntimacy() + msgUpdate.m_nChangedValue;
        if (nResult > 0) {
            nResult = nResult > MAX_COUPLE_INTIMACY ? MAX_COUPLE_INTIMACY : nResult;
        }
        else {
            nResult = 0;
        }

        // 离婚过程中不能增加亲密度
        if (m_nDivorceAgreementID > 0 && m_DivorceAgreement.GetAgreementState() == eDivorceState_Running)
        {
            return;
        }

        m_coupleInfo.SetIntimacy(nResult);

        GameMsg_S2C_UpdateCoupleIntimacy msgUpdateIntimacy;
        msgUpdateIntimacy.m_nIntimacy = m_coupleInfo.GetIntimacy();
        SendPlayerMsg(&msgUpdateIntimacy);

        //log
        unsigned int nMaleID = 0;
        unsigned int nFemaleID = 0;
        if (m_pRoleAttr->GetSex() == ESexType_Male)
        {
            nMaleID = m_pRoleAttr->GetRoleID();
            nFemaleID = m_coupleInfo.m_nCoupleID;
        }
        else
        {
            nFemaleID = m_pRoleAttr->GetRoleID();
            nMaleID = m_coupleInfo.m_nCoupleID;
        }

        RoleCoupleLog(nMaleID, nFemaleID, CRoleCoupleLog::ERoleCoupleCause_AddIntimacy, m_coupleInfo.GetIntimacy(), m_coupleInfo.m_nBlessingValue);
    }
}

void CEntityCoupleComponent::_OnRoleNameChanged(GameMsg_Base & rMsg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_CoupleNameChanged & msgReceived = (GameMsg_G2S_CoupleNameChanged &)rMsg;
    m_coupleInfo.m_strCoupleName = msgReceived.m_strCoupleName;

    GameMsg_S2C_UpdateCoupleName msgUpdate;
    msgUpdate.m_strNewName = msgReceived.m_strCoupleName;
    SendPlayerMsg(&msgUpdate);
}

void CEntityCoupleComponent::_OnAddDivorceAgreementCallBack(QueryBase & query)
{
    CDivorceAgreementInfo *pAgreementInfo = (CDivorceAgreementInfo *)query.m_pAttachment;
    //int nRes = query.m_nAttachment;

    std::string strError = "";
    if (query.m_nResult != 0)
    {
        strError = "EAcceptDivorceRequest_Unkown";
    }
    else
    {
        // 		if ( nRes > 0 )
        // 		{
        // 			strError = "EAcceptDivorceRequest_Unkown";
        // 		}
        // 		else
        // 		{
        // 关联协议书
        // 			m_coupleInfo.m_nDivorceAgreementID = pAgreementInfo->m_nAgreementID;
        // 			CCoupleInfo * pcoupleinfo = new CCoupleInfo( m_coupleInfo );
        // 			AddQuery( QUERY_Couple_UpdateCoupleAgreementID, m_pRoleAttr->GetSex(), pcoupleinfo );
        m_DivorceAgreement.Reset();
        m_nDivorceAgreementID = pAgreementInfo->m_nAgreementID;
        AddQuery(QUERY_Couple_UpdateCoupleAgreementID, m_pRoleAttr->GetRoleID(), NULL, pAgreementInfo->m_nAgreementID);

        // 设置自己协议书内容
        CDivorceAgreementInfo agreementinfo;
        agreementinfo.m_nAgreementID = pAgreementInfo->m_nAgreementID;

        agreementinfo.m_nMaleRoleID = pAgreementInfo->m_nMaleRoleID;
        agreementinfo.m_strMaleRoleName = pAgreementInfo->m_strMaleRoleName;
        agreementinfo.m_bMaleIsAgree = pAgreementInfo->m_bMaleIsAgree;
        agreementinfo.m_nFemaleRoleID = pAgreementInfo->m_nFemaleRoleID;
        agreementinfo.m_strFemaleRoleName = pAgreementInfo->m_strFemaleRoleName;
        agreementinfo.m_bFemaleIsAgree = pAgreementInfo->m_bFemaleIsAgree;
        agreementinfo.m_eState = eDivorceState_Running;
        agreementinfo.m_nProposerID = pAgreementInfo->m_nProposerID;
        agreementinfo.m_nSmallRoleID = pAgreementInfo->m_nSmallRoleID;
        m_DivorceAgreement.UpdateDivorceAgreementInfo(agreementinfo);

        // 行为
        CDivorceAction action;
        action.m_eType = eDivorceAction_Apply;
        action.m_nTime = (unsigned int)time(NULL);
        action.m_nProposerID = m_pRoleAttr->GetRoleID();
        action.m_nAgreementID = m_DivorceAgreement.GetAgreementID();
        m_DivorceAgreement.AddDivorceAction(action);

        // 反馈
        GameMsg_S2C_RequestDivorceSuc msgSuc;
        msgSuc.m_nSmallRoleID = pAgreementInfo->m_nSmallRoleID;
        SendPlayerMsg(&msgSuc);

        // 设置对方协议书内容
        CRoleEntity * pCoupleEntity = CPlayerManager::Instance().GetEntityByRoleID(m_coupleInfo.m_nCoupleID);
        if (pCoupleEntity != NULL)
        {
            CDivorceAgreement & agreement = pCoupleEntity->GetRoleCouple()->GetDivorceAgreement();
            agreement.Reset();
            pCoupleEntity->GetRoleCouple()->SetSelfDivorceAgreementID(agreementinfo.GetAgreementID());
            agreement.UpdateDivorceAgreementInfo(agreementinfo);
            agreement.AddDivorceAction(action, false, false, false);

            pCoupleEntity->SendPlayerMsg(&msgSuc);
        }
        AddQuery(QUERY_Couple_UpdateCoupleAgreementID, m_coupleInfo.m_nCoupleID, NULL, pAgreementInfo->m_nAgreementID);
        //}
    }

}

void CEntityCoupleComponent::_OnAddCoupleCallBack(QueryBase & query)
{
    CCoupleInfoDB *pCoupleInfo = (CCoupleInfoDB *)query.m_pAttachment;
    unsigned int nRequestorRoleID = (unsigned int)query.m_nTag;
    std::string strError = "";

    CRoleEntity *pRequestorEntity = CPlayerManager::Instance().GetEntityByRoleID(nRequestorRoleID);

    if (query.m_nResult != Query_Res_OK || pCoupleInfo == NULL || nRequestorRoleID == 0)
    {
        strError = "EAcceptCoupleRequest_Unkown";
    }
    else if (m_pRoleAttr != NULL)
    {
        m_coupleInfo.m_nPairID = pCoupleInfo->m_nPairID;
        m_coupleInfo.m_nCoupleID = nRequestorRoleID;
        m_coupleInfo.m_nCoupleDate = pCoupleInfo->m_nCoupleDate;
        m_coupleInfo.m_nSmallRoleID = pCoupleInfo->m_nSmallRoleID;
        if (pRequestorEntity != NULL)
            m_coupleInfo.m_strCoupleName = pRequestorEntity->GetRoleName();

        GameMsg_S2C_AcceptCoupleRequestSuc msgSuc;
        msgSuc.m_nPairID = m_coupleInfo.m_nPairID;
        msgSuc.m_nCoupleID = m_coupleInfo.m_nCoupleID;
        msgSuc.m_strCoupleName = m_coupleInfo.m_strCoupleName;
        msgSuc.m_nCoupleDate = m_coupleInfo.m_nCoupleDate;
        msgSuc.m_nSmallRoleID = m_coupleInfo.m_nSmallRoleID;

        SendPlayerMsg(&msgSuc);

        GameMsg_S2G_AcceptCoupleRequest msgAccept;
        msgAccept.m_nPairID = pCoupleInfo->m_nPairID;
        msgAccept.m_nMaleID = pCoupleInfo->m_nMaleID;
        msgAccept.m_nFemaleID = pCoupleInfo->m_nFemaleID;
        msgAccept.m_nCoupleDate = pCoupleInfo->m_nCoupleDate;
        msgAccept.m_nSmallRoleID = pCoupleInfo->m_nSmallRoleID;

        SendMsg2GroupServer(&msgAccept);

        if (m_pRoleDungeon != NULL)
            m_pRoleDungeon->RemoveInviteFriendInfo(nRequestorRoleID);

        CheckCoupleQuest();
    }

    if (!strError.empty())
    {
        GameMsg_S2C_AcceptCoupleRequestFail msgFail;
        msgFail.m_strError = strError;

        SendPlayerMsg(&msgFail);
    }
    else if (m_pRoleAttr != NULL)
    {
        // log
        unsigned int nMaleID = 0;
        unsigned int nFemaleID = 0;

        if (m_pRoleAttr->GetSex() == ESexType_Male)
        {
            nMaleID = m_pRoleAttr->GetRoleID();
            nFemaleID = nRequestorRoleID;
        }
        else
        {
            nFemaleID = m_pRoleAttr->GetRoleID();
            nMaleID = nRequestorRoleID;
        }

        RoleCoupleLog(nMaleID, nFemaleID, CRoleCoupleLog::ERoleCoupleCause_ToLover,
            m_coupleInfo.GetIntimacy(), m_coupleInfo.m_nBlessingValue);
    }

    if (pRequestorEntity != NULL && m_pRoleAttr != NULL)
    {
        CCoupleRequestReplay replay;
        replay.m_nPairID = m_coupleInfo.m_nPairID;
        replay.m_nTargetID = m_pRoleAttr->GetRoleID();
        replay.m_strTargetName = m_pRoleAttr->GetRoleName();
        replay.m_nCoupleDate = m_coupleInfo.m_nCoupleDate;

        if (strError.empty())
            replay.m_bAgree = true;
        else
            replay.m_bAgree = false;

        replay.m_strReason = strError;
        replay.m_nSmallRoleID = m_coupleInfo.m_nSmallRoleID;

        pRequestorEntity->SendEvent(eComponentEvent_CoupleRequestReplay, &replay);
    }

    delete pCoupleInfo, pCoupleInfo = NULL;
}

bool CEntityCoupleComponent::StartWedding(int & nRes, std::string & strError)
{
    if (_CanStartWedding(nRes, strError))
    {
        GetMarried();
    }

    return nRes == ERequestMarry_Suc;
}

void CEntityCoupleComponent::GetMarried()
{
    CCoupleInfo *pCoupleInfo = new CCoupleInfo(m_coupleInfo);
    if (pCoupleInfo == NULL)
    {
        return;
    }

    if (m_pWeddingRoom == NULL)
    {
        return;
    }

    // 对方
    unsigned int nTimeNow = time(NULL);
    unsigned int nBlessingValue = m_pWeddingRoom->GetBlessingValue() + m_coupleInfo.m_nBlessingValue;
    CRoleEntity *pCouple = CPlayerManager::Instance().GetEntityByRoleID(m_coupleInfo.m_nCoupleID);
    if (pCouple != NULL)
    {
        if (!IsMarried())
        {
            pCouple->SetMarriageDate(nTimeNow);
            if (pCouple->GetComponent<CRoleCouple>() != NULL)
            {
                pCouple->GetComponent<CRoleCouple>()->SendMarriedItem();
            }
        }
        pCouple->SetCoupleBlessingValue(nBlessingValue);
    }

    // 自己
    if (!IsMarried())
    {
        SendMarriedItem();
        pCoupleInfo->m_nMarriageDate = nTimeNow;
        m_coupleInfo.m_nMarriageDate = nTimeNow;
    }
    m_coupleInfo.m_nBlessingValue = nBlessingValue;
    pCoupleInfo->m_nBlessingValue = nBlessingValue;

    //  同步到group
    GameMsg_S2G_GetMarried msgNotify;
    msgNotify.m_nPairID = pCoupleInfo->m_nPairID;
    msgNotify.m_nMaleRoleID = m_pWeddingRoom->GroomID();
    msgNotify.m_strMaleRoleName = m_pWeddingRoom->GetGroomName();
    msgNotify.m_nFemaleRoleID = m_pWeddingRoom->BrideID();
    msgNotify.m_strFemaleRoleName = m_pWeddingRoom->GetBrideName();
    msgNotify.m_nMarriageDate = nTimeNow;
    msgNotify.m_nBlessingValue = nBlessingValue;
    ::SendMsg2GroupServer(&msgNotify);

    // 更新婚礼状态
    GameMsg_S2G_UpdateWeddingRecordState s2gRecordMsg;
    s2gRecordMsg.m_nDoType = 1;
    s2gRecordMsg.m_nPairID = m_coupleInfo.m_nPairID;
    s2gRecordMsg.m_bIsRunning = false;
    ::SendMsg2GroupServer(&s2gRecordMsg);

    // DB
    CDelHelper delHelper(pCoupleInfo);
    AddQuery(QUERY_Couple_UpdateCoupleInfo, 0, pCoupleInfo, 0, NULL, &delHelper);

    // 广播
    std::string strBuff;
    SafeFormat(strBuff, CLocalization::Instance().GetString("Couple_Wedding_Get_Married_Notification_To_All"),
        m_pRoleAttr->GetRoleName(), pCouple->GetRoleName());
    GameMsg_S2C_Chat ChatMsg(CLocalization::Instance().GetString("System").c_str(),
        0, eChatSystem, CHATCOLOR_GLOBALCHANNEL, strBuff.c_str(), 0, EChatExtraType_Default, false, 0, false, 0);
    g_pCirculator->BroadCastMsg2AllClient(&ChatMsg);

    // 前端反馈
    GameMsg_S2C_StartWedding msgWeddingBegin;
    msgWeddingBegin.m_nBlessingValue = m_pWeddingRoom->GetBlessingValue();
    msgWeddingBegin.m_nGroomID = m_pRoleAttr->GetSex() == ESexType_Male ? m_pRoleAttr->GetRoleID() : m_coupleInfo.m_nCoupleID;
    msgWeddingBegin.m_BrideID = m_pRoleAttr->GetSex() == ESexType_Female ? m_pRoleAttr->GetRoleID() : m_coupleInfo.m_nCoupleID;
    msgWeddingBegin.m_nMarriedTime = m_coupleInfo.m_nMarriageDate;
    m_pWeddingRoom->SendMsgToAll(&msgWeddingBegin);

    // 发放祝福礼物
    m_pWeddingRoom->SendGiftToBlessinger();
    m_pWeddingRoom->SendGiftToCouple();

    // log
    RoleCoupleLog(m_pWeddingRoom->GroomID(), m_pWeddingRoom->BrideID(), CRoleCoupleLog::ERoleCoupleCause_ToCouple, m_coupleInfo.GetIntimacy(), m_coupleInfo.m_nBlessingValue);
    RoleCoupleLog(m_pWeddingRoom->GroomID(), m_pWeddingRoom->BrideID(), CRoleCoupleLog::ERoleCoupleCause_AddBlessingValue, m_coupleInfo.GetIntimacy(), m_coupleInfo.m_nBlessingValue);
}

void CEntityCoupleComponent::ChangeIntimacy(int nValue, EChangeIntimacyCause eCause, int nPara)
{
    if (nValue != 0 && !IsSingle()) {
        __int64 nResult = m_coupleInfo.GetIntimacy() + nValue;
        if (nResult > 0) {
            nResult = nResult > MAX_COUPLE_INTIMACY ? MAX_COUPLE_INTIMACY : nResult;
        }
        else {
            nResult = 0;
        }

        // 离婚过程中不能增加亲密度
        if (m_nDivorceAgreementID > 0 && m_DivorceAgreement.GetAgreementState() == eDivorceState_Running)
        {
            return;
        }

        m_coupleInfo.SetIntimacy(nResult);
        AddQuery(QUERY_Couple_UpdateIntimacy, m_coupleInfo.m_nPairID, NULL, nValue);

        GameMsg_S2C_UpdateCoupleIntimacy msgUpdate;
        msgUpdate.m_nIntimacy = nResult;
        SendPlayerMsg(&msgUpdate);

        unsigned int nMaleID = 0;
        unsigned int nFemaleID = 0;
        GetCoupleRoleID(nMaleID, nFemaleID);

        GameMsg_S2G_UpdateCoupleIntimacy msgToGroup;
        if (m_pRoleAttr != NULL) {
            msgToGroup.m_nTriggerID = m_pRoleAttr->GetRoleID();
        }
        msgToGroup.m_nCoupleID = m_coupleInfo.m_nCoupleID;
        msgToGroup.m_nChangedValue = nValue;
        msgToGroup.m_nPairID = m_coupleInfo.m_nPairID;
        msgToGroup.m_bMarried = IsMarried();
        msgToGroup.m_bMale = (m_coupleInfo.m_nCoupleID == nFemaleID) ? true : false;
        msgToGroup.m_nSmallRoleID = m_coupleInfo.m_nSmallRoleID;
        ::SendMsg2GroupServer(&msgToGroup);

        //log
        RoleCoupleLog(nMaleID, nFemaleID, CRoleCoupleLog::ERoleCoupleCause_AddIntimacy, m_coupleInfo.GetIntimacy(), m_coupleInfo.m_nBlessingValue);
    }
}

void CEntityCoupleComponent::SendMarriedItem()
{
    std::string strMailTitle = CLocalization::Instance().GetString("Couple_Wedding_Married_Send_Item_Mail_Title");
    std::string strMailContent = CLocalization::Instance().GetString("Couple_Wedding_Married_Send_Item_Mail_Content");

    std::list<CItem> listItem;
    CItem item;
    item.m_nItemType = C_ITEM_COMMON_RING;
    item.m_nItemCount = 1;
    item.m_nValidTime = -1;

    listItem.push_back(item);
    CItemProcess::AddOrMailItems(*(CRoleEntity *)Entity(), listItem, EItemAction_Add_Married, 0, true, EMailType_GotMarried, strMailTitle, strMailContent);
}

void CEntityCoupleComponent::DeleteMarriedItem()
{
    if (m_pRoleItem != NULL)
    {
        //m_pRoleItem->RemoveItemByType( C_ITEM_COMMON_RING, 1, EItemAction_Del_Divorce, 0, false);

        //婚戒会放入婚戒栏位
        m_pRoleItem->DeleteWeddingRing();
        m_pRoleMail->DeleteWeddingRingMail();
    }
}

bool CEntityCoupleComponent::AutoStartWedding(int & nRes, std::string & strError)
{
    CRoleEntity *pCouple = CPlayerManager::Instance().GetEntityByRoleID(m_coupleInfo.m_nCoupleID);
    if (pCouple == NULL || m_pWeddingRoom != pCouple->WeddingRoom())
    {
        nRes = EAutoStartWeddingRoom_NotAllInRoom;
        strError = "EAutoStartWeddingRoom_NotAllInRoom";
    }
    else if (m_coupleInfo.m_nPairID == 0 || pCouple->PairID() == 0 || m_coupleInfo.m_nPairID != pCouple->PairID()
        || m_coupleInfo.m_nCoupleID != pCouple->GetRoleID() || m_pRoleAttr->GetRoleID() != pCouple->CoupleID()) // 非情侣
    {
        nRes = EAutoStartWeddingRoom_NotCouple;
        strError = "EAutoStartWeddingRoom_NotCouple";
    }
    else if (IsMarried()) // 已结婚
    {
        nRes = EAutoStartWeddingRoom_HasMarried;
        strError = "EAutoStartWeddingRoom_HasMarried";
    }
    else if (pCouple->IsMarried()) // 对方已结婚
    {
        nRes = EAutoStartWeddingRoom_OthersHasMarried;
        strError = "EAutoStartWeddingRoom_OthersHasMarried";
    }
    else if (m_coupleInfo.GetIntimacy() < g_nMinIntimacyMarriageRequired
        || (int)pCouple->GetRoleCouple()->Intimacy() < g_nMinIntimacyMarriageRequired)
    {
        nRes = EAutoStartWeddingRoom_LackOfIntimacy;
        strError = "EAutoStartWeddingRoom_LackOfIntimacy";
    }
    else if (pCouple->GetRoleCouple()->GetDivorceAgreement().GetAgreementState() == eDivorceState_Running || m_DivorceAgreement.GetAgreementState() == eDivorceState_Running)
    {
        nRes = EAutoStartWeddingRoom_InDivorce;
        strError = "EAutoStartWeddingRoom_InDivorce";
    }
    else
    {
        GetMarried();
    }

    return nRes == 0;
}

bool CEntityCoupleComponent::_HasReceiveCoupleRequest(unsigned int nSenderID)
{
    for (std::list<CCoupleRequest *>::iterator itr = m_listCoupleRequest.begin(); itr != m_listCoupleRequest.end(); ++itr) {
        if (*itr != NULL && (*itr)->m_nRoleID == nSenderID) {
            return true;
        }
    }

    return false;
}

void CEntityCoupleComponent::_AddCoupleRequest(CCoupleRequest * pCoupleRequest)
{
    if (pCoupleRequest != NULL) {
        m_listCoupleRequest.push_back(pCoupleRequest);
    }
}

void CEntityCoupleComponent::_RemoveCoupleRequest(unsigned int nSenderID)
{
    for (std::list<CCoupleRequest *>::iterator itr = m_listCoupleRequest.begin();
        itr != m_listCoupleRequest.end(); ++itr)
    {
        CCoupleRequest *request = *itr;
        if (request != NULL && request->m_nRoleID == nSenderID)
        {
            delete request;
            m_listCoupleRequest.erase(itr);
            return;
        }
    }
}

bool CEntityCoupleComponent::CheckCoupleSex(char cSelfSex, char cOtherSex)
{
    if (cSelfSex == ESexType_No || cOtherSex == ESexType_No)
    {
        return false;
    }

    if (!CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_Homosexual))
    {
        if (cSelfSex == cOtherSex)
        {
            return false;
        }
    }

    return true;
}

bool CEntityCoupleComponent::IsSingle()
{
    return m_coupleInfo.m_nPairID == 0;
}

bool CEntityCoupleComponent::IsDivorceing(unsigned int nTargetID)
{
    if (CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_NewDivorce))
    {
        if (m_coupleInfo.m_nPairID != 0)
        {
            if (m_nDivorceAgreementID > 0 && m_coupleInfo.m_nCoupleID == nTargetID && m_DivorceAgreement.GetAgreementState() == eDivorceState_Running)
            {
                return true;
            }
        }
    }

    return false;
}

bool CEntityCoupleComponent::CanHaveNewCouple(unsigned int nTargetID)
{
    if (!CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_NewDivorce))
    {
        return m_coupleInfo.m_nPairID == 0;
    }
    else
    {
        if (m_coupleInfo.m_nPairID != 0)
        {
            if (m_nDivorceAgreementID > 0 && m_coupleInfo.m_nCoupleID != nTargetID)
            {
                return true;
            }

            return false;
        }

        return true;
    }
}

unsigned int CEntityCoupleComponent::RoleID()
{
    if (m_pRoleAttr != NULL) {
        return m_pRoleAttr->m_nRoleID;
    }

    return 0;
}

const CCoupleInfo & CEntityCoupleComponent::CoupleInfo()
{
    return m_coupleInfo;
}

unsigned int CEntityCoupleComponent::PairID()
{
    return m_coupleInfo.m_nPairID;
}

unsigned int CEntityCoupleComponent::CoupleID()
{
    return m_coupleInfo.m_nCoupleID;
}

const std::string & CEntityCoupleComponent::CoupleName()
{
    return m_coupleInfo.m_strCoupleName;
}

unsigned int CEntityCoupleComponent::Intimacy()
{
    return m_coupleInfo.GetIntimacy();
}

unsigned int CEntityCoupleComponent::CoupleDate()
{
    return m_coupleInfo.m_nCoupleDate;
}

unsigned int CEntityCoupleComponent::MarriageDate()
{
    return m_coupleInfo.m_nMarriageDate;
}

bool CEntityCoupleComponent::GetCoupleRoleID(unsigned int &rMaleRoleID, unsigned int &rFemaleRoleID)
{
    if (!IsMarried() || NULL == m_pRoleAttr || ESexType_No == m_pRoleAttr->GetSex())
        return false;

    unsigned int nMyRoleID = m_pRoleAttr->GetRoleID();
    unsigned char nMySex = m_pRoleAttr->GetSex();

    if (0 == m_coupleInfo.m_nSmallRoleID)
    {
        if (ESexType_Male == nMySex)
        {
            rMaleRoleID = nMyRoleID;
            rFemaleRoleID = m_coupleInfo.m_nCoupleID;
        }
        else
        {
            rMaleRoleID = m_coupleInfo.m_nCoupleID;
            rFemaleRoleID = nMyRoleID;
        }
    }
    else if (m_coupleInfo.m_nSmallRoleID == nMyRoleID)
    {
        rMaleRoleID = m_coupleInfo.m_nCoupleID;
        rFemaleRoleID = nMyRoleID;
    }
    else
    {
        rMaleRoleID = nMyRoleID;
        rFemaleRoleID = m_coupleInfo.m_nCoupleID;
    }

    return true;
}

bool CEntityCoupleComponent::IsMarried()
{
    return m_coupleInfo.m_nPairID > 0 && m_coupleInfo.m_nCoupleID > 0 && m_coupleInfo.m_nMarriageDate > 0;
}

void CEntityCoupleComponent::ResetCoupleData()
{
    m_coupleInfo.m_nPairID = 0;
    m_coupleInfo.m_nCoupleID = 0;
    m_coupleInfo.m_strCoupleName = "";
    m_coupleInfo.SetIntimacy(0);
    m_coupleInfo.m_nCoupleDate = 0;
    m_coupleInfo.m_nMarriageDate = 0;
    m_coupleInfo.m_nBlessingValue = 0;
    m_coupleInfo.m_nMarriageGiftDate = 0;
    m_coupleInfo.m_nSmallRoleID = 0;
}

CWeddingRoom * CEntityCoupleComponent::WeddingRoom()
{
    return m_pWeddingRoom;
}

void CEntityCoupleComponent::SetRoomRoleType(EWeddingRoomRoleType eRoomRoleType)
{
    if (eRoomRoleType > EWeddingRoomRoleType_None && eRoomRoleType < EWeddingRoomRoleType_Max) {
        m_eRoomRoleType = eRoomRoleType;
    }
}

EWeddingRoomRoleType CEntityCoupleComponent::RoomRoleType()
{
    return m_eRoomRoleType;
}

bool CEntityCoupleComponent::IsHost() const
{
    return m_bHost;
}

void CEntityCoupleComponent::InWeddingRoom(CWeddingRoom *pRoom, bool bHost)
{
    if (pRoom != NULL) {
        m_pWeddingRoom = pRoom;
        m_bHost = bHost;
        m_eRoomRoleType = EWeddingRoomRoleType_Guest;
        if (pRoom->Valid())
        {
            if (m_pRoleAttr != NULL)
            {
                if (pRoom->GroomID() == m_pRoleAttr->GetRoleID())
                {
                    m_eRoomRoleType = EWeddingRoomRoleType_Groom;
                }
                else if (pRoom->BrideID() == m_pRoleAttr->GetRoleID())
                {
                    m_eRoomRoleType = EWeddingRoomRoleType_Bride;
                }
            }
        }
    }
}

void CEntityCoupleComponent::OutWeddingRoom()
{
    m_pWeddingRoom = NULL;
    m_bHost = false;
    m_eRoomRoleType = EWeddingRoomRoleType_None;
    if (m_pRoleInfo != NULL) {
        m_pRoleInfo->ResetRolePosition();
    }
    if (m_pRoleQuestNew != NULL)
    {
        m_pRoleQuestNew->OnAttributeChange();
    }

    m_pRoleHorse->LeaveRoomHorseTask();
}

void CEntityCoupleComponent::SetRoomPlayerInfo(CWeddingRoomPlayerInfo & rRoomPlayer)
{
    if (m_pRoleAttr != NULL && m_pRoleDanceGroup != NULL) {
        rRoomPlayer.m_nRoleID = m_pRoleAttr->GetRoleID();
        rRoomPlayer.m_strRoleName = m_pRoleAttr->GetRoleName();
        rRoomPlayer.m_bIsHost = m_bHost;
        rRoomPlayer.m_eRoleType = (unsigned char)m_eRoomRoleType;
        rRoomPlayer.m_eRoleSex = m_pRoleAttr->GetSex();
        rRoomPlayer.m_nRoleSkin = m_pRoleAttr->GetColor();
        rRoomPlayer.m_nIsVip = m_pRoleVip->IsVIP();
        rRoomPlayer.m_nVipLv = (unsigned short)m_pRoleVip->VIPLevel();
        rRoomPlayer.m_nTransFormID = (unsigned short)m_pRoleAttr->GetTransformId();


        CDanceGroupInfo *pDanceGroup = m_pRoleDanceGroup->GetDanceGroupInfo();
        if (pDanceGroup != NULL) {
            rRoomPlayer.m_strDanceGroupName = pDanceGroup->m_DGBaseInfo.m_strGroupName;
            rRoomPlayer.m_nDanceGroupBadge = pDanceGroup->m_DGBaseInfo.m_nBadge;
            rRoomPlayer.m_nDanceGroupEffect = pDanceGroup->m_DGBaseInfo.m_nEffect;
            CDanceGroupMember * pDGMember = pDanceGroup->GetGroupMember(m_pRoleAttr->GetRoleID());
            if (pDGMember != NULL) {
                rRoomPlayer.m_nGroupTitle = (char)pDGMember->m_nTitle;
            }
        }
        if (m_pRoleInfo != NULL) {
            rRoomPlayer.m_playerPosition = m_pRoleInfo->m_position;
        }
        rRoomPlayer.m_pRoleItem = m_pRoleItem;
        rRoomPlayer.m_pRoleGene = m_pRoleGene;
        rRoomPlayer.m_pRolePet = m_pRolePet;
        rRoomPlayer.m_pRoleMedal = m_pRoleMedal;
        rRoomPlayer.m_pRoleHorse = m_pRoleHorse;
    }
}

bool CEntityCoupleComponent::_CanStartWedding(int & nRes, std::string & strError)
{
    nRes = ERequestMarry_Suc;

    CRoleEntity *pCouple = CPlayerManager::Instance().GetEntityByRoleID(m_coupleInfo.m_nCoupleID);
    if (pCouple == NULL)
    {
        nRes = ERequestMarry_OtherNotOnline;
        strError = "ERequestMarry_OtherNotOnline";
    }
    else if (m_pWeddingRoom == NULL) //在房间
    {
        nRes = ERequestMarry_NotInWeddingRoom;
        strError = "ERequestMarry_NotInWeddingRoom";
    }
    else if (!m_pWeddingRoom->Valid())
    {
        nRes = ERequestMarry_InValidRoom;
        strError = "ERequestMarry_InValidRoom";
    }
    else if (m_pRoleAttr == NULL || m_pWeddingRoom->HostID() != m_pRoleAttr->GetRoleID()) // 非房主
    {
        nRes = ERequestMarry_NotHost;
        strError = "ERequestMarry_NotHost";
    }
    else if (m_pWeddingRoom != pCouple->WeddingRoom()) // 不在同一个房间
    {
        nRes = ERequestMarry_OthersNotInTheSameRoom;
        strError = "ERequestMarry_OthersNotInTheSameRoom";
    }
    else if (m_coupleInfo.m_nPairID == 0 || pCouple->PairID() == 0 || m_coupleInfo.m_nPairID != pCouple->PairID()
        || m_coupleInfo.m_nCoupleID != pCouple->GetRoleID() || m_pRoleAttr->GetRoleID() != pCouple->CoupleID()) // 非情侣
    {
        nRes = ERequestMarry_NotCouple;
        strError = "ERequestMarry_NotCouple";
    }
    else if (pCouple->GetRoleCouple()->GetDivorceAgreement().GetAgreementState() == eDivorceState_Running || m_DivorceAgreement.GetAgreementState() == eDivorceState_Running)
    {
        nRes = ERequestMarry_InDivorce;
        strError = "ERequestMarry_InDivorce";
    }
    else if (m_coupleInfo.GetIntimacy() < g_nMinIntimacyMarriageRequired
        || (int)pCouple->GetRoleCouple()->Intimacy() < g_nMinIntimacyMarriageRequired)
    {
        nRes = ERequestMarry_LackOfIntimacy;
        strError = "ERequestMarry_LackOfIntimacy";
    }

    if (nRes != ERequestMarry_Suc) {
        return false;
    }

    return true;
}

void CEntityCoupleComponent::SendPlayerMsg(GameMsg_Base *pMsg)
{
    if (m_pRoleNet != NULL && pMsg != NULL) {
        m_pRoleNet->SendPlayerMsg(pMsg);
    }
}

unsigned int CEntityCoupleComponent::GetBlessingValue()
{
    return m_coupleInfo.m_nBlessingValue;
}

void CEntityCoupleComponent::SendMarriageDateGift(unsigned int nDate)
{
    //  设置时间
    SetMarriageGiftDate(nDate);

    CRoleEntity * pCoupleEntity = CPlayerManager::Instance().GetEntityByRoleID(m_coupleInfo.m_nCoupleID);
    if (pCoupleEntity != NULL)
    {
        pCoupleEntity->GetRoleCouple()->SetMarriageGiftDate(nDate);
    }

    g_pQueryMgr->AddQuery(QUERY_Couple_UpdateMarriageGiftDate, m_coupleInfo.m_nPairID, NULL, nDate);


    //  发送邮件
    std::string strMailTitle(CLocalization::Instance().GetString("Couple_MarriageDate_Gift_Title"));
    std::string strMailContent(CLocalization::Instance().GetString("Couple_MarriageDate_Gift_Content"));
    if (m_pRoleAttr->GetSex() == ESexType_Male)
    {
        CMailMgr::Instance().SendSystemMailToPlayer( m_pRoleAttr->GetRoleID(), EMailType_WeddingDate,
            strMailTitle, strMailContent, CCoupleConfigDataMgr::Instance().GetMaleReward() );
        CMailMgr::Instance().SendSystemMailToPlayer( m_coupleInfo.m_nCoupleID, EMailType_WeddingDate,
            strMailTitle, strMailContent, CCoupleConfigDataMgr::Instance().GetFemaleReward() );
    }
    else
    {
        CMailMgr::Instance().SendSystemMailToPlayer( m_pRoleAttr->GetRoleID(), EMailType_WeddingDate,
            strMailTitle, strMailContent, CCoupleConfigDataMgr::Instance().GetFemaleReward() );
        CMailMgr::Instance().SendSystemMailToPlayer( m_coupleInfo.m_nCoupleID, EMailType_WeddingDate,
            strMailTitle, strMailContent, CCoupleConfigDataMgr::Instance().GetMaleReward() );
    }
}

void CEntityCoupleComponent::SetMarriageGiftDate(unsigned int nDate)
{
    m_coupleInfo.m_nMarriageGiftDate = nDate;
}

void CEntityCoupleComponent::CheckCoupleQuest()
{
    if (m_pRoleQuest != NULL)
        m_pRoleQuest->PushFresherQuestStateChange();

    if (m_pRoleQuestNew != NULL)
        m_pRoleQuestNew->OnAttributeChange();
}

void CEntityCoupleComponent::CoupleTest(int nPara1, int nPara2, int nPara3)
{
    //int nIndex = nPara1;
    int nIndex = -1;
    CSlotPeer slotPeer;
    switch (nIndex)
    {
    case 0:		// send couple request
    {
                    GameMsg_C2S_SendCoupleRequest msgRequet;
                    msgRequet.m_nTargetID = 178;
                    _OnSendCoupleRequest(msgRequet, slotPeer);
                    m_pRoleChat->SysChat("send couple request");
    }
        break;
    case 1:		// accept couple request
    {
                    GameMsg_C2S_AcceptCoupleRequest  msgAccept;
                    msgAccept.m_nTargetID = 165;
                    _OnAcceptCoupleRequest(msgAccept, slotPeer);
                    m_pRoleChat->SysChat("accept couple request");
    }
        break;
    case 2:		// refuse couple request
    {
                    GameMsg_C2S_RefuseCoupleRequest msgRefuse;
                    msgRefuse.m_nTargetID = 165;
                    msgRefuse.m_strReason = "i am sorry";
                    _OnRefuseCoupleRequest(msgRefuse, slotPeer);
                    m_pRoleChat->SysChat("refuse couple request");
    }
        break;
    case 3:		// separate
    {
                    GameMsg_C2S_CoupleSeparateRequest msgSeparate;
                    msgSeparate.m_nTargetID = 178;
                    _OnCoupleSeparateRequest(msgSeparate, slotPeer);
                    m_pRoleChat->SysChat("separate");
    }
        break;
    case 4:		// divorce
    {
                    GameMsg_C2S_RequestDivorce msgDivorce;
                    _OnRequestDivorce(msgDivorce, slotPeer);
                    m_pRoleChat->SysChat("divorce");
    }
        break;
    case 5:		// create room
    {
                    GameMsg_C2S_CreateWeddingRoom msgCreate;
                    msgCreate.m_nWeddingType = EWeddingType_Traditional;
                    msgCreate.m_strRoomName = "Hi all";
                    msgCreate.m_strPassword = "123456";

                    _OnCreateWeddingRoom(msgCreate, slotPeer);
                    m_pRoleChat->SysChat("create room");
    }
        break;
    case 6:		// try enter room
    {
                    GameMsg_C2S_TryEnterWeddingRoom msgTryEnterRoom;
                    msgTryEnterRoom.m_nRoomID = 1;
                    _OnTyrEnterWeddingRoom(msgTryEnterRoom, slotPeer);
                    m_pRoleChat->SysChat("try enter room");
    }
        break;
    case 7:		// enter room
    {
                    GameMsg_C2S_EnterWeddingRoom msgEnter;
                    msgEnter.m_nRoomID = 1;
                    msgEnter.m_strRoomPassword = "123456";
                    _OnEnterWeddingRoom(msgEnter, slotPeer);
                    m_pRoleChat->SysChat("enter room");
    }
        break;
    case 8:		// exit room
    {
                    GameMsg_C2S_ExitWeddingRoom msgExit;
                    _OnExitWeddingRoom(msgExit, slotPeer);
                    m_pRoleChat->SysChat("exit room");
    }
        break;
    case 9:		// getting marry
    {
                    GameMsg_C2S_RequestMarry msgMarry;
                    _OnRequestMarry(msgMarry, slotPeer);
                    m_pRoleChat->SysChat("getting marry");
    }
        break;
    case 10:	// kick player
    {
                    GameMsg_C2S_KickWeddingRoomPlayer msgKickPlayer;
                    msgKickPlayer.m_nTargetID = nPara2;
                    _OnKickWeddingRoomPlayer(msgKickPlayer, slotPeer);
                    m_pRoleChat->SysChat("kick wedding room player");
    }
        break;
    case 11:	// enter my wedding room
    {
                    GameMsg_C2S_EnterMyWeddingRoom msgEnter;
                    _OnEnterMyWeddingRoom(msgEnter, slotPeer);
                    m_pRoleChat->SysChat("Enter my wedding room");
    }
        break;
    case 12:	// set wedding player room type 
    {
                    GameMsg_C2S_SetWeddingPlayerType msgSetting;
                    msgSetting.m_nTargetID = nPara2;
                    msgSetting.m_nRoomRoleType = (unsigned char)nPara3;
                    _OnSetWeddingPlayerType(msgSetting, slotPeer);
                    m_pRoleChat->SysChat("set wedding player room type");
    }
        break;
    case 13:	//change wedding room password
    {
                    GameMsg_C2S_ChangeWeddingRoomPwd msgRequest;
                    msgRequest.m_strNewPwd = "1";
                    _OnChangeWeddingRoomPwd(msgRequest, slotPeer);
                    m_pRoleChat->SysChat("change wedding room password");
    }
        break;
    case 14:	// change intimacy
    {
                    ChangeIntimacy(nPara2, EChangeIntimacyCause_Debug, 0);
                    m_pRoleChat->SysChat("change intimacy");
    }
        break;

    default:
        ;
    }
}

