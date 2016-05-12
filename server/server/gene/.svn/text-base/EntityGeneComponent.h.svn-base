#ifndef __ENTITYGENE_COMPONENT_H__
#define __ENTITYGENE_COMPONENT_H__

#include "../logic/RoleComponentImpl.h"
#include "../logic/EntityComponent.h"
#include "../../datastructure/ImageData.h"
#include <list>
#include <map>

class GeneInfo;
class CEntityNetComponent;
class CEntityItemComponent;
class CEntityAttributeComponent;
class CRoomComponent;
class CEntityCoupleComponent;
class CEntityPhotoRoomComponent;
class CAmuseRoomComponent;
class CEntityDanceGroupComponent;

typedef std::map<unsigned short, GeneData*> GeneDataMap;

enum EGeneID
{
    EGeneID_AddExpRate = 1001,
    EGeneID_AddExp = 1002,
    EGeneID_AddMoneyRate = 1101,
    EGeneID_AddMoney = 1102,
    EGeneID_AddSuper = 1201,
    EGeneID_AddVipExp = 1301,
    EGeneID_AddIntimacy = 1401,
    EGeneID_TransForm = 1501,
    EGeneID_AddDanceGroupContribution = 2101,
};


class CEntityGeneComponent :
	public CEntityComponent
{
public:
	CEntityGeneComponent(void);
	~CEntityGeneComponent(void);

public:
	virtual void Start();
	virtual void RegComponentNetMsgMap() {}

	virtual bool CreateFromDB(ROLEINFO_DB* pRoleDBInfo);
	virtual bool PacketToDB(ROLEINFO_DB* pRoleDBInfo) const;
    virtual bool PacketToCache( ROLEINFO_DB* pRoleInforDB ) const;

	virtual void SerializeComponent(CParamPool &IOBuff);
	virtual const char* SerializeName() { return "Gene"; }

	virtual void OnEvent(CComponentEvent& rEvent);

	virtual void OnLogin();
	virtual void OnLogout();
	virtual void SendExtraStaticData(){};
	virtual void PacketBuff(std::list<BuffData>& listBuff);

	virtual void OnUpdate(const unsigned long & nTimeElapsed);

	GeneData* FindGene(unsigned short nGeneID);


	bool AddGene(unsigned short nGeneID, int nParam1, int nParam2, const char *szParam, int nTargetIndex = 0);
	bool AddGene(GeneInfo *pGeneInfo, int nTargetIndex = 0);
	void DelGene(unsigned short nGeneID);
	void ResetGene(unsigned short nGeneID, int nParam2);

	void RefreshEquipGene(std::list<GeneInfo*>& rNewGeneList, int nTargetIndex = 0);
    void _RefreshEquipGene(const std::map<unsigned short, GeneData> & mapGeneData);

    void PackGeneForOther(CParamPool& IOBuff);
    void PackGeneAll(RoleRoomImageData::GeneDataMap &geneMap);

	void SendPlayerMsg(GameMsg_Base *pMsg);

    // 合并基因
    void MergeGene(GeneInfo & tempGene, int nCount);

protected:
	GENE_TYPE _GetGeneType(unsigned short nGeneID);
	bool _IsDurationGeneById(unsigned short nGeneID);
	bool _IsDurationGeneByType(int eType);
	bool _AddGeneAction(GeneData& rGeneData, int nTargetIndex = 0);
	void _DelGeneAction(const GeneData& rGeneData);

	void _HandleGeneDB(QUERY_TYPE eQueryType, const GeneData& rGeneData);

	void _PackGeneBrief(CParamPool& IOBuff);

	void _NotifyAddGene(GeneData& rGeneData);
	void _NotifyDelGene(unsigned short nGeneID);
	void _NotifyResetGene(unsigned short nGeneID, int nParam2);

protected:
	GeneDataMap m_AllGene;

	CEntityAttributeComponent*  m_pAttrCOM;
	CEntityNetComponent*  m_pNetCOM;
	CEntityItemComponent*  m_pItemCOM;
	CRoomComponent* m_pStageRoomCOM;
	CEntityCoupleComponent* m_pCoupleCOM;
	CEntityPhotoRoomComponent* m_pPhotoRoomCOM;
	CAmuseRoomComponent* m_pAmuseRoomCOM;
    CEntityDanceGroupComponent* m_pRoleDanceGroup;
};

typedef CRoleComponentImpl<CEntityGeneComponent,CGetRoleProcessorObj,CGetRoleProcessorObj> CRoleGene;

#endif

//end file



