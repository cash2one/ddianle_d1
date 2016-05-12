/////////////////////////////////////////////////////////////////////////////
// Name:        InitLua.h
// Purpose:     C++ 与Lua脚本通信的State初始化类
// Author:      梅久华
// Created:     2010/12/18  
/////////////////////////////////////////////////////////////////////////////

#include "InitLua.h"
std::list<const char *> g_luaFNList;

CInitLua::CInitLua(void)
{
    L = NULL;
}

CInitLua::~CInitLua(void)
{
    lua_close(L);
}

void CInitLua::InitState()
{
    L = luaL_newstate();

    lua_cpcall(L, luaopen_base, 0);
    lua_cpcall(L, luaopen_io, 0);
    lua_cpcall(L, luaopen_string, 0);
    lua_cpcall(L, luaopen_math, 0);
    lua_cpcall(L, luaopen_os, 0);
    lua_cpcall(L, luaopen_table, 0);

    RegLuaRoleFuction();
    RegLuaServerConfigFuction();
}

lua_State * CInitLua::GetLua_State()
{
    if (L == NULL)
    {
        InitState();
    }

    return L;
}

bool CInitLua::LuaLoadFile(const char *filePathName)
{
    if (luaL_loadfile(L, filePathName) || lua_pcall(L, 0, 0, 0))
    {
        return false;
    }

    return true;
}

int CInitLua::DoString(const char *csString, char chRetType, int nRetNum, ...)
{
    lua_getglobal(L, csString);
    if (!lua_isfunction(L, -1)) // 不是函数名
    {
        lua_pop(L, 1);
        lua_settop(L, 0);

        WriteLog(LOGLEVEL_ERROR, "没有定义的DoString(%s)\n", csString);
        return -1;
    }

    if (nRetNum > DOSTRING_RETURN_NUM)
    {
        lua_settop(L, 0);

        WriteLog(LOGLEVEL_ERROR, "msgDoString(%s) 返回值个数错误！！！\n", csString);
        return -1;
    }

    va_list list;
    va_start(list, nRetNum);
    int nParam, nParamNum = 0, nNum;
    void *pointer;
    while ((nParam = va_arg(list, int)) != DOSTRING_PARAM_END)
    {
        switch (nParam)
        {
        case enumSCRIPT_PARAM_NUMBER:
            nNum = va_arg(list, int);
            nParamNum += nNum;
            while (nNum-- > 0)
                lua_pushnumber(L, va_arg(list, int));
            break;
        case enumSCRIPT_PARAM_NUMBER_UNSIGNED:
            nNum = va_arg(list, int);
            nParamNum += nNum;
            while (nNum-- > 0)
                lua_pushnumber(L, va_arg(list, unsigned int));
            break;
        case enumSCRIPT_PARAM_LIGHTUSERDATA:
            nNum = va_arg(list, int);
            nParamNum += nNum;
            while (nNum-- > 0)
            {
                pointer = va_arg(list, void *);
                lua_pushlightuserdata(L, pointer);
            }
            break;
        case enumSCRIPT_PARAM_STRING:
            nNum = va_arg(list, int);
            nParamNum += nNum;
            while (nNum-- > 0)
                lua_pushstring(L, va_arg(list, char *));
            break;
        default:
            WriteLog(LOGLEVEL_ERROR, "msgDoString(%s) 参数类型错误！！！\n", csString);
            lua_settop(L, 0);
            return -1;
            break;
        }
    }
    va_end(list);
    int nState = lua_pcall(L, nParamNum, LUA_MULTRET, 0);
    if (nState != 0)
    {
        WriteLog(LOGLEVEL_DEBUG, "DoString %s\n", csString);
        lua_callalert(L, nState);

        lua_settop(L, 0);
        return -1;
    }

    int	nRet = -1;
    int i = 0;
    for (; i < nRetNum; i++)
    {
        if (chRetType == enumSCRIPT_RETURN_NUMBER)
        {
            if (!lua_isnumber(L, -1 - i))
            {
                WriteLog(LOGLEVEL_ERROR, "调用脚本 %s（参数%d个，返回值%d个） 时，其返回值类型不匹配！\n", csString, nParamNum, nRetNum);
                nRet = -1;
                break;
            }
            m_nDoStringRet[nRetNum - 1 - i] = (int)lua_tonumber(L, -1 - i);
        }
        else if (chRetType == enumSCRIPT_RETURN_STRING)
        {
            if (!lua_isstring(L, -1 - i))
            {
                WriteLog(LOGLEVEL_ERROR, "调用脚本 %s（参数%d个，返回值%d个） 时，其返回值类型不匹配！\n", csString, nParamNum, nRetNum);
                nRet = -1;
                break;
            }
            SetRetString((unsigned char)(nRetNum - 1 - i), lua_tostring(L, -1 - i));
        }
        else
        {
            WriteLog(LOGLEVEL_ERROR, "msgDoString(%s) 返回值类型错误！！！\n", csString);
            lua_settop(L, 0);
            return -1;
        }

        nRet = i + 1;
    }
    lua_settop(L, 0);

    return nRet;
}

