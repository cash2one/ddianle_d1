#include "RoomPosManager.h"
#include "../Macros/Macros.h"
#include <algorithm>

#define INVALID_DANCE_ROOM_POS  -1
#define MIN_DANCE_ROOM_POS      0

PhoneRole PhoneRole::m_invalidPhoneRole = PhoneRole(0, OS_None);

enum EDanceRoomPosState
{
    EDanceRoomPosState_Close = -1,
    EDanceRoomPosState_Idle,
    EDanceRoomPosState_Dancer
};


PhoneRole::PhoneRole()
    : m_nRoleID(0)
    , m_phone(OS_None)
{
}

PhoneRole::PhoneRole(unsigned int nRoleID, Phone_OS eos)
    : m_nRoleID(nRoleID)
    , m_phone(eos)
{
}

PhoneRole::~PhoneRole()
{

}



LWDanceRoomPosInfo::LWDanceRoomPosInfo(int nMaxCount)
    : m_vectDancer(nMaxCount, PhoneRole::m_invalidPhoneRole)
    , m_vectPosState(nMaxCount, EDanceRoomPosState_Idle)
{

}

LWDanceRoomPosInfo::~LWDanceRoomPosInfo()
{

}


int LWDanceRoomPosInfo::GetIdlePos() const
{
    return FindPos(EDanceRoomPosState_Idle);
}

int LWDanceRoomPosInfo::GetFirstDancerPos() const
{
    return FindPos(EDanceRoomPosState_Dancer);
}


bool LWDanceRoomPosInfo::AddDancer(const PhoneRole &phoneRole)
{
    int nPos = GetIdlePos();
    if (!IsValidPos(nPos))
        return false;

    m_vectPosState[nPos] = EDanceRoomPosState_Dancer;
    m_vectDancer[nPos] = phoneRole;

    return true;
}

void LWDanceRoomPosInfo::RemoveDancer(int nPos)
{
    if (!IsValidPos(nPos))
        return;

    m_vectPosState[nPos] = EDanceRoomPosState_Idle;
    m_vectDancer[nPos] = PhoneRole::m_invalidPhoneRole;

    return;
}

PhoneRole LWDanceRoomPosInfo::GetDancer(int nPos) const
{
    if (!IsValidPos(nPos))
        return PhoneRole::m_invalidPhoneRole;

    return m_vectDancer[nPos];
}

bool LWDanceRoomPosInfo::MoveDancerPos(int nOldPos, int nNewPos)
{
    if (!IsValidPos(nOldPos) || !IsValidPos(nNewPos))
        return false;

    std::swap(m_vectPosState[nOldPos], m_vectPosState[nNewPos]);
    std::swap(m_vectDancer[nOldPos], m_vectDancer[nNewPos]);

    return true;
}


bool LWDanceRoomPosInfo::ClosePos(int nPos)
{
    if (!IsValidPos(nPos))
        return true;

    m_vectPosState[nPos] = EDanceRoomPosState_Close;

    return true;
}

void LWDanceRoomPosInfo::OpenPos(int nPos)
{
    if (!IsValidPos(nPos))
        return;

    m_vectPosState[nPos] = EDanceRoomPosState_Idle;
}


bool LWDanceRoomPosInfo::IsPosClosed(int nPos) const
{
    if (!IsValidPos(nPos))
        return true;

    return (m_vectPosState[nPos] == EDanceRoomPosState_Close);
}

bool LWDanceRoomPosInfo::IsPosOpend(int nPos) const
{
    return !IsPosClosed(nPos);
}

bool LWDanceRoomPosInfo::HasFull() const
{
    for (int i = 0; i < (int)m_vectPosState.size(); ++i)
    {
        if (m_vectPosState[i] == EDanceRoomPosState_Idle)
            return false;
    }

    return true;
}

bool LWDanceRoomPosInfo::HasEmpty() const
{
    for (int i = 0; i < (int)m_vectPosState.size(); ++i)
    {
        if (m_vectPosState[i] == EDanceRoomPosState_Dancer)
            return false;
    }

    return true;
}

int LWDanceRoomPosInfo::GetRolePos(unsigned int nRoleID) const
{
    for (size_t i = 0; i < m_vectPosState.size() && i < m_vectDancer.size(); ++i)
    {
        if (m_vectPosState[i] == EDanceRoomPosState_Dancer && m_vectDancer[i].m_nRoleID == nRoleID)
            return i;
    }

    return INVALID_DANCE_ROOM_POS;
}

void LWDanceRoomPosInfo::GetAllPlayer(RoleVec &vecListner)
{
    for (size_t i = 0; i < m_vectPosState.size() && i < m_vectDancer.size(); ++i)
    {
        if (m_vectPosState[i] == EDanceRoomPosState_Dancer && m_vectDancer[i].m_nRoleID != 0)
        {
            vecListner.push_back(std::pair<unsigned int, char>(m_vectDancer[i].m_nRoleID, m_vectDancer[i].m_phone));
        }
    }
}


int LWDanceRoomPosInfo::FindPos(int nState) const
{
    std::vector<int>::const_iterator pos = std::find(m_vectPosState.begin(), m_vectPosState.end(), nState);

    if (pos != m_vectPosState.end())
        return (pos - m_vectPosState.begin());
    else
        return INVALID_DANCE_ROOM_POS;
}

bool LWDanceRoomPosInfo::IsValidPos(int nPos) const
{
    if (MIN_DANCE_ROOM_POS <= nPos && nPos < (int)m_vectPosState.size())
        return true;
    else
        return false;
}

void LWDanceRoomPosInfo::Init()
{
    Clear();
}

void LWDanceRoomPosInfo::Clear()
{
    std::fill(m_vectDancer.begin(), m_vectDancer.end(), PhoneRole::m_invalidPhoneRole);
    std::fill(m_vectPosState.begin(), m_vectPosState.end(), EDanceRoomPosState_Idle);
}


//////////////////////////////////////////////////////////////////////////
// LWRoomPosManager
LWRoomPosManager::LWRoomPosManager(unsigned int dancerMaxCount, unsigned int audienceMaxCount)
    : m_pRoomDancer(NULL)
    , m_pRoomAudience(NULL)
{
    if (dancerMaxCount > 0)
    {
        m_pRoomDancer = new LWDanceRoomPosInfo(dancerMaxCount);
    }
    if (audienceMaxCount > 0)
    {
        m_pRoomAudience = new LWDanceRoomPosInfo(audienceMaxCount);
    }
}

LWRoomPosManager::~LWRoomPosManager()
{
    SAFE_DELETE(m_pRoomDancer);
    SAFE_DELETE(m_pRoomAudience);
}

LWDanceRoomPosInfo* LWRoomPosManager::GetRoomPosInfo(ERoleRoomType eRoleType)
{
    if (eRoleType == RoleRoomType_Dancer)
        return m_pRoomDancer;
    else if (eRoleType == RoleRoomType_Audience)
        return m_pRoomAudience;

    else
        return NULL;
}

bool LWRoomPosManager::GetRolePos(unsigned int nRoleID, ERoleRoomType &eRoleType, unsigned int &pos)
{
    for (unsigned short i = RoleRoomType_None; i < RoleRoomType_Max; ++i)
    {
        LWDanceRoomPosInfo *pPosInfo = GetRoomPosInfo((ERoleRoomType)i);
        if (NULL != pPosInfo)
        {
            int tempPos = pPosInfo->GetRolePos(nRoleID);
            if (tempPos != INVALID_DANCE_ROOM_POS)
            {
                eRoleType = (ERoleRoomType)i;
                pos = tempPos;
                return true;
            }
        }
    }

    return false;
}

bool LWRoomPosManager::AddDancer(ERoleRoomType eRoleType, const PhoneRole &phoneRole)
{
    LWDanceRoomPosInfo *pPosInfo = GetRoomPosInfo(eRoleType);

    if (NULL == pPosInfo)
        return false;

    if (pPosInfo->HasFull())
        return false;

    return pPosInfo->AddDancer(phoneRole);
}


bool LWRoomPosManager::RemoveRole(unsigned int nRoleID)
{
    ERoleRoomType roleType;
    unsigned int pos;
    if (GetRolePos(nRoleID, roleType, pos))
    {
        LWDanceRoomPosInfo *pPosInfo = GetRoomPosInfo(roleType);
        if (NULL != pPosInfo)
        {
            pPosInfo->RemoveDancer(pos);
            return true;
        }
    }

    return false;
}

unsigned int LWRoomPosManager::GetFirstDanceRole()
{
    LWDanceRoomPosInfo *pPosInfo = GetRoomPosInfo(RoleRoomType_Dancer);
    if (NULL == pPosInfo)
        return 0;

    int nPos = pPosInfo->GetFirstDancerPos();

    return pPosInfo->GetDancer(nPos).m_nRoleID;
}

void LWRoomPosManager::SetPostionState(ERoleRoomType eRoleType, unsigned int nPos, ERoomPosState ePosState)
{
    LWDanceRoomPosInfo *pPosInfo = GetRoomPosInfo(eRoleType);
    if (NULL == pPosInfo)
        return;

    if (ePosState == RoomPos_Open)
        pPosInfo->OpenPos(nPos);
    else
        pPosInfo->ClosePos(nPos);

    return;
}

unsigned int LWRoomPosManager::GetPostionRole(ERoleRoomType eRoleType, unsigned int nPos)
{
    LWDanceRoomPosInfo *pPosInfo = GetRoomPosInfo(eRoleType);
    if (NULL == pPosInfo)
        return 0; // 

    return pPosInfo->GetDancer(nPos).m_nRoleID;
}
bool LWRoomPosManager::HasPlayerInPostion(ERoleRoomType eRoleType, unsigned int nPos)
{
    return GetPostionRole(eRoleType, nPos) != 0;
}

bool LWRoomPosManager::PostionOpened(ERoleRoomType eRoleType, unsigned int nPos)
{
    LWDanceRoomPosInfo *pPosInfo = GetRoomPosInfo(eRoleType);
    if (NULL == pPosInfo)
        return false;

    return pPosInfo->IsPosOpend(nPos);
}

void LWRoomPosManager::InitRoomPosInfo()
{
    ClearRoomPosInfo();
}

void LWRoomPosManager::ClearRoomPosInfo()
{
    for (unsigned short i = RoleRoomType_None; i < RoleRoomType_Max; ++i)
    {
        LWDanceRoomPosInfo *pPosInfo = GetRoomPosInfo((ERoleRoomType)i);
        if (NULL != pPosInfo)
        {
            pPosInfo->Clear();
        }
    }

    return;
}
