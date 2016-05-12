#ifndef __MEDALCONDITIONFACTORY_H__
#define __MEDALCONDITIONFACTORY_H__

#include "MedalCondition.h"


enum EMedalProgressType
{
    EMedalProgressType_None                         = 0,
    EMedalProgressType_NewProductBuy_MB             = 1,        //代表新品区MB消费类型
    EMedalProgressType_NewProductBuy_BindMB         = 2,        //代表新品区绑定MB消费类型
    EMedalProgressType_NewProductBuy_GOLD           = 3,        //代表新品区金劵消费类型
    EMedalProgressType_HotSaleBuy_MB                = 4,        //代表热卖区MB消费类型
    EMedalProgressType_HotSaleBuy_BindMB            = 5,        //代表热卖区绑定MB消费类型
    EMedalProgressType_HotSaleBuy_GOLD              = 6,        //代表热卖区金劵消费类型
    EMedalProgressType_NeedNoteBuy_MB               = 7,        //代表收卷区MB消费类型
    EMedalProgressType_NeedNoteBuy_BindMB           = 8,        //代表收卷区绑定MB消费类型
    EMedalProgressType_NeedNoteBuy_GOLD             = 9,        //代表收卷区金劵消费类型
    EMedalProgressType_VipLevel                     = 10,       //代表VIP等级类型
    EMedalProgressType_BuyItem                      = 11,       //代表购买的物品类型
    EMedalProgressType_BuyRank                      = 12,       //代表购物狂排行榜名次类型
    EMedalProgressType_TrumpetRank                  = 13,       //代表喇叭王排行榜名次类型
    EMedalProgressType_LevelRank                    = 14,       //代表升级达人排行榜名次类型
    EMedalProgressType_HonourDanceGroupRank         = 15,       //代表荣誉舞团排行榜名次类型
    EMedalProgressType_ManWifeRank                  = 16,       //代表明星夫妻排行榜名次类型
    EMedalProgressType_Level                        = 17,       //代表等级类型
    EMedalProgressType_HaveItemType                 = 18,       //代表已拥有的物品类型
    EMedalProgressType_HaveEffect                   = 19,       //代表光效分离
    EMedalProgressType_MedalScore                   = 20,       //代表勋章积分
    EMedalProgressType_ClothEffect                  = 21,       //代表闪亮值
    EMedalProgressType_EquipCount                   = 22,       //代表收藏总数
    EMedalProgressType_MedalScoreRank               = 23,       //代表勋章积分排行榜名次类型
    EMedalProgressType_ClothEffectRank              = 24,       //代表闪亮值排行榜的名次类型
    EMedalProgressType_EquipCountRank               = 25,       //代表收藏总数排行榜的名次类型
};

class CMedalConditionFactory
{
public:
    static IMedalCondition* CreateMedalCondition(int nType, IEntity *owner);
};

#endif //__MEDALCONDITIONFACTORY_H__

