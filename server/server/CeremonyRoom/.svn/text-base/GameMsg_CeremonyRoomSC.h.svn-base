#ifndef	__GAMEMSG_CEREMONYROOMSC_H__
#define	__GAMEMSG_CEREMONYROOMSC_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DanceGroup.h"
#include "../../datastructure/DataStruct_Base.h"

class CParamPool;
class CDanceGroupInfo;


class CeremonyPlayerBaseInfo
{
public:
    CeremonyPlayerBaseInfo();
    virtual ~CeremonyPlayerBaseInfo() {}

    virtual bool DoEncode(CParamPool &IOBuff);

    unsigned int m_nPlayerID;
    std::string m_strPlayerName;
    unsigned char m_nPlayerSex;
    bool m_bIsVIP;
    unsigned short m_nVIPLevel;
    /// 玩家舞团职位-团长 副团长 团务 正式成员 见习成员
    unsigned char m_nDanceGroupPos;
    /// 玩家舞团属性-创始人 老团员 小萌新
    unsigned char m_nDanceGroupProperty;
    /// 仪式玩家类型-0观众 1申请人 2新人
    unsigned char m_nPlayerType;
    /// 仪式新人序号-1到8
    int m_nRookieIndex;
};

class CEntityItemComponent;
class CEntityGeneComponent;
class PetComponent;
class CEntityMedalComponent;
class CEntityHorseComponent;

/// 仪式玩家显示信息-需要显示玩家形象及移动
class CeremonyPlayerShowInfo : public CeremonyPlayerBaseInfo
{
public:
    CeremonyPlayerShowInfo();
    virtual ~CeremonyPlayerShowInfo() {}

    virtual bool DoEncode(CParamPool &IOBuff);

    unsigned short m_nDanceGroupBadgeId;
    unsigned short m_nDanceGroupEffectId;
    std::string m_strDanceGroupName;
    /// 玩家移动类型-行走 飞行
    unsigned char m_nMoveType;
    /// 玩家朝向
    float m_fOrient;
    /// 玩家坐标x
    float m_fPosX;
    /// 玩家坐标y
    float m_fPosY;
    /// 玩家坐标z
    float m_fPosZ;
    /// 玩家肤色
    unsigned char m_nPlayerSkin;
    /// 玩家变身形象ID
    unsigned short m_nTransformId;
    CEntityItemComponent* m_pItemCOM;
    CEntityGeneComponent* m_pGeneCOM;
    PetComponent*         m_pPetCOM;
    CEntityMedalComponent *m_pMedalCOM;
    CEntityHorseComponent *m_pHorseCOM;
};

/// 仪式新人介绍信息
class CeremonyRookieInfo
{
public:
    CeremonyRookieInfo();
    bool DoEncode(CParamPool &IOBuff);

    /// 玩家ID
    unsigned int m_nRoleID;
    /// 仪式新人序号
    int m_nRookieIndex;
    /// 新人自我介绍
    std::string m_strIntruduction;
};

/// 仪式房间信息
class CeremonyRoomInfo
{
public:
    CeremonyRoomInfo();
    void DoEncode(CParamPool &IOBuff);

    /// 仪式状态-CeremonyState
    unsigned char m_nCeremonyState;
    /// 仪式开启倒计时
    float m_nCeremontyCD;
    /// 仪式参与者列表 所有人包括普通玩家 申请人 新人
    std::vector<CeremonyPlayerShowInfo> m_vecCeremonyPlayers;
};


/// C2S 申请入团仪式
class GameMsg_C2S_RequestApplyCeremony : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_RequestApplyCeremony);
    GameMsg_C2S_RequestApplyCeremony();

    virtual bool doDecode(CParamPool &IOBuff);
};

/// S2C 申请入团仪式结果
class GameMsg_S2C_RequestApplyCeremonyRes : public GameMsg_Base
{
public:
    GameMsg_S2C_RequestApplyCeremonyRes();

    virtual bool doEncode(CParamPool &IOBuff);

    /// 错误枚举-0表示成功
    unsigned char m_nErrorType;

};


/// C2S 请求进入仪式房间
class GameMsg_C2S_RequestEnterCeremony : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_RequestEnterCeremony);
    GameMsg_C2S_RequestEnterCeremony();

    virtual bool doDecode(CParamPool &IOBuff);

};

/// S2C 进入仪式房间结果
class GameMsg_S2C_EnterCeremonyRes : public GameMsg_Base
{
public:
    GameMsg_S2C_EnterCeremonyRes();
    ~GameMsg_S2C_EnterCeremonyRes(){}

    virtual bool doEncode(CParamPool &IOBuff);

    /// 错误枚举-0表示成功
    unsigned char m_nErrorType;
    /// 仪式房间信息
    CeremonyRoomInfo m_CeremonyRoomInfo;
};

/// C2S 请求退出仪式房间
class GameMsg_C2S_RequestQuitCeremony : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_RequestQuitCeremony);
    GameMsg_C2S_RequestQuitCeremony();

    virtual bool doDecode(CParamPool &IOBuff);

public:
    char            m_nQuitTo;
};

/// S2C 退出仪式房间结果
class GameMsg_S2C_QuitCeremonyRes : public GameMsg_Base
{
public:
    GameMsg_S2C_QuitCeremonyRes();
    virtual bool doEncode(CParamPool &IOBuff);

    /// 错误枚举-0表示成功
    unsigned char m_nErrorType;
    /// 退出类型
    unsigned char m_nQuitType;
    /// 踢人者ID
    unsigned int m_nKickerID;
    // 退出去的地方
    char    m_nQuitTo;
};

/// C2S 请求刷新仪式房间,
class GameMsg_C2S_RequestRefreshCeremony : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_RequestRefreshCeremony);
    GameMsg_C2S_RequestRefreshCeremony();

    virtual bool doDecode(CParamPool &IOBuff);

};

/// S2C 刷新仪式房间结果
class GameMsg_S2C_RefreshCeremonyRes : public GameMsg_Base
{
public:
    GameMsg_S2C_RefreshCeremonyRes();
    virtual bool doEncode(CParamPool &IOBuff);

    /// 错误枚举-0表示成功
    unsigned char m_nErrorType;
    /// 仪式房间信息
    CeremonyRoomInfo m_CeremonyRoomInfo;
};

/// C2S 请求开始仪式
class GameMsg_C2S_RequestStartCeremony : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_RequestStartCeremony);
    GameMsg_C2S_RequestStartCeremony();

    virtual bool doDecode(CParamPool &IOBuff);
};

/// S2C 开启仪式结果
class GameMsg_S2C_StartCeremonyRes : public GameMsg_Base
{
public:
    GameMsg_S2C_StartCeremonyRes();
    virtual bool doEncode(CParamPool &IOBuff);

    /// 错误枚举-0表示成功
    unsigned char m_nErrorType;
    /// 仪式新人介绍信息
    std::vector<CeremonyRookieInfo> m_vecRookieInfos;
};

/// S2C 其他玩家进入仪式房间
class GameMsg_S2C_OtherPlayerEnterCeremony : public GameMsg_Base
{
public:
    GameMsg_S2C_OtherPlayerEnterCeremony();
    virtual bool doEncode(CParamPool &IOBuff);

    CeremonyPlayerShowInfo m_PlayerInfo;
};

/// S2C 其他玩家退出仪式房间
class GameMsg_S2C_OtherPlayerQuitCeremony : public GameMsg_Base
{
public:
    GameMsg_S2C_OtherPlayerQuitCeremony();
    virtual bool doEncode(CParamPool &IOBuff);

    unsigned int m_nPlayerID;
};

/// C2S 设为仪式新人
class GameMsg_C2S_SetCeremonyRookie : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_SetCeremonyRookie);
    GameMsg_C2S_SetCeremonyRookie();
    virtual bool doDecode(CParamPool &IOBuff);

    /// 玩家ID
    unsigned int m_nRoleId;
    /// 新人序号
    int m_nRookieIndex;
};

/// S2C 设为仪式新人结果
class GameMsg_S2C_SetCeremonyRookieRes : public GameMsg_Base
{
public:
    GameMsg_S2C_SetCeremonyRookieRes();
    virtual bool doEncode(CParamPool &IOBuff);

    unsigned char m_nErrorType;
};

/// C2S 取消仪式新人
class GameMsg_C2S_CancelCeremonyRookie : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_CancelCeremonyRookie);
    GameMsg_C2S_CancelCeremonyRookie();
    virtual bool doDecode(CParamPool &IOBuff);

    /// 玩家ID
    unsigned int m_nRoleId;
};

/// S2C 取消仪式新人结果
class GameMsg_S2C_CancelCeremonyRookieRes : public GameMsg_Base
{
public:
    GameMsg_S2C_CancelCeremonyRookieRes();
    virtual bool doEncode(CParamPool &IOBuff);

    /// 错误枚举-0表示成功
    unsigned char m_nErrorType;
};

/// S2C 同步仪式新人信息
class GameMsg_S2C_SyncCeremonyRookie : public GameMsg_Base
{
public:
    GameMsg_S2C_SyncCeremonyRookie();
    virtual bool doEncode(CParamPool &IOBuff);

    /// 玩家ID
    unsigned int m_nRoleId;
    /// 是否设为新人
    bool m_bIsRookie;
    CeremonyPlayerShowInfo m_PlayerInfo;
};

/// C2S 将玩家请出仪式房间
class GameMsg_C2S_KickCeremonyPlayer : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_KickCeremonyPlayer);
    GameMsg_C2S_KickCeremonyPlayer();
    virtual bool doDecode(CParamPool &IOBuff);

    unsigned int m_nRoleId;
};

/// S2C 将玩家请出仪式房间结果
class GameMsg_S2C_KickCeremonyPlayerRes : public GameMsg_Base
{
public:
    GameMsg_S2C_KickCeremonyPlayerRes();
    virtual bool doEncode(CParamPool &IOBuff);

    /// 错误枚举-0表示成功
    unsigned char m_nErrorType;
    /// 玩家ID
    unsigned int m_nRoleId;
};

/// C2S 保存自我介绍
class GameMsg_C2S_SaveCeremonyIntruduction : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_SaveCeremonyIntruduction);
    GameMsg_C2S_SaveCeremonyIntruduction();
    virtual bool doDecode(CParamPool &IOBuff);

    std::string m_strIntruduction;
};

// S2C保存自我介绍
class GameMsg_S2C_SaveCeremonyIntruductionRes : public GameMsg_Base
{
public:
    GameMsg_S2C_SaveCeremonyIntruductionRes();
    ~GameMsg_S2C_SaveCeremonyIntruductionRes();
    virtual bool doEncode(CParamPool &IOBuff);

    unsigned char m_nErrorType;
};


/// C2S 给新人点赞
class GameMsg_C2S_RequestPraiseRookie : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_RequestPraiseRookie);
    GameMsg_C2S_RequestPraiseRookie();
    virtual bool doDecode(CParamPool &IOBuff);

    /// 要赞的新人序号
    int m_nRookieIndex;
    /// 要赞的新人ID
    unsigned int m_nRoleID;
    /// 总共点赞数
    int m_nCount;
};

/// S2C 给新人点赞结果
class GameMsg_S2C_RequestPraiseRookieRes : public GameMsg_Base
{
public:
    GameMsg_S2C_RequestPraiseRookieRes();
    virtual bool doEncode(CParamPool &IOBuff);

    /// 错误枚举-0表示成功
    unsigned char m_nErrorType;
};

// ligh add.
/// s2c 新人获得赞的数量
class GameMsg_S2C_SyncPraiseInfo : public GameMsg_Base
{
public:
    GameMsg_S2C_SyncPraiseInfo();
    ~GameMsg_S2C_SyncPraiseInfo();

    virtual bool doEncode(CParamPool &IOBuff);

    unsigned int m_roleID;              // 被点赞的新人
    unsigned int m_praiseNum;           // role获得的总的赞数
};

// 自动开启的时候，房间开启失败，广播房间关闭
class GameMsg_S2C_CeremonyRoomStartFail : public GameMsg_Base
{
public:
    GameMsg_S2C_CeremonyRoomStartFail();
    ~GameMsg_S2C_CeremonyRoomStartFail();

    virtual bool doEncode(CParamPool &IOBuff);

    unsigned char m_nFailReason;
};

#endif // __GAMEMSG_CEREMONYROOMSC_H__
