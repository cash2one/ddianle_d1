#include "ConstellationConfigManager.h"
#include <string.h>

#include "FileStream.h"
#include "../../socket/ParamPool.h"

ConstellationConfigManager::ConstellationConfigManager(void)
{
    memset(m_szMatchRate, 0, sizeof(m_szMatchRate));
}

ConstellationConfigManager::~ConstellationConfigManager(void)
{
}

bool ConstellationConfigManager::Load(const char *strPath)
{
    CFileStream file;
    file.open( strPath, "rb" );
    return Load(file);
}

bool ConstellationConfigManager::Load(CFileStream &file)
{
    Destroy();

    if (!file.IsOpen())
    {
        return false;
    }

    LoadMatchRate(file);
    LoadWishWords(file);
    LoadQuestion(file);

    //file.close();

    return true;
}

void ConstellationConfigManager::LoadMatchRate(CFileStream &file)
{
    unsigned short nCount = 0;
    file.ReadUShort(nCount);

    for (unsigned short i = 0; i < nCount; i++)
    {
        unsigned short nConstellation = 0;
        unsigned short nMatchConstellation = 0;
        unsigned short nRate = 0;

        file.ReadUShort(nConstellation);
        file.ReadUShort(nMatchConstellation);
        file.ReadUShort(nRate);

        SetMatchRate(nConstellation, nMatchConstellation, nRate);
    }
}

void ConstellationConfigManager::LoadWishWords(CFileStream &file)
{
    unsigned short nCount = 0;
    file.ReadUShort(nCount);

    for (unsigned short i = 0; i < nCount; i++)
    {
        unsigned short nSize = 0;
        std::string strWords;
        file.ReadUShort(nSize);
        file.ReadString(strWords, nSize);

        if (!strWords.empty())
        {
            m_vecWishWords.push_back(strWords);
        }
    }
}

bool ConstellationConfigManager::LoadQuestion(CFileStream &file)
{
    unsigned short nCount = 0;
    file.ReadUShort(nCount);

    for (unsigned short i = 0; i < nCount; i++)
    {
        ConstellationConfig question;
        question.Load(file);
        m_vecQuestion.push_back(question);
    }

    return true;
}

bool ConstellationConfigManager::Save(const char *strPath)
{
    CFileStream file;
    file.open( strPath, "wb" );
    return Save(file);
}

bool ConstellationConfigManager::Save(CFileStream &file)
{
    if (!file.IsOpen())
    {
        return false;
    }

    SaveMatchRate(file);
    SaveWishWords(file);
    SaveQuestion(file);

    //file.close();

    return true;
}


void ConstellationConfigManager::SaveMatchRate(CFileStream &file)
{
    file.WriteUShort(MAX_CONSTELLATION_COUNT * MAX_CONSTELLATION_COUNT);
    for (unsigned short i = 0; i < MAX_CONSTELLATION_COUNT; i++)
    {
        for (unsigned short j = 0; j < MAX_CONSTELLATION_COUNT; j++)
        {
            file.WriteUShort(i);
            file.WriteUShort(j);
            file.WriteUShort((unsigned short)m_szMatchRate[i][j]);
        }
    }
}

void ConstellationConfigManager::SaveWishWords(CFileStream &file)
{
    file.WriteUShort((unsigned short)m_vecWishWords.size());
    for (size_t i = 0; i < m_vecWishWords.size(); i++)
    {
        file.WriteUShort((unsigned short)m_vecWishWords[i].length());
        file.WriteString(m_vecWishWords[i]);
    }
}

void ConstellationConfigManager::SaveQuestion(CFileStream &file)
{
    file.WriteUShort((unsigned short)m_vecQuestion.size());
    for (size_t i = 0; i < m_vecQuestion.size(); i++)
    {
        m_vecQuestion[i].Save(file);
    }
}

void ConstellationConfigManager::Destroy()
{
    memset(m_szMatchRate, 0, sizeof(m_szMatchRate));
    m_vecWishWords.clear();
    m_vecQuestion.clear();
}

void ConstellationConfigManager::AddWishWords(const char *strWords)
{
    std::string words = strWords;
    m_vecWishWords.push_back(words);
}

void ConstellationConfigManager::SetMatchRate(int nConstellation, int nMatchConstellation, int nRate)
{
    if (nConstellation >= 0 && nMatchConstellation >= 0
        && nConstellation < MAX_CONSTELLATION_COUNT
        && nMatchConstellation < MAX_CONSTELLATION_COUNT)
    {
        m_szMatchRate[nConstellation][nMatchConstellation] = nRate;
    }
}

void ConstellationConfigManager::AddQuestion(ConstellationConfig &question)
{
    m_vecQuestion.push_back(question);
}

const std::vector<std::string> &ConstellationConfigManager::WishWordsList() const
{
    return m_vecWishWords;
}

const int *ConstellationConfigManager::MatchRateList() const
{
    return m_szMatchRate[0];
}

void ConstellationConfigManager::AllQuestion(std::vector<ConstellationConfig> &vecQuestion)
{
    vecQuestion = m_vecQuestion;
}



