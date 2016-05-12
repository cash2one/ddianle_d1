#ifndef  __GAMEMSG_GM_MEDAL_H__ 
#define  __GAMEMSG_GM_MEDAL_H__

#include "../share/ServerMsgDef.h"
#include "../../datastructure/DataStruct_Base.h"
#include "../../socket/GameMsg_Base.h"

class GameMsg_G2S_GMDeleteVersionMedal : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_GMDeleteVersionMedal);
    GameMsg_G2S_GMDeleteVersionMedal();
    ~GameMsg_G2S_GMDeleteVersionMedal() {}

    bool doDecode(CParamPool &IOBuff){return true;}
    bool doEncode(CParamPool &IOBuff){return true;}
};

class GameMsg_S2G_GMDeleteVersionMedal : public GameMsg_Base
    {
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_GMDeleteVersionMedal);
    GameMsg_S2G_GMDeleteVersionMedal();
    ~GameMsg_S2G_GMDeleteVersionMedal() {}

    bool doDecode(CParamPool &IOBuff)
    {
        m_nRetCode = IOBuff.GetInt();
        return true;
    }
    bool doEncode(CParamPool &IOBuff)
    {
        IOBuff.AddInt(m_nRetCode);
        return true;
    }

    int m_nRetCode;
};

class GameMsg_M2G_GMDeleteVersionMedal : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_M2G_GMDeleteVersionMedal);
    GameMsg_M2G_GMDeleteVersionMedal();
    ~GameMsg_M2G_GMDeleteVersionMedal(){}

    bool doDecode(CParamPool &IOBuff){return true;}
    bool doEncode(CParamPool &IOBuff){return true;}
};

class GameMsg_G2M_GMDeleteVersionMedal : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2M_GMDeleteVersionMedal);
    GameMsg_G2M_GMDeleteVersionMedal();
    ~GameMsg_G2M_GMDeleteVersionMedal() {}

    bool doDecode(CParamPool &IOBuff)
    {
        m_nRetCode = IOBuff.GetInt();
        return true;
    }
    bool doEncode(CParamPool &IOBuff)
    {
        IOBuff.AddInt(m_nRetCode);
        return true;
    }

    int m_nRetCode;
};
//////////////////////////////////////////////////////////////////////////
class GameMsg_M2G_GMAddMedal: public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_M2G_GMAddMedal);
    GameMsg_M2G_GMAddMedal();
    ~GameMsg_M2G_GMAddMedal(){}

    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

    int m_nPlayerID;
    int m_nMedalID;
};

class GameMsg_G2M_GMAddMedal: public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2M_GMAddMedal);
    GameMsg_G2M_GMAddMedal();
    ~GameMsg_G2M_GMAddMedal(){}

    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

    int m_nRetCode; 
};

#endif
