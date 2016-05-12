#include "GameMsg_Account_Login.h"
#include "../../socket/ParamPool.h"
#include "AccountMsgDef.h"
#include "../../socket/Utility.h"
#include "../../datastructure/Macro_Define.h"

GameMsg_Account_Login::GameMsg_Account_Login(void)
    : GameMsg_Base(MSG_ACCOUNT_Login)
    , m_nLine(0)
    , m_nVersionType(0)
    , m_nPID(0)
    , m_nGateSessionID(0)
{
    memset(m_szPass, 0, sizeof(m_szPass));
}

GameMsg_Account_Login::~GameMsg_Account_Login(void)
{
}

bool GameMsg_Account_Login::doDecode(CParamPool& ioBuff)
{
    m_nLine = ioBuff.GetInt();
    m_nVersionType = ioBuff.GetUChar();
    m_nPID = ioBuff.GetUShort();
    ioBuff.GetStringW(m_strPUID);
    m_nGateSessionID = ioBuff.GetInt64();
    ioBuff.GetStringW(m_strToken);
    ioBuff.GetStringW(m_strPhone);
    ioBuff.GetStringW(m_strName);
    ioBuff.GetMemory(m_szPass , MAX_MD5_LEN - 1);
    ioBuff.GetStringW(m_strLoginIP);

    if (m_strPUID.size() > MAX_PUID_LEN)
    {
        m_strPUID.resize(MAX_PUID_LEN);
    }
    if (m_strPhone.size() >= MAX_PHONE_LEN)
    {
        m_strPhone.resize(MAX_PHONE_LEN-1);
    }
    if (m_strName.size() >= MAX_NAME_LEN)
    {
        m_strName.resize(MAX_NAME_LEN-1);
    }
    if (m_strLoginIP.size() >= MAX_IP_LEN)
    {
        m_strLoginIP.resize(MAX_IP_LEN-1);
    }

    return true;
}

