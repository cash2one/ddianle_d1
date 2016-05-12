#include "DataStruct_BigMama.h"
#include "../../socket/ParamPool.h"

DanceGroupBigMamaInfo::DanceGroupBigMamaInfo()
    : m_nDanceGroupID(0)
    , m_nStamp(0)
    , m_nCurrentRound(0)
    , m_nCurrentStageID(0)
    , m_nState(EBigMamaActiveState_Running)
    , m_nBeaterType(EBigMamaLastBeatType_GroupMember)
    , m_nBeaterRoleID(0)
{

}
DanceGroupBigMamaInfo::~DanceGroupBigMamaInfo()
{

}


BigMamaProperty::BigMamaProperty()
    : m_bBigMamaAllLose(false)
    , m_nCurRound(0)
    , m_nCurMode(0)
    , m_nTargetScore(0)
    , m_nBeaterType(EBigMamaLastBeatType_GroupMember)
{

}

BigMamaProperty::~BigMamaProperty()
{
}
void BigMamaProperty::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddBool(m_bBigMamaAllLose);
    IOBuff.AddUShort(m_nCurRound);
    IOBuff.AddUChar(m_nCurMode);
    IOBuff.AddUInt(m_nTargetScore);
    IOBuff.AddUChar((unsigned char)m_nBeaterType);
    IOBuff.AddStringW(m_strRoleName);

    return;
}
void BigMamaProperty::doDecode(CParamPool &IOBuff)
{
    m_bBigMamaAllLose = IOBuff.GetBool();
    m_nCurRound = IOBuff.GetUShort();
    m_nCurMode = IOBuff.GetUChar();
    m_nTargetScore = IOBuff.GetUInt();
    m_nBeaterType = (EBigMamaLastBeatType)IOBuff.GetUChar();
    IOBuff.GetStringW(m_strRoleName);

    return;
}
