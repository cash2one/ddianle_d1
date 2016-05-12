/** CrontabTimer.h
* Description: 任务计划定时器，类似linux的Crontab功能
* Copyright (C) 2014 ddianle Inc. All rights reserved.
* Author: fanliangyuan
* Create time: 2014-11-4 16:21:09
*/
#ifndef __CRONTAB_H__
#define __CRONTAB_H__

#include <time.h>
#include <string>
#include <vector>

#ifndef WIN32
typedef long long         __int64;
#endif

enum ETimeUnit
{
	ETimeUnit_Year,
	ETimeUnit_Month,
	ETimeUnit_WeekDay,
	ETimeUnit_Day,
	ETimeUnit_Hour,
	ETimeUnit_Minute,
	ETimeUnit_Second,
	ETimeUnit_Max
};

class CCrontab
{
public:
	CCrontab(){}
	virtual ~CCrontab(){}
public:
	/// <summary>
	/// 判断时间在闭区间[beginTimer， endTimer]之内，需要beginTimer与endTimer格式一致，注意：并不能准确判断不同周期内的时间归属
	/// </summary>
	/// <param name="const CCrontabTimer & beginTimer">开始时间</param>
	/// <param name="const CCrontabTimer & endTimer">结束时间</param>
	/// <param name="time_t tTime">被判断时间</param>
	/// <param name="bool bCheckFormat">是否检查beginTimer与endTimer时间格式一致</param>
	/// <param name="bool bCheckInCurTimeCycle">是否在当前时间周期中检查，比如是否在当前的周几而不是以前或者未来的周几</param>
	/// <returns>结果</returns>
	static bool IsBetween(const CCrontab& beginTimer, const CCrontab& endTimer, time_t tTime, bool bCheckFormat, bool bCheckInCurTimeCycle);
	/// <summary>
	/// 判断在当前星期内，时间tTime是否在闭区间[beginTimer， endTimer]之内，需要beginTimer与endTimer格式一致
	/// </summary>
	/// <param name="const CCrontabTimer & beginTimer">开始时间</param>
	/// <param name="const CCrontabTimer & endTimer">结束时间</param>
	/// <param name="time_t tTime">被判断时间</param>
	/// <param name="bool bCheckFormat">是否检查beginTimer与endTimer时间格式一致</param>
	/// <returns>结果</returns>
	static bool IsBetweenInCurWeek(const CCrontab& beginTimer, const CCrontab& endTimer, time_t tTime, bool bCheckFormat);
	/// <summary>
	/// 判断两个时间是否格式相同
	/// </summary>
	/// <param name="const CCrontabTimer & a">时间1</param>
	/// <param name="const CCrontabTimer & b">时间2</param>
	/// <returns>结果</returns>
	static bool IsSameFormat(const CCrontab& a, const CCrontab& b);
	/// <summary>
	/// 判断两个时间是否格式相同
	/// </summary>
	/// <param name="const std::vector<int> & a">时间1</param>
	/// <param name="const std::vector<int> & b">时间2</param>
	/// <returns>结果</returns>
	static bool IsSameFormat(const std::vector<int>& a, const std::vector<int>& b);
private:
	/// <summary>
	/// 将time_t时间格式转换为数组格式
	/// </summary>
	/// <param name="time_t tTime">time_t格式时间</param>
	/// <param name="vecTime">数组格式时间</param>
	/// <param name="bSpecial">是否需要特殊处理，即年份月份都用现实生活数字，周日用7表示</param>
	/// <returns></returns>
	static void ToTimeVec(time_t tTime, std::vector<int>& vecTime, bool bSpecial=true);
public:
	/// <summary>
	/// 以字符串格式进行赋值
	/// </summary>
	/// <param name="const std::string & strFormat">表示时间的格式字符串，例如"2014 * 1 * 1 0 0"，*表示任意</param>
	/// <param name="const std::string & strSep">分隔符集合，例如", |"表示,和空格、|都是分隔符</param>
	/// <returns>结果</returns>
	bool AssignFormat(const std::string& strFormat, const std::string& strSep);
	/// <summary>
	/// 是否到了或者过了时间点
	/// </summary>
	/// <param name="time_t tNow">当前时间</param>
	/// <returns>结果</returns>
	bool IsTimeDone(time_t tNow);
	/// <summary>
	/// 是否到了或者过了当前时间点
	/// </summary>
	/// <returns>结果</returns>
	bool IsTimeDone();
	/// <summary>
	/// 获得时间数组
	/// </summary>
	/// <returns>时间数组</returns>
	const std::vector<int>& GetTimeVec() const;
	/// <summary>
	/// 本对象是否合法
	/// </summary>
	/// <returns>结果</returns>
	bool IsValid();
	/// <summary>
	/// 检测时间数组是否合法
	/// </summary>
	/// <param name="const std::vector<int> & vecTime">时间数组</param>
	/// <returns>结果</returns>
	bool CheckFormat(const std::vector<int>& vecTime);
private:
	/// <summary>
	/// 将时间数组转换成数字，结果数字只用于比较时间大小，与time_t无关
	/// </summary>
	/// <param name="const std::vector<int> & vecTime"></param>
	/// <returns>结果</returns>
	static __int64 ToNumber(const std::vector<int>& vecTime);
private:
	std::string m_strFormat;//字符串形式时间表示
	std::vector<int> m_vecTime;//时间数组
};

#endif

