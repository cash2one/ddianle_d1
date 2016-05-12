
#ifndef __DATAINFORDEFINE_H__
#define __DATAINFORDEFINE_H__


#include <vector>
enum Param_Type
{
	ParamType_Begin = -1,
	ParamType_Bool		,
	ParamType_Long		,
	ParamType_float		,
	ParamType_Percent	,	//百分比整数，比如50表示50%
	ParamType_String	,
	ParamType_Memory	,
	ParamType_Short		,
	ParamType_UChar		,
	ParamType_Time		,	//整数long(毫秒）,存储时单位是short(秒)
	ParamType_ItemID	,	//long,物品ID，只能存储ID<MaxShort的值,使用触发的功能不受限制
	ParamType_SkillID	,	//long,技能ID，只能存储ID<MaxShort的值,使用触发的功能不受限制

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


