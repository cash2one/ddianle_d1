#ifndef __DATASTRUCT_BET_H__
#define __DATASTRUCT_BET_H__

#include <vector>
#include <string>


class CLuckDrawInfoDB
{
public:
    CLuckDrawInfoDB();
    ~CLuckDrawInfoDB();

public:
    unsigned int    m_nRoleID;
    int             m_nFreeTimes;
    unsigned int    m_nResetTime;
};

class CLuckDrawPrizeBase;

class CLuckDrawRecordDB
{
public:
    CLuckDrawRecordDB();
    ~CLuckDrawRecordDB();

public:
    unsigned int                        m_nRoleID;
    std::vector<CLuckDrawPrizeBase *>   m_vectItems;
};

class CLuckDrawRecords
{
public:
    CLuckDrawRecords(const std::string &rRoleName, int nVIPLevel, const CLuckDrawPrizeBase *pBase);
    ~CLuckDrawRecords();

    bool IsLuckDrawPrizeNULL() { return m_pBase == NULL; }

public:
    std::string         m_strRoleName;
    int                 m_nVIPLevel;
    CLuckDrawPrizeBase  *m_pBase;
};

#endif


