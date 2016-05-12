/*
 * RankComponent.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-2-5 9:51:22
 */

#ifndef __RANKCOMPONENT_H__
#define __RANKCOMPONENT_H__

#include "RankDefine.h"

#include "../logic/EntityComponent.h"
#include "../logic/RoleComponentImpl.h"

class CEntityNetComponent;
class CEntityCoupleComponent;
class CEntityAttributeComponent;
class CEntityDanceGroupComponent;
class CEntityItemComponent;
class CEntityMedalComponent;

/**
* CRankComponent:
* 
*/

class CRankComponent : public CEntityComponent
{
public:
    CRankComponent();
    virtual ~CRankComponent();

public:
    virtual void Start();
    virtual void OnEvent(CComponentEvent &refEvent);
    virtual void OnLogin();
    virtual void OnLogout();
    virtual bool CreateFromDB(ROLEINFO_DB *pRoleInforDB);
    virtual bool PacketToDB(ROLEINFO_DB *pRoleInforDB) const;
    virtual void OnUpdate(const unsigned long &nTimeElapsed);
    virtual void SendExtraStaticData() {}
    virtual void PacketBuff(std::list<BuffData> &listBuff) {}
    virtual void SerializeComponent(CParamPool &IOBuff) {}
    virtual const char* SerializeName() { return "Rank"; }

public:
    void RegComponentNetMsgMap();

    void SubmitRankValue(ERankDataType eDataType, int nValue);

private:
    // ��ȡ���а�����б�
    void OnGetRankHall(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ��ȡ���а�����
    void OnGetRankDatas(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ��ȡ������Ϣ
    void OnGetRewardRule( GameMsg_Base &msg, CSlotPeer &slotPeer );

    // ��ȡ���а�����б���
    void OnGetRankHallResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ��ȡ���а����ݽ��
    void OnGetRankDatasResult(GameMsg_Base &msg, CSlotPeer &slotPeer);

    // ��ȡ��ɫ���а�����
    void OnQueryRoleData( QueryBase &rQuery );

private:
    bool IsGetHallTimeOK();
    bool IsGetDatasTimeOK();

    unsigned int CalcCurrentStartTime() const;
    unsigned int CalcSaveDataTime(time_t nNow) const;

    bool GetKeyByDataType(unsigned short nDataType, unsigned int &rKey, bool bFromClient) const;

    void SaveUnsaveData(time_t nNow);
    void SaveSubmitData( unsigned short nDataType, unsigned int nKey, int nValue );

    void LogRoleVisitRank(unsigned short eAction, unsigned char eDetailType, unsigned short nMyRank);

    // ��ͻ��˷�����Ϣ
    void SendPlayerMsg(GameMsg_Base &msg);
    // ��Group Server������Ϣ
    void SendGroupMsg(GameMsg_Base &msg);

    void UpdateRankData( unsigned short nDataType, unsigned int nValue, unsigned int nLoadStartTime );

    void ForceSubmit( ERankDataType eDataType, int nValue );

private:
    CEntityNetComponent         *m_pNetComponent;
    CEntityAttributeComponent   *m_pAttrComponent;
    CEntityCoupleComponent      *m_pCoupleComponent;
    CEntityDanceGroupComponent  *m_pDanceGroupComponent;
    CEntityItemComponent        *m_pItemComponent;
    CEntityMedalComponent       *m_pMedalComponent;

private:
    std::map<unsigned short, unsigned int>  m_mapMyRankData; // ��ɫ���а����� type -> data
    std::map<unsigned short, int>           m_mapUnsaveData; // ��ɫδ�������� type -> data

    unsigned int m_nLastGetHallTime;
    unsigned int m_nLastGetDatasTime;
    unsigned int m_nLastSaveDataTime;

    int m_nLastHour;

    std::set<unsigned char> m_setLogDetailType; // logged detail type
};

typedef CRoleComponentImpl<CRankComponent, CGetRoleProcessorObj, CGetRoleProcessorObj> CRoleRank;

#endif // __RANKCOMPONENT_H__

