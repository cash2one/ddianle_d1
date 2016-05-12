#include "GameMsg_G2S_ChatWindow.h"
#include "ChatMsgDef.h"
#include "../../socket/ParamPool.h"

GameMsg_G2S_ChatWindow::GameMsg_G2S_ChatWindow():GameMsg_Base(MSG_G2S_ChatWindow)
{
	m_strText = "";
}
GameMsg_G2S_ChatWindow::~GameMsg_G2S_ChatWindow()
{
}

bool GameMsg_G2S_ChatWindow::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddString(m_strText.c_str());

	return true;
}

bool GameMsg_G2S_ChatWindow::doDecode(CParamPool &IOBuff)
{
	m_strText = IOBuff.GetString();	
	
	return true;
}




//file end

