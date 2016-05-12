#include "../../datastructure/DataStruct_Game.h"
#include "InitLua.h"
#include "../payment/PaymentInfoMgr.h"
#include "../util/CommonFunc.h"
#include "LuaCallback.h"
#include "LuaCallback.h"
#include "../account/AccountConfigMgr.h"
#include "../account/LoginCirculator.h"

extern string g_sPath;
extern CLoginCirculator *g_pCirculator;

void CInitLua::RegLuaRoleFuction()
{
}

int lua_GetServerPath(lua_State * L)
{
    lua_pushstring(L, g_sPath.c_str());
    return 1;
}

int lua_AddFirstRechargeInfo(lua_State *L)
{
    bool bValid = (lua_gettop(L) == 11
        && lua_isboolean(L, 1)	// version

        && lua_isstring(L, 2)		// Start Time
        && lua_isstring(L, 3)		// End Time

        && lua_isstring(L, 4)		// Male Reward
        && lua_isstring(L, 5)		// Female Reward

        && lua_isstring(L, 6)		// Mail Title
        && lua_isstring(L, 7)		// Mail Content

        && lua_isnumber(L, 8)		// Charge Levels
        && lua_isstring(L, 9)		// Charge Range
        && lua_isstring(L, 10)	// Charge Mcoin
        && lua_isstring(L, 11)	// Charge Money
        );
    if (!bValid)
    {
        PARAM_ERROR;
        return 0;
    }

    Version_Type vType = lua_toboolean(L, 1) != 0 ? Version_Free : Version_Charge;
    std::string strStartTime = lua_tostring(L, 2);
    std::string strEndTime = lua_tostring(L, 3);

    std::string strMaleItem = lua_tostring(L, 4);
    std::string strFemaleItem = lua_tostring(L, 5);

    std::string strMailTitle = lua_tostring(L, 6);
    std::string strMaileContent = lua_tostring(L, 7);

    int nChargeLevel = (unsigned int)lua_tonumber(L, 8);
    std::string strChargeRange = lua_tostring(L, 9);
    std::string strChargeMcoin = lua_tostring(L, 10);
    std::string strChargeMoney = lua_tostring(L, 11);


    std::list<CItem> listMaleItem;
    std::list<CItem> listFemaleItem;
    TransferItemList(strMaleItem.c_str(), listMaleItem);
    TransferItemList(strFemaleItem.c_str(), listFemaleItem);

    std::vector<CChargePlan> vecChargePlan;
    if (!AnalyzingChargePlan(nChargeLevel, strChargeRange.c_str(), strChargeMcoin.c_str(), strChargeMoney.c_str(), vecChargePlan))
    {
        PARAM_ERROR;
        return 0;
    }

    PaymentInfoMgr::Instance().ReadFirstPaymentReward(vType, listMaleItem, listFemaleItem, nChargeLevel, vecChargePlan,
        strMailTitle, strMaileContent, SetTime(strStartTime.c_str()), SetTime(strEndTime.c_str()), strMaleItem, strFemaleItem);

    return 1;
}

int lua_AddRechargeInfo(lua_State *L)
{
    bool bValid = (lua_gettop(L) == 9
        && lua_isboolean(L, 1)	// version

        && lua_isstring(L, 2)		// Start Time
        && lua_isstring(L, 3)		// End Time

        && lua_isnumber(L, 4)		// Charge Levels
        && lua_isstring(L, 5)		// Charge Range
        && lua_isstring(L, 6)		// Charge Mcoin
        && lua_isstring(L, 7)		// Charge Money
        && lua_isstring(L, 8)		// Mail Title
        && lua_isstring(L, 9)		// Mail Content
        );
    if (!bValid)
    {
        PARAM_ERROR;
        return 0;
    }

    Version_Type vType = lua_toboolean(L, 1) != 0 ? Version_Free : Version_Charge;
    std::string strStartTime = lua_tostring(L, 2);
    std::string strEndTime = lua_tostring(L, 3);

    int nChargeLevel = (int)lua_tonumber(L, 4);
    std::string strChargeRange = lua_tostring(L, 5);
    std::string strChargeMcoin = lua_tostring(L, 6);
    std::string strChargeMoney = lua_tostring(L, 7);
    std::string strMailTitle = lua_tostring(L, 8);
    std::string strMailContent = lua_tostring(L, 9);

    std::vector<CChargePlan> vecChargePlan;
    if (!AnalyzingChargePlan(nChargeLevel, strChargeRange.c_str(), strChargeMcoin.c_str(), strChargeMoney.c_str(), vecChargePlan))
    {
        PARAM_ERROR;
        return 0;
    }

    PaymentInfoMgr::Instance().ReadRechargeReward(vType, nChargeLevel, vecChargePlan, SetTime(strStartTime.c_str()), SetTime(strEndTime.c_str()), strMailTitle, strMailContent);

    return 1;
}

int lua_AddFixedChargeInfo(lua_State *L)
{
    unsigned int mCoin = 0;
    unsigned int giftCoin = 0;
    std::string item;
    if (EParseLuaParam_Success != ParseLuaParam(L, mCoin, giftCoin, item))
    {
        PARAM_ERROR;
        return 0;
    }

    PaymentInfoMgr::Instance().AddFixedChargeInfo(mCoin, giftCoin, item);
    return 1;
}

int lua_SetFixedChargeConfig(lua_State *L)
{
    std::string strStartTime;
    std::string strEndTime;

    if (EParseLuaParam_Success != ParseLuaParam(L, strStartTime, strEndTime))
    {
        PARAM_ERROR;
        return 0;
    }

    PaymentInfoMgr::Instance().AddFixedChargeConfig((unsigned int)::SetTime(strStartTime.c_str()), (unsigned int)::SetTime(strEndTime.c_str()));

    return 1;
}
int lua_InitForbiddenRegPlatform(lua_State *L)
{
    std::string strForbidden = "";
    int nError = ParseLuaParam(L, strForbidden);

    if (nError != EParseLuaParam_Success)
    {
        PARAM_ERROR;
        return 0;
    }

    AccountConfigMgr::Instance().InitForbiddenRegPlatform(strForbidden);

    return 1;
}

int lua_InitPlayerCache( lua_State * L )
{
    unsigned int nMaxCount = 0, nValidTime = 0;
    int nError = ParseLuaParam( L, nMaxCount, nValidTime );

    if ( nError != EParseLuaParam_Success )
    {
        PARAM_ERROR;
        return 0;
    }

    if ( g_pCirculator != NULL )
        g_pCirculator->InitPlayerCacheManager( nMaxCount, nValidTime * 60 );

    return 1;
}

void CInitLua::RegLuaServerConfigFuction()
{
    REGFN(GetServerPath);
    REGFN(AddFirstRechargeInfo);
    REGFN(AddRechargeInfo);
    REGFN(AddFixedChargeInfo);
    REGFN(SetFixedChargeConfig);
    REGFN(InitForbiddenRegPlatform);
    REGFN( InitPlayerCache );
}


