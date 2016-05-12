#ifndef _MALL_DEFINE_H_
#define _MALL_DEFINE_H_

#include <map>
#include "../../datastructure/DataStruct_DB.h"

enum eMallPayType
{
	eMallPayType_MCoin             = 1, // 只能M币支付
	eMallPayType_FixedRatioMoney   = 2, // M币、绑定M币和金券支付，金券最高使用比例是贩卖列表静态数据配置，该种支付方式时，不享受折扣
	eMallPayType_Money             = 3, // M币、绑定M币和金券任意支付，优先使用金券，金券不足用绑定M币补充，再不足，使用M币补充
	eMallPayType_BindMCoin         = 4, // M币和绑定M币任意支付，优先使用绑定M币，绑定M币不足时，使用M币
};


//总共8中类型
enum MALL_GOODS_TOTAL_TYPE
{
	GOODS_TYPE_BEGIN = -1,     
	GOODS_TYPE_ALL,			   //所有
	GOODS_TYPE_HOT,			   //热卖
	GOODS_TYPE_CLOTH,	       //服饰
	GOODS_TYPE_EXPENDABLE,	   //道具
	GOODS_TYPE_BADGE,	       //徽章

	GOODS_TYPE_MAX
};

enum MALL_GOODS_TOTAL_MIN_TYPE
{
	GOODS_MIN_TYPE_BEGIN = -1,     
	GOODS_MIN_TYPE_0,	       //新品
	GOODS_MIN_TYPE_1,	       //人气
	GOODS_MIN_TYPE_2,	       //抢购
	GOODS_MIN_TYPE_3,	       //新手

	GOODS_MIN_TYPE_MAX
};

enum MALL_GOODS_BUYFROM_TYPE
{
	GOODS_BUYFROM_TYPE_BEGIN = -1,     
	GOODS_BUYFROM_TYPE_SHOPCART,	       //商城购物车
	GOODS_BUYFROM_TYPE_SAVEMOULD,	       //商城点击保存形象的购买
	GOODS_BUYFROM_TYPE_QUICKBILL,	       //快速支付购买但不立即使用
	GOODS_BUYFROM_TYPE_QUICKBILLANDUSED,   //快速购买并立即使用

	GOODS_BUYFROM_TYPE_MAX
};


enum MALL_GOODS_BUY_TYPE
{
	MALL_GOODS_BUY_TYPE_BEGIN = -1,     
	MALL_GOODS_BUY_TYPE_BUY,				//商城购物车
	MALL_GOODS_BUY_TYPE_SEND,				//赠送
	MALL_GOODS_BUY_TYPE_RENEW,				//续费
	MALL_GOODS_BUY_TYPE_BEASKED_SEND,		//被索要赠送	

	MALL_GOODS_BUY_TYPE_MAX
};


enum MallReturnType
{
	MALL_OK = 0,					//OK
	MALL_PARAM_ERROR,				//客户端传送的购买参数错误
	MALL_NOTOPEN,					//商城没开放
	MALL_NOTSELL,					//不贩卖该商品
	MALL_BILL_CASH_NOTENOUGH,		//人民币不够
	MALL_BILL_CHANGE_NOTENOUGH,		//抵用券不够
	MALL_BILL_MONEY_NOTENOUGH,		//金币不够
	MALL_BILL_INCONFORMITY,			//客户端计算的支付金额与服务器不一致
	MALL_PAYTYPE_INCONFORMITY,		//该商品不支持该支付类型
	MALL_ITEMLIST_EMPTY,			//购买列表为空
	MALL_ITEMGRID_NOTENOUGH,		//物品栏空格不够
	MALL_ITEM_UNIQUEEXIST,			//唯一性物品数量不能购买两个及以上
	MALL_ITEM_NOTRENEW,				//该商品不能续费
	MALL_ITEM_MAILTOOMUCH,			//邮寄的物品太多
	MALL_ITEM_COUNTERROR,			//允许购买的数量有错误
	MALL_ITEM_SEXERROR,				//购买物品的性别有错误
	MALL_ITEM_FRIENDERROR,			//不是好友不能送
	MALL_ITEM_SAMEERROR,			//同时购买了多个相同的物品
	MALL_ITEM_CANNOTBUYBADGECARD,	//无法购买该徽章卡

	MALL_WISH_WISHLISTFULL,			//愿望栏达到最大上限
	MALL_WISH_HASPERMANENTITEM,		//已经拥有该永久物品
	MALL_OPERATE_TOO_OFTEN,			//操作太频繁
	MALL_CAN_NOT_BUY_PRECIOUS_ITEMS,//无法购买珍藏版物品
	MALL_CAN_NOT_MAKE_WISH,			//物品无法许愿
	MALL_INTIMACY_NOTENOUGH,        //婚戒购买亲密度不够
	MALL_NOT_MARRID,                //婚戒购买没结婚
	MALL_UNKNOWN,

	MALL_CAN_NOT_USE_EXPERIENCECARD,	//不能使用体验卡
	MALL_EXPERIENCECARD_TYPE_REFUSED,	//体验卡不支持的类型
	MALL_EXPERIENCECARD_DUPLICATE,		//体验卡已经购买过的物品
	MALL_EXPERIENCECARD_MISSED,			//没有体验卡
	MALL_EXPERIENCECARD_TIMES_LIMIT,	//体验卡超出每天的可体验次数
    MALL_WEDDINGRING_NOT_SEND,          //婚戒不能赠送

	MALL_MAX
};

enum EFinishBillFrom
{
	EFinishBillFrom_None,
	EFinishBillFrom_Buy,
	EFinishBillFrom_Renew,
	EFinishBillFrom_Send,
    EFinishBillFrom_Egg,
};

// 商城购买物品所选择的支付参数
enum EPeddleGoodSel
{
	EPeddleGoodSel_0 = 0,
	EPeddleGoodSel_1,
	EPeddleGoodSel_2,
	EPeddleGoodSel_3,

	EPeddleGoodSel_Experience = 100,			// 体验
};


//客户端传来的单个物品购买信息
struct PeddleGood
{
	itemtype_t      m_nType;		//物品类型
	unsigned char   m_nPayType;		//支付类型
	unsigned char   m_nSelIndex;	//选择的索引, 取值范围[0-3] 0: 1天时效装备或者1个道具 1: 7天时效装备或者5个道具 以此类推
	unsigned int    m_nCost;		//支付金额
	bool			m_bUseCoupon;	//是否使用优惠券
	bool			m_bIsAddUse;	//是否购买后立即使用，客户端无此字段

	PeddleGood()
	{
		m_nType = 0;
		m_nPayType = 0;
		m_nSelIndex = 0;
		m_nCost = 0;
		m_bUseCoupon = false;
		m_bIsAddUse = false;
	}

	PeddleGood& operator = (const PeddleGood& rhs)
	{
		if (this == &rhs)
			return *this;

		m_nType = rhs.m_nType;
		m_nPayType = rhs.m_nPayType;
		m_nSelIndex = rhs.m_nSelIndex;
		m_nCost = rhs.m_nCost;
		m_bUseCoupon = rhs.m_bUseCoupon;
		m_bIsAddUse = rhs.m_bIsAddUse;
		return *this;
	}

	bool doEncode(CParamPool &IOBuff)
	{
		IOBuff.AddUInt(m_nType);
		IOBuff.AddUChar(m_nPayType);
		IOBuff.AddUChar(m_nSelIndex);
		IOBuff.AddUInt(m_nCost);
		IOBuff.AddBool(m_bUseCoupon);
		return true;
	}

	bool doDecode(CParamPool &IOBuff)
	{
		m_nType = IOBuff.GetUInt();
		m_nPayType = IOBuff.GetUChar();
		m_nSelIndex = IOBuff.GetUChar();
		m_nCost = IOBuff.GetUInt();
		m_bUseCoupon = IOBuff.GetBool();
		return true;
	}
};

struct AuctionState
{
    unsigned int    m_uCurPrice;
    bool            m_bTimeOut;
    CItem           m_cItem;
};


typedef std::vector<PeddleGood> VecPeddleGoods;
typedef std::map<itemtype_t, PeddleGood>  MapPeddleGoods;  // key: item type, value: peddle goods
typedef std::map<unsigned char, unsigned int>  CurrencyMap; // key: currency type, value: currency value
typedef XQMap<itemtype_t,MallGood> MapMallGoods;           // key: item type, value: mall goods

#endif //_MALL_DEFINE_H_


//end file


