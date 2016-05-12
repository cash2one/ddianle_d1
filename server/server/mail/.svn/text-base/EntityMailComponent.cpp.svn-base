#include "EntityMailComponent.h"
#include "MailMsgDefine.h"
#include "GameMsg_Mail.h"
#include "../item/EntityItemComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../../socket/GameMsg_Map.h"
#include "../../datastructure/ItemErrorInfo.h"
//#include "../../liblog/LogType.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../friend/EntityFriendComponent.h"
#include "../util/NameDeny.h"
#include "../logic/MsgDeliver.h"

CEntityMailComponent::CEntityMailComponent(void)
:CEntityComponent(true,true)
{
	m_pRoleItem = NULL;
	m_pNetCpt   = NULL;
	m_pRoleAttr = NULL;
    m_pRoleFriend = NULL;

	m_nLastTick = 0;
	m_nUnReadCount = 0;
}

CEntityMailComponent::~CEntityMailComponent(void)
{
	DestoryMailMap(m_mapMailBox);
}

void CEntityMailComponent::Start()
{
	m_pRoleItem = GetComponent<CRoleItem>();
	m_pNetCpt   = GetComponent<CRoleNet>();
	m_pRoleAttr = GetComponent<CRoleAttribute>();
	m_pRoleCouple = GetComponent<CRoleCouple>();
    m_pRoleFriend = GetComponent<CRoleFriend>();
}

bool CEntityMailComponent::CreateFromDB(ROLEINFO_DB* pRoleInforDB)
{
	if (pRoleInforDB != NULL) 
    {
		//m_mapMailBox = pRoleInforDB->mapMailBox;

        mapMail &mailMap = pRoleInforDB->mapMailBox.GetMap();
        mapMail::iterator it = mailMap.begin();
        for ( ; it != mailMap.end(); ++it )
        {
            // 屏蔽黑名单玩家邮件
            if ( m_pRoleFriend->IsInBlackList( it->second->m_nSenderID))
            {
                continue;
            }

            // 计算未读取邮件
            if (it->first >0 && it->second != NULL && !it->second->m_bRead)
            {
                m_nUnReadCount++;
            }

            m_mapMailBox.Add( it->first,it->second);
        }
	}

	return true;
}

void CEntityMailComponent::RegComponentNetMsgMap()
{
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetMailBox);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetMailInfo);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_SendMail);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_DeleteMail);
	GAMEMSG_REGISTERCREATOR(GameMsg_G2S_SendMail);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetMailItem);	
	
	RegMsgProcessor(MSG_C2S_GETMAILBOX, &CEntityMailComponent::GetMailBox);
	RegMsgProcessor(MSG_C2S_GETMAILINFO, &CEntityMailComponent::GetMailInfo);
	RegMsgProcessor(MSG_C2S_SENDMAIL, &CEntityMailComponent::SendMail);
    RegMsgCreatorAndProcessor(GameMsg_G2S_ValidSendMail, &CEntityMailComponent::OnValidSendMail);
    RegMsgProcessor(MSG_C2S_DELETEMAIL, &CEntityMailComponent::DeleteMail);
	RegMsgProcessor(MSG_C2S_GETMAILITEM, &CEntityMailComponent::GetMailItem);
	RegMsgProcessor(MSG_G2S_SENDMAIL, &CEntityMailComponent::OnReceiveMail);

    RegMsgCreatorAndProcessor(GameMsg_C2S_ReceiveStrangeMailFlag, &MsgDeliver::FromClientToGroup);
    RegMsgCreatorAndProcessor(GameMsg_S2C_ReceiveStrangeMailFlag, &MsgDeliver::FromGroupToClient);
    RegMsgCreatorAndProcessor(GameMsg_C2S_SetReceiveStrangeMailFlag, &MsgDeliver::FromClientToGroup);
    RegMsgCreatorAndProcessor(GameMsg_S2C_SetReceiveStrangeMailFlag, &MsgDeliver::FromGroupToClient);

	RegQueryProcessor(QUERY_LoadMail, &CEntityMailComponent::OnDBLoadMail);
	RegQueryProcessor(QUERY_AddMail,  &CEntityMailComponent::OnDBSendMail);
	RegQueryProcessor(QUERY_MailGetRoleID,  &CEntityMailComponent::OnGetRoleID);
	
}

void CEntityMailComponent::OnUpdate(const unsigned long &nTimeElapsed)
{
#define UPDATE_INTERVAL 10
	unsigned int nTimeNow = (unsigned int)time(NULL);
	if (nTimeNow - m_nLastTick > UPDATE_INTERVAL) {
		std::list<__int64>  listMail = m_mapMailBox.GetKeyList();
		std::list<__int64>::iterator it = listMail.begin();
		while (it != listMail.end()){
			MailInfo **ppMail = m_mapMailBox.Find(*it);
			if (ppMail != NULL && *ppMail != NULL){
				MailInfo *pMail = *ppMail;
				if (pMail->m_nSendTime + MAX_MAIL_RESERVE_TIME < nTimeNow ) {
					AddQuery(QUERY_DeleteMail, nTimeNow, NULL, pMail->m_nMailID);
					m_mapMailBox.Remove(pMail->m_nMailID);
					if (!pMail->m_bRead) {
						DecreaseUnReadCount();
					}
					delete pMail, pMail = NULL;
				} else {
					break;
				}
			}
			it++;
		}

		m_nLastTick = nTimeNow;
	}
}

void CEntityMailComponent::OnLogin()
{
	SendUnReadMailMessage();
}

void CEntityMailComponent::DestoryMailMap(mapXQMail & mapXQBox)
{
    mapMail & mapBox = mapXQBox.GetMap();
    mapMail::iterator it =  mapBox.begin();
    while (it != mapBox.end())
    {
        MailInfo * pMail = it->second;
        if (pMail != NULL)
        {
            delete pMail;
        }
        it++;
    }
    mapXQBox.Clear();
}


void CEntityMailComponent::GetMailBox(GameMsg_Base & pMsg ,CSlotPeer & SlotPeer)
{
	GameMsg_C2S_GetMailBox * pMsgGetMailBox = (GameMsg_C2S_GetMailBox *)&pMsg;
	if (pMsgGetMailBox == NULL){
		return;
	}

	SendMailBox();
}

void CEntityMailComponent::SendMailBox()
{
    if ( m_pNetCpt == NULL )
        return;

//     unsigned short nPage = 0;
//    unsigned short nBeginIndex = 0 * MAILNUM_PER_PAGE;
//     unsigned short nCurIndex = 0;
    unsigned short nCount = 0;
    unsigned int nMaxPoolSize = MAX_SIZE_PER_MAILBASE * MAILNUM_PER_PAGE;
    CParamPool poolTemp( NULL, nMaxPoolSize );
    MailInfo *pFirtMail = NULL;
    MailBaseInfo *pMailBase = NULL;

    std::list<__int64> &rListMail = m_mapMailBox.GetKeyList();
    std::list<__int64>::iterator itr = rListMail.begin();
    while ( itr != rListMail.end() )
    {
//         if ( nCurIndex < 0)
//         {
//             ++nCurIndex;
//         }
//         else
        {
            pMailBase = *( m_mapMailBox.Find( *itr ) );

            if ( pMailBase != NULL )
            {
                pMailBase->MailBaseInfo::doEncode( poolTemp );

                pFirtMail = (MailInfo *)pMailBase;
                ++nCount;
            }
        }

        if ( nCount == MAILNUM_PER_PAGE )
        {
            break;
        }

        ++itr;
    }

    if ( pFirtMail != NULL )
    {
        ReadMail( pFirtMail );
        poolTemp.AddUChar( 1 );
        pFirtMail->doEncode( poolTemp );
    }
    else
    {
        poolTemp.AddUChar( 0 );
    }

    GameMsg_S2C_GetMailBox msgMail;
    msgMail.m_nCount = nCount;
    msgMail.SetPoolData( poolTemp.GetData(), poolTemp.GetValidDataSize() );

    m_pNetCpt->SendPlayerMsg( &msgMail );
}

void CEntityMailComponent::GetMailInfo(GameMsg_Base &msg ,CSlotPeer &slotPeer)
{
    GameMsg_C2S_GetMailInfo &rMsgGetMailInfo = dynamic_cast<GameMsg_C2S_GetMailInfo &>(msg);

    if (NULL == m_pNetCpt || 0 == rMsgGetMailInfo.m_nMailID)
        return;

    GameMsg_S2C_GetMailInfo msgMail;

    MailInfo **ppMail = m_mapMailBox.Find(rMsgGetMailInfo.m_nMailID);

    if (NULL != ppMail && NULL != *ppMail)
    {
        MailInfo *pMail = *ppMail;

        ReadMail(pMail);

        msgMail.m_nResult = MAIL_OK;
        msgMail.m_mailInfo = *pMail;
    }

    m_pNetCpt->SendPlayerMsg((GameMsg_Base *)&msgMail);
}

void CEntityMailComponent::DeleteMail(GameMsg_Base & pMsg ,CSlotPeer & SlotPeer)
{
	GameMsg_C2S_DeleteMail * pMsgDeleteMail = (GameMsg_C2S_DeleteMail *)&pMsg;
	if (m_pNetCpt == NULL || pMsgDeleteMail == NULL){
		return;
	}
	std::list<__int64>::iterator aBegin = pMsgDeleteMail->aListMailID.begin();
	while (aBegin != pMsgDeleteMail->aListMailID.end())
	{
		MailInfo **ppMail = m_mapMailBox.Find(*aBegin);
		if (ppMail != NULL) {
			MailInfo * pMail = *ppMail;
			AddQuery(QUERY_DeleteMail, (unsigned int)time(NULL), NULL, pMail->m_nMailID);
			m_mapMailBox.Remove(pMail->m_nMailID);
			if (!pMail->m_bRead) {
				DecreaseUnReadCount();
			}
			delete pMail;
			pMail = NULL;
		}
		aBegin++;
	}
	SendMailBox();
}


void CEntityMailComponent::GetMailItem(GameMsg_Base & pMsg ,CSlotPeer & SlotPeer)
{
	GameMsg_C2S_GetMailItem * pMsgGetMailItem = (GameMsg_C2S_GetMailItem *)&pMsg;
	if (m_pNetCpt == NULL || pMsgGetMailItem == NULL || pMsgGetMailItem->nMailID == 0){
		return;
	}
	GameMsg_S2C_DeleteMail msgMail;
	MailInfo ** pMail = (m_mapMailBox.Find(pMsgGetMailItem->nMailID));
	if (pMail != NULL && *pMail != NULL)
	{
		int nReturn = GetAllMailItem(*pMail);

		GameMsg_S2C_GetMailItem msgReturn;
		if (nReturn == MAIL_OK){
			msgReturn.nResult = MAIL_OK;
		} else {
			msgReturn.nResult = (unsigned char)nReturn;
			GetMailsErrorString(nReturn, msgReturn.m_strError);
		}
		m_pNetCpt->SendPlayerMsg((GameMsg_Base *)&msgReturn);
	}
	
}


// 发送邮件流程：
// 如果请求消息中没有roleID,则根据roleName 获取roleID.
// 如果roleID 是好友，直接发送，否则，去gorupServer 验证能否发送（是否设置拒收陌生人消息）
// group server 返回消息，是否能发，GameServer 执行发送

void CEntityMailComponent::SendMail(GameMsg_Base & pMsg ,CSlotPeer & SlotPeer)
{
    GameMsg_C2S_SendMail *pMsgSendMail = (GameMsg_C2S_SendMail *)&pMsg;

    if (m_pNetCpt == NULL || pMsgSendMail == NULL)
    {
        return;
    }

    if (strlen(pMsgSendMail->m_szReceiverName) == 0 || 
        pMsgSendMail->GetTitle().size() == 0 || 
        pMsgSendMail->GetContent().size() == 0)
    {
        return;
    }

	if (pMsgSendMail->m_nReceiverID > 0)
	{
        if (m_pRoleFriend->IsFriend(pMsgSendMail->m_nReceiverID))
        { // 好友直接发送
            SendMailToPlayer(pMsgSendMail->m_nReceiverID, EMailType_Player_Send_Mail, pMsgSendMail->GetTitle().c_str(), pMsgSendMail->GetContent().c_str(), 0, 0, 0, 0);

            GameMsg_S2C_SendMail msg;
            msg.nResult = MAIL_OK;

            ((CRoleEntity*)Entity())->SendPlayerMsg(&msg);
        }
        else
        { // 请求group server.
            GameMsg_S2G_ValidSendMail req;
            req.m_mailInfoDB.Set(pMsgSendMail->m_nReceiverID, pMsgSendMail->m_szReceiverName, pMsgSendMail->GetTitle(), pMsgSendMail->GetContent());
            ((CRoleEntity*)Entity())->SendMsg2GroupServer(&req); // 去group验证陌生人
        }
	}
	else
    {
        SharedPtr<MailInfoDB> pMailDB(new MailInfoDB(pMsgSendMail->m_nReceiverID, pMsgSendMail->m_szReceiverName, pMsgSendMail->GetTitle(), pMsgSendMail->GetContent()));

        AddQuery(QUERY_MailGetRoleID, pMailDB);
    }
}

void CEntityMailComponent::OnGetRoleID(QueryBase & pQuery)
{
    Query<MailInfoDB> &rQuery = (Query<MailInfoDB> &)pQuery;
    MailInfoDB * pMailDB = NULL;
    if (rQuery.IsValid())
        pMailDB = rQuery.Get();

    GameMsg_S2C_SendMail msg;
    do
    {
        if (NULL == pMailDB || pMailDB->m_nReceiverID == 0)
        {
            msg.nMsg = MAIL_UNKNOWN;
            break;
        }

        if (!m_pRoleFriend->IsFriend(pMailDB->m_nReceiverID))
        { // 不是好友，直接发往group请求，
            // 转发到group去验证
            GameMsg_S2G_ValidSendMail req;
            req.m_mailInfoDB.Set(pMailDB->m_nReceiverID, pMailDB->m_szReceiverName, pMailDB->GetTitle(), pMailDB->GetContent());
            ((CRoleEntity*)Entity())->SendMsg2GroupServer(&req);

            return; // 此时不回复客户端
        }

        SendMailToPlayer(pMailDB->m_nReceiverID, EMailType_Player_Send_Mail, pMailDB->GetTitle().c_str(), pMailDB->GetContent().c_str(),
            pMailDB->m_nMoney, pMailDB->m_nItemType, pMailDB->m_nCount, pMailDB->m_nValidTime);

        msg.nResult = MAIL_OK;

    } while (0);


    ((CRoleEntity*)Entity())->SendPlayerMsg(&msg);
}

void CEntityMailComponent::OnValidSendMail(GameMsg_Base & pMsg, CSlotPeer & SlotPeer)
{
    GameMsg_G2S_ValidSendMail &msg = (GameMsg_G2S_ValidSendMail &)pMsg;

    GameMsg_S2C_SendMail retMsg;
    
    do 
    {
        if (msg.m_nRes != MAIL_OK)
        {
            retMsg.nResult = msg.m_nRes;
            break;
        }

        SendMailToPlayer(msg.m_mailInfoDB.m_nReceiverID, EMailType_Player_Send_Mail, msg.m_mailInfoDB.GetTitle().c_str(), msg.m_mailInfoDB.GetContent().c_str(),
            msg.m_mailInfoDB.m_nMoney, msg.m_mailInfoDB.m_nItemType, msg.m_mailInfoDB.m_nCount, msg.m_mailInfoDB.m_nValidTime);

        retMsg.nResult = MAIL_OK;
    } while (0);

    ((CRoleEntity*)Entity())->SendPlayerMsg(&retMsg);

    return;
}


void CEntityMailComponent::SendMailToPlayer(unsigned int nReceiverID,EMailType nType,const char * pszTitle,const char * pszContent,int nMoney,itemtype_t nItemType,unsigned short nCount,int nValidTime)
{
    if (nReceiverID > 0 && pszTitle != NULL && strlen(pszTitle) > 0 && pszContent != NULL)
    {
        MailInfo * pMail = new MailInfo();
        SharedPtr<MailInfo> sharedMail( pMail );
        if (pMail == NULL || m_pRoleAttr == NULL)
        {
            return;
        }

        std::string strMailTitle( pszTitle );
        NameDeny::Instance().FilterInvalidText( strMailTitle );
        pMail->SetTitle( strMailTitle );

        std::string strMailContent( pszContent );
        NameDeny::Instance().FilterInvalidText( strMailContent );
        pMail->SetContent( strMailContent );

        pMail->m_nSenderID = m_pRoleAttr->GetRoleID();
        pMail->m_nType = (short)nType;
        pMail->SetSenderName(m_pRoleAttr->GetRoleName() );
        pMail->m_nSendTime = (unsigned int)time(NULL);
        pMail->m_bRead = false;
        pMail->m_nMoney = nMoney;
        pMail->m_nBindMCoin = 0; // 绑币不能交易
        pMail->m_nHasItem = (nItemType > 0 && nCount > 0) ? 1 : 0;
        if ( pMail->m_nHasItem == 1 )
        {
            MailItem aItem(nItemType,nCount,nValidTime);
            pMail->m_mapItems.insert(mapMailItems::value_type(nItemType,aItem) );
        }

        AddQuery( QUERY_AddMail, sharedMail, nReceiverID );
    }
}

void CEntityMailComponent::DeleteWeddingRingMail()
{
	std::list<__int64> & listMail = m_mapMailBox.GetKeyList();
	std::list<__int64> tempListMail = listMail;
	std::list<__int64>::iterator it = tempListMail.begin();
	for ( ; it != tempListMail.end(); ++it )
	{
		MailInfo ** pMail = (m_mapMailBox.Find(*it));
		if ( pMail != NULL && *pMail != NULL )
		{
			if ( !(*pMail)->m_mapItems.empty() )
			{
				mapMailItems tempItems = (*pMail)->m_mapItems;
				mapMailItems::iterator it_item = tempItems.begin();
				for ( ; it_item != tempItems.end(); ++it_item )
				{
					ItemConfig* pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID( it_item->second.m_nType );
					if ( pItemInfo != NULL && pItemInfo->IsBadge_Type_WeddingRing() )
					{
						AddQuery(QUERY_DeleteMail, (unsigned int)time(NULL), NULL, *it);
						if (!(*pMail)->m_bRead) 
						{
							DecreaseUnReadCount();
						}

						m_mapMailBox.Remove( (*pMail)->m_nMailID );
					}
				}
			}
		}
	}
}

void CEntityMailComponent::OnDBLoadMail(QueryBase & pQuery)
{
	if (m_pNetCpt == NULL){
		return;
	}

	//第一次发送收件箱信息
	int nTag = pQuery.m_nAttachment;
	if (nTag == MAIL_NOTICE_CLIENT)
	{
		SendMailBox();
	}
}


void CEntityMailComponent::OnDBSendMail(QueryBase & pQuery)
{
    if ( m_pNetCpt == NULL )
        return;

    Query<MailInfo> *pTmp = dynamic_cast<Query<MailInfo> *>( &pQuery );
    if ( pTmp == NULL || pTmp->m_nResult != Query_Res_OK || !pTmp->IsValid() )
        return;

    // 通知组服务器
    GameMsg_S2G_SendMail msg;
    msg.nNoticeRoleID = (unsigned int)pTmp->m_nAttachment;
    msg.m_mailInfo = pTmp->GetRef();
    m_pNetCpt->Send2GroupServer( &msg );
}


void CEntityMailComponent::OnReceiveMail(GameMsg_Base & pMsg ,CSlotPeer & SlotPeer)
{
	GameMsg_G2S_SendMail * pMsgReceiveMail = (GameMsg_G2S_SendMail *)&pMsg;
	if (m_pNetCpt == NULL || pMsgReceiveMail == NULL)
	{
		return;
	}

	if ( m_pRoleFriend->IsInBlackList( pMsgReceiveMail->m_mailInfo.m_nSenderID))
	{
		return;
	}

	MailInfo *pMail = new MailInfo;
	if (pMail != NULL) 
    {
		*pMail = pMsgReceiveMail->m_mailInfo;

		m_mapMailBox.Add2(pMsgReceiveMail->m_mailInfo.m_nMailID, pMail);

		if (!pMail->m_bRead)
		{
			IncreaseUnReadCount();
		}

		//通知客户端有新邮件啦！！
		GameMsg_S2C_ReceiveMail msg;
		msg.nMailID = pMsgReceiveMail->m_mailInfo.m_nMailID;
		m_pNetCpt->SendPlayerMsg((GameMsg_Base *)&msg);

		SendUnReadMailMessage();
	}

}

int CEntityMailComponent::MergeMailItem(mapMailItems & mapItems,MailItem & Item)
{
	if (Item.m_nType == 0 || Item.m_nCount <= 0){
		return MAIL_PARAM_ERROR;
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
	return MAIL_OK;
}

int CEntityMailComponent::GetMailMoney(MailInfo * pMail)
{
    if ( pMail == NULL || m_pRoleAttr == NULL )
        return MAIL_PARAM_ERROR;
    else if ( !HasMoneyInMail( pMail ) )
        return MAIL_NOMONEY_GET;

    if ( pMail->m_nMoney > 0 )
        m_pRoleAttr->ChangeMoney( pMail->m_nMoney, EChangeMoneyCause_Add_Mail, 0 );
    if ( pMail->m_nBindMCoin > 0 )
        m_pRoleAttr->ChangeBindBill( pMail->m_nBindMCoin, EChangeBindBillCause_Add_Mail );

    pMail->m_nMoney = pMail->m_nBindMCoin = 0;
    AddQuery( QUERY_GetMailMoney, 0, NULL, pMail->m_nMailID );

    return MAIL_OK;
}

int CEntityMailComponent::GetAllMailItem(MailInfo * pMail)
{
	if (pMail == NULL || pMail->m_nMailID <= 0)
	{
		return MAIL_PARAM_ERROR;
	}

    bool bHasMoney = HasMoneyInMail( pMail );

	if (!bHasMoney && (pMail->m_nHasItem == 0 || pMail->m_mapItems.size() == 0))
	{
		return MAIL_NOITEM_GET;
	}

	int nReturn = MAIL_OK;
	if (pMail->m_nHasItem != 0 && pMail->m_mapItems.size() == 1)
	{
		nReturn = CanAddItem(pMail->m_mapItems);
		if (nReturn != MAIL_OK) {
			return nReturn;
		}
		if (nReturn == MAIL_OK){
			nReturn = AddItem(pMail->m_mapItems, pMail->m_nMailID);
			if (nReturn == MAIL_OK){
				pMail->m_nHasItem = 0;
				pMail->m_mapItems.clear();
				AddQuery(QUERY_GetAllMailItem,0,NULL,pMail->m_nMailID);
			}
		}
	}

    if ( nReturn == MAIL_OK && bHasMoney )
    {
        nReturn = GetMailMoney( pMail );
    }

	return nReturn;
}

int CEntityMailComponent::CanAddItem(mapMailItems & mapItems)
{
	if (mapItems.size() == 0 || m_pRoleItem == NULL)
	{
		return MAIL_NOITEM_GET;
	}

	mapMailItems::iterator it = mapItems.begin();
	while (it != mapItems.end())
	{
		itemtype_t nItemType = it->first;
		MailItem & aItem = it->second;

		if (aItem.m_nState != 0){
			return MAIL_ITEM_HASGET;
		}
		if (aItem.m_nType == C_ITEM_COMMON_RING) {
			if (m_pRoleCouple != NULL && m_pRoleItem != NULL) {
				if (!(m_pRoleCouple->IsMarried() && (m_pRoleItem->GetItemCount(aItem.m_nType, true, true) == 0)) ) {
					return MAIL_ITEM_NotQualifiedToGetDefaultRing;
				}
			}
		}

		unsigned short nStackNumber = 0;
		//首先物品要能添加
		int nRes = m_pRoleItem->CanAddItem(nItemType,aItem.m_nCount,aItem.m_nValidTime,nStackNumber);
		if (nRes != EItemError_Success) {
			return nRes;
		}
		it++;
	}
	return MAIL_OK;
}

int CEntityMailComponent::AddItem(mapMailItems & mapItems, __int64 nMailID)
{
	if (m_pRoleItem == NULL || mapItems.size() == 0)
    {
		return MAIL_NOITEM_GET;
	}

	mapMailItems::iterator it = mapItems.begin();
	while (it != mapItems.end())
	{
	    itemtype_t nItemType = it->first;
		MailItem & aItem = it->second;

		m_pRoleItem->AddItem(nItemType,aItem.m_nCount,aItem.m_nValidTime,EItemAction_Add_Mail,nMailID,true,false,aItem.m_nDynamicBoxRelatedValue);
		aItem.m_nState = 1;
		it++;
	}
	return MAIL_OK;
}

void CEntityMailComponent::ReadMail(MailInfo * pMail)
{
	if(pMail != NULL && !pMail->m_bRead)
    {
		pMail->m_bRead = true;
		DecreaseUnReadCount();
		AddQuery(QUERY_ReadMail,0,NULL,pMail->m_nMailID);
	}
}

void CEntityMailComponent::GetMailsErrorString(int nErrno, string &strError)
{
	switch(nErrno)
	{
	case MAIL_PARAM_ERROR:
		{
			strError = "MAIL_PARAM_ERROR";
		}
		break;
	case MAIL_NORECEIVER:
		{
			strError = "MAIL_NORECEIVER";
		}
		break;
	case MAIL_MONEY_NOTENOUGH:
		{
			strError = "MAIL_MONEY_NOTENOUGH";
		}
		break;
	case MAIL_NOITEM_GET:
		{
			strError = "MAIL_NOITEM_GET";
		}
		break;
	case MAIL_NOMONEY_GET:
		{
			strError = "MAIL_NOMONEY_GET";
		}
		break;
	case MAIL_ITEMGRID_ITEMFULL:
		{
			strError = "MAIL_ITEMGRID_ITEMFULL";
		}
		break;
	case MAIL_ITEM_UNIQUEEXIST:
		{
			strError = "MAIL_ITEM_UNIQUEEXIST";
		}
		break;
	case MAIL_ITEM_HASGET:
		{
			strError = "MAIL_ITEM_HASGET";
		}
		break;
	case MAIL_ITEM_HASNOTGET:
		{
			strError = "MAIL_ITEM_HASNOTGET";
		}
		break;
	case MAIL_UNKNOWN:
		{
			strError = "MAIL_UNKNOWN";
		}
		break;
	case MAIL_FAIL:
		{
			strError = "MAIL_FAIL";
		}
	case MAIL_ITEM_NotQualifiedToGetDefaultRing:
		{
			strError = "MAIL_ITEM_NotQualifiedToGetDefaultRing";
		}
		break;
	default:
		;
	}
}

void CEntityMailComponent::IncreaseUnReadCount(int n)
{
	if (n <= 0)
		return;

	m_nUnReadCount = m_nUnReadCount + n;

	if (m_nUnReadCount < 0)
		m_nUnReadCount = 0;
}

void CEntityMailComponent::DecreaseUnReadCount(int n)
{
	if (n <= 0)
		return;

	m_nUnReadCount = m_nUnReadCount - n;

	if (m_nUnReadCount < 0)
		m_nUnReadCount = 0;
}

void CEntityMailComponent::SendUnReadMailMessage()
{
	if (m_nUnReadCount > 0)
	{
		GameMsg_S2C_UnReadMail notifyMsg;
		notifyMsg.m_nUnReadCount = (unsigned short)m_nUnReadCount;
		m_pNetCpt->SendPlayerMsg(&notifyMsg);
		m_nUnReadCount = 0;
	}
}

bool CEntityMailComponent::HasMoneyInMail( const MailInfo *pMail ) const
{
    if ( pMail == NULL )
        return false;
    else if ( pMail->m_nMoney <= 0 && pMail->m_nBindMCoin <= 0 )
        return false;

    return true;
}

