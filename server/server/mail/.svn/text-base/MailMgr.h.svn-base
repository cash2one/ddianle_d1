#ifndef __MAIL_MGR_H__
#define __MAIL_MGR_H__

#include "../../socket/GameMsg_Processor.h"
#include "../../datastructure/DataStruct_Base.h"

class CMailMgr : public GameMsg_Processor
{
protected:
    CMailMgr();
    virtual ~CMailMgr();

public:
    static CMailMgr& Instance();

    void Init();
    void RegComponentNetMsgMap();
    void OnAddMail( QueryBase &rQuery );

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

private:
    void SendMail( unsigned int nReceiverID, EMailType nType, const std::string &rSender, const std::string &rTitle, const std::string &rContent,
        int nMoney = 0, int nBindMCoin = 0, itemtype_t nItemType = 0, itemcount_t nCount = 0, int nValidTime = 0, int nDynamicBoxRelatedValue = 0 );

private:
    std::string m_strSystemName;
};

#endif //__MAIL_MGR_H__

//end file

