#include "GameMsg_G2S_NewCompensation.h"
#include <list>
#include "../../datastructure/DataStruct_Base.h"
#include "../share/ServerMsgDef.h"

GameMsg_G2S_SyncCompensation::GameMsg_G2S_SyncCompensation()
:GameMsg_Base(MSG_G2S_SyncCompensation)
{

}

GameMsg_G2S_SyncCompensation::~GameMsg_G2S_SyncCompensation()
{
}

bool GameMsg_G2S_SyncCompensation::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddShort((short)m_CompensationMap.size());
	for (map<int,CompensationInfo>::iterator it = m_CompensationMap.begin(); it!=m_CompensationMap.end(); ++it)
	{
		it->second.EncodeInfo(IOBuff);
	}

	return true;
}

bool GameMsg_G2S_SyncCompensation::doDecode(CParamPool &IOBuff)
{
	int nCompensationCount = IOBuff.GetShort();
	for (int i = 0; i < nCompensationCount; ++i)
	{
		CompensationInfo cInfo;
		cInfo.DecodeInfo(IOBuff);

		m_CompensationMap.insert(std::make_pair(cInfo.m_nID,cInfo));
	}

	return true;
}


GameMsg_G2S_NewCompensation::GameMsg_G2S_NewCompensation()
:GameMsg_Base(MSG_G2S_NewCompensation)
{

}

GameMsg_G2S_NewCompensation::~GameMsg_G2S_NewCompensation()
{
}

bool GameMsg_G2S_NewCompensation::doEncode(CParamPool &IOBuff)
{
	m_CompensationInfo.EncodeInfo(IOBuff);

	return true;
}

bool GameMsg_G2S_NewCompensation::doDecode(CParamPool &IOBuff)
{
	m_CompensationInfo.DecodeInfo(IOBuff);
	return true;
}


