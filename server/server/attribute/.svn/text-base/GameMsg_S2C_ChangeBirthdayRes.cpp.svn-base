#include "GameMsg_S2C_ChangeBirthdayRes.h"
#include "../share/ServerMsgDef.h"
#include "../../datastructure/datainfor/ServerConfig.h"

GameMsg_S2C_ChangeBirthdayRes::GameMsg_S2C_ChangeBirthdayRes(unsigned char nResult /* = CHANGEBIRTHDAY_SUCCESS */)
: GameMsg_Base(MSG_S2C_ChangeBirthdayRes)
, m_nResult(nResult)
, m_nConstellation(0)
{
    m_strBirthday.clear();
}

GameMsg_S2C_ChangeBirthdayRes::~GameMsg_S2C_ChangeBirthdayRes()
{
}

void GameMsg_S2C_ChangeBirthdayRes::SetBirthday(const std::string &rBirthday)
{
    m_strBirthday = rBirthday;

    if (m_strBirthday.size() > (size_t)g_ServerConfig.m_nMaxBirthdayLength)
        m_strBirthday.resize(g_ServerConfig.m_nMaxBirthdayLength);
}

bool GameMsg_S2C_ChangeBirthdayRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nResult);
    IOBuff.AddStringW(m_strBirthday);
    IOBuff.AddUChar(m_nConstellation);

    return true;
}

