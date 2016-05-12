#ifndef __GAMEMSG_CS_LMAGICLUCKDRAW_H__
#define __GAMEMSG_CS_LMAGICLUCKDRAW_H__

#include "../../../socket/GameMsg_Base.h"
#include "../../../datastructure/DataStruct_Base.h"
#include "../../../datastructure/DataStruct_DB.h"

class GameMsg_C2S_RequestMagicArrayConfig : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_RequestMagicArrayConfig);
	GameMsg_C2S_RequestMagicArrayConfig(void);
	~GameMsg_C2S_RequestMagicArrayConfig(void);

public:
	virtual bool doDecode(CParamPool &IOBuff);
};

class GameMsg_S2C_RequestMagicArrayConfigResult : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_RequestMagicArrayConfigResult);
	GameMsg_S2C_RequestMagicArrayConfigResult(void);
	~GameMsg_S2C_RequestMagicArrayConfigResult(void);

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
    std::map<unsigned int, unsigned int>    m_mapCDRecord;
    std::map<unsigned int, MagicArrayRecord>    m_mapRecord;
};

class GameMsg_C2S_PreViewAllReward : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_PreViewAllReward);
    GameMsg_C2S_PreViewAllReward(void);
    ~GameMsg_C2S_PreViewAllReward(void);

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned int        m_nMagicArrayID;
    unsigned int        m_nGroupID;
    int                 m_nPage;
    int                 m_nPageCount;
};

class GameMsg_S2C_PreViewAllRewardResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_PreViewAllRewardResult);
    GameMsg_S2C_PreViewAllRewardResult(void);
    ~GameMsg_S2C_PreViewAllRewardResult(void);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned int    m_nMagicArrayID;
    unsigned int    m_nGroupID;
    int             m_nPage;
    int             m_nPageCount;
    int             m_nSex;
};

class GameMsg_C2S_RequestExchangeItem : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_RequestExchangeItem);
    GameMsg_C2S_RequestExchangeItem(void);
    ~GameMsg_C2S_RequestExchangeItem(void);

public:
    virtual bool doDecode(CParamPool &IOBuff);
};

class GameMsg_C2S_RefreshExchangeItem : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_RefreshExchangeItem);
    GameMsg_C2S_RefreshExchangeItem(void);
    ~GameMsg_C2S_RefreshExchangeItem(void);

public:
    virtual bool doDecode(CParamPool &IOBuff);
};

class GameMsg_S2C_RequestExchangeItemFail : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_RequestExchangeItemFail);
    GameMsg_S2C_RequestExchangeItemFail(void);
    ~GameMsg_S2C_RequestExchangeItemFail(void);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    int                             m_nError;
};

class GameMsg_S2C_RequestExchangeItemResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_RequestExchangeItemResult);
    GameMsg_S2C_RequestExchangeItemResult(void);
    ~GameMsg_S2C_RequestExchangeItemResult(void);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    int                             m_nSex;
    std::map<unsigned int,unsigned int>  m_mapExchangeGroup;
};

class GameMsg_C2S_MagicCall : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_MagicCall);
    GameMsg_C2S_MagicCall(void);
    ~GameMsg_C2S_MagicCall(void);

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
	unsigned int			m_nMagicArrayID;
    unsigned short          m_nCallType;
};

class CallReward
{
public:
    CallReward();
    virtual ~CallReward();

public:
    bool    m_bSpecial;
    CItem   m_RewardItem;
};

class GameMsg_S2C_MagicCallResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_MagicCallResult);
    GameMsg_S2C_MagicCallResult(void);
    ~GameMsg_S2C_MagicCallResult(void);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    int                     m_nErrorFlag;
    unsigned int            m_nMagicArrayID;
    unsigned int            m_nLeftCDTime;
    unsigned int            m_nFixedItemType;
    int                     m_nFixedItemCount;
    std::list<CallReward>   m_listRewardItem;
};

class GameMsg_S2C_NoticeCanFreeCall : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_NoticeCanFreeCall);
    GameMsg_S2C_NoticeCanFreeCall(void);
    ~GameMsg_S2C_NoticeCanFreeCall(void);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nMagicArrayID;
};


class GameMsg_C2S_MagicArrayExchange : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_MagicArrayExchange);
    GameMsg_C2S_MagicArrayExchange(void);
    ~GameMsg_C2S_MagicArrayExchange(void);

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned int			m_nExchangeID;
};

class GameMsg_S2C_MagicArrayExchangeResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_MagicArrayExchangeResult);
    GameMsg_S2C_MagicArrayExchangeResult(void);
    ~GameMsg_S2C_MagicArrayExchangeResult(void);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned int            m_nErrorFlag;
};

#endif

