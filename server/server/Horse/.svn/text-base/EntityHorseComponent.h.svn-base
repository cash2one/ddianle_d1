/************************************************************************
 * HorseComponent.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: luhaoting
 * Create time: 2015-11-10 10:22:20
 ***********************************************************************/


#ifndef __ENTITYHORSE_COMPONENT_H__
#define __ENTITYHORSE_COMPONENT_H__

#include "../item/EntityItemComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../Couple/WeddingRoom.h"
#include "../room/RoomComponent.h"
#include "../room/Room.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../AmuseRoom/AmuseRoomComponent.h"
#include "../AmuseRoom/AmuseRoom.h"
#include "../CeremonyRoom/ceremonyRoom.h"
#include "../Macros/Macros.h"
#include "./Msg/GameMsg_Horse.h"

class CHorseSeatInfo
{
public:
    CHorseSeatInfo(){}
    ~CHorseSeatInfo(){}

    int m_nRoleID;
    int m_nSeatIndex;
};

class CEntityHorseComponent : public CEntityComponent, public CommonSubscriber
{
public:
    CEntityHorseComponent();
    virtual ~CEntityHorseComponent();

public:
    virtual void Start();
    virtual bool CreateFromDB(ROLEINFO_DB* pRoleInforDB);
    virtual void OnLogin();
    virtual void OnLogout();

    virtual void RegComponentNetMsgMap();

    virtual void OnNotify(IParam &param);
public:
    //func

    //卸下坐骑
    void UnMountHorse();
    //从别人坐骑卸下
    void UnMountFromOtherHorse();

    void MountSelfHorse(int HorseType);
    
    void MountOtherHorse(int OtherRoleID, int HorseType ,int HorseSeatIndex);

    /// <summary>
    /// 检测人物当前是否变身
    /// <summary>
    bool CheckIsInTransForm();

    bool CheckIsInDevice();

    //检测在同一个房间
    bool CheckIsSameRoomPlayer(int nBeInvitedRoleId);
    
    bool CheckRoleAllowInvite(int nBeInvitedRoleId);

    bool CheckRoomAllowInvite();
    
    bool CheckAllowHorse();
    //检测坐骑是否可以坐人
    bool CheckAllowHorseMorePassager();
    //邀请成功后的数据处理
    void BeInvitedHorseSucTask(CEntityHorseComponent* HorseOwner, CEntityHorseComponent* BeInviter);
    /// <summary>
    /// 是否坐在自己的坐骑上
    /// <summary>
    bool IsMountSelfHorse();
    /// <summary>
    /// 是否在坐骑上
    /// <summary>
    bool IsMountHorse();

    void BroadcastHorse();
    /// <summary>
    /// 坐骑离开房间
    /// <summary>
    void LeaveRoomHorseTask();

    void PassagerLeave(int nSeatIndex);

public: 
    //msg
    void OnMountHorse(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnUnMountHorse(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnInvitePlayer(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnRecvInviteRet(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void SyncAllDisplayHorse2Other( CParamPool &IOBuff );

    void GetHorseInfo(int &nCurHorseTypeId, int &nCurHorseOwnerId, int &nHorseSeatIndex);

private:
    bool AddNewPassager(CEntityHorseComponent* other);
private:
    CEntityItemComponent        *m_pItemComponent;
    CRoomComponent              *m_pRoomComponent;
    CEntityNetComponent         *m_pNetComponent;
    CEntityCoupleComponent      *m_pCoupleComponent;
    CAmuseRoomComponent         *m_pAmuseRoomComponent;
    CEntityDanceGroupComponent  *m_pDanceGroupComponent;

    int m_nCurHorseOwnerId;             //当前坐骑的主人
    int m_nCurHorseTypeId;              //当前坐骑的类型
    int m_nHorseSeatIndex;              //在坐骑上的位置
    std::map<int, CHorseSeatInfo> m_MapPassengers;     //坐骑乘客数据  <座位, 座位数据>

    bool m_bIsOpen;
    int  m_nRoleID;                     //自己roleid
    unsigned short  m_nSenceId;         //客户端需要服务器转发的场景id
};

typedef CRoleComponentImpl<CEntityHorseComponent,CGetRoleProcessorObj,CGetRoleProcessorObj> CRoleHorse;


#endif
