#include "GameMsg_S2S_Activities.h"
#include <string>
#include "../../socket/ParamPool.h"
#include "../share/ServerMsgDef.h"

GameMsg_S2G_BlessFriend::GameMsg_S2G_BlessFriend()
:GameMsg_Base(MSG_S2G_BlessFriend)
,m_nSenderID(0)
,m_nRcverID(0)
,m_strSenderName("")
,m_nWordsIndex(0)
,m_nBlessTime(0)
{

}

GameMsg_S2G_BlessFriend::~GameMsg_S2G_BlessFriend()
{

}

bool GameMsg_S2G_BlessFriend::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUInt(m_nSenderID);
	IOBuff.AddUInt(m_nRcverID);
	IOBuff.AddStringW(m_strSenderName.c_str());
	IOBuff.AddUShort(m_nWordsIndex);
	IOBuff.AddUInt(m_nBlessTime);

	return true;
}

bool GameMsg_S2G_BlessFriend::doDecode( CParamPool &IOBuff )
{
	m_nSenderID = IOBuff.GetUInt();
	m_nRcverID = IOBuff.GetUInt();
	IOBuff.GetStringW(m_strSenderName);
	m_nWordsIndex = IOBuff.GetUShort();
	m_nBlessTime = IOBuff.GetUInt();

	return true;
}
//-------------------------------------------------GameMsg_G2S_BlessFriend
GameMsg_G2S_BlessFriend::GameMsg_G2S_BlessFriend()
:GameMsg_Base(MSG_G2S_BlessFriend)
,m_nSenderID(0)
,m_nRcverID(0)
,m_strSenderName("")
,m_nWordsIndex(0)
,m_nBlessTime(0)
{

}

GameMsg_G2S_BlessFriend::~GameMsg_G2S_BlessFriend()
{

}

bool GameMsg_G2S_BlessFriend::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUInt(m_nSenderID);
	IOBuff.AddUInt(m_nRcverID);
	IOBuff.AddStringW(m_strSenderName.c_str());
	IOBuff.AddUShort(m_nWordsIndex);
	IOBuff.AddUInt(m_nBlessTime);

	return true;
}

bool GameMsg_G2S_BlessFriend::doDecode( CParamPool &IOBuff )
{
	m_nSenderID = IOBuff.GetUInt();
	m_nRcverID = IOBuff.GetUInt();
	IOBuff.GetStringW(m_strSenderName);
	m_nWordsIndex = IOBuff.GetUShort();
	m_nBlessTime = IOBuff.GetUInt();

	return true;
}


GameMsg_S2G_AddLongactingRechargeBoardCast::GameMsg_S2G_AddLongactingRechargeBoardCast()
    :GameMsg_Base(MSG_S2G_AddLongactingRechargeBroadCase)
{

}

GameMsg_S2G_AddLongactingRechargeBoardCast::~GameMsg_S2G_AddLongactingRechargeBoardCast()
{

}

bool GameMsg_S2G_AddLongactingRechargeBoardCast::doEncode( CParamPool &IOBuff )
{
    m_BoardCast.EncodeInfo(IOBuff);

    return true;
}

bool GameMsg_S2G_AddLongactingRechargeBoardCast::doDecode( CParamPool &IOBuff )
{
    m_BoardCast.DecodeInfo(IOBuff);

    return true;
}

GameMsg_S2G_GetLongactingRechargeBoardCast::GameMsg_S2G_GetLongactingRechargeBoardCast()
    :GameMsg_Base(MSG_S2G_GetLongactingRechargeBroadCase)
{

}

GameMsg_S2G_GetLongactingRechargeBoardCast::~GameMsg_S2G_GetLongactingRechargeBoardCast()
{

}

bool GameMsg_S2G_GetLongactingRechargeBoardCast::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddUInt(m_nRoleID);

    return true;
}

bool GameMsg_S2G_GetLongactingRechargeBoardCast::doDecode( CParamPool &IOBuff )
{
    m_nRoleID = IOBuff.GetUInt();

    return true;
}

GameMsg_G2S_GetLongactingRechargeBoardCast::GameMsg_G2S_GetLongactingRechargeBoardCast()
    :GameMsg_Base(MSG_G2S_GetLongactingRechargeBroadCase)
{

}

GameMsg_G2S_GetLongactingRechargeBoardCast::~GameMsg_G2S_GetLongactingRechargeBoardCast()
{

}

bool GameMsg_G2S_GetLongactingRechargeBoardCast::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddUShort((unsigned short)m_listLongactingBoardCast.size());
    std::list<LongactingCumulativeRechargeBoardCast>::iterator it = m_listLongactingBoardCast.begin();
    for (; it != m_listLongactingBoardCast.end(); ++it)
    {
        it->EncodeInfo(IOBuff);
    }
    return true;
}

bool GameMsg_G2S_GetLongactingRechargeBoardCast::doDecode( CParamPool &IOBuff )
{
    unsigned short nCount = IOBuff.GetShort();
    for (unsigned short i = 0; i < nCount; ++i)
    {
        LongactingCumulativeRechargeBoardCast cast;
        cast.DecodeInfo(IOBuff);

        m_listLongactingBoardCast.push_back(cast);
    }
    return true;
}

