/**
* Service.h
* Description: 抽象的服务，各种Service的基类
*              Service处理网络消息(GameMsg)和DB消息(DB callback)，是业务逻辑的主要实现
* Copyright (C) 2015 ddianle Inc. All rights reserved.
* Author: xieshaobin
* Create time: 2015-04-02 19:11:00
*/

#ifndef __SERVICE_H__
#define __SERVICE_H__

#include "../../socket/GameMsg_Processor.h"

#include <list>
#include <map>
#include <string>

class Service : public GameMsg_Processor
{
public:
    Service( const char* pServiceName );
    virtual ~Service();

public:
    virtual const std::string &ServiceName() const;

    virtual bool Init();
    virtual void Start();
    virtual void Stop();
    virtual void Destroy();

    virtual void OnUpdate(int nMilliseconds);

protected:
    std::string m_strServiceName;
};

class ServiceManager : public Service
{
public:
    ServiceManager();
    virtual ~ServiceManager();

public:
    virtual const std::string &ServiceName() const;

    virtual bool Init();
    virtual void Start();
    virtual void Stop();
    virtual void Destroy();

    virtual void OnUpdate(int nMilliseconds);

public:
    virtual void AddService(Service *service);
    virtual Service *RemoveService(const std::string &name);
    virtual Service *FindService(const std::string &name) const;

protected:
    std::list<Service *> m_runningService;
};

class ServiceDataManager
{
private:
    ServiceDataManager();
    virtual ~ServiceDataManager();

public:
    static void RegisterService(Service *service);
    static Service *FindService(const std::string &name);

protected:
    static std::map<std::string, Service *> m_registeredService;
};

#endif //__SERVICE_H__

