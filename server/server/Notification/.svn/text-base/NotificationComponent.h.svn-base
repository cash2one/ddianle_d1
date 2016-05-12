#ifndef __NOTIFICATION_COMPONENT_H_H__
#define __NOTIFICATION_COMPONENT_H_H__ 1

#include "../logic/EntityComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../../datastructure/DataStruct_DB.h"
#include "../../socket/SharedPtr.h"

#include "NotificationEvent.h"

class LazyMessage
{
public:
    LazyMessage(GameMsg_Base *msg, int nPostTime = 3000, int nThresholdTime = 3000);
    LazyMessage(SharedPtr<GameMsg_Base> &msg, int nPostTime = 3000, int nThresholdTime = 3000);
    virtual ~LazyMessage();

    void Elapsed(int nTime);

    bool IsPostable() const;

    GameMsg_Base *GetMsg();

protected:
    SharedPtr<GameMsg_Base> m_msg;
    int m_nElapsedTime;
    int m_nThresholdTime;
    int m_nPostTime;
};

class CNotificationComponent :
	public CEntityComponent
{
public:
	CNotificationComponent();
	~CNotificationComponent();

public:
    void LazyPost(GameMsg_Base *msg, int nMilliseconds);
    void LazyPost(SharedPtr<GameMsg_Base> &msg, int nMilliseconds);

public:
	void RegComponentNetMsgMap();
	void OnNoPushRating(GameMsg_Base &msg, CSlotPeer &slotPeer);
	void OnInfoCenterGetIDRes(GameMsg_Base &msg, CSlotPeer &slotPeer);
	void InfoCenterMarkReaded(GameMsg_Base &msg, CSlotPeer &slotPeer);

public:
	void Start();
	void OnUpdate(const unsigned long &nTimeElapsed);
	bool CreateFromDB(ROLEINFO_DB* pRoleInfoDB);
	bool PacketToDB(ROLEINFO_DB *pRoleInforDB) const; 
    virtual bool PacketToCache(ROLEINFO_DB *pRoleInfoCache) const;
    void OnLogin();
	void OnLogout();

	void SendExtraStaticData();
	
	void SerializeComponent(CParamPool &IOBuff);
	const char* SerializeName() { return "Notification"; }
	virtual void PacketBuff(std::list<BuffData>& listBuff) {}

	// 检测某个消息是否已经读取
	bool HasReadInfoCenterInfo(unsigned int id, unsigned int lastUpdateTime); // 
	bool MarkInfoCenterRead(unsigned int id, unsigned int now); // 当前时间

	void SendPlayerMsg(GameMsg_Base &msg);
public:
	void AddEvent(CNotificationEvent &event);

protected:
	void UpdatePushRating();
	void PushRatingMessage(unsigned int nNow, int nPushReason);
    void OnLazyPost(int nElapsedMilliseconds);

protected:
	CEntityAttributeComponent   * m_pAttr;
	CEntityNetComponent         * m_pNet;

protected:
	CNotificationEventQueue m_ratingEventQueue;

	int m_nUpdateInterval;

	unsigned int m_nLoginTime;
	unsigned int m_nLastPushRatingTime;

	int m_nLastPushRatedLevel;
	unsigned int m_nNoPushRatingTime;
	unsigned int m_nPushRatedOnlineTime;
	unsigned int m_nPushRatedDailyQuest;
	unsigned int m_nPushRatedDailyShoping;
	int m_nPushRatedCount;					// 本周推送次数
	unsigned int m_nPushRatedCountTime;		// 本周记数时间


	// <info id, read time>
	typedef std::map<unsigned int, unsigned int> InfoCenterReadFlag;
	InfoCenterReadFlag m_infoCenterReadFlag; // 记录什么时候，读取了什么消息

    std::list<LazyMessage> m_listLazyMessage;
};

typedef CRoleComponentImpl<CNotificationComponent,CGetRoleProcessorObj,CGetRoleProcessorObj> CRoleNotification;

#endif //__NOTIFICATION_COMPONENT_H_H__

