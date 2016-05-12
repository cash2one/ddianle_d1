#include "GameMsg_S2G_Login.h"
#include "../../socket/ParamPool.h"
#include "../share/ServerMsgDef.h"

GameMsg_S2G_Login::GameMsg_S2G_Login()
    : GameMsg_Base(MSG_S2G_Login)
{
    m_nSession = 0;
    m_nAccount = 0;
    m_nRoleID = 0;
    m_strRoleName = "";
    m_nSex = 0;
    m_nIndex = 0;
    m_nLevel = 0;
    m_nMoney = 0;
    m_nStar = 0;
    m_bTodayFirstLogin = 0;
    m_nRoomID = 0;
    m_bIsVIP = false;
    m_nVIPLevel = 0;
    m_nAge = 0;
    m_nConstellation = 0;
    m_nTopParcloseID = 0;
    m_nDivorceAgreementOtherRoleID = 0;
    m_nDanceGroupID = 0;
}

GameMsg_S2G_Login::~GameMsg_S2G_Login()
{
}

bool GameMsg_S2G_Login::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt64(m_nSession);
    IOBuff.AddUInt(m_nAccount);
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddString(m_strRoleName.c_str() );
    IOBuff.AddUChar(m_nSex);
    IOBuff.AddUShort(m_nIndex);
    IOBuff.AddUShort(m_nLevel);
    IOBuff.AddInt(m_nMoney);
    IOBuff.AddUInt(m_nStar);
    IOBuff.AddUChar(m_bTodayFirstLogin);
    IOBuff.AddBool(m_bIsVIP);
    IOBuff.AddUShort(m_nVIPLevel);
    IOBuff.AddShort(m_nAge);
    IOBuff.AddShort(m_nConstellation);
    IOBuff.AddUShort(m_nTopParcloseID);
    IOBuff.AddUInt(m_nDivorceAgreementOtherRoleID);
    IOBuff.AddUInt(m_nDanceGroupID);

    std::list<unsigned int>::iterator itr;

    IOBuff.AddUShort((unsigned short)m_listFriend.size() );
    for (itr = m_listFriend.begin(); itr != m_listFriend.end(); ++itr)
        IOBuff.AddUInt(*itr);

    IOBuff.AddUShort((unsigned short)m_listBlackList.size() );
    for (itr = m_listBlackList.begin(); itr != m_listBlackList.end(); ++itr)
        IOBuff.AddUInt(*itr);

    m_coupleInfo.doEncode(IOBuff);

    return true;
}

bool GameMsg_S2G_Login::doDecode(CParamPool &IOBuff)
{
    m_nSession = IOBuff.GetInt64();
    m_nAccount = IOBuff.GetUInt();
    m_nRoleID = IOBuff.GetUInt();
    m_strRoleName = IOBuff.GetString();
    m_nSex = IOBuff.GetUChar();
    m_nIndex = IOBuff.GetUShort();
    m_nLevel = IOBuff.GetUShort();
    m_nMoney = IOBuff.GetInt();
    m_nStar  = IOBuff.GetUInt();
    m_bTodayFirstLogin = IOBuff.GetUChar();
    m_bIsVIP = IOBuff.GetBool();
    m_nVIPLevel = IOBuff.GetUShort();
    m_nAge = IOBuff.GetShort();
    m_nConstellation = IOBuff.GetShort();
    m_nTopParcloseID = IOBuff.GetUShort();
    m_nDivorceAgreementOtherRoleID = IOBuff.GetUInt();
    m_nDanceGroupID = IOBuff.GetUInt();

    unsigned int nSize = IOBuff.GetUShort();
    while (nSize-- > 0)
    {
        m_listFriend.push_back(IOBuff.GetUInt() );
    }

    nSize = IOBuff.GetUShort();
    while (nSize-- > 0)
    {
        m_listBlackList.push_back(IOBuff.GetUInt() );
    }

    m_coupleInfo.doDecode(IOBuff);

    return true;
}


//end of file


