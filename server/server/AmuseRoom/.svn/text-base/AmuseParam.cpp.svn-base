#include "AmuseParam.h"
#include "AmuseEnum.h"
#include "../../datastructure/Macro_Define.h"
#include "../../socket/ParamPool.h"


///////////////////////////////////////////////////////
CAmuseActionParam_Base::CAmuseActionParam_Base(int nActionType)
:m_nActionType((char)nActionType)
{
}

CAmuseActionParam_Base::~CAmuseActionParam_Base()
{
}


///////////////////////////////////////////////////////
CAmuseActionParam_UseDevice::CAmuseActionParam_UseDevice()
:CAmuseActionParam_Base(eAmuseAction_UseDevice)
,m_nPlayerID(0)
,m_nDeviceID(0)
,m_nPartnerType(eAmusePartner_None)
,m_nStartTime(0)
,m_bInviter(false)
{
}

CAmuseActionParam_UseDevice::~CAmuseActionParam_UseDevice()
{
}

bool CAmuseActionParam_UseDevice::doDecode(CParamPool &IOBuff)
{
	m_nDeviceID = IOBuff.GetUChar();

	return true;
}

bool CAmuseActionParam_UseDevice::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar( m_nActionType );
	IOBuff.AddUInt( m_nPlayerID );
	IOBuff.AddUChar( m_nDeviceID );
	IOBuff.AddUChar( m_nPartnerType );
	IOBuff.AddInt( m_nStartTime );
	IOBuff.AddBool( m_bInviter );
	::doEncode( IOBuff, m_listCost );

	return true;
}


///////////////////////////////////////////////////////
CAmuseActionParam_LeaveDevice::CAmuseActionParam_LeaveDevice()
:CAmuseActionParam_Base(eAmuseAction_LeaveDevice)
,m_nPlayerID(0)
{
}

CAmuseActionParam_LeaveDevice::~CAmuseActionParam_LeaveDevice()
{
}

bool CAmuseActionParam_LeaveDevice::doDecode(CParamPool &IOBuff)
{
	return true;
}

bool CAmuseActionParam_LeaveDevice::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar( m_nActionType );
	IOBuff.AddUInt( m_nPlayerID );

	return true;
}


///////////////////////////////////////////////////////
CAmuseActionParam_InviteDevice::CAmuseActionParam_InviteDevice()
:CAmuseActionParam_Base(eAmuseAction_InviteDevice)
,m_nInviteeID(0)
{
}

CAmuseActionParam_InviteDevice::~CAmuseActionParam_InviteDevice()
{
}

bool CAmuseActionParam_InviteDevice::doDecode(CParamPool &IOBuff)
{
	m_nInviteeID = IOBuff.GetUInt();

	return true;
}

bool CAmuseActionParam_InviteDevice::doEncode(CParamPool &IOBuff)
{
	return true;
}


///////////////////////////////////////////////////////
CAmuseActionParam_RequestDevice::CAmuseActionParam_RequestDevice()
:CAmuseActionParam_Base(eAmuseAction_RequestDevice)
,m_nPlayerID(0)
,m_chPlayerSex(ESexType_No)
,m_bIsVIP(false)
,m_nVIPLevel(0)
,m_nDeviceID(0)
{
}

CAmuseActionParam_RequestDevice::~CAmuseActionParam_RequestDevice()
{
}

bool CAmuseActionParam_RequestDevice::doDecode(CParamPool &IOBuff)
{
	return true;
}

bool CAmuseActionParam_RequestDevice::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar( m_nActionType );
	IOBuff.AddUInt( m_nPlayerID );
	IOBuff.AddStringW( m_strPlayerName.c_str() );
	IOBuff.AddUChar( m_chPlayerSex );
	IOBuff.AddBool( m_bIsVIP );
	IOBuff.AddUShort( m_nVIPLevel );
	IOBuff.AddUChar( m_nDeviceID );
	IOBuff.AddStringW( m_strRequestTips.c_str() );

	return true;
}


///////////////////////////////////////////////////////
CAmuseActionParam_ResponseDevice::CAmuseActionParam_ResponseDevice()
:CAmuseActionParam_Base(eAmuseAction_ResponseDevice)
{
}

CAmuseActionParam_ResponseDevice::~CAmuseActionParam_ResponseDevice()
{
}

bool CAmuseActionParam_ResponseDevice::doDecode(CParamPool &IOBuff)
{
	m_nInviterID = IOBuff.GetUInt();
	m_nDeviceID = IOBuff.GetUChar();
	m_bAccept = IOBuff.GetBool();

	return true;
}

bool CAmuseActionParam_ResponseDevice::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar( m_nActionType );
	IOBuff.AddBool( m_bAccept );
	return true;
}


///////////////////////////////////////////////////////
CAmuseActionParam_UsePose::CAmuseActionParam_UsePose()
:CAmuseActionParam_Base(eAmuseAction_UsePose)
,m_nPlayerID(0)
,m_nPoseID(0)
{
}

CAmuseActionParam_UsePose::~CAmuseActionParam_UsePose()
{
}

bool CAmuseActionParam_UsePose::doDecode(CParamPool &IOBuff)
{
	m_nPoseID = IOBuff.GetUChar();

	return true;
}

bool CAmuseActionParam_UsePose::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar( m_nActionType );
	IOBuff.AddUInt( m_nPlayerID );
	IOBuff.AddUChar( m_nPoseID );

	return true;
}


///////////////////////////////////////////////////////
CAmuseActionParam_LeavePose::CAmuseActionParam_LeavePose()
:CAmuseActionParam_Base(eAmuseAction_LeavePose)
,m_nPlayerID(0)
{
}

CAmuseActionParam_LeavePose::~CAmuseActionParam_LeavePose()
{
}

bool CAmuseActionParam_LeavePose::doDecode(CParamPool &IOBuff)
{
	return true;
}

bool CAmuseActionParam_LeavePose::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar( m_nActionType );
	IOBuff.AddUInt( m_nPlayerID );

	return true;
}


CAmuseActionParam_GetTimeEggGiftList::CAmuseActionParam_GetTimeEggGiftList()
:CAmuseActionParam_Base(eAmuseAction_GetTimeEggGiftList)
{
	m_nGiftDiscount = 100;
}

CAmuseActionParam_GetTimeEggGiftList::~CAmuseActionParam_GetTimeEggGiftList()
{

}

bool CAmuseActionParam_GetTimeEggGiftList::doDecode( CParamPool &IOBuff )
{
	return true;
}

bool CAmuseActionParam_GetTimeEggGiftList::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUChar(m_nActionType);
	::doEncode(IOBuff, m_listGift);
	::doEncode(IOBuff, m_listDesire);
	IOBuff.AddUInt(m_nGiftDiscount);
	return true;
}

CAmuseActionParam_CommitTimeEggInfo::CAmuseActionParam_CommitTimeEggInfo()
:CAmuseActionParam_Base(eAmuseAction_CommitTimeEggInfo)
{
	m_nPlayerID = 0;
}

CAmuseActionParam_CommitTimeEggInfo::~CAmuseActionParam_CommitTimeEggInfo()
{

}

bool CAmuseActionParam_CommitTimeEggInfo::doDecode( CParamPool &IOBuff )
{
	IOBuff.GetStringW(m_strWords);
	unsigned short size = IOBuff.GetUShort();
	for (unsigned short i = 0; i < size; ++i)
	{
		PeddleGood aPeddleGood;
		aPeddleGood.doDecode(IOBuff);
		aPeddleGood.m_bUseCoupon = false;
		aPeddleGood.m_bIsAddUse = false;
		m_listGift.push_back(aPeddleGood);
	}
	return true;
}

bool CAmuseActionParam_CommitTimeEggInfo::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUChar(m_nActionType);
	IOBuff.AddUInt(m_nPlayerID);
	return true;
}

CAmuseActionParam_CreateTimeEgg::CAmuseActionParam_CreateTimeEgg()
:CAmuseActionParam_Base(eAmuseAction_CreateTimeEgg)
{

}

CAmuseActionParam_CreateTimeEgg::~CAmuseActionParam_CreateTimeEgg()
{

}

bool CAmuseActionParam_CreateTimeEgg::doDecode( CParamPool &IOBuff )
{
	return true;
}

bool CAmuseActionParam_CreateTimeEgg::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUChar( m_nActionType );
	return true;
}

CAmuseActionParam_OpenTimeEgg::CAmuseActionParam_OpenTimeEgg()
:CAmuseActionParam_Base(eAmuseAction_OpenTimeEgg)
{
	m_chOpenType = 0;
	m_nSelfID = 0;
	m_nPartnerID = 0;
}

CAmuseActionParam_OpenTimeEgg::~CAmuseActionParam_OpenTimeEgg()
{

}

bool CAmuseActionParam_OpenTimeEgg::doDecode( CParamPool &IOBuff )
{
	m_chOpenType = IOBuff.GetUChar();
	return true;
}

bool CAmuseActionParam_OpenTimeEgg::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUChar( m_nActionType );
	IOBuff.AddUChar(m_chOpenType);
	IOBuff.AddUInt(m_nSelfID);
	IOBuff.AddUInt(m_nPartnerID);
	IOBuff.AddStringW(m_strPartnerName);
	IOBuff.AddStringW(m_strPartnerWords);
	::doEncode(IOBuff, m_listFixedReward);
	::doEncode(IOBuff, m_listExtraReward);
	::doEncode(IOBuff, m_listGiftReward);
	IOBuff.AddUInt(m_nFixedMoneyReward);
	IOBuff.AddUInt(m_nExtraMoneyReward);
	return true;
}

///////////////////////////////////////////////////////
CAmuseEventParam_Base::CAmuseEventParam_Base(int nEventType)
:m_nEventType((char)nEventType)
{
}

CAmuseEventParam_Base::~CAmuseEventParam_Base()
{
}


///////////////////////////////////////////////////////
CAmuseEventParam_StartCarrousel::CAmuseEventParam_StartCarrousel()
:CAmuseEventParam_Base(eAmuseEvent_StartCarrousel)
{
}

CAmuseEventParam_StartCarrousel::~CAmuseEventParam_StartCarrousel()
{
}

bool CAmuseEventParam_StartCarrousel::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar( m_nEventType );
	IOBuff.AddInt( m_nStartTime );
	IOBuff.AddStringW( m_strStartSong.c_str() );

	return true;
}


///////////////////////////////////////////////////////
CAmuseEventParam_AddExp::CAmuseEventParam_AddExp()
:CAmuseEventParam_Base(eAmuseEvent_AddExp)
{
}

CAmuseEventParam_AddExp::~CAmuseEventParam_AddExp()
{
}

bool CAmuseEventParam_AddExp::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar( m_nEventType );
	IOBuff.AddUInt( m_nPlayerID );
	IOBuff.AddInt( m_nAddExp );

	return true;
}


///////////////////////////////////////////////////////
CAmuseEventParam_StartSwing::CAmuseEventParam_StartSwing()
:CAmuseEventParam_Base(eAmuseEvent_StartSwing)
{
}

CAmuseEventParam_StartSwing::~CAmuseEventParam_StartSwing()
{
}

bool CAmuseEventParam_StartSwing::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar( m_nEventType );
	IOBuff.AddUChar( m_nStartPower );
	IOBuff.AddInt( m_nStartTime );

	return true;
}


///////////////////////////////////////////////////////
CAmuseEventParam_StartBoat::CAmuseEventParam_StartBoat()
:CAmuseEventParam_Base(eAmuseEvent_StartBoat)
{
}

CAmuseEventParam_StartBoat::~CAmuseEventParam_StartBoat()
{
}

bool CAmuseEventParam_StartBoat::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar( m_nEventType );
	IOBuff.AddUChar( m_nDesDock );
	IOBuff.AddInt( m_nStartTime );

	return true;
}


///////////////////////////////////////////////////////
CAmuseActionParam_Base* CAmuseActionParamFactory::BuildActionParam(int nActionType)
{
	CAmuseActionParam_Base* pActionParam = NULL;

	switch ( nActionType )
	{
	case eAmuseAction_UseDevice:
		{
			pActionParam = new CAmuseActionParam_UseDevice();
		}
		break;
	case eAmuseAction_LeaveDevice:
		{
			pActionParam = new CAmuseActionParam_LeaveDevice();
		}
		break;
	case eAmuseAction_InviteDevice:
		{
			pActionParam = new CAmuseActionParam_InviteDevice();
		}
		break;
	case eAmuseAction_ResponseDevice:
		{
			pActionParam = new CAmuseActionParam_ResponseDevice();
		}
		break;
	case eAmuseAction_UsePose:
		{
			pActionParam = new CAmuseActionParam_UsePose();
		}
		break;
	case eAmuseAction_LeavePose:
		{
			pActionParam = new CAmuseActionParam_LeavePose();
		}
		break;
	case eAmuseAction_GetTimeEggGiftList:
		{
			pActionParam = new CAmuseActionParam_GetTimeEggGiftList();
		}
		break;
	case eAmuseAction_CommitTimeEggInfo:
		{
			pActionParam = new CAmuseActionParam_CommitTimeEggInfo();
		}
		break;
	case eAmuseAction_CreateTimeEgg:
		{
			pActionParam = new CAmuseActionParam_CreateTimeEgg();
		}
		break;
	case eAmuseAction_OpenTimeEgg:
		{
			pActionParam = new CAmuseActionParam_OpenTimeEgg();
		}
		break;
	}

	return pActionParam;
}


PlayMakerInfo::PlayMakerInfo()
	: m_nFsmID(0)
{

}
PlayMakerInfo::PlayMakerInfo(const PlayMakerInfo &rhs)
{
	m_nFsmID = rhs.m_nFsmID;
	m_strFsmEventName = rhs.m_strFsmEventName;
	m_paramMap = rhs.m_paramMap;
}

PlayMakerInfo::~PlayMakerInfo()
{

}

void PlayMakerInfo::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddInt(m_nFsmID);
	IOBuff.AddStringW(m_strFsmEventName);

	IOBuff.AddUShort((unsigned short)m_paramMap.size());
	for (std::map<std::string, std::string>::iterator it = m_paramMap.begin();
		it != m_paramMap.end(); ++it)
	{
		IOBuff.AddStringW(it->first);
		IOBuff.AddStringW(it->second);
	}
}
void PlayMakerInfo::doDecode(CParamPool & IOBuff)
{
	m_nFsmID = IOBuff.GetInt();
	IOBuff.GetStringW(m_strFsmEventName);

	unsigned short nParamCount = IOBuff.GetUShort();
	for (unsigned short i = 0; i < nParamCount; i++)
	{
		std::string strParamName, strParamValue;
		IOBuff.GetStringW(strParamName);
		IOBuff.GetStringW(strParamValue);
		m_paramMap.insert(std::make_pair(strParamName, strParamValue));
	}
}

