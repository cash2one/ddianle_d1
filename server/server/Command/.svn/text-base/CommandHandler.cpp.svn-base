#include <stdio.h>
#include "CommandHandler.h"
#include "../quest/EntityQuestComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../item/EntityItemComponent.h"
#include "../room/RoomComponent.h"
#include "../room/RoomManager.h"
#include "../AmuseRoom/AmuseRoomManager.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../vip/EntityVIPComponent.h"
#include "../vip/VIPDataMgr.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../../datastructure/LogicInitData.h"
#include "../../datastructure/ItemErrorInfo.h"
//#include "../../liblog/LogType.h"
#include "../../socket/Formatter.h"
#include "../../datastructure/Localization.h"
#include "../chat/AdminChat.h"
#include "../activity/EntityActivityComponent.h"
#include "../PhotoRoom/EntityPhotoRoomComponent.h"
#include "../bet/BetComponent.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../SystemSetting/SystemSettingMgr.h"
#include "../room/LiveRoomManager.h"
#include "../quest/EntityQuestNewComponent.h"
#include "../Dungeon/EntityDungeonComponent.h"
#include "../CheckIn/EntityCheckInComponent.h"
#include "../activeness/ActivenessComponent.h"
#include "../roomLib/RoomSettingMgr.h"
#include "../Medal/EntityMedalComponent.h"
#include "../Medal/MedalCfgMgr.h"
/*************************************************************/
/*				    添加任务：addquest						*/
/***********************************************************/

CCmdAddQusetHandler::CCmdAddQusetHandler()
{
}

CCmdAddQusetHandler::~CCmdAddQusetHandler()
{
}

string CCmdAddQusetHandler::Cmd()const
{
	return "addquest";
}

ICmdHandler *CCmdAddQusetHandler::CreateCmdHandler()
{
	return new CCmdAddQusetHandler;
}

void CCmdAddQusetHandler::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{
	CRoleChat *roleChat = (CRoleChat *)&cmp;
	if (roleChat->GetRoleQuest() != NULL) 
    {
		if (roleChat->GetRoleQuestNew()->AddQuestByCmdPersistent((unsigned short)nPara1)) 
        {
			roleChat->SysChat("add quest %d sucess",nPara1);
		}
        else
        {
            roleChat->SysChat("add quest %d failer", nPara1);
        }
	}
}

/*************************************************************/
/*				    完成任务finquest						*/
/***********************************************************/
string CCmdFinQuestHandler::Cmd()const
{
	return "finquest";
}

ICmdHandler* CCmdFinQuestHandler::CreateCmdHandler()
{

	return NULL;
}

void CCmdFinQuestHandler::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{
	CRoleChat *roleChat = (CRoleChat *)&cmp;
	CEntityQuestNewComponent* pQuestCom = roleChat->GetRoleQuestNew();
	if (pQuestCom != NULL)
	{
		pQuestCom->CompleteQuestByCmd((unsigned short)nPara1);
	}
}

CCmdFinQuestHandler::CCmdFinQuestHandler()
{
}

CCmdFinQuestHandler::~CCmdFinQuestHandler()
{
}

/*************************************************************/
/*				    获得accountID							*/
/***********************************************************/
CCmdAccountID::CCmdAccountID()
{
}

CCmdAccountID::~CCmdAccountID()
{
}

string CCmdAccountID::Cmd()const	
{
	return "accountid";
}

ICmdHandler *CCmdAccountID::CreateCmdHandler()
{
	return new CCmdAccountID;
}

void CCmdAccountID::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;
	CEntityAttributeComponent *pRoleAttr = pRoleChat->GetRoleAttr();

	if (pRoleAttr != NULL) {
		pRoleChat->SysChat("accoutid = %d", pRoleAttr->GetAccount());
	}
}

/*************************************************************/
/*				    获得roleID								*/
/***********************************************************/
CCmdRoleID::CCmdRoleID()
{
}

CCmdRoleID::~CCmdRoleID()
{
}

string CCmdRoleID::Cmd()const
{
	return "roleid";
}

ICmdHandler *CCmdRoleID::CreateCmdHandler()
{
	return new CCmdRoleID;
}

void CCmdRoleID::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;
	CEntityAttributeComponent *pRoleAttr = pRoleChat->GetRoleAttr();

	if (pRoleAttr != NULL) {
		pRoleChat->SysChat("roleid = %d", pRoleAttr->GetRoleID());
	}
}

/*************************************************************/
/*				   设置经验addexp							*/
/***********************************************************/
CCmdAddExp::CCmdAddExp()
{
}

CCmdAddExp::~CCmdAddExp()
{
}

string CCmdAddExp::Cmd()const
{
	return "addexp";
}

ICmdHandler *CCmdAddExp::CreateCmdHandler()
{
	return new CCmdAddExp;
}

void CCmdAddExp::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;
	CEntityAttributeComponent *pRoleAttr = pRoleChat->GetRoleAttr();

	if (pRoleAttr != NULL) {
		pRoleAttr->AddAbsExp(nPara1, CRoleExpLog::EAddExpCause_GMCommand, 0);
	}
}

/*************************************************************/
/*				   设置人气值sethot						*/
/***********************************************************/
CCmdSetHot::CCmdSetHot()
{
}

CCmdSetHot::~CCmdSetHot()
{
}

string CCmdSetHot::Cmd()const
{
	return "sethot";
}

ICmdHandler* CCmdSetHot::CreateCmdHandler()
{
	return new CCmdSetHot; 
}

void CCmdSetHot::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;
	CEntityAttributeComponent *pRoleAttr = pRoleChat->GetRoleAttr();

	if (pRoleAttr != NULL) {
		pRoleAttr->SetHot(nPara1, 0, 0);
	}
}
/*************************************************************/
/*				   设置人气值setpre						*/
/***********************************************************/
CCmdSetPre::CCmdSetPre()
{
}

CCmdSetPre::~CCmdSetPre()
{
}

string CCmdSetPre::Cmd()const
{
	return "setpre";
}

ICmdHandler *CCmdSetPre::CreateCmdHandler()
{
	return new CCmdSetPre;
}

void CCmdSetPre::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;
	CEntityAttributeComponent *pRoleAttr = pRoleChat->GetRoleAttr();

	if (pRoleAttr != NULL) {
		pRoleAttr->SetPre(nPara1, 0 , 0);
	}
}

/*************************************************************/
/*				   设置金币setmoney						*/
/***********************************************************/
CCmdSetMoney::CCmdSetMoney()
{
}

CCmdSetMoney::~CCmdSetMoney()
{
}

string CCmdSetMoney::Cmd()const
{
	return "setmoney";
}

ICmdHandler *CCmdSetMoney::CreateCmdHandler()
{
	return new CCmdSetMoney;
}
void CCmdSetMoney::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;
	CEntityAttributeComponent *pRoleAttr = pRoleChat->GetRoleAttr();

	if (pRoleAttr != NULL) {
		pRoleAttr->ChangeMoney(nPara1 - pRoleAttr->GetMoney(), EChangeMoneyCause_GM_Change, 0);
	}	
}

/*************************************************************/
/*				   角色升级levelup 						*/
/***********************************************************/
CCmdLevelUp::CCmdLevelUp()
{
}

CCmdLevelUp::~CCmdLevelUp()
{
}

string CCmdLevelUp::Cmd()const
{
	return	"levelup";
}

ICmdHandler *CCmdLevelUp::CreateCmdHandler()
{
	return new CCmdLevelUp;
}

void CCmdLevelUp::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;
	CEntityAttributeComponent *pRoleAttr = pRoleChat->GetRoleAttr();

	if (pRoleAttr != NULL) {
		if (pRoleAttr->GetLevel() < CLogicInitData::Instance().GetMaxLevel()) {
			pRoleAttr->AddAbsExp(CLogicInitData::Instance().GetExpNeed(pRoleAttr->GetLevel()) - pRoleAttr->GetExp(), CRoleExpLog::EAddExpCause_GMCommand, 0);
		} else {
			pRoleChat->SysChat(CLocalization::Instance().GetString("Chat_System_Remind4").c_str());
		}
	}	
}

/*************************************************************/
/*				   角色升级setlevel 						*/
/***********************************************************/
CCmdSetLevel::CCmdSetLevel()
{
}

CCmdSetLevel::~CCmdSetLevel()
{
}

string CCmdSetLevel::Cmd()const
{
	return "setlevel";
}

ICmdHandler *CCmdSetLevel::CreateCmdHandler()
{
	return new CCmdSetLevel;
}

void CCmdSetLevel::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;
	CEntityAttributeComponent *pRoleAttr = pRoleChat->GetRoleAttr();

	if (pRoleAttr != NULL) {
		if (nPara1 < 1 || nPara1 > CLogicInitData::Instance().GetMaxLevel()) {
			pRoleChat->SysChat(CLocalization::Instance().GetString("Chat_System_Remind5").c_str());
		} else {
			pRoleAttr->SetLevel((unsigned short)nPara1, 0, 0);
		}
	}	
}


/*************************************************************/
/*				   添加物品additem							*/
/***********************************************************/
CCmdAddItem::CCmdAddItem()
{
}

CCmdAddItem::~CCmdAddItem()
{
}

string CCmdAddItem::Cmd()const
{
	return	"additem";
}

ICmdHandler *CCmdAddItem::CreateCmdHandler()
{
	return new CCmdAddItem;
}

void CCmdAddItem::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{
	const int nDefaultDay = 7;
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;
	CEntityItemComponent *pRoleItem = pRoleChat->GetRoleItem();

	if (pRoleItem != NULL) {
		if (nPara2 <= 0) {
			//pRoleChat->SysChat("添加物品参数不正确。");
			pRoleChat->SysChat(CLocalization::Instance().GetString("Chat_System_Remind6").c_str());
			return;
		}
		nPara3 = nPara3 > 0 ? nPara3 : nDefaultDay;
		int nValidTime = nPara3 * 24 * 3600;
		int nRet = pRoleItem->AddItem((itemtype_t)nPara1, (unsigned short)nPara2, nValidTime, EItemAction_Add_DebugCommand, 0, true, false);
		switch (nRet)
		{
		case EItemError_Success:
			{
				ItemConfig *pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID((itemtype_t)nPara1);
				if (pItemInfo != NULL)
				{
					std::string strItemExtra;
					if (pItemInfo->IsEquip())
					{
						SafeFormat(strItemExtra, CLocalization::Instance().GetString("Item_EquipMatune_Day"), nValidTime / (3600 * 24) );
					}
					else
					{
						SafeFormat(strItemExtra, CLocalization::Instance().GetString("Item_Expandable_Count"), nPara2 );
					}
					pRoleChat->SysChat(CLocalization::Instance().GetString("Chat_System_Add_Item_Result").c_str(), pItemInfo->m_strName.c_str(), strItemExtra.c_str());
				}
			}
			break;
		case EItemUse_UniqueExist:
			{
				pRoleChat->SysChat(CLocalization::Instance().GetString("Chat_System_Remind8").c_str());
			}
			break;
		default:	
			pRoleChat->SysChat(CLocalization::Instance().GetString("Chat_System_Remind9").c_str());
		}
	}	
}


/*************************************************************/
/*				   物品冷却完毕itemcdall					*/
/***********************************************************/
CCmdItemCDAll::CCmdItemCDAll()
{
}

CCmdItemCDAll::~CCmdItemCDAll()
{
}

string CCmdItemCDAll::Cmd()const
{
	return "itemcdall";
}

ICmdHandler *CCmdItemCDAll::CreateCmdHandler()
{
	return new CCmdItemCDAll;
}

void CCmdItemCDAll::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;
	CEntityItemComponent *pRoleItem = pRoleChat->GetRoleItem();

	if (pRoleItem != NULL) {
		pRoleItem->releaseAllCDItem();
	}	
}


/*************************************************************/
/*				修改系统时间settime						*/
/*				用法settime 2011-02-11 13:00:00				*/
/***********************************************************/
CCmdSetSysTime::CCmdSetSysTime()
{
}

CCmdSetSysTime::~CCmdSetSysTime()
{
}

string CCmdSetSysTime::Cmd()const
{
	return "settime";
}

ICmdHandler *CCmdSetSysTime::CreateCmdHandler()
{
	return new CCmdSetSysTime;
}

void CCmdSetSysTime::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;

	if (_ParseCmd(strCmd)) {
		_SetSysTime();				
	} else {
		//pRoleChat->SysChat("输入的命令行参数有误。");	
		pRoleChat->SysChat(CLocalization::Instance().GetString("Chat_System_Remind10").c_str());	
	}
}

//用法settime 2011-02-11 13:00:00	
bool CCmdSetSysTime::_ParseCmd(const string &strCmd)
{
#define BUFF_SIZE 256

	char buff[BUFF_SIZE];
	char *pStr1 = buff;
	memset(buff, 0, sizeof(buff));
	if (strCmd.length() >= BUFF_SIZE - 1 || strCmd.length() == 0) {
		return false;
	}

	int nSize = MIN(BUFF_SIZE - 1, strCmd.length());
	memcpy(buff, strCmd.c_str(), nSize);

	while(*pStr1 == ' ') {
		++pStr1;
	}

	pStr1 = strchr(pStr1, ' ');

	char *pStr2 = strchr(pStr1, '-');
	if (pStr2 == NULL) {
		return false;
	}
	*pStr2 = 0;
	m_nYear = (unsigned short)atoi(pStr1);

	++pStr2;
	pStr1 = strchr(pStr2, '-');
	if (pStr1 == NULL) {
		return false;
	}
	*pStr1 = 0;
	m_nMonth = (unsigned short)atoi(pStr2);

	++pStr1;
	pStr2 = strchr(pStr1 ,' ');
	if (pStr2 == NULL) {
		return false;
	}
	*pStr2 = 0;
	m_nDay = (unsigned short)atoi(pStr1);

	++pStr2;
	while(*pStr2 == ' ') {
		++pStr2;
	}

	pStr1 = strchr(pStr2, ':');
	if (pStr1 == NULL) {
		return false;
	}
	*pStr1++ = 0;
	m_nHour = (unsigned short)atoi(pStr2);

	pStr2 = strchr(pStr1, ':');
	if (pStr2 == NULL) {
		return false;
	}
	*pStr2++ = 0;
	m_nMinute = (unsigned short)atoi(pStr1);
	m_nSecond = (unsigned short)atoi(pStr2);

	return true;
}

bool CCmdSetSysTime::_SetSysTime()
{
#ifdef WIN32
	SYSTEMTIME time;
    memset(&time, 0, sizeof(time));
    time.wYear = m_nYear;
	time.wMonth = m_nMonth;
	time.wDay = m_nDay;
	time.wDayOfWeek = 0 ;
	time.wHour = m_nHour;
	time.wMinute = m_nMinute;
	time.wSecond = m_nSecond;
	time.wMilliseconds = 0;

	return (SetLocalTime(&time) > 0 ? true : false);	
#else
	struct tm time;
    memset(&time, 0, sizeof(time));
    time.tm_year = m_nYear - 1900;
	time.tm_mon = m_nMonth - 1;
	time.tm_mday = m_nDay;

	time.tm_hour = m_nHour;
	time.tm_min = m_nMinute;
	time.tm_sec = m_nSecond;

	time_t nTime = mktime(&time);
	return (stime(&nTime) == 0 ? true : false);
#endif

}



/*************************************************************/
/*				   设置M币setpt		 						*/
/***********************************************************/
CCmdAddPt::CCmdAddPt()
{
}

CCmdAddPt::~CCmdAddPt()
{
}

string CCmdAddPt::Cmd()const
{
	return "addpt";
}

ICmdHandler *CCmdAddPt::CreateCmdHandler()
{
	return new CCmdAddPt;
}

void CCmdAddPt::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;
	CEntityAttributeComponent *pRoleAttr = pRoleChat->GetRoleAttr();

	if (pRoleAttr != NULL) {
		pRoleAttr->ChangeBill((unsigned int)nPara1, EChangeBillCause_Add_GMCommand);
	}	
}


CCmdSetOnlineTime::CCmdSetOnlineTime()
{
}

CCmdSetOnlineTime::~CCmdSetOnlineTime()
{
}

string CCmdSetOnlineTime::Cmd()const
{
	return "setonlinetime";
}

ICmdHandler *CCmdSetOnlineTime::CreateCmdHandler()
{
	return new CCmdSetOnlineTime;
}

void CCmdSetOnlineTime::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;
	CEntityAttributeComponent *pRoleAttr = pRoleChat->GetRoleAttr();

	if (pRoleAttr != NULL) {
		pRoleAttr->SetOnlineTime((unsigned int)nPara1);
	}	
}


CCmdSendItem::CCmdSendItem()
{
}

CCmdSendItem::~CCmdSendItem()
{
}

string CCmdSendItem::Cmd()const
{
	return "senditem";
}

ICmdHandler *CCmdSendItem::CreateCmdHandler()
{
	return new CCmdSendItem;
}

void CCmdSendItem::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{
    CRoleChat *pRoleChat = (CRoleChat *)&cmp;
    pRoleChat->SysChat("Command disabled, use additem or GMClient instead!");
}

CCmdWhosyoudaddy::CCmdWhosyoudaddy()
{
}

CCmdWhosyoudaddy::~CCmdWhosyoudaddy()
{
}

string CCmdWhosyoudaddy::Cmd()const
{
	return "whosyourdaddy";
}

ICmdHandler *CCmdWhosyoudaddy::CreateCmdHandler()
{
	return new CCmdWhosyoudaddy;
}

void CCmdWhosyoudaddy::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{

}

CCmdPlayerCount::CCmdPlayerCount()
{
}

CCmdPlayerCount::~CCmdPlayerCount()
{
}

string CCmdPlayerCount::Cmd()const
{
	return "playercount";
}

ICmdHandler *CCmdPlayerCount::CreateCmdHandler()
{
	return new CCmdPlayerCount;
}

void CCmdPlayerCount::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;
	pRoleChat->SysChat("current player count: %d", CPlayerManager::Instance().CountPlayer());
}

CCmdAdminChat::CCmdAdminChat()
{

}

CCmdAdminChat::~CCmdAdminChat()
{

}

std::string CCmdAdminChat::Cmd() const
{
	return "adminchat";
}

ICmdHandler * CCmdAdminChat::CreateCmdHandler()
{
	return new CCmdAdminChat;
}

void CCmdAdminChat::process( CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd )
{
	std::string strName =  Cmd() + " ";
	std::string strContent = strCmd.substr(strName.size(), strCmd.size() - strName.size());
	CAdminChat::AdminChat(strContent.c_str());
}

CCmdAddMCoin::CCmdAddMCoin()
{

}

CCmdAddMCoin::~CCmdAddMCoin()
{

}

std::string CCmdAddMCoin::Cmd() const
{
	return "addmcoin";
}

ICmdHandler * CCmdAddMCoin::CreateCmdHandler()
{
	return new CCmdAddMCoin;
}

void CCmdAddMCoin::process( CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd )
{
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;
	CEntityAttributeComponent *pRoleAttr = pRoleChat->GetRoleAttr();

	if (pRoleAttr != NULL) {
		pRoleAttr->AddMCoinByCmd((unsigned int)nPara1);
	}
}

CCmdAddBindMCoin::CCmdAddBindMCoin()
{

}

CCmdAddBindMCoin::~CCmdAddBindMCoin()
{

}

std::string CCmdAddBindMCoin::Cmd() const
{
	return "addbindmcoin";
}

ICmdHandler * CCmdAddBindMCoin::CreateCmdHandler()
{
	return new CCmdAddBindMCoin;
}

void CCmdAddBindMCoin::process( CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd )
{
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;
	CEntityAttributeComponent *pRoleAttr = pRoleChat->GetRoleAttr();

	if (pRoleAttr != NULL) {
		pRoleAttr->AddBindMCoinByCmd((unsigned int)nPara1);
	}
}

CCmdAddClothEffect::CCmdAddClothEffect()
{

}

CCmdAddClothEffect::~CCmdAddClothEffect()
{

}

std::string CCmdAddClothEffect::Cmd() const
{
    return "addClothEffect";
}

ICmdHandler * CCmdAddClothEffect::CreateCmdHandler()
{
    return new CCmdAddClothEffect;
}

void CCmdAddClothEffect::process( CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd )
{
    CRoleChat *pRoleChat = (CRoleChat *)&cmp;
    CEntityItemComponent *pRoleItem = pRoleChat->GetRoleItem();

    if (pRoleItem != NULL) 
    {
        pRoleItem->AddClothEffectByGM((unsigned int)nPara1);
    }
}

/*************************************************************/
/*		           通过聊天命令发送信息 					*/
/***********************************************************/
CCmdDebug::CCmdDebug()
{

}

CCmdDebug::~CCmdDebug()
{

}

std::string CCmdDebug::Cmd() const
{
	return "debug";
}

ICmdHandler * CCmdDebug::CreateCmdHandler()
{
	return new CCmdDebug;
}

void CCmdDebug::process( CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd )
{
	if (nPara1 < 0 ) {
		return;
	}
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;

	CEntityCoupleComponent *pRoleCouple = pRoleChat->GetRoleCouple();
	if (pRoleCouple != NULL) {
		pRoleCouple->CoupleTest(nPara1, nPara2, nPara3);
	}

	CEntityVIPComponent *pRoleVIP = pRoleChat->GetRoleVIP();
	if (pRoleVIP != NULL) {
		pRoleVIP->OnVIPTest(nPara1, nPara2, nPara3);
	}

	CEntityDanceGroupComponent *pRoleDanceGroup = pRoleChat->GetRoleDanceGroup();
	if (pRoleDanceGroup != NULL) {
		pRoleDanceGroup->DanceGroupTest(nPara1, nPara2, nPara3);
	}

	CEntityActivityComponent *pRoleActivity = pRoleChat->GetRoleActivity();
	if (pRoleActivity != NULL) {
		pRoleActivity->OnTestActivity(nPara1, nPara2, nPara3);
	}

	CRoomComponent *pRoleRoom = pRoleChat->GetRoleRoom();
	if (pRoleRoom != NULL) {
		pRoleRoom->OnRoomTest(nPara1, nPara2, nPara3);
	}

	CEntityPhotoRoomComponent *pRolePhotoRoom = pRoleChat->GetRolePhotoRoom();
	if (pRolePhotoRoom != NULL) {
		pRolePhotoRoom->OnPhotoRoomTest(nPara1, nPara2, nPara3);
	}

}

/*************************************************************/
/*		               修改亲密度--additinacy(int) 		*/
/***********************************************************/
CCmdAddIntimacy::CCmdAddIntimacy()
{

}

CCmdAddIntimacy::~CCmdAddIntimacy()
{

}

std::string CCmdAddIntimacy::Cmd() const
{
	return "addintimacy";
}

ICmdHandler * CCmdAddIntimacy::CreateCmdHandler()
{
	return new CCmdAddIntimacy;
}

void CCmdAddIntimacy::process( CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd )
{
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;
	CEntityCoupleComponent *pRoleCouple = pRoleChat->GetRoleCouple();

	if (pRoleCouple != NULL) {
		pRoleCouple->ChangeIntimacy(nPara1, EChangeIntimacyCause_Command, 0);
	}
}

/*************************************************************/
/*		模拟批量开宝箱openbox(boxid,count)	*/
/***********************************************************/
CCmdOpenBox::CCmdOpenBox()
{

}

CCmdOpenBox::~CCmdOpenBox()
{

}

string CCmdOpenBox::Cmd()const
{
	return "openbox";
}

ICmdHandler * CCmdOpenBox::CreateCmdHandler()
{
	return new CCmdOpenBox;
}

void CCmdOpenBox::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;
	CEntityItemComponent *pRoleItem = pRoleChat->GetRoleItem();

	if ( pRoleItem != NULL )
	{
		pRoleItem->SimulateRandomBox( (itemtype_t)nPara1, nPara2 );
	}
}

/*****************************************************************/
/*	模拟战斗结束物品掉落danceitem(modeid, count) mode:1-4  	*/
/***************************************************************/
CCmdDropDanceItem::CCmdDropDanceItem()
{

}

CCmdDropDanceItem::~CCmdDropDanceItem()
{

}

std::string CCmdDropDanceItem::Cmd() const
{
	return "dropdanceitem";
}

ICmdHandler * CCmdDropDanceItem::CreateCmdHandler()
{
	return new CCmdDropDanceItem;
}

void CCmdDropDanceItem::process( CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd )
{
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;
	CRoomComponent * pRoleRoom = pRoleChat->GetRoleRoom();

	if ( pRoleRoom != NULL )
	{
		if (nPara1 > MusicMode_None && nPara1 < MusicMode_Max) {
			int nTimesSuc = pRoleRoom->SimulateDanceItemDrop( nPara1, nPara2);
			pRoleChat->SysChat(CLocalization::Instance().GetString("Chat_System_Remind16").c_str(), nPara1, nPara2, nTimesSuc);
		} else {
			pRoleChat->SysChat(CLocalization::Instance().GetString("Chat_System_Remind3").c_str());
		}
	}
}

/*****************************************************************/
/*						   修改VIP等级			  				*/
/***************************************************************/
CCmdSetVIPLevel::CCmdSetVIPLevel()
{

}

CCmdSetVIPLevel::~CCmdSetVIPLevel()
{

}

std::string CCmdSetVIPLevel::Cmd() const
{
	return "setviplevel";
}

ICmdHandler * CCmdSetVIPLevel::CreateCmdHandler()
{
	return new CCmdSetVIPLevel;
}

void CCmdSetVIPLevel::process( CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd )
{
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;
	CEntityVIPComponent *pRoleVIP = pRoleChat->GetRoleVIP();

	if (pRoleVIP != NULL) {
		pRoleVIP->CmdSetVIPLevel(nPara1);
		if (nPara1 < 0 || nPara1 > CVIPDataMgr::Instance().GetMaxLevel() ) {
			pRoleChat->SysChat(CLocalization::Instance().GetString("Chat_System_Remind17").c_str(),CVIPDataMgr::Instance().GetMaxLevel());
		} else 
		{
			const char* szYesNo = NULL;
			if (pRoleVIP->IsVIP())
			{
				szYesNo = CLocalization::Instance().GetString("Chat_System_Remind19").c_str();
			}
			else
			{
				szYesNo = CLocalization::Instance().GetString("Chat_System_Remind20").c_str();
			}
			pRoleChat->SysChat(CLocalization::Instance().GetString("Chat_System_Remind18").c_str(), nPara1,szYesNo ,pRoleVIP->VIPLevel() );
		}
	}
}

/*****************************************************************/
/*						修改VIP经验值  						*/
/***************************************************************/
CCmdAddVIPExp::CCmdAddVIPExp()
{

}

CCmdAddVIPExp::~CCmdAddVIPExp()
{

}

std::string CCmdAddVIPExp::Cmd() const
{
	return "addvipexp";
}

ICmdHandler * CCmdAddVIPExp::CreateCmdHandler()
{
	return new CCmdAddVIPExp;
}

void CCmdAddVIPExp::process( CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd )
{
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;

	CEntityVIPComponent *pRoleVIP = pRoleChat->GetRoleVIP();
	if (pRoleVIP != NULL) {
		pRoleVIP->ChangeVIPInfo(0, nPara1, ERoleVipExpCause_AddByGM);
		const char* szYesNo = NULL;
		if (pRoleVIP->IsVIP())
		{
			szYesNo = CLocalization::Instance().GetString("Chat_System_Remind19").c_str();
		}
		else
		{
			szYesNo = CLocalization::Instance().GetString("Chat_System_Remind20").c_str();
		}
		pRoleChat->SysChat(CLocalization::Instance().GetString("Chat_System_Remind21").c_str(), nPara1, szYesNo , pRoleVIP->VIPLevel() );
	}
}

/*****************************************************************/
/*						激活魔法神灯关卡				*/
/***************************************************************/
CCmdActivitedParclose::CCmdActivitedParclose()
{

}

CCmdActivitedParclose::~CCmdActivitedParclose()
{

}

std::string CCmdActivitedParclose::Cmd() const
{
	return "activedParclose";
}

ICmdHandler * CCmdActivitedParclose::CreateCmdHandler()
{
	return new CCmdActivitedParclose;
}

void CCmdActivitedParclose::process( CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd )
{
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;

	CEntityDungeonComponent * pRoleDungeon = pRoleChat->GetRoleDungeon();
	if ( pRoleDungeon != NULL )
	{
		pRoleDungeon->GMActivedParclose( (unsigned int)nPara1 );

		pRoleChat->SysChat(CLocalization::Instance().GetString("Chat_System_Remind22").c_str(), nPara1 );
	}
}


CCmdActivitedThemeParclose::CCmdActivitedThemeParclose()
{

}

CCmdActivitedThemeParclose::~CCmdActivitedThemeParclose()
{

}

std::string CCmdActivitedThemeParclose::Cmd() const
{
	return "activedTheme";
}

ICmdHandler * CCmdActivitedThemeParclose::CreateCmdHandler()
{
	return new CCmdActivitedThemeParclose;
}

void CCmdActivitedThemeParclose::process( CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd )
{
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;

	CEntityDungeonComponent * pRoleDungeon = pRoleChat->GetRoleDungeon();
	if ( pRoleDungeon != NULL )
	{
		pRoleDungeon->GMActivedThemeParclose( (unsigned int)nPara1, (unsigned int)nPara2 );

		pRoleChat->SysChat(CLocalization::Instance().GetString("Chat_System_Remind25").c_str(), nPara1, nPara2 );
	}
}

CCmdCheckIn::CCmdCheckIn()
{

}

CCmdCheckIn::~CCmdCheckIn()
{

}

std::string CCmdCheckIn::Cmd() const
{
	return "clearcheckindata";
}

ICmdHandler * CCmdCheckIn::CreateCmdHandler()
{
	return new CCmdCheckIn;
}

void CCmdCheckIn::process( CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd )
{
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;

	CEntityCheckInComponent * pRoleCheckIn = pRoleChat->GetRoleCheckIn();
	if ( pRoleCheckIn != NULL )
	{
		pRoleCheckIn->GMClearCheckInData();

		pRoleChat->SysChat(CLocalization::Instance().GetString("Chat_System_Remind26").c_str());
	}
}


CCmdAddDebris::CCmdAddDebris()
{

}

CCmdAddDebris::~CCmdAddDebris()
{

}

std::string CCmdAddDebris::Cmd() const
{
	return "addDebris";
}

ICmdHandler * CCmdAddDebris::CreateCmdHandler()
{
	return new CCmdAddDebris;
}

void CCmdAddDebris::process( CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd )
{
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;

	CEntityDungeonComponent * pRoleDungeon = pRoleChat->GetRoleDungeon();
	if ( pRoleDungeon != NULL )
	{
		pRoleDungeon->GMAddDebris( (unsigned int)nPara1, nPara2 );

		pRoleChat->SysChat(CLocalization::Instance().GetString("Chat_System_Remind23").c_str(), nPara1, nPara2 );
	}
}

CCmdAddLiveRoomAuditionScore::CCmdAddLiveRoomAuditionScore()
{

}

CCmdAddLiveRoomAuditionScore::~CCmdAddLiveRoomAuditionScore()
{

}

std::string CCmdAddLiveRoomAuditionScore::Cmd() const
{
    return "addAuditionScore";
}

ICmdHandler * CCmdAddLiveRoomAuditionScore::CreateCmdHandler()
{
    return new CCmdAddLiveRoomAuditionScore;
}

void CCmdAddLiveRoomAuditionScore::process( CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd )
{
    CRoleChat *pRoleChat = (CRoleChat *)&cmp;

    CRoomComponent * pRoleRoom = pRoleChat->GetRoleRoom();
    if ( pRoleRoom != NULL )
    {
        pRoleRoom->AddLiveRoomAuditionScoreRankData( nPara1 );
        pRoleChat->SysChat(CLocalization::Instance().GetString("Chat_System_Remind24").c_str(), nPara1  );
    }
}


/*****************************************************************/
/*			模拟批量幸运转盘luckbet(mode,count)    0-免费 1-收费*/
/***************************************************************/
CCmdLuckBet::CCmdLuckBet()
{

}

CCmdLuckBet::~CCmdLuckBet()
{

}

std::string CCmdLuckBet::Cmd() const
{
	return "luckbet";
}

ICmdHandler * CCmdLuckBet::CreateCmdHandler()
{
	return new CCmdLuckBet;
}

void CCmdLuckBet::process( CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd )
{
    return;
}

//////////////////////////////////////////////////////////////////////////
/*************************************************************/
/*      直播房间设置观众人数上限                                 */
/*************************************************************/
CCmdLiveRoom::CCmdLiveRoom()
{

}

CCmdLiveRoom::~CCmdLiveRoom()
{

}

std::string CCmdLiveRoom::Cmd() const
{
	return "liveroom";
}

ICmdHandler * CCmdLiveRoom::CreateCmdHandler()
{
	return new CCmdLiveRoom;
}

void CCmdLiveRoom::process( CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd )
{
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;
	CRoomComponent *pRoleRoom = pRoleChat->GetRoleRoom();

	if ( pRoleRoom != NULL ) // 只是玩家合法性校验，正常登陆后的玩家这个Component一定不是NULL
	{
		switch (nPara1)
		{
		case 1:
			//CSystemSettingMgr::Instance().SetLiveRoomMinRoleLevel(nPara2);
			//最终还是决定不要给GM这个权限了
			break;
		case 2:
            RoomSettingMgr::Instance().SetLiveRoomMaxAudienceCount(nPara2);
			CLiveRoomManager::Instance().RefreshMaxAudienceCount();
			break;
		case 3:
            RoomSettingMgr::Instance().SetLiveRoomCloseTime(nPara2);
			break;
		case 4:
            RoomSettingMgr::Instance().SetLiveRoomCountDownTime(nPara2);
			break;
		default:
			break;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************/
/*      创建测试房间                                 */
/*************************************************************/
CCmdCreateRoom::CCmdCreateRoom()
{
}

CCmdCreateRoom::~CCmdCreateRoom()
{
}

std::string CCmdCreateRoom::Cmd() const
{
	return "room";
}

ICmdHandler * CCmdCreateRoom::CreateCmdHandler()
{
	return new CCmdCreateRoom;
}

void CCmdCreateRoom::process( CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd )
{
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;
	CRoomComponent *pRoleRoom = pRoleChat->GetRoleRoom();

	if ( pRoleRoom != NULL )
	{
		int nRoomCount = nPara1;
		CRoomManager::Instance().CreateTestRoom(nRoomCount, pRoleRoom);
	}
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************/
/*      创建开放性场景测试房间                               */
/*************************************************************/
CCmdCreateAmuseRoom::CCmdCreateAmuseRoom()
{
}

CCmdCreateAmuseRoom::~CCmdCreateAmuseRoom()
{
}

std::string CCmdCreateAmuseRoom::Cmd() const
{
	return "amuse";
}

ICmdHandler * CCmdCreateAmuseRoom::CreateCmdHandler()
{
	return new CCmdCreateAmuseRoom;
}

void CCmdCreateAmuseRoom::process( CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd )
{
	CRoleChat *pRoleChat = (CRoleChat *)&cmp;
	CAmuseRoomComponent *pRoleRoom = pRoleChat->GetRoleAmuseRoom();

	if ( pRoleRoom != NULL )
	{
		int nSceneID = nPara1;
		int nRoomCount = nPara2;
		CAmuseRoomManager::Instance().CreateTestAmuseRoom(nSceneID, nRoomCount, pRoleRoom);
	}
}
//////////////////////////////////////////////////////////////////////////
/*************************************************************/
/*      接受任务                               */
/*************************************************************/
CCmdAcceptQuest::CCmdAcceptQuest()
{

}

CCmdAcceptQuest::~CCmdAcceptQuest()
{

}

std::string CCmdAcceptQuest::Cmd() const
{
	return "acceptquest";
}

ICmdHandler* CCmdAcceptQuest::CreateCmdHandler()
{
	return new CCmdAcceptQuest();
}

void CCmdAcceptQuest::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{
	CRoleChat* pRoleChat = (CRoleChat *)&cmp;
	CEntityQuestNewComponent* pRoleQuestNew = pRoleChat->GetRoleQuestNew();
	if (pRoleQuestNew != NULL)
	{
		int nQuestID = nPara1;
		pRoleQuestNew->AddQuestByCmd((unsigned short)nQuestID);
	}
}

CCmdGetServerPlayer::CCmdGetServerPlayer()
{

}

CCmdGetServerPlayer::~CCmdGetServerPlayer()
{

}

std::string CCmdGetServerPlayer::Cmd() const
{
	return "getserverplayer";
}

ICmdHandler* CCmdGetServerPlayer::CreateCmdHandler()
{
	return new CCmdGetServerPlayer();
}

void CCmdGetServerPlayer::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{
	CRoleChat& pRoleChat = (CRoleChat&)cmp;
	std::list< unsigned int > roleIDList;
	CPlayerManager::Instance().GetRoleIDList(roleIDList);
	pRoleChat.SysChat(CLocalization::Instance().GetString("Current_Player_Count").c_str(),roleIDList.size());
}

CCmdAddDanceGroupHonor::CCmdAddDanceGroupHonor()
{

}

CCmdAddDanceGroupHonor::~CCmdAddDanceGroupHonor()
{

}

std::string CCmdAddDanceGroupHonor::Cmd() const
{
	return "AddDanceGroupHonor";
}

ICmdHandler* CCmdAddDanceGroupHonor::CreateCmdHandler()
{
	return new CCmdAddDanceGroupHonor();
}

void CCmdAddDanceGroupHonor::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{
	CRoleChat& pRoleChat = (CRoleChat&)cmp;
	CEntityDanceGroupComponent* pDanceGroup = pRoleChat.GetRoleDanceGroup();
	if (pDanceGroup != NULL)
	{
		pDanceGroup->ChangeDanceGroupHonor(nPara1,nPara2,EChangeDanceGroupHonorCause_CMD,0);
	}
}


CCmdEnterDanceGroupByName::CCmdEnterDanceGroupByName()
{

}

CCmdEnterDanceGroupByName::~CCmdEnterDanceGroupByName()
{

}

string CCmdEnterDanceGroupByName::Cmd()const
{
    return "EnterDanceGroupByName";
}

ICmdHandler* CCmdEnterDanceGroupByName::CreateCmdHandler()
{
    return new CCmdEnterDanceGroupByName;
}

void CCmdEnterDanceGroupByName::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{
    CRoleChat& pRoleChat = (CRoleChat&)cmp;
    CEntityDanceGroupComponent* pDanceGroup = pRoleChat.GetRoleDanceGroup();
    if (pDanceGroup != NULL)
    {

        std::string strName =  Cmd() + " ";
        std::string strGroupName = strCmd.substr(strName.size(), strCmd.size() - strName.size());
//        CAdminChat::AdminChat(strContent.c_str());

        pDanceGroup->AddDanceGroupByName(strGroupName);
    }

    return ;
}

CCmdRookieAnyBody::CCmdRookieAnyBody()
{

}
CCmdRookieAnyBody::~CCmdRookieAnyBody()
{

}

string CCmdRookieAnyBody::Cmd()const
{
    return "RookieAnyBody";
}

ICmdHandler* CCmdRookieAnyBody::CreateCmdHandler()
{
    return new CCmdRookieAnyBody;
}
void CCmdRookieAnyBody::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{
    CRoleChat& pRoleChat = (CRoleChat&)cmp;
    CEntityDanceGroupComponent* pDanceGroup = pRoleChat.GetRoleDanceGroup();
    if (pDanceGroup != NULL)
    {
        pDanceGroup->RookieAnyBody(nPara1, nPara2);
    }
}


CCmdGetCeremonyRoomNum::CCmdGetCeremonyRoomNum()
{

}
CCmdGetCeremonyRoomNum::~CCmdGetCeremonyRoomNum()
{

}

string CCmdGetCeremonyRoomNum::Cmd()const
{
    return "ceremonyroomnum";
}

ICmdHandler* CCmdGetCeremonyRoomNum::CreateCmdHandler()
{
    return new CCmdGetCeremonyRoomNum;
}

void CCmdGetCeremonyRoomNum::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{
    CRoleChat *pRoleChat = (CRoleChat *)&cmp;
    CEntityDanceGroupComponent *pRoleDanceGroup = pRoleChat->GetRoleDanceGroup();

    if (NULL != pRoleDanceGroup)
    {
        pRoleDanceGroup->GmGetCeremonyRoomNum(nPara1);
    }

}


CCmdActivenessComp::CCmdActivenessComp()
{
}

CCmdActivenessComp::~CCmdActivenessComp()
{

}

string CCmdActivenessComp::Cmd()const
{
	return "activeness";
}

ICmdHandler* CCmdActivenessComp::CreateCmdHandler()
{
	return new CCmdActivenessComp;
}

void CCmdActivenessComp::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{
	CRoleChat& pRoleChat = (CRoleChat&)cmp;
//	CEntityDanceGroupComponent* pDanceGroup = pRoleChat.GetRoleDanceGroup();

	CRoleActiveness* pComp = pRoleChat.GetComponent<CRoleActiveness>();

	CommonParam param;
	if (pComp != NULL)
	{
		std::string strComponent;
		switch (nPara1)
		{
		case 1:
			strComponent = "checkin";
			break;
		case 2:
			strComponent = "quest_daily";
			break;
		case 3:
			strComponent = "dungeon";
			break;
		case 4:
			strComponent = "dance_mode";
			param.SetParam(std::string("dance_mode"), nPara2);
			break;
		case 5:
			strComponent = "match_room";
			break;
		case 6:
			strComponent = "vip_daily_reward";
			break;
		case 7:
			strComponent = "social_share";
			break;
        case 8:
            strComponent = "magicarray_call";
            break;
        case 9:
            strComponent = "clotheffect_replace";
            break;
        case 10:
            strComponent = "clotheffect_upgrade";
            break;
		}

		param.SetParam(std::string("activeness"), strComponent);
		pComp->OnNotify(param);
	}
}

CCmdMagicArray::CCmdMagicArray()
{
}

CCmdMagicArray::~CCmdMagicArray()
{

}

string CCmdMagicArray::Cmd()const
{
    return "magicarray";
}

ICmdHandler* CCmdMagicArray::CreateCmdHandler()
{
    return new CCmdActivenessComp;
}

void CCmdMagicArray::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{
    CRoleChat& pRoleChat = (CRoleChat&)cmp;
    //	CEntityDanceGroupComponent* pDanceGroup = pRoleChat.GetRoleDanceGroup();

    CBetComponent* pComp = pRoleChat.GetComponent<CRoleBet>();
    if (pComp != NULL)
    {
        pComp->OnGMRollMagicArrayReward((unsigned int)nPara1, (unsigned int)nPara2, (unsigned int)nPara3);
    }
}

/*************************************************************/
/*      勋章                                                 */
/*************************************************************/

CCmdMedalComp::CCmdMedalComp()
{

}

CCmdMedalComp::~CCmdMedalComp()
{

}

std::string CCmdMedalComp::Cmd() const
{
    return "addmedal";
}

ICmdHandler* CCmdMedalComp::CreateCmdHandler()
{
    return new CCmdMedalComp();
}

void CCmdMedalComp::process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)
{
    CRoleChat *pRoleChat = (CRoleChat *)&cmp;
    CEntityMedalComponent *pRoleMedalComp = pRoleChat->GetRoleMedal();
    if (pRoleMedalComp != NULL)
    {
        bool isSuccess = pRoleMedalComp->CmdAddMedal(nPara1);
        if (isSuccess)
        {
            string medalName;
            CMedalCfgMgr::Instance().GetMedalName(nPara1, medalName);
;           pRoleChat->SysChat(CLocalization::Instance().GetString("Chat_System_Add_Medal_Result").c_str(), medalName.c_str());
        }
        else
        {
            pRoleChat->SysChat(CLocalization::Instance().GetString("Chat_System_Remind9").c_str());
        }
    }
}
