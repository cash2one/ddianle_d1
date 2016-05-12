#include "GameMsg_GM_share.h"
#include "../share/ServerMsgDef.h"

GameMsg_M2G_SendGroupServerInfo::GameMsg_M2G_SendGroupServerInfo()
:GameMsg_Base(MSG_GM_M2G_SendGroupServerInfo)
{
}

GameMsg_M2G_SendGroupServerInfo::~GameMsg_M2G_SendGroupServerInfo()
{
}

bool GameMsg_M2G_SendGroupServerInfo::doEncode( CParamPool &IOBuff )
{
	return true;
}

bool GameMsg_M2G_SendGroupServerInfo::doDecode( CParamPool &IOBuff )
{
	return true;
}

GameMsg_G2M_SendGroupServerInfoRes::GameMsg_G2M_SendGroupServerInfoRes()
:GameMsg_Base(MSG_GM_G2S_SendGroupServerInofRes)
{

}

GameMsg_G2M_SendGroupServerInfoRes::~GameMsg_G2M_SendGroupServerInfoRes()
{

}

bool GameMsg_G2M_SendGroupServerInfoRes::doEncode(CParamPool &IOBuff)
{
	return true;
}

bool GameMsg_G2M_SendGroupServerInfoRes::doDecode(CParamPool &IOBuff)
{
	return true;
}

GameMsg_M2S_GMSendMail::GameMsg_M2S_GMSendMail()
:GameMsg_Base(MSG_GM_M2S_SendMail)
{
}

GameMsg_M2S_GMSendMail::~GameMsg_M2S_GMSendMail()
{
}

bool GameMsg_M2S_GMSendMail::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nSlot);

	IOBuff.AddUShort((unsigned short)m_listRcvRoleInfo.size());
	if (m_listRcvRoleInfo.size() > 0) {
		for (list<CRcvSysMailRoleInfo>::iterator itr = m_listRcvRoleInfo.begin(); itr != m_listRcvRoleInfo.end(); ++itr) {
			itr->doEncodeServ(IOBuff);
		}
	}

	return true;
}

bool GameMsg_M2S_GMSendMail::doDecode(CParamPool &IOBuff)
{
	m_nSlot = IOBuff.GetUShort();
	int nCount = IOBuff.GetUShort();
	if (nCount > 0) {
		for (int i = 0; i < nCount; ++i) {
			CRcvSysMailRoleInfo rcvRoleInfo;
			rcvRoleInfo.doDecodeServ(IOBuff);
			m_listRcvRoleInfo.push_back(rcvRoleInfo);
		}
	}

	return true;
}



GameMsg_S2M_GMSendMailRes::GameMsg_S2M_GMSendMailRes()
:GameMsg_Base(MSG_GM_S2M_SendMailRes)
{
}

GameMsg_S2M_GMSendMailRes::~GameMsg_S2M_GMSendMailRes()
{
}

bool GameMsg_S2M_GMSendMailRes::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUShort(m_nSlot);
	IOBuff.AddUShort((unsigned short)m_listRcvRoleInfo.size());
	if (m_listRcvRoleInfo.size() > 0) {
		for (list<CRcvSysMailRoleInfo>::iterator itr = m_listRcvRoleInfo.begin(); itr != m_listRcvRoleInfo.end(); ++itr) {
			itr->doEncodeServ(IOBuff);
		}
	}

	return true;
}

bool GameMsg_S2M_GMSendMailRes::doDecode( CParamPool &IOBuff )
{
	m_nSlot = IOBuff.GetUShort();
	int nCount = IOBuff.GetUShort();
	if (nCount > 0) {
		while(nCount-- > 0) {
			CRcvSysMailRoleInfo rcvMailRoleInfo;
			rcvMailRoleInfo.doDecodeServ(IOBuff);
			m_listRcvRoleInfo.push_back(rcvMailRoleInfo);
		}
	}

	return true;
}

GameMsg_M2S_QueryRangeGift::GameMsg_M2S_QueryRangeGift()
:GameMsg_Base(MSG_GM_M2S_QueryRangeGift)
,m_nGiftID(0)
,m_strStartTime("")
,m_strEndTime("")
{
}

GameMsg_M2S_QueryRangeGift::~GameMsg_M2S_QueryRangeGift()
{
}

bool GameMsg_M2S_QueryRangeGift::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nGiftID);
	IOBuff.AddStringW(m_strStartTime.c_str());
	IOBuff.AddStringW(m_strEndTime.c_str());

	return true;
}

bool GameMsg_M2S_QueryRangeGift::doDecode( CParamPool & IOBuff )
{
	m_nGiftID = IOBuff.GetUInt();
	IOBuff.GetStringW(m_strStartTime);
	IOBuff.GetStringW(m_strEndTime);

	return true;
}


GameMsg_S2M_QueryRangeGiftRes::GameMsg_S2M_QueryRangeGiftRes()
:GameMsg_Base(MSG_GM_S2M_QueryRangeGiftRes)
,m_nActivatedNum(0)
{
}

GameMsg_S2M_QueryRangeGiftRes::~GameMsg_S2M_QueryRangeGiftRes()
{
}

bool GameMsg_S2M_QueryRangeGiftRes::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUInt(m_nActivatedNum);	

	return true;
}

bool GameMsg_S2M_QueryRangeGiftRes::doDecode(CParamPool &IOBuff)
{
	m_nActivatedNum = IOBuff.GetUInt();	

	return true;
}

GameMsg_M2S_QueryMultiGift::GameMsg_M2S_QueryMultiGift()
:GameMsg_Base(MSG_GM_M2S_QueryMultiGift)
{
}

GameMsg_M2S_QueryMultiGift::~GameMsg_M2S_QueryMultiGift()
{
}

bool GameMsg_M2S_QueryMultiGift::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUShort(m_nSlot);
	IOBuff.AddUShort((unsigned short)m_listGiftCodes.size());
	if (m_listGiftCodes.size() > 0) {
		for (list<string>::iterator itr = m_listGiftCodes.begin(); itr != m_listGiftCodes.end(); ++itr) {
			string strCode = *itr;
			IOBuff.AddString(strCode.c_str());
		}
	}

	return true;
}

bool GameMsg_M2S_QueryMultiGift::doDecode( CParamPool &IOBuff )
{
	m_nSlot = IOBuff.GetUShort();
	int nCount = IOBuff.GetUShort();
	if (nCount > 0) {
		while(nCount-- > 0) {
			string strCode = IOBuff.GetString();
			m_listGiftCodes.push_back(strCode);
		}
	}

	return true;
}

GameMsg_S2M_QueryMultiGiftRes::GameMsg_S2M_QueryMultiGiftRes()
:GameMsg_Base(MSG_GM_S2M_QueryMultiGiftRes)
{
}

GameMsg_S2M_QueryMultiGiftRes::~GameMsg_S2M_QueryMultiGiftRes()
{
}

bool GameMsg_S2M_QueryMultiGiftRes::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUShort(m_nSolt);

	IOBuff.AddUShort((unsigned short)m_listQueryCodes.size());
	if (m_listQueryCodes.size() > 0) {
		for (list<CQueryGiftState>::iterator itr = m_listQueryCodes.begin(); itr != m_listQueryCodes.end(); ++itr) {
			itr->doEncode(IOBuff);
		}
	}

	return true;
}

bool GameMsg_S2M_QueryMultiGiftRes::doDecode( CParamPool & IOBuff )
{
	m_nSolt = IOBuff.GetUShort();
	int nCount = IOBuff.GetUShort();
	while (nCount-- > 0) {
		CQueryGiftState info;
		info.doDecode(IOBuff);
		m_listQueryCodes.push_back(info);
	}

	return true;
}


GameMsg_M2S_Announce::GameMsg_M2S_Announce()
:GameMsg_Base(MSG_GM_M2S_Announce)
{

}

GameMsg_M2S_Announce::~GameMsg_M2S_Announce()
{

}

bool GameMsg_M2S_Announce::doEncode( CParamPool &IOBuff )
{
	m_announce.doEncode(IOBuff);

	return true;
}

bool GameMsg_M2S_Announce::doDecode( CParamPool &IOBuff )
{
	m_announce.doDecode(IOBuff);

	return true;
}

GameMsg_M2G_LockOrUnLockByGm::GameMsg_M2G_LockOrUnLockByGm()
:GameMsg_Base(MSG_GM_M2G_LockOrUnLockAccountByGm)
{
	m_nPid = 0;
	m_bLock = true;
	m_nLockTimeType = 0;
}

GameMsg_M2G_LockOrUnLockByGm::~GameMsg_M2G_LockOrUnLockByGm()
{

}

bool GameMsg_M2G_LockOrUnLockByGm::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUShort(m_nPid);
	IOBuff.AddString(m_strPUID.c_str());
	IOBuff.AddString(m_strTransID.c_str());
	IOBuff.AddBool(m_bLock);
	IOBuff.AddInt(m_nLockTimeType);
	return true;
}

bool GameMsg_M2G_LockOrUnLockByGm::doDecode( CParamPool &IOBuff )
{
	m_nPid = IOBuff.GetUShort();
	m_strPUID = IOBuff.GetString();
	m_strTransID = IOBuff.GetString();
	m_bLock = IOBuff.GetBool();
	m_nLockTimeType = IOBuff.GetInt();
	return true;
}

//-------------------------------------------------------------------

GameMsg_M2G_AddMCoinByGm::GameMsg_M2G_AddMCoinByGm()
:GameMsg_Base(MSG_GM_M2G_AddMCoinByGm)
{
	m_nPid = 0;
	m_nAmount = 0;
	m_nBindAmount = 0;
	m_nCurrency = 0;
}

GameMsg_M2G_AddMCoinByGm::~GameMsg_M2G_AddMCoinByGm()
{
}

bool GameMsg_M2G_AddMCoinByGm::doEncode( CParamPool &IOBuff )
{	
	IOBuff.AddUShort(m_nPid);
	IOBuff.AddString(m_strPUID.c_str());
	IOBuff.AddUInt(m_nAmount);
	IOBuff.AddUInt(m_nBindAmount);
	IOBuff.AddString(m_strTransID.c_str());
	IOBuff.AddUInt(m_nCurrency);
	return true;
}

bool GameMsg_M2G_AddMCoinByGm::doDecode( CParamPool &IOBuff )
{	
	m_nPid = IOBuff.GetUShort();
	m_strPUID = IOBuff.GetString();
	m_nAmount = IOBuff.GetUInt();
	m_nBindAmount = IOBuff.GetUInt();
	m_strTransID = IOBuff.GetString();
	m_nCurrency = IOBuff.GetUInt();
	return true;
}

GameMsg_M2G_UpdateCompensation::GameMsg_M2G_UpdateCompensation()
: GameMsg_Base(MSG_GM_M2G_UpdateCompensation)
{

}

GameMsg_M2G_UpdateCompensation::~GameMsg_M2G_UpdateCompensation()
{

}

bool GameMsg_M2G_UpdateCompensation::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddInt(m_nEndTime);
	IOBuff.AddInt(m_nMoney);
	IOBuff.AddStringW(m_strTitle.c_str());
	IOBuff.AddStringW(m_strContent.c_str());
	IOBuff.AddStringW(m_strItemMale.c_str());
	IOBuff.AddStringW(m_strItemFemale.c_str());

	return true;
}

bool GameMsg_M2G_UpdateCompensation::doDecode(CParamPool &IOBuff)
{
	m_nEndTime = IOBuff.GetInt();
	m_nMoney = IOBuff.GetInt();
	IOBuff.GetStringW(m_strTitle);
	IOBuff.GetStringW(m_strContent);
	IOBuff.GetStringW(m_strItemMale);
	IOBuff.GetStringW(m_strItemFemale);

	return true;
}

GameMsg_G2M_UpdateCompensationRes::GameMsg_G2M_UpdateCompensationRes()
:GameMsg_Base(MSG_GM_G2M_UpdateCompensationRes)
{

}

GameMsg_G2M_UpdateCompensationRes::~GameMsg_G2M_UpdateCompensationRes()
{

}

bool GameMsg_G2M_UpdateCompensationRes::doEncode(CParamPool &IOBuff)
{

	IOBuff.AddStringW(m_strError.c_str());

	return true;
}

bool GameMsg_G2M_UpdateCompensationRes::doDecode(CParamPool &IOBuff)
{
	IOBuff.GetStringW(m_strError);

	return true;
}

GameMsg_M2G_GetServerLine::GameMsg_M2G_GetServerLine()
	: GameMsg_Base(MSG_GM_M2G_GetServerLine)
{
	m_nSlot = 0;
}


GameMsg_M2G_GetServerLine::~GameMsg_M2G_GetServerLine()
{

}

bool GameMsg_M2G_GetServerLine::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nSlot);
	return true;
}

bool GameMsg_M2G_GetServerLine::doDecode(CParamPool &IOBuff)
{
	m_nSlot = IOBuff.GetUShort();
	return true;
}

GameMsg_G2M_GetServerLineRes::GameMsg_G2M_GetServerLineRes()
	: GameMsg_Base(MSG_GM_G2M_GetServerLineRes)
{
	m_nSlot = 0;
}

GameMsg_G2M_GetServerLineRes::~GameMsg_G2M_GetServerLineRes()
{

}

bool GameMsg_G2M_GetServerLineRes::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nSlot);
	IOBuff.AddUShort((unsigned short)m_Lines.size());
	std::map<int,std::string>::iterator it = m_Lines.begin();
	for (;it != m_Lines.end() ; it++)
	{
		IOBuff.AddInt(it->first);
		IOBuff.AddStringW(it->second);
	}
	return true;
}

bool GameMsg_G2M_GetServerLineRes::doDecode(CParamPool &IOBuff)
{
	m_nSlot = IOBuff.GetUShort();
	unsigned short nCount = IOBuff.GetUShort();
	for (unsigned short i = 0 ; i < nCount ; i++)
	{
		int nLine = IOBuff.GetInt();
		std::string lineName ;
		IOBuff.GetStringW(lineName);

		m_Lines.insert(make_pair(nLine,lineName));
	}
	return true;
}

GameMsg_M2G_GetRoomList::GameMsg_M2G_GetRoomList()
	: GameMsg_Base(MSG_GM_M2G_GetRoomList)
{
	m_nSlot = 0;
	m_nLine = 0;
	m_nGMAskRoomType = 0;
}

GameMsg_M2G_GetRoomList::~GameMsg_M2G_GetRoomList()
{

}

bool GameMsg_M2G_GetRoomList::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nSlot);
	IOBuff.AddInt(m_nLine);
	IOBuff.AddUShort(m_nGMAskRoomType);
	return true;
}

bool GameMsg_M2G_GetRoomList::doDecode(CParamPool &IOBuff)
{
	m_nSlot = IOBuff.GetUShort();
	m_nLine = IOBuff.GetInt();
	m_nGMAskRoomType = IOBuff.GetUShort();
	return true;
}

GameMsg_G2M_GetRoomListRes::GameMsg_G2M_GetRoomListRes()
	: GameMsg_Base(MSG_GM_G2M_GetRoomListRes)
{
	m_nSlot = 0;
}

GameMsg_G2M_GetRoomListRes::~GameMsg_G2M_GetRoomListRes()
{

}

bool GameMsg_G2M_GetRoomListRes::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nSlot);

	IOBuff.AddUShort((unsigned short)m_RoomList.size());
	std::vector<GMRoomInfo>::iterator it = m_RoomList.begin();
	for (; it != m_RoomList.end() ; it++)
	{
		it->doEncode(IOBuff);
	}
	return true;
}

bool GameMsg_G2M_GetRoomListRes::doDecode(CParamPool &IOBuff)
{
	m_nSlot = IOBuff.GetUShort();
	unsigned short nCount = IOBuff.GetUShort();
	for (unsigned short i = 0 ; i < nCount ; i++)
	{
		GMRoomInfo roomInfo;
		roomInfo.doDecode(IOBuff);
		m_RoomList.push_back(roomInfo);
	}
	return true;
}

GameMsg_M2G_SetMallUpdateTime::GameMsg_M2G_SetMallUpdateTime()
	: GameMsg_Base(MSG_GM_M2G_SetMallUpdateTime)
{

}

GameMsg_M2G_SetMallUpdateTime::~GameMsg_M2G_SetMallUpdateTime()
{

}

bool GameMsg_M2G_SetMallUpdateTime::doDecode(CParamPool &IOBuff)
{
	m_UpdateTime = IOBuff.GetUInt();
	return true;
}

bool GameMsg_M2G_SetMallUpdateTime::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_UpdateTime);
	return true;
}

GameMsg_M2G_GroupBuyControl::GameMsg_M2G_GroupBuyControl()
	: GameMsg_Base(MSG_GM_M2G_GroupBuyControl)
{

}

GameMsg_M2G_GroupBuyControl::~GameMsg_M2G_GroupBuyControl()
{

}

bool GameMsg_M2G_GroupBuyControl::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddInt(m_nLine);
	IOBuff.AddUInt(m_nStartTime);
	IOBuff.AddUInt(m_nEndTime);
	IOBuff.AddUInt(m_nInterval);
	IOBuff.AddUInt(m_nMaxPeople);
	IOBuff.AddUInt(m_nMinPeople);

	return true;
}

bool GameMsg_M2G_GroupBuyControl::doDecode(CParamPool &IOBuff)
{
	m_nLine = IOBuff.GetInt();
	m_nStartTime = IOBuff.GetUInt();
	m_nEndTime = IOBuff.GetUInt();
	m_nInterval = IOBuff.GetUInt();
	m_nMaxPeople = IOBuff.GetUInt();
	m_nMinPeople = IOBuff.GetUInt();

	return true;
}

GameMsg_M2G_SendPrivMCoin::GameMsg_M2G_SendPrivMCoin()
	: GameMsg_Base(MSG_GM_M2G_SendPrivMCoin)
{

}

GameMsg_M2G_SendPrivMCoin::~GameMsg_M2G_SendPrivMCoin()
{

}

bool GameMsg_M2G_SendPrivMCoin::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nSlot);
	IOBuff.AddUShort((unsigned short)m_PlayerMoneyInfo.size());
	for (size_t i = 0 ; i < m_PlayerMoneyInfo.size() ; i++ )
	{
		m_PlayerMoneyInfo[i].doEncode(IOBuff);
	}

	return true;
}

bool GameMsg_M2G_SendPrivMCoin::doDecode(CParamPool &IOBuff)
{
	m_nSlot = IOBuff.GetUShort();
	unsigned short nCount = IOBuff.GetUShort();
	for (unsigned short i = 0 ; i < nCount ; i++)
	{
		PlayerMoneyInfo pmi;
		pmi.doDecode(IOBuff);
		m_PlayerMoneyInfo.push_back(pmi);
	}
	return true;
}

GameMsg_A2M_SendPrivMCoinRet::GameMsg_A2M_SendPrivMCoinRet()
	: GameMsg_Base(MSG_GM_A2M_SendPrivMCoinRet)
{

}

GameMsg_A2M_SendPrivMCoinRet::~GameMsg_A2M_SendPrivMCoinRet()
{

}

bool GameMsg_A2M_SendPrivMCoinRet::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nSlot);
	IOBuff.AddUShort((unsigned short)m_GMRechargeResult.size());
	std::vector<GMRechargeResult>::iterator it = m_GMRechargeResult.begin();
	for (; it != m_GMRechargeResult.end() ; it++ )
	{
		it->doEncode(IOBuff);
	}

	return true;
}

bool GameMsg_A2M_SendPrivMCoinRet::doDecode(CParamPool &IOBuff)
{
	m_nSlot = IOBuff.GetUShort();
	unsigned short nCount = IOBuff.GetUShort();
	for (unsigned short i = 0 ; i < nCount ; i++)
	{
		GMRechargeResult gr;
		gr.doDecode(IOBuff);
		m_GMRechargeResult.push_back(gr);
	}

	return true;
}

GameMsg_M2G_GetServerPlayerCountInfo::GameMsg_M2G_GetServerPlayerCountInfo()
	: GameMsg_Base(MSG_GM_M2G_GetServerPlayerCountInfo)
{

}

GameMsg_M2G_GetServerPlayerCountInfo::~GameMsg_M2G_GetServerPlayerCountInfo()
{

}

bool GameMsg_M2G_GetServerPlayerCountInfo::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nSolt);
	return true;
}

bool GameMsg_M2G_GetServerPlayerCountInfo::doDecode(CParamPool &IOBuff)
{
	m_nSolt = IOBuff.GetUShort();
	return true;
}

GameMsg_G2M_GetServerPlayerCountInfoRet::GameMsg_G2M_GetServerPlayerCountInfoRet()
	: GameMsg_Base(MSG_GM_G2M_GetServerPlayerCountInfoRet)
{

}

GameMsg_G2M_GetServerPlayerCountInfoRet::~GameMsg_G2M_GetServerPlayerCountInfoRet()
{

}

bool GameMsg_G2M_GetServerPlayerCountInfoRet::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nSolt);
	IOBuff.AddUShort((unsigned short)m_ServerPlayerCountInfoMap.size());
	std::map<int,CServerPlayerCountInfo>::iterator it = m_ServerPlayerCountInfoMap.begin();
	for ( ; it != m_ServerPlayerCountInfoMap.end() ; it++ )
	{
		it->second.doEncode(IOBuff);
	}

	return true;
}

bool GameMsg_G2M_GetServerPlayerCountInfoRet::doDecode(CParamPool &IOBuff)
{
	m_nSolt = IOBuff.GetUShort();
	
	unsigned short nCount = IOBuff.GetUShort();
	for (unsigned short i = 0 ; i < nCount ; i++ )
	{
		CServerPlayerCountInfo sInfo ;
		sInfo.doDecode(IOBuff);

		m_ServerPlayerCountInfoMap.insert(make_pair(sInfo.m_nLine,sInfo));
	}

	return true;
}

GameMsg_G2A_GetServerPlayerLimit::GameMsg_G2A_GetServerPlayerLimit()
	: GameMsg_Base(MSG_GM_G2A_GetServerPlayerLimit)
{

}

GameMsg_G2A_GetServerPlayerLimit::~GameMsg_G2A_GetServerPlayerLimit()
{

}

bool GameMsg_G2A_GetServerPlayerLimit::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nSolt);
	return true;
}

bool GameMsg_G2A_GetServerPlayerLimit::doDecode(CParamPool &IOBuff)
{
	m_nSolt = IOBuff.GetUShort();
	return true;
}

GameMsg_A2G_GetServerPlayerLimitRet::GameMsg_A2G_GetServerPlayerLimitRet()
	: GameMsg_Base(MSG_GM_A2M_GetServerPlayerLimitRet)
{

}

GameMsg_A2G_GetServerPlayerLimitRet::~GameMsg_A2G_GetServerPlayerLimitRet()
{

}

bool GameMsg_A2G_GetServerPlayerLimitRet::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nSolt);
	IOBuff.AddInt(m_PlayerLimit);
	return true;
}

bool GameMsg_A2G_GetServerPlayerLimitRet::doDecode(CParamPool &IOBuff)
{
	m_nSolt = IOBuff.GetUShort();
	m_PlayerLimit = IOBuff.GetInt();
	return true;
}

GameMsg_M2A_SetPlayerLimit::GameMsg_M2A_SetPlayerLimit()
	: GameMsg_Base(MSG_GM_M2A_SetServerPlayerLimit)
{

}

GameMsg_M2A_SetPlayerLimit::~GameMsg_M2A_SetPlayerLimit()
{

}

bool GameMsg_M2A_SetPlayerLimit::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddInt(m_PlayerLimit);
	return true;
}

bool GameMsg_M2A_SetPlayerLimit::doDecode(CParamPool &IOBuff)
{
	m_PlayerLimit = IOBuff.GetInt();
	return true;
}

GameMsg_M2A_SendBlackDeviceList::GameMsg_M2A_SendBlackDeviceList()
	: GameMsg_Base(MSG_GM_M2A_SendBlackDeviceList)
{
    m_bFrozenDevice = true;
    m_nSlot = 0;
}

GameMsg_M2A_SendBlackDeviceList::~GameMsg_M2A_SendBlackDeviceList()
{

}

bool GameMsg_M2A_SendBlackDeviceList::doDecode(CParamPool &IOBuff)
{
    m_bFrozenDevice = IOBuff.GetBool();
	m_nSlot = IOBuff.GetInt();
	unsigned short nCount = IOBuff.GetUShort();
	for (unsigned short i = 0 ; i < nCount ; i++)
	{
		std::string strBuff;
		IOBuff.GetStringW(strBuff);
		m_NameList.push_back(strBuff);
	}
	IOBuff.GetStringW(m_GM);
	return true;
}

bool GameMsg_M2A_SendBlackDeviceList::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddBool(m_bFrozenDevice);
	IOBuff.AddInt(m_nSlot);
	IOBuff.AddUShort((unsigned short)m_NameList.size());
	for (size_t i = 0 ; i < m_NameList.size() ; i++)
	{
		IOBuff.AddStringW(m_NameList[i]);
	}
	IOBuff.AddStringW(m_GM);
	return true;
}

GameMsg_A2M_SendBlackDeviceListRet::GameMsg_A2M_SendBlackDeviceListRet()
	: GameMsg_Base(MSG_GM_A2M_SendBlackDeviceListRet)
{

}

GameMsg_A2M_SendBlackDeviceListRet::~GameMsg_A2M_SendBlackDeviceListRet()
{

}

bool GameMsg_A2M_SendBlackDeviceListRet::doDecode(CParamPool &IOBuff)
{
	m_nSlot = IOBuff.GetInt();
	m_Result = IOBuff.GetUShort();
	return true;
}

bool GameMsg_A2M_SendBlackDeviceListRet::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddInt(m_nSlot);
	IOBuff.AddUShort(m_Result);
	return true;
}

GameMsg_M2A_ChangeAccountPassword::GameMsg_M2A_ChangeAccountPassword()
	: GameMsg_Base(MSG_GM_M2A_ChangeAccountPassword)
{

}

GameMsg_M2A_ChangeAccountPassword::~GameMsg_M2A_ChangeAccountPassword()
{

}

bool GameMsg_M2A_ChangeAccountPassword::doDecode(CParamPool &IOBuff)
{
	m_nSlot = IOBuff.GetInt();
	IOBuff.GetStringW(m_Account);
	IOBuff.GetStringW(m_Password);

	return true;
}

bool GameMsg_M2A_ChangeAccountPassword::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddInt(m_nSlot);
	IOBuff.AddStringW(m_Account);
	IOBuff.AddStringW(m_Password);

	return true;
}

GameMsg_A2M_ChangeAccountPasswordRet::GameMsg_A2M_ChangeAccountPasswordRet()
	: GameMsg_Base(MSG_GM_A2M_ChangeAccountPasswordRet)
{

}

GameMsg_A2M_ChangeAccountPasswordRet::~GameMsg_A2M_ChangeAccountPasswordRet()
{

}

bool GameMsg_A2M_ChangeAccountPasswordRet::doDecode(CParamPool &IOBuff)
{
	m_nSlot = IOBuff.GetInt();
	m_Result = IOBuff.GetUShort();
	return true;
}

bool GameMsg_A2M_ChangeAccountPasswordRet::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddInt(m_nSlot);
	IOBuff.AddUShort(m_Result);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// infoCenter
InfoCenterItem::InfoCenterItem()
	: m_id(0)
	, m_updateTime(0)
	, m_group(0)
	, m_type(0)
{
}

bool InfoCenterItem::doDecode(CParamPool &IOBuff)
{
	m_id = IOBuff.GetUInt();
	m_updateTime = IOBuff.GetUInt();
	m_group = IOBuff.GetUChar();
	IOBuff.GetStringW(m_icon);
	m_type = IOBuff.GetUChar();
	IOBuff.GetStringW(m_strTitle);
	IOBuff.GetStringW(m_strContext);
	IOBuff.GetStringW(m_strHref);
	IOBuff.GetStringW(m_strHidPid);

	return true;
}

bool InfoCenterItem::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_id);
	IOBuff.AddUInt(m_updateTime);
	IOBuff.AddUChar(m_group);
	IOBuff.AddStringW(m_icon);
	IOBuff.AddUChar(m_type);
	IOBuff.AddStringW(m_strTitle);
	IOBuff.AddStringW(m_strContext);
	IOBuff.AddStringW(m_strHref);
	IOBuff.AddStringW(m_strHidPid);

	return true;
}


GameMsg_M2G_GetInfoCenterRes::GameMsg_M2G_GetInfoCenterRes()
	: GameMsg_Base(GM_M2G_GetInfoCenterRes)
{
	return ;
}
GameMsg_M2G_GetInfoCenterRes::~GameMsg_M2G_GetInfoCenterRes()
{
	return ;
}
bool GameMsg_M2G_GetInfoCenterRes::doDecode(CParamPool &IOBuff)
{
	return true;
}
bool GameMsg_M2G_GetInfoCenterRes::doEncode(CParamPool &IOBuff)
{
	return true; 
}


GameMsg_G2M_GetInfoCenterRes::GameMsg_G2M_GetInfoCenterRes()
	: GameMsg_Base(GM_G2M_GetInfoCenterRes)

{

}

GameMsg_G2M_GetInfoCenterRes::~GameMsg_G2M_GetInfoCenterRes()
{

}

bool GameMsg_G2M_GetInfoCenterRes::doDecode(CParamPool &IOBuff)
{
	unsigned short ulen = IOBuff.GetUShort();
	for (unsigned short i = 0; i < ulen; ++i)
	{
		m_infos.push_back(InfoCenterItem());
		m_infos[i].doDecode(IOBuff);
	}

	return true;
}

bool GameMsg_G2M_GetInfoCenterRes::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort((unsigned short)m_infos.size());
	for (size_t i = 0; i < m_infos.size(); ++i)
	{
		m_infos[i].doEncode(IOBuff);
	}

	return true;
}

GameMsg_M2G_AddInfoCenterRes::GameMsg_M2G_AddInfoCenterRes()
	: GameMsg_Base(GM_M2G_AddInfoCenterRes)
{

}
GameMsg_M2G_AddInfoCenterRes::~GameMsg_M2G_AddInfoCenterRes()
{

}
bool GameMsg_M2G_AddInfoCenterRes::doDecode(CParamPool &IOBuff)
{
	return m_items.doDecode(IOBuff);
}
bool GameMsg_M2G_AddInfoCenterRes::doEncode(CParamPool &IOBuff)
{
	return m_items.doEncode(IOBuff);
}


GameMsg_G2M_AddInfoCenterRes::GameMsg_G2M_AddInfoCenterRes()
	: GameMsg_Base(GM_G2M_AddInfoCenterRes)
{

}

GameMsg_G2M_AddInfoCenterRes::~GameMsg_G2M_AddInfoCenterRes()
{

}

bool GameMsg_G2M_AddInfoCenterRes::doDecode(CParamPool &IOBuff)
{
	return m_items.doDecode(IOBuff);
}

bool GameMsg_G2M_AddInfoCenterRes::doEncode(CParamPool &IOBuff)
{
	return m_items.doEncode(IOBuff);
}


GameMsg_M2G_UpdateInfoCenterRes::GameMsg_M2G_UpdateInfoCenterRes()
	: GameMsg_Base(GM_M2G_UpdateInfoCenterRes)
{

}
GameMsg_M2G_UpdateInfoCenterRes::~GameMsg_M2G_UpdateInfoCenterRes()
{

}
bool GameMsg_M2G_UpdateInfoCenterRes::doDecode(CParamPool &IOBuff)
{
	return m_items.doDecode(IOBuff);
}

bool GameMsg_M2G_UpdateInfoCenterRes::doEncode(CParamPool &IOBuff)
{
	return m_items.doEncode(IOBuff);
}


GameMsg_G2M_UpdateInfoCenterRes::GameMsg_G2M_UpdateInfoCenterRes()
	: GameMsg_Base(GM_G2M_UpdateInfoCenterRes)
{

}
GameMsg_G2M_UpdateInfoCenterRes::~GameMsg_G2M_UpdateInfoCenterRes()
{

}

bool GameMsg_G2M_UpdateInfoCenterRes::doDecode(CParamPool &IOBuff)
{
	return m_items.doDecode(IOBuff);
}


bool GameMsg_G2M_UpdateInfoCenterRes::doEncode(CParamPool &IOBuff)
{
	return m_items.doEncode(IOBuff);
}

GameMsg_M2G_RemoveInfoCenterRes::GameMsg_M2G_RemoveInfoCenterRes()
	: GameMsg_Base(GM_M2G_RemoveInfoCenterRes)
	, m_id(0)
{

}

GameMsg_M2G_RemoveInfoCenterRes::~GameMsg_M2G_RemoveInfoCenterRes()
{

}

bool GameMsg_M2G_RemoveInfoCenterRes::doDecode(CParamPool &IOBuff)
{
	m_id = IOBuff.GetUInt();
	return true;
}

bool GameMsg_M2G_RemoveInfoCenterRes::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_id);
	return true;
}


GameMsg_G2M_RemoveInfoCenterRes::GameMsg_G2M_RemoveInfoCenterRes()
	: GameMsg_Base(GM_G2M_RemoveInfoCenterRes)
	, m_result(0)
	, m_id(0)
{

}

GameMsg_G2M_RemoveInfoCenterRes::~GameMsg_G2M_RemoveInfoCenterRes()
{

}

bool GameMsg_G2M_RemoveInfoCenterRes::doDecode(CParamPool &IOBuff)
{
	m_result = IOBuff.GetUShort();
	m_id = IOBuff.GetUInt();

	return true;
}

bool GameMsg_G2M_RemoveInfoCenterRes::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_result);
	IOBuff.AddUInt(m_id);

	return true;
}



