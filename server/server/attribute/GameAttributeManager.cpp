#include "GameAttributeManager.h"
#include "../../datastructure/RoleEntity.h"
#include "EntityAttributeComponent.h"
#include "../payment/PaymentInfoMgr.h"
#include "../logic/PlayerManager.h"

GameAttributeManager::GameAttributeManager()
    : m_bIsOpen(false)
    , m_nActIndex(0)
{

}

GameAttributeManager::~GameAttributeManager()
{

}

GameAttributeManager& GameAttributeManager::Instance()
{
    static GameAttributeManager g;
    return g;
}


void GameAttributeManager::Initialize()
{ // 要初始化状态：
    m_nActIndex = PaymentInfoMgr::Instance().FixedChargeIndex(time(NULL));
    m_bIsOpen = (m_nActIndex >= 0);

    return ;
}

void GameAttributeManager::OnUpdate(const unsigned long &lTimeElapsed)
{ // 每秒刷新状态
    const unsigned long kUpdateInterval = 1000; // 间隔时间，毫秒
    static unsigned long lastCheckElapsed = 0;

    lastCheckElapsed += lTimeElapsed;
    if (lastCheckElapsed < kUpdateInterval)
        return ;// 不用检测

    lastCheckElapsed = 0; // 

    // 活动开关状态发生变化，或者活动周期发生改变，
    int nActIndex = PaymentInfoMgr::Instance().FixedChargeIndex(time(NULL));
    do 
    {
        bool bIsOpen = (nActIndex >= 0);
        if (!m_bIsOpen && (m_bIsOpen == bIsOpen)) // 原先没开启，现在也没开启
            break; 

        if (m_bIsOpen && (m_bIsOpen == bIsOpen) && m_nActIndex == nActIndex) // 原先开启，现在也开启，并且属于同一个区间
            break;
        
        // 开关切换，或者开启状态下，属于不同区间，都应该主动通知客户端
        std::list<CRoleEntity*> allRole;
        CPlayerManager::Instance().GetAllPlayers(allRole);
        
        for (std::list<CRoleEntity*>::iterator iter = allRole.begin(); iter != allRole.end(); ++iter)
        {
            // 取得roleAttribute:
            CRoleAttribute* attr = (*iter)->GetComponent<CRoleAttribute>();
            ENSURE_WITH_LOG_CMD(NULL != attr, continue, "error to get null role[%u] attr", (*iter)->GetRoleID());

            attr->SendFixedChargeMsg(false, 0); // 通知下去
        }

        m_bIsOpen = bIsOpen;
        m_nActIndex = nActIndex;

    } while (0);

    return ;
}

