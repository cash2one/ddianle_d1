#include "GameMsg_Gm_medal.h"


GameMsg_G2S_GMDeleteVersionMedal::GameMsg_G2S_GMDeleteVersionMedal()
    :GameMsg_Base(MSG_GM_G2S_DeleteVersionMedal)
{

}

GameMsg_S2G_GMDeleteVersionMedal::GameMsg_S2G_GMDeleteVersionMedal()
    :GameMsg_Base(MSG_GM_S2G_DeleteVersionMedal)
{

}

GameMsg_M2G_GMDeleteVersionMedal::GameMsg_M2G_GMDeleteVersionMedal()
    :GameMsg_Base(MSG_GM_M2G_DeleteVersionMedal)
{

}

GameMsg_G2M_GMDeleteVersionMedal::GameMsg_G2M_GMDeleteVersionMedal()
    :GameMsg_Base(MSG_GM_G2M_DeleteVersionMedal)
{

}

GameMsg_M2G_GMAddMedal::GameMsg_M2G_GMAddMedal()
    :GameMsg_Base(MSG_GM_M2G_AddMedalByGm)
{

}

bool GameMsg_M2G_GMAddMedal::doDecode(CParamPool &IOBuff)
{
    m_nPlayerID = IOBuff.GetInt();
    m_nMedalID = IOBuff.GetInt();
    return true;
}


bool GameMsg_M2G_GMAddMedal::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nPlayerID);
    IOBuff.AddInt(m_nMedalID);
    return true;
}


GameMsg_G2M_GMAddMedal::GameMsg_G2M_GMAddMedal()
    :GameMsg_Base(MSG_GM_G2M_AddMedalByGm)
{

}

bool GameMsg_G2M_GMAddMedal::doDecode(CParamPool &IOBuff)
{
    m_nRetCode = IOBuff.GetInt();
    return true;
}

bool GameMsg_G2M_GMAddMedal::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nRetCode);
    return true;
}

