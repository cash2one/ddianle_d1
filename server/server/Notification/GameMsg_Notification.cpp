#include "../share/ServerMsgDef.h"
#include "GameMsg_Notification.h"


GameMsg_C2S_NoPushRatingNotification::GameMsg_C2S_NoPushRatingNotification()
: GameMsg_Base(MSG_C2S_NoPushRatingNotification)
{
}

GameMsg_C2S_NoPushRatingNotification::~GameMsg_C2S_NoPushRatingNotification()
{
}

bool GameMsg_C2S_NoPushRatingNotification::doDecode(CParamPool &IOBuff)
{
	return true;
}



GameMsg_S2C_PushRatingNotification::GameMsg_S2C_PushRatingNotification()
: GameMsg_Base(MSG_S2C_PushRatingNotification)
{
}

GameMsg_S2C_PushRatingNotification::~GameMsg_S2C_PushRatingNotification()
{
}

bool GameMsg_S2C_PushRatingNotification::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW(m_strMessage.c_str());
	return true;
}

GameMsg_S2C_InfoCenterNotification::GameMsg_S2C_InfoCenterNotification()
	: GameMsg_Base(Msg_S2C_InfoCenterNotification)
	, m_id(0)
	, m_opTime(0)
	, m_type(0)
	, m_group(0)
	, m_opType(0)
{
}
GameMsg_S2C_InfoCenterNotification::~GameMsg_S2C_InfoCenterNotification()
{

}
void GameMsg_S2C_InfoCenterNotification::Set(unsigned int id, unsigned int opTime, unsigned char type, unsigned char group, unsigned char opType)
{
	m_id = id;
	m_opTime = opTime;
	m_type = type;
	m_group = group;
	m_opType = opType;
}
bool GameMsg_S2C_InfoCenterNotification::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar(m_opType);
	if (EInfoCenterOp_Remove != m_opType)
	{
		IOBuff.AddUInt(m_id);
        IOBuff.AddUChar(m_type);
		IOBuff.AddUInt(m_opTime);
		IOBuff.AddUChar(m_group);
	}
	else
	{
		IOBuff.AddUInt(m_id);
        IOBuff.AddUChar(m_type);
	}

	return true;
}

GameMsg_C2S_InfoCenterGetIDRes::GameMsg_C2S_InfoCenterGetIDRes()
	: GameMsg_Base(Msg_C2S_InfoCenterGetIDRes)
{
}

GameMsg_C2S_InfoCenterGetIDRes::~GameMsg_C2S_InfoCenterGetIDRes()
{

}

bool GameMsg_C2S_InfoCenterGetIDRes::doDecode(CParamPool &IOBuff)
{
	unsigned short usize = IOBuff.GetUShort();
	for (unsigned short i = 0; i < usize; ++i)
	{
		m_ids.push_back(IOBuff.GetUInt());
	}

	return true;
}

GameMsg_S2C_InfoCenterGetIDRes::GameMsg_S2C_InfoCenterGetIDRes()
	: GameMsg_Base(Msg_S2C_InfoCenterGetIDRes)
	, m_nErros(EInfoCenter_OK)
{
}

GameMsg_S2C_InfoCenterGetIDRes::~GameMsg_S2C_InfoCenterGetIDRes()
{

}
bool GameMsg_S2C_InfoCenterGetIDRes::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar(m_nErros);

	if (EInfoCenter_OK == m_nErros)
	{
		IOBuff.AddUShort((unsigned short)m_infos.size());
		for (size_t i = 0; i < m_infos.size(); ++i)
		{
			IOBuff.AddUInt(m_infos[i].m_id);
			IOBuff.AddUInt(m_infos[i].m_updateTime);
			IOBuff.AddUChar(m_infos[i].m_group);
			IOBuff.AddStringW(m_infos[i].m_icon);
			IOBuff.AddUChar(m_infos[i].m_type);
			IOBuff.AddStringW(m_infos[i].m_strTitle);
			IOBuff.AddStringW(m_infos[i].m_strContext);
			IOBuff.AddStringW(m_infos[i].m_strHref);
		}
	}

	return true;
}

GameMsg_C2S_InfoCenterMarkReaded::GameMsg_C2S_InfoCenterMarkReaded()
	: GameMsg_Base(Msg_C2S_InfoCenterMarkReaded)
{

}
GameMsg_C2S_InfoCenterMarkReaded::~GameMsg_C2S_InfoCenterMarkReaded()
{

}
bool GameMsg_C2S_InfoCenterMarkReaded::doDecode(CParamPool &IOBuff)
{
	unsigned short usize = IOBuff.GetUShort();
	for (unsigned short i = 0; i < usize; ++i)
	{
		m_ids.push_back(IOBuff.GetUInt());
	}

	return true;
}

