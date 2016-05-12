/********************************************************************
    created:    2015-07-13
    filename:   DataStructure\datainfor\ExpConfig.h
    author:     

    purpose:   LevExp.bytes配置中相对应的数据结构
*********************************************************************/

#ifndef __ExpCONFIG_H__
#define __ExpCONFIG_H__

class CFileStream;

#define ExpDataInfoName  "/data/bytes/LevExp.bytes"

class ExpConfig
{
public:
    ExpConfig(){};
    ~ExpConfig(){};

public:
    bool Load(CFileStream&file);
    bool Save(CFileStream&file);

public:
    unsigned short m_nLev ;
    int  m_nExp ;
};

#endif

