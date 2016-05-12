#ifndef		__WEDDINGROOM_H__
#define		__WEDDINGROOM_H__

#include <string>
#include <map>
#include <vector>
#include "CoupleMacroDef.h"

class CEntityCoupleComponent;
class CWeddingRoomWholeInfo;
class CWeddingRoomBriefInfo;
class GameMsg_Base;


typedef std::map<unsigned int, CEntityCoupleComponent*> WeddingRoomPlayerMap_t;

class CWeddingRoom
{
public:
    CWeddingRoom();
    ~CWeddingRoom();
public:
    EWeddingRoomType RoomType();
    unsigned int RoomID();
    unsigned int PairID();
    unsigned int HostID();
    time_t ExpiredTime();
    time_t StartingNotificatioiTime();
    const std::string & RoomName();
    bool HasPassword() const;
    void SetPassword(const std::string & strPassword);
    const std::string & Password();
    unsigned int GroomID();
    const std::string & GroomName();
    unsigned int BrideID();
    const std::string & BrideName();
    time_t CreateTime();
    const WeddingRoomPlayerMap_t & WeddingRoomPlayerMap();

    int AddWeddingPlayer(CEntityCoupleComponent * pRoleCouple);
    CEntityCoupleComponent * RemoveWeddingPlayer(unsigned int nRoleID);
    CEntityCoupleComponent * RemoveWeddingPlayer(CEntityCoupleComponent * pRoleCouple);


    CEntityCoupleComponent * GetWeddingPlayer(unsigned int nRoleID);

    CEntityCoupleComponent * GetWeddingPlayer(EWeddingRoomRoleType eRoomRoleType);

    void SetValid(bool bValid);
    bool Valid();

    void OnRoomStartingNofiication();
    void OnRoomEndNofiication();
    void OnRoomAutoStart();
    void OnStateChange(bool bValid);

    bool IsHaveBlessing(unsigned int nRoleID);
    void BlessingCouple(unsigned int nRoleID, unsigned int nSex);
    void SendGiftToBlessinger();
    void SendGiftToCouple();
    void RedEnvelope(unsigned int nRoleID, std::string & strRoleName, unsigned int nRedEnvelopeCount);
    unsigned int GetBlessingValue(){
        return m_nBlessingValue;
    }
    void SetBlessingValue(unsigned int nBlessingValue){
        m_nBlessingValue = nBlessingValue;
    }
    unsigned int GetRedEnvelopeAllCount();
    std::string  GetGroomName(){
        return m_strGroomName;
    }
    std::string  GetBrideName(){
        return m_strBrideName;
    }

    void SetRoomBriefInfo(CWeddingRoomBriefInfo & rRoomBriefIno) const;
    void SetWholeInfo(CWeddingRoomWholeInfo & rWholeInfo, bool bHost) const;
    void SendMsgToAllBut(GameMsg_Base *pMsg, CEntityCoupleComponent *pExceptRole);
    void SendMsgToAll(GameMsg_Base *pMsg);
public:
    EWeddingRoomType m_eWeddingType;
    unsigned int m_nRoomID;
    unsigned int m_nPairID;
    unsigned int m_nHostID;

    unsigned int m_nBlessingValue;								// 玩家祝福值
    std::vector<unsigned int> m_BlessingRoleIDList;				// 祝福玩家ID列表
    std::map<unsigned int, unsigned int> m_RedEnvelopePlayers;   // 红包玩家ID->红包数目

    std::string m_strRoomName;
    std::string m_strPassword;
    unsigned int m_nGroomID;
    std::string m_strGroomName;
    unsigned int m_nBrideID;
    std::string m_strBrideName;
    time_t m_nCreateTime;
    bool m_bValid;		                   // 情侣分手后设置为无效 m_bValid = false;

    WeddingRoomPlayerMap_t	m_mapPlayers;
};

#endif

