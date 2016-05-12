#ifndef __ENTITYMAIL_COMPONENT_H__
#define __ENTITYMAIL_COMPONENT_H__

#include "../logic/RoleComponentImpl.h"
#include "../logic/EntityComponent.h"
#include "MailDefine.h"

class CEntityNetComponent;
class CEntityItemComponent;
class CEntityAttributeComponent;
class CEntityCoupleComponent;
class CEntityFriendComponent;

class CEntityMailComponent :
	public CEntityComponent
{
public:
	CEntityMailComponent(void);
	~CEntityMailComponent(void);

public:
	virtual void Start();
	virtual bool CreateFromDB(ROLEINFO_DB* pRoleInforDB);
	virtual bool PacketToDB(ROLEINFO_DB *pRoleInforDB) const {return true;};
	void RegComponentNetMsgMap();
	virtual void OnLogin();
	virtual void OnLogout(){};
	virtual void OnUpdate(const unsigned long &nTimeElapsed);
	virtual void SendExtraStaticData(){};
	virtual void PacketBuff(std::list<BuffData>& listBuff) {}
public:
	void GetMailBox(GameMsg_Base & pMsg ,CSlotPeer & SlotPeer);
	void GetMailInfo(GameMsg_Base &msg ,CSlotPeer &slotPeer);
	void SendMail(GameMsg_Base & pMsg ,CSlotPeer & SlotPeer);
    void OnValidSendMail(GameMsg_Base & pMsg, CSlotPeer & SlotPeer);
    void DeleteMail(GameMsg_Base & pMsg ,CSlotPeer & SlotPeer);
	void GetMailItem(GameMsg_Base & pMsg ,CSlotPeer & SlotPeer);
	void OnReceiveMail(GameMsg_Base & pMsg ,CSlotPeer & SlotPeer);
	int  GetMailMoney(MailInfo * pMail);
	virtual void SerializeComponent(CParamPool &IOBuff) {};
	virtual const char* SerializeName(){ return "Mail"; };

public:
	void SendMailToPlayer(unsigned int nReceiverID,EMailType nType,const char * pszTitle,const char * pszContent,int nMoney,itemtype_t nItemType,unsigned short nCount,int nValidTime);
	void DeleteWeddingRingMail();
	
public:
	void OnGetRoleID(QueryBase & pQuery);
	void OnDBLoadMail(QueryBase & pQuery);
	void OnDBSendMail(QueryBase & pQuery);

private:
	virtual void OnEvent( CComponentEvent& refEvent ){};
	void SendMailBox();
	int  GetAllMailItem(MailInfo * pMail);
	int  MergeMailItem(mapMailItems & mapItems,MailItem & Item);
	void DestoryMailMap(mapXQMail & mapXQBox);

	int  CanAddItem(mapMailItems & mapItems);
	int  AddItem(mapMailItems & mapItems, __int64 nMailID);

	void ReadMail(MailInfo * pMail);
	void GetMailsErrorString(int nErrno, string &strError);

	void IncreaseUnReadCount(int n = 1);
	void DecreaseUnReadCount(int n = 1);
	void SendUnReadMailMessage();

    bool HasMoneyInMail( const MailInfo *pMail ) const;

private:
	CEntityItemComponent       *  m_pRoleItem;
	CEntityNetComponent		   *  m_pNetCpt;
	CEntityAttributeComponent  *  m_pRoleAttr;
	CEntityCoupleComponent     *  m_pRoleCouple;
    CEntityFriendComponent     *  m_pRoleFriend;

	mapXQMail  m_mapMailBox;
	unsigned int m_nLastTick;

	int        m_nUnReadCount;	// Î´¶ÁÓÊ¼þÊýÁ¿
};

typedef CRoleComponentImpl<CEntityMailComponent,CGetRoleProcessorObj,CGetRoleProcessorObj> CRoleMail;

#endif //__ENTITYMALL_COMPONENT_H__


//end file


