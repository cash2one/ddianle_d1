#include "StdAfx.h"

#include "RoleEntity.h"
#include "DataStruct_Base.h"
#include "EquipItem.h"
#include "ItemErrorInfo.h"
#include "../server/libServerFrame/QueryManager.h"
#include "datainfor/ConfigManager.h"
#include "LogicInitData.h"

#include "../server/attribute/EntityAttributeComponent.h"
#include "../server/EntityNetComponent/EntityNetComponent.h"
#include "../server/chat/EntityChatComponent.h"
#include "../server/room/RoomComponent.h"
#include "../server/item/EntityItemComponent.h"
#include "../server/room/RoomManager.h"
#include "../server/vip/EntityVIPComponent.h"
#include "../server/friend/EntityFriendComponent.h"
#include "../server/gene/EntityGeneComponent.h"
#include "../server/quest/EntityQuestComponent.h"
#include "../server/Couple/EntityCoupleComponent.h"
#include "../server/PhotoRoom/EntityPhotoRoomComponent.h"
#include "../server/AmuseRoom/AmuseRoomComponent.h"
#include "../server/chat/ChatManager.h"
#include "../server/chat/SystemChat.h"
#include "../server/login/GameMsg_Player_CreateRoleRes.h"
#include "../server/dancegroup/EntityDanceGroupComponent.h"
#include "../server/gene/EntityGeneComponent.h"
#include "../server/Dungeon/EntityDungeonComponent.h"

#include "../server/chat/GameMsg_S2C_Chat.h"
#include "../server/libServerFrame/GameMsgCacheEnt.h"

#include "../server/Medal/EntityMedalComponent.h"
#include "../server/Horse/EntityHorseComponent.h"


#ifndef WIN32
#define _vsnprintf	vsnprintf
#endif

#include <stdarg.h>
#include <stdexcept>

using namespace std;

extern CQueryManager		*g_pQueryMgr;
extern void SendMsg2AccountServer(LPGameMsg_Base pMsg);


CRoleEntity::CRoleEntity(void)
{
	m_EntityType = EntityType_CRoleEntity;

	m_bValid = false;	//调用CreateRole后才可以设为true
	m_nSaveTime = 0;
	m_bOffLine = false;
	m_tLBSUpPos = 0;
	m_tLBSGetNearby = 0;
	m_bUpdatedLBSPositon = true;
    m_nRandomSaveCycle = (unsigned int)Random(CLogicInitData::Instance().m_nAutoTimeMin, CLogicInitData::Instance().m_nAutoTimeMax);
}

CRoleEntity::~CRoleEntity(void)
{
}

unsigned int CRoleEntity::GetRoleID() const
{
	CRoleAttribute * pAttr = GetComponent<CRoleAttribute>();
	if(pAttr != NULL)
	{
		return pAttr->GetRoleID();
	}
	return 0;
}

const char * CRoleEntity::GetRoleName() const
{
	CRoleAttribute *pAttr = GetComponent<CRoleAttribute>();
	if (pAttr != NULL)
	{
		return pAttr->GetRoleName();
	}

	return NULL;
}

int CRoleEntity::GetMoney() const
{
	CRoleAttribute * pAttr = GetComponent<CRoleAttribute>();
	if(pAttr != NULL)
	{
		return pAttr->GetMoney();
	}
	return 0;
}

unsigned int CRoleEntity::GetLuckStar() const
{
	CRoleDungeon * pAttr = GetComponent<CRoleDungeon>();
	if(pAttr != NULL)
	{
		return pAttr->GetLuckStar();
	}

	return 0;
}

//获取M币
unsigned int CRoleEntity::GetPoint() const
{
	CRoleAttribute * pAttr = GetComponent<CRoleAttribute>();
	if(pAttr != NULL)
	{
		return pAttr->GetBillInfo().nPoint;
	}
	return 0;
}


unsigned int CRoleEntity::GetBindPoint() const
{
	CRoleAttribute * pAttr = GetComponent<CRoleAttribute>();
	if(pAttr != NULL)
	{
		return pAttr->GetBillInfo().nBindPoint;
	}
	return 0;
}

unsigned int CRoleEntity::GetAvailablePoint() const
{
	return GetPoint() + GetBindPoint();
}

unsigned char CRoleEntity::GetSex() const
{
	CRoleAttribute * pAttr = GetComponent<CRoleAttribute>();
	if(pAttr != NULL)
	{
		return pAttr->GetSex();
	}
	return ESexType_No;
}

unsigned char CRoleEntity::GetColor() const
{
	CRoleAttribute * pAttr = GetComponent<CRoleAttribute>();
	if(pAttr != NULL)
    {
		return pAttr->GetColor();
	}
	return BODY_COLOR_YELLOW;
}

CEntityItemComponent * CRoleEntity::GetItemComponent()
{
 	return GetComponent<CRoleItem>();
}

unsigned short CRoleEntity::GetLevel() const
{
	CRoleAttribute * pAttr = GetComponent<CRoleAttribute>();
	if(pAttr != NULL){
		return pAttr->GetLevel();
	}

	return 0;
}

unsigned int CRoleEntity::GetHot() const
{
	CRoleAttribute * pAttr = GetComponent<CRoleAttribute>();
	if(pAttr != NULL){
		return pAttr->GetHot();
	}

	return 0;
}

unsigned int CRoleEntity::GetFriendCount() const
{
	CRoleFriend * pFriend = GetComponent<CRoleFriend>();
	if (pFriend != NULL){
		return pFriend->FriendCount();
	}

	return 0;
}

bool CRoleEntity::IsVIP() const
{
	CRoleVIP *pRoleVIP = GetComponent<CRoleVIP>();
	if (pRoleVIP != NULL) {
		return pRoleVIP->IsVIP();
	}

	return false;
}

int CRoleEntity::GetVIPLevel()const

{
	CRoleVIP *pRoleVIP = GetComponent<CRoleVIP>();
	if (pRoleVIP != NULL) {
		return pRoleVIP->VIPLevel();
	}

	return 0;	// 默认为0级
}

unsigned short CRoleEntity::PID() const
{
	CRoleAttribute * pAttr = GetComponent<CRoleAttribute>();
	if(pAttr != NULL){
		return pAttr->PID();
	}

	return 0;
}

unsigned int CRoleEntity::GetAccount() const
{
	CRoleAttribute * pAttr = GetComponent<CRoleAttribute>();
	if(pAttr != NULL){
		return pAttr->GetAccount();
	}
	return 0;
}

const PLAYERTAG* CRoleEntity::GetPlayerTag()const
{
	CRoleNet * pNet = GetComponent<CRoleNet>();
	if(pNet != NULL){
		return pNet->GetPlayerTag();
	}
	return NULL;
}

__int64 CRoleEntity::GetSessionID() const
{
	CRoleNet *pRoleNet = GetComponent<CRoleNet>();
	if (IsValid() && pRoleNet != NULL)
	{
		return pRoleNet->GetSessionID();
	}
	return 0;
}

unsigned short CRoleEntity::GetIndex() const
{
	CRoleNet *pNet = GetComponent<CRoleNet>();

	if (pNet != NULL)
	{
		return pNet->GetPlayerLogicIndex();
	}
	return 0;
}

unsigned int CRoleEntity::GetEntityID() const
{
	return GetRoleID();
}

void CRoleEntity::SetLocation(int nLocation)
{
	CRoleAttribute *pRoleAttr = GetComponent<CRoleAttribute>();

	if (pRoleAttr != NULL) {
		pRoleAttr->SetLocation(nLocation);
	}
}

int CRoleEntity::GetLocation() const
{
	CRoleAttribute *pRoleAttr = GetComponent<CRoleAttribute>();

	if (pRoleAttr != NULL) {
		return pRoleAttr->GetLocation();
	}

	return Zone_None;
}

void CRoleEntity::ChangeMoney(int nValue, EChangeMoneyCause eChangeMoneyCause, int nAffixData)
{
	CRoleAttribute *pRoleAttr = GetComponent<CRoleAttribute>();

	if (pRoleAttr != NULL) {
		pRoleAttr->ChangeMoney(nValue, eChangeMoneyCause, nAffixData);
	}
}

bool CRoleEntity::IsInBlackList(unsigned int nRoleID)
{
	if (nRoleID > 0) {
		CRoleFriend *pRoleFriend = GetComponent<CRoleFriend>();
		if (pRoleFriend != NULL) {
			return pRoleFriend->IsInBlackList(nRoleID);
		}
	}

	return false;
}

bool CRoleEntity::IsSingle()
{
	CRoleCouple *pRoleCouple = GetComponent<CRoleCouple>();
	if (pRoleCouple != NULL) {
		return pRoleCouple->IsSingle();
	}

	return false;
}

bool CRoleEntity::IsMarried()
{
	CRoleCouple *pRoleCouple = GetComponent<CRoleCouple>();
	if (pRoleCouple != NULL) {
		return pRoleCouple->IsMarried();
	}

	return false;
}

void CRoleEntity::SetPairID(unsigned int nPairID)
{
	CRoleCouple *pRoleCouple = GetComponent<CRoleCouple>();
	if (pRoleCouple != NULL) {
		pRoleCouple->m_coupleInfo.m_nPairID = nPairID;
	}
}

unsigned int CRoleEntity::PairID()
{
	CRoleCouple *pRoleCouple = GetComponent<CRoleCouple>();
	if (pRoleCouple != NULL) {
		return pRoleCouple->PairID();
	}

	return 0;
}

void CRoleEntity::SetCoupleID(unsigned int nCoupleID)
{
	CRoleCouple *pRoleCouple = GetComponent<CRoleCouple>();
	if (pRoleCouple != NULL) {
		pRoleCouple->m_coupleInfo.m_nCoupleID = nCoupleID;
	}
}

unsigned int CRoleEntity::CoupleID()
{
	CRoleCouple *pRoleCouple = GetComponent<CRoleCouple>();
	if (pRoleCouple != NULL) {
		return pRoleCouple->CoupleID();
	}

	return 0;
}

CEntityCoupleComponent * CRoleEntity::GetRoleCouple()
{
	return GetComponent<CRoleCouple>();
}

void CRoleEntity::SetCoupleBlessingValue( unsigned int nBlessingValue )
{
	CRoleCouple *pRoleCouple = GetComponent<CRoleCouple>();
	if (pRoleCouple != NULL) {
		pRoleCouple->m_coupleInfo.m_nBlessingValue  = nBlessingValue;
	}
}

void CRoleEntity::SetCoupleName(const std::string & strName)
{
	CRoleCouple *pRoleCouple = GetComponent<CRoleCouple>();
	if (pRoleCouple != NULL) {
		pRoleCouple->m_coupleInfo.m_strCoupleName = strName;
	}
}

void CRoleEntity::SetCoupleSmallRoleID( unsigned int nRoleID )
{
    CRoleCouple *pRoleCouple = GetComponent<CRoleCouple>();
    if (pRoleCouple != NULL)
    {
        pRoleCouple->m_coupleInfo.m_nSmallRoleID = nRoleID;
    }
}

void CRoleEntity::SetCoupleDate(unsigned int nCoupleDate)
{
	CRoleCouple *pRoleCouple = GetComponent<CRoleCouple>();
	if (pRoleCouple != NULL) {
		pRoleCouple->m_coupleInfo.m_nCoupleDate = nCoupleDate;
	}
}

unsigned int CRoleEntity::CoupleDate()
{
	CRoleCouple *pRoleCouple = GetComponent<CRoleCouple>();
	if (pRoleCouple != NULL) {
		return pRoleCouple->CoupleDate();
	}

	return 0;
}

void CRoleEntity::SetMarriageDate(unsigned int nMarriageDate)
{
	CRoleCouple *pRoleCouple = GetComponent<CRoleCouple>();
	if (pRoleCouple != NULL) {
		pRoleCouple->m_coupleInfo.m_nMarriageDate = nMarriageDate;
	}
}

unsigned int CRoleEntity::MarriageDate()
{
	CRoleCouple *pRoleCouple = GetComponent<CRoleCouple>();
	if (pRoleCouple != NULL) {
		return pRoleCouple->MarriageDate();
	}

	return 0;
}

CWeddingRoom * CRoleEntity::WeddingRoom()
{
	CRoleCouple *pRoleCouple = GetComponent<CRoleCouple>();
	if (pRoleCouple != NULL) {
		return pRoleCouple->WeddingRoom();
	}

	return NULL;
}

CPhotoRoom * CRoleEntity::PhotoRoom()
{
	CRolePhotoRoom *pRolePhotoRoom = GetComponent<CRolePhotoRoom>();
	if (pRolePhotoRoom != NULL) {
		return pRolePhotoRoom->PhotoRoom();
	}

	return NULL;
}

CAmuseRoom * CRoleEntity::AmuseRoom()
{
	CRoleAmuseRoom *pRoleAmuseRoom = GetComponent<CRoleAmuseRoom>();
	if (pRoleAmuseRoom != NULL) {
		return pRoleAmuseRoom->AmuseRoom();
	}

	return NULL;
}

CeremonyRoom * CRoleEntity::GetCeremonyRoom()
{
    CRoleDanceGroup * pEnt = GetComponent<CRoleDanceGroup>();

    if (NULL != pEnt)
        return pEnt->GetCeremonyRoom();

    return NULL;
}

CRoom * CRoleEntity::GetRoom()
{
    CRoomComponent *pRoom = GetRoleRoom();
    if (NULL != pRoom)
        return pRoom->Room();

    return NULL;
}

bool CRoleEntity::InBigMamaRoom()
{
    CEntityDanceGroupComponent *pDanceGroup = GetComponent<CRoleDanceGroup>();
    if (NULL != pDanceGroup)
        return pDanceGroup->IsInBigMamaRoom();

    return false;
}

bool CRoleEntity::InChanllengeRoom()
{
    CEntityDanceGroupComponent *pDanceGroup = GetComponent<CRoleDanceGroup>();
    if (NULL != pDanceGroup)
        return pDanceGroup->IsInChallengeRoom();

    return false;
}

bool CRoleEntity::InGroupRoom()
{
    CRoomComponent *pRoom = GetRoleRoom();
    if (NULL != pRoom)
        return pRoom->IsInGroupRoom();

    return true;
}


ELwRoom CRoleEntity::RoomType()
{
    if (NULL != GetRoom() || InChanllengeRoom() || InBigMamaRoom() || InGroupRoom())
        return ELwRoom_DanceRoom; // 跳舞房间
    else if (NULL != WeddingRoom())
        return ELwRoom_CoupleRoom;
    else if (NULL != GetCeremonyRoom())
        return ELwRoom_CeremonyRoom;
    else if (NULL != AmuseRoom())
        return ELwRoom_AmuseRoom;
    else if (NULL != PhotoRoom())
        return ELwRoom_PhotoRoom;
    else
        return ELwRoom_Out;
}


bool CRoleEntity::InSomeRoom()
{
    return RoomType() != ELwRoom_Out;
}

void CRoleEntity::ResetCoupleData()
{
	CRoleCouple *pRoleCouple = GetComponent<CRoleCouple>();
	if (pRoleCouple != NULL) {
		pRoleCouple->ResetCoupleData();
	}
}

void CRoleEntity::SetLastInvitedPhotoRoomPlayerTime(int nLastInvitedPlayerTime)
{
	CRolePhotoRoom *pRolePhotoRoom = GetComponent<CRolePhotoRoom>();
	if (pRolePhotoRoom != NULL) {
		pRolePhotoRoom->SetLastInviteTime(nLastInvitedPlayerTime);
	}
}

int CRoleEntity::LastInvitedPhotoRoomPlayerTime()
{
	CRolePhotoRoom *pRolePhotoRoom = GetComponent<CRolePhotoRoom>();
	if (pRolePhotoRoom != NULL) {
		return pRolePhotoRoom->LastInvitedTime();
	}

	return 0;
}

bool CRoleEntity::TryBeInvitedToPhotoRoom(unsigned int nRoomID, int nCreatedTime)
{
	CRolePhotoRoom *pRolePhotoRoom = GetComponent<CRolePhotoRoom>();
	if (pRolePhotoRoom != NULL) {
		return pRolePhotoRoom->TryBeInvited(nRoomID, nCreatedTime);
	}

	return false;
}

void CRoleEntity::GetFriendList( std::vector<unsigned int> & friendlist )
{
	CRoleFriend * pFriendComponent = GetComponent<CRoleFriend>();
	if ( pFriendComponent != NULL )
	{
		pFriendComponent->GetFriendList( friendlist );
	}
}

CEntityItemComponent * CRoleEntity::GetRoleItem()
{
	return GetComponent<CRoleItem>();
}

CEntityGeneComponent * CRoleEntity::GetRoleGene()
{
	return GetComponent<CRoleGene>();
}


CEntityDungeonComponent * CRoleEntity::GetRoleDungeon()
{
	return GetComponent<CRoleDungeon>();
}

CRoomComponent * CRoleEntity::GetRoleRoom()
{
	return GetComponent<CRoleRoom>();
}

CEntityAttributeComponent * CRoleEntity::GetRoleAttr()
{
	return GetComponent<CRoleAttribute>();
}

unsigned int CRoleEntity::GetDanceGroupID() const
{
    CRoleDanceGroup *pRoleDanceGroup = GetComponent<CRoleDanceGroup>();

    if (pRoleDanceGroup != NULL)
        return pRoleDanceGroup->GetDanceGroupID();

    return 0;
}

void CRoleEntity::GetDanceGroupInfo( unsigned int nRoleID, std::string & strGroupName, char & nGroupTitle, unsigned short & nBadge, unsigned short & nEffect )
{
	CRoleDanceGroup * pRoleDanceGroup = GetComponent<CRoleDanceGroup>();
	if ( pRoleDanceGroup != NULL )
	{
		CDanceGroupInfo *pDanceGroup = pRoleDanceGroup->GetDanceGroupInfo();
		if (pDanceGroup != NULL)
		{
			strGroupName = pDanceGroup->m_DGBaseInfo.m_strGroupName;
			nBadge = pDanceGroup->m_DGBaseInfo.m_nBadge;
			nEffect = pDanceGroup->m_DGBaseInfo.m_nEffect;

			CDanceGroupMember * pDGMember = pDanceGroup->GetGroupMember( nRoleID );
			if (pDGMember != NULL)
			{
				nGroupTitle = (char)pDGMember->m_nTitle;
			}
		}
	}
}

void CRoleEntity::SendPlayerMsg(GameMsg_Base *pMsg) const
{
	CRoleNet *pNet = GetComponent<CRoleNet>();

	if (pNet != NULL && pMsg != NULL)
	{
		pNet->SendPlayerMsg(pMsg);
	}
}

void CRoleEntity::SendMsg2GroupServer(GameMsg_Base *pMsg)
{
	CRoleNet *pNet = GetComponent<CRoleNet>();
	if (pMsg != NULL && pNet != NULL)
	{
		pNet->Send2GroupServer(pMsg);
	}
}

bool CRoleEntity::GetTodayFirstLoginTag() const
{
	CRoleAttribute * pAttr = GetComponent<CRoleAttribute>();
	if(pAttr != NULL){
		return pAttr->GetTodayFirstLoginTag();
	}
	return 0;
}

unsigned int CRoleEntity::GetRoomID() const
{
	CRoleRoom * pRoom = GetComponent<CRoleRoom>();
	if(pRoom != NULL)
	{
		return pRoom->RoomID();
	}
	return 0;
}

void CRoleEntity::SysChat(const char * szFormat, ...)
{
    char buffer[MAX_CHAT_BUFF_SIZE];
    va_list args;

    va_start(args, szFormat);
    _vsnprintf(buffer, sizeof(buffer), szFormat, args);
    va_end(args);

    buffer[MAX_CHAT_BUFF_SIZE - 1] = '\0';
    CRoleChat *pRoleChat = GetComponent<CRoleChat>();
    if (pRoleChat != NULL)
    {
        IChat *pChannel = CChatManager::Instance().CreateChatChannelBy(eChatSystem, pRoleChat);
        static_cast<CSystemChat *>(pChannel)->SysChat(buffer);
        delete pChannel, pChannel = NULL;
    }
}

void CRoleEntity::EntityUpdate(const unsigned long & nTimeElapsed)
{
	CEntity::EntityUpdate(nTimeElapsed);
}



bool CRoleEntity::PacketAndSendCreateRoleMsg()
{
	GameMsg_S2C_CreateRoleSuc CreateRoleMsg ;
	CreateRoleMsg.AttachEntity( this );
	bool bReturn = true;

	//CRoleAttribute * pRoleAttr = GetComponent<CRoleAttribute>();
	//if(pRoleAttr == NULL)
	//{
	//	bReturn = false;
	//}
	//else
	//{
	//	CreateRoleMsg.AddEntityComponentToSerialize( pRoleAttr );
	//}
	//if( bReturn )
	//{

	//	CRoleFriend * pRoleFriend = GetComponent<CRoleFriend>();
	//	if(pRoleFriend == NULL || !bReturn)
	//	{
	//		bReturn = false;
	//	}
	//	else
	//	{
	//		CreateRoleMsg.AddEntityComponentToSerialize( pRoleFriend );
	//	}
	//}
	//if( bReturn )
	//{

	//	CRoleItem* pRoleItem = GetComponent<CRoleItem>();
	//	if(pRoleItem == NULL || !bReturn)
	//	{
	//		bReturn = false;
	//	}
	//	else
	//	{
	//		CreateRoleMsg.AddEntityComponentToSerialize( pRoleItem );
	//	}
	//}
	//if( bReturn )
	//{

	//	CRoleGene* pRoleGene = GetComponent<CRoleGene>();
	//	if(pRoleGene == NULL || !bReturn)
	//	{
	//		bReturn = false;
	//	}
	//	else
	//	{
	//		CreateRoleMsg.AddEntityComponentToSerialize( pRoleGene );
	//	}
	//}
	//if( bReturn )
	//{

	CRoleNet *pNet = GetComponent<CRoleNet>();
	if(pNet == NULL || !bReturn)
	{
		bReturn = false;
	}
	else
	{
		CreateRoleMsg.nIndex = pNet->GetPlayerLogicIndex();
		bReturn = pNet->SendPlayerMsg( &CreateRoleMsg );
	}
	//}

	return true;
}

bool CRoleEntity::IsInRoomHall()
{
	return NULL != CRoomManager::Instance().GetLobbyPlayer(GetRoleID());
}

void CRoleEntity::ReadToRun(int nTargetIndex)
{
	std::list<GeneInfo*> equipGene;

	CRoleItem* pRoleItem = GetComponent<CRoleItem>();
	if(pRoleItem != NULL)
	{
		pRoleItem->GetCurEquipGene( equipGene );
		pRoleItem->ResumItem();
	}

	CRoleGene* pRoleGene = GetComponent<CRoleGene>();
	if(pRoleGene != NULL)
	{
		pRoleGene->RefreshEquipGene(equipGene, nTargetIndex);
	}
}

void CRoleEntity::SetTransformIDAndNotify( unsigned int nId )
{
	CEntityAttributeComponent *pRoleAttr = GetComponent<CRoleAttribute>();
	if (pRoleAttr != NULL) {
		pRoleAttr->SetTransformIdAndNotify(nId);
	}
}

unsigned int CRoleEntity::TransformID()
{
	CEntityAttributeComponent *pRoleAttr = GetComponent<CRoleAttribute>();
	if (pRoleAttr != NULL) {
		return pRoleAttr->GetTransformId();
	}

	return 0;
}

void CRoleEntity::PackRoleRoomImageData(RoleRoomImageData &imageData)
{
    CRoleItem * pItemComponent = GetComponent<CRoleItem>();
    if (NULL != pItemComponent)
    {
        pItemComponent->PackEquipDbItem(imageData.m_vecRoleItem, imageData.m_mapClothEffect, imageData.m_mapClothEffectSevenColor);
    }

    CRoleGene * pGene = GetComponent<CRoleGene>();
    if (NULL != pGene)
    {
        pGene->PackGeneAll(imageData.m_mapGeneData);
    }

    imageData.m_bHasCarriedPet = false;

    CRoleMedal *pMedal = GetComponent<CRoleMedal>();
    if (NULL != pMedal)
    {
        pMedal->GetActivityMedals(imageData.m_listMedalIDs);
    }

    CRoleHorse *pHorse = GetComponent<CRoleHorse>();
    if (NULL != pHorse)
    {
        pHorse->GetHorseInfo(imageData.m_nCurHorseTypeId, imageData.m_nCurHorseOwnerId, imageData.m_nHorseSeatIndex);
    }

    return;
}



void CRoleEntity::CacheEncode(ROLEINFO_DB* roleCache)
{
    // 每个玩家的缓存数据不得大于 2M
    const unsigned int nBuffSize = 2*1024*1024; //
    static char buff[nBuffSize] = { 0 };
    memset(buff, 0, nBuffSize);

//     static char testBuff[nBuffSize] = { 0 };
//     memset(testBuff, 0, nBuffSize);
//     int testbuffPos = 0;

    GameMsg_PlayerCacheTransferStart s;
    s.m_nAccountID = GetAccount();
    SendMsg2AccountServer(&s);

    CParamPool p(buff, nBuffSize);
    for (unsigned short i = 0; i < ERoleCacheInfoEnumMax; ++i)
    { //
        p.ClearData();
        ROLEINFO_DB::CacheEncodeFunctor f = ROLEINFO_DB::GetCachEncode(ERoleInfoCache(i));
        ENSURE_WITH_LOG_CMD(NULL != f, continue, "encode role cache miss type[%u]", i);
        if (!(roleCache->*f)(p))
        {
            WriteLog(LOGLEVEL_ERROR, "Encode cache error, key = %d", (int)i);
            break; // 编码失败
        }

        unsigned int allSize = p.GetValidDataSize();
        const unsigned int sendStep = 10*1024; // 每一次发送10K数据
        for (unsigned int pos = 0; pos < allSize; pos += sendStep)
        {
            GameMsg_PlayerCacheTransfer t;
            t.m_nAccountID = GetAccount();
            t.m_nKey = i;
            t.m_nAllDataSize = allSize;
            t.m_nThisSize = ((allSize - pos) >= sendStep) ? sendStep : (allSize - pos);
            t.m_pData = const_cast<char*>(p.GetData() + pos);
            SendMsg2AccountServer(&t);
        }
    }

    GameMsg_PlayerCacheTransferEnd e;
    e.m_nAccountID = GetAccount();
    SendMsg2AccountServer(&e);

    return;
}

void CRoleEntity::CacheDecode(ROLEINFO_DB *roleCache, unsigned short key, char* data, int dataSize)
{
    if ( 0 == dataSize || data == NULL || roleCache == NULL )
        return; // 空消息，不解析

    ROLEINFO_DB::CacheDecodeFunctor f = ROLEINFO_DB::GetCacheDecode(ERoleInfoCache(key));
    ENSURE_WITH_LOG_CMD(NULL != f, return, "decode role cache miss type[%u]", key);

    CParamPool buff(data, dataSize);
    try
    {
        (roleCache->*f)(buff); //
        roleCache->SetCachedFlag( (size_t)key );
    }
    catch (overflow_error e)
    {
        WriteLog(LOGLEVEL_ERROR, "Decode cache key = %d Error,ParamPool overflow : %s , decodeSize = %u , nSize = %d .", 
            (int)key, e.what(), buff.GetCurPosition(), dataSize);
    }
}
