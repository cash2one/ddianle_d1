#ifndef __DUNGEONDATAMGR_H__
#define __DUNGEONDATAMGR_H__

#include "DungeonDefine.h"
#include "../../datastructure/datainfor/MagicLampConfig.h"
#include <list>
#include <map>

class CDungeonDataMgr
{
public:
	CDungeonDataMgr();
	virtual ~CDungeonDataMgr(){};

	static CDungeonDataMgr & instance();

public:
	// ���/��ȡ��Ը�ǵ�����
    void AddStarRewardConfig( const CStarRewardConfig & rewardInfo );
	CStarRewardConfig * GetStarRewardConfig( unsigned int nID );

	// ���/��ȡ���������
	void AddDustRewardConfig( const CDustRewardConfig & reward );
	CDustRewardConfig * GetDustRewardConfig();

	// ���/��ȡ�����б�
	void AddParcloseItemList( const std::list<CItem> & items );
	const std::list<CItem> & GetParcloseItemList(){ return m_ParcloseItemList; }

	// ��ȡ��Ը�ǽ�������
	const StarRewardInfoTable & getStarRewardConfig(){ return m_StarRewardConfig; }

	// ��ȡ���������
	const CDustRewardConfig & getDustRewardConfig(){ return m_DustRewardConfig; }
	unsigned int GetDustStoryID(){ return m_DustRewardConfig.m_nStoryID; }

	// ��ȡˮ������
	const CCrystalConfig & getCrystalConfig(){ return m_CrystalConfig; }
	unsigned int GetCrystalInitialValue(){ return m_CrystalConfig.m_nInitialValue; }
    unsigned int GetFavorableBuyVitCount();

	// ��ȡˮ���ָ�ʱ��
	 unsigned int GetCrystalRestoreTime(){ return m_CrystalConfig.m_nRestoreTime; }

	// ��ȡˮ�������ֵ
	 unsigned int GetMaxCrystalValue(){ return m_CrystalConfig.m_nMaxValue; }

	// ���/��ȡ����ˮ������
	void AddBuyCrystalConfig( const CBuyCrystalConfig & config ){ m_BuyCrystalConfig[config.m_nIndex] = config; }
	const BuyCrystalConfigTable & GetBuyCrystalConfig(){ return m_BuyCrystalConfig; }
	CBuyCrystalConfig * GetBuyCrystalConfig( unsigned int nIndex );

	// ���/��ȡ������Ϣ
	void  addCrystalConfig( const CCrystalConfig & config ){ m_CrystalConfig = config; }

	// ���/��ȡ�����ؿ���Ʒ
	void AddSkipParcloseItem( const list<CItem> & items ){ m_SkipParcloseItem = items;}
	const std::list<CItem> & GetSkipParcloseItem(){ return m_SkipParcloseItem; }

	// ��ӹؿ�����
	void GetMainParcloseDrop( unsigned int nParcloseID, std::vector<CItem> & maleitems, std::vector<CItem> & femaleitems, std::vector<DebrisReward> & debrisreward );
    void GetThemeParcloseDrop( unsigned int nThemeID, unsigned int nParcloseID, std::vector<CItem> & maleitems, std::vector<CItem> & femaleitems, std::vector<DebrisReward> & debrisreward );
    void CalcMainlineMaleParcloseDrop( unsigned int nParcloseID, std::list<CItem> & listMaleItem, std::list<DebrisReward> & listDebris );
    void CalcMainlineFemaleParcloseDrop( unsigned int nParcloseID, std::list<CItem> & listFemaleItem, std::list<DebrisReward> & listDebris );
    void CalThemeMaleParcloseDrop( unsigned int nThemeID, unsigned int nParcloseID, std::list<CItem> & listMaleItem, std::list<DebrisReward> & listDebris);
    void CalThemeFemaleParcloseDrop( unsigned int nThemeID, unsigned int nParcloseID, std::list<CItem> & listFemaleItem, std::list<DebrisReward> & listDebris);
    void CalcParcloseDrop( int nSex, EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID, std::list<CItem> & listItem, std::list<DebrisReward> & listDebris);

	// ���ؾ�������
	void AddParcloseGradeExpConfig( int nMusicLevel, const std::map<int,int> & exp );
	void AddParcloseBaseExpConfig( unsigned int nFailExp, unsigned int nWinBaseExp );

	// ��ȡ����
	unsigned int GetParcloseCalcExp( bool isWin, int nGrade, unsigned int nMusicLevel );
	unsigned int GetDungeonGradeExp( int nGrade, unsigned int nMusicLevel );

	// �ʼ�
	void AddRewardMailInfo( const CMagicLampRewardMail & mail ){ m_RewardMail = mail; }
	const CMagicLampRewardMail & GetRewardMail(){ return m_RewardMail; }

	// ����˵��
	void AddDesc( const CMagicLampDesc & desc ){ m_Desc = desc; }
	const CMagicLampDesc & GetDesc(){ return m_Desc; }

    // ��ȡ�������������
    unsigned int GetMaxInviteFriendNum(){ return m_Desc.m_nMaxInviteFriendNum; }

    // ���/��ȡ
    void AddPublicityPictureConfig( CPublicityPictureConfig & picture );
    const std::vector<CPublicityPictureConfig> & GetPublicityPictureConfig(){ return m_PublicityPictureConfig; }

    // ��Ӷ����Ż��������
    void AddGiveGetVitConfig(unsigned int nOnceGiveVitNum, unsigned int nResetTime, unsigned int nGetVitMaxNum);
    void AddQuickFinishConfig(bool bOpenToNotVip, const CItem & costItem);

    // ���ͺͽ�����������ʱ��
    unsigned int GetNextDayResetTime() const;

    // ÿ������������Ŀ
    unsigned int OnceGiveVitNum() const;

    // ��ȡ�ؿ�ɨ����Ʒ
    CItem GetQuickFinishItem();

    // ��ȡÿ����Ի�ȡ���������ֵ
    unsigned int GetDailyGetVitMaxNum() const;

    // ��VIP����Ƿ����ɨ��
    bool QucikFinishOpenToNotVip();

private:
    StarRewardInfoTable             m_StarRewardConfig;						 // ��Ը�ǽ���
    CDustRewardConfig               m_DustRewardConfig;					     // ����ƽ���
    std::list<CItem>                m_ParcloseItemList;						 // �ؿ���ʹ�õ����б�
    CCrystalConfig                  m_CrystalConfig;						 // ˮ������
    BuyCrystalConfigTable           m_BuyCrystalConfig;						 // ����ˮ������
    std::list<CItem>                m_SkipParcloseItem;						 // �����ؿ�ʹ�õ����б�
    std::map<int,std::map<int,int> >    m_ParcloseExpConfig;
    unsigned int	m_nParcloseFailExp;
    unsigned int    m_nParcloseWinBaseExp;
    CMagicLampRewardMail            m_RewardMail;			                // �����ʼ�
    CMagicLampDesc                  m_Desc;				                    // ���ܽ���
    std::vector<CPublicityPictureConfig>    m_PublicityPictureConfig;       // ����ͼ
    unsigned int                    m_nNextDayResetTime;                    // ���ͺͽ�����������ʱ��
    unsigned int                    m_nOnceGiveVitNum;                      // ����������Ŀ
    unsigned int                    m_nDailyGetVitMaxNum;                   // ÿ����ȡ���������ֵ
    CItem                           m_QuickFinishCostItem;                  // �ؿ�ɨ����Ʒ
    bool                            m_bQuickFinishOpenToNotVip;             // ɨ���Ƿ�Է�VIP��ҿ���
};


#endif
