#ifndef __GROUPSERVER_GROUPROOMPROCESSOR_H__
#define __GROUPSERVER_GROUPROOMPROCESSOR_H__


#include "../../socket/GameMsg_Processor.h"
#include "../roomLib/RoomDataStruct.h"

// class GameMsg_Base;
// class CSlotPeer;
class LWRoomManager;
class LWRoomBase;           // 
class CGroupRoleEntity;


// group ���������
class GroupRoomProcessor : public GameMsg_Processor
{
private:
    GroupRoomProcessor();
public:
    ~GroupRoomProcessor();

    static GroupRoomProcessor& Instance(); // 
    // ��ʼ��
    bool Init();
    // ����
    void OnUpdate(time_t tNow);
    void RegisterMessage(); // ע����Ϣ

    // get room manager pointer for room type.
    LWRoomManager* GetRoomManager(ERoomType eRT);
    // use room type and role id to get role room potiner.
    LWRoomBase* GetRoleRoom(ERoomType eRT, unsigned int nRoleID);
    LWRoomBase* GetRoleRoomByAccount(ERoomType eRT, unsigned int nAccountID);
    LWRoomBase* GetRoleRoom(CGroupRoleEntity *pRoleEntity);
    void OnRoleLogOut(CGroupRoleEntity *pGroupRole);
    void OnRoleQuitFromDanceGroup(CGroupRoleEntity *pGroupRole);
    void OnDanceGroupDisband(unsigned int nGroupID);

protected:
    void OnCreateRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnQuitRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
//     void OnGetRoomList(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnTryEnterRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
//    void OnEnterPlayer(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnKickPlayer(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

    // ��/��װ���ϲ���״̬�ı���Ϣ��ȥ
//     void OnInEquip(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
//     void OnOutEquip(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

    void OnChangeRoomInfo(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnChangeMusicInfo(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

    void OnChangeRoleRoomType(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnChangeRoleRoomState(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

    void OnChangeRoomPosState(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnPromoteRoomHost(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

    void OnQuitMarkAward(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

//////////////////////////////////////////////////////////////////////////
    // dance msg.
    // host try to start room.
    void OnStartRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    // client loading music resource progress.
    void OnLoadingStartGameProgress(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer);
    // downloading progress    
    void OnSyncPlayerDownloading(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer);
    // client loading music resource complete.
    void OnReadyRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    // client report mark theirselves every round.
    void OnReportRoundMark(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    // client report effect theirselves
    void OnReportEffectChange(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    // client dance complete, and wait all complete.
    void OnEndRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

protected:
    // ��ҿ������ݸ��´�����
    void OnTransFromIDUpdate(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnOnEquipNotify(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnUnEquipNotify(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnEffectChangeNotify(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnRefreshSevenColor(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnRefreshGeneNotify(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnAddMedals(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnSyncHorseData(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
//    void OnSyncHorseData(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
public:
    // �������ݸ��£�
    void OnRoleNameChanged(CGroupRoleEntity *pGroupRole, const std::string &strName);
    void OnRoleVipChanged(CGroupRoleEntity *pGroupRole, bool bIsVip, unsigned short nVipLev);
    void OnUpdateDanceGroupName(CGroupRoleEntity *pGroupRole, std::string strDanceGroupName);
    void OnUpdateDanceGroupTitle(CGroupRoleEntity *pGroupRole, unsigned char nTitle);
    void OnUpdateDanceGroupBadgeOrEffect(CGroupRoleEntity *pGroupRole, unsigned short nBadge, unsigned short nEffect);

private:
    void ClearRoomManager();

private:
    typedef LWRoomManager* LWRoomManagerPtr;

    // ���еķ��������
    LWRoomManagerPtr m_roomManagerArry[eRoomType_Max];
};



#endif // __GROUPSERVER_GROUPROOMPROCESSOR_H__