/********************************************************************
    created:    2015-07-13
    filename:   DataStructure\datainfor\ConstellationConfig.h
    author:     

    purpose:    Constellation.bytes配置中相对应的数据结构
*********************************************************************/

#ifndef _CONSTELLATION_CONFIG_H_
#define _CONSTELLATION_CONFIG_H_

#include <string>
#include <vector>
#include "../../datastructure/Macro_Define.h"

class CFileStream;
class CParamPool;

//////////////////////////////////////////////////////////////////////////
///星座配置数据结构
//////////////////////////////////////////////////////////////////////////
class ConstellationConfig
{
public:
    ConstellationConfig();
    ~ConstellationConfig();

public:
    bool Load(CFileStream &file);
    bool Save(CFileStream &file);

    void doEncode(CParamPool &IOBuff);
    void doDecode(CParamPool &IOBuff);

public:
    std::string m_strQuestion;
    std::vector<std::string> m_vecAnswer;
};

#endif

