#include "GameMsg_Account_LoginResult.h"
#include "../../socket/ParamPool.h"
#include "AccountMsgDef.h"
#include <string.h>

GameMsg_Account_LoginResult::GameMsg_Account_LoginResult(void)
: GameMsg_Base(MSG_ACCOUNT_LoginResult)
, m_nLoginResult(-1)
, m_nVersionType(0)
, m_nGateSessionID(0)
, m_strErrorMsg("")
{
    memset(m_szRecTag, 0, MAX_REC_LEN);
}

GameMsg_Account_LoginResult::~GameMsg_Account_LoginResult(void)
{
}

bool GameMsg_Account_LoginResult::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddInt(m_nLoginResult);
    IOBuff.AddUChar(m_nVersionType);
    IOBuff.AddInt64(m_nGateSessionID);
    IOBuff.AddStringW(m_szRecTag);
    IOBuff.AddStringW(m_strErrorMsg.c_str());

    return true;
}

