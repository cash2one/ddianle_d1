#include "WordMailManager.h"
#include "../group/Global.h"
#include "GameMsg_Mail.h"
#include "../../socket/GameMsg_Map.h"
#include "../libServerFrame/QueryManager.h"
#include "../../datastructure/Localization.h"
#include "../share/ServerMsgDef.h"
#include "MailDefine.h"

extern CQueryManager *g_pQueryMgr;

CWMailManager::CWMailManager()
    : GameMsg_Processor( true, true )
{
}

CWMailManager::~CWMailManager()
{
}

CWMailManager& CWMailManager::Instance()
{
    static CWMailManager s_WMailManager;
    return s_WMailManager;
}

void CWMailManager::Initialize()
{
    _RegisterMessage();
    _RegisterMessageProcessor();
    m_strSystemName = CLocalization::Instance().GetString( "System" );

    SharedPtr<std::vector<unsigned int> > p(new std::vector<unsigned int>);
    g_pQueryMgr->AddQuery(QUERY_LoadMailStrangerSetting, p);
}

void CWMailManager::SendMailToPlayer( unsigned int nReceiverID, EMailType nType,
    const std::string &rSender, const std::string &rTitle, const std::string &rContent )
{
    SendMail( nReceiverID, nType, rSender, rTitle, rContent );
}

void CWMailManager::SendMailToPlayer( unsigned int nReceiverID, EMailType nType,
    const std::string &rSender, const std::string &rTitle, const std::string &rContent,
    int nMoney, int nBindMCoin )
{
    SendMail( nReceiverID, nType, rSender, rTitle, rContent, nMoney, nBindMCoin );
}

void CWMailManager::SendMailToPlayer( unsigned int nReceiverID, EMailType nType,
    const std::string &rSender, const std::string &rTitle, const std::string &rContent,
    itemtype_t nItemType, itemcount_t nCount, int nValidTime, int nDynamicBoxRelatedValue )
{
    SendMail( nReceiverID, nType, rSender, rTitle, rContent, 0, 0, nItemType, nCount, nValidTime, nDynamicBoxRelatedValue );
}

void CWMailManager::SendMailToPlayer( unsigned int nReceiverID, EMailType nType,
    const std::string &rSender, const std::string &rTitle, const std::string &rContent,
    const CItem &rItem )
{
    SendMail( nReceiverID, nType, rSender, rTitle, rContent, 0, 0,
        rItem.m_nItemType, rItem.m_nItemCount, rItem.m_nValidTime, rItem.m_nDynamicBoxRelatedValue );
}

void CWMailManager::SendMailToPlayer( unsigned int nReceiverID, EMailType nType,
    const std::string &rSender, const std::string &rTitle, const std::string &rContent,
    const std::list<CItem> &rItemList )
{
    for ( std::list<CItem>::const_iterator itr = rItemList.begin();
        itr != rItemList.end(); ++itr )
    {
        SendMailToPlayer( nReceiverID, nType, rSender, rTitle, rContent, *itr );
    }
}

void CWMailManager::SendMailToPlayer( unsigned int nReceiverID, EMailType nType,
    const std::string &rSender, const std::string &rTitle, const std::string &rContent,
    int nMoney, int nBindMCoin, itemtype_t nItemType, itemcount_t nCount, int nValidTime, int nDynamicBoxRelatedValue )
{
    SendMail( nReceiverID, nType, rSender, rTitle, rContent, nMoney, nBindMCoin, nItemType, nCount, nValidTime, nDynamicBoxRelatedValue );
}

void CWMailManager::SendSystemMailToPlayer( unsigned int nReceiverID, EMailType nType,
    const std::string &rTitle, const std::string &rContent )
{
    SendMailToPlayer( nReceiverID, nType, m_strSystemName, rTitle, rContent );
}

void CWMailManager::SendSystemMailToPlayer( unsigned int nReceiverID, EMailType nType,
    const std::string &rTitle, const std::string &rContent,
    int nMoney, int nBindMCoin )
{
    SendMailToPlayer( nReceiverID, nType, m_strSystemName, rTitle, rContent, nMoney, nBindMCoin );
}

void CWMailManager::SendSystemMailToPlayer( unsigned int nReceiverID, EMailType nType,
    const std::string &rTitle, const std::string &rContent,
    itemtype_t nItemType, itemcount_t nCount, int nValidTime, int nDynamicBoxRelatedValue )
{
    SendMailToPlayer( nReceiverID, nType, m_strSystemName, rTitle, rContent, nItemType, nCount, nValidTime, nDynamicBoxRelatedValue );
}

void CWMailManager::SendSystemMailToPlayer( unsigned int nReceiverID, EMailType nType,
    const std::string &rTitle, const std::string &rContent,
    const CItem &rItem )
{
    SendMailToPlayer( nReceiverID, nType, m_strSystemName, rTitle, rContent, rItem );
}

void CWMailManager::SendSystemMailToPlayer( unsigned int nReceiverID, EMailType nType,
    const std::string &rTitle, const std::string &rContent,
    const std::list<CItem> &rItemList )
{
    SendMailToPlayer( nReceiverID, nType, m_strSystemName, rTitle, rContent, rItemList );
}

void CWMailManager::SendSystemMailToPlayer( unsigned int nReceiverID, EMailType nType,
    const std::string &rTitle, const std::string &rContent,
    int nMoney, int nBindMCoin, itemtype_t nItemType, itemcount_t nCount, int nValidTime, int nDynamicBoxRelatedValue )
{
    SendMail( nReceiverID, nType, m_strSystemName, rTitle, rContent, nMoney, nBindMCoin, nItemType, nCount, nValidTime, nDynamicBoxRelatedValue );
}


bool CWMailManager::IsRoleReceiveStrangeMail(unsigned int nRole)
{// 不在set中，则说明他接受邮件
    return m_setRoleReceiveStrangerMail.find(nRole) != m_setRoleReceiveStrangerMail.end();
}

void CWMailManager::SetRoleReceiveStrangeMail(unsigned int nRole, bool bReceive)
{
    if (IsRoleReceiveStrangeMail(nRole) == bReceive)
        return; // 不处理

    if (bReceive)
    {
        m_setRoleReceiveStrangerMail.insert(nRole);
    }
    else
    {
        m_setRoleReceiveStrangerMail.erase(nRole);
    }

    g_pQueryMgr->AddQuery(QUERY_UpdateMailStrangerSetting, nRole, NULL, bReceive); // 去更新数据库
}

void CWMailManager::InitReceiveStangeMailSet(std::vector<unsigned int> &vecRole)
{
    for (std::vector<unsigned int>::iterator iter = vecRole.begin(); iter != vecRole.end(); ++iter)
    {
        m_setRoleReceiveStrangerMail.insert(*iter); // 插入这条
    }
}

void CWMailManager::OnPlayerLogin(CGroupRoleEntity &player)
{ // 通知该标记是否设置
    GameMsg_S2C_ReceiveStrangeMailFlag msg;
    msg.m_bSet = !IsRoleReceiveStrangeMail(player.GetRoleID());
    player.SendPlayerMsg(&msg);
}

void CWMailManager::_RegisterMessage()
{
    GAMEMSG_REGISTERCREATOR( GameMsg_S2G_SendMail );
}

void CWMailManager::_RegisterMessageProcessor()
{
    RegMsgProcessor( MSG_S2G_SENDMAIL, &CWMailManager::_OnSendMail );

    RegMsgCreatorAndProcessor(GameMsg_C2S_ReceiveStrangeMailFlag, &CWMailManager::_OnRequestReceiveStrangeMailFlag);
    RegMsgCreatorAndProcessor(GameMsg_C2S_SetReceiveStrangeMailFlag, &CWMailManager::_OnSetReceiveStrangeMailFlag);
    RegMsgCreatorAndProcessor(GameMsg_S2G_ValidSendMail, &CWMailManager::_OnValidSendMail);

    //DB Call Back
    RegQueryProcessor( QUERY_AddSysMail, &CWMailManager::_OnAddSysMail );
    RegQueryProcessor( QUERY_LoadMailStrangerSetting, &CWMailManager::_OnLoadMailStrangeSetting);

}

void CWMailManager::_OnSendMail( GameMsg_Base & msg, CSlotPeer & slotPeer )
{
	GameMsg_S2G_SendMail * pMsg = (GameMsg_S2G_SendMail *)&msg;
	if (pMsg != NULL)
	{
		CGroupRoleEntity * pRole = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->nNoticeRoleID);
		if (pRole != NULL)
		{
			GameMsg_G2S_SendMail msgSendMail;
			msgSendMail.m_mailInfo = pMsg->m_mailInfo;
			pRole->SendPlayerMsg((GameMsg_Base *)&msgSendMail);
		}
	}
}

void CWMailManager::_OnAddSysMail( QueryBase & refQuery )
{
    Query<MailInfo> *pTmp = dynamic_cast<Query<MailInfo> *>( &refQuery );
    if ( pTmp == NULL || pTmp->m_nResult != Query_Res_OK || !pTmp->IsValid() )
        return;

    CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID( (unsigned int)pTmp->m_nAttachment );
    if ( pRole == NULL )
        return;

    GameMsg_G2S_SendMail msgSendMail;
    msgSendMail.m_mailInfo = pTmp->GetRef();
    pRole->SendPlayerMsg( &msgSendMail );
}

void CWMailManager::_OnLoadMailStrangeSetting(QueryBase &refQuery)
{
    Query<std::vector<unsigned int> > &query = (Query<std::vector<unsigned int> > &)refQuery;
    if (query.IsValid() && NULL != query.Get())
    {
        InitReceiveStangeMailSet(*(query.Get()));
    }
}


bool CWMailManager::_MergeMailItem( mapMailItems & mapItems, const MailItem & Item )
{
	if (Item.m_nType == 0 || Item.m_nCount <= 0){
		return false;
	}

	mapMailItems::iterator it = mapItems.find(Item.m_nType);
	if(it != mapItems.end())
	{
		MailItem & aItem = (it->second);
		aItem.m_nCount = aItem.m_nCount + Item.m_nCount;	
	}
	else{
		mapItems.insert(mapMailItems::value_type(Item.m_nType,Item));
	}
	return true;
}

void CWMailManager::_OnRequestReceiveStrangeMailFlag(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pGroupRole, return, "error to get group role by account[%u]", msg.nAccount);

    GameMsg_S2C_ReceiveStrangeMailFlag retMsg;
    retMsg.m_bSet = !IsRoleReceiveStrangeMail(pGroupRole->GetRoleID());
    pGroupRole->SendPlayerMsg(&retMsg);
}

void CWMailManager::_OnSetReceiveStrangeMailFlag(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pGroupRole, return, "error to get group role by account[%u]", msg.nAccount);

    GameMsg_C2S_SetReceiveStrangeMailFlag &rMsg = (GameMsg_C2S_SetReceiveStrangeMailFlag &)msg;
    SetRoleReceiveStrangeMail(pGroupRole->GetRoleID(), !rMsg.m_bSet);

    GameMsg_S2C_SetReceiveStrangeMailFlag retMsg;
    retMsg.m_bSet = rMsg.m_bSet; 
    pGroupRole->SendPlayerMsg(&retMsg);

    return;
}

void CWMailManager::_OnValidSendMail(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_S2G_ValidSendMail &rMsg = (GameMsg_S2G_ValidSendMail &)msg;

    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount); // 
    ENSURE_WITH_LOG_CMD(NULL != pGroupRole, return, "error to get group role by account[%u]", msg.nAccount);

    GameMsg_G2S_ValidSendMail retMsg;
    retMsg.m_nRes = (IsRoleReceiveStrangeMail(rMsg.m_mailInfoDB.m_nReceiverID) ? MAIL_OK : MAIL_NOT_RECEIVE_STRANGE);
    retMsg.m_mailInfoDB.Set(rMsg.m_mailInfoDB.m_nReceiverID, rMsg.m_mailInfoDB.m_szReceiverName, rMsg.m_mailInfoDB.GetTitle(), rMsg.m_mailInfoDB.GetContent(),
        rMsg.m_mailInfoDB.m_nMoney, rMsg.m_mailInfoDB.m_nItemType, rMsg.m_mailInfoDB.m_nCount, rMsg.m_mailInfoDB.m_nValidTime);

    pGroupRole->SendPlayerMsg(&retMsg); // 
}


void CWMailManager::SendMail( unsigned int nReceiverID, EMailType nType, const std::string &rSender, const std::string &rTitle, const std::string &rContent,
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

