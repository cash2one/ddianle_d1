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

    // ��ȡħ����ĸ���
    void GetMagicArrayID(std::list<unsigned int> & listMagicArrayID);

    void doConfigEndoce(unsigned int nMagicArrayID, CParamPool &IOBuff);

	void RollFemaleReward(int nGroupID, std::list<CItem> & listReward);

    // ��������
    void Destory();
    
	// ���ز�������
	void AddCompensation(const MagicArrayCompensation & compensation);
	void AddMaleCompensation(const MagicArrayCompensation & compensation);
	void AddFemaleCompensation(const MagicArrayCompensation & compensation);

    // ���ؽ�������
    void AddReward(const MagicArrayReward & reward);
    void AddMaleReward(const MagicArrayReward & reward);
    void AddFemaleReward(const MagicArrayReward & reward);
    void CalcMaleRewardTotalRate();
    void CalcFemaleRewardTotalRate();

    // ����Ԥ������
    void AddPreview(const MagicArrayPreview & preview);
    void AddMalePreview(const MagicArrayPreview & preview);
    void AddFemalePreview(const MagicArrayPreview & preview);

    // ���ضһ�����
	void AddExchange(const MagicArrayExchange & exchange);
    void AddBaseExchange(const MagicArrayBaseExchange & exchange);

	// ��������
	bool LoadConfig(const char* pszFileName);

	// ��ȡħ��������
	MagicArrayConfig * GetMagicArrayConfig(unsigned int nMagicArrayID);

    // ��ȡˢ�¶һ���Ʒ���Ļ���
    void GetRefreshExchangeCost(int & nCurrencyType, int & nCurrencyValue);

    // ��ȡˢ�¶һ���Ʒ��Чʱ��
    bool IsInRefreshValidTime(unsigned int nLastRefreshTime);

	// �������
    void RollMagicArrayReward(unsigned int nMagicArrayID, int nSex, const std::list<GroupRate> & listRate, std::list<MagicArrayReward> & reward);

    // ������Ʒ
    void GetMagicArrayCompensation(unsigned int nMagicArrayID, unsigned int nGroupID, int nSex, std::list<CItem> & listItem);

    // ��ȡ�һ�����Ʒ��Ϣ
    void GetBaseExchangeConfig(unsigned int nExchangeID, int nSex, CItem & exchangeitem, CItem & costitem);
    void doBaseExchangeEncode(int nSex, CParamPool & IOBuff);

    // ��ȡˢ�µ���Ʒ��Ϣ
    void GetRrfreshExchangeConfig(unsigned int nExchangeID, int nSex, CItem & exchangeitem, CItem & costitem);
    void GetRrfreshExchangeConfig(int nSex, unsigned int nExchangeID, std::list<MagicArrayExchange> & listExchange);
        itemtype_t GetExchangeCostItemType();
    void GetExchangeInfo(int nSex, unsigned int nTeamID, unsigned int nGroupID, std::list<MagicArrayExchange> & listExchange);

    // ��ȡ������Ϣ
    void GetReward(unsigned int nMagicArrayID, unsigned int nGroupID, int nSex, std::vector<CItem> & vecPreviewItem);

    // ˢ�¶һ�������Ʒ
    void RefreshExchangeGroup(std::map<unsigned int,unsigned int> & mapExchangeGroup);

private:
    // ��ȡ�������ص���rollֵ
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
