#include "SocialShareMgr.h"
#include "SocialShareActivity.h"
#include "SystemActivityManager.h"
#include "../../socket/ParamPool.h"
#include "../../socket/Log.h"
#include "EntityActivityComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../SystemSetting/SystemSettingMgr.h"

typedef std::list<CSocialShareActivity *>::iterator         Itr;
typedef std::list<CSocialShareActivity *>::const_iterator   CItr;

CSocialShareMgr::CSocialShareMgr(void)
{
}

CSocialShareMgr::~CSocialShareMgr(void)
{
}

CSocialShareMgr& CSocialShareMgr::Instance()
{
    static CSocialShareMgr inst;

    return inst;
}

void CSocialShareMgr::Init()
{
    for (Itr bItr = m_listActivities.begin(), eItr = m_listActivities.end(); 
         bItr != eItr; ++bItr)
    {
        CSystemActivityManager::Instance().AddTimerActivity(*bItr);
    }
}

void CSocialShareMgr::AddSocialShareActivity(const CSocialShareActivity &rActivity)
{
    CSocialShareActivity *pActivity = NULL;

    switch (rActivity.GetActivityType())
    {
    case EActivity_MallSocialShare:
        pActivity = new CMallSocialShareActivity((CMallSocialShareActivity &)rActivity);
        break;

    case EActivity_PhotoSocialShare:
        pActivity = new CPhotoSocialShareActivity((CPhotoSocialShareActivity &)rActivity);
        break;

    default:
        LOG_WARNING << "CSocialShareMgr::AddSocialShareActivity() warning: activity type: " 
                    << rActivity.GetActivityType() << " is invalid "<< endl;
        break;
    }

    if (NULL != pActivity)
        m_listActivities.push_back(pActivity);
}

void CSocialShareMgr::AddRewardItemInfo(int nActivityType, const CRewardTitleInfo *pItemInfo)
{
    CSocialShareActivity *pActivity = Find(nActivityType);

    if (NULL != pActivity)
        pActivity->SetRewardItemInfo(pItemInfo);
}

void CSocialShareMgr::AddTimeRangeInfo(int nActivityType, time_t begin, time_t end, unsigned int openType, const std::list<unsigned int> &rListDate, std::string &beginHour, std::string &endHour)
{
    CSocialShareActivity *pActivity = Find(nActivityType);

    if (NULL != pActivity)
        pActivity->AddTimeRangeInfo(begin, end, openType, rListDate, beginHour, endHour);
}

bool CSocialShareMgr::IsShowNow(int nActivityType)
{
    CSocialShareActivity *pActivity = Find(nActivityType);

    if (NULL != pActivity)
        return pActivity->IsShowing();

    return false;
}

bool CSocialShareMgr::IsOpenNow(int nActivityType)
{
    CSocialShareActivity *pActivity = Find(nActivityType);

    if (NULL != pActivity)
        return pActivity->IsOpenNow();

    return false;
}

const CSocialShareActivityData* CSocialShareMgr::GetActivityData(int nActivityType) const
{
    CSocialShareActivity *pActivity = Find(nActivityType);

    if (NULL != pActivity)
        return pActivity->GetActivityData();

    return NULL;
}

bool CSocialShareMgr::CanShareAgain(int nActivityType, time_t lastSharedTime) const
{
    CSocialShareActivity *pActivity = Find(nActivityType);

    if (NULL != pActivity)
        return pActivity->CanParticipateAgain(lastSharedTime);

    return false;
}

void CSocialShareMgr::OnHandleActivityMsg(int nActivityType, CRoleEntity *pRole, CEntityActivityComponent *pActivityComponent, GameMsg_C2S_ActivitySocialShare &msg)
{
    CSocialShareActivity *pActivity = Find(nActivityType);

    if (NULL != pActivity && NULL != pRole && NULL != pActivityComponent)
        pActivity->ProcessActivityMsg(pRole, pActivityComponent, msg);
}

void CSocialShareMgr::OnHandleResendVIPExtraRewardMsg(int nActivityType, unsigned char nOpenVIPFromType, 
    CRoleEntity *pRole, CEntityActivityComponent *pActivityComponent)
{
    CSocialShareActivity *pActivity = Find(nActivityType);

    if (pActivity != NULL && pRole != NULL && pActivityComponent != NULL)
        pActivity->ResendVIPExtraReward(nOpenVIPFromType, pRole, pActivityComponent);
}

unsigned short CSocialShareMgr::Count() const
{
    unsigned short count = 0;

    for (CItr bItr = m_listActivities.begin(), eItr = m_listActivities.end(); 
         bItr != eItr; ++bItr)
    {
        if ((*bItr)->IsShowing())
            ++count;
    }

    return count;
}

CSocialShareActivity* CSocialShareMgr::Find(int nActivityType) const
{
    CSocialShareActivity *pRet = NULL;

    for (CItr bItr = m_listActivities.begin(), eItr = m_listActivities.end(); 
         bItr != eItr; ++bItr)
    {
        if (nActivityType == (*bItr)->GetActivityType())
        {
            pRet = *bItr;

            break;
        }
    }

    return pRet;
}


