#ifndef	__GAMEMSG_REDENVELOPE_H__
#define	__GAMEMSG_REDENVELOPE_H__

#include "../../../socket/GameMsg_Base.h"
#include "RedEnvelope.h"

class GameMsg_S2G_CreateRedEnvelope : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_CreateRedEnvelope);
    GameMsg_S2G_CreateRedEnvelope();
    virtual ~GameMsg_S2G_CreateRedEnvelope();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int        m_nRoleID;
    std::string         m_strRoleName;
    int                 m_nMCoin;
    unsigned int        m_nDanceGroupID;
    bool                m_bIsAnonymity;
};

class GameMsg_G2S_NewRedEnvelope : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_NewRedEnvelope);
    GameMsg_G2S_NewRedEnvelope();
    virtual ~GameMsg_G2S_NewRedEnvelope();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int            m_nSendRoleID;
    std::string             m_strRoleName;
    int                     m_nMCoin;
};

class GameMsg_S2G_OpenRedEnvelope : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_OpenRedEnvelope);
    GameMsg_S2G_OpenRedEnvelope();
    virtual ~GameMsg_S2G_OpenRedEnvelope();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int        m_nRoleID;
    unsigned int        m_nDanceGroupID;
    unsigned int        m_nRedEnvelopeID;
    std::string         m_strRoleName;
};

class GameMsg_G2S_OpenRedEnvelope : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_OpenRedEnvelope);
    GameMsg_G2S_OpenRedEnvelope();
    virtual ~GameMsg_G2S_OpenRedEnvelope();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int        m_nResultFlag;
    int                 m_nCurrencyType;
    int                 m_nCurrencyValue;
    unsigned int        m_nRedEnvelopeID;
    unsigned int        m_nOpenTime;
};

class GameMsg_S2G_GetRedEnvelopeList : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_GetRedEnvelopeList);
    GameMsg_S2G_GetRedEnvelopeList();
    virtual ~GameMsg_S2G_GetRedEnvelopeList();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int        m_nRoleID;
    unsigned int        m_nDanceGroupID;
};

class GameMsg_G2S_GetRedEnvelopeList : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_GetRedEnvelopeList);
    GameMsg_G2S_GetRedEnvelopeList();
    virtual ~GameMsg_G2S_GetRedEnvelopeList();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    std::list<unsigned int>         m_listGetRedEnvelopeID;
    std::list<BriefRedEnvelope>     m_listRedEnvelope;
};

class GameMsg_S2G_GetRedEnvelopeDetails : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_GetRedEnvelopeDetails);
    GameMsg_S2G_GetRedEnvelopeDetails();
    virtual ~GameMsg_S2G_GetRedEnvelopeDetails();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int        m_nRoleID;
    unsigned int        m_nRedEnvelopeID;
};

class GameMsg_G2S_GetRedEnvelopeDetails : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_GetRedEnvelopeDetails);
    GameMsg_G2S_GetRedEnvelopeDetails();
    virtual ~GameMsg_G2S_GetRedEnvelopeDetails();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    RedEnvelope         m_RedEnvelope;
};

class GameMsg_S2G_GetCanOpenRedEnvelopeCount : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_GetCanOpenRedEnvelopeCount);
    GameMsg_S2G_GetCanOpenRedEnvelopeCount();
    virtual ~GameMsg_S2G_GetCanOpenRedEnvelopeCount();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int        m_nRoleID;
    unsigned int        m_nDanceGroupID;
};

class GameMsg_G2S_GetCanOpenRedEnvelopeCount : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_GetCanOpenRedEnvelopeCount);
    GameMsg_G2S_GetCanOpenRedEnvelopeCount();
    virtual ~GameMsg_G2S_GetCanOpenRedEnvelopeCount();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int        m_nCanOpenCount;
};

#endif //
// end of file


