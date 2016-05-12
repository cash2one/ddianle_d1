#ifndef	__GAMEMSG_CS_REDENVELOPE_H__
#define	__GAMEMSG_CS_REDENVELOPE_H__

#include "../../../socket/GameMsg_Base.h"
#include "RedEnvelope.h"

class GameMsg_C2S_GetCanOpenRedEnvelopeCount : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetCanOpenRedEnvelopeCount);
    GameMsg_C2S_GetCanOpenRedEnvelopeCount();
    virtual ~GameMsg_C2S_GetCanOpenRedEnvelopeCount();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
};

class GameMsg_S2C_UpdateCanOpenRedEnvelopeCount : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_UpdateCanOpenRedEnvelopeCount);
    GameMsg_S2C_UpdateCanOpenRedEnvelopeCount();
    virtual ~GameMsg_S2C_UpdateCanOpenRedEnvelopeCount();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int      m_nCanGetNum;
};

class GameMsg_C2S_GetRedEnvelopeList : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetRedEnvelopeList);
    GameMsg_C2S_GetRedEnvelopeList();
    virtual ~GameMsg_C2S_GetRedEnvelopeList();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
};

class GameMsg_S2C_GetRedEnvelopeListResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetRedEnvelopeListResult);
    GameMsg_S2C_GetRedEnvelopeListResult();
    virtual ~GameMsg_S2C_GetRedEnvelopeListResult();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    bool                            m_bAnonymity;
    bool                            m_bActivityShow;
    std::list<unsigned int>         m_listGetRedEnvelopeID;
    std::list<BriefRedEnvelope>     m_listBriefRedEnvelope;
    unsigned int                    m_nRoleID;
};

class GameMsg_C2S_GetRedEnvelopeDetails : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetRedEnvelopeDetails);
    GameMsg_C2S_GetRedEnvelopeDetails();
    virtual ~GameMsg_C2S_GetRedEnvelopeDetails();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int        m_nRedEnvelopeID;
};

class GameMsg_S2C_GetRedEnvelopeDetailsFail : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetRedEnvelopeDetailsFail);
    GameMsg_S2C_GetRedEnvelopeDetailsFail();
    virtual ~GameMsg_S2C_GetRedEnvelopeDetailsFail();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int        m_nError;
};

class GameMsg_S2C_GetRedEnvelopeDetailsResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetRedEnvelopeDetailsResult);
    GameMsg_S2C_GetRedEnvelopeDetailsResult();
    virtual ~GameMsg_S2C_GetRedEnvelopeDetailsResult();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    RedEnvelope     m_RedEnvelope;
};

class GameMsg_S2C_NewRedEnvelopeNotice : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_NewRedEnvelopeNotice);
    GameMsg_S2C_NewRedEnvelopeNotice();
    virtual ~GameMsg_S2C_NewRedEnvelopeNotice();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    std::string         m_strSendRoleName;
};

class GameMsg_C2S_SetAnonymity : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_SetAnonymity);
    GameMsg_C2S_SetAnonymity();
    virtual ~GameMsg_C2S_SetAnonymity();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);
};

class GameMsg_S2C_UpdateAnonymity : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_UpdateAnonymity);
    GameMsg_S2C_UpdateAnonymity();
    virtual ~GameMsg_S2C_UpdateAnonymity();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    bool        m_bAnonymity;
};

class GameMsg_C2S_OpenRedEnvelope : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_OpenRedEnvelope);
    GameMsg_C2S_OpenRedEnvelope();
    virtual ~GameMsg_C2S_OpenRedEnvelope();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int        m_nRedEnvelopeID;
};

class GameMsg_S2C_OpenRedEnvelopeResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_OpenRedEnvelopeResult);
    GameMsg_S2C_OpenRedEnvelopeResult();
    virtual ~GameMsg_S2C_OpenRedEnvelopeResult();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int            m_nRedEnvelopeID;
    unsigned int            m_nResult;
    int                     m_nCurrencyType;
    int                     m_nCurrencyValue;
    unsigned int            m_nOpenTime;
};

#endif //
// end of file


