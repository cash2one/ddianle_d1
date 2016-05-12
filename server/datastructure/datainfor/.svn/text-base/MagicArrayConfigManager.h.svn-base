#ifndef __MAGICARRAYCONFIGMGR_H__
#define __MAGICARRAYCONFIGMGR_H__

#include <string>
#include <map>

#include "MagicArrayConfig.h"
#include "./FileStream.h"

class CFileStream;
class MagicArrayConfigManager
{
public:
	MagicArrayConfigManager();
	virtual ~MagicArrayConfigManager();

	static MagicArrayConfigManager & Instance();

public:
	void doMaleEncode(CParamPool &IOBuff);
	void doFemaleEndoce(CParamPool &IOBuff);

    // 获取魔法阵的个数
    void GetMagicArrayID(std::list<unsigned int> & listMagicArrayID);

    void doConfigEndoce(unsigned int nMagicArrayID, CParamPool &IOBuff);

	void RollFemaleReward(int nGroupID, std::list<CItem> & listReward);

    // 销毁数据
    void Destory();
    
	// 加载补偿数据
	void AddCompensation(const MagicArrayCompensation & compensation);
	void AddMaleCompensation(const MagicArrayCompensation & compensation);
	void AddFemaleCompensation(const MagicArrayCompensation & compensation);

    // 加载奖池数据
    void AddReward(const MagicArrayReward & reward);
    void AddMaleReward(const MagicArrayReward & reward);
    void AddFemaleReward(const MagicArrayReward & reward);
    void CalcMaleRewardTotalRate();
    void CalcFemaleRewardTotalRate();

    // 加载预览数据
    void AddPreview(const MagicArrayPreview & preview);
    void AddMalePreview(const MagicArrayPreview & preview);
    void AddFemalePreview(const MagicArrayPreview & preview);

    // 加载兑换配置
	void AddExchange(const MagicArrayExchange & exchange);
    void AddBaseExchange(const MagicArrayBaseExchange & exchange);

	// 加载配置
	bool LoadConfig(const char* pszFileName);

	// 获取魔法阵配置
	MagicArrayConfig * GetMagicArrayConfig(unsigned int nMagicArrayID);

    // 获取刷新兑换物品消耗货币
    void GetRefreshExchangeCost(int & nCurrencyType, int & nCurrencyValue);

    // 获取刷新兑换物品有效时间
    bool IsInRefreshValidTime(unsigned int nLastRefreshTime);

	// 随机奖励
    void RollMagicArrayReward(unsigned int nMagicArrayID, int nSex, const std::list<GroupRate> & listRate, std::list<MagicArrayReward> & reward);

    // 补偿物品
    void GetMagicArrayCompensation(unsigned int nMagicArrayID, unsigned int nGroupID, int nSex, std::list<CItem> & listItem);

    // 获取兑换的物品信息
    void GetBaseExchangeConfig(unsigned int nExchangeID, int nSex, CItem & exchangeitem, CItem & costitem);
    void doBaseExchangeEncode(int nSex, CParamPool & IOBuff);

    // 获取刷新的物品信息
    void GetRrfreshExchangeConfig(unsigned int nExchangeID, int nSex, CItem & exchangeitem, CItem & costitem);
    void GetRrfreshExchangeConfig(int nSex, unsigned int nExchangeID, std::list<MagicArrayExchange> & listExchange);
        itemtype_t GetExchangeCostItemType();
    void GetExchangeInfo(int nSex, unsigned int nTeamID, unsigned int nGroupID, std::list<MagicArrayExchange> & listExchange);

    // 获取奖励信息
    void GetReward(unsigned int nMagicArrayID, unsigned int nGroupID, int nSex, std::vector<CItem> & vecPreviewItem);

    // 刷新兑换奖励物品
    void RefreshExchangeGroup(std::map<unsigned int,unsigned int> & mapExchangeGroup);

private:
    // 获取奖励奖池的总roll值
    int GetGroupRollTotalRate(int nSex, unsigned int nMagicArrayID, unsigned int nGroupID);

private:
	std::map<unsigned int, std::map<unsigned int, std::vector<MagicArrayReward> > >			m_mapMaleReward;
    std::map<unsigned int, std::map<unsigned int, std::vector<MagicArrayPreview> > >        m_mapMalePreview;
    std::map<unsigned int, std::map<unsigned int, int> >                                    m_mapMaleRewardTotalRate;
	std::map<unsigned int, std::map<unsigned int, std::vector<MagicArrayReward> > >			m_mapFemaleReward;
    std::map<unsigned int, std::map<unsigned int, std::vector<MagicArrayPreview> > >        m_mapFemalePreview;
    std::map<unsigned int, std::map<unsigned int, int> >                                    m_mapFemaleRewardTotalRate;
	std::map<unsigned int, std::map<unsigned int, std::list<MagicArrayCompensation> > >    	m_mapMaleCompensation;
	std::map<unsigned int, std::map<unsigned int, std::list<MagicArrayCompensation> > >	    m_mapFemaleCompensation;
    std::map<unsigned int, MagicArrayConfig*>												m_mapMagicArrayConfig;
    std::map<unsigned int, MagicArrayBaseExchange>                                          m_mapMaleBaseExchangeConfig;
    std::map<unsigned int, MagicArrayBaseExchange>                                          m_mapFemaleBaseExchangeConfig;
    std::map<unsigned int, std::vector<exchangegroup_t> >                                   m_mapExchangeTeam;
    std::map<unsigned int, MagicArrayExchange>                                              m_mapMaleExchangeConfig;
    std::map<unsigned int, MagicArrayExchange>                                              m_mapFemaleExchangeConfig;
};

#endif
