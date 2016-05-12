#include "GameMsg_Account_RebindAccount.h"
#include "../../socket/ParamPool.h"
#include "../../socket/Utility.h"
#include "AccountMsgDef.h"
#include "../../datastructure/datainfor/ItemType.h"

GameMsg_Account_RebindAccount::GameMsg_Account_RebindAccount()
: GameMsg_Base(MSG_ACCOUNT_RebindAccount)
, m_nPid(0)
, m_nServerid(0)
{
    memset(m_szName, 0, MAX_NAME_LEN);
    memset(m_szPass, 0, MAX_PASS_LEN);
    memset(m_szPuid, 0, MAX_PUID_LEN);
    memset(m_szActcode, 0, MAX_TRANSCODE_LEN);
}

GameMsg_Account_RebindAccount::~GameMsg_Account_RebindAccount()
{
}

bool GameMsg_Account_RebindAccount::doDecode(CParamPool &IOBuff)
{
    string strTemp;
    IOBuff.GetStringW(strTemp);
    safememcpy(m_szName, MAX_NAME_LEN, strTemp.c_str(), (unsigned int)strTemp.size());

    IOBuff.GetStringW(strTemp);
    safememcpy(m_szPass, MAX_PASS_LEN, strTemp.c_str(), (unsigned int)strTemp.size());

    IOBuff.GetStringW(strTemp);
    safememcpy(m_szPuid, MAX_PUID_LEN, strTemp.c_str(), (unsigned int)strTemp.size());

    m_nPid = IOBuff.GetInt();
    m_nServerid = IOBuff.GetInt();

    IOBuff.GetStringW(strTemp);
    safememcpy(m_szActcode, MAX_TRANSCODE_LEN, strTemp.c_str(), (unsigned int)strTemp.length());

    return true;
}


GameMsg_Account_RebindAccountResult::GameMsg_Account_RebindAccountResult()
: GameMsg_Base(MSG_ACCOUNT_RebindAccountResult)
, m_nResult(0)
, m_nServerid(0)
, m_nPid(0)
{

}

GameMsg_Account_RebindAccountResult::~GameMsg_Account_RebindAccountResult()
{


}

bool GameMsg_Account_RebindAccountResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nServerid);
    IOBuff.AddInt(m_nPid);
    IOBuff.AddUChar(m_nResult);
    IOBuff.AddStringW(m_strName);
    IOBuff.AddStringW(m_strPwd);
    IOBuff.AddStringW(m_strPuid);
    IOBuff.AddStringW(m_strActCode);

    return true;
}

