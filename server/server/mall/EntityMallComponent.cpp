#include "../logic/LogicCirculator.h"
//#include "../../liblog/LogType.h"
#include "EntityMallComponent.h"
#include "Mall.h"
#include "CurrencyType.h"
#include "MallDataMgr.h"
#include "MallMsgDefine.h"
#include "GameMsg_Mall.h"
#include "../mail/MailMgr.h"
#include "../item/EntityItemComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../friend/EntityFriendComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../mail/EntityMailComponent.h"
#include "../friend/EntityFriendComponent.h"
#include "../vip/EntityVIPComponent.h"
#include "../Notification/NotificationEvent.h"
#include "../Notification/NotificationComponent.h"
#include "../../datastructure/ItemErrorInfo.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../../socket/GameMsg_Map.h"
#include "../../socket/Formatter.h"
#include "../chat/GameMsg_S2C_Chat.h"
#include "../Money/MoneyMgr.h"
#include "../../datastructure/Localization.h"
#include "../vip/VIPDataMgr.h"
#include "../item/ItemProcess.h"
#include <vector>
#include "../../datastructure/datainfor/ServerConfig.h"
#include "GameMsg_SpecialMall.h"
#include "../activity/EntityActivityComponent.h"
#include "../util/CommonFunc.h"
#include "../SystemSetting/SystemSettingMgr.h"
#include "../quest/EntityQuestNewComponent.h"

extern CLogicCirculator* g_pCirculator;

#define UPDATE_COUNT_PER_TIME	1

#define BUFFLEN 256		

#define SPECIALMALL_UPDATE_INTERVAL 100 // ms
#define SPECIALMALL_BUY_INTERVAL    100 // ms
#define SPECIALMALL_BID_INTERVEL    300 // ms

CEntityMallComponent::CEntityMallComponent(void)
:CEntityComponent(true,true)
{
	m_pRoleItem   = NULL;
	m_pRoleAttr   = NULL;
	m_pRoleMail   = NULL;
	m_pNetCpt     = NULL;
	m_pRoleVIP    = NULL;
	m_pRoleCouple = NULL;
	m_pRoleActivity = NULL;
    m_pRoleQuest = NULL;

	m_pNotification = NULL;
	m_nLastRequestTime = 0;
    m_nLastSpecialMallUpdateTime = 0;
    m_nLastSpecialMallBuyTime = 0;
    m_nLastSpecialMallBidTime = 0;
    m_nSendItemCostBill = 0;
    m_nSendItemTime = 0;

    m_bAuctionIsOpen = false;
    m_tAuctionEndTime = 0;

	m_experienceCardUsedNum = 0;						
	m_experienceCardUsedStamp = 0;						
}

CEntityMallComponent::~CEntityMallComponent(void)
{
}

void CEntityMallComponent::Start()
{
	m_pRoleItem    = GetComponent<CRoleItem>();
	m_pRoleAttr    = GetComponent<CRoleAttribute>();
	m_pRoleMail    = GetComponent<CRoleMail>();
	m_pRoleFriend  = GetComponent<CRoleFriend>();
	m_pNetCpt      = GetComponent<CRoleNet>();
	m_pRoleVIP	   = GetComponent<CRoleVIP>();
	m_pRoleCouple  = GetComponent<CRoleCouple>();
	m_pRoleActivity = GetComponent<CRoleActivity>();
	m_pNotification = GetComponent<CRoleNotification>();
    m_pRoleQuest    = GetComponent<CRoleQuestNew>();
}

bool CEntityMallComponent::CreateFromDB(ROLEINFO_DB* pRoleInforDB)
{
    m_mapLimitedEditionBuy = pRoleInforDB->m_mapLimitedEditionBuy;

    m_mapTopPrice = pRoleInforDB->m_mapAuctionNotGiveInfo;

    // 商城每日赠送花费M币统计
    if ( IsTimeIsToday(pRoleInforDB->nMallSendItemTime) && pRoleInforDB->nMallSendItemTime <= (unsigned int)time(NULL) )
    {
        m_nSendItemCostBill = pRoleInforDB->nMallSendItemCostBill;
        m_nSendItemTime = (unsigned int)time(NULL);
    }

	// 体验卡：
	for (std::vector<itemtype_t>::iterator iter = pRoleInforDB->m_vecExperiencedProducts.begin();
		iter != pRoleInforDB->m_vecExperiencedProducts.end(); ++iter)
	{
		m_setExperienceCardBuyedProduct.insert(*iter);
	}
	m_experienceCardUsedNum = pRoleInforDB->m_experiencedTimes;
	m_experienceCardUsedStamp = pRoleInforDB->m_experiencedStamp;

	ResetExperienceCardInfo(time(NULL));
    
    // 愿望物品
    m_DesiredItem.Initialize(pRoleInforDB->m_listDesiredItem);

    return true;
}

bool CEntityMallComponent::PacketToCache(ROLEINFO_DB *pRoleInfoCache) const
{
    pRoleInfoCache->m_vecExperiencedProducts.assign(m_setExperienceCardBuyedProduct.begin(), m_setExperienceCardBuyedProduct.end());

    pRoleInfoCache->m_experiencedTimes = m_experienceCardUsedNum;
    pRoleInfoCache->m_experiencedStamp = m_experienceCardUsedStamp;

    pRoleInfoCache->m_listDesiredItem.clear();
    GetDesireList( pRoleInfoCache->m_listDesiredItem );

    return true;
}


void CEntityMallComponent::OnLogin()
{
    for (std::map<long long, CAuctionNotGivePointInfo>::iterator itr = m_mapTopPrice.begin(); 
         itr != m_mapTopPrice.end();)
    {
        if (itr->second.m_bTopPrice)
            ++itr;
        else
        {
            AuctionReturnPoint(itr->second.m_lLogID, itr->second.m_uIdx, itr->second.m_tEndTime, itr->second.m_uBidPrice);

            m_mapTopPrice.erase(itr++);
        }
    }
}

void CEntityMallComponent::RegComponentNetMsgMap()
{
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_MallGoodsInfo);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_MallSomeGoods);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_MallBuy);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_MallSend);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_MallAskFor);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2S_MallAskFor);
	
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RenewItem);
	
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_EnterMall);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_BeAskedFor_Send);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ActionEnterMall);

    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_SpecialMall_State);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_SpecialMall_StateResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GroupBuying_Items);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_GroupBuying_ItemsResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GroupBuying_Buy);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_GroupBuying_BuyResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_LimitedEdition_Items);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_LimitedEdition_ItemsResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_LimitedEdition_Buy);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_LimitedEdition_BuyResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_Auction_Items);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_Auction_Bid);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_Auction_BidResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_Auction_Surpassed);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_SpecialMall_Pay);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_Auction_UpdateState);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_Auction_BidSuccess);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_Auction_ItemsResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_Auction_CheckTopPriceResult);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ExperienceCardInfo);
	
	RegMsgProcessor(MSG_C2S_MALL_GETGOODS, &CEntityMallComponent::GetGoodsInfo);
	RegMsgProcessor(MSG_C2S_MALL_GETSOMEGOODS, &CEntityMallComponent::GetSomeGoods);
	RegMsgProcessor(MSG_C2S_MALL_BUY, &CEntityMallComponent::OrderBuy);
	RegMsgProcessor(MSG_C2S_MALL_SEND, &CEntityMallComponent::OrderSend);
	RegMsgProcessor(MSG_C2S_MALL_ASKFOR, &CEntityMallComponent::OrderAskFor);
	RegMsgProcessor(MSG_S2S_MALL_ASKFOR, &CEntityMallComponent::OnBeAskedFor);
	
	RegMsgProcessor(MSG_C2S_MALL_RENEW, &CEntityMallComponent::OnRenewItem);
	
	RegMsgProcessor(MSG_C2S_MALL_BEASKED_SEND, &CEntityMallComponent::OnBeAskedSend);
	RegMsgProcessor(MSG_C2S_MALL_EnterMall, &CEntityMallComponent::OnFirsrEnterMallOneDay);

    RegMsgProcessor(MSG_C2S_MALLAction_EnterMall, &CEntityMallComponent::OnEnterMall);
    RegMsgProcessor(MSG_C2S_SpecialMall_State, &CEntityMallComponent::OnGetSpecialMallState);
    RegMsgProcessor(MSG_S2C_SpecialMall_StateResult, &CEntityMallComponent::OnSpecialMallMsgRes);
    RegMsgProcessor(MSG_C2S_GroupBuying_Items, &CEntityMallComponent::OnSpecialMallUpdateMsg);
    RegMsgProcessor(MSG_S2C_GroupBuying_ItemsResult, &CEntityMallComponent::OnGroupBuyingGetItemsRes);
    RegMsgProcessor(MSG_C2S_GroupBuying_Buy, &CEntityMallComponent::OnGroupBuyingBuyMsg);
    RegMsgProcessor(MSG_S2C_GroupBuying_BuyResult, &CEntityMallComponent::OnSpecialMallMsgRes);
    RegMsgProcessor(MSG_C2S_LimitedEdition_Items, &CEntityMallComponent::OnSpecialMallUpdateMsg);
    RegMsgProcessor(MSG_S2C_LimitedEdition_ItemsResult, &CEntityMallComponent::OnLimitedEditionGetItemsRes);
    RegMsgProcessor(MSG_C2S_LimitedEdition_Buy, &CEntityMallComponent::OnLimitedEditionBuyMsg);
    RegMsgProcessor(MSG_S2C_LimitedEdition_BuyResult, &CEntityMallComponent::OnSpecialMallMsgRes);

    RegMsgProcessor(MSG_C2S_Auction_Items, &CEntityMallComponent::OnSpecialMallUpdateMsg);
    RegMsgProcessor(MSG_C2S_Auction_Bid, &CEntityMallComponent::OnAuctionBidMsg);
    RegMsgProcessor(MSG_G2S_Auction_BidResult, &CEntityMallComponent::OnAuctionBidRes);
    RegMsgProcessor(MSG_G2S_Auction_Surpassed, &CEntityMallComponent::OnAuctionSurpassed);
    RegMsgProcessor(MSG_G2S_SpecialMall_Pay, &CEntityMallComponent::OnSpecialMallPay);
    RegMsgProcessor(MSG_G2S_Auction_UpdateState, &CEntityMallComponent::OnAuctionUpdateState);
    RegMsgProcessor(MSG_G2S_Auction_BidSuccess, &CEntityMallComponent::OnAuctionBidSuccess);
    RegMsgProcessor(MSG_G2S_Auction_ItemsResult, &CEntityMallComponent::OnAuctionGetItemsRes);
	RegMsgProcessor(MSG_G2S_Auction_CheckTopPriceResult, &CEntityMallComponent::OnAuctionCheckTopPriceResult);
	RegMsgProcessor(MSG_C2S_MallExperienceCardInfo, &CEntityMallComponent::OnExperienceCardInfo);

    // 愿望
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RequestDesireList);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_WishComeTrue);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_MallDesire);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_SendWishItem);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RemoveDesire);

    RegMsgProcessor(MSG_C2S_Desire, &CEntityMallComponent::OnDesire);
    RegMsgProcessor(MSG_C2S_RequestDesireList, &CEntityMallComponent::OnRequestDesireList);
    RegMsgProcessor(MSG_G2S_WishComeTrue, &CEntityMallComponent::OnGetDesiredItem);
    RegMsgProcessor(MSG_C2S_MALL_SENDWISHITEM, &CEntityMallComponent::OnSendWishItem);
    RegMsgProcessor(MSG_C2S_RemoveDesire, &CEntityMallComponent::OnRemoveDesire);

    RegQueryProcessor(QUERY_AuctionReturnPoint, &CEntityMallComponent::OnDBAuctionReturnPoint);
}

// 发送整个商城的商品列表，属于静态数据
void CEntityMallComponent::SendExtraStaticData()
{
	const static unsigned short kGoodsCountPerMessage = 1000;

	MapMallGoods & allGoods = CMall::Instance().MallGoods();
	list<itemtype_t> &listGoodsID = allGoods.GetKeyList();

	unsigned short nGoodsCount = allGoods.Count();
	unsigned short nMessageCount = nGoodsCount / kGoodsCountPerMessage;
	unsigned short nRemainCount = nGoodsCount % kGoodsCountPerMessage;

	unsigned short i = 0;
	std::vector<unsigned int> vecGoodsID(nGoodsCount, 0);	// 支持随机访问，更快
	for (list<itemtype_t>::iterator it = listGoodsID.begin(); it != listGoodsID.end(); ++it)
	{
		vecGoodsID[i++] = *it;
	}
	
	for (i = 0; i < nMessageCount; i++)
	{
		GameMsg_S2C_AllGoodsInfo msg;
		msg.m_nMessageCount = nMessageCount;
		msg.m_nMessageIndex = i + 1;
		msg.m_nGoodsCount = kGoodsCountPerMessage;

		if (nRemainCount > 0)
			msg.m_nMessageCount = nMessageCount + 1;

		for (unsigned short j = 0; j < kGoodsCountPerMessage; j++)
		{
			msg.m_listGoods.push_back(vecGoodsID[i * kGoodsCountPerMessage + j]);
		}

		m_pNetCpt->SendPlayerMsg(&msg);
	}

	if (nRemainCount > 0 || nMessageCount == 0)
	{
		GameMsg_S2C_AllGoodsInfo msg;
		msg.m_nMessageCount = nMessageCount + 1;
		msg.m_nMessageIndex = nMessageCount + 1;
		msg.m_nGoodsCount = nRemainCount;

		for (unsigned short j = 0; j < nRemainCount; j++)
		{
			msg.m_listGoods.push_back(vecGoodsID[i * kGoodsCountPerMessage + j]);
		}
		m_pNetCpt->SendPlayerMsg(&msg);
	}
}

void CEntityMallComponent::GetGoodsInfo(GameMsg_Base & msg,CSlotPeer & SlotPeer)
{
	GameMsg_C2S_MallGoodsInfo * pMsg = (GameMsg_C2S_MallGoodsInfo *)&msg;
	if(m_pNetCpt != NULL && pMsg != NULL && IsTimeOK())
	{
		GameMsg_S2C_MallGoodsInfo * pMsg2c = CMall::Instance().GetGoodsMsg(pMsg->nType);
		if (pMsg2c != NULL){
			m_pNetCpt->SendPlayerMsg(pMsg2c);
		}
	}
}

void CEntityMallComponent::GetSomeGoods(GameMsg_Base & msg,CSlotPeer & SlotPeer)
{
	GameMsg_C2S_MallSomeGoods * pMsg = (GameMsg_C2S_MallSomeGoods *)&msg;
	if(m_pNetCpt != NULL && pMsg != NULL && IsTimeOK())
	{
		GameMsg_S2C_MallSomeGoodsResult msgReturn;
		std::list<itemtype_t>::iterator aBegin = pMsg->aListType.begin();
		while (aBegin != pMsg->aListType.end()){
			CMall::Instance().GetGoodsDataListByID(*aBegin,msgReturn.aGoodMap);
			aBegin++;
		}
		m_pNetCpt->SendPlayerMsg(&msgReturn);
	}
}


void CEntityMallComponent::OrderBuy(GameMsg_Base & msg,CSlotPeer & SlotPeer)
{
    GameMsg_C2S_MallBuy * pMsg = (GameMsg_C2S_MallBuy *)&msg;
    if (pMsg != NULL && IsTimeOK() && m_pRoleAttr != NULL)
    {
        MapPeddleGoods mapGoods;
        CurrencyMap mapBills;

        int nReturn = CanBuy(pMsg->vecGoods,mapGoods,mapBills);
        if (nReturn == MALL_OK && pMsg->vecGoods.size() > 0) 
        {
            if (!CanBuyBadageCard(pMsg->vecGoods[0].m_nType)) // 如果徽章对应个格子已经打开
                nReturn = MALL_ITEM_CANNOTBUYBADGECARD;
        }

        if (nReturn == MALL_OK)
        {
            //购买并使用
            if (pMsg->m_nBuyFromType == GOODS_BUYFROM_TYPE_SAVEMOULD 
                || pMsg->m_nBuyFromType == GOODS_BUYFROM_TYPE_QUICKBILLANDUSED)
            {
                //保存形象的购买，需要将购买上的新衣服都穿在身上  快速支付购买并使用
                SetGoodsAddUseValue(mapGoods,true);
            }

            nReturn = Buy(mapGoods, mapBills);

            std::vector<itemtype_t> vecExperienceItem;
            for (MapPeddleGoods::iterator it = mapGoods.begin(); it != mapGoods.end(); ++it)
            {
                PeddleGood &goods = it->second;
                AfterBuyGoods(goods);

                if (goods.m_nSelIndex == EPeddleGoodSel_Experience)
                    vecExperienceItem.push_back(goods.m_nType);
            }

            if (!vecExperienceItem.empty())
                NotifyNewExperiencedItem(vecExperienceItem);

            // 每天第一次购买做评价消息推送
            if (nReturn == MALL_OK && m_pNotification)
            {
                CNotificationEvent event;
                event.m_nEventType = NOTIFICATION_FINISH_SHOPING;
                m_pNotification->AddEvent(event);
            }

            if (MALL_OK == nReturn)
            { // 
                for (MapPeddleGoods::iterator iter = mapGoods.begin(); iter != mapGoods.end(); ++iter)
                {
                    unsigned short nCount = 0;
                    int validTime = 0;
                    GetCountOrValidTimeByGoodSelIndex(iter->second.m_nType, iter->second.m_nSelIndex, nCount, validTime);
                    m_pRoleQuest->OnMallBuyItem(iter->second.m_nType, validTime, nCount); // 通知过去
                }
            }
        }
        SendReturnMsg((unsigned char)nReturn,pMsg->m_nBuyFromType,MALL_GOODS_BUY_TYPE_BUY);
    }
}

int CEntityMallComponent::GetPeddleGoodCount(const PeddleGood &goods)
{
    switch (goods.m_nSelIndex)
    {
    case 0:
        {
            return 1;
        }
    	break;
    case 1:
        {
            return 5;
        }
        break;
    case 2:
        {
            return 10;
        }
        break;
    case 3:
        {
            return 20;
        }
    default:return 0;
    }
}

void CEntityMallComponent::AfterBuyGoods(const PeddleGood &goods)
{
    MallGood *mallGoods = CMall::Instance().FindGoods(goods.m_nType);
    if (mallGoods != NULL)
    {
        CommonParam param;
        param.SetParam(std::string("medal"), std::string("buy"));
        param.SetParam(std::string("item_type_form_mall"), (int)goods.m_nType);
        param.SetParam(std::string("item_count_form_mall"), GetPeddleGoodCount(goods) );
        param.SetParam(std::string("is_new"), (int)mallGoods->m_nIsNew);
        param.SetParam(std::string("is_hot"), (int)mallGoods->m_nHot);
        param.SetParam(std::string("use_coupon"), (int)mallGoods->m_nIsCoupon);

        CurrencyMap cost;
        CMall::InitCurrencyMap(cost);
        CMall::CalcGoodsCost(*mallGoods, goods, cost);

        param.SetParam(std::string("mcoin"), (int)cost[EMallCurrencyType_MCoin]);
        param.SetParam(std::string("bind_mcoin"), (int)cost[EMallCurrencyType_BindMCoin]);
        param.SetParam(std::string("money"), (int)cost[EMallCurrencyType_Money]);

        NotifyAll(param);
    }
}

void CEntityMallComponent::OrderSend(GameMsg_Base & msg,CSlotPeer & SlotPeer)
{
	GameMsg_C2S_MallSend * pMsg = (GameMsg_C2S_MallSend *)&msg;
	if (pMsg != NULL && IsTimeOK())
	{
		MapPeddleGoods mapGoods;
		CurrencyMap mapBills;
		int nReturn = CanSend(pMsg->vecGoods,mapGoods,mapBills,pMsg->m_nDestRoleID);
		if (nReturn == MALL_OK)
		{
			nReturn = Send(mapGoods,mapBills,pMsg->m_nDestRoleID,pMsg->m_strInfo, CRoleSendLog::ESendType_Buy);
		}
		SendReturnMsg((unsigned char)nReturn,0,MALL_GOODS_BUY_TYPE_SEND);
	}
}

int CEntityMallComponent::OrderSendFromDivorec( VecPeddleGoods & vecGoods, unsigned int nDestRoleID, unsigned char cSex)
{
	MapPeddleGoods mapGoods;
	CurrencyMap mapBills;
	int nReturn = CanSendFromDivorce(vecGoods,mapGoods,mapBills,nDestRoleID,cSex);
	if (nReturn == MALL_OK)
	{
		nReturn = Send(mapGoods,mapBills,nDestRoleID,"", CRoleSendLog::ESendType_Buy);

		if ( nReturn == MALL_OK )
		{
			SendReturnMsg((unsigned char)nReturn,0,MALL_GOODS_BUY_TYPE_SEND);
		}
	}
	else
	{
		return nReturn;
	}

	return MALL_OK;
}

void CEntityMallComponent::OnRenewItem(GameMsg_Base & msg,CSlotPeer & SlotPeer)
{
	GameMsg_C2S_RenewItem * pMsg = (GameMsg_C2S_RenewItem *)&msg;
	if(pMsg != NULL && IsTimeOK()){
		MapPeddleGoods mapGoods;
		CurrencyMap mapBills;
		int nReturn = CanRenew(pMsg->vecGoods,mapGoods,mapBills,pMsg->m_bFirstUseChange,pMsg->nColumn,pMsg->nIndex);
		if (nReturn == MALL_OK)
		{
			nReturn = Renew(mapGoods,mapBills,pMsg->nColumn,pMsg->nIndex);
		}
		SendReturnMsg((unsigned char)nReturn,0,MALL_GOODS_BUY_TYPE_RENEW);
	}
}


void CEntityMallComponent::OrderAskFor(GameMsg_Base & msg,CSlotPeer & SlotPeer)
{
    GameMsg_C2S_MallAskFor *pMsg = (GameMsg_C2S_MallAskFor *)&msg;

    if(pMsg != NULL && m_pNetCpt != NULL && m_pRoleAttr != NULL && IsTimeOK() )
    {
        for (list<CAskMallGoodInfo>::iterator itr = pMsg->m_listAskGoods.begin(); itr != pMsg->m_listAskGoods.end(); ++itr)
        {
            if(!CMall::Instance().CanBuyGoodByID(itr->m_nSellGoodType) )
            {
                GameMsg_S2C_MallMallAskForResult msgReturn;
                msgReturn.nResult = 1;
                msgReturn.strReturn = "Mall_Can_Not_Buy_Ask_Fail";

                m_pNetCpt->SendPlayerMsg(&msgReturn);

                return;
            }
        }

        if (CSystemSettingMgr::Instance().IsOpenSendGift() )
        {
            CRoleEntity *pDestRole = CPlayerManager::Instance().GetEntityByRoleID(pMsg->m_nDestRoleID);

            if (pDestRole == NULL)
            {
                GameMsg_S2S_MallAskFor *pMsg2G = new GameMsg_S2S_MallAskFor();

                if (pMsg2G != NULL)
                {
                    pMsg2G->m_nAskForRoleID = m_pRoleAttr->GetRoleID();
                    pMsg2G->m_nDestRoleID = pMsg->m_nDestRoleID;
                    pMsg2G->m_listAskGoods = pMsg->m_listAskGoods;
                    pMsg2G->m_strInfo = pMsg->m_strInfo;

                    m_pNetCpt->Send2GroupServer((GameMsg_Base *)pMsg2G);

                    delete pMsg2G, pMsg2G = NULL;
                }
            }
            else
            {
                CEntityMallComponent *pDesMallComponent = pDestRole->GetComponent<CRoleMall>();

                if (pDesMallComponent != NULL)
                {
                    CRoleFriend *friendCmpt = pDesMallComponent->GetComponent<CRoleFriend>();

                    if (friendCmpt->IsInBlackList(m_pRoleAttr->GetRoleID() ) )
                    {
                        GameMsg_S2C_MallMallAskForResult msgReturn;
                        msgReturn.nResult = 0;
                        msgReturn.strReturn = "Mall_Ask_For_Send_Suc";// yes, this is the tip,it was successfully sent.

                        m_pNetCpt->SendPlayerMsg(&msgReturn);
                    }
                    else
                    {
                        pDesMallComponent->ProcessBeAskedFor(m_pRoleAttr->GetRoleID(), pMsg->m_listAskGoods, m_pRoleAttr->GetRoleName(), pMsg->m_strInfo);

                        GameMsg_S2C_MallMallAskForResult msgReturn;
                        msgReturn.nResult = 0;
                        msgReturn.strReturn = "Mall_Ask_For_Send_Suc";

                        m_pNetCpt->SendPlayerMsg(&msgReturn);
                    }
                }
            }
        }
        else
        {
            GameMsg_S2C_MallMallAskForResult msgReturn;
            msgReturn.nResult = MALL_UNKNOWN;
            msgReturn.strReturn = CLocalization::Instance().GetString("System_Tip");

            m_pNetCpt->SendPlayerMsg(&msgReturn);
        }
    }
}

void CEntityMallComponent::OnBeAskedFor(GameMsg_Base & msg,CSlotPeer & SlotPeer)
{
	GameMsg_S2S_MallAskFor * pMsg = (GameMsg_S2S_MallAskFor *)&msg;
	if(pMsg != NULL && m_pNetCpt != NULL){
		if (pMsg->m_nResult == 0){
			//被索要方的处理逻辑
			ProcessBeAskedFor(pMsg->m_nAskForRoleID, pMsg->m_listAskGoods, pMsg->szAskForRoleName, pMsg->m_strInfo);
		}
		else if (pMsg->m_nResult == 1){
			GameMsg_S2C_MallMallAskForResult msgReturn;
			msgReturn.nResult = 2;
			msgReturn.strReturn = "Mall_Not_Onlie_Ask_Fail";
			m_pNetCpt->SendPlayerMsg(&msgReturn);
			//索要方处理逻辑，没找到对方，索要失败！
		}
		else{
			GameMsg_S2C_MallMallAskForResult msgReturn;
			msgReturn.strReturn = "Mall_Ask_For_Send_Suc";
			m_pNetCpt->SendPlayerMsg(&msgReturn);
			//索要方处理逻辑，成功找到对方了，索要成功与否未知，应该都需要成功的
		}
	}
}

void CEntityMallComponent::ProcessBeAskedFor(unsigned int nAskForRoleID, list<CAskMallGoodInfo> &listAskGoodInfo, const char * szAskForRoleName,string strInfo)
{
	if (m_pNetCpt != NULL)
	{
		GameMsg_S2C_MallMallBeAskedFor msg;
		safememcpy(msg.szAskForRoleName,g_ServerConfig.m_MaxNameLimit-1,szAskForRoleName,strlen(szAskForRoleName));
		msg.nAskForRoleID = nAskForRoleID;
		msg.m_listAskGoodInfo = listAskGoodInfo;
		msg.m_strInfo = strInfo;
		for (list<CAskMallGoodInfo>::iterator itr = listAskGoodInfo.begin(); itr != listAskGoodInfo.end(); ++itr) {
			CMall::Instance().GetGoodsDataListByID(itr->m_nSellGoodType, msg.aGoodMap);
		}
		m_pNetCpt->SendPlayerMsg(&msg);
	}
}

void CEntityMallComponent::OnDesire(GameMsg_Base & msg,CSlotPeer & SlotPeer)
{
	GameMsg_C2S_MallDesire & wishmsg = (GameMsg_C2S_MallDesire &)msg;
    if (!IsTimeOK())
    {
        return;
    }

    ItemConfig * pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(wishmsg.m_nTypeID);
    if(pItemInfo == NULL)
    {
        return;
    }

    GameMsg_S2C_MallDesireResult resultmsg;
    if (!CMall::Instance().CanMakeWishByID(wishmsg.m_nTypeID))
    {
        resultmsg.nResult = MALL_CAN_NOT_MAKE_WISH;
        resultmsg.strReturn = "Mall_Can_Not_Make_Wish";
    }
    else if (m_DesiredItem.IsFull())
    {
        resultmsg.nResult = MALL_WISH_WISHLISTFULL;
        resultmsg.strReturn = "Mall_Wish_List_Full";
    }
    else if (m_DesiredItem.IsExist(wishmsg.m_nTypeID))
    {
        resultmsg.nResult = MALL_ITEM_UNIQUEEXIST;
        resultmsg.strReturn = "Mall_Wish_Item_Unique_Exist";
    }
    else if (pItemInfo->IsEquip())
    {
        ItemBase * pItem = m_pRoleItem->SearchItem(wishmsg.m_nTypeID,true);
        if (pItem != NULL && pItem->GetDueTime() <= 0)
        {
            resultmsg.strReturn = "Mall_Wish_Has_Permanent_Item";
            resultmsg.nResult = MALL_WISH_HASPERMANENTITEM;
        }

        int nReturn = m_DesiredItem.AddDesiredItem(wishmsg.m_nTypeID);
        if (nReturn == EItemError_Success)
        {
            // sync to db
            SaveDesiredItemToDB(wishmsg.m_nTypeID);

            // sync to client
            resultmsg.strReturn = "Mall_Wish_Suc";
            unsigned int nCurNum = CMall::Instance().OnDesire(wishmsg.m_nTypeID);
            unsigned int nAddNum = UPDATE_COUNT_PER_TIME;
            if (nCurNum != 0 && nCurNum % nAddNum == 0)
            {
                GameMsg_S2G_MallDesire broadcastmsg;
                broadcastmsg.m_nTypeID = wishmsg.m_nTypeID;
                broadcastmsg.nAddNum = nAddNum;
                m_pNetCpt->Send2GroupServer(&broadcastmsg);
            }

        }
    }
    else
    {
        int nReturn = m_DesiredItem.AddDesiredItem(wishmsg.m_nTypeID);
        if(nReturn == EItemError_Success)
        {
            // sync to db
            SaveDesiredItemToDB(wishmsg.m_nTypeID);

            // sync to client
            resultmsg.strReturn = "Mall_Wish_Suc";
            unsigned int nCurNum = CMall::Instance().OnDesire(wishmsg.m_nTypeID);
            unsigned int nAddNum = UPDATE_COUNT_PER_TIME;
            if (nCurNum != 0 && nCurNum % nAddNum == 0)
            {
                GameMsg_S2G_MallDesire broadcastmsg;
                broadcastmsg.m_nTypeID = wishmsg.m_nTypeID;
                broadcastmsg.nAddNum = nAddNum;
                m_pNetCpt->Send2GroupServer(&broadcastmsg);
            }

        }
    }

    m_pNetCpt->SendPlayerMsg((GameMsg_Base *)&resultmsg);
}

void CEntityMallComponent::OnRemoveDesire(GameMsg_Base & msg, CSlotPeer & SlotPeer)
{
	GameMsg_C2S_RemoveDesire & c2smsg = (GameMsg_C2S_RemoveDesire &)msg;
	ItemConfig * pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(c2smsg.m_nTypeID);
	if (pItemInfo == NULL) 
	{
		return;
	}

	string strError = "";
	if (!m_DesiredItem.IsExist(c2smsg.m_nTypeID))
	{
		strError = "Mall_Wish_Item_Not_Exist";
	}
	else
	{
        if (!m_DesiredItem.RemoveDesiredItem(c2smsg.m_nTypeID))
		{
			strError = "Mall_Data_Error_Remove_Wish_Item_Fail";
		}

        RemoveDesiredItemToDB(c2smsg.m_nTypeID);

        // sync to client
        GameMsg_S2C_RemoveDesireScuess scuessmsg;
        scuessmsg.m_nItemType = c2smsg.m_nTypeID;
        m_pNetCpt->SendPlayerMsg(&scuessmsg);

        // 暂时处理前端问题发假消息，后面要去除
        m_pRoleItem->RefreshClientItem(EItemColumn_Max, 0, 0);
	}

	if (strError != "")
	{
		GameMsg_S2C_RemoveDesireItemFail failMsg;
		failMsg.m_strEorror = strError;
		m_pNetCpt->SendPlayerMsg(&failMsg);
	}
}

void CEntityMallComponent::GetDesireList(std::list<itemtype_t> & listDesire) const
{
    m_DesiredItem.GetListOfDesiredItem(listDesire);
}

void CEntityMallComponent::SaveDesiredItemToDB(itemtype_t nItemType)
{
    AddQuery(QUERY_Desire_Insert, m_pRoleAttr->GetRoleID(), NULL, nItemType);
}

void CEntityMallComponent::RemoveDesiredItemToDB(itemtype_t nItemType)
{
    AddQuery(QUERY_Desire_Delete, m_pRoleAttr->GetRoleID(), NULL, nItemType);
}

void CEntityMallComponent::OnRequestDesireList(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    std::list<itemtype_t> listItemType;
    m_DesiredItem.GetListOfDesiredItem(listItemType);

    GameMsg_S2C_RequestDesireListResult resultmsg;
    resultmsg.m_listDesire = listItemType;
    m_pNetCpt->SendPlayerMsg(&resultmsg);
}

void CEntityMallComponent::OnGetDesiredItem(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_WishComeTrue & g2smsg = (GameMsg_G2S_WishComeTrue &)msg;
    m_DesiredItem.RemoveDesiredItem(g2smsg.m_nItemID);
    RemoveDesiredItemToDB(g2smsg.m_nItemID);
}

void CEntityMallComponent::OnSendWishItem(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
	GameMsg_C2S_SendWishItem *pMsg = (GameMsg_C2S_SendWishItem *)&msg;
	if (pMsg->m_vecGoods.size() != 1)
	{
		return;
	}

	MapPeddleGoods mapGoods;
	CurrencyMap mapBills;
	int nReturn = CanSend(pMsg->m_vecGoods, mapGoods, mapBills, pMsg->m_nDestRoleID);
	if (nReturn == MALL_OK)
	{
        nReturn = Send(mapGoods, mapBills, pMsg->m_nDestRoleID, pMsg->m_strInfo, CRoleSendLog::ESendType_Wish);
        if (nReturn == MALL_OK)
        {
            GameMsg_S2G_WishComeTrue s2gMsg;
            s2gMsg.m_nSrcRoleID = m_pRoleAttr->GetRoleID();
            s2gMsg.m_nDstRoleID = pMsg->m_nDestRoleID;
            s2gMsg.m_nItemID = pMsg->m_vecGoods[0].m_nType;
            m_pNetCpt->Send2GroupServer(&s2gMsg);

            GameMsg_S2C_SendWishItemSuc sucMsg;
            sucMsg.m_nRoleID = pMsg->m_nDestRoleID;
            sucMsg.m_nItemID = pMsg->m_vecGoods[0].m_nType;
            m_pNetCpt->SendPlayerMsg(&sucMsg);
        }
	}

	if (nReturn != MALL_OK)
	{
		GameMsg_S2C_SendWishItemFail failMsg;
		if (CSystemSettingMgr::Instance().IsOpenSendGift())
		{
			if (nReturn == MALL_NOT_MARRID || nReturn == MALL_INTIMACY_NOTENOUGH)
			{
				nReturn = MALL_WEDDINGRING_NOT_SEND;
			}

			GetMallBuyErrorString(nReturn, failMsg.m_strErrorMsg);
		}
		else
		{
			failMsg.m_strErrorMsg = CLocalization::Instance().GetString("System_Tip");
		}

		m_pNetCpt->SendPlayerMsg(&failMsg);
	}
}

void CEntityMallComponent::OnFirsrEnterMallOneDay(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_S2C_EnterMallRes msgEnterRes;
    msgEnterRes.m_strMallTipTitle = CMallDataMgr::Instance().m_strMallTipTitle;
    msgEnterRes.m_strMallTip = CMallDataMgr::Instance().m_strMallTip;
    if (m_pRoleAttr != NULL) 
    {
        unsigned int nTimeNow = time(NULL);
        unsigned int nDaySec = 24 * 60 * 60;
        if (nTimeNow > CMoneyMgr::Instance().GetCurMonthRemindTime(nTimeNow) && nTimeNow < CMoneyMgr::Instance().GetNextMonthMoneyRstTime(nTimeNow)
            && m_pRoleAttr->m_nMallLastMoneyRstRemindtime / nDaySec != nTimeNow / nDaySec) 
        {
            m_pRoleAttr->m_nMallLastMoneyRstRemindtime = nTimeNow;
            msgEnterRes.m_bNotifyWillResetMoney = true;
            AddQuery(QUERY_UpdateMoneyMallRemindTime, m_pRoleAttr->GetRoleID(), NULL, nTimeNow);
        }
    }

    if (m_pNetCpt != NULL) 
    {
        m_pNetCpt->SendPlayerMsg(&msgEnterRes);
    }
}

void CEntityMallComponent::OnEnterMall(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    InsertActionLog(CRoleMallLog::EMallAction_Enter);
}

void CEntityMallComponent::OnBeAskedSend(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
	GameMsg_C2S_BeAskedFor_Send * pMsg = (GameMsg_C2S_BeAskedFor_Send *)&msg;
	if (pMsg != NULL && IsTimeOK())
	{
		MapPeddleGoods mapGoods;
		CurrencyMap mapBills;
		int nReturn = CanSend(pMsg->vecGoods,mapGoods,mapBills,pMsg->m_nDestRoleID);
		if (nReturn == MALL_OK)
		{
			nReturn = Send(mapGoods,mapBills,pMsg->m_nDestRoleID,pMsg->m_strInfo, CRoleSendLog::ESendType_Ask);
		}
		SendReturnMsg((unsigned char)nReturn,0,MALL_GOODS_BUY_TYPE_BEASKED_SEND);
	}
	else
	{
		SendReturnMsg((unsigned char)MALL_OPERATE_TOO_OFTEN,0,MALL_GOODS_BUY_TYPE_BEASKED_SEND);
	}
}

void CEntityMallComponent::GetMallBuyErrorString(int nErrno, string &strError)
{
	if(nErrno == MALL_PARAM_ERROR){
		strError = "Mall_Param_Error";
	}
	else if(nErrno == MALL_NOTOPEN)
	{
		strError = "Mall_Not_Open" ;
	}
	else if(nErrno == MALL_NOTSELL)
	{
		strError = "Mall_Not_Sell" ;
	}
	else if(nErrno == MALL_BILL_CASH_NOTENOUGH)
	{
		strError = "Mall_Bill_Cash_Not_Enough" ;
	}
	else if(nErrno == MALL_BILL_CHANGE_NOTENOUGH)
	{
		strError = "Mall_Bill_Charge_Not_Enough" ;
	}
	else if(nErrno == MALL_BILL_MONEY_NOTENOUGH)
	{
		strError = "Mall_Bill_Money_Not_Enough" ;
	}
	else if(nErrno == MALL_BILL_INCONFORMITY)
	{
		strError = "Mall_Bill_Inconformity" ;
	}
	else if(nErrno == MALL_PAYTYPE_INCONFORMITY)
	{
		strError = "Mall_Paytype_Inconformity" ;
	}
	else if(nErrno == MALL_ITEMLIST_EMPTY)
	{
		strError = "Mall_Item_List_Empty" ;
	}
	else if(nErrno == MALL_ITEMGRID_NOTENOUGH)
	{
		strError = "Mall_Item_Grid_Not_Enough" ;
	}
	else if(nErrno == MALL_ITEM_UNIQUEEXIST)
	{
		strError = "Mall_Item_Unique_Exist" ;
	}
	else if(nErrno == MALL_ITEM_NOTRENEW)
	{
		strError = "Mall_Item_Not_Renew" ;
	}
	else if(nErrno == MALL_ITEM_MAILTOOMUCH)
	{
		strError = "Mall_Item_Mail_Too_Much" ;
	}
	else if(nErrno == MALL_ITEM_COUNTERROR)
	{
		strError = "Mall_Item_Count_Error" ;
	}
	else if(nErrno == MALL_ITEM_SEXERROR)
	{
		strError = "Mall_Item_Sex_Error" ;
	}
	else if(nErrno == MALL_ITEM_FRIENDERROR)
	{
		strError = "Mall_Item_Friend_Error" ;
	}
	else if(nErrno == MALL_ITEM_SAMEERROR)
	{
		strError = "Mall_Item_Same_Error" ;
	}
	else if (nErrno == MALL_ITEM_CANNOTBUYBADGECARD) 
	{
		strError = "Mall_Item_Can_Not_Buy_Badge_Card";
	} 
	else if (nErrno == MALL_OPERATE_TOO_OFTEN) 
	{
		strError = "Mall_Operate_Too_Often";
	}
	else if (nErrno == MALL_CAN_NOT_BUY_PRECIOUS_ITEMS) 
	{
		strError = "Mall_Can_Not_Buy_Precious_Items";
	}
	else if (nErrno == MALL_CAN_NOT_MAKE_WISH)
	{
		strError = "Mall_Can_Not_Make_Wish";
	}
	else if (nErrno == MALL_INTIMACY_NOTENOUGH)
	{
		strError = "MALL_INTIMACY_NOTENOUGH";
	}
	else if (nErrno == MALL_NOT_MARRID)
	{
		strError = "MALL_NOT_MARRID";
	}
	else if (nErrno == MALL_CAN_NOT_USE_EXPERIENCECARD)
	{
		strError = "MALL_CAN_NOT_USE_EXPERIENCECARD";
	}
	else if (nErrno == MALL_EXPERIENCECARD_TYPE_REFUSED)
	{ 
		strError = "MALL_EXPERIENCECARD_TYPE_REFUSED";
	}
	else if (nErrno == MALL_EXPERIENCECARD_DUPLICATE)
	{
		strError = "MALL_EXPERIENCECARD_DUPLICATE";
	}
	else if (nErrno == MALL_EXPERIENCECARD_MISSED)
	{
		strError = "MALL_EXPERIENCECARD_MISSED";
	}
	else if (nErrno == MALL_EXPERIENCECARD_TIMES_LIMIT)
	{
		strError = "MALL_EXPERIENCECARD_TIMES_LIMIT";
	}
    else if (nErrno == MALL_WEDDINGRING_NOT_SEND)
    {
        strError = "MALL_WEDDINGRING_NOT_SEND";
    }
}


void CEntityMallComponent::SendReturnMsg(unsigned char nErrorType,unsigned char nBuyFromType,unsigned char nType)
{
    if (m_pNetCpt != NULL)
    {
        if (nType == MALL_GOODS_BUY_TYPE_BUY)
        {
            GameMsg_S2C_MallMallBuyResult msgReturn;
            msgReturn.m_nBuyFromType = nBuyFromType;
            msgReturn.nResult = nErrorType;
            GetMallBuyErrorString(nErrorType, msgReturn.strReturn);

            m_pNetCpt->SendPlayerMsg(&msgReturn);
        }
        else if(nType == MALL_GOODS_BUY_TYPE_SEND)
        {
            GameMsg_S2C_MallMallSendResult msgReturn;
            msgReturn.nResult = nErrorType;

            if (nErrorType == MALL_OK)
            {
                msgReturn.strReturn = "Mall_Send_OK";
            }
            else if (CSystemSettingMgr::Instance().IsOpenSendGift() )
            {
                GetMallBuyErrorString(nErrorType, msgReturn.strReturn);
            }
            else
            {
                msgReturn.strReturn = CLocalization::Instance().GetString("System_Tip");
            }

            m_pNetCpt->SendPlayerMsg(&msgReturn);
        }
        else if(nType == MALL_GOODS_BUY_TYPE_RENEW)
        {
            GameMsg_S2C_MallRenewResult msgReturn;
            msgReturn.nResult = nErrorType;

            if (nErrorType == MALL_OK)
            {
                msgReturn.strReturn = "Mall_Renew_OK";
            }
            else
            {
                msgReturn.strReturn = "Mall_Renew_Fail";
            }

            m_pNetCpt->SendPlayerMsg(&msgReturn);
        }
        else if (nType == MALL_GOODS_BUY_TYPE_BEASKED_SEND)
        {
            GameMsg_S2C_BeAskedFor_SendRes msgReturn;
            msgReturn.m_nResult = nErrorType;

            if (nErrorType == MALL_OK)
            {
                msgReturn.m_strReturn = "Mall_BeAsked_Send_OK";
            }
            else if (CSystemSettingMgr::Instance().IsOpenSendGift() )
            {
                GetMallBuyErrorString(nErrorType, msgReturn.m_strReturn);
            }
            else
            {
                msgReturn.m_strReturn = CLocalization::Instance().GetString("System_Tip");
            }

            m_pNetCpt->SendPlayerMsg(&msgReturn);
        }
    }
}

bool CEntityMallComponent::IsTimeOK()
{
	unsigned int nCurTime = (unsigned int)time(NULL);
	if (nCurTime - m_nLastRequestTime > 1){
		m_nLastRequestTime = nCurTime;
		return true;
	}
	return false;
}

bool CEntityMallComponent::CheckSpecialMallUpdateTime()
{
    unsigned int nCurTick = GetTickCount();

    if (nCurTick - m_nLastSpecialMallUpdateTime >= SPECIALMALL_UPDATE_INTERVAL)
    {
        m_nLastSpecialMallUpdateTime = nCurTick;

        return true;
    }

    return false;
}

bool CEntityMallComponent::CheckSpecialMallBuyTime()
{
    unsigned int nCurTick = GetTickCount();

    if (nCurTick - m_nLastSpecialMallBuyTime >= SPECIALMALL_BUY_INTERVAL)
    {
        m_nLastSpecialMallBuyTime = nCurTick;

        return true;
    }

    return false;
}

bool CEntityMallComponent::CheckSpecialMallBidTime()
{
    unsigned int nCurTick = GetTickCount();

    if (nCurTick - m_nLastSpecialMallBidTime >= SPECIALMALL_BID_INTERVEL)
    {
        m_nLastSpecialMallBidTime = nCurTick;

        return true;
    }

    return false;
}

int CEntityMallComponent::CanBuy(VecPeddleGoods & vecGoods,MapPeddleGoods & mapGoods,CurrencyMap & mapBills, bool bCheckPackage, bool bCheckBill, int nDiscount, unsigned char nDestPlayerSexType)
{
	if (m_pRoleItem == NULL || m_pRoleAttr == NULL || m_pRoleAttr->GetRoleID() <= 0)
	{
		return MALL_UNKNOWN;
	}

	if (vecGoods.size() == 0)
	{
		return MALL_ITEMLIST_EMPTY;
	}

	bool bGiveToFriend = true;
	if ( nDestPlayerSexType == ESexType_No )
	{
		nDestPlayerSexType = m_pRoleAttr->GetSex();
		bGiveToFriend = false;
	}

	VecPeddleGoods vecGoodsBuy;
	VecPeddleGoods vecGoodsExperienced;
	MapPeddleGoods mapGoodsBuy;
	MapPeddleGoods mapGoodsExperienced;

	for (size_t i = 0; i < vecGoods.size(); ++i)
	{
		if (vecGoods[i].m_nSelIndex == EPeddleGoodSel_Experience)
			vecGoodsExperienced.push_back(vecGoods[i]);
		else
			vecGoodsBuy.push_back(vecGoods[i]);
	}

	int nReturn = MALL_UNKNOWN;
	if (!vecGoodsBuy.empty())
	{
		nReturn = CMall::Instance().CanBuy(vecGoodsBuy,mapGoodsBuy,mapBills,nDestPlayerSexType, m_pRoleCouple->Intimacy(), m_pRoleAttr->IsMarried() );
		if (nReturn != MALL_OK)
		{
			return nReturn;
		}
	}
	if (!vecGoodsExperienced.empty())
	{
		nReturn = CanBuyByExpCards(vecGoodsExperienced, mapGoodsExperienced, nDestPlayerSexType);
		if (nReturn != MALL_OK)
			return nReturn;
	}

	// 合在一起！
	mapGoods.insert(mapGoodsBuy.begin(), mapGoodsBuy.end());
	mapGoods.insert(mapGoodsExperienced.begin(), mapGoodsExperienced.end());

	if (bCheckPackage)
	{
		nReturn = CanAddItem(mapGoods);
		if (nReturn != MALL_OK)
		{
			return nReturn;
		}
	}
	if (bCheckBill && !vecGoodsBuy.empty()) // 存在需要购买的物品，才需要验证价格
	{
		if ( nDiscount > 0 && nDiscount < 100 ) // 时光蛋可以享受的折上折扣
		{
			float fDiscount = nDiscount / 100.0f;
			for (CurrencyMap::iterator it = mapBills.begin(); 
				it != mapBills.end(); ++it)
			{
				it->second = (unsigned int)(it->second*fDiscount);
			}
		}
		nReturn = CanBill(mapBills, bGiveToFriend);
	}

	return nReturn;
}




int CEntityMallComponent::Buy(MapPeddleGoods & mapGoods,CurrencyMap & mapCurrency)
{
	VecPeddleGoods experienceGoods;
	for (MapPeddleGoods::iterator iter = mapGoods.begin(); iter != mapGoods.end(); ++iter)
	{
		if (iter->second.m_nSelIndex == EPeddleGoodSel_Experience)
			experienceGoods.push_back(iter->second);
	}

	int sumCost = 0;
	for (CurrencyMap::iterator iter = mapCurrency.begin(); iter != mapCurrency.end(); ++iter)
	{
		sumCost += iter->second;
	}

	if (mapGoods.size() <= 0)
	{
		return MALL_UNKNOWN;
	}

	int nReturn = AddItem(mapGoods);
	if (nReturn == MALL_OK){

		if (0 != sumCost)
			nReturn = FinishBill(mapCurrency, EFinishBillFrom_Buy);
		if (!experienceGoods.empty())
		{
			// 扣除体验卡次数
			ReduceExperienceTimes(experienceGoods);
		}

		if (nReturn == MALL_OK){
			nReturn = CMall::Instance().InsertMallLog(mapGoods,mapCurrency,m_pRoleAttr->GetRoleID());
		}
	}

	return nReturn;
}

int CEntityMallComponent::CanRenew(VecPeddleGoods & vecGoods,MapPeddleGoods & mapGoods,CurrencyMap & mapBills,bool bFirstUseChange,unsigned short nColumn,unsigned short nIndex)
{
// 	if(m_pRoleItem == NULL || m_pRoleAttr == NULL || m_pRoleAttr->GetRoleID() <= 0)
// 	{
// 		return MALL_UNKNOWN;
// 	}
// 
// 	if (vecGoods.size() == 0){
// 		return MALL_ITEMLIST_EMPTY;
// 	}
// 	if (vecGoods.size() != 1){
// 		return MALL_PARAM_ERROR;
// 	}
// 
// 	int nReturn = CMall::Instance().CanBuy(vecGoods,mapGoods,mapBills,m_pRoleAttr->GetSex() , m_pRoleCouple->Intimacy(), m_pRoleAttr->IsMarried());
// 	if (nReturn == MALL_OK){
// 		if(m_pRoleItem->CanRenewItem((EItemColumn)nColumn,nIndex) != EItemError_Success)
// 		{
// 			return MALL_ITEM_NOTRENEW;
// 		}
// 		nReturn = CanBill(mapBills);
// 	}
// 
// 	return nReturn;

    return 0;
}

int CEntityMallComponent::Renew(MapPeddleGoods & mapGoods,CurrencyMap & mapBills,unsigned short nColumn,unsigned short nIndex)
{
// 	if (mapGoods.size() != 1 || mapBills.size() != MAX_CURRENCY_TYPE || m_pRoleItem == NULL )
// 	{
// 		return MALL_UNKNOWN;
// 	}
// 
// 	itemtype_t nItemType = 0;
// 	unsigned char nSelIndex = 0;
// 	MapPeddleGoods::iterator it = mapGoods.begin();
// 	if (it != mapGoods.end())
// 	{
// 		nItemType = it->first;
// 		PeddleGood & aPeddleGood = it->second;
// 		nSelIndex = aPeddleGood.m_nSelIndex;
// 	}
// 
// 	ItemConfig * pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
// 	if(pItemInfo == NULL){
// 		return MALL_PARAM_ERROR;
// 	}
// 	
// 	int nValidTime = 0;
// 
// 	if (pItemInfo->IsEquip()){
// 		nValidTime = GetValidTimeByGoodSelIndex(nItemType,nSelIndex);
// 		if (nValidTime == 0){
// 			return MALL_PARAM_ERROR;
// 		}
// 	}
// 
// 	__int64 nItemID = 0;
// 	if(m_pRoleItem->RenewItem(EItemAction_Add_Renew, 0, (EItemColumn)nColumn,nIndex,nValidTime,nItemID,true,false) != EItemError_Success)
//     {
// 		return MALL_ITEM_NOTRENEW;
// 	}
// 
// 	int nReturn = FinishBill(mapBills, EFinishBillFrom_Renew);
// 	if (nReturn == MALL_OK){
// 		nReturn = CMall::Instance().InsertMallLog(mapGoods,mapBills,m_pRoleAttr->GetRoleID(),0,nItemID);
// 	}
// 
// 	return nReturn;

    return 0;
}

int CEntityMallComponent::CanSendFromDivorce(VecPeddleGoods & vecGoods,MapPeddleGoods & mapGoods,CurrencyMap & mapBills,unsigned int nDestRoleID,unsigned char cDestSex)
{
	if(m_pRoleMail == NULL || m_pRoleAttr == NULL  || nDestRoleID <= 0 || m_pRoleAttr->GetRoleID() <= 0)
	{
		return MALL_UNKNOWN;
	}

	// 赠送时不可以使用金券
	for (VecPeddleGoods::iterator it = vecGoods.begin(); it != vecGoods.end(); ++it)
	{
		it->m_bUseCoupon = false;
	}

	int nReturn = CMall::Instance().CanBuy(vecGoods,mapGoods,mapBills,cDestSex, 0, 0);
	if (nReturn == MALL_OK)
	{
		nReturn = CanMailItem(mapGoods,nDestRoleID);
		if (nReturn == MALL_OK)
		{
			nReturn = CanBill(mapBills, true/*赠送给好友*/);
		}
	}
	return nReturn;
}

int CEntityMallComponent::CanSend(VecPeddleGoods & vecGoods,MapPeddleGoods & mapGoods,CurrencyMap & mapBills,unsigned int nDestRoleID)
{
    if(m_pRoleMail == NULL || m_pRoleAttr == NULL || m_pRoleFriend == NULL || nDestRoleID <= 0 || m_pRoleAttr->GetRoleID() <= 0)
    {
        return MALL_UNKNOWN;
    }

    if (CSystemSettingMgr::Instance().IsOpenSendGift() )
    {
        const FriendInfo *pFriendInfo = m_pRoleFriend->GetFriend(nDestRoleID);

        if(pFriendInfo == NULL)
        {
            return MALL_ITEM_FRIENDERROR;
        }

        // 赠送时不可以使用金券
        for (VecPeddleGoods::iterator it = vecGoods.begin(); it != vecGoods.end(); ++it)
        {
            it->m_bUseCoupon = false;
        }

        int nReturn = CMall::Instance().CanBuy(vecGoods, mapGoods, mapBills, pFriendInfo->m_nSex, 0, 0);

        if (nReturn == MALL_OK)
        {
            nReturn = CanMailItem(mapGoods, nDestRoleID);

            if (nReturn == MALL_OK)
            {
                nReturn = CanBill(mapBills, true/*赠送给好友*/);
            }
        }

        return nReturn;
    }

    return MALL_UNKNOWN;
}

int CEntityMallComponent::Send(MapPeddleGoods & mapGoods,CurrencyMap & mapBills,unsigned int nDestRoleID,string strInfo, CRoleSendLog::ESendType eSendType)
{
	if (mapGoods.size() <= 0 || mapBills.size() != MAX_CURRENCY_TYPE || nDestRoleID <= 0 || m_pRoleAttr == NULL || m_pRoleAttr->GetRoleID() <= 0){
		return MALL_UNKNOWN;
	}
	int nReturn = OnMailItem(mapGoods,nDestRoleID,strInfo, eSendType);
	if (nReturn == MALL_OK){
		nReturn = FinishBill(mapBills, EFinishBillFrom_Send);
		if (nReturn == MALL_OK){
			nReturn = CMall::Instance().InsertMallLog(mapGoods,mapBills,m_pRoleAttr->GetRoleID(),nDestRoleID);
		}
	}
	return nReturn;
}

int CEntityMallComponent::CanBuyByExpCards(VecPeddleGoods & vecGoods,MapPeddleGoods & mapGoods, unsigned char nDestPlayerSexType)
{
	if (m_pRoleItem == NULL || m_pRoleAttr == NULL || m_pRoleAttr->GetRoleID() <= 0)
		return MALL_UNKNOWN;

	if (vecGoods.empty())
		return MALL_ITEMLIST_EMPTY;
	if ( nDestPlayerSexType == ESexType_No )
		nDestPlayerSexType = m_pRoleAttr->GetSex();

	if (!CMall::Instance().IsOpening())
		return MALL_NOTOPEN;

	if (NULL == m_pRoleItem->SearchItem( CMallDataMgr::Instance().m_experienceCardItemType))
		return MALL_EXPERIENCECARD_MISSED;

	time_t currTime = time(NULL);
	for (size_t i = 0; i < vecGoods.size(); ++i)
	{
		PeddleGood &goods = vecGoods[i];
		MallGood * pMallGood = CMall::Instance().GetGood(goods.m_nType);
		if (NULL == pMallGood)
			return MALL_NOTSELL;
		ItemConfig * pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(goods.m_nType);	
		if (pItemInfo == NULL)
			return MALL_PARAM_ERROR;

		if (!CMall::Instance().isBetween(pMallGood->m_nSell_time0, pMallGood->m_nSell_time1, currTime))
			return MALL_NOTSELL;

		if (1 == pMallGood->m_nIsPrecious)
			return MALL_CAN_NOT_BUY_PRECIOUS_ITEMS;
		if (pMallGood->m_nPayType != goods.m_nPayType)
			return MALL_PAYTYPE_INCONFORMITY;

		if (!IsValidExperiencedItemType(goods.m_nType))
			return MALL_EXPERIENCECARD_TYPE_REFUSED; // 不可以使用体验卡的类型

		if (HasExperiencedItem(goods.m_nType))
			return MALL_EXPERIENCECARD_DUPLICATE;

		if (mapGoods.find(vecGoods[i].m_nType) != mapGoods.end())
			return MALL_ITEM_SAMEERROR;

		if (pItemInfo->m_SexNeed != ESexType_No && pItemInfo->m_SexNeed != nDestPlayerSexType)
			return MALL_ITEM_SEXERROR;

		if ( pItemInfo->m_nIntimacy > 0 && !m_pRoleAttr->IsMarried() )
			return MALL_NOT_MARRID;

		if ( pItemInfo->m_nIntimacy > (int)m_pRoleCouple->Intimacy())
			return MALL_INTIMACY_NOTENOUGH;

		if (goods.m_nCost != 0)
			return MALL_BILL_INCONFORMITY;


		mapGoods.insert(MapPeddleGoods::value_type(goods.m_nType,goods));
	}

    if (!HasExperiencedTimes((unsigned short)vecGoods.size()))
		return MALL_EXPERIENCECARD_TIMES_LIMIT;

	return MALL_OK;
}


void CEntityMallComponent::OnSpecialMallUpdateMsg(GameMsg_Base &msg, CSlotPeer &SlotPeer)
{
    if (CheckSpecialMallUpdateTime())
        SendMsg2GroupServer(&msg);
}

void CEntityMallComponent::OnSpecialMallMsgRes(GameMsg_Base &msg, CSlotPeer &SlotPeer)
{
    if (NULL != m_pNetCpt)
        m_pNetCpt->SendPlayerMsg(&msg);
}

void CEntityMallComponent::OnSpecialMallPay(GameMsg_Base &msg, CSlotPeer &SlotPeer)
{
    GameMsg_G2S_SpecialMall_Pay *pPayMsg = dynamic_cast<GameMsg_G2S_SpecialMall_Pay *>(&msg);

    if (NULL != m_pNetCpt && NULL != pPayMsg && NULL != m_pRoleAttr && NULL != m_pRoleItem)
    {
        GameMsg_S2G_SpecialMall_PayResult resMsg;
        resMsg.m_nSpecialMallType = pPayMsg->m_nSpecialMallType;
        resMsg.m_uRange = pPayMsg->m_uRange;
        resMsg.m_uIdx = pPayMsg->m_uIdx;

        int offsetType = pPayMsg->m_nSpecialMallType - ESpecialMallType_GroupBuying;

        CurrencyMap mapBills;
        mapBills.insert(CurrencyMap::value_type(EMallCurrencyType_MCoin, 0));
        mapBills.insert(CurrencyMap::value_type(EMallCurrencyType_BindMCoin, 0));
        mapBills.insert(CurrencyMap::value_type(EMallCurrencyType_Money, 0));

        switch (pPayMsg->m_nBuyType)
        {
        case eMallPayType_Money:
            mapBills[EMallCurrencyType_Money] = pPayMsg->m_uBuyPrice;
            break;

        case eMallPayType_BindMCoin:
            mapBills[EMallCurrencyType_BindMCoin] = pPayMsg->m_uBuyPrice;
            break;

        case eMallPayType_MCoin:
            mapBills[EMallCurrencyType_MCoin] = pPayMsg->m_uBuyPrice;
            break;
        }

        if (MALL_OK != CanBill(mapBills))
            resMsg.m_bSuccess = false;
        else
        {
            m_pRoleAttr->ChangeBill((unsigned int)mapBills[EMallCurrencyType_MCoin] * (-1), (EChangeBillCause)(EChangeBillCause_Del_GroupBuying + offsetType) );
            m_pRoleAttr->ChangeBindBill((unsigned int)mapBills[EMallCurrencyType_BindMCoin] * (-1), (EChangeBindBillCause)(EChangeBindBillCause_Del_GroupBuying + offsetType) );
            m_pRoleAttr->ChangeMoney((unsigned int)mapBills[EMallCurrencyType_Money] * (-1), (EChangeMoneyCause)(EChangeMoneyCause_Del_GroupBuying + offsetType), 0);

            // add or send
            std::list<CItem> listItem;
            listItem.push_back( pPayMsg->m_cItem );
            CItemProcess::AddOrMailItems( *(CRoleEntity *)Entity(), listItem, (EItemAction)(EItemAction_Add_GroupBuying_Buy + offsetType),
                0, true, EMailType_SpecialMall, pPayMsg->m_strMailTitle, pPayMsg->m_strMailContent );

            resMsg.m_bSuccess = true;

            QuerySaveSpecialMall(pPayMsg->m_uIdx, pPayMsg->m_cItem, pPayMsg->m_nBuyType, pPayMsg->m_uBuyPrice, mapBills, pPayMsg->m_tEndTime, (ESpecialMallType)pPayMsg->m_nSpecialMallType);

            if (ESpecialMallType_LimitedEdition == pPayMsg->m_nSpecialMallType)
                m_mapLimitedEditionBuy[pPayMsg->m_uIdx] = pPayMsg->m_tEndTime;
        }

        m_pNetCpt->Send2GroupServer(&resMsg);
    }
}

void CEntityMallComponent::OnGetSpecialMallState(GameMsg_Base &msg, CSlotPeer &SlotPeer)
{
    if (CheckSpecialMallUpdateTime())
        SendMsg2GroupServer(&msg);

    if (m_mapTopPrice.size() > 0)
    {
        GameMsg_S2G_Auction_CheckTopPrice checkTopMsg;

        for (std::map<long long, CAuctionNotGivePointInfo>::const_iterator itr = m_mapTopPrice.begin(); 
             itr != m_mapTopPrice.end(); ++itr)
        {
            CAuctionTopPriceData topData;
            topData.m_lLogID = itr->first;
            topData.m_uIdx = itr->second.m_uIdx;
            topData.m_tEndTime = itr->second.m_tEndTime;

            checkTopMsg.m_mapTopInfo.insert(std::make_pair(itr->first, topData));
        }

        SendMsg2GroupServer(&checkTopMsg);
    }
}

void CEntityMallComponent::OnGroupBuyingGetItemsRes(GameMsg_Base &msg, CSlotPeer &SlotPeer)
{
    GameMsg_S2C_GroupBuying_ItemsResult *pResMsg = dynamic_cast<GameMsg_S2C_GroupBuying_ItemsResult *>(&msg);
    time_t tTime = time(NULL);

    if (NULL != m_pNetCpt && NULL != pResMsg)
    {
        for (std::list<CGroupBuyingItemData>::iterator bIt = pResMsg->m_listItems.begin(), eIt = pResMsg->m_listItems.end(); 
             bIt != eIt; ++bIt)
            bIt->m_bAlreadyBuy = CheckSpecialMallAlreadyBuy(ESpecialMallType_GroupBuying, bIt->m_uIdx, bIt->m_cItem.m_nItemType);

        tTime = time(NULL) - tTime;
        pResMsg->m_fCD -= tTime;
        if (pResMsg->m_fCD < 0)
            pResMsg->m_fCD = 0;

        m_pNetCpt->SendPlayerMsg(pResMsg);
    }
}

void CEntityMallComponent::OnGroupBuyingBuyMsg(GameMsg_Base &msg, CSlotPeer &SlotPeer)
{
    GameMsg_C2S_GroupBuying_Buy *pMsg = dynamic_cast<GameMsg_C2S_GroupBuying_Buy *>(&msg);

    if (NULL != pMsg && CheckSpecialMallBuyTime() && 
        CheckSpecialMallForBuy(msg, ESpecialMallType_GroupBuying, pMsg->m_uIdx, pMsg->m_uGoodsID))
        SendMsg2GroupServer(&msg);
}

void CEntityMallComponent::OnLimitedEditionGetItemsRes(GameMsg_Base &msg, CSlotPeer &SlotPeer)
{
    GameMsg_S2C_LimitedEdition_ItemsResult *pResMsg = dynamic_cast<GameMsg_S2C_LimitedEdition_ItemsResult *>(&msg);
    time_t tTime = time(NULL);

    if (NULL != m_pNetCpt && NULL != pResMsg)
    {
        for (std::list<CLimitedEditionItemData>::iterator bIt = pResMsg->m_listItems.begin(), eIt = pResMsg->m_listItems.end(); 
            bIt != eIt; ++bIt)
            bIt->m_bAlreadyBuy = CheckSpecialMallAlreadyBuy(ESpecialMallType_LimitedEdition, bIt->m_uIdx, bIt->m_cItem.m_nItemType);

        tTime = time(NULL) - tTime;
        pResMsg->m_fCD -= tTime;
        if (pResMsg->m_fCD < 0)
            pResMsg->m_fCD = 0;

        m_pNetCpt->SendPlayerMsg(pResMsg);
    }
}

void CEntityMallComponent::OnLimitedEditionBuyMsg(GameMsg_Base &msg, CSlotPeer &SlotPeer)
{
    GameMsg_C2S_LimitedEdition_Buy *pMsg = dynamic_cast<GameMsg_C2S_LimitedEdition_Buy *>(&msg);

    if (NULL != pMsg && CheckSpecialMallBuyTime() && 
        CheckSpecialMallForBuy(msg, ESpecialMallType_LimitedEdition, pMsg->m_uIdx, pMsg->m_uGoodsID))
        SendMsg2GroupServer(&msg);
}

void CEntityMallComponent::OnAuctionGetItemsRes(GameMsg_Base &msg, CSlotPeer &SlotPeer)
{
    GameMsg_G2S_Auction_ItemsResult *pResMsg = dynamic_cast<GameMsg_G2S_Auction_ItemsResult *>(&msg);
    time_t tTime = time(NULL);

    if (NULL != m_pNetCpt && NULL != pResMsg)
    {
        if (ESpecialMallState_OnSale == pResMsg->m_nState)
            m_bAuctionIsOpen = true;
        else
            m_mapAuctionData.clear();
        m_tAuctionEndTime = pResMsg->m_tEndTime;

        GameMsg_S2C_Auction_ItemsResult getItemsMsg;
        getItemsMsg.m_nState = pResMsg->m_nState;
        getItemsMsg.m_fCD = pResMsg->m_fCD;
        getItemsMsg.m_bSearch = pResMsg->m_bSearch;
        getItemsMsg.m_strSearchKey = pResMsg->m_strSearchKey;
        getItemsMsg.m_uCurPage = pResMsg->m_uCurPage;
        getItemsMsg.m_uTotalCount = pResMsg->m_uTotalCount;
        getItemsMsg.m_listItems = pResMsg->m_listItems;

        for (std::list<CAuctionItemData>::iterator bIt = getItemsMsg.m_listItems.begin(), eIt = getItemsMsg.m_listItems.end(); 
             bIt != eIt; ++bIt)
        {
             bIt->m_bAlreadyBuy = CheckSpecialMallAlreadyBuy(ESpecialMallType_Auction, bIt->m_uIdx, bIt->m_cItem.m_nItemType);

             AuctionState state;
             state.m_uCurPrice = bIt->m_uCurPrice;
             state.m_bTimeOut = (0 == bIt->m_fCountDown);
             state.m_cItem = bIt->m_cItem;

             m_mapAuctionData[bIt->m_uIdx] = state;
        }

        tTime = time(NULL) - tTime;
        getItemsMsg.m_fCD -= tTime;
        if (getItemsMsg.m_fCD < 0)
            getItemsMsg.m_fCD = 0;

        m_pNetCpt->SendPlayerMsg(&getItemsMsg);
    }
}

void CEntityMallComponent::OnAuctionBidMsg(GameMsg_Base &msg, CSlotPeer &SlotPeer)
{
    GameMsg_C2S_Auction_Bid *pMsg = dynamic_cast<GameMsg_C2S_Auction_Bid *>(&msg);

    if (NULL != pMsg && CheckSpecialMallBidTime() && NULL != m_pNetCpt && NULL != m_pRoleAttr)
    {
        if (CheckForAuctionBid(msg, pMsg->m_uIdx, pMsg->m_uGoodsID, pMsg->m_uPrice) )
        {
            std::map<unsigned short, AuctionState>::iterator itr = m_mapAuctionData.find(pMsg->m_uIdx);

            if (m_mapAuctionData.end() == itr)
                return;

            QuerySaveAuctionBid(pMsg->m_uIdx, itr->second.m_cItem, pMsg->m_uPrice, m_tAuctionEndTime);

            m_pRoleAttr->ChangeBill((unsigned int)pMsg->m_uPrice * (-1), EChangeBillCause_Del_Auction, true, false, false );

            m_pNetCpt->Send2GroupServer(&msg);
        }
    }
}

void CEntityMallComponent::OnAuctionBidRes(GameMsg_Base &msg, CSlotPeer &SlotPeer)
{
    GameMsg_G2S_Auction_BidResult *pMsg = dynamic_cast<GameMsg_G2S_Auction_BidResult *>(&msg);

    if (NULL != pMsg && NULL != m_pNetCpt)
    {
        if (ESpecialMallError_None != pMsg->m_nErrorType)
        {
            AuctionReturnPoint(-1, pMsg->m_uIdx, m_tAuctionEndTime, pMsg->m_uPrice);
        }

        GameMsg_S2C_Auction_BidResult resMsg;
        resMsg.m_nErrorType = pMsg->m_nErrorType;
        resMsg.m_uIdx = pMsg->m_uIdx;
        resMsg.m_cItem = pMsg->m_cItem;

        m_pNetCpt->SendPlayerMsg(&resMsg);
    }
}

void CEntityMallComponent::OnAuctionSurpassed(GameMsg_Base &msg, CSlotPeer &SlotPeer)
{
    GameMsg_G2S_Auction_Surpassed *pMsg = dynamic_cast<GameMsg_G2S_Auction_Surpassed *>(&msg);

    if (NULL != pMsg && NULL != m_pNetCpt)
    {
        AuctionReturnPoint(-1, pMsg->m_uIdx, m_tAuctionEndTime, pMsg->m_uPrice);

        GameMsg_S2C_Auction_Surpassed resMsg;
        resMsg.m_uIdx = pMsg->m_uIdx;
        resMsg.m_cItem = pMsg->m_cItem;

        m_pNetCpt->SendPlayerMsg(&resMsg);
    }
}

void CEntityMallComponent::OnAuctionUpdateState(GameMsg_Base &msg, CSlotPeer &SlotPeer)
{
    GameMsg_G2S_Auction_UpdateState *pMsg = dynamic_cast<GameMsg_G2S_Auction_UpdateState *>(&msg);

    if (NULL != pMsg)
    {
        std::map<unsigned short, AuctionState>::iterator it = m_mapAuctionData.find(pMsg->m_uIdx);

        if (it != m_mapAuctionData.end())
        {
            it->second.m_uCurPrice = pMsg->m_uCurPrice;
            it->second.m_bTimeOut = pMsg->m_bTimeOut;
        }
        else
        {
            AuctionState state;
            state.m_uCurPrice = pMsg->m_uCurPrice;
            state.m_bTimeOut = pMsg->m_bTimeOut;

            m_mapAuctionData.insert(std::make_pair(pMsg->m_uIdx, state) );
        }
    }
}

void CEntityMallComponent::OnAuctionBidSuccess(GameMsg_Base &msg, CSlotPeer &SlotPeer)
{
    GameMsg_G2S_Auction_BidSuccess *pMsg = dynamic_cast<GameMsg_G2S_Auction_BidSuccess *>(&msg);

    if (NULL != pMsg && NULL != m_pRoleAttr)
    {
        GameMsg_S2C_Auction_BidSuccess resMsg;
        resMsg.m_cItem = pMsg->m_cItem;

        if (NULL != m_pNetCpt)
            m_pNetCpt->SendPlayerMsg(&resMsg);

		// 累计消费统计
		m_pRoleActivity->AddCumulativeSpendBill( pMsg->m_uPrice );

		// vip 成长值
		m_pRoleVIP->VIPBillExpBounus((int)pMsg->m_uPrice);
    }
}

void CEntityMallComponent::OnAuctionCheckTopPriceResult(GameMsg_Base &msg, CSlotPeer &SlotPeer)
{
    GameMsg_G2S_Auction_CheckTopPriceResult *pResMsg = dynamic_cast<GameMsg_G2S_Auction_CheckTopPriceResult *>(&msg);

    if (NULL != pResMsg)
    {
        std::map<long long, CAuctionNotGivePointInfo>::const_iterator topPriceItr;

        for (std::map<long long, bool>::const_iterator bTopItr = pResMsg->m_mapTopInfo.begin(); 
             bTopItr != pResMsg->m_mapTopInfo.end(); ++bTopItr)
        {
            if (bTopItr->second)
                continue;

            topPriceItr = m_mapTopPrice.find(bTopItr->first);

            if (topPriceItr != m_mapTopPrice.end())
            {
                AuctionReturnPoint(topPriceItr->second.m_lLogID, topPriceItr->second.m_uIdx, 
                    topPriceItr->second.m_tEndTime, topPriceItr->second.m_uBidPrice);
            }
        }

        m_mapTopPrice.clear();
    }
}

void CEntityMallComponent::OnExperienceCardInfo(GameMsg_Base &msg, CSlotPeer &SlotPeer)
{
	if (NULL == m_pNetCpt)
	{
		WriteLog(LOGLEVEL_ERROR, "error missed netcpt...");
		return ;
	}

	time_t now = time(NULL);
	ResetExperienceCardInfo(now);
	GameMsg_S2C_ExperienceCardInfoRes resMsg;
	for (std::set<itemtype_t>::iterator iter = m_setExperienceCardBuyedProduct.begin();
		iter != m_setExperienceCardBuyedProduct.end(); ++iter)
	{
		resMsg.m_vecExperiencedItem.push_back(*iter);
	}
	resMsg.m_usedTime = m_experienceCardUsedNum;
	resMsg.m_nCountDown = (GetNextDayStartTime(now) - m_experienceCardUsedStamp);

	m_pNetCpt->SendPlayerMsg(&resMsg);
}

bool CEntityMallComponent::CheckSpecialMallAlreadyBuy(unsigned char nSpecialMallType, unsigned short idx, itemtype_t uGoodsID)
{
    const ItemConfigManager &rItemsInfoMgr = ConfigManager::Instance().GetItemConfigManager();
    const ItemConfig *pItemInfo = rItemsInfoMgr.GetByID(uGoodsID);

    const ItemBase *pItem = NULL;
    time_t tTime = time(NULL);

    if (NULL != pItemInfo && pItemInfo->m_bUnique && NULL != m_pRoleItem)
    {
        pItem = m_pRoleItem->SearchItem(uGoodsID, true);

        if (NULL != pItem && pItem->GetDueTime() <= 0)
            return true;
        else if (!CanBuyBadageCard(uGoodsID))
            return true;
    }

    if (ESpecialMallType_LimitedEdition == nSpecialMallType)
    {
        std::map<unsigned short, unsigned int>::iterator itr = m_mapLimitedEditionBuy.find(idx);
        if (m_mapLimitedEditionBuy.end() != itr)
        {
            if (itr->second >= (unsigned int)tTime)
                return true;
            else
                m_mapLimitedEditionBuy.erase(itr);
        }
    }

    return false;
}

bool CEntityMallComponent::CheckSpecialMallForBuy(GameMsg_Base &msg, unsigned char nSpecialMallType, unsigned short idx, itemtype_t uGoodsID)
{
    const ItemConfigManager &rItemsInfoMgr = ConfigManager::Instance().GetItemConfigManager();
    const ItemConfig *pItemInfo = rItemsInfoMgr.GetByID(uGoodsID);
    CItem item;
    item.m_nItemType = uGoodsID;

    if (NULL == pItemInfo)
    {
        SendSpecialMallErrorBuyRes(nSpecialMallType, ESpecialMallError_NotFound, item);

        return false;
    }
    else if (pItemInfo->IsEquip() && NULL != m_pRoleItem && NULL != m_pRoleAttr)
    {
        const ItemBase *pItem = m_pRoleItem->SearchItem(uGoodsID, true);

        if (NULL != pItem)
        {
            if (pItem->GetDueTime() <= 0)
            {
                SendSpecialMallErrorBuyRes(nSpecialMallType, ESpecialMallError_Exist, item);

                return false;
            }
            else if (ESexType_No != pItemInfo->m_SexNeed && pItemInfo->m_SexNeed != m_pRoleAttr->m_nSex)
            {
                SendSpecialMallErrorBuyRes(nSpecialMallType, ESpecialMallError_SexNotMatch, item);

                return false;
            }
        }
    }
    else if (!CanBuyBadageCard(uGoodsID))
    {
        SendSpecialMallErrorBuyRes(nSpecialMallType, ESpecialMallError_CanNotBuyBadgeCard, item);

        return false;
    }

    if (ESpecialMallType_LimitedEdition == nSpecialMallType)
    {
        std::map<unsigned short, unsigned int>::iterator itr = m_mapLimitedEditionBuy.find(idx);

        if (m_mapLimitedEditionBuy.end() != itr && time(NULL) < (time_t)itr->second)
        {
            SendSpecialMallErrorBuyRes(nSpecialMallType, ESpecialMallError_AlreadyBuy, item);

            return false;
        }
    }

    return true;
}

bool CEntityMallComponent::CheckForAuctionBid(GameMsg_Base &msg, unsigned short idx, itemtype_t uGoodsID, unsigned int price)
{
    if (NULL != m_pRoleAttr && CheckSpecialMallForBuy(msg, ESpecialMallType_Auction, idx, uGoodsID))
    {
        std::map<unsigned short, AuctionState>::iterator itr = m_mapAuctionData.find(idx);
        unsigned int nPoint = m_pRoleAttr->GetPoint();
        CItem item;
        item.m_nItemType = uGoodsID;

        if (!m_bAuctionIsOpen)
        {
            SendSpecialMallErrorBuyRes(ESpecialMallType_Auction, ESpecialMallError_NotOpen, item);

            return false;
        }
        else if (m_mapAuctionData.end() == itr)
        {
            SendSpecialMallErrorBuyRes(ESpecialMallType_Auction, ESpecialMallError_NotFound, item);

            return false;
        }
        else if (uGoodsID != itr->second.m_cItem.m_nItemType)
        {
            SendSpecialMallErrorBuyRes(ESpecialMallType_Auction, ESpecialMallError_NotFound, item);

            return false;
        }
        else if (itr->second.m_bTimeOut)
        {
            SendSpecialMallErrorBuyRes(ESpecialMallType_Auction, ESpecialMallError_BidTimeOut, itr->second.m_cItem);

            return false;
        }
        else if (price <= itr->second.m_uCurPrice)
        {
            SendSpecialMallErrorBuyRes(ESpecialMallType_Auction, ESpecialMallError_BidNotEnough, itr->second.m_cItem);

            return false;
        }
        else if (nPoint < price)
        {
            LOG_ERROR << "MALLLOG role[" << m_pRoleAttr->GetRoleID() 
                << "] need mcoin[" << price 
                << "] > has mcoin[" << nPoint 
                << "]" << std::endl;

            SendSpecialMallErrorBuyRes(ESpecialMallType_Auction, ESpecialMallError_MoneyNotEnough, itr->second.m_cItem);

            return false;
        }

        return true;
    }

    return false;
}

void CEntityMallComponent::SendSpecialMallErrorBuyRes(unsigned char nSpecialMallType, unsigned char errorType, const CItem &item)
{
    if (NULL == m_pNetCpt)
        return;

    switch (nSpecialMallType)
    {
    case ESpecialMallType_GroupBuying:
        {
            GameMsg_S2C_GroupBuying_BuyResult resMsg;
            resMsg.m_nErrorType = errorType;
            resMsg.m_cItem = item;

            m_pNetCpt->SendPlayerMsg(&resMsg);
        }
        break;

    case ESpecialMallType_LimitedEdition:
        {
            GameMsg_S2C_LimitedEdition_BuyResult resMsg;
            resMsg.m_nErrorType = errorType;
            resMsg.m_cItem = item;

            m_pNetCpt->SendPlayerMsg(&resMsg);
        }
        break;

    case ESpecialMallType_Auction:
        {
            GameMsg_S2C_Auction_BidResult resMsg;
            resMsg.m_nErrorType = errorType;
            resMsg.m_cItem = item;

            m_pNetCpt->SendPlayerMsg(&resMsg);
        }
        break;
    }
}

void CEntityMallComponent::AuctionReturnPoint(long long nLogID, unsigned short idx, unsigned int nEndTime, unsigned int price)
{
    if (NULL != m_pNetCpt && NULL != m_pRoleAttr && 0 != price)
    {
        CAuctionReturnPointDB *pInfo = new CAuctionReturnPointDB;

        if (NULL != pInfo)
        {
            pInfo->m_lLogID = nLogID;
            pInfo->m_uRoleID = m_pRoleAttr->GetRoleID();
            pInfo->m_uAccountID = m_pRoleAttr->GetAccount();
            pInfo->m_uIdx = idx;
            pInfo->m_nEndTime = nEndTime;
            pInfo->m_nPoint = price;
            pInfo->m_nNewPoint = price + m_pRoleAttr->GetPoint();
            pInfo->m_eChangeBillCause = EChangeBillCause_Add_Auction;

            CDelHelper delHelper(pInfo);

            AddQuery(QUERY_AuctionReturnPoint, 0, pInfo, 0, NULL, &delHelper);
        }
    }
}

void CEntityMallComponent::QuerySaveSpecialMall(unsigned short idx, const CItem &item, unsigned char buyType, unsigned int buyPrice, CurrencyMap &mapBills, time_t tEndTime, ESpecialMallType eType)
{
    CSpecialMallInfoDB *pInfo = new CSpecialMallInfoDB;

    if (NULL != pInfo && NULL != m_pRoleAttr)
    {
        pInfo->m_uRoleID = m_pRoleAttr->GetRoleID();
        pInfo->m_uIdx = idx;
        pInfo->m_uItemType = item.m_nItemType;
        pInfo->m_uItemCount = item.m_nItemCount;
        pInfo->m_nValidTime = item.m_nValidTime;
        pInfo->m_nBuyType = buyType;
        pInfo->m_uBuyPrice = buyPrice;
        pInfo->m_uUsedMoney = mapBills[EMallCurrencyType_Money];
        pInfo->m_uUsedBindPt = mapBills[EMallCurrencyType_BindMCoin];
        pInfo->m_uUsedPt = mapBills[EMallCurrencyType_MCoin];
        pInfo->m_tEndTime = tEndTime;
        pInfo->m_eType = eType;

        AddQuery(QUERY_SaveSpecialMall, 0, pInfo);
    }
}

void CEntityMallComponent::QuerySaveAuctionBid(unsigned short idx, const CItem &item, unsigned int bidPrice, time_t tEndTime)
{
    CAuctionBidInfoDB *pInfo = new CAuctionBidInfoDB;

    if (NULL != pInfo && NULL != m_pRoleAttr)
    {
        pInfo->m_uRoleID = m_pRoleAttr->GetRoleID();
        pInfo->m_uIdx = idx;
        pInfo->m_uItemType = item.m_nItemType;
        pInfo->m_uItemCount = item.m_nItemCount;
        pInfo->m_nValidTime = item.m_nValidTime;
        pInfo->m_uBidPrice = bidPrice;
        pInfo->m_tEndTime = tEndTime;

        const char * name = m_pRoleAttr->GetRoleName();
        if (NULL != name)
            pInfo->m_strRoleName = name;

        AddQuery(QUERY_SaveAuctionBid, 0, pInfo);
    }
}

void CEntityMallComponent::OnDBAuctionReturnPoint(QueryBase &rQuery)
{
    CAuctionReturnPointDB *pDBInfo = static_cast<CAuctionReturnPointDB *>(rQuery.m_pAttachment);

    if (NULL != pDBInfo && rQuery.m_nResult == Query_Res_OK && pDBInfo->m_bSuccess)
        m_pRoleAttr->ChangeBill(pDBInfo->m_nPoint, EChangeBillCause_Add_Auction, false);

    delete pDBInfo, pDBInfo = NULL;
}

int CEntityMallComponent::CanBill(CurrencyMap & mapCurrency, bool bGiveToFriend)
{
	if (m_pRoleAttr == NULL || mapCurrency.size() != MAX_CURRENCY_TYPE)
	{
		return MALL_UNKNOWN;
	}

	CurrencyMap::iterator itNeedPayMCoin = mapCurrency.find(EMallCurrencyType_MCoin);
	CurrencyMap::iterator itNeedPayBindMCoin = mapCurrency.find(EMallCurrencyType_BindMCoin);
	CurrencyMap::iterator itNeedPayMoney = mapCurrency.find(EMallCurrencyType_Money);

	if (itNeedPayMCoin == mapCurrency.end() || itNeedPayBindMCoin == mapCurrency.end() || itNeedPayMoney == mapCurrency.end())
		return MALL_UNKNOWN;

	unsigned int nPoint = m_pRoleAttr->GetPoint();
	unsigned int nBindPoint = m_pRoleAttr->GetBindPoint();
	unsigned int nMoney = m_pRoleAttr->GetMoney();

	WriteLog(LOGLEVEL_DEBUG, "[INFO] MALLLOG role[%u] bGiveToFriend=[%s]to pay mcoin=[%u] bindmcoin=[%u] money=[%u], has mcoin=[%u] bindmcoin=[%u] money=[%u]",
		m_pRoleAttr->GetRoleID(), bGiveToFriend?"true":"false", itNeedPayMCoin->second, itNeedPayBindMCoin->second, itNeedPayMoney->second, nPoint, nBindPoint, nMoney);

	if (nMoney < itNeedPayMoney->second) // 金券不足用绑定M币补充支付
	{
		itNeedPayBindMCoin->second = itNeedPayBindMCoin->second + itNeedPayMoney->second - nMoney;
		itNeedPayMoney->second = nMoney;
	}
	if (nBindPoint < itNeedPayBindMCoin->second) // 绑定M币不足用M币补充支付
	{
		itNeedPayMCoin->second = itNeedPayMCoin->second + itNeedPayBindMCoin->second - nBindPoint;
		itNeedPayBindMCoin->second = nBindPoint;
	}

	if (bGiveToFriend) // 赠送给好友时，只能使用M币支付
	{
		itNeedPayMCoin->second = itNeedPayMCoin->second + itNeedPayBindMCoin->second + itNeedPayMoney->second;
		itNeedPayBindMCoin->second = 0;
		itNeedPayMoney->second = 0;
	}

	WriteLog(LOGLEVEL_DEBUG, "[INFO] MALLLOG role[%u] after calc: need mcoin=[%u] bindmcoin=[%u] money=[%u], has mcoin=[%u] bindmcoin=[%u] money=[%u]",
		m_pRoleAttr->GetRoleID(), itNeedPayMCoin->second, itNeedPayBindMCoin->second, itNeedPayMoney->second, nPoint, nBindPoint, nMoney);

	if (nPoint < itNeedPayMCoin->second) // M币不足直接报错
	{
		WriteLog(LOGLEVEL_ERROR, "[ERROR] MALLLOG role[%u] after calc: need mcoin[%u] > has mcoin[%u]",
			m_pRoleAttr->GetRoleID(), itNeedPayMCoin->second, nPoint);
		return MALL_BILL_CASH_NOTENOUGH;
	}

	return MALL_OK;
}

int CEntityMallComponent::CanAddItem(MapPeddleGoods & mapGoods)
{
	if (m_pRoleItem == NULL || mapGoods.size() == 0)
	{
		return MALL_ITEMLIST_EMPTY;
	}

	list<CItem> listItem;

	CItem aItem;
	MapPeddleGoods::iterator it = mapGoods.begin();
	while (it != mapGoods.end())
	{
		itemtype_t nItemType = it->first;
		PeddleGood & aPeddleGood = it->second;

		//需要累计所有物品需要的总空格，而不仅仅是单独检查每一个物品是否能添加成功了就行啦！
		ItemConfig * pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
		if(pItemInfo == NULL)
		{
			return MALL_PARAM_ERROR;
		}

		unsigned short nCount = 0;
		int nValidTime = 0;

		if (pItemInfo->IsExpandable())
		{
			nCount = GetCountByGoodSelIndex(nItemType,aPeddleGood.m_nSelIndex);
			if (nCount == 0)
			{
				return MALL_PARAM_ERROR;
			}
		}
		else
		{
			nValidTime = GetValidTimeByGoodSelIndex(nItemType,aPeddleGood.m_nSelIndex);
			nCount = 1;
			if (nValidTime == 0)
			{
				return MALL_PARAM_ERROR;
			}
		}
		aItem.m_nItemType = nItemType;
		aItem.m_nItemCount = nCount;
		aItem.m_nValidTime = nValidTime;
		listItem.push_back(aItem);
		it++;
	}
	int nRes = EItemError_Success;
	if ( (nRes = m_pRoleItem->CanAddAllItem(listItem, false)) != EItemError_Success)
	{
		return nRes;
	}
	return MALL_OK;
}

void CEntityMallComponent::SetGoodsAddUseValue(MapPeddleGoods & mapGoods, bool bAddUse)
{
	MapPeddleGoods::iterator it = mapGoods.begin();
	while (it != mapGoods.end())
	{
		PeddleGood & aPeddleGood = it->second;
		aPeddleGood.m_bIsAddUse = bAddUse;
		it++;
	}
}

bool CEntityMallComponent::CanBuyBadageCard(itemtype_t itemType)
{
    if (NULL == m_pRoleAttr)
        return false;

    unsigned char gridNum = m_pRoleAttr->GetBadgeGridNum();

    if (ITEMID_EXTEND_BADAGE_GRID_CARD_PRIMARY == itemType && gridNum > 2)
        return false;
    else if (ITEMID_EXTEND_BADAGE_GRID_CARD_INTERMEDIATE == itemType && gridNum > 3)
        return false;
    else if (ITEMID_EXTEND_BADAGE_GRID_CARD_LEVEL_ADVANCED == itemType && gridNum > 4)
        return false;

    return true;
}

int CEntityMallComponent::AddItem(MapPeddleGoods & mapGoods)
{
	if (m_pRoleItem == NULL || mapGoods.size() == 0)
	{
		return MALL_ITEMLIST_EMPTY;
	}

	MapPeddleGoods::iterator it = mapGoods.begin();
	while (it != mapGoods.end())
	{
		itemtype_t nItemType = it->first;
		PeddleGood & aPeddleGood = it->second;

		unsigned short nCount = 0;
		int nValidTime = 0;

		int nRes = GetCountOrValidTimeByGoodSelIndex(nItemType, aPeddleGood.m_nSelIndex, nCount, nValidTime);
		if ( nRes != MALL_OK )
		{
			return nRes;
		}

		if ( (nRes = m_pRoleItem->AddItem(nItemType,nCount,nValidTime, EItemAction_Add_BuyInMall, aPeddleGood.m_nCost, true, aPeddleGood.m_bIsAddUse)) != EItemError_Success){
			return nRes;
		}

		// 购买婚戒
		ItemConfig * pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
		if ( pItemInfo != NULL )
		{
			if ( pItemInfo->m_nIntimacy  > 0  &&  pItemInfo->IsBadge_Type_WeddingRing() )
			{	// 婚戒给对方发送邮件
				std::string strMailTitle(CLocalization::Instance().GetString("Couple_Buy_Ring_Title"));
				std::string strMailContent(CLocalization::Instance().GetString("Couple_Buy_Ring_Content"));
                CMailMgr::Instance().SendSystemMailToPlayer( m_pRoleCouple->CoupleID(), EMailType_BuyRing,
                    strMailTitle, strMailContent, nItemType, nCount, nValidTime );
			}
		}

		it++;
	}

	return MALL_OK;
}

unsigned short CEntityMallComponent::GetCountByGoodSelIndex(itemtype_t nItemType,unsigned char nSelIndex)
{
	ItemConfig * pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
	if(pItemInfo == NULL){
		return 0;
	}

	if (pItemInfo->IsExpandable())
	{
		if (nSelIndex == 0){
			return 1;
		}
		else if (nSelIndex == 1){
			return 5;
		}
		else if (nSelIndex == 2){
			return 10;
		}
		else if (nSelIndex == 3){
			return 20;
		}

	}
	return 0;
}

int CEntityMallComponent::GetValidTimeByGoodSelIndex(itemtype_t nItemType,unsigned char nSelIndex)
{
	ItemConfig * pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
	if(pItemInfo == NULL){
		return 0;
	}

	if (pItemInfo->IsEquip())
	{
		if (nSelIndex == 0){
			return 60 * 60 * 24;
		}
		else if (nSelIndex == 1){
			return 7 * 60 * 60 * 24;
		}
		else if (nSelIndex == 2){
			return 30 * 60 * 60 * 24;
		}
		else if (nSelIndex == 3){
			return -1; // 永久
		}
		else if (nSelIndex == EPeddleGoodSel_Experience)
		{
			return CMallDataMgr::Instance().m_dayCanExperienced * 60 * 60 * 24; // 体验卡可以体验的时间
		}
	}
	return 0;
}

int CEntityMallComponent::GetCountOrValidTimeByGoodSelIndex(itemtype_t nItemType,unsigned char nSelIndex, unsigned short& nCount, int& nValidTime)
{
	ItemConfig * pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
	if(pItemInfo == NULL){
		return MALL_PARAM_ERROR;
	}

	if (pItemInfo->IsExpandable()){
		nCount = GetCountByGoodSelIndex(nItemType, nSelIndex);
        nValidTime = -1;
		if (nCount == 0){
			return MALL_PARAM_ERROR;
		}
	}
	else{
		nValidTime = GetValidTimeByGoodSelIndex(nItemType, nSelIndex);
		nCount = 1;
		if (nValidTime == 0){
			return MALL_PARAM_ERROR;
		}
	}
	return MALL_OK;
}

bool CEntityMallComponent::GetCItemByPeddleGood(const PeddleGood& good, CItem& item)
{
	item.m_nItemType = good.m_nType;
	int nRes = GetCountOrValidTimeByGoodSelIndex(good.m_nType, good.m_nSelIndex, item.m_nItemCount, item.m_nValidTime);
	return nRes == MALL_OK;
}

int CEntityMallComponent::CanMailItem(MapPeddleGoods & mapGoods,unsigned int nDestRoleID)
{
	if (m_pRoleMail == NULL || mapGoods.size() == 0)
	{
		return MALL_ITEMLIST_EMPTY;
	}

	MapPeddleGoods::iterator it = mapGoods.begin();
	while (it != mapGoods.end())
	{
		itemtype_t nItemType = it->first;
		PeddleGood & aPeddleGood = it->second;

		ItemConfig * pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
		if(pItemInfo == NULL){
			return MALL_PARAM_ERROR;
		}

		unsigned short m_nCount = 0;
		int nValidTime = 0;

		if (pItemInfo->IsExpandable()){
			m_nCount = GetCountByGoodSelIndex(nItemType,aPeddleGood.m_nSelIndex);
			if(m_nCount == 0){
				return MALL_PARAM_ERROR;
			}
		}
		else{
			nValidTime = GetValidTimeByGoodSelIndex(nItemType,aPeddleGood.m_nSelIndex);
			m_nCount = 1;
			if(nValidTime == 0){
				return MALL_PARAM_ERROR;
			}
		}

		if(pItemInfo->m_bUnique && m_nCount > 1){
			return MALL_ITEM_UNIQUEEXIST;
		}

		it++;
	}
	return MALL_OK;
}

int CEntityMallComponent::OnMailItem(MapPeddleGoods & mapGoods,unsigned int nDestRoleID,string strInfo, CRoleSendLog::ESendType eSendType)
{
	MapPeddleGoods::iterator it = mapGoods.begin();
	while (it != mapGoods.end())
	{
		itemtype_t nItemType = it->first;
		PeddleGood & aPeddleGood = it->second;

		ItemConfig * pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
		if(pItemInfo == NULL){
			return MALL_PARAM_ERROR;
		}

		unsigned short nItemCount = 0;
		int nValidTime = 0;

		if (pItemInfo->IsExpandable()){
			nItemCount = GetCountByGoodSelIndex(nItemType,aPeddleGood.m_nSelIndex);
			if(nItemCount == 0){
				return MALL_PARAM_ERROR;
			}
		}
		else{
			nValidTime = GetValidTimeByGoodSelIndex(nItemType,aPeddleGood.m_nSelIndex);
			nItemCount = 1;
			if(nValidTime == 0){
				return MALL_PARAM_ERROR;
			}
		}

		std::string strContent;
		SafeFormat(strContent, CLocalization::Instance().GetString("Mail_Content1_Send_Item"), m_pRoleAttr->GetRoleName());

		if (strInfo.size() > 0) {
			if (m_pRoleAttr != NULL) {
				if (m_pRoleAttr->GetSex() == ESexType_Male) {
					SafeFormat(strContent, CLocalization::Instance().GetString("Mail_Content2_Send_Item"), m_pRoleAttr->GetRoleName(), "\n", strInfo.c_str(), "\n");
				} else if (m_pRoleAttr->GetSex() == ESexType_Female) {
					SafeFormat(strContent, CLocalization::Instance().GetString("Mail_Content3_Send_Item"), m_pRoleAttr->GetRoleName(), "\n", strInfo.c_str(), "\n");
				} else {
					WriteLog(LOGLEVEL_ERROR, "CEntityMallComponent::OnMailItem Get Role Sex Error\n");
				}
			}
		}

        CMailMgr::Instance().SendSystemMailToPlayer( nDestRoleID, EMailType_Player_Send_Item,
            CLocalization::Instance().GetString( "Mail_Title_Send_Item" ), strContent, nItemType, nItemCount, nValidTime );
		_LogRoleSend(m_pRoleAttr->GetRoleID(), eSendType, nItemType, nItemCount, nValidTime, nDestRoleID, 0);
		it++;
	}

	return MALL_OK;
}

bool CEntityMallComponent::IsValidExperiencedItemType(itemtype_t itemType)
{
	ItemConfig* itemConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(itemType);
	if (NULL == itemConfig)
	{
		WriteLog(LOGLEVEL_ERROR, "error to find item type[%u]...", itemType);
		return false;
	}

	unsigned char an1 = eItemClassType_Equip;
	unsigned char an2 = eItemEquipType_Cloth;
// 	static unsigned char an3[] = {eItemClothType_Body, eItemClothType_Leg, eItemClothType_Feet, eItemClothType_Suit};
// 	static size_t an3Size = sizeof(an3)/sizeof(an3[0]);
	if (itemConfig->m_anType[0] != an1 || itemConfig->m_anType[1] != an2)
		return false;

	for (size_t i = 0; i < CMallDataMgr::Instance().m_vecExperienceType.size(); ++i)
	{
		if (itemConfig->m_anType[2] == CMallDataMgr::Instance().m_vecExperienceType[i])
			return true;
	}

	return false; // wrong type.
}

bool CEntityMallComponent::HasExperiencedItem(itemtype_t itemType)
{
	return m_setExperienceCardBuyedProduct.find(itemType) != m_setExperienceCardBuyedProduct.end();
}

// if has enough experienced times.
bool CEntityMallComponent::HasExperiencedTimes(unsigned short experienceNum)
{
	ResetExperienceCardInfo(time((NULL)));

	return m_experienceCardUsedNum + experienceNum <= CMallDataMgr::Instance().m_experienceCardTimesPerDay;
}

bool CEntityMallComponent::ReduceExperienceTimes(const VecPeddleGoods &experienceGoods)
{
	if (NULL == m_pRoleItem->SearchItem(CMallDataMgr::Instance().m_experienceCardItemType))
	{
		WriteLog(LOGLEVEL_ERROR, "Miss experience card.");
		return false;
	}

	// 1、使用记录
	if (!HasExperiencedTimes((unsigned short)experienceGoods.size()))
	{
		WriteLog(LOGLEVEL_ERROR, "has no times.");
		return false;
	}
	m_experienceCardUsedNum += (unsigned short)experienceGoods.size();

	// 2、体验物品记录
	for (VecPeddleGoods::const_iterator citer = experienceGoods.begin(); citer != experienceGoods.end(); ++citer)
	{
		if (HasExperiencedItem(citer->m_nType))
		{
			WriteLog (LOGLEVEL_ERROR, "experienced items[%u]", citer->m_nType);
			return false;
		}
	}
		
	AddQuery(QUERY_ExperienceCard_UpdateTime, m_experienceCardUsedNum, NULL, m_experienceCardUsedStamp);
	for (size_t i = 0; i < experienceGoods.size(); ++i)
		AddQuery(QUERY_ExperienceCard_AddExperiencedItem, experienceGoods[i].m_nType, NULL);
	
	return true; // 成功扣除
}

void CEntityMallComponent::ResetExperienceCardInfo(time_t now)
{
	if (!IsSameDay(m_experienceCardUsedStamp, now))
	{
		m_experienceCardUsedStamp = now;
		m_experienceCardUsedNum = 0;
	}
}

void CEntityMallComponent::NotifyNewExperiencedItem(std::vector<itemtype_t> &items)
{
	time_t now = time(NULL);
	ResetExperienceCardInfo(now);
	GameMsg_S2C_ExperiencedItemNtf ntf;

	ntf.m_vecExperiencedItemNew.assign(items.begin(), items.end());
	
	ntf.m_usedTime = m_experienceCardUsedNum;
	ntf.m_nCountDown = GetNextDayStartTime(now) - m_experienceCardUsedStamp;

	m_pNetCpt->SendPlayerMsg(&ntf);
}

int CEntityMallComponent::FinishBill(CurrencyMap &mapCurrency,EFinishBillFrom from)
{
	if (m_pRoleAttr == NULL || mapCurrency.size() != MAX_CURRENCY_TYPE)
    {
		return MALL_UNKNOWN;
	}

	CurrencyMap::iterator itPoint = mapCurrency.find(EMallCurrencyType_MCoin);
	CurrencyMap::iterator itBindPoint = mapCurrency.find(EMallCurrencyType_BindMCoin);
	CurrencyMap::iterator itMoney = mapCurrency.find(EMallCurrencyType_Money);
	if(itPoint == mapCurrency.end() || itBindPoint == mapCurrency.end() || itMoney == mapCurrency.end())
		return MALL_UNKNOWN;

	unsigned int nPoint = itPoint->second;
	unsigned int nBindPoint = itBindPoint->second;
	unsigned int nMoney = itMoney->second;

	// 再次检查下
	if (nPoint > m_pRoleAttr->GetPoint() || nBindPoint > m_pRoleAttr->GetBindPoint() 
        || nMoney > (unsigned int)m_pRoleAttr->GetMoney() || nPoint + nBindPoint + nMoney == 0)
	{
		WriteLog(LOGLEVEL_ERROR, "MALLLOG role[%u] need mcoin=[%u] bindmcoin=[%u] money=[%u], has mcoin=[%u] bindmcoin=[%u] money=[%u]",
			m_pRoleAttr->GetRoleID(), nPoint, nBindPoint, nMoney, m_pRoleAttr->GetPoint(), m_pRoleAttr->GetBindPoint(), m_pRoleAttr->GetMoney());

		return MALL_UNKNOWN;
	}

    if ( from == EFinishBillFrom_Egg || from == EFinishBillFrom_Send )
    {
        m_pRoleAttr->ChangeBill(nPoint*(-1), EChangeBillCause_Del_BuyItem, true, false, false );
        m_pRoleAttr->ChangeBindBill(nBindPoint*(-1), EChangeBindBillCause_Del_BuyItem, true, false );

        if ( m_pRoleVIP->IsVIP() )
        {
            // 在线跨天处理
            if ( !IsTimeIsToday(m_nSendItemTime) )
            {
                m_nSendItemCostBill = 0;
            }

            int nRemainBill = CVIPDataMgr::Instance().GetSendAddVipExpMaxBill() - m_nSendItemCostBill;
            if ( nRemainBill > 0 )
            {
                if ( (unsigned int)nRemainBill >= nPoint )
                {
                    m_pRoleVIP->VIPBillExpBounus(nPoint);
                    m_nSendItemCostBill += nPoint;
                }
                else
                {
                    m_pRoleVIP->VIPBillExpBounus(nRemainBill);
                    m_nSendItemCostBill += nRemainBill;
                }
            }
            m_nSendItemTime = (unsigned int)time(NULL);

            AddQuery(QUERY_UpdateMallSendItemEvent, m_pRoleAttr->GetRoleID(), NULL, m_nSendItemCostBill);
        }
    }
    else
    {
        m_pRoleAttr->ChangeBill(nPoint*(-1), EChangeBillCause_Del_BuyItem);
        m_pRoleAttr->ChangeBindBill(nBindPoint*(-1), EChangeBindBillCause_Del_BuyItem);
    }

	m_pRoleAttr->ChangeMoney(nMoney*(-1), EChangeMoneyCause_Del_BuyItem, 0);
	return MALL_OK;
}


void CEntityMallComponent::_LogRoleSend(unsigned int nRoleID, CRoleSendLog::ESendType eSendType, itemtype_t nItemType, unsigned short nItemCount,
				  int nDuration, unsigned int nTargetRoleID, __int64 nMailID)
{
    CRoleSendLog * pLog = new CRoleSendLog();
    pLog->m_nRoleID = nRoleID;
    pLog->m_eSendType = eSendType;
    pLog->m_nItemType = nItemType;
    pLog->m_nItemCount = nItemCount;
    pLog->m_nDuration = nDuration;
    pLog->m_nTargetRoleID = nTargetRoleID;
    pLog->m_nMailID = nMailID;
    AddQuery(QUERY_LogRoleSend, 0, pLog);
}

void CEntityMallComponent::InsertActionLog(CRoleMallLog::EMallAction eAction)
{
    CRoleMallLog * pLog = new CRoleMallLog();
    pLog->m_nRoleID = m_pRoleAttr->GetRoleID();
    pLog->m_eAction = eAction;

    AddQuery(QUERY_LogRoleMall, 0, pLog);
}


//end file


