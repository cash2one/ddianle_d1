#include <algorithm>
#include "PaymentInfoMgr.h"
#include "../item/ItemProcess.h"
#include "../../datastructure/RoleEntity.h"
#include "../../socket/Log.h"
#include "../attribute/EntityAttributeComponent.h"

#include "../util/CommonFunc.h"
#include "../../socket/SocketMgr.h"
#include "../SystemSetting/SystemSettingMgr.h"
#include "../login/GameMsg_Payment.h"


static std::list<CItem> s_emptyItem;

PaymentInfoMgr::PaymentInfoMgr()
{
}

PaymentInfoMgr::~PaymentInfoMgr()
{
}

PaymentInfoMgr & PaymentInfoMgr::Instance()
{
	static PaymentInfoMgr s_PaymentInfoMgr;

	return s_PaymentInfoMgr;
}

void PaymentInfoMgr::ReadFirstPaymentReward( Version_Type versionType, std::list<CItem>& listMaleReward, std::list<CItem>& listFemaleReward,
	int nChargeLevels, std::vector<CChargePlan>& vecChargePlan, string &strMailTitle, string & strMailContent,
	time_t startTime, time_t endTime, std::string& strMaleItem, std::string& strFemaleItem )
{
	FirstPaymentInfo	Info;

	Info.m_StartTime = (int)startTime;			// 起止时间
	Info.m_EndTime = (int)endTime;

	Info.m_listMaleReward = listMaleReward;		// 道具奖励
	Info.m_listFemaleReward = listFemaleReward;

	Info.m_strMaleReward = strMaleItem;			// 道具奖励,物品列表
	Info.m_strFemaleReward = strFemaleItem;

	Info.m_nChargeLevels = nChargeLevels;		// 金券、Mcoin奖励
	Info.m_vecChargePlan = vecChargePlan;

	Info.m_strMailTitle = strMailTitle;			// 邮件
	Info.m_strMailContent = strMailContent;

	if( versionType == Version_Free )
	{
		m_FreeVerFirstPaymentInfo.push_back( Info );
	}
	else
	{
		m_ChargeVerFirstPaymentInfo.push_back( Info );
	}
}

bool	PaymentInfoMgr::FirstChargeRewardIsOpen( Version_Type versionType, int nRechargeTime )
{
	if( versionType == Version_Free )
	{
		std::vector<FirstPaymentInfo>::iterator it = m_FreeVerFirstPaymentInfo.begin( );
		for( ; it != m_FreeVerFirstPaymentInfo.end( ); it++ )
		{
			if( nRechargeTime > it->m_StartTime && nRechargeTime < it->m_EndTime )
			{
				return true;
			}
		}
	}
	else if( versionType == Version_Charge )
	{
		std::vector<FirstPaymentInfo>::iterator it = m_ChargeVerFirstPaymentInfo.begin( );
		for( ; it != m_ChargeVerFirstPaymentInfo.end( ); it++ )
		{
			if( nRechargeTime > it->m_StartTime && nRechargeTime < it->m_EndTime )
			{
				return true;
			}
		}
	}
	else
	{
		WriteLog(LOGLEVEL_ERROR, "[ERROR]-[@PaymentInfoMgr::FirstChargeRewardIsOpen]-[versionType has exceptional situation!]" );
	}

	return false;
}



bool PaymentInfoMgr::RechargeRewardIsOpen( Version_Type versionType, int nRechargeTime )
{
	std::vector<ReChargeInfo>::iterator itr;
	if( versionType == Version_Free )
	{
		for( itr = m_FreeVersionRechargeInfo.begin( ); itr != m_FreeVersionRechargeInfo.end( ); itr++ )
		{
			if( nRechargeTime > itr->m_StartTime && nRechargeTime < itr->m_EndTime )
			{
				return true;
			}
		}
	}
	else
	{
		for( itr = m_ChargeVersionRechargeInfo.begin( ); itr != m_ChargeVersionRechargeInfo.end( ); itr++ )
		{
			if( nRechargeTime > itr->m_StartTime && nRechargeTime < itr->m_EndTime )
			{
				return true;
			}
		}
	}
	return false;
}


ReChargeInfo* PaymentInfoMgr::GetRechargeInfo( Version_Type versionType, int nRechargeTime )
{
	if( versionType == Version_Free )
	{
		std::vector<ReChargeInfo>::iterator it = m_FreeVersionRechargeInfo.begin( );
		for( ; it != m_FreeVersionRechargeInfo.end( ); it++ )
		{
			if( nRechargeTime > it->m_StartTime && nRechargeTime < it->m_EndTime )
			{
				return &( *it );
			}
		}
	}
	else
	{
		std::vector<ReChargeInfo>::iterator it = m_ChargeVersionRechargeInfo.begin( );
		for( ; it != m_ChargeVersionRechargeInfo.end( ); it++ )
		{
			if( nRechargeTime > it->m_StartTime && nRechargeTime < it->m_EndTime )
			{
				return &( *it );
			}
		}
	}
	return NULL;
}

bool PaymentInfoMgr::GetRechargeReward( int &nMoneyReward, int &nMcoinReward, std::string &strMailTitle, std::string & strMailContent, 
	const int nChargeAmount, Version_Type versionType, int nRechargeTime )
{
	bool bRes = true;
	nMoneyReward = nMcoinReward = 0;

	ReChargeInfo* pCur = GetRechargeInfo( versionType, nRechargeTime );

	if( pCur != NULL && pCur->m_vecChargePlan.size( ) > 0 )
	{
		strMailTitle = pCur->m_strMailTitle;
		strMailContent = pCur->m_strMailContent;

		CalculateReward( nMoneyReward, nMcoinReward, nChargeAmount, pCur->m_vecChargePlan.size( ), pCur->m_vecChargePlan, versionType, nRechargeTime );
	}
	else
	{
		bRes = false;
		WriteLog(LOGLEVEL_ERROR, "[ERROR]-[@PaymentInfoMgr::GetRechargeReward]-[ReChargeInfo* pCur is NULL]" );
	}

	return bRes;
}

void PaymentInfoMgr::GetRechargeRewardPlan( std::vector<CChargePlan> & vecChargePlan, Version_Type versionType, int nRechargeTime )
{
	ReChargeInfo* pCur = GetRechargeInfo( versionType, nRechargeTime );
	if( pCur != NULL )
	{
		vecChargePlan = pCur->m_vecChargePlan;
	}
	else
	{
		WriteLog(LOGLEVEL_ERROR, "Error: PaymentInfoMgr::RewardCurrencyPlanForClient error pCur is Null" );
	}
}

FirstPaymentInfo*	PaymentInfoMgr::GetCurrentPaymentInfo(Version_Type versionType)
{
	int nTime = (int)time(NULL);
	return GetFirstPaymentInfo(versionType,nTime);
}

FirstPaymentInfo* PaymentInfoMgr::GetFirstPaymentInfo(Version_Type versionType,int nRechargeTime)
{
	if (versionType == Version_Free)
	{
		std::vector<FirstPaymentInfo>::iterator it = m_FreeVerFirstPaymentInfo.begin();
		for (;it != m_FreeVerFirstPaymentInfo.end() ; it++)
		{
			if (nRechargeTime > it->m_StartTime && nRechargeTime < it->m_EndTime )
			{
				return &(*it);
			}
		}
	}
	else if (versionType == Version_Charge)
	{
		std::vector<FirstPaymentInfo>::iterator it = m_ChargeVerFirstPaymentInfo.begin();
		for (;it != m_ChargeVerFirstPaymentInfo.end() ; it++)
		{
			if (nRechargeTime > it->m_StartTime && nRechargeTime < it->m_EndTime )
			{
				return &(*it);
			}
		}
	}
	else
	{
		WriteLog(LOGLEVEL_ERROR, "[ERROR]-[@PaymentInfoMgr::GetFirstPaymentInfo]-[version Type is %d]", versionType );
		return NULL;
	}
	return NULL;
}

std::string PaymentInfoMgr::MailTitle(Version_Type versionType,int nRechargeTime)
{
	FirstPaymentInfo* pCur = GetFirstPaymentInfo(versionType,nRechargeTime);
	if (pCur != NULL)
	{
		return pCur->m_strMailTitle;
	}
	return "";
}

std::string PaymentInfoMgr::MailContent(Version_Type versionType,int nRechargeTime)
{
	FirstPaymentInfo* pCur = GetFirstPaymentInfo(versionType,nRechargeTime);
	if (pCur != NULL)
	{
		return pCur->m_strMailContent;
	}
	return "";
}

const std::list<CItem>& PaymentInfoMgr::MaleRewards(Version_Type versionType,int nRechargeTime)
{
	FirstPaymentInfo* pCur = GetFirstPaymentInfo(versionType,nRechargeTime);
	if (pCur != NULL)
	{
		return pCur->m_listMaleReward;
	}
	return s_emptyItem;
}

const std::list<CItem>& PaymentInfoMgr::FemaleRewards(Version_Type versionType,int nRechargeTime)
{
	FirstPaymentInfo* pCur = GetFirstPaymentInfo(versionType,nRechargeTime);
	if (pCur != NULL)
	{
		return pCur->m_listFemaleReward;
	}
	return s_emptyItem;
}

bool PaymentInfoMgr::GetFirstChargeReward( int &nMoneyReward, int &nMcoinReward, const int nChargeAmount, Version_Type versionType, int nRechargeTime )
{
	bool bRes = true;
	nMoneyReward = nMcoinReward = 0;

	FirstPaymentInfo* pCur = GetFirstPaymentInfo( versionType, nRechargeTime );

	if( pCur != NULL && pCur->m_vecChargePlan.size( ) > 0 )
	{
		CalculateReward( nMoneyReward, nMcoinReward, nChargeAmount, pCur->m_vecChargePlan.size( ), pCur->m_vecChargePlan, versionType, nRechargeTime );
	}
	else
	{
		bRes = false;
		WriteLog(LOGLEVEL_ERROR, "[ERROR]-[@PaymentInfoMgr::GetFirstChargeReward(]-[pCur is Null]" );
	}

	return bRes;
}

void PaymentInfoMgr::GetFirstChargeRewardPlan( std::vector<CChargePlan> & vecChargePlan, Version_Type versionType, int nRechargeTime )
{
	FirstPaymentInfo* pCur = GetFirstPaymentInfo( versionType, nRechargeTime );
	if( pCur != NULL )
	{
		vecChargePlan = pCur->m_vecChargePlan;
	}
	else
	{
		WriteLog(LOGLEVEL_ERROR, "Error: PaymentInfoMgr::RewardCurrencyPlanForClient error pCur is Null" );
	}
}

int	PaymentInfoMgr::GetFirstChargeStartTime( Version_Type versionType, int nRechargeTime )
{
	FirstPaymentInfo* pCur = GetFirstPaymentInfo( versionType, nRechargeTime );
	if (pCur != NULL)
	{
		return pCur->m_StartTime;
	}
	return 0;
}

void PaymentInfoMgr::ReadRechargeReward( Version_Type versionType,	int nChargeLevels, std::vector<CChargePlan>& vecChargePlan, 
	time_t startTime, time_t endTime, const std::string &strMailTitle, const std::string &strMailContent)
{
	ReChargeInfo Info;

	Info.m_nChargeLevels = nChargeLevels;
	Info.m_vecChargePlan = vecChargePlan;

	Info.m_StartTime = (int)startTime;
	Info.m_EndTime = (int)endTime;
	Info.m_strMailTitle = strMailTitle;
	Info.m_strMailContent = strMailContent;

	if (versionType == Version_Free) 
	{
		m_FreeVersionRechargeInfo.push_back( Info );
	}
	else 
	{
		m_ChargeVersionRechargeInfo.push_back( Info );
	}
}

void PaymentInfoMgr::CalculateReward( int &nMoneyReward, int &nMcoinReward, int nChargeAmount, int nPlanSize, const std::vector<CChargePlan> & vecChargePlan, Version_Type versionType, int nRechargeTime )
{
	nMoneyReward = nMcoinReward = 0;
	for ( int i = nPlanSize - 1; i >= 0; --i )
	{
		const CChargePlan & plan = vecChargePlan[ i ];
		if ( nChargeAmount >= (int)plan.m_nChargeRangeLower )
		{
			nMoneyReward = ( nChargeAmount * plan.m_nChargeMoneyRatio / 100 ) + plan.m_nChargeMoneyQuota;	// 金券奖励,向下取整
			nMcoinReward = ( nChargeAmount * plan.m_nChargeMcoinRatio / 100 ) + plan.m_nChargeMcoinQuota;	// 绑定M币奖励,向下取整

			break;
		}
	}
}


// void PaymentInfoMgr::SetFixedChargeConfig(unsigned short isOpen, unsigned int uStartTime, unsigned int uEndTime)
// {
// 	m_bOpenedFixedCharge = (isOpen == 0 ? false : true);
// 
// 	m_uStartTimeFixedCharge = uStartTime;
// 	m_uEndTimeFixedCharge = uEndTime;
// }

void PaymentInfoMgr::AddFixedChargeConfig(unsigned int uStartTime, unsigned int uEndTime)
{
    if (uStartTime >= uEndTime)
        return ; // 不添加

    m_fixedChargeTimeSpanVec.push_back(TimeSpan(uStartTime, uEndTime));

    return ;
}


void PaymentInfoMgr::AddFixedChargeInfo(unsigned int mcoin, unsigned giftcoin, const std::string &strItem)
{
	m_mapGiftFixedCharge[mcoin] = giftcoin; // 
    m_fixedGiftItemMap[mcoin] = strItem;
}

unsigned PaymentInfoMgr::FixedChargeGiftCoin(unsigned int mcoin)
{
	FixedChargeGiftMCoinMap::iterator iter = m_mapGiftFixedCharge.find(mcoin);

	if (m_mapGiftFixedCharge.end() == iter)
		return 0;
	else
		return iter->second;
}

std::string PaymentInfoMgr::FixedChargeGiftItem(unsigned int mcoin)
{
    FixedGiftItemMap::iterator iter = m_fixedGiftItemMap.find(mcoin);
    if (m_fixedGiftItemMap.end() == iter)
        return "";
    else
        return iter->second;
}


bool PaymentInfoMgr::FixedChargeIsRun(time_t nowTime, unsigned int *pStartTime, unsigned int *pEndTime)
{
    int nActIndex = FixedChargeIndex(nowTime);
    if (nActIndex >= 0 && nActIndex < (int)m_fixedChargeTimeSpanVec.size())
    {
        if (NULL != pStartTime)
            *pStartTime = m_fixedChargeTimeSpanVec[nActIndex].first;
        if (NULL != pEndTime)
            *pEndTime = m_fixedChargeTimeSpanVec[nActIndex].second;

        return true;
    }

    return false;
}

int PaymentInfoMgr::FixedChargeIndex(time_t nowTime)
{
    for (size_t i = 0; i < m_fixedChargeTimeSpanVec.size(); ++i)
    {
        if (nowTime > (time_t)m_fixedChargeTimeSpanVec[i].first && nowTime <= (time_t)m_fixedChargeTimeSpanVec[i].second)
        {
            return i;
        }
    }

    return -1;
}


// bool PaymentInfoMgr::CanOpenFirstCharge(time_t nowTime)
// {
// 	return m_bOpenedFixedCharge ? (m_uStartTimeFixedCharge < m_uEndTimeFixedCharge && nowTime < m_uEndTimeFixedCharge) : false;
// }

