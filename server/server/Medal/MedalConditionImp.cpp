#include "MedalConditionImp.h"
#include "../vip/EntityVIPComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../item/EntityItemComponent.h"
#include "../Medal/MedalCfgMgr.h"
#include "../Medal/EntityMedalComponent.h"

#include <stdlib.h>

CMedalConditionVip::CMedalConditionVip(IEntity *owner)
    : IMedalCondition(owner)
{
}

bool CMedalConditionVip::CheckIsAllowGet(int nTotalProgress)
{
    CEntityVIPComponent *pRoleVip = ((CEntity *)m_pOwner)->GetComponent<CRoleVIP>();
    if (NULL == pRoleVip)
    {
        return false;
    }

    if (!pRoleVip->IsVIP() )
    {
        return false;
    }

    return (GetCurProgress() >= nTotalProgress && GetAreadyReward() == 0 );
}

bool CMedalConditionVip::process(IParam  &param)
{
    int nCurVipLevel = 0;
    if (!param.GetParam("vip_level", nCurVipLevel))
    {
        return false;
    }

    CEntityVIPComponent *pRoleVip = ((CEntity *)m_pOwner)->GetComponent<CRoleVIP>();
    if (pRoleVip == NULL)
        return false;

    if (pRoleVip->m_VIPInfo.m_bValid == false)
    {
        return false;
    }

    SetCurProgress(nCurVipLevel);
    return true;
}

bool CMedalConditionVip::check()
{
    if (m_pOwner == NULL)
        return false;

    CEntityVIPComponent *pRoleVip = ((CEntity *)m_pOwner)->GetComponent<CRoleVIP>();
    if (pRoleVip == NULL)
        return false;

    if (pRoleVip->m_VIPInfo.m_bValid == false)
    {
        return false;
    }

    SetCurProgress(pRoleVip->VIPLevel());
    return false;
}
//////////////////////////////////////////////////////////////////////////
CMedalConditionLevel::CMedalConditionLevel(IEntity *owner)
    : IMedalCondition(owner)
{
}

bool CMedalConditionLevel::process(IParam  &param)
{
    int nCurLevel = 0;
    if (!param.GetParam("level", nCurLevel))
    {
        return false;
    }

    SetCurProgress(nCurLevel);
    return true;
}

bool CMedalConditionLevel::check()
{
    if (m_pOwner == NULL)
        return false;

    CEntityAttributeComponent *pRoleAttr = ((CEntity *)m_pOwner)->GetComponent<CRoleAttribute>();
    if (pRoleAttr == NULL)
        return false;

    SetCurProgress(pRoleAttr->GetLevel());
    return false;
}

//////////////////////////////////////////////////////////////////////////
CMedalConditionMallConsume ::CMedalConditionMallConsume(int nConsumeType, IEntity *owner)
    : IMedalCondition(owner)
    , m_nConsumeType(nConsumeType)
{
}

bool CMedalConditionMallConsume::process(IParam &param)
{
    bool bGotParam = false;

    int nInTargetZone = 0;
    int nCost = 0;
    int nItemType = 0;

    const char *strIsNewKey = "is_new";
    const char *strIsHotKey = "is_hot";
    const char *strUseCouponKey = "use_coupon";
    const char *strMCoinKey = "mcoin";
    const char *strBindMCoinKey = "bind_mcoin";
    const char *strMoneyKey = "money";

    switch (m_nConsumeType)
    {
    case eMallConsumeType_NewMCoin:
        bGotParam = param.GetParam(strIsNewKey, nInTargetZone);
        bGotParam = bGotParam && nInTargetZone && param.GetParam(strMCoinKey, nCost);
        break;
    case eMallConsumeType_NewBindMCoin:
        bGotParam = param.GetParam(strIsNewKey, nInTargetZone);
        bGotParam = bGotParam && nInTargetZone && param.GetParam(strBindMCoinKey, nCost);
        break;
    case eMallConsumeType_NewMoney:
        bGotParam = param.GetParam(strIsNewKey, nInTargetZone);
        bGotParam = bGotParam && nInTargetZone && param.GetParam(strMoneyKey, nCost);
        break;
    case eMallConsumeType_HotMCoin:
        bGotParam = param.GetParam(strIsHotKey, nInTargetZone);
        bGotParam = bGotParam && nInTargetZone && param.GetParam(strMCoinKey, nCost);
        break;
    case eMallConsumeType_HotBindMCoin:
        bGotParam = param.GetParam(strIsHotKey, nInTargetZone);
        bGotParam = bGotParam && nInTargetZone && param.GetParam(strBindMCoinKey, nCost);
        break;
    case eMallConsumeType_HotMoney:
        bGotParam = param.GetParam(strIsHotKey, nInTargetZone);
        bGotParam = bGotParam && nInTargetZone && param.GetParam(strMoneyKey, nCost);
        break;
    case eMallConsumeType_CouponMCoin:
        bGotParam = param.GetParam(strUseCouponKey, nInTargetZone);
        bGotParam = bGotParam && nInTargetZone && param.GetParam(strMCoinKey, nCost);
        break;
    case eMallConsumeType_CouponBindMCoin:
        bGotParam = param.GetParam(strUseCouponKey, nInTargetZone);
        bGotParam = bGotParam && nInTargetZone && param.GetParam(strBindMCoinKey, nCost);
        break;
    case eMallConsumeType_CouponMoney:
        bGotParam = param.GetParam(strUseCouponKey, nInTargetZone);
        bGotParam = bGotParam && nInTargetZone && param.GetParam(strMoneyKey, nCost);
        break;
    case eMallConsumeType_BuyItem:
        bGotParam = param.GetParam("item_type_form_mall", nItemType);
        break;
    default:
        return false;
    }

    if (!bGotParam)
    {
        return false;
    }

    if (m_nConsumeType > eMallConsumeType_Invalid && m_nConsumeType < eMallConsumeType_BuyItem)
    {
        SetCurProgress(GetCurProgress() + nCost);
    }
    else if (eMallConsumeType_BuyItem == m_nConsumeType)
    {
        std::list<unsigned int> idlist;
        CMedalCfgMgr::Instance().GetSpecialParams(GetConditionID() ,idlist);
        
        std::list <unsigned int >findlist;
        findlist.push_back(nItemType);
        int nCount = GetContainCount(idlist, findlist);
        if (nCount > 0)//购买的道具 是需要检测的道具
        {
            int buycount = 0;
            param.GetParam("item_count_form_mall", buycount);
            SetCurProgress(GetCurProgress() + buycount);
        }
        return true;
    }

    return false;
}

bool CMedalConditionMallConsume::check()
{
    return true;
}

void CMedalConditionMallConsume::SetConsumeType(int nConsumeType)
{
    m_nConsumeType = nConsumeType;
}

int CMedalConditionMallConsume::GetContainCount(std::list<unsigned int> dest, std::list<unsigned int> findlist)
{
    int count = 0;
    for (std::list<unsigned int>::const_iterator findit = findlist.begin();
        findlist.end() != findit ; findit++)
    {
        for (std::list<unsigned int>::const_iterator destit = dest.begin();
            dest.end()!= destit ;destit++)
        {
            if (*findit == *destit)
            {
                ++count;
            }
        }
    }

    return count;
;
}

//////////////////////////////////////////////////////////////////////////
CMedalConditionHaveItem::CMedalConditionHaveItem(IEntity *owner)
    : IMedalCondition(owner)
{
}

bool CMedalConditionHaveItem::process(IParam &param)
{
    bool bGotParam = false;
    int nItemType;
    bGotParam = param.GetParam("item_type", nItemType);
    
    if (!bGotParam)
    {
        return false;
    }

    if (m_pOwner == NULL)
        return false;

    CEntityItemComponent *pRoleItem = ((CEntity *)m_pOwner)->GetComponent<CRoleItem>();
    if (pRoleItem == NULL)
        return false;

    std::list<unsigned int> speclist;
    CMedalCfgMgr::Instance().GetSpecialParams(GetConditionID(), speclist);

    int count = 0;
    for(std::list<unsigned int>::iterator it = speclist.begin(); it != speclist.end(); ++it)
    {
        count += pRoleItem->GetItemCount( *it, false, false);
    }

    if (count <= GetCurProgress() )
    {
        return false;
    }

    SetCurProgress(count);
    return true;
}

bool CMedalConditionHaveItem::check()
{
    if (m_pOwner == NULL)
        return false;

    CEntityItemComponent *pRoleItem = ((CEntity *)m_pOwner)->GetComponent<CRoleItem>();
    if (pRoleItem == NULL)
        return false;

    std::list<unsigned int> speclist;
    CMedalCfgMgr::Instance().GetSpecialParams(GetConditionID(), speclist);

    int count = 0;
    for(std::list<unsigned int>::iterator it = speclist.begin(); it != speclist.end(); ++it)
    {
        count += pRoleItem->GetItemCount( *it, false, false);
    }

    if (count <= GetCurProgress() )
    {
        return false;
    }
    SetCurProgress(count);
    return true;
}
//////////////////////////////////////////////////////////////////////////
CMedalConditionHaveEffect::CMedalConditionHaveEffect(IEntity *owner)
    :IMedalCondition(owner)
{
}

bool CMedalConditionHaveEffect::process(IParam &param)
{

    bool bGotParam = false;
    int nEffectType = 0;
    bGotParam = param.GetParam("effect_change", nEffectType);

    if (!bGotParam)
    {
        return false;
    }


    if (m_pOwner == NULL)
        return false;

    CEntityItemComponent *pRoleItem = ((CEntity *)m_pOwner)->GetComponent<CRoleItem>();
    if (pRoleItem == NULL)
        return false;

    std::list<unsigned int> speclist;
    CMedalCfgMgr::Instance().GetSpecialParams(GetConditionID(), speclist);

    list<unsigned int> findlist;
    pRoleItem->CheckEffectListExist(speclist, findlist);

    if (findlist.size() > (size_t)GetCurProgress())
    {
        SetCurProgress(findlist.size() );
    }

    return true;
}

bool CMedalConditionHaveEffect::check()
{
    if (m_pOwner == NULL)
        return false;

    CEntityItemComponent *pRoleItem = ((CEntity *)m_pOwner)->GetComponent<CRoleItem>();
    if (pRoleItem == NULL)
        return false;

    std::list<unsigned int> speclist;
    CMedalCfgMgr::Instance().GetSpecialParams(GetConditionID(), speclist);

    list<unsigned int> findlist;
    pRoleItem->CheckEffectListExist(speclist, findlist);

    if (findlist.size() > (size_t)GetCurProgress())
    {
        SetCurProgress(findlist.size() );
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////

CMedalConditionRank::CMedalConditionRank(ERankDataType eRankType, IEntity *owner)
    : IMedalCondition( owner )
    , m_eRankType(eRankType)
{
}

bool CMedalConditionRank::process(IParam &param)
{
    bool bGotParam = false;

    int nRank = 0;

    const char *strIsMCoin = "mcoinrank";
    const char *strIsExp = "exprank";
    const char *strIsHorn = "hornrank";
    const char *strIsGrouhonor = "grouhonorrank";
    const char *strIsIntimacy = "intimacyrank";
    const char *strIsMedal = "medalrank";
    const char *strIsClothEffect = "clotheffectrank";
    const char *strIsEquipCount = "equipcountrank";

    switch (m_eRankType)
    {
    case ERankDataType_MCoin:
        bGotParam = param.GetParam(strIsMCoin, nRank);
        break;
    case ERankDataType_Horn:
        bGotParam = param.GetParam(strIsHorn, nRank);
        break;
    case ERankDataType_Exp:
        bGotParam = param.GetParam(strIsExp, nRank);
        break;
    case ERankDataType_Group_Honor:
        bGotParam = param.GetParam(strIsGrouhonor, nRank);
        break;
    case ERankDataType_Intimacy:
        bGotParam = param.GetParam(strIsIntimacy, nRank);
        break;
    case ERankDataType_Medal:
        bGotParam = param.GetParam(strIsMedal, nRank);
        break;
    case ERankDataType_ClothEffect:
        bGotParam = param.GetParam(strIsClothEffect, nRank);
        break;
    case ERankDataType_Cloths:
        bGotParam = param.GetParam(strIsEquipCount, nRank);
        break;
    case ERankDataType_Max:
        return false;
    default:
        return false;
    }

    if (!bGotParam)
    {
        return false;
    }

    SetCurProgress(nRank);
    return true;
}

bool CMedalConditionRank::check()
{
    return true;
}

bool CMedalConditionRank::CheckIsAllowGet(int nTotalProgress)
{
    if (GetCurProgress() == 0)
    {
        return false;
    }

    return (GetCurProgress() <= nTotalProgress && GetAreadyReward() == 0 );
}

///////////////////////////////////////////////////////////////////////////////////////////

CMedalConditionMedalScore::CMedalConditionMedalScore(IEntity *owner)
    : IMedalCondition(owner)
{

}

bool CMedalConditionMedalScore::process(IParam &param)
{
    int medalScore = 0;
    bool bGotParam = false;
    bGotParam = param.GetParam(std::string("medal_score"), medalScore);
    if (!bGotParam)
    {
        return false;
    }

    SetCurProgress(medalScore);
    return true;
}

bool CMedalConditionMedalScore::check()
{
    if (NULL == m_pOwner)
    {
        return false;
    }

    CEntityMedalComponent *pRoleMedal = ((CEntity*)m_pOwner)->GetComponent<CRoleMedal>();
    if (NULL == pRoleMedal)
    {
        return false;
    }

    int medalScore = pRoleMedal->EncodePlayerMedalScore();
    SetCurProgress(medalScore);
    
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////

CMedalConditionClothEffect::CMedalConditionClothEffect(IEntity *owner)
    : IMedalCondition(owner)
{

}

bool CMedalConditionClothEffect::process(IParam &param)
{
    int clothEffectScore = 0;
    bool bGotParam = false;
    bGotParam = param.GetParam(std::string("cloth_effect"), clothEffectScore);
    if (!bGotParam)
    {
        return false;
    }
  
    SetCurProgress(clothEffectScore);
    return true;
}

bool CMedalConditionClothEffect::check()
{
    if (NULL == m_pOwner)
    {
        return false;
    }

    CEntityItemComponent *pRoleItem = ((CEntity*)m_pOwner)->GetComponent<CRoleItem>();
    if (NULL == pRoleItem)
    {
        return false;
    }

    int clothEffectScore = pRoleItem->GetPerpetualEffectEquipScore();
    SetCurProgress(clothEffectScore);

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////

CMedalConditionEquipCount::CMedalConditionEquipCount(IEntity *owner)
    : IMedalCondition(owner)
{

}

bool CMedalConditionEquipCount::process(IParam &param)
{
    int equipCount = 0;
    bool bGotParam = false;
    bGotParam = param.GetParam(std::string("equip_count"), equipCount);
    if (!bGotParam)
    {
        return false;
    }

    SetCurProgress(equipCount);
    return true;
}

bool CMedalConditionEquipCount::check()
{
    if (NULL == m_pOwner)
    {
        return false;
    }

    CEntityItemComponent *pRoleItem = ((CEntity*)m_pOwner)->GetComponent<CRoleItem>();
    if (NULL == pRoleItem)
    {
        return false;
    }

    int equipCount = pRoleItem->GetAllPerpetualEquipCountNoRing();
    SetCurProgress(equipCount);

    return true;
}
