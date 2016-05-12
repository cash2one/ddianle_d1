#include "CommonFunc.h"
#include <ctype.h>
#include <time.h>
#include <set>
#include "../lua/InitLua.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../../socket/conv.h"
#include "../../socket/Formatter.h"
#ifdef WIN32
#include <io.h>
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

void LocalTime(const time_t *timep, struct tm *result)
{
    memset(result, 0, sizeof (struct tm));
#ifdef WIN32
    localtime_s(result, timep);
#else
    localtime_r(timep, result);
#endif
}

std::string StandardTime(time_t time)
{
    std::string strBuff;
    struct tm tmBuff;
    LocalTime(&time, &tmBuff);
    Formatter::Format(strBuff, "%04d-%02d-%02d %02d:%02d:%02d",
        tmBuff.tm_year + 1900, tmBuff.tm_mon + 1, tmBuff.tm_mday,
        tmBuff.tm_hour, tmBuff.tm_min, tmBuff.tm_sec);

    return strBuff;
}

void LocalizationTime(time_t time, const std::string &strFormat, std::string &strBuff)
{
    struct tm tmBuff;
    LocalTime(&time, &tmBuff);
    SafeFormat(strBuff, strFormat,
        tmBuff.tm_year + 1900, tmBuff.tm_mon + 1, tmBuff.tm_mday,
        tmBuff.tm_hour, tmBuff.tm_min, tmBuff.tm_sec);
}

void SplitString(std::string str, std::string pattern, std::vector< std::string > &rStrVec)
{
    if (str.empty())
    {
        return;
    }
    std::string::size_type pos;
    std::vector< std::string > result;
    str += pattern;
    std::string::size_type size = str.size();
    for (std::string::size_type i = 0; i < size; i++)
    {
        pos = str.find(pattern, i);
        if (pos < size)
        {
            std::string s = str.substr(i, pos - i);
            rStrVec.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
}

void SplitString(std::string str, std::string pattern, std::vector<int> &vec)
{
    std::string::size_type pos;
    std::vector< std::string > result;
    str += pattern;
    std::string::size_type size = str.size();
    for (std::string::size_type i = 0; i < size; i++)
    {
        pos = str.find(pattern, i);
        if (pos < size)
        {
            std::string s = str.substr(i, pos - i);
            vec.push_back(atoi(s.c_str()));
            i = pos + pattern.size() - 1;
        }
    }
}//end

void SplitString(std::string str, std::string pattern, std::list<unsigned short> &vec)
{
    std::string::size_type pos;
    std::vector< std::string > result;
    str += pattern;
    std::string::size_type size = str.size();
    for (std::string::size_type i = 0; i < size; i++)
    {
        pos = str.find(pattern, i);
        if (pos < size)
        {
            std::string s = str.substr(i, pos - i);
            vec.push_back((unsigned short)atoi(s.c_str()));
            i = pos + pattern.size() - 1;
        }
    }
}//end

void SplitString(std::string str, std::string pattern, std::set<int> &rSet)
{
    if (str.empty())
    {
        rSet.clear();

        return;
    }

    std::string::size_type pos;

    str += pattern;

    for (std::string::size_type i = 0, size = str.size(); i < size; ++i)
    {
        pos = str.find(pattern, i);

        if (pos < size)
        {
            std::string substr = str.substr(i, pos - i);
            rSet.insert(atoi(substr.c_str()));

            i = pos + pattern.size() - 1;
        }
    }
}

void TrimSpace(string& str)
{
    int nBegin = 0;
    while (nBegin < (int)str.size())
    {
        if (str[nBegin] == ' ' || str[nBegin] == '\n' || str[nBegin] == '\t' || str[nBegin] == '\r' || str[nBegin] == '\b')
        {
            ++nBegin;
        }
        else
        {
            break;
        }
    }

    int nEnd = (int)(str.size() - 1);
    while (nEnd >= 0)
    {
        if (str[nEnd] == ' ' || str[nEnd] == '\n' || str[nEnd] == '\t' || str[nEnd] == '\r' || str[nEnd] == '\b')
        {
            --nEnd;
        }
        else
        {
            break;
        }
    }

    int nCount = nEnd - nBegin + 1;
    if (nCount > 0)
    {
        str = str.substr(nBegin, nCount);
    }
}

void RemoveToken_W(string& str, const char* sToken)
{
    string r;
    unsigned int i = 0;
    while (i < str.size())
    {
        if (!(str[i] & 0x80))
        {
            r.push_back(str[i]);
            i += 1;
        }
        else
        {
            bool bTrim = false;
            for (unsigned int j = 0; j < strlen(sToken); j += 2)
            {
                if (str[i] == sToken[j] && str[i + 1] == sToken[j + 1])
                    bTrim = true;
            }
            if (!bTrim)
            {
                r.push_back(str[i]);
                r.push_back(str[i + 1]);
            }
            i += 2;
        }
    }
    str = r;
}

void RemoveToken(string& str, const char* sToken)
{
    string r;
    unsigned int i = 0;
    while (i < str.size())
    {
        if (str[i] & 0x80)
        {
            r.push_back(str[i]);
            r.push_back(str[i + 1]);
            i += 2;
        }
        else
        {
            bool bTrim = false;
            for (unsigned int j = 0; j < strlen(sToken); j++)
            {
                if (str[i] == sToken[j])
                    bTrim = true;
            }
            if (!bTrim)
                r.push_back(str[i]);
            i++;
        }
    }
    str = r;
}

bool IsValidBirthDay(int nYear, int nMonth, int nDay)
{
    if (nYear < 1900 || nYear > 2013)
    {
        return false;
    }
    if (nMonth < 1 || nMonth > 12)
    {
        return false;
    }
    if (nDay < 1 || nDay > 31)
    {
        return false;
    }

    switch (nMonth)
    {
    case 2:// February
    {
               if ((0 == nYear % 400) ||
                   (0 == nYear % 4 && 0 != nYear % 100))
               {
                   if (nDay > 29)
                       return false;
               }
               else if (nDay > 28)
               {
                   return false;
               }
    }
        break;

    case  4:// April
    case  6:// June
    case  9:// September
    case 11:// November
    {
                if (nDay > 30)
                    return false;
    }
        break;

    default:
        break;
    }

    return true;
}
void GetBirthDay(int nBirthDay, int & nrYear, int & nrMonth, int & nrDay)
{
    nrYear = nBirthDay / 10000;
    int nMonthDay = nBirthDay % 10000;
    nrMonth = nMonthDay / 100;
    nrDay = nMonthDay - nrMonth * 100;
}

short BirthdayToAge(const char *strBirthday)
{
    if (strBirthday == NULL)
    {
        return 0;
    }

    int nBirthYear = 0, nCurrYear = 0;
    int nBirthMonth = 0, nCurrMonth = 0;
    int nBirthDay = 0, nCurrDay = 0;

    GetBirthDay(atoi(strBirthday), nBirthYear, nBirthMonth, nBirthDay);

    if (!IsValidBirthDay(nBirthYear, nBirthMonth, nBirthDay))
    {
        return 0;
    }

    time_t tNow = time(NULL);
    struct tm tmNow;
    LocalTime(&tNow, &tmNow);

    nCurrYear = tmNow.tm_year + 1900;
    nCurrMonth = tmNow.tm_mon + 1;
    nCurrDay = tmNow.tm_mday;

    int nAge = nCurrYear - nBirthYear;
    if (nCurrMonth < nBirthMonth || (nCurrMonth == nBirthMonth && nCurrDay < nBirthDay))
    {
        nAge = nAge - 1;
    }

    if (nAge < 0)
    {
        nAge = 0;
    }

    return (short)nAge;
}

//判断星座
int checkConstellation(int nMonth, int nDay)
{
    unsigned int nConstellation = 0;

    if ((12 < nMonth) || (nMonth < 0))
    {
        return 0;
    }
    if ((nDay < 1) || (nDay >31))
    {
        return 0;
    }

    switch (nMonth)
    {
    case 1:
    {
              if (nDay < 20)
              {
                  nConstellation = Capricornus;//摩羯座				
              }
              else
              {
                  nConstellation = Aquarius;	//水瓶座
              }
    }
        break;
    case 2:
    {
              if (nDay < 19)
              {
                  nConstellation = Aquarius;
              }
              else
              {
                  nConstellation = Pisces;	//双鱼座
              }
    }
        break;
    case 3:
    {
              if (nDay < 21)
              {
                  nConstellation = Pisces;
              }
              else
              {
                  nConstellation = Aries;		//白羊座
              }
    }
        break;
    case 4:
    {
              if (nDay < 20)
              {
                  nConstellation = Aries;
              }
              else
              {
                  nConstellation = Taurus;	//金牛座
              }
    }
        break;
    case 5:
    {
              if (nDay < 21)
              {
                  nConstellation = Taurus;
              }
              else
              {
                  nConstellation = Gemini;	//双子座
              }
    }
        break;
    case 6:
    {
              if (nDay < 22)
              {
                  nConstellation = Gemini;
              }
              else
              {
                  nConstellation = Cancer;	//巨蟹座
              }
    }
        break;
    case 7:
    {
              if (nDay < 23)
              {
                  nConstellation = Cancer;
              }
              else
              {
                  nConstellation = Leo;		//双子座
              }
    };
        break;
    case 8:
    {
              if (nDay < 23)
              {
                  nConstellation = Leo;
              }
              else
              {
                  nConstellation = Virgo;		//处女座
              }
    }
        break;
    case 9:
    {
              if (nDay < 23)
              {
                  nConstellation = Virgo;
              }
              else
              {
                  nConstellation = Libra;		//天秤座
              }
    }
        break;
    case 10:
    {
               if (nDay < 24)
               {
                   nConstellation = Libra;
               }
               else
               {
                   nConstellation = Scorpio;	//天蝎座
               }
    }
        break;
    case 11:
    {
               if (nDay < 23)
               {
                   nConstellation = Scorpio;
               }
               else
               {
                   nConstellation = Sagittarius;//射手座
               }
    }
        break;
    case 12:
    {
               if (nDay < 22)
               {
                   nConstellation = Sagittarius;
               }
               else
               {
                   nConstellation = Capricornus;//摩羯座
               }
    }
        break;
    default:
        ;
    }

    return nConstellation;
}

time_t SetTime(int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond)
{
    struct tm time;
    memset(&time, 0, sizeof (time));
    time.tm_year = nYear - 1900;
    time.tm_mon = nMonth - 1;
    time.tm_mday = nDay;

    time.tm_hour = nHour;
    time.tm_min = nMinute;
    time.tm_sec = nSecond;

    time_t tTime = mktime(&time);
    return tTime;
}

bool IsTimeIsToday(time_t tTime)
{
    time_t nNow = time(NULL);

    return IsSameDay(tTime, nNow);
}

bool IsTimeRangeApart(const std::map<time_t, time_t>& mapTimeRange)
{
    std::list<time_t> origlist, orderlist;
    for (std::map<time_t, time_t>::const_iterator cit = mapTimeRange.begin();
        cit != mapTimeRange.end(); ++cit)
    {
        origlist.push_back(cit->first);
        origlist.push_back(cit->second);
    }
    orderlist = origlist;
    orderlist.sort();
    return origlist == orderlist;
}

// Formart : YYYY-MM-DD hh:mm:ss
time_t SetTime(const char * szTime)
{
    int nYear = 0;
    int nMonth = 0;
    int nDay = 0;
    int nHour = 0;
    int nMinute = 0;
    int nSecond = 0;
    sscanf(szTime, "%4d-%2d-%2d %2d:%2d:%2d", &nYear, &nMonth, &nDay, &nHour, &nMinute, &nSecond);
    return SetTime(nYear, nMonth, nDay, nHour, nMinute, nSecond);
}

time_t GetNextDayStartTime(time_t nTime)
{
    struct tm tTime;
    LocalTime(&nTime, &tTime);

    tTime.tm_hour = 0;
    tTime.tm_min = 0;
    tTime.tm_sec = 0;

    time_t nTodayStart = mktime(&tTime);

    return (nTodayStart + 24 * 60 * 60);
}

time_t GetTodayStartTime(time_t nTime)
{
    struct tm tTime;
    memset( &tTime, 0, sizeof( tTime ) );
    LocalTime(&nTime, &tTime);

    tTime.tm_hour = 0;
    tTime.tm_min = 0;
    tTime.tm_sec = 0;

    return mktime(&tTime);
}

int GetYear(time_t nTime)
{
    struct tm tTime;
    LocalTime(&nTime, &tTime);

    return tTime.tm_year + 1900;
}
int GetCurYear()
{
    time_t now;
    time(&now);
    return GetYear(now);
}

int GetMonth(time_t nTime)
{
    struct tm tTime;
    LocalTime(&nTime, &tTime);

    return tTime.tm_mon + 1;
}

int GetCurMonth()
{
    time_t now;
    time(&now);
    return GetMonth(now);
}

int GetMonthDay(time_t nTime)
{
    struct tm tTime;
    LocalTime(&nTime, &tTime);

    return tTime.tm_mday;
}
int GetCurMonthDay()
{
    time_t now;
    time(&now);
    return GetMonthDay(now);
}

int GetYearDay(time_t nTime)
{
    struct tm tTime;
    LocalTime(&nTime, &tTime);

    return tTime.tm_yday;
}

int GetCurYearDay()
{
    time_t now;
    time(&now);
    return GetYearDay(now);
}

unsigned int PrevMondayStamp(time_t nTime)
{
    time_t zero = GetTodayStartTime(nTime);
    int weekday = GetWeekDay(nTime);
    if (weekday == 0)
        weekday = 7;
    return (unsigned int)(zero - (weekday - 1) * 24 * 60 * 60);
}
unsigned int PrevSundayStamp(time_t nTime)
{
    time_t zero = GetTodayStartTime(nTime);
    return (unsigned int)(zero - (GetWeekDay(nTime)) * 24 * 60 * 60);
}

unsigned int PrevTimeStamp(time_t nTime, int nWeekday, int nHour)
{
    if (nWeekday < 0 || nWeekday > 6 ||
        nHour < 0 || nHour > 23)
    {
        return 0;
    }

    struct tm tTime;
    memset( &tTime, 0, sizeof( tTime ) );
    LocalTime(&nTime, &tTime);

    tTime.tm_hour = nHour;
    tTime.tm_min = 0;
    tTime.tm_sec = 0;

    unsigned int offset = 0;
    unsigned int nTodayStart = (unsigned int)mktime(&tTime);

    if (tTime.tm_wday >= nWeekday)
        offset = tTime.tm_wday - nWeekday;
    else
        offset = 7 + tTime.tm_wday - nWeekday;

    offset *= 24 * 3600;

    if (nTodayStart > offset)
        return (nTodayStart - offset);

    return 0;
}

int DiffDays(time_t tAfter, time_t tBefore)
{
    struct tm tmAfter, tmBefore;
    memset(&tmAfter, 0, sizeof (tmAfter));
    memset(&tmBefore, 0, sizeof (tmBefore));

    LocalTime(&tAfter, &tmAfter);
    LocalTime(&tBefore, &tmBefore);

    if (tmAfter.tm_year == tmBefore.tm_year)
    {
        return tmAfter.tm_yday - tmBefore.tm_yday;
    }

    tmAfter.tm_hour = 0;
    tmAfter.tm_min = 0;
    tmAfter.tm_sec = 0;

    tmBefore.tm_hour = 0;
    tmBefore.tm_min = 0;
    tmBefore.tm_sec = 0;

    tAfter = mktime(&tmAfter);
    tBefore = mktime(&tmBefore);

    return int((tAfter - tBefore) / 24 / 3600);
}

int DiffDays(time_t t1, time_t t2, unsigned int standard)
{
    struct tm tm1;
    LocalTime(&t1, &tm1);

    struct tm tm2;
    LocalTime(&t2, &tm2);

    if (tm1.tm_hour * 60 * 60 + tm1.tm_min * 60 + tm1.tm_sec >= (int)standard)
    {
        t1 += 24 * 60 * 60;
    }

    if (tm2.tm_hour * 60 * 60 + tm2.tm_min * 60 + tm2.tm_sec >= (int)standard)
    {
        t2 += 24 * 60 * 60;
    }

    return DiffDays(t1, t2);
}

int GetWeekDay(time_t nTime)
{
    struct tm tTime;
    LocalTime(&nTime, &tTime);

    return tTime.tm_wday;
}

int GetWeekDay()
{
    time_t now;
    time(&now);
    return GetWeekDay(now);
}

int GetHour(time_t nTime)
{
    struct tm tTime;
    LocalTime(&nTime, &tTime);

    return tTime.tm_hour;
}

int GetCurHour()
{
    time_t now;
    time(&now);

    return GetHour(now);
}

int GetMinute(time_t nTime)
{
    struct tm tTime;
    LocalTime(&nTime, &tTime);

    return tTime.tm_min;
}

int GetCurMinute()
{
    time_t now;
    time(&now);

    return GetMinute(now);
}

int GetSecond(time_t nTime)
{
    struct tm tTime;
    LocalTime(&nTime, &tTime);

    return tTime.tm_sec;
}

int GetHourMinuteSecond(time_t nTime)
{
    struct tm tTime;
    LocalTime(&nTime, &tTime);

    return tTime.tm_hour * 60 * 60 + tTime.tm_min * 60 + tTime.tm_sec;
}

int GetCurSecond()
{
    time_t now;
    time(&now);

    return GetSecond(now);
}

void SetBit(int & nNum, int nPos, int nValue)
{
    if (nNum >= 0 && nNum < (int)sizeof(__int64)* 8)
    {
        int nBase = 0x1;
        if (nValue == 0)
            nNum &= !(nBase << nPos);
        else
            nNum |= nBase << nPos;
    }
}

void SetBit(__int64 & nNum, int nPos, int nValue)
{
    if (nPos >= 0 && nPos < (int)sizeof(__int64)* 8)
    {
        __int64 nBase = 0x1;
        if (nValue == 0)
            nNum &= !(nBase << nPos);
        else
            nNum |= nBase << nPos;
    }
}

int GetBit(__int64  nNum, int nPos)
{
    if (nPos >= 0 && nPos < (int)sizeof(__int64)* 8)
    {
        __int64 nBase = 0x1;
        return (int)((nNum >> nPos) & nBase);
    }

    return 0;
}

int CountOne(__int64 nNum)
{
    int nCount = 0;
    while (nNum > 0)
    {
        nCount++;
        nNum &= nNum - 1;
    }

    return nCount;
}


//将字符串的物品列表转化为物品列表：格式 Type,Count,ValidTime|Type,Count,ValidTime. 如：1001,1,1|1002,1,1
bool TransferItemList(const char * szItemList, std::list< CItem > & rItemList)
{
    bool bRes = true;
    std::vector< std::string > vecItemInfo;
    SplitString(szItemList, "|", vecItemInfo);
    for (size_t i = 0; i < vecItemInfo.size(); i++)
    {
        std::vector< std::string > vecItemDetailInfo;
        SplitString(vecItemInfo[i], ",", vecItemDetailInfo);

        if (vecItemDetailInfo.size() == 3)
        {
            TrimSpace(vecItemDetailInfo[0]);
            TrimSpace(vecItemDetailInfo[1]);
            TrimSpace(vecItemDetailInfo[2]);

            CItem item(atoi(vecItemDetailInfo[0].c_str()), (unsigned short)atoi(vecItemDetailInfo[1].c_str()), atoi(vecItemDetailInfo[2].c_str()), false);
            rItemList.push_back(item);
        }
        else
        {
            bRes = false;
            WriteLog(LOGLEVEL_ERROR, "Error: TransferItemList error item info(%s)", szItemList);
        }
    }
    return bRes;
}

//将字符串的物品列表转化为物品列表：格式 Type,Count,ValidTime|Type,Count,ValidTime. 如：1001,1,1|1002,1,1
bool TransferList(const char * szItemList, std::vector< unsigned int > & rList)
{
    bool bRes = true;
    std::vector< std::string > vecItemInfo;
    SplitString(szItemList, "|", vecItemInfo);

    for (size_t i = 0; i < vecItemInfo.size(); i++)
    {
        unsigned int nValue = atoi(vecItemInfo[i].c_str());
        rList.push_back(nValue);
    }
    return bRes;
}

bool TransferList(const char * szItemList, std::list< unsigned short > & rList)
{
    bool bRes = true;
    std::vector< std::string > vecItemInfo;
    SplitString(szItemList, "|", vecItemInfo);

    for (size_t i = 0; i < vecItemInfo.size(); i++)
    {
        unsigned short nValue = (unsigned short)atoi(vecItemInfo[i].c_str());
        rList.push_back(nValue);
    }
    return bRes;
}

void TransferString(const char * szItemList, unsigned int & nMinValue, unsigned int & nMaxValue)
{
    std::vector< std::string > vecItemInfo;
    SplitString(szItemList, ":", vecItemInfo);

    if (vecItemInfo.size() == 2)
    {
        nMinValue = atoi(vecItemInfo[0].c_str());
        nMaxValue = atoi(vecItemInfo[1].c_str());
    }
}

bool TransferKeyValueList(const char *szKeyValueList, std::map<unsigned short, unsigned short> &rKeyvalueList)
{
    bool bRes = true;
    std::vector<std::string> vecKeyValueInfo;
    SplitString(szKeyValueList, "|", vecKeyValueInfo);
    for (size_t i = 0; i < vecKeyValueInfo.size(); ++i)
    {
        std::vector<std::string> vecDetialInfo;
        SplitString(vecKeyValueInfo[i], ":", vecDetialInfo);

        if (vecDetialInfo.size() == 2)
        {
            TrimSpace(vecDetialInfo[0]);
            TrimSpace(vecDetialInfo[1]);
            rKeyvalueList.insert(make_pair((unsigned short)atoi(vecDetialInfo[0].c_str()), (unsigned short)atoi(vecDetialInfo[1].c_str())));
        }
        else
        {
            bRes = false;
        }
    }//end for
    return bRes;
}

bool TransferKeyValueList(const char *szKeyValueList, std::map<unsigned int, unsigned int> &rKeyvalueList)
{
    bool bRes = true;
    std::vector<std::string> vecKeyValueInfo;
    SplitString(szKeyValueList, "|", vecKeyValueInfo);
    for (size_t i = 0; i < vecKeyValueInfo.size(); ++i)
    {
        std::vector<std::string> vecDetialInfo;
        SplitString(vecKeyValueInfo[i], ":", vecDetialInfo);

        if (vecDetialInfo.size() == 2)
        {
            TrimSpace(vecDetialInfo[0]);
            TrimSpace(vecDetialInfo[1]);
            rKeyvalueList.insert(make_pair((unsigned int)atoi(vecDetialInfo[0].c_str()), (unsigned int)atoi(vecDetialInfo[1].c_str())));
        }
        else
        {
            bRes = false;
        }
    }//end for
    return bRes;
}

bool TransferKeyValueStr(string &strValue, std::map<unsigned short, unsigned short> &keyValueList)
{
    std::string token;
    std::map<unsigned short, unsigned short>::iterator iter = keyValueList.begin(), iterEnd = keyValueList.end();
    for (; iter != iterEnd; ++iter)
    {
        if (strValue.empty())
        {
            Formatter(token) << iter->first << ':' << iter->second;
            strValue += token;
        }
        else
        {
            Formatter(token) << '|' << iter->first << ':' << iter->second;
            strValue += token;
        }
    }

    return true;
}

void PacketAtoi(const char * szPacket, list<int> & listPacketDetail)
{
    if (szPacket == NULL)
    {
        return;
    }

    const char *pTemp = szPacket;
    while (*pTemp != '\0')
    {
        int nCount = 1;
        if (*pTemp != ',' && *pTemp != ' ' && ((*pTemp >= '0' && *pTemp <= '9') || *pTemp == '-'))
        {
            int n = atoi(pTemp);
            listPacketDetail.push_back(n);
            if (n < 0)
            {
                nCount++;
                n = -n;
            }
            while ((n /= 10) != 0)
            {
                nCount++;
            }
        }
        pTemp = pTemp + nCount;
    }
}

bool TransferKeyDynamicBox(const char *szKeyValueList, unsigned int & nitemID, unsigned int & nRelatedRate, unsigned int & nItemRate)
{
    bool bRes = true;
    std::vector<std::string> vecKeyValueInfo;
    SplitString(szKeyValueList, ":", vecKeyValueInfo);

    if (vecKeyValueInfo.size() != 2)
    {
        return false;
    }

    for (size_t i = 0; i < vecKeyValueInfo.size(); ++i)
    {
        std::vector<std::string> vecDetialInfo;
        SplitString(vecKeyValueInfo[i], "|", vecDetialInfo);

        if (i == 0)
        {
            nitemID = (unsigned int)atoi(vecDetialInfo[0].c_str());
        }
        else
        {
            if (vecDetialInfo.size() == 2)
            {
                TrimSpace(vecDetialInfo[0]);
                TrimSpace(vecDetialInfo[1]);

                nRelatedRate = (unsigned int)atoi(vecDetialInfo[0].c_str());
                nItemRate = (unsigned int)atoi(vecDetialInfo[1].c_str());
            }
            else
            {
                bRes = false;
            }
        }


    }//end for
    return bRes;
}

bool TransferItem(const char * szItem, CItem &item)
{
    bool bRes = true;
    std::vector< std::string > vecItemDetailInfo;
    SplitString(szItem, ",", vecItemDetailInfo);

    if (vecItemDetailInfo.size() == 3)
    {
        TrimSpace(vecItemDetailInfo[0]);
        TrimSpace(vecItemDetailInfo[1]);
        TrimSpace(vecItemDetailInfo[2]);

        item.m_nItemType = atoi(vecItemDetailInfo[0].c_str());
        item.m_nItemCount = (itemcount_t)atoi(vecItemDetailInfo[1].c_str());
        item.m_nValidTime = atoi(vecItemDetailInfo[2].c_str());
    }
    else
    {
        bRes = false;
        WriteLog(LOGLEVEL_ERROR, "Error: TransferItem error item info(%s)", szItem);
    }

    return bRes;
}


//将字符串的充值返利计划解析出来
bool AnalyzingChargePlan(int nChargeLevels, const char * strChargeRange, const char * strChargeMcoin, const char * strChargeMoney, std::vector< CChargePlan > & rChargePlan)
{
    bool bRes = true;
    std::vector< std::string > vecChargeRangeInfo;	SplitString(strChargeRange, "|", vecChargeRangeInfo);
    std::vector< std::string > vecMcoinRewardInfo;	SplitString(strChargeMcoin, "|", vecMcoinRewardInfo);
    std::vector< std::string > vecMoneyRewardInfo;	SplitString(strChargeMoney, "|", vecMoneyRewardInfo);
    if (vecChargeRangeInfo.size() != (size_t)nChargeLevels
        || vecMcoinRewardInfo.size() != (size_t)nChargeLevels
        || vecMoneyRewardInfo.size() != (size_t)nChargeLevels)
    {
        bRes = false;
        WriteLog(LOGLEVEL_ERROR, "Error: AnalyzingChargePlan error!  ChargePlanLevels(%d) not equal to vetor.size(%d,%d,%d)",
            nChargeLevels, vecChargeRangeInfo.size(), vecMcoinRewardInfo.size(), vecMoneyRewardInfo.size());
    }

    for (int i = 0; i < nChargeLevels; i++)
    {
        std::vector< std::string > vecChargeRangeDetailInfo;	SplitString(vecChargeRangeInfo[i], ",", vecChargeRangeDetailInfo);		// 充值区间
        std::vector< std::string > vecMcoinRewardDetailInfo;	SplitString(vecMcoinRewardInfo[i], ",", vecMcoinRewardDetailInfo);		// Mcoin
        std::vector< std::string > vecMoneyRewardDetailInfo;	SplitString(vecMoneyRewardInfo[i], ",", vecMoneyRewardDetailInfo);		// 金券

        if (vecChargeRangeDetailInfo.size() < 2)
        {
            WriteLog(LOGLEVEL_ERROR, "Error: AnalyzingChargePlan error!  vecChargeRangeInfo %s has only %d element",
                vecChargeRangeInfo[i].c_str(), vecChargeRangeDetailInfo.size());
            return false;
        }
        if (vecMcoinRewardDetailInfo.size() < 2)
        {
            WriteLog(LOGLEVEL_ERROR, "Error: AnalyzingChargePlan error!  vecMcoinRewardInfo %s has only %d element",
                vecMcoinRewardInfo[i].c_str(), vecMcoinRewardDetailInfo.size());
            return false;
        }
        if (vecMoneyRewardDetailInfo.size() < 2)
        {
            WriteLog(LOGLEVEL_ERROR, "Error: AnalyzingChargePlan error!  vecMoneyRewardInfo %s has only %d element",
                vecMoneyRewardInfo[i].c_str(), vecMoneyRewardDetailInfo.size());
            return false;
        }

        TrimSpace(vecChargeRangeDetailInfo[0]);	//Range Lower
        TrimSpace(vecChargeRangeDetailInfo[1]);	//Range Upper
        TrimSpace(vecMcoinRewardDetailInfo[0]);	//Mcoin Ratio
        TrimSpace(vecMcoinRewardDetailInfo[1]);	//Mcoin Quota
        TrimSpace(vecMoneyRewardDetailInfo[0]);	//Money Ratio
        TrimSpace(vecMoneyRewardDetailInfo[1]);	//Money Quota


        CChargePlan chargeplan(atoi(vecChargeRangeDetailInfo[0].c_str()), atoi(vecChargeRangeDetailInfo[1].c_str()),
            atoi(vecMcoinRewardDetailInfo[0].c_str()), atoi(vecMcoinRewardDetailInfo[1].c_str()),
            atoi(vecMoneyRewardDetailInfo[0].c_str()), atoi(vecMoneyRewardDetailInfo[1].c_str()));

        rChargePlan.push_back(chargeplan);
    }
    return bRes;
}

bool CheckDirExist(const char* szDir)
{
    return 0 == access(szDir, 0);
}

bool CreateDir(const char* szDir)
{
#ifdef WIN32
    return 0 == mkdir(szDir);
#else
    return 0 == mkdir(szDir, 0777);
#endif
}

int GetChargeEventID(int nVersionType)
{
    return eEvent_FinishFirstPayment;//免费版和付费版共用一个首充
}

int GetChargeSendRewardEventID(int nVersionType)
{
    return eEvent_SentFirstPaymentReward;//免费版和付费版共用一个首充
}

bool IsSameDay(time_t time1, time_t time2)
{
    tm result1, result2;

    LocalTime(&time1, &result1);
    LocalTime(&time2, &result2);

    return (
        result1.tm_year == result2.tm_year &&
        result1.tm_mon == result2.tm_mon &&
        result1.tm_mday == result2.tm_mday
        );
}

bool IsSameDayAndSecond(time_t time1, time_t time2)
{
    tm result1, result2;

    LocalTime(&time1, &result1);
    LocalTime(&time2, &result2);

    return (result1.tm_year == result2.tm_year &&
        result1.tm_mon == result2.tm_mon &&
        result1.tm_mday == result2.tm_mday &&
        result1.tm_hour == result2.tm_hour &&
        result1.tm_min == result2.tm_min &&
        result1.tm_sec == result2.tm_sec);
}

int GetLocalDays(time_t nSeconds)
{
    struct tm tTime;
    LocalTime(&nSeconds, &tTime);

    return tTime.tm_year * 365 + tTime.tm_yday;
}

int GetLocalHourOfDay(time_t nSeconds)
{
    struct tm tTime;
    LocalTime(&nSeconds, &tTime);

    return tTime.tm_hour;
}

