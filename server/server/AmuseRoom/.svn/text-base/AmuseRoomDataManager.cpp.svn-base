#include "AmuseRoomDataManager.h"
#include "AmuseEnum.h"
#include "../../socket/Log.h"
#include "../util/CommonFunc.h"
#include "../mall/Mall.h"
#include "../logic/LogicCirculator.h"
#include "../mall/GameMsg_S2G_TimeEgg.h"
#include "../libServerFrame/QueryManager.h"
#include "GameMsg_S2C_AmuseMsg.h"
#include "AmuseRoomComponent.h"
#include "../../datastructure/Localization.h"
#include "../../socket/Formatter.h"
#include "../mail/MailMgr.h"
#include "../item/ItemProcess.h"

extern CLogicCirculator* g_pCirculator;
///////////////////////////////////////////////////////
CAmuseDeviceData::CAmuseDeviceData()
:m_nDeviceID(0)
,m_nCapacity(0)
,m_nInviteType(eAmuseInvite_None)
{
}

CAmuseDeviceData::~CAmuseDeviceData()
{
}


///////////////////////////////////////////////////////
CAmuseCarrouselData::CAmuseCarrouselData()
:m_bIsLoop(false)
,m_nInterval(0)
{

}

CAmuseCarrouselData::~CAmuseCarrouselData()
{
}

std::vector<std::string>& CAmuseCarrouselData::GetStartSongs()
{
	return m_vecStartSong;
}


///////////////////////////////////////////////////////
CAmuseExpData::CAmuseExpData()
:m_bIsLoop(false)
,m_nInterval(0)
,m_nAddExp(0)
{
}

CAmuseExpData::~CAmuseExpData()
{
}


///////////////////////////////////////////////////////
CAmuseSwingData::CAmuseSwingData()
:m_nDeviceID(0)
,m_nPushTime(0)
,m_nSwingTime(0)
{
}

CAmuseSwingData::~CAmuseSwingData()
{
}


///////////////////////////////////////////////////////
CAmuseBoatData::CAmuseBoatData()
:m_bIsLoop(false)
,m_nInterval(0)
,m_nBoatDock(0)
{

}

CAmuseBoatData::~CAmuseBoatData()
{
}

///////////////////////////////////////////////////////
CAmuseRoomDataManager::CAmuseRoomDataManager()
    : GameMsg_Processor( false, true )
    , m_bSentTimeEggSetting( false )
    , m_bInited( false )
    , m_bConnectedGroup( false )
{
}

CAmuseRoomDataManager::~CAmuseRoomDataManager()
{
}

CAmuseRoomDataManager& CAmuseRoomDataManager::Instance()
{
	static CAmuseRoomDataManager stcAmuseDeviceMgr;
	return stcAmuseDeviceMgr;
}

void CAmuseRoomDataManager::AddDeviceData(int nSceneID, int nDeviceID, int nCapacity, int nInviteType)
{
	if ( nCapacity <= 1 )
	{
		nInviteType = eAmuseInvite_None;
	}

	if ( nInviteType > 3 )
	{
		nInviteType = eAmuseInvite_None;
	}

	CAmuseDeviceData deviceData;
	deviceData.m_nDeviceID = nDeviceID;
	deviceData.m_nCapacity = nCapacity;
	deviceData.m_nInviteType = nInviteType;

	AmuseDeviceDataMap& sceneDeviceData = GetDeviceData( nSceneID );
	sceneDeviceData.insert( std::make_pair( nDeviceID, deviceData ) );
}

AmuseDeviceDataMap& CAmuseRoomDataManager::GetDeviceData(int nSceneID)
{
	AmuseSceneDeviceMap::iterator it = m_mapDeviceData.find( nSceneID );
	if ( it == m_mapDeviceData.end() )
	{
		AmuseDeviceDataMap deviceDataMap;
		m_mapDeviceData.insert( std::make_pair( nSceneID, deviceDataMap ) );
	}

	return m_mapDeviceData[nSceneID];
}

void CAmuseRoomDataManager::AddCarrouselData(int nSceneID, bool bIsLoop, int nInterval, std::vector<std::string>& vecStartSong)
{
	AmuseCarrouselDataMap::iterator it = m_mapCarrouselData.find( nSceneID );
	if ( it != m_mapCarrouselData.end() )
	{
		WriteLog(LOGLEVEL_WARNING, "CAmuseRoomDataManager add same carrousel data, ignore!" );
	}
	else
	{
		CAmuseCarrouselData carrouselData;
		carrouselData.m_bIsLoop = bIsLoop;
		carrouselData.m_nInterval = nInterval;
		carrouselData.m_vecStartSong = vecStartSong;

		m_mapCarrouselData.insert( std::make_pair( nSceneID, carrouselData ) );
	}
}

bool CAmuseRoomDataManager::GetCarrouselData(int nSceneID, CAmuseCarrouselData& rCarrouselData)
{
	AmuseCarrouselDataMap::iterator it = m_mapCarrouselData.find( nSceneID );
	if ( it != m_mapCarrouselData.end() )
	{
		rCarrouselData.m_bIsLoop = it->second.m_bIsLoop;
		rCarrouselData.m_nInterval = it->second.m_nInterval;
		rCarrouselData.m_vecStartSong = it->second.m_vecStartSong;

		return true;
	}

	return false;
}

void CAmuseRoomDataManager::AddExpData(int nSceneID, bool bIsLoop, int nInterval, int nAddExp)
{
	AmuseExpDataMap::iterator it = m_mapExpData.find( nSceneID );
	if ( it != m_mapExpData.end() )
	{
		WriteLog(LOGLEVEL_WARNING, "CAmuseRoomDataManager add same exp data, ignore!" );
	}
	else
	{
		CAmuseExpData expData;
		expData.m_bIsLoop = bIsLoop;
		expData.m_nInterval = nInterval;
		expData.m_nAddExp = nAddExp;

		m_mapExpData.insert( std::make_pair( nSceneID, expData ) );
	}
}

bool CAmuseRoomDataManager::GetExpData(int nSceneID, CAmuseExpData& rExpData)
{
	AmuseExpDataMap::iterator it = m_mapExpData.find( nSceneID );
	if ( it != m_mapExpData.end() )
	{
		rExpData.m_bIsLoop = it->second.m_bIsLoop;
		rExpData.m_nInterval = it->second.m_nInterval;
		rExpData.m_nAddExp = it->second.m_nAddExp;

		return true;
	}

	return false;
}

void CAmuseRoomDataManager::AddSwingData(int nSceneID, int nDeviceID, int nPushTime, int nSwingTime)
{
	AmuseSwingDataMap::iterator it = m_mapSwingData.find( nSceneID );
	if ( it != m_mapSwingData.end() )
	{
		WriteLog(LOGLEVEL_WARNING, "CAmuseRoomDataManager add same swing data, ignore!" );
	}
	else
	{
		CAmuseSwingData swingData;
		swingData.m_nDeviceID = nDeviceID;
		swingData.m_nPushTime = nPushTime;
		swingData.m_nSwingTime = nSwingTime;

		m_mapSwingData.insert( std::make_pair( nSceneID, swingData ) );
	}
}

bool CAmuseRoomDataManager::GetSwingData(int nSceneID, CAmuseSwingData& rSwingData)
{
	AmuseSwingDataMap::iterator it = m_mapSwingData.find( nSceneID );
	if ( it != m_mapSwingData.end() )
	{
		rSwingData.m_nDeviceID = it->second.m_nDeviceID;
		rSwingData.m_nPushTime = it->second.m_nPushTime;
		rSwingData.m_nSwingTime = it->second.m_nSwingTime;

		return true;
	}

	return false;
}

void CAmuseRoomDataManager::AddBoatData(int nSceneID, bool bIsLoop, int nInterval, int nBoatDock)
{
	AmuseBoatDataMap::iterator it = m_mapBoatData.find( nSceneID );
	if ( it != m_mapBoatData.end() )
	{
		WriteLog(LOGLEVEL_WARNING, "CAmuseRoomDataManager add same boat data, ignore!" );
	}
	else
	{
		CAmuseBoatData boatData;
		boatData.m_bIsLoop = bIsLoop;
		boatData.m_nInterval = nInterval;
		boatData.m_nBoatDock = nBoatDock;

		m_mapBoatData.insert( std::make_pair( nSceneID, boatData ) );
	}
}

bool CAmuseRoomDataManager::GetBoatData(int nSceneID, CAmuseBoatData& rBoatData)
{
	AmuseBoatDataMap::iterator it = m_mapBoatData.find( nSceneID );
	if ( it != m_mapBoatData.end() )
	{
		rBoatData.m_bIsLoop = it->second.m_bIsLoop;
		rBoatData.m_nInterval = it->second.m_nInterval;
		rBoatData.m_nBoatDock = it->second.m_nBoatDock;

		return true;
	}

	return false;
}

void CAmuseRoomDataManager::AddEggData( int nSceneID, int nDeviceID )
{
	if (!m_mapEggData[nSceneID].insert(nDeviceID).second)
	{
		WriteLog(LOGLEVEL_WARNING, "CAmuseRoomDataManager add same egg data, ignore!" );
	}
}

bool CAmuseRoomDataManager::GetEggData( int nSceneID, int nDeviceID )
{
	AmuseEggDataMap::iterator it_scene = m_mapEggData.find(nSceneID);
	if (it_scene == m_mapEggData.end())
	{
		return false;
	}
	std::set<int>::iterator it_device = it_scene->second.find(nDeviceID);
	if (it_device == it_scene->second.end())
	{
		return false;
	}
	return true;
}

bool CAmuseRoomDataManager::GetEggData( int nSceneID, std::set<int>& rEggData )
{
	AmuseEggDataMap::iterator it_scene = m_mapEggData.find(nSceneID);
	if (it_scene == m_mapEggData.end())
	{
		return false;
	}
	rEggData = it_scene->second;
	return true;
}

bool CAmuseRoomDataManager::ParseSetting( unsigned int nTimeEggOpenDuration, unsigned int nTimeEggDestroyDuration, const string& strCostItem, 
										 const string& strGiftItem, const string& strTogetherMaleItem, const string& strTogetherFemaleItem, 
										 const string& strMaleItem, const string& strFemaleItem, int nFixedMoneyReward, int nExtraMoneyReward, 
										 int nGiftDiscount, const string& strHelpTitle, const string& strHelpContent)
{
    bool bRes = true;
    m_timeEggSetting.m_nTimeEggOpenDuration = nTimeEggOpenDuration;
    m_timeEggSetting.m_nTimeEggDestroyDuration = nTimeEggOpenDuration+nTimeEggDestroyDuration;
    m_timeEggSetting.m_listCostItem.clear();
    m_timeEggSetting.m_listGiftItem.clear();
    m_timeEggSetting.m_listTogetherMaleItem.clear();
    m_timeEggSetting.m_listTogetherFemaleItem.clear();
    m_timeEggSetting.m_listMaleItem.clear();
    m_timeEggSetting.m_listFemaleItem.clear();
    m_timeEggSetting.m_nFixedMoneyReward = nFixedMoneyReward;
    m_timeEggSetting.m_nExtraMoneyReward = nExtraMoneyReward;
    m_timeEggSetting.m_nGiftDiscount = nGiftDiscount;
    m_timeEggSetting.m_strHelpTitle = strHelpTitle;
    m_timeEggSetting.m_strHelpContent = strHelpContent;

    bRes = bRes && TransferItemList( strCostItem.c_str(), m_timeEggSetting.m_listCostItem );
    bRes = bRes && TransferItemList( strGiftItem.c_str(), m_timeEggSetting.m_listGiftItem );
    bRes = bRes && TransferItemList( strTogetherMaleItem.c_str(), m_timeEggSetting.m_listTogetherMaleItem );
    bRes = bRes && TransferItemList( strTogetherFemaleItem.c_str(), m_timeEggSetting.m_listTogetherFemaleItem );
    bRes = bRes && TransferItemList( strMaleItem.c_str(), m_timeEggSetting.m_listMaleItem );
    bRes = bRes && TransferItemList( strFemaleItem.c_str(), m_timeEggSetting.m_listFemaleItem );

    return bRes;
}

void CAmuseRoomDataManager::Init()
{
	//_CheckMallItem(m_timeEggSetting.m_listCostItem);
	_CheckMallItem(m_timeEggSetting.m_listGiftItem);
	//_CheckMallItem(m_timeEggSetting.m_listTogetherMaleItem);
	//_CheckMallItem(m_timeEggSetting.m_listTogetherFemaleItem);
	//_CheckMallItem(m_timeEggSetting.m_listMaleItem);
	//_CheckMallItem(m_timeEggSetting.m_listFemaleItem);
	m_bInited = true;

	if ( !m_bSentTimeEggSetting && g_pCirculator && m_bConnectedGroup )
	{
		GameMsg_S2G_TimeEggSetting msg;
		msg.m_setting = m_timeEggSetting;
		SendMsg2GroupServer( &msg );
		m_bSentTimeEggSetting = true;
	}

    RegNetMsgMap();
}

void CAmuseRoomDataManager::_CheckMallItem( std::list<CItem>& listItem )
{
	for (std::list<CItem>::iterator it = listItem.begin(); it != listItem.end(); )
	{
		if ( !CMall::Instance().CanBuyGoodByID(it->m_nItemType) )
		{
			it = listItem.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void CAmuseRoomDataManager::OnGroupServerConnected()
{
	m_bConnectedGroup = true;
	if ( !m_bSentTimeEggSetting && m_bInited )
	{
		GameMsg_S2G_TimeEggSetting msg;
		msg.m_setting = m_timeEggSetting;
		SendMsg2GroupServer( &msg );
		m_bSentTimeEggSetting = true;
	}
}

void CAmuseRoomDataManager::RegNetMsgMap()
{
    RegQueryProcessor( Query_OpenTimeEgg, &CAmuseRoomDataManager::OnDBOpenTimeEgg );
}

void CAmuseRoomDataManager::OnDBOpenTimeEgg( QueryBase &rQuery )
{
    OpenTimeEggEntry *pInfo = (OpenTimeEggEntry *)rQuery.m_pAttachment;
    if ( pInfo == NULL )
    {
        return;
    }
    else if ( pInfo->nError == 0 )
    {
        GameMsg_S2G_OpenTimeEgg s2gOpenEgg;
        s2gOpenEgg.m_nEggID = pInfo->m_nEggID;
        SendMsg2GroupServer( &s2gOpenEgg );
    }

    if ( pInfo->nError == 1 || pInfo->m_roleinfo.size() != MAX_EGG_PLAYER_COUNT )
        OpenTimeEggFailed( pInfo );
    else
        OpenTimeEggSuccess( pInfo );

    delete pInfo, rQuery.m_pAttachment = pInfo = NULL;
}

void CAmuseRoomDataManager::OpenTimeEggSuccess( OpenTimeEggEntry *pInfo ) const
{
    if ( pInfo == NULL || pInfo->m_roleinfo.size() != MAX_EGG_PLAYER_COUNT )
        return;

    for ( int i = 0; i < MAX_EGG_PLAYER_COUNT; ++i )
    {
        CRoleEntity *pEntity = NULL;
        CAmuseRoomComponent *pAmuseComponent = NULL;

        const RoleTimeEggEntry &self = pInfo->m_roleinfo[i];
        const RoleTimeEggEntry &partner = pInfo->m_roleinfo[(i + 1) % MAX_EGG_PLAYER_COUNT];

        std::list<CItem> listRewardItem;
        CAmuseActionParam_OpenTimeEgg *pParam = NULL;

        pEntity = CPlayerManager::Instance().GetEntityByRoleID( self.m_nRoleID );
        if ( pEntity != NULL )
        {
            pAmuseComponent = pEntity->GetComponent<CRoleAmuseRoom>();

            if ( pAmuseComponent != NULL && ( pInfo->m_nOpenType == OpenEggType_Together || pInfo->m_nOpenRoleID == self.m_nRoleID ) )
                pParam = new CAmuseActionParam_OpenTimeEgg;
        }

        if ( partner.m_item.m_nItemType > 0 && partner.m_item.m_nItemCount > 0 )
        {
            listRewardItem.push_back( partner.m_item );
            if ( pParam != NULL )
                pParam->m_listGiftReward.push_back( partner.m_item );
        }

        int nExtraMoneyReward = 0;
        int nTotalMoneyReward = m_timeEggSetting.m_nFixedMoneyReward;
        const std::list<CItem> *pFixedReward = &m_timeEggSetting.m_listMaleItem;
        const std::list<CItem> *pExtraReward = &m_timeEggSetting.m_listTogetherMaleItem;
        if ( self.m_nSex == ESexType_Female )
        {
            pFixedReward = &m_timeEggSetting.m_listFemaleItem;
            pExtraReward = &m_timeEggSetting.m_listTogetherFemaleItem;
        }

        if ( pFixedReward != NULL && !pFixedReward->empty() )
        {
            listRewardItem.insert( listRewardItem.end(), pFixedReward->begin(), pFixedReward->end() );
            if ( pParam != NULL )
                pParam->m_listFixedReward.insert( pParam->m_listFixedReward.end(), pFixedReward->begin(), pFixedReward->end() );
        }
        if ( pInfo->m_nOpenType == OpenEggType_Together )
        {
            if ( pExtraReward != NULL && !pExtraReward->empty() )
            {
                listRewardItem.insert( listRewardItem.end(), pExtraReward->begin(), pExtraReward->end() );
                if ( pParam != NULL )
                    pParam->m_listExtraReward.insert( pParam->m_listExtraReward.end(), pExtraReward->begin(), pExtraReward->end() );
            }
            nExtraMoneyReward = m_timeEggSetting.m_nExtraMoneyReward;
            nTotalMoneyReward += nExtraMoneyReward;
        }

        std::string title;
        SafeFormat( title, CLocalization::Instance().GetString( "Time_Egg_Mail_Title" ), partner.m_strRoleName.c_str() );

        if ( pParam == NULL )
        {
            if ( nTotalMoneyReward > 0 )
            {
                CMailMgr::Instance().SendSystemMailToPlayer( self.m_nRoleID, EMailType_TimeEgg, title, partner.m_words, nTotalMoneyReward );
            }
            CMailMgr::Instance().SendSystemMailToPlayer( self.m_nRoleID, EMailType_TimeEgg, title, partner.m_words, listRewardItem );
        }
        else
        {
            CMailMgr::Instance().SendSystemMailToPlayer( self.m_nRoleID, EMailType_TimeEgg, title, partner.m_words );
            CItemProcess::AddOrMailItems( *pEntity, listRewardItem, EItemAction_Add_TimeEgg, 0, true, EMailType_TimeEgg, title, partner.m_words.c_str() );

            if ( nTotalMoneyReward > 0 )
            {
                pEntity->ChangeMoney( nTotalMoneyReward, EChangeMoneyCause_Add_TimeEgg, 0 );
            }

            pParam->m_strPartnerWords = partner.m_words;
            pParam->m_nPartnerID = partner.m_nRoleID;
            pParam->m_strPartnerName = partner.m_strRoleName;
            pParam->m_nSelfID = self.m_nRoleID;
            pParam->m_chOpenType = (unsigned char)pInfo->m_nOpenType;
            pParam->m_nFixedMoneyReward = m_timeEggSetting.m_nFixedMoneyReward;
            pParam->m_nExtraMoneyReward = nExtraMoneyReward;

            GameMsg_S2C_DoAmuseActionSuc msgSuc;
            msgSuc.m_pActionParam = pParam;
            pEntity->SendPlayerMsg( &msgSuc );
        }

        if ( pAmuseComponent != NULL )
            pAmuseComponent->OnOpenTimeEggSuc( pInfo->m_nEggID );
    }
}

void CAmuseRoomDataManager::OpenTimeEggFailed( const OpenTimeEggEntry *pInfo ) const
{
    if ( pInfo == NULL )
        return;

    GameMsg_S2C_DoAmuseActionFail msgFail;
    msgFail.m_strError = pInfo->strError;

    int count = 0;
    for ( std::vector<RoleTimeEggEntry>::const_iterator itr = pInfo->m_roleinfo.begin();
        itr != pInfo->m_roleinfo.end() && count < MAX_EGG_PLAYER_COUNT; ++itr, ++count )
    {
        CRoleEntity *pRoleEntity = CPlayerManager::Instance().GetEntityByRoleID( itr->m_nRoleID );
        if ( pRoleEntity != NULL )
            pRoleEntity->SendPlayerMsg( &msgFail );
    }
}

