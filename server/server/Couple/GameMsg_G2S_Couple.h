#ifndef		__GAMEMSG_G2S_COUPLE_H__
#define		__GAMEMSG_G2S_COUPLE_H__


#include <string>
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Couple.h"
#include "../../datastructure/DataStruct_Base.h"

class CParamPool;
//////////////////////////////////////////////////////////////////
class GameMsg_G2S_NotifyHasBeenSeparated :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_NotifyHasBeenSeparated);
	GameMsg_G2S_NotifyHasBeenSeparated();
	~GameMsg_G2S_NotifyHasBeenSeparated();
public:
	virtual bool doEncode(CParamPool &IOBuff);
	virtual bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nSeparatedBy;
	std::string m_strSeparatedBy;
};


//////////////////////////////////////////////////////////////////
class GameMsg_G2S_NotifyHasBeenDivorced :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_NotifyHasBeenDivorced);
	GameMsg_G2S_NotifyHasBeenDivorced();
	~GameMsg_G2S_NotifyHasBeenDivorced();
public:
	virtual bool doEncode(CParamPool &IOBuff);
	virtual bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nDivorcedBy;
	std::string m_strDivorcedBy;
};

//////////////////////////////////////////////////////////////////
class GameMsg_G2S_WeddingRoomStartingNofication :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_WeddingRoomStartingNofication);
	GameMsg_G2S_WeddingRoomStartingNofication();
	~GameMsg_G2S_WeddingRoomStartingNofication();
public:
	virtual bool doEncode(CParamPool &IOBuff);
	virtual bool doDecode(CParamPool &IOBuff);
public:
	int m_nLine;
	string m_strLineName;
	unsigned int m_nRoomID;
	unsigned int m_nStartTime;
};

//////////////////////////////////////////////////////////////////
class GameMsg_G2S_AutoStartWeddingFail :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_AutoStartWeddingFail);
	GameMsg_G2S_AutoStartWeddingFail();
	~GameMsg_G2S_AutoStartWeddingFail();
public:
	virtual bool doEncode(CParamPool &IOBuff);
	virtual bool doDecode(CParamPool &IOBuff);
public:
	unsigned char m_nError;
	std::string m_strError;
};

//////////////////////////////////////////////////////////////////
class GameMsg_G2S_UpdateCoupleIntimacy :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_UpdateCoupleIntimacy);
	GameMsg_G2S_UpdateCoupleIntimacy();
	~GameMsg_G2S_UpdateCoupleIntimacy();
public:
	virtual bool doEncode(CParamPool &IOBuff);
	virtual bool doDecode(CParamPool &IOBuff);
public:
	 int m_nChangedValue;
};

//////////////////////////////////////////////////////////////////
class GameMsg_G2S_CoupleNameChanged :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_CoupleNameChanged);
	GameMsg_G2S_CoupleNameChanged();
	~GameMsg_G2S_CoupleNameChanged();
public:
	virtual bool doEncode(CParamPool &IOBuff);
	virtual bool doDecode(CParamPool &IOBuff);
public:
	std::string m_strCoupleName;
};

///////////////////////////////////////////////////
class GameMsg_G2S_BlessingNotify :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_BlessingNotify);
	GameMsg_G2S_BlessingNotify();
	~GameMsg_G2S_BlessingNotify(){};

public:
	virtual bool doDecode( CParamPool& IOBuff );
	virtual bool doEncode( CParamPool& IOBuff );

public:
	std::string   m_strManName;
	std::string   m_strWomanName;
	unsigned int  m_nPointIndex;
};

///////////////////////////////////////////////////
class GameMsg_G2S_BlessingCouple :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_BlessingCouple);
	GameMsg_G2S_BlessingCouple();
	~GameMsg_G2S_BlessingCouple(){};

public:
	virtual bool doDecode( CParamPool& IOBuff );
	virtual bool doEncode( CParamPool& IOBuff );

public:
	unsigned int  m_nRoleID;
	unsigned int  m_nPairID;
	unsigned int m_nSex;
};

///////////////////////////////////////////////////
class GameMsg_G2S_GetWeddingRecordList :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_GetWeddingRecordList);
	GameMsg_G2S_GetWeddingRecordList();
	~GameMsg_G2S_GetWeddingRecordList(){};

public:
	virtual bool doDecode( CParamPool& IOBuff );
	virtual bool doEncode( CParamPool& IOBuff );

public:
	unsigned int                       m_nCount;
	std::vector<WeddingRecordInfo>   m_RecordList;
    
};

///////////////////////////////////////////////////
class GameMsg_G2S_GetBlessingRankList :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_GetBlessingRankList);
	GameMsg_G2S_GetBlessingRankList();
	~GameMsg_G2S_GetBlessingRankList(){};

public:
	virtual bool doDecode( CParamPool& IOBuff );
	virtual bool doEncode( CParamPool& IOBuff );

public:
    unsigned int                         m_nCurrentPage;
	unsigned int                         m_nRankTotalCount;
    unsigned short                       m_nPageCount;
	std::vector<CBlessingValueRankInfo>  m_BlessingValueRankVec;

};

///////////////////////////////////////////////////
class GameMsg_G2S_AddRedEnvelopeBlessingValue :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_AddRedEnvelopeBlessingValue);
	GameMsg_G2S_AddRedEnvelopeBlessingValue();
	~GameMsg_G2S_AddRedEnvelopeBlessingValue(){};

public:
	virtual bool doDecode( CParamPool& IOBuff );
	virtual bool doEncode( CParamPool& IOBuff );

public:
	unsigned int        m_nPairID;
	unsigned int        m_nRedEnvelopeCount;
	unsigned int        m_nRoleID;
	std::string         m_strRoleName;

};

///////////////////////////////////////////////////
class GameMsg_G2S_RedEnvelopeDelMoney :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_RedEnvelopeDelMoney);
	GameMsg_G2S_RedEnvelopeDelMoney();
	~GameMsg_G2S_RedEnvelopeDelMoney(){};

public:
	virtual bool doDecode( CParamPool& IOBuff );
	virtual bool doEncode( CParamPool& IOBuff );

public:
	unsigned int        m_nRedEnvelopeCount;
	unsigned int		m_nPairID;
};


///////////////////////////////////////////////////
class GameMsg_G2S_GetCoupleInfo :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_GetCoupleInfo);
	GameMsg_G2S_GetCoupleInfo();
	~GameMsg_G2S_GetCoupleInfo(){};

public:
	virtual bool doDecode( CParamPool& IOBuff );
	virtual bool doEncode( CParamPool& IOBuff );

public:
	unsigned int	m_nRank;
	COfflineCoupleInfo       m_CoupleInfo;
};


///////////////////////////////////////////////////
class GameMsg_G2S_GetBlessingValue :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_GetBlessingValue);
	GameMsg_G2S_GetBlessingValue();
	~GameMsg_G2S_GetBlessingValue(){};

public:
	virtual bool doDecode( CParamPool& IOBuff );
	virtual bool doEncode( CParamPool& IOBuff );

public:
	unsigned int            m_nBlessingValue;
    unsigned short          m_nLine;
};

///////////////////////////////////////////////////
class GameMsg_G2S_GetPersonInfo :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_GetPersonInfo);
	GameMsg_G2S_GetPersonInfo();
	~GameMsg_G2S_GetPersonInfo(){};

public:
	virtual bool doDecode( CParamPool& IOBuff );
	virtual bool doEncode( CParamPool& IOBuff );

public:
	RoleImageOfflineData           m_OffLineData;
};

///////////////////////////////////////////
class GameMsg_G2S_GetDivorceeAnimationPersonInfo :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_GetDivorceeAnimationPersonInfo);
	GameMsg_G2S_GetDivorceeAnimationPersonInfo();
	~GameMsg_G2S_GetDivorceeAnimationPersonInfo(){};

public:
	virtual bool doDecode( CParamPool& IOBuff );
	virtual bool doEncode( CParamPool& IOBuff );

public:
	RoleImageOfflineData           m_OffLineData;
};

///////////////////////////////////////////////////
class GameMsg_G2S_BlessingError :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_BlessingError);
	GameMsg_G2S_BlessingError();
	~GameMsg_G2S_BlessingError(){};

public:
	virtual bool doDecode( CParamPool& IOBuff );
	virtual bool doEncode( CParamPool& IOBuff );

public:
	std::string		m_strError;

};

///////////////////////////////////////////////////
class GameMsg_G2S_BlessingCoupleScuess :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_BlessingCoupleScuess);
	GameMsg_G2S_BlessingCoupleScuess();
	~GameMsg_G2S_BlessingCoupleScuess(){};

public:
	virtual bool doDecode( CParamPool& IOBuff );
	virtual bool doEncode( CParamPool& IOBuff );


};


///////////////////////////////////////////////////
class GameMsg_G2S_RedEnvelopeScuess :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_RedEnvelopeScuess);
	GameMsg_G2S_RedEnvelopeScuess();
	~GameMsg_G2S_RedEnvelopeScuess(){};

public:
	virtual bool doDecode( CParamPool& IOBuff );
	virtual bool doEncode( CParamPool& IOBuff );

};

///////////////////////////////////////////////////
class GameMsg_G2S_WeddingNotice :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_WeddingNotice);
	GameMsg_G2S_WeddingNotice();
	~GameMsg_G2S_WeddingNotice(){};

public:
	virtual bool doDecode( CParamPool& IOBuff );
	virtual bool doEncode( CParamPool& IOBuff );

public:
	std::string			m_strMaleRoleName;
	std::string			m_strFemaleRoleName;

};

///////////////////////////////////////////////////
class GameMsg_G2S_RefreshDivorceAction :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_RefreshDivorceAction);
	GameMsg_G2S_RefreshDivorceAction();
	~GameMsg_G2S_RefreshDivorceAction(){};

public:
	virtual bool doDecode( CParamPool& IOBuff );
	virtual bool doEncode( CParamPool& IOBuff );

public:
	CDivorceAgreementInfo m_DivorceAgreementInfo;
	CDivorceAction	m_Action;

};


#endif






