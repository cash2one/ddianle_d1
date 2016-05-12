#include "EntityComponent.h"

CEntityComponent::CEntityComponent(bool bProNetMsg, bool bProQueryMsg)
    : GameMsg_Processor(bProNetMsg, bProQueryMsg)
    , m_pEntity(NULL)
    , m_nIndexInEntity(-1)
    , m_ComponentName("")
{
}

CEntityComponent::~CEntityComponent(void)
{
}


int CEntityComponent::GetIndexInEntity()
{
    return m_nIndexInEntity;
}

void CEntityComponent::SetIndexInEntity(int nIndexInEntity)
{
    m_nIndexInEntity = nIndexInEntity;
}

void CEntityComponent::AttachToEntity(IEntity * pEntity)
{
    m_pEntity = pEntity;
}

IEntity *CEntityComponent::Entity()
{
    return m_pEntity;
}

void CEntityComponent::NotifyAll(IParam &param)
{
    for (std::list<ISubscriber *>::iterator it = m_listSubscriber.begin();
        it != m_listSubscriber.end(); ++it)
    {
        CommonSubscriber *subscriber = (CommonSubscriber *)*it;

        std::string strSubscriberName = subscriber->GetSubscriberName();
        std::string strPublisherName = GetPublisherName();

        if (strSubscriberName.empty() || strPublisherName.empty() || param.ExistParam(strSubscriberName))
        {
            subscriber->OnNotify(param);
        }
        else
        {
            param.SetParam(strSubscriberName, strPublisherName);
            subscriber->OnNotify(param);
            param.RemoveParam(strSubscriberName);
        }
    }
}

