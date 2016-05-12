#include "GameMsg_S2G_UpdateAttrInfo.h"
#include "../share/ServerMsgDef.h"

GameMsg_S2G_UpdateAttrInfo::GameMsg_S2G_UpdateAttrInfo():
GameMsg_Base(MSG_S2G_UpdateAttrInfo)
{
}

GameMsg_S2G_UpdateAttrInfo::GameMsg_S2G_UpdateAttrInfo(CWorldRoleUpdateAttrInfo &info):
GameMsg_Base(MSG_S2G_UpdateAttrInfo)
{
	m_RoleUpdateAttrInfo = info;
}

GameMsg_S2G_UpdateAttrInfo::~GameMsg_S2G_UpdateAttrInfo()
{
}

bool GameMsg_S2G_UpdateAttrInfo::doEncode(CParamPool &IOBuff)
{
	m_RoleUpdateAttrInfo.doEncode(IOBuff);
	return true;
}

bool GameMsg_S2G_UpdateAttrInfo::doDecode(CParamPool &IOBuff)
{
	m_RoleUpdateAttrInfo.doDecode(IOBuff);
	return true;
}

GameMsg_S2G_UpdateRoleName::GameMsg_S2G_UpdateRoleName(unsigned int nRoleID, const char* pRoleNema)
:GameMsg_Base(MSG_S2G_UpdateRoleName)
,m_nRoleID(nRoleID)
{
	memset(m_szName, 0, MAX_NAME_LEN);
	if ( pRoleNema != NULL )
	{
		size_t nSize = strlen(pRoleNema);
		if (nSize > MAX_NAME_LEN - 1) {
			nSize = MAX_NAME_LEN - 1;
		}
		memcpy(m_szName, pRoleNema, nSize);
	}
}

GameMsg_S2G_UpdateRoleName::~GameMsg_S2G_UpdateRoleName()
{
}

bool GameMsg_S2G_UpdateRoleName::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddString(m_szName);
	return true;
}

bool GameMsg_S2G_UpdateRoleName::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();
	string strTemp;
	strTemp = IOBuff.GetString();
	safememcpy(m_szName, MAX_NAME_LEN, strTemp.c_str(), (unsigned int)strTemp.size());

	return true;
}

GameMsg_S2G_UpdateLevel::GameMsg_S2G_UpdateLevel(unsigned int nRoleID, unsigned short nLevel)
:GameMsg_Base(MSG_S2G_UpdateRoleLevel)
,m_nRoleID(nRoleID)
,m_nLevel(nLevel)
{
}

GameMsg_S2G_UpdateLevel::~GameMsg_S2G_UpdateLevel()
{
}

bool GameMsg_S2G_UpdateLevel::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddUShort(m_nLevel);

	return true;
}

bool GameMsg_S2G_UpdateLevel::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();
	m_nLevel = IOBuff.GetUShort();

	return true;
}

GameMsg_S2G_UpdateMoney::GameMsg_S2G_UpdateMoney(unsigned int nRoleID, unsigned int nMoney)
:GameMsg_Base(MSG_S2G_UpdateRoleMoney)
,m_nRoleID(nRoleID)
,m_nMoney(nMoney)
{
}

GameMsg_S2G_UpdateMoney::~GameMsg_S2G_UpdateMoney()
{
}

bool GameMsg_S2G_UpdateMoney::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddInt(m_nMoney);

	return true;
}

bool GameMsg_S2G_UpdateMoney::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();
	m_nMoney = IOBuff.GetInt();

	return true;
}



GameMsg_S2G_UpdateRoleLocation::GameMsg_S2G_UpdateRoleLocation(): GameMsg_Base(MSG_S2G_UpdateRoleLocation)
{
	m_nRoleID = 0;
	m_nRoleLocation = 0;
}

GameMsg_S2G_UpdateRoleLocation::~GameMsg_S2G_UpdateRoleLocation()
{
}

bool GameMsg_S2G_UpdateRoleLocation::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddUShort(m_nRoleLocation);
	return true;
}

bool GameMsg_S2G_UpdateRoleLocation::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();
	m_nRoleLocation = IOBuff.GetUShort();
	return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2G_UpdateRoleAge::GameMsg_S2G_UpdateRoleAge(unsigned int nRoleID /* = 0 */, short nAge /* = 0 */, short nConstellation /* = 0 */)
    : GameMsg_Base(MSG_S2G_UpdateRoleAge)
    , m_nRoleID(nRoleID)
    , m_nAge(nAge)
    , m_nConstellation(nConstellation)
{
}

GameMsg_S2G_UpdateRoleAge::~GameMsg_S2G_UpdateRoleAge()
{
}

bool GameMsg_S2G_UpdateRoleAge::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddShort(m_nAge);
    IOBuff.AddShort(m_nConstellation);

    return true;
}

bool GameMsg_S2G_UpdateRoleAge::doDecode(CParamPool &IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    m_nAge = IOBuff.GetShort();
    m_nConstellation = IOBuff.GetShort();

    return true;
}

// end of file



