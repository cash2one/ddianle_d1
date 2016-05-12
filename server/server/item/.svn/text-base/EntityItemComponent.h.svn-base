/*
*2011-11-9
*by wangshijun
*把entity组件化。
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
    void OnQueryRechargeRewardEnable( GameMsg_Base &msg, CSlotPeer &slotPeer );		// 客户端 询问 充值奖励 是否开放
    void OnGetRechargePlanDetail( GameMsg_Base &msg, CSlotPeer &slotPeer );			// 客户端请求 充值奖励规则
    void OnUpdateClothGroupName(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnGetCoolDown(GameMsg_Base &msg, CSlotPeer &slotPeer);

public:
    // 获取传在身上的衣服物品类型
    void GetEquipedCloth(std::list<itemtype_t> & listItemType);
    void GetAllPerpetualEquip(std::list<ItemEquip*> & listCloth);

    // 获取所有坐骑类型
    void GetAllHorseTypeId(std::set<int> & setItemType);

    // 为命令行写的接口，释放所有物品的冷却时间
    void releaseAllCDItem();

	//  物品是否能够添加
    int CanAddItem(itemtype_t nItemType, unsigned short nCount,int nValidTime,unsigned short & nStackNumber, bool bAllowUniqueEquip = true);

    //是否能一次添加多个物品，有任何一个不能添加都会返回false
    int  CanAddAllItem(const list<CItem> &ItemList, bool bAllowUniqueEquip = true);

    // 这个接口可以随意添加任何类型的物品，内部逻辑会自动检查并处理各种情况
    // nValidTime字段表示如果是装备的话，就是装备的有效期  0表示默认静态数据表配置，>0表示有效时间长度，-1表示永久
    // bNoticeClient字段表示是否是通知客户端
    // bAddUse字段表示是否添加上就立即使用
    // nDynamicBoxRelatedValue字段动态礼盒关联值
    int AddItem(itemtype_t nItemType, unsigned short nCount, int nValidTime, EItemAction eItemAction, __int64 nSrcData, bool bNoticeClient, bool bUsed, int nDynamicBoxRelatedValue = 0);
    int AddUniqueItem(ItemConfig * pItemConfig, int nCount, int nValidTime, EItemAction eItemAction, __int64 nSrcData, bool bNoticeClient, bool bUsed);
    int AddOverlayItem(ItemConfig * pItemConfig, int nCount, EItemAction eItemAction, __int64 nSrcData, bool bNoticeClient, bool bUsed, int nDynamicBoxRelatedValue = 0);

    // 批量添加物品
    bool AddItem(const std::list<CItem> & listItem, EItemAction eItemAction, __int64 nSrcData, bool bNoticeClient);

    // 使用物品
    bool RemoveItemByType(itemtype_t nItemType, int nCount, EItemAction eItemAction);
	int UseItem(int nColumn, int nSlot);
	int UseConsumables(ItemBase * pItemBase);

    // 开包裹(礼包、宝箱、动态宝箱)
    bool OpenPackage(itemid_t nItemID, itemtype_t nItemType, int nDynamicBoxRelatedValue);
    bool BatchOpenPackage(itemid_t nItemID, itemtype_t nItemType, int nDynamicBoxRelatedValue, int nCount, 
            std::list<CItem> & listItem, std::map<int, int> & mapCurrency);
    void FreshPackageResultToClient(itemtype_t nItemType, int nCount, std::list<CItem> & addItemList, std::map<int, int> & mapCurrency);

    // 刷新物品基因
    void RefreshItemGene(itemtype_t nItemType);
    void RefreshItemGene(itemtype_t nItemType, int nCount, GeneInfo & gene);
    void FreshGeneResultToClient(const GeneInfo & gene, itemtype_t nItemType, int nCount);

	// 查找物品
	// bSearchInEquiped 是否查找已经装备的物品
	ItemBase * SearchItem(itemtype_t nItemType, bool bSearchInEquiped = false);

	// 装备是否穿戴上了
	bool IsEquiped(itemtype_t nItemType);

    // 获取物品的数量
    // bIncludeEquipedClothCapsule表示是否包含已装备在身上的服饰
    // bIncludeEquipedBadge表示是否搜索角色已装备在身的徽章
    unsigned int GetItemCount(itemtype_t nItemType, bool bIncludeEquipedClothCapsule, bool bIncludeEquipedBadge);

    // 获取所有永久服饰的数量
    // bIncludeBadge 是否包含徽章
    // bIncludeWeddingRing 是否包含婚戒
    unsigned int GetAllPerpetualEquipCount(bool bIncludeBadge, bool bIncludeWeddingRing);
    unsigned int GetAllPerpetualEquipCountNoRing() const { return m_nPerpetualEquipCount; }

    // 获取所有永久光效衣服的光效积分
    unsigned int GetPerpetualEffectEquipScore() { return m_nPerpetualEffectScore; }
    unsigned int GetPerpetualEffectEquipScore(const ItemEquip * pItemEquip);

    // 获取衣服的属性
    unsigned int GetClothAttr(itemtype_t nItemType, EClothAttrType eType);

    // 开礼包
    void GetPacketItemInfo(unsigned short nRoleIndex, itemtype_t nPacketID, list<CItem> & listItem, std::map<int, int> & mapCurrency);
    void SimulateRandomBox(itemtype_t nBoxID, int nCount);

    // vip过期操作
    void SetExpiredVIPEquipment();

    // 删除所有婚戒物品
    void DeleteWeddingRing();

    // 获取衣服基因
    void GetCurEquipGene(std::list<GeneInfo*>& rEquipGene);

    // 获取婚戒基因
    int  GetCurWeddingRingGeneEffect();

    // 获取穿戴婚戒类型
    unsigned int GetPlayerWeddingRingItemType();

    // 获取玩家装备信息
    void PackOnlinePlayerInfo(RoleImageOfflineData & Info);

    void PackEquipDbItem(std::vector<DBItem> &vecEquipDbItem, RoleRoomImageData::ClotheEffectMap &clotheEffectMap, RoleRoomImageData::SevenColorMap &sevenColorMap);

    void ResumItem();

    // 获取物品
    ItemBase * GetItem(int nColumn, int nSlot);

    // 广播消息给其他人
    void SendPlayerMsgToOthers(GameMsg_Base * pMsg);

	// 判断物品能/否装备
	int CanEquip(ItemEquip * pEquip, CRoleVIPInfo * pVipInfo = NULL);
	int CanUnEquip(ItemEquip * pEquip);

	// 穿上装备
    int OnEquip(ItemEquip * pEquip, bool bRefreshClotheGroup = false, CRoleVIPInfo * pVipInfo = NULL);

	// 脱下装备
	int UnEquip(ItemEquip * pEquip);

    // 刷新物品
    void RefreshClientItem(int nColumnType, int nIndex, itemid_t nItemID);
	
////////////////////////////////  添加使用物品
private:
    // 添加唯一物品
	void UpdateItemDueTime(ItemBase * pItemBase, int nAddTime, EItemAction eItemAction, bool bNoticeClient = true, bool bUsed = false);
	int AddNewItem(ItemConfig * pItemConfig, int nCount, int nDueTime, EItemAction eItemAction, bool bNoticeClient = true, bool bUsed = false, int nDynamicBoxRelatedValue = 0);
    void DeleteItem(ItemBase * pItemBase, EItemAction eItemAction);
    void UpdateItemCount(ItemBase * pItemBase, int nCount, EItemAction eItemAction, bool bNoticeClient = true, bool bUsed = false, bool bCheckUsetimes = false);

    // 批量使用物品
    int BatchUseItem(int nColumnType, int nSelectSlot, itemtype_t nItemType, int nCount);
    void MergeItem(std::list<CItem> & listItem);
    
    // 使用单个物品
    bool AddPacketItemToRole(itemtype_t nPacketID, unsigned int nCount = 1);
    bool AddPacthPacketItemToRole(unsigned short nRoleIndex, itemtype_t nPacketID, unsigned int nCount, 
                list<CItem> & listItem, std::map<int, int> & mapCurrency);
    bool RandomBoxItem(itemtype_t nBoxID, list<CItem>& rItemList);
    bool AddBoxItemToRole( itemtype_t nBoxID );
    bool AddPatchBoxItemToRole( itemtype_t nBoxID, int nCount, std::list<CItem> & itemlist );

    // 使用动态宝箱
    bool AddDynamicBoxItemToRole( __int64 itemID, int nRelatedValue, itemtype_t nBoxID, int nCount = 1 );
    bool AddPatchDynamicBoxItemToRole( __int64 itemID, int nRelatedValue, itemtype_t nBoxID, int nCount, list<CItem> & listItem );

    // 生成动态宝箱物品
    bool GenerateDynamicBoxItem( __int64 itemID, int nRelatedValue, itemtype_t nBoxID, list<CItem> &itemlist );

    // 添加物品的分支
    int CanAddUniqueItem(ItemConfig * pItemConfig, unsigned short nCount, bool bAllowUniqueEquip);
    int CanAddOverlayItem(ItemConfig * pItemConfig, unsigned short nCount);

	// 判断物品能够使用
	int IsCanUsed(ItemBase * pItemBase, int nCount);
	int IsCanUsed(const std::list<itemid_t> & listItemID, int nCount);

    // 获取物品能添加数目
    unsigned int GetColumnPutInMaxCount(int nColumn, itemtype_t nItemType, const std::list<itemid_t> & listItemID);
    int GetItemPutInNeedSlotCount(int nColumn, itemtype_t nItemType, int nCount, const std::list<itemid_t> & listItemID);

    // 物品过期处理
    void OnItemDueTime(ItemBase * pItemBase);

    void SaveItemToItemInfo(const ItemBase* pItem,DBItem& itemInfo);    

    // 获取所有永久光效衣服的光效积分
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

    //物品添加到人物身上的时候调用。
    void OnItemAddToRole(const ItemBase*pItem);

    //物品从人物身上移走的时候调用。
    void OnItemRemoveFromRole(const ItemBase*pItem);

    // 刷新自己物品信息
    void RefreshClientItem(const std::list<itemgrid_t> & listGrid);
    
	// log日志
    void _LogRoleItem(unsigned int nRoleID, EItemAction eItemAction, EItemColumn eItemColumn, __int64 nItemID, itemtype_t nItemType, unsigned short nOldCount,
        int nOldDuration, unsigned short nNewCount, int nNewDuration, __int64 nAdditionalValue);

    void _SendExtendItemBinData();
    void SendClientItemForOther(int nColumn, int nSlot, ItemBase * pItem);

    void _PackItemBrief( CParamPool & IOBuff );
    void _PackItemBaseBrief(ItemBase * pItemBase, int nColumn, CParamPool & IOBuff);


    void SendPlayerMsg(GameMsg_Base *pMsg);

private:
    // 获赔默认衣服配置
    ItemConfig * GetDefaultClothConfig(EItemClothType clothTyp);

    // 获取默认衣服
    ItemEquip * GetDefaultCloth(EItemClothType clothTyp);

    // 添加默认服饰
    void AddDefaultColthToPlayer();

    // 创建默认服饰
	void CreateDefaultColth(EItemClothType clothTyp);

    // 获取默认服饰配置装备孔位
    int GetDefaultClothPos(EItemClothType clothType);

    // 获取默认装备放在背包的默认位置
    int GetDefaultClothBagSlot(EItemClothType clothType);

    // 检查默认部位是否有穿戴，没有穿默认衣服
    void CheckDefaultDressPos(EItemClothType pos);

    // 是否是默认服饰
    bool IsDefaultCloth(itemid_t nItemID) const;

    void SerializeDefaultClothToBuff( std::string &rBuff ) const;
public:
    // 开始收集身上衣服变化
    void BeginCollectEquipChange();

    // 结束收集身上衣服变化
    void EndCollectEquipChange();

    // 初始化衣服搭配
    void InitializeClothGroup(const std::list<RoleClothGroup> & group, const CRoleVIPInfo & vip);

    // 选择衣服搭配
    void OnSelectClothGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);
    bool IsValidClothGroup(const RoleClothGroup & group, std::string & strError);

    // 更新衣服搭配
    void OnUpdateClothGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);

    //刷新搭配，其他入口（商城购买使用）导致的搭配变更
    void _RefreshClothGroup( ItemEquip * pEquip );

    // 搭配初始成身上衣服
    void _InitClothGroup(unsigned char nGroupID);

    // 搭配初始成默认衣服
    void _InitGroupClothByDefault(unsigned char nGroupID);

    //检查搭配里的装备，不能装备的就报错，并且替换成默认装备，装备或替换信息会放入mapItemsDress
	void _CheckClothGroup(std::map<unsigned char, itemtype_t> & mapEquip, std::list<ItemEquip*> & listOnEquip, 
        std::map<itemtype_t, stringw>& mapItemError, CRoleVIPInfo * pVipInfo = NULL);

public:
    // 初始化互斥数据
    void InitializeEquipConflict();

    //装备完备性检查，即检查各种位置冲突并予以处理
    void EquipIntegrityCheck();

    //获得互斥装备位置
    void _GetConflictEquipPos(EItemClothType pos, std::set<EItemClothType>& setConflictEquipPos);
    bool IsConflictEquiped(EItemClothType pos);

    //装备真正装上前的位置冲突性检查，把互斥位置的装备先卸下
    void EquipConflictCheck(ItemEquip * pItem, bool bOnEquipDefault = false);

public:
    // 初始化物品冷却时间
    void InitializeCoolDown(const std::list<CoolDownData> & listCooling);

    // 物品是否在冷却中
    bool IsItemInCoolDown(itemtype_t nItemType);

    // 更新物品冷却
    void UpdateItemCoolDown(unsigned short nItemCoolDownTag,int nDuration);
    void UpdateItemCoolDown(itemtype_t nItemType);

public:
    // 穿上变身装备
    void EquipTransformItem(itemtype_t nItemType, int nDueTime);

    //脱掉变身装备
    void UnEquipTransformItem(ItemEquip * pEquip);

    // 变身
    void ApplyTransform(const std::map<int, itemtype_t>& mapClothPos, int nMatuneTime);

    // 卸掉变身装备
    void UnApplyTransform();

    // 是否在变身中
    bool IsInTransForm();

public:
    // 替换衣服特效(包括首次装备)   
    void OnReplaceClothEffect(GameMsg_Base &msg, CSlotPeer &slotPeer);
    int ReplaceClothEffect(EItemColumn eClothColumn, int nClothSlot, unsigned int nEffectID);

    // 光效升级
    void OnUpgradeEffect(GameMsg_Base &msg, CSlotPeer &slotPeer);
    int UpgradeEffect(EItemColumn eItemColumn, int nItemSlot, itemtype_t & nItemType, unsigned int & nUpEffectID);

    // 添加角色特效
    void AddRoleEffect(unsigned int nEffectID, itemtype_t nRelatedItemType);
    void AddRoleEffect(itemtype_t nItemType);

    // 添加衣服特效
    void AddClothEffect(unsigned int nEffectID, ItemEquip * pCloth, bool bRefreshToClient = true);

    // GM 添加衣服特效
    void AddClothEffectByGM(unsigned int nEffectID);

    // 广播特效变化
    void RefreshEffectChangeToOthers(itemtype_t nItemType, unsigned int nEffectID);

    // log日志
    void LogClothEffect(ClothEffectLog::EClothEffectAction eAction, itemid_t nItemID, unsigned int nOldEffectID, unsigned int nNewEffectID);

    // 检查是否有列表中特效ID
    void CheckEffectListExist(const std::list<unsigned int> & listCheck, std::list<unsigned int> & listReturn);

    // 获取图鉴100%的个数
    unsigned int GetHandbookFinishCount();

public:
    // 仓库、背包之间移动物品
    void OnMoveItem(GameMsg_Base &msg, CSlotPeer &slotPeer);
    int MoveItem(const std::list<itemgrid_t> & listToGrid, std::list<itemgrid_t> & listAddGrid);

    // 检查背包是否足够
    bool IsBagEnoughSlot(const std::map<itemtype_t, itemcountcolumn_t> & mapSelectItem);
    int GetMoveToBagColumn(ItemBase * pItemBase);

    // 开通仓库格子
    void OnAddStorageCapacity(GameMsg_Base &msg, CSlotPeer &slotPeer);
    int AddStorageCapacity();

    // 计算仓库的容量
    int CalcStorageCapacity(const StorageCapacityRecord & record, int nVipLevel);
    void UpdateStorageCapacity(int nVipLevel);

    // 插入/更新仓库容量
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
    std::list<itemid_t>                   m_listDefaultClothID;                         // 默认衣服ID
    bool                                  m_bGotCoolDown;                               // 是否获取过冷却列表
    unsigned long                         m_ItemUpdateDelay;                            // 物品心跳间隔
    std::map<unsigned char, RoleClothGroup> m_mapClothGroup;                            // 服饰搭配 key-搭配ID，value-配搭数据结构
    static std::map<EItemClothType, std::set<EItemClothType> > m_mapConflictEquipPos;   // 装备位置互斥映射表
    ClothEffect                           m_ClothEffect;                                // 衣服特效
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

