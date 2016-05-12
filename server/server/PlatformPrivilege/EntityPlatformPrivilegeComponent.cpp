#include "EntityPlatformPrivilegeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../Message/PlatformPrivilege/UCVIP/GameMsg_UCVIP.h"
#include "../../socket/GameMsg_Map.h"
#include "../activity/PlatformPrivilegeDataMgr.h"
#include "../item/ItemProcess.h"
#include "UCVIP/UCVIP.h"

CEntityPlatformPrivilege::CEntityPlatformPrivilege()
:CEntityComponent( true, true )
{
}

CEntityPlatformPrivilege::~CEntityPlatformPrivilege()
{
}

void CEntityPlatformPrivilege::Start()
{
	this->m_pRoleNet = this->GetComponent<CRoleNet>();
	this->m_pRoleAttribute = this->GetComponent<CRoleAttribute>();
}

void CEntityPlatformPrivilege::RegComponentNetMsgMap()
{
	GAMEMSG_REGISTERCREATOR( GameMsg_C2S_GetUCVIPRewardStatue );
	GAMEMSG_REGISTERCREATOR( GameMsg_C2S_GainUCVIPReward );
	GAMEMSG_REGISTERCREATOR( GameMsg_C2S_GetUCVIPRewardDetial );

	RegMsgProcessor( MSG_C2S_GetUCVIPRewardStatue, &CEntityPlatformPrivilege::_OnGetUCVIPRewardStatue );
	RegMsgProcessor( MSG_C2S_GetUCVIPRewardDetail, &CEntityPlatformPrivilege::_OnGetUCVIPRewardDetial );
	RegMsgProcessor( MSG_C2S_GainUCVIPReward, &CEntityPlatformPrivilege::_OnGainUCVIPReward );
}

bool CEntityPlatformPrivilege::CreateFromDB(ROLEINFO_DB* pRoleInfoDB )
{
	if( pRoleInfoDB != NULL )
	{
		this->m_setUCVIPReward = pRoleInfoDB->setUCVIPReward;

		return true;
	}

	return false;
}

bool CEntityPlatformPrivilege::PacketToDB(ROLEINFO_DB*pRoleInfoDB) const
{
	return true;
}

void CEntityPlatformPrivilege::SerializeComponent(CParamPool &IOBuff)
{
	
}

void CEntityPlatformPrivilege::OnLogin()
{

}

void CEntityPlatformPrivilege::OnLogout()
{
	
}

void CEntityPlatformPrivilege::OnUpdate(const unsigned long & nTimeElapsed)
{

}

void CEntityPlatformPrivilege::OnEvent(CComponentEvent& refEvent )
{

}


void CEntityPlatformPrivilege::_OnGetUCVIPRewardStatue( GameMsg_Base & rMsg, CSlotPeer & rSlotPeer )
{
	GameMsg_C2S_GetUCVIPRewardStatue *pMsg = ( GameMsg_C2S_GetUCVIPRewardStatue *)&rMsg;

	this->m_ucvipInfo.Reset();
	this->m_ucvipInfo.nUCVIPLevel = pMsg->m_nUCVIPLevel;

	if( this->m_pRoleAttribute == NULL )
	{
		//strError = "Server_error";
		//nErrorCode = 3;
		return ;
	}
	std::string strError = "";
	//int nErrorCode = 0;
	if( !CPlatformPrivilegeDataMgr::Instance().IsOpen() )
	{
		// "UCVIPReward_Closed"平台功能没有开启
		strError = "UCVIP_function_has_been_turned_off";
		//nErrorCode = 1;
	}
// 	else if( pMsg->m_nUCVIPLevel < 0 )
// 	{
// 		// "No UCVIP Privilege"无UCVIP权限
// 		strError = "Privilege_reward_yet_open";
// 		//nErrorCode = 2;
// 	}
	else if( pMsg->m_nUCVIPLevel == 0 )
	{
		return ;//不做任何处理
	}


	if( strError.empty() )
	{
		//this->m_ucvipInfo.nUCVIPLevel = pMsg->m_nUCVIPLevel;

		//检查当前权限可领取的状态 todo
		GameMsg_S2C_GetUCVIPRewardStatueSuc msgSuc;

		//判断权限状态 （1，可领取，2已领取，0不可领取）
		std::map<int, CPlatofromPrivilegeReward> &tempRewardList = CPlatformPrivilegeDataMgr::Instance().GetPlatformUCRewardList( this->m_pRoleAttribute->GetSex() );
		std::map<int, CPlatofromPrivilegeReward>::iterator iter = tempRewardList.begin(), iterEnd = tempRewardList.end();
		for( ; iter != iterEnd; ++iter )
		{
			CPlatofromPrivilegeReward tempReward = iter->second;
			//没有领取过
			if( this->m_setUCVIPReward.find( tempReward.nVipIndex ) == this->m_setUCVIPReward.end() )
			{
				//等级达到
				if( this->m_ucvipInfo.nUCVIPLevel >= tempReward.nLowLevel )
					//&& this->m_ucvipInfo.nUCVIPLevel <= tempReward->nHighLevel )
				{
					switch( tempReward.nVipIndex )
					{
						/*
					case 2://第2项特殊判定
						//判断是否完成了首次充值
						if( this->m_pRoleAttribute->m_bCompleteFirstPaymentTag )
							msgSuc.mapRewardState[ tempReward.nVipIndex ] = 1;
						else
							msgSuc.mapRewardState[ tempReward.nVipIndex ] = 0;
						break;
						*/
					case 2:
					default:
						msgSuc.mapRewardState[ tempReward.nVipIndex ] = 1;
						break;
					}
				}
				else
				{
					msgSuc.mapRewardState[ tempReward.nVipIndex ] = 0;
				}
			}//end if
			else
			{
				msgSuc.mapRewardState[ tempReward.nVipIndex ] = 2;
			}
		}//end for
		this->_SendPlayerMsg( &msgSuc );
	}
	else
	{
		GameMsg_S2C_GetUCVIPRewardStatueFail msgFail;
		//msgFail.m_nErrorCode = (unsigned char)nErrorCode; //返回错误码
		msgFail.strError = strError;
		this->_SendPlayerMsg( &msgFail );
	}
}

void CEntityPlatformPrivilege::_OnGetUCVIPRewardDetial( GameMsg_Base &rMsg, CSlotPeer & rSlotPeer )
{
	GameMsg_C2S_GetUCVIPRewardDetial *pMsg = ( GameMsg_C2S_GetUCVIPRewardDetial *)&rMsg;

	std::string strError = "";
	if( !CPlatformPrivilegeDataMgr::Instance().IsOpen() )
	{
		strError = "UCVIP_function_has_been_turned_off";
		//nErrorCode = 1; //平台功能没有开启
	}
	else if( pMsg->m_nRewardIndex <= 0 || pMsg->m_nRewardIndex > MAX_UCVIP_PRIVILEGE_NUM )
	{
		strError = "Illegal_Information";
		//nErrorCode = 2; //非法特权编号
	}

	if( strError.empty() )
	{
		if( this->m_pRoleAttribute != NULL )
		{
			CPlatofromPrivilegeReward * pReward = CPlatformPrivilegeDataMgr::Instance().GetPlatoformUCReward( pMsg->m_nRewardIndex, this->m_pRoleAttribute->GetSex() ); 
			if( pReward != NULL )
			{
				GameMsg_S2C_GetUCVIPRewardDetialSuc msgSuc( pReward->listItem, pReward->nLowLevel, pMsg->m_nRewardIndex );
				this->_SendPlayerMsg( &msgSuc );
			}
		}
	}
	else
	{
		GameMsg_S2C_GetUCVIPRewardDetialFail msgFail;
		//msgFail.m_nErrorCode = nErrorCode;
		msgFail.strError = strError;
		this->_SendPlayerMsg(&msgFail );
	}
}

void CEntityPlatformPrivilege::_OnGainUCVIPReward( GameMsg_Base & rMsg, CSlotPeer & rSlotPeer )
{
	GameMsg_C2S_GainUCVIPReward *pMsg = ( GameMsg_C2S_GainUCVIPReward *)&rMsg;

	std::string strError = "";
	//int nErrorCode = 0;
	if( !CPlatformPrivilegeDataMgr::Instance().IsOpen() )
	{
		strError = "UCVIP_function_has_been_turned_off";
		//nErrorCode = 1; //平台功能没有开启
	}
	else if( pMsg->m_nRewardIndex <= 0 || pMsg->m_nRewardIndex > MAX_UCVIP_PRIVILEGE_NUM )
	{
		strError = "Illegal_Information";
		//nErrorCode = 2; //领取的特权ID不存在
	}
	else if( this->m_setUCVIPReward.find( pMsg->m_nRewardIndex ) != this->m_setUCVIPReward.end() )
	{
		strError = "You_have_Received_this_ward_over";
		//nErrorCode = 3; //已经领取过
	}
	else
	{
		 //判断是否是可以领取的状态
		CPlatofromPrivilegeReward * pReward = CPlatformPrivilegeDataMgr::Instance().GetPlatoformUCReward( pMsg->m_nRewardIndex, this->m_pRoleAttribute->GetSex() ); 
		//存在奖励并且没有领取过
		if( pReward != NULL 
			&& this->m_setUCVIPReward.find( pMsg->m_nRewardIndex ) == this->m_setUCVIPReward.end())
		{
			if( this->m_ucvipInfo.nUCVIPLevel >= pReward->nLowLevel )
				//&& this->m_ucvipInfo.nUCVIPLevel <= pReward->nHighLevel )
			{
				switch( pMsg->m_nRewardIndex )
				{
					/*
				case 2:
					if( this->m_pRoleAttribute->m_bCompleteFirstPaymentTag )
					{
						strError = "";
						//nErrorCode = 0；
					}
					else
					{
						strError = "Did_not_complete_the_first_recharge_can_not_receive_awards";
						//nErrorCode = 5; //请先完成首次充值
					}
					break;
					*/
				case 2:
				default:
					//nErrorCode = 0；
					strError = "";
					break;
				}
			}
			else
			{
				strError = "Privilege_level_not_reached_can_not_receive_awards";
				//nErrorCode = 4; //等级未达到
			}
		}
	}

	if( strError.empty() )
	{
		if( this->m_pRoleAttribute != NULL )
		{
			CPlatofromPrivilegeReward * pReward = CPlatformPrivilegeDataMgr::Instance().GetPlatoformUCReward( pMsg->m_nRewardIndex, this->m_pRoleAttribute->GetSex() ); 
			if( pReward != NULL )
			{
				std::string& strMailTitle = CPlatformPrivilegeDataMgr::Instance().MaileTitle();
				std::string& strMailContent = CPlatformPrivilegeDataMgr::Instance().MailContent();
				//发送邮件
				CItemProcess::AddOrMailItems(*(CRoleEntity*)(this->Entity()), pReward->listItem, 
					EItemAction_Add_GainPlatformPrivilegeReward, 0, true, 
					EMailType_GainPlatformPrivilegeReward, strMailTitle, strMailContent );
				//存入数据库中
				this->AddQuery( Query_PlatformPrivilege_AddReward, this->m_pRoleAttribute->m_nRoleID, NULL, pMsg->m_nRewardIndex, NULL ); 
				this->m_setUCVIPReward.insert( pMsg->m_nRewardIndex );

				GameMsg_S2C_GainUCVIPRewardSuc msgSuc;
				msgSuc.m_nRewardInex = pMsg->m_nRewardIndex;
				this->_SendPlayerMsg( &msgSuc );
			}
		}
	}
	else 
	{
		GameMsg_S2C_GainUCVIPRewardFail msgFail;
		//msgFail.m_nErrorCode = (unsigned char)nErrorCode;//返回错误码
		msgFail.strError = strError;
		this->_SendPlayerMsg( &msgFail );
	}
}

void CEntityPlatformPrivilege::_SendPlayerMsg( GameMsg_Base * pMsg )
{
	if( this->GetRoleNet() != NULL && pMsg != NULL )
	{
		this->GetRoleNet()->SendPlayerMsg( pMsg );
	}
}
