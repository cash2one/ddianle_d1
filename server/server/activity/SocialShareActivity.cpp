#include "SocialShareActivity.h"
#include "GameMsg_Activity.h"
#include "MallShareMgr.h"
#include "../logic/LogicCirculator.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "EntityActivityComponent.h"
#include "../util/Crontab.h"
#include "ActivityDataStruct.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../item/ItemProcess.h"
#include "../PhotoRoom/PhotoRoomMacroDefine.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../mall/Mall.h"

CSocialShareActivityShow::CSocialShareActivityShow(EActivity eActivity)
{
	SetActivityType(eActivity);
}

CSocialShareActivityShow::~CSocialShareActivityShow()
{

}

void CSocialShareActivityShow::Init( const char* strBeginTimeShow, const char* strEndTimeShow )
{
	CCrontabActivity::Init(strBeginTimeShow, strEndTimeShow);
}

void CSocialShareActivityShow::OnActivityBegin()
{
	if ( m_nActivityType == EActivity_MallSocialShare )
	{
		CMallSocialShareMgr::Instance().SetIsOpeningShow(true);
	}
}

void CSocialShareActivityShow::OnActivityEnd()
{
	if ( m_nActivityType == EActivity_MallSocialShare )
	{
		CMallSocialShareMgr::Instance().SetIsOpeningShow(false);
	}
}

void CSocialShareActivityShow::UpdateStateToClient( CEntityNetComponent & rNetCom )
{
	
}

//////////////////////////////////////////////////////////////////////////

#define MAX_ELAPSED_TIME    3

CSocialShareActivity::CSocialShareActivity(EActivity eActivity)
{
    m_nActivityType = eActivity;
    m_pData = NULL;
    m_tLastUpdateTime = time(NULL);
}

void CSocialShareActivity::Init(time_t beginShowTime, time_t endShowTime)
{
    if (NULL != m_pData)
    {
        m_pData->m_nActivityType = (unsigned char)m_nActivityType;

        CMonthWeekDayActivity::Init(m_pData->m_nBeginTime, m_pData->m_nEndTime, beginShowTime, endShowTime);
    }
}

void CSocialShareActivity::SetRewardItemInfo(const CRewardTitleInfo *pItemInfo)
{
    if (NULL != m_pData && NULL != pItemInfo)
    {
        CRewardTitleInfo *pInfo = new CRewardTitleInfo(*pItemInfo);

        m_pData->m_listReward.push_back(pInfo);
    }
}

void CSocialShareActivity::OnActivityBegin()
{
    OnActivityChange(true);
}

void CSocialShareActivity::OnActivityEnd()
{
    OnActivityChange(false);
}

const CSocialShareActivityData* CSocialShareActivity::GetActivityData()
{
    if (NULL != m_pData)
    {
        m_pData->m_bIsOpen = IsOpenNow();
        m_pData->m_bIsShow = IsShowing();
    }

    return m_pData;
}

void CSocialShareActivity::ProcessActivityMsg(CRoleEntity *, CEntityActivityComponent *, GameMsg_C2S_ActivitySocialShare &)
{
    LOG_WARNING << "CSocialShareActivity warning: Need implement ProcessActivityMsg()" << endl;
}

void CSocialShareActivity::ResendVIPExtraReward(unsigned char nOpenVIPFromType, CRoleEntity *pRole, CEntityActivityComponent *pActivityComponent)
{
    WriteLog(LOGLEVEL_WARNING, "CSocialShareActivity warning: Need implement ResendVIPReward()");
}

void CSocialShareActivity::OnActivityChange(bool bBegin)
{
    time_t now = time(NULL);

    if (now > m_tLastUpdateTime + MAX_ELAPSED_TIME)
    {
        CPlayerManager::Instance().OnActivityChange((unsigned char)m_nActivityType, bBegin);

        m_tLastUpdateTime = now;
    }
}

//////////////////////////////////////////////////////////////////////////

CMallSocialShareActivity::CMallSocialShareActivity(const CMallSocialShareActivityData &rData, time_t beginShowTime, time_t endShowTime)
    : CSocialShareActivity(EActivity_MallSocialShare)
{
    m_pData = new CMallSocialShareActivityData(rData);

    CSocialShareActivity::Init(beginShowTime, endShowTime);
}

CMallSocialShareActivity::CMallSocialShareActivity(const CMallSocialShareActivity &ref)
    : CSocialShareActivity(EActivity_MallSocialShare)
{
    CMallSocialShareActivityData *pData = dynamic_cast<CMallSocialShareActivityData *>(ref.m_pData);

    if (NULL != pData)
    {
        m_pData = new CMallSocialShareActivityData(*pData);

        CSocialShareActivity::Init(ref.m_nBeginShowTime, ref.m_nEndShowTime);
    }
}

CMallSocialShareActivity::~CMallSocialShareActivity()
{
    delete m_pData;
    m_pData = NULL;
}

void CMallSocialShareActivity::SetRewardItemInfo(const CRewardTitleInfo *pItemInfo)
{
    if (NULL == m_pData || NULL == pItemInfo)
        return;

    const CStringRewardInfo *pStringInfo = dynamic_cast<CStringRewardInfo *>(const_cast<CRewardTitleInfo *>(pItemInfo));

    if (NULL == pStringInfo)
        return;

    CStringRewardInfo *pInfo = new CStringRewardInfo(*pStringInfo);

    m_pData->m_listReward.push_back(pInfo);
}

void CMallSocialShareActivity::ProcessActivityMsg(CRoleEntity *pRole, CEntityActivityComponent *pActivityComponent, GameMsg_C2S_ActivitySocialShare &msg)
{
    CMallSocialShareActivityData *pActivityData = dynamic_cast<CMallSocialShareActivityData *>(m_pData);

    if (NULL == pRole || NULL == pActivityComponent || NULL == pActivityData)
        return;

    CEntityNetComponent *pNet = pActivityComponent->GetComponent<CRoleNet>();

    if (pNet && IsOpenNow() && !pActivityComponent->IsSharedToSocial((EUISocialShare)msg.m_nUI) )
    {
        CMallSocialShareData *pShareData = dynamic_cast<CMallSocialShareData *>(msg.m_pData);

        if (NULL == pShareData)
            return;

        std::list<CItem> listItems;
        bool bHasOppositeSex = false;

        for (std::list<itemtype_t>::iterator bItr = pShareData->m_listTryCloth.begin(), eItr = pShareData->m_listTryCloth.end(); 
             bItr != eItr; ++bItr)
        {
            ItemConfig *pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(*bItr);
            MallGood *pMallGood = CMall::Instance().FindGoods(*bItr);

            if (NULL != pItemInfo && NULL != pMallGood && !pMallGood->m_nIsPrecious && pActivityData->m_nRewardItemTime > 0 && 
                pActivityData->m_setClothPos.find(pItemInfo->m_anType[2]) != pActivityData->m_setClothPos.end())
            {
                if (ESexType_No != pItemInfo->m_SexNeed && pRole->GetSex() != pItemInfo->m_SexNeed)
                {
                    bHasOppositeSex = true;
                    break;
                }

                CItem item(pItemInfo->m_nType, 1, pActivityData->m_nRewardItemTime * 24 * 3600, false);
                listItems.push_back(item);
            }
        }

        if (!bHasOppositeSex && listItems.size() > 0)
        {
            pActivityComponent->SetSharedToSocial((EUISocialShare)msg.m_nUI, true);

            CItemProcess::AddOrMailItems(*pRole, listItems, EItemAction_Add_MallSocialShare, EActivity_MallSocialShare, 
                true, EMailType_MallSocialShare, pActivityData->m_strMailTitle, pActivityData->m_strMailContent);

            CReward reward;
            reward.m_listItems = listItems;

            GameMsg_S2C_ActivitySocialShare retMsg;
            retMsg.m_nUI = msg.m_nUI;
            retMsg.m_listRewards.push_back(reward);

            pNet->SendPlayerMsg(&retMsg);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

CPhotoSocialShareActivity::CPhotoSocialShareActivity(const CPhotoSocialShareActivityData &rData, time_t beginShowTime, time_t endShowTime)
    : CSocialShareActivity(EActivity_PhotoSocialShare)
{
    m_pData = new CPhotoSocialShareActivityData(rData);

    CSocialShareActivity::Init(beginShowTime, endShowTime);
}

CPhotoSocialShareActivity::CPhotoSocialShareActivity(const CPhotoSocialShareActivity &ref)
    : CSocialShareActivity(EActivity_PhotoSocialShare)
{
    CPhotoSocialShareActivityData *pData = dynamic_cast<CPhotoSocialShareActivityData *>(ref.m_pData);

    if (NULL != pData)
    {
        m_pData = new CPhotoSocialShareActivityData(*pData);

        CSocialShareActivity::Init(ref.m_nBeginShowTime, ref.m_nEndShowTime);
    }
}

CPhotoSocialShareActivity::~CPhotoSocialShareActivity()
{
    delete m_pData;
    m_pData = NULL;
}

void CPhotoSocialShareActivity::SetRewardItemInfo(const CRewardTitleInfo *pItemInfo)
{
    if (NULL == m_pData || NULL == pItemInfo)
        return;

    const CItemRewardInfo *pItemRewardInfo = dynamic_cast<CItemRewardInfo *>(const_cast<CRewardTitleInfo *>(pItemInfo));

    if (NULL == pItemRewardInfo)
        return;

    for (std::list<CRewardTitleInfo *>::iterator itr = m_pData->m_listReward.begin(); 
         itr != m_pData->m_listReward.end(); ++itr)
    {
        if ((*itr)->m_nIdx == pItemRewardInfo->m_nIdx)
        {
            CItemRewardInfo *pData = dynamic_cast<CItemRewardInfo *>(*itr);

            if (pData != NULL)
            {
                pData->m_listMaleRewards.push_back(*(pItemRewardInfo->m_listMaleRewards.begin() ) );
                pData->m_listFemaleRewards.push_back(*(pItemRewardInfo->m_listFemaleRewards.begin() ) );

                return;
            }
        }
    }

    CItemRewardInfo *pInfo = new CItemRewardInfo(*pItemRewardInfo);

    m_pData->m_listReward.push_back(pInfo);
}

void CPhotoSocialShareActivity::ProcessActivityMsg(CRoleEntity *pRole, CEntityActivityComponent *pActivityComponent, GameMsg_C2S_ActivitySocialShare &msg)
{
    CPhotoSocialShareActivityData *pActivityData = dynamic_cast<CPhotoSocialShareActivityData *>(m_pData);

    if (NULL == pRole || NULL == pActivityComponent || NULL == pActivityData)
        return;

    CEntityNetComponent *pNet = pActivityComponent->GetComponent<CRoleNet>();
    CEntityAttributeComponent *pRoleAttrComponent = pActivityComponent->GetComponent<CRoleAttribute>();

    if (pNet && IsOpenNow() && !pActivityComponent->IsSharedToSocial((EUISocialShare)msg.m_nUI) )
    {
        CPhotoSocialShareData *pShareData = dynamic_cast<CPhotoSocialShareData *>(msg.m_pData);
        CItemRewardInfo *pItems = dynamic_cast<CItemRewardInfo *>(*pActivityData->m_listReward.begin() );

        if (pShareData != NULL && pItems != NULL && pShareData->m_nPeopleNum >= pActivityData->m_nPeopleNumber && 
            (EPhotoRoomTheme_Normal == pShareData->m_nRoomTheme || EPhotoRoomTheme_Couple == pShareData->m_nRoomTheme) )
        {
            pActivityComponent->SetSharedToSocial((EUISocialShare)msg.m_nUI, true);
            pActivityComponent->SetNotGetPhotoShareVIPReward(!pRole->IsVIP() );

            GameMsg_S2C_ActivitySocialShare retMsg;
            retMsg.m_nUI = msg.m_nUI;

            std::list<CReward> &rRewards = pItems->m_listMaleRewards;
            if (pRole->GetSex() == ESexType_Female)
                rRewards = pItems->m_listFemaleRewards;

            for (std::list<CReward>::const_iterator itr = rRewards.begin(); 
                 itr != rRewards.end(); ++itr)
            {
                CReward reward;

                reward.m_listItems = itr->m_listItems;
                reward.m_nMoney = itr->m_nMoney;
                reward.m_nBindMCoin = itr->m_nBindMCoin;
                reward.m_nRewardType = itr->m_nRewardType;

                retMsg.m_listRewards.push_back(reward);

                if (itr->m_nRewardType == ERewardType_VIP && !pRole->IsVIP() )
                    continue;

                if (reward.m_listItems.size() != 0)
                {
                    CItemProcess::AddOrMailItems(*pRole, reward.m_listItems, EItemAction_Add_PhotoSocialShare, EActivity_PhotoSocialShare, 
                        true, EMailType_PhotoSocialShare, pActivityData->m_strMailTitle, pActivityData->m_strMailContent);
                }

                if (reward.m_nMoney > 0)
                {
                    pRoleAttrComponent->ChangeMoney(reward.m_nMoney, EChangeMoneyCause_Add_PhotoSocialShare, 0);
                }

                if (reward.m_nBindMCoin > 0)
                {
                    pRoleAttrComponent->ChangeBindBill(reward.m_nBindMCoin, EChangeBindBillCause_Add_PhotoSocialShare);
                }
            }

            pNet->SendPlayerMsg(&retMsg);
        }
    }
}

void CPhotoSocialShareActivity::ResendVIPExtraReward(unsigned char nOpenVIPFromType, CRoleEntity *pRole, CEntityActivityComponent *pActivityComponent)
{
    CPhotoSocialShareActivityData *pActivityData = dynamic_cast<CPhotoSocialShareActivityData *>(m_pData);

    if (pRole == NULL || pActivityComponent == NULL || pActivityData == NULL)
        return;

    CEntityNetComponent *pNet = pActivityComponent->GetComponent<CRoleNet>();
    CEntityAttributeComponent *pRoleAttrComponent = pActivityComponent->GetComponent<CRoleAttribute>();
    CItemRewardInfo *pItems = dynamic_cast<CItemRewardInfo *>(*pActivityData->m_listReward.begin() );

    if (pNet != NULL && pItems != NULL)
    {
        GameMsg_S2C_RequestVIPExtraRewardRes vipRewardRes;
        vipRewardRes.m_nOpenVipFromType = nOpenVIPFromType;

        if (!pActivityComponent->CanGetPhotoShareVIPReward() )
        {
            vipRewardRes.m_nErrorType = ERequestVIPExtraRewardErrorType_AlreadyGot;

            pNet->SendPlayerMsg(&vipRewardRes);

            return;
        }

        pActivityComponent->SetNotGetPhotoShareVIPReward(false);

        std::list<CReward> &rRewards = pItems->m_listMaleRewards;
        if (pRole->GetSex() == ESexType_Female)
            rRewards = pItems->m_listFemaleRewards;

        for (std::list<CReward>::const_iterator itr = rRewards.begin(); 
            itr != rRewards.end(); ++itr)
        {
            if (itr->m_nRewardType == ERewardType_VIP)
            {
                CReward reward;

                reward.m_listItems = itr->m_listItems;
                reward.m_nMoney = itr->m_nMoney;
                reward.m_nBindMCoin = itr->m_nBindMCoin;
                reward.m_nRewardType = itr->m_nRewardType;

                vipRewardRes.m_listRewards.push_back(reward);

                if (reward.m_listItems.size() != 0)
                {
                    CItemProcess::AddOrMailItems(*pRole, reward.m_listItems, EItemAction_Add_PhotoShareVIPExtraReward, EActivity_PhotoSocialShare, 
                        true, EMailType_PhotoShareVIPExtraReward, pActivityData->m_strMailTitle, pActivityData->m_strMailContent);
                }

                if (reward.m_nMoney > 0)
                {
                    pRoleAttrComponent->ChangeMoney(reward.m_nMoney, EChangeMoneyCause_Add_PhotoShareVIPExtraReward, 0);
                }

                if (reward.m_nBindMCoin > 0)
                {
                    pRoleAttrComponent->ChangeBindBill(reward.m_nBindMCoin, EChangeBindBillCause_Add_PhotoShareVIPExtraReward);
                }

                pNet->SendPlayerMsg(&vipRewardRes);

                return;
            }
        }
    }
}


