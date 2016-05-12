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

// ���а�Ĵ�СӦ�÷�������������
class CBlessingValueRankMgr : public CRankMgr
{
public:
	CBlessingValueRankMgr();
	virtual ~CBlessingValueRankMgr();

public :
	// ��ʼ��
	void LoadRankInfo();

	// ����ף��ֵ������Ϣ
	void LoadRankInfoSuc();

public :
	// ����
	void UpdateBlessingValueInfo( CBlessingValueRankInfo & info );

	// ���½�ɫ����
	void UpdatePlayerNickname( unsigned int nPairID, unsigned int nRoleID, std::string & strNickname );

	// ɾ��
	void DelBlessingValueInfo( unsigned int nPairID );

	// ��ȡ����
	void GetBlessingRank( int nBeginRank, int nEndRank, std::list<CRankInfo*> & bList );

	// ��ȡȫ������
	void GetBlessingRankList(int nPage, int nPageCount, std::vector<CBlessingValueRankInfo> & rList);

	// ��ȡ�Լ�������
	int GetSelfRank( unsigned int nKey );

	// ��ȡ�����ܸ���
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
	// ��ʼ��
	void Initialize(){};

public :
	// ��Ӽ�¼
	void AddRecord( const WeddingRecordInfo & record );

	// ɾ����¼
	void DeleteRecord( unsigned int nPairID );

	// ���»����Ƿ�����п���
	void UpdateRecordIsRunning( unsigned int nPairID, bool bIsRunning );

	// ����ף��ֵ
	void UpdateBlessingValue( unsigned int nPairID, unsigned int nBlessingValue );

	// ���½�ɫ����
	void UpdatePlayerNickname( unsigned int nPairID, unsigned int nRoleID, std::string & strNickname );

	// ��ӽ�ɫף��ID
	void AddBlessingRoleIDInList( unsigned int nPairID, unsigned int nRoleID );

	// ��ȡ���м�¼ֵ
	const std::deque<WeddingRecordInfo> & GetRecordList();
	void GetRecordList( std::vector<WeddingRecordInfo> & record );

	// ���Ҽ�¼ֵ
    WeddingRecordInfo * FindWeddingRecord( unsigned int nPairID );

private :
	// �Ƿ��ڼ�¼��
	bool IsInRecord( unsigned int nPairID );

	bool IsInList( unsigned int nRoleID, std::vector<unsigned int> & IDList );

private :
	std::deque<WeddingRecordInfo>    m_WiddingRecord;

};

#endif
