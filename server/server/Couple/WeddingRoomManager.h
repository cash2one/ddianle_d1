#ifndef		__WEDDINGROOMMANAGER_H__
#define		__WEDDINGROOMMANAGER_H__

#include <list>
#include <map>
#include "WeddingRoom.h"
#include "CoupleMacroDef.h"
#include "../../socket/GameMsg_Processor.h"
#include "../../datastructure/LogStructDef.h"

class GameMsg_Base;
class CEntityCoupleComponent;

class CWeddingRoomManager : public GameMsg_Processor
{
public:
    typedef std::list<unsigned int> WeddingRoomIDList;
    typedef std::map<unsigned int, CWeddingRoom*> WeddingRoomMap;//	key - roomID
    typedef std::multimap<unsigned int, CWeddingRoom*> TimeWeddingRoomMap_t;	 //	key - expiredtime 	
private:
    CWeddingRoomManager();
    ~CWeddingRoomManager();

public:
    static CWeddingRoomManager & Instance();
    void Init();
    void OnUpdate(const unsigned long &lTimeElapsed);
    EWeddingRoomType RoomIDIndex(unsigned int nRoomID);

    void RegisterMessage();

public:
    void OnCoupleSeparated(const GameMsg_Base & rMsg, CEntityCoupleComponent & rRoleCouple);

    void OnGetWeddingRoomList(const GameMsg_Base & rMsg, CEntityCoupleComponent & rRoleCouple);

    int  OnCreateWeddingRoom(const GameMsg_Base & rMsg, CEntityCoupleComponent & rRoleCouple);

    void OnTryEnterWeddingRoom(const GameMsg_Base & rMsg, CEntityCoupleComponent & rRoleCouple);
    void OnEnterWeddingRoom(const GameMsg_Base & rMsg, CEntityCoupleComponent & rRoleCouple);

    void OnEnterMyWeddingRoom(const GameMsg_Base & rMsg, CEntityCoupleComponent & rRoleCouple);

    void OnStartWedding(const GameMsg_Base & rMsg, CEntityCoupleComponent & rRoleCouple);

    void OnExitRoom(const GameMsg_Base & rMsg, CEntityCoupleComponent & rRoleCouple);

    void OnKickWeddingRoomPlayer(const GameMsg_Base & rMsg, CEntityCoupleComponent & rRoleCouple);

    void OnSetWeddingPlayerType(const GameMsg_Base & rMsg, CEntityCoupleComponent & rRoleCouple);

    void OnChangeWeddingRoomPwd(const GameMsg_Base & rMsg, CEntityCoupleComponent & rRoleCouple);

    void OnBlessingCouple(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnAddRedEnvelpeBlessingValue(GameMsg_Base &msg, CSlotPeer &slotPeer);

    CWeddingRoom *RemoveRoom(unsigned int nRoomID);
    void DestroyRoom(unsigned int nRoomID, CRoleCoupleLog::ECoupleDestoryCase eAction);

    void PlayerLogout(CEntityCoupleComponent *pRoleCouple);

    // 查找结婚房间
    CWeddingRoom *FindWeddingRoomByPairID(unsigned int nPairID);
    void RemoveAllPlayer(CWeddingRoom *  pWeddingRoom);

private:
    unsigned int _GenerateWeddingRoomID();
    void _ReleaseWeddingRoomID(unsigned int nRoomID);

    bool ExistsSameCoupleRoom(unsigned int nPairID);

    bool _AddWeddingRoom(CWeddingRoom *pRoom);

    void _InTimeExpiredRoomMap(CWeddingRoom *pRoom);
    void _OutTimeExpiredRoomMap(CWeddingRoom *pRoom);

    void _InTimeStartNoficationRoomMap(CWeddingRoom *pRoom);
    void _OutTimeStartNoficationRoomMap(CWeddingRoom *pRoom);

    CWeddingRoom *_FindWeddingRoom(unsigned int nRoomID);


    int _PlayerInWeddingRoom(CEntityCoupleComponent &rRoleCouple, CWeddingRoom *pRoom, bool bCreate);
    void _PlayerOutWeddingRoom(CEntityCoupleComponent &rRoleCouple, EWeddingRoomQuitTo eQuitTo = EWeddingRoomQuitTo_Wait);


    bool _EnterWeddingRoom(CEntityCoupleComponent &rRoleCouple, CWeddingRoom *pRoom, const string & strPassword, int & nError, std::string & strError);
public:
    WeddingRoomIDList m_listIdleID;
    WeddingRoomIDList m_listSpawnedID;
    WeddingRoomMap m_AllRooms[EWeddingRoom_Max];
    TimeWeddingRoomMap_t m_mapTimeExpiredRooms;
    TimeWeddingRoomMap_t m_mapTimeStartNofication;
};

#endif

