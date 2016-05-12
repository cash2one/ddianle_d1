#include "DataStruct_Bet.h"

#include "datainfor/LuckDrawPrizeBase.h"
#include "datainfor/LuckDrawPrizeFactory.h"


CLuckDrawInfoDB::CLuckDrawInfoDB()
    : m_nRoleID(0)
    , m_nFreeTimes(0)
    , m_nResetTime(0)
{
}

CLuckDrawInfoDB::~CLuckDrawInfoDB()
{
}

//////////////////////////////////////////////////////////////////////////

CLuckDrawRecordDB::CLuckDrawRecordDB()
    : m_nRoleID(0)
{
}

CLuckDrawRecordDB::~CLuckDrawRecordDB()
{
    for (std::vector<CLuckDrawPrizeBase *>::iterator itr = m_vectItems.begin(); 
         itr != m_vectItems.end(); ++itr)
    {
        delete *itr;

        *itr = NULL;
    }

    m_vectItems.clear();
}

//////////////////////////////////////////////////////////////////////////

CLuckDrawRecords::CLuckDrawRecords(const std::string &rRoleName, int nVIPLevel, const CLuckDrawPrizeBase *pBase)
    : m_strRoleName(rRoleName)
    , m_nVIPLevel(nVIPLevel)
{
    m_pBase = CLuckDrawPrizeFactory::CreateLuckDrawPrizeByInstance(pBase);
}

CLuckDrawRecords::~CLuckDrawRecords()
{
    delete m_pBase;
}

