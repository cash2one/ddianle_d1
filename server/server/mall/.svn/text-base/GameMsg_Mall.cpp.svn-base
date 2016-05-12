#include "GameMsg_Mall.h"
#include "../share/ServerMsgDef.h"
#include "../../socket/Utility.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2G_MallDesire
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2G_MallDesire::GameMsg_S2G_MallDesire(void):_baseClass(MSG_S2G_MALLDESIRE)
{
	m_nTypeID = 0;
	nAddNum = 0;
}

GameMsg_S2G_MallDesire::~GameMsg_S2G_MallDesire(void)
{
}

bool GameMsg_S2G_MallDesire::doDecode( CParamPool& IOBuff )
{
	m_nTypeID = IOBuff.GetUInt();
	nAddNum = IOBuff.GetUInt();
	return true;
}

bool GameMsg_S2G_MallDesire::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt(m_nTypeID);
	IOBuff.AddUInt(nAddNum);
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_G2S_MallDesire
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_G2S_MallDesire::GameMsg_G2S_MallDesire(void):_baseClass(MSG_G2S_MALLDESIRE)
{
	m_nTypeID = 0;
	nAddNum = 0;
}

GameMsg_G2S_MallDesire::~GameMsg_G2S_MallDesire(void)
{
}

bool GameMsg_G2S_MallDesire::doDecode( CParamPool& IOBuff )
{
	m_nTypeID = IOBuff.GetUInt();
	nAddNum = IOBuff.GetUInt();
	return true;
}

bool GameMsg_G2S_MallDesire::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt(m_nTypeID);
	IOBuff.AddUInt(nAddNum);
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2S_MallAskFor
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2S_MallAskFor::GameMsg_S2S_MallAskFor(void):_baseClass(MSG_S2S_MALL_ASKFOR)
{
	m_nResult = 0;
	memset(szAskForRoleName,0,MAX_NAME_LEN);
	m_nDestRoleID = 0;
}

GameMsg_S2S_MallAskFor::~GameMsg_S2S_MallAskFor(void)
{
}

bool GameMsg_S2S_MallAskFor::doDecode( CParamPool& IOBuff )
{
	m_nResult = IOBuff.GetUChar();

	std::string strName;
	strName = IOBuff.GetString();
	CopyString2Buffer(strName,szAskForRoleName,MAX_NAME_LEN-1);
	m_nAskForRoleID = IOBuff.GetUInt();
	m_nDestRoleID = IOBuff.GetUInt();

	int nSize = IOBuff.GetUShort();
	while (nSize-- > 0) {
		CAskMallGoodInfo good;
		good.doDecode( IOBuff );
		m_listAskGoods.push_back(good);
	}
	m_strInfo = IOBuff.GetString();
	return true;
}

bool GameMsg_S2S_MallAskFor::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUChar(m_nResult);
	IOBuff.AddString(szAskForRoleName);
	IOBuff.AddUInt(m_nAskForRoleID);
	IOBuff.AddUInt(m_nDestRoleID);

	IOBuff.AddUShort((unsigned short)m_listAskGoods.size());
	for (list<CAskMallGoodInfo>::iterator itr = m_listAskGoods.begin(); itr != m_listAskGoods.end(); ++itr) {
		itr->doEncode(IOBuff);
	}
	IOBuff.AddString(m_strInfo.c_str());
	return true;
}

GameMsg_S2G_WishComeTrue::GameMsg_S2G_WishComeTrue():
GameMsg_Base(MSG_S2G_WishComeTrue),
m_nSrcRoleID(0),
m_nDstRoleID(0),
m_nItemID(0)
{
}

GameMsg_S2G_WishComeTrue::~GameMsg_S2G_WishComeTrue()
{
}

bool GameMsg_S2G_WishComeTrue::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nSrcRoleID);
	IOBuff.AddUInt(m_nDstRoleID);
	IOBuff.AddUInt(m_nItemID);

	return true;
}

bool GameMsg_S2G_WishComeTrue::doDecode(CParamPool &IOBuff)
{
	m_nSrcRoleID = IOBuff.GetUInt();
	m_nDstRoleID = IOBuff.GetUInt();
	m_nItemID = IOBuff.GetUInt();

	return true;
}

GameMsg_G2S_WishComeTrue::GameMsg_G2S_WishComeTrue():
GameMsg_Base(MSG_G2S_WishComeTrue),
m_nSrcRoleID(0),
m_nDstRoleID(0),
m_nItemID(0)
{
}

GameMsg_G2S_WishComeTrue::~GameMsg_G2S_WishComeTrue()
{
}

bool GameMsg_G2S_WishComeTrue::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nSrcRoleID);
	IOBuff.AddUInt(m_nDstRoleID);
	IOBuff.AddUInt(m_nItemID);

	return true;
}

bool GameMsg_G2S_WishComeTrue::doDecode(CParamPool &IOBuff)
{
	m_nSrcRoleID = IOBuff.GetUInt();
	m_nDstRoleID = IOBuff.GetUInt();
	m_nItemID = IOBuff.GetUInt();

	return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_G2S_MallReload::GameMsg_G2S_MallReload()
    : GameMsg_Base(MSG_G2S_Mall_Reload)
{
}

bool GameMsg_G2S_MallReload::doDecode(CParamPool &IOBuff)
{
    return true;
}

bool GameMsg_G2S_MallReload::doEncode(CParamPool &IOBuff)
{
    return true;
}

//file end

