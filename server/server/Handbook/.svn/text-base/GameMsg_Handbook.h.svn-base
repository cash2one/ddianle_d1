/************************************************************************
 * GameMsg_Handbook.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: luhaoting
 * Create time: 2015-9-21 10:22:20
 ***********************************************************************/
#ifndef __GAMEMSG_HANDBOOK_H__
#define __GAMEMSG_HANDBOOK_H__

#include "../../socket/GameMsg_Base.h"
#include "../share/ServerMsgDef.h"

#define MAX_UNIT_SIZE 50

enum eMSM_HANDBOOK
{
    MSG_HANDBOOK_BEGIN = MSG_TYPE_HANDBOOK,              //0x2600

    MSG_C2S_QueryHandBookCostumeInfo,                    //请求图鉴服饰全局数据
    Msg_S2C_QueryHandBookCostumeInfo,                    //返回图鉴服饰全局数据

    MSG_C2S_MyCollect,                //请求我的收藏
    MSG_S2C_MyCollect,                //返回我的收藏

    MSG_C2S_CostumePersonalInfo,                    //请求服饰个人数据
    MSG_S2C_CostumePersonalInfo,                    //返回服饰个人数据

    MSG_C2S_SetCostumeState,                   //请求设置服饰选项
    MSG_S2C_SetCostumeState,                   //返回设置服饰选项

    MSG_C2S_EffectPersonalInfo,                   //请求特效个人数据
    MSG_S2C_EffectPersonalInfo,                   //返回特效个人数据

    MSG_C2S_SetEffectState,                    //请求设置特效选项
    MSG_S2C_SetEffectState,                    //返回设置特效选项

    MSG_S2G_HandBookCostumeGolbalInfo,          // 向group申请全局服饰数据
    MSG_G2S_HandBookCostumeGolbalInfo,          // group返回全局服饰数据
    MSG_S2G_HandBookEffectGolbalInfo,          // 向group申请全局特效数据
    MSG_G2S_HandBookEffectGolbalInfo,          // group返回全局特效数据
    MSG_S2G_HandBookChangeGolbalInfo,           // 全局数据变更
};

class GolbalPerInfo
{
public:
    GolbalPerInfo(){};
    ~GolbalPerInfo(){};
public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);
public:
    unsigned int m_nId;
    unsigned int m_nPraisedCount;
    unsigned int m_nEnvyCount;
    unsigned int m_nCollectCount;
};

class GameMsg_S2G_GolbalCostumeData: public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_GolbalCostumeData);
    GameMsg_S2G_GolbalCostumeData();

    bool doEncode(CParamPool &IOBuff){return true;};
    bool doDecode(CParamPool &IOBuff){return true;};
};


class GameMsg_S2G_GolbalEffectData: public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_GolbalEffectData);
    GameMsg_S2G_GolbalEffectData();

    bool doEncode(CParamPool &IOBuff){return true;};
    bool doDecode(CParamPool &IOBuff){return true;};
};

class GameMsg_G2S_GolbalEffectData: public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_GolbalEffectData);
    GameMsg_G2S_GolbalEffectData();

    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

    std::list<GolbalPerInfo> m_listPerInfo;
    bool m_isEnd;
};

class GameMsg_G2S_GolbalCostumeData: public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_GolbalCostumeData);
    GameMsg_G2S_GolbalCostumeData();

    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

    std::list<GolbalPerInfo> m_listPerInfo;
    bool m_isEnd;
};

class GameMsg_S2G_ChangeGolbalData : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_ChangeGolbalData);
    GameMsg_S2G_ChangeGolbalData();

    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

    int m_nHandBookType;
    int m_nId;
    int m_nChangeIndex;
    bool m_bIsAdd;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_GolbalCostumeData : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GolbalCostumeData);
    GameMsg_C2S_GolbalCostumeData();

    bool doDecode(CParamPool &IOBuff){return true;};
};

class GameMsg_S2C_GolbalCostumeData : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_GolbalCostumeData);
    GameMsg_S2C_GolbalCostumeData();

    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

    std::list<GolbalPerInfo> m_listPerInfo;
    bool m_isEnd;
};
//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_CostumeCollectData : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_CostumeCollectData);
    GameMsg_C2S_CostumeCollectData();

    bool doDecode(CParamPool &IOBuff){return true;}
};

class GameMsg_S2C_CostumeCollectData : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_CostumeCollectData);
    GameMsg_S2C_CostumeCollectData();

    bool doEncode(CParamPool &IOBuff);

    std::list<unsigned int> m_listCollect;
};


//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_CostumePersonalData : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_CostumePersonalData);
    GameMsg_C2S_CostumePersonalData();

    bool doEncode(CParamPool &IOBuff)
    {
        return true;
    }

    bool doDecode(CParamPool &IOBuff)
    {
        m_nCostumeID = IOBuff.GetUInt();
        return true;
    };
    
    unsigned int m_nCostumeID;
};

class GameMsg_S2C_CostumePersonalData : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_CostumePersonalData);
    GameMsg_S2C_CostumePersonalData();

    bool doEncode(CParamPool &IOBuff);

    bool m_bIsPraised;
    bool m_bIsCollected;
    bool m_bIsEnvied;
};

class GameMsg_C2S_EffectPersonalData : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_EffectPersonalData);
    GameMsg_C2S_EffectPersonalData();

    bool doEncode(CParamPool &IOBuff)
    {
        return true;
    }

    bool doDecode(CParamPool &IOBuff)
    {
        m_nEffectID = IOBuff.GetUInt();
        return true;
    };

    unsigned int m_nEffectID;
};

class GameMsg_S2C_EffectPersonalData : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_EffectPersonalData);
    GameMsg_S2C_EffectPersonalData();

    bool doEncode(CParamPool &IOBuff);

    bool m_bIsPraised;
    bool m_bIsCollected;
    bool m_bIsEnvied;

    unsigned int m_nPraisedTotal;
    unsigned int m_nCollectedTotal;
    unsigned int m_nEnviedTotal;
};

//////////////////////////////////////////////////////////////////////////

enum
{
    CostumeStateChangeFail = 0,
    CostumeStateChangeSuc = 1,
    CostumeStateChangeMax = 2,
};

class GameMsg_C2S_SetCostumeState : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_SetCostumeState);
    GameMsg_C2S_SetCostumeState();

    bool doDecode(CParamPool &IOBuff);

    unsigned int m_nId;
    unsigned char m_byType;
    bool m_bFlag;
};

class GameMsg_S2C_SetCostumeState : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_SetCostumeState);
    GameMsg_S2C_SetCostumeState();

    bool doEncode(CParamPool &IOBuff);

    unsigned char m_byRetCode;
};


class GameMsg_C2S_SetEffectState : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_SetEffectState);
    GameMsg_C2S_SetEffectState();

    bool doDecode(CParamPool &IOBuff);

    unsigned int m_nId;
    unsigned char m_byType;
    bool m_bFlag;
};

class GameMsg_S2C_SetEffectState : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_SetEffectState);
    GameMsg_S2C_SetEffectState();

    bool doEncode(CParamPool &IOBuff);

    unsigned char m_byRetCode;
};

#endif
