#ifndef	__LIBGAMEMSG_GM_SHARE_H__
#define __LIBGAMEMSG_GM_SHARE_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"
#include "GMDefine.h"
#include "../../datastructure/datainfor/GMDataInfo.h"

#include <list>
#include <string>


class CParamPool;

class GameMsg_M2G_SendGroupServerInfo:
	public GameMsg_Base
{
public:	
	GAMEMSG_CREATEDECLARE(GameMsg_M2G_SendGroupServerInfo);
	GameMsg_M2G_SendGroupServerInfo();	
	~GameMsg_M2G_SendGroupServerInfo();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
};

class GameMsg_G2M_SendGroupServerInfoRes:
	public GameMsg_Base
{
public:	
	GAMEMSG_CREATEDECLARE(GameMsg_G2M_SendGroupServerInfoRes);
	GameMsg_G2M_SendGroupServerInfoRes();	
	~GameMsg_G2M_SendGroupServerInfoRes();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
};


class GameMsg_M2S_GMSendMail:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_M2S_GMSendMail);
	GameMsg_M2S_GMSendMail();
	~GameMsg_M2S_GMSendMail();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	list<CRcvSysMailRoleInfo> m_listRcvRoleInfo;
	unsigned short m_nSlot;
};


class GameMsg_S2M_GMSendMailRes:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2M_GMSendMailRes);
	GameMsg_S2M_GMSendMailRes();
	~GameMsg_S2M_GMSendMailRes();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	list<CRcvSysMailRoleInfo> m_listRcvRoleInfo;
	unsigned short m_nSlot;
};

class GameMsg_M2S_QueryRangeGift:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_M2S_QueryRangeGift);
	GameMsg_M2S_QueryRangeGift();
	~GameMsg_M2S_QueryRangeGift();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool & IOBuff);
public:
	unsigned int m_nGiftID;
	std::string m_strStartTime;
	std::string m_strEndTime;
};

class GameMsg_S2M_QueryRangeGiftRes:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2M_QueryRangeGiftRes);
	GameMsg_S2M_QueryRangeGiftRes();		
	~GameMsg_S2M_QueryRangeGiftRes();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nActivatedNum;
};

class GameMsg_M2S_QueryMultiGift:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_M2S_QueryMultiGift);
	GameMsg_M2S_QueryMultiGift();
	~GameMsg_M2S_QueryMultiGift();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	std::list<std::string> m_listGiftCodes;

	unsigned short m_nSlot;
};

class GameMsg_S2M_QueryMultiGiftRes:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2M_QueryMultiGiftRes);
	GameMsg_S2M_QueryMultiGiftRes();
	~GameMsg_S2M_QueryMultiGiftRes();
public:
	bool doEncode(CParamPool & IOBuff);
	bool doDecode(CParamPool & IOBuff);
public:
	std::list<CQueryGiftState> m_listQueryCodes;
	unsigned short m_nSolt;
};

class GameMsg_M2S_Announce:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_M2S_Announce);
	GameMsg_M2S_Announce();
	~GameMsg_M2S_Announce();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	CAnnounceBase m_announce;
};

class GameMsg_M2G_LockOrUnLockByGm:
	public GameMsg_Base
{
public:	
	GAMEMSG_CREATEDECLARE(GameMsg_M2G_LockOrUnLockByGm);
	GameMsg_M2G_LockOrUnLockByGm();	
	~GameMsg_M2G_LockOrUnLockByGm();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned short m_nPid;
	std::string m_strPUID;
	std::string m_strTransID;
	bool m_bLock;//锁定还是解锁
	int m_nLockTimeType;//右边数组的下标，[10min   1hour   1day    7day    15day    30day    90day    永久]
};

class GameMsg_M2G_AddMCoinByGm:
	public GameMsg_Base
{
public:	
	GAMEMSG_CREATEDECLARE(GameMsg_M2G_AddMCoinByGm);
	GameMsg_M2G_AddMCoinByGm();	
	~GameMsg_M2G_AddMCoinByGm();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned short m_nPid;
	std::string m_strPUID;
	unsigned int m_nAmount;
	unsigned int m_nBindAmount;
	std::string m_strTransID;
	unsigned int m_nCurrency;
};

class GameMsg_M2G_UpdateCompensation:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_M2G_UpdateCompensation);
	GameMsg_M2G_UpdateCompensation();
	~GameMsg_M2G_UpdateCompensation();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	int m_nEndTime;
	int m_nMoney;
	std::string m_strTitle;
	std::string m_strContent;
	std::string m_strItemMale;
	std::string m_strItemFemale;
};

class GameMsg_G2M_UpdateCompensationRes : 
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2M_UpdateCompensationRes);
	GameMsg_G2M_UpdateCompensationRes();
	~GameMsg_G2M_UpdateCompensationRes();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	std::string m_strError;
};

class GameMsg_M2G_GetServerLine :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_M2G_GetServerLine);
	GameMsg_M2G_GetServerLine();
	~GameMsg_M2G_GetServerLine();

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned short m_nSlot;
};

class GameMsg_G2M_GetServerLineRes : 
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2M_GetServerLineRes);
	GameMsg_G2M_GetServerLineRes();
	~GameMsg_G2M_GetServerLineRes();

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	std::map<int,std::string> m_Lines;
	unsigned short m_nSlot;

};

class GameMsg_M2G_GetRoomList : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_M2G_GetRoomList);
	GameMsg_M2G_GetRoomList();
	~GameMsg_M2G_GetRoomList();

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	int m_nLine;
	unsigned short m_nSlot;
	unsigned short m_nGMAskRoomType;
};

class GameMsg_G2M_GetRoomListRes : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2M_GetRoomListRes);
	GameMsg_G2M_GetRoomListRes();
	~GameMsg_G2M_GetRoomListRes();

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	std::vector<GMRoomInfo> m_RoomList;
	unsigned short m_nSlot;
};

class GameMsg_M2G_SetMallUpdateTime : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_M2G_SetMallUpdateTime);
	GameMsg_M2G_SetMallUpdateTime();
	~GameMsg_M2G_SetMallUpdateTime();

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	unsigned int m_UpdateTime;
};

class GameMsg_M2G_GroupBuyControl : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_M2G_GroupBuyControl);
	GameMsg_M2G_GroupBuyControl();
	~GameMsg_M2G_GroupBuyControl();

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	int m_nLine;
	unsigned int m_nStartTime;
	unsigned int m_nEndTime;
	unsigned int m_nInterval;
	unsigned int m_nMinPeople;
	unsigned int m_nMaxPeople;
};

class GameMsg_M2G_SendPrivMCoin : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_M2G_SendPrivMCoin);
	GameMsg_M2G_SendPrivMCoin();
	~GameMsg_M2G_SendPrivMCoin();

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	std::vector<PlayerMoneyInfo> m_PlayerMoneyInfo;
	unsigned short m_nSlot;
};

class GameMsg_A2M_SendPrivMCoinRet : public GameMsg_Base
{
	GAMEMSG_CREATEDECLARE(GameMsg_A2M_SendPrivMCoinRet);
	GameMsg_A2M_SendPrivMCoinRet();
	~GameMsg_A2M_SendPrivMCoinRet();

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	std::vector<GMRechargeResult> m_GMRechargeResult;
	unsigned short m_nSlot;
};

class GameMsg_M2G_GetServerPlayerCountInfo : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_M2G_GetServerPlayerCountInfo);
	GameMsg_M2G_GetServerPlayerCountInfo();
	~GameMsg_M2G_GetServerPlayerCountInfo();

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	unsigned short m_nSolt;
};

class GameMsg_G2M_GetServerPlayerCountInfoRet : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2M_GetServerPlayerCountInfoRet);
	GameMsg_G2M_GetServerPlayerCountInfoRet();
	~GameMsg_G2M_GetServerPlayerCountInfoRet();

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	std::map<int,CServerPlayerCountInfo> m_ServerPlayerCountInfoMap;
	unsigned short m_nSolt;
};

class GameMsg_G2A_GetServerPlayerLimit : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2A_GetServerPlayerLimit);
	GameMsg_G2A_GetServerPlayerLimit();
	~GameMsg_G2A_GetServerPlayerLimit();

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	unsigned short m_nSolt;
};

class GameMsg_A2G_GetServerPlayerLimitRet : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_A2G_GetServerPlayerLimitRet);
	GameMsg_A2G_GetServerPlayerLimitRet();
	~GameMsg_A2G_GetServerPlayerLimitRet();

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	unsigned short m_nSolt;
	int m_PlayerLimit;
};

class GameMsg_M2A_SetPlayerLimit : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_M2A_SetPlayerLimit);
	GameMsg_M2A_SetPlayerLimit();
	~GameMsg_M2A_SetPlayerLimit();

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	int m_PlayerLimit;
};

class GameMsg_M2A_SendBlackDeviceList : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_M2A_SendBlackDeviceList);
	GameMsg_M2A_SendBlackDeviceList();
	~GameMsg_M2A_SendBlackDeviceList();

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	std::vector<std::string> m_NameList;
	std::string m_GM;
    bool m_bFrozenDevice;
	int m_nSlot;
};

class GameMsg_A2M_SendBlackDeviceListRet : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_A2M_SendBlackDeviceListRet);
	GameMsg_A2M_SendBlackDeviceListRet();
	~GameMsg_A2M_SendBlackDeviceListRet();

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	unsigned short m_Result;
	int m_nSlot;
};

class GameMsg_M2A_ChangeAccountPassword : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_M2A_ChangeAccountPassword);
	GameMsg_M2A_ChangeAccountPassword();
	~GameMsg_M2A_ChangeAccountPassword();
public:
	bool doDecode(CParamPool &IOBuff);
	bool doEncode(CParamPool &IOBuff);

public:
	std::string m_Account;
	std::string m_Password;
	int m_nSlot;
};

class GameMsg_A2M_ChangeAccountPasswordRet : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_A2M_ChangeAccountPasswordRet);
	GameMsg_A2M_ChangeAccountPasswordRet();
	~GameMsg_A2M_ChangeAccountPasswordRet();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned short m_Result;
	int m_nSlot;
};


//////////////////////////////////////////////////////////////////////////
// 信息中心
#define MAX_INFOCENTER_TITLE_LEN			256
#define MAX_INFOCENTER_CTX_LEN				1024
#define MAX_INFOCENTER_HREF_LEN				512
#define MAX_INFOCENTER_HIDPID_LEN			512
struct InfoCenterItem
{
	unsigned int			m_id;
	unsigned int			m_updateTime;
	unsigned char			m_group;
	std::string				m_icon;
	unsigned char			m_type; // 新闻，分享
	// 	char					m_title[MAX_INFOCENTER_TITLE_LEN];
	// 	char					m_contex[MAX_INFOCENTER_CTX_LEN];
	// 	char					m_href[MAX_INFOCENTER_HREF_LEN];
	// 	char					m_hidpid[MAX_INFOCENTER_HIDPID_LEN];
	std::string				m_strTitle;
	std::string				m_strContext;
	std::string				m_strHref;
	std::string				m_strHidPid;

	InfoCenterItem();

	bool doDecode(CParamPool &IOBuff);
	bool doEncode(CParamPool &IOBuff);

};
typedef std::map<unsigned int, InfoCenterItem*> InfoCenterPtrMap;

// 查询所有的信息
class GameMsg_M2G_GetInfoCenterRes : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_M2G_GetInfoCenterRes);
	GameMsg_M2G_GetInfoCenterRes();
	~GameMsg_M2G_GetInfoCenterRes();
public:
	bool doDecode(CParamPool &IOBuff);
	bool doEncode(CParamPool &IOBuff);

};

class GameMsg_G2M_GetInfoCenterRes : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2M_GetInfoCenterRes);
	GameMsg_G2M_GetInfoCenterRes();
	~GameMsg_G2M_GetInfoCenterRes();
public:
	bool doDecode(CParamPool &IOBuff);
	bool doEncode(CParamPool &IOBuff);
public:
	std::vector<InfoCenterItem> m_infos;
};

// 添加
class GameMsg_M2G_AddInfoCenterRes : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_M2G_AddInfoCenterRes);
	GameMsg_M2G_AddInfoCenterRes();
	~GameMsg_M2G_AddInfoCenterRes();
public:
	bool doDecode(CParamPool &IOBuff);
	bool doEncode(CParamPool &IOBuff);

	InfoCenterItem m_items; // ID是无效的
};

class GameMsg_G2M_AddInfoCenterRes : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2M_AddInfoCenterRes);
	GameMsg_G2M_AddInfoCenterRes();
	~GameMsg_G2M_AddInfoCenterRes();
public:
	bool doDecode(CParamPool &IOBuff);
	bool doEncode(CParamPool &IOBuff);
public:

	InfoCenterItem			m_items; 
};

// 更新
class GameMsg_M2G_UpdateInfoCenterRes : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_M2G_UpdateInfoCenterRes);
	GameMsg_M2G_UpdateInfoCenterRes();
	~GameMsg_M2G_UpdateInfoCenterRes();
public:
	bool doDecode(CParamPool &IOBuff);
	bool doEncode(CParamPool &IOBuff);

	InfoCenterItem m_items; // ID是无效的
};

class GameMsg_G2M_UpdateInfoCenterRes : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2M_UpdateInfoCenterRes);
	GameMsg_G2M_UpdateInfoCenterRes();
	~GameMsg_G2M_UpdateInfoCenterRes();
public:
	bool doDecode(CParamPool &IOBuff);
	bool doEncode(CParamPool &IOBuff);
public:
	InfoCenterItem			m_items; 
};


// 删除
class GameMsg_M2G_RemoveInfoCenterRes : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_M2G_RemoveInfoCenterRes);
	GameMsg_M2G_RemoveInfoCenterRes();
	~GameMsg_M2G_RemoveInfoCenterRes();
public:
	bool doDecode(CParamPool &IOBuff);
	bool doEncode(CParamPool &IOBuff);

	unsigned int		m_id; //
};

class GameMsg_G2M_RemoveInfoCenterRes : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2M_RemoveInfoCenterRes);
	GameMsg_G2M_RemoveInfoCenterRes();
	~GameMsg_G2M_RemoveInfoCenterRes();
public:
	bool doDecode(CParamPool &IOBuff);
	bool doEncode(CParamPool &IOBuff);
public:
	unsigned short		m_result;
	unsigned int		m_id; //
};



#endif


