/*
** Ϊ�˽��Ʃ��ͻ�������õ���ҵĸ�����Ϣ,������Ϣ�ֲ��ڸ��������.
** Ϊ�˱��ڹ�����ȫ�����������һ��
** ��ѯ�����ҵ���Ϣ����ͬһ����Ϸ�������򲻾������������
** ͨ�����������ѯ��ҵ���Ϣ
*/

#ifndef __PLAYER_INFO_COMPONENT_H__
#define __PLAYER_INFO_COMPONENT_H__

#include "../logic/RoleComponentImpl.h"
#include "../logic/EntityComponent.h"
#include "../../socket/GameMsg_Map.h"
#include "PlayerSettingDefine.h"

class PlayerInfo;
class PlayerPet;
class PlayerPetSkill;
struct ROLEINFO_DB;

class CRoomComponent;
class CEntityFriendComponent;
class CEntityNetComponent;
class CEntityAttributeComponent;
class CEntityItemComponent;
class CEntityAchievementComponent;
class CEntityCoupleComponent;
class CEntityVIPComponent;
class CAmuseRoomComponent;
class CRoomComponent;
class PetComponent;
class CEntityDanceGroupComponent;
class CEntityMallComponent;

class CPlayerInfoComponent : public CEntityComponent
{
private:
    CEntityNetComponent             * m_pNet;
    CEntityFriendComponent          * m_pFriend;
    CEntityAttributeComponent       * m_pRoleAttr;
    CEntityItemComponent            * m_pRoleItem;
    CEntityAchievementComponent     * m_pAch;
    CEntityCoupleComponent          * m_pRoleCouple;
    CEntityVIPComponent             * m_pRoleVIP;
    CAmuseRoomComponent             * m_pRoleAmuseRoom;
    CRoomComponent                  * m_pRoleRoom;
    PetComponent                    * m_pRolePet;
    CEntityDanceGroupComponent      * m_pRoleDanceGroup;
    CEntityMallComponent            * m_pRoleMall;

public:
    CPlayerInfoComponent();
    ~CPlayerInfoComponent();

public:
    virtual void Start();
    virtual const char *SerializeName(void);
    virtual void OnLogin();
    virtual void OnLogout();
    virtual bool CreateFromDB(ROLEINFO_DB* pRoleInforDB);
    virtual bool PacketToCache( ROLEINFO_DB* pRoleInforDB ) const;
    void RegComponentNetMsgMap();

public:
    bool AcceptRoomInvitation();
    bool IsOpenLBS();
    void ResetRolePosition();

    // ������Ϣ��ָ��������б��ɿ���
    void SendMsgToPlayerList(std::list<unsigned int> roleIDList, GameMsg_Base &msg, bool bToOtherLine = false);

private:
    void OnGetRoleInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnGetRoleInfoBackwardByGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnGetPlayerInfoForwardByGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);

    // sync offline data to group
    void GetOfflinePlayerInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void SyncPlayerDataToGroup(unsigned int nGainRoleID, EOfflinePlayerDataFrom eFrom); 

    void OnGetAnnouncement(GameMsg_Base &msg, CSlotPeer &slotPeer);

    void _OnOptionSetting(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void _OnPlayerMoveTo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void _OnPlayerMoveType(GameMsg_Base &msg, CSlotPeer &slotPeer);

    // ͬ�����䣬������ұ�����
    void OnSyncPlayerMotion(GameMsg_Base &msg, CSlotPeer &slotPeer);

    // ͬ�������Դ����״̬
    void OnSyncPlayerDownloading(GameMsg_Base &msg, CSlotPeer &slotPeer);

    void _OnPlayMakerSync(GameMsg_Base &msg, CSlotPeer &slotPeer);

    void _OnForward(GameMsg_Base &msg, CSlotPeer &slotPeer);

    void _SetOptionSetting(EPlayerSetting eOption, int nState);

    void SendPlayerMsg(GameMsg_Base *pMsg);
    void SendGroupServerMsg(GameMsg_Base *pMsg);

    void SetPlayerInfo(PlayerInfo &playerInfo, CRoleEntity &role);

protected:
    void SendMsgToAll(GameMsg_Base &msg, bool bToSelf = false);

public:
    CPlayerSetting m_playerSetting;
    CPlayerPosition m_position;
};

typedef CRoleComponentImpl<CPlayerInfoComponent,CGetRoleProcessorObj,CGetRoleProcessorObj> CPlayerInfo;

#endif

