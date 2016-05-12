/*
** 好友会在初始化时候发送全部好友的部分信息。
** 好友列表详细信息会跟服务端申请。
**客户端添加或者删除好友的成功服务端会发送反馈消息，客户端自行维护该列表。
**	客户端打开列表时候，会发送好友详细信息的请求，GameServer会向GroupServer请求具体信息，等待返回再发送至客户端。
**	任何形式的添加好友(从陌生人添加、黑名单添加、屏蔽明的那添加)都会在在数据库的存储过程里面完成对应的数据修改
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

	// 加好友——要求必须两人都必须在本线、都在线
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
    
	// 给自己发送消息
	void SendPlayerMsg(GameMsg_Base *pMsg);

	// 给在线好友发送信息(不知道是否还有更高效的接口)
    void SendOnlinePlayersMsg(GameMsg_Base *pMsg); 

	// 获取好友列表
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

	// 修改好友备注
	void OnReworkFriendNotes( GameMsg_Base &msg, CSlotPeer &slotPeer );

    // 获取推荐好友
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

    // 朋友下线，更新状态
    void FriendLogout(unsigned int nFriendRoleID);

    // 朋友上线
    void FriendLogin(unsigned int nFriendRoleID);

    // 更新自己好友在线状态
    void UpdateFriendOnlineState();

    // 通知好友更新自己的在线状态
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
    map< unsigned int, FriendInfo * >   m_mapFriend;            // 好友名单
    map< unsigned int, FriendInfo * >   m_mapBlackList;         // 黑名单:好友被拉至屏蔽列表
    list<FriendInfo>                    m_listApplication;      // 请求加自己为好友的列表
    list<FriendInfo>                    m_listRecentlyPlayMate; // 最近玩伴，游戏开始时候才算进去
    list<FriendInfo>                    m_listRecentlyChatMate; // 最近聊天，相互私聊的对象
    std::set<unsigned int>              m_setRecommendFriends;  // 推荐过的好友角色ID

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


