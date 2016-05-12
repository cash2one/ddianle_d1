#include "PhotoRoomDataStruct.h"
#include "../item/EntityItemComponent.h"
#include "../gene/EntityGeneComponent.h"
#include "../Pet/PetComponent.h"
#include "../Pet/PetService.h"
#include "../Pet/GameMsg_Pet.h"
#include "../Medal/EntityMedalComponent.h"


CPhotoRoomPlayerInfo::CPhotoRoomPlayerInfo()
    : m_nRoleID(0)
    , m_strRoleName("")
    , m_eRoleSex(0)
    , m_nRoleSkin(0)
    , m_strDanceGroupName("")
    , m_nGroupTitle(0)
    , m_nDanceGroupBadge( 0 )
    , m_nDanceGroupEffect( 0 )
    , m_bIsHost(false)
    , m_ePlayerState(EPhotoRoomPlayerState_Free)
    , m_bIsVIP(false)
    , m_nVIPLevel(0)
    , m_nTransFormID(0)
    , m_pRoleItem(NULL)
    , m_pRoleGene(NULL)
    , m_pRolePet(NULL)
    , m_pRoleMedal( NULL )
{
}

CPhotoRoomPlayerInfo::~CPhotoRoomPlayerInfo()
{
}

void CPhotoRoomPlayerInfo::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddStringW(m_strRoleName.c_str());
    IOBuff.AddUChar(m_eRoleSex);
    IOBuff.AddUChar(m_nRoleSkin);
    IOBuff.AddStringW(m_strDanceGroupName.c_str());
    IOBuff.AddUChar(m_nGroupTitle);
    IOBuff.AddUShort(m_nDanceGroupBadge);
    IOBuff.AddUShort(m_nDanceGroupEffect);
    IOBuff.AddBool(m_bIsHost);
    IOBuff.AddUChar((unsigned char)m_ePlayerState);
    IOBuff.AddBool(m_bIsVIP);
    IOBuff.AddUShort(m_nVIPLevel);
    IOBuff.AddUShort(m_nTransFormID);

    if (m_pRoleItem != NULL)
    {
        m_pRoleItem->PackItemForOther(IOBuff);
    }

    if (m_pRoleGene != NULL)
    {
        m_pRoleGene->PackGeneForOther(IOBuff);
    }

    if (m_pRolePet != NULL)
    {
        Pet *pet = m_pRolePet->GetCarriedPet();
        IOBuff.AddBool(pet != NULL ? true : false);
        if (pet != NULL)
        {
            PlayerPet petInfo;
            PetService::Instance().EncodePetBrief(*pet, petInfo);
            petInfo.doEncode(IOBuff);
        }
    }

    if ( m_pRoleMedal != NULL )
    {
        m_pRoleMedal->SyncAllDisplayMedalsForOther( IOBuff );
    }
}
//------------------------------------------------------CPhotoRoomWholeInfo
CPhotoRoomWholeInfo::CPhotoRoomWholeInfo()
:m_nRoomID(0)
,m_strRoomName("")
,m_nRoomType(0)
,m_nPhotoRoomTimeRemaind(0)
,m_nSelectedBGID(0)
{

}

CPhotoRoomWholeInfo::~CPhotoRoomWholeInfo()
{

}

void CPhotoRoomWholeInfo::doEncode( CParamPool & IOBufff )
{
	IOBufff.AddUInt(m_nRoomID);
	IOBufff.AddStringW(m_strRoomName.c_str());
	IOBufff.AddUChar((unsigned char)m_nRoomType);
	IOBufff.AddInt(m_nPhotoRoomTimeRemaind);
	IOBufff.AddUShort(m_nSelectedBGID);

	IOBufff.AddUShort((unsigned short)m_listOwnedBG.size());
	for (std::list<unsigned short>::iterator itr = m_listOwnedBG.begin(); itr != m_listOwnedBG.end(); ++itr) {
		IOBufff.AddUShort(*itr);
	}

	IOBufff.AddUShort((unsigned short)m_listOwnedGroupAction.size());
	for (std::list<unsigned short>::iterator itr = m_listOwnedGroupAction.begin(); itr != m_listOwnedGroupAction.end(); ++itr) {
		IOBufff.AddUShort(*itr);
	}

	IOBufff.AddUShort((unsigned short)m_listPlayers.size());
	for (std::list<CPhotoRoomPlayerInfo>::iterator itr = m_listPlayers.begin(); itr != m_listPlayers.end(); ++itr) {
		itr->doEncode(IOBufff);
	}

	IOBufff.AddUShort((unsigned short)m_listNPCs.size());
	for (std::list<CPhotoRoomNPCInfo>::iterator itr = m_listNPCs.begin(); itr != m_listNPCs.end(); ++itr) {
		itr->doEncode(IOBufff);
	}
}

//--------------------------------------CPhotoRoomNotifiationTimer
CPhotoRoomNotifiationTimer::CPhotoRoomNotifiationTimer()
:m_nLevel(0)
,m_nTimeBeforeExpired(0)
,m_nNotifcationType(0)
{

}

CPhotoRoomNotifiationTimer::~CPhotoRoomNotifiationTimer()
{

}

//-----------------------CInvitPhotoRoomInfo
CInvitPhotoRoomInfo::CInvitPhotoRoomInfo(unsigned int nRoomID, int nRoomCreatedTime, int nTimeRemain)
:m_nRoomID(nRoomID)
,m_nRoomCreatedTime(nRoomCreatedTime)
,m_nTimeRemain(nTimeRemain)
{

}

CInvitPhotoRoomInfo::~CInvitPhotoRoomInfo()
{

}



CPhotoRoomNPCInfo::CPhotoRoomNPCInfo()
{
	m_nSexType = ESexType_Male;
	m_nSkin = 1;
}

CPhotoRoomNPCInfo::~CPhotoRoomNPCInfo()
{

}

void CPhotoRoomNPCInfo::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUChar(m_nSexType);
	IOBuff.AddUChar(m_nSkin);
	IOBuff.AddStringW(m_strName.c_str());
	IOBuff.AddStringW(m_strHeadwear.c_str());
	IOBuff.AddStringW(m_strHair.c_str());
	IOBuff.AddStringW(m_strFace.c_str());
	IOBuff.AddStringW(m_strShoulders.c_str());
	IOBuff.AddStringW(m_strGloves.c_str());
	IOBuff.AddStringW(m_strRighthand.c_str());
	IOBuff.AddStringW(m_strLefthand.c_str());
	IOBuff.AddStringW(m_strCloth.c_str());
	IOBuff.AddStringW(m_strShoes.c_str());
	IOBuff.AddStringW(m_strSuit.c_str());
	IOBuff.AddStringW(m_strLeg.c_str());
}

