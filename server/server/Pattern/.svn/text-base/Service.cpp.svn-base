#include "Service.h"

Service::Service( const char* pServiceName )
    : GameMsg_Processor(true, true)
    , m_strServiceName( pServiceName )
{
}

Service::~Service()
{
}

const std::string &Service::ServiceName() const
{
    return m_strServiceName;
}

bool Service::Init()
{
    return true;
}

void Service::Start()
{
}

void Service::Stop()
{
}

void Service::Destroy()
{
}

void Service::OnUpdate(int nMilliseconds)
{
}


//////////////////////////////////////////////////////////////////////////
ServiceManager::ServiceManager()
    : Service( "ServiceManager" )
{
}

ServiceManager::~ServiceManager()
{

}

const std::string &ServiceManager::ServiceName() const
{
    static const std::string kDefaultServiceName("ServiceManager");
    return kDefaultServiceName;
}

bool ServiceManager::Init()
{
    for (std::list<Service *>::iterator it = m_runningService.begin();
        it != m_runningService.end(); ++it)
    {
        Service *service = *it;
        service->Init();
    }

    return true;
}

void ServiceManager::Start()
{
    for (std::list<Service *>::iterator it = m_runningService.begin();
        it != m_runningService.end(); ++it)
    {
        Service *service = *it;
        service->Start();
    }
}

void ServiceManager::Stop()
{
    for (std::list<Service *>::iterator it = m_runningService.begin();
        it != m_runningService.end(); ++it)
    {
        Service *service = *it;
        service->Stop();
    }
}

void ServiceManager::Destroy()
{
    for (std::list<Service *>::iterator it = m_runningService.begin();
        it != m_runningService.end(); ++it)
    {
        Service *service = *it;
        service->Destroy();
    }
}

void ServiceManager::OnUpdate(int nMilliseconds)
{
    for (std::list<Service *>::iterator it = m_runningService.begin();
        it != m_runningService.end(); ++it)
    {
        Service *service = *it;
        service->OnUpdate(nMilliseconds);
    }
}


void ServiceManager::AddService(Service *service)
{
    m_runningService.push_back(service);
}

Service *ServiceManager::RemoveService(const std::string &name)
{
    for (std::list<Service *>::iterator it = m_runningService.begin();
        it != m_runningService.end(); ++it)
    {
        Service *service = *it;
        if (service != NULL && service->ServiceName() == name)
        {
            m_runningService.erase(it);
            return service;
        }
    }
    return NULL;
}

Service *ServiceManager::FindService(const std::string &name) const
{
    for (std::list<Service *>::const_iterator it = m_runningService.begin();
        it != m_runningService.end(); ++it)
    {
        Service *service = *it;
        if (service != NULL && service->ServiceName() == name)
        {
            return service;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////////
std::map<std::string, Service *> ServiceDataManager::m_registeredService;

ServiceDataManager::ServiceDataManager()
{
}

ServiceDataManager::~ServiceDataManager()
{
}

void ServiceDataManager::RegisterService(Service *service)
{
    m_registeredService[service->ServiceName()] = service;
}

Service *ServiceDataManager::FindService(const std::string &name)
{
    std::map<std::string, Service *>::const_iterator it = m_registeredService.find(name);
    if (it != m_registeredService.end())
    {
        return it->second;
    }
    return NULL;
}

