
#ifndef __DATAINFORDEFINE_H__
#define __DATAINFORDEFINE_H__


#include <vector>
enum Param_Type
{
	ParamType_Begin = -1,
	ParamType_Bool		,
	ParamType_Long		,
	ParamType_float		,
	ParamType_Percent	,	//�ٷֱ�����������50��ʾ50%
	ParamType_String	,
	ParamType_Memory	,
	ParamType_Short		,
	ParamType_UChar		,
	ParamType_Time		,	//����long(���룩,�洢ʱ��λ��short(��)
	ParamType_ItemID	,	//long,��ƷID��ֻ�ܴ洢ID<MaxShort��ֵ,ʹ�ô����Ĺ��ܲ�������
	ParamType_SkillID	,	//long,����ID��ֻ�ܴ洢ID<MaxShort��ֵ,ʹ�ô����Ĺ��ܲ�������

	ParamType_MaxNumber
};
struct SimpleValue
{
	Param_Type m_ParamType;	//bool,long,float,percent
	union
	{
		long m_lValue;
		float m_fValue;
	};
};

struct SimpleRangeValue
{
	Param_Type m_ParamType;	//bool,long,float,percent
	union
	{
		long m_lMinValue;
		float m_fMinValue;
	};
	union
	{
		long m_lMaxValue;
		float m_fMaxValue;
	};
};

#endif//__DATAINFORDEFINE_H__



//end file


