#include "MailMsgDefine.h"
#include "../share/ServerMsgDef.h"
#include "../../socket/Utility.h"
#include "../../datastructure/datainfor/ServerConfig.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_C2S_GetMailBox
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_C2S_GetMailBox::GameMsg_C2S_GetMailBox(void):_baseClass(MSG_C2S_GETMAILBOX)
{
	nPage = 0;
}

GameMsg_C2S_GetMailBox::~GameMsg_C2S_GetMailBox(void)
{
}

bool GameMsg_C2S_GetMailBox::doDecode( CParamPool& IOBuff )
{
	nPage = IOBuff.GetUShort();
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_GetMailBox
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_GetMailBox::GameMsg_S2C_GetMailBox(void)
    : GameMsg_Base( MSG_S2C_GETMAILBOX )
    , m_nCount( 0 )
    , m_strPoolData( "" )
{
}

GameMsg_S2C_GetMailBox::~GameMsg_S2C_GetMailBox(void)
{
}

bool GameMsg_S2C_GetMailBox::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddUShort( m_nCount );

    if ( m_nCount > 0 && !m_strPoolData.empty() )
    {
        IOBuff.AddMemory( m_strPoolData.c_str(), (unsigned short)m_strPoolData.size() );
    }

    return true;
}

void GameMsg_S2C_GetMailBox::SetPoolData( const char *pData, unsigned int nDataSize )
{
    m_strPoolData.assign( pData, nDataSize );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_C2S_GetMailInfo
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_C2S_GetMailInfo::GameMsg_C2S_GetMailInfo(void)
    : _baseClass(MSG_C2S_GETMAILINFO)
{
    m_nMailID = 0;
    m_nType = 0;
}

bool GameMsg_C2S_GetMailInfo::doDecode( CParamPool& IOBuff )
{
    m_nMailID = IOBuff.GetInt64();

    return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_GetMailInfo
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_GetMailInfo::GameMsg_S2C_GetMailInfo(void)
    : _baseClass(MSG_S2C_GETMAILINFO)
{
    m_nResult = MAIL_FAIL;
}

bool GameMsg_S2C_GetMailInfo::doEncode( CParamPool& IOBuff )
{
    IOBuff.AddUChar(m_nResult);

    if (MAIL_OK == m_nResult)
        m_mailInfo.doEncode(IOBuff);

    return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_C2S_SendMail
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_C2S_SendMail::GameMsg_C2S_SendMail(void)
    : _baseClass(MSG_C2S_SENDMAIL)
{
    m_nReceiverID = 0;
    memset(m_szReceiverName,0,MAX_NAME_LEN);
    m_strTitle.clear();
    m_strContent.clear();
}

void GameMsg_C2S_SendMail::SetTitle(const std::string &rTitle)
{
    m_strTitle = rTitle;

    if (m_strTitle.size() > (size_t)g_ServerConfig.m_nMaxMailTitleLength)
        m_strTitle.resize(g_ServerConfig.m_nMaxMailTitleLength);
}

void GameMsg_C2S_SendMail::SetContent(const std::string &rContent)
{
    m_strContent = rContent;

    if (m_strContent.size() > (size_t)g_ServerConfig.m_nMaxMailContentLength)
        m_strContent.resize(g_ServerConfig.m_nMaxMailContentLength);
}

bool GameMsg_C2S_SendMail::doDecode( CParamPool& IOBuff )
{
    m_nReceiverID = IOBuff.GetUInt();

    string strTemp;
    IOBuff.GetStringW(strTemp);
    CopyString2Buffer(strTemp, m_szReceiverName, g_ServerConfig.m_MaxNameLimit - 1);

    IOBuff.GetStringW(strTemp);
    SetTitle(strTemp);

    IOBuff.GetStringW(strTemp);
    SetContent(strTemp);

    return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_SendMail
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_SendMail::GameMsg_S2C_SendMail(void):_baseClass(MSG_S2C_SENDMAIL)
{
	nResult = MAIL_FAIL;
}

GameMsg_S2C_SendMail::~GameMsg_S2C_SendMail(void)
{
}

bool GameMsg_S2C_SendMail::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUChar(nResult);
	return true;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_C2S_DeleteMail
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_C2S_DeleteMail::GameMsg_C2S_DeleteMail(void):_baseClass(MSG_C2S_DELETEMAIL)
{
}

GameMsg_C2S_DeleteMail::~GameMsg_C2S_DeleteMail(void)
{
}

bool GameMsg_C2S_DeleteMail::doDecode( CParamPool& IOBuff )
{
	unsigned short nCount = IOBuff.GetUShort();
	if (nCount > 0)
	{
		for (unsigned short i = 0;i<nCount;i++)
		{
			__int64 nMailID = IOBuff.GetInt64();
			aListMailID.push_back(nMailID);
		}
	}
	
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_DeleteMail
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_DeleteMail::GameMsg_S2C_DeleteMail(void):_baseClass(MSG_S2C_DELETEMAIL)
{
	nResult = MAIL_FAIL;
	strReturn = "";
}

GameMsg_S2C_DeleteMail::~GameMsg_S2C_DeleteMail(void)
{
}

bool GameMsg_S2C_DeleteMail::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUChar(nResult);
	IOBuff.AddStringW(strReturn.c_str());
	return true;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_C2S_GetMailItem
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_C2S_GetMailItem::GameMsg_C2S_GetMailItem(void):_baseClass(MSG_C2S_GETMAILITEM)
{
	nMailID = 0;
}

GameMsg_C2S_GetMailItem::~GameMsg_C2S_GetMailItem(void)
{
}

bool GameMsg_C2S_GetMailItem::doDecode( CParamPool& IOBuff )
{
	nMailID = IOBuff.GetInt64();
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_GetMailItem
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_GetMailItem::GameMsg_S2C_GetMailItem(void):_baseClass(MSG_S2C_GETMAILITEM)
{
	nResult = MAIL_FAIL;
	m_strError = "";
}

GameMsg_S2C_GetMailItem::~GameMsg_S2C_GetMailItem(void)
{
}

bool GameMsg_S2C_GetMailItem::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUChar(nResult);
	IOBuff.AddStringW(m_strError.c_str());
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_ReceiveMail
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_ReceiveMail::GameMsg_S2C_ReceiveMail(void):_baseClass(MSG_S2C_RECEIVEMAIL)
{
	nMailID = 0;
}

GameMsg_S2C_ReceiveMail::~GameMsg_S2C_ReceiveMail(void)
{
}

bool GameMsg_S2C_ReceiveMail::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddInt64(nMailID);
	return true;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_UnReadMail
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_UnReadMail::GameMsg_S2C_UnReadMail(void):_baseClass(MSG_S2C_UNREADMAIL)
{
	m_nUnReadCount = 0;
}

GameMsg_S2C_UnReadMail::~GameMsg_S2C_UnReadMail(void)
{
}

bool GameMsg_S2C_UnReadMail::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUShort(m_nUnReadCount);
	return true;
}

//end


