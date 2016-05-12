#include "PlatformPrivilegeDataMgr.h"
#include "../util/CommonFunc.h"

CPlatformPrivilegeDataMgr::CPlatformPrivilegeDataMgr(void) : m_bOpen(false)
{
}

void CPlatformPrivilegeDataMgr::SetPlatformPrivilege(const char *pszName, const char *pszContent, const char * pszMailTitle, const char* pszMailContent)
{
    this->m_strName = pszName;
    this->m_strMailContent = pszContent;
    this->m_strMailTitle = pszMailTitle;
    this->m_strMailContent = pszMailContent;

    this->m_bOpen = true;
}

void CPlatformPrivilegeDataMgr::AddPlatformPrivilegeReward(int nVipIndex, int nLowLevel, int nHighLevel, const char *maleItem, const char *femaleItem, int nType)
{
    CPlatofromPrivilegeReward reward;
    reward.nVipIndex = nVipIndex;
    reward.nLowLevel = nLowLevel;
    reward.nHighLevel = nHighLevel;

    if (nType == PLATOFROM_PRIVILEGE_TYPE_UC)
    {
        reward.listItem.clear();
        TransferItemList(maleItem, reward.listItem);
        this->m_mapUCMaleReward[nVipIndex] = reward;

        reward.listItem.clear();
        TransferItemList(femaleItem, reward.listItem);
        this->m_mapUCFemaleReward[nVipIndex] = reward;
    }
}

CPlatofromPrivilegeReward *CPlatformPrivilegeDataMgr::GetPlatoformUCReward(int nRewardIndex, unsigned char nSex)
{
    if (nSex == ESexType_Male)
    {
        std::map<int, CPlatofromPrivilegeReward>::iterator iter = this->m_mapUCMaleReward.find(nRewardIndex);
        if (iter != this->m_mapUCMaleReward.end())
        {
            return &(iter->second);
        }
    }
    else if (nSex == ESexType_Female)
    {
        std::map<int, CPlatofromPrivilegeReward>::iterator iter = this->m_mapUCFemaleReward.find(nRewardIndex);
        if (iter != this->m_mapUCFemaleReward.end())
        {
            return &(iter->second);
        }
    }

    return NULL;
}

std::string & CPlatformPrivilegeDataMgr::MaileTitle()
{
    return this->m_strMailTitle;
}

std::string & CPlatformPrivilegeDataMgr::MailContent()
{
    return this->m_strMailContent;
}

