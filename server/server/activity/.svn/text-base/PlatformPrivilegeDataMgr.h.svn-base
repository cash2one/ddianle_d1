#ifndef _PLATFORMPRIVILEGEDAAMGR_H__
#define _PLATFORMPRIVILEGEDAAMGR_H__
#pragma once

#include <string>
#include <map>
#include "../../datastructure/DataStruct_Base.h"
#include "ActivityDataStruct.h"


#define  PLATOFROM_PRIVILEGE_TYPE_UC 1//对应lua文件


class CPlatformPrivilegeDataMgr
{
public:
    CPlatformPrivilegeDataMgr();
    ~CPlatformPrivilegeDataMgr(){}

    static CPlatformPrivilegeDataMgr & Instance()
    {
        static CPlatformPrivilegeDataMgr g_instance;
        return g_instance;
    }

public:

    void SetPlatformPrivilege(const char *pszName, const char *pszContent, const char * pszMailTitle, const char* pszMailContent);
    void AddPlatformPrivilegeReward(int nVipIndex, int nLowLevel, int nHighLevel, const char *maleItem, const char *femaleItem, int nType);

    inline bool IsOpen(){
        return this->m_bOpen;
    }

public:
    CPlatofromPrivilegeReward *GetPlatoformUCReward(int nRewardIndex, unsigned char nSex);
    inline std::map<int, CPlatofromPrivilegeReward>& GetPlatformUCRewardList(unsigned char nSex)
    {
        if (nSex == ESexType_Male)
            return m_mapUCMaleReward;
        else
            return m_mapUCFemaleReward;
    }

    std::string & MaileTitle();
    std::string & MailContent();

public:
    bool m_bOpen;

    std::string m_strName;
    std::string m_strContent;
    std::string m_strMailTitle;
    std::string m_strMailContent;

    std::map<int, CPlatofromPrivilegeReward> m_mapUCMaleReward;
    std::map<int, CPlatofromPrivilegeReward> m_mapUCFemaleReward;

};

#endif//_PLATFORMPRIVILEGEDAAMGR_H__

