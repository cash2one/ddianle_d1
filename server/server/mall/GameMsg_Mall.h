#ifndef __GAMEMSG_MALL_DEFINE_H__
#define __GAMEMSG_MALL_DEFINE_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"
#include "../../datastructure/DataStruct_Base.h"
#include <string>
#include <list>

class GameMsg_S2G_MallDesire :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_MallDesire);
	GameMsg_S2G_MallDesire(void);
	~GameMsg_S2G_MallDesire(void);

	itemtype_t m_nTypeID;
	unsigned int nAddNum;

public:
	virtual bool doDecode( CParamPool& IOBuff );
	virtual bool doEncode( CParamPool& IOBuff );
};

class GameMsg_G2S_MallDesire :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_MallDesire);
	GameMsg_G2S_MallDesire(void);
	~GameMsg_G2S_MallDesire(void);

	itemtype_t m_nTypeID;
	unsigned int nAddNum;
public:
	virtual bool doDecode( CParamPool& IOBuff );
	virtual bool doEncode( CParamPool& IOBuff );
};


class GameMsg_S2S_MallAskFor:
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2S_MallAskFor);
	GameMsg_S2S_MallAskFor(void);
	~GameMsg_S2S_MallAskFor(void);

	unsigned char   m_nResult;

	char            szAskForRoleName[MAX_NAME_LEN];
	unsigned int    m_nAskForRoleID;
	unsigned int    m_nDestRoleID;
	list<CAskMallGoodInfo>	m_listAskGoods;
	std::string     m_strInfo;

public:
	virtual bool doDecode( CParamPool& IOBuff );
	virtual bool doEncode( CParamPool& IOBuff );
};

class GameMsg_S2G_WishComeTrue:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_WishComeTrue);
	GameMsg_S2G_WishComeTrue();
	~GameMsg_S2G_WishComeTrue();
public:
	unsigned int m_nSrcRoleID;
	unsigned int m_nDstRoleID;
	itemtype_t m_nItemID;
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
};

class GameMsg_G2S_WishComeTrue:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_WishComeTrue);
	GameMsg_G2S_WishComeTrue();
	~GameMsg_G2S_WishComeTrue();
public:
	unsigned int m_nSrcRoleID;
	unsigned int m_nDstRoleID;
	itemtype_t m_nItemID;
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_G2S_MallReload : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_MallReload);
    GameMsg_G2S_MallReload();
    ~GameMsg_G2S_MallReload() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);
};

#endif

//end

