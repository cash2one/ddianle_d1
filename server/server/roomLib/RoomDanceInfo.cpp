#include "RoomDanceInfo.h"
#include "../../socket/Utility.h"
#include "../../socket/Formatter.h"
#include "../../socket/Md5.h"
#include "../../socket/Log.h"
#include "RoomCheckDataMgr.h"
#include "../../datastructure/datainfor/MusicConfig.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../music/MusicStageFileManager.h"
#include "../grade/GradeInfoMgr.h"



//////////////////////////////////////////////////////////////////////////
// LWDanceRoleInfo
LWDanceRoleInfo::LWDanceRoleInfo(unsigned int nRoleID, ERoleRoomState eRoleRoomState)
    : m_nRoleID(nRoleID)
    , m_eState(eRoleRoomState)
    , m_nTotalMark(0)
{
    // empty
}

LWDanceRoleInfo::~LWDanceRoleInfo()
{
    // empty
}

bool LWDanceRoleInfo::RecvRoundMark(int nMode, int nRound, unsigned int nMark,
    char nKeyRank, const std::vector<int>& rRoundRank)
{
    return m_markInfo.AddRoundMark(nMode, nRound, nMark, nKeyRank, rRoundRank);
}

void LWDanceRoleInfo::ClearMark()
{
    m_nTotalMark = 0;
    m_markInfo.ClearMark();

    return;
}


void LWDanceRoleInfo::SumTotalMark(int nRank, int nRankMark)
{
    m_nTotalMark += m_markInfo.TotalRankCount(nRank) * nRankMark;
    
    return;
}

void LWDanceRoleInfo::CalcGrade()
{
    int nGrade = RoomDanceGrade_D;

    if (m_markInfo.TotalRankBase() > 0)
    {
        int nAccuracy = m_nTotalMark / m_markInfo.TotalRankBase();
        if (nAccuracy >= 100)
        {
            nGrade = RoomDanceGrade_SS;
        }
        else if (nAccuracy >= 90)
        {
            if (m_markInfo.TotalRankPercent(RoomRoundRank_Good) < 1 &&
                m_markInfo.TotalRankPercent(RoomRoundRank_Bad) < 1 &&
                m_markInfo.TotalRankCount(RoomRoundRank_Miss) == 0)
            {
                nGrade = RoomDanceGrade_S;
            }
            else
            {
                nGrade = RoomDanceGrade_A;
            }
        }
        else if (nAccuracy >= 80)
        {
            if (m_markInfo.TotalRankCount(RoomRoundRank_Miss) == 0)
            {
                nGrade = RoomDanceGrade_A;
            }
            else
            {
                nGrade = RoomDanceGrade_B;
            }
        }
        else if (nAccuracy >= 70)
        {
            if (m_markInfo.TotalRankCount(RoomRoundRank_Miss) == 0)
            {
                nGrade = RoomDanceGrade_B;
            }
            else
            {
                nGrade = RoomDanceGrade_C;
            }
        }
        else if (nAccuracy >= 60)
        {
            nGrade = RoomDanceGrade_C;
        }
    }

    m_markInfo.WinGrade(nGrade);

    return;
}


unsigned int LWDanceRoleInfo::GetTotalScore() const
{
    return m_markInfo.Mark();
}


//////////////////////////////////////////////////////////////////////////
// LWRoomDanceInfo:
LWRoomDanceInfo::LWRoomDanceInfo()
{
    Init();
}

LWRoomDanceInfo::~LWRoomDanceInfo()
{
    Clear();
}

void LWRoomDanceInfo::Init()
{ // 每次创建房间的时候，需要通过他来执行初始化
    m_eState = eRoomState_Idle; // 
    m_nPlayScene = 0;
    m_nPlayMusic = 0;
    m_nPlayMusicMode = 0;
    m_nPlayMusicLevel = 0;
    m_strCheckCode = "";
    m_strMusicSource = "";
    m_nCheckTime = 0;
    m_nStartTime = 0;

    for (DancerMap::iterator iter = m_mapDancers.begin(); iter != m_mapDancers.end(); ++iter)
    {
        SAFE_DELETE(iter->second);
    }

    m_mapDancers.clear();
}

void LWRoomDanceInfo::Clear()
{
    Init();
}

// request start dance in room.
EStartRoomRes LWRoomDanceInfo::StartRoomDance(unsigned int nRoleID)
{
    EStartRoomRes eRes = StartRoom_Success;

    do 
    {
        // 所有的人都准备了
        if (!CheckAllRoleState(RoleRoomState_Ready))
        {
            eRes = StartRoom_NotReady;
            break;
        }

        // not test team mode and sweat heart mode.
        m_eState = eRoomState_Preparing;
        m_nStartTime = (unsigned int)time(NULL);
        CodeRandom(MAX_ROOMKEY_LENGTH).swap(m_strCheckCode);

        PrepareMusicAndScene();

        m_nCheckTime = CRoomCheckDataMgr::Instance().GetRoomCheck(m_nPlayMusicMode).m_nCheckTime;
    } while (0);

    return eRes; // 
}

// recieve client loading progress.
void LWRoomDanceInfo::OnLoadingStartGameProgress(unsigned int nRoleID, unsigned char nRate)
{
    // do nothing. 
}

// client load all res complete.
void LWRoomDanceInfo::OnLoadComplete(unsigned int nRoleID)
{
    SetDancerState(nRoleID, RoleRoomState_ToStart); // 加载完毕
    m_eState = eRoomState_Waiting; // 等待开启
}

// client report round mark.
void LWRoomDanceInfo::OnReportRoundMark(unsigned int nRoleID, const std::string &rMD5Code,
    unsigned int nMark, char nKeyRank, unsigned char nRound, const std::vector<char> &rRoundRank)
{
    if (RoomRoundRank_None > nKeyRank || nKeyRank > RoomRoundRank_Max || nRound < 0)
        return;

    if (!IsPlaying())
    {
        return;
    }

    std::string strCode;
    std::vector<int> vectRoundRank(rRoundRank.begin(), rRoundRank.end());
    Formatter(strCode) << GetCheckCode() << (int)nRound << (int)nKeyRank << nMark;

    for (unsigned int i = 0; i < vectRoundRank.size(); ++i)
    {
        std::string strRank;
        Formatter(strRank) << vectRoundRank[i];
        strCode.append(strRank);
    }

    unsigned char szMD5Code[16] = { 0 };
    MDString(strCode.c_str(), szMD5Code);
    BinaryData2String(szMD5Code, sizeof(szMD5Code)).swap(strCode);

    if (rMD5Code != strCode)
    {
        return;
    } // 消息加密验证

    LWDanceRoleInfo *dancer = GetDanceRole(nRoleID);
    ENSURE_WITH_LOG_CMD(NULL != dancer, return, "error to get role[%u] in DanceRoom.", nRoleID);
    
    dancer->RecvRoundMark(GetPlayMusicMode(), nRound, nMark, nKeyRank, vectRoundRank);

    return;
}

// 跳舞结束
LWRoomDanceInfo::EEndRoomResult LWRoomDanceInfo::EndRoom(unsigned int nRoleID)
{
    LWDanceRoleInfo *pRole = GetDanceRole(nRoleID);
    if (NULL == pRole)
        return EEndRoomResult_Fail;
    else if (!IsPlaying() && GetState() != eRoomState_Ending)
        return EEndRoomResult_Fail;
    else if (pRole->GetState() != RoleRoomState_Start)
        return EEndRoomResult_Fail;
    else if (!HasLegalEndTime())
        return EEndRoomResult_TooFast; // 关闭太早了。

    pRole->SetState(RoleRoomState_ToEnd); // 进入等待结束状态
    m_eState = eRoomState_Ending; // 

    return EEndRoomResult_Sucess;
}

void LWRoomDanceInfo::OnDanceStartCallBack()
{
    m_eState = eRoomState_Playing;
    SetAllDancerState(RoleRoomState_Start);

    for (DancerMap::iterator iter = m_mapDancers.begin(); iter != m_mapDancers.end(); ++iter)
    {
        iter->second->ClearMark();
    }
}

void LWRoomDanceInfo::OnDanceEndCallBack()
{
    m_eState = eRoomState_Idle; // 进入新的一轮状态
    SetAllDancerState(RoleRoomState_Wait); // 所有人变为wait状态
    CalcDanceResult();
}

void LWRoomDanceInfo::FillMusicInfo(int nPlayScene, int nPlayMusic, unsigned char nPlayMusicMode, unsigned char nPlayMusicLevel)
{
    m_nPlayScene = nPlayScene;
    m_nPlayMusic = nPlayMusic;
    m_nPlayMusicMode = nPlayMusicMode;
    m_nPlayMusicLevel = nPlayMusicLevel;

    return;
}

void LWRoomDanceInfo::AddRole(unsigned int nRoleID, ERoleRoomState eRoleRoomState)
{
    ENSURE_WITH_LOG_CMD(!HasDancer(nRoleID), return, "dance room info already has this player.")

    m_mapDancers.insert(std::make_pair(nRoleID, new LWDanceRoleInfo(nRoleID, eRoleRoomState)));

    return; 
}

void LWRoomDanceInfo::RemoveRole(unsigned int nRoleID)
{
    DancerMap::iterator iter = m_mapDancers.find(nRoleID);
    if (iter != m_mapDancers.end())
    {
        SAFE_DELETE(iter->second);
        m_mapDancers.erase(iter);
    }

    return;
}

void LWRoomDanceInfo::RemoveNotToStartRole(std::vector<unsigned int> &vecRole)
{
    for (DancerMap::iterator iter = m_mapDancers.begin(); iter != m_mapDancers.end(); ++iter)
    {
        LWDanceRoleInfo *pDanceRoleInfo = iter->second;
        if (NULL != pDanceRoleInfo && pDanceRoleInfo->GetState() != RoleRoomState_ToStart)
            vecRole.push_back(iter->first);
    }

    for (size_t i = 0; i < vecRole.size(); ++i)
    {
        RemoveRole(vecRole[i]);
    }

    return;
}

void LWRoomDanceInfo::RemoveNotToEndRole(std::vector<unsigned int> &vecRole)
{
    for (DancerMap::iterator iter = m_mapDancers.begin(); iter != m_mapDancers.end(); ++iter)
    {
        LWDanceRoleInfo *pDanceRoleInfo = iter->second;
        if (NULL != pDanceRoleInfo && pDanceRoleInfo->GetState() != RoleRoomState_ToEnd)
            vecRole.push_back(iter->first);
    }

    for (size_t i = 0; i < vecRole.size(); ++i)
    {
        RemoveRole(vecRole[i]);
    }

    return;
}



// 取得跳舞信息
bool LWRoomDanceInfo::RoleMarkInfo(unsigned int nRoleID, CRoomMarkInfo& markInfo)
{
    LWDanceRoleInfo *pDancer = GetDanceRole(nRoleID);
    ENSURE_WITH_LOG_CMD(NULL != pDancer, return false, "error get role[%u] mark info", nRoleID);
    
    markInfo = pDancer->GetMarkInfo();
    return true;
}


bool LWRoomDanceInfo::LoadStage(std::vector<char> &vecAndroidStage, std::vector<char> &vecIosStage, std::vector<char> &vecWinPhoneStage)
{
    std::string strStageFile;
    CMusicStageFileManager::GetStageFileName(m_strMusicSource,
        m_nPlayMusicMode, (int)m_nPlayMusicLevel, strStageFile);

    if (strStageFile.empty())
        return false;

    if (CMusicStageFileManager::LoadStage(strStageFile, vecAndroidStage, vecIosStage, vecWinPhoneStage))
    {
        int nStageTime = CMusicStageFileManager::CalcuStageTime(m_nPlayMusicMode, vecAndroidStage, vecIosStage, vecWinPhoneStage);
        SetCheckTime(nStageTime);

        return true;
    }

    return false;
}

void LWRoomDanceInfo::CalcDanceResult()
{
    for (DancerMap::const_iterator itr = m_mapDancers.begin(); itr != m_mapDancers.end(); ++itr)
    {
        LWDanceRoleInfo* dancer = itr->second;
        ENSURE_WITH_LOG_CMD(NULL != dancer, continue, "error get NULL dancer in room");

        SumRankMark(dancer);
//        DanceResultRanking(dancer);
    }
}

bool LWRoomDanceInfo::HasStart() const
{
    return (GetState() == eRoomState_Preparing
        || GetState() == eRoomState_Playing
        || GetState() == eRoomState_SelectPartner
        || GetState() == eRoomState_Ending);
}

bool LWRoomDanceInfo::HasDancer() const
{
    return !m_mapDancers.empty(); // 
}

bool LWRoomDanceInfo::HasDancer(unsigned int nRoleID) const
{
    return m_mapDancers.find(nRoleID) != m_mapDancers.end();
}


//////////////////////////////////////////////////////////////////////////
// LWRoomDanceInfo private func.
bool LWRoomDanceInfo::CheckAllRoleState(ERoleRoomState eState)
{
    for (DancerMap::iterator iter = m_mapDancers.begin(); iter != m_mapDancers.end(); ++iter)
    {
        if (iter->second->GetState() != eState)
            return false;
    }

    return true;
}

bool LWRoomDanceInfo::IsPlaying()
{
    return GetState() == eRoomState_Playing;
}

void LWRoomDanceInfo::SetAllDancerState(ERoleRoomState eState)
{
    for (DancerMap::iterator iter = m_mapDancers.begin(); iter != m_mapDancers.end(); ++iter)
    {
        iter->second->SetState(eState);
    }

    return;
}

void LWRoomDanceInfo::SetDancerState(unsigned int nRoleID, ERoleRoomState eState)
{
    LWDanceRoleInfo *pRoleInfo = GetDanceRole(nRoleID);
    if (NULL != pRoleInfo)
        pRoleInfo->SetState(eState);

    return;
}

ERoleRoomState LWRoomDanceInfo::GetDancerState(unsigned int nRoleID)
{
    LWDanceRoleInfo *pRoleInfo = GetDanceRole(nRoleID);
    if (NULL != pRoleInfo)
        pRoleInfo->GetState();

    return RoleRoomState_None;
}

bool LWRoomDanceInfo::HasLegalEndTime() const
{
    return ((unsigned int)time(NULL) >= m_nStartTime + m_nCheckTime);
}

LWDanceRoleInfo* LWRoomDanceInfo::GetDanceRole(unsigned int nRoleID)
{
    DancerMap::iterator iter = m_mapDancers.find(nRoleID);
    if (iter != m_mapDancers.end())
        return iter->second;

    return NULL;
}



void LWRoomDanceInfo::PrepareMusicAndScene()
{
    const MusicConfig* pMusicInfo = ConfigManager::Instance().GetMusicConfigManager().GetByID((unsigned short)m_nPlayMusic);

    if (pMusicInfo != NULL)
        m_strMusicSource = pMusicInfo->m_strMusicSource;

    return;
}

void LWRoomDanceInfo::SumRankMark(LWDanceRoleInfo *pDancer) const
{
    if (pDancer == NULL)
        return;

    int nRankMark = 0;

    for (int nRank = RoomRoundRank_Miss; nRank < RoomRoundRank_Max; ++nRank)
    {
        nRankMark = CGradeInfoManager::Instance().GetGradePercent(nRank);

        pDancer->SumTotalMark(nRank, nRankMark);
    }

    pDancer->CalcGrade();
}

