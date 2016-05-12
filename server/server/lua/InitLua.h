/////////////////////////////////////////////////////////////////////////////
// Name:        InitLua.h
// Purpose:     C++ 与Lua脚本通信的State初始化类
// Author:      梅久华
// Created:     2010/12/18  
/////////////////////////////////////////////////////////////////////////////

#ifndef _INIT_LUA_H_
#define _INIT_LUA_H_

#include <iostream>
#include <string>
#include <list>
#include <algorithm>
#include <string.h>
#include "../../socket/Log.h"

extern "C"
{
#include "Lua.h"
#include "LuaLib.h"
#include "LauxLib.h"
}

#ifdef WIN32
#pragma warning(disable:4996) // 去除警告sprintf/strncpy
#endif

using namespace std;

extern std::list<const char *> g_luaFNList;

#define REGFN_INIT g_luaFNList.clear();
#define PARAM_ERROR { WriteLog(LOGLEVEL_ERROR, "lua扩展函数[%s]参数个数或者类型错误!",__FUNCTION__ ); }
#define REGFN(fn) \
{ \
    lua_pushstring(L, "" #fn ""); \
    lua_pushcfunction(L, lua_##fn); \
    lua_settable(L, LUA_GLOBALSINDEX); \
    if (find(g_luaFNList.begin(), g_luaFNList.end(), ""#fn"") != g_luaFNList.end()) { \
        WriteLog(LOGLEVEL_ERROR, "msg发现登记lua同名函数[%s]\n", ""#fn""); \
    } \
    else { \
        g_luaFNList.push_back(""#fn""); \
    } \
} 

#define DOSTRING_PARAM_END	999999999
#define DOSTRING_RETURN_NUM	20
#define DOSTRING_RETURN_STRING_LEN	512

enum EScriptParamType
{
    enumSCRIPT_PARAM_NUMBER = 1,
    enumSCRIPT_PARAM_NUMBER_UNSIGNED,
    enumSCRIPT_PARAM_LIGHTUSERDATA,
    enumSCRIPT_PARAM_STRING,
};

enum EScriptReturnType
{
    enumSCRIPT_RETURN_NONE,
    enumSCRIPT_RETURN_NUMBER,
    enumSCRIPT_RETURN_STRING,
};


class CInitLua
{
private:
    CInitLua(void);
    ~CInitLua(void);

public:
    static CInitLua& GetInstance()
    {
        static CInitLua sLua;
        return sLua;
    }

    void InitState();

    lua_State * GetLua_State();
    bool LuaLoadFile(const char *filePathName);

    //根据函数名调用函数。
    int DoString(const char *csString, char chRetType, int nRetNum, ...);

    char *GetRetString(unsigned char chID)
    {
        char * pResult = NULL;
        if (chID < DOSTRING_RETURN_NUM)
        {
            pResult = m_szDoStringRet[chID];
        }
        return pResult;
    }

    bool GetRetNum(int & nResult, unsigned char chID)
    {
        bool bResult = false;
        if (chID < DOSTRING_RETURN_NUM)
        {
            nResult = m_nDoStringRet[chID];
            bResult = true;
        }
        return bResult;
    }

protected:
    bool SetRetString(unsigned char chID, const char *cszString)
    {
        if (chID >= DOSTRING_RETURN_NUM) return false;
        strncpy(m_szDoStringRet[chID], cszString, DOSTRING_RETURN_STRING_LEN - 1);
        m_szDoStringRet[chID][DOSTRING_RETURN_STRING_LEN - 1] = '\0';
        return true;
    }

    // 使用lua pcall的错误报告函数
    void lua_callalert(lua_State* L, int status)
    {
        if (status != 0)
        {
            lua_getglobal(L, "_ALERT");
            if (lua_isfunction(L, -1))
            {
                lua_insert(L, -2);
                lua_call(L, 1, 0);
            }
            else
            { // no _ALERT function; print it on stderr
                WriteLog(LOGLEVEL_ERROR, "lua_err", "%s\n", lua_tostring(L, -2));
                lua_pop(L, 2);
            }
        }
    }

    void RegLuaRoleFuction();
    void RegLuaServerConfigFuction();

private:
    int         m_nDoStringRet[DOSTRING_RETURN_NUM];
    char        m_szDoStringRet[DOSTRING_RETURN_NUM][DOSTRING_RETURN_STRING_LEN];

    lua_State   *L;

};

#endif //_INIT_LUA_H_

