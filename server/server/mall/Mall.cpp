#include "../libServerFrame/QueryManager.h"
#include "Mall.h"
#include "CurrencyType.h"
#include "MallMsgDefine.h"
#include "GameMsg_Mall.h"
#include "../share/ServerMsgDef.h"
#include "../../socket/Ini.h"
#include "../../socket/GameMsg_Map.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../AmuseRoom/AmuseRoomDataManager.h"
#include "../logic/PlayerManager.h"

extern CIni g_GameConfig;
extern CQueryManager* g_pQueryMgr;


CMall::CMall():GameMsg_Processor(true,true)
{
}

CMall::~CMall()
{
    for (int i=0;i<GOODS_TYPE_MAX;i++)
    {
        if (m_pGoodsMsg[i] != NULL)
        {
            delete m_pGoodsMsg[i];
            m_pGoodsMsg[i] = NULL;
        }
    }
}

CMall& CMall::Instance()
{
    static CMall stcMall;
    return stcMall;
}

void CMall::Initialize()
{
    m_MapGoods.Clear();

    m_bIsOpening = false;
    m_bInitialized = false;
    m_nVersion = 0;

    m_nNextEventTime = 0x7FFFFFFF;

    for (int i=0;i<GOODS_TYPE_MAX;i++)
    {
        m_pGoodsMsg[i] = NULL;
    }

    RegComponentNetMsgMap();

    if(g_GameConfig.GetInt("SupertMarket","IsOpening", 0)==1)
        Open();
}

bool CMall::IsOpening() const
{
    return m_bIsOpening && m_bInitialized;
}

const char * CMall::SerializeName()
{
    return "MallManager";	
}

void CMall::SeriizeMallGoodsInfo(CParamPool & IOBuff)
{
    ItemConfig* pItemInfo = NULL;
    MallGood* pGood = NULL;

    int nCount = 0;
    unsigned int nOldPos = IOBuff.GetCurPosition();
    IOBuff.AddUShort(0);
    list<itemtype_t> & listGoods = m_MapGoods.GetKeyList();
    for (list<itemtype_t>::iterator it = listGoods.begin();it != listGoods.end();it++)
    {	
        pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(*it);	
        pGood = m_MapGoods.Find(*it);
        if(pItemInfo == NULL || pGood == NULL || pGood->m_nState != 1) {
            continue;
        }

        if (pGood != NULL && pGood->m_nType > 0){
            pGood->doEncode(IOBuff);
            ++nCount;
        }
    }

    unsigned int nFinalPos = IOBuff.GetCurPosition();
    IOBuff.SetCurPosition( nOldPos );
    IOBuff.ReplaceShort((short)nCount);
    IOBuff.SetCurPosition( nFinalPos );
}

MapMallGoods & CMall::MallGoods()
{
    return m_MapGoods;
}

MallGood * CMall::FindGoods(itemtype_t id)
{
    MallGood * pGood = m_MapGoods.Find(id);
    return pGood;
}

void CMall::RegComponentNetMsgMap()
{
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_MallDesire);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_MallReload);

    RegMsgProcessor(MSG_G2S_MALLDESIRE, &CMall::OnDesireSync);
    RegMsgProcessor(MSG_G2S_Mall_Reload, &CMall::OnMallReload);

    //数据库
    RegQueryProcessor(QUERY_Mall_LoadInfo, &CMall::OnLoadGoodsInfo);
}

void CMall::OnDesireSync(GameMsg_Base & msg,CSlotPeer & SlotPeer)
{
    GameMsg_G2S_MallDesire * pMsg = (GameMsg_G2S_MallDesire *)&msg;
    if(pMsg != NULL){
        AddGoodsDesireCount(pMsg->m_nTypeID,pMsg->nAddNum);
    }
}

void CMall::LoadGoodsInfo()
{
    list<MallGood *> *listTemp = new list<MallGood *>;
    if (listTemp)
    {
        g_pQueryMgr->AddQuery(QUERY_Mall_LoadInfo,0,listTemp);
        WriteLog(LOGLEVEL_DEBUG, "提交查询所有 Mall商品信息 的数据库请求");
    }
}

void CMall::OnLoadGoodsInfo(QueryBase & pQuery)
{
    ASSERT(pQuery.m_pAttachment != NULL);
    if (pQuery.m_pAttachment == NULL){
        return;
    }
    if(pQuery.m_nResult!=0 && pQuery.m_nResult != Query_Res_Empty){
        WriteLog(LOGLEVEL_ERROR, "接受到查询所有 Mall 信息的结果－－－－失败．");
        delete (list<MallGood *> *)pQuery.m_pAttachment;
        return;
    }
    InitGoodsInfo((list<MallGood*> *)(pQuery.m_pAttachment));	
    int nVersion = pQuery.m_nAttachment;
    if (nVersion > 0){
        m_nVersion = (unsigned short)nVersion;
    }
    CAmuseRoomDataManager::Instance().Init();
    m_bInitialized = true;
    CPlayerManager::Instance().OnMallReload();
}

void CMall::InitGoodsInfo(list<MallGood*> *pList)
{
    WriteLog(LOGLEVEL_DEBUG, "接受到查询所有 Mall 信息的结果，共有%d个．",pList->size());
    m_nNextEventTime = 0x7FFFFFFF;
    unsigned int timeNow = (unsigned int)time(NULL);

    if(pList->size()>0)
    {
        m_MapGoods.Clear();

        MallGood * pGood = NULL;
        list<MallGood*>::iterator itTemp = pList->begin();
        for(;itTemp != pList->end();itTemp++)
        {
            pGood = (MallGood*)(*itTemp);

            //不在贩卖期间
            if(!isBetween(pGood->m_nSell_time0,pGood->m_nSell_time1,timeNow)){
                pGood->m_nState = 0;
            }
            //不在打折期间
            if(!isBetween(pGood->m_nDiscount_time0,pGood->m_nDiscount_time1,timeNow)){
                pGood->m_nDiscount = 100;
            }

            AddGood(pGood);

            if(pGood->m_nSell_time0 > timeNow && pGood->m_nSell_time0 < m_nNextEventTime)
                m_nNextEventTime = pGood->m_nSell_time0;
            if(pGood->m_nSell_time1 > timeNow && pGood->m_nSell_time1 < m_nNextEventTime)
                m_nNextEventTime = pGood->m_nSell_time1;
            if(pGood->m_nDiscount_time0 > timeNow && pGood->m_nDiscount_time0 < m_nNextEventTime)
                m_nNextEventTime = pGood->m_nDiscount_time0;
            if(pGood->m_nDiscount_time1 > timeNow && pGood->m_nDiscount_time1 < m_nNextEventTime)
                m_nNextEventTime = pGood->m_nDiscount_time1;

            delete pGood;
        }
    }
    delete pList;	

    InitMsg();
}

void CMall::AddGood(MallGood *good)
{
    if (good != NULL && good->m_nType > 0){
        m_MapGoods.Add(good->m_nType,*good);
    }
}

MallGood * CMall::GetGood(itemtype_t nType)
{
    return m_MapGoods.Find(nType);
}

bool CMall::Open()
{	
    if(!m_bIsOpening && !m_bInitialized){
        m_bIsOpening = true;
        LoadGoodsInfo();
        return true;
    }
    return false;
}

void CMall::Close()
{
    m_bIsOpening = false;
    m_bInitialized = false;
    m_nVersion = 0;
    m_MapGoods.Clear();
}


void CMall::Reload()
{
    Close();
    Open();
}

bool CMall::InitMsg()
{	
    unsigned short nGoodsNum  = 0;
    int nMaxDataSize = 0;

    unsigned short nCount[GOODS_TYPE_MAX]  = {0};
    char * pBuffer[GOODS_TYPE_MAX]  = {NULL};
    char * pTemp[GOODS_TYPE_MAX]	   = {NULL};
    unsigned char  * pnGoodsType[GOODS_TYPE_MAX]  = {NULL};
    unsigned short * pnGoodsNum[GOODS_TYPE_MAX]  = {NULL};

    //总数量
    nGoodsNum = m_MapGoods.Count();
    nMaxDataSize = sizeof(unsigned char) + sizeof(unsigned short)+nGoodsNum*(sizeof(MallGood));

    //先按照最大的情况分配内存
    for (unsigned char i=0;i<GOODS_TYPE_MAX;i++)
    {
        pBuffer[i] = new char[nMaxDataSize+1];
        memset(pBuffer[i],0,nMaxDataSize+1);
        pTemp[i] = pBuffer[i];

        pnGoodsType[i] = (unsigned char*)pTemp[i];
        *(pnGoodsType[i]) = 0;
        pTemp[i] += sizeof(unsigned char);

        pnGoodsNum[i] = (unsigned short*)pTemp[i];
        *(pnGoodsNum[i]) = 0;
        pTemp[i] += sizeof(unsigned short);
    }

    ItemConfig* pItemInfo = NULL;
    MallGood* pGood = NULL;

    list<itemtype_t> & listGoods = m_MapGoods.GetKeyList();
    list<itemtype_t>::iterator it = listGoods.begin();
    for (;it != listGoods.end();it++)
    {	
        pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(*it);	
        pGood = m_MapGoods.Find(*it);
        if(pItemInfo == NULL || pGood == NULL || pGood->m_nState != 1)
            continue;

        PacketToBuffer(&(pTemp[GOODS_TYPE_ALL]),pGood,nCount[GOODS_TYPE_ALL]);

        //道具
        if (pItemInfo->IsExpandable()){
            PacketToBuffer(&(pTemp[GOODS_TYPE_EXPENDABLE]),pGood,nCount[GOODS_TYPE_EXPENDABLE]);
        }
        //装备
        else if (pItemInfo->IsCloth()){
            PacketToBuffer(&(pTemp[GOODS_TYPE_CLOTH]),pGood,nCount[GOODS_TYPE_CLOTH]);
        }

        else if (pItemInfo->IsBadge()){
            PacketToBuffer(&(pTemp[GOODS_TYPE_BADGE]),pGood,nCount[GOODS_TYPE_BADGE]);
        }

        //如果是热销
        if (pGood->m_nHot > 0){
            PacketToBuffer(&(pTemp[GOODS_TYPE_HOT]),pGood,nCount[GOODS_TYPE_HOT]);
        }
    }

    for (unsigned char i=0;i<GOODS_TYPE_MAX;i++)
    {
        *(pnGoodsType[i]) = i;
        *(pnGoodsNum[i]) = nCount[i];

        unsigned short nDataSize = sizeof(unsigned char)+sizeof(unsigned short)+nCount[i]*(sizeof(MallGood));	

        char * pData = new char[nDataSize+1];
        memset(pData,0,nDataSize+1);
        GameMsg_S2C_MallGoodsInfo * m_msgTemp = new GameMsg_S2C_MallGoodsInfo;
        memcpy(pData,pBuffer[i],nDataSize);
        m_msgTemp->pData = pData;
        m_msgTemp->nDataSize = nDataSize;

        if(m_pGoodsMsg[i])
            delete m_pGoodsMsg[i];
        m_pGoodsMsg[i] = m_msgTemp;

        delete []pBuffer[i];
    }

    return true;
}

void CMall::PacketToBuffer(char ** pTempGood,MallGood * pGood, unsigned short & nCount)
{
    MallGood * pTemp = (MallGood *)(*pTempGood);
    (*pTemp) = *pGood;
    (*pTempGood) += sizeof(MallGood);
    nCount++;
}

GameMsg_S2C_MallGoodsInfo * CMall::GetGoodsMsg(unsigned char nType)
{
    if(m_nNextEventTime<= (unsigned int)time(NULL))
        Reload();

    if (nType < GOODS_TYPE_MAX)
    {
        return m_pGoodsMsg[nType];
    }
    return NULL;
}

bool CMall::GetGoodsDataListByID(itemtype_t nTypeID,MapMallGoods & aGoodMap)
{
    ItemConfig * pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(nTypeID);	
    MallGood * pGood = GetGood(nTypeID);
    if(pItemInfo != NULL && pGood != NULL){
        aGoodMap.Add(pGood->m_nType,*pGood);
        return true;
    }

    return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//buy logic
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CMall::CanBuy(VecPeddleGoods & vecGoods,MapPeddleGoods & mapGoods,CurrencyMap & mapBills,unsigned char nDestPlayerSexType, unsigned int nIntimacy, bool IsMarried )
{
    if (!IsOpening()){
        return MALL_NOTOPEN;
    }

    if(m_nNextEventTime<= (unsigned int)time(NULL))
        Reload();

    mapGoods.clear();
    InitCurrencyMap(mapBills);

    int nReturn = MALL_OK;
    VecPeddleGoods::iterator it = vecGoods.begin();
    while(nReturn == MALL_OK && it != vecGoods.end())
    {
        nReturn = AddPeddleGood(mapGoods,mapBills,*it,it->m_bUseCoupon,nDestPlayerSexType, nIntimacy, IsMarried );
        it++;
    }
    return nReturn;
}

bool CMall::CanBuyGoodByID(itemtype_t nTypeID)
{
    MallGood * pGood = GetGood(nTypeID);
    return  pGood != NULL && pGood->m_nState == 1 && pGood->m_nIsPrecious == 0;
};

bool CMall::CanMakeWishByID(itemtype_t nTypeID)
{
    MallGood * pGood = GetGood(nTypeID);
    return  pGood != NULL && pGood->m_nIsPrecious == 0;
}

int CMall::AddBuyGoods(CurrencyMap & mapCurrency, const MallGood *pMallGood, int nSelIndex, bool bUseCoupon)
{
    // 折扣计算
    int nDiscount = 100;
    if (pMallGood->m_nPayType != eMallPayType_FixedRatioMoney) // 此支付类型不享受折扣
    {
        nDiscount = pMallGood->m_nDiscount;
    }

    int nCost = 0;
    switch (nSelIndex)
    {
    case 0:
        nCost = (pMallGood->m_nPrice*nDiscount / 100);
        break;
    case 1:
        nCost = (pMallGood->m_nPrice1*nDiscount / 100);
        break;
    case 2:
        nCost = (pMallGood->m_nPrice2*nDiscount / 100);
        break;
    case 3:
        nCost = (pMallGood->m_nPrice3*nDiscount / 100);
        break;
    default:
        nCost = -1;
        return -1;
    }

    // 计算可能需要支付的货币（后面CanBill中会计算最终需要支付的货币）
    switch (pMallGood->m_nPayType)
    {
    case eMallPayType_MCoin:
        ModifyCurrencyMap(mapCurrency, EMallCurrencyType_MCoin, nCost);
        break;
    case eMallPayType_FixedRatioMoney:
        if (pMallGood->m_nUseChangePercent > 0 && pMallGood->m_nUseChangePercent < 100 && bUseCoupon)
        {
            int nMoney = pMallGood->m_nUseChangePercent * nCost / 100;
            ModifyCurrencyMap(mapCurrency, EMallCurrencyType_Money, nMoney);
            ModifyCurrencyMap(mapCurrency, EMallCurrencyType_BindMCoin, nCost - nMoney);
        }
        else
        {
            ModifyCurrencyMap(mapCurrency, EMallCurrencyType_BindMCoin, nCost);
        }
        break;
    case eMallPayType_Money:
        ModifyCurrencyMap(mapCurrency, EMallCurrencyType_Money, nCost);
        break;
    case eMallPayType_BindMCoin:
        ModifyCurrencyMap(mapCurrency, EMallCurrencyType_BindMCoin, nCost);
        break;
    default:
        return -1;
    }

    return 0;
}

int CMall::AddPeddleGood(MapPeddleGoods &mapGoods, CurrencyMap &mapCurrency, PeddleGood &peddleGoods,
    bool bUseCoopon, unsigned char nDestPlayerSexType, unsigned int nIntimacy, bool IsMarried)
{
    if (peddleGoods.m_nSelIndex >= 4 || peddleGoods.m_nCost <= 0)
    {
        return MALL_PARAM_ERROR;
    }

    if (peddleGoods.m_nPayType != eMallPayType_MCoin && peddleGoods.m_nPayType != eMallPayType_FixedRatioMoney 
        && peddleGoods.m_nPayType != eMallPayType_Money && peddleGoods.m_nPayType != eMallPayType_BindMCoin)
    {
        WriteLog(LOGLEVEL_ERROR, "[ERROR] invalid good pay type=%d", (int)peddleGoods.m_nPayType);
        return MALL_PARAM_ERROR;
    }

    // 重复添加物品
    MapPeddleGoods::iterator it = mapGoods.find(peddleGoods.m_nType);
    if (it != mapGoods.end())
    {
        return MALL_ITEM_SAMEERROR;
    }

    // 商城不贩卖该商品
    MallGood *pMallGood = GetGood(peddleGoods.m_nType);
    if (pMallGood == NULL || pMallGood->m_nState != 1)
    {
        return MALL_NOTSELL;
    }

    // 非卖品
    if (pMallGood->m_nIsPrecious == 1)
    {
        return MALL_CAN_NOT_BUY_PRECIOUS_ITEMS;
    }

    // 支付类型不一致
    if (pMallGood->m_nPayType != peddleGoods.m_nPayType)
    {
        return MALL_PAYTYPE_INCONFORMITY;
    }

    ItemConfig * pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(peddleGoods.m_nType);
    if (pItemInfo == NULL)
    {
        return MALL_PARAM_ERROR;
    }
    if (pItemInfo->m_SexNeed != ESexType_No && pItemInfo->m_SexNeed != nDestPlayerSexType)
    {
        return MALL_ITEM_SEXERROR;
    }

    // 折扣计算
    int nDiscount = 100;
    if (peddleGoods.m_nPayType != eMallPayType_FixedRatioMoney) // 此支付类型不享受折扣
    {
        nDiscount = pMallGood->m_nDiscount;
    }

    // 判断物品需要的亲密度
    if (pItemInfo->m_nIntimacy > 0 && !IsMarried)
    {
        return MALL_NOT_MARRID;
    }

    if (pItemInfo->m_nIntimacy > (int)nIntimacy)
    {
        return MALL_INTIMACY_NOTENOUGH;
    }

    if (!IsValidCost(*pMallGood, peddleGoods.m_nSelIndex, peddleGoods.m_nCost, nDiscount))
    {
        WriteLog("[ERROR] MALLLOG good price[%d] client cost[%d]", 
            (int)pMallGood->m_nPrice3*nDiscount / 100, (int)peddleGoods.m_nCost);
        return MALL_BILL_INCONFORMITY;
    }

    // 添加购物商品
    mapGoods.insert(MapPeddleGoods::value_type(peddleGoods.m_nType,peddleGoods));

    return CalcGoodsCost(*pMallGood, peddleGoods, mapCurrency);
}

bool CMall::TransGoodsPay(unsigned char uPayType, unsigned int nCost, bool bUseCoupon, unsigned char nUseCouponPersent, CurrencyMap &currencyMap)
{
	if (EMallCurrencyType_Max - 1 != currencyMap.size())
	{
		WriteLog(LOGLEVEL_ERROR, "currencyMap size error.");
		return false;
	}

	unsigned int currency[EMallCurrencyType_Max] = {0};
	if (!TransGoodsPay(uPayType, nCost, bUseCoupon, nUseCouponPersent, currency))
		return false;

	if (0 != currency[EMallCurrencyType_Money])
		ModifyCurrencyMap(currencyMap, EMallCurrencyType_Money, currency[EMallCurrencyType_Money]);
	if (0 != currency[EMallCurrencyType_BindMCoin])
		ModifyCurrencyMap(currencyMap, EMallCurrencyType_BindMCoin, currency[EMallCurrencyType_BindMCoin]);
	if (0 != currency[EMallCurrencyType_MCoin])
		ModifyCurrencyMap(currencyMap, EMallCurrencyType_MCoin, currency[EMallCurrencyType_MCoin]);

	return true;
}


bool CMall::TransGoodsPay(unsigned char uPayType, unsigned int nCost, bool bUseCoupon, unsigned char nUseCouponPersent, unsigned int currency[])
{
	switch (uPayType)
	{
	case eMallPayType_MCoin:
		currency[EMallCurrencyType_MCoin] = nCost;
		break;
	case eMallPayType_FixedRatioMoney:
		if (nUseCouponPersent > 0 && nUseCouponPersent < 100 && bUseCoupon)
		{
			int nMoney = nUseCouponPersent * nCost / 100;
			currency[EMallCurrencyType_Money] = nMoney;
			currency[EMallCurrencyType_BindMCoin] = nCost - nMoney;
		}
		else
		{
			currency[EMallCurrencyType_BindMCoin] = nCost;
		}
		break;
	case eMallPayType_Money:
		currency[EMallCurrencyType_Money] = nCost;
		break;
	case eMallPayType_BindMCoin:
		currency[EMallCurrencyType_BindMCoin] = nCost;
		break;
	default:
		return false;
		break;
	}

	return true;
}


void CMall::InitCurrencyMap(CurrencyMap & mapBills)
{
    mapBills.clear();
    mapBills.insert(CurrencyMap::value_type(EMallCurrencyType_MCoin, 0));
    mapBills.insert(CurrencyMap::value_type(EMallCurrencyType_BindMCoin, 0));
    mapBills.insert(CurrencyMap::value_type(EMallCurrencyType_Money, 0));
}

bool CMall::ModifyCurrencyMap(CurrencyMap & mapBills,unsigned char nCurrencyType, unsigned int nCost)
{
    CurrencyMap::iterator it = mapBills.find(nCurrencyType);
    if (it != mapBills.end())
    {
        unsigned int * pTotalCost = (unsigned int *)&(it->second);
        unsigned int nCurTotalCost = *pTotalCost;
        (*pTotalCost) = nCurTotalCost + nCost; 
        return true;
    }
    return false;
}

int CMall::InsertMallLog(MapPeddleGoods & mapGoods,CurrencyMap & mapBills,unsigned int nSelfRoleID,unsigned int nDestRoleID,__int64 nItemID)
{
    // 整笔交易log
    MALL_LOG * pMalllog = new MALL_LOG();
    if (pMalllog != NULL)
    {
        CurrencyMap::iterator itMCoin = mapBills.find(EMallCurrencyType_MCoin);
        CurrencyMap::iterator itBindMCoin = mapBills.find(EMallCurrencyType_BindMCoin);
        CurrencyMap::iterator itMoney = mapBills.find(EMallCurrencyType_Money);

        pMalllog->m_nRoleID = nSelfRoleID;
        pMalllog->m_nCostPoint = (itMCoin == mapBills.end() ? 0 : itMCoin->second);
        pMalllog->m_nCostBindPoint = (itBindMCoin == mapBills.end() ? 0 : itBindMCoin->second);
        pMalllog->m_nCostMoney = (itMoney == mapBills.end() ? 0 : itMoney->second);
        pMalllog->m_nDestSendRoleID = nDestRoleID;
        pMalllog->m_nItemID = nItemID;

        MapPeddleGoods::iterator it = mapGoods.begin();
        while (it != mapGoods.end())
        {
            PeddleGood & aPeddleGood = it->second;

			MALL_LOG::GoodsPayInfo goodsPayInfo;
			unsigned int currency[EMallCurrencyType_Max] = {0};
			MallGood * pMallGoods = GetGood(it->second.m_nType);
			goodsPayInfo.m_itemType = aPeddleGood.m_nType;
			goodsPayInfo.m_nSelType = aPeddleGood.m_nSelIndex;
			if (NULL != pMallGoods && TransGoodsPay(aPeddleGood.m_nPayType, aPeddleGood.m_nCost, aPeddleGood.m_bUseCoupon, pMallGoods->m_nUseChangePercent, currency))
			{
				goodsPayInfo.m_uBindMCoin = currency[EMallCurrencyType_BindMCoin];
				goodsPayInfo.m_uMCoin = currency[EMallCurrencyType_MCoin];
				goodsPayInfo.m_uMoney = currency[EMallCurrencyType_Money];
			}

            if (aPeddleGood.m_nType > 0)
			{
                pMalllog->m_vecGoodsPayInfo.push_back(goodsPayInfo);
            }
            it++;
        }

        g_pQueryMgr->AddQuery(QUERY_Mall_SaveLog,0,pMalllog,0,NULL);
    }

    // 单个物品日志
    std::vector<MallSigleItemLog> *pSingleItemLog = new std::vector<MallSigleItemLog>;
    MapPeddleGoods::iterator it = mapGoods.begin();
    for ( ; it != mapGoods.end(); ++it )
    {
        MallSigleItemLog loginfo;
        loginfo.nRoleID = nSelfRoleID;
        loginfo.nItemType = it->second.m_nType;
        loginfo.nPayType = it->second.m_nPayType;

        MallGood * pMallGood = GetGood(it->second.m_nType);
		unsigned int currency[EMallCurrencyType_Max] = {0};
        if (pMallGood != NULL && TransGoodsPay(pMallGood->m_nPayType, it->second.m_nCost, it->second.m_bUseCoupon, pMallGood->m_nUseChangePercent, currency))
        {
			loginfo.nBindPoint = currency[EMallCurrencyType_BindMCoin];
			loginfo.nPoint = currency[EMallCurrencyType_MCoin];
			loginfo.nMoney = currency[EMallCurrencyType_Money];
        }

        pSingleItemLog->push_back( loginfo );
    }
    g_pQueryMgr->AddQuery(QUERY_Mall_SigleItemLog,0,pSingleItemLog,0,NULL);

    return 0;
}

unsigned int CMall::OnDesire(itemtype_t nTypeID)
{
    MallGood *pMallGood = GetGood(nTypeID);
    if (pMallGood != NULL)
    {
        pMallGood->AddDesireCount();
        return pMallGood->DesireCount();
    }
    return 0;
}

void CMall::AddGoodsDesireCount(itemtype_t nTypeID, unsigned int nAddCount)
{
    MallGood *pMallGood = GetGood(nTypeID);
    if (pMallGood != NULL)
    {
        pMallGood->AddDesireCount(nAddCount);
    }
}

void CMall::OnMallReload(GameMsg_Base & msg, CSlotPeer & SlotPeer)
{
    LOG_DEBUG << "Mall will reload goods info from database" << std::endl;

    Reload();
}

int CMall::CalcGoodsCost(const MallGood & mallGoods, const PeddleGood &peddleGoods, CurrencyMap &mapCurrency)
{
    // 计算可能需要支付的货币（后面CanBill中会计算最终需要支付的货币）
    switch (mallGoods.m_nPayType)
    {
    case eMallPayType_MCoin:
        ModifyCurrencyMap(mapCurrency, EMallCurrencyType_MCoin, peddleGoods.m_nCost);
        break;
    case eMallPayType_FixedRatioMoney:
        if (mallGoods.m_nUseChangePercent > 0 && mallGoods.m_nUseChangePercent < 100 && peddleGoods.m_bUseCoupon)
        {
            int nMoney = mallGoods.m_nUseChangePercent * peddleGoods.m_nCost / 100;
            ModifyCurrencyMap(mapCurrency, EMallCurrencyType_Money, nMoney);
            ModifyCurrencyMap(mapCurrency, EMallCurrencyType_BindMCoin, peddleGoods.m_nCost - nMoney);
        }
        else
        {
            ModifyCurrencyMap(mapCurrency, EMallCurrencyType_BindMCoin, peddleGoods.m_nCost);
        }
        break;
    case eMallPayType_Money:
        ModifyCurrencyMap(mapCurrency, EMallCurrencyType_Money, peddleGoods.m_nCost);
        break;
    case eMallPayType_BindMCoin:
        ModifyCurrencyMap(mapCurrency, EMallCurrencyType_BindMCoin, peddleGoods.m_nCost);
        break;
    default:
        return MALL_PARAM_ERROR;
        break;
    }

    return MALL_OK;
}

bool CMall::IsValidCost(const MallGood &mallGoods, int nSelIndex, unsigned int nCost, int nDiscount)
{
    switch (nSelIndex)
    {
    case 0:
        return (mallGoods.m_nPrice != 0 && mallGoods.m_nPrice*nDiscount / 100 == nCost);
    case 1:
        return (mallGoods.m_nPrice1 != 0 && mallGoods.m_nPrice1*nDiscount / 100 == nCost);
    case 2:
        return (mallGoods.m_nPrice2 != 0 && mallGoods.m_nPrice2*nDiscount / 100 == nCost);
    case 3:
        return (mallGoods.m_nPrice3 != 0 && mallGoods.m_nPrice3*nDiscount / 100 == nCost);
    }

    return false;
}

