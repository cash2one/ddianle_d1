#include "CompensationComponent.h"
#include "CompensationManager.h"
#include "GameMsg_S2C_NewCompensation.h"
#include "../item/EntityItemComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../mail/EntityMailComponent.h"
#include "../../socket/GameMsg_Map.h"
#include "../../datastructure/ItemErrorInfo.h"
#include <algorithm>
#include "../logic/LogicCirculator.h"
#include "../item/ItemProcess.h"

CCompensationComponent::CCompensationComponent()
:CEntityComponent(true,true)
,m_pItemCom(NULL)
,m_pNetCom(NULL)
,m_pRoleCom(NULL)
,m_pMailCom(NULL)
{
}

CCompensationComponent::~CCompensationComponent()
{
}

void CCompensationComponent::Start()
{
	m_pItemCom = GetComponent<CRoleItem>();
	m_pNetCom = GetComponent<CRoleNet>();
	m_pRoleCom = GetComponent<CRoleAttribute>();
	m_pMailCom = GetComponent<CRoleMail>();
}

void CCompensationComponent::OnLogin()
{
	NoticeNextCompensation();
}

bool CCompensationComponent::CreateFromDB(ROLEINFO_DB* pRoleInfoDB)
{
	for ( std::vector<int>::iterator it = pRoleInfoDB->m_CompensationReward.begin(); it != pRoleInfoDB->m_CompensationReward.end(); ++it )
	{
		m_setCompensation.insert( *it );
	}
	
	return true;
}

bool CCompensationComponent::PacketToCache( ROLEINFO_DB* pRoleInforDB ) const
{
    if ( pRoleInforDB == NULL )
        return false;

    pRoleInforDB->m_CompensationReward.assign( m_setCompensation.begin(), m_setCompensation.end() );

    return true;
}

void CCompensationComponent::RegComponentNetMsgMap()
{
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetCompensation);

	RegMsgProcessor(MSG_C2S_GetCompensation,&CCompensationComponent::OnGetCompensation);
	RegQueryProcessor(Query_InsertReceiveCompensation,&CCompensationComponent::OnInsertRoleCompensation);
}

int CCompensationComponent::NextCompensationID()
{
	int nFindID = INVALID_COMPESSATION_ID;
	int nTimeNow = time( NULL );

	CompensationMgr& cMgr = CompensationMgr::Instance();
	for (std::map<int,CompensationInfo>::iterator it = cMgr.m_CompensationMap.begin(); it!=cMgr.m_CompensationMap.end(); ++it)
	{
		if ( it->second.m_nEndTime > nTimeNow && m_setCompensation.find( it->first ) == m_setCompensation.end())
		{
			nFindID = it->first;
			break;
		}
	}

	return nFindID;
}

void CCompensationComponent::NoticeNextCompensation()
{
	int nFindID = NextCompensationID();
	if (nFindID != INVALID_COMPESSATION_ID)
	{
		GameMsg_S2C_NewCompensation noticeMsg;
		m_pNetCom->SendPlayerMsg( &noticeMsg );
	}
}

void CCompensationComponent::OnGetCompensation(GameMsg_Base& msg,CSlotPeer& slot)
{
	//取得一条可以领取的补偿ID
	int nFindID = NextCompensationID();
	if (nFindID != INVALID_COMPESSATION_ID)
	{
		m_setCompensation.insert(nFindID);
		m_pRoleCom->AddQuery(Query_InsertReceiveCompensation,nFindID,NULL,m_pRoleCom->m_nRoleID,NULL);
	}
	else
	{
		GameMsg_S2C_GetCompensationFail resMsg;
		resMsg.m_strError = "GetCompensation_Invalid";
		m_pNetCom->SendPlayerMsg( &resMsg );
	}
}

void CCompensationComponent::OnInsertRoleCompensation(QueryBase& query)
{
	if (query.m_nResult == Query_Res_OK)
	{
		int nCompensationID = query.m_nAttachment;
		CompensationMgr& cMgr = CompensationMgr::Instance();
		if (cMgr.m_CompensationMap.find(nCompensationID) != cMgr.m_CompensationMap.end())
		{
			CompensationInfo& cInfo = cMgr.m_CompensationMap[nCompensationID];
			std::list<CItem>& itemReward = (m_pRoleCom->m_nSex == ESexType_Male) ? cInfo.m_MaleItems : cInfo.m_FemaleItems;
			CItemProcess::AddOrMailItems(*(CRoleEntity *)Entity(), itemReward, EItemAction_Add_Compensation, 0, true, EMailType_Compensation, cInfo.m_strTitle, cInfo.m_strContent);
			m_pRoleCom->ChangeMoney(cInfo.m_nMoney, EChangeMoneyCause_Compensation, 0);

			//通知客户端玩家领取成功
			GameMsg_S2C_GetCompensationSuc resMsg;
			resMsg.m_nMoney = cInfo.m_nMoney;
			resMsg.m_strTitle = cInfo.m_strTitle;
			resMsg.m_strContent = cInfo.m_strContent;
			resMsg.m_Item = itemReward;
			m_pNetCom->SendPlayerMsg( &resMsg );
		}
	}

	NoticeNextCompensation();
}

