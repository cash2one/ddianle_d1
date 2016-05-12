/** CrontabTimer.h
* Description: ����ƻ���ʱ��������linux��Crontab����
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
	/// �ж�ʱ���ڱ�����[beginTimer�� endTimer]֮�ڣ���ҪbeginTimer��endTimer��ʽһ�£�ע�⣺������׼ȷ�жϲ�ͬ�����ڵ�ʱ�����
	/// </summary>
	/// <param name="const CCrontabTimer & beginTimer">��ʼʱ��</param>
	/// <param name="const CCrontabTimer & endTimer">����ʱ��</param>
	/// <param name="time_t tTime">���ж�ʱ��</param>
	/// <param name="bool bCheckFormat">�Ƿ���beginTimer��endTimerʱ���ʽһ��</param>
	/// <param name="bool bCheckInCurTimeCycle">�Ƿ��ڵ�ǰʱ�������м�飬�����Ƿ��ڵ�ǰ���ܼ���������ǰ����δ�����ܼ�</param>
	/// <returns>���</returns>
	static bool IsBetween(const CCrontab& beginTimer, const CCrontab& endTimer, time_t tTime, bool bCheckFormat, bool bCheckInCurTimeCycle);
	/// <summary>
	/// �ж��ڵ�ǰ�����ڣ�ʱ��tTime�Ƿ��ڱ�����[beginTimer�� endTimer]֮�ڣ���ҪbeginTimer��endTimer��ʽһ��
	/// </summary>
	/// <param name="const CCrontabTimer & beginTimer">��ʼʱ��</param>
	/// <param name="const CCrontabTimer & endTimer">����ʱ��</param>
	/// <param name="time_t tTime">���ж�ʱ��</param>
	/// <param name="bool bCheckFormat">�Ƿ���beginTimer��endTimerʱ���ʽһ��</param>
	/// <returns>���</returns>
	static bool IsBetweenInCurWeek(const CCrontab& beginTimer, const CCrontab& endTimer, time_t tTime, bool bCheckFormat);
	/// <summary>
	/// �ж�����ʱ���Ƿ��ʽ��ͬ
	/// </summary>
	/// <param name="const CCrontabTimer & a">ʱ��1</param>
	/// <param name="const CCrontabTimer & b">ʱ��2</param>
	/// <returns>���</returns>
	static bool IsSameFormat(const CCrontab& a, const CCrontab& b);
	/// <summary>
	/// �ж�����ʱ���Ƿ��ʽ��ͬ
	/// </summary>
	/// <param name="const std::vector<int> & a">ʱ��1</param>
	/// <param name="const std::vector<int> & b">ʱ��2</param>
	/// <returns>���</returns>
	static bool IsSameFormat(const std::vector<int>& a, const std::vector<int>& b);
private:
	/// <summary>
	/// ��time_tʱ���ʽת��Ϊ�����ʽ
	/// </summary>
	/// <param name="time_t tTime">time_t��ʽʱ��</param>
	/// <param name="vecTime">�����ʽʱ��</param>
	/// <param name="bSpecial">�Ƿ���Ҫ���⴦��������·ݶ�����ʵ�������֣�������7��ʾ</param>
	/// <returns></returns>
	static void ToTimeVec(time_t tTime, std::vector<int>& vecTime, bool bSpecial=true);
public:
	/// <summary>
	/// ���ַ�����ʽ���и�ֵ
	/// </summary>
	/// <param name="const std::string & strFormat">��ʾʱ��ĸ�ʽ�ַ���������"2014 * 1 * 1 0 0"��*��ʾ����</param>
	/// <param name="const std::string & strSep">�ָ������ϣ�����", |"��ʾ,�Ϳո�|���Ƿָ���</param>
	/// <returns>���</returns>
	bool AssignFormat(const std::string& strFormat, const std::string& strSep);
	/// <summary>
	/// �Ƿ��˻��߹���ʱ���
	/// </summary>
	/// <param name="time_t tNow">��ǰʱ��</param>
	/// <returns>���</returns>
	bool IsTimeDone(time_t tNow);
	/// <summary>
	/// �Ƿ��˻��߹��˵�ǰʱ���
	/// </summary>
	/// <returns>���</returns>
	bool IsTimeDone();
	/// <summary>
	/// ���ʱ������
	/// </summary>
	/// <returns>ʱ������</returns>
	const std::vector<int>& GetTimeVec() const;
	/// <summary>
	/// �������Ƿ�Ϸ�
	/// </summary>
	/// <returns>���</returns>
	bool IsValid();
	/// <summary>
	/// ���ʱ�������Ƿ�Ϸ�
	/// </summary>
	/// <param name="const std::vector<int> & vecTime">ʱ������</param>
	/// <returns>���</returns>
	bool CheckFormat(const std::vector<int>& vecTime);
private:
	/// <summary>
	/// ��ʱ������ת�������֣��������ֻ���ڱȽ�ʱ���С����time_t�޹�
	/// </summary>
	/// <param name="const std::vector<int> & vecTime"></param>
	/// <returns>���</returns>
	static __int64 ToNumber(const std::vector<int>& vecTime);
private:
	std::string m_strFormat;//�ַ�����ʽʱ���ʾ
	std::vector<int> m_vecTime;//ʱ������
};

#endif

