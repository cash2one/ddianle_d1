#include "GMDataInfo.h"

GMRoomInfo::GMRoomInfo()
{
	m_RoomID = 0;
	m_PlayerMainID = 0;

	m_MusicID = 0;
	m_MusicMode = 0;
	m_MusicDiff = 0;
}

GMRoomInfo::~GMRoomInfo()
{

}

void  GMRoomInfo::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUShort(m_RoomID);
	IOBuff.AddStringW(m_RoomName);
	IOBuff.AddStringW(m_RoomPassword);

	IOBuff.AddUInt(m_PlayerMainID);
	IOBuff.AddStringW(m_PlayerMainName);

	IOBuff.AddUShort((unsigned short)m_PlayerList.size());
	std::vector<GMRoomPlayerInfo>::iterator it = m_PlayerList.begin();
	for( ; it != m_PlayerList.end() ; it++)
	{
		IOBuff.AddUInt(it->m_playerID);
		IOBuff.AddStringW(it->m_playerName);
	}

	IOBuff.AddUShort(m_MusicID);
	IOBuff.AddStringW(m_MusicName);
	IOBuff.AddUShort(m_MusicMode);
	IOBuff.AddUShort(m_MusicDiff);
}

void GMRoomInfo::doDecode(CParamPool & IOBuff)
{
	m_RoomID = IOBuff.GetUShort();
	IOBuff.GetStringW(m_RoomName);
	IOBuff.GetStringW(m_RoomPassword);

	m_PlayerMainID = IOBuff.GetUInt();
	IOBuff.GetStringW(m_PlayerMainName);

	unsigned short playerListCount = IOBuff.GetUShort();
	for (unsigned short i = 0 ; i < playerListCount ; i++)
	{
		GMRoomPlayerInfo playerInfo;
		playerInfo.m_playerID = IOBuff.GetInt();
		IOBuff.GetStringW(playerInfo.m_playerName);

		m_PlayerList.push_back(playerInfo);
	}

	m_MusicID = IOBuff.GetUShort();
	IOBuff.GetStringW(m_MusicName);
	m_MusicMode = IOBuff.GetUShort();
	m_MusicDiff = IOBuff.GetUShort();

}

void GMRoomInfo::AddPlayerInfo(GMRoomPlayerInfo player)
{
	m_PlayerList.push_back(player);
}

void PlayerMoneyInfo::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddStringW(m_PlayerPUID);
	IOBuff.AddInt(m_nMoney);
	IOBuff.AddInt(m_nBindMoney);
}

void PlayerMoneyInfo::doDecode(CParamPool & IOBuff)
{
	IOBuff.GetStringW(m_PlayerPUID);
	m_nMoney = IOBuff.GetInt();
	m_nBindMoney = IOBuff.GetInt();
}

void GMRechargeResult::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddStringW(m_PlayerPUID);
	IOBuff.AddBool(m_Result);
}

void GMRechargeResult::doDecode(CParamPool & IOBuff)
{
	IOBuff.GetStringW(m_PlayerPUID);
	m_Result = IOBuff.GetBool();
}

void CServerPlayerCountInfo::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddInt(m_nLine);
	IOBuff.AddStringW(m_strName);
	IOBuff.AddInt(m_nCurrentCount);
	IOBuff.AddInt(m_nLimitCount);
}

void CServerPlayerCountInfo::doDecode(CParamPool & IOBuff)
{
	m_nLine = IOBuff.GetInt();
	IOBuff.GetStringW(m_strName);
	m_nCurrentCount = IOBuff.GetInt();
	m_nLimitCount = IOBuff.GetInt();
}

