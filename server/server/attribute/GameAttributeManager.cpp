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
{ // Ҫ��ʼ��״̬��
    m_nActIndex = PaymentInfoMgr::Instance().FixedChargeIndex(time(NULL));
    m_bIsOpen = (m_nActIndex >= 0);

    return ;
}

void GameAttributeManager::OnUpdate(const unsigned long &lTimeElapsed)
{ // ÿ��ˢ��״̬
    const unsigned long kUpdateInterval = 1000; // ���ʱ�䣬����
    static unsigned long lastCheckElapsed = 0;

    lastCheckElapsed += lTimeElapsed;
    if (lastCheckElapsed < kUpdateInterval)
        return ;// ���ü��

    lastCheckElapsed = 0; // 

    // �����״̬�����仯�����߻���ڷ����ı䣬
    int nActIndex = PaymentInfoMgr::Instance().FixedChargeIndex(time(NULL));
    do 
    {
        bool bIsOpen = (nActIndex >= 0);
        if (!m_bIsOpen && (m_bIsOpen == bIsOpen)) // ԭ��û����������Ҳû����
            break; 

        if (m_bIsOpen && (m_bIsOpen == bIsOpen) && m_nActIndex == nActIndex) // ԭ�ȿ���������Ҳ��������������ͬһ������
            break;
        
        // �����л������߿���״̬�£����ڲ�ͬ���䣬��Ӧ������֪ͨ�ͻ���
        std::list<CRoleEntity*> allRole;
        CPlayerManager::Instance().GetAllPlayers(allRole);
        
        for (std::list<CRoleEntity*>::iterator iter = allRole.begin(); iter != allRole.end(); ++iter)
        {
            // ȡ��roleAttribute:
            CRoleAttribute* attr = (*iter)->GetComponent<CRoleAttribute>();
            ENSURE_WITH_LOG_CMD(NULL != attr, continue, "error to get null role[%u] attr", (*iter)->GetRoleID());

            attr->SendFixedChargeMsg(false, 0); // ֪ͨ��ȥ
        }

        m_bIsOpen = bIsOpen;
        m_nActIndex = nActIndex;

    } while (0);

    return ;
}

