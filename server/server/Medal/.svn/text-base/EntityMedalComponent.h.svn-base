/************************************************************************
 * MedalComponent.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: luhaoting
 * Create time: 2015-8-19 10:22:20
 ***********************************************************************/


#ifndef __ENTITYMEDAL_COMPONENT_H__
#define __ENTITYMEDAL_COMPONENT_H__

#include "../logic/EntityComponent.h"
#include "../logic/RoleComponentImpl.h"
#include "../Macros/Macros.h"
#include "../../datastructure/LogStructDef.h"

class CEntityNetComponent;
class CRoomComponent;
class CEntityCoupleComponent;
class CEntityPhotoRoomComponent;
class CAmuseRoomComponent;
class CEntityDanceGroupComponent;
class CRankComponent;

class MedalUnitDB;
class CMedalProgressCfg;
class CMedalCfgMgr;
class IMedalCondition;
class GameMsg_G2S_GetRankAllDataForMedal;


#define UPDATE_INTERVAL 300000 //ms

/**
* CEntityMedalComponent:
* 
*/
class CEntityMedalComponent : public CEntityComponent, public CommonSubscriber
{
private:
    class RankData;
public:
    CEntityMedalComponent();
    virtual ~CEntityMedalComponent();

public:
    virtual void Start();
    virtual bool CreateFromDB(ROLEINFO_DB* pRoleInforDB);
    virtual bool PacketToCache(ROLEINFO_DB *pRoleInfoCache) const;
    virtual void OnLogin();
    virtual void OnLogout();
    virtual void RegComponentNetMsgMap();
    virtual void OnNotify(IParam &param);
    virtual void OnUpdate(const unsigned long & nTimeElapsed);
public:
    //msg
    void OnGetMedalInfo(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnGetConditionInfo(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnGetMedalHandBook(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnGetMedalExchangeInfo(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnSetMedalShow(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnGetMedalExchange(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnGetMedalActivityInfo(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnGetMedalScore(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnGetMedalScoreDescribe(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnGetMedalConditionReward(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnGroupRankDataResultForMedal(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnGroupRankDataResult(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

public:
    void InitMedalConfig();

    void ChangeMedalScore(int nWearChange, int nCumulativeChange);

    MedalUnitDB* FindMedal(int nMedalID);
    IMedalCondition* FindCondition(int nConditionID);
    bool CheckMedalHaveGot(int nMedalID);

    bool ExchangeMedal(int nMedalID);
    bool SetMedalState(int nMedalID, int nMedalState,bool isNeedBoardcast = true);
    bool CmdAddMedal(int nMedalID);

    //void DeleteConditionFormTypeList(int nConditionId);

    void SyncAllDisplayMedalsForOther( CParamPool &IOBuff );

    void EncodePlayerActivityMedals( std::list<PlayerMedal> &rInfos );
    void EncodePlayerShowMedals( std::list<PlayerMedal> &rInfos );
    int EncodePlayerMedalScore();
    void BroadcastShowMedal( int nMedalID );
    void GetActivityMedals(std::list<int>   &listMedalIDs);

    void SetMedalActivited(const std::list<int> & listmedalid, CMedalLog::EMedalSource eMedalSource);

    void SetMedalFromUnderWayToActivated(int nMedalID);
    void SetMedalFromActivatedToShow(int nMedalID);
    void SetMedalFromActivatedToInvalid(int nMedalID);
    void setMedalFromShowToActiviated(int nMedalID);
    void SetMedalFormShowToInvalid(int nMedalID);
    void SetMedalFromInvalidToActivated(int nMedalID);
    bool IsOpen(){return m_bIsOpen;}
    void CheckPushMsgTask(IMedalCondition &Condition);
    int CalculateWearMedalScore(int nState);

    void LogMedal(int MedalID, CMedalLog::EMedalAction eAction, CMedalLog::EMedalSource eMedalSource = CMedalLog::EMedalUnKonw, int ConditionID = 0);
private:
    void OnLoginCheckPushMsg();
    void GetSortMedal(std::list<int> &listMedalID);
    void GetMedalActivityInfo();
    void RankConditionDo(IMedalCondition *medalcondit, const RankData &rankData);
    void getAllRankCondition(std::list<int > & result);

    void InsertMedalIDToStateTypeList(int nMedalID, EMedalState destState);
    void DelMedalIDFormStateTypeList(int nMedalID, EMedalState State);
    /// <summary>
    /// 将条件id 加入 map<条件关心类型，条件进度id> 内存数据中
    /// <summary>
    //void InsertConditionIDToTypeList(int nConditID, int nType);
    bool CheckIsVisibility(int nVisibility);

    void UpdateMedalToDB( const MedalUnitDB *pMedalUnitDB );
private:
    CEntityNetComponent         *m_pNetComponent;
    CEntityAttributeComponent   *m_pAttrComponent;
    CEntityItemComponent        *m_pItemComponent;
    CRoomComponent              *m_pRoomComponent;
    CEntityCoupleComponent      *m_pCoupleComponent;
    CEntityPhotoRoomComponent   *m_pPhotoRoomComponent;
    CAmuseRoomComponent         *m_pAmuseRoomComponent;
    CEntityDanceGroupComponent  *m_pDanceGroupComponent;
    CRankComponent              *m_pRankComponent;

private:
    std::map<int, MedalUnitDB*> m_mapAllMedal;               //map<勋章id，勋章数据>
    std::map<int, std::list<int > > m_mapStateTypeMedal;    //map<状态id，勋章id>

    std::map<int, IMedalCondition*> m_mapAllCondition;          //map<条件进度id， 条件类型类>
    std::map<int, std::list<int > > m_mapConditionType;         //map<条件进度类型type，条件进度id> 用来做类型检索

    bool m_bIsOpen;
    unsigned char m_nSceneID;

    int m_nMedalWearScore;
    int m_nMedalCumulativeScore;

    //用于推送的数据
    std::set<int> m_setNeedPushRankConditionMedalId;
    long m_nInterval;
    bool m_bIsRankOpen;

    class RankData
    {
    public:
        RankData();
    public:
        int m_RankMCoin;
        int m_RankExp;
        int m_RankHorn;
        int m_RankGroupHonor;
        int m_RankIntimacy;
        int m_RankMedalScore;
        int m_RankClothEffect;
        int m_RankEquipCount;
    };
};

typedef CRoleComponentImpl<CEntityMedalComponent,CGetRoleProcessorObj,CGetRoleProcessorObj> CRoleMedal;

#endif //__ENTITYMEDAL_COMPONENT_H__

