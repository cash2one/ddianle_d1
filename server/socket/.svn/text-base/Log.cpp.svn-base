#include "Log.h"
#include <stdarg.h>
#include <string.h>

#ifdef WIN32
#pragma warning(disable:4996)

#define Lock(x)			EnterCriticalSection(x)
#define Unlock(x)		LeaveCriticalSection(x)
#define vsnprintf	_vsnprintf
#else
#include <syslog.h>
#define Lock(x)			pthread_mutex_lock(x)
#define Unlock(x)		pthread_mutex_unlock(x)
#endif

CLog g_logDefault;
CLog g_logCore;
CLog g_DBLog;
CLog g_DBQueryExceptionLog;

//Db处理SQL的超过最大时间警告  阀值设置 
int g_DbWarningProcessTime;
//Db处理消息 超过最大等待时间警告  阀值设置
int g_DbWarningWaitTime;
//Db处理消息 队列超过最大数目  阀值设置
int g_DbWarningQueueSize;

const char *CLog::m_LogLevelString[LOGLEVEL_MAX] = {
    "[NONE]",
    "[TRACE]",
    "[INFO]",
    "[WARN]",
    "[ERROR]",
    "[SYSTEM]",
};

CLog::CLog()
{
    m_nMode = 0;
    m_pFile = NULL;

#ifdef WIN32
    InitializeCriticalSection(&m_lock);
    InitializeCriticalSection(&m_StreamLock);
#else
    pthread_mutex_init(&m_lock, NULL);
    pthread_mutex_init(&m_StreamLock, NULL);
#endif
}

CLog::~CLog()
{
    if (m_pFile)
    {
        fclose(m_pFile);
        m_pFile = NULL;
    }

#ifdef WIN32
    DeleteCriticalSection(&m_lock);
    DeleteCriticalSection(&m_StreamLock);
#else
    pthread_mutex_destroy(&m_lock);
    pthread_mutex_destroy(&m_StreamLock);
#endif

}

int CLog::SetLogMode(int nLogMode /* = LOGMODE_STDOUT */, const char* szFileName /* = NULL */)
{
    m_nMode = nLogMode;
    if ((m_nMode&LOGMODE_FILE))
    {
        time_t now = time(NULL);
        tm* pNow = localtime(&now);
        tmLast.tm_mday = pNow->tm_mday;
        char szBuff[MAX_PATH];
        if (szFileName == NULL || strlen(szFileName) == 0)
        {
            strcpy(m_szFileName, "./log");
        }
        else
        {
            if (szFileName != m_szFileName)
                strcpy(m_szFileName, szFileName);
        }
        if (m_nMode & LOGMODE_TRUNCATE)
        {
            if (m_nMode & LOGMODE_YEAR)
                sprintf(szBuff, "%s.%04d-%02d-%02d.log", m_szFileName, 1900 + pNow->tm_year, 1 + pNow->tm_mon, pNow->tm_mday);
            else
                sprintf(szBuff, "%s.%02d-%02d.log", m_szFileName, 1 + pNow->tm_mon, pNow->tm_mday);
        }
        else
        {
            strcpy(szBuff, m_szFileName);
        }

        m_pFile = fopen(szBuff, "a+");
        if (m_pFile == NULL)m_nMode &= ~LOGMODE_FILE;
    }

    return m_nMode;
}

void CLog::Write(LOGLEVEL level, const char* lpszOutString, int mDay)
{
    if (level <= LOGLEVEL_NONE || level >= LOGLEVEL_MAX)
    {
        return;
    }

    Lock(&m_lock);
    if (level >= LOGLEVEL_TRACE)
    {
        if (m_nMode & LOGMODE_STDOUT)
        {
            fwrite(lpszOutString, strlen(lpszOutString), 1, stdout);
            fflush(stdout);
        }
    }
    if (m_nMode & LOGMODE_STDERR)
    {
        fwrite(lpszOutString, strlen(lpszOutString), 1, stderr);
        fflush(stderr);
    }
    if ((m_nMode & LOGMODE_FILE) && (m_nMode & LOGMODE_TRUNCATE) && tmLast.tm_mday != mDay)
    {
        //tmLast.tm_mday = pNow->tm_mday;
        fclose(m_pFile); m_pFile = NULL;
        SetLogMode(m_nMode, m_szFileName);
    }
    if ((m_nMode & LOGMODE_FILE))
    {
        fwrite(lpszOutString, strlen(lpszOutString), 1, m_pFile);
        fflush(m_pFile);
    }
    if (m_nMode & LOGMODE_SYSLOG)
    {
#ifdef WIN32
#else
        //syslog()
#endif
    }
    Unlock(&m_lock);
}

void CLog::Write(LOGLEVEL level, const char* lpszFmt, ...)
{
    if (level <= LOGLEVEL_NONE || level >= LOGLEVEL_MAX)
    {
        return;
    }

    char szBuff[MAX_LOG] = { 0 };

    if (m_nMode == 0)
    {
        //SetLogMode(LOGMODE_FILE|LOGMODE_STDOUT|LOGMODE_CR);
        return;
    }
    if (lpszFmt)
    {
        time_t now = time(NULL);
        tm* pNow = localtime(&now);
        if (m_nMode & LOGMODE_YEAR)
            sprintf(szBuff, "%s[%04d-%02d-%02d %02d:%02d:%02d]", m_LogLevelString[level],
            1900 + pNow->tm_year, 1 + pNow->tm_mon, pNow->tm_mday, pNow->tm_hour, pNow->tm_min, pNow->tm_sec);
        else
            sprintf(szBuff, "%s[%02d-%02d %02d:%02d:%02d]", m_LogLevelString[level],
            1 + pNow->tm_mon, pNow->tm_mday, pNow->tm_hour, pNow->tm_min, pNow->tm_sec);
        size_t nLen = strlen(szBuff);
        va_list argList;
        va_start(argList, lpszFmt);
        vsnprintf(szBuff + nLen, MAX_LOG - nLen - 3, lpszFmt, argList);
        va_end(argList);

#ifdef WIN32
#else
        //将文件转为DOS格式的文件
        char* ptrEnd = szBuff + strlen(szBuff) - 1;
        if (*ptrEnd == 0x0A)
        {
            *ptrEnd++ = 0x0D;
            *ptrEnd++ = 0x0A;
            *ptrEnd++ = 0;
        }
#endif
        if (LOGMODE_CR)
        {
            char* ptrEnd = szBuff + strlen(szBuff) - 1;

            if (*ptrEnd != 0x0A)
            {
                ptrEnd++;
                *ptrEnd++ = 0x0D;
                *ptrEnd++ = 0x0A;
                *ptrEnd++ = 0;
            }
        }
        Write(level, szBuff, pNow->tm_mday);
    }
}

void CLog::Write(const char* file, const char* function, const char* lpszFmt, ...)
{
    char szBuff[MAX_LOG] = { 0 };
    LOGLEVEL level = LOGLEVEL_DEBUG;

    if (m_nMode == 0)
    {
        //SetLogMode(LOGMODE_FILE|LOGMODE_STDOUT|LOGMODE_CR);
        return;
    }
    if (lpszFmt)
    {
        time_t now = time(NULL);
        tm* pNow = localtime(&now);
        if (m_nMode & LOGMODE_YEAR)
            sprintf(szBuff, "%s[%04d-%02d-%02d %02d:%02d:%02d]", m_LogLevelString[level],
            1900 + pNow->tm_year, 1 + pNow->tm_mon, pNow->tm_mday, pNow->tm_hour, pNow->tm_min, pNow->tm_sec);
        else
            sprintf(szBuff, "%s[%02d-%02d %02d:%02d:%02d]", m_LogLevelString[level],
            1 + pNow->tm_mon, pNow->tm_mday, pNow->tm_hour, pNow->tm_min, pNow->tm_sec);
        size_t nLen = strlen(szBuff);
        va_list argList;
        va_start(argList, lpszFmt);
        vsnprintf(szBuff + nLen, MAX_LOG - nLen - 4, lpszFmt, argList);
        va_end(argList);

        if (level == LOGLEVEL_TRACE)
        {
            nLen = strlen(szBuff);
            char szFunctionInfo[1024];
            sprintf(szFunctionInfo, " , file : %s , function : %s . ", file, function);
            if (strlen(szFunctionInfo) + nLen + 4 < MAX_LOG)
            {
                sprintf(szBuff + nLen, "%s", szFunctionInfo);
            }
        }
#ifdef WIN32
#else
        //将文件转为DOS格式的文件
        char* ptrEnd = szBuff + strlen(szBuff) - 1;
        if (*ptrEnd == 0x0A)
        {
            *ptrEnd++ = 0x0D;
            *ptrEnd++ = 0x0A;
            *ptrEnd++ = 0;
        }
#endif
        if (LOGMODE_CR)
        {
            char* ptrEnd = szBuff + strlen(szBuff) - 1;

            if (*ptrEnd != 0x0A)
            {
                ptrEnd++;
                *ptrEnd++ = 0x0D;
                *ptrEnd++ = 0x0A;
                *ptrEnd++ = 0;
            }
        }
        Write(level, szBuff, pNow->tm_mday);
    }
}

void CLog::Write(const char* file, const char* function, LOGLEVEL level, const char* lpszFmt, ...)
{
    if (level <= LOGLEVEL_NONE || level >= LOGLEVEL_MAX)
    {
        return;
    }

    char szBuff[MAX_LOG] = { 0 };

    if (m_nMode == 0)
    {
        //SetLogMode(LOGMODE_FILE|LOGMODE_STDOUT|LOGMODE_CR);
        return;
    }
    if (lpszFmt)
    {
        time_t now = time(NULL);
        tm* pNow = localtime(&now);
        if (m_nMode & LOGMODE_YEAR)
            sprintf(szBuff, "%s[%04d-%02d-%02d %02d:%02d:%02d]", m_LogLevelString[level],
            1900 + pNow->tm_year, 1 + pNow->tm_mon, pNow->tm_mday, pNow->tm_hour, pNow->tm_min, pNow->tm_sec);
        else
            sprintf(szBuff, "%s[%02d-%02d %02d:%02d:%02d]", m_LogLevelString[level],
            1 + pNow->tm_mon, pNow->tm_mday, pNow->tm_hour, pNow->tm_min, pNow->tm_sec);
        size_t nLen = strlen(szBuff);
        va_list argList;
        va_start(argList, lpszFmt);
        vsnprintf(szBuff + nLen, MAX_LOG - nLen - 4, lpszFmt, argList);
        va_end(argList);
        if (level == LOGLEVEL_TRACE)
        {
            nLen = strlen(szBuff);
            char szFunctionInfo[1024];
            sprintf(szFunctionInfo, " , file : %s , function : %s . ", file, function);
            if (strlen(szFunctionInfo) + nLen + 4 < MAX_LOG)
            {
                sprintf(szBuff + nLen, "%s", szFunctionInfo);
            }
        }

#ifdef WIN32
#else
        //将文件转为DOS格式的文件
        char* ptrEnd = szBuff + strlen(szBuff) - 1;
        if (*ptrEnd == 0x0A)
        {
            *ptrEnd++ = 0x0D;
            *ptrEnd++ = 0x0A;
            *ptrEnd++ = 0;
        }
#endif
        if (m_nMode & LOGMODE_CR)
        {
            char* ptrEnd = szBuff + strlen(szBuff) - 1;

            if (*ptrEnd != 0x0A)
            {
                ptrEnd++;
                *ptrEnd++ = 0x0D;
                *ptrEnd++ = 0x0A;
                *ptrEnd++ = 0;
            }
        }
        Write(level, szBuff, pNow->tm_mday);
    }
}

void CLog::EndWrite()
{
    //结束输入，输出当前buff的内容到公共资源中，然后清空buff内容
    time_t now = time(NULL);
    tm* pNow = localtime(&now);
    Write(m_nLogLevel, m_szBuff, pNow->tm_mday);
    memset(m_szBuff, 0, MAX_LOG);
    m_nLogLevel = LOGLEVEL_NONE;

    //Unlock(&m_StreamLock);
}

CLog& CLog::operator<< (std::ostream& (*op) (std::ostream&))
{
    int nLen = strlen(m_szBuff);
    if (nLen + 1 < MAX_LOG)
    {
        sprintf(m_szBuff + nLen, "\n");
    }
    EndWrite();
    return *this;
}

CLog& CLog::operator << (LOGLEVEL level)
{
    if (level <= LOGLEVEL_NONE || level >= LOGLEVEL_MAX)
    {
        return *this;
    }

    //Lock(&m_StreamLock);

    m_nLogLevel = level;
    int nLen = strlen(m_szBuff);

    if (nLen + 40 >= MAX_LOG)
    {
        EndWrite();
        return *this;
    }

    time_t now = time(NULL);
    tm* pNow = localtime(&now);
    if (m_nMode & LOGMODE_YEAR)
        sprintf(m_szBuff, "%s[%04d-%02d-%02d %02d:%02d:%02d]", m_LogLevelString[level],
        1900 + pNow->tm_year, 1 + pNow->tm_mon, pNow->tm_mday, pNow->tm_hour, pNow->tm_min, pNow->tm_sec);
    else
        sprintf(m_szBuff, "%s[%02d-%02d %02d:%02d:%02d]", m_LogLevelString[level],
        1 + pNow->tm_mon, pNow->tm_mday, pNow->tm_hour, pNow->tm_min, pNow->tm_sec);

    return *this;
}

CLog& CLog::operator << (int n)
{
    int nLen = strlen(m_szBuff);
    if (nLen + 12 >= MAX_LOG)
    {
        EndWrite();
        return *this;
    }
    sprintf(m_szBuff + nLen, "%d", n);
    return *this;
}

CLog& CLog::operator << (unsigned int n)
{
    int nLen = strlen(m_szBuff);
    if (nLen + 12 >= MAX_LOG)
    {
        EndWrite();
        return *this;
    }
    sprintf(m_szBuff + nLen, "%u", n);
    return *this;
}

CLog& CLog::operator << (short n)
{
    int nLen = strlen(m_szBuff);
    if (nLen + 6 >= MAX_LOG)
    {
        EndWrite();
        return *this;
    }
    sprintf(m_szBuff + nLen, "%hd", n);
    return *this;
}

CLog& CLog::operator << (unsigned short n)
{
    int nLen = strlen(m_szBuff);
    if (nLen + 6 >= MAX_LOG)
    {
        EndWrite();
        return *this;
    }
    sprintf(m_szBuff + nLen, "%hu", n);
    return *this;
}

CLog& CLog::operator << (char c)
{
    int nLen = strlen(m_szBuff);
    if (nLen + 2 >= MAX_LOG)
    {
        EndWrite();
        return *this;
    }
    sprintf(m_szBuff + nLen, "%c", c);
    return *this;
}

CLog& CLog::operator << (unsigned char c)
{
    int nLen = strlen(m_szBuff);
    if (nLen + 2 >= MAX_LOG)
    {
        EndWrite();
        return *this;
    }
    sprintf(m_szBuff + nLen, "%c", c);
    return *this;
}

CLog& CLog::operator << (const char* str)
{
    int nLen = strlen(m_szBuff);
    int nInputLen = strlen(str);
    if (nLen + nInputLen >= MAX_LOG)
    {
        EndWrite();
        return *this;
    }
    sprintf(m_szBuff + nLen, "%s", str);
    return *this;
}

CLog& CLog::operator << (const std::string& str)
{
    int nLen = strlen(m_szBuff);
    int nInputLen = str.size();
    if (nLen + nInputLen >= MAX_LOG)
    {
        EndWrite();
        return *this;
    }
    sprintf(m_szBuff + nLen, "%s", str.c_str());
    return *this;
}

CLog& CLog::operator << (float f)
{
    int nLen = strlen(m_szBuff);
    if (nLen + 8 >= MAX_LOG)
    {
        EndWrite();
        return *this;
    }
    sprintf(m_szBuff + nLen, "%f", f);
    return *this;
}

CLog& CLog::operator << (double d)
{
    int nLen = strlen(m_szBuff);
    if (nLen + 20 >= MAX_LOG)
    {
        EndWrite();
        return *this;
    }
    sprintf(m_szBuff + nLen, "%lf", d);
    return *this;
}

CLog& CLog::operator << (const void* p)
{
    int nLen = strlen(m_szBuff);
    if (nLen + 11 >= MAX_LOG)
    {
        EndWrite();
        return *this;
    }
    sprintf(m_szBuff + nLen, "%p", p);
    return *this;
}

CLog& CLog::operator << (const int* p)
{
    int nLen = strlen(m_szBuff);
    if (nLen + 11 >= MAX_LOG)
    {
        EndWrite();
        return *this;
    }
    sprintf(m_szBuff + nLen, "%p", p);
    return *this;
}

CLog& CLog::operator << (long n)
{
    int nLen = strlen(m_szBuff);
    if (nLen + 11 >= MAX_LOG)
    {
        EndWrite();
        return *this;
    }
    sprintf(m_szBuff + nLen, "%ld", n);
    return *this;
}

CLog& CLog::operator << (bool b)
{
    int nLen = strlen(m_szBuff);
    if (nLen + 6 >= MAX_LOG)
    {
        EndWrite();
        return *this;
    }
    if (b)
    {
        sprintf(m_szBuff + nLen, "true");
    }
    else
    {
        sprintf(m_szBuff + nLen, "false");
    }
    return *this;
}

void todo()
{}

