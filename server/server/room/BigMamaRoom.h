#ifndef __GAMESERVER_BIGMAMAROOM_H__
#define __GAMESERVER_BIGMAMAROOM_H__

#include "../Macros/Macros.h"
#include <list>
#include <string>
#include <vector>

class DanceBaseRoom;
class CRoomMarkInfo;

/**
* DanceGroupArcadeRoomManager:
* manager dance group arcade room
*/

class BigMamaRoomManager
{
private:
    BigMamaRoomManager();
    virtual ~BigMamaRoomManager();
    BigMamaRoomManager(const BigMamaRoomManager &);

public:
    CLASS_INSTANCE_FUNC(BigMamaRoomManager)

public:
    unsigned int CreateAndEnterRoom( unsigned int nRoleID, const std::string &rRoomName, 
        int nScene, int nMusic, unsigned char nMusicMode, unsigned char nMusicLevel );

    bool Prepare( unsigned int nRoomID, unsigned char nPhoneOS, std::string &rCheckKey, std::string &rStage );
    void Start( unsigned int nRoomID );
    bool End( unsigned int nRoomID, unsigned int nRoleID, unsigned int &rScore, CRoomMarkInfo &rMark );

    // 把进度广播给房间里的所有人
    void OnLoadingProgress(unsigned int nRoleID, unsigned int nRoomID, unsigned char nRate);
    void ReportRoundMark( unsigned int nRoomID, unsigned int nRoleID, const std::string &rMD5Code, 
        unsigned int nMark, char nKeyRank, unsigned char nRound, const std::vector<char> &rRoundRank );

private:
    bool IsValidRoomID( unsigned int nRoomID ) const;

    void RemoveRoom( unsigned int nRoomID );

    DanceBaseRoom* FindRoom( unsigned int nRoomID ) const;

    bool LoadStageInfo( DanceBaseRoom *pRoom, unsigned char nPhoneOS, std::string &rStage ) const;

private:
    unsigned int    m_nPreUsedRoomID;
    std::vector<DanceBaseRoom *> m_vectRooms;
    std::list<DanceBaseRoom *> m_listIdleRooms;
};

#endif // __GAMESERVER_BIGMAMAROOM_H__

