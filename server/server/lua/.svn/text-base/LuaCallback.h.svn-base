/** LuaCallback.h
* Description: lua 回调接口 辅助功能
* Copyright (C) 2014 ddianle Inc. All rights reserved.
* Author: fanliangyuan
* Create time: 2014-9-15 11:44:14
*/

#ifndef __LUA_CALLBACK_H__
#define __LUA_CALLBACK_H__

#include <string>

#ifdef WIN32
#pragma warning(disable:4127)   // 条件表达式是常量的警告
#endif // WIN32

extern "C"
{
#include "Lua.h"
#include "LuaLib.h"
#include "LauxLib.h"
}

template<class T>
bool LuaParamConvert(lua_State * L, int idx, T& t)
{
    if (lua_isnumber(L, idx))
    {
        t = (T)lua_tonumber(L, idx);
        return true;
    }
    return false;
}

template<>
bool LuaParamConvert(lua_State * L, int idx, std::string& t)
{
    if (lua_isstring(L, idx))
    {
        t = lua_tostring(L, idx);
        return true;
    }
    return false;
}

template<>
bool LuaParamConvert(lua_State * L, int idx, bool& t)
{
    if (lua_isboolean(L, idx))
    {
        t = (lua_toboolean(L, idx) != 0 ? true : false);
        return true;
    }
    return false;
}

#define CONVERT_FAIL_RETURN(t) do{if(!LuaParamConvert(L, i++, t)){return i-1;}}while(0)

enum EParseLuaParam
{
    EParseLuaParam_ParamCountError = -1,//参数个数不对
    EParseLuaParam_Success,//成功
};

//返回值为错误码或者参数下标，正数表示转换失败的参数下标
template<class T>
int ParseLuaParam(lua_State * L, T& t)
{
    int nRes = 0;
    nRes = (lua_gettop(L) == 1 ? EParseLuaParam_Success : EParseLuaParam_ParamCountError);
    if (nRes == EParseLuaParam_Success)
    {
        int i = 1;
        CONVERT_FAIL_RETURN(t);
    }
    return nRes;
}

template<class T1, class T2>
int ParseLuaParam(lua_State * L, T1& t1, T2& t2)
{
    int nRes = 0;
    nRes = (lua_gettop(L) == 2 ? EParseLuaParam_Success : EParseLuaParam_ParamCountError);
    if (nRes == EParseLuaParam_Success)
    {
        int i = 1;
        CONVERT_FAIL_RETURN(t1);
        CONVERT_FAIL_RETURN(t2);
    }
    return nRes;
}

template<class T1, class T2, class T3>
int ParseLuaParam(lua_State * L, T1& t1, T2& t2, T3& t3)
{
    int nRes = 0;
    nRes = (lua_gettop(L) == 3 ? EParseLuaParam_Success : EParseLuaParam_ParamCountError);
    if (nRes == EParseLuaParam_Success)
    {
        int i = 1;
        CONVERT_FAIL_RETURN(t1);
        CONVERT_FAIL_RETURN(t2);
        CONVERT_FAIL_RETURN(t3);
    }
    return nRes;
}

template<class T1, class T2, class T3, class T4>
int ParseLuaParam(lua_State * L, T1& t1, T2& t2, T3& t3, T4& t4)
{
    int nRes = 0;
    nRes = (lua_gettop(L) == 4 ? EParseLuaParam_Success : EParseLuaParam_ParamCountError);
    if (nRes == EParseLuaParam_Success)
    {
        int i = 1;
        CONVERT_FAIL_RETURN(t1);
        CONVERT_FAIL_RETURN(t2);
        CONVERT_FAIL_RETURN(t3);
        CONVERT_FAIL_RETURN(t4);
    }
    return nRes;
}

template<class T1, class T2, class T3, class T4, class T5>
int ParseLuaParam(lua_State * L, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5)
{
    int nRes = 0;
    nRes = (lua_gettop(L) == 5 ? EParseLuaParam_Success : EParseLuaParam_ParamCountError);
    if (nRes == EParseLuaParam_Success)
    {
        int i = 1;
        CONVERT_FAIL_RETURN(t1);
        CONVERT_FAIL_RETURN(t2);
        CONVERT_FAIL_RETURN(t3);
        CONVERT_FAIL_RETURN(t4);
        CONVERT_FAIL_RETURN(t5);
    }
    return nRes;
}

template<class T1, class T2, class T3, class T4, class T5, class T6>
int ParseLuaParam(lua_State * L, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6)
{
    int nRes = 0;
    nRes = (lua_gettop(L) == 6 ? EParseLuaParam_Success : EParseLuaParam_ParamCountError);
    if (nRes == EParseLuaParam_Success)
    {
        int i = 1;
        CONVERT_FAIL_RETURN(t1);
        CONVERT_FAIL_RETURN(t2);
        CONVERT_FAIL_RETURN(t3);
        CONVERT_FAIL_RETURN(t4);
        CONVERT_FAIL_RETURN(t5);
        CONVERT_FAIL_RETURN(t6);
    }
    return nRes;
}

template<class T1, class T2, class T3, class T4, class T5, class T6, class T7>
int ParseLuaParam(lua_State * L, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7)
{
    int nRes = 0;
    nRes = (lua_gettop(L) == 7 ? EParseLuaParam_Success : EParseLuaParam_ParamCountError);
    if (nRes == EParseLuaParam_Success)
    {
        int i = 1;
        CONVERT_FAIL_RETURN(t1);
        CONVERT_FAIL_RETURN(t2);
        CONVERT_FAIL_RETURN(t3);
        CONVERT_FAIL_RETURN(t4);
        CONVERT_FAIL_RETURN(t5);
        CONVERT_FAIL_RETURN(t6);
        CONVERT_FAIL_RETURN(t7);
    }
    return nRes;
}

template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
int ParseLuaParam(lua_State * L, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, T8& t8)
{
    int nRes = 0;
    nRes = (lua_gettop(L) == 8 ? EParseLuaParam_Success : EParseLuaParam_ParamCountError);
    if (nRes == EParseLuaParam_Success)
    {
        int i = 1;
        CONVERT_FAIL_RETURN(t1);
        CONVERT_FAIL_RETURN(t2);
        CONVERT_FAIL_RETURN(t3);
        CONVERT_FAIL_RETURN(t4);
        CONVERT_FAIL_RETURN(t5);
        CONVERT_FAIL_RETURN(t6);
        CONVERT_FAIL_RETURN(t7);
        CONVERT_FAIL_RETURN(t8);
    }
    return nRes;
}

template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
int ParseLuaParam(lua_State * L, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, T8& t8, T9& t9)
{
    int nRes = 0;
    nRes = (lua_gettop(L) == 9 ? EParseLuaParam_Success : EParseLuaParam_ParamCountError);
    if (nRes == EParseLuaParam_Success)
    {
        int i = 1;
        CONVERT_FAIL_RETURN(t1);
        CONVERT_FAIL_RETURN(t2);
        CONVERT_FAIL_RETURN(t3);
        CONVERT_FAIL_RETURN(t4);
        CONVERT_FAIL_RETURN(t5);
        CONVERT_FAIL_RETURN(t6);
        CONVERT_FAIL_RETURN(t7);
        CONVERT_FAIL_RETURN(t8);
        CONVERT_FAIL_RETURN(t9);
    }
    return nRes;
}

template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
int ParseLuaParam(lua_State * L, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, T8& t8, T9& t9, T10& t10)
{
    int nRes = 0;
    nRes = (lua_gettop(L) == 10 ? EParseLuaParam_Success : EParseLuaParam_ParamCountError);
    if (nRes == EParseLuaParam_Success)
    {
        int i = 1;
        CONVERT_FAIL_RETURN(t1);
        CONVERT_FAIL_RETURN(t2);
        CONVERT_FAIL_RETURN(t3);
        CONVERT_FAIL_RETURN(t4);
        CONVERT_FAIL_RETURN(t5);
        CONVERT_FAIL_RETURN(t6);
        CONVERT_FAIL_RETURN(t7);
        CONVERT_FAIL_RETURN(t8);
        CONVERT_FAIL_RETURN(t9);
        CONVERT_FAIL_RETURN(t10);
    }
    return nRes;
}

template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11>
int ParseLuaParam(lua_State * L, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, T8& t8, T9& t9, T10& t10, T11& t11)
{
    int nRes = 0;
    nRes = (lua_gettop(L) == 11 ? EParseLuaParam_Success : EParseLuaParam_ParamCountError);
    if (nRes == EParseLuaParam_Success)
    {
        int i = 1;
        CONVERT_FAIL_RETURN(t1);
        CONVERT_FAIL_RETURN(t2);
        CONVERT_FAIL_RETURN(t3);
        CONVERT_FAIL_RETURN(t4);
        CONVERT_FAIL_RETURN(t5);
        CONVERT_FAIL_RETURN(t6);
        CONVERT_FAIL_RETURN(t7);
        CONVERT_FAIL_RETURN(t8);
        CONVERT_FAIL_RETURN(t9);
        CONVERT_FAIL_RETURN(t10);
        CONVERT_FAIL_RETURN(t11);
    }
    return nRes;
}

template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12>
int ParseLuaParam(lua_State * L, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, T8& t8, T9& t9, T10& t10, T11& t11, T12& t12)
{
    int nRes = 0;
    nRes = (lua_gettop(L) == 12 ? EParseLuaParam_Success : EParseLuaParam_ParamCountError);
    if (nRes == EParseLuaParam_Success)
    {
        int i = 1;
        CONVERT_FAIL_RETURN(t1);
        CONVERT_FAIL_RETURN(t2);
        CONVERT_FAIL_RETURN(t3);
        CONVERT_FAIL_RETURN(t4);
        CONVERT_FAIL_RETURN(t5);
        CONVERT_FAIL_RETURN(t6);
        CONVERT_FAIL_RETURN(t7);
        CONVERT_FAIL_RETURN(t8);
        CONVERT_FAIL_RETURN(t9);
        CONVERT_FAIL_RETURN(t10);
        CONVERT_FAIL_RETURN(t11);
        CONVERT_FAIL_RETURN(t12);
    }
    return nRes;
}

template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10,
class T11, class T12, class T13>
int ParseLuaParam(lua_State * L, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, T8& t8, T9& t9, T10& t10,
    T11& t11, T12& t12, T13& t13)
{
    int nRes = 0;
    nRes = (lua_gettop(L) == 13 ? EParseLuaParam_Success : EParseLuaParam_ParamCountError);
    if (nRes == EParseLuaParam_Success)
    {
        int i = 1;
        CONVERT_FAIL_RETURN(t1);
        CONVERT_FAIL_RETURN(t2);
        CONVERT_FAIL_RETURN(t3);
        CONVERT_FAIL_RETURN(t4);
        CONVERT_FAIL_RETURN(t5);
        CONVERT_FAIL_RETURN(t6);
        CONVERT_FAIL_RETURN(t7);
        CONVERT_FAIL_RETURN(t8);
        CONVERT_FAIL_RETURN(t9);
        CONVERT_FAIL_RETURN(t10);
        CONVERT_FAIL_RETURN(t11);
        CONVERT_FAIL_RETURN(t12);
        CONVERT_FAIL_RETURN(t13);
    }
    return nRes;
}

template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10,
class T11, class T12, class T13, class T14>
int ParseLuaParam(lua_State * L, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, T8& t8, T9& t9, T10& t10,
    T11& t11, T12& t12, T13& t13, T14& t14)
{
    int nRes = 0;
    nRes = (lua_gettop(L) == 14 ? EParseLuaParam_Success : EParseLuaParam_ParamCountError);
    if (nRes == EParseLuaParam_Success)
    {
        int i = 1;
        CONVERT_FAIL_RETURN(t1);
        CONVERT_FAIL_RETURN(t2);
        CONVERT_FAIL_RETURN(t3);
        CONVERT_FAIL_RETURN(t4);
        CONVERT_FAIL_RETURN(t5);
        CONVERT_FAIL_RETURN(t6);
        CONVERT_FAIL_RETURN(t7);
        CONVERT_FAIL_RETURN(t8);
        CONVERT_FAIL_RETURN(t9);
        CONVERT_FAIL_RETURN(t10);
        CONVERT_FAIL_RETURN(t11);
        CONVERT_FAIL_RETURN(t12);
        CONVERT_FAIL_RETURN(t13);
        CONVERT_FAIL_RETURN(t14);
    }
    return nRes;
}

template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, 
class T11, class T12, class T13, class T14, class T15>
int ParseLuaParam(lua_State * L, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, T8& t8, T9& t9, T10& t10,
    T11& t11, T12& t12, T13& t13, T14& t14, T15& t15)
{
    int nRes = 0;
    nRes = (lua_gettop(L) == 15 ? EParseLuaParam_Success : EParseLuaParam_ParamCountError);
    if (nRes == EParseLuaParam_Success)
    {
        int i = 1;
        CONVERT_FAIL_RETURN(t1);
        CONVERT_FAIL_RETURN(t2);
        CONVERT_FAIL_RETURN(t3);
        CONVERT_FAIL_RETURN(t4);
        CONVERT_FAIL_RETURN(t5);
        CONVERT_FAIL_RETURN(t6);
        CONVERT_FAIL_RETURN(t7);
        CONVERT_FAIL_RETURN(t8);
        CONVERT_FAIL_RETURN(t9);
        CONVERT_FAIL_RETURN(t10);
        CONVERT_FAIL_RETURN(t11);
        CONVERT_FAIL_RETURN(t12);
        CONVERT_FAIL_RETURN(t13);
        CONVERT_FAIL_RETURN(t14);
        CONVERT_FAIL_RETURN(t15);
    }
    return nRes;
}

template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10,
class T11, class T12, class T13, class T14, class T15, class T16>
int ParseLuaParam(lua_State * L, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, T8& t8, T9& t9, T10& t10,
    T11& t11, T12& t12, T13& t13, T14& t14, T15& t15, T16& t16)
{
    int nRes = 0;
    nRes = (lua_gettop(L) == 16 ? EParseLuaParam_Success : EParseLuaParam_ParamCountError);
    if (nRes == EParseLuaParam_Success)
    {
        int i = 1;
        CONVERT_FAIL_RETURN(t1);
        CONVERT_FAIL_RETURN(t2);
        CONVERT_FAIL_RETURN(t3);
        CONVERT_FAIL_RETURN(t4);
        CONVERT_FAIL_RETURN(t5);
        CONVERT_FAIL_RETURN(t6);
        CONVERT_FAIL_RETURN(t7);
        CONVERT_FAIL_RETURN(t8);
        CONVERT_FAIL_RETURN(t9);
        CONVERT_FAIL_RETURN(t10);
        CONVERT_FAIL_RETURN(t11);
        CONVERT_FAIL_RETURN(t12);
        CONVERT_FAIL_RETURN(t13);
        CONVERT_FAIL_RETURN(t14);
        CONVERT_FAIL_RETURN(t15);
        CONVERT_FAIL_RETURN(t16);
    }
    return nRes;
}

template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10,
class T11, class T12, class T13, class T14, class T15, class T16, class T17>
int ParseLuaParam(lua_State * L, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, T8& t8, T9& t9, T10& t10,
    T11& t11, T12& t12, T13& t13, T14& t14, T15& t15, T16& t16, T17& t17)
{
    int nRes = 0;
    nRes = (lua_gettop(L) == 17 ? EParseLuaParam_Success : EParseLuaParam_ParamCountError);
    if (nRes == EParseLuaParam_Success)
    {
        int i = 1;
        CONVERT_FAIL_RETURN(t1);
        CONVERT_FAIL_RETURN(t2);
        CONVERT_FAIL_RETURN(t3);
        CONVERT_FAIL_RETURN(t4);
        CONVERT_FAIL_RETURN(t5);
        CONVERT_FAIL_RETURN(t6);
        CONVERT_FAIL_RETURN(t7);
        CONVERT_FAIL_RETURN(t8);
        CONVERT_FAIL_RETURN(t9);
        CONVERT_FAIL_RETURN(t10);
        CONVERT_FAIL_RETURN(t11);
        CONVERT_FAIL_RETURN(t12);
        CONVERT_FAIL_RETURN(t13);
        CONVERT_FAIL_RETURN(t14);
        CONVERT_FAIL_RETURN(t15);
        CONVERT_FAIL_RETURN(t16);
        CONVERT_FAIL_RETURN(t17);
    }
    return nRes;
}


template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10,
class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18>
    int ParseLuaParam(lua_State * L, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, T8& t8, T9& t9, T10& t10,
    T11& t11, T12& t12, T13& t13, T14& t14, T15& t15, T16& t16, T17& t17, T18& t18)
{
    int nRes = 0;
    nRes = (lua_gettop(L) == 18 ? EParseLuaParam_Success : EParseLuaParam_ParamCountError);
    if (nRes == EParseLuaParam_Success)
    {
        int i = 1;
        CONVERT_FAIL_RETURN(t1);
        CONVERT_FAIL_RETURN(t2);
        CONVERT_FAIL_RETURN(t3);
        CONVERT_FAIL_RETURN(t4);
        CONVERT_FAIL_RETURN(t5);
        CONVERT_FAIL_RETURN(t6);
        CONVERT_FAIL_RETURN(t7);
        CONVERT_FAIL_RETURN(t8);
        CONVERT_FAIL_RETURN(t9);
        CONVERT_FAIL_RETURN(t10);
        CONVERT_FAIL_RETURN(t11);
        CONVERT_FAIL_RETURN(t12);
        CONVERT_FAIL_RETURN(t13);
        CONVERT_FAIL_RETURN(t14);
        CONVERT_FAIL_RETURN(t15);
        CONVERT_FAIL_RETURN(t16);
        CONVERT_FAIL_RETURN(t17);
        CONVERT_FAIL_RETURN(t18);
    }
    return nRes;
}


template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10,
class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19>
    int ParseLuaParam(lua_State * L, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6, T7& t7, T8& t8, T9& t9, T10& t10,
    T11& t11, T12& t12, T13& t13, T14& t14, T15& t15, T16& t16, T17& t17, T18& t18, T19& t19)
{
    int nRes = 0;
    nRes = (lua_gettop(L) == 19 ? EParseLuaParam_Success : EParseLuaParam_ParamCountError);
    if (nRes == EParseLuaParam_Success)
    {
        int i = 1;
        CONVERT_FAIL_RETURN(t1);
        CONVERT_FAIL_RETURN(t2);
        CONVERT_FAIL_RETURN(t3);
        CONVERT_FAIL_RETURN(t4);
        CONVERT_FAIL_RETURN(t5);
        CONVERT_FAIL_RETURN(t6);
        CONVERT_FAIL_RETURN(t7);
        CONVERT_FAIL_RETURN(t8);
        CONVERT_FAIL_RETURN(t9);
        CONVERT_FAIL_RETURN(t10);
        CONVERT_FAIL_RETURN(t11);
        CONVERT_FAIL_RETURN(t12);
        CONVERT_FAIL_RETURN(t13);
        CONVERT_FAIL_RETURN(t14);
        CONVERT_FAIL_RETURN(t15);
        CONVERT_FAIL_RETURN(t16);
        CONVERT_FAIL_RETURN(t17);
        CONVERT_FAIL_RETURN(t18);
        CONVERT_FAIL_RETURN(t19);
    }
    return nRes;
}

#endif

