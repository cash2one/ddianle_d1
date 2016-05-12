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
	// 添加/获取许愿星的配置
    void AddStarRewardConfig( const CStarRewardConfig & rewardInfo );
	CStarRewardConfig * GetStarRewardConfig( unsigned int nID );

	// 添加/获取搓神灯配置
	void AddDustRewardConfig( const CDustRewardConfig & reward );
	CDustRewardConfig * GetDustRewardConfig();

	// 添加/获取道具列表
	void AddParcloseItemList( const std::list<CItem> & items );
	const std::list<CItem> & GetParcloseItemList(){ return m_ParcloseItemList; }

	// 获取许愿星奖励配置
	const StarRewardInfoTable & getStarRewardConfig(){ return m_StarRewardConfig; }

	// 获取搓神灯配置
	const CDustRewardConfig & getDustRewardConfig(){ return m_DustRewardConfig; }
	unsigned int GetDustStoryID(){ return m_DustRewardConfig.m_nStoryID; }

	// 获取水晶配置
	const CCrystalConfig & getCrystalConfig(){ return m_CrystalConfig; }
	unsigned int GetCrystalInitialValue(){ return m_CrystalConfig.m_nInitialValue; }
    unsigned int GetFavorableBuyVitCount();

	// 获取水晶恢复时间
	 unsigned int GetCrystalRestoreTime(){ return m_CrystalConfig.m_nRestoreTime; }

	// 获取水晶的最大值
	 unsigned int GetMaxCrystalValue(){ return m_CrystalConfig.m_nMaxValue; }

	// 添加/获取购买水晶配置
	void AddBuyCrystalConfig( const CBuyCrystalConfig & config ){ m_BuyCrystalConfig[config.m_nIndex] = config; }
	const BuyCrystalConfigTable & GetBuyCrystalConfig(){ return m_BuyCrystalConfig; }
	CBuyCrystalConfig * GetBuyCrystalConfig( unsigned int nIndex );

	// 添加/获取副本信息
	void  addCrystalConfig( const CCrystalConfig & config ){ m_CrystalConfig = config; }

	// 添加/获取跳过关卡物品
	void AddSkipParcloseItem( const list<CItem> & items ){ m_SkipParcloseItem = items;}
	const std::list<CItem> & GetSkipParcloseItem(){ return m_SkipParcloseItem; }

	// 添加关卡掉落
	void GetMainParcloseDrop( unsigned int nParcloseID, std::vector<CItem> & maleitems, std::vector<CItem> & femaleitems, std::vector<DebrisReward> & debrisreward );
    void GetThemeParcloseDrop( unsigned int nThemeID, unsigned int nParcloseID, std::vector<CItem> & maleitems, std::vector<CItem> & femaleitems, std::vector<DebrisReward> & debrisreward );
    void CalcMainlineMaleParcloseDrop( unsigned int nParcloseID, std::list<CItem> & listMaleItem, std::list<DebrisReward> & listDebris );
    void CalcMainlineFemaleParcloseDrop( unsigned int nParcloseID, std::list<CItem> & listFemaleItem, std::list<DebrisReward> & listDebris );
    void CalThemeMaleParcloseDrop( unsigned int nThemeID, unsigned int nParcloseID, std::list<CItem> & listMaleItem, std::list<DebrisReward> & listDebris);
    void CalThemeFemaleParcloseDrop( unsigned int nThemeID, unsigned int nParcloseID, std::list<CItem> & listFemaleItem, std::list<DebrisReward> & listDebris);
    void CalcParcloseDrop( int nSex, EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID, std::list<CItem> & listItem, std::list<DebrisReward> & listDebris);

	// 加载经验配置
	void AddParcloseGradeExpConfig( int nMusicLevel, const std::map<int,int> & exp );
	void AddParcloseBaseExpConfig( unsigned int nFailExp, unsigned int nWinBaseExp );

	// 获取经验
	unsigned int GetParcloseCalcExp( bool isWin, int nGrade, unsigned int nMusicLevel );
	unsigned int GetDungeonGradeExp( int nGrade, unsigned int nMusicLevel );

	// 邮件
	void AddRewardMailInfo( const CMagicLampRewardMail & mail ){ m_RewardMail = mail; }
	const CMagicLampRewardMail & GetRewardMail(){ return m_RewardMail; }

	// 功能说明
	void AddDesc( const CMagicLampDesc & desc ){ m_Desc = desc; }
	const CMagicLampDesc & GetDesc(){ return m_Desc; }

    // 获取邀请好友最大次数
    unsigned int GetMaxInviteFriendNum(){ return m_Desc.m_nMaxInviteFriendNum; }

    // 添加/获取
    void AddPublicityPictureConfig( CPublicityPictureConfig & picture );
    const std::vector<CPublicityPictureConfig> & GetPublicityPictureConfig(){ return m_PublicityPictureConfig; }

    // 添加二期优化相关配置
    void AddGiveGetVitConfig(unsigned int nOnceGiveVitNum, unsigned int nResetTime, unsigned int nGetVitMaxNum);
    void AddQuickFinishConfig(bool bOpenToNotVip, const CItem & costItem);

    // 赠送和接收体力重置时间
    unsigned int GetNextDayResetTime() const;

    // 每次赠送体力数目
    unsigned int OnceGiveVitNum() const;

    // 获取关卡扫荡物品
    CItem GetQuickFinishItem();

    // 获取每天可以获取的最大体力值
    unsigned int GetDailyGetVitMaxNum() const;

    // 非VIP玩家是否可以扫荡
    bool QucikFinishOpenToNotVip();

private:
    StarRewardInfoTable             m_StarRewardConfig;						 // 许愿星奖励
    CDustRewardConfig               m_DustRewardConfig;					     // 搓神灯奖励
    std::list<CItem>                m_ParcloseItemList;						 // 关卡可使用道具列表
    CCrystalConfig                  m_CrystalConfig;						 // 水晶配置
    BuyCrystalConfigTable           m_BuyCrystalConfig;						 // 购买水晶配置
    std::list<CItem>                m_SkipParcloseItem;						 // 跳过关卡使用道具列表
    std::map<int,std::map<int,int> >    m_ParcloseExpConfig;
    unsigned int	m_nParcloseFailExp;
    unsigned int    m_nParcloseWinBaseExp;
    CMagicLampRewardMail            m_RewardMail;			                // 奖励邮件
    CMagicLampDesc                  m_Desc;				                    // 功能介绍
    std::vector<CPublicityPictureConfig>    m_PublicityPictureConfig;       // 宣传图
    unsigned int                    m_nNextDayResetTime;                    // 赠送和接收体力重置时间
    unsigned int                    m_nOnceGiveVitNum;                      // 赠送体力数目
    unsigned int                    m_nDailyGetVitMaxNum;                   // 每天收取体力的最大值
    CItem                           m_QuickFinishCostItem;                  // 关卡扫荡物品
    bool                            m_bQuickFinishOpenToNotVip;             // 扫荡是否对非VIP玩家开放
};


#endif
