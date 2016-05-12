#include "GameMsg_UCVIP.h"

#include "../../../share/ServerMsgDef.h"

GameMsg_C2S_GetUCVIPRewardStatue::GameMsg_C2S_GetUCVIPRewardStatue()
:GameMsg_Base( MSG_C2S_GetUCVIPRewardStatue )
,m_nUCVIPLevel( 0 )
{

}

bool GameMsg_C2S_GetUCVIPRewardStatue::doDecode(CParamPool &IOBuff)
{
	this->m_nUCVIPLevel = IOBuff.GetUShort();

	return true;
}

GameMsg_S2C_GetUCVIPRewardStatueSuc::GameMsg_S2C_GetUCVIPRewardStatueSuc()
:GameMsg_Base( MSG_S2C_GetUCVIPRewardStatueSuc )
{

}

bool GameMsg_S2C_GetUCVIPRewardStatueSuc::doEncode(CParamPool & IOBuff)
{
	if( this->mapRewardState.size() > 0 )
	{
		IOBuff.AddInt( (int)this->mapRewardState.size() );
		std::map<int, unsigned char>::iterator iter = this->mapRewardState.begin(), iterEnd = this->mapRewardState.end();
		for(; iter != iterEnd; ++iter )
		{
			IOBuff.AddInt( iter->first );
			IOBuff.AddUChar( iter->second );
		}
	}
	return true;
}

GameMsg_S2C_GetUCVIPRewardStatueFail::GameMsg_S2C_GetUCVIPRewardStatueFail()
:GameMsg_Base( MSG_S2C_GetUCVIPRewardStatueFail )
{

}

bool GameMsg_S2C_GetUCVIPRewardStatueFail::doEncode(CParamPool & IOBuff)
{
	//IOBuff.AddUChar( this->m_nErrorCode );
	IOBuff.AddStringW( this->strError.c_str() );

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////
GameMsg_C2S_GetUCVIPRewardDetial::GameMsg_C2S_GetUCVIPRewardDetial()
:GameMsg_Base( MSG_C2S_GetUCVIPRewardDetail )
,m_nRewardIndex( 0 )
{

}

bool GameMsg_C2S_GetUCVIPRewardDetial::doDecode(CParamPool &IOBuff)
{
	this->m_nRewardIndex = IOBuff.GetInt();

	return true;	
}

GameMsg_S2C_GetUCVIPRewardDetialSuc::GameMsg_S2C_GetUCVIPRewardDetialSuc( std::list<CItem> &rRewardList, int nLowLevel, int nRewardIndex )
:GameMsg_Base( MSG_C2S_GetUCVIPRewardDetailSuc )
,nRewardIndex(  nRewardIndex )
,nLowLevel(  nLowLevel )
{
	this->nRewardIndex = nRewardIndex;
	this->m_rewardList = rRewardList;
}

bool GameMsg_S2C_GetUCVIPRewardDetialSuc::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddInt( this->nRewardIndex );
	IOBuff.AddInt( this->nLowLevel );
	IOBuff.AddUShort( (unsigned short)m_rewardList.size() );
	if( !this->m_rewardList.empty() )
	{
		std::list<CItem>::iterator iter = this->m_rewardList.begin(), iterEnd = this->m_rewardList.end();
		for( ; iter != iterEnd; ++iter )
		{
			(*iter).doEncode( IOBuff );
		}
	}
	return true;
}

GameMsg_S2C_GetUCVIPRewardDetialFail::GameMsg_S2C_GetUCVIPRewardDetialFail()
:GameMsg_Base( MSG_C2S_GetUCVIPRewardDetailFail )
{

}

bool GameMsg_S2C_GetUCVIPRewardDetialFail::doEncode(CParamPool & IOBuff)
{
	//IOBuff.AddUChar( this->m_nErrorCode );
	IOBuff.AddStringW( this->strError.c_str() );

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////
GameMsg_C2S_GainUCVIPReward::GameMsg_C2S_GainUCVIPReward()
:GameMsg_Base( MSG_C2S_GainUCVIPReward )
,m_nRewardIndex( 0 )
{

}

bool GameMsg_C2S_GainUCVIPReward::doDecode(CParamPool &IOBuff )
{
	this->m_nRewardIndex = IOBuff.GetInt();

	return true;
}

GameMsg_S2C_GainUCVIPRewardSuc::GameMsg_S2C_GainUCVIPRewardSuc()
:GameMsg_Base( MSG_S2C_GainUCVIPRewardSuc )
,m_nRewardInex( 0 )
{

}

bool GameMsg_S2C_GainUCVIPRewardSuc::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddInt( this->m_nRewardInex );

	return true;
}

GameMsg_S2C_GainUCVIPRewardFail::GameMsg_S2C_GainUCVIPRewardFail()
:GameMsg_Base( MSG_S2C_GainUCVIPRewardFail )
//,m_nErrorCode( 0 )
{
}

bool GameMsg_S2C_GainUCVIPRewardFail::doEncode(CParamPool & IOBuff)
{
	//IOBuff.AddUChar( this->m_nErrorCode );
	IOBuff.AddStringW( this->strError.c_str() );

	return true;
}
