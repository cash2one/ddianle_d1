#include "ConstellationConfig.h"
#include <string.h>

#include "FileStream.h"
#include "../../socket/ParamPool.h"



ConstellationConfig::ConstellationConfig()
{
}

ConstellationConfig::~ConstellationConfig()
{
}

bool ConstellationConfig::Load(CFileStream &file)
{
    unsigned short nSize = 0;
    file.ReadUShort(nSize);
    file.ReadString(m_strQuestion, nSize);

    unsigned char nQuestionCount = 0;
    file.ReadUChar(nQuestionCount);
    for (int i = 0; i < (int)nQuestionCount; i++)
    {
        nSize = 0;
        std::string strAnswer;
        file.ReadUShort(nSize);
        file.ReadString(strAnswer, nSize);

        m_vecAnswer.push_back(strAnswer);
    }

    return true;
}

bool ConstellationConfig::Save(CFileStream &file)
{
    file.WriteUShort((unsigned short)m_strQuestion.size());
    file.WriteString(m_strQuestion);

    file.WriteUChar((unsigned char)m_vecAnswer.size());
    for (size_t i = 0; i < m_vecAnswer.size(); i++)
    {
        file.WriteUShort((unsigned short)m_vecAnswer[i].length());
        file.WriteString(m_vecAnswer[i]);
    }

    return true;
}


void ConstellationConfig::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddStringW(m_strQuestion.c_str());
    IOBuff.AddUShort((unsigned short)m_vecAnswer.size());
    for (size_t i = 0; i < m_vecAnswer.size(); i++)
    {
        IOBuff.AddStringW(m_vecAnswer[i].c_str());
    }
}

void ConstellationConfig::doDecode(CParamPool &IOBuff)
{
    IOBuff.GetStringW(m_strQuestion);
    unsigned short nAnswerCount = IOBuff.GetUShort();
    for (unsigned short i = 0; i < nAnswerCount; i++)
    {
        std::string strAnswer;
        IOBuff.GetStringW(strAnswer);
        m_vecAnswer.push_back(strAnswer);
    }
}



