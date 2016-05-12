/*
** 进展中的成就没有特别的队列来记录，客户端如果需要知道已经完成或者已经激活（即完成一部分），则遍历静态数据的全部的成就，
** 然后在RequirementHandler中取得对应的数值，即完成进度。
** 已完成的部分(领奖或者未领奖都记录)则在m_listAchievement中保存。
*/
#ifndef	__ENTITYACHIEVEMENT_H__
#define __ENTITYACHIEVEMENT_H__

#include <list>
#include "../logic/RoleComponentImpl.h"
#include "../logic/EntityComponent.h"
#include "IEntityAchivementComponent.h"

using namespace std;

class CEntityAttributeComponent;
class CEntityNetComponent;
class CEntityChatComponent;
class CEntityItemComponent;
class CEntityQuestComponent;
class CEntityAchievementComponent;
class AchievementConfig;
class CRoomComponent;

class CEntityAchievementComponent :
	public CEntityComponent, public IEntityAchievementComponent
{
public:
	CEntityAchievementComponent();
	~CEntityAchievementComponent();
	virtual CEntityAttributeComponent *		GetRoleAttr() const {return m_pRoleAttr;}
	virtual CEntityQuestComponent *			GetRoleQuest() const {return m_pRoleQuest;}
	virtual CEntityChatComponent	*		GetRoleChat() const {return m_pRoleChat;}
	virtual CEntityNetComponent *			GetRoleNet() const {return m_pRoleNet;}
	virtual CEntityItemComponent*			GetRoleItem() const {return m_pRoleItem;}
	virtual CEntityCheckInComponent*		GetRoleCheck() const {return m_pRoleCheck;}
public:
	virtual void OnEvent(CComponentEvent& refEvent );
	virtual void Start();
	virtual void OnLogin();
	virtual void OnLogout();
	virtual void OnUpdate(const unsigned long &nTimeElapsed);
	virtual void SerializeComponent(CParamPool &IOBuff);
	virtual const char* SerializeName(){ return "Achievement"; };
	virtual bool CreateFromDB( ROLEINFO_DB* pRoleInfoDB);
	virtual bool PacketToDB(ROLEINFO_DB *pRoleInforDB) const;
    virtual bool PacketToCache(ROLEINFO_DB *pRoleInfoCache) const;
    virtual void SendExtraStaticData(){};
	virtual void PacketBuff(std::list<BuffData>& listBuff) {}
public:
	void RegComponentNetMsgMap();
	void OnGetAchievementList(GameMsg_Base &msg, CSlotPeer & slotPeer);
	void OnGetAchievementReward(GameMsg_Base &msg, CSlotPeer &slotPeer);
	void ItemUse(itemtype_t nItemType, unsigned short nCount);
	void UpdateAchievement(unsigned short nSeiriesID);
	void MusicEnd(CRoomComponent& rRoleRoomCOM);
	void SetAchivementFightResult(CFightResult *pResult, CRoomComponent & roomComponent, int nScore, int nRank);
	void RoomStart(CRoomComponent & roomComponent);
	void RoomPerRoundScoreReport(CRoomComponent & roomComponent, int nScore, int nRank);
	unsigned int m_AchiScore();
private:
	bool IsInAchievementList(unsigned int nAchievementID);
	bool IsInTitleList(unsigned short nTitleID);
	CAchievementInfo *GetAchievementInfoBy(unsigned int nAchievementID);
	bool CanMeetAchivementCriteria(unsigned short nAchievementID);
	void AddNewAchievement( unsigned short nAchievement, unsigned int nCompletedTime, AchievementConfig *pAchievementDoc);
	void AddTitle(unsigned short nTitleID);	
	void AddTitleToDB(unsigned short nTitileID);
	void AddAchivement(unsigned short nAchievementID, unsigned int nCompletedTime,  AchievementConfig *pDoc, unsigned char nState);
	void AddAchievementToDB(unsigned short nAchievementID, unsigned int nCompletedTime);
	void UpdateAchievementStateToDB(unsigned int nRoleID, unsigned short nAchievementID, unsigned char nState);
	void SendPlayerMsg(GameMsg_Base *pMsg);
	bool IsFlower(itemtype_t nItemType);

	//记录角色完成成就
	void _LogRoleAchievement(unsigned int nRoleID, CRoleAchievementLog::EAchievementAction eAchievementAction, unsigned int nAchievementID);
	//记录成就物品的获得
	void _LogRoleAchievedItem(unsigned int nRoleID, unsigned int nAchievementID, itemtype_t nItemType, unsigned short nItemCount, int nDuration);
private:
	CEntityAttributeComponent		* m_pRoleAttr;
	CEntityNetComponent				* m_pRoleNet;
	CEntityItemComponent			* m_pRoleItem;
	CEntityChatComponent			* m_pRoleChat;
	CEntityQuestComponent			* m_pRoleQuest;
	CEntityCheckInComponent			* m_pRoleCheck;
public:
	unsigned int m_nLastUpdateTime;
private:
	int	m_nLastSpecialCount;
};

typedef CRoleComponentImpl<CEntityAchievementComponent,CGetRoleProcessorObj,CGetRoleProcessorObj> CRoleAchievement;

#endif

//end file




