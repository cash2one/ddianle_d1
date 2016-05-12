#include "Localization.h"
#include "../socket/Log.h"
#include "../socket/Ini.h"

extern string g_sPath;

CLocalization::CLocalization()
{
}
CLocalization::~CLocalization()
{
}

//获取实例
CLocalization & CLocalization::Instance()
{
	static CLocalization s_Instance;
	return s_Instance;
}


//初始化实例
bool CLocalization::Init()
{
	_Register();
	bool bRes = true;
	if (bRes)
	{
		bRes = _Load();
	}
	if (bRes)
	{
		bRes = _Check();
	}
	return bRes;
}

//获取字符串内容
std::string CLocalization::GetString(std::string strKey)
{
	std::map< std::string, std::string >::iterator it = m_stringMap.find(strKey);
	if (it != m_stringMap.end())
	{
		return it->second;
	}
	else
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CLocalization::GetString. Unregistered String(%s)", strKey.c_str());
		return strKey;
	}
}

//获取字符串内容
std::string CLocalization::GetString(std::string strKey, unsigned int nMaxStringSize)
{
	std::map< std::string, std::string >::iterator it = m_stringMap.find(strKey);
	if (it != m_stringMap.end())
	{
		std::string strRet = it->second;
		if (strRet.size() > nMaxStringSize)
		{
			WriteLog(LOGLEVEL_WARNING, "Warning: CLocalization::GetString. Key(%s), Value(%s), MaxSize(%u), CurrentSize(%u)", strKey.c_str(), strRet.c_str(), nMaxStringSize, strRet.size());
			strRet.resize(nMaxStringSize);
		}
		return strRet;
	}
	else
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CLocalization::GetString. Unregistered String(%s)", strKey.c_str());
		return strKey;
	}
}

//注册相关的本地化字符串
void CLocalization::_Register()
{
	m_stringMap["System"] = "";
	m_stringMap["AdminAnnounce"] = "";
	m_stringMap["NewPlayerRoom"] = "";
	m_stringMap["DefaultRoomName"] = "";

	m_stringMap["Chat_FirstChargeReward1"] = "";
	m_stringMap["Chat_FirstChargeReward2"] = "";

	m_stringMap["Mail_Title_Send_Item"] = "";
	m_stringMap["Mail_Content1_Send_Item"] = "";
	m_stringMap["Mail_Content2_Send_Item"] = "";
	m_stringMap["Mail_Content3_Send_Item"] = "";

	m_stringMap["Chat_Beta_Reward_Content1"] = "";
	m_stringMap["Chat_Beta_Reward_Content2"] = "";
	m_stringMap["Chat_Beta_Reward_Content3"] = "";

	m_stringMap["Online_Reward_Mail_Title"] = "";
	m_stringMap["Online_Reward_Mail_Content1"] = "";
	m_stringMap["Online_Reward_Mail_Content2"] = "";
	m_stringMap["Online_Reward_Mail_Content3"] = "";

	m_stringMap["Level_Up_Reward_Mail_Sender"] = "";
	m_stringMap["Level_Up_Reward_Mail_Title"] = "";
	m_stringMap["Level_Up_Reward_Mail_Content"] = "";

	m_stringMap["Chat_System_Remind1"] = "";
	m_stringMap["Chat_System_Remind2"] = "";
	m_stringMap["Chat_System_Remind3"] = "";
	m_stringMap["Chat_System_Remind4"] = "";
	m_stringMap["Chat_System_Remind5"] = "";
	m_stringMap["Chat_System_Remind6"] = "";
	m_stringMap["Chat_System_Remind6"] = "";
	m_stringMap["Chat_System_Remind7"] = "";
	m_stringMap["Chat_System_Remind8"] = "";
	m_stringMap["Chat_System_Remind9"] = "";
	m_stringMap["Chat_System_Remind10"] = "";
	m_stringMap["Chat_System_Remind11"] = "";
	m_stringMap["Chat_System_Remind12"] = "";
	m_stringMap["Chat_System_Remind13"] = "";
	m_stringMap["Chat_System_Remind14"] = "";
	m_stringMap["Chat_System_Remind15"] = "";
	m_stringMap["Chat_System_Remind16"] = "";
	m_stringMap["Chat_System_Remind17"] = "";
	m_stringMap["Chat_System_Remind18"] = "";
	m_stringMap["Chat_System_Remind19"] = "";
	m_stringMap["Chat_System_Remind20"] = "";
	m_stringMap["Chat_System_Remind21"] = "";
	m_stringMap["Chat_System_Remind22"] = "";
    m_stringMap["Chat_System_Remind23"] = "";
    m_stringMap["Chat_System_Remind24"] = "";
	m_stringMap["Chat_System_Remind25"] = "";
	m_stringMap["Chat_System_Remind26"] = "";
	m_stringMap["Chat_System_NotEnoughLevel"] = "";
	m_stringMap["Chat_System_Add_Item_Result"] = "";
    m_stringMap["Chat_System_Add_Medal_Result"] = "";

	m_stringMap["Mail_Title_CheckIn_Daily_Reward"] = "";
	m_stringMap["Mail_Content_CheckIn_Daily_Reward"] = "";

	m_stringMap["CheckIn_Mail_Item_Title"] = "";
	m_stringMap["CheckIn_Mail_Item_Content"] = "";
	m_stringMap["CheckIn_Mail_Money_Title"] = "";
	m_stringMap["CheckIn_Mail_Money_Content"] = "";
	m_stringMap["CheckIn_Mail_BindCoin_Title"] = "";
	m_stringMap["CheckIn_Mail_BindCoin_Content"] = "";

	m_stringMap["Server_Will_ShutDown"] = "";

	// VIP
	m_stringMap["Mail_Title_VIP_Reward"] = "";
	m_stringMap["Mail_Content_VIP_Reward"] = "";
	m_stringMap["VIP_Week_Card"] = "";
	m_stringMap["VIP_Month_Card"] = "";
	m_stringMap["VIP_Season_Card"] = "";
	m_stringMap["VIP_Year_Card"] = "";

	m_stringMap["Item_Expandable_Count"] = "";
	m_stringMap["Item_EquipMatune_Day"] = "";
	m_stringMap["Item_EquipMatune_Forever"] = "";
    m_stringMap["Item_Money_Coin"] = "";
    m_stringMap["Item_Money_BindCoin"] = "";
    m_stringMap["Item_Money_Money"] = "";
    m_stringMap["Item_Money_Count"] = "";

    // time
    m_stringMap["Time_Hour"] = "";
    m_stringMap["Time_Minute"] = "";
    m_stringMap["Time_Second"] = "";

	// giftcode
	m_stringMap["Mail_Title_Activate_Gift_Code"] = "";
	m_stringMap["Mail_Content_Activate_Gift_Code"] = "";
	m_stringMap["Mail_Title_ExitDanceGroup"] = "";
	m_stringMap["Mail_Content_ExitDanceGroup"] = "";
	m_stringMap["Mail_Title_PreDismissDanceGroup"] = "";
	m_stringMap["Mail_Content_PreDismissDanceGroup"] = "";
	m_stringMap["Mail_Title_ChangeDanceGroupLeader"] = "";
	m_stringMap["Mail_Content_ChangeDanceGroupLeader"] = "";
	m_stringMap["Mail_Title_DismissDanceGroup"] = "";
	m_stringMap["Mail_Content_DismissDanceGroup"] = "";
	//record
	m_stringMap["DanceGroup_Record_AddDGMember"]="";
	m_stringMap["DanceGroup_Record_KickOutMember"]="";
    m_stringMap["DanceGroup_Record_ChangeMemberTitle"] = "";
    m_stringMap["DanceGroup_Record_ChangeMemberProperty"] = "";
	m_stringMap["DanceGroup_Record_Title1"] = "";
	m_stringMap["DanceGroup_Record_Title2"] = "";
	m_stringMap["DanceGroup_Record_Title3"] = "";
	m_stringMap["DanceGroup_Record_Title4"] = "";
	m_stringMap["DanceGroup_Record_Title5"] = "";
	m_stringMap["DanceGroup_Record_DonateRes"] = "";
	m_stringMap["DanceGroup_Record_ConsumeRes"]="";
	m_stringMap["DanceGroup_Record_LevelUpDes"]="";
	m_stringMap["DanceGroup_Record_ChangeTuanhui"]="";
    m_stringMap["DanceGroup_Record_UnlockBadge"] = "";
	m_stringMap["DanceGroup_Record_Exit"]="";
    m_stringMap["DanceGroup_Record_Invitation"] = "";
    m_stringMap["DanceGroup_Record_OpenStorey"] = "";
    m_stringMap["DanceGroup_Announce"] = "";
    m_stringMap["DanceGroup_Room_Name"] = "";
	m_stringMap["DanceGroup_NewRedEnvelope"] = "";

	//活动
	m_stringMap["Activity_Server_Busy"] = "";

	//累计在线
	m_stringMap["CumulativeRechargeActivity_String1"] = "";
	m_stringMap["CumulativeRechargeActivity_String2"] = "";
	m_stringMap["CumulativeRechargeActivity_String3"] = "";
	m_stringMap["CumulativeRechargeActivity_String4"] = "";
	m_stringMap["CumulativeRechargeActivity_String5"] = "";
	m_stringMap["CumulativeRechargeActivity_String6"] = "";

	//累计消费
	m_stringMap["CumulativeSpendActivity_String1"] = "";
	m_stringMap["CumulativeSpendActivity_String2"] = "";
	m_stringMap["CumulativeSpendActivity_String3"] = "";
	m_stringMap["CumulativeSpendActivity_String4"] = "";
	m_stringMap["CumulativeSpendActivity_String5"] = "";
	m_stringMap["CumulativeSpendActivity_String6"] = "";

    m_stringMap["GetActivityRewardsError1"] = "";
    m_stringMap["GetActivityRewardsError2"] = "";
    m_stringMap["GetActivityRewardsError3"] = "";
    m_stringMap["GetActivityRewardsError4"] = "";

	//兑换物品活动
	m_stringMap["ExchangeItems_String1"] = "";
	m_stringMap["ExchangeItems_String2"] = "";
	m_stringMap["ExchangeItems_String3"] = "";
	m_stringMap["ExchangeItems_String4"] = "";


	m_stringMap["Friend_Login_Notify"] = "";

	//舞团
	m_stringMap["Chat_Not_In_Any_DanceGroup"] = "";
	m_stringMap["Chat_DanceGroup_Not_Load_Completed"] = "";

	//玩家设置
	m_stringMap["Player_Setting_Error_Para"] = "";

	//情侣
	m_stringMap["Couple_Request_Can_Not_Add_Self"]="";
	m_stringMap["Couple_Request_Too_Frequently"]="";
	m_stringMap["Couple_Request_Poor_Level"]="";
	m_stringMap["Couple_Request_Has_Couple"]="";
	m_stringMap["Couple_Request_Not_Have_Require_Item1"]="";
	m_stringMap["Couple_Request_Not_Have_Require_Item2"]="";
	m_stringMap["Couple_Request_Receiver_Not_OffLine"]="";
	m_stringMap["Couple_Request_Receiver_Has_Couple"]="";
	m_stringMap["Couple_Request_Receiver_Poor_Level"]="";
	m_stringMap["Couple_Request_Data_Error"]="";
	m_stringMap["Couple_Request_Same_Gender"]="";
	m_stringMap["Couple_Request_Has_Process"]="";
	m_stringMap["Couple_Accept_Couple_Request_Other_Offline"]="";

	m_stringMap["Couple_Separated_Mail_Title"]="";
	m_stringMap["Couple_Separated_Mail_Content"]="";
	m_stringMap["Couple_Divorce_Mail_Title"]="";
	m_stringMap["Couple_Divorce_Mail_Content"]="";
	m_stringMap["Couple_Divorce_System_Notification"]="";
	m_stringMap["Couple_Wedding_Get_Married_Mail_Nofication_Title"]="";
	m_stringMap["Couple_Wedding_Get_Married_Mail_Nofication_Content"]="";
	m_stringMap["Couple_Wedding_Get_Married_Notification_To_All"]="";
	m_stringMap["Couple_Wedding_Married_Send_Item_Mail_Title"]="";
	m_stringMap["Couple_Wedding_Married_Send_Item_Mail_Content"]="";
	m_stringMap["Couple_Buy_Ring_Title"]="";
	m_stringMap["Couple_Buy_Ring_Content"]="";
	m_stringMap["Couple_BlessingSender_Gift_Title"]="";
	m_stringMap["Couple_BlessingSender_Gift_Content"]="";
	m_stringMap["Couple_BlessingCouple_Gift_Title"]="";
	m_stringMap["Couple_BlessingCouple_Gift_Content"]="";
	m_stringMap["Couple_RedEnvelope_Content"]="";
	m_stringMap["Couple_MarriageDate_Gift_Title"]="";
	m_stringMap["Couple_MarriageDate_Gift_Content"]="";
	

	m_stringMap["Festival_Bless_Friend_Reward_Subject"]="";
	m_stringMap["Festival_Bless_Friend_Reward_Body"]="";
	m_stringMap["Festival_Bless_Friend_SysChat"]="";
	
	m_stringMap["OpenBox_Announce"]="";
    m_stringMap["MagicArray_Announce"] = "";

	m_stringMap["Xmas_Free_Reward_Mail_Subject"]="";
	m_stringMap["Xmas_Free_Reward_Mail_Body"]="";
	m_stringMap["Festival_Wish_Reward_Mail_Subject"]="";
	m_stringMap["Festival_Wish_Reward_Mail_Body"]="";
	m_stringMap["Festival_Bless_Reward_Mail_Subject"]="";
	m_stringMap["Festival_Bless_Reward_Mail_Body"]="";

	m_stringMap["VIP_Level_Up_Rewrad_Mail_Title"]="";
	m_stringMap["VIP_Level_Up_Rewrad_Mail_Content"]="";
	m_stringMap["VIP_Level_Up_Announce"]="";
	m_stringMap["VIP_Online_Announce"] = "";

	m_stringMap["Room_Reward_Tips"]="";

	m_stringMap["Bet_LuckDraw_MailTitle"]="";
	m_stringMap["Bet_LuckDraw_MailContent"]="";
	m_stringMap["Bet_LuckDraw_Announce"]="";
	
	m_stringMap["Bet_LuckExchange_CostFail"]="";
	m_stringMap["Bet_LuckExchange_AddFail"]="";

	m_stringMap["Photo_Room_Name"]="";
	m_stringMap["Photo_Room_Expired_Notification"] = "";

	m_stringMap["Puzzle_MailTitle"]="";
	m_stringMap["Puzzle_MailContent"]="";
	m_stringMap["Puzzle_Finish_Announce"]="";

	// Live room
	m_stringMap["Live_Room_Name"] = "";
	m_stringMap["Live_Room_GM_Name_Prefix"] = "";
	m_stringMap["Live_Room_Audience_Chat_Tip"] = "";
	m_stringMap["Live_Room_Private_Chat_Tip"] = "";
	m_stringMap["Live_Room_Vote_Open"] = "";
	m_stringMap["Live_Room_Vote_Close"] = "";
	m_stringMap["Live_Room_Vote_Tip"] = "";

	//Time egg
	m_stringMap["Time_Egg_Mail_Title"] = "";

	// Constellation match
	m_stringMap["Constellation_Room_Name"] = "";
	m_stringMap["Constellation_Match_Suc_Tip"] = "";
	m_stringMap["Current_Player_Count"] = "";

	// patch use item
	m_stringMap["PatchUseItem_Desc_Exp"] = "";
	m_stringMap["PatchUseItem_Desc_Exp_Value"] = "";
	m_stringMap["PatchUseItem_Desc_Money"] = "";
	m_stringMap["PatchUseItem_Desc_Money_Value"] = "";
	m_stringMap["PatchUseItem_Desc_Super"] = "";
	m_stringMap["PatchUseItem_Desc_Up"] = "";
	m_stringMap["PatchUseItem_Desc_Intimacy"] = "";
	m_stringMap["PatchUseItem_Desc_TransForm"] = "";
    m_stringMap["PatchUseItem_Desc_DanceGroupContribution"] = "";

    // rank
    m_stringMap["RankList_Dance_Group_ID"] = "";

    m_stringMap["System_Tip"] = "";

    // 日期和时间
    m_stringMap["Date_Format"] = "";
    m_stringMap["Time_Format"] = "";

    // 充值补发M币邮件通知
    m_stringMap["Charge_Retry_Mail_Sender"] = "";
    m_stringMap["Charge_Retry_Mail_Title"] = "";
    m_stringMap["Charge_Retry_Mail_Content"] = "";
    m_stringMap["Charge_Retry_Mail_Content_GM"] = "";

    // 宠物
    m_stringMap["Pet_IncubationGet_Announce"] = "";

    // 广场舞大妈
    m_stringMap["BigMamaGuideMailTitle"] = "";
    m_stringMap["BigMamaGuideMailContext"] = "";
    m_stringMap["BigMamaBagNotEnoughMailTitle"] = "";
    m_stringMap["BigMamaBagNotEnoughMailContext"] = "";

}

//加载
bool CLocalization::_Load()
{
	CIni fileReadIni;
	extern CIni g_Config;
	char szFileName[MAX_PATH];
	g_Config.GetString("GameServer", "LocalizationFile", szFileName, MAX_PATH, "data/localization.ini");
	if(fileReadIni.Load((g_sPath + szFileName).c_str()) == false)
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CLocalization::_Load. Can not load the localization file(%s).", szFileName);
		return false;
	}
	std::string strContent = "";
	std::map< std::string, std::string >::iterator it = m_stringMap.begin();
	for (; it != m_stringMap.end(); ++it)
	{
		fileReadIni.GetString("Localization", it->first.c_str(), strContent, "");
		it->second = strContent;
	}
	return true;
}

//检查
bool CLocalization::_Check()
{
	bool bRes = true;
	std::map< std::string, std::string >::iterator it = m_stringMap.begin();
	for (; it != m_stringMap.end(); ++it)
	{
		if (it->second == "")
		{
			WriteLog(LOGLEVEL_ERROR, "Error: CLocalization::_Check. The string(%s) is not loaded.", it->first.c_str());
			bRes = false;
		}
	}
	return bRes;
}


