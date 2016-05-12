#ifndef __ENTITYMALL_COMPONENT_H__
#define __ENTITYMALL_COMPONENT_H__

#include "../logic/RoleComponentImpl.h"
#include "../logic/EntityComponent.h"
#include "MallDefine.h"
#include "DesiredItem.h"

class CEntityNetComponent;
class CEntityItemComponent;
class CEntityMailComponent;
class CEntityFriendComponent;
class CEntityAttributeComponent;
class CEntityFriendComponent;
class CEntityVIPComponent;
class CEntityCoupleComponent;
class CNotificationComponent;
class GameMsg_C2S_MallGoodsInfo;
class CEntityActivityComponent;
class CEntityQuestNewComponent;


class CEntityMallComponent :
	public CEntityComponent
{
public:
	CEntityMallComponent(void);
	~CEntityMallComponent(void);

public:
	virtual void Start();
	virtual bool CreateFromDB(ROLEINFO_DB* pRoleInforDB);
	virtual bool PacketToDB(ROLEINFO_DB *pRoleInforDB) const {return true;};
    virtual bool PacketToCache(ROLEINFO_DB *pRoleInfoCache) const;
    void RegComponentNetMsgMap();
	virtual void SendExtraStaticData();
	virtual void PacketBuff(std::list<BuffData>& listBuff) {}

public:
	//进来的第一步，接受订单
	void OrderBuy(GameMsg_Base & msg,CSlotPeer & SlotPeer);
	void OrderSend(GameMsg_Base & msg,CSlotPeer & SlotPeer);//赠送物品
	int OrderSendFromDivorec( VecPeddleGoods & vecGoods, unsigned int nDestRoleID, unsigned char cSex);
	void OnRenewItem(GameMsg_Base & msg,CSlotPeer & SlotPeer);
	void OrderAskFor(GameMsg_Base & msg,CSlotPeer & SlotPeer);
	void OnBeAskedFor(GameMsg_Base & msg,CSlotPeer & SlotPeer);
	
	void OnBeAskedSend(GameMsg_Base & msg, CSlotPeer & slotPeer);
	int FinishBill(CurrencyMap &mapBills, EFinishBillFrom from);
    
    // 每天第一次进入商城
	void OnFirsrEnterMallOneDay(GameMsg_Base & msg, CSlotPeer & slotPeer);

    // 进入商城
    void OnEnterMall(GameMsg_Base & msg, CSlotPeer & slotPeer);

	int CanBuy(VecPeddleGoods & vecGoods,MapPeddleGoods & mapGoods,CurrencyMap & mapBills, bool bCheckPackage=true, bool bCheckBill=true, int nDiscount=100, unsigned char nDestPlayerSexType=ESexType_No);
	int Buy(MapPeddleGoods & mapGoods,CurrencyMap & mapBills);
	int CanBill(CurrencyMap & mapBills/*需要支付的货币*/, bool bGiveToFriend = false/*是否赠送给好友，默认不赠送*/);

	int CanRenew(VecPeddleGoods & vecGoods,MapPeddleGoods & mapGoods,CurrencyMap & mapBills,bool bFirstUseChange,unsigned short nColumn,unsigned short nIndex);
	//续费，目前该功能已经取消
	int Renew(MapPeddleGoods & mapGoods,CurrencyMap & mapBills,unsigned short nColumn,unsigned short nIndex);

	int CanSend(VecPeddleGoods & vecGoods,MapPeddleGoods & mapGoods,CurrencyMap & mapBills,unsigned int nDestRoleID);
	int CanSendFromDivorce(VecPeddleGoods & vecGoods,MapPeddleGoods & mapGoods,CurrencyMap & mapBills,unsigned int nDestRoleID,unsigned char cDestSex);
	int Send(MapPeddleGoods & mapGoods,CurrencyMap & mapBills,unsigned int nDestRoleID,string strInfo, CRoleSendLog::ESendType eSendType);
	
	int CanBuyByExpCards(VecPeddleGoods & vecGoods,MapPeddleGoods & mapGoods, unsigned char nDestPlayerSexType=ESexType_No);

public:
    void OnDesire(GameMsg_Base & msg, CSlotPeer & SlotPeer);
    void OnRemoveDesire(GameMsg_Base & msg, CSlotPeer & SlotPeer);
    void OnSendWishItem(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnRequestDesireList(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnGetDesiredItem(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void GetDesireList(std::list<itemtype_t> & listDesire) const;
    void SaveDesiredItemToDB(itemtype_t nItemType);
    void RemoveDesiredItemToDB(itemtype_t nItemType);

public:
	void GetGoodsInfo(GameMsg_Base & msg , CSlotPeer & SlotPeer);
	void GetSomeGoods(GameMsg_Base & msg,CSlotPeer & SlotPeer);
	//成功就返回MALL_OK
	static int GetCountOrValidTimeByGoodSelIndex(itemtype_t nItemType,unsigned char nSelIndex, unsigned short& nCount, int& nValidTime);
	static bool GetCItemByPeddleGood(const PeddleGood& good, CItem& item);
	static void GetMallBuyErrorString(int nErrno, string &StrError);
	static int GetValidTimeByGoodSelIndex(itemtype_t nItemType,unsigned char nSelIndex);

private:
    // special mall
    void OnSpecialMallUpdateMsg(GameMsg_Base &msg, CSlotPeer &SlotPeer);
    void OnSpecialMallMsgRes(GameMsg_Base &msg, CSlotPeer &SlotPeer);
    void OnSpecialMallPay(GameMsg_Base &msg, CSlotPeer &SlotPeer);

    void OnGetSpecialMallState(GameMsg_Base &msg, CSlotPeer &SlotPeer);

    void OnGroupBuyingGetItemsRes(GameMsg_Base &msg, CSlotPeer &SlotPeer);
    void OnGroupBuyingBuyMsg(GameMsg_Base &msg, CSlotPeer &SlotPeer);

    void OnLimitedEditionGetItemsRes(GameMsg_Base &msg, CSlotPeer &SlotPeer);
    void OnLimitedEditionBuyMsg(GameMsg_Base &msg, CSlotPeer &SlotPeer);

    void OnAuctionGetItemsRes(GameMsg_Base &msg, CSlotPeer &SlotPeer);
    void OnAuctionBidMsg(GameMsg_Base &msg, CSlotPeer &SlotPeer);
    void OnAuctionBidRes(GameMsg_Base &msg, CSlotPeer &SlotPeer);
    void OnAuctionSurpassed(GameMsg_Base &msg, CSlotPeer &SlotPeer);
    void OnAuctionUpdateState(GameMsg_Base &msg, CSlotPeer &SlotPeer);
    void OnAuctionBidSuccess(GameMsg_Base &msg, CSlotPeer &SlotPeer);
	void OnAuctionCheckTopPriceResult(GameMsg_Base &msg, CSlotPeer &SlotPeer);
	void OnExperienceCardInfo(GameMsg_Base &msg, CSlotPeer &SlotPeer);

    bool CheckSpecialMallAlreadyBuy(unsigned char nSpecialMallType, unsigned short idx, itemtype_t uGoodsID);
    bool CheckSpecialMallForBuy(GameMsg_Base &msg, unsigned char nSpecialMallType, unsigned short idx, itemtype_t uGoodsID);
    bool CheckForAuctionBid(GameMsg_Base &msg, unsigned short idx, itemtype_t uGoodsID, unsigned int price);
    void SendSpecialMallErrorBuyRes(unsigned char nSpecialMallType, unsigned char errorType, const CItem &item);
    void AuctionReturnPoint(long long nLogID, unsigned short idx, unsigned int nEndTime, unsigned int price);

    void QuerySaveSpecialMall(unsigned short idx, const CItem &item, unsigned char buyType, unsigned int buyPrice, CurrencyMap &mapBills, time_t tEndTime, ESpecialMallType eType);
    void QuerySaveAuctionBid(unsigned short idx, const CItem &item, unsigned int bidPrice, time_t tEndTime);

    void OnDBAuctionReturnPoint(QueryBase &rQuery);

private:
	int CanAddItem(MapPeddleGoods & mapGoods);
	int CanMailItem(MapPeddleGoods & mapGoods,unsigned int nDestRoleID);

	int AddItem(MapPeddleGoods & mapGoods);
	int OnMailItem(MapPeddleGoods & mapGoods,unsigned int nDestRoleID,string strInfo, CRoleSendLog::ESendType eSendType);

	// 检测是否可以购买的类型
	bool IsValidExperiencedItemType(itemtype_t itemType);
	// 
	bool HasExperiencedItem(itemtype_t itemType);
	// if has enough experienced times.
	bool HasExperiencedTimes(unsigned short experienceNum);
	// 
	bool ReduceExperienceTimes(const VecPeddleGoods &experienceGoods);

	void ResetExperienceCardInfo(time_t now);
	void NotifyNewExperiencedItem(std::vector<itemtype_t> &items);

    void AfterBuyGoods(const PeddleGood &goods);
    int GetPeddleGoodCount(const PeddleGood &goods);

private:
	void SendReturnMsg(unsigned char nErrorType,unsigned char nBuyFromType,unsigned char nType);
	virtual void OnEvent( CComponentEvent& refEvent ){};
	virtual void OnUpdate(const unsigned long &nTimeElapsed) {};
	virtual void SerializeComponent(CParamPool &IOBuff) {};
	virtual const char* SerializeName(){ return "Mall"; };
	virtual void OnLogin();
	virtual void OnLogout(){};
	void ProcessBeAskedFor(unsigned int nAskForRoleID,list<CAskMallGoodInfo> &listAskGoodInfo,const char * szAskForRoleName,string strInfo);

private:
	static unsigned short GetCountByGoodSelIndex(itemtype_t nItemType,unsigned char nSelIndex);
	
	void SetGoodsAddUseValue(MapPeddleGoods & mapGoods,bool nAddUse);

    bool CanBuyBadageCard(itemtype_t itemType);

	// 赠送log
	void _LogRoleSend(unsigned int nRoleID, CRoleSendLog::ESendType eSendType, itemtype_t nItemType, unsigned short nItemCount,
		int nDuration, unsigned int nTargetRoleID, __int64 nMailID);

    // 商城日志
    void InsertActionLog(CRoleMallLog::EMallAction eAction);

private:
	CEntityItemComponent		*  m_pRoleItem;
	CEntityAttributeComponent	*  m_pRoleAttr;
	CEntityMailComponent		*  m_pRoleMail;
	CEntityFriendComponent		*  m_pRoleFriend;
	CEntityNetComponent			*  m_pNetCpt;
	CEntityVIPComponent			*  m_pRoleVIP;
	CEntityCoupleComponent      *  m_pRoleCouple;
	CEntityActivityComponent    *  m_pRoleActivity;
	CNotificationComponent      *  m_pNotification;
    CEntityQuestNewComponent    *  m_pRoleQuest;

private:
	bool IsTimeOK();
    bool CheckSpecialMallUpdateTime();
    bool CheckSpecialMallBuyTime();
    bool CheckSpecialMallBidTime();

private:
	unsigned int m_nLastRequestTime;
    unsigned int m_nLastSpecialMallUpdateTime;
    unsigned int m_nLastSpecialMallBuyTime;
    unsigned int m_nLastSpecialMallBidTime;
    unsigned int m_nSendItemCostBill;
    unsigned int m_nSendItemTime;

    bool         m_bAuctionIsOpen;
    time_t       m_tAuctionEndTime;
    std::map<unsigned short, unsigned int>  m_mapLimitedEditionBuy;  // id, end time
    std::map<unsigned short, AuctionState>  m_mapAuctionData;        // id, max bid price, time out, item
    std::map<long long, CAuctionNotGivePointInfo> m_mapTopPrice; // logID -> Info

	std::set<itemtype_t> m_setExperienceCardBuyedProduct;				// experienced product.
	unsigned short m_experienceCardUsedNum;							// experiencedCard used times
	unsigned int m_experienceCardUsedStamp;							// stamp

    DesiredItem         m_DesiredItem;                              //  list of desired item;
	
};

typedef CRoleComponentImpl<CEntityMallComponent,CGetRoleProcessorObj,CGetRoleProcessorObj> CRoleMall;

#endif //__ENTITYMALL_COMPONENT_H__


//end file


