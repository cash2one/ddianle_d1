#ifndef __GAMEMSG_ACCOUNT_LOGIN_H__
#define __GAMEMSG_ACCOUNT_LOGIN_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"

#include <string>

class CParamPool;

class GameMsg_Account_Login : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_Account_Login);

    GameMsg_Account_Login(void);
    virtual ~GameMsg_Account_Login(void);

public :
    virtual bool doDecode(CParamPool& IoBuff);

public:
    int               m_nLine;
    unsigned char     m_nVersionType;
    unsigned short    m_nPID;
    std::string       m_strPUID;
    __int64           m_nGateSessionID;
    std::string       m_strToken;
    std::string       m_strPhone;
    std::string       m_strName;
    char              m_szPass[MAX_MD5_LEN];
    std::string       m_strLoginIP;

};

#endif

