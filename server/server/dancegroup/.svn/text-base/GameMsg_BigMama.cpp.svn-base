
#include "DanceGroupMsgDef.h"
#include "DataStruct_BigMama.h"
#include "GameMsg_BigMama.h"
#include "../item/EntityItemComponent.h"
#include "../gene/EntityGeneComponent.h"
#include "../Pet/PetComponent.h"
#include "../Medal/EntityMedalComponent.h"
#include "../Pet/PetService.h"
#include "../roomLib/RoomDataStruct.h"

GameMsg_S2C_SyncBigMamaConfig::GameMsg_S2C_SyncBigMamaConfig()
    : GameMsg_Base(MSG_S2C_SyncBigMamaConfig)
    , m_nCurState(EBigMamaState_NotOpen)
    , m_nBeaterType(EBigMamaLastBeatType_GroupMember)
    , m_nRewardState(0)
{

}

GameMsg_S2C_SyncBigMamaConfig::~GameMsg_S2C_SyncBigMamaConfig()
{

}

bool GameMsg_S2C_SyncBigMamaConfig::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUChar((unsigned char)m_nCurState);
    IOBuff.AddUChar((unsigned char)m_nBeaterType);
    IOBuff.AddStringW(m_strRoleName);
    IOBuff.AddUChar(m_nRewardState);

    return true;
}

bool GameMsg_S2C_SyncBigMamaConfig::doDecode(CParamPool& IOBuff)
{
    m_nCurState = (EBigMamaState)IOBuff.GetUChar();
    m_nBeaterType = (EBigMamaLastBeatType)IOBuff.GetUChar();
    IOBuff.GetStringW(m_strRoleName);
    m_nRewardState = IOBuff.GetUChar();

    return true;
}



GameMsg_C2S_RequestBigMamaStageInfo::GameMsg_C2S_RequestBigMamaStageInfo()
    : GameMsg_Base(MSG_C2S_RequestBigMamaStageInfo)
{}
GameMsg_C2S_RequestBigMamaStageInfo::~GameMsg_C2S_RequestBigMamaStageInfo()
{}
bool GameMsg_C2S_RequestBigMamaStageInfo::doEncode(CParamPool& IOBuff)
{
    return true;
}
bool GameMsg_C2S_RequestBigMamaStageInfo::doDecode(CParamPool& IOBuff)
{
    return true;
}

GameMsg_S2C_SyncBigMamaStageInfo::GameMsg_S2C_SyncBigMamaStageInfo()
    : GameMsg_Base(MSG_S2C_SyncBigMamaStageInfo)
    , m_bOpened(true)
{
}
GameMsg_S2C_SyncBigMamaStageInfo::~GameMsg_S2C_SyncBigMamaStageInfo()
{}
bool GameMsg_S2C_SyncBigMamaStageInfo::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddBool(m_bOpened);
    if (m_bOpened)
        m_property.doEncode(IOBuff);
    return true;
}

bool GameMsg_S2C_SyncBigMamaStageInfo::doDecode(CParamPool& IOBuff)
{
    m_bOpened = IOBuff.GetBool();
    if (m_bOpened)
        m_property.doDecode(IOBuff);
    return true;
}


GameMsg_C2S_RequestFightBigMama::GameMsg_C2S_RequestFightBigMama()
    : GameMsg_Base(MSG_C2S_RequestFightBigMama)
    , m_nPhoneOS(0)
{

}
GameMsg_C2S_RequestFightBigMama::~GameMsg_C2S_RequestFightBigMama()
{}
bool GameMsg_C2S_RequestFightBigMama::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUChar(m_nPhoneOS);

    return true;
}
bool GameMsg_C2S_RequestFightBigMama::doDecode(CParamPool &IOBuff)
{
    m_nPhoneOS = IOBuff.GetUChar();

    return true;
}



BigMamaPlayerInfo::BigMamaPlayerInfo()
{
    m_nRoleID = 0;
    m_strRoleName = "";
    m_bIsHost = false;
    m_chRoleType = RoleRoomType_None;
    m_nIsBoss = false;
    m_chRolePos = 0;
    m_chRoleState = RoleRoomState_Wait;
    m_eColorTeam = EDanceColorTeam_None;
    m_nRoleSex = ESexType_No;
    m_nRoleSkin = 1;
    m_nGroupTitle = -1;
    m_strDanceGroupName = "";
    m_nDanceGroupBadge = 0;
    m_nDanceGroupEffect = 0;
    m_bIsVIP = false;
    m_nVIPLevel = 0;
    m_nMoveType = 0;
    m_nTransFormID = 0;
    m_bEncodeMedal = false;

    m_pItemCOM = NULL;
    m_pGeneCOM = NULL;
    m_pPetCOM = NULL;
    m_pMedalCOM = NULL;
}

BigMamaPlayerInfo::~BigMamaPlayerInfo()
{
}

void BigMamaPlayerInfo::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddStringW(m_strRoleName.c_str());
    IOBuff.AddBool(m_bIsHost);
    IOBuff.AddUChar(m_chRoleType);
    IOBuff.AddBool(m_nIsBoss);
    IOBuff.AddUChar(m_chRolePos);
    IOBuff.AddUChar(m_chRoleState);
    IOBuff.AddUChar((unsigned char)m_eColorTeam);
    IOBuff.AddUChar(m_nRoleSex);
    IOBuff.AddUChar(m_nRoleSkin);
    IOBuff.AddChar(m_nGroupTitle);
    IOBuff.AddStringW(m_strDanceGroupName.c_str());
    IOBuff.AddUShort(m_nDanceGroupBadge);
    IOBuff.AddUShort(m_nDanceGroupEffect);
    IOBuff.AddBool(m_bIsVIP);
    IOBuff.AddUShort(m_nVIPLevel);
    IOBuff.AddUChar(m_nMoveType);
    IOBuff.AddUShort(m_nTransFormID);

    if (m_pItemCOM != NULL)
    {
        m_pItemCOM->PackItemForOther(IOBuff);
    }
    else
    {
        int nOldPos1 = IOBuff.GetCurPosition();
        IOBuff.AddUShort(0);

        int nOldPos2 = IOBuff.GetCurPosition();
        IOBuff.AddUShort(0);

        std::map<EItemClothType, itemtype_t>::iterator it = m_Equips.begin();
        short nCount = 0;
        for (; it != m_Equips.end(); ++it)
        {
            if (it->second > 0)
            {
                IOBuff.AddUInt(it->second);
                IOBuff.AddUShort((unsigned short)EItemColumn_ClothOnPlayer);
                IOBuff.AddUShort((unsigned short)it->first);

                char szItemBuff[256] = { 0 };
                memset(szItemBuff, 0, sizeof(szItemBuff));
                CParamPool itemBuff(szItemBuff, sizeof(szItemBuff));
                itemBuff.AddUShort(0);
                itemBuff.AddUShort(1);
                itemBuff.AddInt(-1);
                itemBuff.AddUChar(1);
                itemBuff.AddUInt(0);

                unsigned int nItemSize = itemBuff.GetValidDataSize();
                ASSERT(nItemSize == itemBuff.GetCurPosition());

                IOBuff.AddUChar((unsigned char)nItemSize);
                IOBuff.AddMemory(itemBuff.GetData(), (unsigned short)nItemSize);

                nCount++;
            }
        }
        int nFinalPos = IOBuff.GetCurPosition();
        IOBuff.SetCurPosition(nOldPos2);
        IOBuff.ReplaceShort(nCount);
        IOBuff.SetCurPosition(nFinalPos);
        IOBuff.AddUShort(0);

        // ÆßÉ«²ÊºçÐÅÏ¢(AI Boss)
        IOBuff.AddUShort(0);

        if (IOBuff.GetCurPosition() - nOldPos1 - sizeof(unsigned short) != 0)
        {
            nFinalPos = IOBuff.GetCurPosition();
            IOBuff.SetCurPosition(nOldPos1);
            IOBuff.ReplaceShort((short)(nFinalPos - nOldPos1 - sizeof(unsigned short)));
            IOBuff.SetCurPosition(nFinalPos);
        }
    }

    if (m_pGeneCOM != NULL)
    {
        m_pGeneCOM->PackGeneForOther(IOBuff);
    }
    else
    {
        IOBuff.AddUShort(0);
    }

    if (m_pPetCOM != NULL)
    {
        Pet *pet = m_pPetCOM->GetCarriedPet();
        IOBuff.AddBool(pet != NULL ? true : false);
        if (pet != NULL)
        {
            PlayerPet petInfo;
            PetService::Instance().EncodePetBrief(*pet, petInfo);
            petInfo.doEncode(IOBuff);
        }
    }
    else
    {
        IOBuff.AddBool(false);
    }

    if (m_bEncodeMedal && m_pMedalCOM != NULL)
    {
        m_pMedalCOM->SyncAllDisplayMedalsForOther(IOBuff);
    }
    else if (m_bEncodeMedal)
    {
        IOBuff.AddUShort(0);
    }
}

BigMamaRoomInfo::BigMamaRoomInfo()
    : m_nRoomType(eRoomType_BigMama)
    , m_nScene(RAN_SCENE_ID)
    , m_nMusic(RAN_MUSIC_ID)
    , m_nMusicMode(MusicMode_None)
    , m_nMusicLevel(MusicLevel_None)
    , m_nSelfRoomType(RoleRoomType_Dancer)
    , m_nTeamMode(EDanceRoomTeamMode_None)
    , m_bIsHost(true)
{
}

BigMamaRoomInfo::~BigMamaRoomInfo()
{
}

void BigMamaRoomInfo::doEncode(CParamPool& IOBuff)
{
    IOBuff << m_nRoomType << m_nScene << m_nMusic << m_nMusicMode << m_nMusicLevel
        << m_nSelfRoomType << m_nTeamMode << m_bIsHost;

    IOBuff.AddUShort((unsigned short)m_listDancer.size());
    for (std::list<BigMamaPlayerInfo>::iterator itr = m_listDancer.begin();
        itr != m_listDancer.end(); ++itr)
    {
        itr->doEncode(IOBuff);
    }

    return;
}



GameMsg_S2C_RequestFightBigMamaRes::GameMsg_S2C_RequestFightBigMamaRes()
    : GameMsg_Base(MSG_S2C_RequestFightBigMamaRes)
    , m_nErrorType(EFightBigMamaResult_Sucess)
{

}
GameMsg_S2C_RequestFightBigMamaRes::~GameMsg_S2C_RequestFightBigMamaRes()
{

}
bool GameMsg_S2C_RequestFightBigMamaRes::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUChar((unsigned char)m_nErrorType);
    IOBuff.AddStringW(m_strCheckKey);
    IOBuff.AddPerFixSizeMemory(m_szStage.c_str(), (unsigned short)m_szStage.size());
    m_roomInfo.doEncode(IOBuff);

    return true;
}

GameMsg_C2S_RequestGetBigMamaReward::GameMsg_C2S_RequestGetBigMamaReward()
    : GameMsg_Base(MSG_C2S_RequestGetBigMamaReward)
{

}
GameMsg_C2S_RequestGetBigMamaReward::~GameMsg_C2S_RequestGetBigMamaReward()
{
}

bool GameMsg_C2S_RequestGetBigMamaReward::doEncode(CParamPool& IOBuff)
{
    return true;
}
bool GameMsg_C2S_RequestGetBigMamaReward::doDecode(CParamPool& IOBuff)
{
    return true;
}



GameMsg_S2C_RequestGetBigMamaRewardRes::GameMsg_S2C_RequestGetBigMamaRewardRes()
    : GameMsg_Base(MSG_S2C_RequestGetBigMamaRewardRes)
    , m_nErrorType(EBigMamaRewardResult_Sucess)
    , m_nDanceGroupContribution(0)
    , m_bIsGuideReward(false)
{

}
GameMsg_S2C_RequestGetBigMamaRewardRes::~GameMsg_S2C_RequestGetBigMamaRewardRes()
{

}
bool GameMsg_S2C_RequestGetBigMamaRewardRes::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUChar((unsigned char)m_nErrorType);
    m_RewardInfo.doEncode(IOBuff);
    IOBuff.AddUInt(m_nDanceGroupContribution);
    IOBuff.AddBool(m_bIsGuideReward);

    return true;
}



GameMsg_C2S_CompleteBigMamaGuide::GameMsg_C2S_CompleteBigMamaGuide()
    : GameMsg_Base(MSG_C2S_CompleteBigMamaGuide)
{

}
GameMsg_C2S_CompleteBigMamaGuide::~GameMsg_C2S_CompleteBigMamaGuide()
{

}
bool GameMsg_C2S_CompleteBigMamaGuide::doEncode(CParamPool& IOBuff)
{
    return true;
}
bool GameMsg_C2S_CompleteBigMamaGuide::doDecode(CParamPool& IOBuff)
{
    return true;
}


GameMsg_C2S_RequestBigMamaGuideReward::GameMsg_C2S_RequestBigMamaGuideReward()
    : GameMsg_Base(MSG_C2S_RequestBigMamaGuideReward)
    , m_nRewardType(0)
{

}
GameMsg_C2S_RequestBigMamaGuideReward::~GameMsg_C2S_RequestBigMamaGuideReward()
{

}
bool GameMsg_C2S_RequestBigMamaGuideReward::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUChar(m_nRewardType);
    return true;
}

bool GameMsg_C2S_RequestBigMamaGuideReward::doDecode(CParamPool& IOBuff)
{
    m_nRewardType = IOBuff.GetUChar();

    return true;
}



GameMsg_S2C_NoticeBigMamaState::GameMsg_S2C_NoticeBigMamaState()
    : GameMsg_Base(MSG_S2C_NoticeBigMamaState)
    , m_nState(0)
{

}
GameMsg_S2C_NoticeBigMamaState::~GameMsg_S2C_NoticeBigMamaState()
{}
bool GameMsg_S2C_NoticeBigMamaState::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUChar(m_nState);
    return true;
}
bool GameMsg_S2C_NoticeBigMamaState::doDecode(CParamPool& IOBuff)
{
    m_nState = IOBuff.GetUChar();
    return true;
}

GameMsg_S2C_BigMamaIntroduce::GameMsg_S2C_BigMamaIntroduce()
    : GameMsg_Base(MSG_S2C_BigMamaIntroduce)
{

}

GameMsg_S2C_BigMamaIntroduce::~GameMsg_S2C_BigMamaIntroduce()
{

}

bool GameMsg_S2C_BigMamaIntroduce::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddStringW(m_strIntroduce);

    return true;
}

bool GameMsg_S2C_BigMamaIntroduce::doDecode(CParamPool& IOBuff)
{
    IOBuff.GetStringW(m_strIntroduce);
    return true;
}





