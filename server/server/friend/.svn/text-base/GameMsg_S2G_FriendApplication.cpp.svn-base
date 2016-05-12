#include "GameMsg_S2G_FriendApplication.h"
#include "../share/ServerMsgDef.h"
#include "../../socket/Utility.h"


GameMsg_S2G_FriendApplication::GameMsg_S2G_FriendApplication(unsigned int nSenderID, unsigned int nReceiverID, unsigned char nCallBy, unsigned char nCallTo, bool bInsertMsgBox)
    : GameMsg_Base(MSG_S2G_FriendApplication)
    , m_nSenderID(nSenderID)
    , m_nReceiverID(nReceiverID)
    , m_nCallBy(nCallBy)
    , m_nCallTo(nCallTo)
    , m_bInsertMsgBox(bInsertMsgBox)
{
}

bool GameMsg_S2G_FriendApplication::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nSenderID);
    IOBuff.AddUInt(m_nReceiverID);
    IOBuff.AddUChar(m_nCallBy);
    IOBuff.AddUChar(m_nCallTo);
    IOBuff.AddBool(m_bInsertMsgBox);

    return true;
}

bool GameMsg_S2G_FriendApplication::doDecode(CParamPool &IOBuff)
{
    m_nSenderID = IOBuff.GetUInt();
    m_nReceiverID = IOBuff.GetUInt();
    m_nCallBy = IOBuff.GetUChar();
    m_nCallTo = IOBuff.GetUChar();
    m_bInsertMsgBox = IOBuff.GetBool();

    return true;
}


