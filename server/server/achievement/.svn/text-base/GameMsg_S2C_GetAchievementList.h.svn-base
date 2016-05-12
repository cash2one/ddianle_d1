#ifndef	__GAMEMSG_S2C_GETACHIEVEMENTLIST_H__
#define __GAMEMSG_S2C_GETACHIEVEMENTLIST_H__

#include <string>
#include <list>
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"

class CParamPool;
class AchievementConfig;
class CEntityAchievementComponent;
class IRequirementHandler;

using namespace std;

class CAchievementRewardItem
{
public:
	CAchievementRewardItem(itemtype_t nItemID = 0, unsigned short nCount = 0);
	~CAchievementRewardItem();
public:
	void doEncode(CParamPool &IOBuff);
public:
	itemtype_t m_nItemID;
	unsigned short m_nCount;
};

class	CAchievementlistEntry 
{
public:
	CAchievementlistEntry(unsigned short m_nSeriesID = 0, unsigned short	nAchievementID = 0, unsigned int nCurrentValue = 0, unsigned int nTime = 0, unsigned char nState = 0);
	~CAchievementlistEntry();
public:
	unsigned short	m_nAchievementID;
	unsigned short	m_nSeriesID;
	unsigned int	m_nCompletedTime;	
	unsigned int	m_nCurrentValue;
	unsigned char	m_nState;					// 0表示进行中，1表示完成未领奖
public:
	void doEncode(CParamPool &IOBuff);
	bool operator==(const CAchievementlistEntry & rhs);
};

class GameMsg_S2C_GetAchievementList:
public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetAchievementList);
	GameMsg_S2C_GetAchievementList();
	GameMsg_S2C_GetAchievementList(const CEntityAchievementComponent *pAchievement);
	~GameMsg_S2C_GetAchievementList();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	list<CAchievementlistEntry>		m_listAchievement;
};

#endif
// end of file




