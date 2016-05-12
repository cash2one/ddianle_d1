#include "ParamPool.h"
#include "Utility.h"
#include <stdexcept>

CParamPool::CParamPool(char * pMemory, int nSize)
{
    m_bNeedDelete = false;
    m_pMemory = NULL;
    Attach(pMemory, nSize);
    m_nMemorySize = nSize;
}

CParamPool::CParamPool(const CParamPool& _src, unsigned int _nPosition, unsigned int _nDataSize)
{
    m_bNeedDelete = false;
    Attach(_src.m_pMemory + _nPosition, _nDataSize);
}

CParamPool::~CParamPool(void)
{
    if (m_bNeedDelete)
        delete[]m_pMemory, m_pMemory = NULL;
}

void CParamPool::Attach(char * pMemory, int nSize)
{
    if (m_bNeedDelete)
        delete[]m_pMemory, m_pMemory = NULL;
    m_nMemorySize = nSize;
    if (pMemory)
    {
        m_pMemory = pMemory;
        m_bNeedDelete = false;
    }
    else
    {
        m_pMemory = new char[nSize];
        m_bNeedDelete = true;

    }
    m_pchCurPos = m_pMemory;
    m_nDataSize = 0;
    m_pMaxMemoryPtr = m_pMemory + m_nMemorySize;
}

//在开始读取之前，调用这个函数来重新设置当前位置
void CParamPool::Reset()
{
    m_pchCurPos = m_pMemory;
}

void CParamPool::ClearData()//清除所有数据
{
    m_pchCurPos = m_pMemory;
    m_nDataSize = 0;
}

bool CParamPool::AddBool(bool bValue)
{
    char chValue = 0;
    if (bValue)
        chValue = 1;
    return AddUChar(chValue);
}

const bool CParamPool::GetBool()
{
    char chValue = GetUChar();
    return chValue != 0;
}

bool CParamPool::AddUChar(unsigned char chValue)
{
    if (m_nDataSize + sizeof(unsigned char) > m_nMemorySize)
        return false;
    *m_pchCurPos = chValue;
    m_nDataSize += sizeof(unsigned char);
    m_pchCurPos += sizeof(unsigned char);
    return true;
}

bool CParamPool::AddChar(char chValue)
{
    if (m_nDataSize + sizeof(char) > m_nMemorySize)
        return false;
    *m_pchCurPos = chValue;
    m_nDataSize += sizeof(char);
    m_pchCurPos += sizeof(char);
    return true;
}

const void CParamPool::_SpaceCheck(int nSize) //throw(overflow_error)
{
    if (m_pchCurPos + nSize > m_pMaxMemoryPtr)
        throw overflow_error("ParamPool overflow ");
}

const unsigned char& CParamPool::GetUChar()
{
    _SpaceCheck(sizeof(unsigned char));
    m_pchCurPos += sizeof(unsigned char);
    return *(unsigned char*)(m_pchCurPos - sizeof(unsigned char));
}

bool CParamPool::ReplaceShort(short nValue)
{
    if (GetCurPosition() + sizeof(short) > m_nMemorySize)
        return false;
    *((short*)m_pchCurPos) = nValue;
    m_pchCurPos += sizeof(short);
    return true;
}

bool CParamPool::ReplaceUChar(unsigned char nValue)
{
    if (GetCurPosition() + sizeof(unsigned char) > m_nMemorySize)
        return false;
    *((unsigned char*)m_pchCurPos) = nValue;
    m_pchCurPos += sizeof(char);
    return true;
}

std::string CParamPool::GetBuffString()
{
    if (NULL == GetData() || 0 == GetValidDataSize())
        return "";
    return std::string(GetData(), GetValidDataSize());
}

bool CParamPool::AddShort(short nValue)
{
    if (m_nDataSize + sizeof(short) > m_nMemorySize)
        return false;
    *((short*)m_pchCurPos) = nValue;
    m_nDataSize += sizeof(short);
    m_pchCurPos += sizeof(short);
    return true;
}

const short& CParamPool::GetShort()
{
    _SpaceCheck(sizeof(short));
    m_pchCurPos += sizeof(short);
    return *(short*)(m_pchCurPos - sizeof(short));
}

bool CParamPool::AddUShort(unsigned short nValue)
{
    if (m_nDataSize + sizeof(unsigned short) > m_nMemorySize)
        return false;
    *((unsigned short*)m_pchCurPos) = nValue;
    m_nDataSize += sizeof(unsigned short);
    m_pchCurPos += sizeof(unsigned short);
    return true;
}

const unsigned short& CParamPool::GetUShort()
{
    _SpaceCheck(sizeof(unsigned short));
    m_pchCurPos += sizeof(unsigned short);
    return *(unsigned short*)(m_pchCurPos - sizeof(unsigned short));
}

bool CParamPool::AddInt(int nValue)
{
    if (m_nDataSize + sizeof(int) > m_nMemorySize)
        return false;
    *((int*)m_pchCurPos) = nValue;
    m_nDataSize += sizeof(int);
    m_pchCurPos += sizeof(int);
    return true;
}

const int& CParamPool::GetInt()
{
    _SpaceCheck(sizeof(int));
    m_pchCurPos += sizeof(int);
    return *(int*)(m_pchCurPos - sizeof(int));
}

bool CParamPool::AddUInt(unsigned int nValue)
{
    if (m_nDataSize + sizeof(unsigned int) > m_nMemorySize)
        return false;
    *((unsigned int*)m_pchCurPos) = nValue;
    m_nDataSize += sizeof(unsigned int);
    m_pchCurPos += sizeof(unsigned int);
    return true;
}

const unsigned int& CParamPool::GetUInt()
{
    _SpaceCheck(sizeof(unsigned int));
    m_pchCurPos += sizeof(unsigned int);
    return *(unsigned int*)(m_pchCurPos - sizeof(unsigned int));
}

bool CParamPool::AddFloat(float fValue)
{
    if (m_nDataSize + sizeof(float) > m_nMemorySize)
        return false;
    *((float*)m_pchCurPos) = fValue;
    m_nDataSize += sizeof(float);
    m_pchCurPos += sizeof(float);
    return true;
}

const float& CParamPool::GetFloat()
{
    _SpaceCheck(sizeof(float));
    m_pchCurPos += sizeof(float);
    return *(float*)(m_pchCurPos - sizeof(float));
}

bool CParamPool::AddInt64(__int64 nValue)
{
    if (m_nDataSize + sizeof(__int64) > m_nMemorySize)
        return false;
    *((__int64*)m_pchCurPos) = nValue;
    m_nDataSize += sizeof(__int64);
    m_pchCurPos += sizeof(__int64);
    return true;
}

const __int64& CParamPool::GetInt64()
{
    _SpaceCheck(sizeof(__int64));
    m_pchCurPos += sizeof(__int64);
    return *(__int64*)(m_pchCurPos - sizeof(__int64));
}


bool CParamPool::AddPerFixSizeMemory(const void * pchData, unsigned short nSize)
{
    if (m_nDataSize + nSize + sizeof(short) > m_nMemorySize)
        return false;
    *((short*)m_pchCurPos) = nSize;
    m_nDataSize += sizeof(short);
    m_pchCurPos += sizeof(short);
    if (nSize > 0)
    {
        memcpy(m_pchCurPos, pchData, nSize);
        m_nDataSize += nSize;
        m_pchCurPos += nSize;
    }
    return true;
}

bool CParamPool::AddMemory(const void * pchData, unsigned short nSize)
{
    if (m_nDataSize + nSize > m_nMemorySize)
        return false;
    if (nSize > 0)
    {
        memcpy(m_pchCurPos, pchData, nSize);
        m_nDataSize += nSize;
        m_pchCurPos += nSize;
    }
    return true;
}

bool CParamPool::GetPerFixSizeMemory(void * pchData, unsigned short &nSize)
{
    _SpaceCheck(sizeof(unsigned short));
    nSize = *((short*)m_pchCurPos);
    m_pchCurPos += sizeof(short);

    if (nSize > 0)
    {
        _SpaceCheck(nSize);
        memcpy(pchData, m_pchCurPos, nSize);
        m_pchCurPos += nSize;

    }
    return true;
}

bool CParamPool::GetPerFixSizeMemory(std::string &strBuff)
{
    _SpaceCheck(sizeof(unsigned short));
    unsigned short nSize = *((short*)m_pchCurPos);
    m_pchCurPos += sizeof(short);

    if (nSize > 0)
    {
        _SpaceCheck(nSize);
        strBuff.assign(m_pchCurPos, nSize);
        m_pchCurPos += nSize;
    }
    return true;
}


bool CParamPool::GetMemory(void * pchData, unsigned short nSize)
{

    if (nSize > 0)//&& m_pchCurPos+nSize-m_pMemory<=m_nMemorySize
    {
        _SpaceCheck(sizeof(nSize));
        memcpy(pchData, m_pchCurPos, nSize);
        m_pchCurPos += nSize;
    }
    else
    {
        return false;
    }
    return true;
}

bool CParamPool::AddString(const char * pszValue)
{
    //unsigned short nLen = ( unsigned short )strlen(pszValue);
    //AddUShort( nLen );
    //if( nLen > 0 )
    //{
    //	AddMemory( pszValue,nLen );
    //}
    unsigned long nstrLen = (unsigned long)strlen(pszValue);
    if (m_nDataSize + nstrLen + sizeof(char) > m_nMemorySize)
        return false;

    if (nstrLen > 0)
    {
        strcpy(m_pchCurPos, pszValue);
    }
    else
    {
        *m_pchCurPos = 0;
    }
    nstrLen += sizeof(char);//NULL
    m_nDataSize += nstrLen;
    m_pchCurPos += nstrLen;
    return true;
}

bool CParamPool::AddStringW(const char * pszValue)
{
    unsigned short nLen = 0;
    if (pszValue != NULL){
        nLen = (unsigned short)strlen(pszValue);
    }
    AddUShort(nLen);
    if (nLen > 0){
        return AddMemory(pszValue, nLen);
    }
    return true;
}

bool CParamPool::AddString(const std::string & strResult)
{
    return AddString(strResult.c_str());
}

bool CParamPool::AddStringW(const std::string & strResult)
{
    return AddStringW(strResult.c_str());
}

char* CParamPool::GetString()
{
    _SpaceCheck(sizeof(char));

    char*pchCurPosTemp = m_pchCurPos;
    int nstrLen = (int)strlen(pchCurPosTemp) + sizeof(char);
    m_pchCurPos += nstrLen;
    return pchCurPosTemp;
}

void CParamPool::GetString(std::string & strresult)
{
    strresult = GetString();
}

void CParamPool::GetStringW(std::string & strresult)
{
    strresult = "";
    unsigned short nLen = GetUShort();
    if (nLen > 0)
    {
        _SpaceCheck(nLen);
        char*pchCurPosTemp = m_pchCurPos;

        char * pTemp = new char[nLen + 1];
        memset(pTemp, 0, nLen + 1);
        memcpy(pTemp, pchCurPosTemp, nLen);
        strresult = pTemp;
        delete[]pTemp;
        int nstrLen = (int)nLen;
        m_pchCurPos += nstrLen;
    }
}

unsigned int CParamPool::GetValidDataSize()//打包数据的时候,得到打进去的数据长度
{
    return m_nDataSize;
}

unsigned int CParamPool::GetCurPosition()//得到当前位置相对于起始的位置
{
    return (unsigned int)(m_pchCurPos - m_pMemory);
}

void CParamPool::SetCurPosition(unsigned int Pos)
{
    m_pchCurPos = m_pMemory + Pos;
}

const char* CParamPool::GetData()
{
    return m_pMemory;
}

bool CParamPool::HasRemaining()
{
    return (m_pchCurPos < m_pMaxMemoryPtr);
}

int CParamPool::Remaining()
{
    return (int)(m_pMaxMemoryPtr - m_pchCurPos);
}

void CParamPool::Settle()
{
    if (m_pMemory)
    {
        if (m_pchCurPos < m_pMemory + m_nMemorySize)
        {
            memcpy(m_pMemory, m_pchCurPos, Remaining());
            m_nMemorySize = Remaining();
            m_pchCurPos = m_pMemory;
            m_pMaxMemoryPtr = m_pMemory + m_nMemorySize;
        }
        else
        {
            ClearData();
            m_nMemorySize = 0;
        }
    }
}

char * CParamPool::CloneCurrentBuffer()
{
    char * pResult = NULL;
    if (m_nDataSize > 0)
    {
        pResult = new char[m_nDataSize];
        memset(pResult, 0, m_nDataSize);
        memcpy(pResult, m_pMemory, m_nDataSize);
    }
    return pResult;
}
