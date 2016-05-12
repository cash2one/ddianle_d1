#ifndef __COUPLE_MANAGER_H__
#define __COUPLE_MANAGER_H__

#include "../Rank/RankMgr.h"
#include <deque>
//#include "../../datastructure/DataStruct_Couple.h"
#include "../../datastructure/DataStruct_Couple.h"

#define MAX_BLESSINGRANK_COUNT 200

class CBlessingRankInfo : public CRankInfo
{
public:
	CBlessingRankInfo( const CBlessingValueRankInfo & rankinfo );
	virtual ~CBlessingRankInfo();

public:
	CBlessingValueRankInfo     m_BlessingValueRankInfo;
};

// 排行榜的大小应该放在子类中设置
class CBlessingValueRankMgr : public CRankMgr
{
public:
	CBlessingValueRankMgr();
	virtual ~CBlessingValueRankMgr();

public :
	// 初始化
	void LoadRankInfo();

	// 加载祝福值排行信息
	void LoadRankInfoSuc();

public :
	// 更新
	void UpdateBlessingValueInfo( CBlessingValueRankInfo & info );

	// 更新角色名字
	void UpdatePlayerNickname( unsigned int nPairID, unsigned int nRoleID, std::string & strNickname );

	// 删除
	void DelBlessingValueInfo( unsigned int nPairID );

	// 获取排名
	void GetBlessingRank( int nBeginRank, int nEndRank, std::list<CRankInfo*> & bList );

	// 获取全部数据
	void GetBlessingRankList(int nPage, int nPageCount, std::vector<CBlessingValueRankInfo> & rList);

	// 获取自己的排名
	int GetSelfRank( unsigned int nKey );

	// 获取排行总个数
	unsigned int GetRankTotalCount();

private :
	std::vector<CBlessingValueRankInfo>        m_RankVec; 
	bool	m_bLoadRankInfoSuc;
};


class CWeddingRecord
{
public :
	CWeddingRecord(){};
	virtual ~CWeddingRecord(){};

	enum
	{
		EWeddingRecordCount = 20
	};

public :
	// 初始化
	void Initialize(){};

public :
	// 添加记录
	void AddRecord( const WeddingRecordInfo & record );

	// 删除记录
	void DeleteRecord( unsigned int nPairID );

	// 更新婚礼是否进行中开关
	void UpdateRecordIsRunning( unsigned int nPairID, bool bIsRunning );

	// 更新祝福值
	void UpdateBlessingValue( unsigned int nPairID, unsigned int nBlessingValue );

	// 更新角色名称
	void UpdatePlayerNickname( unsigned int nPairID, unsigned int nRoleID, std::string & strNickname );

	// 添加角色祝福ID
	void AddBlessingRoleIDInList( unsigned int nPairID, unsigned int nRoleID );

	// 获取所有记录值
	const std::deque<WeddingRecordInfo> & GetRecordList();
	void GetRecordList( std::vector<WeddingRecordInfo> & record );

	// 查找记录值
    WeddingRecordInfo * FindWeddingRecord( unsigned int nPairID );

private :
	// 是否在记录中
	bool IsInRecord( unsigned int nPairID );

	bool IsInList( unsigned int nRoleID, std::vector<unsigned int> & IDList );

private :
	std::deque<WeddingRecordInfo>    m_WiddingRecord;

};

#endif
