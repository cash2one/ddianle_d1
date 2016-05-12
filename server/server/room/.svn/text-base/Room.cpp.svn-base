#include "Room.h"
#include "RoomComponent.h"
#include "../roomLib/RoomCheckDataMgr.h"
#include "LiveRoomDataMgr.h"
#include "../roomLib/GameMsg_S2C_RoomMsg.h"
#include "../roomLib/RoomSettingMgr.h"
#include "GameMsg_S2C_LiveRoomMsg.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../SystemSetting/SystemSettingMgr.h"
#include "RoomManager.h"
#include "../music/MusicBoardMgr.h"
#include "../logic/LogicCirculator.h"
#include "RoomTimer.h"
#include "../util/TimerMgr.h"
#include "../friend/EntityFriendComponent.h"
#include "../friend/GameMsg_S2C_Friend.h"
#include "../util/TimerMgr.h"
#include "../roomLib/RoomMarkInfo.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../music/MusicStageFileManager.h"

extern CLogicCirculator* g_pCirculator;

CRoom::CRoom(ERoomType eType, unsigned int nRoomID)
: m_eRoomType(eType)
, m_eState(eRoomState_Idle)
, m_nRoomID(nRoomID)
, m_strRoomName("")
, m_strRoomPwd("")
, m_bMatch(false)
, m_bStart(false)
, m_nStartTime(0)
, m_nCreateTime(0)
, m_nCheckTime(0)
, m_nStartDeadlineTime(0)
, m_nChoosedMusicID(RAN_MUSIC_ID)
, m_nPlayMusicID(RAN_MUSIC_ID)
, m_nDancerCount(0)
, m_nAudienceCount(0)
, m_nGMCount(0)
, m_nDancerCapacity(MAX_ROOM_PLAYER_COUNT)
, m_nAudienceCapacity(MAX_ROOM_AUDIENCE_COUNT)
, m_nGMCapacity(MAX_ROOM_GM_COUNT)
, m_nStartRoomDancersCount(0)
, m_nStartTeamsCount(0)
, m_bLiveRoomUseDefaultCloth(false)
, m_eRoomTeamMode(EDanceRoomTeamMode_None)
, m_bOpenVote(0)
, m_nLastSendHotValueTime(0)
, m_pHostRoomCOM(NULL)
{
    m_nCreateTime = (int)time(NULL);

    m_nChoosedScene = RoomSettingMgr::Instance().RoomDefaultScene();
    m_nPlayScene = RoomSettingMgr::Instance().RoomDefaultScene();

    m_nChoosedMusicMode = RoomSettingMgr::Instance().RoomDefaultMode();
    m_nPlayMusicMode = RoomSettingMgr::Instance().RoomDefaultMode();

    m_nChoosedMusicLevel = RoomSettingMgr::Instance().RoomDefaultLevel();
    m_nPlayMusicLevel = RoomSettingMgr::Instance().RoomDefaultLevel();

    for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; ++i)
    {
        m_szPosDancer[i] = RoomPos_Open;
        m_szDancer[i] = NULL;
    }

    for (int i = 0; i < MAX_ROOM_AUDIENCE_COUNT; ++i)
    {
        m_szPosAudience[i] = RoomPos_Open;
        m_szAudience[i] = NULL;
    }

    for (int i = 0; i < MAX_ROOM_GM_COUNT; ++i)
    {
        m_szPosGM[i] = RoomPos_Open;
        m_szGM[i] = NULL;
    }

    if (eType == eRoomType_LiveRoomPlayer || eType == eRoomType_LiveRoomGroup || eType == eRoomType_LiveRoomOther)
    {
        m_nAudienceCapacity = RoomSettingMgr::Instance().LiveRoomMaxAudienceCount();
    }

    for (int i = EDanceColorTeam_None; i < EDanceColorTeam_Max; ++i) {
        m_szDanceTeam[i].SetTeamColor((EDanceColorTeam)i);
    }

    memset(m_szLiveRoomDancerID, 0, sizeof(m_szLiveRoomDancerID));
    memset(m_szLiveRoomGroupID, 0, sizeof(m_szLiveRoomGroupID));

    m_bOpenVote = CLiveRoomDataManager::Instance().IsVoteOpen();
    m_nLastSendHotValueTime = 0;
}

CRoom::~CRoom(void)
{
    m_mapLiveDanceScore.clear();
    for (std::vector<ITimer*>::iterator it = m_vecTimersOnStart.begin(); it != m_vecTimersOnStart.end(); ++it)
    {
        delete *it;
    }
    m_vecTimersOnStart.clear();
}

unsigned int CRoom::RoomID() const
{
    return m_nRoomID;
}

const std::string& CRoom::RoomName() const
{
    return m_strRoomName;
}

const std::string& CRoom::RoomPwd() const
{
    return m_strRoomPwd;
}

void CRoom::ChangeRoomInfo(const char* pRoomName, const char* pRoomPwd, const char *pDesc)
{
    m_strRoomName.assign(pRoomName);
    m_strRoomPwd.assign(pRoomPwd);
    m_strLiveRoomDescription.assign(pDesc);
}

void CRoom::SetMatch(bool bMatch)
{
    m_bMatch = bMatch;
}

int CRoom::ChoosedScene() const
{
    return m_nChoosedScene;
}

int CRoom::ChoosedMusicID() const
{
    return m_nChoosedMusicID;
}

int CRoom::ChoosedMusicMode() const
{
    return m_nChoosedMusicMode;
}

int CRoom::PlayScene() const
{
    return m_nPlayScene;
}

int CRoom::PlayMusicID() const
{
    return m_nPlayMusicID;
}

int CRoom::PlayMusicMode() const
{
    return m_nPlayMusicMode;
}

int CRoom::PlayMusicLevel() const
{
    return m_nPlayMusicLevel;
}

const std::string& CRoom::PlayCheckCode() const
{
    return m_strCheckCode;
}

const std::string& CRoom::PlayMusicStage() const
{
    return m_strStageFile;
}

int CRoom::ChoosedMusicLevel() const
{
    return m_nChoosedMusicLevel;
}

void CRoom::ChangeChoosedScene(int nScene)
{
    m_nChoosedScene = nScene;
}

void CRoom::ChangeChoosedMusic(int nMusicID, int nMusicMode, int nMusicLevel)
{
    m_nChoosedMusicID = nMusicID;
    m_nChoosedMusicMode = nMusicMode;
    m_nChoosedMusicLevel = nMusicLevel;
}

bool CRoom::HasPwd() const
{
    return !m_strRoomPwd.empty();
}

bool CRoom::PwdPassed(const char* pRoomPwd) const
{
    if (HasPwd() && pRoomPwd != NULL && m_strRoomPwd.compare(pRoomPwd) != 0)
    {
        return false;
    }

    return true;
}

bool CRoom::IsMatch() const
{
    return m_bMatch;
}

bool CRoom::HasStart() const
{
    return m_bStart;
}

bool CRoom::IsStartReady() const
{
    for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; ++i)
    {
        if (m_szDancer[i] != NULL && m_szDancer[i]->RoleRoomState() != RoleRoomState_Ready)
        {
            return false;
        }
    }

    return true;
}

bool CRoom::IsPlayReady() const
{
    for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; ++i)
    {
        if (m_szDancer[i] != NULL && m_szDancer[i]->RoleRoomState() != RoleRoomState_ToStart)
        {
            return false;
        }
    }

    return true;
}

bool CRoom::IsEndReady() const
{
    for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; ++i)
    {
        if (m_szDancer[i] != NULL && m_szDancer[i]->RoleRoomState() != RoleRoomState_ToEnd)
        {
            return false;
        }
    }

    return true;
}

bool CRoom::HasDancer() const
{
    return (m_nDancerCount > 0);
}

bool CRoom::HasAudience() const
{
    return (m_nAudienceCount > 0);
}

bool CRoom::HasGM() const
{
    return (m_nGMCount > 0);
}

void CRoom::SetDancer(int nPos, CRoomComponent *pRole)
{
    if (nPos > -1 && nPos < MAX_ROOM_PLAYER_COUNT)
    {
        CEntityAttributeComponent* pAttr = NULL;
        if (pRole)
        {
            pAttr = pRole->AttrCOM();
            m_setSexDancer[pAttr->GetSex()].insert(pAttr->GetRoleID());
            m_mapDancer2Sex[pAttr->GetRoleID()] = pAttr->GetSex();
        }
        else
        {
            if (m_szDancer[nPos])
            {
                pAttr = m_szDancer[nPos]->AttrCOM();
                m_setSexDancer[pAttr->GetSex()].erase(pAttr->GetRoleID());
                m_mapDancer2Sex.erase(pAttr->GetRoleID());
            }
        }
        m_szDancer[nPos] = pRole;
    }
}
void CRoom::SetLiveRoomDancer(int nPos, unsigned int nRoleID, const std::string &strRoleName)
{
    if (nPos > -1 && nPos < MAX_ROOM_PLAYER_COUNT)
    {
        m_szLiveRoomDancerID[nPos] = nRoleID;
        m_szLiveRoomDancerName[nPos] = strRoleName;
    }
}
void CRoom::RemoveLiveRoomDancer(unsigned int nRoleID)
{
    for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; i++)
    {
        if (m_szLiveRoomDancerID[i] == nRoleID)
        {
            m_szLiveRoomDancerID[i] = 0;
            m_szLiveRoomDancerName[i].clear();
        }
    }
}

std::string CRoom::GetLiveRoomDancerName(unsigned int nRoleID)
{
    for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; i++)
    {
        if (m_szLiveRoomDancerID[i] == nRoleID)
        {
            return m_szLiveRoomDancerName[i];
        }
    }
    return std::string("");
}

bool CRoom::IsLiveRoomDancer(unsigned int nRoleID)
{
    for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; i++)
    {
        if (m_szLiveRoomDancerID[i] == nRoleID)
        {
            return true;
        }
    }
    return false;
}

void CRoom::SetLiveRoomGroup(int nPos, unsigned int nGroupID, const std::string &strGroupName)
{
    if (nPos > -1 && nPos < MAX_ROOM_PLAYER_COUNT)
    {
        m_szLiveRoomGroupID[nPos] = nGroupID;
        m_szLiveRoomGroupName[nPos] = strGroupName;
    }
}

int CRoom::MaleDancerCount() const
{
    return (int)m_setSexDancer[ESexType_Male].size();
}

int CRoom::FemaleDancerCount() const
{
    return (int)m_setSexDancer[ESexType_Female].size();
}

int CRoom::MaxDancerPos() const
{
    return m_nDancerCapacity - 1;
}

int CRoom::MaxAudiencePos() const
{
    return m_nAudienceCapacity - 1;
}

int CRoom::MaxGMPos() const
{
    return m_nGMCapacity - 1;
}

int CRoom::DancerCount() const
{
    return m_nDancerCount;
}

int CRoom::AudienceCount() const
{
    return m_nAudienceCount;
}

int CRoom::GMCount() const
{
    return m_nGMCount;
}

int CRoom::DancerCapacity() const
{
    return m_nDancerCapacity;
}

int CRoom::AudienceCapacity() const
{
    return m_nAudienceCapacity;
}

void CRoom::SetAudienceCapacity(int nCount)
{
    m_nAudienceCapacity = nCount;
}


int CRoom::GMCapacity() const
{
    return m_nGMCapacity;
}

int CRoom::StartTime() const
{
    return m_nStartTime;
}

int CRoom::CreateTime() const
{
    return m_nCreateTime;
}

int CRoom::GetCheckTime() const
{
    return m_nCheckTime;
}

void CRoom::SetCheckTime(int nCheckTime)
{
    nCheckTime -= 5;
    m_nCheckTime = (nCheckTime > 0 ? nCheckTime : 0);
}

bool CRoom::HasEndCountDown() const
{
    return (unsigned int)m_nStartDeadlineTime <= (unsigned int)time(NULL);
}

int CRoom::PosState(int nRoleRoomType, int nRoleRoomPos) const
{
    int nPosState = RoomPos_Open;

    if (IsNormalRoom() || IsAuditionRoom())
    {
        if (nRoleRoomType == RoleRoomType_Dancer && nRoleRoomPos >= 0 && nRoleRoomPos < MAX_ROOM_PLAYER_COUNT)
        {
            nPosState = m_szPosDancer[nRoleRoomPos];
        }
        else if (nRoleRoomType == RoleRoomType_Audience && nRoleRoomPos >= 0 && nRoleRoomPos < MAX_ROOM_AUDIENCE_COUNT)
        {
            nPosState = m_szPosAudience[nRoleRoomPos];
        }
    }
    else if (IsLiveRoom())
    {
        if (nRoleRoomType == RoleRoomType_Dancer && nRoleRoomPos >= 0 && nRoleRoomPos < m_nDancerCapacity)
        {
            nPosState = m_szPosDancer[nRoleRoomPos];
        }
        else if (nRoleRoomType == RoleRoomType_Audience && nRoleRoomPos >= 0 && nRoleRoomPos < m_nAudienceCapacity)
        {
            nPosState = RoomPos_Open;
        }
        else if (nRoleRoomType == RoleRoomType_GM && nRoleRoomPos >= 0 && nRoleRoomPos < m_nGMCapacity)
        {
            nPosState = m_szPosGM[nRoleRoomPos];
        }
    }

    return nPosState;
}

bool CRoom::ChangePosState(int nRoleRoomType, int nRoleRoomPos, int nRoomPosState)
{
    bool bRes = false;

    if (IsNormalRoom() || IsAuditionRoom())
    {
        if (nRoleRoomType == RoleRoomType_Dancer)
        {
            bRes = _ChangePosDancerState(nRoleRoomPos, nRoomPosState);
        }
        else if (nRoleRoomType == RoleRoomType_Audience)
        {
            bRes = _ChangePosAudienceState(nRoleRoomPos, nRoomPosState);
        }
    }
    else if (IsLiveRoom())
    {
        if (nRoleRoomType == RoleRoomType_Dancer)
        {
            bRes = _ChangePosDancerState(nRoleRoomPos, nRoomPosState);
        }
        else if (nRoleRoomType == RoleRoomType_Audience)
        {
            bRes = _ChangePosAudienceState(nRoleRoomPos, nRoomPosState);
        }
        else if (nRoleRoomType == RoleRoomType_GM)
        {
            bRes = _ChangePosGMState(nRoleRoomPos, nRoomPosState);
        }
    }

    return bRes;
}

void CRoom::SetRoomHost(CRoomComponent *pRoleRoom)
{
    m_pHostRoomCOM = pRoleRoom;
}

CRoomComponent* CRoom::AssignHost()
{
    if (m_pHostRoomCOM == NULL)
    {
        for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; ++i)
        {
            if (m_szDancer[i] != NULL)
            {
                m_pHostRoomCOM = m_szDancer[i];

                break;
            }
        }
    }

    return m_pHostRoomCOM;
}

CRoomComponent* CRoom::ReassignHost(int nRoleRoomType, int nPos)
{
    if (nRoleRoomType == RoleRoomType_Dancer)
    {
        if (m_szDancer[nPos] != NULL)
        {
            m_pHostRoomCOM = m_szDancer[nPos];

            return m_pHostRoomCOM;
        }
    }

    return NULL;
}

CRoomComponent* CRoom::RoomHost()
{
    return m_pHostRoomCOM;
}

int CRoom::RoomState() const
{
    return m_eState;
}

void CRoom::SetState(ERoomState eState)
{
    m_eState = eState;
}

void CRoom::RoomPrepare()
{
    m_bStart = true;
    m_eState = eRoomState_Preparing;
    m_nStartTime = 0;
    m_mapSelectingPartner.clear();
    m_mapSelectedPartner.clear();
    m_listRoundRange.clear();
    m_nStartRoomDancersCount = m_nDancerCount;
    m_nStartDeadlineTime = (int)time(NULL) + RoomSettingMgr::Instance().LiveRoomCountDownTime();

    for (int i = EDanceColorTeam_Red; i < EDanceColorTeam_Max; ++i)
    {
        if (TeamMemberCount((EDanceColorTeam)i) > 0)
        {
            ++m_nStartTeamsCount;
        }
    }

    // music
    MusicConfig* pMusicInfo = PrepareMusicAndScene();

    // stage file
    if (pMusicInfo != NULL)
    {
        CMusicStageFileManager::GetStageFileName(pMusicInfo->m_strMusicSource,
            m_nPlayMusicMode, m_nPlayMusicLevel, m_strStageFile);
    }

    // check code
    PrepareCheckCode();
}

MusicConfig* CRoom::PrepareMusicAndScene()
{
    MusicConfig* pMusicInfo = NULL;

    // 新手包和魔法神灯关卡房间随机音乐时，只随机小包音乐
    bool bIsDungeonRoom = (RoomType() == eRoomType_Dungeon ? true : false);
    bool bIsFresherInstaller = (g_pCirculator->GetServerInstallerID() == INSTALLER_FRESHER ? true : false);
    bool bOnlyFresher = bIsFresherInstaller || bIsDungeonRoom;

    // 新手引导(新手房间)只使用新手引导音乐
    bool bIsNewPlayerRoom = (RoomType() == eRoomType_NewPlayer ? true : false);

    // 赋值角色歌曲背景信息
    m_nPlayScene = m_nChoosedScene;
    m_nPlayMusicMode = m_nChoosedMusicMode;
    m_nPlayMusicLevel = m_nChoosedMusicLevel;
    m_nPlayMusicID = m_nChoosedMusicID;

    // filter scene
    if (m_nChoosedScene == RAN_SCENE_ID)
    {
        m_nPlayScene = ConfigManager::Instance().GetSceneModeManager().RandomScene(bOnlyFresher);
    }

    // filter mode(TODO: RANDOM MusicMode_Sweethearts)
    if (m_nChoosedMusicMode == MusicMode_None)
    {
        m_nPlayMusicMode = ConfigManager::Instance().GetMusicStageManager().RandomMode((unsigned short)m_nPlayMusicID, MusicMode_Sweethearts);
    }

    // filter level
    if (m_nChoosedMusicLevel == MusicLevel_None)
    {
        m_nPlayMusicLevel = ConfigManager::Instance().GetMusicStageManager().RandomLevel((unsigned short)m_nPlayMusicID);
    }

    // filter music
    if (m_nChoosedMusicID == RAN_MUSIC_ID)
    {
        if (bOnlyFresher)
        {
            m_nPlayMusicID = ConfigManager::Instance().GetMusicStageManager().RandomMusic(
                (unsigned char)m_nPlayMusicMode, (unsigned char)m_nPlayMusicLevel, bOnlyFresher);

            pMusicInfo = ConfigManager::Instance().GetMusicConfigManager().GetByID((unsigned short)m_nPlayMusicID);
            if (pMusicInfo != NULL && !pMusicInfo->m_bIsFresher)
            {
                pMusicInfo = ConfigManager::Instance().GetMusicConfigManager().GetAnyFresherMusic();
                if (pMusicInfo != NULL)
                {
                    m_nPlayMusicID = pMusicInfo->m_nMusicID;

                    if (!ConfigManager::Instance().GetMusicStageManager().IsHaveMode((unsigned short)m_nPlayMusicID, (unsigned char)m_nPlayMusicMode))
                    {
                        m_nPlayMusicMode = ConfigManager::Instance().GetMusicStageManager().RandomMode((unsigned short)m_nPlayMusicID, MusicMode_Sweethearts);
                    }

                    if (!ConfigManager::Instance().GetMusicStageManager().IsHaveLevel((unsigned short)m_nPlayMusicID, (unsigned char)m_nPlayMusicLevel))
                    {
                        m_nPlayMusicLevel = ConfigManager::Instance().GetMusicStageManager().RandomLevel((unsigned short)m_nPlayMusicID);
                    }
                }
            }
        }
        else
        {
            m_nPlayMusicID = ConfigManager::Instance().GetMusicStageManager().RandomMusic(
                (unsigned char)m_nPlayMusicMode, (unsigned char)m_nPlayMusicLevel);
        }
    }

    // new player
    if (bIsNewPlayerRoom)
    {
        int nRandonChance = 3;
        while (nRandonChance > 0)
        {
            int nMusicID = CMusicBoardManager::Instance().RandomFresherSong((unsigned char)m_nPlayMusicMode);
            if (nMusicID != RAN_MUSIC_ID)
            {
                m_nPlayMusicID = nMusicID;
                break;
            }

            --nRandonChance;
        }
    }

    pMusicInfo = ConfigManager::Instance().GetMusicConfigManager().GetByID((unsigned short)m_nPlayMusicID);

    return pMusicInfo;
}

void CRoom::PrepareCheckCode()
{
    m_strCheckCode = CodeRandom(MAX_ROOMKEY_LENGTH);
    const CRoomCheck& rRoomCheck = CRoomCheckDataMgr::Instance().GetRoomCheck(m_nPlayMusicMode);
    m_nCheckTime = rRoomCheck.m_nCheckTime;
}

void CRoom::RoomStart()
{
    m_eState = eRoomState_Playing;
    m_nStartTime = time(NULL);

    for (std::vector<ITimer*>::iterator it = m_vecTimersOnStart.begin(); it != m_vecTimersOnStart.end(); ++it)
    {
        ITimer* pTimer = *it;
        pTimer->m_nTriggerTime += m_nStartTime;
        CTimerMgr::Instance().Add(pTimer);
    }

    m_vecTimersOnStart.clear();
    m_mapSelectingPartner.clear();

    for (int i = 0; i < m_nDancerCapacity; ++i)
    {
        if (m_szDancer[i] != NULL)
        {
            if (m_nPlayMusicMode == MusicMode_Sweethearts)
            {
                CEntityAttributeComponent* pAttrCOM = m_szDancer[i]->AttrCOM();
                m_mapSelectingPartner.insert(std::make_pair(pAttrCOM->GetRoleID(), 0));
            }

            m_szDancer[i]->OnStartDance();
        }
    }
}

void CRoom::RoomEnd()
{
    if (m_nPlayMusicMode == MusicMode_Sweethearts)
    {
        AskMakeFriend();
    }

    m_bMatch = false;
    m_bStart = false;
    m_eState = eRoomState_Idle;
    m_nStartTime = 0;
    m_nCheckTime = 0;
    m_nStartDeadlineTime = 0;

    m_nPlayScene = RAN_SCENE_ID;
    m_nPlayMusicID = RAN_MUSIC_ID;
    m_nPlayMusicMode = MusicMode_Taiko;
    m_nPlayMusicLevel = MusicLevel_Easy;
    m_nStartRoomDancersCount = 0;
    m_nStartTeamsCount = 0;
    m_mapSelectingPartner.clear();
    m_mapSelectedPartner.clear();
    m_listRoundRange.clear();

    m_strCheckCode = "";
    m_strStageFile = "";

    for (int i = EDanceColorTeam_None; i < EDanceColorTeam_Max; ++i) {
        m_szDanceTeam[i].Reset();
    }

    m_AuditionRoleInfo.clear();
}

CRoomComponent* CRoom::Player(int nRoleRoomType, int nRoleRoomPos)
{
    CRoomComponent* pPlayer = NULL;

    if (IsNormalRoom() || IsAuditionRoom())
    {
        if (nRoleRoomType == RoleRoomType_Dancer && nRoleRoomPos >= 0 && nRoleRoomPos < m_nDancerCapacity)
        {
            pPlayer = m_szDancer[nRoleRoomPos];
        }
        else if (nRoleRoomType == RoleRoomType_Audience && nRoleRoomPos >= 0 && nRoleRoomPos < m_nAudienceCapacity)
        {
            pPlayer = m_szAudience[nRoleRoomPos];
        }
    }
    else if (IsLiveRoom())
    {
        if (nRoleRoomType == RoleRoomType_Dancer && nRoleRoomPos >= 0 && nRoleRoomPos < m_nDancerCapacity)
        {
            pPlayer = m_szDancer[nRoleRoomPos];
        }
        else if (nRoleRoomType == RoleRoomType_Audience)
        {
            if (m_mapLiveRoomAudience.find(nRoleRoomPos) != m_mapLiveRoomAudience.end())
            {
                pPlayer = m_mapLiveRoomAudience.find(nRoleRoomPos)->second;
            }
        }
        else if (nRoleRoomType == RoleRoomType_GM && nRoleRoomPos >= 0 && nRoleRoomPos < m_nGMCapacity)
        {
            pPlayer = m_szGM[nRoleRoomPos];
        }
    }

    return pPlayer;
}

int CRoom::PlayerIn(CRoomComponent& rRoleRoomCOM, int nRoleRoomType, int nRoleRoomState, bool bHost)
{
    int nPos = -1;

    if (IsNormalRoom() || IsAuditionRoom())
    {
        if (nRoleRoomType == RoleRoomType_Dancer)
        {
            nPos = _GetIdleDancerPos();
            _DancerSettleIn(rRoleRoomCOM, nPos);
        }
        else if (nRoleRoomType == RoleRoomType_Audience)
        {
            nPos = _GetIdleAudiencePos();
            _AudienceSettleIn(rRoleRoomCOM, nPos);
        }
        if (nPos >= 0 && bHost)
        {
            m_pHostRoomCOM = &rRoleRoomCOM;
        }
    }
    else if (IsLiveRoom())
    {
        if (nRoleRoomType == RoleRoomType_Dancer)
        {
            nPos = _GetIdleDancerPos();
            _DancerSettleIn(rRoleRoomCOM, nPos);
        }
        else if (nRoleRoomType == RoleRoomType_Audience)
        {
            nPos = _GetIdleAudiencePos();
            _AudienceSettleIn(rRoleRoomCOM, nPos);
        }
        else if (nRoleRoomType == RoleRoomType_GM)
        {
            nPos = _GetIdleGMPos();
            if (nPos >= 0 && nPos < m_nGMCapacity)
            {
                _GMSettleIn(rRoleRoomCOM, nPos);
            }
            else
            {
                nPos = _GetIdleAudiencePos();
                _AudienceSettleIn(rRoleRoomCOM, nPos);
            }
        }
    }

    return nPos;
}

CRoomComponent* CRoom::PlayerOut(int nRoleRoomType, int nRoleRoomPos)
{
    CRoomComponent* pPlayer = NULL;

    if (nRoleRoomType == RoleRoomType_Dancer)
    {
        pPlayer = _DancerMoveOut(nRoleRoomPos);
    }
    else if (nRoleRoomType == RoleRoomType_Audience)
    {
        pPlayer = _AudienceMoveOut(nRoleRoomPos);
    }
    else if (nRoleRoomType == RoleRoomType_GM)
    {
        pPlayer = _GMMoveOut(nRoleRoomPos);
    }

    return pPlayer;
}

const AuditionPlayerInfo * CRoom::GetAuditionRivalPlayerInfo(unsigned int nRoleID)
{
    std::map<unsigned int, AuditionPlayerInfo>::iterator it = m_AuditionRoleInfo.begin();
    for (; it != m_AuditionRoleInfo.end(); ++it)
    {
        if (it->first != nRoleID)
        {
            return &(it->second);
        }
    }

    return NULL;
}

CRoomComponent* CRoom::GetAuditionRoomRivalPos(CRoomComponent * pRoleRoomCOM)
{
    for (int i = 0; i < m_nDancerCapacity; ++i)
    {
        if (m_szDancer[i] == pRoleRoomCOM)
        {
            continue;
        }

        if (m_szDancer[i] != NULL)
        {
            return m_szDancer[i];
        }
    }

    return NULL;
}

int CRoom::PlayerChangeType(CRoomComponent& rRoleRoomCOM, int nRoleRoomType)
{
    int nPos = -1;

    if (nRoleRoomType == RoleRoomType_Audience)
    {
        nPos = _GetIdleAudiencePos();
        _ChangeToAudience(rRoleRoomCOM, nPos);
    }
    else if (nRoleRoomType == RoleRoomType_Dancer)
    {
        nPos = _GetIdleDancerPos();
        _ChangeToDancer(rRoleRoomCOM, nPos);
    }

    return nPos;
}

void CRoom::EncodeBriefInfo(RoomBriefInfo& rBriefInfo) const
{
    rBriefInfo.m_nRoomID = m_nRoomID;
    rBriefInfo.m_strRoomName = m_strRoomName;
    rBriefInfo.m_bHasPwd = HasPwd();
    rBriefInfo.m_bHasStart = HasStart();
    rBriefInfo.m_chMode = (char)m_nChoosedMusicMode;
    rBriefInfo.m_chRoomColor = (char)RoomColor();
    rBriefInfo.m_chRoomCapacity = (char)DancerCapacity();
    rBriefInfo.m_bTop = RoomTop();

    for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; ++i)
    {
        if (m_szDancer[i] != NULL && m_szDancer[i]->AttrCOM() != NULL)
        {
            rBriefInfo.m_szRoleSex[i] = m_szDancer[i]->AttrCOM()->GetSex();
        }
    }
}


void CRoom::EncodeLiveBriefInfo(LiveRoomBriefInfo& rBriefInfo) const
{
    rBriefInfo.m_nRoomID = m_nRoomID;
    rBriefInfo.m_strRoomName = m_strRoomName;
    rBriefInfo.m_chState = m_eState;
    rBriefInfo.m_chType = m_eRoomType;
    rBriefInfo.m_nAudienceCount = (unsigned short)m_nAudienceCount;
    rBriefInfo.m_strLiveRoomDesc = m_strLiveRoomDescription;

    if (m_eRoomType == eRoomType_LiveRoomPlayer)
    {
        for (int i = 0; i < m_nDancerCapacity; ++i)
        {
            rBriefInfo.m_listPlayerName.push_back(m_szLiveRoomDancerName[i]);
        }
    }
    else
    {
        for (int i = 0; i < m_nDancerCapacity; ++i)
        {
            rBriefInfo.m_listPlayerName.push_back(m_szLiveRoomGroupName[i]);
        }
    }
}

void CRoom::EncodeWholeInfo(RoomWholeInfo& rWholeInfo, bool bHost) const
{
    rWholeInfo.m_nRoomID = m_nRoomID;
    rWholeInfo.m_strRoomName = m_strRoomName;
    rWholeInfo.m_strRoomPwd = (bHost ? m_strRoomPwd : "");
    rWholeInfo.m_bHasPwd = !m_strRoomPwd.empty();
    rWholeInfo.m_bIsHost = bHost;
    rWholeInfo.m_chScene = (char)m_nChoosedScene;
    rWholeInfo.m_nMusic = (short)m_nChoosedMusicID;
    rWholeInfo.m_chMode = (char)m_nChoosedMusicMode;
    rWholeInfo.m_chLevel = (char)m_nChoosedMusicLevel;
    rWholeInfo.m_eTeamMode = m_eRoomTeamMode;
    rWholeInfo.m_chRoomColor = (char)RoomColor();

    for (int i = 0; i < m_nDancerCapacity; ++i)
    {
        rWholeInfo.m_szPosDancer[i] = (char)m_szPosDancer[i];

        if (m_szDancer[i] != NULL)
        {
            RoomPlayerInfo playerInfo;
            m_szDancer[i]->EncodePlayerInfo(playerInfo);

            rWholeInfo.m_lstDancer.push_back(playerInfo);
        }
    }

    for (int i = 0; i < m_nAudienceCapacity; ++i)
    {
        rWholeInfo.m_szPosAudience[i] = (char)m_szPosAudience[i];

        if (m_szAudience[i] != NULL)
        {
            RoomPlayerInfo playerInfo;
            m_szAudience[i]->EncodePlayerInfo(playerInfo);

            rWholeInfo.m_lstAudience.push_back(playerInfo);
        }
    }
}


void CRoom::EncodeLiveWholeInfo(LiveRoomWholeInfo& rWholeInfo) const
{
    rWholeInfo.m_nRoomID = m_nRoomID;
    rWholeInfo.m_strRoomName = m_strRoomName;
    rWholeInfo.m_chRoomType = (char)m_eRoomType;
    rWholeInfo.m_chState = (char)m_eState;
    rWholeInfo.m_chScene = (char)m_nChoosedScene;
    rWholeInfo.m_nMusic = (short)m_nChoosedMusicID;
    rWholeInfo.m_chMode = (char)m_nChoosedMusicMode;
    rWholeInfo.m_chLevel = (char)m_nChoosedMusicLevel;
    rWholeInfo.m_bVote = m_bOpenVote;
    rWholeInfo.m_bDefaultCloth = m_bLiveRoomUseDefaultCloth;
    rWholeInfo.m_strLiveRoomDesc = m_strLiveRoomDescription;

    for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; ++i)
    {
        if (m_szDancer[i] != NULL)
        {
            RoomPlayerInfo playerInfo;
            m_szDancer[i]->EncodePlayerInfo(playerInfo);

            rWholeInfo.m_listDancer.push_back(playerInfo);
        }
    }
    for (int i = 0; i < m_nGMCapacity; ++i)
    {
        if (m_szGM[i] != NULL)
        {
            RoomPlayerInfo playerInfo;
            m_szGM[i]->EncodePlayerInfo(playerInfo);

            rWholeInfo.m_listGM.push_back(playerInfo);
        }
    }

    for (std::map<unsigned int, CRoomComponent*>::const_iterator it = m_mapLiveRoomAudience.begin();
        it != m_mapLiveRoomAudience.end(); ++it)
    {
        if (it->second != NULL)
        {
            LiveRoomAudienceInfo playerInfo;
            it->second->EncodeAudienceInfo(playerInfo);

            rWholeInfo.m_listAudience.push_back(playerInfo);
        }
    }
}
void CRoom::EncodeLivePlayerInfo(LiveRoomPlayerInfo& rInfo, CRoomComponent* pRoleRoomCOM) const
{
    rInfo.m_chRoomRoleType = (char)pRoleRoomCOM->RoleRoomType();
    for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; ++i)
    {
        if (m_szDancer[i] != NULL)
        {
            RoomPlayerInfo playerInfo;
            m_szDancer[i]->EncodePlayerInfo(playerInfo);

            rInfo.m_listDancer.push_back(playerInfo);
        }
    }

    rInfo.m_bSendAudience = pRoleRoomCOM->AttrCOM()->IsGM();
    if (rInfo.m_bSendAudience)
    {
        for (std::map<unsigned int, CRoomComponent*>::const_iterator it = m_mapLiveRoomAudience.begin();
            it != m_mapLiveRoomAudience.end(); ++it)
        {
            if (it->second != NULL)
            {
                LiveRoomAudienceInfo playerInfo;
                it->second->EncodeAudienceInfo(playerInfo);

                rInfo.m_listAudience.push_back(playerInfo);
            }
        }
    }
}

void CRoom::EncodeWholeRoomTeamInfo(CWholeDanceRoomTeamInfo &rWholeInfo) const
{
    rWholeInfo.m_eRoomTeamMode = m_eRoomTeamMode;
    for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; ++i) {
        if (m_szDancer[i] != NULL) {
            if (m_szDancer[i]->AttrCOM() != NULL) {
                CRoleDanceRoomTeamInfo roleTeamInfo;
                roleTeamInfo.m_nRoleID = m_szDancer[i]->AttrCOM()->GetRoleID();
                roleTeamInfo.m_eRoomTeamMode = m_eRoomTeamMode;
                roleTeamInfo.m_eRoomColorTeam = m_szDancer[i]->ColorTeam();
                rWholeInfo.m_listRoleDanceRoomTeamInfo.push_back(roleTeamInfo);
            }
        }
    }
}

void CRoom::SendMsgToAll(GameMsg_Base* pMsg, CRoomComponent* pExceptRoomCOM)
{
    if (IsNormalRoom() || IsAuditionRoom())
    {
        if (pExceptRoomCOM == NULL)
        {
            for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; ++i)
            {
                if (m_szDancer[i] != NULL)
                {
                    m_szDancer[i]->SendPlayerMsg(pMsg);
                }
            }

            for (int i = 0; i < MAX_ROOM_AUDIENCE_COUNT; ++i)
            {
                if (m_szAudience[i] != NULL)
                {
                    m_szAudience[i]->SendPlayerMsg(pMsg);
                }
            }
        }
        else
        {
            for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; ++i)
            {
                if (m_szDancer[i] != NULL && m_szDancer[i] != pExceptRoomCOM)
                {
                    m_szDancer[i]->SendPlayerMsg(pMsg);
                }
            }

            for (int i = 0; i < MAX_ROOM_AUDIENCE_COUNT; ++i)
            {
                if (m_szAudience[i] != NULL && m_szAudience[i] != pExceptRoomCOM)
                {
                    m_szAudience[i]->SendPlayerMsg(pMsg);
                }
            }
        }
    }
    else if (IsLiveRoom())
    {
        if (pExceptRoomCOM == NULL)
        {
            for (int i = 0; i < m_nDancerCapacity; ++i)
            {
                if (m_szDancer[i] != NULL)
                {
                    m_szDancer[i]->SendPlayerMsg(pMsg);
                }
            }

            for (int i = 0; i < m_nGMCapacity; ++i)
            {
                if (m_szGM[i] != NULL)
                {
                    m_szGM[i]->SendPlayerMsg(pMsg);
                }
            }

            for (std::map<unsigned int, CRoomComponent*>::iterator it = m_mapLiveRoomAudience.begin();
                it != m_mapLiveRoomAudience.end(); ++it)
            {
                if (it->second != NULL)
                {
                    it->second->SendPlayerMsg(pMsg);
                }
            }
        }
        else
        {
            for (int i = 0; i < m_nDancerCapacity; ++i)
            {
                if (m_szDancer[i] != NULL && m_szDancer[i] != pExceptRoomCOM)
                {
                    m_szDancer[i]->SendPlayerMsg(pMsg);
                }
            }

            for (int i = 0; i < m_nGMCapacity; ++i)
            {
                if (m_szGM[i] != NULL && m_szGM[i] != pExceptRoomCOM)
                {
                    m_szGM[i]->SendPlayerMsg(pMsg);
                }
            }

            for (std::map<unsigned int, CRoomComponent*>::iterator it = m_mapLiveRoomAudience.begin();
                it != m_mapLiveRoomAudience.end(); ++it)
            {
                if (it->second != NULL && it->second != pExceptRoomCOM)
                {
                    it->second->SendPlayerMsg(pMsg);
                }
            }
        }
    }
}

void CRoom::SendMsgToAllDancer(GameMsg_Base* pMsg, CRoomComponent* pExceptRoomCOM)
{
    if (pExceptRoomCOM == NULL)
    {
        for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; ++i)
        {
            if (m_szDancer[i] != NULL)
            {
                m_szDancer[i]->SendPlayerMsg(pMsg);
            }
        }
    }
    else
    {
        for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; ++i)
        {
            if (m_szDancer[i] != NULL && m_szDancer[i] != pExceptRoomCOM)
            {
                m_szDancer[i]->SendPlayerMsg(pMsg);
            }
        }
    }
}

void CRoom::SendMsgToGM(GameMsg_Base* pMsg, CRoomComponent* pExceptRoomCOM)
{
    for (int i = 0; i < m_nGMCapacity; ++i)
    {
        if (m_szGM[i] != NULL && m_szGM[i] != pExceptRoomCOM)
        {
            m_szGM[i]->SendPlayerMsg(pMsg);
        }
    }
}

int CRoom::RoomColor() const
{
    if (m_pHostRoomCOM != NULL && m_eRoomType == eRoomType_Common)
    {
        return	m_pHostRoomCOM->GetRoomColor();
    }

    return 0;
}

bool CRoom::RoomTop() const
{
    if (m_pHostRoomCOM != NULL && m_eRoomType == eRoomType_Common)
    {
        return m_pHostRoomCOM->GetRoomTop();
    }

    return false;
}

bool CRoom::LiveRoomDefaultCloth() const
{
    return m_bLiveRoomUseDefaultCloth;
}

void CRoom::SetDefaultCloth(bool bDefaultCloth)
{
    m_bLiveRoomUseDefaultCloth = bDefaultCloth;
}

ERoomType CRoom::RoomType()
{
    return m_eRoomType;
}

void CRoom::SetRoomTeamMode(EDanceRoomTeamMode eRoomTeamMode)
{
    m_eRoomTeamMode = eRoomTeamMode;
}

EDanceRoomTeamMode CRoom::TeamMode() const
{
    return m_eRoomTeamMode;
}

int CRoom::StartRoomDancersCount()
{
    return m_nStartRoomDancersCount;
}

int CRoom::StartTeamsCount() const
{
    return m_nStartTeamsCount;
}

void CRoom::SwitchTeamMode(EDanceRoomTeamMode eTeamMode, EDanceColorTeam eCHostColorTeam)
{

    if (eTeamMode >= EDanceRoomTeamMode_None && eTeamMode <= EDanceRoomTeamMode_Team &&
        eCHostColorTeam >= EDanceColorTeam_None && eCHostColorTeam < EDanceColorTeam_Max) {
        if (m_pHostRoomCOM != NULL) {
            int nHostPos = m_pHostRoomCOM->RoleRoomPos();
            int nCount = DancerCount();
            m_eRoomTeamMode = eTeamMode;
            int nHostSide = nHostPos % 2;
            for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; ++i) {
                if (m_szDancer[i] != NULL) {
                    if (eTeamMode == EDanceRoomTeamMode_None) {
                        m_szDancer[i]->SetColorTeam(EDanceColorTeam_None);
                    }
                    else if (eTeamMode == EDanceRoomTeamMode_Team) {
                        if (eCHostColorTeam > EDanceColorTeam_None && eCHostColorTeam < EDanceColorTeam_Max) {
                            EDanceColorTeam eAnotherColor = (eCHostColorTeam == EDanceColorTeam_Red) ? EDanceColorTeam_Blue : EDanceColorTeam_Red;
                            if (nCount == 2) {
                                if (i != nHostPos) {
                                    m_szDancer[i]->SetColorTeam(eAnotherColor);
                                }
                                else {
                                    m_szDancer[i]->SetColorTeam(eCHostColorTeam);
                                }
                            }
                            else {
                                EDanceColorTeam eColorTeam = (i % 2) == nHostSide ? eCHostColorTeam : eAnotherColor;
                                m_szDancer[i]->SetColorTeam(eColorTeam);
                            }
                        }
                    }
                }
            }
        }
    }
}

void CRoom::PlayerJoinTeam(CRoomComponent& rRoleRoomCOM, EDanceColorTeam eTeamColor)
{
    if (eTeamColor > EDanceColorTeam_None && eTeamColor < EDanceColorTeam_Max) {
        rRoleRoomCOM.JoionTeam(eTeamColor);
    }
}

void CRoom::PlayerExitTeam(CRoomComponent& rRoleRoomCOM)
{
    if (rRoleRoomCOM.ColorTeam() > EDanceColorTeam_None && rRoleRoomCOM.ColorTeam() < EDanceColorTeam_Max) {
        rRoleRoomCOM.ExitTeam();
    }
}

CDanceTeam * CRoom::GetDanceTeam(EDanceColorTeam eColorTeam)
{
    if (eColorTeam > EDanceColorTeam_None && eColorTeam < EDanceColorTeam_Max) {
        return &m_szDanceTeam[eColorTeam];
    }

    return NULL;
}

int CRoom::TeamMemberCount(EDanceColorTeam eColorTeam) const
{
    int nCount = 0;
    if (eColorTeam > EDanceColorTeam_None && eColorTeam < EDanceColorTeam_Max) {
        for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; ++i) {
            if (m_szDancer[i] != NULL) {
                if (m_szDancer[i]->ColorTeam() == eColorTeam) {
                    ++nCount;
                }
            }
        }
    }

    return nCount;
}

EDanceColorTeam CRoom::GetAutoSelectedColorTeam()
{
    int nCount = MAX_ROOM_PLAYER_COUNT;
    EDanceColorTeam eColorTeam = EDanceColorTeam_Red;
    for (int i = EDanceColorTeam_Red; i < EDanceColorTeam_Max; ++i) {
        if (TeamMemberCount((EDanceColorTeam)i) < nCount) {
            nCount = TeamMemberCount((EDanceColorTeam)i);
            eColorTeam = (EDanceColorTeam)i;
        }
    }

    return eColorTeam;
}

bool CRoom::IsTeamMatched()
{
    int nCount = 0;
    for (int i = EDanceColorTeam_Red; i < EDanceColorTeam_Max; ++i) {
        nCount = TeamMemberCount((EDanceColorTeam)i) - nCount;
    }

    return (nCount == 0);
}

void CRoom::AddTeamMarkInfo(EDanceColorTeam eColorTeam, int nMark)
{
    if (eColorTeam > EDanceColorTeam_None && eColorTeam < EDanceColorTeam_Max) {
        m_szDanceTeam[eColorTeam].AddTeamMark(nMark);
    }
}

int CRoom::TotalTeamMark(EDanceColorTeam eColorTeam)
{
    if (eColorTeam > EDanceColorTeam_None && eColorTeam < EDanceColorTeam_Max) {
        return m_szDanceTeam[eColorTeam].TotalTeamMark();
    }

    return 0;
}

void CRoom::ResetTeamMark()
{
    for (int i = EDanceColorTeam_None; i < EDanceColorTeam_Max; ++i) {
        m_szDanceTeam[i].Reset();
    }
}

int CRoom::TeamExp(EDanceColorTeam eColorTeam) const
{
    if (eColorTeam > EDanceColorTeam_None && eColorTeam < EDanceColorTeam_Max) {
        return m_szDanceTeam[eColorTeam].TeamExp();
    }

    return 0;
}

int CRoom::TryEnterLiveRoomRoleType(CRoomComponent& rRoleRoomCOM)
{
    for (int i = 0; i < m_nDancerCapacity; i++)
    {
        if (m_szLiveRoomDancerID[i] == rRoleRoomCOM.AttrCOM()->GetRoleID())
        {
            return RoleRoomType_Dancer;
        }
    }

    if (m_nGMCount < m_nGMCapacity && rRoleRoomCOM.AttrCOM()->IsGM())
    {
        return RoleRoomType_GM;
    }

    if (m_mapLiveRoomAudience.size() < (size_t)m_nAudienceCapacity || rRoleRoomCOM.AttrCOM()->IsGM())
    {
        return RoleRoomType_Audience;
    }

    return RoleRoomType_None;
}

std::map<unsigned int, CRoomComponent*> & CRoom::LiveRoomAudicenceMap()
{
    return m_mapLiveRoomAudience;
}


void CRoom::AddDancerScore(unsigned int nTime, CLiveRoomRoundDancerScore &score)
{
    m_mapLiveDanceScore.insert(std::make_pair(nTime, score));
}

const std::map<unsigned int, CLiveRoomRoundDancerScore> & CRoom::GetDancerScore() const
{
    return m_mapLiveDanceScore;
}

CRoomComponent* CRoom::_GetDancerRoomCOM(unsigned int nRoleID)
{
    for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; ++i)
    {
        if (m_szDancer[i] != NULL && m_szDancer[i]->AttrCOM() != NULL && m_szDancer[i]->AttrCOM()->GetRoleID() == nRoleID)
        {
            return m_szDancer[i];
        }
    }

    return NULL;
}

CRoomComponent* CRoom::_GetAudienceRoomCOM(unsigned int nRoleID)
{
    for (int i = 0; i < MAX_ROOM_AUDIENCE_COUNT; ++i)
    {
        if (m_szAudience[i] != NULL && m_szAudience[i]->AttrCOM() != NULL && m_szAudience[i]->AttrCOM()->GetRoleID() == nRoleID)
        {
            return m_szAudience[i];
        }
    }

    return NULL;
}

CRoomComponent* CRoom::_GetRoleRoomCOM(unsigned int nRoleID)
{
    CRoomComponent* pRoleRoomCOM = _GetDancerRoomCOM(nRoleID);
    if (pRoleRoomCOM == NULL)
    {
        pRoleRoomCOM = _GetAudienceRoomCOM(nRoleID);
    }

    return pRoleRoomCOM;
}

bool CRoom::_DancerSettleIn(CRoomComponent& rRoleRoomCOM, int nDancerPos)
{
    if (nDancerPos >= 0 && nDancerPos < MAX_ROOM_PLAYER_COUNT && m_szPosDancer[nDancerPos] == RoomPos_Open && m_szDancer[nDancerPos] == NULL)
    {
        SetDancer(nDancerPos, &rRoleRoomCOM);
        ++m_nDancerCount;

        return true;
    }

    return false;
}

bool CRoom::_AudienceSettleIn(CRoomComponent& rRoleRoomCOM, int nAudiencePos)
{
    if (IsNormalRoom() || IsAuditionRoom())
    {
        if (nAudiencePos >= 0 && nAudiencePos < MAX_ROOM_AUDIENCE_COUNT && m_szPosAudience[nAudiencePos] == RoomPos_Open && m_szAudience[nAudiencePos] == NULL)
        {
            m_szAudience[nAudiencePos] = &rRoleRoomCOM;
            ++m_nAudienceCount;

            return true;
        }
    }
    else if (IsLiveRoom())
    {
        if (m_mapLiveRoomAudience.size() < (size_t)m_nAudienceCapacity || rRoleRoomCOM.AttrCOM()->IsGM())
        {
            if (nAudiencePos >= 0)
            {
                m_mapLiveRoomAudience[rRoleRoomCOM.AttrCOM()->GetRoleID()] = &rRoleRoomCOM;
                ++m_nAudienceCount;
                return true;
            }
        }
    }

    return false;
}

bool CRoom::_GMSettleIn(CRoomComponent& rRoleRoomCOM, int nPos)
{
    if (nPos >= 0 && nPos < m_nGMCapacity && m_szPosGM[nPos] == RoomPos_Open && m_szGM[nPos] == NULL)
    {
        m_szGM[nPos] = &rRoleRoomCOM;
        ++m_nGMCount;

        return true;
    }

    return false;
}

CRoomComponent* CRoom::_DancerMoveOut(int nDancerPos)
{
    CRoomComponent* pDancer = NULL;

    if (nDancerPos >= 0 && nDancerPos < MAX_ROOM_PLAYER_COUNT)
    {
        if (m_pHostRoomCOM == m_szDancer[nDancerPos])
        {
            m_pHostRoomCOM = NULL;
        }

        pDancer = m_szDancer[nDancerPos];

        if (pDancer != NULL)
        {
            if (m_nPlayMusicMode == MusicMode_Sweethearts)
            {
                unsigned int nRoleID = pDancer->AttrCOM()->GetRoleID();
                if (m_eState == eRoomState_Playing)
                {//玩家掉线时将对应分数计入情侣组合总分
                    _SaveSweetheartsInfoOnPlayerLogout(nRoleID);
                }//选择搭档前或选择搭档时掉线，则删除对应的选择关系
                if (m_mapSelectedPartner.empty())
                {
                    m_mapSelectingPartner.erase(nRoleID);
                    for (SelectingMap::iterator it = m_mapSelectingPartner.begin();
                        it != m_mapSelectingPartner.end(); ++it)
                    {
                        if (it->second == nRoleID)
                        {
                            it->second = 0;
                        }
                    }
                }
            }
            SetDancer(nDancerPos, NULL);
            --m_nDancerCount;
        }
    }

    return pDancer;
}

CRoomComponent* CRoom::_AudienceMoveOut(int nAudiencePos)
{
    CRoomComponent* pAudience = NULL;

    if (IsNormalRoom() || IsAuditionRoom())
    {
        if (nAudiencePos >= 0 && nAudiencePos < MAX_ROOM_AUDIENCE_COUNT)
        {
            if (m_pHostRoomCOM == m_szAudience[nAudiencePos])
            {
                m_pHostRoomCOM = NULL;
            }

            pAudience = m_szAudience[nAudiencePos];

            m_szAudience[nAudiencePos] = NULL;
            --m_nAudienceCount;
        }
    }
    else if (IsLiveRoom())
    {
        unsigned int nRoleID = nAudiencePos;
        std::map<unsigned int, CRoomComponent*>::iterator it = m_mapLiveRoomAudience.find(nRoleID);
        if (it != m_mapLiveRoomAudience.end())
        {
            pAudience = it->second;
            m_mapLiveRoomAudience.erase(it);
            --m_nAudienceCount;
        }
    }

    return pAudience;
}

CRoomComponent* CRoom::_GMMoveOut(int nGMPos)
{
    CRoomComponent* pGM = NULL;

    if (nGMPos >= 0 && nGMPos < MAX_ROOM_AUDIENCE_COUNT)
    {
        if (m_pHostRoomCOM == m_szGM[nGMPos])
        {
            m_pHostRoomCOM = NULL;
        }

        pGM = m_szGM[nGMPos];

        m_szGM[nGMPos] = NULL;
        --m_nGMCount;
    }

    return pGM;
}

bool CRoom::_ChangeToDancer(CRoomComponent& rRoleRoomCOM, int nDancerPos)
{
    if (nDancerPos >= 0 && nDancerPos < MAX_ROOM_PLAYER_COUNT && m_szPosDancer[nDancerPos] == RoomPos_Open && m_szDancer[nDancerPos] == NULL)
    {
        if (IsNormalRoom() || IsAuditionRoom())
        {
            if (rRoleRoomCOM.RoleRoomType() == RoleRoomType_Audience && m_szAudience[rRoleRoomCOM.RoleRoomPos()] == &rRoleRoomCOM)
            {
                m_szAudience[rRoleRoomCOM.RoleRoomPos()] = NULL;
                --m_nAudienceCount;
            }
        }
        else if (IsLiveRoom())
        {
            if (rRoleRoomCOM.RoleRoomType() == RoleRoomType_Audience)
            {
                std::map<unsigned int, CRoomComponent*>::iterator it = m_mapLiveRoomAudience.find(rRoleRoomCOM.AttrCOM()->GetRoleID());
                if (it != m_mapLiveRoomAudience.end())
                {
                    m_mapLiveRoomAudience.erase(it);
                    --m_nAudienceCount;
                }
            }
            else if (rRoleRoomCOM.RoleRoomType() == RoleRoomType_GM && m_szGM[rRoleRoomCOM.RoleRoomPos()] == &rRoleRoomCOM)
            {
                m_szGM[rRoleRoomCOM.RoleRoomPos()] = NULL;
                --m_nGMCount;
            }
        }
        else
        {
            return false;
        }

        SetDancer(nDancerPos, &rRoleRoomCOM);
        ++m_nDancerCount;

        return true;
    }

    return false;
}

bool CRoom::_ChangeToAudience(CRoomComponent& rRoleRoomCOM, int nAudiencePos)
{
    if (IsNormalRoom() || IsAuditionRoom())
    {
        if (nAudiencePos >= 0 && nAudiencePos < MAX_ROOM_AUDIENCE_COUNT && m_szPosAudience[nAudiencePos] == RoomPos_Open && m_szAudience[nAudiencePos] == NULL)
        {
            if (rRoleRoomCOM.RoleRoomType() == RoleRoomType_Dancer && m_szDancer[rRoleRoomCOM.RoleRoomPos()] == &rRoleRoomCOM)
            {
                SetDancer(rRoleRoomCOM.RoleRoomPos(), NULL);
                --m_nDancerCount;

                m_szAudience[nAudiencePos] = &rRoleRoomCOM;
                ++m_nAudienceCount;
            }
        }
    }
    else if (IsLiveRoom())
    {
        SetDancer(rRoleRoomCOM.RoleRoomPos(), NULL);
        --m_nDancerCount;

        m_mapLiveRoomAudience[rRoleRoomCOM.AttrCOM()->GetRoleID()] = &rRoleRoomCOM;
        ++m_nAudienceCount;

        return true;
    }

    return false;
}

bool CRoom::_ChangePosDancerState(int nDancerPos, int nPosState)
{
    if (nDancerPos >= 0 && nDancerPos < MAX_ROOM_PLAYER_COUNT && m_szPosDancer[nDancerPos] != nPosState)
    {
        m_szPosDancer[nDancerPos] = nPosState;

        /*if ( nPosState == RoomPos_Open )
        {
        ++m_nDancerCapacity;
        }
        else
        {
        --m_nDancerCapacity;
        }*/

        return true;
    }

    return false;
}

bool CRoom::_ChangePosAudienceState(int nAudiencePos, int nPosState)
{
    if (IsNormalRoom() || IsAuditionRoom())
    {
        if (nAudiencePos >= 0 && nAudiencePos < m_nAudienceCapacity && m_szPosAudience[nAudiencePos] != nPosState)
        {
            m_szPosAudience[nAudiencePos] = nPosState;

            /*if ( nPosState == RoomPos_Open )
            {
            ++m_nAudienceCapacity;
            }
            else
            {
            --m_nAudienceCapacity;
            }*/

            return true;
        }
    }
    else if (IsLiveRoom())
    {
        return true;
    }

    return false;
}

bool CRoom::_ChangePosGMState(int nPos, int nState)
{
    if (nPos >= 0 && nPos < m_nGMCapacity && m_szPosGM[nPos] != nState)
    {
        m_szPosGM[nPos] = nState;
        return true;
    }

    return false;
}

int CRoom::_GetIdleDancerPos()
{
    if (m_nDancerCount < m_nDancerCapacity)
    {
        for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; ++i)
        {
            if (m_szPosDancer[i] == RoomPos_Open && m_szDancer[i] == NULL)
            {
                return i;
            }
        }
    }

    return -1;
}

int CRoom::_GetIdleAudiencePos()
{
    if (IsNormalRoom() || IsAuditionRoom())
    {
        if (m_nAudienceCount < m_nAudienceCapacity)
        {
            for (int i = 0; i < MAX_ROOM_AUDIENCE_COUNT; ++i)
            {
                if (m_szPosAudience[i] == RoomPos_Open && m_szAudience[i] == NULL)
                {
                    return i;
                }
            }
        }
        return -1;
    }
    else if (IsLiveRoom())
    {
        if (m_nAudienceCount < m_nAudienceCapacity)
        {
            return 0;
        }
        return -1;
    }

    return -1;
}

const SelectedMap& CRoom::GetSelectedMap()
{
    return m_mapSelectedPartner;
}
int CRoom::_GetIdleGMPos()
{
    if (m_nGMCount < m_nGMCapacity)
    {
        for (int i = 0; i < MAX_ROOM_GM_COUNT; ++i)
        {
            if (m_szPosGM[i] == RoomPos_Open && m_szGM[i] == NULL)
            {
                return i;
            }
        }
    }

    return -1;
}
void CRoom::EndToSelectPartner()
{
    unsigned char nGroupID = 1;
    //先判断互选情况
    set<set<unsigned int> > setSelectEachOtherPair;
    for (SelectingMap::iterator it1 = m_mapSelectingPartner.begin();
        it1 != m_mapSelectingPartner.end(); ++it1)
    {
        for (SelectingMap::iterator it2 = m_mapSelectingPartner.begin();
            it2 != m_mapSelectingPartner.end(); ++it2)
        {
            if (it1->first == it2->second && it1->second == it2->first)
            {
                set<unsigned int> tmp;
                tmp.insert(it1->first);
                tmp.insert(it1->second);
                setSelectEachOtherPair.insert(tmp);
            }
        }
    }
    for (set<set<unsigned int> >::iterator it1 = setSelectEachOtherPair.begin();
        it1 != setSelectEachOtherPair.end(); ++it1)
    {
        SelectedMap::mapped_type& val = m_mapSelectedPartner[nGroupID];
        val.m_bSelectEachOther = true;
        for (set<unsigned int>::iterator it2 = it1->begin();
            it2 != it1->end(); ++it2)
        {
            val.m_vecRoleId.push_back(*it2);
            m_mapSelectingPartner.erase(*it2);
        }
        ++nGroupID;
    }
    if (setSelectEachOtherPair.size() == 1)//如果有一对是互选，那么剩下的人就是一组
    {
        SelectedMap::mapped_type& val = m_mapSelectedPartner[nGroupID];
        val.m_bSelectEachOther = false;
        while (!m_mapSelectingPartner.empty())
        {
            SelectingMap::iterator it = m_mapSelectingPartner.begin();
            val.m_vecRoleId.push_back(it->first);
            m_mapSelectingPartner.erase(it);
        }
        ++nGroupID;
    }
    //无互选情况
    if (!m_mapSelectingPartner.empty())
    {
        std::set<unsigned int> setSexDancer[ESexType_Max];
        for (int i = 0; i < ESexType_Max; ++i)
        {
            setSexDancer[i] = m_setSexDancer[i];
        }
        while (!m_mapSelectingPartner.empty())
        {
            SelectingMap::iterator it = m_mapSelectingPartner.begin();
            //WriteLog("m_mapSelectingPartner elem(%u, %u)", it->first, it->second);
            bool bMatched = false;
            if (it->second != 0)
            {
                SelectingMap::iterator rit = m_mapSelectingPartner.find(it->second);
                if (rit != m_mapSelectingPartner.end())
                {
                    SelectedMap::mapped_type& val = m_mapSelectedPartner[nGroupID];
                    //优先匹配互选玩家
                    if (rit->second == it->first)
                    {
                        val.m_bSelectEachOther = true;
                        bMatched = true;
                    }//系统辅助搭配
                    else if (rit->second == 0)
                    {
                        val.m_bSelectEachOther = false;
                        bMatched = true;
                    }
                    if (bMatched)
                    {
                        val.m_vecRoleId.push_back(it->first);
                        val.m_vecRoleId.push_back(it->second);
                        //WriteLog("xindong 1 EndToSelectPartner nGroupID=%u, %u<=>%u", nGroupID, val.m_vecRoleId[0], val.m_vecRoleId[1]);
                    }
                }
            }
            else//系统辅助搭配
            {
                unsigned int nRoleID = 0;
                unsigned int nCount = 0;
                std::vector<unsigned int> vecSelector;
                for (SelectingMap::iterator tmpit = m_mapSelectingPartner.begin(); tmpit != m_mapSelectingPartner.end(); ++tmpit)
                {
                    if (tmpit->second == it->first)
                    {
                        nRoleID = tmpit->first;
                        ++nCount;
                    }
                }
                if (nCount == 1 && nRoleID != 0)
                {
                    SelectedMap::mapped_type& val = m_mapSelectedPartner[nGroupID];
                    val.m_vecRoleId.push_back(it->first);
                    val.m_vecRoleId.push_back(nRoleID);
                    val.m_bSelectEachOther = false;
                    bMatched = true;
                    //WriteLog("xindong 2 EndToSelectPartner nGroupID=%u, %u<=>%u", nGroupID, val.m_vecRoleId[0], val.m_vecRoleId[1]);
                }
            }
            //还不匹配就随机
            if (!bMatched)
            {
                int nOppSex = ESexType_Male + ESexType_Female - m_mapDancer2Sex[it->first];
                SelectedMap::mapped_type& val = m_mapSelectedPartner[nGroupID];
                val.m_vecRoleId.push_back(it->first);
                if (!setSexDancer[nOppSex].empty())
                {
                    int rd = rand() % setSexDancer[nOppSex].size();
                    std::set<unsigned int>::iterator iit = setSexDancer[nOppSex].begin();
                    std::advance(iit, rd);
                    val.m_vecRoleId.push_back(*iit);
                }
                val.m_bSelectEachOther = false;
                //WriteLog("xindong 3 EndToSelectPartner nGroupID=%u, %u<=>%u", nGroupID, val.m_vecRoleId[0], val.m_vecRoleId[val.m_vecRoleId.size()-1]);
                bMatched = true;
            }
            if (bMatched)
            {
                SelectedMap::mapped_type& val = m_mapSelectedPartner[nGroupID];
                for (std::vector<unsigned int>::iterator vit = val.m_vecRoleId.begin();
                    vit != val.m_vecRoleId.end(); ++vit)
                {
                    m_mapSelectingPartner.erase(*vit);
                    setSexDancer[m_mapDancer2Sex[*vit]].erase(*vit);
                }
                ++nGroupID;
            }
        }
    }
    for (SelectedMap::iterator it = m_mapSelectedPartner.begin(); it != m_mapSelectedPartner.end(); ++it)
    {
        size_t nSize = it->second.m_vecRoleId.size();
        for (size_t i = 0; i < nSize; ++i)
        {
            unsigned int nRoleID = it->second.m_vecRoleId[i];
            CRoomComponent* pRoomComponent = _GetDancerRoomCOM(nRoleID);
            if (pRoomComponent)
            {
                pRoomComponent->RoleMarkInfo().SetBeckoningGroupId(it->first);
            }
        }
    }
    for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; ++i)
    {
        if (m_szDancer[i] != NULL)
        {
            m_szDancer[i]->RoleMarkInfo().ClearRoundRankMap();
        }
    }
    RemoveFrontRoundRange();
    SetState(eRoomState_Playing);
}

bool CRoom::IsLiveRoom() const
{
    return (m_eRoomType == eRoomType_LiveRoomPlayer
        || m_eRoomType == eRoomType_LiveRoomGroup
        || m_eRoomType == eRoomType_LiveRoomOther);
}
void CRoom::BeginToSelectPartner()
{
    SetState(eRoomState_SelectPartner);
    GameMsg_S2C_BeginToSelectPartner msg;
    SendMsgToAllDancer(&msg);
}

bool CRoom::IsNormalRoom() const
{
    return (m_eRoomType == eRoomType_Common
        || m_eRoomType == eRoomType_NewPlayer);
}

bool CRoom::IsAuditionRoom() const
{
    return m_eRoomType == eRoomType_LiveRoomAudition;
}

bool CRoom::IsDungeonRoom() const
{
    return m_eRoomType == eRoomType_Dungeon;
}

void CRoom::OpenVote(bool bOpen)
{
    m_bOpenVote = bOpen;
}

bool CRoom::IsOpenVote() const
{
    return m_bOpenVote;
}

void CRoom::OnUpdateDancerHot(int nRoleID, int nHot)
{
    if (nRoleID > 0 && nHot > 0)
    {
        // update hot rank
        CLiveRoomHotRank *pHotRank = m_hotRankManager.Find(nRoleID);
        if (pHotRank != NULL)
        {
            CLiveRoomHotRank newRank = *pHotRank;
            newRank.m_nScore += nHot;
            newRank.m_nUpdateTime = (unsigned int)time(NULL);
            m_hotRankManager.Update(newRank);
        }
        else
        {
            CLiveRoomHotRank rank;
            rank.m_nID = nRoleID;
            rank.m_strName = GetLiveRoomDancerName(nRoleID);
            rank.m_nScore = nHot;
            rank.m_nUpdateTime = (unsigned int)time(NULL);
            m_hotRankManager.Update(rank);
        }
    }

    // update message
    unsigned int nNow = (unsigned int)time(NULL);
    if (IsOpenVote() && nNow > m_nLastSendHotValueTime + CLiveRoomDataManager::Instance().m_nRefreshInterval)
    {
        m_nLastSendHotValueTime = nNow;

        GameMsg_S2C_DancerVoteInfo msg;
        for (int i = 0; i < m_nDancerCapacity; i++)
        {
            if (m_szDancer[i] != NULL)
            {
                LiveRoomDancerVoteInfo voteInfo;
                voteInfo.m_nRoleID = m_szDancer[i]->AttrCOM()->GetRoleID();
                voteInfo.m_nHot = 0;

                CLiveRoomHotRank *pRank = m_hotRankManager.Find(voteInfo.m_nRoleID);
                if (pRank != NULL)
                {
                    voteInfo.m_nHot = pRank->m_nScore;
                }

                msg.m_hotList.push_back(voteInfo);
            }
        }
        SendMsgToAll(&msg);
    }
}

void CRoom::OnGetHotList(CRoomComponent &rRoleRoomCOM, int nPage)
{
    static const int kHotListPageSize = 6;

    int nTotalCount = m_hotRankManager.Count();
    int nTotalPage = nTotalCount / kHotListPageSize;
    if (nTotalCount % kHotListPageSize > 0)
    {
        nTotalPage++;
    }

    if (nPage >= nTotalPage)
    {
        nPage = nTotalPage - 1;
    }
    if (nPage < 0)
    {
        nPage = 0;
    }

    int nStartIndex = nPage * kHotListPageSize;
    std::list<CLiveRoomHotRank> hotList;
    m_hotRankManager.Range(nStartIndex, kHotListPageSize, hotList);

    GameMsg_S2C_GetLiveRoomHotListRes res;
    res.m_nCurPage = (unsigned short)nPage;
    res.m_nTotalPage = (unsigned short)nTotalPage;
    for (std::list<CLiveRoomHotRank>::iterator it = hotList.begin(); it != hotList.end(); ++it)
    {
        LiveRoomHotInfo info;
        info.m_nRoleID = it->m_nID;
        info.m_strRoleName = it->m_strName;
        info.m_nPlace = ++nStartIndex;
        info.m_nHot = it->m_nScore;
        res.m_liveRoomHotList.push_back(info);
    }

    rRoleRoomCOM.SendPlayerMsg(&res);
}

int CRoom::SelectPartner(unsigned int nSelectorRoleID, unsigned int nBeSelectedRoleID, std::string& strRes)
{
    SelectingMap::iterator it = m_mapSelectingPartner.find(nSelectorRoleID);
    if (it == m_mapSelectingPartner.end())
    {
        strRes = "ESelectPartner_NotRightPartner";
        return ESelectPartner_NotRightPartner;
    }
    if (it->second != 0)
    {
        strRes = "ESelectPartner_Selected";
        return ESelectPartner_Selected;
    }
    it = m_mapSelectingPartner.find(nBeSelectedRoleID);
    if (it == m_mapSelectingPartner.end())
    {
        strRes = "ESelectPartner_NotRightPartner";
        return ESelectPartner_NotRightPartner;
    }
    int nSelectorRoleSex = m_mapDancer2Sex[nSelectorRoleID];
    int sum = 0;
    if (m_setSexDancer[nSelectorRoleSex].find(nSelectorRoleID) != m_setSexDancer[nSelectorRoleSex].end())
    {
        ++sum;
        int nOppSex = ESexType_Male + ESexType_Female - nSelectorRoleSex;
        if (m_setSexDancer[nOppSex].find(nBeSelectedRoleID) != m_setSexDancer[nOppSex].end())
        {
            ++sum;
        }
    }
    if (sum != 2)
    {
        strRes = "ESelectPartner_NotRightPartner";
        return ESelectPartner_NotRightPartner;
    }
    m_mapSelectingPartner[nSelectorRoleID] = nBeSelectedRoleID;
    //WriteLog("SelectPartner ok:nSelectorRoleID=%u, nBeSelectedRoleID=%u", nSelectorRoleID, nBeSelectedRoleID);
    strRes = "ESelectPartner_Success";
    return ESelectPartner_Success;
}

void CRoom::_CalcSweetheartsInfo()
{
    for (SelectedMap::iterator it = m_mapSelectedPartner.begin(); it != m_mapSelectedPartner.end(); ++it)
    {
        std::vector<CRoomComponent*> vecRoomComponent;
        size_t nSize = it->second.m_vecRoleId.size();
        for (size_t i = 0; i < nSize; ++i)
        {
            unsigned int nRoleID = it->second.m_vecRoleId[i];
            CRoomComponent* pRoomComponent = _GetDancerRoomCOM(nRoleID);
            if (pRoomComponent)
            {
                vecRoomComponent.push_back(pRoomComponent);
            }
        }
        it->second.m_nSweetValue = it->second.m_nAccumulatedSweetValue;
        it->second.m_nMarkValue = it->second.m_nAccumulatedMarkValue;
        //可能有的玩家已下线
        if (vecRoomComponent.size() == 1)
        {
            it->second.m_nMarkValue += vecRoomComponent[0]->RoleMarkInfo().Mark();
            vecRoomComponent[0]->RoleMarkInfo().SetBeckoningValue(it->second.m_nSweetValue);
        }
        else if (vecRoomComponent.size() == 2)
        {
            unsigned int nSweetValueAdd = _CalcSweetValue(vecRoomComponent[0]->RoleMarkInfo(), vecRoomComponent[1]->RoleMarkInfo(), true);
            it->second.m_nAccumulatedSweetValue += nSweetValueAdd;
            it->second.m_nSweetValue += nSweetValueAdd;
            it->second.m_nMarkValue = vecRoomComponent[0]->RoleMarkInfo().Mark() + vecRoomComponent[1]->RoleMarkInfo().Mark();
            vecRoomComponent[0]->RoleMarkInfo().SetBeckoningValue(it->second.m_nSweetValue);
            vecRoomComponent[1]->RoleMarkInfo().SetBeckoningValue(it->second.m_nSweetValue);
            //WriteLog("xindong _CalcSweetheartsInfo %u->%u, %u->%u", vecRoomComponent[0]->AttrCOM()->GetRoleID(), vecRoomComponent[0]->RoleMarkInfo().Mark(), 
            //	vecRoomComponent[1]->AttrCOM()->GetRoleID(), vecRoomComponent[1]->RoleMarkInfo().Mark());
            //WriteLog("xindong _CalcSweetheartsInfo Group=%d, nSweetValueAdd=%u, m_nAccumulatedSweetValue=%u, m_nSweetValue=%u, m_nMarkValue=%u", 
            //	(int)it->first, nSweetValueAdd, it->second.m_nAccumulatedSweetValue, it->second.m_nSweetValue, it->second.m_nMarkValue);
        }
    }
}

void CRoom::CalcSweetRank()
{
    __int64 nWeight = 0;
    std::map<__int64, int> mapWeight2GroupId;
    for (SelectedMap::iterator it = m_mapSelectedPartner.begin(); it != m_mapSelectedPartner.end(); ++it)
    {
        std::vector<CRoomComponent*> vecRoomComponent;
        size_t nSize = it->second.m_vecRoleId.size();
        for (size_t i = 0; i < nSize; ++i)
        {
            unsigned int nRoleID = it->second.m_vecRoleId[i];
            CRoomComponent* pRoomComponent = _GetDancerRoomCOM(nRoleID);
            if (pRoomComponent)
            {
                vecRoomComponent.push_back(pRoomComponent);
            }
        }
        nWeight = it->second.m_nSweetValue;
        nWeight = (nWeight << 32) + it->second.m_nMarkValue + vecRoomComponent.size();
        mapWeight2GroupId[nWeight] = it->first;
    }
    size_t nRank = mapWeight2GroupId.size();
    for (std::map<__int64, int>::iterator it = mapWeight2GroupId.begin();
        it != mapWeight2GroupId.end(); ++it)
    {
        m_mapSelectedPartner[(unsigned char)it->second].m_nRank = nRank;
        --nRank;
    }
}

void CRoom::_SaveSweetheartsInfoOnPlayerLogout(unsigned int nOfflineRoleID)
{
    CRoomComponent* pRoomComponentLogout = NULL;
    for (SelectedMap::iterator it = m_mapSelectedPartner.begin(); it != m_mapSelectedPartner.end(); ++it)
    {
        std::vector<CRoomComponent*> vecRoomComponent;
        size_t nSize = it->second.m_vecRoleId.size();
        for (size_t i = 0; i < nSize; ++i)
        {
            unsigned int nRoleID = it->second.m_vecRoleId[i];
            CRoomComponent* pRoomComponent = _GetDancerRoomCOM(nRoleID);
            if (pRoomComponent)
            {
                if (nRoleID == nOfflineRoleID)
                {
                    pRoomComponentLogout = pRoomComponent;
                }
                vecRoomComponent.push_back(pRoomComponent);
            }
        }
        if (pRoomComponentLogout)
        {
            it->second.m_nAccumulatedMarkValue += pRoomComponentLogout->RoleMarkInfo().Mark();
            if (vecRoomComponent.size() == 2)
            {
                unsigned int nSweetValueAdd = _CalcSweetValue(vecRoomComponent[0]->RoleMarkInfo(), vecRoomComponent[1]->RoleMarkInfo(), true);
                it->second.m_nAccumulatedSweetValue += nSweetValueAdd;
                //WriteLog("_SaveSweetheartsInfoOnPlayerLogout Group=%d, nSweetValueAdd=%u, m_nAccumulatedSweetValue=%u, m_nAccumulatedMarkValue=%u", 
                //	(int)it->first, nSweetValueAdd, it->second.m_nAccumulatedSweetValue, 
                //	it->second.m_nAccumulatedMarkValue);
            }
            break;
        }
    }
}

void CRoom::GetSweethearsInfo(std::vector<SweetheartsInfo>& vecSweetheartsInfo)
{
    _CalcSweetheartsInfo();
    vecSweetheartsInfo.resize(m_mapSelectedPartner.size());
    size_t i = 0;
    for (SelectedMap::iterator it = m_mapSelectedPartner.begin(); it != m_mapSelectedPartner.end(); ++it)
    {
        vecSweetheartsInfo[i].m_nGroupID = it->first;
        vecSweetheartsInfo[i].m_nSweetValue = it->second.m_nSweetValue;
        vecSweetheartsInfo[i].m_nMarkValue = it->second.m_nMarkValue;
        ++i;
    }
}

void CRoom::UpdateSweethearsInfo()
{
    if (!m_mapSelectedPartner.empty())
    {
        GameMsg_S2C_UpdateSweetheartsInfo msg;
        GetSweethearsInfo(msg.m_vecSweetheartsInfo);
        SendMsgToAll(&msg);
    }
}

void CRoom::AskMakeFriend()
{
    for (SelectedMap::iterator it = m_mapSelectedPartner.begin(); it != m_mapSelectedPartner.end(); ++it)
    {
        if (it->second.m_vecRoleId.size() == MAX_SWEET_GROUP_PLAYER_COUNT)
        {
            unsigned int nRoleID1 = it->second.m_vecRoleId[0];
            unsigned int nRoleID2 = it->second.m_vecRoleId[1];
            CRoleEntity* pRole1 = CPlayerManager::Instance().GetEntityByRoleID(nRoleID1);
            CRoleEntity* pRole2 = CPlayerManager::Instance().GetEntityByRoleID(nRoleID2);
            if (pRole1 && pRole2)
            {
                CRoomComponent* pRoleRoomCOM1 = pRole1->GetComponent<CRoleRoom>();
                CRoomComponent* pRoleRoomCOM2 = pRole2->GetComponent<CRoleRoom>();
                if (pRoleRoomCOM1 && pRoleRoomCOM2)
                {
                    CEntityFriendComponent* pComponent1 = pRole1->GetComponent<CRoleFriend>();
                    if (pComponent1 && !pComponent1->IsFriend(nRoleID2))
                    {
                        GameMsg_S2C_MakeFriend msg1;
                        msg1.m_nRoleID = nRoleID2;
                        msg1.m_strRoleName = pRole2->GetRoleName();
                        msg1.m_nSex = pRole2->GetSex();
                        msg1.m_bVIP = pRole2->IsVIP();
                        msg1.m_nVIPLevel = (unsigned short)pRole2->GetVIPLevel();
                        pRoleRoomCOM1->SendPlayerMsg(&msg1);

                        GameMsg_S2C_MakeFriend msg2;
                        msg2.m_nRoleID = nRoleID1;
                        msg2.m_strRoleName = pRole1->GetRoleName();
                        msg2.m_nSex = pRole1->GetSex();
                        msg2.m_bVIP = pRole1->IsVIP();
                        msg2.m_nVIPLevel = (unsigned short)pRole1->GetVIPLevel();
                        pRoleRoomCOM2->SendPlayerMsg(&msg2);
                    }
                }
            }
        }
    }
}

void CRoom::AddTimer(ITimer* pTimer)
{
    m_vecTimersOnStart.push_back(pTimer);
}

void CRoom::SetRoundRange(int nMode, int nBegin, int nEnd)
{
    RoundRange rr;
    rr.nBegin = nBegin;
    rr.nEnd = nEnd;
    rr.nMode = nMode;
    m_listRoundRange.push_back(rr);
}

void CRoom::RemoveFrontRoundRange()
{
    if (!m_listRoundRange.empty())
    {
        m_listRoundRange.erase(m_listRoundRange.begin());
    }
}

void CRoom::ClearRoundRanges()
{
    m_listRoundRange.clear();
}

int CRoom::GetMusicModeByRound(int nRound)
{
    std::list<RoundRange>::iterator it = m_listRoundRange.begin();
    for (; it != m_listRoundRange.end(); ++it)
    {
        if (nRound >= it->nBegin && nRound <= it->nEnd)
        {
            return it->nMode;
        }
    }
    return MusicMode_None;
}

unsigned int CRoom::GetSweetRankByGroupId(unsigned int nGroupId)
{
    unsigned int nRank = 0;
    SelectedMap::iterator it = m_mapSelectedPartner.find((unsigned char)nGroupId);
    if (it != m_mapSelectedPartner.end())
    {
        nRank = it->second.m_nRank;
    }
    return nRank;
}

unsigned int CRoom::_CalcSweetValue(CRoomMarkInfo& left, CRoomMarkInfo& right, bool bClear)
{
    const std::map<int, std::map<int, int> >& mapSweetValue = CSystemSettingMgr::Instance().GetBeckoningValueMap();
    unsigned int sum = 0;
    std::vector<unsigned int> vecMatchedRound;
    RoundRankMap& rrm1 = left.GetRoundRankMap();
    RoundRankMap& rrm2 = right.GetRoundRankMap();
    if (rrm1.empty() || rrm2.empty())
    {
        return 0;
    }
    for (RoundRankMap::iterator it = rrm1.begin(); it != rrm1.end(); ++it)
    {
        RoundRankMap::const_iterator iit = rrm2.find(it->first);
        if (iit != rrm2.end())
        {
            vecMatchedRound.push_back(it->first);
            int nMode = GetMusicModeByRound(it->first);
            if (nMode != MusicMode_None)
            {
                std::map<int, std::map<int, int> >::const_iterator modeIt = mapSweetValue.find(nMode);
                if (modeIt != mapSweetValue.end())
                {
                    size_t nRankSize = it->second.size();
                    if (nRankSize == iit->second.size())
                    {
                        for (size_t i = 0; i < nRankSize; ++i)
                        {
                            if (it->second[i] == iit->second[i])
                            {
                                std::map<int, int>::const_iterator valueIt = modeIt->second.find(it->second[i]);
                                if (valueIt != modeIt->second.end())
                                {
                                    sum += valueIt->second;
                                    //WriteLog("xindong _CalcSweetValue nMode=%d, nRound=%u, nRank=%u, nValue=%u",
                                    //	nMode, it->first, it->second[i], valueIt->second);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (bClear)
    {
        for (std::vector<unsigned int>::iterator it = vecMatchedRound.begin();
            it != vecMatchedRound.end(); ++it)
        {
            rrm1.erase(*it);
            rrm2.erase(*it);
        }
    }
    return sum;
}

int CRoom::GetFriendCount(unsigned int nRoleID)
{
    int nCount = 0;
    for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; i++)
    {
        CRoomComponent* pDancer = m_szDancer[i];
        if (pDancer != NULL)
        {
            CEntityFriendComponent* pPlayerFriendCom = pDancer->FriendCOM();
            if (pPlayerFriendCom != NULL && pPlayerFriendCom->IsFriend(nRoleID))
            {
                nCount++;
            }
        }
    }
    return nCount;
}


int CRoom::GetDanceGroupPlayerCount(unsigned int nDanceGroupID)
{
    if (nDanceGroupID == 0)
    {
        return 0;
    }
    int nCount = 0;
    for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; i++)
    {
        CRoomComponent* pDancer = m_szDancer[i];
        if (pDancer != NULL)
        {
            CEntityDanceGroupComponent* pGroupCom = pDancer->DanceGroupCOM();
            if (pGroupCom != NULL && pGroupCom->GetDanceGroupID() == nDanceGroupID)
            {
                nCount++;
            }
        }
    }
    return nCount;
}

bool CRoom::HasLover(unsigned int nRoldID)
{
    for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; i++)
    {
        CRoomComponent* pDancer = m_szDancer[i];
        if (pDancer != NULL)
        {
            CEntityCoupleComponent* pCoupleCom = pDancer->CoupleCOM();
            if (pCoupleCom != NULL && pCoupleCom->m_coupleInfo.m_nPairID == nRoldID)
            {
                return true;
            }
        }
    }
    return false;
}

CRoomComponent* CRoom::GetDancerByPos(int nPos)
{
    if (nPos >= 0 && nPos < MAX_ROOM_PLAYER_COUNT)
    {
        return m_szDancer[nPos];
    }
    return NULL;
}

bool CRoom::GetDancerInfoByPos(int nPos, unsigned int& nPlayerID, std::string& playerName)
{
    nPlayerID = 0;
    CRoomComponent* pDancer = GetDancerByPos(nPos);
    if (pDancer == NULL)
    {
        return false;
    }
    nPlayerID = pDancer->AttrCOM()->m_nRoleID;
    playerName = pDancer->AttrCOM()->m_szRoleName;

    return true;
}

CRoomComponent* CRoom::GetAudienceByPos(int nPos)
{
    if (nPos >= 0 && nPos < MAX_ROOM_PLAYER_COUNT)
    {
        return m_szAudience[nPos];
    }
    return NULL;
}

bool CRoom::GetAudienceInfoByPos(int nPos, unsigned int& nPlayerID, std::string& playerName)
{
    nPlayerID = 0;
    CRoomComponent* pDancer = GetAudienceByPos(nPos);
    if (pDancer == NULL)
    {
        return false;
    }
    nPlayerID = pDancer->AttrCOM()->m_nRoleID;
    playerName = pDancer->AttrCOM()->m_szRoleName;

    return true;
}

void CRoom::ToGMRoomInfo(GMRoomInfo& gmRoomInfo)
{
    gmRoomInfo.m_RoomID = (unsigned short)RoomID();
    gmRoomInfo.m_RoomName = RoomName();
    gmRoomInfo.m_RoomPassword = RoomPwd();

    GetDancerInfoByPos(0, gmRoomInfo.m_PlayerMainID, gmRoomInfo.m_PlayerMainName);

    for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; i++)
    {
        GMRoomPlayerInfo playerInfo;
        GetDancerInfoByPos(i, playerInfo.m_playerID, playerInfo.m_playerName);
        if (playerInfo.m_playerID != 0)
        {
            gmRoomInfo.AddPlayerInfo(playerInfo);
        }
        GetAudienceInfoByPos(i, playerInfo.m_playerID, playerInfo.m_playerName);
        if (playerInfo.m_playerID != 0)
        {
            gmRoomInfo.AddPlayerInfo(playerInfo);
        }
    }
}

void CRoom::AddAudiationPlayerInfo(AuditionPlayerInfo & playerinfo)
{
    m_AuditionRoleInfo.insert(std::make_pair(playerinfo.m_nRoleID, playerinfo));
}

