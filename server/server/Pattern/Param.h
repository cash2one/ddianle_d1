/**
* Filename: Param
* Description: �����ӿڣ����ڲ�ͬģ�����Ϣ���ݣ���������������ģʽ����һ���Ż�
* Copyright (C) 2015 ddianle Inc. All rights reserved.
* Author: xieshaobin
* Create time: 2015-05-14 16:48:23
*/

#ifndef _PATTERN_PARAM_H_
#define _PATTERN_PARAM_H_

#include <map>
#include <string>

#ifdef WIN32
#pragma warning(disable:4100)
#endif

// ������ӿ�
class IParam
{
public:
    virtual ~IParam() {}
    virtual void SetParam(const std::string &paramName, const std::string &paramValue) = 0;
    virtual void SetParam(const std::string &paramName, int paramValue) = 0;
    virtual bool GetParam(const std::string &paramName, std::string &paramValue) = 0;
    virtual bool GetParam(const std::string &paramName, int &paramValue) = 0;
    virtual bool ExistParam(const std::string &paramName) = 0;
    virtual void RemoveParam(const std::string &paramName) = 0;
    virtual void Clear() = 0;
};


// �����Ĳ�����ʵ��
class CommonParam : public IParam
{
public:
    CommonParam();
    virtual ~CommonParam();

public:
    virtual void SetParam(const std::string &paramName, const std::string &paramValue);
    virtual void SetParam(const std::string &paramName, int paramValue);
    virtual bool GetParam(const std::string &paramName, std::string &paramValue);
    virtual bool GetParam(const std::string &paramName, int &paramValue);
    virtual bool ExistParam(const std::string &paramName);
    virtual void RemoveParam(const std::string &paramName);
    virtual void Clear();

protected:
    // parameter map, key: paramName, value: paramValue
    std::map<std::string, std::string> m_mapParam;
};

#endif //_PATTERN_PARAM_H_
