#include "GameMsg_External.h"
#include "../share/ServerMsgDef.h"
#include "../../datastructure/datainfor/ServerConfig.h"

GameMsg_EX2G_SendMail::GameMsg_EX2G_SendMail()
: GameMsg_Base(MSG_EX2G_SendMail)
, m_nPID(0)
, m_strPUID("")
, m_strTitle("")
, m_strContent("")
, m_nMoney(0)
, m_nItemType(0)
, m_nCount(0)
, m_nValidTime(0)
, m_strOrderID("")
{
}

GameMsg_EX2G_SendMail::~GameMsg_EX2G_SendMail()
{
}

bool GameMsg_EX2G_SendMail::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nPID);
	IOBuff.AddStringW(m_strPUID.c_str());
	IOBuff.AddStringW(m_strTitle.c_str());
	IOBuff.AddStringW(m_strContent.c_str());
	IOBuff.AddUInt(m_nMoney);
	IOBuff.AddUInt(m_nItemType);
	IOBuff.AddUShort(m_nCount);
	IOBuff.AddInt(m_nValidTime);
	IOBuff.AddStringW(m_strOrderID.c_str());
	return true;
}

bool GameMsg_EX2G_SendMail::doDecode(CParamPool &IOBuff)
{
	m_nPID = IOBuff.GetUShort();
	IOBuff.GetStringW(m_strPUID);
	if (m_strPUID.size() > MAX_PUID_LEN)
	{
		m_strPUID.resize(MAX_PUID_LEN);
	}
	IOBuff.GetStringW(m_strTitle);
	if (m_strTitle.size() >= (size_t)g_ServerConfig.m_nMaxMailTitleLength)
	{
		m_strTitle.resize(g_ServerConfig.m_nMaxMailTitleLength);
	}
	IOBuff.GetStringW(m_strContent);
	if (m_strContent.size() >= (size_t)g_ServerConfig.m_nMaxMailContentLength)
	{
		m_strContent.resize(g_ServerConfig.m_nMaxMailContentLength);
	}
	m_nMoney = IOBuff.GetUInt();
	m_nItemType = IOBuff.GetUInt();
	m_nCount = IOBuff.GetUShort();
	m_nValidTime = IOBuff.GetInt();
	IOBuff.GetStringW(m_strOrderID);
	if (m_strOrderID.size() > 64)
	{
		m_strOrderID.resize(64);
	}
	return true;
}

GameMsg_EX2G_SendMailGift::GameMsg_EX2G_SendMailGift()
: GameMsg_Base(MSG_EX2G_SendMailGift)
, m_nPID(0)
, m_strPUID("")
, m_nMailGiftID(0)
, m_strOrderID("")
{
}

GameMsg_EX2G_SendMailGift::~GameMsg_EX2G_SendMailGift()
{
}

bool GameMsg_EX2G_SendMailGift::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nPID);
	IOBuff.AddStringW(m_strPUID.c_str());
	IOBuff.AddInt(m_nMailGiftID);
	IOBuff.AddStringW(m_strOrderID.c_str());
	return true;
}

bool GameMsg_EX2G_SendMailGift::doDecode(CParamPool &IOBuff)
{
	m_nPID = IOBuff.GetUShort();

	IOBuff.GetStringW(m_strPUID);
	if (m_strPUID.size() > MAX_PUID_LEN)
	{
		m_strPUID.resize(MAX_PUID_LEN);
	}

	m_nMailGiftID = IOBuff.GetInt();
	IOBuff.GetStringW(m_strOrderID);
	if (m_strOrderID.size() > 64)
	{
		m_strOrderID.resize(64);
	}
	return true;
}


