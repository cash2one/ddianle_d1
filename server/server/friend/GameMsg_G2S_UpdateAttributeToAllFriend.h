#ifndef		__GAMEMSG_G2S_UPDATEATTRIBUTETOALLFRIENDS__
#define		__GAMEMSG_G2S_UPDATEATTRIBUTETOALLFRIENDS__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"

class GameMsg_G2S_UpdateAttributeToAllFriends:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_UpdateAttributeToAllFriends);
	GameMsg_G2S_UpdateAttributeToAllFriends();
	~GameMsg_G2S_UpdateAttributeToAllFriends();
public:
	unsigned int m_nRoleID;
	string m_strName;
	unsigned short m_nLevel;
	bool m_bIsVIP;
	unsigned short m_nVIPLevel;
public:
	virtual bool doEncode(CParamPool &IOBuff);
	virtual bool doDecode(CParamPool &IOBuff);
};



#endif



