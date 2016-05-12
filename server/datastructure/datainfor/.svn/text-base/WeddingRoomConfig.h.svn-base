/********************************************************************
    created:    2015-07-13
    filename:   DataStructure\datainfor\WeddingRoomConfig
    author:     

    purpose:    WeddingInfo.bytes对应配置数据结构
*********************************************************************/

#ifndef __WEDDINGROOMINFO_H__
#define __WEDDINGROOMINFO_H__

#include <string>
#include <vector>
#include <list>
#include "../DataStruct_Base.h"

#define WeddingRoomDataInfoName  "/data/bytes/WeddingInfo.bytes"

class CFileStream;


//////////////////////////////////////////////////////////////////////////
///亲密度升级配置  只是读取没有起作用
//////////////////////////////////////////////////////////////////////////
class IntimacyLevelConfig
{
public:
    IntimacyLevelConfig(){};
    ~IntimacyLevelConfig(){};

public:
    bool Load(CFileStream&file);
    bool Save(CFileStream&file);

public:
    unsigned short      m_nLev ;
    unsigned int        m_nExp ;
    std::string         m_strTitle ;
};



//////////////////////////////////////////////////////////////////////////
///婚房配置数据结构
//////////////////////////////////////////////////////////////////////////
class WeddingRoomConfig
{
public:
    WeddingRoomConfig();
    ~WeddingRoomConfig();

public:
    bool Load(CFileStream&file);
    bool Save(CFileStream&file);

public:
    unsigned char    m_nType  ;
    std::string      m_strIcon ;
    std::string      m_strName ;
    std::string      m_strRoom ;
    itemtype_t       m_nItemID;
    unsigned short   m_nItemCount;
};


#endif
