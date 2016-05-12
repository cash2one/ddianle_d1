/*
** ���ѻ��ڳ�ʼ��ʱ����ȫ�����ѵĲ�����Ϣ��
** �����б���ϸ��Ϣ�����������롣
**�ͻ�����ӻ���ɾ�����ѵĳɹ�����˻ᷢ�ͷ�����Ϣ���ͻ�������ά�����б�
**	�ͻ��˴��б�ʱ�򣬻ᷢ�ͺ�����ϸ��Ϣ������GameServer����GroupServer���������Ϣ���ȴ������ٷ������ͻ��ˡ�
**	�κ���ʽ����Ӻ���(��İ������ӡ���������ӡ��������������)�����������ݿ�Ĵ洢����������ɶ�Ӧ�������޸�
*/

#ifndef __ENTITYFRIEND_FRIENDCOMPONENT_H__
#define __ENTITYFRIEND_FRIENDCOMPONENT_H__
#include "../logic/RoleComponentImpl.h"
#include "../logic/EntityComponent.h"
#include "../chat/GameMsg_G2S_Chat.h"

class CEntityChatComponent;
class CEntityNetComponent;
class CEntityAttributeComponent;
class CRoomComponent;
class CEntityMailComponent;
class CEntityQuestComponent;
class CEntityVIPComponent;

class CEntityFriendComponent :
	public CEntityComponent
{
public:
	CEntityFriendComponent(void);
	~CEntityFriendComponent(void);

public:
	virtual void Start();
	void RegComponentNetMsgMap();
	void OnEvent(CComponentEvent& refEvent );
	bool IsInBlackList(unsigned int nRoleID);
	bool IsFriend(unsigned int nRoleID);
	unsigned short FriendCount();
	unsigned short BlackListCount();
	FriendInfo * GetFriend(unsigned int nRoleID);

	void UpdateFriendOnlineInfo(FriendInfo* pFriend,CFriendOnlineInfo& friendInfo);

	// �Ӻ��ѡ���Ҫ��������˶������ڱ��ߡ�������
	bool MakeOnlineFriend(unsigned int nFriendID, unsigned char nCallBy, unsigned char nCallTo);

public:
	bool CreateFromDB(ROLEINFO_DB* pRoleInfoDB);
	virtual bool PacketToDB(ROLEINFO_DB *pRoleInforDB) const;
	virtual void SerializeComponent(CParamPool &IOBuff);
	virtual const char* SerializeName(){ return "Friend"; };
	virtual void OnLogin();
	virtual void OnLogout();
	virtual void OnUpdate(const unsigned long &nTimeElapsed) {};
	virtual void SendExtraStaticData();
	virtual void PacketBuff(std::list<BuffData>& listBuff) {}

	void UpdateAttributeToAllFriends();
	void UpdateFriendInfo(unsigned int nFriendID);
	void PushOnlineInfoToAllFriends();
	void RequestAllFriendOnlineInfo();
	void AddRecentlyPlayMate(unsigned int nRoleID, const std::string& rName);
	void AddRecentlyChatMate(unsigned int nRoleID, const std::string& rName);
    
	// ���Լ�������Ϣ
	void SendPlayerMsg(GameMsg_Base *pMsg);

	// �����ߺ��ѷ�����Ϣ(��֪���Ƿ��и���Ч�Ľӿ�)
    void SendOnlinePlayersMsg(GameMsg_Base *pMsg); 

	// ��ȡ�����б�
	void GetFriendList( std::vector<unsigned int> &rolelist );

private:
	CEntityNetComponent * GetRoleNet(){return m_pNetComponent;}
	CEntityChatComponent * GetRoleChat(){return m_pChatComponent;}
	CEntityAttributeComponent * GetRoleAttr(){return m_pRoleAttr;}
	CRoomComponent * GetRoleRoom() {return m_pRoleRoom;}
	CEntityVIPComponent *GetRoleVIP() {return m_pRoleVIP;}

	void OnFriendApplication(GameMsg_Base &msg, CSlotPeer &slotPeer);
	void OnFriendApplicationGSBack2Sender(GameMsg_Base &msg, CSlotPeer &slotPeer);
	void OnFriendApplicationGSForward2Receiver(GameMsg_Base &msg, CSlotPeer &slotPeer);

	void OnAcceptApplication(GameMsg_Base &msg, CSlotPeer & slotPeer);
	void OnRefuseApplication(GameMsg_Base &msg, CSlotPeer & slotPeer);

	void OnAddBlackFriend(GameMsg_Base &msg, CSlotPeer &slotPeer);
	void OnRemoveFriend(GameMsg_Base &msg, CSlotPeer & slotPeer);

	void OnAddBlackList(GameMsg_Base &msg, CSlotPeer & slotPeer);
	void OnRemoveBlackList(GameMsg_Base &msg, CSlotPeer & slotPeer);

	void OnFindPlayer(GameMsg_Base &msg, CSlotPeer & slotPeer);
	void OnFindPlayerResult(GameMsg_Base &msg, CSlotPeer & slotPeer);

	void OnGetBlackListOnlineInfo(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void OnGetTempListOnlineInfo(GameMsg_Base & msg, CSlotPeer & slotPeer);

	void OnGetBlackListOnlineInfoFromWorld(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void OnGetTempListOnlineInfoFromWorld(GameMsg_Base & msg, CSlotPeer & slotPeer);

	void _OnGetFriendList(GameMsg_Base &msg, CSlotPeer &slotPeer);
	void OnPushOnlineInfoToAllFriendsFromWorld(GameMsg_Base &msg, CSlotPeer &slotPeer);

	void OnUpdateFriendAttributeFromWorld(GameMsg_Base &msg, CSlotPeer &slotPeer);

	void OnNotifiedHasBeenAccepted(GameMsg_Base &msg, CSlotPeer &slotPeer);
	void OnNotifyGameHasBeenRemovedFriend(GameMsg_Base &msg, CSlotPeer &slotPeer);
	void OnNotifyGameHasBeenDeletedBlacklist(GameMsg_Base &msg, CSlotPeer &slotPeer);

	void OnNotifiedFriendLogin(GameMsg_Base &msg, CSlotPeer &slotPeer);

	// �޸ĺ��ѱ�ע
	void OnReworkFriendNotes( GameMsg_Base &msg, CSlotPeer &slotPeer );

    // ��ȡ�Ƽ�����
    void OnGetRecommendFriends(GameMsg_Base &msg, CSlotPeer &slotPeer);

	void NotifyGroupAddBlackListFriend(unsigned int nRoleID, FriendInfo & friendInfo);
	void NotifyGroupRemoveBlackList(unsigned nRoleID, unsigned int nRemoveID);
	void NotifyGroupBecomeFriends(unsigned int nRoleID, const FriendInfo &friendInfo);

	void AddBlackListCallBack(QueryBase &query);
	void RemoveFriendDBCallBack(QueryBase &query);
	void RemoveBlackListDBCallBack(QueryBase &query);
	void BecomeFriendsCallBack(QueryBase &query);
	void AddBlackListFriendDBCallBack(QueryBase &query);
	void GetBeAppliedPlayerInfoDBCallBack(QueryBase &query);

private:
	bool CheckFriendRelation(int nLeftNameCode, int nRightNameCode, int nLeftGender, int nRightGender);
	void AddFriend(FriendInfo *pFriendInfo);

	void InsertIntoFriendMap(FriendInfo * pFriendInfo);
	void InsertIntoBlackMap(FriendInfo * pFriendInfo);
	FriendInfo * RemoveFriendEntity(unsigned int nRoleID);
	FriendInfo * RemoveBlackEntry(unsigned int nRoleID);

	void AddApplication(const FriendInfo & friendInfo);
	void RemoveApplication(unsigned int nRoleID);
	bool IsInApplicationList(const FriendInfo & friendInfo);

	unsigned short FriendsLimitedCount();
	unsigned short BlackListLimitedCount();

	FriendInfo * GetBlack(unsigned int nRoleID);

	void SendBlackList();
	void SendFindPlayerSetting();

    void CheckRecommendResetTime(unsigned int nNow);
    void RandomRecommendFriends(std::list<CFindPlayerInfo> &rFriends, unsigned char oppositeSex, unsigned int nMaxCount);

    void LogRoleFriends(unsigned char eAction, unsigned char eMode, unsigned int nFriendID);

    // �������ߣ�����״̬
    void FriendLogout(unsigned int nFriendRoleID);

    // ��������
    void FriendLogin(unsigned int nFriendRoleID);

    // �����Լ���������״̬
    void UpdateFriendOnlineState();

    // ֪ͨ���Ѹ����Լ�������״̬
    void NoticeFriendLogin();

    // 

private:
    CEntityNetComponent                *m_pNetComponent;
    CEntityChatComponent               *m_pChatComponent;
    CEntityAttributeComponent          *m_pRoleAttr;
    CRoomComponent                     *m_pRoleRoom;
    CEntityMailComponent               *m_pRoleMail;
    CEntityQuestComponent              *m_pRoleQuest;
    CEntityVIPComponent                *m_pRoleVIP;

public:
    map< unsigned int, FriendInfo * >   m_mapFriend;            // ��������
    map< unsigned int, FriendInfo * >   m_mapBlackList;         // ������:���ѱ����������б�
    list<FriendInfo>                    m_listApplication;      // ������Լ�Ϊ���ѵ��б�
    list<FriendInfo>                    m_listRecentlyPlayMate; // �����飬��Ϸ��ʼʱ������ȥ
    list<FriendInfo>                    m_listRecentlyChatMate; // ������죬�໥˽�ĵĶ���
    std::set<unsigned int>              m_setRecommendFriends;  // �Ƽ����ĺ��ѽ�ɫID

private:
    unsigned int                        m_nBlackListInterval;
    unsigned int                        m_nPlayMateInterval;
    unsigned int                        m_nChatMateInterval;
    unsigned int                        m_nLastFindPlayerTime;
    unsigned int                        m_nLastGetRecommendTime;
    unsigned int                        m_nRecommendRefreshCount;
    unsigned int                        m_nLastRecommendResetTime;
};

typedef CRoleComponentImpl<CEntityFriendComponent,CGetRoleProcessorObj,CGetRoleProcessorObj> CRoleFriend;

#endif

//end file


