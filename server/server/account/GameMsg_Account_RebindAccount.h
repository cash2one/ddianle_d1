#ifndef __GAMEMSG_ACCOUNT_REBINDACCOUNT_H__
#define __GAMEMSG_ACCOUNT_REBINDACCOUNT_H__

#include "../../datastructure/Macro_Define.h"
#include "../../socket/GameMsg_Base.h"
#include <string>

class CParamPool;

class GameMsg_Account_RebindAccount :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_Account_RebindAccount);
    GameMsg_Account_RebindAccount();
    ~GameMsg_Account_RebindAccount();

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    char m_szName[MAX_NAME_LEN];        //MAX_NAME_LEN
    char m_szPass[MAX_PASS_LEN];
    char m_szPuid[MAX_PUID_LEN];        //最长身份证长度为18+1个字节		MAX_IDCARD_LEN
    int m_nPid;
    int m_nServerid;                    // 大区ID
    char m_szActcode[MAX_TRANSCODE_LEN];// 账号迁移码
};


class GameMsg_Account_RebindAccountResult :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_Account_RebindAccountResult);
    GameMsg_Account_RebindAccountResult();
    ~GameMsg_Account_RebindAccountResult();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char m_nResult; // 0: 成功，1:激活码不存在
    int m_nServerid;
    int m_nPid;
    std::string m_strName;
    std::string m_strPwd;
    std::string m_strPuid;
    std::string m_strActCode;
};


#endif // __GAMEMSG_ACCOUNT_REBINDACCOUNT_H__

