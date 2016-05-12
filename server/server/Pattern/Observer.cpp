#include "Observer.h"

//////////////////////////////////////////////////////////////////////////
CommonSubscriber::CommonSubscriber()
{
}

CommonSubscriber::~CommonSubscriber()
{
}

void CommonSubscriber::OnNotify(IParam &param)
{
    // DO NOTHING
}

const std::string &CommonSubscriber::GetSubscriberName()
{
    return m_strSubscriberName;
}

void CommonSubscriber::SetSubscriberName(const std::string &strName)
{
    m_strSubscriberName = strName;
}

//////////////////////////////////////////////////////////////////////////
CommonPublisher::CommonPublisher()
{
}

CommonPublisher::~CommonPublisher()
{
}

void CommonPublisher::AddSubscriber(ISubscriber *subscriber)
{
    m_listSubscriber.push_back(subscriber);
}

void CommonPublisher::RemoveSubscriber(ISubscriber *subscriber)
{
    for (std::list<ISubscriber *>::iterator it = m_listSubscriber.begin();
        it != m_listSubscriber.end(); ++it)
    {
        if (*it == subscriber)
        {
            m_listSubscriber.erase(it);
            break;
        }
    }
}

void CommonPublisher::NotifyAll(IParam &param)
{
    for (std::list<ISubscriber *>::iterator it = m_listSubscriber.begin();
        it != m_listSubscriber.end(); ++it)
    {
        ISubscriber *subscriber = *it;
        subscriber->OnNotify(param);
    }
}

const std::string &CommonPublisher::GetPublisherName()
{
    return m_strPublisherName;
}

void CommonPublisher::SetPublisherName(const std::string &strName)
{
    m_strPublisherName = strName;
}

