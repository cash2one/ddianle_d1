#ifndef		__QUESTTARGETSEND_H__
#define		__QUESTTARGETSEND_H__

class CParamPool;

class CQuestTargetSend
{
public:
	CQuestTargetSend();
	~CQuestTargetSend();
public:
	void doEncode(CParamPool &IOBuff);
public:
	unsigned char	m_nType;				// 歌曲 物品消耗 物品搜集
	unsigned int	m_nIndex;				// 歌曲ID 物品ID 物品ID等，详见eTargetType
	unsigned int	m_nRequiredCount;		// 任务要求的次数
	unsigned int	m_nCompletedCount;		// 已完成的次数
};

#endif

// end of file


