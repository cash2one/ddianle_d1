/************************************************************************
 * HandbookComponent.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: luhaoting
 * Create time: 2015-9-15 10:22:20
 ***********************************************************************/

#ifndef __ENTITYHANDBOOK_COMPONENT_H__
#define __ENTITYHANDBOOK_COMPONENT_H__


#include "../EntityNetComponent/EntityNetComponent.h"
#include "../logic/EntityComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../logic/RoleComponentImpl.h"
#include "HandbookCfg.h"
#include "HandbookStruct.h"
#include <map>
#include "../../socket/GameMsg_Map.h"

/*
    CEntityHandbookComponent 
*/

class CEntityHandbookComponent : public CEntityComponent, public CommonSubscriber
{
public:
    CEntityHandbookComponent();
    virtual ~CEntityHandbookComponent();

    virtual void Start();
    virtual bool CreateFromDB(ROLEINFO_DB* pRoleInforDB);
    virtual bool PacketToCache(ROLEINFO_DB *pRoleInfoCache) const;
    virtual void OnLogin();
    virtual void OnLogout();    //下线存储DB
    virtual void RegComponentNetMsgMap();

public:
    //msg
    void GroupGolbalCostumeDataRet(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void GroupGolbalEffectDataRet(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void ClientGolbalCostumeData(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void CostumeCollectRet(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void CostumePersonalDataRet(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void EffectPersonalDataRet(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

    void SetCostumeState(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void SetEffectState(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

    void SetStateTask(unsigned int ID, unsigned int nType, int index ,bool isAdd);
public:
    //func
//     void LoadPersonalCostumeData(QueryBase &rQuery);
//     void LoadPersonalEffectData(QueryBase &rQuery);
    void InitPersonalData();

    void DBUpdatePersonalData(std::set<int> listUpdateDbKey, std::map<int , std::list<CHandbookPersonalInfo *> > *pDestMap);
    void DBInsertPersonalData(int nHandBookType , std::map<int , CHandbookPersonalInfo* > &map);

private:
    //func
    bool DecodeFromDB(OUT std::list<CHandbookPersonalInfo *>& listData, std::string &strData);
    void DeleteDbMap( std::map<int , std::list<CHandbookPersonalInfo *> > *m_DBMap);
    void DeleteNeedInsertMap( std::map<int, CHandbookPersonalInfo*> &map);

    int FindNotFullMapDBKey(std::map<int , std::list<CHandbookPersonalInfo *> > *Map);

    int UpdatePersonalInfo(CHandbookPersonalInfo* unit ,int nHandbookType ,int index ,bool isAdd);
    int SetStateTaskSubFunc(int ItemId 
        ,unsigned int nType
        ,int index ,bool isAdd
        ,std::map<int , CHandbookPersonalInfo*> &MapCostumeOrEffect 
        ,std::map<int , std::list<CHandbookPersonalInfo *> >* MapCostumeOrEffectList4DB
        ,std::map<int , CHandbookPersonalInfo* > &mapNeedInsert
        ,std::set<int> &UpdateID
        ,std::map<int, int> &mapIdDBId);

    int updateCollect(CHandbookPersonalInfo *Pc_unit, CHandbookUnit &Golbal_unit, bool isAdd);
    int updatePraise(CHandbookPersonalInfo *Pc_unit, CHandbookUnit &Golbal_unit, bool isAdd);
    int updateEnvy(CHandbookPersonalInfo *Pc_unit, CHandbookUnit &Golbal_unit, bool isAdd);
private:
    std::map<int, int> m_MapCostumeIDKey;   //map<CostumeId, DbKey>
    std::map<int, int> m_MapEffectIDKey;   //map<EffectId, DbKey>

    std::map<int , std::list<CHandbookPersonalInfo *> > *m_MapCostumeList4DB;   //map<DbKey,...>
    std::map<int , std::list<CHandbookPersonalInfo *> > *m_MapEffectList4DB;    //map<DbKey,...>

    //需要Insert集合
    std::map<int, CHandbookPersonalInfo *> m_MapNeedInsertCostumeList;    //map<CostumeId, ..>
    std::map<int, CHandbookPersonalInfo *> m_MapNeedInsertEffectList;    //map<EffectId, ..>

    std::map<int , CHandbookPersonalInfo*> m_MapCostume;        //map<CostumeId,...>
    std::map<int , CHandbookPersonalInfo*> m_MapEffect;         //map<EffectId,...>

    std::set<int> m_SetCollectCostume;          //服饰收藏的存储集合

    std::set<int> m_SetCostumeUpdateID;       //list<DbKey>
    std::set<int> m_SetEffectUpdateID;       //list<DbKey>

    //form groupsvr
    std::map<int , CHandbookUnit> m_GlobalCostumeData;
    std::map<int , CHandbookUnit> m_GlobalEffectData;

    CEntityNetComponent         *m_pNetComponent;
    CEntityAttributeComponent   *m_pAttrComponent;
    bool m_bIsOpen;
};

typedef CRoleComponentImpl < CEntityHandbookComponent,CGetRoleProcessorObj,CGetRoleProcessorObj > CRoleHandbook;

#endif //__ENTITYHANDBOOK_COMPONENT_H__

