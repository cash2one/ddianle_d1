#include "GameMsg_G2S_FriendApplicationForward2Receiver.h"
#include "../share/ServerMsgDef.h"
#include "../../socket/Utility.h"
#include "../../datastructure/datainfor/ItemType.h"


GameMsg_G2S_FriendApplicationForward2Receiver::GameMsg_G2S_FriendApplicationForward2Receiver()
    : GameMsg_Base(MSG_G2S_FriendApplicationForward2Receiver)
    , m_nSenderID(0)
    , m_nSex(0)
    , m_nLevel(0)
    , m_nCallBy(0)
    , m_nCallTo(0)
    , m_bIsVIP(false)
    , m_nVIPLevel(0)
    , m_bInsertMsgBox(false)
{
    memset(m_szRoleName, 0, MAX_NAME_LEN);
}

bool GameMsg_G2S_FriendApplicationForward2Receiver::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nSenderID);
    IOBuff.AddString(m_szRoleName);
    IOBuff.AddUChar(m_nSex);
    IOBuff.AddUShort(m_nLevel);
    IOBuff.AddUChar(m_nCallBy);
    IOBuff.AddUChar(m_nCallTo);
    IOBuff.AddBool(m_bIsVIP);
    IOBuff.AddUShort(m_nVIPLevel);
    IOBuff.AddBool(m_bInsertMsgBox);

    return true;
}

bool GameMsg_G2S_FriendApplicationForward2Receiver::doDecode(CParamPool &IOBuff)
{
    m_nSenderID = IOBuff.GetUInt();
    string str;
    str = IOBuff.GetString();
    CopyString2Buffer(str, m_szRoleName, MAX_NAME_LEN - 1);
    m_nSex = IOBuff.GetUChar();
    m_nLevel = IOBuff.GetUShort();
    m_nCallBy = IOBuff.GetUChar();
    m_nCallTo = IOBuff.GetUChar();
    m_bIsVIP = IOBuff.GetBool();
    m_nVIPLevel = IOBuff.GetUShort();
    m_bInsertMsgBox = IOBuff.GetBool();

    return true;
}

// end of file

