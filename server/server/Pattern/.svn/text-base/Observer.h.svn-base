/**
* Filename: Observer
* Description: 观察者模式基础类，供观察者实现类继承
* Copyright (C) 2015 ddianle Inc. All rights reserved.
* Author: xieshaobin
* Create time: 2015-05-14 14:13:34
*/

#ifndef _PATTERN_OBSERVER_H_
#define _PATTERN_OBSERVER_H_

#include "Param.h"

#include <list>
#include <string>

//////////////////////////////////////////////////////////////////////////
// 观察者（订阅者）接口，关心某一主题或事件的类可以实现此接口
class ISubscriber
{
public:
    virtual ~ISubscriber() {}
    // 主题或事件发生变化时，被观察者（信息发布者）会调用此方法通知观察者
    virtual void OnNotify(IParam &param) = 0;
};


//////////////////////////////////////////////////////////////////////////
// 被观察者（信息发布者）接口，掌握一定信息并要通知其它类的类，可以实现此接口
class IPublisher
{
public:
    virtual ~IPublisher() {}
    // 添加观察者（订阅者）到观察者列表，当信息发生变化时，会通知列表中的观察者
    virtual void AddSubscriber(ISubscriber *subscriber) = 0;

    // 从观察者列表中移除指定观察者
    virtual void RemoveSubscriber(ISubscriber *subscriber) = 0;

    // 信息/状态发生变化时，调用此方法通知观察者（订阅者）信息发生了变化
    virtual void NotifyAll(IParam &param) = 0;
};


//////////////////////////////////////////////////////////////////////////
// 简单的订阅者
class CommonSubscriber : public ISubscriber
{
public:
    CommonSubscriber();
    virtual ~CommonSubscriber();

public:
    virtual void OnNotify(IParam &param);

    // 订阅者/观察者名字
    virtual const std::string &GetSubscriberName();
    virtual void SetSubscriberName(const std::string &strName);

protected:
    std::string m_strSubscriberName;
};


//////////////////////////////////////////////////////////////////////////
// 简单的信息发布者
class CommonPublisher : public IPublisher
{
public:
    CommonPublisher();
    virtual ~CommonPublisher();

public:
    virtual void AddSubscriber(ISubscriber *subscriber);
    virtual void RemoveSubscriber(ISubscriber *subscriber);
    virtual void NotifyAll(IParam &param);

    // 发布者/被观察者名字
    virtual const std::string &GetPublisherName();
    virtual void SetPublisherName(const std::string &strName);

protected:
    // 观察者列表
    std::list<ISubscriber *> m_listSubscriber;
    std::string m_strPublisherName;
};

#endif //_PATTERN_OBSERVER_H_
