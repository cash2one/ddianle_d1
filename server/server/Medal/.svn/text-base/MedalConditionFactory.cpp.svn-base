#include "MedalConditionFactory.h"
#include "MedalConditionImp.h"
#include <stdlib.h>

IMedalCondition* CMedalConditionFactory::CreateMedalCondition(int nType, IEntity *owner)
{
    switch (nType)
    {
    case EMedalProgressType_NewProductBuy_MB:
        return new CMedalConditionMallConsume(CMedalConditionMallConsume::eMallConsumeType_NewMCoin, owner);
    case EMedalProgressType_NewProductBuy_BindMB:
        return new CMedalConditionMallConsume(CMedalConditionMallConsume::eMallConsumeType_NewBindMCoin, owner);
    case EMedalProgressType_NewProductBuy_GOLD:
        return new CMedalConditionMallConsume(CMedalConditionMallConsume::eMallConsumeType_NewMoney, owner);
    case EMedalProgressType_HotSaleBuy_MB:
        return new CMedalConditionMallConsume(CMedalConditionMallConsume::eMallConsumeType_HotMCoin, owner);
    case EMedalProgressType_HotSaleBuy_BindMB:
        return new CMedalConditionMallConsume(CMedalConditionMallConsume::eMallConsumeType_HotBindMCoin, owner);
    case EMedalProgressType_HotSaleBuy_GOLD:
        return new CMedalConditionMallConsume(CMedalConditionMallConsume::eMallConsumeType_HotMoney, owner);
    case EMedalProgressType_NeedNoteBuy_MB:
        return new CMedalConditionMallConsume(CMedalConditionMallConsume::eMallConsumeType_CouponMCoin, owner);
    case EMedalProgressType_NeedNoteBuy_BindMB:
        return new CMedalConditionMallConsume(CMedalConditionMallConsume::eMallConsumeType_CouponBindMCoin, owner);
    case EMedalProgressType_NeedNoteBuy_GOLD:
        return new CMedalConditionMallConsume(CMedalConditionMallConsume::eMallConsumeType_CouponMoney, owner);
    case EMedalProgressType_BuyItem:
        return new CMedalConditionMallConsume(CMedalConditionMallConsume::eMallConsumeType_BuyItem, owner);
    case EMedalProgressType_VipLevel:
        return new CMedalConditionVip(owner);
    case EMedalProgressType_Level:
        return new CMedalConditionLevel(owner);
    case EMedalProgressType_HaveItemType:
        return new CMedalConditionHaveItem(owner);
    case EMedalProgressType_BuyRank:
        return new CMedalConditionRank(ERankDataType_MCoin, owner);
    case EMedalProgressType_TrumpetRank:
        return new CMedalConditionRank(ERankDataType_Horn, owner);
    case EMedalProgressType_LevelRank:
        return new CMedalConditionRank(ERankDataType_Exp, owner);
    case EMedalProgressType_HonourDanceGroupRank:
        return new CMedalConditionRank(ERankDataType_Group_Honor, owner);
    case EMedalProgressType_ManWifeRank:
        return new CMedalConditionRank(ERankDataType_Intimacy, owner);
    case EMedalProgressType_HaveEffect:
        return new CMedalConditionHaveEffect(owner);
    case EMedalProgressType_MedalScore:
        return new CMedalConditionMedalScore(owner);
    case EMedalProgressType_ClothEffect:
        return new CMedalConditionClothEffect(owner);
    case EMedalProgressType_EquipCount:
        return new CMedalConditionEquipCount(owner);
    case EMedalProgressType_MedalScoreRank:
        return new CMedalConditionRank(ERankDataType_Medal, owner);
    case EMedalProgressType_ClothEffectRank:
        return new CMedalConditionRank(ERankDataType_ClothEffect, owner);
    case EMedalProgressType_EquipCountRank:
        return new CMedalConditionRank(ERankDataType_Cloths, owner);
        // TODO: ≈≈––∞Ò

    default:
        break;
    }

    return NULL;
}

