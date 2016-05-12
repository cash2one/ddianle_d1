#include "GameMsg_Mail.h"
#include "../share/ServerMsgDef.h"
#include "MailDefine.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2G_SendMail
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2G_SendMail::GameMsg_S2G_SendMail(void):_baseClass(MSG_S2G_SENDMAIL)
{
	nNoticeRoleID = 0;
}

GameMsg_S2G_SendMail::~GameMsg_S2G_SendMail(void)
{
}

bool GameMsg_S2G_SendMail::doDecode( CParamPool& IOBuff )
{
	nNoticeRoleID = IOBuff.GetUInt();
	m_mailInfo.doDecodeFromServer1(IOBuff);
	return true;
}

bool GameMsg_S2G_SendMail::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt(nNoticeRoleID);
	m_mailInfo.doEncodeToServer1(IOBuff);
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_G2S_SendMail
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_G2S_SendMail::GameMsg_G2S_SendMail(void):_baseClass(MSG_G2S_SENDMAIL)
{
}

GameMsg_G2S_SendMail::~GameMsg_G2S_SendMail(void)
{
}

bool GameMsg_G2S_SendMail::doDecode( CParamPool& IOBuff )
{
	m_mailInfo.doDecodeFromServer1(IOBuff);

	return true;
}

bool GameMsg_G2S_SendMail::doEncode( CParamPool& IOBuff )
{
	m_mailInfo.doEncodeToServer1(IOBuff);

	return true;
}


GameMsg_A2S_SendMail::GameMsg_A2S_SendMail( void ) :_baseClass( MSG_A2S_SENDMAIL )
{
	m_nRoleID = 0;
}

GameMsg_A2S_SendMail::~GameMsg_A2S_SendMail( void )
{

}

bool GameMsg_A2S_SendMail::doDecode( CParamPool& IOBuff )
{
	m_nRoleID = IOBuff.GetUInt( );
	m_mailInfo.doDecodeFromServer( IOBuff );

	return true;
}

bool GameMsg_A2S_SendMail::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt( m_nRoleID );
	m_mailInfo.doEncodeToServer( IOBuff );

	return true;
}



GameMsg_C2S_ReceiveStrangeMailFlag::GameMsg_C2S_ReceiveStrangeMailFlag()
    : GameMsg_Base(Msg_C2S_ReceiveStrangeMailFlag)
{

}
GameMsg_C2S_ReceiveStrangeMailFlag::~GameMsg_C2S_ReceiveStrangeMailFlag(){}

bool GameMsg_C2S_ReceiveStrangeMailFlag::doDecode(CParamPool& IOBuff)
{
    return true;
}

bool GameMsg_C2S_ReceiveStrangeMailFlag::doEncode(CParamPool& IOBuff)
{
    return true;
}


GameMsg_S2C_ReceiveStrangeMailFlag::GameMsg_S2C_ReceiveStrangeMailFlag()
    : GameMsg_Base(Msg_S2C_ReceiveStrangeMailFlag)
    , m_bSet(false)
{

}
GameMsg_S2C_ReceiveStrangeMailFlag::~GameMsg_S2C_ReceiveStrangeMailFlag()
{

}

bool GameMsg_S2C_ReceiveStrangeMailFlag::doDecode(CParamPool& IOBuff)
{
    m_bSet = IOBuff.GetBool();

    return true;
}

bool GameMsg_S2C_ReceiveStrangeMailFlag::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddBool(m_bSet);

    return true;
}


GameMsg_C2S_SetReceiveStrangeMailFlag::GameMsg_C2S_SetReceiveStrangeMailFlag()
    : GameMsg_Base(Msg_C2S_SetReceiveStrangeMailFlag)
    , m_bSet(false)
{

}
GameMsg_C2S_SetReceiveStrangeMailFlag::~GameMsg_C2S_SetReceiveStrangeMailFlag()
{

}

bool GameMsg_C2S_SetReceiveStrangeMailFlag::doDecode(CParamPool& IOBuff)
{
    m_bSet = IOBuff.GetBool();
    return true;
}

bool GameMsg_C2S_SetReceiveStrangeMailFlag::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddBool(m_bSet);

    return true;
}

GameMsg_S2C_SetReceiveStrangeMailFlag::GameMsg_S2C_SetReceiveStrangeMailFlag()
    : GameMsg_Base(Msg_S2C_SetReceiveStrangeMailFlag)
    , m_bSet(false)
{

}
GameMsg_S2C_SetReceiveStrangeMailFlag::~GameMsg_S2C_SetReceiveStrangeMailFlag()
{}

bool GameMsg_S2C_SetReceiveStrangeMailFlag::doDecode(CParamPool& IOBuff)
{
    m_bSet = IOBuff.GetBool();
    return true;
}
bool GameMsg_S2C_SetReceiveStrangeMailFlag::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddBool(m_bSet);

    return true;
}


GameMsg_C2S_CanSendMail::GameMsg_C2S_CanSendMail()
    : GameMsg_Base(Msg_C2S_CanSendMail)
    , m_nReceiveRoleID(0)
    , m_strReceiveRoleName("")
{

}
GameMsg_C2S_CanSendMail::~GameMsg_C2S_CanSendMail()
{

}

bool GameMsg_C2S_CanSendMail::doDecode(CParamPool& IOBuff)
{
    m_nReceiveRoleID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strReceiveRoleName);

    return true;
}
bool GameMsg_C2S_CanSendMail::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUInt(m_nReceiveRoleID);
    IOBuff.AddStringW(m_strReceiveRoleName);

    return true;
}

GameMsg_S2C_CanSendMail::GameMsg_S2C_CanSendMail()
    : GameMsg_Base(Msg_S2C_CanSendMail)
    , m_nRet(0)
{
}

GameMsg_S2C_CanSendMail::~GameMsg_S2C_CanSendMail()
{
    
}

bool GameMsg_S2C_CanSendMail::doDecode(CParamPool& IOBuff)
{
    m_nRet = IOBuff.GetUChar();
    return true;
}

bool GameMsg_S2C_CanSendMail::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUChar(m_nRet);

    return true;
}

GameMsg_S2G_ValidSendMail::GameMsg_S2G_ValidSendMail()
    : GameMsg_Base(MSG_S2G_ValidSendMail)
{}
GameMsg_S2G_ValidSendMail::~GameMsg_S2G_ValidSendMail()
{}
bool GameMsg_S2G_ValidSendMail::doDecode(CParamPool& IOBuff)
{
    m_mailInfoDB.doDecode(IOBuff);
    return true;
}

bool GameMsg_S2G_ValidSendMail::doEncode(CParamPool& IOBuff)
{
    m_mailInfoDB.doEncode(IOBuff);
    return true;
}


GameMsg_G2S_ValidSendMail::GameMsg_G2S_ValidSendMail()
    : GameMsg_Base(MSG_G2S_ValidSendMail)
    , m_nRes(MAIL_OK)
{}
GameMsg_G2S_ValidSendMail::~GameMsg_G2S_ValidSendMail(){}

bool GameMsg_G2S_ValidSendMail::doDecode(CParamPool& IOBuff)
{
    m_nRes = IOBuff.GetUChar();
    m_mailInfoDB.doDecode(IOBuff);

    return true;
}

bool GameMsg_G2S_ValidSendMail::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUChar(m_nRes);
    m_mailInfoDB.doEncode(IOBuff);

    return true;
}


//end file

