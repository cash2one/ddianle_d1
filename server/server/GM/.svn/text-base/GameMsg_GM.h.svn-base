#ifndef		__GAMEMSG_GM_H__
#define		__GAMEMSG_GM_H__

#include "../../socket/GameMsg_Base.h"
#include <list>
#include <string>
#include "../../datastructure/DataStruct_Base.h"
#include "GMDefine.h"
#include "GMDataStruct.h"
#include "../../datastructure/datainfor/GMDataInfo.h"


class GameMsg_C2S_GMLogin :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GMLogin);
    GameMsg_C2S_GMLogin();
    ~GameMsg_C2S_GMLogin();
public:
    bool doDecode(CParamPool &IOBuff);
public:
    char m_szName[GM_MAX_NAME_LEN];
    char m_szPasswd[GM_MAX_PASSWD_LEN];
};

class GameMsg_S2C_GMLoginSuc :
    public GameMsg_Base
{
public:
    GameMsg_S2C_GMLoginSuc();
    ~GameMsg_S2C_GMLoginSuc();
public:
    bool doEncode(CParamPool &IOBuff);
public:
    char m_szName[GM_MAX_NAME_LEN];
    int  m_Priv;
};

class GameMsg_S2C_GMLoginFail :
    public GameMsg_Base
{
public:
    GameMsg_S2C_GMLoginFail();
    ~GameMsg_S2C_GMLoginFail();
public:
    bool doEncode(CParamPool & IOBuff);
public:
    int m_nError;
};

class GameMsg_C2S_GMSendMail :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GMSendMail);
    GameMsg_C2S_GMSendMail();
    ~GameMsg_C2S_GMSendMail();
public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);
    std::string ToJson();
public:
    list<CRcvSysMailRoleInfo> m_listRcvRoleInfo;
};


class GameMsg_S2C_GMSendMailRes :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GMSendMailRes);
    GameMsg_S2C_GMSendMailRes();
    ~GameMsg_S2C_GMSendMailRes();
public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);
public:
    list<CRcvSysMailRoleInfo> m_listRcvRoleInfo;
};

class GameMsg_C2S_QueryRangeGift :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_QueryRangeGift);
    GameMsg_C2S_QueryRangeGift();
    ~GameMsg_C2S_QueryRangeGift();
public:
    bool doDecode(CParamPool & IOBuff);
    std::string ToJson();
public:
    unsigned int m_nGiftID;
    std::string m_strStartTime;
    std::string m_strEndTime;
};

class GameMsg_S2C_QueryRangeGiftRes :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_QueryRangeGiftRes);
    GameMsg_S2C_QueryRangeGiftRes();
    ~GameMsg_S2C_QueryRangeGiftRes();
public:
    bool doEncode(CParamPool &IOBuff);
public:
    unsigned int m_nActivatedNum;
};

class GameMsg_C2S_QueryMultiGift :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_QueryMultiGift);
    GameMsg_C2S_QueryMultiGift();
    ~GameMsg_C2S_QueryMultiGift();
public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);
    std::string ToJson();
public:
    std::list<std::string> m_listGiftCodes;
};

class GameMsg_S2C_QueryMultiGiftRes :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_QueryMultiGiftRes);
    GameMsg_S2C_QueryMultiGiftRes();
    ~GameMsg_S2C_QueryMultiGiftRes();
public:
    bool doEncode(CParamPool & IOBuff);
    bool doDecode(CParamPool & IOBuff);
public:
    list<CQueryGiftState> m_listQueryCodes;
};

class GameMsg_S2C_KickPlayer :
    public GameMsg_Base
{
public:
    GameMsg_S2C_KickPlayer();
    ~GameMsg_S2C_KickPlayer();
public:
    bool doEncode(CParamPool & IOBuff);
public:
    unsigned short m_nReason;
};

class GameMsg_C2S_GrantGMCmd :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GrantGMCmd);
    GameMsg_C2S_GrantGMCmd();
    ~GameMsg_C2S_GrantGMCmd();
public:
    bool doDecode(CParamPool & IOBuff);
public:
    char			m_szName[GM_MAX_NAME_LEN];
    std::list<std::string>	m_listGrantCmds;
};

class GameMsg_S2C_GrantCmdSuc :
    public GameMsg_Base
{
public:
    GameMsg_S2C_GrantCmdSuc();
    ~GameMsg_S2C_GrantCmdSuc();
public:
    bool doEncode(CParamPool &IOBuff);
public:
    char			m_szName[GM_MAX_NAME_LEN];
    std::list<std::string>	m_listFailCmd;
};

class GameMsg_S2C_GrantGMCmdFail :
    public GameMsg_Base
{
public:
    GameMsg_S2C_GrantGMCmdFail();
    ~GameMsg_S2C_GrantGMCmdFail();
public:
    bool doEncode(CParamPool & IOBuff);
public:
    char			m_szName[GM_MAX_NAME_LEN];
    unsigned short	m_nErrno;
};

class GameMsg_C2S_RemoveGMCmd :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_RemoveGMCmd);
    GameMsg_C2S_RemoveGMCmd();
    ~GameMsg_C2S_RemoveGMCmd();
public:
    bool doDecode(CParamPool & IOBuff);
public:
    char			m_szName[GM_MAX_NAME_LEN];
    std::list<std::string>	m_listRemoveCmds;
};

class GameMsg_S2C_RemoveGMCmdSuc :
    public GameMsg_Base
{
public:
    GameMsg_S2C_RemoveGMCmdSuc();
    ~GameMsg_S2C_RemoveGMCmdSuc();
public:
    bool doEncode(CParamPool &IOBuff);
public:
    char			m_szName[GM_MAX_NAME_LEN];
    std::list<std::string>	m_listFailCmd;
};

class GameMsg_S2C_RemoveGMCmdFail :
    public GameMsg_Base
{
public:
    GameMsg_S2C_RemoveGMCmdFail();
    ~GameMsg_S2C_RemoveGMCmdFail();
public:
    bool doEncode(CParamPool &IOBuff);
public:
    char			m_szName[GM_MAX_NAME_LEN];
    unsigned short	m_nErrno;
};

class GameMsg_C2S_GMSendAnnounceOnce :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GMSendAnnounceOnce);
    GameMsg_C2S_GMSendAnnounceOnce();
    ~GameMsg_C2S_GMSendAnnounceOnce();
public:
    bool doDecode(CParamPool & IOBuff);
    std::string ToJson();
public:
    unsigned short m_nPID;
    std::string m_strContent;
    bool m_bIsAdminChat;
    bool m_bIsSysChat;
    int m_nDelayTime;
};

class GameMsg_C2S_GMSendAnnounceInterval :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GMSendAnnounceInterval);
    GameMsg_C2S_GMSendAnnounceInterval();
    ~GameMsg_C2S_GMSendAnnounceInterval();
public:
    bool doDecode(CParamPool &IOBuff);
    std::string ToJson();
public:
    CAnnounceInfo m_annouce;
};

class GameMsg_S2C_GMSendAnnounceSuc :
    public GameMsg_Base
{
public:
    GameMsg_S2C_GMSendAnnounceSuc();
    ~GameMsg_S2C_GMSendAnnounceSuc();
public:
    bool doEncode(CParamPool &IOBuff);
public:
    unsigned short m_nPID;
    std::string m_strContent;
    bool m_bIsAdminChat;
    bool m_bIsSysChat;
};

class GameMsg_S2C_GMSendAnnounceFail :
    public GameMsg_Base
{
public:
    GameMsg_S2C_GMSendAnnounceFail();
    ~GameMsg_S2C_GMSendAnnounceFail();
public:
    bool doEncode(CParamPool &IOBuff);
public:
    unsigned short m_nPID;
    std::string m_strContent;
    bool m_bIsAdminChat;
    bool m_bIsSysChat;
};
class GameMsg_C2S_GMLockOrUnLockAccount :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GMLockOrUnLockAccount);
    GameMsg_C2S_GMLockOrUnLockAccount();
    ~GameMsg_C2S_GMLockOrUnLockAccount();
public:
    bool doDecode(CParamPool &IOBuff);
    std::string ToJson();
public:
    unsigned short m_nPid;
    std::string m_strPUID;
    bool m_bLock;//锁定还是解锁
    int m_nLockTimeType;//右边数组的下标，[10min   1hour   1day    7day    15day    30day    90day    永久]
    unsigned short m_nLockReason;
};

class GameMsg_C2S_GMAddMCoin :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GMAddMCoin);
    GameMsg_C2S_GMAddMCoin();
    ~GameMsg_C2S_GMAddMCoin();
public:
    bool doDecode(CParamPool &IOBuff);
    std::string ToJson();
public:
    unsigned short m_nPid;
    std::string m_strPUID;
    unsigned int m_nAmount;
    unsigned int m_nBindAmount;
    unsigned int m_nCurrency;
};

class GameMsg_C2S_SendCompensation :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_SendCompensation);
    GameMsg_C2S_SendCompensation();
    ~GameMsg_C2S_SendCompensation();
public:
    bool doDecode(CParamPool &IOBuff);
    std::string ToJson();
public:
    std::string m_strEndTime;
    int m_nMoney;
    std::string m_strTitle;
    std::string m_strContent;
    std::string m_strItemMale;
    std::string m_strItemFemale;

};

class GameMsg_S2C_SendCompensationRes :
    public GameMsg_Base
{
public:
    GameMsg_S2C_SendCompensationRes();
    ~GameMsg_S2C_SendCompensationRes();
public:
    bool doEncode(CParamPool &IOBuff);

public:
    std::string m_strError;
};

class GameMsg_S2C_PrivError : public GameMsg_Base
{
public:
    GameMsg_S2C_PrivError();
    ~GameMsg_S2C_PrivError();
public:
    bool doEncode(CParamPool &IOBuff);
public:
    int m_nErrorCode;
};

class GameMsg_C2S_GetUsers : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetUsers);
    GameMsg_C2S_GetUsers();
    ~GameMsg_C2S_GetUsers();
public:
    bool doDecode(CParamPool &IOBuff);
};

class GameMsg_S2C_GetUserRes : public GameMsg_Base
{
public:
    GameMsg_S2C_GetUserRes();
    ~GameMsg_S2C_GetUserRes();

public:
    std::map<std::string, int> m_UserPrivList;

    void AddUser(std::string& user, int priv);

public:
    bool doEncode(CParamPool &IOBuff);
};

class GameMsg_C2S_SetPriv : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_SetPriv);
    GameMsg_C2S_SetPriv();
    ~GameMsg_C2S_SetPriv();

public:
    std::string m_UserName;
    int m_Priv;

public:
    bool doDecode(CParamPool &IOBuff);
    std::string ToJson();
};

class GameMsg_S2C_SetPrivRes : public GameMsg_Base
{
public:
    GameMsg_S2C_SetPrivRes();
    ~GameMsg_S2C_SetPrivRes();
public:
    int m_nRes;
public:
    bool doEncode(CParamPool &IOBuff);
};

class GameMsg_C2S_AddUser : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_AddUser);
    GameMsg_C2S_AddUser();
    ~GameMsg_C2S_AddUser();
public:
    std::string m_UserName;
    std::string m_Password;
public:
    bool doDecode(CParamPool &IOBuff);
    std::string ToJson();
};

class GameMsg_S2C_AddUserRes : public GameMsg_Base
{
public:
    GameMsg_S2C_AddUserRes();
    ~GameMsg_S2C_AddUserRes();
public:
    int m_nRes;
    std::string m_UserName;
    int m_Priv;
public:
    bool doEncode(CParamPool &IOBuff);
};

class GameMsg_C2S_DelUser : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_DelUser);
    GameMsg_C2S_DelUser();
    ~GameMsg_C2S_DelUser();
public:
    std::string m_UserName;
public:
    bool doDecode(CParamPool &IOBuff);
    std::string ToJson();
};

class GameMsg_S2C_DelUserRes : public GameMsg_Base
{
public:
    GameMsg_S2C_DelUserRes();
    ~GameMsg_S2C_DelUserRes();

public:
    int m_nRes;
public:
    bool doEncode(CParamPool &IOBuff);
};

class GameMsg_C2S_GMGetServerLine : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GMGetServerLine);
    GameMsg_C2S_GMGetServerLine();
    ~GameMsg_C2S_GMGetServerLine();

public:
    bool doDecode(CParamPool &IOBuff);
    std::string ToJson();
};

class GameMsg_S2C_GMGetServerLineRes : public GameMsg_Base
{
public:
    GameMsg_S2C_GMGetServerLineRes();
    ~GameMsg_S2C_GMGetServerLineRes();
public:
    bool doEncode(CParamPool &IOBuff);

public:
    std::map<int, std::string> m_Lines;
};

class GameMsg_C2S_GMGetServerRoomList : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GMGetServerRoomList);
    GameMsg_C2S_GMGetServerRoomList();
    ~GameMsg_C2S_GMGetServerRoomList();

public:
    bool doDecode(CParamPool &IOBuff);
    std::string ToJson();
public:
    int m_nLine;
    unsigned short m_GMAskRoomType;
};

class GameMsg_S2C_GMGetServerRoomListRes : public GameMsg_Base
{
public:
    GameMsg_S2C_GMGetServerRoomListRes();
    ~GameMsg_S2C_GMGetServerRoomListRes();

public:
    bool doEncode(CParamPool &IOBuff);

public:
    std::vector<GMRoomInfo> m_RoomList;
};

class GameMsg_C2S_GMSetMallUpdateTime : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GMSetMallUpdateTime);
    GameMsg_C2S_GMSetMallUpdateTime();
    ~GameMsg_C2S_GMSetMallUpdateTime();
public:
    bool doDecode(CParamPool &IOBuff);
    std::string ToJson();
public:
    unsigned int m_UpdateTime;
};

class GameMsg_C2S_GMGroupBuyControl : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GMGroupBuyControl);
    GameMsg_C2S_GMGroupBuyControl();
    ~GameMsg_C2S_GMGroupBuyControl();

public:
    bool doDecode(CParamPool &IOBuff);
    std::string ToJson();

public:
    int m_nLine;
    unsigned int m_nStartTime;
    unsigned int m_nEndTime;
    unsigned int m_nInterval;
    unsigned int m_nMinPeople;
    unsigned int m_nMaxPeople;
};

class GameMsg_S2C_SetMallUpdateTimeRet : public GameMsg_Base
{
public:
    GameMsg_S2C_SetMallUpdateTimeRet();
    ~GameMsg_S2C_SetMallUpdateTimeRet();

public:
    bool doEncode(CParamPool &IOBuff);

};

class GameMsg_S2C_GroupBuyControlRet : public GameMsg_Base
{
public:
    GameMsg_S2C_GroupBuyControlRet();
    ~GameMsg_S2C_GroupBuyControlRet();

public:
    bool doEncode(CParamPool &IOBuff);
};

class GameMsg_C2S_ChangeSelfPassword : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_ChangeSelfPassword);
    GameMsg_C2S_ChangeSelfPassword();
    ~GameMsg_C2S_ChangeSelfPassword();
public:
    bool doDecode(CParamPool &IOBuff);
    std::string ToJson();
public:
    std::string m_OldPassword;
    std::string m_NewPassword;
};

class GameMsg_S2C_ChangeSelfPasswordRet : public GameMsg_Base
{
public:
    GameMsg_S2C_ChangeSelfPasswordRet();
    ~GameMsg_S2C_ChangeSelfPasswordRet();

public:
    bool doEncode(CParamPool &IOBuff);
public:
    bool m_bResult;
};

class GameMsg_C2S_ChangePassword : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_ChangePassword);
    GameMsg_C2S_ChangePassword();
    ~GameMsg_C2S_ChangePassword();
public:
    bool doDecode(CParamPool &IOBuff);
    std::string ToJson();
public:
    std::string m_UserName;
    std::string m_Password;
};

class GameMsg_C2S_GMSendPrivMCoin : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GMSendPrivMCoin);
    GameMsg_C2S_GMSendPrivMCoin();
    ~GameMsg_C2S_GMSendPrivMCoin();
public:
    bool doDecode(CParamPool &IOBuff);
    std::string ToJson();
public:
    vector<PlayerMoneyInfo> m_PlayerMoneyInfo;
};

class GameMsg_S2C_SendPrivMCoinRet : public GameMsg_Base
{
public:
    GameMsg_S2C_SendPrivMCoinRet();
    ~GameMsg_S2C_SendPrivMCoinRet();

public:
    bool doEncode(CParamPool &IOBuff);
public:
    vector<GMRechargeResult> m_PlayerResult;
};

class GameMsg_C2S_GetServerPlayerInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetServerPlayerInfo);
    GameMsg_C2S_GetServerPlayerInfo();
    ~GameMsg_C2S_GetServerPlayerInfo();

public:
    bool doDecode(CParamPool &IOBuff);
};

class GameMsg_S2C_GetServerPlayerInfoRet : public GameMsg_Base
{
public:
    GameMsg_S2C_GetServerPlayerInfoRet();
    ~GameMsg_S2C_GetServerPlayerInfoRet();

public:
    bool doEncode(CParamPool &IOBuff);

public:
    vector<CServerPlayerCountInfo> m_ServerPlayerCountInfoList;
};

class GameMsg_C2S_SetServerPlayerInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_SetServerPlayerInfo);
    GameMsg_C2S_SetServerPlayerInfo();
    ~GameMsg_C2S_SetServerPlayerInfo();

public:
    bool doDecode(CParamPool &IOBuff);

public:
    int m_nLimit;
};

class GameMsg_S2C_SetServerPlayerInfoRet : public GameMsg_Base
{
public:
    GameMsg_S2C_SetServerPlayerInfoRet();
    ~GameMsg_S2C_SetServerPlayerInfoRet();
public:
    bool doEncode(CParamPool &IOBuff);
};

class GameMsg_S2C_GetServerPlayerLimit : public GameMsg_Base
{
public:
    GameMsg_S2C_GetServerPlayerLimit();
    ~GameMsg_S2C_GetServerPlayerLimit();
public:
    bool doEncode(CParamPool &IOBuff);
public:
    int m_nLimit;
};

class GameMsg_C2S_SendBlackDeviceList : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_SendBlackDeviceList);
    GameMsg_C2S_SendBlackDeviceList();
    ~GameMsg_C2S_SendBlackDeviceList();
public:
    bool doDecode(CParamPool &IOBuff);
    std::string ToJson();

public:
    std::vector<std::string> m_DeviceNameList;
    bool m_bFrozenDevice;
};

class GameMsg_S2C_SendBlackDeviceListRes : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_SendBlackDeviceListRes);
    GameMsg_S2C_SendBlackDeviceListRes();
    ~GameMsg_S2C_SendBlackDeviceListRes();
public:
    bool doEncode(CParamPool &IOBuff);
public:
    unsigned short m_Result;
};

class GameMsg_C2S_ChangeAccountPassword : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_ChangeAccountPassword);
    GameMsg_C2S_ChangeAccountPassword();
    ~GameMsg_C2S_ChangeAccountPassword();
public:
    bool doDecode(CParamPool &IOBuff);
    std::string ToJson();

public:
    std::string m_AccountName;
    std::string m_Password;
};

class GameMsg_S2C_ChangeAccountPasswordRes : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_ChangeAccountPasswordRes);
    GameMsg_S2C_ChangeAccountPasswordRes();
    ~GameMsg_S2C_ChangeAccountPasswordRes();
public:
    bool doEncode(CParamPool &IOBuff);
public:
    unsigned short m_Result;
};

class GameMsg_C2S_DeleteAnnounce : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_DeleteAnnounce);
    GameMsg_C2S_DeleteAnnounce();
    ~GameMsg_C2S_DeleteAnnounce();
public:
    bool doDecode(CParamPool &IOBuff);
public:
    int m_nAnnounceID;
};

class GameMsg_S2C_DeleteAnnounceRes : public GameMsg_Base
{
public:
    GameMsg_S2C_DeleteAnnounceRes();
    ~GameMsg_S2C_DeleteAnnounceRes();
public:
    bool doEncode(CParamPool &IOBuff);
public:
    bool m_bResult;
};

class GameMsg_C2S_GetAnnounce : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetAnnounce);
    GameMsg_C2S_GetAnnounce();
    ~GameMsg_C2S_GetAnnounce();
public:
    bool doDecode(CParamPool &IOBuff);

};

class GameMsg_S2C_GetAnnounceRes : public GameMsg_Base
{
public:
    GameMsg_S2C_GetAnnounceRes();
    ~GameMsg_S2C_GetAnnounceRes();
public:
    bool doEncode(CParamPool &IOBuff);
public:
    std::vector<CAnnounceInfo> m_AnnounceInfoList;
};

//////////////////////////////////////////////////////////////////////////
// 

class GameMsg_C2S_QueryLog : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_QueryLog);
    GameMsg_C2S_QueryLog();
    ~GameMsg_C2S_QueryLog();
public:
    bool doDecode(CParamPool &IOBuff);
public:
    EGMActionType m_eActionType;
    unsigned int m_nStartTime;
    unsigned int m_nEndTime;
};

class QueryLogInfo
{
public:
    QueryLogInfo();
    ~QueryLogInfo();
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int m_nLogID;          // 执行二次查询
    std::string m_strAccouont; 
    unsigned int m_nOperTime;
    std::string m_strOperDesc;
};

class GameMsg_S2C_QueryLogRet : public GameMsg_Base
{
public:
    GameMsg_S2C_QueryLogRet();
    ~GameMsg_S2C_QueryLogRet();
public:
    bool doEncode(CParamPool &IOBuff);

//    EGMActionType m_eActionType;
    unsigned int m_nLogNum;
    bool m_bEndMsg;         
    std::list<QueryLogInfo> m_logInfo;
};

// 查询单条记录：
class GameMsg_C2S_QueryLogItem : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_QueryLogItem);
    GameMsg_C2S_QueryLogItem();
    ~GameMsg_C2S_QueryLogItem();
public:
    bool doDecode(CParamPool &IOBuff);
public:
    unsigned int m_nLogID; // 查询某条log 记录
};

class GameMsg_S2C_QueryLogItemRet : public GameMsg_Base
{
public:
    GameMsg_S2C_QueryLogItemRet();
    ~GameMsg_S2C_QueryLogItemRet();
public:
    bool doEncode(CParamPool &IOBuff);

    //    EGMActionType m_eActionType;
    unsigned int m_nLogNum;
    bool m_bEndMsg;
    std::list<std::string> m_logDesc; // 类型啥的就不用了 
};


////////////////////////////////////////////////////////////////////////// 
// GM消息
struct InfoCenterItemCM
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

    InfoCenterItemCM();

    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

};

// 查询所有的信息
class GameMsg_C2M_GetInfoCenterResCM : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2M_GetInfoCenterResCM);
    GameMsg_C2M_GetInfoCenterResCM();
    ~GameMsg_C2M_GetInfoCenterResCM();
public:
    bool doDecode(CParamPool &IOBuff);

};

class GameMsg_M2C_GetInfoCenterResCM : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_M2C_GetInfoCenterResCM);
    GameMsg_M2C_GetInfoCenterResCM();
    ~GameMsg_M2C_GetInfoCenterResCM();
public:
    bool doEncode(CParamPool &IOBuff);
public:
    std::vector<InfoCenterItemCM> m_infos;
};

// 添加
class GameMsg_C2M_AddInfoCenterResCM : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2M_AddInfoCenterResCM);
    GameMsg_C2M_AddInfoCenterResCM();
    ~GameMsg_C2M_AddInfoCenterResCM();
public:
    bool doDecode(CParamPool &IOBuff);

    InfoCenterItemCM m_items; // ID是无效的
};

class GameMsg_M2C_AddInfoCenterResCM : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_M2C_AddInfoCenterResCM);
    GameMsg_M2C_AddInfoCenterResCM();
    ~GameMsg_M2C_AddInfoCenterResCM();
public:
    bool doEncode(CParamPool &IOBuff);
public:

    InfoCenterItemCM			m_items;
};

// 更新
class GameMsg_C2M_UpdateInfoCenterResCM : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2M_UpdateInfoCenterResCM);
    GameMsg_C2M_UpdateInfoCenterResCM();
    ~GameMsg_C2M_UpdateInfoCenterResCM();
public:
    bool doDecode(CParamPool &IOBuff);

    InfoCenterItemCM m_items; // ID是无效的
};

class GameMsg_M2C_UpdateInfoCenterResCM : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_M2C_UpdateInfoCenterResCM);
    GameMsg_M2C_UpdateInfoCenterResCM();
    ~GameMsg_M2C_UpdateInfoCenterResCM();
public:
    bool doEncode(CParamPool &IOBuff);
public:
    InfoCenterItemCM			m_items;
};


// 删除
class GameMsg_C2M_RemoveInfoCenterResCM : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2M_RemoveInfoCenterResCM);
    GameMsg_C2M_RemoveInfoCenterResCM();
    ~GameMsg_C2M_RemoveInfoCenterResCM();
public:
    bool doDecode(CParamPool &IOBuff);

    unsigned int		m_id; //
};

class GameMsg_M2C_RemoveInfoCenterResCM : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_M2C_RemoveInfoCenterResCM);
    GameMsg_M2C_RemoveInfoCenterResCM();
    ~GameMsg_M2C_RemoveInfoCenterResCM();
public:
    bool doEncode(CParamPool &IOBuff);
public:
    unsigned short		m_result;
    unsigned int		m_id; //
};
//////////////////////////////////////////////////////////////////////////
//勋章
//////////////////////////////////////////////////////////////////////////

enum
{
    MedalFail = 0,
    MedalSuc = 1,
};

class GameMsg_M2G_DeleteVersionMedal : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_M2G_DeleteVersionMedal);
    GameMsg_M2G_DeleteVersionMedal();
    ~GameMsg_M2G_DeleteVersionMedal(){}

    bool doDecode(CParamPool &IOBuff){
        return true;
    }
};

class GameMsg_G2M_DeleteVersionMedal :public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2M_DeleteVersionMedal);
    GameMsg_G2M_DeleteVersionMedal();
    ~GameMsg_G2M_DeleteVersionMedal(){}

    bool doEncode(CParamPool &IOBuff);

    int m_nRetCode;
};

class GameMsg_C2M_AddMedal : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2M_AddMedal);
    GameMsg_C2M_AddMedal();
    ~GameMsg_C2M_AddMedal(){}

    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

    int m_nPlayerID;
    int m_nMedalID;
};


class GameMsg_M2C_AddMedal : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_M2C_AddMedal);
    GameMsg_M2C_AddMedal();
    ~GameMsg_M2C_AddMedal(){}

    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

    int m_nRetCode;
};


class GameMsg_C2M_RequestLockList : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2M_RequestLockList);
    GameMsg_C2M_RequestLockList();
    ~GameMsg_C2M_RequestLockList(){}

    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);


public:
    unsigned int m_nStartTime; // 开始时间，
    unsigned int m_nEndTime; // 结束时间
};

class GameMsg_M2C_ResponseLockList : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_M2C_ResponseLockList);
    GameMsg_M2C_ResponseLockList();
    ~GameMsg_M2C_ResponseLockList(){}

    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    std::list<PlayerLockedItem> m_lstLockItem;
};

// 请求骚扰信息列表
class GameMsg_C2M_HarassList : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2M_HarassList);
    GameMsg_C2M_HarassList();
    ~GameMsg_C2M_HarassList(){}

    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int m_nStartTime;
    unsigned int m_nEndTime;

    std::string m_strHarassInfo;
};

class GameMsg_M2C_HarassList : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_M2C_HarassList);
    GameMsg_M2C_HarassList();
    ~GameMsg_M2C_HarassList(){}

    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    std::list<RoleHarassInfo> m_lstRoleHarass; // 敏感词记录

};

// 读取玩家时间段内的骚扰信息
class GameMsg_C2M_RoleHarassChatInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2M_RoleHarassChatInfo);
    GameMsg_C2M_RoleHarassChatInfo();
    ~GameMsg_C2M_RoleHarassChatInfo(){}

    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int m_nRoleID;

    unsigned int m_nStartTime;
    unsigned int m_nEndTime;

    std::string m_strHarassInfo;
};

class GameMsg_M2C_RoleHarassChatInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_M2C_RoleHarassChatInfo);
    GameMsg_M2C_RoleHarassChatInfo();
    ~GameMsg_M2C_RoleHarassChatInfo(){}

    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int m_nPlatformID;
    std::string m_strPuid;
    unsigned int m_nRoleID;
    std::list<RoleChatInfo> m_lstRoleChatInfo;
};

class GameMsg_C2M_RequestRechargeRecordList : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2M_RequestRechargeRecordList);
    GameMsg_C2M_RequestRechargeRecordList();
    ~GameMsg_C2M_RequestRechargeRecordList(){}

    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);


public:
    unsigned int m_nStartTime; // 开始时间，
    unsigned int m_nEndTime; // 结束时间
    unsigned int m_nQueryBeginIndex;
    unsigned int m_nQueryCount;
};

class GameMsg_M2C_ResponseRechargeRecordList : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_M2C_ResponseRechargeRecordList);
    GameMsg_M2C_ResponseRechargeRecordList();
    ~GameMsg_M2C_ResponseRechargeRecordList(){}

    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    std::list<DeviceRechargeInfo> m_listRecharge;
};

#endif

