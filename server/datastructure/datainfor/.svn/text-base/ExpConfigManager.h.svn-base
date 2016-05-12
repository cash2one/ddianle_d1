/********************************************************************
    created:    2015-07-13
    filename:   DataStructure\datainfor\ExpConfigManager.h
    author:     

    purpose:    对升级经验配置数据进行管理
*********************************************************************/

#ifndef __EXPCONFIGMGR_H_
#define __EXPCONFIGMGR_H_

#include <vector>
#include <map>
#include "./ExpConfig.h"

class ExpConfigManager
{
public:
    ExpConfigManager() ;
    virtual ~ExpConfigManager() ;

public:
    //释放所有对象
    virtual void Destroy();
    bool LoadExpInfo(const char* pszFileName);
    int GetByLev(unsigned short nlev) ;

private:
    std::map<unsigned short , int> m_mapExp ;
};







#endif

