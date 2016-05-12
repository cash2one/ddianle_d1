#ifndef __MALLMSGDEFINE_H__
#define __MALLMSGDEFINE_H__

#include "../../socket/GameMsg_Base.h"
#include "MallDefine.h"
#include <string>
#include <list>

using namespace std;

class GameMsg_C2S_MallGoodsInfo :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_MallGoodsInfo);
	GameMsg_C2S_MallGoodsInfo(void);
	~GameMsg_C2S_MallGoodsInfo(void);

	unsigned char nType;

public:
	virtual bool doDecode( CParamPool& IOBuff );
};



class GameMsg_S2C_MallGoodsInfo :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_MallGoodsInfo);
	GameMsg_S2C_MallGoodsInfo(void);
	~GameMsg_S2C_MallGoodsInfo(void);

	unsigned short nDataSize;
	char * pData;

public:
	virtual bool doEncode( CParamPool& IOBuff );
};


class GameMsg_C2S_MallSomeGoods :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_MallSomeGoods);
	GameMsg_C2S_MallSomeGoods(void);
	~GameMsg_C2S_MallSomeGoods(void);

	std::list<itemtype_t>  aListType;

public:
	virtual bool doDecode( CParamPool& IOBuff );
};



class GameMsg_S2C_MallSomeGoodsResult :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_MallSomeGoodsResult);
	GameMsg_S2C_MallSomeGoodsResult(void);
	~GameMsg_S2C_MallSomeGoodsResult(void);

	MapMallGoods aGoodMap;

public:
	virtual bool doEncode( CParamPool& IOBuff );
};


class GameMsg_C2S_MallBuy :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_MallBuy);
	GameMsg_C2S_MallBuy(void);
	~GameMsg_C2S_MallBuy(void);

	unsigned char m_nBuyFromType;
	VecPeddleGoods vecGoods;

public:
	virtual bool doDecode( CParamPool& IOBuff );
};


class GameMsg_S2C_MallMallBuyResult :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_MallMallBuyResult);
	GameMsg_S2C_MallMallBuyResult(void);
	~GameMsg_S2C_MallMallBuyResult(void);

	unsigned char m_nBuyFromType;
	unsigned char nResult;
	std::string strReturn;

public:
	virtual bool doEncode( CParamPool& IOBuff );
};


class GameMsg_C2S_MallSend :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_MallSend);
	GameMsg_C2S_MallSend(void);
	~GameMsg_C2S_MallSend(void);

	unsigned int m_nDestRoleID;
	VecPeddleGoods vecGoods;
	string  m_strInfo;

public:
	virtual bool doDecode( CParamPool& IOBuff );
};


class GameMsg_S2C_MallMallSendResult :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_MallMallSendResult);
	GameMsg_S2C_MallMallSendResult(void);
	~GameMsg_S2C_MallMallSendResult(void);

	unsigned char nResult;
	std::string strReturn;

public:
	virtual bool doEncode( CParamPool& IOBuff );
};


class GameMsg_C2S_MallAskFor:
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_MallAskFor);
	GameMsg_C2S_MallAskFor(void);
	~GameMsg_C2S_MallAskFor(void);

	unsigned int    m_nDestRoleID;
	list<CAskMallGoodInfo> m_listAskGoods;
	std::string     m_strInfo;

public:
	virtual bool doDecode( CParamPool& IOBuff );
};

class GameMsg_S2C_MallMallAskForResult :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_MallMallAskForResult);
	GameMsg_S2C_MallMallAskForResult(void);
	~GameMsg_S2C_MallMallAskForResult(void);

	unsigned char nResult;
	std::string strReturn;

public:
	virtual bool doEncode( CParamPool& IOBuff );
};


class GameMsg_S2C_MallMallBeAskedFor :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_MallMallBeAskedFor);
	GameMsg_S2C_MallMallBeAskedFor(void);
	~GameMsg_S2C_MallMallBeAskedFor(void);

	char szAskForRoleName[MAX_NAME_LEN];
	unsigned int  nAskForRoleID;
	list<CAskMallGoodInfo> m_listAskGoodInfo;
	MapMallGoods  aGoodMap;
	std::string   m_strInfo;

public:
	virtual bool doEncode( CParamPool& IOBuff );
};


class GameMsg_C2S_MallDesire :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_MallDesire);
	GameMsg_C2S_MallDesire(void);
	~GameMsg_C2S_MallDesire(void);

	itemtype_t m_nTypeID;

public:
	virtual bool doDecode( CParamPool& IOBuff );
};


class GameMsg_C2S_RequestDesireList :
    public GameMsg_Base
{
    typedef  GameMsg_Base _baseClass;

public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_RequestDesireList);
    GameMsg_C2S_RequestDesireList(void);
    ~GameMsg_C2S_RequestDesireList(void);

public:
    virtual bool doDecode(CParamPool& IOBuff);
};

class GameMsg_S2C_RequestDesireListResult :
    public GameMsg_Base
{
    typedef  GameMsg_Base _baseClass;
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_RequestDesireListResult);
    GameMsg_S2C_RequestDesireListResult(void);
    ~GameMsg_S2C_RequestDesireListResult(void);

public:
    virtual bool doEncode(CParamPool& IOBuff);

public:
    std::list<itemtype_t>           m_listDesire;
};


class GameMsg_S2C_MallDesireResult :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_MallDesireResult);
	GameMsg_S2C_MallDesireResult(void);
	~GameMsg_S2C_MallDesireResult(void);

	unsigned char nResult;
	std::string strReturn;

public:
	virtual bool doEncode( CParamPool& IOBuff );
};

class GameMsg_C2S_RemoveDesire :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_RemoveDesire);
	GameMsg_C2S_RemoveDesire(); 
	~GameMsg_C2S_RemoveDesire();
public:
	itemtype_t m_nTypeID;
public:
	bool doDecode(CParamPool & IOBuff);
};

class GameMsg_S2C_RemoveDesireItemFail :
public	GameMsg_Base
{
public:
	GameMsg_S2C_RemoveDesireItemFail();
	~GameMsg_S2C_RemoveDesireItemFail();
public:
	bool doEncode(CParamPool & IOBuff);
public:
	string m_strEorror;
};


class GameMsg_S2C_RemoveDesireScuess :
    public	GameMsg_Base
{
public:
    GameMsg_S2C_RemoveDesireScuess();
    ~GameMsg_S2C_RemoveDesireScuess();

public:
    bool doEncode(CParamPool & IOBuff);

public:
    itemtype_t m_nItemType;
};


class GameMsg_C2S_RenewItem :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_RenewItem);
	GameMsg_C2S_RenewItem(void);
	~GameMsg_C2S_RenewItem(void);

	unsigned short nColumn;
	unsigned short nIndex;

	bool m_bFirstUseChange;
	VecPeddleGoods vecGoods;

public:
	virtual bool doDecode( CParamPool& IOBuff );
};


class GameMsg_S2C_MallRenewResult :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_MallRenewResult);
	GameMsg_S2C_MallRenewResult(void);
	~GameMsg_S2C_MallRenewResult(void);

	unsigned char nResult;
	std::string strReturn;

public:
	virtual bool doEncode( CParamPool& IOBuff );
};

class GameMsg_C2S_SendWishItem :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_SendWishItem);
	GameMsg_C2S_SendWishItem();	
	~GameMsg_C2S_SendWishItem();
public:
	unsigned int m_nDestRoleID;
	bool m_bFirstUseChange;
	VecPeddleGoods m_vecGoods;
	string  m_strInfo;
public:
	bool doDecode(CParamPool & IOBuff);
};

class GameMsg_S2C_SendWishItemSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_SendWishItemSuc();
	~GameMsg_S2C_SendWishItemSuc();
public:
	unsigned int m_nRoleID;
	itemtype_t m_nItemID;
public:
	bool doEncode(CParamPool &IOBuff);
};

class GameMsg_S2C_SendWishItemFail:
	public GameMsg_Base
{
public:
	GameMsg_S2C_SendWishItemFail();
	~GameMsg_S2C_SendWishItemFail();
public:
	string m_strErrorMsg;
public:
	bool doEncode(CParamPool &IOBuff);
};

class GameMsg_C2S_BeAskedFor_Send:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_BeAskedFor_Send);
	GameMsg_C2S_BeAskedFor_Send();
	~GameMsg_C2S_BeAskedFor_Send();	
public:
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nDestRoleID;
	VecPeddleGoods vecGoods;
	string  m_strInfo;
};

class GameMsg_S2C_BeAskedFor_SendRes:
	public GameMsg_Base
{
public:
	GameMsg_S2C_BeAskedFor_SendRes();
	~GameMsg_S2C_BeAskedFor_SendRes();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	unsigned char m_nResult;
	std::string m_strReturn;
};

class GameMsg_C2S_EnterMall :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_EnterMall);
	GameMsg_C2S_EnterMall();	
	~GameMsg_C2S_EnterMall();
public:
	bool doDecode(CParamPool & IOBuff);
};

class GameMsg_S2C_EnterMallRes :
	public GameMsg_Base
{
public:
	GameMsg_S2C_EnterMallRes();
	~GameMsg_S2C_EnterMallRes();

public:
	bool doEncode(CParamPool &IOBuff);

public:
	bool m_bNotifyWillResetMoney;
	std::string m_strMallTipTitle;
	std::string m_strMallTip;
};

class GameMsg_S2C_AllGoodsInfo :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GameMsg_S2C_AllGoodsInfo();
	~GameMsg_S2C_AllGoodsInfo(void);

public:
	virtual bool doEncode( CParamPool& IOBuff );

public:
	unsigned short m_nMessageCount;		// 商品信息消息总个数
	unsigned short m_nMessageIndex;		// 当前这个消息的索引号
	unsigned short m_nGoodsCount;		// 这个消息内的商品数量
	list<itemtype_t> m_listGoods;	// 商品列表信息
};

class GameMsg_C2S_ActionEnterMall :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_ActionEnterMall);
    GameMsg_C2S_ActionEnterMall();	
    ~GameMsg_C2S_ActionEnterMall();

public:
    bool doDecode(CParamPool & IOBuff);
};
// 体验卡信息请求
class GameMsg_C2S_ExperienceCardInfo :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ExperienceCardInfo);
	GameMsg_C2S_ExperienceCardInfo(void);
	~GameMsg_C2S_ExperienceCardInfo(void);

public:
	virtual bool doDecode( CParamPool& IOBuff );
};

class GameMsg_S2C_ExperienceCardInfoRes : public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_ExperienceCardInfoRes);
	GameMsg_S2C_ExperienceCardInfoRes(void);
	~GameMsg_S2C_ExperienceCardInfoRes(void);

public:
	virtual bool doEncode( CParamPool& IOBuff );

	std::vector<itemtype_t> m_vecExperiencedItem;
	unsigned short m_usedTime; // 今天已经使用的次数
	unsigned int m_nCountDown; // 倒计时
};


class GameMsg_S2C_ExperiencedItemNtf : public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_ExperiencedItemNtf);
	GameMsg_S2C_ExperiencedItemNtf(void);
	~GameMsg_S2C_ExperiencedItemNtf(void);
public:
	virtual bool doEncode( CParamPool& IOBuff );

	std::vector<itemtype_t> m_vecExperiencedItemNew; // 新添加的体验物品
	unsigned short m_usedTime; // 今天已经使用的次数
	unsigned int m_nCountDown; // 倒计时

};

#endif



