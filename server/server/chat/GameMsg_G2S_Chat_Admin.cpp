#include "GameMsg_G2S_Chat_Admin.h"
#include "../../datastructure/Macro_Define.h"
#include "ChatMsgDef.h"

GameMsg_G2S_Chat_Admin::GameMsg_G2S_Chat_Admin():GameMsg_Base(MSG_G2S_Admin)
{
	m_nColor = CHATCOLOR_ADMINCHANNEL;
	m_strText = "";
}

GameMsg_G2S_Chat_Admin::GameMsg_G2S_Chat_Admin(unsigned char nColor, const char *szText):GameMsg_Base(MSG_G2S_Admin)
{
	m_nColor = nColor;	
	m_strText = szText;
}


GameMsg_G2S_Chat_Admin::~GameMsg_G2S_Chat_Admin()
{
}

bool GameMsg_G2S_Chat_Admin::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar(m_nColor);
	IOBuff.AddString(m_strText.c_str());

	return true;
}
bool GameMsg_G2S_Chat_Admin::doDecode(CParamPool &IOBuff)
{
	m_nColor = IOBuff.GetUChar();
	m_strText = IOBuff.GetString();

	return true;
}

//file end

