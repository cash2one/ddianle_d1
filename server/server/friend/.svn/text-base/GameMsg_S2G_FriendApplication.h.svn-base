#ifndef __GAMEMSG_S2G_FRIENDAPPLICATION_H__
#define __GAMEMSG_S2G_FRIENDAPPLICATION_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"
#include <string>
#include "../../datastructure/datainfor/ItemType.h"

using namespace std;
class CParamPool;

class GameMsg_S2G_FriendApplication : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_FriendApplication);
    GameMsg_S2G_FriendApplication(unsigned int nSenderID = 0, unsigned int nReceiverID = 0, unsigned char nCallBy = 0, unsigned char nCallTo = 0, bool bInsertMsgBox = false);
    ~GameMsg_S2G_FriendApplication() {}

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int    m_nSenderID;
    unsigned int    m_nReceiverID;
    unsigned char   m_nCallBy;
    unsigned char   m_nCallTo;
    bool            m_bInsertMsgBox;
};





#endif

// end of file

