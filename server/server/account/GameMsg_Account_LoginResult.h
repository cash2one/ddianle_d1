#ifndef __GAMEMSG_ACCOUNT_LOGINRESULT_H__
#define __GAMEMSG_ACCOUNT_LOGINRESULT_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"

#include <string>

class CParamPool;

class GameMsg_Account_LoginResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_Account_LoginResult);
    GameMsg_Account_LoginResult(void);
    ~GameMsg_Account_LoginResult(void);

public:
    virtual bool doEncode(CParamPool& IoBuff);

public:
    int m_nLoginResult;
    unsigned char m_nVersionType;
    __int64 m_nGateSessionID;
    char m_szRecTag[MAX_REC_LEN];
    std::string m_strErrorMsg;
};

#endif

