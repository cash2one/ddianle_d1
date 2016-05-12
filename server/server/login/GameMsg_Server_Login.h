/*
*½ÇÉ«µÇÂ½
*
*/
#ifndef __GameMsg_Server_LOGINFO_H__
#define __GameMsg_Server_LOGINFO_H__

#include "../../socket/GameMsg_Base.h"

class GameMsg_Server_Login : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE( GameMsg_Server_Login );
    GameMsg_Server_Login( void );
    ~GameMsg_Server_Login( void );

public:
    virtual bool doDecode( CParamPool& IoBuff );

public:
    unsigned char m_nInstallerID;
    unsigned char m_nVersionType;
    bool m_bSwitchLine;
};
#endif

//file end

