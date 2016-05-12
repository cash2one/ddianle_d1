/********************************************************************
    created:    2015-07-13
    filename:   DataStructure\datainfor\ConstellationConfigManager.h
    author:     

    purpose:    对星座配置数据进行管理
*********************************************************************/

#ifndef _CONSTELLATION_CONFIG_MANAGER_H_
#define _CONSTELLATION_CONFIG_MANAGER_H_

#include <string>
#include <vector>
#include "../../datastructure/Macro_Define.h"
#include "ConstellationConfig.h"

class CFileStream;
class CParamPool;

// 星座配置数据管理
class ConstellationConfigManager
{
public:
    ConstellationConfigManager(void);
    ~ConstellationConfigManager(void);

public:
    bool Load(const char *strPath);
    bool Load(CFileStream &file);
    void LoadMatchRate(CFileStream &file);
    void LoadWishWords(CFileStream &file);
    bool LoadQuestion(CFileStream &file);

    bool Save(const char *strPath);
    bool Save(CFileStream &file);
    void SaveMatchRate(CFileStream &file);
    void SaveWishWords(CFileStream &file);
    void SaveQuestion(CFileStream &file);

    void Destroy();

    void AddWishWords(const char *strWords);
    void SetMatchRate(int nConstellation, int nMatchConstellation, int nRate);
    void AddQuestion(ConstellationConfig &question);

    const std::vector<std::string> &WishWordsList() const;
    const int *MatchRateList() const;
    void AllQuestion(std::vector<ConstellationConfig> &vecQuestion);

protected:
    std::vector<std::string> m_vecWishWords;
    int m_szMatchRate[MAX_CONSTELLATION_COUNT][MAX_CONSTELLATION_COUNT];
    std::vector<ConstellationConfig> m_vecQuestion;
};

#endif

