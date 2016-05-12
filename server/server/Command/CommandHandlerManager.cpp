#include "CommandHandlerManager.h"
#include "CommandHandler.h"

using namespace std;

CCmdHanderManager::CCmdHanderManager()
{

}

CCmdHanderManager::~CCmdHanderManager()
{
    while (!m_mapCmdHandler.empty())
    {
        map<string, ICmdHandler *>::iterator it = m_mapCmdHandler.begin();
        ICmdHandler *handler = it->second;
        delete handler;
        it->second = NULL;
        m_mapCmdHandler.erase(it);
    }
    m_mapCmdHandler.clear();
}

CCmdHanderManager & CCmdHanderManager::Instance()
{
    static CCmdHanderManager s_rCmdHandlerManager;
    return s_rCmdHandlerManager;
}


void CCmdHanderManager::Init()
{
    Add(new CCmdFinQuestHandler);
    Add(new CCmdAddQusetHandler);
    Add(new CCmdAccountID);
    Add(new CCmdRoleID);
    Add(new CCmdAddExp);
    Add(new CCmdSetHot);
    Add(new CCmdSetPre);
    Add(new CCmdSetMoney);
    Add(new CCmdLevelUp);
    Add(new CCmdSetLevel);
    Add(new CCmdAddItem);
    Add(new CCmdItemCDAll);
    Add(new CCmdSetSysTime);
    Add(new CCmdAddPt);
    Add(new CCmdSetOnlineTime);
    Add(new CCmdSendItem);
    Add(new CCmdWhosyoudaddy);
    Add(new CCmdPlayerCount);
    Add(new CCmdAdminChat);
    Add(new CCmdAddMCoin);
    Add(new CCmdAddBindMCoin);
    Add(new CCmdDebug);
    Add(new CCmdAddIntimacy);
    Add(new CCmdOpenBox);
    Add(new CCmdDropDanceItem);
    Add(new CCmdAddVIPExp);
    Add(new CCmdSetVIPLevel);
    Add(new CCmdLuckBet);
    Add(new CCmdLiveRoom);
    Add(new CCmdCreateRoom);
    Add(new CCmdCreateAmuseRoom);
    Add(new CCmdAcceptQuest);
    Add(new CCmdActivitedParclose);
    Add(new CCmdAddDebris);
    Add(new CCmdGetServerPlayer);
    Add(new CCmdAddDanceGroupHonor);
    Add(new CCmdEnterDanceGroupByName);
    Add(new CCmdRookieAnyBody);
    Add(new CCmdGetCeremonyRoomNum);
    Add(new CCmdAddLiveRoomAuditionScore);
    Add(new CCmdActivitedThemeParclose);
    Add(new CCmdCheckIn);
	Add(new CCmdActivenessComp);
    Add(new CCmdAddClothEffect);
    Add(new CCmdMagicArray);
    Add(new CCmdMedalComp);
}

void CCmdHanderManager::Add(ICmdHandler *pHandler)
{
    if (pHandler != NULL)
    {
        map<string, ICmdHandler *>::iterator itr = m_mapCmdHandler.find(pHandler->Cmd());
        if (itr == m_mapCmdHandler.end())
        {
            string cmd = pHandler->Cmd();
            m_mapCmdHandler.insert(make_pair(cmd, pHandler));
        }
    }
}

void CCmdHanderManager::Remove(const string & cmd)
{
    map<string, ICmdHandler *>::iterator itr = m_mapCmdHandler.find(cmd);
    if (itr != m_mapCmdHandler.end())
    {
        delete itr->second;
        itr->second = NULL;
        m_mapCmdHandler.erase(itr);
    }
}

ICmdHandler *CCmdHanderManager::GetCmdHandler(const string &cmd)
{
    map<string, ICmdHandler *>::iterator itr = m_mapCmdHandler.find(cmd);
    if (itr != m_mapCmdHandler.end())
    {
        return itr->second;
    }

    return NULL;
}

