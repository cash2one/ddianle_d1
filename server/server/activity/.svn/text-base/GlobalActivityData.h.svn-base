#ifndef	__GLOBAL_ACTIVITY_DATA_H__
#define __GLOBAL_ACTIVITY_DATA_H__

//活动相关的全局数据
class CGlobalActivityData
{
protected:
	CGlobalActivityData();
	virtual ~CGlobalActivityData();

public:
	static CGlobalActivityData & Instance();
public:
	//增加额外经验奖励比率
	void IncreaseExtraExpRate(unsigned short nDeltaExpRate);
	//减少额外经验奖励比率
	void DecreaseExtraExpRate(unsigned short nDeltaExpRate);
	//获取额外经验奖励比率，百分比
	unsigned int GetExtraExpRate(){return m_nExtraExpRate;}
private:
	unsigned int			m_nExtraExpRate;	//活动额外经验奖励比率，百分比
};

#endif


