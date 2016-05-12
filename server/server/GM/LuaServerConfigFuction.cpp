#include "../lua/InitLua.h"
#include <algorithm>
#include "priv/PrivMgr.h"

extern string g_sPath;

int lua_GetServerPath(lua_State * L)
{
    lua_pushstring(L, g_sPath.c_str());
    return 1;
}

int lua_AddPriv(lua_State * L)
{
    bool bValid = (lua_gettop(L) == 2 && lua_isstring(L, 1) && lua_isnumber(L, 2));
    if (!bValid)
    {
        PARAM_ERROR;
    }

    CPrivMgr::Instance().AddCmdInfo(lua_tostring(L, 1), (unsigned int)lua_tonumber(L, 2));

    return 0;
}

void CInitLua::RegLuaServerConfigFuction()
{
    REGFN(GetServerPath);
    REGFN(AddPriv);
}

