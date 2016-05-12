#ifndef __COMMON_FUNC_H__
#define __COMMON_FUNC_H__

#include "../../datastructure/Macro_Define.h"
#include <list>
#include <string>
#include "../../datastructure/DataStruct_Base.h"

using namespace std;

void LocalTime(const time_t *timep, struct tm *result);
std::string StandardTime(time_t time);
void LocalizationTime(time_t time, const std::string &strFormat, std::string &strBuff);
void SplitString(std::string str, std::string pattern, std::vector< std::string > &rStrVec);
void SplitString(std::string str, std::string pattern, std::vector<int> &vec);
void SplitString(std::string str, std::string pattern, std::list<unsigned short> &vec);
void SplitString(std::string str, std::string pattern, std::set<int> &rSet);


void TrimSpace(string& str);
void RemoveToken(string& str, const char* sToken);
void RemoveToken_W(string& str, const char* sToken);

bool IsValidBirthDay(int nYear, int nMonth, int nDay);
void GetBirthDay(int nBirthDay, int & nrYear, int & nrMonth, int & nrDay);
short BirthdayToAge(const char *strBirthday);

int checkConstellation(int nMonth, int nDay);//判断星座

int GetWeekDay(); // 0- 6 sunday - saturday
int GetWeekDay(time_t nTime);
int GetCurHour();
int GetHour(time_t nTime);
int GetCurMinute();
int GetMinute(time_t nTime);
int GetCurSecond();
int GetSecond(time_t nTime);

int GetHourMinuteSecond(time_t nTime);

int GetYear(time_t nTime);
int GetCurYear();

int GetMonth(time_t nTime);
int GetCurMonth();

int GetMonthDay(time_t nTime);
int GetCurMonthDay();

int GetYearDay(time_t nTime);
int GetCurYearDay();

unsigned int PrevMondayStamp(time_t nTime);
unsigned int PrevSundayStamp(time_t nTime);
unsigned int PrevTimeStamp(time_t nTime, int nWeekday, int nHour);

int DiffDays(time_t t1, time_t t2);
int DiffDays(time_t t1, time_t t2, unsigned int standard);

time_t SetTime(int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond);
// Formart : YYYY-MM-DD hh:mm:ss
time_t SetTime(const char * szTime);

time_t GetNextDayStartTime(time_t nTime);
time_t GetTodayStartTime(time_t nTime);

bool IsTimeIsToday(time_t tTime);
//判断各个时间段是否是分开的，即没有交叉的
bool IsTimeRangeApart(const std::map<time_t, time_t>& mapTimeRange);

// 位操作
void SetBit(int & n, int nPos, int nValue);
void SetBit(__int64 & nNum, int nPos, int nValue);
int GetBit(__int64  nNum, int nPos);
int CountOne(__int64 nNum);
bool TransferItemList(const char * szItemList, std::list< CItem > & rItemList);
bool TransferItem(const char * szItem, CItem &item);
bool TransferList(const char * szItemList, std::vector< unsigned int > & rList);
bool TransferList(const char * szItemList, std::list< unsigned short > & rList);
void TransferString(const char * szItemList, unsigned int & nMinValue, unsigned int & nMaxValue);
bool TransferKeyValueList(const char *szKeyValueList, std::map<unsigned short, unsigned short> &rKeyvalueList);
bool TransferKeyValueList(const char *szKeyValueList, std::map<unsigned int, unsigned int> &rKeyvalueList);
bool TransferKeyValueStr(string &strValue, std::map<unsigned short, unsigned short> &keyValueList);

// 解析宝箱lua配置
void PacketAtoi(const char * szPacket, list<int> & listPacketDetail);

// 动态礼盒配置
bool TransferKeyDynamicBox(const char *szKeyValueList, unsigned int & nitemID, unsigned int & nRelatedRate, unsigned int & nItemRate);
// 解析首充奖励计划
bool AnalyzingChargePlan(int nChargeLevels, const char * strChargeRange, const char * strChargeMcoin, const char * strChargeMoney, std::vector< CChargePlan > & rChargePlan);
//判断目录的存在
bool CheckDirExist(const char* szDir);
//创建目录
bool CreateDir(const char* szDir);
//根据客户端版本获得充值事件id
int GetChargeEventID(int nVersionType);
//根据客户端版本获得充值奖励发送事件id
int GetChargeSendRewardEventID(int nVersionType);
//是否为自然日的同一天
bool IsSameDay(time_t time1, time_t time2);

// 是否为同一天的同一时刻(秒)
bool IsSameDayAndSecond(time_t time1, time_t time2);

int GetLocalDays(time_t nSeconds);			// 从1970到现在当地共经历多少天
int GetLocalHourOfDay(time_t nSeconds);		// 该时间点在当地是几点


template<typename T>
struct classCompPair 
{
	bool operator() (const pair<T, T>& pair_1, const pair<T, T>& pair_2) const
	{
		return (pair_1.first < pair_2.first) || (!(pair_1.first > pair_2.first && pair_1.first < pair_2.first) && (pair_1.second < pair_2.second));
	}
};

#define SAFE_NEW(x) (new(x))
#define CHECK_NULL_RETURN(x) if(x==NULL)return;

#endif//__COMMON_FUNC_H__

