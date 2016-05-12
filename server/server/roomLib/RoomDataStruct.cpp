#include "RoomDataStruct.h"
//#include "RoomComponent.h"
//#include "../attribute/EntityAttributeComponent.h"
// 
#include "../Pet/PetConfig/PetConfigManager.h"
#include "../Pet/PetConfig/PetSkillConfig.h"
#include "../../socket/Log.h"

CDanceTeam::CDanceTeam()
    : m_eColor(EDanceColorTeam_None)
    , m_nTotalMark(0)
    , m_nExp(0)
{
}

CDanceTeam::~CDanceTeam()
{
}

bool CDanceTeam::operator < (const CDanceTeam & rRhs)
{
    return m_nTotalMark < rRhs.m_nTotalMark;
}

void CDanceTeam::Reset()
{
    m_nTotalMark = 0;
    m_nRank = 0;
    m_nExp = 0;
}

void CDanceTeam::SetTeamColor(EDanceColorTeam eColor)
{
    m_eColor = eColor;
}

EDanceColorTeam CDanceTeam::TeamColor() const
{
    return m_eColor;
}

void CDanceTeam::SetRank(int nRank)
{
    m_nRank = nRank;
}

int CDanceTeam::Rank() const
{
    return m_nRank;
}

void CDanceTeam::AddTeamMark( int nMark )
{
    if (nMark > 0)
    {
        __int64 nTotalMark = m_nTotalMark + nMark;
        m_nTotalMark = nTotalMark > MAX_DANCE_TEAM_MARK ? MAX_DANCE_TEAM_MARK : (int)nTotalMark;

        WriteLog(LOGLEVEL_DEBUG, "---team[%d] add mark[%d], total mark[%d]\n", m_eColor, nMark, m_nTotalMark);
    }
}

int CDanceTeam::TotalTeamMark() const
{
    return m_nTotalMark;
}

void CDanceTeam::AddTeamExp( int nExp )
{
    if (nExp > 0)
    {
        m_nExp += nExp;
    }
}

int CDanceTeam::TeamExp() const
{
    return m_nExp;
}

DanceTeamRankCompare::DanceTeamRankCompare()
{

}

DanceTeamRankCompare::~DanceTeamRankCompare()
{

}

bool DanceTeamRankCompare::operator()(const CDanceTeam & lhs, const CDanceTeam & rhs) const
{
    return lhs.m_nTotalMark > rhs.m_nTotalMark;
}

CRoomDanceScoreInfo::CRoomDanceScoreInfo()
{
    Clear();
}

CRoomDanceScoreInfo::~CRoomDanceScoreInfo()
{
}

void CRoomDanceScoreInfo::Clear()
{
    m_szDancerPlace = 0;
    m_szDancerGrade = 0;
    m_szDancerMark = 0;
    m_szDancerExp = 0;
    m_szDancerPerfect = 0;
    m_szDancerCool = 0;
    m_szDancerGood = 0;
    m_szDancerBad = 0;
    m_szDancerMiss = 0;
    m_szDancerSpecial = 0;
}

void CRoomDanceScoreInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar( m_szDancerPlace );
    IOBuff.AddUChar( m_szDancerGrade );
    IOBuff.AddUInt( m_szDancerMark );
    IOBuff.AddUInt( m_szDancerExp );
    IOBuff.AddInt( m_szDancerPerfect );
    IOBuff.AddInt( m_szDancerCool );
    IOBuff.AddInt( m_szDancerGood );
    IOBuff.AddInt( m_szDancerBad );
    IOBuff.AddInt( m_szDancerMiss );
    IOBuff.AddInt( m_szDancerSpecial );
}


CLiveRoomDancerScore::CLiveRoomDancerScore()
{
    Clear();
}

CLiveRoomDancerScore::~CLiveRoomDancerScore()
{

}

void CLiveRoomDancerScore::Clear()
{
    m_strRoleName.clear();
    m_score.Clear();
    m_nHotValue = 0;
}

void CLiveRoomDancerScore::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddStringW(m_strRoleName.c_str());
    m_score.doEncode(IOBuff);
    //IOBuff.AddInt(m_nHotValue);
}


CLiveRoomRoundDancerScore::CLiveRoomRoundDancerScore()
{
    m_nRoundEndTime = 0;
    m_nDancerCount = 0;
    for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; i++)
    {
        m_score[i].Clear();
    }
}

CLiveRoomRoundDancerScore::~CLiveRoomRoundDancerScore()
{
}

void CLiveRoomRoundDancerScore::doEncode(CParamPool &IOBuff)
{
    if (m_nRoundEndTime > 0)
    {
        IOBuff.AddUShort((unsigned short)m_nDancerCount);
    }
    else
    {
        IOBuff.AddUShort(0);
    }

    for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; i++)
    {
        if (!m_score[i].m_strRoleName.empty())
        {
            m_score[i].doEncode(IOBuff);
        }
    }
}

AuditionPlayerInfo::AuditionPlayerInfo()
    : m_nRoleID(0)
    , m_strRoleName("")
    , m_nAuditionScore(0)
{
}

//////////////////////////////////////////////////////////////////////////
PlayerRoomStatus::PlayerRoomStatus()
    : m_bCostEndurance(false)
    , m_nBonusExp(0)
{
}

PlayerRoomStatus::~PlayerRoomStatus()
{
}

void PlayerRoomStatus::Clear()
{
    m_bCostEndurance = false;
    m_mapSkill.clear();
    m_nBonusExp = 0;
}

void PlayerRoomStatus::CostEndurance(bool bCost)
{
    m_bCostEndurance = bCost;
}

std::map<SkillID, int> &PlayerRoomStatus::SkillMap()
{
    return m_mapSkill;
}

bool PlayerRoomStatus::HasCastPetSkill() const
{
    return !m_mapSkill.empty();
}

int PlayerRoomStatus::GetSkillDanceMarkRatio(int nMusicMode) const
{
    if (!HasCastPetSkill())
    {
        return 0;
    }

    int nRatio = 0;
    for (std::map<SkillID, int>::const_iterator it = m_mapSkill.begin();
        it != m_mapSkill.end(); ++it)
    {
        SkillID nSkillID = it->first;
        int nSkillValue = it->second;

        const PetSkillConfig *skillConfig = PetConfigManager::Instance().GetPetSkill(nSkillID);
        if (skillConfig != NULL && skillConfig->GetType() == ePetSkillType_DanceTotalScore
            && (nMusicMode == skillConfig->GetSkillParam() || skillConfig->GetSkillParam() == 0))
        {
            nRatio += nSkillValue;
        }
    }

    return nRatio;
}

int PlayerRoomStatus::GetSkillRankMark(int nMusicMode, int nMusicLevel, int nRank)
{
    if (!HasCastPetSkill())
    {
        return 0;
    }

    int nSkillMark = 0;
    for (std::map<SkillID, int>::const_iterator it = m_mapSkill.begin();
        it != m_mapSkill.end(); ++it)
    {
        SkillID nSkillID = it->first;
        int nSkillValue = it->second;

        const PetSkillConfig *skillConfig = PetConfigManager::Instance().GetPetSkill(nSkillID);
        if (skillConfig != NULL)
        {
            switch (skillConfig->GetType())
            {
            case ePetSkillType_DanceModePerfectScore:
                if (nMusicMode == skillConfig->GetSkillParam()
                    || skillConfig->GetSkillParam() == 0)
                {
                    nSkillMark += nSkillValue;
                }
                break;
            case ePetSkillType_DanceLevelPerfectScore:
                if (nMusicLevel == skillConfig->GetSkillParam()
                    || skillConfig->GetSkillParam() == 0)
                {
                    nSkillMark += nSkillValue;
                }
                break;
            default:
                break;
            }
        }
    }

    return nSkillMark;
}

int PlayerRoomStatus::GetSkillRewardExpRatio(int nMusicMode, int nMusicLevel)
{
    if (!HasCastPetSkill())
    {
        return 0;
    }

    int nExtraExpRatio = 0;
    for (std::map<SkillID, int>::const_iterator it = m_mapSkill.begin();
        it != m_mapSkill.end(); ++it)
    {
        SkillID nSkillID = it->first;
        int nSkillValue = it->second;

        const PetSkillConfig *skillConfig = PetConfigManager::Instance().GetPetSkill(nSkillID);
        if (skillConfig != NULL)
        {
            switch (skillConfig->GetType())
            {
            case ePetSkillType_DanceModeExp:
                if (nMusicMode == skillConfig->GetSkillParam() || skillConfig->GetSkillParam() == 0)
                {
                    nExtraExpRatio += nSkillValue;
                }
                break;
            case ePetSkillType_DanceLevelExp:
                if (nMusicLevel == skillConfig->GetSkillParam() || skillConfig->GetSkillParam() == 0)
                {
                    nExtraExpRatio += nSkillValue;
                }
                break;
            default:
                break;
            }
        }
    }

    return nExtraExpRatio;
}

int PlayerRoomStatus::GetSkillRewardMoney() const
{
    if (!HasCastPetSkill())
    {
        return 0;
    }

    int nDropRate = 0;
    int nDropAmount = 0;
    for (std::map<SkillID, int>::const_iterator it = m_mapSkill.begin();
        it != m_mapSkill.end(); ++it)
    {
        SkillID nSkillID = it->first;
        int nSkillValue = it->second;

        const PetSkillConfig *skillConfig = PetConfigManager::Instance().GetPetSkill(nSkillID);
        if (skillConfig != NULL && skillConfig->GetType() == ePetSkillType_DanceDropMoney)
        {
            nDropRate = skillConfig->GetSkillParam();
            nDropAmount += nSkillValue;
        }
    }

    if (nDropRate > 0 && nDropAmount > 0)
    {
        int nRandom = (int)(Random() % 100);
        if (nDropRate > nRandom)
        {
            return nDropAmount;
        }
    }

    return 0;
}

int PlayerRoomStatus::GetSkillRewardItemRatio() const
{
    if (!HasCastPetSkill())
    {
        return 0;
    }

    int nDropRatio = 0;
    for (std::map<SkillID, int>::const_iterator it = m_mapSkill.begin();
        it != m_mapSkill.end(); ++it)
    {
        SkillID nSkillID = it->first;
        int nSkillValue = it->second;

        const PetSkillConfig *skillConfig = PetConfigManager::Instance().GetPetSkill(nSkillID);
        if (skillConfig != NULL && skillConfig->GetType() == ePetSkillType_DanceDropRate)
        {
            nDropRatio += nSkillValue;
        }
    }

    return nDropRatio;
}

void PlayerRoomStatus::SetBonusExp(int nBonusExp)
{
    if ( m_bCostEndurance )
        m_nBonusExp = nBonusExp;
    else
        m_nBonusExp = 0;
}

int PlayerRoomStatus::GetBonusExp() const
{
    return m_nBonusExp;
}




//////////////////////////////////////////////////////////////////////////
RoomPlayerInfo::RoomPlayerInfo()
{
    m_nRoleID = 0;
    m_strRoleName = "";
    m_bIsHost = false;
    m_chRoleType = RoleRoomType_None;
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

    //     m_pItemCOM = NULL;
    //     m_pGeneCOM = NULL;
    //     m_pPetCOM = NULL;
    //     m_pMedalCOM = NULL;
    //     m_pHorseCOM = NULL;
}

RoomPlayerInfo::~RoomPlayerInfo()
{
}

void RoomPlayerInfo::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddStringW(m_strRoleName.c_str());
    IOBuff.AddBool(m_bIsHost);
    IOBuff.AddUChar(m_chRoleType);
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

    IOBuff.AddPerFixSizeMemory(m_strData.data(), m_strData.size());

    //    m_roleRoomImageData.RoomEncode(IOBuff); 
    //     if (m_pItemCOM != NULL)
    //     {
    //         m_pItemCOM->PackItemForOther(IOBuff);
    //     }
    // 
    //     if (m_pGeneCOM != NULL)
    //     {
    //         m_pGeneCOM->PackGeneForOther(IOBuff);
    //     }
    // 
    //     if (m_pPetCOM != NULL)
    //     {
    //         Pet *pet = m_pPetCOM->GetCarriedPet();
    //         IOBuff.AddBool(pet != NULL ? true : false);
    //         if (pet != NULL)
    //         {
    //             PlayerPet petInfo;
    //             PetService::Instance().EncodePetBrief(*pet, petInfo);
    //             petInfo.doEncode(IOBuff);
    //         }
    //     }
    // 
    //     if ( m_pMedalCOM != NULL )
    //     {
    //         m_pMedalCOM->SyncAllDisplayMedalsForOther( IOBuff );
    //     }
    // 
    //     if(m_pHorseCOM != NULL)
    //     {
    //         m_pHorseCOM->SyncAllDisplayHorse2Other(IOBuff);
    //     }

}

void RoomPlayerInfo::doDecode(CParamPool& IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strRoleName);
    m_bIsHost = IOBuff.GetBool();
    m_chRoleType = IOBuff.GetUChar();
    m_chRolePos = IOBuff.GetUChar();
    m_chRoleState = IOBuff.GetUChar();
    m_eColorTeam = (EDanceColorTeam)IOBuff.GetUChar();
    m_nRoleSex = IOBuff.GetUChar();
    m_nRoleSkin = IOBuff.GetUChar();
    m_nGroupTitle = IOBuff.GetUChar();
    IOBuff.GetStringW(m_strDanceGroupName);
    m_nDanceGroupBadge = IOBuff.GetUShort();
    m_nDanceGroupEffect = IOBuff.GetUShort();
    m_bIsVIP = IOBuff.GetBool();
    m_nVIPLevel = IOBuff.GetUShort();
    m_nMoveType = IOBuff.GetUChar();
    m_nTransFormID = IOBuff.GetUShort();

    IOBuff.GetPerFixSizeMemory(m_strData);
    //    m_roleRoomImageData.RoomDecode(IOBuff);

    return;
}

RoomBriefInfo::RoomBriefInfo()
{
    m_nRoomID = 0;
    m_strRoomName = "";
    m_bHasPwd = false;
    m_bHasStart = false;
    m_chMode = MusicMode_None;
    m_chRoomColor = RoomColor_None;
    m_chRoomCapacity = MAX_ROOM_PLAYER_COUNT;
    m_bTop = false;
    memset(m_szRoleSex, (char)ESexType_No, MAX_ROOM_PLAYER_COUNT);
}

RoomBriefInfo::~RoomBriefInfo()
{
}

void RoomBriefInfo::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUInt(m_nRoomID);
    IOBuff.AddStringW(m_strRoomName.c_str());
    IOBuff.AddBool(m_bHasPwd);
    IOBuff.AddBool(m_bHasStart);
    IOBuff.AddUChar(m_chMode);
    IOBuff.AddUChar(m_chRoomColor);
    IOBuff.AddUChar(m_chRoomCapacity);
    IOBuff.AddBool(m_bTop);

    for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; ++i)
    {
        IOBuff.AddUChar((char)m_szRoleSex[i]);
    }
}


RoomWholeInfo::RoomWholeInfo()
{
    m_nRoomID = 0;
    m_strRoomName = "";
    m_strRoomPwd = "";
    m_bHasPwd = false;
    m_bIsHost = false;
    m_chScene = RAN_SCENE_ID;
    m_nMusic = RAN_MUSIC_ID;
    m_chMode = MusicMode_None;
    m_chLevel = MusicLevel_None;
    m_eTeamMode = EDanceRoomTeamMode_None;
    m_chRoomColor = 0;

    memset(m_szPosDancer, (char)RoomPos_Open, MAX_ROOM_PLAYER_COUNT);
    memset(m_szPosAudience, (char)RoomPos_Open, MAX_ROOM_AUDIENCE_COUNT);
}

RoomWholeInfo::~RoomWholeInfo()
{
}

void RoomWholeInfo::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUInt(m_nRoomID);
    IOBuff.AddStringW(m_strRoomName.c_str());
    IOBuff.AddUChar(m_chRoomColor);
    IOBuff.AddStringW(m_strRoomPwd.c_str());
    IOBuff.AddBool(m_bHasPwd);
    IOBuff.AddBool(m_bIsHost);
    IOBuff.AddUChar(m_chScene);
    IOBuff.AddUShort(m_nMusic);
    IOBuff.AddUChar(m_chMode);
    IOBuff.AddUChar(m_chLevel);
    IOBuff.AddUChar((unsigned char)m_eTeamMode);

    for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; ++i)
    {
        IOBuff.AddUChar(m_szPosDancer[i]);
    }

    for (int i = 0; i < MAX_ROOM_AUDIENCE_COUNT; ++i)
    {
        IOBuff.AddUChar(m_szPosAudience[i]);
    }

    IOBuff.AddUShort((unsigned short)m_lstDancer.size());
    for (std::list<RoomPlayerInfo>::iterator it = m_lstDancer.begin(); it != m_lstDancer.end(); ++it)
    {
        (*it).doEncode(IOBuff);
    }

    IOBuff.AddUShort((unsigned short)m_lstAudience.size());
    for (std::list<RoomPlayerInfo>::iterator it = m_lstAudience.begin(); it != m_lstAudience.end(); ++it)
    {
        (*it).doEncode(IOBuff);
    }
}

void RoomWholeInfo::doDecode(CParamPool& IOBuff)
{
    m_nRoomID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strRoomName);
    m_chRoomColor = IOBuff.GetUChar();
    IOBuff.GetStringW(m_strRoomPwd);
    m_bHasPwd = IOBuff.GetBool();
    m_bIsHost = IOBuff.GetBool();
    m_chScene = IOBuff.GetUChar();
    m_nMusic = IOBuff.GetUShort();
    m_chMode = IOBuff.GetUChar();
    m_chLevel = IOBuff.GetUChar();
    m_eTeamMode = (EDanceRoomTeamMode)IOBuff.GetUChar();

    for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; ++i)
    {
        m_szPosDancer[i] = IOBuff.GetUChar();
    }

    for (int i = 0; i < MAX_ROOM_AUDIENCE_COUNT; ++i)
    {
        m_szPosAudience[i] = IOBuff.GetUChar();
    }

    unsigned short lstDancerSize = IOBuff.GetUShort();
    for (unsigned short i = 0; i < lstDancerSize; ++i)
    {
        RoomPlayerInfo r;
        r.doDecode(IOBuff);
        m_lstDancer.push_back(r);
    }

    unsigned short lstAudienceSize = IOBuff.GetUShort();
    for (unsigned short i = 0; i < lstAudienceSize; ++i)
    {
        RoomPlayerInfo r;
        r.doDecode(IOBuff);
        m_lstAudience.push_back(r);
    }

    return;
}



//////////////////////////////////////////////////////////////////////////
CDanceTeamMarkSend::CDanceTeamMarkSend()
    :m_eColorTeam(EDanceColorTeam_None)
    , m_nTotalMark(0)
{

}

CDanceTeamMarkSend::~CDanceTeamMarkSend()
{

}

void CDanceTeamMarkSend::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddUChar((unsigned char)m_eColorTeam);
    IOBuff.AddInt(m_nTotalMark);
}


//////////////////////////////////////////////////////////////////////////
// 
InivteePlayerInfo::InivteePlayerInfo()
{
    m_nRoleID = 0;
    m_strRoleName = "";
    m_chRoleLevel = 0;
    m_chRoleSex = ESexType_No;
    m_bIsVIP = false;
    m_nVIPLevel = 0;
}

InivteePlayerInfo::~InivteePlayerInfo()
{
}

void InivteePlayerInfo::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddStringW(m_strRoleName.c_str());
    IOBuff.AddUChar(m_chRoleLevel);
    IOBuff.AddUChar(m_chRoleSex);
    IOBuff.AddBool(m_bIsVIP);
    IOBuff.AddUShort(m_nVIPLevel);
}





CRoleDanceRoomTeamInfo::CRoleDanceRoomTeamInfo()
    :m_nRoleID(0)
    , m_eRoomTeamMode(EDanceRoomTeamMode_None)
    , m_eRoomColorTeam(EDanceColorTeam_Red)
{

}

CRoleDanceRoomTeamInfo::~CRoleDanceRoomTeamInfo()
{

}

void CRoleDanceRoomTeamInfo::doEncoede(CParamPool & IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddUChar((unsigned char)m_eRoomTeamMode);
    IOBuff.AddUChar((unsigned char)m_eRoomColorTeam);
}

CWholeDanceRoomTeamInfo::CWholeDanceRoomTeamInfo()
    :m_eRoomTeamMode(EDanceRoomTeamMode_None)
{

}

CWholeDanceRoomTeamInfo::~CWholeDanceRoomTeamInfo()
{

}

void CWholeDanceRoomTeamInfo::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddUChar((unsigned char)m_eRoomTeamMode);
    IOBuff.AddUShort((unsigned short)m_listRoleDanceRoomTeamInfo.size());
    for (std::list<CRoleDanceRoomTeamInfo>::iterator itr = m_listRoleDanceRoomTeamInfo.begin(); itr != m_listRoleDanceRoomTeamInfo.end(); ++itr) {
        itr->doEncoede(IOBuff);
    }
}





