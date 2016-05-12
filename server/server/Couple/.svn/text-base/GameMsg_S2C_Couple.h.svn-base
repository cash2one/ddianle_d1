#ifndef __GAMEMSG_S2C_MARRIAGE_H__
#define __GAMEMSG_S2C_MARRIAGE_H__

#include <string>
#include <list>
#include "../../socket/GameMsg_Base.h"
#include "CoupleDataStructrue.h"
#include "../../datastructure/DataStruct_Couple.h"
#include "../item/EntityItemComponent.h"
#include "../gene/EntityGeneComponent.h"
#include "../Pet/PetComponent.h"

enum ESendCoupleRequestRes
{
    ESendCoupleRequest_Suc = 0,
    ESendCoupleRequest_CanNotAddSelf,
    ESendCoupleRequest_RequestTooFrequent,
    ESendCoupleRequest_LackOfLevel,
    ESendCoupleRequest_NotSingle,
    ESendCoupleRequest_LackOfRequiredItem,
    ESendCoupleRequest_RcverNotOnlineOrNotOnCurLine,
    ESendCoupleRequest_RcverNotSingle,
    ESendCoupleRequest_RcverLackOfLevel,
    ESendCoupleRequest_SexError,
    ESendCoupleRequest_IsDivorceing,  // 正在和对方离婚当中

    ESendCoupleRequest_Unknown,
};

enum EAcceptCoupleRequestRes
{
    EAcceptCoupleRequest_Suc = 0,
    EAcceptCoupleRequest_DataError,						// 内部数据错误
    EAcceptCoupleRequest_RequestHasProcessed,			// 请求已经被处理过或不存在
    EAcceptCoupleRequest_SelfLackOfLevel,				// 等级不足
    EAcceptCoupleRequest_OtherLackOfLevel,				// 对方等级不足
    EAcceptCoupleRequest_SelfNotSingle,					// 非单身
    EAcceptCoupleRequest_OtherNotSingle,				// 对方非单身
    EAcceptCoupleRequest_OtherNotOnlineOrNotOnCurLine,	// 对方不在本线或已下线
    EAcceptCoupleRequest_Couple_Data_Error,				// 对方内部数据错误
    EAcceptCoupleRequest_Sex_Error,						// 性别不符


    EAcceptCoupleRequest_Unkown,						// 未知原因
};

enum ECreateWeddingRoomRes
{
    ECreateWeddingRoom_Suc = 0,

    CreateWeddingRoom_NotHasCouple,
    CreateWeddingRoom_HasMarried,
    CreateWeddingRoom_LackOfIntimacy,
    CreateWeddingRoom_HasTheSameCoupleRoom,
    CreateWeddingRoom_LackOfRequiredItem,
    CreateWeddingRoom_InRoom,
    CreateWeddingRoom_EmptyName,
    CreateWeddingRoom_NameTooLong,
    CreateWeddingRoom_PwdTooLong,
    CreateWeddingRoom_RoomFull,
    CreateWeddingRoom_IncorrectWeddingType,
    CreateWeddingRoom_InDanceRoom,
    CreateWeddingRoom_InDanceLobby,
    CreateWeddingRoom_InPhotoRoom,
    CreateWeddingRoom_InAmuseRoom,
    CreateWeddingRoom_NotInRoom,                // 不能进入房间
    CreateWeddingRoom_NotCostItem,              // 没有消耗物品
    CreateWeddingRoom_WeddingConfigErr,         // 婚房配置错误
    CreateWeddingRoom_InDivorce,		        // 正在离婚中

    CreateWeddingRoom_Unknow,
    CreateWeddingRoom_InCeremonyRoom,
};

enum EEnterWeddingRoomRes
{
    EEnterWeddingRoom_Suc = 0,
    EEnterWeddingRoom_InRoom,
    EEnterWeddingRoom_RoomNotExists,
    EEnterWeddingRoom_HasPassword,
    EEnterWeddingRoom_IncorrectPassword,
    EEnterWeddingRoom_RoomFull,
    EEnterWeddingRoom_InDanceRoom,
    EEnterWeddingRoom_InDanceLobby,
    EEnterWeddingRoom_InPhotoRoom,
    EEnterWeddingRoom_InAmuseRoom,
    EEnterWeddingRoom_InCeremonyRoom,
};

enum EExitWeddingRoomRes
{
    EExitWeddingRoom_Suc,
    EExitWeddingRoom_NotInRoom,
};

enum ERequestMarry
{
    ERequestMarry_Suc = 0,
    ERequestMarry_OtherNotOnline,
    ERequestMarry_NotInWeddingRoom,
    ERequestMarry_NotHost,
    ERequestMarry_OthersNotInTheSameRoom,
    ERequestMarry_NotCouple,
    ERequestMarry_HasMarried,
    ERequestMarry_OthersHasMarried,
    ERequestMarry_LackOfIntimacy,
    ERequestMarry_NotHasCouple,
    ERequestMarry_InValidRoom,
    ERequestMarry_InDivorce,				// 正在结婚中
};

enum EAutoStartWeddingRoomRes
{
    EAutoStartWeddingRoom_Suc = 0,
    EAutoStartWeddingRoom_HostNotInRoom,
    EAutoStartWeddingRoom_NotAllInRoom,
    EAutoStartWeddingRoom_NotCouple,
    EAutoStartWeddingRoom_HasMarried,
    EAutoStartWeddingRoom_OthersHasMarried,
    EAutoStartWeddingRoom_LackOfIntimacy,
    EAutoStartWeddingRoom_InDivorce,
};

enum ECoupleSeparateRequestRes
{
    ECoupleSeparateRequest_Suc = 0,
    ECoupleSeparateRequest_YouAreSingle,
    ECoupleSeparateRequest_NotYouCoupleYet,
};

enum ERequestDivorceRes
{
    ERequestDivorce_Suc = 0,
    ERequestDivorce_NotMarriedNot,
    ERequestDivorce_LackOfDivorceItem,
    ERequestDivorce_Unknown,
};

enum EDivorceMsgError
{
    EDivorceMsgError_Suc = 0,
    EDivorceMsgError_AgreementNot,		// 协议书不存在
    EDivorceMsgError_AgreementStateErr,	// 协议书状态错误
    EDivorceMsgError_NotMarried,        // 没有结婚
    EDivorceMsgError_NotDivorceItem,    // 没有离婚道具
    EDivorceMsgError_NotMandatoryDivorceItem,    // 没有强制离婚道具
    EDivorceMsgError_HaveAgree,			// 已经同意过离婚
    EDivorceMsgError_NotRightPeople,    // 操作不是正确的人
    EDivorceMsgError_HaveAgreement,     // 已经有了协议书
    EDivorceMsgError_InRoom,			// 在房间内
    EDivorceMsgError_FunctionNotOpen,	// 功能未开放
};


enum ERequestKickWeddingRoomPlayer
{
    ERequestKickWeddingRoomPlayer_Suc = 0,
    ERequestKickWeddingRoomPlayer_NotInRoom,
    ERequestKickWeddingRoomPlayer_NotHost,
    ERequestKickWeddingRoomPlayer_RoomNotExists,
    ERequestKickWeddingRoomPlayer_TargetNotInRoom,
    ERequestKickWeddingRoomPlayer_CanNotKickSelf,


    ERequestKickWeddingRoomPlayer_Unknown,
};

enum EEnterMyWeddingRoomRes
{
    EEnterMyWeddingRoom_Suc = 0,
    EEnterMyWeddingRoom_NotHasCouple,
    EEnterMyWeddingRoom_HasMarried,
    EEnterMyWeddingRoom_AlreadyInRoom,
    EEnterMyWeddingRoom_RoomNotExistsOrNotOnCurLine,

};

enum ESetWeddingPlayerType
{
    ESetWeddingPlayerType_Suc = 0,
    ESetWeddingPlayerType_InvalidType,
    ESetWeddingPlayerType_NotInRoomOrWeddingBegan,
    ESetWeddingPlayerType_RoomNotExists,
    ESetWeddingPlayerType_NoAuthority,
    ESetWeddingPlayerType_CanNotSetGroomType,
    ESetWeddingPlayerType_CanNotSetBrideType,
    ESetWeddingPlayerType_TargetNotInRoom,
    ESetWeddingPlayerType_BestmanSexError,
    ESetWeddingPlayerType_BridesmaildSexError,

};

enum EChangeWeddingRoomPwdRes
{
    EChangeWeddingRoomPwd_Suc = 0,
    EChangeWeddingRoomPwd_PwdTooLong,
    EChangeWeddingRoomPwd_NotInRoom,
    EChangeWeddingRoomPwd_RoomNotExists,
    EChangeWeddingRoomPwd_NoAutority,
};


///////////////////////////////////////////////////////
class GameMsg_S2C_SendCoupleRequestSuc :
    public GameMsg_Base
{
public:
    GameMsg_S2C_SendCoupleRequestSuc();
    ~GameMsg_S2C_SendCoupleRequestSuc();
public:
    virtual bool doEncode(CParamPool &IOBuff);
};


///////////////////////////////////////////////////////
class GameMsg_S2C_SendCoupleRequestFail :
    public GameMsg_Base
{
public:
    GameMsg_S2C_SendCoupleRequestFail();
    ~GameMsg_S2C_SendCoupleRequestFail();
public:
    virtual bool doEncode(CParamPool &IOBuff);
public:
    unsigned char m_nError;
    std::string m_strError;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_StartWedding :
    public GameMsg_Base
{
public:
    GameMsg_S2C_StartWedding();
    ~GameMsg_S2C_StartWedding();

public:
    unsigned int m_nBlessingValue;
    unsigned int m_nGroomID;
    unsigned int m_BrideID;
    unsigned int m_nMarriedTime;
    virtual bool doEncode(CParamPool &IOBuff);
};

///////////////////////////////////////////////////////
class GameMsg_S2C_WeddingRoomStartingNofication :
    public GameMsg_Base
{
public:
    GameMsg_S2C_WeddingRoomStartingNofication();
    ~GameMsg_S2C_WeddingRoomStartingNofication();
public:
    virtual bool doEncode(CParamPool &IOBuff);
public:
    int m_nLine;
    string m_strLineName;
    unsigned int m_nRoomID;
    unsigned int m_nStartTime;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_ReceiveCoupleRequest :
    public GameMsg_Base
{
public:
    GameMsg_S2C_ReceiveCoupleRequest();
    ~GameMsg_S2C_ReceiveCoupleRequest();
public:
    virtual bool doEncode(CParamPool & IOBuff);
public:
    unsigned int m_nSenderID;
    std::string m_strSenderName;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_AcceptCoupleRequestSuc :
    public GameMsg_Base
{
public:
    GameMsg_S2C_AcceptCoupleRequestSuc();
    ~GameMsg_S2C_AcceptCoupleRequestSuc();
public:
    virtual bool doEncode(CParamPool & IOBuff);
public:
    unsigned int m_nPairID;
    unsigned int m_nCoupleID;
    std::string m_strCoupleName;
    unsigned int m_nCoupleDate;
    unsigned int m_nSmallRoleID;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_AcceptCoupleRequestFail :
    public GameMsg_Base
{
public:
    GameMsg_S2C_AcceptCoupleRequestFail();
    ~GameMsg_S2C_AcceptCoupleRequestFail();
public:
    virtual bool doEncode(CParamPool & IOBuff);
public:
    std::string m_strError;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_CoupleRequestReplay : public GameMsg_Base
{
public:
    GameMsg_S2C_CoupleRequestReplay();
    ~GameMsg_S2C_CoupleRequestReplay();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    CCoupleRequestReplay    m_requestReplay;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_CoupleSeparateRequestSuc :
    public GameMsg_Base
{
public:
    GameMsg_S2C_CoupleSeparateRequestSuc();
    ~GameMsg_S2C_CoupleSeparateRequestSuc();
public:
    virtual bool doEncode(CParamPool &IOBuff);
};

///////////////////////////////////////////////////////
class GameMsg_S2C_CoupleSeparateRequestFail :
    public GameMsg_Base
{
public:
    GameMsg_S2C_CoupleSeparateRequestFail();
    ~GameMsg_S2C_CoupleSeparateRequestFail();
public:
    virtual bool doEncode(CParamPool &IOBuff);
public:
    std::string m_strError;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_NotifydHasBeenSeparated :
    public GameMsg_Base
{
public:
    GameMsg_S2C_NotifydHasBeenSeparated();
    ~GameMsg_S2C_NotifydHasBeenSeparated();
public:
    virtual bool doEncode(CParamPool &IOBuff);
public:
    unsigned int m_nSeparatedBy;
    std::string m_strSeparatedBy;
};

///////////////////////////////////////////////////////
//class GameMsg_S2C_RequestMarrySuc:
//	public GameMsg_Base
//{
//public:
//	GameMsg_S2C_RequestMarrySuc();
//	~GameMsg_S2C_RequestMarrySuc();
//public:
//	virtual bool doEncode(CParamPool &IOBuff);
//};
///////////////////////////////////////////////////////
class GameMsg_S2C_RequestMarryFail :
    public GameMsg_Base
{
public:
    GameMsg_S2C_RequestMarryFail();
    ~GameMsg_S2C_RequestMarryFail();
public:
    virtual bool doEncode(CParamPool &IOBuff);
public:
    unsigned char m_nError;
    std::string m_strError;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_AutoStartWeddingRoomFail :
    public GameMsg_Base
{
public:
    GameMsg_S2C_AutoStartWeddingRoomFail();
    ~GameMsg_S2C_AutoStartWeddingRoomFail();
public:
    virtual bool doEncode(CParamPool &IOBuff);
public:
    unsigned char m_nError;
    std::string m_strError;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_RequestDivorceSuc :
    public GameMsg_Base
{
public:
    GameMsg_S2C_RequestDivorceSuc();
    ~GameMsg_S2C_RequestDivorceSuc();
public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned int m_nSmallRoleID;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_RequestDivorceFail :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_RequestDivorceFail);
    GameMsg_S2C_RequestDivorceFail();
    ~GameMsg_S2C_RequestDivorceFail();
public:
    virtual bool doEncode(CParamPool &IOBuff);
public:
    unsigned char m_nError;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_NotifyHasBeenDivorced :
    public GameMsg_Base
{
public:
    GameMsg_S2C_NotifyHasBeenDivorced();
    ~GameMsg_S2C_NotifyHasBeenDivorced();
public:
    virtual bool doEncode(CParamPool & IOBuff);
public:
    unsigned int m_nDivorcedBy;
    std::string m_strDivorcedBy;
};
///////////////////////////////////////////////////////
class GameMsg_S2C_GetWeddingRoomListRes :
    public GameMsg_Base
{
public:
    GameMsg_S2C_GetWeddingRoomListRes();
    ~GameMsg_S2C_GetWeddingRoomListRes();
public:
    virtual bool doEncode(CParamPool & IOBuff);
public:
    unsigned short m_nCurPage;
    unsigned short m_nTotalCount;
    std::list<CWeddingRoomBriefInfo> m_listRoomInfo;
};
///////////////////////////////////////////////////////
class GameMsg_S2C_CreateWeddingRoomFail :
    public GameMsg_Base
{
public:
    GameMsg_S2C_CreateWeddingRoomFail();
    ~GameMsg_S2C_CreateWeddingRoomFail();
public:
    virtual bool doEncode(CParamPool & IOBuff);
public:
    unsigned char m_nError;
    std::string m_strError;
    int		m_nNeedCount;
};
///////////////////////////////////////////////////////
class GameMsg_S2C_EnterWeddingRoomSuc :
    public GameMsg_Base
{
public:
    GameMsg_S2C_EnterWeddingRoomSuc();
    ~GameMsg_S2C_EnterWeddingRoomSuc();
public:
    virtual bool doEncode(CParamPool & IOBuff);
public:
    CWeddingRoomWholeInfo m_roomWholeInfo;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_EnterWeddingPlayer :
    public GameMsg_Base
{
public:
    GameMsg_S2C_EnterWeddingPlayer();
    ~GameMsg_S2C_EnterWeddingPlayer();
public:
    virtual bool doEncode(CParamPool & IOBuff);
public:
    CWeddingRoomPlayerInfo m_roomPlayer;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_TryEnterWeddingRoomFail :
    public GameMsg_Base
{
public:
    GameMsg_S2C_TryEnterWeddingRoomFail();
    ~GameMsg_S2C_TryEnterWeddingRoomFail();
public:
    virtual bool doEncode(CParamPool & IOBuff);
public:
    int m_nRoomID;
    unsigned char m_nError;
    std::string m_strError;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_EnterWeddingRoomFail :
    public GameMsg_Base
{
public:
    GameMsg_S2C_EnterWeddingRoomFail();
    ~GameMsg_S2C_EnterWeddingRoomFail();
public:
    virtual bool doEncode(CParamPool &IOBuff);
public:
    unsigned char m_nError;
    std::string m_strError;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_EnterMyWeddingRoomFail :
    public GameMsg_Base
{
public:
    GameMsg_S2C_EnterMyWeddingRoomFail();
    ~GameMsg_S2C_EnterMyWeddingRoomFail();
public:
    virtual bool doEncode(CParamPool & IOBuff);
public:
    unsigned char m_nError;
    std::string m_strError;
};
///////////////////////////////////////////////////////
class GameMsg_S2C_ExitWeddingRoomSuc :
    public GameMsg_Base
{
public:
    GameMsg_S2C_ExitWeddingRoomSuc();
    ~GameMsg_S2C_ExitWeddingRoomSuc();
public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    char        m_nQuitTo;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_ExitWeddingRoomFail :
    public GameMsg_Base
{
public:
    GameMsg_S2C_ExitWeddingRoomFail();
    ~GameMsg_S2C_ExitWeddingRoomFail();
public:
    virtual bool doEncode(CParamPool &IOBuff);
public:
    unsigned char m_nError;
    std::string m_strError;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_PlayerExitedRoom :
    public GameMsg_Base
{
public:
    GameMsg_S2C_PlayerExitedRoom();
    ~GameMsg_S2C_PlayerExitedRoom();
public:
    virtual bool doEncode(CParamPool &IOBuff);
public:
    unsigned int m_nExitID;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_KickWeddingPlayerSuc :
    public GameMsg_Base
{
public:
    GameMsg_S2C_KickWeddingPlayerSuc();
    ~GameMsg_S2C_KickWeddingPlayerSuc();
public:
    virtual bool doEncode(CParamPool &IOBuff);
public:
    unsigned int m_nTargetID;
};

////////////////////////////////////////////////////
class GameMsg_S2C_KickWeddingPlayerFail :
    public GameMsg_Base
{
public:
    GameMsg_S2C_KickWeddingPlayerFail();
    ~GameMsg_S2C_KickWeddingPlayerFail();
public:
    virtual bool doEncode(CParamPool &IOBuff);
public:
    unsigned char m_nError;
    std::string m_strError;
};

////////////////////////////////////////////////////
class GameMsg_S2C_SetWeddingPlayerTypeSuc :
    public GameMsg_Base
{
public:
    GameMsg_S2C_SetWeddingPlayerTypeSuc();
    ~GameMsg_S2C_SetWeddingPlayerTypeSuc();
public:
    virtual bool doEncode(CParamPool &IOBuff);
public:
    unsigned int m_nTargetID;
    unsigned char m_RoomRoleType;
};

////////////////////////////////////////////////////
class GameMsg_S2C_SetWeddingPlayerTypeFail :
    public GameMsg_Base
{
public:
    GameMsg_S2C_SetWeddingPlayerTypeFail();
    ~GameMsg_S2C_SetWeddingPlayerTypeFail();
public:
    virtual bool doEncode(CParamPool &IOBuff);
public:
    std::string m_strError;
};

////////////////////////////////////////////////////
class GameMsg_S2C_NotifyChangeWeddingPlayerType :
    public GameMsg_Base
{
public:
    GameMsg_S2C_NotifyChangeWeddingPlayerType();
    ~GameMsg_S2C_NotifyChangeWeddingPlayerType();
public:
    virtual bool doEncode(CParamPool &IOBuff);
public:
    unsigned int m_nTargetID;
    unsigned char m_nRoomRoleType;
};

////////////////////////////////////////////////////
class GameMsg_S2C_ChangeWeddingRoomPwdSuc :
    public GameMsg_Base
{
public:
    GameMsg_S2C_ChangeWeddingRoomPwdSuc();
    ~GameMsg_S2C_ChangeWeddingRoomPwdSuc();
public:
    virtual bool doEncode(CParamPool &IOBuff);
public:
    std::string m_strNewPwd;
};

////////////////////////////////////////////////////
class GameMsg_S2C_ChangeWeddingRoomPwdFail :
    public GameMsg_Base
{
public:
    GameMsg_S2C_ChangeWeddingRoomPwdFail();
    ~GameMsg_S2C_ChangeWeddingRoomPwdFail();
public:
    virtual bool doEncode(CParamPool &IOBuff);
public:
    std::string m_strError;
};

////////////////////////////////////////////////////
class GameMsg_S2C_UpdateCoupleIntimacy :
    public GameMsg_Base
{
public:
    GameMsg_S2C_UpdateCoupleIntimacy();
    ~GameMsg_S2C_UpdateCoupleIntimacy();
public:
    virtual bool doEncode(CParamPool &IOBuff);
public:
    unsigned int m_nIntimacy;
};

////////////////////////////////////////////////////
class GameMsg_S2C_WeddingRoomStateChanged :
    public GameMsg_Base
{
public:
    GameMsg_S2C_WeddingRoomStateChanged();
    ~GameMsg_S2C_WeddingRoomStateChanged();
public:
    virtual bool doEncode(CParamPool &IOBuff);
public:
    bool m_bValid;
};

////////////////////////////////////////////////////
class GameMsg_S2C_UpdateCoupleName :
    public GameMsg_Base
{
public:
    GameMsg_S2C_UpdateCoupleName();
    ~GameMsg_S2C_UpdateCoupleName();
public:
    virtual bool doEncode(CParamPool &IOBuff);
public:
    std::string m_strNewName;
};

///////////////////////////////////////////////////
class GameMsg_S2C_BlessingNotify :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_BlessingNotify);
    GameMsg_S2C_BlessingNotify();
    ~GameMsg_S2C_BlessingNotify(){};

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    std::string   m_strManName;
    std::string   m_strWomanName;
    unsigned int  m_nPointIndex;
};


//////////////////////////////////////////////////////
class GameMsg_S2C_GetBlessingRankListResult :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetBlessingRankListResult);
    GameMsg_S2C_GetBlessingRankListResult();
    ~GameMsg_S2C_GetBlessingRankListResult();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned short						   m_nCurrentPage;
    unsigned short                         m_nPageCount;
    unsigned short						   m_nRankTotalCount;
    std::vector<CBlessingValueRankInfo>    m_BlessingRankVec;

};

//////////////////////////////////////////////////////
class GameMsg_S2C_GetCoupleInfoResult :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetCoupleInfoResult);
    GameMsg_S2C_GetCoupleInfoResult();
    ~GameMsg_S2C_GetCoupleInfoResult(){};

public:
    virtual bool doEncode(CParamPool & IOBuff);

public:
    unsigned int	m_nRank;
    COfflineCoupleInfo m_CoupleInfo;
};


//////////////////////////////////////////////////////
class GameMsg_S2C_GetWeddingRecordListResult :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetWeddingRecordListResult);
    GameMsg_S2C_GetWeddingRecordListResult();
    ~GameMsg_S2C_GetWeddingRecordListResult(){};

public:
    virtual bool doEncode(CParamPool & IOBuff);
    bool         IsBlessed(const std::vector<unsigned int> & BlessingRoleIDList, unsigned int nRoleID);

public:
    unsigned int                              m_nRoleID;
    std::vector<WeddingRecordInfo>         m_WeddingRecord;
};


//////////////////////////////////////////////////////
class GameMsg_S2C_GetBlessingValueResult :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetBlessingValueResult);
    GameMsg_S2C_GetBlessingValueResult();
    ~GameMsg_S2C_GetBlessingValueResult(){};

public:
    virtual bool doEncode(CParamPool & IOBuff);

public:
    unsigned int                              m_nBlessingValue;
    unsigned short                            m_nLine;
};

//////////////////////////////////////////////////////
class GameMsg_S2C_GetPersonInfoResult :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetPersonInfoResult);
    GameMsg_S2C_GetPersonInfoResult();
    ~GameMsg_S2C_GetPersonInfoResult(){};

public:
    virtual bool doEncode(CParamPool & IOBuff);

public:
    RoleImageOfflineData        m_OffLineData;
    CEntityItemComponent*  m_pRoleItem;
    CEntityGeneComponent*  m_pRoleGene;
    PetComponent*          m_pRolePet;
};


//////////////////////////////////////////////////////
class GameMsg_S2C_BlessingError :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_BlessingError);
    GameMsg_S2C_BlessingError();
    ~GameMsg_S2C_BlessingError(){};

public:
    virtual bool doEncode(CParamPool & IOBuff);

public:
    std::string	   m_strError;
};

//////////////////////////////////////////////////////
class GameMsg_S2C_BlessingCoupleSuc :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_BlessingCoupleSuc);
    GameMsg_S2C_BlessingCoupleSuc();
    ~GameMsg_S2C_BlessingCoupleSuc(){};

public:
    virtual bool doEncode(CParamPool & IOBuff);

};

//////////////////////////////////////////////////////
class GameMsg_S2C_RedEnvelopeSuc :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_RedEnvelopeSuc);
    GameMsg_S2C_RedEnvelopeSuc();
    ~GameMsg_S2C_RedEnvelopeSuc(){};

public:
    virtual bool doEncode(CParamPool & IOBuff);

};

//////////////////////////////////////////////////////
class GameMsg_S2C_GetCoupleRedEnvelopeGiftInfoResult :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetCoupleRedEnvelopeGiftInfoResult);
    GameMsg_S2C_GetCoupleRedEnvelopeGiftInfoResult();
    ~GameMsg_S2C_GetCoupleRedEnvelopeGiftInfoResult(){};

public:
    virtual bool doEncode(CParamPool & IOBuff);

public:
    unsigned int	m_nRedEnvelopeCount;
    std::map<unsigned int, itemtype_t> m_RedEnvelopeInfo;
    std::map<unsigned int, std::list<CItem> > m_PacketInfo;


};

//////////////////////////////////////////////////////
class GameMsg_S2C_CancelDivorceSuc :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_CancelDivorceSuc);
    GameMsg_S2C_CancelDivorceSuc();
    ~GameMsg_S2C_CancelDivorceSuc(){};

public:
    virtual bool doEncode(CParamPool & IOBuff);

};

//////////////////////////////////////////////////////
class GameMsg_S2C_CancelDivorceFail :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_CancelDivorceFail);
    GameMsg_S2C_CancelDivorceFail();
    ~GameMsg_S2C_CancelDivorceFail(){};

public:
    virtual bool doEncode(CParamPool & IOBuff);

public:
    char		m_cErrorType;
};


//////////////////////////////////////////////////////
class GameMsg_S2C_MandatoryDivorceSuc :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_MandatoryDivorceSuc);
    GameMsg_S2C_MandatoryDivorceSuc();
    ~GameMsg_S2C_MandatoryDivorceSuc(){};

public:
    virtual bool doEncode(CParamPool & IOBuff);

};

//////////////////////////////////////////////////////
class GameMsg_S2C_MandatoryDivorceFail :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_MandatoryDivorceFail);
    GameMsg_S2C_MandatoryDivorceFail();
    ~GameMsg_S2C_MandatoryDivorceFail(){};

public:
    virtual bool doEncode(CParamPool & IOBuff);

public:
    char		m_cErrorType;
};

//////////////////////////////////////////////////////
class GameMsg_S2C_AgreeDivorceSuc :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_AgreeDivorceSuc);
    GameMsg_S2C_AgreeDivorceSuc();
    ~GameMsg_S2C_AgreeDivorceSuc(){};

public:
    virtual bool doEncode(CParamPool & IOBuff);

};

//////////////////////////////////////////////////////
class GameMsg_S2C_AgreeDivorceFail :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_AgreeDivorceFail);
    GameMsg_S2C_AgreeDivorceFail();
    ~GameMsg_S2C_AgreeDivorceFail(){};

public:
    virtual bool doEncode(CParamPool & IOBuff);

public:
    char		m_cErrorType;
};

//////////////////////////////////////////////////////
class GameMsg_S2C_GiftDivorceSuc :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GiftDivorceSuc);
    GameMsg_S2C_GiftDivorceSuc();
    ~GameMsg_S2C_GiftDivorceSuc(){};

public:
    virtual bool doEncode(CParamPool & IOBuff);

public:


};

//////////////////////////////////////////////////////
class GameMsg_S2C_GiftDivorceFail :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GiftDivorceFail);
    GameMsg_S2C_GiftDivorceFail();
    ~GameMsg_S2C_GiftDivorceFail(){};

public:
    virtual bool doEncode(CParamPool & IOBuff);

public:
    char		m_cErrorType;
};

//////////////////////////////////////////////////////
class GameMsg_S2C_RefreshAction :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_RefreshAction);
    GameMsg_S2C_RefreshAction();
    ~GameMsg_S2C_RefreshAction(){};

public:
    virtual bool doEncode(CParamPool & IOBuff);

public:
    CDivorceAction m_action;
    CDivorceAgreementInfo m_DivorceAgreementInfo;
};

//////////////////////////////////////////////////////
class GameMsg_S2C_DivorceWishItem :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_DivorceWishItem);
    GameMsg_S2C_DivorceWishItem();
    ~GameMsg_S2C_DivorceWishItem(){};

public:
    virtual bool doEncode(CParamPool & IOBuff);

public:
    std::vector<itemtype_t> m_WishItem;
};

//////////////////////////////////////////////////////
struct DivorcePlayerMsg
{
    unsigned int nRoleID;
    std::string strRoleName;
    char cRoleSex;
    char cRoleSkin;
    char cDanceGroupPos;
    unsigned short nDanceGroupBadgeID;
    unsigned short nDanceGroupEffectID;
    std::string strDanceGroupName;
    bool bIsVip;
    unsigned short nVipLevel;
    char cMoveType;
    CEntityItemComponent * pRoleItem;
    CEntityGeneComponent * pRoleGene;
    RoleImageOfflineData        OffLineData;

    DivorcePlayerMsg()
    {
        nRoleID = 0;;
        strRoleName = "";
        cRoleSex = 0;
        cRoleSkin = 0;
        cDanceGroupPos = 0;
        nDanceGroupBadgeID = 0;
        nDanceGroupEffectID = 0;
        strDanceGroupName = "";
        bIsVip = false;
        nVipLevel = 0;
        cMoveType = 0;
        pRoleItem = NULL;
        pRoleGene = NULL;
    }

    void doEncode(CParamPool & IOBuff)
    {
        IOBuff.AddUInt(nRoleID);
        IOBuff.AddStringW(strRoleName);
        IOBuff.AddChar(cRoleSex);
        IOBuff.AddChar(cRoleSkin);
        IOBuff.AddChar(cDanceGroupPos);
        IOBuff.AddStringW(strDanceGroupName);
        IOBuff.AddUShort(nDanceGroupBadgeID);
        IOBuff.AddUShort(nDanceGroupEffectID);
        IOBuff.AddBool(bIsVip);
        IOBuff.AddUShort(nVipLevel);
        IOBuff.AddChar(cMoveType);

        if (pRoleItem != NULL)
        {
            pRoleItem->PackItemForOther(IOBuff);
        }
        else
        {
            int nOldPos1 = IOBuff.GetCurPosition();
            IOBuff.AddUShort(0);

            int nOldPos2 = IOBuff.GetCurPosition();
            IOBuff.AddUShort(0);

            short nCount = 0;
            for (size_t i = 0; i < OffLineData.m_vecRoleItem.size(); ++i)
            {
                IOBuff.AddUInt(OffLineData.m_vecRoleItem[i].m_nTypeID);
                IOBuff.AddUShort(OffLineData.m_vecRoleItem[i].m_nColumn);
                IOBuff.AddUShort(OffLineData.m_vecRoleItem[i].m_nIndex);

                char szItemBuff[256] = { 0 };
                memset(szItemBuff, 0, sizeof(szItemBuff));
                CParamPool itemBuff(szItemBuff, sizeof(szItemBuff));
                itemBuff.AddUShort(0);
                itemBuff.AddUShort(1);
                itemBuff.AddInt(-1);
                itemBuff.AddUChar(1);

                // 衣服特效
                std::map<itemid_t, unsigned int>::iterator it = OffLineData.m_mapClothEffect.find(OffLineData.m_vecRoleItem[i].m_nItemID);
                if (it != OffLineData.m_mapClothEffect.end())
                {
                    itemBuff.AddUInt(it->second);
                }
                else
                {
                    itemBuff.AddUInt(0);
                }

                unsigned int nItemSize = itemBuff.GetValidDataSize();
                ASSERT(nItemSize == itemBuff.GetCurPosition());

                IOBuff.AddUChar((unsigned char)nItemSize);
                IOBuff.AddMemory(itemBuff.GetData(), (unsigned short)nItemSize);

                nCount++;
            }

            int nFinalPos = IOBuff.GetCurPosition();
            IOBuff.SetCurPosition(nOldPos2);
            IOBuff.ReplaceShort(nCount);
            IOBuff.SetCurPosition(nFinalPos);

            // 七色彩虹信息
            IOBuff.AddUShort((unsigned short)OffLineData.m_mapClothEffectSevenColor.size());
            for (std::map<unsigned short, std::set<ESevenColorType> >::iterator it = OffLineData.m_mapClothEffectSevenColor.begin();
                it != OffLineData.m_mapClothEffectSevenColor.end(); ++it)
            {
                IOBuff.AddUShort(it->first);
                IOBuff.AddUShort((unsigned short)it->second.size());
                for (std::set<ESevenColorType>::iterator itColor = it->second.begin();
                    itColor != it->second.end(); ++itColor)
                {
                    IOBuff.AddUChar((unsigned char)(*itColor));
                }
            }

            if (IOBuff.GetCurPosition() - nOldPos1 - sizeof(unsigned short) != 0)
            {
                nFinalPos = IOBuff.GetCurPosition();
                IOBuff.SetCurPosition(nOldPos1);
                IOBuff.ReplaceShort((short)(nFinalPos - nOldPos1 - sizeof(unsigned short)));
                IOBuff.SetCurPosition(nFinalPos);
            }
        }

        if (pRoleGene != NULL)
        {
            pRoleGene->PackGeneForOther(IOBuff);
        }
        else
        {
            IOBuff.AddUShort(0);
        }
    }
};


class GameMsg_S2C_DivorceAnimationSuc :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_DivorceAnimationSuc);
    GameMsg_S2C_DivorceAnimationSuc();
    ~GameMsg_S2C_DivorceAnimationSuc(){};

public:
    virtual bool doEncode(CParamPool & IOBuff);

public:
    DivorcePlayerMsg m_MaleMsg;
    DivorcePlayerMsg m_FemaleMsg;
    std::list<CDivorceAction> m_Actions;
    CDivorceAgreementInfo m_DivorceAgreementInfo;
};

//////////////////////////////////////////////////////
class GameMsg_S2C_DivorceAnimationFail :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_DivorceAnimationFail);
    GameMsg_S2C_DivorceAnimationFail();
    ~GameMsg_S2C_DivorceAnimationFail(){};

public:
    virtual bool doEncode(CParamPool & IOBuff);

public:
    char		m_cErrorType;
};

//////////////////////////////////////////////////////
class GameMsg_S2C_DivorceHaveNewCouple :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_DivorceHaveNewCouple);
    GameMsg_S2C_DivorceHaveNewCouple();
    ~GameMsg_S2C_DivorceHaveNewCouple(){};

public:
    virtual bool doEncode(CParamPool & IOBuff);

};

#endif

