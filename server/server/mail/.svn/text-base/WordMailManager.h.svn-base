#ifndef __WORLDMAILMANAGER_H__
#define __WORLDMAILMANAGER_H__

#include "../../socket/GameMsg_Processor.h"
#include "../../datastructure/DataStruct_DB.h"

class CGroupRoleEntity;

class CWMailManager : public GameMsg_Processor
{
protected:
    CWMailManager();
    virtual ~CWMailManager();

public:
    static CWMailManager& Instance();
    void Initialize();

public:
    void SendMailToPlayer( unsigned int nReceiverID, EMailType nType, const std::string &rSender, const std::string &rTitle, const std::string &rContent );
    void SendMailToPlayer( unsigned int nReceiverID, EMailType nType, const std::string &rSender, const std::string &rTitle, const std::string &rContent,
        int nMoney, int nBindMCoin = 0 );
    void SendMailToPlayer( unsigned int nReceiverID, EMailType nType, const std::string &rSender, const std::string &rTitle, const std::string &rContent,
        itemtype_t nItemType, itemcount_t nCount, int nValidTime, int nDynamicBoxRelatedValue = 0 );
    void SendMailToPlayer( unsigned int nReceiverID, EMailType nType, const std::string &rSender, const std::string &rTitle, const std::string &rContent,
        const CItem &rItem );
    void SendMailToPlayer( unsigned int nReceiverID, EMailType nType, const std::string &rSender, const std::string &rTitle, const std::string &rContent,
        const std::list<CItem> &rItemList );
    void SendMailToPlayer( unsigned int nReceiverID, EMailType nType, const std::string &rSender, const std::string &rTitle, const std::string &rContent,
        int nMoney, int nBindMCoin, itemtype_t nItemType, itemcount_t nCount, int nValidTime = 0, int nDynamicBoxRelatedValue = 0 );

    void SendSystemMailToPlayer( unsigned int nReceiverID, EMailType nType, const std::string &rTitle, const std::string &rContent );
    void SendSystemMailToPlayer( unsigned int nReceiverID, EMailType nType, const std::string &rTitle, const std::string &rContent,
        int nMoney, int nBindMCoin = 0 );
    void SendSystemMailToPlayer( unsigned int nReceiverID, EMailType nType, const std::string &rTitle, const std::string &rContent,
        itemtype_t nItemType, itemcount_t nCount, int nValidTime, int nDynamicBoxRelatedValue = 0 );
    void SendSystemMailToPlayer( unsigned int nReceiverID, EMailType nType, const std::string &rTitle, const std::string &rContent,
        const CItem &rItem );
    void SendSystemMailToPlayer( unsigned int nReceiverID, EMailType nType, const std::string &rTitle, const std::string &rContent,
        const std::list<CItem> &rItemList );
    void SendSystemMailToPlayer( unsigned int nReceiverID, EMailType nType, const std::string &rTitle, const std::string &rContent,
        int nMoney, int nBindMCoin, itemtype_t nItemType, itemcount_t nCount, int nValidTime = 0, int nDynamicBoxRelatedValue = 0 );

    // ret: (receive:true, not receive: false)
    bool IsRoleReceiveStrangeMail(unsigned int nRole); // 
    void SetRoleReceiveStrangeMail(unsigned int nRole, bool bReceive);
    void InitReceiveStangeMailSet(std::vector<unsigned int> &vecRole);

    void OnPlayerLogin(CGroupRoleEntity &player);

private:
    void _RegisterMessage();
    void _RegisterMessageProcessor();

    void _OnSendMail( GameMsg_Base & msg, CSlotPeer & slotPeer );
    void _OnAddSysMail( QueryBase & refQuery );

    void _OnLoadMailStrangeSetting(QueryBase &refQuery);

    bool _MergeMailItem( mapMailItems & mapItems, const MailItem & Item );

    void _OnRequestReceiveStrangeMailFlag(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void _OnSetReceiveStrangeMailFlag(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void _OnValidSendMail(GameMsg_Base & msg, CSlotPeer & slotPeer);



    void SendMail( unsigned int nReceiverID, EMailType nType, const std::string &rSender, const std::string &rTitle, const std::string &rContent,
        int nMoney = 0, int nBindMCoin = 0, itemtype_t nItemType = 0, itemcount_t nCount = 0, int nValidTime = 0, int nDynamicBoxRelatedValue = 0 );

private:
    std::string m_strSystemName;

    // 不接受陌生人邮件的玩家
    std::set<unsigned int> m_setRoleReceiveStrangerMail;
};

#endif

