#include "GameMsg_G2S_DanceGroup.h"
#include "DanceGroupMsgDef.h"
#include "../../socket/ParamPool.h"
#include "../../datastructure/DataStruct_DanceGroup.h"
#include "../../socket/inc/SocketLib.h"

// ----------------------------------------------- GameMsg_G2S_UpdateDanceGroupBaseInfo

GameMsg_G2S_UpdateDanceGroupBaseInfo::GameMsg_G2S_UpdateDanceGroupBaseInfo()
    : GameMsg_Base(MSG_G2S_UpdateDanceGroupBaseInfo)
    , m_nUpdateType(EDanceGroupBaseInfoUpdateType_Data)
{
}

bool GameMsg_G2S_UpdateDanceGroupBaseInfo::doDecode(CParamPool &IOBuff)
{
    m_DGBaseInfo.doDecode(IOBuff);
    m_nUpdateType = IOBuff.GetUShort();

    return true;
}

bool GameMsg_G2S_UpdateDanceGroupBaseInfo::doEncode(CParamPool &IOBuff)
{
    m_DGBaseInfo.doEncode(IOBuff);
    IOBuff.AddUShort(m_nUpdateType);

    return true;
}

// ----------------------------------------------- GameMsg_G2S_AddDanceGroupMemberInfo

GameMsg_G2S_AddDanceGroupMemberInfo::GameMsg_G2S_AddDanceGroupMemberInfo()
    : GameMsg_Base(MSG_G2S_AddDanceGroupMemberInfo)
    , m_nGroupID(0)
{
}

bool GameMsg_G2S_AddDanceGroupMemberInfo::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    m_MemberInfo.doDecode(IOBuff);

    return true;
}

bool GameMsg_G2S_AddDanceGroupMemberInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    m_MemberInfo.doEncode(IOBuff);

    return true;
}

// ----------------------------------------------- GameMsg_G2S_UpdateDanceGroupMemberInfo

GameMsg_G2S_UpdateDanceGroupMemberInfo::GameMsg_G2S_UpdateDanceGroupMemberInfo()
    : GameMsg_Base(MSG_G2S_UpdateDanceGroupMemberInfo)
    , m_nGroupID(0)
{
}

bool GameMsg_G2S_UpdateDanceGroupMemberInfo::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    m_MemberInfo.doDecode(IOBuff);

    return true;
}

bool GameMsg_G2S_UpdateDanceGroupMemberInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    m_MemberInfo.doEncode(IOBuff);

    return true;
}

// ----------------------------------------------- GameMsg_G2S_RemoveDanceGroupMemberInfo

GameMsg_G2S_RemoveDanceGroupMemberInfo::GameMsg_G2S_RemoveDanceGroupMemberInfo()
    : GameMsg_Base(MSG_G2S_RemoveDanceGroupMemberInfo)
    , m_nGroupID(0)
    , m_nRoleID(0)
{
}

bool GameMsg_G2S_RemoveDanceGroupMemberInfo::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    m_nRoleID = IOBuff.GetUInt();

    return true;
}

bool GameMsg_G2S_RemoveDanceGroupMemberInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    IOBuff.AddUInt(m_nRoleID);

    return true;
}

// ----------------------------------------------- GameMsg_G2S_AddRequestDanceGroupInfo

GameMsg_G2S_AddRequestDanceGroupInfo::GameMsg_G2S_AddRequestDanceGroupInfo()
    : GameMsg_Base(MSG_G2S_AddRequestDanceGroupInfo)
    , m_nGroupID(0)
{
}

bool GameMsg_G2S_AddRequestDanceGroupInfo::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    m_RequestInfo.doDecode(IOBuff);

    return true;
}

bool GameMsg_G2S_AddRequestDanceGroupInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    m_RequestInfo.doEncode(IOBuff);

    return true;
}

// ----------------------------------------------- GameMsg_G2S_RemoveRequestDanceGroupInfo

GameMsg_G2S_RemoveRequestDanceGroupInfo::GameMsg_G2S_RemoveRequestDanceGroupInfo()
    : GameMsg_Base(MSG_G2S_RemoveRequestDanceGroupInfo)
    , m_nGroupID(0)
    , m_nRoleID(0)
{
}

bool GameMsg_G2S_RemoveRequestDanceGroupInfo::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    m_nRoleID = IOBuff.GetUInt();

    return true;
}

bool GameMsg_G2S_RemoveRequestDanceGroupInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    IOBuff.AddUInt(m_nRoleID);

    return true;
}

// ----------------------------------------------- GameMsg_G2S_CreateDanceGroupResult

GameMsg_G2S_CreateDanceGroupResult::GameMsg_G2S_CreateDanceGroupResult()
    : GameMsg_Base(MSG_G2S_CreateDanceGroupResult)
    , m_nResult(ECreateDanceGroupResult_Success)
    , m_nCreateCD(0)
    , m_pDGInfo(NULL)
{
}

GameMsg_G2S_CreateDanceGroupResult::~GameMsg_G2S_CreateDanceGroupResult()
{
    delete m_pDGInfo;
    m_pDGInfo = NULL;
}

bool GameMsg_G2S_CreateDanceGroupResult::doDecode(CParamPool &IOBuff)
{
    m_nResult = IOBuff.GetUShort();
    m_nCreateCD = IOBuff.GetUInt();

    if (NULL == m_pDGInfo)
        m_pDGInfo = new CDanceGroupInfo;

    if (NULL != m_pDGInfo && ECreateDanceGroupResult_Success == m_nResult)
        m_pDGInfo->doDecode(IOBuff);

    return true;
}

bool GameMsg_G2S_CreateDanceGroupResult::doEncode(CParamPool &IOBuff)
{
    if (NULL == m_pDGInfo && ECreateDanceGroupResult_Success == m_nResult)
        m_nResult = ECreateDanceGroupResult_Error;

    IOBuff.AddUShort(m_nResult);
    IOBuff.AddUInt(m_nCreateCD);

    if (NULL != m_pDGInfo && ECreateDanceGroupResult_Success == m_nResult)
        m_pDGInfo->doEncode(IOBuff);

    return true;
}

// ----------------------------------------------- GameMsg_G2S_EnterDanceGroupSuccess

GameMsg_G2S_EnterDanceGroupSuccess::GameMsg_G2S_EnterDanceGroupSuccess()
    : GameMsg_Base(MSG_G2S_EnterDanceGroupSuccess)
    , m_pDGInfo(NULL)
{
}

GameMsg_G2S_EnterDanceGroupSuccess::~GameMsg_G2S_EnterDanceGroupSuccess()
{
    delete m_pDGInfo;
    m_pDGInfo = NULL;
}

bool GameMsg_G2S_EnterDanceGroupSuccess::doDecode(CParamPool &IOBuff)
{
    if (NULL == m_pDGInfo)
        m_pDGInfo = new CDanceGroupInfo;

    if (NULL != m_pDGInfo)
        m_pDGInfo->doDecode(IOBuff);

    return true;
}

bool GameMsg_G2S_EnterDanceGroupSuccess::doEncode(CParamPool &IOBuff)
{
    if (NULL != m_pDGInfo)
        m_pDGInfo->doEncode(IOBuff);

    return true;
}

// ----------------------------------------------- GameMsg_G2S_DonateDanceGroupResourceResult

GameMsg_G2S_DonateDanceGroupResourceResult::GameMsg_G2S_DonateDanceGroupResourceResult()
    : GameMsg_Base(MSG_G2S_DonateDanceGroupResourceResult)
    , m_nResult(EDonateDanceGroupResourceResult_Success)
    , m_nResA(0)
    , m_nResB(0)
    , m_nResC(0)
{
}

bool GameMsg_G2S_DonateDanceGroupResourceResult::doDecode(CParamPool &IOBuff)
{
    m_nResult = IOBuff.GetUShort();
    m_nResA = IOBuff.GetUInt();
    m_nResB = IOBuff.GetUInt();
    m_nResC = IOBuff.GetUInt();

    return true;
}

bool GameMsg_G2S_DonateDanceGroupResourceResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nResult);
    IOBuff.AddUInt(m_nResA);
    IOBuff.AddUInt(m_nResB);
    IOBuff.AddUInt(m_nResC);

    return true;
}

// ----------------------------------------------- GameMsg_G2S_UpdateDanceGroupActivityStarState

GameMsg_G2S_UpdateDanceGroupActivityStarState::GameMsg_G2S_UpdateDanceGroupActivityStarState()
    : GameMsg_Base(MSG_G2S_UpdateDanceGroupActivityStarState)
    , m_nGroupID(0)
    , m_nStarLevel(1)
    , m_nCurrentPower(0)
{
}

bool GameMsg_G2S_UpdateDanceGroupActivityStarState::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    m_nStarLevel = IOBuff.GetUShort();
    m_nCurrentPower = IOBuff.GetUInt();

    return true;
}

bool GameMsg_G2S_UpdateDanceGroupActivityStarState::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    IOBuff.AddUShort(m_nStarLevel);
    IOBuff.AddUInt(m_nCurrentPower);

    return true;
}

// ----------------------------------------------- GameMsg_G2S_UpdateDanceGroupActivityInfo
GameMsg_G2S_UpdateDanceGroupActivityInfo::GameMsg_G2S_UpdateDanceGroupActivityInfo()
:GameMsg_Base(MSG_G2S_UpdateDanceGroupActivityInfo)
{

}//end

bool GameMsg_G2S_UpdateDanceGroupActivityInfo::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nGroupID);
	m_nActivityInfo.doEncode(IOBuff);

	return true;
}//end

bool GameMsg_G2S_UpdateDanceGroupActivityInfo::doDecode(CParamPool &IOBuff)
{
	m_nGroupID = IOBuff.GetUInt();
	m_nActivityInfo.doDecode(IOBuff);
	return true;
}//end

// ----------------------------------------------- GameMsg_G2S_DanceGroupActivityReset
GameMsg_G2S_DanceGroupActivityReset::GameMsg_G2S_DanceGroupActivityReset()
:GameMsg_Base(MSG_G2S_DanceGroupActivityReset)
,m_nGroupID(0)
,m_nRoleID(0)
{

}//end

bool GameMsg_G2S_DanceGroupActivityReset::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nGroupID);
	IOBuff.AddUInt(m_nRoleID);

	return true;
}//end

bool GameMsg_G2S_DanceGroupActivityReset::doDecode(CParamPool &IOBuff)
{
	m_nGroupID = IOBuff.GetUInt();
	m_nRoleID = IOBuff.GetUInt();

	return true;
}//end

// ----------------------------------------------- GameMsg_G2S_DanceGroupAddRecordResult
GameMsg_G2S_DanceGroupAddRecordResult::GameMsg_G2S_DanceGroupAddRecordResult()
:GameMsg_Base(MSG_G2S_DanceGroupAddRecordResult)
{

}

bool GameMsg_G2S_DanceGroupAddRecordResult::doEncode(CParamPool &IOBuff)
{
	m_recordInfo.doEncode(IOBuff);

	return true;
}//end

bool GameMsg_G2S_DanceGroupAddRecordResult::doDecode(CParamPool &IOBuff)
{
	m_recordInfo.doDecode(IOBuff);

	return true;
}//end

//////////////////////////////////////////////////////////////////////////

GameMsg_G2S_DanceGroupShopGetInfoResult::GameMsg_G2S_DanceGroupShopGetInfoResult()
    : GameMsg_Base(MSG_G2S_DanceGroupShopGetInfoResult)
    , m_nErrorCode(EDanceGroupShopResult_Success)
{
}

bool GameMsg_G2S_DanceGroupShopGetInfoResult::doDecode(CParamPool &IOBuff)
{
    m_nErrorCode = IOBuff.GetUShort();

    unsigned short count = IOBuff.GetUShort();
    unsigned short first = 0;
    unsigned short second = 0;

    for (unsigned short i = 0; i < count; ++i)
    {
        first = IOBuff.GetUShort();
        second = IOBuff.GetUShort();

        m_mapUseCount.insert(std::make_pair(first, second));
    }

    return true;
}

bool GameMsg_G2S_DanceGroupShopGetInfoResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nErrorCode);

    IOBuff.AddUShort((unsigned short)m_mapUseCount.size());
    for (std::map<unsigned short, unsigned short>::const_iterator bIt = m_mapUseCount.begin(), 
         eIt = m_mapUseCount.end(); bIt != eIt; ++bIt)
    {
        IOBuff.AddUShort(bIt->first);
        IOBuff.AddUShort(bIt->second);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_G2S_GetMyDanceGroupInfo::GameMsg_G2S_GetMyDanceGroupInfo()
    : GameMsg_Base(MSG_G2S_GetMyDanceGroupInfo)
    , m_nClickedPlace(0)
    , m_pDGInfo(NULL)
{
}

GameMsg_G2S_GetMyDanceGroupInfo::~GameMsg_G2S_GetMyDanceGroupInfo()
{
    delete m_pDGInfo;
    m_pDGInfo = NULL;
}

bool GameMsg_G2S_GetMyDanceGroupInfo::doDecode(CParamPool &IOBuff)
{
    m_nClickedPlace = IOBuff.GetUShort();
    bool bHasInfo = IOBuff.GetBool();

    if (bHasInfo)
    {
        if (NULL == m_pDGInfo)
            m_pDGInfo = new CDanceGroupInfo;

        if (NULL != m_pDGInfo)
            m_pDGInfo->doDecode(IOBuff);
    }

    return true;
}

bool GameMsg_G2S_GetMyDanceGroupInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nClickedPlace);
    IOBuff.AddBool(NULL != m_pDGInfo);
    if (NULL != m_pDGInfo)
        m_pDGInfo->doEncode(IOBuff);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_G2S_AcceptDanceGroupMemberInvitationResult::GameMsg_G2S_AcceptDanceGroupMemberInvitationResult()
    : GameMsg_Base(MSG_G2S_AcceptDanceGroupMemberInvitationResult)
    , m_nResult(EAcceptDanceGroupMemberInvitationResult_Success)
{
}

bool GameMsg_G2S_AcceptDanceGroupMemberInvitationResult::doDecode(CParamPool &IOBuff)
{
    m_nResult = IOBuff.GetUShort();

    return true;
}

bool GameMsg_G2S_AcceptDanceGroupMemberInvitationResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nResult);

    return true;
}

GameMsg_G2S_BroadcastMemberKicked::GameMsg_G2S_BroadcastMemberKicked()
    : GameMsg_Base(MSG_G2S_BroadcastMemberKicked)
    , m_nKickedRoleID(0)
    , m_nGroupID(0)
{

}

GameMsg_G2S_BroadcastMemberKicked::~GameMsg_G2S_BroadcastMemberKicked()
{

}

bool GameMsg_G2S_BroadcastMemberKicked::doDecode(CParamPool &IOBuff)
{
    m_nKickedRoleID = IOBuff.GetUInt();
    m_nGroupID = IOBuff.GetUInt();

    return true;
}

bool GameMsg_G2S_BroadcastMemberKicked::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nKickedRoleID);
    IOBuff.AddUInt(m_nGroupID);

    return true;
}

GameMsg_G2S_BroadcastMemberQuit::GameMsg_G2S_BroadcastMemberQuit()
    : GameMsg_Base(MSG_G2S_BroadcastMemberQuit)
    , m_nQuitRoleID(0)
    , m_nGroupID(0)
{

}
GameMsg_G2S_BroadcastMemberQuit::~GameMsg_G2S_BroadcastMemberQuit()
{

}

bool GameMsg_G2S_BroadcastMemberQuit::doDecode(CParamPool &IOBuff)
{
    m_nQuitRoleID = IOBuff.GetUInt();
    m_nGroupID = IOBuff.GetUInt();

    return true;
}

bool GameMsg_G2S_BroadcastMemberQuit::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nQuitRoleID);
    IOBuff.AddUInt(m_nGroupID);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_G2S_UpdateDanceGroupChallengeActive::GameMsg_G2S_UpdateDanceGroupChallengeActive()
    : GameMsg_Base( MSG_G2S_UpdateDanceGroupChallengeActive )
    , m_nGroupID( 0 )
    , m_nActive( 0 )
{
}

GameMsg_G2S_UpdateDanceGroupChallengeActive::~GameMsg_G2S_UpdateDanceGroupChallengeActive()
{
}

bool GameMsg_G2S_UpdateDanceGroupChallengeActive::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nGroupID << m_nActive;

    return true;
}

bool GameMsg_G2S_UpdateDanceGroupChallengeActive::doDecode( CParamPool& IOBuff )
{
    IOBuff >> m_nGroupID >> m_nActive;

    return true;
}


