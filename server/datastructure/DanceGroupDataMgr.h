#ifndef		__DANCEGROUPDATAMGR_H__
#define		__DANCEGROUPDATAMGR_H__

#include <map>
#include <vector>
#include <list>
#include "../datastructure/DataStruct_Base.h"

//typedef std::map< unsigned short, unsigned int> DGStarDayItem; //id ->itemId
typedef std::map< unsigned short, std::list<CItem> > DGStarDayItem; //id ->itemId

class CDGLevelUpRequireRes
{
public:
	CDGLevelUpRequireRes();
	~CDGLevelUpRequireRes();
public:
	unsigned short m_nGroupLevel;
	unsigned int m_nResA;
	unsigned int m_nResB;
	unsigned int m_nResC;
};

class CDGActivityStarBaseInfo
{
public:
	CDGActivityStarBaseInfo(){}
	~CDGActivityStarBaseInfo(){}

public:
	unsigned short m_nStarLevel;
	unsigned short m_nProgress;
	unsigned int m_nContributionLow;
	unsigned int m_nContributionMid;
	unsigned int m_nContributionHigh;
	std::string m_strName;
};

class CDGShopItemInfo
{
public:
	CDGShopItemInfo()
		:m_nId( 0 ),m_nExchangeCount( 0 ),m_nNeedContributionValue( 0 )
	{}
	~CDGShopItemInfo(){}

public:
	unsigned short m_nId;
	CItem m_maleItem;
	CItem m_femaleItem;
	unsigned short m_nExchangeCount;
	unsigned short m_nNeedContributionValue;
};

class CDGShopInfo
{
public:
	CDGShopInfo()
		:m_nDayIndex( 0 )
	{}
	~CDGShopInfo(){}

public:
	unsigned short m_nDayIndex;//��Ӧ���������
	std::map<unsigned short, CDGShopItemInfo> m_dayItemList; //��Ӧ��ǰ���Զһ�����Ʒ�б�
};


// ������ʽ���˽���
class CeremonyNewerPrize
{
public:
    CeremonyNewerPrize();
    ~CeremonyNewerPrize();

    unsigned int    m_nRanking;            // ����
    std::string     m_strSender;
    std::string     m_strMailTitle;         // �ʼ�����
    std::string     m_strContext;           // 
    std::string     m_strItem;            // ��������
};

// ������ʽ��ͨ��Ա����
class CeremonyNormalPrize
{
public:
    CeremonyNormalPrize();
    ~CeremonyNormalPrize();

    std::string     m_strSender;
    std::string m_strMailTitle;         //
    std::string     m_strContext;           // 
    std::string     m_strItem;            // ��������
    unsigned int    m_nMaxPraiser;      // ����������
};


class CDanceGroupDataMgr
{
private:
	CDanceGroupDataMgr();
	~CDanceGroupDataMgr();
public:
	static CDanceGroupDataMgr & Instance();
	void AddDanceGroupInfo(unsigned short nMaxDanceGroupLevel, int nMemberLoginGroupHonorBonusLevelNeed, int nMemberLoginGropHonorBonus,
						   int nMinRoomDancersGroupHonorBonusNeed, int nMemberMatchBonusLeve1,
						   unsigned int nIncDGHonorNumPerDonatedResA, unsigned int nIncDGHonorNumPerDonatedResB, unsigned int nIncDGHonorNumPerDonatedResC,
						   unsigned int nIncMemberContributionNumPerDonatedResA, unsigned int nIncMemberContributionNumPerDonatedResB, unsigned int nIncMemberContributionNumPerDonatedResC);
	void AddDGLevelUpRequireResInfo(CDGLevelUpRequireRes *pInfo);
	CDGLevelUpRequireRes * GetLevelUpRequireResBy(unsigned short nLevel);
	unsigned short MaxDanceGroupLevel();
	int MemberLoginGroupHonorBonusLevelNeed();
	int MemberLoginGropHonorBonus();
	int MinRoomDancersGroupHonorBonusNeed();
	unsigned short RoomGroupHonorBonusGeneID();
	int MemberMatchGroupHonorBonusLeve1();
	unsigned int IncDGHonorNumPerDonatedResA();
	unsigned int IncDGHonorNumPerDonatedResB();
	unsigned int IncDGHonorNumPerDonatedResC();

	unsigned int IncMemberContributionNumPerDonatedResA();
	unsigned int IncMemberContributionNumPerDonatedResB();
	unsigned int IncMemberContributionNumPerDonatedResC();

    // ������ʽ��أ�
    void SetCeremonyApplyInfo(unsigned char freeApplyTimes, unsigned char applyCostType, unsigned int applyCostNum);
    void SetCeremonyPraiseInfo(unsigned char freePraiseTime, unsigned char PraiseCostType, unsigned int PraiseCostNum, unsigned int nPraiseCD);
    void SetCeremonyBaseInfo(unsigned int applyDuration, unsigned int startDuration, unsigned int nEnterNewGroupCD, unsigned int nEnterOldGroupCD); 

public:
	//���ժ���ǻ��Ϣ
	void SetBaseInfo( const char* szMale, const char* szFemale,
		unsigned short nNormalCount, int nNormalPower, int nNormalContribution,
		unsigned short nMoneyCount, int nMoneyNum, int nMoneyPower, int nMoneyContribution,
		unsigned short nPointCount, int nPointNum, int nPointPower, int nPointContribution,
		string strRuleContent  );
	//�������ǽ���
	void SetStarProgress( int nStarLevel, int nStarProgress, unsigned int nConLow, unsigned int nConMid, unsigned int nConHigh, const char *name );
	//��������Ϣ
	void AddStarPacket( unsigned short nDay, unsigned short nIndex, const char* szMale, const char* szFemale );

	std::map<unsigned short, CDGActivityStarBaseInfo > &GetStarBaseInfoList(){ return m_mStarProgress;}
	bool GetPacketListByDay( DGStarDayItem &mList, unsigned char sex, unsigned short nDay = 1); //���ݵ�ǰ��ĳһ������������������

	std::map<unsigned short, CDGShopInfo> &GetDGShopInfo(){ return m_mDGShopInfo; }
	CDGShopInfo& GetDayShopInfo();
	void AddDGShopItem( CDGShopItemInfo shopItemInfo, unsigned int nDayIndex );
	std::list<CItem>& GetRewardListBySex( ESexType eSex );

    void AddNewManPrize(CeremonyNewerPrize &prize);
    void AddNormalPrize(CeremonyNormalPrize &prize);

    CeremonyNewerPrize* GetCeremonyNewerPrize(int ranking);
    CeremonyNormalPrize* GetCeremonyNormalPrize(int maxPrizer);

public:
	std::map<unsigned short, CDGLevelUpRequireRes *> m_mapDanceGroupLevelUpRequireRes;
	unsigned short m_nMaxDanceGroupLevel;
	int m_nMemberLoginGroupHonorBonusLevelNeed;
	int	m_nMemberLoginGropHonorBonus;			
	int m_nMinRoomDancersGroupHonorBonusNeed;
	unsigned short m_RoomGroupHonorBonusGeneID;
	int m_nMemberMatchBonusLeve1;

	unsigned int m_nIncDGHonorNumPerDonatedResA;
	unsigned int m_nIncDGHonorNumPerDonatedResB;
	unsigned int m_nIncDGHonorNumPerDonatedResC;

	unsigned int m_nIncMemberContributionNumPerDonatedResA;
	unsigned int m_nIncMemberContributionNumPerDonatedResB;
	unsigned int m_nIncMemberContributionNumPerDonatedResC;

	////////////////////////////////////
	//��һ�²��� 
	unsigned short m_nActivityStarNormalCount;
	int m_nActivityStarNormalPower;
	int m_nActivityStarNormalContribution;
	//��ȯ
	unsigned short m_nActivityStarMoneyCount;
	int m_nActivityStarMoneyNum;
	int m_nActivityStarMoneyPower;
	int m_nActivityStarMoneyContribution;
	//point
	unsigned short m_nActivityStarPointCount;
	int m_nActivityStarPointNum;
	int m_nActivityStarPointPower;
	int m_nActivityStarPointContribution;
	string m_strRuleContent;

	//ÿ���״ε������һ�¡���Ľ���(���)
	//unsigned short nMalePid;
	//unsigned short nFemalepId;
	std::list<CItem> m_maleList;
	std::list<CItem> m_femaleList;

	//��ʱֻд��һ��ʱ�������
	std::map<unsigned short, DGStarDayItem> m_mMaleItem; //week
	std::map<unsigned short ,DGStarDayItem> m_mFemaleItem; //week

	//���ǽ�����Ϣ
	std::map<unsigned short, CDGActivityStarBaseInfo > m_mStarProgress; 

	//�����̵�����
	std::map<unsigned short, CDGShopInfo> m_mDGShopInfo;

    // ������ʽÿ���������
    unsigned char m_nCeremonyFreeApplyTimes;
    unsigned char m_nCeremonyApplyCostType;
    unsigned int m_nCeremonyApplyCostNum;
    // ������ʽÿ�ε������
    unsigned char m_nCeremonyFreePraiseTimes;
    unsigned char m_nCeremonyPraiseCostType;
    unsigned int m_nCeremonyPraiseCostNum;
    unsigned int m_nCeremonyPraiseCD;
    // ��ʽ���뵽�������ʱ��
    unsigned int m_nCeremonyApplyDuration;
    unsigned int m_nCeremonyStartDuration;

    // �����µ��ŵ���ȴʱ��
    unsigned int m_nEnterNewGroupCD;
    // �����ϵ��ŵ���ȴʱ��
    unsigned int m_nEnterOldGroupCD;

    // ������ʽ������
    // ���˽���������ͨ��Ա�������Ѿ�����
    std::vector<CeremonyNewerPrize> m_ceremonyNewerPrizeSorted;
    std::vector<CeremonyNormalPrize> m_ceremonyNormalPrizeSorted;

};



#endif

