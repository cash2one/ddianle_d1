#ifndef _RANK_MGR_H_
#define _RANK_MGR_H_

#include <list>
#include <map>
#include <set>
#include <vector>
#include <string>

class CRankMgr;
class CRankRow;
class CRankInfo
{
	friend class CRankMgr;
	friend class CRankRow;
public:
	CRankInfo(unsigned int nKey, unsigned int nValue)
	{
		m_nKey = nKey;
		m_nValue = nValue;
	}
	virtual ~CRankInfo(){}

    unsigned int GetKey() const { return m_nKey; }
    unsigned int GetValue() const { return m_nValue; }

    virtual void SetValue( unsigned int nValue );
    virtual bool ChangeValue( int nChangeValue );

protected:
	unsigned int	m_nKey;			//Key
	unsigned int	m_nValue;		//值
};

class CRankRow
{
	friend class CRankMgr;
private:
	CRankRow(unsigned int nRowID, unsigned int nMaxRowCount);
	~CRankRow();

private:
	//添加元素，返回元素排名，如果返回0表示添加失败
	unsigned int AddRankInfo(CRankInfo * pRankInfo);
	//向前面插入记录
	bool PushFront(CRankInfo * pRankInfo);
	//向后面插入记录
	bool PushBack(CRankInfo * pRankInfo);

	//弹出Key对应元素，返回之前排名
	CRankInfo * PopRankInfo(unsigned int nKey, unsigned int & rnRanking);
	//弹出第一个元素
	CRankInfo * PopFront();
	//弹出最后一个元素
	CRankInfo * PopBack();

    // 第一个元素
    const CRankInfo* Front() const;
    // 检查是否在排行榜中
    bool IsInRow(unsigned int nKey);

	//根据Key获取玩家排名
	unsigned int GetRank(unsigned int nKey);
	//获取范围,返回实际获取的数量
	unsigned int GetRange(unsigned int nStartPos, unsigned int nCount, std::list< CRankInfo * > & rRankInfoList) const;

	//是否已经满
	bool IsFull(){return m_nCurrentCount == m_cnMaxRowCount;}
	//是否空
	bool IsEmpty() const {return m_nCurrentCount == 0;}
	//获取队列最大值
	unsigned int GetBeginValue(){return m_nBeginValue;}
	//获取队列最小值
	unsigned int GetEndValue(){return m_nEndValue;}
	//获取队列编号
	unsigned int GetRowID(){return m_cnRowID;}
	//获取排行信息
	CRankInfo * GetRankInfo(unsigned int nKey);
	//查找第一个小于Value的位置
	int Find(unsigned int nValue);

private:
	unsigned int m_nBeginValue;					//队列中元素最大值
	unsigned int m_nEndValue;					//队列中元素最小值
	unsigned int m_nCurrentCount;				//当前队列的大小
	unsigned int m_cnRowID;						//队列编号
	unsigned int m_cnMaxRowCount;				//每个Row的最大容纳量
	CRankInfo ** m_aRankInfo;
	std::map< unsigned int, CRankInfo * > m_KeyInfoMap;
};

class CRankMgr
{
public:
	enum
	{
		EDefaultMaxRowCount = 20000
	};
public:
	CRankMgr();
	CRankMgr(unsigned int nMaxRowCount);
    CRankMgr(unsigned int nMaxRowCount, unsigned int nMaxTotalCount);
	virtual ~CRankMgr();
	
public:
	//数据重置
	void Reset();
	//添加元素，返回名次，如果返回值为0表示插入失败
	unsigned int AddNewRankInfo(CRankInfo * pRankInfo);
	//获取元素
	CRankInfo * GetRankInfo(unsigned int nKey);
	//移除元素，需要管理内存
	CRankInfo * RemoveRankInfo(unsigned int nKey);
	//更新排行值
	CRankInfo * UpdateRankInfo(unsigned int nKey, unsigned int nValue, unsigned int & rnSrcRank, unsigned int & rnDesRank);
	//改变排行值
	CRankInfo * ChangeRankInfo(unsigned int nKey, int nChangedValue, unsigned int & rnSrcRank, unsigned int & rnDesRank);

public:
	//总数
	unsigned int TotalCount() const;
	//根据Key获取排名
	unsigned int GetRankByKey(unsigned int nKey);
	//获取范围信息，闭区间[nBeginRank, nEndRank]
	void GetRange(unsigned int nBeginRank, unsigned int nEndRank, std::list< CRankInfo * > & rRankInfoList) const;

    const CRankInfo* Front() const;
    // 获取最小值
    unsigned int GetMinValue() const;

    // 检查是否在排行榜中
    bool IsInRank(unsigned int nKey);

    // 移除最后一个排名
    void PopBack();

private:
	//根据值查找该插入的列，返回NULL表示需要新增列了
	CRankRow * _GetRowToInsert_Up(unsigned int nValue);
	//根据值查找该插入的列，返回NULL表示出错了。为名次下降时查找用。
	CRankRow * _GetRowToInsert_Down(unsigned int nValue, unsigned int nSrcRowID);
	//向上插入一条记录，当名次上升时使用，返回插入后的排名
	unsigned int _InsertRankInfo_Up(CRankInfo * pRankInfo);
	//向下插入一条记录，当名次下降时使用，返回插入后的排名
	unsigned int _InsertRankInfo_Down(CRankInfo * pRankInfo, unsigned int nSrcRowID);
	//根据角色编号获取Row
	CRankRow * _GetRowByKey(unsigned int nKey);
	//根据角色编号获取排名信息
	CRankInfo * _GetRankInfoByKey(unsigned int nKey);
	//向下整理排行榜，如果增加了新的列，返回true，否则false
	bool _SortOutDown(unsigned int nBeginRow, CRankInfo * pExtraRankInfo);

	std::vector< CRankRow * > m_RankRowVec;
	std::map< unsigned int, CRankRow * > m_KeyRowMap;
	std::map< unsigned int, CRankInfo * > m_KeyInfoMap;

	unsigned int m_nTotalCount;								//总数
    unsigned int m_nMaxTotalCount;                          // 最大总数
	unsigned int m_cnMaxRowCount;							//每个Row的最大容纳量
};


//////////////////////////////////////////////////////////////////////////
// 上面的排行榜太复杂了, 这里写个简单版本的, 建议用下面这个, 小数据量性能可接受
//////////////////////////////////////////////////////////////////////////
class CRankValue_Base
{
public:
	CRankValue_Base()
	{
		m_nID = 0;
		m_nScore = 0;
		m_nRankTide = 0;
		m_nUpdateTime = 0;
		m_nLastRank = 0;
	}
	virtual ~CRankValue_Base()
	{
	}

	bool operator < (const CRankValue_Base &that) const
	{
		if (m_nID == that.m_nID)
		{
			return false;
		}
		else if (m_nScore > that.m_nScore)
		{
			return true;// 排行榜要倒序排分数
		}
		else if (m_nScore < that.m_nScore)
		{
			return false;
		}
		else if (m_nUpdateTime < that.m_nUpdateTime)
		{
			return true;
		}
		else if (m_nUpdateTime > that.m_nUpdateTime)
		{
			return false;
		}
		else
		{
			return m_nID < that.m_nID;
		}
	}

public:
	// 这个类占用内存要尽可能地小，以节省内存(Manager中的set和map都是直接存储值的，而不是指针)
	int m_nID;		// ID
	int m_nScore;	// 分数
	mutable int m_nRankTide;// 排行变化趋势
	unsigned int m_nUpdateTime; // 上次更新时间
	mutable std::string m_strName;
	mutable int m_nLastRank;	// 上次排名
};

class CRankManager_Base
{
public:
	CRankManager_Base()
	{
	}
	virtual ~CRankManager_Base()
	{
	}

public:
	// 增加一个分数
	void Add(const CRankValue_Base &value)
	{
		m_mapValue.insert(std::make_pair(value.m_nID, value));
		m_setRank.insert(value);
	}

	// 删除一个分数
	void Remove(int nID)
	{
		std::map<int, CRankValue_Base>::iterator itMap = m_mapValue.find(nID);
		if (itMap != m_mapValue.end())
		{
			m_setRank.erase(itMap->second);
			m_mapValue.erase(itMap);
		}
	}

	// 更新一个分数
	void Set(CRankValue_Base &value)
	{
		std::map<int, CRankValue_Base>::iterator itMap = m_mapValue.find(value.m_nID);
		if (itMap != m_mapValue.end())
		{
			value.m_nLastRank = itMap->second.m_nLastRank;
			value.m_nRankTide = itMap->second.m_nRankTide;
			m_setRank.erase(itMap->second);
			m_mapValue[value.m_nID] = value;
			m_setRank.insert(value);
		}
		else
		{
			Add(value);
		}
	}

	// 查找一个现有的数据
	CRankValue_Base *Find(int nID)
	{
		std::map<int, CRankValue_Base>::iterator itMap = m_mapValue.find(nID);
		if (itMap != m_mapValue.end())
		{
			return &itMap->second;
		}
		return NULL;
	}


	// 第一个排名
	const CRankValue_Base *Head()
	{
		if (!m_setRank.empty())
		{
			return &(*(m_setRank.begin()));
		}
		return NULL;
	}

	// 最后一个排名
	const CRankValue_Base *Tail()
	{
		if (!m_setRank.empty())
		{
			return &(*(m_setRank.rbegin()));
		}
		return NULL;
	}

	// 查询分数的对应排名
	int Rank(int nID)
	{
		std::map<int, CRankValue_Base>::iterator itMap = m_mapValue.find(nID);
		if (itMap != m_mapValue.end())
		{
			std::set<CRankValue_Base>::iterator itSet = m_setRank.find(itMap->second);
			return (int)std::distance(m_setRank.begin(), itSet) + 1;
		}
		return -1;
	}

	// 清空排行榜
	void Clear()
	{
		m_setRank.clear();
		m_mapValue.clear();
	}

	// 排行榜排行量
	int Count() const
	{
		return (int)m_setRank.size();
	}

	// 从startIndex开始取count个排行数据
	void Range(int nStartIndex, int nCount, std::list<CRankValue_Base> &list)
	{
		int nSize = (int)m_setRank.size();
		if (nStartIndex < nSize && nStartIndex >= 0)
		{
			std::set<CRankValue_Base>::iterator itSet = m_setRank.begin();
			std::advance(itSet, nStartIndex);
			for (int i = 0; itSet != m_setRank.end() && i < nCount; ++i, ++itSet)
			{
				list.push_back(*itSet);
			}
		}
	}

	// 全部排名
	const std::set<CRankValue_Base> &AllRank() const
	{
		return m_setRank;
	}

	// 更新排行数据, 这个函数开销最大
	int Update(CRankValue_Base &value)
	{
		int oldRank = 0;
		int newRank = 0;
		if (m_mapValue.find(value.m_nID) != m_mapValue.end())
		{
			CRankValue_Base &oldValue = m_mapValue[value.m_nID];
			std::set<CRankValue_Base>::iterator itSet = m_setRank.find(oldValue);

			oldRank = (int)std::distance(m_setRank.begin(), itSet) + 1;
			m_setRank.erase(oldValue);

			m_mapValue[value.m_nID] = value;
			std::set<CRankValue_Base>::iterator itNew = m_setRank.insert(value).first;
			
			newRank = (int)std::distance(m_setRank.begin(), itNew) + 1;
			m_setRank.find(value)->m_nRankTide = newRank - oldRank;

			itSet = m_setRank.begin();
			std::advance(itSet, oldRank - 1);
			if (newRank > oldRank)
			{
				for (; itSet != itNew; ++itSet)
				{
					itSet->m_nRankTide = itSet->m_nRankTide + 1;
				}
				itNew->m_nRankTide = itSet->m_nRankTide - (newRank - oldRank);
			}
			else if (oldRank > newRank)
			{
				itNew->m_nRankTide = itSet->m_nRankTide + (oldRank - newRank);
				for (++itNew; itNew != itSet && itNew != m_setRank.end(); ++itNew)
				{
					itNew->m_nRankTide = itNew->m_nRankTide - 1;
				}
			}
		}
		else
		{
			m_mapValue[value.m_nID] = value;
			std::set<CRankValue_Base>::iterator itSet = m_setRank.insert(value).first;
			newRank = (int)std::distance(m_setRank.begin(), itSet) + 1;
			value.m_nRankTide = newRank;
			for (++itSet; itSet != m_setRank.end(); ++itSet)
			{
				itSet->m_nRankTide = itSet->m_nRankTide - 1;
			}
		}
		return newRank;
	}

	// 定期刷新排行榜，定期更新排行变化趋势
	void Update()
	{
		std::set<CRankValue_Base>::iterator itSet = m_setRank.begin();
		for (int nRank = 1; itSet != m_setRank.end(); ++itSet, ++nRank)
		{
			if (itSet->m_nLastRank > 0)
			{
				itSet->m_nRankTide = itSet->m_nLastRank - nRank;
			}
			itSet->m_nLastRank = nRank;

			m_mapValue[itSet->m_nID].m_nLastRank = itSet->m_nLastRank;
			m_mapValue[itSet->m_nID].m_nRankTide = itSet->m_nRankTide;
		}
	}

protected:
	std::set<CRankValue_Base> m_setRank;
	std::map<int, CRankValue_Base> m_mapValue;
};

#endif //_RANK_MGR_H_

