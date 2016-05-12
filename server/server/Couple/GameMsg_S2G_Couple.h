#ifndef		__GAMEMSG_S2G_MARRIAGE_H__
#define		__GAMEMSG_S2G_MARRIAGE_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Couple.h"
#include "../../datastructure/DataStruct_Base.h"


class CParamPool;

////////////////////////////////////////////////////////////////
class GameMsg_S2G_AcceptCoupleRequest : 
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_AcceptCoupleRequest);
	GameMsg_S2G_AcceptCoupleRequest();
	~GameMsg_S2G_AcceptCoupleRequest();
public:
	virtual bool doEncode(CParamPool &IOBuff);
	virtual bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nPairID;
	unsigned int m_nMaleID;
	unsigned int m_nFemaleID;
	unsigned int m_nCoupleDate;
    unsigned int m_nSmallRoleID;
};
////////////////////////////////////////////////////////////////
class GameMsg_S2G_CoupleSeprate: 
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_CoupleSeprate);
	GameMsg_S2G_CoupleSeprate();
	~GameMsg_S2G_CoupleSeprate();
public:
	virtual bool doEncode(CParamPool &IOBuff);
	virtual bool doDecode(CParamPool &IOBuff);
public:
    unsigned int m_nPairID;
	unsigned int m_nSeparateID;
	unsigned int m_nToBeSeparateID;
	bool m_bNeedNotify;
};
////////////////////////////////////////////////////////////////
class GameMsg_S2G_GetMarried: 
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_GetMarried);
	GameMsg_S2G_GetMarried();
	~GameMsg_S2G_GetMarried();

public:
	virtual bool doEncode(CParamPool &IOBuff);
	virtual bool doDecode(CParamPool &IOBuff);

public:
	unsigned int m_nPairID;
	unsigned int m_nMaleRoleID;
	std::string m_strMaleRoleName;
	unsigned int m_nFemaleRoleID;
	std::string m_strFemaleRoleName;
	unsigned int m_nMarriageDate;
	unsigned int m_nBlessingValue;
};

////////////////////////////////////////////////////////////////
class GameMsg_S2G_Divorced: 
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_Divorced);
	GameMsg_S2G_Divorced();
	~GameMsg_S2G_Divorced();
public:
	virtual bool doEncode(CParamPool &IOBuff);
	virtual bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nPairID;
	unsigned int m_nDivorceID;
	unsigned int m_nBeDivorcedID;
	bool m_bNeedNotify;
};
////////////////////////////////////////////////////////////////
class GameMsg_S2G_WeddingRoomStartingNofication: 
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_WeddingRoomStartingNofication);
	GameMsg_S2G_WeddingRoomStartingNofication();
	~GameMsg_S2G_WeddingRoomStartingNofication();
public:
	virtual bool doEncode(CParamPool &IOBuff);
	virtual bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nGroomID;
	unsigned int m_nBrideID;
	int m_nLine;
	string m_strLineName;
	unsigned int m_nRoomID;
	unsigned int m_nStartTime;
};
////////////////////////////////////////////////////////////////
class GameMsg_S2G_AutoStartWeddingFail: 
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_AutoStartWeddingFail);
	GameMsg_S2G_AutoStartWeddingFail();
	~GameMsg_S2G_AutoStartWeddingFail();
public:
	virtual bool doEncode(CParamPool &IOBuff);
	virtual bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nGroomID;
	unsigned int m_nBrideID;
	unsigned char m_nError;
	std::string m_strError;
};

////////////////////////////////////////////////////////////////
class GameMsg_S2G_UpdateCoupleIntimacy: 
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_UpdateCoupleIntimacy);
	GameMsg_S2G_UpdateCoupleIntimacy();
	~GameMsg_S2G_UpdateCoupleIntimacy();
public:
	virtual bool doEncode(CParamPool &IOBuff);
	virtual bool doDecode(CParamPool &IOBuff);
public:
    unsigned int m_nPairID;
	unsigned int m_nTriggerID;
	unsigned int m_nCoupleID;
	int m_nChangedValue;
    bool         m_bMarried;
    bool         m_bMale;
    unsigned int m_nSmallRoleID;
};


/////////////////////////////////////////////////////
class GameMsg_S2G_BlessingNotify :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_BlessingNotify);
	GameMsg_S2G_BlessingNotify();
	~GameMsg_S2G_BlessingNotify(){};
public:
	virtual bool doDecode( CParamPool& IOBuff );
	virtual bool doEncode( CParamPool& IOBuff );

public:
	unsigned int	m_nMaleRoleID;
	std::string   m_strManName;
	unsigned int	m_nFemaleRoleID;
	std::string   m_strWomanName;
	unsigned int  m_nPointIndex;

};


/////////////////////////////////////////////////////
class GameMsg_S2G_UpdateWeddingRecord :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_UpdateWeddingRecord);
	GameMsg_S2G_UpdateWeddingRecord();
	~GameMsg_S2G_UpdateWeddingRecord(){};

	enum EBlessingType
	{
		EBlessingType_BlessingCouple = 1,
		EBlessingType_RedEnvelope,
	};

public:
	virtual bool doDecode( CParamPool& IOBuff );
	virtual bool doEncode( CParamPool& IOBuff );

public:
	unsigned int		m_nBlessingType;
	unsigned int		m_nPairID;
	unsigned int		m_nBlessingValue;
	unsigned int        m_nBlessingRoleID;
};


/////////////////////////////////////////////////////
class GameMsg_S2G_BlessingCouple :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_BlessingCouple);
	GameMsg_S2G_BlessingCouple();
	~GameMsg_S2G_BlessingCouple(){};
public:
	virtual bool doDecode( CParamPool& IOBuff );
	virtual bool doEncode( CParamPool& IOBuff );

public:
	unsigned int  m_nRoleID;            // ×£¸£½ÇÉ«ID
	unsigned int  m_nSex;
	unsigned int  m_nPairID;


};

/////////////////////////////////////////////////////
class GameMsg_S2G_GetWeddingRecordList :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_GetWeddingRecordList);
	GameMsg_S2G_GetWeddingRecordList();
	~GameMsg_S2G_GetWeddingRecordList(){};

public:
	virtual bool doDecode( CParamPool& IOBuff );
	virtual bool doEncode( CParamPool& IOBuff );

public:
    unsigned int    m_nRoleID;

};

/////////////////////////////////////////////////////
class GameMsg_S2G_AddWeddingRecord :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_AddWeddingRecord);
	GameMsg_S2G_AddWeddingRecord();
	~GameMsg_S2G_AddWeddingRecord(){};

public:
	virtual bool doEncode( CParamPool& IOBuff );
	virtual bool doDecode( CParamPool& IOBuff );

public:
	WeddingRecordInfo      m_Record;
};

/////////////////////////////////////////////////////
class GameMsg_S2G_UpdateWeddingRecordState:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_UpdateWeddingRecordState);
	GameMsg_S2G_UpdateWeddingRecordState();
	~GameMsg_S2G_UpdateWeddingRecordState(){};

public:
	virtual bool doEncode( CParamPool& IOBuff );
	virtual bool doDecode( CParamPool& IOBuff );

public:
	unsigned int      m_nDoType;
	unsigned int      m_nPairID;
	bool      m_bIsRunning;
};

/////////////////////////////////////////////////////
class GameMsg_S2G_GetBlessingRankList:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_GetBlessingRankList);
	GameMsg_S2G_GetBlessingRankList();
	~GameMsg_S2G_GetBlessingRankList(){};

public:
	virtual bool doEncode( CParamPool& IOBuff );
	virtual bool doDecode( CParamPool& IOBuff );

public:
	unsigned short	  m_nCurrentPage;
	unsigned int      m_nRoleID;
	unsigned int      m_nPairID;
    unsigned short    m_nPageCount;
};

/////////////////////////////////////////////////////
class GameMsg_S2G_RedEnvelope:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_RedEnvelope);
	GameMsg_S2G_RedEnvelope();
	~GameMsg_S2G_RedEnvelope(){};

public:
	virtual bool doEncode( CParamPool& IOBuff );
	virtual bool doDecode( CParamPool& IOBuff );

public:
	unsigned int      m_nRoleID;
	unsigned int      m_nPairID;
	unsigned int      m_nRedEnvelopeCount;
};

/////////////////////////////////////////////////////
class GameMsg_S2G_GetCoupleInfo:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_GetCoupleInfo);
	GameMsg_S2G_GetCoupleInfo();
	~GameMsg_S2G_GetCoupleInfo(){};

public:
	virtual bool doEncode( CParamPool& IOBuff );
	virtual bool doDecode( CParamPool& IOBuff );

public:
	unsigned int      m_nRoleID;
	unsigned int      m_nPairID;
};


/////////////////////////////////////////////////////
class GameMsg_S2G_GetBlessingValue:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_GetBlessingValue);
	GameMsg_S2G_GetBlessingValue();
	~GameMsg_S2G_GetBlessingValue(){};

public:
	virtual bool doEncode( CParamPool& IOBuff );
	virtual bool doDecode( CParamPool& IOBuff );

public:
	unsigned int      m_nRoleID;
	unsigned int      m_nPairID;
};

/////////////////////////////////////////////////////
class GameMsg_S2G_GetPersonInfo:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_GetPersonInfo);
	GameMsg_S2G_GetPersonInfo();
	~GameMsg_S2G_GetPersonInfo(){};

public:
	virtual bool doEncode( CParamPool& IOBuff );
	virtual bool doDecode( CParamPool& IOBuff );

public:
	unsigned int      m_nSelfRoleID;
	unsigned int      m_nRoleID;
};

/////////////////////////////////////////////////////
class GameMsg_S2G_GetDivorceAnimationPersonInfo:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_GetDivorceAnimationPersonInfo);
	GameMsg_S2G_GetDivorceAnimationPersonInfo();
	~GameMsg_S2G_GetDivorceAnimationPersonInfo(){};

public:
	virtual bool doEncode( CParamPool& IOBuff );
	virtual bool doDecode( CParamPool& IOBuff );

public:
	unsigned int      m_nSelfRoleID;
	unsigned int      m_nCoupleRoleID;
};

/////////////////////////////////////////////////////
class GameMsg_S2G_WeddingNotice:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_WeddingNotice);
	GameMsg_S2G_WeddingNotice();
	~GameMsg_S2G_WeddingNotice(){};

public:
	virtual bool doEncode( CParamPool& IOBuff );
	virtual bool doDecode( CParamPool& IOBuff );

public:
	unsigned int	m_nRoleID;
	std::string		m_strMaleRoleName;
	std::string		m_strFeMaleRoleName;
};

/////////////////////////////////////////////////////
class GameMsg_S2G_AddRedEnvelopeBlessingValue:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_AddRedEnvelopeBlessingValue);
	GameMsg_S2G_AddRedEnvelopeBlessingValue();
	~GameMsg_S2G_AddRedEnvelopeBlessingValue(){};

public:
	virtual bool doEncode( CParamPool& IOBuff );
	virtual bool doDecode( CParamPool& IOBuff );

public:
	unsigned int	m_nBlessingRoleID;
	std::string		m_strBlessingRoleName;
	unsigned int	m_nPairID;
	unsigned int	m_nRedEnvelopeCount;
};

/////////////////////////////////////////////////////
class GameMsg_S2G_RefreshDivorceAction:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_RefreshDivorceAction);
	GameMsg_S2G_RefreshDivorceAction();
	~GameMsg_S2G_RefreshDivorceAction(){};

public:
	virtual bool doEncode( CParamPool& IOBuff );
	virtual bool doDecode( CParamPool& IOBuff );

public:
	unsigned int	m_nRoleID;
	CDivorceAction m_action;
	CDivorceAgreementInfo m_DivorceAgreementInfo;

};

#endif


