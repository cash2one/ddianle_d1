#include "../logic/LogicCirculator.h"
#include "EntityItemComponent.h"
#include "../../datastructure/DataStruct_Base.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../util/IDCreater.h"
#include "../lua/InitLua.h"
//#include "../../liblog/LogType.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../../datastructure/DataStruct_Game.h"
#include "../../datastructure/ItemErrorInfo.h"
#include "../../datastructure/LogicInitData.h"
#include "../../datastructure/DataStruct_DB.h"
#include "../payment/PaymentInfoMgr.h"
#include "../mall/GameMsg_Mall.h"
#include "../../datastructure/Localization.h"

#include "../quest/EntityQuestComponent.h"
#include "../gene/EntityGeneComponent.h"
#include "../achievement/EntityAchievementComponent.h"
#include "../room/Room.h"
#include "../room/RoomComponent.h"
#include "../roomLib/GameMsgNewRoom.h"
#include "../roomLib/GameMsgRoomBroadcast.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../vip/EntityVIPComponent.h"
#include "../Couple/WeddingRoom.h"
#include "../mail/MailMgr.h"
#include "../box/BoxInfoManager.h"
#include "../box/DynamciBoxInfoManager.h"
#include "../SystemSetting/SystemSettingMgr.h"
#include "../login/GameMsg_Player_CreateRoleRes.h"
#include "ItemMsgDefine.h"
#include "../../socket/GameMsg_Map.h"
#include "../../socket/Formatter.h"
#include "GameMsg_C2S_AwardMsg.h"
#include "GameMsg_S2C_AwardMsg.h"
#include "../login/GameMsg_Payment.h"
#include "../chat/GameMsg_S2G_Chat.h"
#include "../chat/ChatManager.h"
#include "../activity/GiftCodeInfoMgr.h"
#include "../PhotoRoom/EntityPhotoRoomComponent.h"
#include "../PhotoRoom/PhotoRoom.h"
#include "../AmuseRoom/AmuseRoomComponent.h"
#include "../AmuseRoom/AmuseRoom.h"
#include "../util/Split.h"
#include "../util/CommonFunc.h"
#include "../quest/EntityQuestNewComponent.h"
#include "../vip/VIPDataMgr.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../CeremonyRoom/ceremonyRoom.h"
#include "BagConfigManager.h"
#include "../Handbook/HandbookCfg.h"
#include "../Rank/RankComponent.h"

#ifdef WIN32
#pragma warning(disable:4061)
#endif

#include <set>

extern CIDCreater     *  g_pIDCreater;
extern CLogicCirculator* g_pCirculator;
extern int g_nLine;

#define ITEM_UPDATE_DELAY	5000

#define ITEM_BUFF_SIZE 256


std::map<EItemClothType, std::set<EItemClothType> > CEntityItemComponent::m_mapConflictEquipPos;

CEntityItemComponent::CEntityItemComponent(void)
    :CEntityComponent(true,true)
    , m_bCollectingRefreshChange(false)
{
    m_ItemUpdateDelay = 0;
    m_bGotCoolDown = false;

    m_pRoleQuest = NULL;
    m_pRoleAttr = NULL;
    m_pNetCpt = NULL;
    m_pRoleAch = NULL;
    m_pRoleGene = NULL;
    m_pRoleRoom = NULL;
    m_pRoleCouple = NULL;
    m_pRoleVIP = NULL;
    m_pRolePhotoRoom = NULL;
    m_pRoleAmuseRoom = NULL;
    m_pRoleQuestNew = NULL;
    m_pRankComponent = NULL;

    m_nLastSendGiftCodeTime = 0;
    m_tUpdateClothGroupName = 0;

    m_nPerpetualEffectScore = 0;
    m_nPerpetualEquipCount = 0;
    m_bPerpetualEffectScoreChange = false;
    m_bPerpetualEquipCountChange = false;

    //装备位置互斥关系设定
    InitializeEquipConflict();
}

CEntityItemComponent::~CEntityItemComponent(void)
{
    Destroy();
}

void CEntityItemComponent::Start()
{
    m_pRoleQuest = GetComponent<CRoleQuest>(); 
    m_pRoleAttr  = GetComponent<CRoleAttribute>();
    m_pNetCpt    = GetComponent<CRoleNet>();
    m_pRoleAch   = GetComponent<CRoleAchievement>();
    m_pRoleGene  = GetComponent<CRoleGene>();
    m_pRoleRoom	 = GetComponent<CRoleRoom>();
    m_pRoleCouple = GetComponent<CRoleCouple>();
    m_pRoleVIP= GetComponent<CRoleVIP>();
    m_pRolePhotoRoom  = GetComponent<CRolePhotoRoom>();
    m_pRoleAmuseRoom = GetComponent<CRoleAmuseRoom>();
    m_pRoleQuestNew = GetComponent<CRoleQuestNew>();
    m_pRoleDanceGroup = GetComponent<CRoleDanceGroup>();
    m_pRankComponent = GetComponent<CRoleRank>();
}

void CEntityItemComponent::RegComponentNetMsgMap()
{
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_UseItem);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_BatchUseItem);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RemoveItem);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ItemColumn);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ChangeEquipItem);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ItemAward);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetFirstPaymentPacketDtails);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetRechargeRewardsRules);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RechargeRewardEnable);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ReplaceClothEffect);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_LevelUpEffect);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_MoveItem);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_AddStorageCapacity);
    
   
    RegMsgProcessor(MSG_C2S_UseItem, &CEntityItemComponent::OnUseItem);
    RegMsgProcessor(MSG_C2S_BatchUseItem, &CEntityItemComponent::OnBatchUseItem);
    RegMsgProcessor(MSG_C2S_RemoveItem, &CEntityItemComponent::OnRemoveItem);
    RegMsgProcessor(MSG_C2S_ItemColumn, &CEntityItemComponent::OnAddItemColumn);
    RegMsgProcessor(MSG_C2S_ChangeEquipItem, &CEntityItemComponent::OnChangeEquipItem);
    RegMsgProcessor(MSG_C2S_ItemAward, &CEntityItemComponent::OnActivateGift);
    RegMsgProcessor(MSG_C2S_ReplaceClothEffect, &CEntityItemComponent::OnReplaceClothEffect);
    RegMsgProcessor(MSG_C2S_LevelUpEffect, &CEntityItemComponent::OnUpgradeEffect);

    RegMsgProcessor( MSG_C2S_GetRechargeRewardsRules, &CEntityItemComponent::OnGetRechargePlanDetail );		// 客户端请求 充值奖励规则
    RegMsgProcessor( MSG_C2S_RechargeRewardEnable, &CEntityItemComponent::OnQueryRechargeRewardEnable );		// 客户端 询问 充值奖励 是否开放

    RegMsgProcessor(MSG_C2S_GetFirstPaymentRewards, &CEntityItemComponent::OnGetFirstPaymentPacketDetail);
    RegMsgProcessor(MSG_C2S_MoveItem, &CEntityItemComponent::OnMoveItem);
    RegMsgProcessor(MSG_C2S_AddStorageCapacity, &CEntityItemComponent::OnAddStorageCapacity);

    
    RegMsgCreatorAndProcessor(GameMsg_C2S_SelectClothGroup, &CEntityItemComponent::OnSelectClothGroup);
    RegMsgCreatorAndProcessor(GameMsg_C2S_UpdateClothGroup, &CEntityItemComponent::OnUpdateClothGroup);
    RegMsgCreatorAndProcessor(GameMsg_C2S_UpdateClothGroupName, &CEntityItemComponent::OnUpdateClothGroupName);
    RegMsgCreatorAndProcessor(GameMsg_C2S_GetCoolDown, &CEntityItemComponent::OnGetCoolDown);
    RegQueryProcessor(QUERY_Item_GetActivateGiftInfo, &CEntityItemComponent::OnGetActivateInfoCB);
    RegQueryProcessor(QUERY_Item_GetActivateGift, &CEntityItemComponent::OnGetActivateGiftCodeCB);

}

unsigned int CEntityItemComponent::GetPlayerWeddingRingItemType()
{
    itemid_t nItemID = m_EquipManager.GetItemID(EEquipColumn_WeddingRingOnPlayer, WeddingRing_BAG_INDEX);
    ItemEquip * pCloth = (ItemEquip *)m_ItemManager.GetItemByID(nItemID);
    if (pCloth != NULL)
    {
        return pCloth->GetType();
    }

    return 0;
}

int CEntityItemComponent::OnEquip(ItemEquip * pEquip, bool bRefreshClotheGroup, CRoleVIPInfo * pVipInfo)
{
    if (pEquip == NULL || CanEquip(pEquip, pVipInfo) != EItemError_Success || pEquip->IsEquiped())
	{
		return EItemUse_NOtCanEquip;
	}

    if (pEquip->IsCloth())
    {
        // unequip this equip slot
        itemid_t nItemID = m_EquipManager.GetItemID(m_EquipManager.GetEquipColumn(pEquip), pEquip->GetEquipConfigPos());
        if (nItemID > 0)
        {
            ItemEquip * pCurrentEquip = (ItemEquip*)m_ItemManager.GetItemByID(nItemID);
            if (pCurrentEquip != NULL)
            {
                UnEquip(pCurrentEquip);
            }
        }

        // equip manager
        int nColumn = pEquip->GetColumn(), nSlot = pEquip->GetGrid();
        int nError = m_EquipManager.OnEquip(pEquip);

        if (nError != EItemError_Success)
        {
            WriteLog(LOGLEVEL_ERROR, "[Item] OnEquip_Cloth_Add: ItemID = %lld, nColumn = %d, nSlot = %d", pEquip->GetItemID(), pEquip->GetColumn(), pEquip->GetGrid());
        }

        // sync to client
        RefreshClientItem(nColumn, nSlot, pEquip->GetItemID());

        // sync to others
        SendClientItemForOther(pEquip->GetColumn(), pEquip->GetGrid(), pEquip);

        // reset grid
        pEquip->SetGrid(nColumn, nSlot);

        // conflict check
        EquipConflictCheck(pEquip, bRefreshClotheGroup);

        // update clothgroup
        if (bRefreshClotheGroup)
        {
            _RefreshClothGroup(pEquip);
        }

        if (pEquip->GetColumn() == -1 || pEquip->GetGrid() == -1)
        {
            WriteLog(LOGLEVEL_ERROR, "[Item] OnEquip_Cloth: ItemID = %lld, nColumn = %d, nSlot = %d", pEquip->GetItemID(), pEquip->GetColumn(), pEquip->GetGrid());
        }
    }
    else
    {
        // can equip
        int nError = m_EquipManager.CanEquip(pEquip);
        if (nError != EItemError_Success)
        {
            return nError;
        }

        // bag manager
        int nColumn = pEquip->GetColumn(), nSlot = pEquip->GetGrid();
        nError = m_BagManager.RemoveItem(pEquip);

        // sync to client
        RefreshClientItem(nColumn, nSlot, 0);

        // equip manager
        nError = m_EquipManager.OnEquip(pEquip);
        if (nError != EItemError_Success)
        {
            WriteLog(LOGLEVEL_ERROR, "[Item] OnEquip_Badge_Add: ItemID = %lld, nColumn = %d, nSlot = %d", pEquip->GetItemID(), pEquip->GetColumn(), pEquip->GetGrid());
        }

        // sync to client
        RefreshClientItem(pEquip->GetColumn(), pEquip->GetGrid(), pEquip->GetItemID());

        // sync to others
        SendClientItemForOther(pEquip->GetColumn(), pEquip->GetGrid(), pEquip);

        // gene
        std::list<GeneInfo*> equipGene;
        GetCurEquipGene(equipGene);
        m_pRoleGene->RefreshEquipGene(equipGene);

        // sync to db
        UpdateItemToDB(pEquip);

        if (pEquip->GetColumn() == -1 || pEquip->GetGrid() == -1)
        {
            WriteLog(LOGLEVEL_ERROR, "[Item] OnEquip_Badge: ItemID = %lld, nColumn = %d, nSlot = %d", pEquip->GetItemID(), pEquip->GetColumn(), pEquip->GetGrid());
        }
    }
	
    { // 同步穿装备数据
        GameMsg_S2G_OnEquipNotify sgMsg;
        sgMsg.m_nEffectID = pEquip->GetEffectID();
        int nColumn = (pEquip->GetColumn() == (int)EBagColumn_ClothBag ? (int)EEquipColumn_ClothOnPlayer : pEquip->GetColumn());
        sgMsg.m_dbEquip.Initialize(pEquip->GetItemID(), pEquip->GetType(), nColumn, pEquip->GetEquipConfigPos(), pEquip->GetCount(), pEquip->GetLeftUseTimes(),
            pEquip->GetDueTime(), pEquip->GetDynamicBoxRelatedValue(), true);

        ((CRoleEntity*)Entity())->SendMsg2GroupServer(&sgMsg);
    }


	// quist
	m_pRoleQuestNew->OnUseItem(pEquip->GetType());

	return EItemError_Success;
}

void CEntityItemComponent::EquipTransformItem(itemtype_t nItemType, int nDueTime)
{
    ItemConfig * pItemConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
    if (pItemConfig == NULL)
    {
        return;
    }

    nDueTime += (int)time(NULL);
    ItemBase * pItemBase = m_ItemManager.AddItem(pItemConfig, 1, nDueTime);
    if (pItemBase == NULL)
    {
        return;
    }

	m_EquipManager.OnEquip(EEquipColumn_TransformClothOnPlayer, (ItemEquip*)pItemBase);

    { // 同步穿装备数据
        GameMsg_S2G_OnEquipNotify sgMsg;
        ItemEquip* pEquip = (ItemEquip*)pItemBase;
        sgMsg.m_nEffectID = 0;
        int nColumn = (pEquip->GetColumn() == (int)EBagColumn_ClothBag ? (int)EEquipColumn_ClothOnPlayer : pEquip->GetColumn());
        sgMsg.m_dbEquip.Initialize(pEquip->GetItemID(), pEquip->GetType(), nColumn, pEquip->GetGrid(), pEquip->GetCount(), pEquip->GetLeftUseTimes(),
            pEquip->GetDueTime(), pEquip->GetDynamicBoxRelatedValue(), true);

        ((CRoleEntity*)Entity())->SendMsg2GroupServer(&sgMsg);
    }

    //DB操作
    InsertItemToDB(pItemBase, pItemBase->GetColumn(), pItemBase->GetGrid());

    //通知自己和他人
    RefreshClientItem(pItemBase->GetColumn(), pItemBase->GetGrid(), pItemBase->GetItemID());
    SendClientItemForOther(EItemColumn_ClothOnPlayer,(unsigned short)pItemBase->GetGrid(),pItemBase);
}

void CEntityItemComponent::UnEquipTransformItem(ItemEquip * pEquip)
{
    if(pEquip == NULL || !pEquip->IsEquip())
    {
        return;
    }

    if(pEquip->IsCloth())
    {
        DeleteItemToDB(pEquip);

        // 删除装备
        int nColumn = pEquip->GetColumn();
        int nSlot = pEquip->GetGrid();
		itemid_t nItemID = pEquip->GetItemID();

		m_EquipManager.UnEquip(EEquipColumn_TransformClothOnPlayer, pEquip);
		m_ItemManager.RemoveItem(pEquip->GetItemID());

        //通知自己和他人
		RefreshClientItem(nColumn, nSlot, nItemID);
        SendClientItemForOther(EItemColumn_ClothOnPlayer, nSlot, NULL);

        { // 同步脱装备数据
            GameMsg_S2G_UnEquipNotify sgMsg;
            sgMsg.m_nItemID = nItemID;
            sgMsg.m_nColumn = (unsigned short)nColumn;
            sgMsg.m_nIndex = (unsigned short)nSlot;
            ((CRoleEntity*)Entity())->SendMsg2GroupServer(&sgMsg);
        }
    }
}

void CEntityItemComponent::ApplyTransform( const std::map<int, itemtype_t>& mapClothPos, int nMatuneTime )
{
    //如果未变身则通知他人本人卸下正常装
    if ( !IsInTransForm() )
    {
		std::list<itemid_t> listItemID;
		m_EquipManager.GetEquipedCloth(listItemID);

		std::list<itemid_t>::iterator it = listItemID.begin();
		for (; it != listItemID.end(); ++it)
		{
			ItemBase * pItemBase = m_ItemManager.GetItemByID(*it);
			if (pItemBase != NULL)
			{
				SendClientItemForOther(EItemColumn_ClothOnPlayer, ((ItemEquip*)pItemBase)->GetEquipConfigPos(), NULL);
			}
		}
    }
    else//否则卸下旧变身
    {
        std::list<itemid_t> listItemID;
		m_EquipManager.UnEquipAll(EEquipColumn_TransformClothOnPlayer, listItemID);

        std::list<itemid_t>::iterator it = listItemID.begin();
        for ( ; it != listItemID.end(); ++it )
        {
            ItemBase * pItemBase = m_ItemManager.GetItemByID(*it);
            if (pItemBase != NULL)
            {
                UnEquipTransformItem(((ItemEquip*)pItemBase));
            }
        }
    }

    //穿上新变身
    for (std::map<int, itemtype_t>::const_iterator it = mapClothPos.begin(); it != mapClothPos.end(); ++it)
    {
        EquipTransformItem(it->second, nMatuneTime);
    }
}

void CEntityItemComponent::UnApplyTransform()
{
    // 卸下变身
	std::list<itemid_t> listItemID;
    m_EquipManager.GetAllItemID(EEquipColumn_TransformClothOnPlayer, listItemID);

	std::list<itemid_t>::iterator it = listItemID.begin();
	for (; it != listItemID.end(); ++it)
	{
		ItemBase * pItemBase = m_ItemManager.GetItemByID(*it);
		if (pItemBase != NULL)
		{
			UnEquipTransformItem(((ItemEquip*)pItemBase));
		}
	}

	// 通知他人本人穿上正常装
	listItemID.clear();
	m_EquipManager.GetEquipedCloth(listItemID);
	std::list<itemid_t>::iterator itr = listItemID.begin();
	for (; itr != listItemID.end(); ++itr)
	{
		ItemBase * pItemBase = m_ItemManager.GetItemByID(*itr);
		if (pItemBase != NULL)
		{
			SendClientItemForOther(EItemColumn_ClothOnPlayer, ((ItemEquip*)pItemBase)->GetEquipConfigPos(), pItemBase);
		}
	}
}

bool CEntityItemComponent::IsInTransForm()
{
	return m_EquipManager.GetColumnItemCount(EEquipColumn_TransformClothOnPlayer) > 0;
}

int CEntityItemComponent::ReplaceClothEffect(EItemColumn eClothColumn, int nClothSlot, unsigned int nEffectID)
{
    // 功能是否开放
    if (!CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_ClothEffect))
    {
        return EItemUse_FunctionNotOpen;
    }

    // 衣服存在不
    ItemBase * pItemBase = GetItem(eClothColumn, nClothSlot);
    if (pItemBase == NULL || !pItemBase->IsCloth() || pItemBase->GetDueTime() > 0)
    {
        return EItemUse_InvalidTarget;
    }

    //  衣服是否能加特效
    ItemEquip * pCloth = (ItemEquip*)pItemBase;
    if (pCloth->IsOldEffectCloth())
    {
        return EItemUse_ClothNotAddEffect;
    }

    // 部位是否开放
    if (!CSystemSettingMgr::Instance().IsOpenClothEffectPos(pCloth->GetEquipConfigPos()))
    {
        return EItemUse_ClothPosNotOpen;
    }

    // 存在特效配置
    ClothEffectConfig * pEffectConfig = ConfigManager::Instance().GetEffectConfigMgr().GetClothEffectConfig(nEffectID);
    if (pEffectConfig == NULL)
    {
        return EItemUse_EffectConfigNot;
    }

    // 光效石存在不
    ItemBase * pStoneItemBase = m_ItemManager.GetItemByType(pEffectConfig->m_nRelatedItem);
    if (pStoneItemBase == NULL)
    {
        return EItemUse_InvalidTarget;
    }

    // 光效石数量是否够
    if (GetItemCount(pEffectConfig->m_nRelatedItem, false, false) < pEffectConfig->m_nCostCount)
    {
        return EItemError_NotEnoughItem;
    }

    // 是否同一个特效
    if (pCloth->GetEffectID() == pEffectConfig->m_nEffectID)
    {
        return EItemUse_ClothEffectExist;
    }

    // 是否同一套的低等级特效
    ClothEffectConfig * pCurEffectConfig = ConfigManager::Instance().GetEffectConfigMgr().GetClothEffectConfig(pCloth->GetEffectID());
    if (pCurEffectConfig != NULL && pCurEffectConfig->m_nSuitID == pEffectConfig->m_nSuitID &&
        pCurEffectConfig->m_nLevel >= pEffectConfig->m_nLevel)
    {
        return EItemUse_InvalidTarget;
    }

    // 扣除物品
	if (!RemoveItemByType(pStoneItemBase->GetType(), pEffectConfig->m_nCostCount, EItemAction_Del_ReplaceClothEffect))
    {
        return EItemUse_RemoveItemError;
    }

    // log
    LogClothEffect(ClothEffectLog::EClothEffectAction_Replace, pCloth->GetItemID(), pCloth->GetEffectID(),
        pEffectConfig->m_nEffectID);

    int nOldScore = (int)GetPerpetualEffectEquipScore( pCloth );

    // 添加衣服光效
    AddClothEffect(pEffectConfig->m_nEffectID, pCloth);

    int nNewScore = (int)GetPerpetualEffectEquipScore( pCloth );
    if ( nOldScore != nNewScore )
    {
        UpdateEquipPerpetualEffectScore( (unsigned int)nNewScore, (unsigned int)nOldScore );

        if ( m_pRankComponent != NULL )
            m_pRankComponent->SubmitRankValue( ERankDataType_ClothEffect, nNewScore - nOldScore );
    }

    //通知medal模块
    CommonParam param;
    param.SetParam(std::string("medal"),std::string("effect_change") );
    param.SetParam(std::string("effect_change"), pEffectConfig->m_nEffectID);
    param.SetParam(std::string("cloth_effect"), m_nPerpetualEffectScore);
    NotifyAll(param);

    // 通知activeness模块
    CommonParam param_ac;
    param_ac.SetParam(std::string("activeness"), std::string("clotheffect_replace"));
    NotifyAll(param_ac);

    if (pCloth->IsEquiped())
    { // 
        GameMsg_S2G_EffectChangeNotify sgMsg;
        sgMsg.m_nItemID = pCloth->GetItemID();
        sgMsg.m_nEffectID = pCloth->GetEffectID();

        ((CRoleEntity*)Entity())->SendMsg2GroupServer(&sgMsg);
    }

    return EItemError_Success;
}

void CEntityItemComponent::OnReplaceClothEffect(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_ReplaceClothEffect & c2smsg = (GameMsg_C2S_ReplaceClothEffect&)msg;

    int nError = ReplaceClothEffect((EItemColumn)c2smsg.m_nClothColumn, (int)c2smsg.m_nClothSlot, c2smsg.m_nEffectID);
    if (nError != EItemError_Success)
    {
        GameMsg_S2C_ReplaceClothEffectFail failmsg;
        failmsg.m_nError = nError;
        SendPlayerMsg(&failmsg);
    }
    else
    {
        m_pRoleQuestNew->OnClotheEffectReplace(1);
        GameMsg_S2C_ReplaceClothEffectResult sucessmsg;
        SendPlayerMsg(&sucessmsg);
    }
}

void CEntityItemComponent::OnUpgradeEffect(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_LevelUpEffect & c2smsg = (GameMsg_C2S_LevelUpEffect&)msg;

    itemtype_t nRelatedItemType = 0;
    unsigned int nUpEffectID = 0;
    int nError = UpgradeEffect((EItemColumn)c2smsg.m_nItemColumn, (int)c2smsg.m_nItemSlot, nRelatedItemType, nUpEffectID);
    if (nError != EItemError_Success)
    {
        GameMsg_S2C_LevelUpEffectFail failmsg;
        failmsg.m_nError = nError;
        SendPlayerMsg(&failmsg);
    }
    else
    {
        GameMsg_S2C_LevelUpEffectResult sucessmsg;
        sucessmsg.m_nItemtype = nRelatedItemType;
        sucessmsg.m_nLevelUpEffectID = nUpEffectID;
        SendPlayerMsg(&sucessmsg);
    }
}

int CEntityItemComponent::UpgradeEffect(EItemColumn eItemColumn, int nItemSlot, itemtype_t & nItemType, unsigned int & nUpEffectID)
{
    // 功能是否开放
    if (!CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_ClothEffect))
    {
        return EItemUse_FunctionNotOpen;
    }

    ItemBase * pItemBase = GetItem(eItemColumn, nItemSlot);
    if(pItemBase == NULL)
    {
        return EItemUse_InvalidTarget;
    }

    unsigned int nEffectID = 0;
    if (pItemBase->IsCloth())
    {
        nEffectID = ((ItemEquip*)pItemBase)->GetEffectID();
    }
    else
    {
        nEffectID = ConfigManager::Instance().GetEffectConfigMgr().GetClothEffectIDByItem(pItemBase->GetType());
    }

    // 配置错误
    ClothEffectConfig * pEffectConfig = ConfigManager::Instance().GetEffectConfigMgr().GetClothEffectConfig(nEffectID);
    if (pEffectConfig == NULL)
    {
        return EItemUse_EffectConfigNot;
    }

    // 升级配置
    ClothEffectUpgrade * pUpgradeConfig = ConfigManager::Instance().GetEffectConfigMgr().GetClothEffectUpgradeConfig(nEffectID);
    if (pUpgradeConfig == NULL)
    {
        return EItemUse_EffectConfigNot;
    }

    // 下一等级配置
    ClothEffectConfig * pNextEffectConfig = ConfigManager::Instance().GetEffectConfigMgr().GetClothEffectConfig(pUpgradeConfig->m_nUpgradeID);
    if (pNextEffectConfig == NULL)
    {
        return EItemUse_MaxLevel;
    }

    // 是否消耗物品够
    std::list<CItem> listCostItem;
    EMallCurrencyType   eCurrentType = EMallCurrencyType_None;
    unsigned int nCostCurrent= 0;
    if (!m_ClothEffect.IsSuitCollectFinish(pEffectConfig->m_nSuitID))
    {
        listCostItem = pUpgradeConfig->m_listCostItem;
        eCurrentType = (EMallCurrencyType)pUpgradeConfig->m_nCostMoneyType;
        nCostCurrent = pUpgradeConfig->m_nCostMoneyNum;
    }
    else
    {
        listCostItem = pUpgradeConfig->m_listDiscountCostItem;
        eCurrentType = (EMallCurrencyType)pUpgradeConfig->m_nDiscountCostMoneyType;
        nCostCurrent = pUpgradeConfig->m_nDiscountCostMoneyNum;
    }

    // 检查物品是否足够
    std::list<CItem>::iterator it = listCostItem.begin();
    for (; it != listCostItem.end(); ++it)
    {
        if (GetItemCount(it->m_nItemType, false, false) < it->m_nItemCount)
        {
            return EItemError_NotEnoughItem;
        }
    }

    // 钱是否够
    if (eCurrentType != EMallCurrencyType_None && nCostCurrent > 0)
    {
        if (!m_pRoleAttr->IsEnoughCurrency(eCurrentType, nCostCurrent))
        {
            return EItemUse_NotEnoughMoney;
        }
    }

     // 扣除物品
    for (it = listCostItem.begin(); it != listCostItem.end(); ++it)
    {
        RemoveItemByType(it->m_nItemType, it->m_nItemCount, EItemAction_Del_LevelUpEffect);
    }

    // 扣除金钱
    switch(eCurrentType)
    {
    case EMallCurrencyType_MCoin:
        m_pRoleAttr->ChangeBill((-1)*nCostCurrent, EChangeBillCause_Del_ClothEffectUpgrade);
        break;
    case EMallCurrencyType_BindMCoin:
        m_pRoleAttr->ChangeBindBill((-1)*nCostCurrent, EChangeBindBillCause_Del_ClothEffectUpgrade);
        break;
    case  EMallCurrencyType_Money:
        m_pRoleAttr->ChangeMoney((-1)*nCostCurrent, EChangeMoneyCause_Del_ClothEffectUpgrade, 0);
        break;
    case EMallCurrencyType_None:
    case EMallCurrencyType_Max:
    default:break;
    }

    // 处理特效ID
    if (pItemBase->IsCloth())
    {
        int nOldScore = (int)GetPerpetualEffectEquipScore( (ItemEquip *)pItemBase );

        // 升级特效
        AddClothEffect(pNextEffectConfig->m_nEffectID, (ItemEquip*)pItemBase);

        int nNewScore = (int)GetPerpetualEffectEquipScore( (ItemEquip *)pItemBase );
        if ( nOldScore != nNewScore )
        {
            UpdateEquipPerpetualEffectScore( (unsigned int)nNewScore, (unsigned int)nOldScore );

            if ( m_pRankComponent != NULL )
                m_pRankComponent->SubmitRankValue( ERankDataType_ClothEffect, nNewScore - nOldScore );
        }

        // 取出成功后信息
        nItemType = pItemBase->GetType();
        nUpEffectID = pNextEffectConfig->m_nEffectID;
    }
    else
    {
        // 添加新的光效石
        AddItem(pNextEffectConfig->m_nRelatedItem, 1, -1, EItemAction_Add_LevelUpEffect, 0, true, false, false);

        // 添加到角色特效
        AddRoleEffect(pNextEffectConfig->m_nEffectID, 0);

        // 取出成功后信息
        nItemType = pNextEffectConfig->m_nRelatedItem;
        nUpEffectID = pNextEffectConfig->m_nEffectID;
    }

    // log
    LogClothEffect(ClothEffectLog::EClothEffectAction_Upgrade, pItemBase->GetItemID(), nEffectID,
        pNextEffectConfig->m_nEffectID);
    m_pRoleQuestNew->OnClotheEffectLevelUp(1);

    //通知medal模块
    CommonParam param;
    param.SetParam(std::string("medal"),std::string("effect_change") );
    param.SetParam(std::string("effect_change"), pNextEffectConfig->m_nEffectID);
    param.SetParam(std::string("cloth_effect"), m_nPerpetualEffectScore);
    NotifyAll(param);

    // 通知activeness模块
    CommonParam param_ac;
    param_ac.SetParam(std::string("activeness"), std::string("clotheffect_upgrade"));
    NotifyAll(param_ac);

    if (((ItemEquip*)pItemBase)->IsEquiped())
    { // 
        GameMsg_S2G_EffectChangeNotify sgMsg;
        sgMsg.m_nItemID = pItemBase->GetItemID();
        sgMsg.m_nEffectID = ((ItemEquip*)pItemBase)->GetEffectID();

        ((CRoleEntity*)Entity())->SendMsg2GroupServer(&sgMsg);
    }


    return EItemError_Success;
}

void CEntityItemComponent::AddRoleEffect(unsigned int nEffectID, itemtype_t nRelatedItemType)
{
    m_ClothEffect.EffectChange(nEffectID, nRelatedItemType);
}

void CEntityItemComponent::AddRoleEffect(itemtype_t nItemType)
{
    ItemConfig * pItemConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
    if (pItemConfig == NULL)
    {
        return;
    }

    ClothEffectConfig * pEffectConfig = ConfigManager::Instance().GetEffectConfigMgr().GetClothEffectConfigByItem(nItemType);
    if (pEffectConfig != NULL)
    {
        m_ClothEffect.EffectChange(pEffectConfig->m_nEffectID, 0);
    }
}

void CEntityItemComponent::AddClothEffect(unsigned int nEffectID, ItemEquip * pCloth, bool bRefreshToClient)
{
    if (pCloth == NULL || nEffectID == 0)
    {
        return;
    }

    AddRoleEffect(nEffectID, pCloth->GetType());

    // sync to db
    if (pCloth->GetEffectID() > 0)
    {
        pCloth->SetEffectID(nEffectID);
        UpdateClothEffectToDB(pCloth);
    }
    else
    {
        pCloth->SetEffectID(nEffectID);
        InsertClothEffectToDB(pCloth);
    }

    // sync to client
    if (bRefreshToClient)
    {
        RefreshClientItem(pCloth->GetColumn(), pCloth->GetGrid(), pCloth->GetItemID());
        RefreshEffectChangeToOthers(pCloth->GetType(), pCloth->GetEffectID());
    }
}

void CEntityItemComponent::RefreshEffectChangeToOthers(itemtype_t nItemType, unsigned int nEffectID)
{
    GameMsg_S2C_RefreshEffectChangeToOthers refreshmsg;
    refreshmsg.m_nItemType = nItemType;
    refreshmsg.m_nEffectID = nEffectID;
    SendPlayerMsgToOthers(&refreshmsg);
}

void CEntityItemComponent::AddClothEffectByGM(unsigned int nEffectID)
{
    ClothEffectConfig * pClothEffectConfig = ConfigManager::Instance().GetEffectConfigMgr().GetClothEffectConfig(nEffectID);
    if (pClothEffectConfig == NULL)
    {
        return;
    }

    itemid_t nItemID = m_EquipManager.GetClothItemID(pClothEffectConfig->m_nClothPos);
    ItemEquip * pCloth = (ItemEquip*)m_ItemManager.GetItemByID(nItemID);
    if (pCloth == NULL)
    {
        return;
    }

    // log
    LogClothEffect(ClothEffectLog::EClothEffectAction_GM, pCloth->GetItemID(), pCloth->GetEffectID(),
        nEffectID);

    int nOldScore = (int)GetPerpetualEffectEquipScore( pCloth );

    AddClothEffect(nEffectID, pCloth);

    int nNewScore = (int)GetPerpetualEffectEquipScore( pCloth );
    if ( nOldScore != nNewScore )
    {
        UpdateEquipPerpetualEffectScore( (unsigned int)nNewScore, (unsigned int)nOldScore );

        if ( m_pRankComponent != NULL )
            m_pRankComponent->SubmitRankValue( ERankDataType_ClothEffect, nNewScore - nOldScore );
    }

    //通知medal模块
    CommonParam param;
    param.SetParam(std::string("medal"), std::string("cloth_effect"));
    param.SetParam(std::string("cloth_effect"), m_nPerpetualEffectScore);
    NotifyAll(param);
}

void CEntityItemComponent::LogClothEffect(ClothEffectLog::EClothEffectAction eAction, itemid_t nItemID, unsigned int nOldEffectID, unsigned int nNewEffectID)
{
    ClothEffectLog * pLog = new ClothEffectLog();
    pLog->m_nRoleID = m_pRoleAttr->GetRoleID();
    pLog->m_nItemID = nItemID;
    pLog->m_nOldEffectID = nOldEffectID;
    pLog->m_nNewEffectID = nNewEffectID;
    pLog->m_eAction = eAction;

    AddQuery(QUERY_LogRoleClothEffect, 0, pLog);
}

void CEntityItemComponent::CheckEffectListExist(const std::list<unsigned int> & listCheck, std::list<unsigned int> & listReturn)
{
    m_ClothEffect.CheckEffectListExist(listCheck, listReturn);
}

unsigned int CEntityItemComponent::GetHandbookFinishCount()
{
    return m_ClothEffect.GetHandbookFinishCount();
}

void CEntityItemComponent::OnChangeEquipItem(GameMsg_Base & msg,CSlotPeer & SlotPeer)
{
}

void CEntityItemComponent::OnActivateGift(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    if (CGiftCodeInfoMgr::Instance().IsActivityOpening()) 
    {
        GameMsg_C2S_ItemAward	*pMsg = (GameMsg_C2S_ItemAward *)&msg;
        if (pMsg == NULL || m_pRoleAttr == NULL) 
        {
            return;
        }

        if (time(NULL) - m_nLastSendGiftCodeTime < MAX_SENDGIFT_TIME_INTERVAL) 
        {
            GameMsg_S2C_ItemAwardFail failMsg;
            failMsg.m_strError = "Item_Activate_Frequently";
            SendPlayerMsg(&failMsg);
            return;
        }

        if (pMsg->m_strItemCode.size() > MAX_GIFTCODE_NUM) 
        {
            GameMsg_S2C_ItemAwardFail failMsg;
            failMsg.m_strError = "Item_Activate_Code_Len_Error";
            SendPlayerMsg(&failMsg);
            return;
        }

        CActivateGiftInfo *pGiftIno = new CActivateGiftInfo;
        pGiftIno->m_strCode = pMsg->m_strItemCode;

        CDelHelper delHelper(pGiftIno);
        AddQuery(QUERY_Item_GetActivateGiftInfo, m_pRoleAttr->GetRoleID(), pGiftIno, 0, NULL, &delHelper);
    }
}

void CEntityItemComponent::OnGetFirstPaymentPacketDetail(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    Version_Type versionType = (Version_Type)m_pNetCpt->GetVerType();

    int nNow = (int)time(NULL);
    if( CSystemSettingMgr::Instance().CanFirstPaymentReward() && 					// 配置文件
        PaymentInfoMgr::Instance( ).FirstChargeRewardIsOpen( versionType, nNow ) )	// lua脚本
    {
        GameMsg_S2C_GetFirstPaymentPacketDetailsRes resMsg;

        // 首次充值 ( 金券|Mcoin) 奖励规则
        PaymentInfoMgr::Instance( ).GetFirstChargeRewardPlan( resMsg.m_vecChargePlan, versionType, nNow );

        // 首次充值 道具奖励
        if( m_pRoleAttr != NULL ) 
        {
            if( m_pRoleAttr->GetSex( ) == ESexType_Male ) 
            {
                resMsg.m_listRewards = PaymentInfoMgr::Instance( ).MaleRewards( versionType, nNow );
            }
            else if( m_pRoleAttr->GetSex( ) == ESexType_Female )
            {
                resMsg.m_listRewards = PaymentInfoMgr::Instance( ).FemaleRewards( versionType, nNow );
            }
        }
        SendPlayerMsg( &resMsg );
    }
    else
    {
        GameMsg_S2C_FirstPaymentStateChange msgIsOpen;
        msgIsOpen.m_bFirstPaymentRewardsIsOpen = false;
        SendPlayerMsg( &msgIsOpen );
    }
}

void CEntityItemComponent::OnQueryRechargeRewardEnable( GameMsg_Base &msg, CSlotPeer &slotPeer )
{
    GameMsg_C2S_RechargeRewardEnable *pMsg = (GameMsg_C2S_RechargeRewardEnable *)&msg;
    // 版本类型
    Version_Type versionType = (Version_Type)m_pNetCpt->GetVerType( );
    int nNow = (int)time( NULL ); // 奖励发放的开关以服务器当前时间为准

    // 充值奖励 开关 ( 充值时间段 作为开关 )
    if( pMsg != NULL && m_pRoleAttr != NULL )
    {
        GameMsg_S2C_RechargeRewardsEnableSuc sucMsg;
        if( PaymentInfoMgr::Instance( ).RechargeRewardIsOpen( versionType, nNow ) )
        {
            sucMsg.m_bRewardOPen = true;
        }
        else
        {
            sucMsg.m_bRewardOPen = false;
        }
        SendPlayerMsg( &sucMsg );
    }
}

void CEntityItemComponent::OnGetRechargePlanDetail( GameMsg_Base &msg, CSlotPeer &slotPeer )
{
    //	GameMsg_C2S_GetRechargeRewardsRules *pMsg = (GameMsg_C2S_GetRechargeRewardsRules *)&msg;
    // 版本类型
    Version_Type versionType = (Version_Type)m_pNetCpt->GetVerType( );
    int nNow = (int)time( NULL ); // 奖励发放的开关以服务器当前时间为准

    // 充值奖励 开关 ( 充值时间段 作为开关 )
    if( PaymentInfoMgr::Instance( ).RechargeRewardIsOpen( versionType, nNow ) )
    {
        if( m_pRoleAttr != NULL)
        {
            GameMsg_S2C_GetRechargeRewardsRulesSuc sucMsg;
            // 日常充值奖励规则 
            PaymentInfoMgr::Instance( ).GetRechargeRewardPlan( sucMsg.m_vecChargePlan, versionType, nNow );
            SendPlayerMsg( &sucMsg );
        }
    }
}

void CEntityItemComponent::OnSelectClothGroup( GameMsg_Base &msg, CSlotPeer &slotPeer )
{
    GameMsg_C2S_SelectClothGroup *pMsg = (GameMsg_C2S_SelectClothGroup *)&msg;
    GameMsg_S2C_SelectClothGroup retMsg;
    retMsg.m_nGroupID = pMsg->m_nGroupID;
	std::list<ItemEquip*> listOnEquip;
    std::list<itemtype_t> listUnEquip;
	std::list<ItemEquip*>::iterator itr;
    std::list<itemtype_t>::iterator itun;
    std::map<unsigned char, itemtype_t> mapCurrentGroupItem;
    std::map<unsigned char, itemtype_t> mapCheckClothGroup;
    std::map<unsigned char, itemtype_t>::iterator itt;
    std::map<unsigned char, RoleClothGroup>::iterator itCurrentGroup;
    std::map<unsigned char, RoleClothGroup>::iterator itGroup = m_mapClothGroup.find(pMsg->m_nGroupID);
    if (itGroup == m_mapClothGroup.end())
    {
        retMsg.m_strError.assign("SelectClothGroup_GroupNotExist");
        goto END;
    }

    if (!IsValidClothGroup(itGroup->second, retMsg.m_strError))
    {
        goto END;
    }

    if ( itGroup->second.IsEmpty() )
    {
        _InitGroupClothByDefault(pMsg->m_nGroupID);
    }
   
    itCurrentGroup = m_mapClothGroup.find(m_pRoleAttr->GetClothGroupID());
    if (itCurrentGroup != m_mapClothGroup.end())
    {
        mapCurrentGroupItem = itCurrentGroup->second.GetClothes();
    }
    
    m_pRoleAttr->SetClothGroupID(pMsg->m_nGroupID);

    // check
    mapCheckClothGroup = itGroup->second.GetClothes();
    _CheckClothGroup( mapCheckClothGroup, listOnEquip, retMsg.m_mapItemError );
    itGroup->second.SetClothes( mapCheckClothGroup );

    // unequip
    itt = mapCurrentGroupItem.begin();
    for (; itt != mapCurrentGroupItem.end(); ++itt)
    {
        if ( itGroup->second.IsPosEmpty( itt->first ) )
        {
            listUnEquip.push_back(itt->second);
        }
    }

  //  BeginCollectEquipChange();
    itun = listUnEquip.begin();
    for (; itun != listUnEquip.end(); ++itun)
    {
        ItemEquip * pCloth = (ItemEquip *)m_ItemManager.GetItemByType(*itun);
        if (pCloth != NULL && pCloth->IsEquiped())
        {
            UnEquip(pCloth);
        }
    }

    // onequip
	itr = listOnEquip.begin();
	for (; itr != listOnEquip.end(); ++itr)
	{
		if (*itr == NULL)
		{
			continue;
		}

		if (OnEquip(*itr) == EItemError_Success)
		{
            itGroup->second.AddCloth( (*itr)->GetEquipConfigPos(), (*itr)->GetType() );
		}
	}

    // integrity check
    EquipIntegrityCheck();
   // EndCollectEquipChange();

    if (!retMsg.m_mapItemError.empty())
    {
        retMsg.m_strError.assign("UpdateClothGroup_ItemError");//指定的道具不合法

        RoleClothGroup* pClothGroup = new RoleClothGroup();
        *pClothGroup = itGroup->second;
        AddQuery(QUERY_UpdateRoleClothGroup, 1, NULL, m_pRoleAttr->GetRoleID(), pClothGroup);
    }

END:
    if ( !retMsg.m_strError.empty() )
    {
        WriteLog(LOGLEVEL_ERROR, "role %u OnSelectClothGroup error = %s", m_pRoleAttr->GetRoleID(), retMsg.m_strError.c_str());
    }
    SendPlayerMsg(&retMsg);
}

bool CEntityItemComponent::IsValidClothGroup(const RoleClothGroup & group, std::string & strError)
{
    //变身时不能使用搭配
    if ( m_pRoleAttr->GetTransformId() > 0 )
    {
        strError.assign("SelectClothGroup_Transform");
        return false;
    }

    //vip等级不够
    if ( m_pRoleVIP->VIPLevel() < group.GetVIPRequest() )
    {
        strError.assign("SelectClothGroup_VIPLevelNotEnough");
        return false;
    }

    //vip过期了
    if ( group.GetVIPRequest() > 0 && !m_pRoleVIP->IsVIP() )
    {
        strError.assign("SelectClothGroup_VIPExpired");
        return false;
    }

    return true;
}

void CEntityItemComponent::OnUpdateClothGroup( GameMsg_Base &msg, CSlotPeer &slotPeer )
{
    GameMsg_C2S_UpdateClothGroup *pMsg = (GameMsg_C2S_UpdateClothGroup *)&msg;
	GameMsg_S2C_UpdateClothGroup retMsg;
	retMsg.m_nGroupID = pMsg->m_nGroupID;
    std::map<unsigned char, RoleClothGroup>::iterator itGroup = m_mapClothGroup.find(pMsg->m_nGroupID);
    if ( itGroup == m_mapClothGroup.end() )
    {
        retMsg.m_strError.assign("UpdateClothGroup_GroupNotExist");//搭配不存在
        SendPlayerMsg(&retMsg);
        return;
    }

    if (!IsValidClothGroup(itGroup->second, retMsg.m_strError))
    {
        SendPlayerMsg(&retMsg);
        return;
    }

	std::map<unsigned char, itemtype_t> mapItemEquip;
	for (std::list<ClothGroupItem>::iterator it = pMsg->m_listItems.begin();
		it != pMsg->m_listItems.end(); ++it)
	{
		if (it->m_nType >= (unsigned char)eItemClothType_Max)
		{
			retMsg.m_strError.assign("UpdateClothGroup_InvalidEquipPos");//非法的装备位置
			SendPlayerMsg(&retMsg);
			return;
		}
		mapItemEquip[it->m_nType] = it->m_nItemType;
	}

	std::list<ItemEquip*> listOnEquip;
	_CheckClothGroup(mapItemEquip, listOnEquip, retMsg.m_mapItemError);

    // unequip
   // BeginCollectEquipChange();
    std::list<itemtype_t> listUnEquip;
    std::map<unsigned char, itemtype_t>::const_iterator itt = itGroup->second.GetClothes().begin();
    for ( ; itt != itGroup->second.GetClothes().end(); ++itt )
    {
        if (mapItemEquip.end() == mapItemEquip.find(itt->first))
        {
            listUnEquip.push_back(itt->second);
        }
    }

    std::list<itemtype_t>::iterator itun = listUnEquip.begin();
    for (; itun != listUnEquip.end(); ++itun)
    {
        ItemEquip * pCloth = (ItemEquip *)m_ItemManager.GetItemByType(*itun);
        if (pCloth != NULL && pCloth->IsEquiped())
        {
            UnEquip(pCloth);
        }
    }

    // onEquip
	std::list<ItemEquip*>::iterator itr = listOnEquip.begin();
	for (; itr != listOnEquip.end(); ++itr)
	{
		if (*itr == NULL)
		{
			continue;
		}

		if (OnEquip(*itr) == EItemError_Success)
		{
			mapItemEquip[(*itr)->GetEquipConfigPos()] = (*itr)->GetType();
		}
	}

    // intergrity check
    EquipIntegrityCheck();
  //  EndCollectEquipChange();

	// sync to db
    itGroup->second.SetClothes( mapItemEquip );
    RoleClothGroup* pClothGroup = new RoleClothGroup();
    *pClothGroup = itGroup->second;

	int nQueryType = QUERY_UpdateRoleClothGroup;
	if (itGroup->second.IsEmpty())
	{
		nQueryType = QUERY_AddRoleClothGroup;
	}

    int nUpdateItems = 1;//更新道具标志
    AddQuery(nQueryType, nUpdateItems, NULL, m_pRoleAttr->GetRoleID(), pClothGroup);
    for ( std::map<unsigned char, itemtype_t>::const_iterator it = itGroup->second.GetClothes().begin();
        it != itGroup->second.GetClothes().end(); ++it )
    {
        ClothGroupItem e;
        e.m_nType = it->first;
        e.m_nItemType = it->second;
        retMsg.m_listItems.push_back(e);
    }

	m_pRoleAttr->SetClothGroupID(pMsg->m_nGroupID);
    if (!retMsg.m_mapItemError.empty())
    {
        retMsg.m_strError.assign("UpdateClothGroup_ItemError");
    }
    SendPlayerMsg(&retMsg);
}

void CEntityItemComponent::OnUpdateClothGroupName( GameMsg_Base &msg, CSlotPeer &slotPeer )
{
    GameMsg_C2S_UpdateClothGroupName *pMsg = (GameMsg_C2S_UpdateClothGroupName *)&msg;
    time_t tNow = time(NULL);
    GameMsg_S2C_UpdateClothGroupName retMsg;
    retMsg.m_strGroupName = pMsg->m_strGroupName;
    retMsg.m_nGroupID = pMsg->m_nGroupID;
    std::map<unsigned char, RoleClothGroup>::iterator itGroup = m_mapClothGroup.find(pMsg->m_nGroupID);
    if ( itGroup == m_mapClothGroup.end() )
    {
        retMsg.m_strError.assign("UpdateClothGroupName_GroupNotExist");//搭配不存在
    }
    else if ( pMsg->m_strGroupName.size() > MAX_CLOTH_GROUP_NAME_LEN )
    {
        retMsg.m_strError.assign("UpdateClothGroupName_NameTooLong");//搭配名字太长
    }
    else if ( tNow - m_tUpdateClothGroupName < UPDATE_CLOTH_GROUP_NAME_TIME )
    {
        retMsg.m_strError.assign("UpdateClothGroupName_UpdateTooOften");//搭配名字更新太频繁
    }
    else
    {
        itGroup->second.SetGroupName( pMsg->m_strGroupName );
        m_tUpdateClothGroupName = tNow;
        RoleClothGroup* pClothGroup = new RoleClothGroup();
        *pClothGroup = itGroup->second;
        int nUpdateItems = 0;//更新道具标志

        int nQueryType = QUERY_UpdateRoleClothGroup;
        if ( itGroup->second.IsEmpty() )
        {
            nQueryType = QUERY_AddRoleClothGroup;
        }

        AddQuery(nQueryType, nUpdateItems, NULL, m_pRoleAttr->GetRoleID(), pClothGroup);
    }
    if ( !retMsg.m_strError.empty() )
    {
        WriteLog(LOGLEVEL_ERROR, "role %u OnUpdateClothGroupName error = %s", m_pRoleAttr->GetRoleID(), retMsg.m_strError.c_str());
    }
    SendPlayerMsg(&retMsg);
}

void CEntityItemComponent::EquipConflictCheck(ItemEquip * pCloth, bool bOnEquipDefault)
{
    if( pCloth == NULL || !pCloth->RequireIntegrityCheck() )
    {
        return ;
    }

    std::set<EItemClothType> setConflictEquipPos;
    _GetConflictEquipPos((EItemClothType)pCloth->GetAnType(2), setConflictEquipPos);
    for (std::set<EItemClothType>::iterator it = setConflictEquipPos.begin(); it != setConflictEquipPos.end(); ++it)
    {
        itemid_t nItemID = m_EquipManager.GetClothItemID((int)*it);
        ItemEquip * pDefaultItem = (ItemEquip *)m_ItemManager.GetItemByID(nItemID);
        if( pDefaultItem != NULL )
        {
            UnEquip( pDefaultItem );
        }
    }

    if (bOnEquipDefault)
    {
        //对需要装备的位置却没有装备的填充默认装备
        CheckDefaultDressPos(eItemClothType_Hair);
        CheckDefaultDressPos(eItemClothType_Face);
        CheckDefaultDressPos(eItemClothType_Body);
        CheckDefaultDressPos(eItemClothType_Glove);
        CheckDefaultDressPos(eItemClothType_Leg);
        CheckDefaultDressPos(eItemClothType_Feet);
        CheckDefaultDressPos(eItemClothType_Skin);
    }
}


void CEntityItemComponent::EquipIntegrityCheck()
{
    //检查装备的完整性
    for (std::map<EItemClothType, std::set<EItemClothType> >::iterator it = m_mapConflictEquipPos.begin();
        it != m_mapConflictEquipPos.end(); ++it)
    {
        itemid_t nItemID = m_EquipManager.GetItemID(EEquipColumn_ClothOnPlayer, (int)(it->first));
        ItemEquip * pCloth = (ItemEquip *)m_ItemManager.GetItemByID(nItemID);
        if (pCloth != NULL)
        {
            for (std::set<EItemClothType>::iterator itSet = it->second.begin(); itSet != it->second.end(); ++itSet)
            {
                itemid_t nAgainstID = m_EquipManager.GetItemID(EEquipColumn_ClothOnPlayer, (int)(*itSet));
                ItemEquip * pAgainstCloth = (ItemEquip *)m_ItemManager.GetItemByID(nAgainstID);
                if (pAgainstCloth != NULL)
                {
                    UnEquip(pAgainstCloth);
                }
            }
        }
    }

    //对需要装备的位置却没有装备的填充默认装备
    CheckDefaultDressPos(eItemClothType_Hair);
    CheckDefaultDressPos(eItemClothType_Face);
    CheckDefaultDressPos(eItemClothType_Body);
    CheckDefaultDressPos(eItemClothType_Glove);
    CheckDefaultDressPos(eItemClothType_Leg);
    CheckDefaultDressPos(eItemClothType_Feet);
    CheckDefaultDressPos(eItemClothType_Skin);
}

bool CEntityItemComponent::IsDefaultCloth(itemid_t nItemID) const
{
    std::list<itemid_t>::const_iterator it = find(m_listDefaultClothID.begin(), m_listDefaultClothID.end(), nItemID);
    if (it != m_listDefaultClothID.end())
    {
        return true;
    }

    return false;
}

void CEntityItemComponent::SerializeDefaultClothToBuff( std::string &rBuff ) const
{
    char szBuff[256] = { 0 };
    CParamPool buffPool( szBuff, sizeof( szBuff ) );

    buffPool.AddUShort( (unsigned short)m_listDefaultClothID.size() );
    rBuff.append( buffPool.GetData(), buffPool.GetValidDataSize() );

    for ( std::list<itemid_t>::const_iterator itr = m_listDefaultClothID.begin();
        itr != m_listDefaultClothID.end(); ++itr )
    {
        ItemBase *pItem = m_ItemManager.GetItemByID( *itr );
        if ( pItem != NULL )
        {
            buffPool.ClearData();
            buffPool.AddUShort( (unsigned short)pItem->GetColumn() );
            buffPool.AddUShort( (unsigned short)pItem->GetGrid() );

            rBuff.append( buffPool.GetData(), buffPool.GetValidDataSize() );
        }
    }
}

void CEntityItemComponent::CheckDefaultDressPos(EItemClothType pos)
{
    itemid_t nItemID = m_EquipManager.GetItemID(EEquipColumn_ClothOnPlayer, (int)pos);
    ItemEquip * pCloth = (ItemEquip*)m_ItemManager.GetItemByID(nItemID);
    if (pCloth == NULL && !IsConflictEquiped(pos))
    {
        ItemEquip * pDefaultCloth = GetDefaultCloth(pos);
        if (pDefaultCloth != NULL)
        {
            OnEquip(pDefaultCloth);
        }
    }
}

void CEntityItemComponent::OnUseItem(GameMsg_Base & msg,CSlotPeer & SlotPeer)
{
    GameMsg_C2S_UseItem & c2smsg = (GameMsg_C2S_UseItem &)msg;
	int nError = UseItem((EItemColumn)c2smsg.nColumn, c2smsg.nIndex);
	if (EItemError_Success != nError)
	{
		GameMsg_S2C_UseItemFailed msgFailed;
		msgFailed.m_nErrorCode = (unsigned char)nError;
		m_pNetCpt->SendPlayerMsg(&msgFailed);
	}
}

void CEntityItemComponent::OnBatchUseItem(GameMsg_Base & msg,CSlotPeer & SlotPeer)
{
    GameMsg_C2S_BatchUseItem & c2smsg = (GameMsg_C2S_BatchUseItem &)msg;

    int nError = BatchUseItem((EItemColumn)c2smsg.m_nColumn, c2smsg.m_nIndex, c2smsg.m_nItemID, c2smsg.m_nCount);
    if (nError != EItemError_Success)
    {
        GameMsg_S2C_UseItemFailed msgFailed;
        msgFailed.m_nErrorCode = (unsigned char)nError;
        m_pNetCpt->SendPlayerMsg(&msgFailed);
    }
}

void CEntityItemComponent::OnRemoveItem(GameMsg_Base & msg,CSlotPeer & SlotPeer)
{
    GameMsg_C2S_RemoveItem * pMsg = (GameMsg_C2S_RemoveItem *)&msg;
    if(pMsg != NULL && pMsg->nColumn == EItemColumn_WishList)
	{
        //RemoveItem((EItemColumn)pMsg->nColumn, pMsg->nIndex, -1, true, EItemAction_Del_RemoveItem);
    }
}

void CEntityItemComponent::OnAddItemColumn(GameMsg_Base & msg,CSlotPeer & SlotPeer)
{
    GameMsg_C2S_ItemColumn * pMsg = (GameMsg_C2S_ItemColumn *)&msg; 
    if(pMsg != NULL && pMsg->nColumn == (unsigned short)EEquipColumn_BadgeOnPlayer)
    {
        if (m_pRoleAttr != NULL && pMsg->nAddIndex >= 2 && pMsg->nAddIndex < BADGE_BAG_MAXNUM )
        {
            GameMsg_S2C_ItemColumn msgReturn;
            msgReturn.nResult = 1;

            if ( pMsg->nAddIndex == m_pRoleAttr->GetBadgeGridNum() )
            {
                switch (pMsg->nAddIndex)
                {
                case 2:
                    {
						  if (!SearchItem(ITEMID_EXTEND_BADAGE_GRID_CARD_PRIMARY))
                        {
                            msgReturn.strError = "Item_Need_Primary_Badage_Card";		
                        } 
						else 
						{
                            RemoveItemByType(ITEMID_EXTEND_BADAGE_GRID_CARD_PRIMARY, 1, EItemAction_Del_UseItem);
                            msgReturn.nResult = 0;
                        }
                    }
                    break;
                case 3:
                    {
						  if (!SearchItem(ITEMID_EXTEND_BADAGE_GRID_CARD_INTERMEDIATE))
                        {
                            msgReturn.strError = "Item_Need_Intermediate_Badage_Card";		
                        } else {
                            RemoveItemByType(ITEMID_EXTEND_BADAGE_GRID_CARD_INTERMEDIATE, 1, EItemAction_Del_UseItem);
                            msgReturn.nResult = 0;
                        }
                    }
                    break;
                case 4:
                    {
						  if (!SearchItem(ITEMID_EXTEND_BADAGE_GRID_CARD_LEVEL_ADVANCED))
                        {
                            msgReturn.strError = "Item_Need_Advanced_Badge_Card";		
                        } else {
                            RemoveItemByType(ITEMID_EXTEND_BADAGE_GRID_CARD_LEVEL_ADVANCED, 1, EItemAction_Del_UseItem);
                            msgReturn.nResult = 0;
                        }
                    }
                    break;
                default:
                    break;
                }

                if (msgReturn.nResult == 0) 
                {
                    m_pRoleAttr->SetBadgeGridNum(m_pRoleAttr->GetBadgeGridNum() + 1);
                    m_EquipManager.AddSlotCount(EEquipColumn_BadgeOnPlayer, 1);
                }
                msgReturn.nColumn = (unsigned short)EEquipColumn_BadgeOnPlayer;
                msgReturn.nTotalGridNum = m_pRoleAttr->GetBadgeGridNum();
            }
            else
            {
                msgReturn.strError = "Item_Need_Badage_Open_Low";
            }

            if (m_pNetCpt != NULL)
            {
                m_pNetCpt->SendPlayerMsg(&msgReturn);
            }
        }
    }
}

void CEntityItemComponent::InitializeRoleEffect(const std::set<unsigned int> & setRoleEffect, const std::map<unsigned short,std::map<unsigned char,std::string> > & handbookProgressData,
    const std::map<unsigned short, std::map<unsigned char, std::string> > & colorProgressData )
{
    m_ClothEffect.CreateFromDB(m_pRoleAttr->GetRoleID(), setRoleEffect, handbookProgressData, colorProgressData);
}

void CEntityItemComponent::InitializeClothEffect(const std::map<itemid_t,unsigned int> & mapClothEffect)
{
    std::map<itemid_t,unsigned int>::const_iterator it = mapClothEffect.begin();
    for (; it != mapClothEffect.end(); ++it)
    {
        // 特效存在
        ClothEffectConfig * pEffectConfig = ConfigManager::Instance().GetEffectConfigMgr().GetClothEffectConfig(it->second);
        if (pEffectConfig == NULL)
        {
            continue;
        }

        // 物品存在
        ItemBase * pItemBase = m_ItemManager.GetItemByID(it->first);
        if (pItemBase == NULL)
        {
            continue;
        }

        if (!pItemBase->IsCanAddEffectCloth())
        {
            continue;
        }

        ((ItemEquip*)pItemBase)->SetEffectID(it->second);
    }
}

void CEntityItemComponent::InitializeBag()
{
    for(unsigned short i = 0; i < EItemColumn_Max; i++)
    {
        int nMaxSlotCount = CLogicInitData::Instance().GetColumnMaxSlotNum((EItemColumn)i);
        int nMaxOpenSlot = nMaxSlotCount;

        if (i == EItemColumn_BadgeOnPlayer)
        {
            nMaxOpenSlot = m_pRoleAttr->GetBadgeGridNum();
        }

		//m_ItemManager.InitializeBag((EItemColumn)i, nMaxSlotCount, nMaxOpenSlot);
    }
}

void CEntityItemComponent::BeginCollectEquipChange()
{
    m_mapCollectRefresh.clear();
    m_bCollectingRefreshChange = true;
}

void CEntityItemComponent::EndCollectEquipChange()
{
    m_bCollectingRefreshChange = false;

    GameMsg_S2C_BatchRefreshItem refreshmsg;
    std::map<itemgrid_t, itemid_t>::const_iterator it = m_mapCollectRefresh.begin();
    for (; it != m_mapCollectRefresh.end(); ++it)
    {
        ItemBase * pItemBase = m_ItemManager.GetItemByID(it->second);
        refreshmsg.m_mapChangeItem.insert(make_pair(it->first, pItemBase));
    }
    m_pNetCpt->SendPlayerMsg(&refreshmsg);
}

void CEntityItemComponent::InitializeClothGroup(const std::list<RoleClothGroup> & listClothGroup, const CRoleVIPInfo & vip)
{
    for (unsigned char i = 1; i <= DEFAULT_CLOTH_GROUP_COUNT; ++i)
    {
        RoleClothGroup& clothGroup = m_mapClothGroup[i];
        clothGroup.SetGroupID( i );
        clothGroup.SetVIPRequest( 0 );
    }

    const VIPLevelPrivInfoContainer & mapVIPLevelPrivInfo = CVIPDataMgr::Instance().VIPLevelUpContainer();
    for (VIPLevelPrivInfoContainer::const_iterator it = mapVIPLevelPrivInfo.begin();
        it != mapVIPLevelPrivInfo.end(); ++it)
    {
        std::map<unsigned char, RoleClothGroup>::iterator fit = m_mapClothGroup.find((unsigned char)it->second->m_nClothGroupCount);
        if ( it->second->m_nClothGroupCount > 0 && fit == m_mapClothGroup.end() )
        {
            RoleClothGroup& clothGroup = m_mapClothGroup[(unsigned char)it->second->m_nClothGroupCount];
            clothGroup.SetGroupID( (unsigned char)it->second->m_nClothGroupCount );
            clothGroup.SetVIPRequest( (unsigned short)it->first );
        }
    }

    for (std::list< RoleClothGroup >::const_iterator it = listClothGroup.begin();
        it != listClothGroup.end(); ++it)
    {
        RoleClothGroup& clothGroup = m_mapClothGroup[it->GetGroupID()];
        clothGroup.SetGroupID( it->GetGroupID() );
        clothGroup.SetGroupName( it->GetGroupName() );
        clothGroup.SetClothes( it->GetClothes() );
    }

    if (m_pRoleAttr->GetClothGroupID() == 0)
    {
        m_pRoleAttr->SetClothGroupID(1);
    }

    std::map<unsigned char, RoleClothGroup>::iterator itr = m_mapClothGroup.find(m_pRoleAttr->GetClothGroupID());
    if (itr != m_mapClothGroup.end())
    {
        if (itr->second.IsEmpty())
        {
            _InitGroupClothByDefault(itr->first);
        }

        std::list<ItemEquip*> listOnEquip;
        std::map<itemtype_t, stringw> mapError;
        CRoleVIPInfo * pVipInfo = const_cast <CRoleVIPInfo*>(&vip);
        std::map<unsigned char, itemtype_t> mapCheckClothGroup = itr->second.GetClothes();
        _CheckClothGroup( mapCheckClothGroup, listOnEquip, mapError, pVipInfo );
        itr->second.SetClothes( mapCheckClothGroup );

        for (std::list<ItemEquip*>::iterator itt = listOnEquip.begin(); itt != listOnEquip.end(); ++itt)
        {
            OnEquip(*itt, false, pVipInfo);
        }

        EquipIntegrityCheck();
        _RefreshClothGroup(NULL);
    }
}

void CEntityItemComponent::Destroy()
{
    m_ItemManager.Destroy();
	m_BagManager.Destory();
    m_EquipManager.Destory();
    m_listDefaultClothID.clear();
    m_mapClothGroup.clear();
    m_ClothEffect.Destory();
}

void CEntityItemComponent::OnUpdate(const unsigned long & nTimeElapsed)
{
    m_ItemUpdateDelay += nTimeElapsed;
    if (m_ItemUpdateDelay >= ITEM_UPDATE_DELAY)
    {
        m_ItemUpdateDelay -= ITEM_UPDATE_DELAY;

        std::list<ItemBase*> listItem;
        m_ItemManager.CheckItemDueTime(listItem);
        if (!listItem.empty())
        {
            std::list<ItemBase*>::iterator it = listItem.begin();
            for (; it != listItem.end(); ++it)
            {
                OnItemDueTime(*it);
            }
        }
    }

    if ( m_bPerpetualEffectScoreChange )
    {
        GameMsg_S2C_RefreshPerpetualEffectScore refreshScore;
        refreshScore.m_nScore = m_nPerpetualEffectScore;

        m_pNetCpt->SendPlayerMsg( &refreshScore );
        m_bPerpetualEffectScoreChange = false;
    }

    if ( m_bPerpetualEquipCountChange )
    {
        GameMsg_S2C_RefreshPerpetualEquipCount refreshCount;
        refreshCount.m_nCount = m_nPerpetualEquipCount;

        m_pNetCpt->SendPlayerMsg( &refreshCount );
        m_bPerpetualEquipCountChange = false;
    }
}

void CEntityItemComponent::OnEvent(CComponentEvent& refEvent )
{
}

void CEntityItemComponent::GetAllPerpetualEquip(std::list<ItemEquip*> & listCloth)
{
    std::list<itemid_t> listItemID;
    m_EquipManager.GetAllItemID(EEquipColumn_BadgeOnPlayer, listItemID);
    m_EquipManager.GetAllItemID(EEquipColumn_WeddingRingOnPlayer, listItemID);
    m_BagManager.GetAllItemID(EBagColumn_ClothBag, listItemID);
    m_BagManager.GetAllItemID(EBagColumn_Storage, listItemID);
    m_BagManager.GetAllItemID(EBagColumn_BadgeBag, listItemID);

    std::list<itemid_t>::iterator it = listItemID.begin();
    for (; it != listItemID.end(); ++it)
    {
        ItemBase * pItemBase = m_ItemManager.GetItemByID(*it);
        if (pItemBase != NULL && !IsDefaultCloth(*it) && pItemBase->IsEquip() 
            && ((ItemEquip*)pItemBase)->IsPerpetual())
        {
            listCloth.push_back(((ItemEquip*)pItemBase));
        }
    }
}

void CEntityItemComponent::GetEquipedCloth(std::list<itemtype_t> & listItemType)
{
	std::list<itemid_t> listItemID;
	m_EquipManager.GetEquipedCloth(listItemID);

	std::list<itemid_t>::iterator it = listItemID.begin();
	for (; it != listItemID.end(); ++it)
	{
		ItemBase * pItemBase = m_ItemManager.GetItemByID(*it);
		if (pItemBase != NULL)
		{
			listItemType.push_back(pItemBase->GetType());
		}
	}
}

void CEntityItemComponent::GetAllHorseTypeId(std::set<int> & setItemType)
{
    std::list<itemid_t> listItemID;
    m_BagManager.GetAllItemID(EBagColumn_ClothBag, listItemID);

    std::list<itemid_t>::iterator it = listItemID.begin();
    for ( ; it != listItemID.end(); ++it)
    {
        ItemBase * pItemBase = m_ItemManager.GetItemByID(*it);
        if (pItemBase != NULL && pItemBase->IsHorse() )
        {
            setItemType.insert( pItemBase->GetType() );
        }
    }
    listItemID.clear();

    m_BagManager.GetAllItemID(EBagColumn_Storage, listItemID);
    it = listItemID.begin();
    for (; it != listItemID.end(); ++it)
    {
        ItemBase * pItemBase = m_ItemManager.GetItemByID(*it);
        if (pItemBase != NULL && pItemBase->IsHorse())
        {
            setItemType.insert(pItemBase->GetType());
        }
    }
}


void CEntityItemComponent::releaseAllCDItem()
{
	m_ItemManager.ClearAllInCoolingItem();
}

int CEntityItemComponent::CanAddOverlayItem(ItemConfig * pItemConfig, unsigned short nCount)
{
    if (nCount <= 0 || pItemConfig == NULL)
    {
        return EItemUse_Unknown;
    }

    EItemColumn nColumn = (EItemColumn)ItemType2Column(pItemConfig->GetType()); 
    if(nColumn == EItemColumn_Invalid)
    {
        return EItemUse_ConfigError;
    }

    std::list<itemid_t> listItemID;
    m_ItemManager.SearchItemByType(pItemConfig->GetType(), listItemID);
	int nBagColumn = (int)m_BagManager.GetBagColumn(nColumn);
    if (nCount > GetColumnPutInMaxCount(nBagColumn, pItemConfig->GetType(), listItemID))
    {
        return EItemUse_ColumnFull;
    }

    return EItemError_Success;
}

int CEntityItemComponent::CanAddUniqueItem(ItemConfig * pItemConfig, unsigned short nCount, bool bAllowUniqueEquip)
{
    if(pItemConfig == NULL)
    {
        return EItemUse_ConfigError;
    }

    if (nCount > 1 || nCount <= 0)
    {
        return EItemUse_Unknown;
    }

    std::list<itemid_t> listItemID;
    m_ItemManager.SearchItemByType(pItemConfig->m_nType, listItemID);
    if (listItemID.size() > 1)
    {
        return EItemUse_Unknown;
    }
    else if (listItemID.size() == 1)
    {
        ItemBase * pItemBase = m_ItemManager.GetItemByID(*listItemID.begin());
        if (pItemBase == NULL)
        {
            return EItemUse_Unknown;
        }

        if (pItemBase->GetDueTime() <= 0 && !bAllowUniqueEquip)
        {
            return EItemUse_UniqueExist;
        }
    }
    else
    {
        EItemColumn nColumn = (EItemColumn)ItemType2Column(pItemConfig->GetType()); 
        if(nColumn == EItemColumn_Invalid)
        {
            return EItemUse_ConfigError;
        }
		
		EBagColumn eBagColumn = m_BagManager.GetBagColumn(nColumn);
		if (m_BagManager.GetColumnEmptyCount(eBagColumn) <= 0)
		{
			return EItemUse_ColumnFull;
		}
    }

    return EItemError_Success;
}

int CEntityItemComponent::CanAddItem(itemtype_t nItemType,unsigned short nCount,int nValidTime,unsigned short & nStackNumber, bool bAllowUniqueEquip)
{
    ItemConfig * pItemConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
    if(pItemConfig == NULL || pItemConfig->m_nMaxStackCount== 0)
    {
        return EItemUse_ConfigError;
    }

    if (pItemConfig->IsUnique())
    {
        return CanAddUniqueItem(pItemConfig, nCount, bAllowUniqueEquip);
    }
    else
    {
        return CanAddOverlayItem(pItemConfig, nCount);
    }
}

int CEntityItemComponent::AddNewItem(ItemConfig * pItemConfig, int nCount, int nDueTime, EItemAction eItemAction, bool bNoticeClient, bool bUsed, int nDynamicBoxRelatedValue)
{
	// item manager
    nDueTime = nDueTime > 0 ? (nDueTime + (int)time(NULL)) : nDueTime;
	ItemBase * pItemBase = m_ItemManager.AddItem(pItemConfig, nCount, nDueTime, 0, nDynamicBoxRelatedValue);
	if (pItemBase == NULL)
	{
        return EItemUse_Unknown;
	}

	// bag manager
	int nError = m_BagManager.AddItem(pItemBase);
	if (EItemError_Success != nError)
	{
		return nError;
	}

	// role
	OnItemAddToRole(pItemBase);

	// DB
	InsertItemToDB(pItemBase, pItemBase->GetColumn(), pItemBase->GetGrid());

	//log
	_LogRoleItem(m_pRoleAttr->GetRoleID(), eItemAction, (EItemColumn)pItemBase->GetColumn(), pItemBase->GetItemID(), pItemBase->GetType(),
		0, 0, pItemBase->GetCount(), pItemBase->GetDueTime(), 0);
    WriteLog(LOGLEVEL_DEBUG, "item_log: addnewitem roleid = %d, line = %d, action = %d, column = %d, slot = %d, itemid = %lld, typeid = %u, count = %d, duetime = %d, time = %u", 
        m_pRoleAttr->GetRoleID(), g_nLine, (int)eItemAction, pItemBase->GetColumn(), pItemBase->GetGrid(), pItemBase->GetItemID(), pItemBase->GetType(),
        pItemBase->GetCount(), pItemBase->GetDueTime(), (unsigned int)time(NULL));

	// cloth effect
	if (pItemBase->IsCloth() && pItemBase->GetEffectIDConfig() > 0 && pItemBase->IsCanAddEffectCloth())
	{
		AddClothEffect(pItemBase->GetEffectIDConfig(), (ItemEquip*)pItemBase, false);
	}

	if (bNoticeClient)
	{
		RefreshClientItem(pItemBase->GetColumn(), pItemBase->GetGrid(), pItemBase->GetItemID());
	}

	if (bUsed)
	{
		int nError = UseItem(pItemBase->GetColumn(), pItemBase->GetGrid());
		if (EItemError_Success != nError)
		{
			GameMsg_S2C_UseItemFailed msgFailed;
			msgFailed.m_nErrorCode = (unsigned char)nError;
			m_pNetCpt->SendPlayerMsg(&msgFailed);
		}
	}

    return EItemError_Success;
}


void CEntityItemComponent::UpdateItemDueTime(ItemBase * pItemBase, int nAddTime, EItemAction eItemAction, bool bNoticeClient, bool bUsed)
{
	if (pItemBase == NULL )
	{
		return;
	}

	// 更新物品的时间
	if (nAddTime == 0)
	{
		nAddTime = pItemBase->GetDueTimeConfig();
	}

	int nOldDueTime = pItemBase->GetDueTime();
	pItemBase->AddDuration(nAddTime);
	m_ItemManager.UpdateItemDueTime(nOldDueTime, pItemBase->GetDueTime(), pItemBase->GetItemID());
    m_BagManager.SyncItemChange(pItemBase);

	// DB
	UpdateItemToDB(pItemBase);

	//log
    _LogRoleItem(m_pRoleAttr->GetRoleID(), eItemAction, (EItemColumn)pItemBase->GetColumn(), pItemBase->GetItemID(), pItemBase->GetType(),
		pItemBase->GetCount(), nOldDueTime, pItemBase->GetCount(), pItemBase->GetDueTime(), 0);
    WriteLog(LOGLEVEL_DEBUG, "item_log: updatetime roleid = %d, line = %d, action = %d, column = %d, slot = %d, itemid = %lld, typeid = %u, count = %d, duetime = %d, time = %u",
        m_pRoleAttr->GetRoleID(), g_nLine, (int)eItemAction, pItemBase->GetColumn(), pItemBase->GetGrid(), pItemBase->GetItemID(), pItemBase->GetType(),
        pItemBase->GetCount(), pItemBase->GetDueTime(), (unsigned int)time(NULL));

	// sync to client
	if (bNoticeClient)
	{
		RefreshClientItem(pItemBase->GetColumn(), pItemBase->GetGrid(), pItemBase->GetItemID());
	}

    // used
	if (bUsed)
	{
		int nError = UseItem(pItemBase->GetColumn(), pItemBase->GetGrid());
		if (EItemError_Success != nError)
		{
			GameMsg_S2C_UseItemFailed msgFailed;
			msgFailed.m_nErrorCode = (unsigned char)nError;
			m_pNetCpt->SendPlayerMsg(&msgFailed);
		}
	}

    // storage item
    if (pItemBase->GetColumn() == (int)EBagColumn_Storage)
    {
        GameMsg_S2C_NoticeItemInStorage inmsg;
        inmsg.m_nItemType = pItemBase->GetType();
        SendPlayerMsg(&inmsg);
    }
}

int CEntityItemComponent::AddOverlayItem(ItemConfig * pItemConfig, int nCount, EItemAction eItemAction, __int64 nSrcData, bool bNoticeClient, bool bAddUse, int nDynamicBoxRelatedValue)
{
    std::list<itemid_t> listExistItemID;
    m_ItemManager.SearchItemByType(pItemConfig->GetType(), listExistItemID);

    int nCanAddCount = (int)GetColumnPutInMaxCount((int)EBagColumn_ExpandableBag, pItemConfig->GetType(), listExistItemID);
    if (nCount > nCanAddCount)
    {
        return EItemUse_ColumnFull;
    }

    std::list<itemid_t>::iterator it = listExistItemID.begin();
    for (; it != listExistItemID.end(); ++it)
    {
        if (nCount <= 0)
        {
            break;
        }

        ItemBase * pItemBase = m_ItemManager.GetItemByID(*it);
        if (pItemBase != NULL && pItemBase->GetColumn() != (int)EBagColumn_Storage)
        {
            int nCanAddCount = (int)(pItemConfig->m_nMaxStackCount - pItemBase->GetCount());
            if (nCanAddCount > 0)
            {
                int nAddCount = nCanAddCount >= nCount ? nCount : nCanAddCount;
                UpdateItemCount(pItemBase, nAddCount, eItemAction, bNoticeClient, bAddUse);
                nCount -= nAddCount;
            }
        }
    }

    if (nCount >= 1)
    {
        int nNeedSlotCount = pItemConfig->m_nMaxStackCount < 1 ? 1 : 1 + (nCount - 1) / pItemConfig->m_nMaxStackCount;
        for (int i = 1; i <= nNeedSlotCount; ++i)
        {
            int nAddCount = pItemConfig->m_nMaxStackCount >= nCount ? nCount : pItemConfig->m_nMaxStackCount;
            AddNewItem(pItemConfig, nAddCount, 0, eItemAction, bNoticeClient, bAddUse, nDynamicBoxRelatedValue);
            nCount -= nAddCount;
        }
    }

    return EItemError_Success;
}

int CEntityItemComponent::AddUniqueItem(ItemConfig * pItemConfig, int nCount, int nValidTime, EItemAction eItemAction, __int64 nSrcData, bool bNoticeClient, bool bUsed)
{
    if (pItemConfig == NULL)
    {
        return EItemUse_ConfigError;
    }

    ItemBase * pItemBase = m_ItemManager.GetItemByType(pItemConfig->m_nType);
    if (pItemBase != NULL)
    {
        if (pItemBase->GetDueTime() <= 0)
        {
            return EItemUse_UniqueExist;
        }

        UpdateItemDueTime(pItemBase, nValidTime, eItemAction, bNoticeClient);

        if (bUsed)
        {
            int nError = UseItem(pItemBase->GetColumn(), pItemBase->GetGrid());
            if (EItemError_Success != nError)
            {
                GameMsg_S2C_UseItemFailed msgFailed;
                msgFailed.m_nErrorCode = (unsigned char)nError;
                m_pNetCpt->SendPlayerMsg(&msgFailed);
            }
        }
    }
    else
    {
        if (m_BagManager.GetColumnEmptyCount(EBagColumn_ClothBag) <= 0)
        {
            return EItemUse_ColumnFull;
        }

        // item manager
        return AddNewItem(pItemConfig, nCount, nValidTime, eItemAction, bNoticeClient, bUsed);
    }

    return EItemError_Success;
}

bool CEntityItemComponent::AddItem(const std::list<CItem> & listItem, EItemAction eItemAction, __int64 nSrcData, bool bNoticeClient)
{
    if (CanAddAllItem(listItem) != EItemError_Success)
    {
        return false;
    }

    std::list<CItem>::const_iterator it = listItem.begin();
    for (; it != listItem.end(); ++it)
    {
        const CItem & rItem = *it;
        AddItem(rItem.m_nItemType, rItem.m_nItemCount, rItem.m_nValidTime, eItemAction, nSrcData, bNoticeClient, false);
    }

    return true;
}

int CEntityItemComponent::AddItem(itemtype_t nItemType, unsigned short nCount, int nDueTime, EItemAction eItemAction, __int64 nSrcData, bool bNoticeClient, bool bUsed, int nDynamicBoxRelatedValue)
{
    ItemConfig * pItemConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
    if (pItemConfig == NULL)
    {
        return EItemUse_ConfigError;
    }

    int nError = EItemError_Success;
    if (pItemConfig->IsUnique())
    {
        nError = AddUniqueItem(pItemConfig, nCount, nDueTime, eItemAction, nSrcData, bNoticeClient, bUsed);
    }
    else
    {
        nError = AddOverlayItem(pItemConfig, nCount, eItemAction, nSrcData, bNoticeClient, bUsed, nDynamicBoxRelatedValue);
    }

    if (nError == EItemError_Success)
    {
        // 任务
        m_pRoleQuestNew->OnItemChange();

        // 新手七日目标(金券购买)
        if (m_pRoleQuest != NULL && eItemAction == EItemAction_Add_BuyInMall)
        {
            m_pRoleQuest->OnBuyItem();
        }

        if ( (nDueTime == 0 || nDueTime == -1) && pItemConfig->IsEquip() && !pItemConfig->IsBadge_Type_WeddingRing() )
        {
            int nScore = GetPerpetualEffectEquipScore( (ItemEquip *)m_ItemManager.GetItemByType( nItemType ) );

            UpdatePerpetualEquipCount( nCount );
            UpdateEquipPerpetualEffectScore( (unsigned int)nScore, 0 );

            if ( m_pRankComponent != NULL )
            {
                m_pRankComponent->SubmitRankValue( ERankDataType_Cloths, (int)nCount );
                m_pRankComponent->SubmitRankValue( ERankDataType_ClothEffect, nScore );
            }
        }

        CommonParam param;
        param.SetParam("medal", "additem");
        param.SetParam("item_type", (int)nItemType);
        param.SetParam("item_action", (int)eItemAction);
        param.SetParam("item_count", (int)nCount);
        param.SetParam("item_duration", nDueTime);
        param.SetParam(std::string("cloth_effect"), m_nPerpetualEffectScore);
        param.SetParam(std::string("equip_count"), m_nPerpetualEquipCount);
        NotifyAll(param);
    }

	return nError;
}

void CEntityItemComponent::DeleteWeddingRing()
{
    std::list<itemid_t> listItemID; 
    m_BagManager.GetAllItemID(EBagColumn_BadgeBag, listItemID);
    m_EquipManager.GetAllItemID(EEquipColumn_WeddingRingOnPlayer, listItemID);

    std::list<itemid_t>::iterator it = listItemID.begin();
    for ( ; it != listItemID.end(); ++it )
    {
		ItemBase * pItemBase = m_ItemManager.GetItemByID(*it);
        if (pItemBase != NULL && pItemBase->IsWeddingRing())
        {
			RemoveItemByType(pItemBase->GetType(), pItemBase->GetCount(), EItemAction_Del_Divorce);
        }
    }
}

unsigned int CEntityItemComponent::GetItemCount(itemtype_t nItemType, bool bIncludeEquipedClothCapsule, bool bIncludeEquipedBadge)
{
    std::list<itemid_t> listItemID;
    m_ItemManager.SearchItemByType(nItemType, listItemID);

    unsigned int nReturnValue = 0;
    std::list<itemid_t>::iterator it = listItemID.begin();
    for (; it != listItemID.end(); ++it)
    {
        ItemBase * pItemBase = m_ItemManager.GetItemByID(*it);
        if (pItemBase != NULL)
        {
            //已装备的服饰
            if (!bIncludeEquipedClothCapsule && pItemBase->IsCloth() &&((ItemEquip*)pItemBase)->IsEquiped())
            {
                continue;
            }

            //已装备的徽章
            if (!bIncludeEquipedBadge && pItemBase->IsBadge() && ((ItemEquip*)pItemBase)->IsEquiped())
            {
                continue;
            }

            nReturnValue += pItemBase->GetCount();
        }
    }

    return nReturnValue;
};

unsigned int CEntityItemComponent::GetAllPerpetualEquipCount(bool bIncludeBadge, bool bIncludeWeddingRing)
{
    std::list<ItemEquip*> listEquip;
    GetAllPerpetualEquip(listEquip);

    unsigned int nCount = 0;
    for (std::list<ItemEquip*>::iterator it = listEquip.begin(); it != listEquip.end(); ++it)
    {
        if ((*it) == NULL)
        {
            continue;
        }

        if (!bIncludeBadge && (*it)->IsBadge() && !(*it)->IsWeddingRing())
        {
            continue;
        }

        if (!bIncludeWeddingRing && (*it)->IsWeddingRing())
        {
            continue;
        }
        nCount++;
    }

    return nCount;
}

unsigned int CEntityItemComponent::GetPerpetualEffectEquipScore(const ItemEquip * pItemEquip)
{
    if (pItemEquip == NULL || !pItemEquip->IsEquip() || pItemEquip->IsWeddingRing() || pItemEquip->GetDueTime() > 0)
    {
        return 0;
    }

    unsigned int nScore = 0;
    if (CHandbookCfgMgr::Instance().IsEffectCloth(pItemEquip->GetType()))
    {
        nScore += CHandbookCfgMgr::Instance().GetEffectLevelScore(pItemEquip->GetType(),
            (EItemClothType)pItemEquip->GetEquipConfigPos(), pItemEquip->IsBadge());
    }

    if (pItemEquip->GetEffectID() > 0)
    {
        nScore += ConfigManager::Instance().GetEffectConfigMgr().GetClothEffectScore(pItemEquip->GetEffectID(),
            (EItemClothType)pItemEquip->GetEquipConfigPos());
    }
    return nScore;
}

unsigned int CEntityItemComponent::GetClothAttr(itemtype_t nItemType, EClothAttrType eType)
{
    ItemBase * pItemBase = m_ItemManager.GetItemByType(nItemType);
    if (pItemBase == NULL || !pItemBase->IsEquip() || pItemBase->GetDueTime() > 0)
    {
        return 0;
    }

    return CHandbookCfgMgr::Instance().GetClothBaseAttr(nItemType, ((ItemEquip*)pItemBase)->GetEffectID(), eType);
}

unsigned int CEntityItemComponent::GetAllPerpetualEffectEquipScore()
{
    std::list<ItemEquip*> listEquip;
    GetAllPerpetualEquip(listEquip);

    unsigned int nScore = 0;
    for (std::list<ItemEquip*>::iterator it = listEquip.begin(); it != listEquip.end(); ++it)
    {
        nScore += GetPerpetualEffectEquipScore(*it);
    }

    return nScore;
}

ItemBase* CEntityItemComponent::SearchItem(itemtype_t nItemType, bool bSearchInEquiped)
{
	std::list<itemid_t> listItemID;
	m_ItemManager.SearchItemByType(nItemType, listItemID);

	std::list<itemid_t>::iterator it = listItemID.begin();
	for (; it != listItemID.end(); ++it)
	{
		ItemBase * pItemBase = m_ItemManager.GetItemByID(*it);
		if (pItemBase == NULL)
		{
			continue;
		}

		if (!bSearchInEquiped && pItemBase->IsEquip() && ((ItemEquip*)pItemBase)->IsEquiped())
		{
			continue;
		}

		return pItemBase;
	}

	return NULL;
}

bool CEntityItemComponent::IsEquiped(itemtype_t nItemType)
{
	ItemBase * pItemBase = SearchItem(nItemType, true);
	if (pItemBase != NULL && pItemBase->IsEquip())
	{
		return ((ItemEquip*)pItemBase)->IsEquiped();
	}

	return false;
}

bool CEntityItemComponent::RemoveItemByType(itemtype_t nItemType, int nCount, EItemAction eItemAction)
{
	std::list<itemid_t> listItemID;
	m_ItemManager.SearchItemByType(nItemType, listItemID);

    if (IsCanUsed(listItemID, nCount) != EItemError_Success)
    {
		return false;
    }

	// 先找出时效短的删除
	std::map<itemid_t, ItemBase*> mapTime;
	std::list<ItemBase*>   listItemInStorage;
    std::list<ItemBase*>   listItemInBag;
    std::list<CItem>   listStorageUsedItem;
	std::list<itemid_t>::iterator it = listItemID.begin();
	for (; it != listItemID.end(); ++it)
	{
		ItemBase * pItemBase = m_ItemManager.GetItemByID(*it);
		if (pItemBase != NULL)
		{
			// 服饰必须脱掉才能删除
			if (pItemBase->GetColumn() == EItemColumn_ClothBag && pItemBase->IsCloth() &&
				((ItemEquip*)pItemBase)->IsEquiped())
			{
				continue;
			}

			if (pItemBase->GetDueTime() > 0)
			{
				mapTime[pItemBase->GetDueTime()] = pItemBase;
			}
			else
			{
                if (pItemBase->GetColumn() != (int)EBagColumn_Storage)
                {
                    listItemInBag.push_back(pItemBase);
                }
                else
                {
                    listItemInStorage.push_back(pItemBase);
                }
			}
		}
	}

	// 先删除时间短的
    int nNeedDeleteCount = nCount;
	std::map<itemid_t, ItemBase*>::iterator itTime = mapTime.begin();
	for (; itTime != mapTime.end(); ++itTime)
	{
		ItemBase * pItemBase = itTime->second;
        if (pItemBase != NULL && nNeedDeleteCount > 0)
		{
            int nTempCount = pItemBase->GetCount() > nNeedDeleteCount ? nNeedDeleteCount : pItemBase->GetCount();
            if (pItemBase->GetColumn() == (int)EBagColumn_Storage)
            {
                CItem useditem;
                useditem.m_nItemType = pItemBase->GetType();
                useditem.m_nItemCount = (itemcount_t)nTempCount;
                useditem.m_nValidTime = pItemBase->GetDueTime();
                listStorageUsedItem.push_back(useditem);
            }

            if (nTempCount == pItemBase->GetCount())
            {
                DeleteItem(pItemBase, eItemAction);
            }
            else
            {
                UpdateItemCount(pItemBase, (-1)*nTempCount, eItemAction);
            }
			
            nNeedDeleteCount -= nTempCount;
		}
	}

    std::list<ItemBase*>::iterator itStorage = listItemInBag.begin();
    for (; itStorage != listItemInBag.end(); ++itStorage)
    {
        ItemBase * pItemBase = *itStorage;
        if (pItemBase != NULL && nNeedDeleteCount > 0)
        {
            int nTempCount = pItemBase->GetCount() > nNeedDeleteCount ? nNeedDeleteCount : pItemBase->GetCount();
            if (nTempCount == pItemBase->GetCount())
            {
                DeleteItem(pItemBase, eItemAction);
            }
            else
            {
                UpdateItemCount(pItemBase, (-1)*nTempCount, eItemAction);
            }

            nNeedDeleteCount -= nTempCount;
        }
    }

	std::list<ItemBase*>::iterator itTemp = listItemInStorage.begin();
	for (; itTemp != listItemInStorage.end(); ++itTemp)
	{
		ItemBase * pItemBase = *itTemp;
        if (pItemBase != NULL && nNeedDeleteCount > 0)
		{
            int nTempCount = pItemBase->GetCount() > nNeedDeleteCount ? nNeedDeleteCount : pItemBase->GetCount();

            CItem useditem;
            useditem.m_nItemType = pItemBase->GetType();
            useditem.m_nItemCount = (itemcount_t)nTempCount;
            useditem.m_nValidTime = pItemBase->GetDueTime();
            listStorageUsedItem.push_back(useditem);

            if (nTempCount == pItemBase->GetCount())
            {
                DeleteItem(pItemBase, eItemAction);
            }
            else
            {
                UpdateItemCount(pItemBase, (-1)*nTempCount, eItemAction);
            }

            nNeedDeleteCount -= nTempCount;
		}
	}

    // sync storage item used
    if (!listStorageUsedItem.empty())
    {
        GameMsg_S2C_NoticeStorageItemUsed notice;        
        notice.m_listUsedItem = listStorageUsedItem;
        SendPlayerMsg(&notice);
    }

    // cool down time
    UpdateItemCoolDown(nItemType);

    // 任务
    m_pRoleQuestNew->OnUseItem(nItemType, nCount);

    // 成就
    m_pRoleAch->ItemUse(nItemType, (unsigned short)nCount);

	return true;
};

int CEntityItemComponent::CanUnEquip(ItemEquip * pEquip)
{
	if (pEquip == NULL || !pEquip->IsEquip())
	{
		return EItemUse_ParameterErr;
	}

	if (!m_EquipManager.IsEquip(pEquip))
	{
		return EItemUse_ParaError;
	}

	return EItemError_Success;
}

int CEntityItemComponent::CanEquip(ItemEquip * pEquip, CRoleVIPInfo * pVipInfo)
{
    if (pEquip == NULL)
    {
        return EItemUse_InvalidTarget;
    }

    if (pEquip->GetVipLevelConfig() > 0)
    {
        if (pVipInfo != NULL)
        {
            if (!pVipInfo->IsVIP())
            {
                return EItemUse_VIPRequire;
            }

            if (pVipInfo->m_nLevel < pEquip->GetVipLevelConfig())
            {
                return EItemUse_PoorVIPLevel;
            }
        }
        else
        {
            if (!m_pRoleVIP->IsVIP())
            {
                return EItemUse_VIPRequire;
            }

            if (m_pRoleVIP->VIPLevel() < pEquip->GetVipLevelConfig())
            {
                return EItemUse_PoorVIPLevel;
            }
        }
    }

    return EItemError_Success;
}

int CEntityItemComponent::UnEquip(ItemEquip * pEquip )
{
	if (pEquip == NULL)
	{
		return EItemUse_ParaError;
	}

    if (pEquip->IsCloth())
    {
        int nColumn = pEquip->GetColumn(), nSlot = pEquip->GetGrid();
        m_EquipManager.UnEquip(pEquip);
        pEquip->SetGrid(nColumn, nSlot);

        // sync to client
        RefreshClientItem((int)EEquipColumn_ClothOnPlayer, pEquip->GetEquipConfigPos(), 0);
        RefreshClientItem(pEquip->GetColumn(), pEquip->GetGrid(), pEquip->GetItemID());

        // sync to others
        SendClientItemForOther((int)EEquipColumn_ClothOnPlayer, pEquip->GetEquipConfigPos(), NULL);

        if (pEquip->GetColumn() == -1 || pEquip->GetGrid() == -1)
        {
            WriteLog(LOGLEVEL_ERROR, "[Item] UnEquip_Cloth: ItemID = %lld, nColumn = %d, nSlot = %d", pEquip->GetItemID(), pEquip->GetColumn(), pEquip->GetGrid());
        }
    }
    else
    {
        // equip manager
        int nEquipColumn = pEquip->GetColumn();
        int nSlot = pEquip->GetGrid();
        int nError = m_EquipManager.UnEquip(pEquip);
        if (nError != EItemError_Success)
        {
            WriteLog(LOGLEVEL_ERROR, "[Item] UnEquip_Badge_Un: ItemID = %lld, nColumn = %d, nSlot = %d", pEquip->GetItemID(), pEquip->GetColumn(), pEquip->GetGrid());
        }

        // sync to client
        RefreshClientItem(nEquipColumn, nSlot, 0);

        // bag manager
        nError = m_BagManager.AddItem(pEquip);

        // sync to client
        RefreshClientItem(pEquip->GetColumn(), pEquip->GetGrid(), pEquip->GetItemID());

        // sync to others
        SendClientItemForOther(nEquipColumn, nSlot, NULL);

        // gene
        std::list<GeneInfo*> equipGene;
        GetCurEquipGene(equipGene);
        m_pRoleGene->RefreshEquipGene(equipGene);

        // sync to db
        UpdateItemToDB(pEquip);

        if (pEquip->GetColumn() == -1 || pEquip->GetGrid() == -1)
        {
            WriteLog(LOGLEVEL_ERROR, "[Item] UnEquip_Badge: ItemID = %lld, nColumn = %d, nSlot = %d", pEquip->GetItemID(), pEquip->GetColumn(), pEquip->GetGrid());
        }
    }

    { // 同步脱装备数据
        GameMsg_S2G_UnEquipNotify sgMsg;
        sgMsg.m_nItemID = pEquip->GetItemID();
        sgMsg.m_nColumn = (unsigned short)pEquip->GetColumn();
        sgMsg.m_nIndex = (unsigned short)pEquip->GetGrid();
        ((CRoleEntity*)Entity())->SendMsg2GroupServer(&sgMsg);
    }

	
	return EItemError_Success;
}

void CEntityItemComponent::ResumItem()
{
    ItemReadyToRun();
}

void CEntityItemComponent::ItemReadyToRun()
{
//     for(unsigned short nColumnTypeOut=0;nColumnTypeOut<EItemColumn_Max;nColumnTypeOut++)
//     {
// 		if (nColumnTypeOut == EItemColumn_ClothOnPlayer || nColumnTypeOut == EItemColumn_WishList ||
// 			nColumnTypeOut == EItemColumn_TransformClothOnPlayer)
//         {
//             continue;
//         }
// 
// 
// 
//         std::list<itemid_t> listItemID;
//         m_ItemManager.SearchItem((EItemColumn)nColumnTypeOut, listItemID);
// 
//         std::list<itemid_t>::iterator it = listItemID.begin();
//         for ( ; it != listItemID.end(); ++it)
//         {
//             ItemBase * pItemBase = m_ItemManager.GetItemByID(*it);
//             if( pItemBase != NULL )
//             {
//                 OnItemAddToRole( pItemBase );
//             }
//         }
//     }
}

int CEntityItemComponent::UseItem(int nColumn, int nSlot)
{
	ItemBase * pItemBase = GetItem(nColumn, nSlot);
	if (pItemBase == NULL)
	{
		return EItemUse_Unknown;
	}

	int nError = IsCanUsed(pItemBase, 1);
	if (nError  != EItemError_Success)
	{
		return nError;
	}

	if (pItemBase->IsExpandable())
	{
		return UseConsumables(pItemBase);
	}
	else if (pItemBase->IsEquip())
	{
		ItemEquip * pCloth = (ItemEquip*)pItemBase;
		if (pCloth->IsEquiped())
		{
			return UnEquip(pCloth);
		}
		else
		{
			return OnEquip(pCloth, true);
		}
	}

	return EItemError_Success;
}

void CEntityItemComponent::UpdateItemCount(ItemBase * pItemBase, int nCount, EItemAction eItemAction, bool bNoticeClient, bool bUsed, bool bCheckUsetimes)
{
	if (pItemBase == NULL || pItemBase->GetCount() + nCount < 0)
	{
		return;
	}

	// item manager
    int nOldCount = pItemBase->GetCount();
    pItemBase->SetCount(pItemBase->GetCount() + nCount, bCheckUsetimes);
	m_BagManager.SyncItemChange(pItemBase);

	// db
	UpdateItemCountToDB(pItemBase);

    // log
    _LogRoleItem(m_pRoleAttr->GetRoleID(), eItemAction, (EItemColumn)pItemBase->GetColumn(), pItemBase->GetItemID(), pItemBase->GetType(),
        (unsigned short)nOldCount, pItemBase->GetDueTime(), pItemBase->GetCount(), pItemBase->GetDueTime(), 0);
    WriteLog(LOGLEVEL_DEBUG, "item_log: updatecount roleid = %d, line = %d, action = %d, column = %d, slot = %d, itemid = %lld, typeid = %u, count = %d, duetime = %d, time = %u",
        m_pRoleAttr->GetRoleID(), g_nLine, (int)eItemAction, pItemBase->GetColumn(), pItemBase->GetGrid(), pItemBase->GetItemID(), pItemBase->GetType(),
        pItemBase->GetCount(), pItemBase->GetDueTime(), (unsigned int)time(NULL));

	// sync to client
	if (bNoticeClient)
	{
		RefreshClientItem(pItemBase->GetColumn(), pItemBase->GetGrid(), pItemBase->GetItemID());
	}
	
	// use 
	if (bUsed)
	{
		int nError = UseItem(pItemBase->GetColumn(), pItemBase->GetGrid());
		if (EItemError_Success != nError)
		{
			GameMsg_S2C_UseItemFailed msgFailed;
			msgFailed.m_nErrorCode = (unsigned char)nError;
			m_pNetCpt->SendPlayerMsg(&msgFailed);
		}
	}
}

void CEntityItemComponent::DeleteItem(ItemBase * pItemBase, EItemAction eItemAction)
{
	if (pItemBase == NULL)
	{
		return;
	}

	int nColumn = pItemBase->GetColumn();
	int nSlot = pItemBase->GetGrid();
	itemid_t nItemID = pItemBase->GetItemID();
	itemtype_t nItemType = pItemBase->GetType();
	int nOldCount = pItemBase->GetCount();
	int nDueTime = pItemBase->GetDueTime();

    if (nColumn == -1 || nSlot == -1)
    {
        WriteLog(LOGLEVEL_ERROR, "[Item] DeleteItem: nColumn = %d, nSlot = %d", nColumn, nSlot);
    }

	if (pItemBase->IsEquip() && ((ItemEquip*)pItemBase)->IsEquiped())
	{
        if (pItemBase->IsCloth())
        {
            // bag manager
            m_BagManager.RemoveItem(pItemBase);
        }

        // equip manager
        m_EquipManager.UnEquip((ItemEquip*)pItemBase);

        // sync to client
        RefreshClientItem(nColumn, nSlot, 0);

        // check integrity
        if (pItemBase->IsCloth() && ((ItemEquip*)pItemBase)->RequireIntegrityCheck())
        {
            EquipIntegrityCheck();
        }

        // refresh gene
        std::list<GeneInfo*> equipGene;
        GetCurEquipGene(equipGene);
        m_pRoleGene->RefreshEquipGene(equipGene);
    }
    else
    {
        // bag manager
        m_BagManager.RemoveItem(pItemBase);

        // sync to client
        RefreshClientItem(nColumn, nSlot, 0);
    }

    // DB
    DeleteItemToDB(pItemBase);

	// item manager
	m_ItemManager.RemoveItem(nItemID);

	// quset
    m_pRoleQuestNew->OnItemChange();

    // log
    _LogRoleItem(m_pRoleAttr->GetRoleID(), eItemAction, (EItemColumn)nColumn, nItemID, nItemType,
        (unsigned short)nOldCount, nDueTime, 0, 0, 0);
    WriteLog(LOGLEVEL_DEBUG, "item_log: delete roleid = %d, line = %d, action = %d, column = %d, slot = %d, itemid = %lld, typeid = %u, count = %d, duetime = %d, time = %u",
        m_pRoleAttr->GetRoleID(), g_nLine, (int)eItemAction, nColumn, nSlot, nColumn, nItemType, nOldCount, nDueTime, (unsigned int)time(NULL));
}

void CEntityItemComponent::RefreshItemGene(itemtype_t nItemType)
{
    ItemConfig * pConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
    if (pConfig == NULL)
    {
        return;
    }

    std::list<GeneInfo *> ::iterator aBegin = pConfig->m_listGene.begin();
    while (aBegin != pConfig->m_listGene.end())
    {
        GeneInfo * info = (*aBegin);
        if (info != NULL && info->m_nGeneID > 0)
        {
            m_pRoleGene->AddGene(info->m_nGeneID, info->m_nParam1, info->m_nParam2, info->m_strParam.c_str(), m_pNetCpt->GetPlayerLogicIndex());
        }
        aBegin++;
    }
}

void CEntityItemComponent::RefreshItemGene(itemtype_t nItemType, int nCount, GeneInfo & gene)
{
    ItemConfig * pConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
    if (pConfig == NULL)
    {
        return;
    }

    std::list<GeneInfo *> ::iterator aBegin = pConfig->m_listGene.begin();
    while (aBegin != pConfig->m_listGene.end())
    {
        GeneInfo * info = (*aBegin);
        if (info != NULL && info->m_nGeneID > 0)
        {
            gene = *info;
            m_pRoleGene->MergeGene(gene, nCount);
            m_pRoleGene->AddGene(gene.m_nGeneID, gene.m_nParam1, gene.m_nParam2, gene.m_strParam.c_str(), m_pNetCpt->GetPlayerLogicIndex());
        }
        aBegin++;
    }
}

int CEntityItemComponent::UseConsumables(ItemBase * pItemBase)
{
	// begin collect change
    m_BagManager.BeginCollectBagChange();

    // update cool down
	UpdateItemCoolDown(pItemBase->GetCoolDownTag(), pItemBase->GetCoolDownTime());

    // achieve
    m_pRoleAch->ItemUse(pItemBase->GetType(), 1);

    // quest
    m_pRoleQuestNew->OnUseItem(pItemBase->GetType());
    m_pRoleQuestNew->OnAttributeChange();

	// handle 
    itemtype_t nItemType = pItemBase->GetType();
    int nDynamicBoxValue = pItemBase->GetDynamicBoxRelatedValue();
    itemid_t nItemID = pItemBase->GetItemID();
	if (pItemBase->IsDeleteByUsed(-1))
	{
        DeleteItem(pItemBase, EItemAction_Del_UseItem);
	}
	else
	{
        UpdateItemCount(pItemBase, -1, EItemAction_Del_UseItem, true, false, true);
	}

    // open package
    ItemConfig * pItemConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
    if (pItemConfig != NULL && pItemConfig->IsPackage() && !OpenPackage(nItemID, nItemType, nDynamicBoxValue))
    {
        return EItemUse_ScriptError;
    }

    // gene
    RefreshItemGene(nItemType);

    // end collect change
    std::list<itemgrid_t> listGrid;
    m_BagManager.EndCollectBagChange(listGrid);

    // sync to client
    RefreshClientItem(listGrid);

    return EItemError_Success;
}

void CEntityItemComponent::MergeItem(std::list<CItem> & listItem)
{
    std::map<itemtype_t,std::list<CItem>::iterator> mapItemLocation;
    std::list<CItem>::iterator it = listItem.begin();
    for (;it != listItem.end();)
    {
        if (it == listItem.begin())
        {
            mapItemLocation[it->m_nItemType] = it;
            ++it;
        }
        else
        {
            std::map<itemtype_t,std::list<CItem>::iterator>::iterator itr = mapItemLocation.find(it->m_nItemType);
            if (itr == mapItemLocation.end())
            {
                mapItemLocation[it->m_nItemType] = it;
                ++it;
            }
            else
            {
                std::list<CItem>::iterator  itItem = itr->second;
                ItemConfig * pItemConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(it->m_nItemType);
                if (pItemConfig != NULL)
                {
                    if (pItemConfig->IsEquip())
                    {
                        if (it->m_nValidTime > 0 && itItem->m_nValidTime > 0)
                        {
                            itItem->m_nValidTime += it->m_nValidTime;
                        }

                        if (itItem->m_nValidTime > 0 && it->m_nValidTime <= 0)
                        {
                            itItem->m_nValidTime = it->m_nValidTime;
                        }

                        listItem.erase(it++);
                    }
                    else
                    {
                        if (itItem->m_nItemCount + it->m_nItemCount <= pItemConfig->m_nMaxStackCount)
                        {
                            itItem->m_nItemCount += it->m_nItemCount;
                            listItem.erase(it++);
                        }
                        else 
                        {
                            if (it->m_nItemCount >= pItemConfig->m_nMaxStackCount)
                            {
                                ++it;
                            }
                            else
                            {
                                int nDeleteCount = pItemConfig->m_nMaxStackCount - itItem->m_nItemCount;
                                it->m_nItemCount -= (itemcount_t)nDeleteCount;
                                itItem->m_nItemCount += (itemcount_t)nDeleteCount;
                                mapItemLocation.erase(itr);
                                mapItemLocation[it->m_nItemType] = it;
                                ++it;
                            }
                        }
                    }
                }
                else
                {
                    ++it;
                }
            }
        }
    }
}

int CEntityItemComponent::BatchUseItem(int nColumnType, int nSelectSlot, itemtype_t nItemType, int nCount)
{
    // 获取物品所在背包格
	std::list<itemid_t> listItemID;
	m_ItemManager.SearchItemByType(nItemType, listItemID);
	int nError = IsCanUsed(listItemID, nCount);
	if (nError != EItemError_Success)
	{
		return nError;
	}

	// 玩家选定物品
	itemid_t nItemID = m_BagManager.GetItemID((EBagColumn)nColumnType, nSelectSlot);
	ItemBase * pSelectItem = m_ItemManager.GetItemByID(nItemID);
    ItemConfig * pSelectItemConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
	if (pSelectItem == NULL || pSelectItemConfig == NULL)
	{
		return EItemUse_InvalidTarget;
	}

	std::list<itemid_t>::iterator it = find(listItemID.begin(), listItemID.end(), nItemID);
	if (it != listItemID.end())
	{
		listItemID.erase(it);
		listItemID.push_front(nItemID);
	}

	m_BagManager.BeginCollectBagChange();
	std::list<CItem> listAddItem;
    std::map<int, int> mapCurrency;
	std::list<itemid_t>::iterator itr = listItemID.begin();
    int nDeleteCount = nCount;
	for (; itr != listItemID.end(); ++itr)
	{
        if (nDeleteCount <= 0)
        {
            break;
        }

		ItemBase * pItemBase = m_ItemManager.GetItemByID(*itr);
		if (pItemBase == NULL)
		{
			continue;
		}

        int nTempCount = nDeleteCount >= pItemBase->GetCount() ? pItemBase->GetCount() : nDeleteCount;
        int nDynamicBoxValue = pItemBase->GetDynamicBoxRelatedValue();
        if (pItemBase->IsDeleteByUsed((-1)*nTempCount))
        {
            DeleteItem(pItemBase, EItemAction_Del_UseItem);
        }
        else
        {
            UpdateItemCount(pItemBase, (-1)*nTempCount, EItemAction_Del_UseItem, true, false, true);
        }

        if (pSelectItemConfig->IsPackage() && !BatchOpenPackage(*itr, nItemType, nDynamicBoxValue, nTempCount, listAddItem, mapCurrency))
        {
            continue;
        }

        nDeleteCount -= nTempCount;
	}

	// 基因效果
    GeneInfo gene_effect;
    RefreshItemGene(nItemType, nCount, gene_effect);

	// 冷却时间
    UpdateItemCoolDown(pSelectItemConfig->m_nCoolDownTag, pSelectItemConfig->m_nCooldownTime);

	// 合并物品
	MergeItem(listAddItem);

	// 给前端反馈
    if (pSelectItemConfig->IsPackage())
    {
        FreshPackageResultToClient(nItemType, nCount, listAddItem, mapCurrency);
    }
    else
    {
        FreshGeneResultToClient(gene_effect, nItemType, nCount);
    }
    
	// 反馈
    m_pRoleAch->ItemUse(nItemType, (unsigned short)nCount);
	m_pRoleQuestNew->OnUseItem(nItemType, nCount);
	m_pRoleQuestNew->OnAttributeChange();

    // 通知给前端
	std::list<itemgrid_t> listGrid;
    m_BagManager.EndCollectBagChange(listGrid);
    RefreshClientItem(listGrid);

    return EItemError_Success;
}

bool CEntityItemComponent::OpenPackage(itemid_t nItemID, itemtype_t nItemType, int nDynamicBoxRelatedValue)
{
    ItemConfig * pConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
    if (pConfig == NULL)
    {
        return false;
    }

    switch (pConfig->m_anType[1])
    {
    case eItemExpendableType_Packet:
        return AddPacketItemToRole(nItemType);

    case eItemExpendableType_Box:
        return AddBoxItemToRole(nItemType);

    case eItemExpendableType_DynamicBox:
        return AddDynamicBoxItemToRole(nItemID, nDynamicBoxRelatedValue, nItemType);

    default:
        return false;
    }
}

bool CEntityItemComponent::BatchOpenPackage(itemid_t nItemID, itemtype_t nItemType, int nDynamicBoxRelatedValue, int nCount, 
        std::list<CItem> & listItem, std::map<int, int> & mapCurrency)
{
    ItemConfig * pConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
    if (pConfig == NULL)
    {
        return false;
    }

    switch (pConfig->m_anType[1])
    {
    case eItemExpendableType_Packet:
        return AddPacthPacketItemToRole(m_pNetCpt->GetPlayerLogicIndex(), nItemType, nCount, listItem, mapCurrency);

    case eItemExpendableType_Box:
        return AddPatchBoxItemToRole(nItemType, nCount, listItem);

    case eItemExpendableType_DynamicBox:
        return AddPatchDynamicBoxItemToRole(nItemID, nDynamicBoxRelatedValue, nItemType, nCount, listItem);

    default:
        return false;
    }
}

void CEntityItemComponent::FreshPackageResultToClient(itemtype_t nItemType, int nCount, std::list<CItem> & addItemList, std::map<int, int> & mapCurrency)
{
    ItemConfig * pConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
    if (pConfig == NULL)
    {
        return;
    }

    switch (pConfig->m_anType[1])
    {
        case eItemExpendableType_Packet:
        {
            GameMsg_S2C_OpenPackageResult msgResult;
            msgResult.m_nCount = nCount;
            msgResult.m_nItemTypeID = nItemType;
            msgResult.m_listItems = addItemList;
            msgResult.m_mapCurrency = mapCurrency;
            SendPlayerMsg(&msgResult);
        }
        break;

        case eItemExpendableType_Box:
        case eItemExpendableType_DynamicBox:
        {
            GameMsg_S2C_OpenBoxResult msgResutl;
            msgResutl.m_nCount = nCount;
            msgResutl.m_nItemType = nItemType;
            msgResutl.m_listItems = addItemList;
            SendPlayerMsg(&msgResutl);
        }
        break;

    default:
        break;
    }
}

void CEntityItemComponent::FreshGeneResultToClient(const GeneInfo & gene, itemtype_t nItemType, int nCount)
{
    std::string strContent;
    switch (gene.m_nGeneID)
    {
    case  EGeneID_AddExpRate:
        {
            SafeFormat(strContent, CLocalization::Instance().GetString("PatchUseItem_Desc_Exp"),
                gene.m_nParam2 / 3600, gene.m_nParam1);
        }
        break;
    case  EGeneID_AddExp:
        {
            SafeFormat(strContent, CLocalization::Instance().GetString("PatchUseItem_Desc_Exp_Value"), gene.m_nParam1);
        }
        break;
    case EGeneID_AddMoneyRate:
        {
            SafeFormat(strContent, CLocalization::Instance().GetString("PatchUseItem_Desc_Money"),
                gene.m_nParam2, gene.m_nParam1);
        }
        break;
    case  EGeneID_AddMoney:
        {
            SafeFormat(strContent, CLocalization::Instance().GetString("PatchUseItem_Desc_Money_Value"), gene.m_nParam1);
        }
        break;
    case  EGeneID_AddSuper:
        {
            SafeFormat(strContent, CLocalization::Instance().GetString("PatchUseItem_Desc_Super"), gene.m_nParam1);
        }
        break;
    case EGeneID_AddVipExp:
        {
            SafeFormat(strContent, CLocalization::Instance().GetString("PatchUseItem_Desc_Up"),
                gene.m_nParam2 / (24 * 3600), gene.m_nParam1);
        }
        break;
    case EGeneID_AddIntimacy:
        {
            SafeFormat(strContent, CLocalization::Instance().GetString("PatchUseItem_Desc_Intimacy"), gene.m_nParam1);
        }
        break;
    case EGeneID_TransForm:
        {
            SafeFormat(strContent, CLocalization::Instance().GetString("PatchUseItem_Desc_TransForm"), gene.m_nParam2);
        }
        break;
    case EGeneID_AddDanceGroupContribution:
        {
            SafeFormat(strContent, CLocalization::Instance().GetString("PatchUseItem_Desc_DanceGroupContribution"), gene.m_nParam1);
        }
        break;
    }

    if (!strContent.empty())
    {
        GameMsg_S2C_PatchUseItemEffect sucMsg;
        sucMsg.m_nItemID = nItemType;
        sucMsg.m_nCount = nCount;
        sucMsg.m_strDesc = strContent;
        SendPlayerMsg(&sucMsg);
    }
}

void CEntityItemComponent::RefreshClientItem(const std::list<itemgrid_t> & listGrid)
{
    GameMsg_S2C_BatchRefreshItem refreshmsg;
	std::list<itemgrid_t>::const_iterator it = listGrid.begin();
    for (; it != listGrid.end(); ++it)
    {
        ItemBase * pItemBase = m_ItemManager.GetItemByID(m_BagManager.GetItemID((EBagColumn)it->first, it->second));
        refreshmsg.m_mapChangeItem.insert(make_pair(*it, pItemBase));
    }

    m_pNetCpt->SendPlayerMsg(&refreshmsg);
}

void CEntityItemComponent::RefreshClientItem(int nColumnType, int nIndex, itemid_t nItemID)
{
    if (m_bCollectingRefreshChange)
    {
        itemgrid_t grid(nColumnType, nIndex);
        m_mapCollectRefresh[grid] = nItemID;
    }
    else
    {
        GameMsg_S2C_RefreshItem msg;
        ItemBase * pItemBase = m_ItemManager.GetItemByID(nItemID);
        msg.m_nItemType = pItemBase != NULL ? pItemBase->GetType() : 0;
        msg.m_mapItems.insert(std::make_pair((unsigned short)nIndex, pItemBase));
        msg.m_nColumn = (unsigned short)nColumnType;
        m_pNetCpt->SendPlayerMsg(&msg);
    }
}

void CEntityItemComponent::SendClientItemForOther(int nColumn, int nSlot, ItemBase * pItem)
{
    if (NULL != pItem && !pItem->IsEquip())
        return;// 其他消息不广播
    //bNotifyOther
    GameMsg_S2C_RefreshItemForOther msg2other;
    msg2other.nRoleID = m_pRoleAttr->GetRoleID();
    if (NULL != pItem)
        msg2other.itemEquip = *((ItemEquip*)pItem);
    msg2other.nColumn = (unsigned short)nColumn;
    msg2other.nIndex = (unsigned short)nSlot;
    msg2other.nItemType = pItem == NULL?0:pItem->GetType();
   
    SendPlayerMsgToOthers(&msg2other);
}

void CEntityItemComponent::GetCurEquipGene(std::list<GeneInfo*>& rEquipGene)
{
    rEquipGene.clear();

    std::list<itemid_t> listItemID;
    m_EquipManager.GetAllItemID(EEquipColumn_BadgeOnPlayer, listItemID);

    std::list<itemid_t>::iterator it = listItemID.begin();
    for ( ; it != listItemID.end(); ++it)
    {
        ItemBase * pItemBase = m_ItemManager.GetItemByID(*it);
        if ( pItemBase != NULL  )
        {
            std::list<GeneInfo *> listGene;
            pItemBase->GetGene(listGene);

            if ( !listGene.empty() )
            {
                list<GeneInfo *> ::iterator aBegin = listGene.begin();
                rEquipGene.insert(rEquipGene.end(), listGene.begin(), listGene.end());
            }
        }
    }

    listItemID.clear();
    m_EquipManager.GetAllItemID(EEquipColumn_WeddingRingOnPlayer, listItemID);

    it = listItemID.begin();
    for ( ; it != listItemID.end(); ++it)
    {
        ItemBase * pItemBase = m_ItemManager.GetItemByID(*it);
        if ( pItemBase != NULL )
        {
            std::list<GeneInfo *> listGene;
            pItemBase->GetGene(listGene);

            if ( !listGene.empty() )
            {
                list<GeneInfo *> ::iterator aBegin = listGene.begin();
                rEquipGene.insert(rEquipGene.end(), listGene.begin(), listGene.end());
            }
        }
    }
}

int  CEntityItemComponent::GetCurWeddingRingGeneEffect()
{
    std::list<GeneInfo*> rEquipGene;

    std::list<itemid_t> listItemID;
    m_EquipManager.GetAllItemID(EEquipColumn_WeddingRingOnPlayer, listItemID);

    std::list<itemid_t>::iterator it = listItemID.begin();
    for ( ; it != listItemID.end(); ++it)
    {
        ItemBase * pItemBase = m_ItemManager.GetItemByID(*it);
        if ( pItemBase != NULL )
        {
            ItemConfig * pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(pItemBase->GetType());
            if ( !pItemInfo->m_listGene.empty() )
            {
                list<GeneInfo *> ::iterator aBegin = pItemInfo->m_listGene.begin();
                rEquipGene.insert( rEquipGene.end(), pItemInfo->m_listGene.begin(), pItemInfo->m_listGene.end() );
            }
        }
    }

    std::list<GeneInfo*>::iterator iter = rEquipGene.begin();
    if ( iter != rEquipGene.end())
    {
        return (*iter)->m_nParam1;
    }

    return 0;
}

void CEntityItemComponent::OnItemDueTime(ItemBase * pItemBase)
{	
    if (pItemBase == NULL)
    {
        return;
    }

    CommonParam param;
    param.SetParam(std::string("ItemDueTime"), pItemBase->GetType());
    NotifyAll(param);

    DeleteItem(pItemBase, EItemAction_Del_ItemExpired);
}

//物品添加到人物身上的时候调用。
void CEntityItemComponent::OnItemAddToRole( const ItemBase*pItem )
{
    if(pItem != NULL && m_pNetCpt != NULL)
    {
        CInitLua::GetInstance().DoString("ItemAdd",enumSCRIPT_RETURN_NONE,0
            ,enumSCRIPT_PARAM_NUMBER,2,pItem->GetType(),m_pNetCpt->GetPlayerLogicIndex(),DOSTRING_PARAM_END);
    }
}

//物品从人物身上移走的时候调用。
void CEntityItemComponent::OnItemRemoveFromRole( const ItemBase*pItem )
{
    if(pItem != NULL && m_pNetCpt != NULL)
    {
        CInitLua::GetInstance().DoString("ItemRemove",enumSCRIPT_RETURN_NONE,0
            ,enumSCRIPT_PARAM_NUMBER,2,pItem->GetType(),m_pNetCpt->GetPlayerLogicIndex(),DOSTRING_PARAM_END);
    }
}

void CEntityItemComponent::InitializeCoolDown(const std::list<CoolDownData> & listCooling)
{
    m_ItemManager.InitializeCoolDown(listCooling);
}

bool CEntityItemComponent::IsItemInCoolDown(itemtype_t nItemType) 
{
    ItemConfig* pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
    if ( pItemInfo != NULL )
    {
        return m_ItemManager.IsInCooling(pItemInfo->m_nCoolDownTag);
    }

    return false;
}

void CEntityItemComponent::UpdateItemCoolDown(itemtype_t nItemType)
{
    ItemConfig * pConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
    if (pConfig != NULL)
    {
        UpdateItemCoolDown(pConfig->m_nCoolDownTag, pConfig->m_nCooldownTime);
    }
}

void CEntityItemComponent::UpdateItemCoolDown(unsigned short nItemCoolDownTag,int nDuration)
{
    if (nItemCoolDownTag <= 0)
    {
        return;
    }

	nDuration += (int)time(NULL);
    m_ItemManager.UpdateCoolingCD(nItemCoolDownTag, nDuration);

    // 通知前端
    GameMsg_S2C_UpdateCoolDown msg;
    msg.m_ItemCoolDown.m_nType = nItemCoolDownTag;
    msg.m_ItemCoolDown.m_nDuration = nDuration-(int)time(NULL);
    SendPlayerMsg(&msg);

    // 更新数据库
    CoolDownData *pCoolDown = new CoolDownData;
    if (pCoolDown != NULL) 
    {
        pCoolDown->m_nType = nItemCoolDownTag;
        pCoolDown->m_nDuration = nDuration;
        CDelHelper delHelper(pCoolDown);
        AddQuery(QUERY_UpdateCoolDown, m_pRoleAttr->GetRoleID(), pCoolDown, 0, NULL, &delHelper);
    }
}

void CEntityItemComponent::InitializeRepeatItem()
{
    std::list<itemid_t> listItemID;
    m_BagManager.GetAllItemID(EBagColumn_ClothBag, listItemID);

    std::set<std::list<itemid_t> > setRepeatItem;
    for (std::list<itemid_t>::iterator it = listItemID.begin(); it != listItemID.end(); ++it)
    {
        ItemBase * pItemBase = m_ItemManager.GetItemByID(*it);
        if (pItemBase != NULL)
        {
            std::list<itemid_t> listRepeat;
            m_ItemManager.SearchItemByType(pItemBase->GetType(), listRepeat);

            if (listRepeat.size() >= 2)
            {
                setRepeatItem.insert(listRepeat);
            }
        }
    }

    for (std::set<std::list<itemid_t> >::iterator its = setRepeatItem.begin(); its != setRepeatItem.end(); ++its)
    {
        unsigned short nEffectLevel = 0;
        itemid_t nSelectItemID = 0;
        for (std::list<itemid_t>::const_iterator itr = its->begin(); itr != its->end(); ++itr)
        {
            ItemBase * pItemBase = m_ItemManager.GetItemByID(*itr);
            if (pItemBase != NULL)
            {
                unsigned int nEffectID = ((ItemEquip*)pItemBase)->GetEffectID();
                ClothEffectConfig * pClothEffectConfig = ConfigManager::Instance().GetEffectConfigMgr().GetClothEffectConfig(nEffectID);
                if (pClothEffectConfig != NULL && pClothEffectConfig->m_nLevel > nEffectLevel)
                {
                    nSelectItemID = *itr;
                    nEffectLevel = pClothEffectConfig->m_nLevel;
                }

                if (itr == its->begin())
                {
                    nSelectItemID = *itr;
                }
            }
        }

        for (std::list<itemid_t>::const_iterator itr = its->begin(); itr != its->end(); ++itr)
        {
            if (*itr != nSelectItemID)
            {
                ItemBase * pItemBase = m_ItemManager.GetItemByID(*itr);
                if (pItemBase != NULL)
                {
                    m_BagManager.RemoveItem(pItemBase);
                    m_ItemManager.RemoveItem(*itr);
                }
            }
        }
    }
}

void CEntityItemComponent::InitializeItem(const std::list<DBItem> & listDBItem)
{
    // load data
    std::list<DBItem> listErrrorData;
    std::list<itemid_t> listEquip;
    for (std::list<DBItem>::const_iterator it = listDBItem.begin(); it != listDBItem.end(); ++it)
    {
        if (it->m_nColumn == (int)EEquipColumn_TransformClothOnPlayer)
        {
            continue;
        }

        // add to itemmanager
        if (!m_ItemManager.LoadItemData(*it))
        {
            continue;
        }

        if (it->m_bEquiped && it->m_nColumn != (int)EBagColumn_ClothBag)
        {
            // add to bag manager
            if (m_EquipManager.InitializeDBData((EEquipColumn)it->m_nColumn, it->m_nIndex, it->m_nItemID) != EItemError_Success)
            {
                listErrrorData.push_back(*it);
            }
        }
        else
        {
            // add to equip manager
            if (m_BagManager.InitalizeDBData((EBagColumn)it->m_nColumn, it->m_nIndex, it->m_nItemID) != EItemError_Success)
            {
                listErrrorData.push_back(*it);
            }
        }
    }

    // dispose error
    std::list<DBItem>::iterator itr = listErrrorData.begin();
    for (; itr != listErrrorData.end(); ++itr)
    {
        ItemBase * pItemBase = m_ItemManager.GetItemByID(itr->m_nItemID);
        if (pItemBase != NULL)
        {
            if (pItemBase->IsEquip())
            {
                ((ItemEquip*)pItemBase)->UnEquip();
            }

            m_BagManager.AddItem(pItemBase);
        }
    }

    // unequip all cloth
    std::list<itemid_t> listCloth;
    m_BagManager.GetAllItemID(EBagColumn_ClothBag, listCloth);
    for (std::list<itemid_t>::iterator itc = listCloth.begin(); itc != listCloth.end(); ++itc)
    {
        ItemBase * pItemBase = m_ItemManager.GetItemByID(*itc);
        if (pItemBase != NULL && ((ItemEquip*)pItemBase)->IsEquiped())
        {
            ((ItemEquip*)pItemBase)->UnEquip();
        }
    }

}

bool CEntityItemComponent::CreateFromDB(ROLEINFO_DB* pRoleInforDB)
{
    // init storage capacity
    int nStorageCapacity = CalcStorageCapacity(pRoleInforDB->m_StorageCapacity, pRoleInforDB->VIPInfo.m_nLevel);

    // init bagmanager and equipmanager
    m_BagManager.Initalize(nStorageCapacity);
    m_EquipManager.Initialize(m_pRoleAttr->GetBadgeGridNum());

    // load data
    InitializeItem(pRoleInforDB->listItem);

    // 初始化物品冷却时间
    InitializeCoolDown(pRoleInforDB->listCoolDown);

	// 添加默认衣服
	AddDefaultColthToPlayer();

    // 初始化服饰特效
    InitializeClothEffect(pRoleInforDB->mapClothEffect);
    InitializeRoleEffect(pRoleInforDB->setRoleEffect, pRoleInforDB->mapHandbookProgress, pRoleInforDB->mapColorProgress);

    // 处理重复物品
    InitializeRepeatItem();

    // 初始化服饰搭配
    InitializeClothGroup(pRoleInforDB->m_listClothGroup, pRoleInforDB->VIPInfo);

    m_nPerpetualEffectScore = GetAllPerpetualEffectEquipScore();
    m_nPerpetualEquipCount = GetAllPerpetualEquipCount( true, false );

    return true;
}

bool CEntityItemComponent::PacketToDB(ROLEINFO_DB*pRoleInforDB) const
{
    if (pRoleInforDB == NULL)
    {
        return false;
    }

    std::list<DBItem> listCloth;
    m_ItemManager.PacketToDB(listCloth);

    for (std::list<DBItem>::iterator it = listCloth.begin(); it != listCloth.end(); ++it)
    {
        if (!IsDefaultCloth(it->m_nItemID))
        {
            pRoleInforDB->listItem.push_back(*it);
        }
    }

    return true;
}

bool CEntityItemComponent::PacketToCache( ROLEINFO_DB* pRoleInforDB ) const
{
    if ( pRoleInforDB == NULL )
        return false;

    // 仓库
    pRoleInforDB->m_StorageCapacity = m_StorageCapacityRecord;

    // 服饰搭配
    pRoleInforDB->m_listClothGroup.clear();
    for ( std::map<unsigned char, RoleClothGroup>::const_iterator groupItr = m_mapClothGroup.begin();
        groupItr != m_mapClothGroup.end(); ++groupItr )
    {
        pRoleInforDB->m_listClothGroup.push_back( groupItr->second );
    }

    // 物品、服饰特效、冷却时间
    pRoleInforDB->listItem.clear();
    pRoleInforDB->mapClothEffect.clear();
    pRoleInforDB->listCoolDown.clear();
    m_ItemManager.PacketToCache( pRoleInforDB->listItem, pRoleInforDB->mapClothEffect, pRoleInforDB->listCoolDown );

    // 删除默认服饰
    for ( std::list<DBItem>::iterator itemItr = pRoleInforDB->listItem.begin(); itemItr != pRoleInforDB->listItem.end(); )
    {
        if ( IsDefaultCloth( itemItr->m_nItemID ) )
        {
            itemItr = pRoleInforDB->listItem.erase( itemItr );
        }
        else
        {
            ++itemItr;
        }
    }

    // 角色特效、图鉴进度、特效进度
    pRoleInforDB->setRoleEffect.clear();
    pRoleInforDB->mapHandbookProgress.clear();
    pRoleInforDB->mapColorProgress.clear();
    m_ClothEffect.PacketToCache( pRoleInforDB->setRoleEffect, pRoleInforDB->mapHandbookProgress, pRoleInforDB->mapColorProgress );

    return true;
}

void CEntityItemComponent::PackItemForOther( CParamPool & IOBuff ) 
{
    int nOldPos = IOBuff.GetCurPosition();
    IOBuff.AddUShort( 0 );

    _PackItemBrief( IOBuff );

    m_ClothEffect.PackItemForOther(IOBuff);

    if( IOBuff.GetCurPosition() - nOldPos - sizeof(unsigned short) != 0 )
    {
        int nFinalPos = IOBuff.GetCurPosition(); 
        IOBuff.SetCurPosition( nOldPos );
        IOBuff.ReplaceShort( nFinalPos - nOldPos - sizeof(unsigned short) );
        IOBuff.SetCurPosition( nFinalPos );
    }
}

void CEntityItemComponent::PackOnlinePlayerInfo(RoleImageOfflineData & Info)
{
    std::list<itemid_t> listItemID;

    // 徽章
    m_EquipManager.GetAllItemID(EEquipColumn_BadgeOnPlayer, listItemID);

    // 戒指
    m_EquipManager.GetAllItemID(EEquipColumn_WeddingRingOnPlayer, listItemID);

    // 获取身上衣服
    if (IsInTransForm())
    {
        m_EquipManager.GetAllItemID(EEquipColumn_TransformClothOnPlayer, listItemID);
    }
    else
    {
        m_EquipManager.GetAllItemID(EEquipColumn_ClothOnPlayer, listItemID);
    }

    // 复制数据
    std::list<itemid_t>::iterator it = listItemID.begin();
    for ( ; it != listItemID.end(); ++it)
    {
        ItemBase * pItemBase = m_ItemManager.GetItemByID(*it);
        if(pItemBase!= NULL)
        {
            DBItem item;
            item.m_nItemID   = pItemBase->GetItemID();
            item.m_nTypeID   = pItemBase->GetType();
            int nTempColumn = pItemBase->GetColumn();
            item.m_nColumn = nTempColumn == (int)EBagColumn_ClothBag ? (int)EEquipColumn_ClothOnPlayer : nTempColumn;
            item.m_nIndex = (unsigned short)pItemBase->GetGrid();
            item.m_nUseTimes = pItemBase->GetLeftUseTimes();
            item.m_nDuration = pItemBase->GetDueTime();
            item.m_bEquiped = ((ItemEquip*)pItemBase)->IsEquiped();

            Info.m_vecRoleItem.push_back( item );

            if (pItemBase->IsCloth() && ((ItemEquip*)pItemBase)->GetEffectID() > 0)
            {
                Info.m_mapClothEffect[pItemBase->GetItemID()] = ((ItemEquip*)pItemBase)->GetEffectID();
            }
        }
    }

    m_ClothEffect.PackItemForOther(Info.m_mapClothEffectSevenColor);
}

void CEntityItemComponent::PackEquipDbItem(std::vector<DBItem> &vecEquipDbItem, RoleRoomImageData::ClotheEffectMap &clotheEffectMap, RoleRoomImageData::SevenColorMap &sevenColorMap)
{
    std::list<itemid_t> listItemID;

    // 徽章
    m_EquipManager.GetAllItemID(EEquipColumn_BadgeOnPlayer, listItemID);
    // 戒指
    m_EquipManager.GetAllItemID(EEquipColumn_WeddingRingOnPlayer, listItemID);
    // 获取身上衣服
    if (IsInTransForm())
        m_EquipManager.GetAllItemID(EEquipColumn_TransformClothOnPlayer, listItemID);
    else
        m_EquipManager.GetAllItemID(EEquipColumn_ClothOnPlayer, listItemID);

    // 复制数据
    std::list<itemid_t>::iterator it = listItemID.begin();
    for (; it != listItemID.end(); ++it)
    {
        ItemBase * pItemBase = m_ItemManager.GetItemByID(*it);
        if (pItemBase != NULL)
        {
            DBItem item;
            item.m_nItemID = pItemBase->GetItemID();
            item.m_nTypeID = pItemBase->GetType();
    
            if (pItemBase->GetColumn() == EBagColumn_ClothBag)
            {
                item.m_nColumn = EEquipColumn_ClothOnPlayer;
                item.m_nIndex = ((ItemEquip*)pItemBase)->GetEquipConfigPos();
            }
            else
            {
                item.m_nColumn = (unsigned short)pItemBase->GetColumn();
                item.m_nIndex = (unsigned short)pItemBase->GetGrid();
            }

            item.m_nUseTimes = pItemBase->GetLeftUseTimes();
            item.m_nDuration = pItemBase->GetDueTime();
            item.m_bEquiped = ((ItemEquip*)pItemBase)->IsEquiped();

            vecEquipDbItem.push_back(item);

            if (pItemBase->IsCloth() && ((ItemEquip*)pItemBase)->GetEffectID() > 0)
            {
                clotheEffectMap[pItemBase->GetItemID()] = ((ItemEquip*)pItemBase)->GetEffectID();
            }
        }
    }

    m_ClothEffect.PackItemForOther(sevenColorMap);

    return;
}


void CEntityItemComponent::_PackItemBaseBrief(ItemBase * pItemBase, int nColumn, CParamPool & IOBuff)
{
    if (pItemBase == NULL)
    {
        return;
    }

    IOBuff.AddUInt(pItemBase->GetType());
    IOBuff.AddUShort((unsigned short)nColumn);

    if ( nColumn == (int)EEquipColumn_BadgeOnPlayer && pItemBase->IsWeddingRing() )
    {
        IOBuff.AddUShort(5);
    }
    else if (nColumn == (int)EEquipColumn_ClothOnPlayer)
    {
        IOBuff.AddUShort(((ItemEquip*)pItemBase)->GetEquipConfigPos());
    }
    else
    {
        IOBuff.AddUShort((unsigned short)pItemBase->GetGrid());
    }

    char szItemBuff[ITEM_BUFF_SIZE] = {0};
    memset(szItemBuff, 0, sizeof(szItemBuff));
    CParamPool itemBuff(szItemBuff, sizeof(szItemBuff));
    pItemBase->Encode(&itemBuff);
    unsigned int nItemSize = itemBuff.GetValidDataSize();
    ASSERT(nItemSize == itemBuff.GetCurPosition());

    IOBuff.AddUChar((unsigned char)nItemSize);
    IOBuff.AddMemory(itemBuff.GetData(), (unsigned short)nItemSize);
}

void CEntityItemComponent::_PackItemBrief( CParamPool & IOBuff ) 
{
    unsigned short nItemNumber = 0;

    int nOldPos = IOBuff.GetCurPosition();
    IOBuff.AddUShort(0);

    // 获取身上衣服
    if (IsInTransForm())
    {
        std::list<itemid_t> listItemID;
        m_EquipManager.GetAllItemID(EEquipColumn_TransformClothOnPlayer, listItemID);

        std::list<itemid_t>::iterator it = listItemID.begin();
        for ( ; it != listItemID.end(); ++it )
        {
            ItemBase * pItemBase = m_ItemManager.GetItemByID(*it);
            if (pItemBase != NULL)
            {
                _PackItemBaseBrief(pItemBase, EEquipColumn_TransformClothOnPlayer, IOBuff);
                nItemNumber++;
            }
        }
    }
    else
    {
        std::list<itemid_t> listItemID;
        m_EquipManager.GetAllItemID(EEquipColumn_ClothOnPlayer, listItemID);

        std::list<itemid_t>::iterator it = listItemID.begin();
        for (; it != listItemID.end(); ++it)
        {
            ItemBase * pItemBase = m_ItemManager.GetItemByID(*it);
            if (pItemBase != NULL)
            {
                _PackItemBaseBrief(pItemBase, EEquipColumn_ClothOnPlayer, IOBuff);
                nItemNumber++;
            }
        }
    }

    // 徽章、戒指
    std::list<itemid_t> listItemID;
    m_EquipManager.GetAllItemID(EEquipColumn_BadgeOnPlayer, listItemID);

    std::list<itemid_t>::iterator it = listItemID.begin();
    for ( ; it != listItemID.end(); ++it)
    {
        ItemBase * pItemBase = m_ItemManager.GetItemByID(*it);
        if(pItemBase!= NULL)
        {
            _PackItemBaseBrief(pItemBase, EItemColumn_BadgeOnPlayer, IOBuff);
            nItemNumber++;
        }
    }

    // 结婚戒指
    listItemID.clear();
    m_EquipManager.GetAllItemID(EEquipColumn_WeddingRingOnPlayer, listItemID);

    it = listItemID.begin();
    for ( ; it != listItemID.end(); ++it)
    {
        ItemBase * pItemBase = m_ItemManager.GetItemByID(*it);
        if(pItemBase!= NULL)
        {
            _PackItemBaseBrief(pItemBase, EItemColumn_WeddingRingOnPlayer, IOBuff);
            nItemNumber++;
        }
    }

    int nFinalPos = IOBuff.GetCurPosition();
    IOBuff.SetCurPosition( nOldPos );
    IOBuff.ReplaceShort( nItemNumber );
    IOBuff.SetCurPosition( nFinalPos );
}

void CEntityItemComponent::OnLogin()
{
    std::string buff;
    // 背包数据
    m_ItemManager.SerializeToBuff( buff );
    // 默认服饰
    SerializeDefaultClothToBuff( buff );
    m_ClothEffect.SerializeToBuff( buff );

    char szBuff[256] = { 0 };
    CParamPool buffPool( szBuff, sizeof( szBuff ) );
    buffPool << m_nPerpetualEquipCount << m_nPerpetualEffectScore;
    buff.append( buffPool.GetData(), buffPool.GetValidDataSize() );

    const unsigned short nMaxSize = 1024 * 10; // 10K
    unsigned int nTotal = buff.size();
    const char *pBuff = buff.c_str();

    for ( unsigned int i = 0; i < nTotal; i += nMaxSize )
    {
        GameMsg_S2C_SerializeItemInfo msg;
        msg.m_pBuff = pBuff;

        if ( i + nMaxSize < nTotal )
        {
            msg.m_bEnd = false;
            msg.m_nBuffSize = nMaxSize;
        }
        else
        {
            msg.m_bEnd = true;
            msg.m_nBuffSize = (unsigned short)(nTotal - i);
        }

        SendPlayerMsg( &msg );

        pBuff += nMaxSize;
    }

    GameMsg_S2C_GetClothGroups msg;
    msg.m_nCurGroupID = m_pRoleAttr->GetClothGroupID();
    msg.m_mapClothGroup = m_mapClothGroup;
    SendPlayerMsg(&msg);

    GameMsg_S2C_SyncBagConfig syncmsg;
    syncmsg.m_nBadgeCapacity = m_pRoleAttr->GetBadgeGridNum();
    syncmsg.m_nStorageCapacity = m_BagManager.GetStoragesCapacity();
    syncmsg.m_strBagDescribe = BagConfigManager::Instance().GetBagDescribe();
    syncmsg.m_nAddStorageCapacityCurrencyType = BagConfigManager::Instance().GetAddStorageCurrencyType();
    syncmsg.m_nAddStorageCapacityCurrencyValue = BagConfigManager::Instance().GetAddStorageCurrencyValue();
    SendPlayerMsg(&syncmsg);


    std::list<itemid_t> listItemID;
    m_EquipManager.GetEquipedCloth(listItemID);
    for (std::list<itemid_t>::iterator it = listItemID.begin(); it != listItemID.end(); ++it)
    {
        ItemEquip * pEquip = (ItemEquip*)m_ItemManager.GetItemByID(*it);
        if (pEquip != NULL)
        {
            // 同步穿装备数据
            GameMsg_S2G_OnEquipNotify sgMsg;
            sgMsg.m_nEffectID = pEquip->GetEffectID();
            int nColumn = (pEquip->GetColumn() == (int)EBagColumn_ClothBag ? (int)EEquipColumn_ClothOnPlayer : pEquip->GetColumn());
            sgMsg.m_dbEquip.Initialize(pEquip->GetItemID(), pEquip->GetType(), nColumn, pEquip->GetEquipConfigPos(), pEquip->GetCount(), pEquip->GetLeftUseTimes(),
                pEquip->GetDueTime(), pEquip->GetDynamicBoxRelatedValue(), true);
            ((CRoleEntity*)Entity())->SendMsg2GroupServer(&sgMsg);
        }
    }


    std::list<GeneInfo*> equipGene;
    GetCurEquipGene(equipGene);
    for (std::list<GeneInfo*>::iterator it = equipGene.begin(); it != equipGene.end(); ++it)
    {
        m_pRoleGene->DelGene((*it)->m_nGeneID);
    }
    m_pRoleGene->RefreshEquipGene(equipGene);
}

void CEntityItemComponent::OnLogout()
{
}

void CEntityItemComponent::SendExtraStaticData()
{
    _SendExtendItemBinData();
}

int CEntityItemComponent::GetItemPutInNeedSlotCount(int nColumn, itemtype_t nItemType, int nCount, const std::list<itemid_t> & listItemID)
{
    int nTempCount = nCount;
    for (std::list<itemid_t>::const_iterator itItemID = listItemID.begin();
        itItemID != listItemID.end(); ++itItemID)
    {
        ItemBase * pItemBase = m_ItemManager.GetItemByID(*itItemID);
        if (pItemBase != NULL && pItemBase->GetColumn() == nColumn)
        {
            nTempCount -= pItemBase->GetLeftStackCount();
            nTempCount = nTempCount < 0 ? 0 : nTempCount;
        }
    }

    if (nTempCount > 0)
    {
        ItemConfig * pItemConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
        if (pItemConfig != NULL && pItemConfig->m_nMaxStackCount > 0)
        {
            return nTempCount%pItemConfig->m_nMaxStackCount > 0 ? (nTempCount / pItemConfig->m_nMaxStackCount + 1) :
                nTempCount / pItemConfig->m_nMaxStackCount;
        }
    }

    return nTempCount;
}

unsigned int CEntityItemComponent::GetColumnPutInMaxCount(int nColumn, itemtype_t nItemType, const std::list<itemid_t> & listItemID)
{
    unsigned int nCanAddCount = 0;
    for (std::list<itemid_t>::const_iterator itItemID = listItemID.begin();
        itItemID != listItemID.end(); ++itItemID)
    {
        ItemBase * pItemBase = m_ItemManager.GetItemByID(*itItemID);
        if (pItemBase != NULL && pItemBase->GetColumn() == nColumn)
        {
            nCanAddCount += pItemBase->GetLeftStackCount();
        }
    }

    ItemConfig * pItemConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
    if (pItemConfig != NULL)
    {
        nCanAddCount += pItemConfig->m_nMaxStackCount*m_BagManager.GetColumnEmptyCount((EBagColumn)nColumn);
    }

    return nCanAddCount;
}

int CEntityItemComponent::IsCanUsed(const std::list<itemid_t> & listItemID, int nCount)
{
    int nError = EItemError_Success;
    for (std::list<itemid_t>::const_iterator it = listItemID.begin();
        it != listItemID.end(); ++it)
    {
        ItemBase * pItemBase = m_ItemManager.GetItemByID(*it);
        if (pItemBase != NULL && nCount > 0)
        {
            nError = IsCanUsed(pItemBase, pItemBase->GetCount());
            if (nError == EItemError_Success)
            {
                if (pItemBase->GetCount() >= nCount)
                {
                    return EItemError_Success;
                }
                else
                {
                    nCount -= (int)pItemBase->GetCount();
                }
            }
            else
            {
                return nError;
            }
        }
    }

    return EItemError_NotEnoughItem;
}

int CEntityItemComponent::IsCanUsed(ItemBase * pItemBase, int nCount)
{
    if(pItemBase == NULL)
    {
        return EItemUse_Unknown;
    }

	if (pItemBase->GetCount() < nCount)
	{
		return EItemError_NotEnoughItem;
	}

    if (!pItemBase->IsValid())
    {
        return EItemUse_Unknown;
    }

    if (pItemBase->GetLeftUseTimes() == 0)
    {
        return EItemError_CannotUse;
    }

    if (pItemBase->GetCoolDownTag() > 0 && m_ItemManager.IsInCooling(pItemBase->GetCoolDownTag()))
    {
        return EItemUse_NeedCoolDown;
    }

    if (pItemBase->GetSexConfig() != ESexType_No && m_pRoleAttr->GetSex() != pItemBase->GetSexConfig())
    {
        return EItemUse_NeedSex;
    }

    if (pItemBase->GetLevelConfig() > 0 && m_pRoleAttr->GetLevel() < pItemBase->GetLevelConfig())
    {
        return EItemUse_NeedLevel;
    }

    if (pItemBase->GetVipLevelConfig() > 0) 
    {
        if (!m_pRoleVIP->IsVIP())
        {
            return EItemUse_VIPRequire;
        }

        if (m_pRoleVIP->VIPLevel() < pItemBase->GetVipLevelConfig())
        {
            return EItemUse_VIPRequire;
        }
    }

    std::list<GeneInfo*> listGene;
    pItemBase->GetGene(listGene);
    for (std::list<GeneInfo*>::iterator it = listGene.begin(); it != listGene.end(); ++it)
    {
        if ((*it) != NULL && (*it)->m_nGeneID == EGeneID_AddDanceGroupContribution 
            && m_pRoleDanceGroup->GetDanceGroupID() <= 0)
        {
            return EItemUse_NotInDanceGroup;
        }
    }

    return EItemError_Success;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//把一个物品加入到DB
void CEntityItemComponent::InsertItemToDB(const ItemBase * pItemBase, int nColumn, int nIndex)
{
	if (pItemBase == NULL)
	{
		return;
	}

	DBItem * pItemInfo = new DBItem();
	if (pItemInfo != NULL)
	{
		bool bEquiped = pItemBase->IsEquip() && ((ItemEquip*)pItemBase)->IsEquiped();
		pItemInfo->Initialize(pItemBase->GetItemID(), pItemBase->GetType(), pItemBase->GetColumn(), pItemBase->GetGrid(),
			pItemBase->GetCount(), pItemBase->GetLeftUseTimes(), pItemBase->GetDueTime(), pItemBase->GetDynamicBoxRelatedValue(), bEquiped);

		pItemInfo->m_nColumn = (unsigned short)nColumn;
        pItemInfo->m_nIndex = (unsigned short)nIndex;

		CDelHelper delHelper(pItemInfo);
		AddQuery(QUERY_AddRoleItem, 0, pItemInfo, m_pRoleAttr->GetRoleID(), NULL, &delHelper);
	}
};

void CEntityItemComponent::InsertClothEffectToDB(const ItemEquip * pCloth)
{
    if (pCloth == NULL)
    {
        return;
    }

    ClothEffectDB * pEffect = new ClothEffectDB();
    if (pEffect != NULL)
    {
        pEffect->m_nEffectID = pCloth->GetEffectID();
        pEffect->m_nRoleID = m_pRoleAttr->GetRoleID();
        pEffect->m_nItemID = pCloth->GetItemID();

        AddQuery(QUERY_InsertClothEffect,m_pRoleAttr->GetRoleID(), pEffect);
    }
}

void CEntityItemComponent::UpdateClothEffectToDB(const ItemEquip * pCloth)
{
    if (pCloth == NULL)
    {
        return;
    }

    ClothEffectDB * pEffect = new ClothEffectDB();
    if (pEffect != NULL)
    {
        pEffect->m_nEffectID = pCloth->GetEffectID();
        pEffect->m_nRoleID = m_pRoleAttr->GetRoleID();
        pEffect->m_nItemID = pCloth->GetItemID();

        AddQuery(QUERY_UpdateClothEffect,0,pEffect);
    }
}

void CEntityItemComponent::UpdateItemToDB(const ItemBase * pItemBase)
{
    if (pItemBase != NULL)
    {
        DBItem * pItemInfo = new DBItem();
        if(pItemInfo != NULL)
        {
			bool bEquiped = pItemBase->IsEquip() && ((ItemEquip*)pItemBase)->IsEquiped();
			pItemInfo->Initialize(pItemBase->GetItemID(), pItemBase->GetType(), pItemBase->GetColumn(), pItemBase->GetGrid(),
				pItemBase->GetCount(), pItemBase->GetLeftUseTimes(), pItemBase->GetDueTime(), pItemBase->GetDynamicBoxRelatedValue(), bEquiped);
            pItemInfo->m_nColumn = (unsigned short)pItemBase->GetColumn();
            pItemInfo->m_nIndex = (unsigned short)pItemBase->GetGrid();

            AddQuery(QUERY_UpdateRoleItem,0,pItemInfo,m_pRoleAttr->GetRoleID(),NULL);
        }
    }
}

//从DB中删除物品
void CEntityItemComponent::DeleteItemToDB(const ItemBase * pItem)
{
    if (g_pQueryMgr != NULL && pItem != NULL)
    {
        AddQuery(QUERY_RemoveRoleItem, (unsigned int)time(NULL), NULL, pItem->GetItemID(), NULL);
    }
}

//修改DB中物品的数量数据
void CEntityItemComponent::UpdateItemCountToDB(const ItemBase * pItem)
{
    if (pItem != NULL && g_pQueryMgr != NULL && pItem->GetCount() > 0)
    {
        AddQuery(QUERY_UpdateItemCount,pItem->GetCount(),NULL,pItem->GetItemID(),NULL);
    }
}

int CEntityItemComponent::CanAddAllItem(const list<CItem> &listItem, bool bAllowUniqueEquip)
{
    int nRes = EItemError_Success;
    std::map<unsigned short,unsigned short> mapGrid;
    for (std::list<CItem>::const_iterator itr = listItem.begin(); itr != listItem.end(); itr++)
    {
        itemtype_t nItemType = itr->m_nItemType;
        unsigned short nCount = itr->m_nItemCount;
        int nValidTime = itr->m_nValidTime;

        unsigned short nStackNumber = 0;
            
        if ( (nRes = CanAddItem(nItemType,nCount,nValidTime,nStackNumber, bAllowUniqueEquip)) != EItemError_Success)
        {
            return nRes;
        }

        EItemColumn nColumn = (EItemColumn)ItemType2Column(nItemType); 
        if(nColumn == EItemColumn_Invalid)
        {
            return EItemUse_CreatItemError;
        }
        std::map<unsigned short,unsigned short>::iterator itGrid = mapGrid.find((unsigned short)nColumn);
        if(itGrid != mapGrid.end())
        {
            itGrid->second = itGrid->second + nStackNumber;
        }
        else
        {
            mapGrid.insert(std::map<unsigned short,unsigned short>::value_type((unsigned short)nColumn,nStackNumber));
        }
    }

    //check empty grid is enough
    std::map<unsigned short,unsigned short> ::iterator itEmptyGrid = mapGrid.begin();
    while (itEmptyGrid != mapGrid.end())
    {
        unsigned short nColumn = itEmptyGrid->first;
        unsigned short nNeedCount = itEmptyGrid->second;

		EBagColumn eBagColumn = m_BagManager.GetBagColumn((EItemColumn)nColumn);
        unsigned short nCurEmptyGridNum = (unsigned short)m_BagManager.GetColumnEmptyCount(eBagColumn);
        if (nCurEmptyGridNum < nNeedCount)
        {
            return EItemUse_ColumnFull;
        }
        itEmptyGrid++;
    }

    return nRes;
}

void CEntityItemComponent::GetPacketItemInfo(unsigned short nRoleIndex, itemtype_t nPacketID, 
            list<CItem> & listItem, std::map<int, int> & mapCurrency)
{
    if (nPacketID <= 0 || nRoleIndex <= 0)
    {
        return;
    }

    if (CInitLua::GetInstance().DoString("GetPacketItemInfo",
        enumSCRIPT_RETURN_STRING, 1, enumSCRIPT_PARAM_NUMBER, 2, nPacketID, nRoleIndex,
        DOSTRING_PARAM_END) != -1)
    {
        string strItem = CInitLua::GetInstance().GetRetString(0);
        std::list<int> listPacket;
        PacketAtoi(strItem.c_str(), listPacket);
        if (listPacket.size() % 3 == 0)
        {
            int n = 0;
            CItem item;
            for (list<int>::iterator itr = listPacket.begin(); itr != listPacket.end(); itr++)
            {
                if (n % 3 == 0)
                {
                    item.m_nItemType = (itemtype_t)*itr;
                }
                else if (n % 3 == 1)
                {
                    item.m_nItemCount = (itemcount_t)*itr;
                }
                else
                {
                    item.m_nValidTime = *itr;
                    listItem.push_back(item);
                }
                n++;
            }
        }
    }

    if (CInitLua::GetInstance().DoString("GetPacketCurrencyInfo",
        enumSCRIPT_RETURN_STRING, 1, enumSCRIPT_PARAM_NUMBER, 2, nPacketID, nRoleIndex,
        DOSTRING_PARAM_END) != -1)
    {
        std::map<int, int> mapTempCurrency;
        string strItem = CInitLua::GetInstance().GetRetString(0);
        Split(strItem, "|", ":", mapTempCurrency);

        for (std::map<int, int>::iterator it = mapTempCurrency.begin(); it != mapTempCurrency.end(); ++it)
        {
            mapCurrency[it->first] += it->second;
        }
    }
}

bool CEntityItemComponent::AddPacketItemToRole(itemtype_t nPacketID, unsigned int nCount)
{
    int nResult = 0;
    CInitLua & lua = CInitLua::GetInstance();

    if (lua.DoString("GetPacketItems", enumSCRIPT_RETURN_NUMBER, 1, 
		enumSCRIPT_PARAM_NUMBER, 2, nPacketID, m_pNetCpt->GetPlayerLogicIndex(), DOSTRING_PARAM_END) != -1)
    {
        lua.GetRetNum(nResult ,0);
    }

    if (nResult == 0)
    {
        //发送一个错误消息给客户端，说包裹id无效
        GameMsg_S2C_ItemAwardFail failMsg;
        failMsg.m_strError = "Packet_ID_Invalid!";
        SendPlayerMsg(&failMsg);
        return false;
    }

    GameMsg_S2C_OpenPackageResult msgResult;
	GetPacketItemInfo(m_pNetCpt->GetPlayerLogicIndex(), nPacketID, msgResult.m_listItems, msgResult.m_mapCurrency);
    SendPlayerMsg(&msgResult);

    return true;
}

bool CEntityItemComponent::AddPacthPacketItemToRole(unsigned short nRoleIndex, itemtype_t nPacketID, unsigned int nCount, 
                list<CItem> & listItem, std::map<int, int> & mapCurrency)
{
    int nResult = 0;
    CInitLua & lua = CInitLua::GetInstance();

    for (unsigned int i = 0; i < nCount; ++i)
    {
        if( lua.DoString("GetPacketItems",enumSCRIPT_RETURN_NUMBER,1,enumSCRIPT_PARAM_NUMBER,2,nPacketID, nRoleIndex,DOSTRING_PARAM_END) != -1)
        {
            lua.GetRetNum(nResult ,0);
        }

        if (nResult == 0)
        {
            //发送一个错误消息给客户端，说包裹id无效
            GameMsg_S2C_ItemAwardFail failMsg;
            failMsg.m_strError = "Packet_ID_Invalid!";
            SendPlayerMsg(&failMsg);
            return false;
        }

        GetPacketItemInfo(nRoleIndex,nPacketID,listItem, mapCurrency);
    }

    return true;
}

bool CEntityItemComponent::RandomBoxItem(itemtype_t nBoxID, list<CItem>& rItemList)
{
    CBoxInfo* pBoxInfo = CBoxInfoManager::Instance().GetBoxInfoByID( nBoxID );
    if ( pBoxInfo != NULL )
    {
        bool bIsMale = ( m_pRoleAttr->GetSex() == ESexType_Male );
        pBoxInfo->GetRandomItem( bIsMale, rItemList );

        return true;
    }

    return false;
}

bool CEntityItemComponent::GenerateDynamicBoxItem( __int64 itemID, int nRelatedValue, itemtype_t nBoxID, list<CItem> &itemlist )
{
    CDynamicBoxInfo* pBoxInfo = CDynamicBoxInfoManager::Instance().GetDynamicBoxInfoByID( nBoxID );
    if ( pBoxInfo != NULL )
    {
        bool bIsMale = ( m_pRoleAttr->GetSex() == ESexType_Male );
        pBoxInfo->GetRandomItem( nRelatedValue, bIsMale, itemlist );

        return true;
    }

    return false;
}

void CEntityItemComponent::SimulateRandomBox(itemtype_t nBoxID, int nCount)
{
    GameMsg_S2C_SimulateRandomItem boxMsg;
    while ( nCount > 0 )
    {
        std::list<CItem> boxItem;
        if ( RandomBoxItem( nBoxID, boxItem ) )
        {
            for ( std::list<CItem>::iterator it_boxitem = boxItem.begin(); it_boxitem != boxItem.end(); ++it_boxitem )
            {
                std::map<CItem, unsigned int>::iterator it_allitem = boxMsg.allItem.find( *it_boxitem );
                if ( it_allitem != boxMsg.allItem.end() )
                {
                    it_allitem->second += 1;
                }
                else
                {
                    boxMsg.allItem.insert( std::make_pair( *it_boxitem, 1 ) );
                }
            }
        }
        --nCount;
    }

    if ( !boxMsg.allItem.empty() )
    {
        SendPlayerMsg( &boxMsg );
    }
}

bool CEntityItemComponent::AddBoxItemToRole( itemtype_t nBoxID )
{
    std::list<CItem> boxItem;
    if (!RandomBoxItem( nBoxID, boxItem ))
    {
        return false;
    }

    for (std::list<CItem>::iterator it = boxItem.begin(); it != boxItem.end(); ++it)
    {
        AddItem(it->m_nItemType, it->m_nItemCount, it->m_nValidTime, EItemAction_Add_Box, nBoxID, false, false, false);
        if (it->m_bAnnounce) 
        {
            ItemConfig *pBoxInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(nBoxID);
            ItemConfig *pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(it->m_nItemType);
            if (m_pRoleAttr != NULL && pBoxInfo != NULL && pItemInfo != NULL)
            {
                std::string strItemExtra;
                if (pItemInfo->IsEquip())
                {
                    if (it->m_nValidTime > 0)
                    {
                        int nDaY = ( it->m_nValidTime - 1 ) / ( 3600 * 24 ) + 1;
                        SafeFormat(strItemExtra, CLocalization::Instance().GetString( "Item_EquipMatune_Day" ), nDaY);
                    }
                    else 
                    {
                        strItemExtra = CLocalization::Instance().GetString("Item_EquipMatune_Forever");
                    }
                }
                else 
                {
                    SafeFormat(strItemExtra, CLocalization::Instance().GetString( "Item_Expandable_Count" ),
                        it->m_nItemCount);
                }

                std::string strContent;
                SafeFormat(strContent, CLocalization::Instance().GetString("OpenBox_Announce"),
                    m_pRoleAttr->GetRoleName(), pBoxInfo->m_strName.c_str(), 
                    pItemInfo->m_strName.c_str(), strItemExtra.c_str());

                CChatManager::Instance().SendAnnounce(true, true, strContent);
            }
        }
    }

    GameMsg_S2C_OpenBoxResult msgResult;
    msgResult.m_nCount = 1;
    msgResult.m_nItemType = nBoxID;
    msgResult.m_listItems = boxItem;
    SendPlayerMsg(&msgResult);

    return true;
}

bool CEntityItemComponent::AddPatchBoxItemToRole(itemtype_t nBoxID, int nCount, std::list<CItem> &itemList)
{
    std::list<CItem> boxItem;
    for (int i = 0; i < nCount; ++i)
    {
        RandomBoxItem(nBoxID, boxItem);
    }

    for (std::list<CItem>::iterator it = boxItem.begin(); it != boxItem.end(); ++it)
    {
        AddItem(it->m_nItemType, it->m_nItemCount, it->m_nValidTime, EItemAction_Add_Box, nBoxID, false, false, false);
        if (it->m_bAnnounce)
        {
            ItemConfig *pBoxInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(nBoxID);
            ItemConfig *pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(it->m_nItemType);
            if (m_pRoleAttr != NULL && pBoxInfo != NULL && pItemInfo != NULL)
            {
                std::string strItemExtra;
                if (pItemInfo->IsEquip())
                {
                    if (it->m_nValidTime > 0)
                    {
                        int nDaY = ( it->m_nValidTime - 1 ) / ( 3600 * 24 ) + 1;
                        SafeFormat(strItemExtra, CLocalization::Instance().GetString("Item_EquipMatune_Day"), nDaY);
                    }
                    else 
                    {
                        strItemExtra = CLocalization::Instance().GetString("Item_EquipMatune_Forever");
                    }
                }
                else
                {
                    SafeFormat(strItemExtra, CLocalization::Instance().GetString("Item_Expandable_Count"),
                        it->m_nItemCount );
                }

                std::string strContent;
                SafeFormat(strContent, CLocalization::Instance().GetString("OpenBox_Announce"),
                    m_pRoleAttr->GetRoleName(), pBoxInfo->m_strName.c_str(),
                    pItemInfo->m_strName.c_str(), strItemExtra.c_str());

                CChatManager::Instance().SendAnnounce(true, true, strContent);
            }
        }
    }

    itemList.insert(itemList.end(), boxItem.begin(),boxItem.end());

    return true;
}

bool CEntityItemComponent::AddPatchDynamicBoxItemToRole( __int64 itemID, int nRelatedValue, itemtype_t nBoxID, int nCount, list<CItem> & listItem )
{
    std::list<CItem> boxItem;
    for (int i = 0; i < nCount; ++i)
    {
        GenerateDynamicBoxItem(itemID, nRelatedValue, nBoxID, boxItem);
    }

    for (std::list<CItem>::iterator it = boxItem.begin(); it != boxItem.end(); ++it)
    {
        AddItem(it->m_nItemType, it->m_nItemCount, it->m_nValidTime, EItemAction_Add_Box, nBoxID, false, false, false);
        if (it->m_bAnnounce)
        {
            ItemConfig *pBoxInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(nBoxID);
            ItemConfig *pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(it->m_nItemType);
            if (m_pRoleAttr != NULL && pBoxInfo != NULL && pItemInfo != NULL)
            {
                std::string strItemExtra;
                if (pItemInfo->IsEquip())
                {
                    if (it->m_nValidTime > 0)
                    {
                        int nDaY = ( it->m_nValidTime - 1 ) / ( 3600 * 24 ) + 1;
                        SafeFormat(strItemExtra, CLocalization::Instance().GetString("Item_EquipMatune_Day"), nDaY);
                    }
                    else 
                    {
                        strItemExtra = CLocalization::Instance().GetString("Item_EquipMatune_Forever");
                    }
                }
                else 
                {
                    SafeFormat(strItemExtra, CLocalization::Instance().GetString("Item_Expandable_Count"),
                        it->m_nItemCount);
                }

                std::string strContent;
                SafeFormat(strContent, CLocalization::Instance().GetString("OpenBox_Announce"),
                    m_pRoleAttr->GetRoleName(), pBoxInfo->m_strName.c_str(),
                    pItemInfo->m_strName.c_str(), strItemExtra.c_str());

                CChatManager::Instance().SendAnnounce(true, true, strContent);
            }
        }		
    }

    listItem.insert( listItem.end(), boxItem.begin(), boxItem.end() );

    return true;
}

bool CEntityItemComponent::AddDynamicBoxItemToRole( __int64 itemID, int nRelatedValue, itemtype_t nBoxID, int nCount )
{
    std::list<CItem> boxItem;
    for ( int i = 0; i < nCount; ++i )
    {
        GenerateDynamicBoxItem( itemID, nRelatedValue, nBoxID, boxItem ) ;
    }

    for ( std::list<CItem>::iterator it = boxItem.begin(); it != boxItem.end(); ++it )
    {
        AddItem(it->m_nItemType, it->m_nItemCount, it->m_nValidTime, EItemAction_Add_Box, nBoxID, false, false, false);
        if (it->m_bAnnounce)
        {
            ItemConfig * pBoxInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(nBoxID);
            ItemConfig * pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(it->m_nItemType);
            if (m_pRoleAttr != NULL && pBoxInfo != NULL && pItemInfo != NULL) 
            {
                std::string strItemExtra;
                if ( pItemInfo->IsEquip() ) 
                {
                    if ( it->m_nValidTime > 0 ) 
                    {
                        int nDaY = ( it->m_nValidTime - 1 ) / ( 3600 * 24 ) + 1;
                        SafeFormat(strItemExtra, CLocalization::Instance().GetString("Item_EquipMatune_Day"), nDaY);
                    }
                    else 
                    {
                        strItemExtra = CLocalization::Instance().GetString( "Item_EquipMatune_Forever" );
                    }
                }
                else
                {
                    SafeFormat(strItemExtra, CLocalization::Instance().GetString("Item_Expandable_Count"), 
                        it->m_nItemCount);
                }

                std::string strContent;
                SafeFormat(strContent, CLocalization::Instance().GetString("OpenBox_Announce"),
                    m_pRoleAttr->GetRoleName(), pBoxInfo->m_strName.c_str(),
                    pItemInfo->m_strName.c_str(), strItemExtra.c_str());

                CChatManager::Instance().SendAnnounce(true, true, strContent);
            }
        }
    }

    GameMsg_S2C_OpenBoxResult msgResutl;
    msgResutl.m_nCount = nCount;
    msgResutl.m_nItemType = nBoxID;
    msgResutl.m_listItems = boxItem;
    SendPlayerMsg(&msgResutl);

    return true;
}

void CEntityItemComponent::SetExpiredVIPEquipment()
{
    // 卸载徽章栏过期物品
    std::list<itemid_t> listItemID;
    m_EquipManager.GetAllItemID(EEquipColumn_BadgeOnPlayer, listItemID);
    std::list<itemid_t>::iterator it = listItemID.begin();
    for (std::list<itemid_t>::iterator it = listItemID.begin(); it != listItemID.end(); ++it)
    {
        ItemBase * pItemBase = m_ItemManager.GetItemByID(*it);
        if (pItemBase != NULL && pItemBase->GetVipLevelConfig() > 0 && ((ItemEquip*)pItemBase)->IsEquiped()) 
        {
            UnEquip((ItemEquip*)pItemBase);
        }
    }

    // 卸载衣服栏过期物品
    listItemID.clear();
    m_EquipManager.GetEquipedCloth(listItemID);
    for (std::list<itemid_t>::iterator it = listItemID.begin(); it != listItemID.end(); ++it)
    {
        ItemEquip * pEquip = (ItemEquip*)m_ItemManager.GetItemByID(*it);
        if (pEquip != NULL && pEquip->IsEquiped() && pEquip->GetVipLevelConfig() > 0)
        {
            UnEquip(pEquip);
        }
    }

    // integrity check
    EquipIntegrityCheck();
}

void CEntityItemComponent::AddDefaultColthToPlayer()
{
    CreateDefaultColth(eItemClothType_Hair);
    CreateDefaultColth(eItemClothType_Face);
    CreateDefaultColth(eItemClothType_Body);
    CreateDefaultColth(eItemClothType_Glove);
    CreateDefaultColth(eItemClothType_Leg);
    CreateDefaultColth(eItemClothType_Feet);
    CreateDefaultColth(eItemClothType_Skin);
}

int CEntityItemComponent::GetDefaultClothPos(EItemClothType clothType)
{
    switch(clothType)
    {
    case eItemClothType_Hair:return 0;
    case eItemClothType_Face:return 1;
    case eItemClothType_Body:return 2;
    case eItemClothType_Glove:return 3;
    case eItemClothType_Leg:return 4;
    case eItemClothType_Feet:return 5;
    case eItemClothType_Skin:return 5 + (int)m_pRoleAttr->GetColor();
    default:break;
    }

    return -1;
}

int CEntityItemComponent::GetDefaultClothBagSlot(EItemClothType clothType)
{
    switch(clothType)
    {
    case eItemClothType_Hair:return 0;
    case eItemClothType_Face:return 1;
    case eItemClothType_Body:return 2;
    case eItemClothType_Glove:return 3;
    case eItemClothType_Leg:return 4;
    case eItemClothType_Feet:return 5;
    case eItemClothType_Skin:return 6;
    default:break;
    }

    return -1;
}

void CEntityItemComponent::CreateDefaultColth(EItemClothType clothType)
{
	ItemConfig * pItemConfig = GetDefaultClothConfig(clothType);
	if (pItemConfig == NULL)
	{
		return;
	}

    ItemEquip * pEquip = (ItemEquip*)m_ItemManager.AddItem(pItemConfig, 1, 0, GetDefaultClothPos(clothType) + 1);
    if (pEquip != NULL)
    {
        m_BagManager.AddItem(pEquip);
		
		// 添加保存中
		m_listDefaultClothID.push_back(pEquip->GetItemID());
	}
}

void CEntityItemComponent::SendPlayerMsg(GameMsg_Base *pMsg)
{
    if (m_pNetCpt != NULL)
    {
        m_pNetCpt->SendPlayerMsg(pMsg);
    }
}

void CEntityItemComponent::SendPlayerMsgToOthers(GameMsg_Base * pMsg)
{
    if (m_pRoleRoom != NULL && m_pRoleRoom->Room() != NULL) 
    {
        m_pRoleRoom->Room()->SendMsgToAll(pMsg, m_pRoleRoom);
    }

    if (m_pRoleCouple != NULL && m_pRoleCouple->WeddingRoom() != NULL) 
    {
        m_pRoleCouple->WeddingRoom()->SendMsgToAllBut(pMsg, m_pRoleCouple);
    }

    if (m_pRolePhotoRoom != NULL && m_pRolePhotoRoom->PhotoRoom() != NULL) 
    {
        m_pRolePhotoRoom->PhotoRoom()->SendMsgToAllBut(pMsg, m_pRolePhotoRoom);
    }

    if (m_pRoleAmuseRoom != NULL && m_pRoleAmuseRoom->AmuseRoom() != NULL ) 
    {
        m_pRoleAmuseRoom->AmuseRoom()->SendMsgToAll(pMsg, m_pRoleAmuseRoom);
    }

    if (m_pRoleDanceGroup != NULL && m_pRoleDanceGroup->GetCeremonyRoom() != NULL)
    {
        m_pRoleDanceGroup->GetCeremonyRoom()->RoomBroadcastExcept(m_pRoleAttr->GetRoleID(), *pMsg);
    }
}

void CEntityItemComponent::OnGetActivateGiftCodeCB(QueryBase &query)
{
    CActivateGiftInfo *pInfo = (CActivateGiftInfo *)query.m_pAttachment;
    if (query.m_nResult == 0)
    {
        if (pInfo->m_nOwnRoleID > 0 && m_pRoleAttr != NULL)
        {
            GameMsg_S2C_ItemAwardFail failMsg;
            failMsg.m_strError = "Item_Activate_Have_Done";
            SendPlayerMsg(&failMsg);
        }
        else
        {
            string strTitle = CLocalization::Instance().GetString("Mail_Title_Activate_Gift_Code");
            string strContent;
            SafeFormat(strContent, CLocalization::Instance().GetString("Mail_Content_Activate_Gift_Code"),
                pInfo->m_strCode.c_str());

            CMailMgr::Instance().SendSystemMailToPlayer( m_pRoleAttr->GetRoleID(), EMailType_Activate_Gift_Code,
                strTitle, strContent, (itemtype_t)pInfo->m_nItemID, (itemcount_t)pInfo->m_nItemCount, -1 );

            GameMsg_S2C_ItemAwardSuc sucMsg;
            sucMsg.m_nItemID = pInfo->m_nItemID;
            sucMsg.m_nItemCount = pInfo->m_nItemCount;
            SendPlayerMsg(&sucMsg);
        }
    }

    delete pInfo, pInfo= NULL;
}

void CEntityItemComponent::OnGetActivateInfoCB(QueryBase &query)
{
    CActivateGiftInfo *pGiftInfo = (CActivateGiftInfo *)query.m_pAttachment;
    bool bOk = true;

    if (pGiftInfo == NULL) {
        return;
    }

    if (query.m_nResult == 0) {
        unsigned int nCurTime = (unsigned int)time(NULL);

        if (pGiftInfo->m_nItemID == 0) {
            GameMsg_S2C_ItemAwardFail failMsg;
            failMsg.m_strError = "Item_Activate_Code_Error";
            bOk = false;
            SendPlayerMsg(&failMsg);
        } else if (pGiftInfo->m_nStartTime > nCurTime) {
            GameMsg_S2C_ItemAwardFail failMsg;
            failMsg.m_strError = "Item_Activate_Not_Time_To";
            bOk = false;
            SendPlayerMsg(&failMsg);
        } else if (pGiftInfo->m_nEndTime < nCurTime) {
            GameMsg_S2C_ItemAwardFail failMsg;
            failMsg.m_strError = "Item_Activate_Code_Expired";
            bOk = false;
            SendPlayerMsg(&failMsg);
        } else if (pGiftInfo != NULL && m_pRoleAttr != NULL) {
            if (pGiftInfo->m_nOwnRoleID > 0) {
                GameMsg_S2C_ItemAwardFail failMsg;
                if (pGiftInfo->m_nOwnRoleID == m_pRoleAttr->GetRoleID()) {
                    failMsg.m_strError = "Item_Activate_You_Have_Used_Code";
                } else {
                    failMsg.m_strError = "Item_Activate_Code_Have_Used";
                }
                bOk = false;
                SendPlayerMsg(&failMsg);
            }
        } 
        if (bOk) {
            CActivateGiftInfo *pQueryGiftInfo = new CActivateGiftInfo(*pGiftInfo);
            CDelHelper delHelper(pQueryGiftInfo);
            AddQuery(QUERY_Item_GetActivateGift, m_pRoleAttr->GetRoleID(), pQueryGiftInfo, 0, NULL, &delHelper);
        }
    }

    delete pGiftInfo, pGiftInfo = NULL;
}

void CEntityItemComponent::_LogRoleItem(unsigned int nRoleID, EItemAction eItemAction, EItemColumn eItemColumn, __int64 nItemID, itemtype_t nItemType, unsigned short nOldCount,
    int nOldDuration, unsigned short nNewCount, int nNewDuration, __int64 nAdditionalValue)
{
    CRoleItemLog * pLog = new CRoleItemLog();
    pLog->m_nRoleID = nRoleID;
    pLog->m_eItemAction = eItemAction;
    pLog->m_eItemColumn = eItemColumn;
    pLog->m_nItemID = nItemID;
    pLog->m_nItemType = nItemType;
    pLog->m_nOldCount = nOldCount;
    pLog->m_nOldDuration = nOldDuration;
    pLog->m_nNewCount = nNewCount;
    pLog->m_nNewDuration = nNewDuration;
    AddQuery(QUERY_LogRoleItem, 0, pLog);
}

void CEntityItemComponent::_SendExtendItemBinData()
{
    unsigned short nBinDataSize = (unsigned short)ConfigManager::Instance().GetItemConfigManager().ExtendItemBinDataSize();
    char *pBInData = const_cast<char *>(ConfigManager::Instance().GetItemConfigManager().ExtendItemBinData());
    CParamPool poolExtendBinData(pBInData, nBinDataSize);
    GameMsg_S2C_SynExtendItemBinData msgSyn;
    msgSyn.m_nBinDataSize = nBinDataSize;
    poolExtendBinData.GetMemory(msgSyn.m_szExtendItemBinData, msgSyn.m_nBinDataSize);
    SendPlayerMsg(&msgSyn);
}

ItemConfig * CEntityItemComponent::GetDefaultClothConfig(EItemClothType clothType)
{
    int index = GetDefaultClothPos(clothType);
    if (index == -1)
    {
        return NULL;
    }

    itemtype_t nItemType = CLogicInitData::Instance().GetDefaultColth( m_pRoleAttr->GetSex() == ESexType_Male, (unsigned short)index);
    return ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
}

ItemEquip * CEntityItemComponent::GetDefaultCloth(EItemClothType clothTyp)
{
	itemid_t nItemID = GetDefaultClothPos(clothTyp) + 1;
	return (ItemEquip*)m_ItemManager.GetItemByID(nItemID);
}

void CEntityItemComponent::_InitGroupClothByDefault(unsigned char nGroupID)
{
    m_mapClothGroup[nGroupID].SetGroupID( nGroupID );
    RoleClothGroup* pClothGroup = new RoleClothGroup();
    *pClothGroup = m_mapClothGroup[nGroupID];

    ItemConfig * pItemConfig = GetDefaultClothConfig(eItemClothType_Hair);
    if (pItemConfig != NULL)
    {
        pClothGroup->AddCloth( eItemClothType_Hair, pItemConfig->GetType() );
    }

    pItemConfig = GetDefaultClothConfig(eItemClothType_Face);
    if (pItemConfig != NULL)
    {
        pClothGroup->AddCloth( eItemClothType_Face, pItemConfig->GetType() );
    }

    pItemConfig = GetDefaultClothConfig(eItemClothType_Body);
    if (pItemConfig != NULL)
    {
        pClothGroup->AddCloth( eItemClothType_Body, pItemConfig->GetType() );
    }

    pItemConfig = GetDefaultClothConfig(eItemClothType_Glove);
    if (pItemConfig != NULL)
    {
        pClothGroup->AddCloth( eItemClothType_Glove, pItemConfig->GetType() );
    }

    pItemConfig = GetDefaultClothConfig(eItemClothType_Leg);
    if (pItemConfig != NULL)
    {
        pClothGroup->AddCloth( eItemClothType_Leg, pItemConfig->GetType() );
    }

    pItemConfig = GetDefaultClothConfig(eItemClothType_Feet);
    if (pItemConfig != NULL)
    {
        pClothGroup->AddCloth( eItemClothType_Feet, pItemConfig->GetType() );
    }

    pItemConfig = GetDefaultClothConfig(eItemClothType_Skin);
    if (pItemConfig != NULL)
    {
        pClothGroup->AddCloth( eItemClothType_Skin, pItemConfig->GetType() );
    }

    m_mapClothGroup[nGroupID] = *pClothGroup;
    AddQuery(QUERY_AddRoleClothGroup, 0, NULL, m_pRoleAttr->GetRoleID(), pClothGroup);
}

void CEntityItemComponent::_InitClothGroup(unsigned char nGroupID)
{
    m_mapClothGroup[nGroupID].SetGroupID( nGroupID );

    RoleClothGroup* pClothGroup = new RoleClothGroup();
    *pClothGroup = m_mapClothGroup[nGroupID];

    for (int i = eItemClothType_Hair; i < eItemClothType_Max; ++i)
    {
        if ( i != eItemClothType_All )
        {
            itemid_t nItemID = m_EquipManager.GetClothItemID(i);
            ItemEquip * pCloth = (ItemEquip *)m_ItemManager.GetItemByID(nItemID);
            if (pCloth != NULL)
            {
                pClothGroup->AddCloth( (unsigned char)i, pCloth->GetType() );
            }
        }
    }

    m_mapClothGroup[nGroupID] = *pClothGroup;
    AddQuery(QUERY_AddRoleClothGroup, 0, NULL, m_pRoleAttr->GetRoleID(), pClothGroup);
}

void CEntityItemComponent::_CheckClothGroup(std::map<unsigned char, itemtype_t> & mapEquip, std::list<ItemEquip*> & listOnEquip, 
    std::map<itemtype_t, stringw>& mapItemError, CRoleVIPInfo * pVipInfo)
{
    std::list<EItemClothType> listErrorPos;
    std::map<unsigned char, itemtype_t>::iterator it = mapEquip.begin();
    for (; it != mapEquip.end(); ++it)
    {
        if (it->second == 0)
        {
            continue;
        }

        ItemBase * pItemBase = m_ItemManager.GetItemByType(it->second);
        if (pItemBase == NULL || !pItemBase->IsCloth())
        {
            listErrorPos.push_back((EItemClothType)it->first);
            mapItemError[it->second].assign("EquipItem_NotExist");
            continue;
        }

        ItemEquip * pCloth = (ItemEquip*)pItemBase;
        if (pCloth->GetAnType(2) != it->first)
        {
            listErrorPos.push_back((EItemClothType)it->first);
            mapItemError[it->second].assign("ItemUse_EquipPos");
            continue;
        }

        int nError = CanEquip((ItemEquip*)pItemBase, pVipInfo);
        if (nError != EItemError_Success)
        {
            if (nError == EItemUse_VIPRequire)
            {
                mapItemError[it->second].assign("EquipItem_VIPRequire");//vip过期，需要vip，自动替换成默认装备或者不穿
            }
            else if (nError == EItemUse_PoorVIPLevel)
            {
                mapItemError[it->second].assign("EquitItem_PoorVIPLevel");//vip等级不够
            }

            listErrorPos.push_back((EItemClothType)it->first);
            continue;
        }

        if (!pCloth->IsEquiped())
        {
            listOnEquip.push_back(pCloth);
        }
    }

    // error pos by default 
    std::list<EItemClothType>::iterator itr = listErrorPos.begin();
    for (; itr != listErrorPos.end(); ++itr)
    {
        ItemEquip * pDefaultEquip = GetDefaultCloth(*itr);
        if (pDefaultEquip != NULL)
        {
            if (!pDefaultEquip->IsEquiped())
            {
                listOnEquip.push_back(pDefaultEquip);
            }
            else
            {
                std::map<unsigned char, itemtype_t>::iterator itt = mapEquip.find(*itr);
                if (itt != mapEquip.end())
                {
                    itt->second = pDefaultEquip->GetType();
                }
            }
        }
    }
}

void CEntityItemComponent::_RefreshClothGroup(ItemEquip * pEquip)
{
    std::map<unsigned char, RoleClothGroup>::iterator itGroup = m_mapClothGroup.find(m_pRoleAttr->GetClothGroupID());
    if (itGroup == m_mapClothGroup.end())
    {
        return;
    }

    bool bRefresh = false;
    if ( pEquip != NULL && pEquip->IsCloth() && itGroup->second.GetClothItemType( pEquip->GetEquipConfigPos() ) != pEquip->GetType() )
    {
        itGroup->second.AddCloth( pEquip->GetEquipConfigPos(), pEquip->GetType() );
        bRefresh = true;
    }
   
    for (std::map<unsigned char, itemtype_t>::const_iterator itr = itGroup->second.GetClothes().begin();
        itr != itGroup->second.GetClothes().end(); ++itr)
    {
        itemid_t nItemID = m_EquipManager.GetEquipedCloth((EItemClothType)itr->first);
        ItemEquip * pCloth = (ItemEquip *)m_ItemManager.GetItemByID(nItemID);
        itemtype_t nType = pCloth != NULL ? pCloth->GetType() : 0;
        if (itr->second != nType)
        {
            itGroup->second.AddCloth( itr->first, nType );
            bRefresh = true;
        }
    }

    std::set<EItemClothType> setDefault;
    setDefault.insert(eItemClothType_Hair);
    setDefault.insert(eItemClothType_Face);
    setDefault.insert(eItemClothType_Body);
    setDefault.insert(eItemClothType_Glove);
    setDefault.insert(eItemClothType_Leg);
    setDefault.insert(eItemClothType_Feet);
    setDefault.insert(eItemClothType_Skin);
    for (std::set<EItemClothType>::iterator itr = setDefault.begin(); itr != setDefault.end(); ++itr)
    {
        if ( itGroup->second.IsPosEmpty( (unsigned char)*itr ) )
        {
            itemid_t nItemID = m_EquipManager.GetEquipedCloth( *itr );
            ItemEquip *pCloth = (ItemEquip *)m_ItemManager.GetItemByID( nItemID );
            if ( pCloth != NULL && pCloth->IsEquiped() )
            {
                itGroup->second.AddCloth( (unsigned char)*itr, pCloth->GetType() );
                bRefresh = true;
            }
        }
    }

    if (bRefresh)
    {
        RoleClothGroup* pClothGroup = new RoleClothGroup();
        *pClothGroup = itGroup->second;
        int nUpdateItems = 1;//更新道具标志
        AddQuery(QUERY_UpdateRoleClothGroup, nUpdateItems, NULL, m_pRoleAttr->GetRoleID(), pClothGroup);

        GameMsg_S2C_UpdateClothGroup synMsg;
        synMsg.m_nGroupID = itGroup->first;
        for (std::map<unsigned char, itemtype_t>::const_iterator itItem = itGroup->second.GetClothes().begin();
            itItem != itGroup->second.GetClothes().end(); ++itItem)
        {
            ClothGroupItem e;
            e.m_nType = itItem->first;
            e.m_nItemType = itItem->second;
            synMsg.m_listItems.push_back(e);
        }
        SendPlayerMsg(&synMsg);
    }
}

bool CEntityItemComponent::IsConflictEquiped(EItemClothType pos)
{
    std::set<EItemClothType> setConflictEquipPos;
    _GetConflictEquipPos(pos, setConflictEquipPos);
    for (std::set<EItemClothType>::iterator it = setConflictEquipPos.begin(); it != setConflictEquipPos.end(); ++it)
    {
        itemid_t nItemID = m_EquipManager.GetClothItemID((int)*it);
        ItemEquip * pCloth = (ItemEquip *)m_ItemManager.GetItemByID(nItemID);
        if (pCloth != NULL)
        {
            return pCloth->IsEquiped();
        }
    }

    return false;
}

void CEntityItemComponent::_GetConflictEquipPos( EItemClothType pos, std::set<EItemClothType>& setConflictEquipPos )
{
    for (std::map<EItemClothType, std::set<EItemClothType> >::iterator it = m_mapConflictEquipPos.begin();
        it != m_mapConflictEquipPos.end(); ++it)
    {
        if ( it->first == pos )
        {
            setConflictEquipPos.insert(it->second.begin(), it->second.end());
        }
        else if ( it->second.find(pos) != it->second.end() )
        {
            setConflictEquipPos.insert(it->first);
        }
    }
}

void CEntityItemComponent::OnGetCoolDown( GameMsg_Base &msg, CSlotPeer &slotPeer )
{
    GameMsg_S2C_GetCoolDown retMsg;
    m_ItemManager.GetItemCoolingData(retMsg.m_listItemCoolDown);
    SendPlayerMsg(&retMsg);
}

void CEntityItemComponent::InitializeEquipConflict()
{
    if ( m_mapConflictEquipPos.empty() )
    {
        m_mapConflictEquipPos[eItemClothType_Suit].insert(eItemClothType_Body);
        m_mapConflictEquipPos[eItemClothType_Suit].insert(eItemClothType_Leg);

        std::set<EItemClothType>& setAll = m_mapConflictEquipPos[eItemClothType_All];
        for (int i = eItemClothType_Hair; i < eItemClothType_Max; ++i)
        {
            if ( i != eItemClothType_All && i != eItemClothType_Skin)
            {
                setAll.insert((EItemClothType)i);
            }
        }
    }
}

void CEntityItemComponent::SaveItemToItemInfo(const ItemBase* pItem,DBItem& itemInfo)
{
    ASSERT(pItem);

    if(pItem)
    {
        itemInfo.m_nTypeID   =  pItem->GetType();
        itemInfo.m_nCount    =  pItem->GetCount();
        itemInfo.m_nItemID   =  pItem->GetItemID();
        itemInfo.m_nUseTimes =  pItem->GetLeftUseTimes();
        itemInfo.m_nDuration =  pItem->GetDueTime();
        itemInfo.m_nDynamicBoxRelatedValue = pItem->GetDynamicBoxRelatedValue();
        if(pItem->IsEquip())
        {
			itemInfo.m_bEquiped = ((ItemEquip*)pItem)->IsEquiped();
        }
    }
}

void CEntityItemComponent::UpdateEquipPerpetualEffectScore( unsigned int nNewScore, unsigned int nOldScore )
{
    if ( nNewScore == nOldScore )
        return;

    m_nPerpetualEffectScore = m_nPerpetualEffectScore + nNewScore - nOldScore;
    m_bPerpetualEffectScoreChange = true;
}

void CEntityItemComponent::UpdatePerpetualEquipCount( unsigned int nChangeValue )
{
    if ( nChangeValue == 0 )
        return;

    m_nPerpetualEquipCount += nChangeValue;
    m_bPerpetualEquipCountChange = true;
}

ItemBase * CEntityItemComponent::GetItem(int nColumn, int nSlot)
{
	itemid_t nItemID = 0;
    if (nColumn >= (int)EBagColumn_Max)
    {
		nItemID = m_EquipManager.GetItemID((EEquipColumn)nColumn, nSlot);
    }
	else
	{
		nItemID = m_BagManager.GetItemID((EBagColumn)nColumn, nSlot);
	}

	return m_ItemManager.GetItemByID(nItemID);
}

void CEntityItemComponent::OnMoveItem(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_MoveItem & c2smsg = (GameMsg_C2S_MoveItem&)msg;
    std::list<itemgrid_t> listAddGrid;
    int nError = MoveItem(c2smsg.m_listFromGrid, listAddGrid);
    if (nError != EItemError_Success)
    {
        GameMsg_S2C_MoveItemFail failmsg;
        failmsg.m_nErrorFlag = nError;
        SendPlayerMsg(&failmsg);
    }
    else
    {
        GameMsg_S2C_MoveItemScuess scuessmsg;
        scuessmsg.m_listGrid = listAddGrid;
        SendPlayerMsg(&scuessmsg);
    }
}

bool CEntityItemComponent::IsBagEnoughSlot(const std::map<itemtype_t, itemcountcolumn_t> & mapSelectItem)
{
    std::map<int, int> mapNeedSlot;
    for (std::map<itemtype_t, itemcountcolumn_t>::const_iterator it = mapSelectItem.begin(); it != mapSelectItem.end(); ++it)
    {
        std::list<itemid_t> listExistItemID;
        m_ItemManager.SearchItemByType(it->first, listExistItemID);

        int nNeedSlotCount = GetItemPutInNeedSlotCount(it->second.second, it->first, it->second.first, listExistItemID);
        mapNeedSlot[it->second.second] += nNeedSlotCount;
    }

    for (std::map<int, int>::iterator itr = mapNeedSlot.begin(); itr != mapNeedSlot.end(); ++itr)
    {
        if ((int)m_BagManager.GetColumnEmptyCount((EBagColumn)itr->first) < itr->second)
        {
            return false;
        }
    }

    return true;
}

int CEntityItemComponent::GetMoveToBagColumn(ItemBase * pItemBase)
{
    if (pItemBase == NULL)
    {
        return (int)EBagColumn_Invalid;
    }

    if (pItemBase->GetColumn() <= (int)EBagColumn_BadgeBag && pItemBase->GetColumn() > (int)EBagColumn_Invalid)
    {
        return (int)EBagColumn_Storage;
    }
    else if (pItemBase->GetColumn() == (int)EBagColumn_Storage)
    {
        if (pItemBase->IsCloth() || pItemBase->IsHorse())
        {
            return (int)EBagColumn_ClothBag;
        }
        else if (pItemBase->IsExpandable())
        {
            return (int)EBagColumn_ExpandableBag;
        }
        else if (pItemBase->IsBadge())
        {
            return (int)EBagColumn_BadgeBag;
        }
    }

    WriteLog(LOGLEVEL_ERROR, "[Item] GetMoveToBagColumn: itemid = %lld, column = %d", pItemBase->GetItemID(), pItemBase->GetColumn());
    return (int)EBagColumn_Invalid;
}

int CEntityItemComponent::MoveItem(const std::list<itemgrid_t> & listToGrid, std::list<itemgrid_t> & listAddGrid)
{
    std::map<itemtype_t, itemcountcolumn_t> mapSelectItem;
    for (std::list<itemgrid_t>::const_iterator it = listToGrid.begin(); it != listToGrid.end(); ++it)
    {
        itemid_t nItemID = m_BagManager.GetItemID((EBagColumn)it->first, it->second);
        ItemBase * pItemBase = m_ItemManager.GetItemByID(nItemID);
        if (pItemBase == NULL)
        {
            return EItemUse_EmptyPos;
        }

        std::map<itemtype_t, itemcountcolumn_t>::iterator itr = mapSelectItem.find(pItemBase->GetType());
        if (itr != mapSelectItem.end())
        {
            itr->second.first += pItemBase->GetCount();
        }
        else
        {
            int nToColumn = GetMoveToBagColumn(pItemBase);
            itemcountcolumn_t cct(pItemBase->GetCount(), nToColumn);
            mapSelectItem.insert(make_pair(pItemBase->GetType(), cct));
        }
    }

    if (!IsBagEnoughSlot(mapSelectItem))
    {
        return EItemUse_ColumnFull;
    }
    
    m_BagManager.BeginCollectBagChange();
    for (std::list<itemgrid_t>::const_iterator it = listToGrid.begin(); it != listToGrid.end(); ++it)
    {
        itemid_t nItemID = m_BagManager.GetItemID((EBagColumn)it->first, it->second);
        ItemBase * pItemBase = m_ItemManager.GetItemByID(nItemID);
        if (pItemBase == NULL)
        {
            return EItemUse_EmptyPos;
        }

        int nToColumn = GetMoveToBagColumn(pItemBase);
        if (pItemBase->IsEquip())
        {
            m_BagManager.RemoveItem(pItemBase);
            m_BagManager.AddItem((EBagColumn)nToColumn, pItemBase);
            UpdateItemToDB(pItemBase);

            itemgrid_t grid(nToColumn, pItemBase->GetGrid());
            listAddGrid.push_back(grid);
        }
        else
        {
            std::list<itemid_t> listExistItemID;
            m_ItemManager.SearchItemByType(pItemBase->GetType(), listExistItemID);

            int nTempCount = pItemBase->GetCount();
            int nFirstMoveGrid = -1;
            for (std::list<itemid_t>::iterator itr = listExistItemID.begin(); itr != listExistItemID.end(); ++itr)
            {
                if (nTempCount <= 0)
                {
                    break;
                }

                ItemBase * pExistBase = m_ItemManager.GetItemByID(*itr);
                if (pExistBase != NULL && pExistBase->GetColumn() == nToColumn)
                {
                    int nUpdateCount = (int)pExistBase->GetLeftStackCount() >= nTempCount ? nTempCount : (int)pExistBase->GetLeftStackCount();
                    if (nUpdateCount > 0)
                    {
                        nFirstMoveGrid = nFirstMoveGrid > 0 ? nFirstMoveGrid : pExistBase->GetGrid();
                        UpdateItemCount(pExistBase, nUpdateCount, EItemAction_Add_Move, false);
                        pItemBase->SetCount(pItemBase->GetCount() - nUpdateCount);
                        nTempCount -= nUpdateCount;
                    }
                }
            }

            if (nTempCount > 0)
            {
                m_BagManager.RemoveItem(pItemBase);
                m_BagManager.AddItem((EBagColumn)nToColumn, pItemBase);
                UpdateItemToDB(pItemBase);

                itemgrid_t grid(nToColumn, pItemBase->GetGrid());
                listAddGrid.push_back(grid);
            }
            else
            {
                DeleteItem(pItemBase, EItemAction_Del_Move);

                itemgrid_t grid(nToColumn, nFirstMoveGrid);
                listAddGrid.push_back(grid);
            }
        }
    }

    std::list<itemgrid_t> listGrid;
    m_BagManager.EndCollectBagChange(listGrid);
    RefreshClientItem(listGrid);

    return EItemError_Success;
}

void CEntityItemComponent::OnAddStorageCapacity(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    int nError = AddStorageCapacity();
    if (nError != EItemError_Success)
    {
        GameMsg_S2C_AddStorageCapacityFail failmsg;
        failmsg.m_nErrorFlag = nError;
        SendPlayerMsg(&failmsg);
    }
    else
    {
        GameMsg_S2C_AddStorageCapacityScuess scuessmsg;
        scuessmsg.m_nStorageCapacity = m_BagManager.GetStoragesCapacity();
        SendPlayerMsg(&scuessmsg);
    }
}

int CEntityItemComponent::AddStorageCapacity()
{
    if (m_BagManager.GetStorageCanAddCapacity() == 0)
    {
        return EItemUse_ColumnFull;
    }

    int nCostType = BagConfigManager::Instance().GetAddStorageCurrencyType();
    int nCostValue = BagConfigManager::Instance().GetAddStorageCurrencyValue();
    if (!m_pRoleAttr->IsEnoughCurrency((EMallCurrencyType)nCostType, nCostValue))
    {
        return EItemUse_NotEnoughMoney;
    }

    switch (nCostType)
    {
    case EMallCurrencyType_MCoin:
        m_pRoleAttr->ChangeBill((-1)*nCostValue, EChangeBillCause_Del_AddStorageCapacity);
        break;
    case EMallCurrencyType_Money:
        m_pRoleAttr->ChangeMoney((-1)*nCostValue, EChangeMoneyCause_Del_AddStorageCapacity, 0);
        break;
    case EMallCurrencyType_BindMCoin:
        m_pRoleAttr->ChangeBindBill((-1)*nCostValue, EChangeBindBillCause_Del_AddStorageCapacity);
        break;
    default:
        break;
    }

    m_BagManager.AddStorageCapacity(1);
    

    if (m_StorageCapacityRecord.m_nVipLevel == 0 && m_StorageCapacityRecord.m_nAddCapacity == 0)
    {
        m_StorageCapacityRecord.m_nAddCapacity++;
        InsertStorageCapacityToDB();
    }
    else
    {
        m_StorageCapacityRecord.m_nAddCapacity++;
        UpdateStorageCapacityToDB();
    }
    
    return EItemError_Success;
}

void CEntityItemComponent::UpdateStorageCapacity(int nVipLevel)
{
    if (nVipLevel > m_StorageCapacityRecord.m_nVipLevel)
    {
        if (m_StorageCapacityRecord.m_nVipLevel == 0 && m_StorageCapacityRecord.m_nAddCapacity == 0)
        {
            m_StorageCapacityRecord.m_nVipLevel = m_pRoleVIP->VIPLevel();
            InsertStorageCapacityToDB();
        }
        else
        {
            m_StorageCapacityRecord.m_nVipLevel = m_pRoleVIP->VIPLevel();
            UpdateStorageCapacityToDB();
        }

        int nNewCapacity = CVIPDataMgr::Instance().GetStorageCapacity(m_StorageCapacityRecord.m_nVipLevel) + m_StorageCapacityRecord.m_nAddCapacity;
        int nAddCapacity = nNewCapacity - m_BagManager.GetStoragesCapacity();
        m_BagManager.AddStorageCapacity(nAddCapacity);

        GameMsg_S2C_AddStorageCapacityScuess scuessmsg;
        scuessmsg.m_nStorageCapacity = m_BagManager.GetStoragesCapacity();
        SendPlayerMsg(&scuessmsg);
    }
}

int CEntityItemComponent::CalcStorageCapacity(const StorageCapacityRecord & record, int nVipLevel)
{
    if (record.m_nVipLevel == 0 && record.m_nAddCapacity == 0 && nVipLevel > 0)
    {
        m_StorageCapacityRecord.m_nVipLevel = nVipLevel;
        m_StorageCapacityRecord.m_nAddCapacity = record.m_nAddCapacity;

        InsertStorageCapacityToDB();
    }
    else if (nVipLevel > record.m_nVipLevel)
    {
        m_StorageCapacityRecord.m_nVipLevel = nVipLevel;
        m_StorageCapacityRecord.m_nAddCapacity = record.m_nAddCapacity;

        UpdateStorageCapacityToDB();
    }
    else
    {
        m_StorageCapacityRecord.m_nVipLevel = record.m_nVipLevel;
        m_StorageCapacityRecord.m_nAddCapacity = record.m_nAddCapacity;
    }

    if (m_StorageCapacityRecord.m_nVipLevel > 0)
    {
        return CVIPDataMgr::Instance().GetStorageCapacity(m_StorageCapacityRecord.m_nVipLevel) + m_StorageCapacityRecord.m_nAddCapacity;
    }
    else
    {
        return BagConfigManager::Instance().GetStorageMinCapacity() + m_StorageCapacityRecord.m_nAddCapacity;
    }
    

//    return 0;
}

void CEntityItemComponent::InsertStorageCapacityToDB()
{
    StorageCapacityRecord * pStorageRecord = new StorageCapacityRecord(m_StorageCapacityRecord);
    if (pStorageRecord != NULL)
    {
        AddQuery(QUERY_InsertStorageCapacity, m_pRoleAttr->GetRoleID(), pStorageRecord);
    }
}

void CEntityItemComponent::UpdateStorageCapacityToDB()
{
    StorageCapacityRecord * pStorageRecord = new StorageCapacityRecord(m_StorageCapacityRecord);
    if (pStorageRecord != NULL)
    {
        AddQuery(QUERY_UpdateStorageCapacity, m_pRoleAttr->GetRoleID(), pStorageRecord);
    }
}



