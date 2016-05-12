#ifndef	__ENTITYMONTHCARDCOMPONENT_H__
#define __ENTITYMONTHCARDCOMPONENT_H__
#include "../logic/RoleComponentImpl.h"
#include "../logic/EntityComponent.h"

class CEntityNetComponent;
class CEntityAttributeComponent;
class CEntityChatComponent;
class CEntityItemComponent;
class CEntityQuestComponent;
class CEntityAchievementComponent;
class CEntityMailComponent;

class CEntityMonthCardComponent :
	public CEntityComponent
{
public:
	CEntityMonthCardComponent();
	~CEntityMonthCardComponent();
	
public:
	void Start();
	void RegComponentNetMsgMap();
	virtual bool CreateFromDB(ROLEINFO_DB* pRoleInforDB);
	virtual bool PacketToDB(ROLEINFO_DB*pRoleInforDB) const;
    virtual bool PacketToCache( ROLEINFO_DB* pRoleInforDB ) const;
	virtual void SerializeComponent(CParamPool &IOBuff) ;
	virtual void OnLogin();
	virtual void OnLogout();
	virtual void OnUpdate(const unsigned long & nTimeElapsed);
	virtual void OnEvent(CComponentEvent& refEvent );
	virtual const char* SerializeName(){ return "MonthCard"; };
	unsigned char GetCurrentCheckCount() const;		// 当前签到次数
	virtual void SendExtraStaticData(){};
	virtual void PacketBuff(std::list<BuffData>& listBuff) {}

public:
	// 获取月卡信息
	void OnGetMonthCardInfo(GameMsg_Base &rMsg, CSlotPeer & slotPeer);

	// 开通月卡
	void OnOpenMonthCard(GameMsg_Base &rMsg, CSlotPeer & slotPeer);
	int OpenMonthCard();

	// 续费月卡
	void OnRenewMonthCard(GameMsg_Base &rMsg, CSlotPeer & slotPeer);
	int RenewMonthCard();

	// 领取每日奖励
	void OnGetEveryDayReward(GameMsg_Base &rMsg, CSlotPeer & slotPeer);
	int GetEveryDayReward();

	// 是否能领取奖励
	bool isCanReward(unsigned int nOpenTime, unsigned int nRewardTime);

public:
	MonthCardDBTable	m_MonthCard;

public:
	CEntityAttributeComponent * m_pRoleAttr;
	CEntityNetComponent * m_pRoleNet;
	CEntityItemComponent * m_pRoleItem;
};

typedef CRoleComponentImpl<CEntityMonthCardComponent,CGetRoleProcessorObj,CGetRoleProcessorObj> CRoleMonthCard;

#endif


//end file


