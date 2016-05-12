/*
*2011-11-9
*by wangshijun
*��entity�������
*2012-04-17
*by meijiuhua
*item mgr logic
*/
#ifndef __ENTITYITEM_COMPONENT_H__
#define __ENTITYITEM_COMPONENT_H__

#include "../logic/RoleComponentImpl.h"
#include "../logic/EntityComponent.h"
#include "../../datastructure/EquipItem.h"
#include "../../datastructure/datainfor/ItemType.h"
#include "../../datastructure/Macro_Define.h"
#include "../../datastructure/ImageData.h"
#include "ItemManager.h"
#include "ClothEffect.h"
#include "BagManager.h"
#include "EquipManager.h"

using namespace std;

class CEntityQuestComponent;
class CEntityNetComponent;
class CEntityGeneComponent;
class CEntityAttributeComponent;
class CEntityAchievementComponent;
class CRoomComponent;
class CEntityCoupleComponent;
class CEntityVIPComponent;
class CEntityPhotoRoomComponent;
class CAmuseRoomComponent;
class CEntityQuestNewComponent;
class ClothGroupItem;
class CEntityDanceGroupComponent;
class CRankComponent;

typedef std::pair<int, int> itemcountcolumn_t;
struct ClothTempInfo
{
    EItemColumn m_nColumn;
    int m_nIndex;
    bool m_bHaveEquiped;
    itemtype_t m_nItemType;
};

class CEntityItemComponent :
    public CEntityComponent
{
public:
    CEntityItemComponent(void);
    ~CEntityItemComponent(void);

public:
    virtual void Start();
    void RegComponentNetMsgMap();
    virtual void OnUpdate(const unsigned long & nTimeElapsed);
    void OnEvent(CComponentEvent& refEvent);
    bool CreateFromDB(ROLEINFO_DB* pRoleInforDB);
    virtual bool PacketToDB(ROLEINFO_DB*pRoleInforDB) const;
    virtual bool PacketToCache( ROLEINFO_DB* pRoleInforDB ) const;
    void PackItemForOther( CParamPool & IOBuff );
    virtual const char* SerializeName(){ return "Item"; };
    virtual void OnLogin();
    virtual void OnLogout();
    virtual void SendExtraStaticData();
    virtual void PacketBuff(std::list<BuffData>& listBuff) {}

public:
    void OnUseItem(GameMsg_Base & msg,CSlotPeer & SlotPeer);
    void OnBatchUseItem(GameMsg_Base & msg,CSlotPeer & SlotPeer);
    void OnRemoveItem(GameMsg_Base & msg,CSlotPeer & SlotPeer);
    void OnAddItemColumn(GameMsg_Base & msg,CSlotPeer & SlotPeer);
    void OnChangeEquipItem(GameMsg_Base & msg,CSlotPeer & SlotPeer);
    void OnActivateGift(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnGetFirstPaymentPacketDetail(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnQueryRechargeRewardEnable( GameMsg_Base &msg, CSlotPeer &slotPeer );		// �ͻ��� ѯ�� ��ֵ���� �Ƿ񿪷�
    void OnGetRechargePlanDetail( GameMsg_Base &msg, CSlotPeer &slotPeer );			// �ͻ������� ��ֵ��������
    void OnUpdateClothGroupName(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnGetCoolDown(GameMsg_Base &msg, CSlotPeer &slotPeer);

public:
    // ��ȡ�������ϵ��·���Ʒ����
    void GetEquipedCloth(std::list<itemtype_t> & listItemType);
    void GetAllPerpetualEquip(std::list<ItemEquip*> & listCloth);

    // ��ȡ������������
    void GetAllHorseTypeId(std::set<int> & setItemType);

    // Ϊ������д�Ľӿڣ��ͷ�������Ʒ����ȴʱ��
    void releaseAllCDItem();

	//  ��Ʒ�Ƿ��ܹ����
    int CanAddItem(itemtype_t nItemType, unsigned short nCount,int nValidTime,unsigned short & nStackNumber, bool bAllowUniqueEquip = true);

    //�Ƿ���һ����Ӷ����Ʒ�����κ�һ��������Ӷ��᷵��false
    int  CanAddAllItem(const list<CItem> &ItemList, bool bAllowUniqueEquip = true);

    // ����ӿڿ�����������κ����͵���Ʒ���ڲ��߼����Զ���鲢����������
    // nValidTime�ֶα�ʾ�����װ���Ļ�������װ������Ч��  0��ʾĬ�Ͼ�̬���ݱ����ã�>0��ʾ��Чʱ�䳤�ȣ�-1��ʾ����
    // bNoticeClient�ֶα�ʾ�Ƿ���֪ͨ�ͻ���
    // bAddUse�ֶα�ʾ�Ƿ�����Ͼ�����ʹ��
    // nDynamicBoxRelatedValue�ֶζ�̬��й���ֵ
    int AddItem(itemtype_t nItemType, unsigned short nCount, int nValidTime, EItemAction eItemAction, __int64 nSrcData, bool bNoticeClient, bool bUsed, int nDynamicBoxRelatedValue = 0);
    int AddUniqueItem(ItemConfig * pItemConfig, int nCount, int nValidTime, EItemAction eItemAction, __int64 nSrcData, bool bNoticeClient, bool bUsed);
    int AddOverlayItem(ItemConfig * pItemConfig, int nCount, EItemAction eItemAction, __int64 nSrcData, bool bNoticeClient, bool bUsed, int nDynamicBoxRelatedValue = 0);

    // ���������Ʒ
    bool AddItem(const std::list<CItem> & listItem, EItemAction eItemAction, __int64 nSrcData, bool bNoticeClient);

    // ʹ����Ʒ
    bool RemoveItemByType(itemtype_t nItemType, int nCount, EItemAction eItemAction);
	int UseItem(int nColumn, int nSlot);
	int UseConsumables(ItemBase * pItemBase);

    // ������(��������䡢��̬����)
    bool OpenPackage(itemid_t nItemID, itemtype_t nItemType, int nDynamicBoxRelatedValue);
    bool BatchOpenPackage(itemid_t nItemID, itemtype_t nItemType, int nDynamicBoxRelatedValue, int nCount, 
            std::list<CItem> & listItem, std::map<int, int> & mapCurrency);
    void FreshPackageResultToClient(itemtype_t nItemType, int nCount, std::list<CItem> & addItemList, std::map<int, int> & mapCurrency);

    // ˢ����Ʒ����
    void RefreshItemGene(itemtype_t nItemType);
    void RefreshItemGene(itemtype_t nItemType, int nCount, GeneInfo & gene);
    void FreshGeneResultToClient(const GeneInfo & gene, itemtype_t nItemType, int nCount);

	// ������Ʒ
	// bSearchInEquiped �Ƿ�����Ѿ�װ������Ʒ
	ItemBase * SearchItem(itemtype_t nItemType, bool bSearchInEquiped = false);

	// װ���Ƿ񴩴�����
	bool IsEquiped(itemtype_t nItemType);

    // ��ȡ��Ʒ������
    // bIncludeEquipedClothCapsule��ʾ�Ƿ������װ�������ϵķ���
    // bIncludeEquipedBadge��ʾ�Ƿ�������ɫ��װ������Ļ���
    unsigned int GetItemCount(itemtype_t nItemType, bool bIncludeEquipedClothCapsule, bool bIncludeEquipedBadge);

    // ��ȡ�������÷��ε�����
    // bIncludeBadge �Ƿ��������
    // bIncludeWeddingRing �Ƿ�������
    unsigned int GetAllPerpetualEquipCount(bool bIncludeBadge, bool bIncludeWeddingRing);
    unsigned int GetAllPerpetualEquipCountNoRing() const { return m_nPerpetualEquipCount; }

    // ��ȡ�������ù�Ч�·��Ĺ�Ч����
    unsigned int GetPerpetualEffectEquipScore() { return m_nPerpetualEffectScore; }
    unsigned int GetPerpetualEffectEquipScore(const ItemEquip * pItemEquip);

    // ��ȡ�·�������
    unsigned int GetClothAttr(itemtype_t nItemType, EClothAttrType eType);

    // �����
    void GetPacketItemInfo(unsigned short nRoleIndex, itemtype_t nPacketID, list<CItem> & listItem, std::map<int, int> & mapCurrency);
    void SimulateRandomBox(itemtype_t nBoxID, int nCount);

    // vip���ڲ���
    void SetExpiredVIPEquipment();

    // ɾ�����л����Ʒ
    void DeleteWeddingRing();

    // ��ȡ�·�����
    void GetCurEquipGene(std::list<GeneInfo*>& rEquipGene);

    // ��ȡ������
    int  GetCurWeddingRingGeneEffect();

    // ��ȡ�����������
    unsigned int GetPlayerWeddingRingItemType();

    // ��ȡ���װ����Ϣ
    void PackOnlinePlayerInfo(RoleImageOfflineData & Info);

    void PackEquipDbItem(std::vector<DBItem> &vecEquipDbItem, RoleRoomImageData::ClotheEffectMap &clotheEffectMap, RoleRoomImageData::SevenColorMap &sevenColorMap);

    void ResumItem();

    // ��ȡ��Ʒ
    ItemBase * GetItem(int nColumn, int nSlot);

    // �㲥��Ϣ��������
    void SendPlayerMsgToOthers(GameMsg_Base * pMsg);

	// �ж���Ʒ��/��װ��
	int CanEquip(ItemEquip * pEquip, CRoleVIPInfo * pVipInfo = NULL);
	int CanUnEquip(ItemEquip * pEquip);

	// ����װ��
    int OnEquip(ItemEquip * pEquip, bool bRefreshClotheGroup = false, CRoleVIPInfo * pVipInfo = NULL);

	// ����װ��
	int UnEquip(ItemEquip * pEquip);

    // ˢ����Ʒ
    void RefreshClientItem(int nColumnType, int nIndex, itemid_t nItemID);
	
////////////////////////////////  ���ʹ����Ʒ
private:
    // ���Ψһ��Ʒ
	void UpdateItemDueTime(ItemBase * pItemBase, int nAddTime, EItemAction eItemAction, bool bNoticeClient = true, bool bUsed = false);
	int AddNewItem(ItemConfig * pItemConfig, int nCount, int nDueTime, EItemAction eItemAction, bool bNoticeClient = true, bool bUsed = false, int nDynamicBoxRelatedValue = 0);
    void DeleteItem(ItemBase * pItemBase, EItemAction eItemAction);
    void UpdateItemCount(ItemBase * pItemBase, int nCount, EItemAction eItemAction, bool bNoticeClient = true, bool bUsed = false, bool bCheckUsetimes = false);

    // ����ʹ����Ʒ
    int BatchUseItem(int nColumnType, int nSelectSlot, itemtype_t nItemType, int nCount);
    void MergeItem(std::list<CItem> & listItem);
    
    // ʹ�õ�����Ʒ
    bool AddPacketItemToRole(itemtype_t nPacketID, unsigned int nCount = 1);
    bool AddPacthPacketItemToRole(unsigned short nRoleIndex, itemtype_t nPacketID, unsigned int nCount, 
                list<CItem> & listItem, std::map<int, int> & mapCurrency);
    bool RandomBoxItem(itemtype_t nBoxID, list<CItem>& rItemList);
    bool AddBoxItemToRole( itemtype_t nBoxID );
    bool AddPatchBoxItemToRole( itemtype_t nBoxID, int nCount, std::list<CItem> & itemlist );

    // ʹ�ö�̬����
    bool AddDynamicBoxItemToRole( __int64 itemID, int nRelatedValue, itemtype_t nBoxID, int nCount = 1 );
    bool AddPatchDynamicBoxItemToRole( __int64 itemID, int nRelatedValue, itemtype_t nBoxID, int nCount, list<CItem> & listItem );

    // ���ɶ�̬������Ʒ
    bool GenerateDynamicBoxItem( __int64 itemID, int nRelatedValue, itemtype_t nBoxID, list<CItem> &itemlist );

    // �����Ʒ�ķ�֧
    int CanAddUniqueItem(ItemConfig * pItemConfig, unsigned short nCount, bool bAllowUniqueEquip);
    int CanAddOverlayItem(ItemConfig * pItemConfig, unsigned short nCount);

	// �ж���Ʒ�ܹ�ʹ��
	int IsCanUsed(ItemBase * pItemBase, int nCount);
	int IsCanUsed(const std::list<itemid_t> & listItemID, int nCount);

    // ��ȡ��Ʒ�������Ŀ
    unsigned int GetColumnPutInMaxCount(int nColumn, itemtype_t nItemType, const std::list<itemid_t> & listItemID);
    int GetItemPutInNeedSlotCount(int nColumn, itemtype_t nItemType, int nCount, const std::list<itemid_t> & listItemID);

    // ��Ʒ���ڴ���
    void OnItemDueTime(ItemBase * pItemBase);

    void SaveItemToItemInfo(const ItemBase* pItem,DBItem& itemInfo);    

    // ��ȡ�������ù�Ч�·��Ĺ�Ч����
    unsigned int GetAllPerpetualEffectEquipScore();
    void UpdateEquipPerpetualEffectScore( unsigned int nNewScore, unsigned int nOldScore );
    void UpdatePerpetualEquipCount( unsigned int nChangeValue );

private:
    void OnGetActivateGiftCodeCB(QueryBase &query);
    void OnGetActivateInfoCB(QueryBase &query);
    void InitializeBag();
    void InitializeItem(const std::list<DBItem> & listDBItem);
    void InitializeRepeatItem();
    void InitializeRoleEffect(const std::set<unsigned int> & setRoleEffect, const std::map<unsigned short,std::map<unsigned char,std::string> > & handbookProgressData,
        const std::map<unsigned short, std::map<unsigned char, std::string> > & colorProgressData );
    void InitializeClothEffect(const std::map<itemid_t,unsigned int> & mapClothEffect);
    void Destroy();
    void ItemReadyToRun();

    //��Ʒ��ӵ��������ϵ�ʱ����á�
    void OnItemAddToRole(const ItemBase*pItem);

    //��Ʒ�������������ߵ�ʱ����á�
    void OnItemRemoveFromRole(const ItemBase*pItem);

    // ˢ���Լ���Ʒ��Ϣ
    void RefreshClientItem(const std::list<itemgrid_t> & listGrid);
    
	// log��־
    void _LogRoleItem(unsigned int nRoleID, EItemAction eItemAction, EItemColumn eItemColumn, __int64 nItemID, itemtype_t nItemType, unsigned short nOldCount,
        int nOldDuration, unsigned short nNewCount, int nNewDuration, __int64 nAdditionalValue);

    void _SendExtendItemBinData();
    void SendClientItemForOther(int nColumn, int nSlot, ItemBase * pItem);

    void _PackItemBrief( CParamPool & IOBuff );
    void _PackItemBaseBrief(ItemBase * pItemBase, int nColumn, CParamPool & IOBuff);


    void SendPlayerMsg(GameMsg_Base *pMsg);

private:
    // ����Ĭ���·�����
    ItemConfig * GetDefaultClothConfig(EItemClothType clothTyp);

    // ��ȡĬ���·�
    ItemEquip * GetDefaultCloth(EItemClothType clothTyp);

    // ���Ĭ�Ϸ���
    void AddDefaultColthToPlayer();

    // ����Ĭ�Ϸ���
	void CreateDefaultColth(EItemClothType clothTyp);

    // ��ȡĬ�Ϸ�������װ����λ
    int GetDefaultClothPos(EItemClothType clothType);

    // ��ȡĬ��װ�����ڱ�����Ĭ��λ��
    int GetDefaultClothBagSlot(EItemClothType clothType);

    // ���Ĭ�ϲ�λ�Ƿ��д�����û�д�Ĭ���·�
    void CheckDefaultDressPos(EItemClothType pos);

    // �Ƿ���Ĭ�Ϸ���
    bool IsDefaultCloth(itemid_t nItemID) const;

    void SerializeDefaultClothToBuff( std::string &rBuff ) const;
public:
    // ��ʼ�ռ������·��仯
    void BeginCollectEquipChange();

    // �����ռ������·��仯
    void EndCollectEquipChange();

    // ��ʼ���·�����
    void InitializeClothGroup(const std::list<RoleClothGroup> & group, const CRoleVIPInfo & vip);

    // ѡ���·�����
    void OnSelectClothGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);
    bool IsValidClothGroup(const RoleClothGroup & group, std::string & strError);

    // �����·�����
    void OnUpdateClothGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);

    //ˢ�´��䣬������ڣ��̳ǹ���ʹ�ã����µĴ�����
    void _RefreshClothGroup( ItemEquip * pEquip );

    // �����ʼ�������·�
    void _InitClothGroup(unsigned char nGroupID);

    // �����ʼ��Ĭ���·�
    void _InitGroupClothByDefault(unsigned char nGroupID);

    //���������װ��������װ���ľͱ��������滻��Ĭ��װ����װ�����滻��Ϣ�����mapItemsDress
	void _CheckClothGroup(std::map<unsigned char, itemtype_t> & mapEquip, std::list<ItemEquip*> & listOnEquip, 
        std::map<itemtype_t, stringw>& mapItemError, CRoleVIPInfo * pVipInfo = NULL);

public:
    // ��ʼ����������
    void InitializeEquipConflict();

    //װ���걸�Լ�飬��������λ�ó�ͻ�����Դ���
    void EquipIntegrityCheck();

    //��û���װ��λ��
    void _GetConflictEquipPos(EItemClothType pos, std::set<EItemClothType>& setConflictEquipPos);
    bool IsConflictEquiped(EItemClothType pos);

    //װ������װ��ǰ��λ�ó�ͻ�Լ�飬�ѻ���λ�õ�װ����ж��
    void EquipConflictCheck(ItemEquip * pItem, bool bOnEquipDefault = false);

public:
    // ��ʼ����Ʒ��ȴʱ��
    void InitializeCoolDown(const std::list<CoolDownData> & listCooling);

    // ��Ʒ�Ƿ�����ȴ��
    bool IsItemInCoolDown(itemtype_t nItemType);

    // ������Ʒ��ȴ
    void UpdateItemCoolDown(unsigned short nItemCoolDownTag,int nDuration);
    void UpdateItemCoolDown(itemtype_t nItemType);

public:
    // ���ϱ���װ��
    void EquipTransformItem(itemtype_t nItemType, int nDueTime);

    //�ѵ�����װ��
    void UnEquipTransformItem(ItemEquip * pEquip);

    // ����
    void ApplyTransform(const std::map<int, itemtype_t>& mapClothPos, int nMatuneTime);

    // ж������װ��
    void UnApplyTransform();

    // �Ƿ��ڱ�����
    bool IsInTransForm();

public:
    // �滻�·���Ч(�����״�װ��)   
    void OnReplaceClothEffect(GameMsg_Base &msg, CSlotPeer &slotPeer);
    int ReplaceClothEffect(EItemColumn eClothColumn, int nClothSlot, unsigned int nEffectID);

    // ��Ч����
    void OnUpgradeEffect(GameMsg_Base &msg, CSlotPeer &slotPeer);
    int UpgradeEffect(EItemColumn eItemColumn, int nItemSlot, itemtype_t & nItemType, unsigned int & nUpEffectID);

    // ��ӽ�ɫ��Ч
    void AddRoleEffect(unsigned int nEffectID, itemtype_t nRelatedItemType);
    void AddRoleEffect(itemtype_t nItemType);

    // ����·���Ч
    void AddClothEffect(unsigned int nEffectID, ItemEquip * pCloth, bool bRefreshToClient = true);

    // GM ����·���Ч
    void AddClothEffectByGM(unsigned int nEffectID);

    // �㲥��Ч�仯
    void RefreshEffectChangeToOthers(itemtype_t nItemType, unsigned int nEffectID);

    // log��־
    void LogClothEffect(ClothEffectLog::EClothEffectAction eAction, itemid_t nItemID, unsigned int nOldEffectID, unsigned int nNewEffectID);

    // ����Ƿ����б�����ЧID
    void CheckEffectListExist(const std::list<unsigned int> & listCheck, std::list<unsigned int> & listReturn);

    // ��ȡͼ��100%�ĸ���
    unsigned int GetHandbookFinishCount();

public:
    // �ֿ⡢����֮���ƶ���Ʒ
    void OnMoveItem(GameMsg_Base &msg, CSlotPeer &slotPeer);
    int MoveItem(const std::list<itemgrid_t> & listToGrid, std::list<itemgrid_t> & listAddGrid);

    // ��鱳���Ƿ��㹻
    bool IsBagEnoughSlot(const std::map<itemtype_t, itemcountcolumn_t> & mapSelectItem);
    int GetMoveToBagColumn(ItemBase * pItemBase);

    // ��ͨ�ֿ����
    void OnAddStorageCapacity(GameMsg_Base &msg, CSlotPeer &slotPeer);
    int AddStorageCapacity();

    // ����ֿ������
    int CalcStorageCapacity(const StorageCapacityRecord & record, int nVipLevel);
    void UpdateStorageCapacity(int nVipLevel);

    // ����/���²ֿ�����
    void InsertStorageCapacityToDB();
    void UpdateStorageCapacityToDB();

private:
	void InsertItemToDB(const ItemBase * pItemBase, int nColumn, int nSlot);
    void UpdateItemToDB(const ItemBase * pItemBase);
    void UpdateItemCountToDB(const ItemBase * pItem);
    void DeleteItemToDB(const ItemBase * pItem);
    void InsertClothEffectToDB(const ItemEquip * pCloth);
    void UpdateClothEffectToDB(const ItemEquip * pCloth);

private:
    std::list<itemid_t>                   m_listDefaultClothID;                         // Ĭ���·�ID
    bool                                  m_bGotCoolDown;                               // �Ƿ��ȡ����ȴ�б�
    unsigned long                         m_ItemUpdateDelay;                            // ��Ʒ�������
    std::map<unsigned char, RoleClothGroup> m_mapClothGroup;                            // ���δ��� key-����ID��value-������ݽṹ
    static std::map<EItemClothType, std::set<EItemClothType> > m_mapConflictEquipPos;   // װ��λ�û���ӳ���
    ClothEffect                           m_ClothEffect;                                // �·���Ч
    time_t                                m_tUpdateClothGroupName;
    unsigned int                          m_nLastSendGiftCodeTime;
	ItemManager                           m_ItemManager;
    BagManager                            m_BagManager;
    EquipManager                          m_EquipManager;
    StorageCapacityRecord                 m_StorageCapacityRecord;
    bool                                  m_bCollectingRefreshChange;
    std::map<itemgrid_t, itemid_t>        m_mapCollectRefresh;
    unsigned int                          m_nPerpetualEffectScore;
    unsigned int                          m_nPerpetualEquipCount;
    bool                                  m_bPerpetualEffectScoreChange;
    bool                                  m_bPerpetualEquipCountChange;

private:
    CEntityAttributeComponent			* m_pRoleAttr;
    CEntityQuestComponent			 	* m_pRoleQuest;
    CEntityNetComponent				    * m_pNetCpt;
    CEntityAchievementComponent	        * m_pRoleAch;
    CEntityGeneComponent                * m_pRoleGene;
    CRoomComponent						* m_pRoleRoom;
    CEntityCoupleComponent				* m_pRoleCouple;
    CEntityVIPComponent					* m_pRoleVIP;
    CEntityPhotoRoomComponent			* m_pRolePhotoRoom;
    CAmuseRoomComponent					* m_pRoleAmuseRoom;
    CEntityQuestNewComponent			* m_pRoleQuestNew;
    CEntityDanceGroupComponent          * m_pRoleDanceGroup;
    CRankComponent                      * m_pRankComponent;
};

typedef CRoleComponentImpl<CEntityItemComponent,CGetRoleProcessorObj,CGetRoleProcessorObj> CRoleItem;

#endif

