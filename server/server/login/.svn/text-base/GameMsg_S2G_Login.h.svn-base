#ifndef __GAMEMSG_S2G_LOGIN_H__
#define __GAMEMSG_S2G_LOGIN_H__

#include <string>
#include <list>
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Couple.h"
#include "../../datastructure/ImageData.h"

using namespace std;
class CParamPool;

class GameMsg_S2G_Login : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_Login);
    GameMsg_S2G_Login();
    ~GameMsg_S2G_Login();

private:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    __int64             m_nSession;
    unsigned int        m_nAccount;
    unsigned int        m_nRoleID;
    string              m_strRoleName;
    unsigned char       m_nSex;
    unsigned short      m_nIndex;
    unsigned short      m_nLevel;
    int                 m_nMoney;
    unsigned int        m_nStar;                // 闯关 幸运星
    unsigned char       m_bTodayFirstLogin;
    unsigned short      m_nRoomID;
    bool                m_bIsVIP;
    unsigned short      m_nVIPLevel;
    short               m_nAge;
    short               m_nConstellation;
    unsigned short      m_nTopParcloseID;       // 最高挑战关卡ID
    unsigned int        m_nDivorceAgreementOtherRoleID; // 离婚协议书对方ID
    unsigned int        m_nDanceGroupID;
    unsigned char       m_nSkinColor;
    int                 m_nTransFormID;


    list<unsigned int>  m_listFriend;
    list<unsigned int>  m_listBlackList;

    CCoupleInfo         m_coupleInfo;

    RoleRoomImageData   m_imageData;
};

#endif

//end of file


