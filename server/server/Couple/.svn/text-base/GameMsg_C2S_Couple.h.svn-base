#ifndef __GAMEMSG_C2S_MARRIAGE_H__
#define __GAMEMSG_C2S_MARRIAGE_H__

#include <string>
#include "../../socket/GameMsg_Base.h"
#include "../mall/MallDefine.h"


///////////////////////////////////////////////////////
class GameMsg_C2S_SendCoupleRequest:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_SendCoupleRequest);
	GameMsg_C2S_SendCoupleRequest();
	~GameMsg_C2S_SendCoupleRequest();
public:
	virtual bool doDecode( CParamPool &IOBuff );
public:
	unsigned int m_nTargetID;
};
///////////////////////////////////////////////////////
class GameMsg_C2S_AcceptCoupleRequest:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_AcceptCoupleRequest);
	GameMsg_C2S_AcceptCoupleRequest();
	~GameMsg_C2S_AcceptCoupleRequest();
public:
	virtual bool doDecode( CParamPool &IOBuff );
public:
	unsigned int m_nTargetID;
};

///////////////////////////////////////////////////////
class GameMsg_C2S_RefuseCoupleRequest:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_RefuseCoupleRequest);
	GameMsg_C2S_RefuseCoupleRequest();
	~GameMsg_C2S_RefuseCoupleRequest();
public:
	virtual bool doDecode( CParamPool &IOBuff );
public:
	unsigned int m_nTargetID;
	std::string m_strReason;
};

///////////////////////////////////////////////////////
class GameMsg_C2S_CoupleSeparateRequest:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_CoupleSeparateRequest);
	GameMsg_C2S_CoupleSeparateRequest();
	~GameMsg_C2S_CoupleSeparateRequest();
public:
	virtual bool doDecode( CParamPool &IOBuff );
public:
	unsigned int m_nTargetID;
};

///////////////////////////////////////////////////////
class GameMsg_C2S_RequestMarry:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_RequestMarry);
	GameMsg_C2S_RequestMarry();
	~GameMsg_C2S_RequestMarry();
public:
	virtual bool doDecode(CParamPool & IOBuff);
};

///////////////////////////////////////////////////////
class GameMsg_C2S_RequestDivorce:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_RequestDivorce);
	GameMsg_C2S_RequestDivorce();
	~GameMsg_C2S_RequestDivorce();
public:
	virtual bool doDecode( CParamPool &IOBuff );
public:
	unsigned int m_nDivorceID;
};

///////////////////////////////////////////////////////
class GameMsg_C2S_CreateWeddingRoom:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_CreateWeddingRoom);
	GameMsg_C2S_CreateWeddingRoom();
	~GameMsg_C2S_CreateWeddingRoom();
public:
	virtual bool doDecode( CParamPool &IOBuff );
public:
	unsigned char m_nWeddingType;
	std::string m_strRoomName;
	std::string m_strPassword;
};
///////////////////////////////////////////////////////
class GameMsg_C2S_GetWeddingRoomList:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetWeddingRoomList);
	GameMsg_C2S_GetWeddingRoomList();
	~GameMsg_C2S_GetWeddingRoomList();
public:
	virtual bool doDecode( CParamPool &IOBuff );
public:
	unsigned short      m_nCurPage;
    unsigned short      m_nPageShowNum;
};

///////////////////////////////////////////////////////
class GameMsg_C2S_TryEnterWeddingRoom:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_TryEnterWeddingRoom);
	GameMsg_C2S_TryEnterWeddingRoom();
	~GameMsg_C2S_TryEnterWeddingRoom();
public:
	virtual bool doDecode( CParamPool &IOBuff );
public:
	unsigned int m_nRoomID;
};

///////////////////////////////////////////////////////
class GameMsg_C2S_EnterWeddingRoom:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_EnterWeddingRoom);
	GameMsg_C2S_EnterWeddingRoom();
	~GameMsg_C2S_EnterWeddingRoom();
public:
	virtual bool doDecode(CParamPool & IOBuff);
public:
	unsigned int m_nRoomID;
	std::string m_strRoomPassword;
};

///////////////////////////////////////////////////////
class GameMsg_C2S_EnterMyWeddingRoom:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_EnterMyWeddingRoom);
	GameMsg_C2S_EnterMyWeddingRoom();
	~GameMsg_C2S_EnterMyWeddingRoom();
public:
	virtual bool doDecode( CParamPool &IOBuff );
};

///////////////////////////////////////////////////////
class GameMsg_C2S_ExitWeddingRoom:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ExitWeddingRoom);
	GameMsg_C2S_ExitWeddingRoom();
	~GameMsg_C2S_ExitWeddingRoom();
public:
	virtual bool doDecode(CParamPool & IOBuff);

public:
    char        m_nQuitTo;
};

///////////////////////////////////////////////////////
class GameMsg_C2S_KickWeddingRoomPlayer:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_KickWeddingRoomPlayer);
	GameMsg_C2S_KickWeddingRoomPlayer();
	~GameMsg_C2S_KickWeddingRoomPlayer();
public:
	virtual bool doDecode(CParamPool & IOBuff);
public:
	unsigned int m_nTargetID;
};
///////////////////////////////////////////////////////
class GameMsg_C2S_SetWeddingPlayerType:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_SetWeddingPlayerType);
	GameMsg_C2S_SetWeddingPlayerType();
	~GameMsg_C2S_SetWeddingPlayerType();
public:
	virtual bool doDecode(CParamPool & IOBuff);
public:
	unsigned int m_nTargetID;
	unsigned char m_nRoomRoleType;
};
///////////////////////////////////////////////////////
class GameMsg_C2S_ChangeWeddingRoomPwd:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ChangeWeddingRoomPwd);
	GameMsg_C2S_ChangeWeddingRoomPwd();
	~GameMsg_C2S_ChangeWeddingRoomPwd();
public:
	virtual bool doDecode(CParamPool & IOBuff);
public:
	std::string m_strNewPwd;
};

///////////////////////////////////////////////////////
class GameMsg_C2S_GetBlessingRankList:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetBlessingRankList);
	GameMsg_C2S_GetBlessingRankList();
	~GameMsg_C2S_GetBlessingRankList();

 public:
 	virtual bool doDecode(CParamPool & IOBuff);

public:
	unsigned short	m_nCurrentPage;
    unsigned short  m_nPageCount;
};

///////////////////////////////////////////////////////
class GameMsg_C2S_BlessingCouple:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_BlessingCouple);
	GameMsg_C2S_BlessingCouple();
	~GameMsg_C2S_BlessingCouple(){};

public:
	virtual bool doDecode(CParamPool & IOBuff);

public:
	unsigned int    m_nPairID;

};

///////////////////////////////////////////////////////
class GameMsg_C2S_GetWeddingRecordList :
      public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetWeddingRecordList);
	GameMsg_C2S_GetWeddingRecordList();
	~GameMsg_C2S_GetWeddingRecordList(){};

public:
	virtual bool doDecode(CParamPool & IOBuff);

};

///////////////////////////////////////////////////////
class GameMsg_C2S_MoneyBlessingPair:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_MoneyBlessingPair);
	GameMsg_C2S_MoneyBlessingPair();
	~GameMsg_C2S_MoneyBlessingPair(){};

public:
	virtual bool doDecode(CParamPool & IOBuff);

public:
	unsigned int    m_nPairID;
	unsigned int    m_nRedEnvelopeCount;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_GetCoupleInfo:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetCoupleInfo);
	GameMsg_C2S_GetCoupleInfo();
	~GameMsg_C2S_GetCoupleInfo(){};

public:
	virtual bool doDecode(CParamPool & IOBuff);

public:
	unsigned int     m_nPairID;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_GetBlessingValue:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetBlessingValue);
	GameMsg_C2S_GetBlessingValue();
	~GameMsg_C2S_GetBlessingValue(){};

public:
	virtual bool doDecode(CParamPool & IOBuff);

public:
	unsigned int     m_nPairID;

};

///////////////////////////////////////////////////////
class GameMsg_C2S_GetPersonInfo:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetPersonInfo);
	GameMsg_C2S_GetPersonInfo();
	~GameMsg_C2S_GetPersonInfo(){};

public:
	virtual bool doDecode(CParamPool & IOBuff);

public:
	unsigned int     m_nRoleID;

};

///////////////////////////////////////////////////////
class GameMsg_C2S_GetCoupleRedEnvelopeGiftInfo:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetCoupleRedEnvelopeGiftInfo);
	GameMsg_C2S_GetCoupleRedEnvelopeGiftInfo();
	~GameMsg_C2S_GetCoupleRedEnvelopeGiftInfo(){};

public:
	virtual bool doDecode(CParamPool & IOBuff);

public:
	unsigned int   m_nPairID;

};

///////////////////////////////////////////////////////
class GameMsg_C2S_CancelDivorce:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_CancelDivorce);
	GameMsg_C2S_CancelDivorce();
	~GameMsg_C2S_CancelDivorce(){};

public:
	virtual bool doDecode(CParamPool & IOBuff);

};

///////////////////////////////////////////////////////
class GameMsg_C2S_CompulsoryDivorce:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_CompulsoryDivorce);
	GameMsg_C2S_CompulsoryDivorce();
	~GameMsg_C2S_CompulsoryDivorce(){};

public:
	virtual bool doDecode(CParamPool & IOBuff);

};

///////////////////////////////////////////////////////
class GameMsg_C2S_AgreeDivorce:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_AgreeDivorce);
	GameMsg_C2S_AgreeDivorce();
	~GameMsg_C2S_AgreeDivorce(){};

public:
	virtual bool doDecode(CParamPool & IOBuff);
};

///////////////////////////////////////////////////////
class GameMsg_C2S_DivorceChat:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_DivorceChat);
	GameMsg_C2S_DivorceChat();
	~GameMsg_C2S_DivorceChat(){};

public:
	virtual bool doDecode(CParamPool & IOBuff);

public:
	std::string		m_strChatContent;

};

///////////////////////////////////////////////////////
class GameMsg_C2S_DivorceGift:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_DivorceGift);
	GameMsg_C2S_DivorceGift();
	~GameMsg_C2S_DivorceGift(){};

public:
	virtual bool doDecode(CParamPool & IOBuff);

public:
	unsigned int m_nDestRoleID;
	VecPeddleGoods vecGoods;
};

///////////////////////////////////////////////////////
class GameMsg_C2S_DivorceWish:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_DivorceWish);
	GameMsg_C2S_DivorceWish();
	~GameMsg_C2S_DivorceWish(){};

public:
	virtual bool doDecode(CParamPool & IOBuff);

public:
	unsigned int m_nDestRoleID;
};

///////////////////////////////////////////////////////
class GameMsg_C2S_DivorceAnimation:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_DivorceAnimation);
	GameMsg_C2S_DivorceAnimation();
	~GameMsg_C2S_DivorceAnimation(){};

public:
	virtual bool doDecode(CParamPool & IOBuff);

};


#endif




