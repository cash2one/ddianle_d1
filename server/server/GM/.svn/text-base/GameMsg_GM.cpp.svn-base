#include "GameMsg_GM.h"
#include "GMMsgDef.h"
#include "../../socket/Utility.h"
#include "../../datastructure/datainfor/GMDataInfo.h"
#include <sstream>

GameMsg_C2S_GMLogin::GameMsg_C2S_GMLogin() :
GameMsg_Base(GM_C2S_Login)
{
    memset(m_szName, 0, GM_MAX_NAME_LEN);
    memset(m_szPasswd, 0, GM_MAX_PASSWD_LEN);
}

GameMsg_C2S_GMLogin::~GameMsg_C2S_GMLogin()
{
}

bool GameMsg_C2S_GMLogin::doDecode(CParamPool &IOBuff)
{
    string strTemp;
    IOBuff.GetStringW(strTemp);
    safememcpy(m_szName, GM_MAX_NAME_LEN - 1, strTemp.c_str(), (int)strTemp.size());

    IOBuff.GetStringW(strTemp);
    safememcpy(m_szPasswd, GM_MAX_PASSWD_LEN - 1, strTemp.c_str(), (int)strTemp.size());

    return true;
}

GameMsg_S2C_GMLoginSuc::GameMsg_S2C_GMLoginSuc()
:GameMsg_Base(GM_S2C_LoginSuc)
{
    memset(m_szName, 0, GM_MAX_NAME_LEN);
    m_Priv = 0;
}

GameMsg_S2C_GMLoginSuc::~GameMsg_S2C_GMLoginSuc()
{
}

bool GameMsg_S2C_GMLoginSuc::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddStringW(m_szName);
    IOBuff.AddInt(m_Priv);
    return true;
}

GameMsg_C2S_GMSendMail::GameMsg_C2S_GMSendMail()
:GameMsg_Base(GM_C2S_SendMail)
{
}

GameMsg_C2S_GMSendMail::~GameMsg_C2S_GMSendMail()
{
}

bool GameMsg_C2S_GMSendMail::doEncode(CParamPool &IOBuff)
{
    return true;
}

bool GameMsg_C2S_GMSendMail::doDecode(CParamPool &IOBuff)
{
    int nCount = IOBuff.GetUShort();
    if (nCount > 0) {
        for (int i = 0; i < nCount; ++i) {
            CRcvSysMailRoleInfo rcvRoleInfo;
            rcvRoleInfo.doDecodeCli(IOBuff);
            m_listRcvRoleInfo.push_back(rcvRoleInfo);
        }
    }

    return true;
}

GameMsg_S2C_GMSendMailRes::GameMsg_S2C_GMSendMailRes()
:GameMsg_Base(GM_S2C_SendMailRes)
{
}

GameMsg_S2C_GMSendMailRes::~GameMsg_S2C_GMSendMailRes()
{
}

bool GameMsg_S2C_GMSendMailRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_listRcvRoleInfo.size());
    if (m_listRcvRoleInfo.size() > 0) {
        for (list<CRcvSysMailRoleInfo>::iterator itr = m_listRcvRoleInfo.begin(); itr != m_listRcvRoleInfo.end(); ++itr) {
            itr->doEncodeCli(IOBuff);
        }
    }

    return true;
}

bool GameMsg_S2C_GMSendMailRes::doDecode(CParamPool &IOBuff)
{
    return true;
}

GameMsg_C2S_QueryRangeGift::GameMsg_C2S_QueryRangeGift()
:GameMsg_Base(GM_C2S_QueryRangeGift)
, m_nGiftID(0)
, m_strStartTime("")
, m_strEndTime("")
{
}

GameMsg_C2S_QueryRangeGift::~GameMsg_C2S_QueryRangeGift()
{
}

bool GameMsg_C2S_QueryRangeGift::doDecode(CParamPool & IOBuff)
{
    m_nGiftID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strStartTime);
    IOBuff.GetStringW(m_strEndTime);

    return true;
}

GameMsg_S2C_QueryRangeGiftRes::GameMsg_S2C_QueryRangeGiftRes()
:GameMsg_Base(GM_S2C_QueryRangeGiftRes)
, m_nActivatedNum(0)
{
}

GameMsg_S2C_QueryRangeGiftRes::~GameMsg_S2C_QueryRangeGiftRes()
{
}

bool GameMsg_S2C_QueryRangeGiftRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nActivatedNum);

    return true;
}

GameMsg_C2S_QueryMultiGift::GameMsg_C2S_QueryMultiGift()
:GameMsg_Base(GM_C2S_QueryMultiGift)
{
}

GameMsg_C2S_QueryMultiGift::~GameMsg_C2S_QueryMultiGift()
{
}

bool GameMsg_C2S_QueryMultiGift::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_listGiftCodes.size());
    if (m_listGiftCodes.size() > 0) {
        for (list<string>::iterator itr = m_listGiftCodes.begin(); itr != m_listGiftCodes.end(); ++itr) {
            string strCode = *itr;
            IOBuff.AddStringW(strCode.c_str());
        }
    }

    return true;
}

bool GameMsg_C2S_QueryMultiGift::doDecode(CParamPool &IOBuff)
{
    int nCount = IOBuff.GetUShort();
    if (nCount > 0) {
        while (nCount-- > 0) {
            string strCode;
            IOBuff.GetStringW(strCode);
            m_listGiftCodes.push_back(strCode);
        }
    }

    return true;
}

GameMsg_S2C_QueryMultiGiftRes::GameMsg_S2C_QueryMultiGiftRes()
:GameMsg_Base(GM_S2C_QueryMultiGiftRes)
{
}

GameMsg_S2C_QueryMultiGiftRes::~GameMsg_S2C_QueryMultiGiftRes()
{
}

bool GameMsg_S2C_QueryMultiGiftRes::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_listQueryCodes.size());
    if (m_listQueryCodes.size() > 0) {
        for (list<CQueryGiftState>::iterator itr = m_listQueryCodes.begin(); itr != m_listQueryCodes.end(); ++itr) {
            itr->doEncode(IOBuff);
        }
    }

    return true;
}

bool GameMsg_S2C_QueryMultiGiftRes::doDecode(CParamPool & IOBuff)
{
    int nCount = IOBuff.GetUShort();
    if (nCount > 0) {
        CQueryGiftState info;
        info.doDecode(IOBuff);
        m_listQueryCodes.push_back(info);
    }

    return true;
}

GameMsg_S2C_GMLoginFail::GameMsg_S2C_GMLoginFail()
:GameMsg_Base(GM_S2C_LoginFail)
, m_nError(0)
{
}

GameMsg_S2C_GMLoginFail::~GameMsg_S2C_GMLoginFail()
{
}

bool GameMsg_S2C_GMLoginFail::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddInt(m_nError);
    return true;
}

GameMsg_S2C_KickPlayer::GameMsg_S2C_KickPlayer()
: GameMsg_Base(0)
, m_nReason(0)
{
}

GameMsg_S2C_KickPlayer::~GameMsg_S2C_KickPlayer()
{
}

bool GameMsg_S2C_KickPlayer::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddUShort(m_nReason);

    return true;
}

GameMsg_C2S_GrantGMCmd::GameMsg_C2S_GrantGMCmd()
:GameMsg_Base(GM_C2S_GrantGMCmds)
{
    memset(m_szName, 0, GM_MAX_NAME_LEN);
}

GameMsg_C2S_GrantGMCmd::~GameMsg_C2S_GrantGMCmd()
{
}

bool GameMsg_C2S_GrantGMCmd::doDecode(CParamPool & IOBuff)
{
    string strName;
    IOBuff.GetStringW(strName);
    memcpy(m_szName, strName.c_str(), GM_MAX_NAME_LEN - 1);

    int nCount = IOBuff.GetUShort();
    if (nCount > 0) {
        string strCmd;
        IOBuff.GetStringW(strCmd);
        m_listGrantCmds.push_back(strCmd);
    }

    return true;
}

GameMsg_S2C_GrantCmdSuc::GameMsg_S2C_GrantCmdSuc()
:GameMsg_Base(GM_S2C_GrantGMCmdsSuc)
{
    memset(m_szName, 0, GM_MAX_NAME_LEN);
}

GameMsg_S2C_GrantCmdSuc::~GameMsg_S2C_GrantCmdSuc()
{
}

bool GameMsg_S2C_GrantCmdSuc::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddStringW(m_szName);
    IOBuff.AddUShort((unsigned short)m_listFailCmd.size());
    for (list<string>::iterator itr = m_listFailCmd.begin(); itr != m_listFailCmd.end(); ++itr) {
        IOBuff.AddStringW(itr->c_str());
    }

    return true;
}

GameMsg_S2C_GrantGMCmdFail::GameMsg_S2C_GrantGMCmdFail()
:GameMsg_Base(GM_S2C_GrantGMCmdsFail)
, m_nErrno(0)
{
    memset(m_szName, 0, GM_MAX_NAME_LEN);
}

GameMsg_S2C_GrantGMCmdFail::~GameMsg_S2C_GrantGMCmdFail()
{
}

bool GameMsg_S2C_GrantGMCmdFail::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddStringW(m_szName);
    IOBuff.AddUShort(m_nErrno);

    return true;
}

GameMsg_C2S_RemoveGMCmd::GameMsg_C2S_RemoveGMCmd()
:GameMsg_Base(GM_C2S_RemoveGMCmds)
{
    memset(m_szName, 0, GM_MAX_NAME_LEN);
}

GameMsg_C2S_RemoveGMCmd::~GameMsg_C2S_RemoveGMCmd()
{
}

bool GameMsg_C2S_RemoveGMCmd::doDecode(CParamPool & IOBuff)
{
    string strName;
    IOBuff.GetStringW(strName);
    memcpy(m_szName, strName.c_str(), GM_MAX_NAME_LEN - 1);

    int nCount = IOBuff.GetUShort();
    if (nCount > 0) {
        string strCmd;
        IOBuff.GetStringW(strCmd);
        m_listRemoveCmds.push_back(strCmd);
    }

    return true;
}

GameMsg_S2C_RemoveGMCmdSuc::GameMsg_S2C_RemoveGMCmdSuc()
:GameMsg_Base(GM_C2S_RemoveGMCmdsSuc)
{
    memset(m_szName, 0, GM_MAX_NAME_LEN);
}

GameMsg_S2C_RemoveGMCmdSuc::~GameMsg_S2C_RemoveGMCmdSuc()
{
}

bool GameMsg_S2C_RemoveGMCmdSuc::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddStringW(m_szName);
    IOBuff.AddUShort((unsigned short)m_listFailCmd.size());
    for (list<string>::iterator itr = m_listFailCmd.begin(); itr != m_listFailCmd.end(); ++itr) {
        IOBuff.AddStringW(itr->c_str());
    }

    return true;
}

GameMsg_S2C_RemoveGMCmdFail::GameMsg_S2C_RemoveGMCmdFail()
:GameMsg_Base(GM_C2S_RemoveGMCmdsFail)
, m_nErrno(0)
{
}

GameMsg_S2C_RemoveGMCmdFail::~GameMsg_S2C_RemoveGMCmdFail()
{
}

bool GameMsg_S2C_RemoveGMCmdFail::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddStringW(m_szName);
    IOBuff.AddUShort(m_nErrno);

    return true;
}


GameMsg_C2S_GMSendAnnounceOnce::GameMsg_C2S_GMSendAnnounceOnce()
:GameMsg_Base(GM_C2S_SendAnnounceOnce)
, m_nPID(0)
, m_strContent("")
, m_bIsAdminChat(false)
, m_bIsSysChat(false)
, m_nDelayTime(0)
{

}

GameMsg_C2S_GMSendAnnounceOnce::~GameMsg_C2S_GMSendAnnounceOnce()
{

}

bool GameMsg_C2S_GMSendAnnounceOnce::doDecode(CParamPool & IOBuff)
{
    m_nPID = IOBuff.GetUShort();
    IOBuff.GetStringW(m_strContent);
    m_bIsAdminChat = IOBuff.GetBool();
    m_bIsSysChat = IOBuff.GetBool();
    m_nDelayTime = IOBuff.GetInt();

    return true;
}

GameMsg_C2S_GMSendAnnounceInterval::GameMsg_C2S_GMSendAnnounceInterval()
:GameMsg_Base(GM_C2S_SendAnnounceInterval)
{

}

GameMsg_C2S_GMSendAnnounceInterval::~GameMsg_C2S_GMSendAnnounceInterval()
{

}

bool GameMsg_C2S_GMSendAnnounceInterval::doDecode(CParamPool &IOBuff)
{
    m_annouce.doDecode(IOBuff);

    return true;
}


GameMsg_S2C_GMSendAnnounceSuc::GameMsg_S2C_GMSendAnnounceSuc()
:GameMsg_Base(GM_S2C_SendAnnounceSuc)
{
}

GameMsg_S2C_GMSendAnnounceSuc::~GameMsg_S2C_GMSendAnnounceSuc()
{

}

bool GameMsg_S2C_GMSendAnnounceSuc::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nPID);
    IOBuff.AddStringW(m_strContent.c_str());
    IOBuff.AddBool(m_bIsAdminChat);
    IOBuff.AddBool(m_bIsSysChat);

    return true;
}


GameMsg_S2C_GMSendAnnounceFail::GameMsg_S2C_GMSendAnnounceFail()
:GameMsg_Base(GM_S2C_SendAnnounceFail)
{

}

GameMsg_S2C_GMSendAnnounceFail::~GameMsg_S2C_GMSendAnnounceFail()
{

}

bool GameMsg_S2C_GMSendAnnounceFail::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nPID);
    IOBuff.AddStringW(m_strContent.c_str());
    IOBuff.AddBool(m_bIsAdminChat);
    IOBuff.AddBool(m_bIsSysChat);

    return true;
}

GameMsg_C2S_GMLockOrUnLockAccount::GameMsg_C2S_GMLockOrUnLockAccount()
: GameMsg_Base(GM_C2S_GMLockOrUnLockAccount)
{
    m_nPid = 0;
    m_bLock = true;
    m_nLockTimeType = 0;
    m_nLockReason = 0;
}

GameMsg_C2S_GMLockOrUnLockAccount::~GameMsg_C2S_GMLockOrUnLockAccount()
{

}

bool GameMsg_C2S_GMLockOrUnLockAccount::doDecode(CParamPool &IOBuff)
{
    m_nPid = IOBuff.GetUShort();
    IOBuff.GetStringW(m_strPUID);
    m_bLock = IOBuff.GetBool();
    m_nLockTimeType = IOBuff.GetInt();
    m_nLockReason = IOBuff.GetShort();
    return true;
}

GameMsg_C2S_GMAddMCoin::GameMsg_C2S_GMAddMCoin()
: GameMsg_Base(GM_C2S_GMAddMCoin)
{
    m_nPid = 0;
    m_nAmount = 0;
    m_nBindAmount = 0;
    m_nCurrency = 0;
}
GameMsg_C2S_GMAddMCoin::~GameMsg_C2S_GMAddMCoin()
{
}

bool GameMsg_C2S_GMAddMCoin::doDecode(CParamPool &IOBuff)
{
    m_nPid = IOBuff.GetUShort();
    IOBuff.GetStringW(m_strPUID);
    m_nAmount = IOBuff.GetUInt();
    m_nBindAmount = IOBuff.GetUInt();
    m_nCurrency = IOBuff.GetUInt();
    return true;
}

GameMsg_C2S_SendCompensation::GameMsg_C2S_SendCompensation()
:GameMsg_Base(GM_C2S_SendCompensation)
, m_nMoney(0)
{
}

GameMsg_C2S_SendCompensation::~GameMsg_C2S_SendCompensation()
{

}

bool GameMsg_C2S_SendCompensation::doDecode(CParamPool &IOBuff)
{
    IOBuff.GetStringW(m_strEndTime);
    m_nMoney = IOBuff.GetInt();
    IOBuff.GetStringW(m_strTitle);
    IOBuff.GetStringW(m_strContent);
    IOBuff.GetStringW(m_strItemMale);
    IOBuff.GetStringW(m_strItemFemale);

    return true;
}

GameMsg_S2C_SendCompensationRes::GameMsg_S2C_SendCompensationRes()
: GameMsg_Base(GM_S2C_SendCompensationRes)
{

}

GameMsg_S2C_SendCompensationRes::~GameMsg_S2C_SendCompensationRes()
{

}

bool GameMsg_S2C_SendCompensationRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddStringW(m_strError.c_str());

    return true;
}

GameMsg_S2C_PrivError::GameMsg_S2C_PrivError()
: GameMsg_Base(GM_S2C_PrivError)
{

}

GameMsg_S2C_PrivError::~GameMsg_S2C_PrivError()
{

}

bool GameMsg_S2C_PrivError::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nErrorCode);
    return true;
}

GameMsg_C2S_GetUsers::GameMsg_C2S_GetUsers()
: GameMsg_Base(GM_C2S_GetUsers)
{

}

GameMsg_C2S_GetUsers::~GameMsg_C2S_GetUsers()
{

}

bool GameMsg_C2S_GetUsers::doDecode(CParamPool &IOBuff)
{
    return true;
}

GameMsg_S2C_GetUserRes::GameMsg_S2C_GetUserRes()
: GameMsg_Base(GM_S2C_GetUserRes)
{

}

GameMsg_S2C_GetUserRes::~GameMsg_S2C_GetUserRes()
{

}

void GameMsg_S2C_GetUserRes::AddUser(std::string& user, int priv)
{
    m_UserPrivList.insert(make_pair(user, priv));
}

bool GameMsg_S2C_GetUserRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_UserPrivList.size());
    std::map<std::string, int>::iterator it = m_UserPrivList.begin();
    for (; it != m_UserPrivList.end(); it++)
    {
        IOBuff.AddStringW(it->first);
        IOBuff.AddInt(it->second);
    }
    return true;
}

GameMsg_C2S_SetPriv::GameMsg_C2S_SetPriv()
: GameMsg_Base(GM_C2S_SetPriv)
{

}

GameMsg_C2S_SetPriv::~GameMsg_C2S_SetPriv()
{

}

bool GameMsg_C2S_SetPriv::doDecode(CParamPool &IOBuff)
{
    IOBuff.GetStringW(m_UserName);
    m_Priv = IOBuff.GetInt();

    return true;
}

GameMsg_S2C_SetPrivRes::GameMsg_S2C_SetPrivRes()
: GameMsg_Base(GM_S2C_SetPrivRes)
{

}

GameMsg_S2C_SetPrivRes::~GameMsg_S2C_SetPrivRes()
{

}

bool GameMsg_S2C_SetPrivRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nRes);
    return true;
}

GameMsg_C2S_AddUser::GameMsg_C2S_AddUser()
: GameMsg_Base(GM_C2S_AddUser)
{

}

GameMsg_C2S_AddUser::~GameMsg_C2S_AddUser()
{

}

bool GameMsg_C2S_AddUser::doDecode(CParamPool &IOBuff)
{
    IOBuff.GetStringW(m_UserName);
    IOBuff.GetStringW(m_Password);
    return true;
}

GameMsg_S2C_AddUserRes::GameMsg_S2C_AddUserRes()
: GameMsg_Base(GM_S2C_AddUserRes)
{

}

GameMsg_S2C_AddUserRes::~GameMsg_S2C_AddUserRes()
{

}

bool GameMsg_S2C_AddUserRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nRes);
    IOBuff.AddStringW(m_UserName);
    IOBuff.AddInt(m_Priv);
    return true;
}

GameMsg_C2S_DelUser::GameMsg_C2S_DelUser()
: GameMsg_Base(GM_C2S_DelUser)
{

}

GameMsg_C2S_DelUser::~GameMsg_C2S_DelUser()
{

}

bool GameMsg_C2S_DelUser::doDecode(CParamPool &IOBuff)
{
    IOBuff.GetStringW(m_UserName);
    return true;
}

GameMsg_S2C_DelUserRes::GameMsg_S2C_DelUserRes()
: GameMsg_Base(GM_S2C_DelUserRes)
{
    m_nRes = 0;
}

GameMsg_S2C_DelUserRes::~GameMsg_S2C_DelUserRes()
{

}

bool GameMsg_S2C_DelUserRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nRes);
    return true;
}


std::string GameMsg_C2S_GMSendMail::ToJson()
{

    std::stringstream strStream;
    strStream << "[";
    list<CRcvSysMailRoleInfo>::iterator it = m_listRcvRoleInfo.begin();
    for (; it != m_listRcvRoleInfo.end(); it++)
    {
        if (it == m_listRcvRoleInfo.begin())
        {
            strStream << "{";
        }
        else
        {
            strStream << ",{";
        }
        CRcvSysMailRoleInfo& mailInfo = *it;
        strStream << "'title' : '" << mailInfo.m_MailInfo.m_strTitle.c_str() << "' , "
            << "'content' : '" << mailInfo.m_MailInfo.m_strContent.c_str() << "' , "
            << "'money' : '" << mailInfo.m_MailInfo.m_nMoney << "' , "
            << "'itemtype' : '" << mailInfo.m_MailInfo.m_nItemType << "' , "
            << "'itemcount' : '" << mailInfo.m_MailInfo.m_nItemCount << "' , "
            << "'itemduration' : '" << mailInfo.m_MailInfo.m_nItemDuration << "' , "
            << "'pid' : '" << mailInfo.m_nPid << "' , "
            << "'puid' : '" << mailInfo.m_strPUID.c_str() << "'}";
    }
    strStream << "]";

    return strStream.str();
}

std::string GameMsg_C2S_QueryRangeGift::ToJson()
{
    char szBuff[1024];
    sprintf(szBuff, "{'giftid' : '%u' , 'starttime' : '%s' , 'endtime' : '%s'}", m_nGiftID, m_strStartTime.c_str(), m_strEndTime.c_str());
    std::string szJson = szBuff;
    return szJson;
}

std::string GameMsg_C2S_QueryMultiGift::ToJson()
{
    std::string szJson = "[";
    std::list<std::string>::iterator it = m_listGiftCodes.begin();
    for (; it != m_listGiftCodes.end(); it++)
    {
        if (it == m_listGiftCodes.begin())
        {
            szJson += "{";
        }
        else
        {
            szJson += ",{";
        }
        szJson += "'code' : '";
        szJson += *it;
        szJson += "'}";
    }
    szJson += "]";
    return szJson;
}

std::string GameMsg_C2S_GMSendAnnounceOnce::ToJson()
{
    char szBuff[10240];
    sprintf(szBuff, "{'pid':'%d','content':'%s','isadminchat' : '%d' , 'issyschat' : '%d' , 'delaytime' : '%d'}",
        m_nPID, m_strContent.c_str(), (int)m_bIsAdminChat, (int)m_bIsSysChat, m_nDelayTime);
    std::string szJson = szBuff;
    return szJson;
}

std::string GameMsg_C2S_GMSendAnnounceInterval::ToJson()
{
    stringstream ss;
    ss << "{'pid':'" << m_annouce.m_nPID << "','content':'" << m_annouce.m_strContent << "','isadminchat' : '" << (int)m_annouce.m_bIsAdminChat
        << "' , 'issyschat' : '" << (int)m_annouce.m_bIsSysChat << "','starttime' : '" << m_annouce.m_nStartTime << "' , 'endtime' : '" << m_annouce.m_nEndTime
        << "','interval':'" << m_annouce.m_nSendInterval << "','id':'" << m_annouce.m_nID << "','lastsendtime':'" << m_annouce.m_nLastSendTime
        << "','state':'" << m_annouce.m_nState << "'}";

    return ss.str();

}

string GameMsg_C2S_GMLockOrUnLockAccount::ToJson()
{
    char szBuff[1024];
    sprintf(szBuff, "{'pid':'%d','puid':'%s','block':'%d','locktimetype':'%d'}",
        m_nPid, m_strPUID.c_str(), (int)m_bLock, m_nLockTimeType);
    std::string szJson = szBuff;
    return szJson;
}

string GameMsg_C2S_GMAddMCoin::ToJson()
{
    char szBuff[1024];
    sprintf(szBuff, "{'pid':'%d' , 'puid' : '%s' , 'amount' : '%d' , 'bindamount' : '%d'}",
        m_nPid, m_strPUID.c_str(), m_nAmount, m_nBindAmount);
    std::string szJson = szBuff;
    return szJson;
}

string GameMsg_C2S_SendCompensation::ToJson()
{
    stringstream ss;

    ss << "{'endtime':'" << m_strEndTime.c_str() << "','money':'" << m_nMoney << "','title'='" << m_strTitle.c_str() << "','content'='"
        << m_strContent.c_str() << "','itemmale'='" << m_strItemMale.c_str() << "','itemfemale'='" << m_strItemFemale.c_str() << "'}";

    return ss.str();
}

string GameMsg_C2S_SetPriv::ToJson()
{
    char szBuff[1024];
    sprintf(szBuff, "{'username':'%s' , 'priv' : '%d'}", m_UserName.c_str(), m_Priv);
    std::string szJson = szBuff;
    return szJson;
}

string GameMsg_C2S_AddUser::ToJson()
{
    char szBuff[1024];
    sprintf(szBuff, "{'username':'%s' , 'password' : '%s'}", m_UserName.c_str(), m_Password.c_str());
    std::string szJson = szBuff;
    return szJson;
}

string GameMsg_C2S_DelUser::ToJson()
{
    char szBuff[1024];
    sprintf(szBuff, "{'username':'%s'}", m_UserName.c_str());
    std::string szJson = szBuff;
    return szJson;
}

GameMsg_C2S_GMGetServerLine::GameMsg_C2S_GMGetServerLine()
: GameMsg_Base(GM_C2S_GetServerLine)
{

}

GameMsg_C2S_GMGetServerLine::~GameMsg_C2S_GMGetServerLine()
{

}

bool GameMsg_C2S_GMGetServerLine::doDecode(CParamPool &IOBuff)
{
    return true;
}

std::string GameMsg_C2S_GMGetServerLine::ToJson()
{
    return "{}";
}

GameMsg_S2C_GMGetServerLineRes::GameMsg_S2C_GMGetServerLineRes()
: GameMsg_Base(GM_S2C_GetServerLineRes)
{

}

GameMsg_S2C_GMGetServerLineRes::~GameMsg_S2C_GMGetServerLineRes()
{

}

bool GameMsg_S2C_GMGetServerLineRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_Lines.size());
    std::map<int, std::string>::iterator it = m_Lines.begin();
    for (; it != m_Lines.end(); it++)
    {
        IOBuff.AddInt(it->first);
        IOBuff.AddStringW(it->second);
    }
    return true;
}

GameMsg_C2S_GMGetServerRoomList::GameMsg_C2S_GMGetServerRoomList()
: GameMsg_Base(GM_C2S_GetServerRoomList)
{
    m_nLine = 0;
}

GameMsg_C2S_GMGetServerRoomList::~GameMsg_C2S_GMGetServerRoomList()
{

}

bool GameMsg_C2S_GMGetServerRoomList::doDecode(CParamPool &IOBuff)
{
    m_nLine = IOBuff.GetInt();
    m_GMAskRoomType = IOBuff.GetUShort();
    return true;
}

std::string GameMsg_C2S_GMGetServerRoomList::ToJson()
{
    char szBuff[100];
    sprintf(szBuff, "{'line':'%d','roomtype':'%d'}", m_nLine, (int)m_GMAskRoomType);
    std::string szJson = szBuff;
    return szJson;
}

GameMsg_S2C_GMGetServerRoomListRes::GameMsg_S2C_GMGetServerRoomListRes()
: GameMsg_Base(GM_S2C_GetServerRoomListRes)
{

}

GameMsg_S2C_GMGetServerRoomListRes::~GameMsg_S2C_GMGetServerRoomListRes()
{

}

bool GameMsg_S2C_GMGetServerRoomListRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_RoomList.size());
    std::vector<GMRoomInfo>::iterator it = m_RoomList.begin();
    for (; it != m_RoomList.end(); it++)
    {
        it->doEncode(IOBuff);
    }
    return true;
}

GameMsg_C2S_GMSetMallUpdateTime::GameMsg_C2S_GMSetMallUpdateTime()
: GameMsg_Base(GM_C2S_SetMallUpdateTime)
{

}

GameMsg_C2S_GMSetMallUpdateTime::~GameMsg_C2S_GMSetMallUpdateTime()
{

}

bool GameMsg_C2S_GMSetMallUpdateTime::doDecode(CParamPool &IOBuff)
{
    m_UpdateTime = IOBuff.GetUInt();
    return true;
}

std::string GameMsg_C2S_GMSetMallUpdateTime::ToJson()
{
    char szBuff[100];
    sprintf(szBuff, "{'mallupdatetime':'%d'}", m_UpdateTime);
    std::string szJson = szBuff;
    return szJson;
}

GameMsg_C2S_GMGroupBuyControl::GameMsg_C2S_GMGroupBuyControl()
: GameMsg_Base(GM_C2S_GroupBuyControl)
{

}

GameMsg_C2S_GMGroupBuyControl::~GameMsg_C2S_GMGroupBuyControl()
{

}

bool GameMsg_C2S_GMGroupBuyControl::doDecode(CParamPool &IOBuff)
{
    m_nLine = IOBuff.GetInt();
    m_nStartTime = IOBuff.GetUInt();
    m_nEndTime = IOBuff.GetUInt();
    m_nInterval = IOBuff.GetUInt();
    m_nMaxPeople = IOBuff.GetUInt();
    m_nMinPeople = IOBuff.GetUInt();

    return true;
}

std::string GameMsg_C2S_GMGroupBuyControl::ToJson()
{
    stringstream ss;
    ss << "{'line':'" << m_nLine << "','startTime':'" << m_nStartTime
        << "','endTime':'" << m_nEndTime << "','interval':'" << m_nInterval
        << "','maxPeople':'" << m_nMaxPeople << "','minPeople' : '" << m_nMinPeople << "'}";
    std::string str = ss.str();
    return str;
}

GameMsg_S2C_SetMallUpdateTimeRet::GameMsg_S2C_SetMallUpdateTimeRet()
: GameMsg_Base(GM_S2C_SetMallUpdateTimeRet)
{

}

GameMsg_S2C_SetMallUpdateTimeRet::~GameMsg_S2C_SetMallUpdateTimeRet()
{

}

bool GameMsg_S2C_SetMallUpdateTimeRet::doEncode(CParamPool &IOBuff)
{
    return true;
}

GameMsg_S2C_GroupBuyControlRet::GameMsg_S2C_GroupBuyControlRet()
: GameMsg_Base(GM_S2C_GroupBuyControlRet)
{

}

GameMsg_S2C_GroupBuyControlRet::~GameMsg_S2C_GroupBuyControlRet()
{

}

bool GameMsg_S2C_GroupBuyControlRet::doEncode(CParamPool &IOBuff)
{
    return true;
}

GameMsg_C2S_ChangeSelfPassword::GameMsg_C2S_ChangeSelfPassword()
: GameMsg_Base(GM_C2S_ChangeSelfPassword)
{

}

GameMsg_C2S_ChangeSelfPassword::~GameMsg_C2S_ChangeSelfPassword()
{

}

bool GameMsg_C2S_ChangeSelfPassword::doDecode(CParamPool &IOBuff)
{
    IOBuff.GetStringW(m_OldPassword);
    IOBuff.GetStringW(m_NewPassword);
    return true;
}

GameMsg_S2C_ChangeSelfPasswordRet::GameMsg_S2C_ChangeSelfPasswordRet()
: GameMsg_Base(GM_S2C_ChangeSelfPasswordRet)
{
    m_bResult = false;
}

GameMsg_S2C_ChangeSelfPasswordRet::~GameMsg_S2C_ChangeSelfPasswordRet()
{

}

bool GameMsg_S2C_ChangeSelfPasswordRet::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddBool(m_bResult);
    return true;
}

GameMsg_C2S_ChangePassword::GameMsg_C2S_ChangePassword()
: GameMsg_Base(GM_C2S_ChangePassword)
{

}

GameMsg_C2S_ChangePassword::~GameMsg_C2S_ChangePassword()
{

}

bool GameMsg_C2S_ChangePassword::doDecode(CParamPool &IOBuff)
{
    IOBuff.GetStringW(m_UserName);
    IOBuff.GetStringW(m_Password);
    return true;
}

GameMsg_C2S_GMSendPrivMCoin::GameMsg_C2S_GMSendPrivMCoin()
: GameMsg_Base(GM_C2S_SendPrivMCoin)
{

}

GameMsg_C2S_GMSendPrivMCoin::~GameMsg_C2S_GMSendPrivMCoin()
{

}

bool GameMsg_C2S_GMSendPrivMCoin::doDecode(CParamPool &IOBuff)
{
    unsigned short nCount = IOBuff.GetUShort();
    for (unsigned short i = 0; i < nCount; i++)
    {
        PlayerMoneyInfo pmi;
        pmi.doDecode(IOBuff);

        m_PlayerMoneyInfo.push_back(pmi);
    }
    return true;
}

std::string GameMsg_C2S_GMSendPrivMCoin::ToJson()
{
    stringstream ss;
    ss << "[";
    for (unsigned short i = 0; i < (unsigned short)m_PlayerMoneyInfo.size(); i++)
    {
        ss << "{ 'puid' : '" << m_PlayerMoneyInfo[i].m_PlayerPUID << "' , 'money' : '"
            << m_PlayerMoneyInfo[i].m_nMoney << "' , 'bindmoney' : '" << m_PlayerMoneyInfo[i].m_nBindMoney << "' }";
        if (i != m_PlayerMoneyInfo.size() - 1)
        {
            ss << " , ";
        }
    }
    ss << "]";
    return ss.str();
}

GameMsg_S2C_SendPrivMCoinRet::GameMsg_S2C_SendPrivMCoinRet()
: GameMsg_Base(GM_S2C_SendPrivMCoinRet)
{

}

GameMsg_S2C_SendPrivMCoinRet::~GameMsg_S2C_SendPrivMCoinRet()
{

}

bool GameMsg_S2C_SendPrivMCoinRet::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_PlayerResult.size());
    std::vector<GMRechargeResult>::iterator it = m_PlayerResult.begin();
    for (; it != m_PlayerResult.end(); it++)
    {
        it->doEncode(IOBuff);
    }
    return true;
}

GameMsg_C2S_GetServerPlayerInfo::GameMsg_C2S_GetServerPlayerInfo()
: GameMsg_Base(GM_C2S_GetServerPlayerInfo)
{

}

GameMsg_C2S_GetServerPlayerInfo::~GameMsg_C2S_GetServerPlayerInfo()
{

}

bool GameMsg_C2S_GetServerPlayerInfo::doDecode(CParamPool &IOBuff)
{
    return true;
}

GameMsg_S2C_GetServerPlayerInfoRet::GameMsg_S2C_GetServerPlayerInfoRet()
: GameMsg_Base(GM_S2C_GetServerPlayerInfoRet)
{

}

GameMsg_S2C_GetServerPlayerInfoRet::~GameMsg_S2C_GetServerPlayerInfoRet()
{

}

bool GameMsg_S2C_GetServerPlayerInfoRet::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_ServerPlayerCountInfoList.size());
    vector<CServerPlayerCountInfo>::iterator it = m_ServerPlayerCountInfoList.begin();
    for (; it != m_ServerPlayerCountInfoList.end(); it++)
    {
        it->doEncode(IOBuff);
    }
    return true;
}

GameMsg_C2S_SetServerPlayerInfo::GameMsg_C2S_SetServerPlayerInfo()
: GameMsg_Base(GM_C2S_SetServerPlayerInfo)
{

}

GameMsg_C2S_SetServerPlayerInfo::~GameMsg_C2S_SetServerPlayerInfo()
{

}

bool GameMsg_C2S_SetServerPlayerInfo::doDecode(CParamPool &IOBuff)
{
    m_nLimit = IOBuff.GetUShort();
    return true;
}

GameMsg_S2C_SetServerPlayerInfoRet::GameMsg_S2C_SetServerPlayerInfoRet()
: GameMsg_Base(GM_S2C_SetServerPlayerInfoRet)
{

}

GameMsg_S2C_SetServerPlayerInfoRet::~GameMsg_S2C_SetServerPlayerInfoRet()
{

}

bool GameMsg_S2C_SetServerPlayerInfoRet::doEncode(CParamPool &IOBuff)
{
    return true;
}

GameMsg_S2C_GetServerPlayerLimit::GameMsg_S2C_GetServerPlayerLimit()
: GameMsg_Base(GM_S2C_ServerPlayerLimit)
{

}

GameMsg_S2C_GetServerPlayerLimit::~GameMsg_S2C_GetServerPlayerLimit()
{

}

bool GameMsg_S2C_GetServerPlayerLimit::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nLimit);
    return true;
}

GameMsg_C2S_SendBlackDeviceList::GameMsg_C2S_SendBlackDeviceList()
: GameMsg_Base(GM_C2S_SendBlackDeviceList)
{
    m_bFrozenDevice = true;
}

GameMsg_C2S_SendBlackDeviceList::~GameMsg_C2S_SendBlackDeviceList()
{

}

bool GameMsg_C2S_SendBlackDeviceList::doDecode(CParamPool &IOBuff)
{
    m_bFrozenDevice = IOBuff.GetBool();
    unsigned short nNameCount = IOBuff.GetUShort();
    for (unsigned short i = 0; i < nNameCount; i++)
    {
        std::string strName;
        IOBuff.GetStringW(strName);
        m_DeviceNameList.push_back(strName);
    }
    return true;
}

std::string GameMsg_C2S_SendBlackDeviceList::ToJson()
{
    stringstream ss;
    ss << "[";
    for (unsigned short i = 0; i < (unsigned short)m_DeviceNameList.size(); i++)
    {
        ss << "{ 'name' : '" << m_DeviceNameList[i] << "'";
        if (i != m_DeviceNameList.size() - 1)
        {
            ss << " , ";
        }
    }
    ss << "]";

    return ss.str();
}

GameMsg_S2C_SendBlackDeviceListRes::GameMsg_S2C_SendBlackDeviceListRes()
: GameMsg_Base(GM_S2C_SendBlackDeviceListRes)
{

}

GameMsg_S2C_SendBlackDeviceListRes::~GameMsg_S2C_SendBlackDeviceListRes()
{

}

bool GameMsg_S2C_SendBlackDeviceListRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_Result);

    return true;
}

GameMsg_C2S_ChangeAccountPassword::GameMsg_C2S_ChangeAccountPassword()
: GameMsg_Base(GM_C2S_ChangeAccountPassword)
{

}

GameMsg_C2S_ChangeAccountPassword::~GameMsg_C2S_ChangeAccountPassword()
{

}

bool GameMsg_C2S_ChangeAccountPassword::doDecode(CParamPool &IOBuff)
{
    IOBuff.GetStringW(m_AccountName);
    IOBuff.GetStringW(m_Password);

    return true;
}

GameMsg_S2C_ChangeAccountPasswordRes::GameMsg_S2C_ChangeAccountPasswordRes()
: GameMsg_Base(GM_S2C_ChangeAccountPasswordRes)
{

}

GameMsg_S2C_ChangeAccountPasswordRes::~GameMsg_S2C_ChangeAccountPasswordRes()
{

}

bool GameMsg_S2C_ChangeAccountPasswordRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_Result);
    return true;
}

GameMsg_C2S_DeleteAnnounce::GameMsg_C2S_DeleteAnnounce()
: GameMsg_Base(GM_C2S_DeleteAnnounce)
{

}

GameMsg_C2S_DeleteAnnounce::~GameMsg_C2S_DeleteAnnounce()
{

}

bool GameMsg_C2S_DeleteAnnounce::doDecode(CParamPool &IOBuff)
{
    m_nAnnounceID = IOBuff.GetInt();

    return true;
}

GameMsg_S2C_DeleteAnnounceRes::GameMsg_S2C_DeleteAnnounceRes()
: GameMsg_Base(GM_S2C_DeleteAnnounceRes)
{

}

GameMsg_S2C_DeleteAnnounceRes::~GameMsg_S2C_DeleteAnnounceRes()
{

}

bool GameMsg_S2C_DeleteAnnounceRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddBool(m_bResult);
    return true;
}

GameMsg_C2S_GetAnnounce::GameMsg_C2S_GetAnnounce()
: GameMsg_Base(GM_C2S_GetAnnounce)
{

}

GameMsg_C2S_GetAnnounce::~GameMsg_C2S_GetAnnounce()
{

}

bool GameMsg_C2S_GetAnnounce::doDecode(CParamPool &IOBuff)
{
    return true;
}

GameMsg_S2C_GetAnnounceRes::GameMsg_S2C_GetAnnounceRes()
: GameMsg_Base(GM_S2C_GetAnnounceRes)
{

}

GameMsg_S2C_GetAnnounceRes::~GameMsg_S2C_GetAnnounceRes()
{

}

bool GameMsg_S2C_GetAnnounceRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_AnnounceInfoList.size());
    std::vector<CAnnounceInfo>::iterator it = m_AnnounceInfoList.begin();
    for (; it != m_AnnounceInfoList.end(); it++)
    {
        CAnnounceInfo& announceInfo = *it;
        IOBuff.AddUInt(announceInfo.m_nID);
        IOBuff.AddInt(announceInfo.m_nSendInterval);
        IOBuff.AddUInt(announceInfo.m_nStartTime);
        IOBuff.AddUInt(announceInfo.m_nEndTime);
        IOBuff.AddStringW(announceInfo.m_strContent);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////
// query log.
GameMsg_C2S_QueryLog::GameMsg_C2S_QueryLog()
    : GameMsg_Base(GM_C2S_QueryLog)
    , m_eActionType(EGMActionType_None)
    , m_nStartTime(0)
    , m_nEndTime(0)
{

}
GameMsg_C2S_QueryLog::~GameMsg_C2S_QueryLog()
{

}
bool GameMsg_C2S_QueryLog::doDecode(CParamPool &IOBuff)
{
    m_eActionType = (EGMActionType)IOBuff.GetUChar();
    m_nStartTime = IOBuff.GetUInt();
    m_nEndTime = IOBuff.GetUInt();

    return true;
}

QueryLogInfo::QueryLogInfo()
    : m_nLogID(0)
    , m_nOperTime(0)
{

}
QueryLogInfo::~QueryLogInfo(){}
bool QueryLogInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nLogID);
    IOBuff.AddStringW(m_strAccouont);
    IOBuff.AddUInt(m_nOperTime);
    IOBuff.AddStringW(m_strOperDesc);

    return true;
}


GameMsg_S2C_QueryLogRet::GameMsg_S2C_QueryLogRet()
    : GameMsg_Base(GM_S2C_QueryLogRet)
    , m_nLogNum(0)
    , m_bEndMsg(true)
{

}

GameMsg_S2C_QueryLogRet::~GameMsg_S2C_QueryLogRet()
{

}

bool GameMsg_S2C_QueryLogRet::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nLogNum);
    IOBuff.AddBool(m_bEndMsg);

    IOBuff.AddUShort(m_logInfo.size());
    for (std::list<QueryLogInfo>::iterator iter = m_logInfo.begin(); iter != m_logInfo.end(); ++iter)
    {
        iter->doEncode(IOBuff);
    }

    return true;
}


GameMsg_C2S_QueryLogItem::GameMsg_C2S_QueryLogItem()
    : GameMsg_Base(GM_C2S_QueryLogItem)
    , m_nLogID(0)
{}
GameMsg_C2S_QueryLogItem::~GameMsg_C2S_QueryLogItem()
{}
bool GameMsg_C2S_QueryLogItem::doDecode(CParamPool &IOBuff)
{
    m_nLogID = IOBuff.GetUInt();
    return true;
}


GameMsg_S2C_QueryLogItemRet::GameMsg_S2C_QueryLogItemRet()
    : GameMsg_Base(GM_S2C_QueryLogItemRet)
    , m_nLogNum(0)
    , m_bEndMsg(false)
{}
GameMsg_S2C_QueryLogItemRet::~GameMsg_S2C_QueryLogItemRet()
{

}
bool GameMsg_S2C_QueryLogItemRet::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nLogNum);
    IOBuff.AddBool(m_bEndMsg);
    IOBuff.AddUShort(m_logDesc.size());
    for (std::list<std::string>::iterator iter = m_logDesc.begin(); iter != m_logDesc.end(); ++iter)
    {
        IOBuff.AddStringW(*iter);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////
// infoCenter
InfoCenterItemCM::InfoCenterItemCM()
: m_id(0)
, m_updateTime(0)
, m_group(0)
, m_type(0)
{
}

bool InfoCenterItemCM::doDecode(CParamPool &IOBuff)
{
    m_id = IOBuff.GetUInt();
    m_updateTime = IOBuff.GetUInt();
    m_group = IOBuff.GetUChar();
    IOBuff.GetStringW(m_icon);
    m_type = IOBuff.GetUChar();
    IOBuff.GetStringW(m_strTitle);
    IOBuff.GetStringW(m_strContext);
    IOBuff.GetStringW(m_strHref);
    IOBuff.GetStringW(m_strHidPid);

    return true;
}

bool InfoCenterItemCM::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_id);
    IOBuff.AddUInt(m_updateTime);
    IOBuff.AddUChar(m_group);
    IOBuff.AddStringW(m_icon);
    IOBuff.AddUChar(m_type);
    IOBuff.AddStringW(m_strTitle);
    IOBuff.AddStringW(m_strContext);
    IOBuff.AddStringW(m_strHref);
    IOBuff.AddStringW(m_strHidPid);

    return true;
}


GameMsg_C2M_GetInfoCenterResCM::GameMsg_C2M_GetInfoCenterResCM()
: GameMsg_Base(GM_C2M_GetInfoCenterRes)
{
    return;
}
GameMsg_C2M_GetInfoCenterResCM::~GameMsg_C2M_GetInfoCenterResCM()
{
    return;
}
bool GameMsg_C2M_GetInfoCenterResCM::doDecode(CParamPool &IOBuff)
{
    return true;
}

GameMsg_M2C_GetInfoCenterResCM::GameMsg_M2C_GetInfoCenterResCM()
: GameMsg_Base(GM_M2C_GetInfoCenterRes)

{

}

GameMsg_M2C_GetInfoCenterResCM::~GameMsg_M2C_GetInfoCenterResCM()
{

}
bool GameMsg_M2C_GetInfoCenterResCM::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_infos.size());
    for (size_t i = 0; i < m_infos.size(); ++i)
    {
        m_infos[i].doEncode(IOBuff);
    }

    return true;
}

GameMsg_C2M_AddInfoCenterResCM::GameMsg_C2M_AddInfoCenterResCM()
: GameMsg_Base(GM_C2M_AddInfoCenterRes)
{

}
GameMsg_C2M_AddInfoCenterResCM::~GameMsg_C2M_AddInfoCenterResCM()
{

}
bool GameMsg_C2M_AddInfoCenterResCM::doDecode(CParamPool &IOBuff)
{
    return m_items.doDecode(IOBuff);
}


GameMsg_M2C_AddInfoCenterResCM::GameMsg_M2C_AddInfoCenterResCM()
: GameMsg_Base(GM_M2C_AddInfoCenterRes)
{

}

GameMsg_M2C_AddInfoCenterResCM::~GameMsg_M2C_AddInfoCenterResCM()
{

}
bool GameMsg_M2C_AddInfoCenterResCM::doEncode(CParamPool &IOBuff)
{
    return m_items.doEncode(IOBuff);
}


GameMsg_C2M_UpdateInfoCenterResCM::GameMsg_C2M_UpdateInfoCenterResCM()
: GameMsg_Base(GM_C2M_UpdateInfoCenterRes)
{

}
GameMsg_C2M_UpdateInfoCenterResCM::~GameMsg_C2M_UpdateInfoCenterResCM()
{

}
bool GameMsg_C2M_UpdateInfoCenterResCM::doDecode(CParamPool &IOBuff)
{
    return m_items.doDecode(IOBuff);
}

GameMsg_M2C_UpdateInfoCenterResCM::GameMsg_M2C_UpdateInfoCenterResCM()
: GameMsg_Base(GM_M2C_UpdateInfoCenterRes)
{

}
GameMsg_M2C_UpdateInfoCenterResCM::~GameMsg_M2C_UpdateInfoCenterResCM()
{

}
bool GameMsg_M2C_UpdateInfoCenterResCM::doEncode(CParamPool &IOBuff)
{
    return m_items.doEncode(IOBuff);
}

GameMsg_C2M_RemoveInfoCenterResCM::GameMsg_C2M_RemoveInfoCenterResCM()
: GameMsg_Base(GM_C2M_RemoveInfoCenterRes)
, m_id(0)
{

}

GameMsg_C2M_RemoveInfoCenterResCM::~GameMsg_C2M_RemoveInfoCenterResCM()
{

}

bool GameMsg_C2M_RemoveInfoCenterResCM::doDecode(CParamPool &IOBuff)
{
    m_id = IOBuff.GetUInt();
    return true;
}

GameMsg_M2C_RemoveInfoCenterResCM::GameMsg_M2C_RemoveInfoCenterResCM()
: GameMsg_Base(GM_M2C_RemoveInfoCenterRes)
, m_result(0)
, m_id(0)
{

}

GameMsg_M2C_RemoveInfoCenterResCM::~GameMsg_M2C_RemoveInfoCenterResCM()
{

}

bool GameMsg_M2C_RemoveInfoCenterResCM::doEncode(CParamPool &IOBuff)
{
    m_result = IOBuff.GetUShort();
    m_id = IOBuff.GetUInt();

    return true;
}

GameMsg_M2G_DeleteVersionMedal::GameMsg_M2G_DeleteVersionMedal()
:GameMsg_Base(GM_M2G_DeleteVersionMedal)
{

}

GameMsg_G2M_DeleteVersionMedal::GameMsg_G2M_DeleteVersionMedal()
: GameMsg_Base(GM_G2M_DeleteVersionMedal)
{

}

bool GameMsg_G2M_DeleteVersionMedal::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nRetCode);
    return true;
}

GameMsg_C2M_AddMedal::GameMsg_C2M_AddMedal()
:GameMsg_Base(GM_C2M_AddMedal)
{

}

bool GameMsg_C2M_AddMedal::doDecode(CParamPool &IOBuff)
{
    m_nPlayerID = IOBuff.GetInt();
    m_nMedalID = IOBuff.GetInt();
    return true;
}

bool GameMsg_C2M_AddMedal::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nPlayerID);
    IOBuff.AddInt(m_nMedalID);
    return true;
}

GameMsg_M2C_AddMedal::GameMsg_M2C_AddMedal()
:GameMsg_Base(GM_M2C_AddMedal)
{

}

bool GameMsg_M2C_AddMedal::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nRetCode);
    return true;
}

bool GameMsg_M2C_AddMedal::doDecode(CParamPool &IOBuff)
{
    m_nRetCode = IOBuff.GetInt();
    return true;
}


GameMsg_C2M_RequestLockList::GameMsg_C2M_RequestLockList()
: GameMsg_Base(GM_C2M_RequestLockList)
, m_nStartTime(0)
, m_nEndTime(0)
{

}

bool GameMsg_C2M_RequestLockList::doDecode(CParamPool &IOBuff)
{
    m_nStartTime = IOBuff.GetUInt();
    m_nEndTime = IOBuff.GetUInt();
    return true;
}

bool GameMsg_C2M_RequestLockList::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nStartTime);
    IOBuff.AddUInt(m_nEndTime);

    return true;
}


GameMsg_M2C_ResponseLockList::GameMsg_M2C_ResponseLockList()
: GameMsg_Base(GM_M2C_ResponseLockList)
{

}

bool GameMsg_M2C_ResponseLockList::doDecode(CParamPool &IOBuff)
{
    unsigned short len = IOBuff.GetUShort();
    for (unsigned short i = 0; i < len; ++i)
    {
        PlayerLockedItem item;
        item.doDecode(IOBuff);

        m_lstLockItem.push_back(item); // Ìí¼Ó
    }

    return true;
}

bool GameMsg_M2C_ResponseLockList::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_lstLockItem.size());
    for (std::list<PlayerLockedItem>::iterator iter = m_lstLockItem.begin(); iter != m_lstLockItem.end(); ++iter)
    {
        iter->doEncode(IOBuff);
    }

    return true;
}

GameMsg_C2M_HarassList::GameMsg_C2M_HarassList()
: GameMsg_Base(GM_Msg_C2M_HarassList)
, m_nStartTime(0)
, m_nEndTime(0)
{

}

bool GameMsg_C2M_HarassList::doDecode(CParamPool &IOBuff)
{
    m_nStartTime = IOBuff.GetUInt();
    m_nEndTime = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strHarassInfo);

    return true;
}

bool GameMsg_C2M_HarassList::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nStartTime);
    IOBuff.AddUInt(m_nEndTime);
    IOBuff.AddStringW(m_strHarassInfo);

    return true;
}

GameMsg_M2C_HarassList::GameMsg_M2C_HarassList()
: GameMsg_Base(GM_Msg_M2C_HarassList)
{

}

bool GameMsg_M2C_HarassList::doDecode(CParamPool &IOBuff)
{
    unsigned short len = IOBuff.GetUShort();

    for (unsigned short i = 0; i < len; ++i)
    {
        RoleHarassInfo ri;
        ri.doDecode(IOBuff);
        m_lstRoleHarass.push_back(ri);
    }

    return true;
}

bool GameMsg_M2C_HarassList::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_lstRoleHarass.size());
    for (std::list<RoleHarassInfo>::iterator iter = m_lstRoleHarass.begin(); iter != m_lstRoleHarass.end(); ++iter)
    {
        iter->doEncode(IOBuff);
    }

    return true;
}

GameMsg_C2M_RoleHarassChatInfo::GameMsg_C2M_RoleHarassChatInfo()
: GameMsg_Base(GM_Msg_C2M_RoleHarassChatInfo)
, m_nRoleID(0)
, m_nStartTime(0)
, m_nEndTime(0)
{

}

bool GameMsg_C2M_RoleHarassChatInfo::doDecode(CParamPool &IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    m_nStartTime = IOBuff.GetUInt();
    m_nEndTime = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strHarassInfo);

    return true;
}

bool GameMsg_C2M_RoleHarassChatInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddUInt(m_nStartTime);
    IOBuff.AddUInt(m_nEndTime);

    IOBuff.AddStringW(m_strHarassInfo);

    return true;
}

GameMsg_M2C_RoleHarassChatInfo::GameMsg_M2C_RoleHarassChatInfo()
: GameMsg_Base(GM_Msg_M2C_RoleHarassChatInfo)
, m_nPlatformID(0)
, m_nRoleID(0)
{
}

bool GameMsg_M2C_RoleHarassChatInfo::doDecode(CParamPool &IOBuff)
{
    m_nPlatformID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strPuid);
    m_nRoleID = IOBuff.GetUInt();


    unsigned short len = IOBuff.GetUShort();
    for (unsigned short i = 0; i < len; ++i)
    {
        RoleChatInfo ri;
        ri.doDecode(IOBuff);
        m_lstRoleChatInfo.push_back(ri);
    }

    return true;
}

bool GameMsg_M2C_RoleHarassChatInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nPlatformID);
    IOBuff.AddStringW(m_strPuid);
    IOBuff.AddUInt(m_nRoleID);

    IOBuff.AddUShort((unsigned short)m_lstRoleChatInfo.size());

    for (std::list<RoleChatInfo>::iterator iter = m_lstRoleChatInfo.begin(); iter != m_lstRoleChatInfo.end(); ++iter)
    {
        iter->doEncode(IOBuff);
    }

    return true;
}

GameMsg_C2M_RequestRechargeRecordList::GameMsg_C2M_RequestRechargeRecordList()
: GameMsg_Base(GM_C2M_RequestRechargeRecordList)
, m_nStartTime(0)
, m_nEndTime(0)
, m_nQueryBeginIndex(0)
, m_nQueryCount(0)
{
}

bool GameMsg_C2M_RequestRechargeRecordList::doDecode(CParamPool &IOBuff)
{
    m_nStartTime = IOBuff.GetUInt();
    m_nEndTime = IOBuff.GetUInt();
    m_nQueryBeginIndex = IOBuff.GetUInt();
    m_nQueryCount = IOBuff.GetUInt();

    return true;
}

bool GameMsg_C2M_RequestRechargeRecordList::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nStartTime);
    IOBuff.AddUInt(m_nEndTime);
    IOBuff.AddUInt(m_nQueryBeginIndex);
    IOBuff.AddUInt(m_nQueryCount);

    return true;
}

GameMsg_M2C_ResponseRechargeRecordList::GameMsg_M2C_ResponseRechargeRecordList()
: GameMsg_Base(GM_M2C_ResponseRechargeRecordList)
{
}

bool GameMsg_M2C_ResponseRechargeRecordList::doDecode(CParamPool &IOBuff)
{
    return true;
}

bool GameMsg_M2C_ResponseRechargeRecordList::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddShort((unsigned short)m_listRecharge.size());

    for (std::list<DeviceRechargeInfo>::iterator it = m_listRecharge.begin();
        it != m_listRecharge.end(); ++it)
    {
        it->doEncode(IOBuff);
    }

    return true;
}



