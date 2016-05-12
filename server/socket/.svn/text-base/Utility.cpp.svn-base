#include "StdAfx.h"
#include <string.h>
#include <sys/timeb.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef WIN32
#pragma warning(disable:4996 4267 4101)
#ifndef FD_SETSIZE
#define FD_SETSIZE      1024
#endif /* FD_SETSIZE */
#ifndef NTDDI_WIN7SP1
#define NTDDI_WIN7SP1 0x06010100
#endif
#include <winsock2.h>
#else
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

//For init_daemon
#include <unistd.h> 
#include <signal.h> 
#include <sys/param.h> 
#include <sys/types.h> 
#include <sys/stat.h> 

//For GetHostAddr()
#include <stdio.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <net/if_arp.h>

#endif

#include "Utility.h"

CHost s_Host;

CHost::CHost()
{
#ifdef WIN32
    WSADATA wsaData;
    (void)WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    m_name[0] = 0;
    gethostname(m_name, HOST_NAME_MAX);
#ifdef WIN32
    hostent* pHost = gethostbyname(m_name);
    if (pHost != NULL)
    {
        int i = 0;
        while (pHost->h_addr_list[i] != NULL)
        {
            m_aAddr.push_back(((in_addr*)pHost->h_addr_list[i])->s_addr);
            i++;
        }
    }
#else

#define MAXINTERFACES   16

    int fd, intrface, retn = 0;
    struct ifreq buf[MAXINTERFACES];
    struct ifconf ifc;

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0)
    {
        ifc.ifc_len = sizeof(buf);
        ifc.ifc_buf = (caddr_t)buf;
        if (!ioctl(fd, SIOCGIFCONF, (char *)&ifc))
        {
            intrface = ifc.ifc_len / sizeof (struct ifreq);
            //printf("interface num is intrface=%d\n",intrface);
            while (intrface-- > 0)
            {
                //printf ("net device %s\n", buf[intrface].ifr_name);

                /*Jugde whether the net card status is promisc*/
                if (!(ioctl(fd, SIOCGIFFLAGS, (char *)&buf[intrface])))
                {
                    if (buf[intrface].ifr_flags & IFF_PROMISC)
                    {
                        //puts ("the interface is PROMISC");
                        retn++;
                    }
                }

                /*Jugde whether the net card status is up*/
                if (buf[intrface].ifr_flags & IFF_UP)
                {
                    //puts("the interface status is UP");
                }
                else
                {
                    //puts("the interface status is DOWN");
                }

                /*Get IP of the net card */
                if (!(ioctl(fd, SIOCGIFADDR, (char *)&buf[intrface])))
                {
                    //puts ("IP address is:");
                    //puts(inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr));
                    //puts("");
                    //puts (buf[intrface].ifr_addr.sa_data);

                    //如果不是127.0.0.1则加入到ip地址列表中
                    if (((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr.s_addr != inet_addr("127.0.0.1"))
                        m_aAddr.push_front(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr.s_addr);
                }

                /*Get HW ADDRESS of the net card */
                //if (!(ioctl (fd, SIOCGIFHWADDR, (char *) &buf[intrface])))
                //{
                //	puts ("HW address is:");

                //	printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                //		(unsigned char)buf[intrface].ifr_hwaddr.sa_data[0],
                //		(unsigned char)buf[intrface].ifr_hwaddr.sa_data[1],
                //		(unsigned char)buf[intrface].ifr_hwaddr.sa_data[2],
                //		(unsigned char)buf[intrface].ifr_hwaddr.sa_data[3],
                //		(unsigned char)buf[intrface].ifr_hwaddr.sa_data[4],
                //		(unsigned char)buf[intrface].ifr_hwaddr.sa_data[5]);

                //	puts("");
                //	puts("");
                //}
            }
        }
        //else
        //	perror ("cpm: ioctl");

    }
    //else
    //	perror ("cpm: socket");

    close(fd);
#endif
}

string CHost::GetHostAddr(unsigned int *addr /* = NULL */, unsigned int nEth)
{
    if (nEth < m_aAddr.size())
    {
        if (addr) *addr = m_aAddr[nEth];
        in_addr in; in.s_addr = m_aAddr[nEth];
        return inet_ntoa(in);
    }
    return "";
}

string CHost::GetHostName()
{
    return m_name;
}

bool CHost::HostNameEquals(const char* pszHostName)
{
#ifdef _WIN32
    return stricmp(m_name, pszHostName) == 0;
#else
    return strcasecmp(m_name, pszHostName) == 0;
#endif
}

string GetExecPath(char** argv)
{
    char szPath[MAX_PATH] = { 0 };

#ifdef WIN32
    if (GetModuleFileName(NULL, szPath, MAX_PATH) != 0)
    {
        char *ptr = strrchr(szPath, '\\');
        *(++ptr) = 0;
    }
#else
    if (argv[0][0] == '/')
    {
        strcpy(szPath, argv[0]);
    }
    else
    {
        getcwd(szPath, MAX_PATH);
        if (szPath[strlen(szPath) - 1] != '/')
        {
            strcat(szPath, "/");
        }
        strcat(szPath, argv[0]);
    }
    char *ptr = strrchr(szPath, '/');
    *(++ptr) = 0;
#endif

    return szPath;
}

//解析由;分割的字符串
bool ParseString(char* arg, list<string> &result)
{
    char buff[1024];	// big enough
    int sz;

    sz = 0;
    while ((*arg) != '\0')
    {
        if (sz >= 1023)
        {
            TRACE("ParseString error : stack overflow!\n");
            break;
        }

        switch (*arg)
        {
        case ';':	// 分割符号
        case '\n':
            if (sz > 0)
            {
                buff[sz] = '\0';
                result.push_back(buff);
            }
            sz = 0;
            ++arg;
            break;
        case ' ':	// 过滤掉空格字符
            ++arg;
            break;
        default:	// 其他普通字符
            buff[sz++] = *arg;
            ++arg;
            break;
        }
    }

    if (sz > 0)
    {
        buff[sz] = '\0';
        result.push_back(buff);
    }

    return result.size() > 0;
}

#ifdef _DEBUG
#ifdef WIN32
#define vsnprintf	_vsnprintf
#endif
void _Debug_Trace(const char* lpszFmt, ...)
{
    char buff[MAX_TRACE_STRING];
    if (lpszFmt)
    {
        va_list argList;
        va_start(argList, lpszFmt);
        //vfprintf(stdout, lpszFmt, argList);
        vsnprintf(buff, MAX_TRACE_STRING - 1, lpszFmt, argList);
        buff[MAX_TRACE_STRING - 1] = 0;
        va_end(argList);
        //fflush(stdout);
        fwrite(buff, strlen(buff), 1, stdout);
#ifdef WIN32
        OutputDebugString(buff);
#endif
    }
}
#else
void _Debug_Trace(const char* lpszFmt, ...)
{
}
#endif


struct _TickCount
{
    struct timeb tb;
    _TickCount()
    {
        ftime(&tb);
    }
};

static _TickCount s_tc;

#ifdef WIN32
#else
void init_daemon(void)
{
    pid_t pid;

    if ((pid = fork()) != 0)
        exit(0);//是父进程，结束父进程 
    else if (pid < 0)
        exit(1);//fork失败，退出 
    //是第一子进程，后台继续执行 

    setsid();//第一子进程成为新的会话组长和进程组长 
    //并与控制终端分离 
    if ((pid = fork()) != 0)
        exit(0);//是第一子进程，结束第一子进程 
    else if (pid < 0)
        exit(1);//fork失败，退出 
    //是第二子进程，继续 
    //第二子进程不再是会话组长 

    int i;
    for (i = 0; i < NOFILE; ++i)//关闭打开的文件描述符 
        close(i);
    chdir("/");//改变工作目录到/ 
    umask(0);//重设文件创建掩模 
    return;
}

unsigned int GetCurrentProcessId()
{
    return getpid();
}

unsigned int GetTickCount()
{
    struct timeb tbNow;
    ftime(&tbNow);
    return (tbNow.time - s_tc.tb.time) * 1000 + (tbNow.millitm - s_tc.tb.millitm);
}

void sig_empty(int nSignal)
{
}
#endif

unsigned int GetOctupleTickCount()
{
    struct timeb tbNow;
    ftime(&tbNow);
    return (unsigned int)((tbNow.time - s_tc.tb.time) * (1000 >> 3) + ((tbNow.millitm - s_tc.tb.millitm) >> 3));
}

unsigned int Random()
{
#ifdef WIN32
    return (rand() << 15) | rand();	//这个值最大为MAXSHORT*MAXSHORT,小于MAXLONG,大约是MAXLONG的一半
#else
    return rand();
#endif
}

//[nStart, nEnd]
int Random(int nStart, int nEnd)
{
    unsigned int nRN = Random();

    int nNum = nRN % (nEnd - nStart + 1);
    nNum = nNum > 0 ? nNum : -nNum;

    return nNum + nStart;
}

int RateRandom(const std::vector<int> & rRateVec)
{
    int nRateIndex = -1;

    if (rRateVec.size() > 0)
    {
        int nRadix = 0;

        for (std::vector< int >::const_iterator it = rRateVec.begin(); it != rRateVec.end(); ++it)
        {
            nRadix += *it;
        }

        if (nRadix > 0)
        {
            int nRandom = Random() % nRadix;
            int nRateVecSize = (int)rRateVec.size();
            for (nRateIndex = 0; nRateIndex < nRateVecSize; ++nRateIndex)
            {
                nRandom -= rRateVec[nRateIndex];
                if (nRandom < 0)
                {
                    break;
                }
            }
        }
    }

    return nRateIndex;
}

string CodeRandom(int nLength)
{
    string strCode;

    for (int i = 0; i < nLength; ++i)
    {
        char chCode = (char)Random('A', 'Z');
        strCode.append(1, chCode);
    }

    return strCode;
}

void CopyString2Buffer(string & src, char* DesBuffer, int nDesBufferLen)
{
    if (DesBuffer != NULL)
    {
        int nLen = src.length();
        if (nLen > nDesBufferLen)
        {
            nLen = nDesBufferLen;
        }
        memcpy(DesBuffer, src.c_str(), nLen);
    }
}


char * WideChar2MultiChar(wchar_t * pwcSrc)
{
#ifdef WIN32
    setlocale(LC_CTYPE, "chs");
#else
    setlocale(LC_CTYPE, "zh_CN.gb18030");
#endif

    char * pszResult = NULL;
    int nLen = wcstombs(NULL, pwcSrc, 0);
    if (nLen > 0)
    {
        pszResult = new char[nLen + 1];
        wcstombs(pszResult, pwcSrc, nLen);
        pszResult[nLen] = 0;
    }
    return pszResult;
}

wchar_t * MultiChar2WideChar(const char * pszSrc)
{
#ifdef WIN32
    setlocale(LC_CTYPE, "chs");
#else
    setlocale(LC_CTYPE, "zh_CN.gb18030");
#endif

    wchar_t* pwcResult = NULL;
    int nLen = mbstowcs(NULL, pszSrc, 0);
    if (nLen > 0)
    {
        pwcResult = new wchar_t[nLen + 1];
        mbstowcs(pwcResult, pszSrc, nLen + 1);
    }
    return pwcResult;
}


void safememcpy(char * des, unsigned int  Deslen, const char* src, unsigned int SrcLen)
{
    unsigned int len = Deslen;
    if (Deslen > SrcLen)
    {
        len = SrcLen;
    }
    memcpy(des, src, len);
}

string BinaryData2String(const unsigned char *src, unsigned int srcLen)
{
    string strDes;

    for (unsigned int i = 0; i < srcLen; ++i)
    {
        unsigned char temp = *src / 16;
        unsigned char dest = (temp >= 10 ? temp - 10 + 'a' : temp + '0');
        strDes.append(1, dest);

        temp = *src % 16;
        dest = (temp >= 10 ? temp - 10 + 'a' : temp + '0');
        strDes.append(1, dest);

        src++;
    }

    return strDes;
}

#ifdef WIN32
// 多字节编码转为UTF8编码
bool MBToUTF8(unsigned int CodePage, vector<char>& pu8, const char* pmb, int mLen)
{
    // convert an MBCS string to widechar 
    int nLen = MultiByteToWideChar(CodePage, 0, pmb, mLen, NULL, 0);

    WCHAR* lpszW = NULL;
    try
    {
        lpszW = new WCHAR[nLen];
    }
    catch (bad_alloc &memExp)
    {
        return false;
    }

    int nRtn = MultiByteToWideChar(CodePage, 0, pmb, mLen, lpszW, nLen);

    if (nRtn != nLen)
    {
        delete[] lpszW;
        return false;
    }
    // convert an widechar string to utf8
    int utf8Len = WideCharToMultiByte(CP_UTF8, 0, lpszW, nLen, NULL, 0, NULL, NULL);
    if (utf8Len <= 0)
    {
        return false;
    }
    pu8.resize(utf8Len);
    nRtn = WideCharToMultiByte(CP_UTF8, 0, lpszW, nLen, &*pu8.begin(), utf8Len, NULL, NULL);
    delete[] lpszW;

    if (nRtn != utf8Len)
    {
        pu8.clear();
        return false;
    }
    return true;
}

// UTF8编码转为多字节编码
bool UTF8ToMB(unsigned int CodePage, vector<char>& pmb, const char* pu8, int utf8Len)
{
    // convert an UTF8 string to widechar 
    int nLen = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8Len, NULL, 0);

    WCHAR* lpszW = NULL;
    try
    {
        lpszW = new WCHAR[nLen];
    }
    catch (bad_alloc &memExp)
    {
        return false;
    }

    int nRtn = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8Len, lpszW, nLen);

    if (nRtn != nLen)
    {
        delete[] lpszW;
        return false;
    }

    // convert an widechar string to Multibyte 
    int MBLen = WideCharToMultiByte(CodePage, 0, lpszW, nLen, NULL, 0, NULL, NULL);
    if (MBLen <= 0)
    {
        return false;
    }
    pmb.resize(MBLen);
    nRtn = WideCharToMultiByte(CodePage, 0, lpszW, nLen, &*pmb.begin(), MBLen, NULL, NULL);
    delete[] lpszW;

    if (nRtn != MBLen)
    {
        pmb.clear();
        return false;
    }
    return true;
}

// 多字节编码转为Unicode编码
bool MBToUnicode(unsigned int CodePage, vector<wchar_t>& pun, const char* pmb, int mLen)
{
    // convert an MBCS string to widechar 
    int uLen = MultiByteToWideChar(CodePage, 0, pmb, mLen, NULL, 0);

    if (uLen <= 0)
    {
        return false;
    }
    pun.resize(uLen);

    int nRtn = MultiByteToWideChar(CodePage, 0, pmb, mLen, &*pun.begin(), uLen);

    if (nRtn != uLen)
    {
        pun.clear();
        return false;
    }
    return true;
}

//Unicode编码转为多字节编码
bool UnicodeToMB(unsigned int CodePage, vector<char>& pmb, const wchar_t* pun, int uLen)
{
    // convert an widechar string to Multibyte 
    int MBLen = WideCharToMultiByte(CodePage, 0, pun, uLen, NULL, 0, NULL, NULL);
    if (MBLen <= 0)
    {
        return false;
    }
    pmb.resize(MBLen);
    int nRtn = WideCharToMultiByte(CodePage, 0, pun, uLen, &*pmb.begin(), MBLen, NULL, NULL);

    if (nRtn != MBLen)
    {
        pmb.clear();
        return false;
    }
    return true;
}

// UTF8编码转为Unicode
bool UTF8ToUnicode(vector<wchar_t>& pun, const char* pu8, int utf8Len)
{
    // convert an UTF8 string to widechar 
    int nLen = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8Len, NULL, 0);
    if (nLen <= 0)
    {
        return false;
    }
    pun.resize(nLen);
    int nRtn = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8Len, &*pun.begin(), nLen);

    if (nRtn != nLen)
    {
        pun.clear();
        return false;
    }

    return true;
}

// Unicode编码转为UTF8
bool UnicodeToUTF8(vector<char>& pu8, const wchar_t* pun, int uLen)
{
    // convert an widechar string to utf8
    int utf8Len = WideCharToMultiByte(CP_UTF8, 0, pun, uLen, NULL, 0, NULL, NULL);
    if (utf8Len <= 0)
    {
        return false;
    }
    pu8.resize(utf8Len);
    int nRtn = WideCharToMultiByte(CP_UTF8, 0, pun, uLen, &*pu8.begin(), utf8Len, NULL, NULL);

    if (nRtn != utf8Len)
    {
        pu8.clear();
        return false;
    }
    return true;
}
#endif

