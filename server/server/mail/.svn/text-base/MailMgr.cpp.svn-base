#include "MailMgr.h"
#include "../libServerFrame/QueryManager.h"
#include "GameMsg_Mail.h"
#include "../../datastructure/Localization.h"
#include "../../datastructure/datainfor/ServerConfig.h"

extern CQueryManager*	g_pQueryMgr;
extern void SendMsg2GroupServer(LPGameMsg_Base pMsg);

CMailMgr::CMailMgr()
    : GameMsg_Processor( false, true )
{
}

CMailMgr::~CMailMgr()
{
}

CMailMgr& CMailMgr::Instance()
{
    static CMailMgr stcMailMgr;
    return stcMailMgr;
}

void CMailMgr::Init()
{
    RegComponentNetMsgMap();
    m_strSystemName = CLocalization::Instance().GetString( "System" );
}

void CMailMgr::RegComponentNetMsgMap()
{
    //数据库
    RegQueryProcessor( QUERY_AddSysMail, &CMailMgr::OnAddMail );
}

void CMailMgr::SendMailToPlayer( unsigned int nReceiverID, EMailType nType,
    const std::string &rSender, const std::string &rTitle, const std::string &rContent )
{
    SendMail( nReceiverID, nType, rSender, rTitle, rContent );
}

void CMailMgr::SendMailToPlayer( unsigned int nReceiverID, EMailType nType,
    const std::string &rSender, const std::string &rTitle, const std::string &rContent,
    int nMoney, int nBindMCoin )
{
    SendMail( nReceiverID, nType, rSender, rTitle, rContent, nMoney, nBindMCoin );
}

void CMailMgr::SendMailToPlayer( unsigned int nReceiverID, EMailType nType,
    const std::string &rSender, const std::string &rTitle, const std::string &rContent,
    itemtype_t nItemType, itemcount_t nCount, int nValidTime, int nDynamicBoxRelatedValue )
{
    SendMail( nReceiverID, nType, rSender, rTitle, rContent, 0, 0, nItemType, nCount, nValidTime, nDynamicBoxRelatedValue );
}

void CMailMgr::SendMailToPlayer( unsigned int nReceiverID, EMailType nType,
    const std::string &rSender, const std::string &rTitle, const std::string &rContent,
    const CItem &rItem )
{
    SendMail( nReceiverID, nType, rSender, rTitle, rContent, 0, 0,
        rItem.m_nItemType, rItem.m_nItemCount, rItem.m_nValidTime, rItem.m_nDynamicBoxRelatedValue );
}

void CMailMgr::SendMailToPlayer( unsigned int nReceiverID, EMailType nType,
    const std::string &rSender, const std::string &rTitle, const std::string &rContent,
    const std::list<CItem> &rItemList )
{
    for ( std::list<CItem>::const_iterator itr = rItemList.begin();
        itr != rItemList.end(); ++itr )
    {
        SendMailToPlayer( nReceiverID, nType, rSender, rTitle, rContent, *itr );
    }
}

void CMailMgr::SendMailToPlayer( unsigned int nReceiverID, EMailType nType,
    const std::string &rSender, const std::string &rTitle, const std::string &rContent,
    int nMoney, int nBindMCoin, itemtype_t nItemType, itemcount_t nCount, int nValidTime, int nDynamicBoxRelatedValue )
{
    SendMail( nReceiverID, nType, rSender, rTitle, rContent, nMoney, nBindMCoin, nItemType, nCount, nValidTime, nDynamicBoxRelatedValue );
}

void CMailMgr::SendSystemMailToPlayer( unsigned int nReceiverID, EMailType nType,
    const std::string &rTitle, const std::string &rContent )
{
    SendMailToPlayer( nReceiverID, nType, m_strSystemName, rTitle, rContent );
}

void CMailMgr::SendSystemMailToPlayer( unsigned int nReceiverID, EMailType nType,
    const std::string &rTitle, const std::string &rContent,
    int nMoney, int nBindMCoin )
{
    SendMailToPlayer( nReceiverID, nType, m_strSystemName, rTitle, rContent, nMoney, nBindMCoin );
}

void CMailMgr::SendSystemMailToPlayer( unsigned int nReceiverID, EMailType nType,
    const std::string &rTitle, const std::string &rContent,
    itemtype_t nItemType, itemcount_t nCount, int nValidTime, int nDynamicBoxRelatedValue )
{
    SendMailToPlayer( nReceiverID, nType, m_strSystemName, rTitle, rContent, nItemType, nCount, nValidTime, nDynamicBoxRelatedValue );
}

void CMailMgr::SendSystemMailToPlayer( unsigned int nReceiverID, EMailType nType,
    const std::string &rTitle, const std::string &rContent,
    const CItem &rItem )
{
    SendMailToPlayer( nReceiverID, nType, m_strSystemName, rTitle, rContent, rItem );
}

void CMailMgr::SendSystemMailToPlayer( unsigned int nReceiverID, EMailType nType,
    const std::string &rTitle, const std::string &rContent,
    const std::list<CItem> &rItemList )
{
    SendMailToPlayer( nReceiverID, nType, m_strSystemName, rTitle, rContent, rItemList );
}

void CMailMgr::SendSystemMailToPlayer( unsigned int nReceiverID, EMailType nType,
    const std::string &rTitle, const std::string &rContent,
    int nMoney, int nBindMCoin, itemtype_t nItemType, itemcount_t nCount, int nValidTime, int nDynamicBoxRelatedValue )
{
    SendMail( nReceiverID, nType, m_strSystemName, rTitle, rContent, nMoney, nBindMCoin, nItemType, nCount, nValidTime, nDynamicBoxRelatedValue );
}

void CMailMgr::SendMail( unsigned int nReceiverID, EMailType nType, const std::string &rSender, const std::string &rTitle, const std::string &rContent,
    int nMoney, int nBindMCoin, itemtype_t nItemType, itemcount_t nCount, int nValidTime, int nDynamicBoxRelatedValue )
{
    if ( nReceiverID == 0 )
        return;
    if ( rTitle.empty() )
        return;

    MailInfo *pMail = new MailInfo;
    SharedPtr<MailInfo> sharedMail( pMail );

    pMail->m_nSenderID = 0;
    pMail->m_nType = (short)nType;
    if ( rSender.empty() )
        pMail->SetSenderName( m_strSystemName );
    else
        pMail->SetSenderName( rSender );
    pMail->SetTitle( rTitle );
    pMail->SetContent( rContent );
    pMail->m_nSendTime = (unsigned int)time( NULL );
    pMail->m_bRead = false;
    pMail->m_nMoney = nMoney;
    pMail->m_nBindMCoin = nBindMCoin;
    pMail->m_nHasItem = (nItemType > 0 && nCount > 0) ? 1 : 0;

    if ( pMail->m_nHasItem == 1 )
    {
        MailItem item( nItemType, nCount, nValidTime, nDynamicBoxRelatedValue );
        pMail->m_mapItems.insert( std::make_pair( nItemType, item ) );
    }

    g_pQueryMgr->AddQuery( QUERY_AddSysMail, sharedMail, nReceiverID );
}

void CMailMgr::OnAddMail( QueryBase &rQuery )
{
    Query<MailInfo> *pTmp = dynamic_cast<Query<MailInfo> *>( &rQuery );
    if ( pTmp == NULL || pTmp->m_nResult != Query_Res_OK || !pTmp->IsValid() )
        return;

    // 通知组服务器
    GameMsg_S2G_SendMail msg;
    msg.nNoticeRoleID = (unsigned int)pTmp->m_nAttachment;
    msg.m_mailInfo = pTmp->GetRef();
    SendMsg2GroupServer( &msg );
}

//end file


