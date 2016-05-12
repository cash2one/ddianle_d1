#ifndef _SUPER_MARKET_H_
#define _SUPER_MARKET_H_

#include <vector>
#include <time.h>
#include "MallDefine.h"
#include "../../socket/GameMsg_Processor.h"
#include "../../datastructure/datainfor/XQMap.h"
#include "CurrencyType.h"

using namespace std;

class GameMsg_S2C_MallGoodsInfo;

class CMall: public GameMsg_Processor
{
protected:
    CMall();
    virtual ~CMall();

public:
    static CMall& Instance();
    void Initialize();

    const char * SerializeName();
    void SeriizeMallGoodsInfo(CParamPool & IOBuff);
    MapMallGoods & MallGoods();
    MallGood * FindGoods(itemtype_t id);

    void OnLoadGoodsInfo(QueryBase& pQuery);
    void RegComponentNetMsgMap();
	
	bool TransGoodsPay(unsigned char uPayType, unsigned int nCost, bool bUseCoupon, unsigned char nUseCouponPersent, unsigned int currency[EMallCurrencyType_Max]);
	bool TransGoodsPay(unsigned char uPayType, unsigned int nCost, bool bUseCoupon, unsigned char nUseCouponPersent, CurrencyMap &currencyMap);


public:
	bool IsOpening() const;
    int CanBuy(VecPeddleGoods & vecGoods,MapPeddleGoods & mapGoods,CurrencyMap & mapBills,unsigned char nDestPlayerSexType, unsigned int nIntimacy, bool IsMarried );

	//nDestRoleID != 0 ±Ì æ‘˘ÀÕ
    int InsertMallLog(MapPeddleGoods & mapGoods,CurrencyMap & mapBills,unsigned int nSelfRoleID,unsigned int nDestRoleID = 0,__int64 nItemID = 0);

    GameMsg_S2C_MallGoodsInfo * GetGoodsMsg(unsigned char nType);

    unsigned int OnDesire(itemtype_t nTypeID);

    void OnDesireSync(GameMsg_Base & msg,CSlotPeer & SlotPeer);
    void AddGoodsDesireCount(itemtype_t nTypeID, unsigned int nAddCount);

    bool CanBuyGoodByID(itemtype_t nTypeID);
    bool CanMakeWishByID(itemtype_t nTypeID);

    bool GetGoodsDataListByID(itemtype_t nTypeID,MapMallGoods & aGoodMap);

    int AddBuyGoods(CurrencyMap & mapBills, const MallGood *pMallGood, int nSelIndex, bool bUseCoupon = false);


    MallGood * GetGood(itemtype_t nType);
	bool isBetween(unsigned int time0,unsigned int time1,unsigned int timenow = (unsigned int)time(NULL)){return (time0==0 || time0<=timenow) && (time1==0 || time1>timenow);};

private:
    bool Open();
    void Close();
    void Reload();
    void LoadGoodsInfo();
    void InitGoodsInfo(list<MallGood *> *pList);
    void PacketToBuffer(char ** pTempGood,MallGood * pGood, unsigned short & nCount);
    bool InitMsg();

    void AddGood(MallGood *good);


    int  AddPeddleGood(MapPeddleGoods & mapGoods,CurrencyMap & mapCurrency,PeddleGood & Good,bool bUseCoopon,unsigned char nDestPlayerSexType);
    static bool ModifyCurrencyMap(CurrencyMap & mapBills,unsigned char nPayType,unsigned int nCost);
    int  AddPeddleGood(MapPeddleGoods & mapGoods,CurrencyMap & mapBills,PeddleGood & Good,bool bUseCoopon,unsigned char nDestPlayerSexType,unsigned int nIntimacy, bool IsMarried);

    bool ModifyBillMap(CurrencyMap & mapBills,unsigned char nPayType,unsigned int nCost);

public:
    static void InitCurrencyMap(CurrencyMap & mapBills);
    static bool IsValidCost(const MallGood &rMallGoods, int nSelIndex, unsigned int nCost, int nDiscount);
    static int CalcGoodsCost(const MallGood &rMallGoods, const PeddleGood &Goods, CurrencyMap & mapCurrency);

private:
    void OnMallReload(GameMsg_Base & msg,CSlotPeer & SlotPeer);

private:
    bool             m_bIsOpening;
    bool             m_bInitialized;
    unsigned short   m_nVersion;
    MapMallGoods     m_MapGoods;
    GameMsg_S2C_MallGoodsInfo * m_pGoodsMsg[GOODS_TYPE_MAX];

    unsigned int     m_nNextEventTime;
};


#endif //_SUPER_MARKET_H_

