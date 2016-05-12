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

    //ж������
    void UnMountHorse();
    //�ӱ�������ж��
    void UnMountFromOtherHorse();

    void MountSelfHorse(int HorseType);
    
    void MountOtherHorse(int OtherRoleID, int HorseType ,int HorseSeatIndex);

    /// <summary>
    /// ������ﵱǰ�Ƿ����
    /// <summary>
    bool CheckIsInTransForm();

    bool CheckIsInDevice();

    //�����ͬһ������
    bool CheckIsSameRoomPlayer(int nBeInvitedRoleId);
    
    bool CheckRoleAllowInvite(int nBeInvitedRoleId);

    bool CheckRoomAllowInvite();
    
    bool CheckAllowHorse();
    //��������Ƿ��������
    bool CheckAllowHorseMorePassager();
    //����ɹ�������ݴ���
    void BeInvitedHorseSucTask(CEntityHorseComponent* HorseOwner, CEntityHorseComponent* BeInviter);
    /// <summary>
    /// �Ƿ������Լ���������
    /// <summary>
    bool IsMountSelfHorse();
    /// <summary>
    /// �Ƿ���������
    /// <summary>
    bool IsMountHorse();

    void BroadcastHorse();
    /// <summary>
    /// �����뿪����
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

    int m_nCurHorseOwnerId;             //��ǰ���������
    int m_nCurHorseTypeId;              //��ǰ���������
    int m_nHorseSeatIndex;              //�������ϵ�λ��
    std::map<int, CHorseSeatInfo> m_MapPassengers;     //����˿�����  <��λ, ��λ����>

    bool m_bIsOpen;
    int  m_nRoleID;                     //�Լ�roleid
    unsigned short  m_nSenceId;         //�ͻ�����Ҫ������ת���ĳ���id
};

typedef CRoleComponentImpl<CEntityHorseComponent,CGetRoleProcessorObj,CGetRoleProcessorObj> CRoleHorse;


#endif
