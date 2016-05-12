#include "MeetActivity.h"

#include "../../datastructure/RoleEntity.h"
#include "../logic/PlayerManager.h"

CMeetActivity::CMeetActivity(unsigned int nPhotoCount, unsigned int nWaitTime)
:m_nCoupleCount(0),
m_nPhotoCount(nPhotoCount),
m_nWaitTime(nWaitTime)
{

}

void CMeetActivity::OnUpdate(const unsigned long &lTimeElapsed, time_t tNow)
{
    MeetWaitList::iterator it = m_WaitMaleList.begin();
    for (; it != m_WaitMaleList.end();)
    {
        if (tNow - it->intime >= m_nWaitTime)
        {
            HandleTimeOut(it->roleid);
            m_WaitMaleList.erase(it++);
        }
        else
        {
            ++it;
        }
    }

    MeetWaitList::iterator iter = m_WaitFemaleList.begin();
    for (; iter != m_WaitFemaleList.end();)
    {
        if (tNow - iter->intime >= m_nWaitTime)
        {
            HandleTimeOut(iter->roleid);
            m_WaitFemaleList.erase(iter++);
        }
        else
        {
            ++iter;
        }
    }
}

bool CMeetActivity::IsValid()
{
    return true;
}

void CMeetActivity::Init()
{

}

void CMeetActivity::SetPhotoIndexCount(int nCount)
{
    m_nPhotoCount = nCount;
}

int CMeetActivity::MatchInMaleWaitList(unsigned int nRoleID)
{
    if (m_WaitMaleList.empty())
    {
        MeetPlayerObject playerObject;
        playerObject.roleid = nRoleID;
        playerObject.sex = ESexType_Female;

        unsigned int nTimeNow = (unsigned int)time(NULL);
        MeetWait wait;
        wait.roleid = nRoleID;
        wait.intime = nTimeNow;
        playerObject.waitlink = m_WaitFemaleList.insert(m_WaitFemaleList.end(), wait);
        playerObject.status = MPS_MATCHING;

        m_MeetPlayer[nRoleID] = playerObject;

        WriteLog(LOGLEVEL_DEBUG, "Debug: MeetActivity.match roleid:%u, index:%s", nRoleID, "in female list");

        return EMeetErr_InWaitList;
    }


    MeetPlayerObject playerObject;
    playerObject.roleid = nRoleID;
    playerObject.sex = ESexType_Female;

    MeetWait waiting = *(m_WaitMaleList.begin());
    unsigned int nCoupleID = waiting.roleid;
    MeetCouple couple(m_nPhotoCount);
    couple.nID = m_nCoupleCount++;
    couple.nOppositeID[0] = nRoleID;
    couple.nOppositeID[1] = nCoupleID;
    playerObject.couplelink = m_MeetCoupleTable.insert(m_MeetCoupleTable.end(), std::make_pair(couple.nID, couple));
    playerObject.status = MPS_RUNNING;
    m_MeetPlayer[nRoleID] = playerObject;

    MeetPlayerObjectTable::iterator itCouple = m_MeetPlayer.find(nCoupleID);
    if (itCouple != m_MeetPlayer.end())
    {
        itCouple->second.couplelink = playerObject.couplelink;
        itCouple->second.status = MPS_RUNNING;

        if (itCouple->second.waitlink != m_WaitMaleList.end())
        {
            m_WaitMaleList.erase(itCouple->second.waitlink);
        }
    }

    WriteLog(LOGLEVEL_DEBUG, "Debug: MeetActivity.match roleid:%u, index:%s, beMatch roleid:%d", nRoleID, "匹配到：male list", nCoupleID);

    return EMeetErr_Success;
}

int CMeetActivity::MatchInFemaleWaitList(unsigned int nRoleID)
{
    if (m_WaitFemaleList.empty())
    {
        MeetPlayerObject playerObject;
        playerObject.roleid = nRoleID;
        playerObject.sex = ESexType_Male;

        unsigned int nTimeNow = (unsigned int)time(NULL);
        MeetWait wait;
        wait.roleid = nRoleID;
        wait.intime = nTimeNow;
        playerObject.waitlink = m_WaitMaleList.insert(m_WaitMaleList.end(), wait);
        playerObject.status = MPS_MATCHING;

        m_MeetPlayer[nRoleID] = playerObject;

        WriteLog(LOGLEVEL_DEBUG, "Debug: MeetActivity.match roleid:%u,index:%s", nRoleID, "in male list");

        return EMeetErr_InWaitList;
    }


    MeetPlayerObject playerObject;
    playerObject.roleid = nRoleID;
    playerObject.sex = ESexType_Male;

    MeetWait waiting = *(m_WaitFemaleList.begin());
    unsigned int nCoupleID = waiting.roleid;
    MeetCouple couple(m_nPhotoCount);
    couple.nID = m_nCoupleCount++;
    couple.nOppositeID[0] = nCoupleID;
    couple.nOppositeID[1] = nRoleID;
    playerObject.couplelink = m_MeetCoupleTable.insert(m_MeetCoupleTable.end(), std::make_pair(couple.nID, couple));
    playerObject.status = MPS_RUNNING;
    m_MeetPlayer[nRoleID] = playerObject;

    MeetPlayerObjectTable::iterator itCouple = m_MeetPlayer.find(nCoupleID);
    if (itCouple != m_MeetPlayer.end())
    {
        itCouple->second.couplelink = playerObject.couplelink;
        itCouple->second.status = MPS_RUNNING;

        if (itCouple->second.waitlink != m_WaitFemaleList.end())
        {
            m_WaitFemaleList.erase(itCouple->second.waitlink);
        }
    }

    WriteLog(LOGLEVEL_DEBUG, "Debug: MeetActivity.match roleid:%u, index:%s,beMatch roleid:%d", nRoleID, "匹配到：famale list", nCoupleID);

    return EMeetErr_Success;
}

int CMeetActivity::Match(unsigned int nRoleID, unsigned int nSex, unsigned int nCoupleID)
{
    MeetPlayerObjectTable::iterator it = m_MeetPlayer.find(nRoleID);
    if (it != m_MeetPlayer.end())
    {
        return EMeetErr_HaveMatch;
    }

    MeetPlayerObjectTable::iterator itCouple = m_MeetPlayer.find(nCoupleID);
    if (itCouple != m_MeetPlayer.end())
    {
        if (itCouple->second.status == MPS_MATCHING)
        {
            MeetPlayerObject playerObject;
            playerObject.roleid = nRoleID;
            playerObject.sex = nSex;

            MeetCouple couple(m_nPhotoCount);
            couple.nID = m_nCoupleCount++;
            if (nSex == ESexType_Male)
            {
                couple.nOppositeID[0] = nCoupleID;
                couple.nOppositeID[1] = nRoleID;
            }
            else
            {
                couple.nOppositeID[0] = nRoleID;
                couple.nOppositeID[1] = nCoupleID;
            }
            playerObject.couplelink = m_MeetCoupleTable.insert(m_MeetCoupleTable.end(), std::make_pair(couple.nID, couple));
            playerObject.status = MPS_RUNNING;

            itCouple->second.couplelink = playerObject.couplelink;
            itCouple->second.status = MPS_RUNNING;

            if (nSex == ESexType_Male)
            {
                if (itCouple->second.waitlink != m_WaitFemaleList.end())
                {
                    m_WaitFemaleList.erase(itCouple->second.waitlink);
                }
            }
            else
            {
                if (itCouple->second.waitlink != m_WaitMaleList.end())
                {
                    m_WaitMaleList.erase(itCouple->second.waitlink);
                }
            }

            m_MeetPlayer[nRoleID] = playerObject;

            WriteLog(LOGLEVEL_DEBUG, "Debug: MeetActivity.match roleid:%u, index:%s,beMatch roleid:%d", nRoleID, "匹配到：情侣", nCoupleID);
        }
        else
        {
            // 在等待列表中找
            if (nSex == ESexType_Male)
            {
                return MatchInFemaleWaitList(nRoleID);
            }
            else
            {
                return MatchInMaleWaitList(nRoleID);
            }
        }
    }
    else
    {
        // 在等待列表中找
        if (nSex == ESexType_Male)
        {
            return MatchInFemaleWaitList(nRoleID);
        }
        else
        {
            return MatchInMaleWaitList(nRoleID);
        }
    }

    return EMeetErr_Success;
}

void CMeetActivity::CancelMatch(unsigned int nRoleID, unsigned int nSex)
{
    MeetPlayerObjectTable::iterator it = m_MeetPlayer.find(nRoleID);
    if (it != m_MeetPlayer.end() && it->second.status == MPS_MATCHING)
    {
        if (nSex == ESexType_Male)
        {
            m_WaitMaleList.erase(it->second.waitlink);
        }
        else
        {
            m_WaitFemaleList.erase(it->second.waitlink);
        }
        m_MeetPlayer.erase(it);
        WriteLog(LOGLEVEL_DEBUG, "Debug: MeetActivity.leave_MatchCancel roleid:%d", nRoleID);
    }
}

int CMeetActivity::PhotoEvent(unsigned int nRoleID, unsigned int nPhotoIndex)
{
    MeetPlayerObjectTable::iterator it = m_MeetPlayer.find(nRoleID);
    if (it == m_MeetPlayer.end())
    {
        return EMeetErr_NotFindPlayer;
    }

    if (it->second.status != MPS_RUNNING)
    {
        return EMeetErr_PlayerStatusErr;
    }

    if (nPhotoIndex < 1 || nPhotoIndex > m_nPhotoCount)
    {
        return EMeetErr_ClientErr;
    }

    if (it->second.sex == ESexType_Male)
    {
        if (it->second.couplelink != m_MeetCoupleTable.end())
        {
            if (it->second.couplelink->second.maleIsPhotoFlag[nPhotoIndex - 1])
            {
                return EMeetErr_PhotoIndexHave;
            }

            it->second.couplelink->second.maleIsPhotoFlag[nPhotoIndex - 1] = true;
        }
    }
    else
    {
        if (it->second.couplelink != m_MeetCoupleTable.end())
        {
            if (it->second.couplelink->second.femaleIsPhotoFlag[nPhotoIndex - 1])
            {
                return EMeetErr_PhotoIndexHave;
            }

            it->second.couplelink->second.femaleIsPhotoFlag[nPhotoIndex - 1] = true;
        }
    }

    return EMeetErr_Success;
}

void CMeetActivity::RemovePlayer(unsigned int nRoleID)
{
    MeetPlayerObjectTable::iterator it = m_MeetPlayer.find(nRoleID);
    if (it != m_MeetPlayer.end())
    {
        if (it->second.status == MPS_MATCHING)
        {
            it->second.sex == ESexType_Male ? m_WaitMaleList.erase(it->second.waitlink) : m_WaitFemaleList.erase(it->second.waitlink);
        }
        else
        {
            if (it->second.couplelink != m_MeetCoupleTable.end())
            {
                unsigned int nOppositeID = 0;
                if (it->second.sex == ESexType_Male)
                {
                    nOppositeID = it->second.couplelink->second.nOppositeID[0];
                }
                else
                {
                    nOppositeID = it->second.couplelink->second.nOppositeID[1];
                }

                MeetPlayerObjectTable::iterator iter = m_MeetPlayer.find(nOppositeID);
                if (iter == m_MeetPlayer.end())
                {
                    m_MeetCoupleTable.erase(it->second.couplelink);
                }
                else
                {
                    if (iter->second.status == MPS_RUNNING)
                    {
                        if (iter->second.couplelink != m_MeetCoupleTable.end())
                        {
                            if (iter->second.couplelink->second.nID != it->second.couplelink->second.nID)
                            {
                                m_MeetCoupleTable.erase(it->second.couplelink);
                            }
                        }
                    }
                }
            }
        }

        m_MeetPlayer.erase(it);
        WriteLog(LOGLEVEL_DEBUG, "Debug: MeetActivity.leave_Destory roleid:%d", nRoleID);
    }
}


bool CMeetActivity::IsCanDestoryAmuseScene(unsigned int nRoleID)
{
    MeetPlayerObjectTable::iterator it = m_MeetPlayer.find(nRoleID);
    if (it != m_MeetPlayer.end())
    {
        if (it->second.status == MPS_RUNNING)
        {
            if (it->second.couplelink != m_MeetCoupleTable.end())
            {
                unsigned int nOppositeID = 0;
                if (it->second.sex == ESexType_Male)
                {
                    nOppositeID = it->second.couplelink->second.nOppositeID[0];
                }
                else
                {
                    nOppositeID = it->second.couplelink->second.nOppositeID[1];
                }

                MeetPlayerObjectTable::iterator iter = m_MeetPlayer.find(nOppositeID);
                if (iter == m_MeetPlayer.end())
                {
                    return true;
                }
                else
                {
                    if (iter->second.status == MPS_RUNNING)
                    {
                        if (iter->second.couplelink != m_MeetCoupleTable.end())
                        {
                            if (iter->second.couplelink->second.nID != it->second.couplelink->second.nID)
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}

void CMeetActivity::HandleTimeOut(unsigned int nRoleID)
{
    MeetPlayerObjectTable::iterator it = m_MeetPlayer.find(nRoleID);
    if (it != m_MeetPlayer.end())
    {
        this->SendMatchFailed(EMeetErr_TimeOut, it->first);
        // 广播给前端时间到
        m_MeetPlayer.erase(it);

        WriteLog(LOGLEVEL_DEBUG, "Debug: MeetActivity.leave_timeOut roleid:%d", nRoleID);
    }
}

void CMeetActivity::ActivityEnd(unsigned int nRoleID, MeetPhotoInfoList & photosinfo, unsigned int &nSameIndexCount)
{
    MeetPlayerObjectTable::iterator it = m_MeetPlayer.find(nRoleID);
    if (it != m_MeetPlayer.end())
    {
        if (it->second.status == MPS_RUNNING)
        {
            for (unsigned int i = 0; i < m_nPhotoCount; ++i)
            {
                MeetPhotoInfo info;
                info.nIndex = i + 1;

                info.nIsMalePhoto = it->second.couplelink->second.maleIsPhotoFlag[i];
                info.nIsFemalePhoto = it->second.couplelink->second.femaleIsPhotoFlag[i];

                if (it->second.sex == ESexType_Male)
                {
                    it->second.couplelink->second.nIsMaleGet = true;
                }
                else
                {
                    it->second.couplelink->second.nIsFemaleGet = true;
                }

                if (info.nIsMalePhoto && info.nIsFemalePhoto)
                {
                    nSameIndexCount++;
                }

                photosinfo.push_back(info);
            }
        }
    }
}

unsigned int CMeetActivity::GetOppositeID(unsigned int nRoleID)
{
    MeetPlayerObjectTable::iterator it = m_MeetPlayer.find(nRoleID);
    if (it != m_MeetPlayer.end())
    {
        if (it->second.status == MPS_RUNNING)
        {
            if (it->second.sex == ESexType_Male)
            {
                return it->second.couplelink->second.nOppositeID[0];
            }
            else
            {
                return it->second.couplelink->second.nOppositeID[1];
            }

        }
    }

    return 0;
}

bool CMeetActivity::IsInActivity(unsigned int nRoleID)
{
    MeetPlayerObjectTable::iterator it = m_MeetPlayer.find(nRoleID);
    if (it != m_MeetPlayer.end())
    {
        return true;
    }

    return false;
}

bool CMeetActivity::IsBothCanReward(unsigned int nRoleID, unsigned int & nOppositeID, int & nIsMail)
{
    MeetPlayerObjectTable::iterator it = m_MeetPlayer.find(nRoleID);
    if (it == m_MeetPlayer.end())
    {
        return false;
    }

    if (it->second.status == MPS_MATCHING)
    {
        return false;
    }

    // 对方找的到不
    if (it->second.sex == ESexType_Male)
    {
        nOppositeID = it->second.couplelink->second.nOppositeID[0];
    }
    else
    {
        nOppositeID = it->second.couplelink->second.nOppositeID[1];
    }

    MeetPlayerObjectTable::iterator itOpposite = m_MeetPlayer.find(nOppositeID);
    if (itOpposite == m_MeetPlayer.end())
    {
        nIsMail = 1;
        return true;
    }
    else
    {
        if (itOpposite->second.status == MPS_RUNNING)
        {
            if (itOpposite->second.couplelink->second.nID != it->second.couplelink->second.nID)
            {
                nIsMail = 1;
                return true;
            }

            if (itOpposite->second.sex == ESexType_Male)
            {
                if (itOpposite->second.couplelink->second.nIsMaleGet)
                {
                    return true;
                }
            }
            else
            {
                if (itOpposite->second.couplelink->second.nIsFemaleGet)
                {
                    return true;
                }
            }
        }
        else
        {
            nIsMail = 1;
            return true;
        }
    }

    return false;
}

bool CMeetActivity::IsOppositeWaitReward(unsigned int nRoleID)
{
    MeetPlayerObjectTable::iterator it = m_MeetPlayer.find(nRoleID);
    if (it != m_MeetPlayer.end())
    {
        if (it->second.status == MPS_RUNNING)
        {
            unsigned int nOppositeID = 0;
            if (it->second.sex == ESexType_Male)
            {
                nOppositeID = it->second.couplelink->second.nOppositeID[0];
            }
            else
            {
                nOppositeID = it->second.couplelink->second.nOppositeID[1];
            }

            MeetPlayerObjectTable::iterator itOpposite = m_MeetPlayer.find(nOppositeID);
            if (itOpposite != m_MeetPlayer.end() && itOpposite->second.status == MPS_RUNNING)
            {
                if (itOpposite->second.couplelink->second.nID == it->second.couplelink->second.nID)
                {
                    if (it->second.sex == ESexType_Male)
                    {
                        if (itOpposite->second.couplelink->second.nIsFemaleGet  && !it->second.couplelink->second.nIsMaleGet)
                        {
                            return true;
                        }
                    }
                    else
                    {
                        if (itOpposite->second.couplelink->second.nIsMaleGet  && !it->second.couplelink->second.nIsFemaleGet)
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool CMeetActivity::IsOppositeCanGetReward(unsigned int nRoleID, unsigned int & nOppositeID)
{
    MeetPlayerObjectTable::iterator it = m_MeetPlayer.find(nRoleID);
    if (it != m_MeetPlayer.end())
    {
        if (it->second.status == MPS_RUNNING)
        {
            if (it->second.sex == ESexType_Male)
            {
                nOppositeID = it->second.couplelink->second.nOppositeID[0];
                return it->second.couplelink->second.nIsMaleGet;
            }
            else
            {
                nOppositeID = it->second.couplelink->second.nOppositeID[1];
                return it->second.couplelink->second.nIsFemaleGet;
            }
        }
    }

    return false;
}

void CMeetActivity::SetCoupleAmuseSceneID(unsigned int nRoleID, int nAmuseRoomID)
{
    MeetPlayerObjectTable::iterator it = m_MeetPlayer.find(nRoleID);
    if (it != m_MeetPlayer.end())
    {
        if (it->second.status == MPS_RUNNING)
        {
            it->second.couplelink->second.nAmuseSceneID = nAmuseRoomID;
        }
    }
}

int CMeetActivity::GetCoupleAmuseSceneID(unsigned int nRoleID)
{
    MeetPlayerObjectTable::iterator it = m_MeetPlayer.find(nRoleID);
    if (it != m_MeetPlayer.end())
    {
        if (it->second.status == MPS_RUNNING)
        {
            if (it->second.couplelink != m_MeetCoupleTable.end())
            {
                return it->second.couplelink->second.nAmuseSceneID;
            }
        }
    }

    return 0;
}

void CMeetActivity::SendMatchFailed(unsigned int nErr, unsigned int nRoleID)
{
    CRoleEntity *pTarget = CPlayerManager::Instance().GetEntityByRoleID(nRoleID);
    if (pTarget != NULL)
    {
        std::string strErr = "";
        switch (nErr)
        {
        case  EMeetErr_Waiting:
        {
                                  strErr = "EMeetErr_Waiting";
        }
            break;
        case  EMeetErr_TimeOut:
        {
                                  strErr = "EMeetErr_TimeOut";
        }
            break;
        case  EMeetErr_HaveMatch:
        {
                                    strErr = "EMeetErr_HaveMatch";
        }
        default:
            break;
        }

        GameMsg_S2C_MeetMatchFailed s2cMsg;
        s2cMsg.m_strError = strErr;
        pTarget->SendPlayerMsg(&s2cMsg);
    }
}

