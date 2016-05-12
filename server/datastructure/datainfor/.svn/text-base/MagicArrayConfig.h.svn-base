#ifndef __MAGICARRAYCONFIG_H__
#define __MAGICARRAYCONFIG_H__

#include "../DataStruct_Base.h"
#include "FileStream.h"
#include "../../socket/ParamPool.h"

struct GroupRate
{
	unsigned int		nGroupID;
	unsigned int		nRate;
};

class MagicArrayConfig
{
public:
	MagicArrayConfig();
	virtual ~MagicArrayConfig();

public:
	bool Load(CFileStream & file);
	void doEncode(CParamPool & IOBuff);

public:
	unsigned int	        m_nMagicArrayID;
	unsigned int	        m_nFreeCallCD;
    unsigned int            m_nAccumulatedCount;
    std::list<GroupRate>    m_listAccumulatedRollRate;
    std::list<GroupRate>    m_listFirstRollRate;
	unsigned int	        m_nCurrencyType;
	unsigned int	        m_nOnceCallValue;
    std::list<CItem>        m_listOnceCostItem;
    std::list<CItem>        m_listOnceFixedItem;
    unsigned int            m_nTenCallValue;
    std::list<CItem>        m_listTenCostItem;
    std::list<GroupRate>    m_listOnceCallRate;
    unsigned int            m_nFirstTenCallRateCount;
    std::list<GroupRate>    m_listFirstTenCallRate;
    unsigned int            m_nSecondTenCallRateCount;
    std::list<GroupRate>    m_listSecondTenCallRate;
    std::list<CItem>        m_listTenFixedItem;
    unsigned int	        m_nRefreshExchangeCType;
    unsigned int	        m_nRefreshExchangeCValue;
    unsigned int            m_nRefreshBeginTime;
    unsigned int            m_nRefreshEndTime;
};

class MagicArrayReward
{
public:
	MagicArrayReward();
	virtual ~MagicArrayReward();

public:
	bool Load(CFileStream & file);

public:
	unsigned int			m_nMagicArrayID;
	unsigned int			m_nGroupID;
	CItem					m_Item;
	int						m_nSex;
	int						m_nWeight;
	int						m_nCurrencyType;
};

class MagicArrayPreview
{
public:
    MagicArrayPreview();
    virtual ~MagicArrayPreview();

public:
    bool Load(CFileStream & file);

public:
    unsigned int			m_nMagicArrayID;
    unsigned int			m_nGroupID;
    CItem					m_Item;
    int						m_nSex;
};

class MagicArrayCompensation
{
public:
	MagicArrayCompensation();
	virtual ~MagicArrayCompensation();

public:
	bool Load(CFileStream & file);

public:
	unsigned int			m_nMagicArrayID;
	unsigned int			m_nGroupID;
	CItem					m_nCompessationItem;
	int						m_nSex;
};

typedef pair<unsigned int, itemtype_t> exchangegroup_t;
class MagicArrayExchange
{
public:
	MagicArrayExchange();
	virtual ~MagicArrayExchange();

public:
	bool Load(CFileStream & file);

public:
    unsigned int            m_nExchangeID;
	unsigned int			m_nTeamID;
	unsigned int			m_nGroupID;
	CItem					m_ExchangeItem;
	CItem					m_CostItem;
	int						m_nSex;
};

class MagicArrayBaseExchange
{
public:
    MagicArrayBaseExchange();
    virtual ~MagicArrayBaseExchange();

public:
    bool Load(CFileStream & file);

public:
    unsigned int            m_nExchangeID;
    CItem					m_ExchangeItem;
    CItem					m_CostItem;
    int                     m_nSex;
};



#endif

