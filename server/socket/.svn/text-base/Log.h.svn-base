#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#ifdef WIN32
#ifndef NTDDI_WIN7SP1
#define NTDDI_WIN7SP1 0x06010100
#endif
#include <winsock2.h>
#endif
#include <time.h>
#ifdef WIN32
#ifndef NTDDI_WIN7SP1
#define NTDDI_WIN7SP1 0x06010100
#endif
#include <windows.h>



#else
#include <pthread.h>
#endif

#include <map>
#include <string>
#include <ostream>

#define LOGMODE_STDOUT		0x0001	//输出到标准输出
#define LOGMODE_STDERR		0x0002	//输出到标准错误
#define LOGMODE_FILE		0x0004	//输出到文件
#define LOGMODE_SYSLOG		0x0008	//输出到系统log,暂未实现
#define LOGMODE_YEAR		0x0100	//输出的文本中带年份
#define LOGMODE_CR			0x0200	//输出的文本后带回车换行(如果自身没有的话)
#define LOGMODE_TRUNCATE	0x1000	//每天自动生成新的文件，此标志跟LOGMODE_FILE一起使用时，szFileName参数是文件名的前缀，自动在后面加.date.log

enum LOGLEVEL
{
    LOGLEVEL_NONE = 0,
    LOGLEVEL_TRACE = 1,
    LOGLEVEL_DEBUG,
    LOGLEVEL_WARNING,
    LOGLEVEL_ERROR,
    LOGLEVEL_SYSTEM,

    LOGLEVEL_MAX,
};


#ifndef MAX_PATH
#define MAX_PATH	260
#endif
#ifndef MAX_LOG
#define MAX_LOG		10240
#endif
class CLog
{
public:
    CLog();
    ~CLog();

    int SetLogMode(int nLogMode = LOGMODE_STDOUT, const char* szFileName = NULL);
    void Write(const char* file, const char* function, const char* lpszFmt, ...);
    void Write(LOGLEVEL level, const char* lpszFmt, ...);
    void Write(const char* file, const char* function, LOGLEVEL level, const char* lpszFmt, ...);

    CLog& operator << (std::ostream& (*op) (std::ostream&));
    CLog& operator << (LOGLEVEL level);
    CLog& operator << (int n);
    CLog& operator << (unsigned int n);
    CLog& operator << (short n);
    CLog& operator << (unsigned short n);
    CLog& operator << (char c);
    CLog& operator << (unsigned char c);
    CLog& operator << (const char* str);
    CLog& operator << (const std::string& str);
    CLog& operator << (float f);
    CLog& operator << (double d);
    CLog& operator << (const void* p);
    CLog& operator << (const int* p);
    CLog& operator << (long n);
    CLog& operator << (bool b);

private:
    void Write(LOGLEVEL level, const char* lpszOutString, int mDay);
    void EndWrite();

protected:
    int m_nMode;
    char m_szFileName[MAX_PATH];
    char m_szBuff[MAX_LOG];
    FILE* m_pFile;
    tm tmLast;
    LOGLEVEL m_nLogLevel;
#ifdef WIN32
    CRITICAL_SECTION m_lock;
    CRITICAL_SECTION m_StreamLock;
#else
    pthread_mutex_t m_lock;
    pthread_mutex_t m_StreamLock;
#endif

private:
    static const char *m_LogLevelString[LOGLEVEL_MAX];
};

extern CLog g_logDefault;
extern CLog g_logCore;
extern CLog g_DBQueryExceptionLog;
extern int g_DbWarningProcessTime;
extern int g_DbWarningWaitTime;
extern int g_DbWarningQueueSize;

#define WriteLog(level,fmt,...)	g_logDefault.Write(__FILE__,__FUNCTION__,level,fmt,##__VA_ARGS__)
#define WriteLogCore(level,fmt,...)	g_logCore.Write(__FILE__,__FUNCTION__,level,fmt,##__VA_ARGS__)
#define WriteLogDB(level,fmt,...)	g_DBLog.Write(__FILE__,__FUNCTION__,level,fmt,##__VA_ARGS__)

#define WriteQueryExceptionLog(level,fmt,...) g_DBQueryExceptionLog.Write(__FILE__,__FUNCTION__,level,fmt,##__VA_ARGS__)


#define LOG(lv)     g_logDefault<<##lv
#define LOG_TRACE   g_logDefault << LOGLEVEL_TRACE
#define LOG_DEBUG   g_logDefault << LOGLEVEL_DEBUG
#define LOG_WARNING g_logDefault << LOGLEVEL_WARNING
#define LOG_ERROR   g_logDefault << LOGLEVEL_ERROR
#define LOG_SYSTEM  g_logDefault << LOGLEVEL_SYSTEM


#define ENSURE_WITH_LOG_CMD(stat, cmd, fmt, ...)\
if (!(stat))\
{\
    WriteLog(LOGLEVEL_ERROR, fmt, ##__VA_ARGS__); \
    cmd; \
}
void todo();

#endif//__LOG_H__
