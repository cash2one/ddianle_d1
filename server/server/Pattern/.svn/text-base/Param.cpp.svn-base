#include "Param.h"

#include <sstream>

CommonParam::CommonParam()
{

}

CommonParam::~CommonParam()
{

}


void CommonParam::SetParam(const std::string &paramName, const std::string &paramValue)
{
    m_mapParam[paramName] = paramValue;
}

void CommonParam::SetParam(const std::string &paramName, int paramValue)
{
    std::stringstream ss;
    ss << paramValue;
    SetParam(paramName, ss.str());
}

bool CommonParam::GetParam(const std::string &paramName, std::string &paramValue)
{
    if (m_mapParam.find(paramName) != m_mapParam.end())
    {
        paramValue = m_mapParam.find(paramName)->second;
        return true;
    }

    return false;
}

bool CommonParam::GetParam(const std::string &paramName, int &paramValue)
{
    std::string strParamValue;
    bool bHasParam = GetParam(paramName, strParamValue);
    if (bHasParam)
    {
        std::stringstream ss;
        ss << strParamValue;
        ss >> paramValue;
    }

    return bHasParam;
}

bool CommonParam::ExistParam(const std::string &paramName)
{
    return (m_mapParam.find(paramName) != m_mapParam.end());
}

void CommonParam::RemoveParam(const std::string &paramName)
{
    m_mapParam.erase(paramName);
}

void CommonParam::Clear()
{
    m_mapParam.clear();
}

