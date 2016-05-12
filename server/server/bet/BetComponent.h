#ifndef __BET_COMPONENT_H__
#define __BET_COMPONENT_H__

#include "../logic/RoleComponentImpl.h"
#include "../logic/EntityComponent.h"
#include "MagicArray/MagicArrayManager.h"


class CEntityNetComponent;
class CEntityAttributeComponent;
class CEntityItemComponent;
class CEntityQuestNewComponent;
struct DrawItem;

class CBetComponent : public CEntityComponent
{
public:
    CBetComponent(void);
    ~CBetComponent(void);

public:
    virtual void Start();
    virtual void OnEvent(CComponentEvent& refEvent) {}
    virtual void SerializeComponent(CParamPool& IOBuff) {}
    virtual const char* SerializeName() { return "Bet"; }
    virtual bool CreateFromDB(ROLEINFO_DB* pRoleInforDB);
    virtual bool PacketToDB(ROLEINFO_DB* pRoleInforDB) const { return true; }
    virtual bool PacketToCache(ROLEINFO_DB *pRoleInfoCache) const;
    virtual void OnLogin();
    virtual void OnLogout();
    virtual void OnUpdate(const unsigned long& lTimeElapsed);
    virtual void SendExtraStaticData(){};
    virtual void PacketBuff(std::list<BuffData>& listBuff) {}

public:
    void RegComponentNetMsgMap();

public:
    // Ԥ������
    void OnPreviewAllReward(GameMsg_Base &msg, CSlotPeer &slotPeer);

    // ħ���ٻ�
    void OnMagicCall(GameMsg_Base &msg, CSlotPeer &slotPeer);

    // ��ȡ�һ���Ʒ��Ϣ
    void OnGetExchangeItemInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);

    // �һ���Ʒ
    void OnExchange(GameMsg_Base &msg, CSlotPeer &slotPeer);

    // ��ȡħ������Ϣ
    void OnGetMagicArrayInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);

    // ˢ�¶һ�������Ʒ
    void OnRefreshExchangeItem(GameMsg_Base &msg, CSlotPeer &slotPeer);

    // GM roll����
    void OnGMRollMagicArrayReward(unsigned int nMagicArrayID, unsigned int nGmType, unsigned int nRollCount);
   
public:
    void _SendPlayerMsg(GameMsg_Base *pMsg);
	CEntityAttributeComponent * GetRoleAttr();
	CEntityItemComponent * GetRoleItem();

protected:
    CEntityNetComponent*        m_pRoleNetCOM;
    CEntityAttributeComponent*  m_pRoleAttrCOM;
    CEntityItemComponent*       m_pRoleItemCOM;
    CEntityQuestNewComponent*   m_pQuestCOM;

private:
    bool                         m_bIsOpening;
    unsigned int                 m_nLuckInterval;
    bool                         m_bMagicArrayOpen;
    std::map<unsigned int, bool> m_mapNoticeFreeCall;
    MagicArrayManager            m_MagicArrayManager;
};

typedef CRoleComponentImpl<CBetComponent,CGetRoleProcessorObj,CGetRoleProcessorObj> CRoleBet;

#endif
//file end

