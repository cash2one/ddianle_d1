/************************************************************************
 * GameMsg_Horse.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: luhaoting
 * Create time: 2015-11-10 10:22:20
 ***********************************************************************/

#ifndef __GAMEMSG_HORSE_H__
#define __GAMEMSG_HORSE_H__

#include "../../../socket/GameMsg_Base.h"
#include "../../share/CommonMsgDef.h"

enum eMSG_HORSE
{
    MSG_HORSE_BEGIN = MSG_TYPE_HORSE,

    MSG_C2S_MountHorse,                //请求使用座驾
    MSG_S2C_MountHorseRet,             //使用座驾结果

    MSG_C2S_UnmountHorse,           //请求取消座驾
    MSG_S2C_UnmountHorseRet,            //取消座驾结果

//    MSG_S2C_SYNC_HorseData,         //同步坐骑数据

    MSG_C2S_SendHorseInvite,        //发送邀请消息
    MSG_S2C_SendHorseInvuteRet,     //发送邀请消息回复

    MSG_S2C_RecvHorseInvite,        //接收到邀请消息
    MSG_C2S_RecvHorseInviteRet,     //接收到邀请消息回复
    MSG_S2C_RecvHorseInviteRetRes,  //回复邀请的回复
};

enum EHorseMsgRet
{
    eHorseSuc,              //成功
    eHorseItemTimeOut,      //坐骑道具失效
    eHorseInTransForm,      //有变身药水
    eHorseInDevice,         //在设施上面
    eHorseNotAllowMount,    //坐骑不允许装备
    eHorseInviteAccept,     //接收邀请
    eHorseInviteRefuse,     //拒绝邀请
    eHorseInviteNotAllow,   //不允许邀请
    eHorseInviteLegal,      //邀请非法
    eHorseInviteLeave,      //邀请发起者不在房间
    eHorseBeInviteLeave,    //被邀请者不在房间
    eInviteHorseFull,       //坐骑已经满员
    eInviteHorseChange,     //发起者坐骑已经变更
    eHorseUnknow,           //未知
    eHorseInviteHorseing,   //被邀请者坐着坐骑
};

class GameMsg_C2S_MountHorse: public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_MountHorse);

    GameMsg_C2S_MountHorse();
    ~GameMsg_C2S_MountHorse();

    virtual bool doDecode( CParamPool &IOBuff );

public:
    unsigned int m_nHorseTypeID;
    unsigned short m_nSceneId;
};


class GameMsg_S2C_MountHorseRet : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_MountHorseRet);

    GameMsg_S2C_MountHorseRet();
    ~GameMsg_S2C_MountHorseRet();

    virtual bool doEncode( CParamPool &IOBuff );

public:
    unsigned char m_byFlag;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_UnmountHorse : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_UnmountHorse);

    GameMsg_C2S_UnmountHorse():GameMsg_Base(MSG_C2S_UnmountHorse){};
    ~GameMsg_C2S_UnmountHorse(){};

    virtual bool doDecode( CParamPool &IOBuff )
    {
        m_nSceneId = IOBuff.GetUShort();
        return true;
    }
    unsigned short m_nSceneId;
};

class GameMsg_S2C_UnmountHorseRet : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_UnmountHorseRet);

    GameMsg_S2C_UnmountHorseRet();
    ~GameMsg_S2C_UnmountHorseRet();

    virtual bool doEncode( CParamPool &IOBuff );

public:
    unsigned char m_byFlag;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_SendHorseInvite : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_SendHorseInvite);

    GameMsg_C2S_SendHorseInvite();
    ~GameMsg_C2S_SendHorseInvite();

    virtual bool doDecode( CParamPool &IOBuff );

public:
    std::list<unsigned int> m_ListInviteID;
    unsigned short m_nSceneId;
};

class GameMsg_S2C_SendHorseInviteRet : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_SendHorseInviteRet);

    GameMsg_S2C_SendHorseInviteRet();
    ~GameMsg_S2C_SendHorseInviteRet();

    virtual bool doEncode( CParamPool &IOBuff );

public:
    unsigned char m_byFlag;
};

class GameMsg_S2C_RecvHorseInvite : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_RecvHorseInvite);

    GameMsg_S2C_RecvHorseInvite();
    ~GameMsg_S2C_RecvHorseInvite();

    virtual bool doEncode( CParamPool &IOBuff );

public:
    unsigned int m_nInviteSendRoleId;
    unsigned int m_nHorseType;
    std::string m_strInviterName;
    unsigned short m_nSceneId;
};

class GameMsg_C2S_RecvHorseInviteRet : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_RecvHorseInviteRet);

    GameMsg_C2S_RecvHorseInviteRet();
    ~GameMsg_C2S_RecvHorseInviteRet();

    virtual bool doDecode( CParamPool &IOBuff );

public:

    bool m_bIsAccept;
    unsigned int m_nHorseType;//用于验证是否和主人坐骑类型一致
    unsigned int m_nInviteSendRoleId;
    unsigned short m_nSenceId;
};


class GameMsg_S2C_RecvHorseInviteRetRes : public GameMsg_Base
{
public:
    
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_RecvHorseInviteRetRes);

    GameMsg_S2C_RecvHorseInviteRetRes();
    ~GameMsg_S2C_RecvHorseInviteRetRes();

    virtual bool doEncode( CParamPool &IOBuff );

public:
    unsigned short m_nFlag;
};


#endif
