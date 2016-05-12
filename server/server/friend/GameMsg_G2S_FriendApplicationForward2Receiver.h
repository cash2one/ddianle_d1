#ifndef __GAMEMSG_G2S_FRIENDAPPLICATIONFORWARD2RECEIVER_H__
#define __GAMEMSG_G2S_FRIENDAPPLICATIONFORWARD2RECEIVER_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"
#include <string>

using namespace std;
class CParamPool;

class GameMsg_G2S_FriendApplicationForward2Receiver : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_FriendApplicationForward2Receiver);
    GameMsg_G2S_FriendApplicationForward2Receiver();
    ~GameMsg_G2S_FriendApplicationForward2Receiver() {}

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int    m_nSenderID;
    char            m_szRoleName[MAX_NAME_LEN];
    unsigned char   m_nSex;
    unsigned short  m_nLevel;
    unsigned char   m_nCallBy;
    unsigned char   m_nCallTo;
    bool            m_bIsVIP;
    unsigned short  m_nVIPLevel;
    bool            m_bInsertMsgBox;
};





#endif

// end of file

