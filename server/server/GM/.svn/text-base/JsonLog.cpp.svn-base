#include "JsonLog.h"
#include "../../socket/json/value.h"
#include "../../socket/json/reader.h"
#include "../../socket/json/writer.h"

#include "../../socket/Formatter.h"

#include "../util/CommonFunc.h"

#include "GMDataStruct.h"
#include "GameMsg_GM.h"
#include "../../datastructure/datainfor/GMDataInfo.h"


namespace GMJsonLog
{
    static std::string s_jsonType = "t";
    static std::string s_jsonValue = "v";
    static std::string s_strSendAnnounceOnceJsonHead = "sendAnnounceOnce";
    static std::string s_strSendAnnounceIntervalJsonHead = "sendAnnounceInterval";
    static std::string s_strSendSystemMailJsonHead = "sendMail";
    static std::string s_strLockPlayerJsonHead = "frozen";
    static std::string s_strSendMCoinJsonHead = "sendMCoin";
    static std::string s_strCompsition = "compsition";
    static std::string s_strAddUser = "addUser";
    static std::string s_strDelUser = "delUser";
    static std::string s_strSetUserPriv = "usrPriv";
    static std::string s_strChangeGMPassword = "changeGMPassword";
    static std::string s_strSetMallUpdateTime = "mallUpdate";
    static std::string s_strGroupBuyControl = "groupBuyC";
    static std::string s_strAddPrivMCoin = "privMCoin";
    static std::string s_strChangeAccountPassword = "chagepassword";
    static std::string s_strSendBlackDeviceList = "blackDevice";
    static std::string s_strAddMdeal = "addMedal";
    static std::string s_strDeleteVersionMdeal = "deleteVersionMedal";
    static std::string s_strAddInfoCenter = "addinfocenter";
    static std::string s_strUpdateInfoCenter = "updateInfocenter";
    static std::string s_strRemoveInfoCenter = "removeInfocenter";

    class RegisterTempClass
    {
    public:
        typedef std::string (*JsonToDescFunPtr)(Json::Value &jv);
        typedef void (*JsonToDescListFunPtr)(Json::Value &jv, std::list<std::string> &lstJson);

        struct TypeAndOper
        {
            std::string m_strJsonType;
            JsonToDescFunPtr m_funToDesc;
            JsonToDescListFunPtr m_funToDescList;
        };


    #define RegisterOper(strJsonType, funToDesc, funToDescList)\
            {\
            TypeAndOper o;\
            o.m_strJsonType = strJsonType;\
            o.m_funToDesc = funToDesc;\
            o.m_funToDescList = funToDescList;\
            operMap.insert(std::make_pair(o.m_strJsonType, o));}


        RegisterTempClass()
        {
            RegisterOper(s_strSendAnnounceOnceJsonHead, SendAnnounceOnceJsonToDesc, SendAnnounceOnceJsonToDescList);
            RegisterOper(s_strSendAnnounceIntervalJsonHead, SendAnnounceIntervalJsonToDesc, SendAnnounceIntervalJsonToDescList);
            RegisterOper(s_strSendSystemMailJsonHead, SendSystemMailJsonToDesc, SendSystemMailJsonToDescList);
            RegisterOper(s_strLockPlayerJsonHead, LockPlayerJsonToDesc, LockPlayerJsonToDescList);
            RegisterOper(s_strSendMCoinJsonHead, SendMCoinJsonToDesc, SendMCoinJsonToDescList);
            RegisterOper(s_strCompsition, CompsitionJsonToDesc, CompsitionToDescList);
            RegisterOper(s_strAddUser, AddUserJsonToDesc, AddUserToDescList);
            RegisterOper(s_strDelUser, DelUserJsonToDesc, DelUserToDescList);
            RegisterOper(s_strSetUserPriv, SetUserPrivJsonToDesc, SetUserPrivToDescList);
            RegisterOper(s_strChangeGMPassword, ChangeGMPasswordJsonToDesc, ChangeGMPasswordToDescList);
            RegisterOper(s_strSetMallUpdateTime, SetMallUpdateTimeJsonToDesc, SetMallUpdateTimeToDescList);
            RegisterOper(s_strAddPrivMCoin, AddPrivMCoinJsonToDesc, AddPrivMCoinToDescList);
            RegisterOper(s_strChangeAccountPassword, ChangeAccountPasswordJsonToDesc, ChangeAccountPasswordToDescList);
            RegisterOper(s_strSendBlackDeviceList, SendBlackDeviceListJsonToDesc, SendBlackDeviceListToDescList);
            RegisterOper(s_strAddMdeal, AddMedalJsonToDesc, AddMedalToDescList);
            RegisterOper(s_strDeleteVersionMdeal, DeleteVersionMedalJsonToDesc, DeleteVersionMedalToDescList);
            RegisterOper(s_strAddInfoCenter, AddInfoCenterJsonToDesc, AddInfoCenterToDescList);
            RegisterOper(s_strUpdateInfoCenter, UpdateInfoCenterJsonToDesc, UpdateInfoCenterToDescList);
            RegisterOper(s_strRemoveInfoCenter, RemoveInfoCenterJsonToDesc, RemoveInfoCenterToDescList);
            RegisterOper(s_strGroupBuyControl, SetGroupBuyControlJsonToDesc, SetGroupBuyControlToDescList);




        }


        JsonToDescFunPtr GetDescFun(const std::string &jsonType)
        {
            std::map<std::string, TypeAndOper>::iterator iter = operMap.find(jsonType);
            if (iter == operMap.end())
                return NULL;

            return iter->second.m_funToDesc;
        }
        JsonToDescListFunPtr GetDescListFun(const std::string &jsonType)
        {
            std::map<std::string, TypeAndOper>::iterator iter = operMap.find(jsonType);
            if (iter == operMap.end())
                return NULL;

            return iter->second.m_funToDescList;
        }


        std::map<std::string, TypeAndOper> operMap;
    };

    static RegisterTempClass s_register; // 用于构造函数初始化



    std::string ToJsonString(std::string &strJsonType, Json::Value &jv)
    {
        Json::FastWriter jfw;
        Json::Value root;
        root[s_jsonType] = strJsonType;
        root[s_jsonValue] = jv;
        return jfw.write(root);
    }

    std::string JsonToDesc(std::string jsonString)
    {
        Json::Reader jr;
        Json::Value root;
        if (!jr.parse(jsonString, root))
            return "error json"; // 无法解析的

        if (!root.isMember(s_jsonType))
            return "no type json";

        if (!root[s_jsonType].isString())
            return "error type json";

        if (!root.isMember(s_jsonValue))
            return "no value json";

        if (!root[s_jsonValue].isObject() && !root[s_jsonValue].isArray())
            return "error value json";

        std::string jtype = root[s_jsonType].asString();

        RegisterTempClass::JsonToDescFunPtr f = s_register.GetDescFun(jtype);
        if (NULL == f)
            return "miss json type";

        return f(root[s_jsonValue]);
    }

    void JsonToDescList(std::string jsonString, std::list<std::string> &listDesc)
    {
        Json::Reader jr;
        Json::Value root;
        if (!jr.parse(jsonString, root))
            return ; // 无法解析的

        if (!root.isMember(s_jsonType))
            return ;

        if (!root[s_jsonType].isString())
            return ;

        if (!root.isMember(s_jsonValue))
            return ;

        if (!root[s_jsonValue].isObject() && !root[s_jsonValue].isArray())
            return ;

        std::string jtype = root[s_jsonType].asString();

        RegisterTempClass::JsonToDescListFunPtr f = s_register.GetDescListFun(jtype);
        if (NULL == f)
            return ;

        f(root[s_jsonValue], listDesc);
        return;
    }

    namespace KeyStrSendAnnounceOnce
    {
        static std::string pid = "p";
        static std::string context = "c";
        static std::string isAdminChat = "a";
        static std::string isVoiceChat = "v";
        static std::string delayTime = "d";
    }

    std::string SendAnnounceOnceToJson(unsigned short nPID, std::string &strContext, bool bIsAdminChat, bool bIsVoiceChat, int nDealyTime)
    {
        Json::Value jv;
        jv[KeyStrSendAnnounceOnce::pid] = nPID;
        jv[KeyStrSendAnnounceOnce::context] = strContext;
        jv[KeyStrSendAnnounceOnce::isAdminChat] = bIsAdminChat;
        jv[KeyStrSendAnnounceOnce::isVoiceChat] = bIsVoiceChat;
        jv[KeyStrSendAnnounceOnce::delayTime] = nDealyTime;

        return ToJsonString(s_strSendAnnounceOnceJsonHead, jv);
    }

    std::string SendAnnounceOnceJsonToDesc(Json::Value &jv)
    {
        unsigned short nPID = 0;
        std::string strContext = "";
        bool bIsAdminChat = false; 
        bool bIsVoiceChat = false; 
        int nDealyTime = 0;

        if (jv.isMember(KeyStrSendAnnounceOnce::pid) && jv[KeyStrSendAnnounceOnce::pid].isNumeric())
            nPID = (unsigned short)jv[KeyStrSendAnnounceOnce::pid].asInt();
        if (jv.isMember(KeyStrSendAnnounceOnce::context) && jv[KeyStrSendAnnounceOnce::context].isString())
            strContext = jv[KeyStrSendAnnounceOnce::context].asString();
        if (jv.isMember(KeyStrSendAnnounceOnce::isAdminChat) && jv[KeyStrSendAnnounceOnce::isAdminChat].isBool())
            bIsAdminChat = jv[KeyStrSendAnnounceOnce::isAdminChat].asBool();
        if (jv.isMember(KeyStrSendAnnounceOnce::isVoiceChat) && jv[KeyStrSendAnnounceOnce::isVoiceChat].isBool())
            bIsVoiceChat = jv[KeyStrSendAnnounceOnce::isVoiceChat].asBool();
        if (jv.isMember(KeyStrSendAnnounceOnce::delayTime) && jv[KeyStrSendAnnounceOnce::delayTime].isNumeric())
            nDealyTime = jv[KeyStrSendAnnounceOnce::delayTime].asInt();

        std::string strDest;
        Formatter::Format(strDest, "pid[%d],delay[%d],show[%s%s],ctx[%s]",
            (int)nPID, nDealyTime, bIsAdminChat ? "scroll," : "", bIsVoiceChat ? "chat" : "", strContext.c_str());
        return strDest;
    }

    void SendAnnounceOnceJsonToDescList(Json::Value &jv, std::list<std::string> &listDesc)
    {
        std::string desc = SendAnnounceOnceJsonToDesc(jv);
        listDesc.push_back(desc);
    }

    namespace KeyStrSendAnnounceInterval
    {
        static std::string pid = "p";
        static std::string context = "c";
        static std::string isAdminChat = "a";
        static std::string isVoiceChat = "v";
        static std::string startTime = "st";
        static std::string endTime = "et";
        static std::string interVal = "i";
    }
    std::string SendAnnounceIntervalToJson(CAnnounceInfo &announceInfo)
    {
        Json::Value jv;
        jv[KeyStrSendAnnounceInterval::pid] = announceInfo.m_nPID;
        jv[KeyStrSendAnnounceInterval::context] = announceInfo.m_strContent;
        jv[KeyStrSendAnnounceInterval::isAdminChat] = announceInfo.m_bIsAdminChat;
        jv[KeyStrSendAnnounceInterval::isVoiceChat] = announceInfo.m_bIsSysChat;
        jv[KeyStrSendAnnounceInterval::startTime] = announceInfo.m_nStartTime;
        jv[KeyStrSendAnnounceInterval::endTime] = announceInfo.m_nEndTime;
        jv[KeyStrSendAnnounceInterval::interVal] = announceInfo.m_nSendInterval;

        return ToJsonString(s_strSendAnnounceIntervalJsonHead, jv);
    }

    std::string SendAnnounceIntervalJsonToDesc(Json::Value &jv)
    {
        CAnnounceInfo announceInfo;
        if (jv.isMember(KeyStrSendAnnounceInterval::pid) && jv[KeyStrSendAnnounceInterval::pid].isNumeric())
            announceInfo.m_nPID = (unsigned short)jv[KeyStrSendAnnounceInterval::pid].asInt();
        if (jv.isMember(KeyStrSendAnnounceInterval::context) && jv[KeyStrSendAnnounceInterval::context].isString())
            announceInfo.m_strContent = jv[KeyStrSendAnnounceInterval::context].asString();
        if (jv.isMember(KeyStrSendAnnounceInterval::isAdminChat) && jv[KeyStrSendAnnounceInterval::isAdminChat].isBool())
            announceInfo.m_bIsAdminChat = jv[KeyStrSendAnnounceInterval::isAdminChat].asBool();
        if (jv.isMember(KeyStrSendAnnounceInterval::isVoiceChat) && jv[KeyStrSendAnnounceInterval::isVoiceChat].isBool())
            announceInfo.m_bIsSysChat = jv[KeyStrSendAnnounceInterval::isVoiceChat].asBool();
        if (jv.isMember(KeyStrSendAnnounceInterval::startTime) && jv[KeyStrSendAnnounceInterval::startTime].isNumeric())
            announceInfo.m_nStartTime = jv[KeyStrSendAnnounceInterval::startTime].asInt();
        if (jv.isMember(KeyStrSendAnnounceInterval::endTime) && jv[KeyStrSendAnnounceInterval::endTime].isNumeric())
            announceInfo.m_nEndTime = jv[KeyStrSendAnnounceInterval::endTime].asInt();
        if (jv.isMember(KeyStrSendAnnounceInterval::interVal) && jv[KeyStrSendAnnounceInterval::interVal].isNumeric())
            announceInfo.m_nSendInterval = jv[KeyStrSendAnnounceInterval::interVal].asInt();

        std::string strDest;
        Formatter::Format(strDest, "pid[%d],show[%s%s],time[%s,%s,%d],ctx[%s]",
            (int)announceInfo.m_nPID, announceInfo.m_bIsAdminChat ? "scroll," : "", announceInfo.m_bIsSysChat ? "chat" : "",
            StandardTime(announceInfo.m_nStartTime).c_str(), StandardTime(announceInfo.m_nEndTime).c_str(), announceInfo.m_nSendInterval,
            announceInfo.m_strContent.c_str());
        return strDest;
    }

    void SendAnnounceIntervalJsonToDescList(Json::Value &jv, std::list<std::string> &listDesc)
    {
        std::string strDesc = SendAnnounceIntervalJsonToDesc(jv);
        listDesc.push_back(strDesc);
    }


    namespace KeyStrSendSystemMail
    {
        static std::string pid = "p";
        static std::string puid = "pu";
        static std::string mailTitle = "mt";
        static std::string mailContent = "mc";
        static std::string itemType = "it";
        static std::string itemCount = "ic";
        static std::string itemDuration = "is";
        static std::string money = "m";
    }
    std::string SendSystemMailToJson(std::list<CRcvSysMailRoleInfo> &lstSendMail)
    {
        Json::Value jvArry;
        for (std::list<CRcvSysMailRoleInfo>::iterator iter = lstSendMail.begin(); iter != lstSendMail.end(); ++iter)
        {
            Json::Value jv;
            jv[KeyStrSendSystemMail::pid] = iter->m_nPid;
            jv[KeyStrSendSystemMail::puid] = iter->m_strPUID;
            jv[KeyStrSendSystemMail::mailTitle] = iter->m_MailInfo.m_strTitle;
            jv[KeyStrSendSystemMail::mailContent] = iter->m_MailInfo.m_strContent;
            jv[KeyStrSendSystemMail::itemType] = iter->m_MailInfo.m_nItemType;
            jv[KeyStrSendSystemMail::itemCount] = iter->m_MailInfo.m_nItemCount;
            jv[KeyStrSendSystemMail::itemDuration] = iter->m_MailInfo.m_nItemDuration;
            jv[KeyStrSendSystemMail::money] = iter->m_MailInfo.m_nMoney;
            jvArry.append(jv);
        }

        return ToJsonString(s_strSendSystemMailJsonHead, jvArry);
    }

    std::string SendSystemMailJsonToDescSingleItem(Json::Value &jv)
    {
        unsigned short	nPid = 0;
        std::string	    strPUID;
        string			strTitle;
        string			strContent;
        itemtype_t  	nItemType = 0;
        int				nItemCount = 0;
        int				nItemDuration = 0;
        int				nMoney = 0;

        if (jv.isMember(KeyStrSendSystemMail::pid) && jv[KeyStrSendSystemMail::pid].isNumeric())
            nPid = (unsigned short)jv[KeyStrSendSystemMail::pid].asInt();
        if (jv.isMember(KeyStrSendSystemMail::puid) && jv[KeyStrSendSystemMail::puid].isString())
            strPUID = jv[KeyStrSendSystemMail::puid].asString();
        if (jv.isMember(KeyStrSendSystemMail::mailTitle) && jv[KeyStrSendSystemMail::mailTitle].isString())
            strTitle = jv[KeyStrSendSystemMail::mailTitle].asString();
        if (jv.isMember(KeyStrSendSystemMail::mailContent) && jv[KeyStrSendSystemMail::mailContent].isString())
            strContent = jv[KeyStrSendSystemMail::mailContent].asString();
        if (jv.isMember(KeyStrSendSystemMail::itemType) && jv[KeyStrSendSystemMail::itemType].isNumeric())
            nItemType = jv[KeyStrSendSystemMail::itemType].asInt();
        if (jv.isMember(KeyStrSendSystemMail::itemCount) && jv[KeyStrSendSystemMail::itemCount].isNumeric())
            nItemCount = jv[KeyStrSendSystemMail::itemCount].asInt();
        if (jv.isMember(KeyStrSendSystemMail::itemDuration) && jv[KeyStrSendSystemMail::itemDuration].isNumeric())
            nItemDuration = jv[KeyStrSendSystemMail::itemDuration].asInt();
        if (jv.isMember(KeyStrSendSystemMail::money) && jv[KeyStrSendSystemMail::money].isNumeric())
            nMoney = jv[KeyStrSendSystemMail::money].asInt();


        std::string strDest;
        Formatter::Format(strDest, "role[%d,%s], item[%d,%d,%d], money[%d], mail[%s|%s]",
            (int)nPid, strPUID.c_str(), (int)nItemType, (int)nItemCount, (int)nItemDuration, (int)nMoney, strTitle.c_str(), strContent.c_str());
        return strDest; // 
    }

    std::string SendSystemMailJsonToDesc(Json::Value &jv)
    {
        if (!jv.isArray() || jv.empty())
            return "";

        if (jv.size() > 1)
            return "more mails";

        return SendSystemMailJsonToDescSingleItem(jv[(Json::UInt)0]); // 
    }
    void SendSystemMailJsonToDescList(Json::Value &jv, std::list<std::string> &listDesc)
    {
        if (!jv.isArray() || jv.empty())
            return;

        for (int i = 0; i < (int)jv.size(); ++i)
        {
            listDesc.push_back(SendSystemMailJsonToDescSingleItem(jv[i]));
        }

        return;
    }


    namespace KeyStrLockPlayer
    {
        static std::string pid = "p";
        static std::string puid = "pu";
        static std::string lockType = "lt";
        static std::string isLock = "il";
        static std::string lockReson = "lr";
    }
    std::string LockPlayerToJson(unsigned short nPid, std::string strPuid, bool bLock, int nLockTime, unsigned short nLockReason)
    {
        Json::Value jv;
        jv[KeyStrLockPlayer::pid] = nPid;
        jv[KeyStrLockPlayer::puid] = strPuid;
        jv[KeyStrLockPlayer::isLock] = bLock;
        jv[KeyStrLockPlayer::lockType] = nLockTime;
        jv[KeyStrLockPlayer::lockReson] = nLockReason;

        return ToJsonString(s_strLockPlayerJsonHead, jv);
    }

    std::string LockPlayerJsonToDesc(Json::Value &jv)
    {
        unsigned short nPid = 0;
        std::string strPUID;
        bool bLock = true;//锁定还是解锁
        int nLockTimeType = 0;//右边数组的下标，[10min   1hour   1day    7day    15day    30day    90day    永久]
        unsigned short nLockReason = 0;

        if (jv.isMember(KeyStrLockPlayer::pid) && jv[KeyStrLockPlayer::pid].isNumeric())
            nPid = (unsigned short)jv[KeyStrLockPlayer::pid].asInt();
        if (jv.isMember(KeyStrLockPlayer::puid) && jv[KeyStrLockPlayer::puid].isString())
            strPUID = jv[KeyStrLockPlayer::puid].asString();
        if (jv.isMember(KeyStrLockPlayer::isLock) && jv[KeyStrLockPlayer::isLock].isBool())
            bLock = jv[KeyStrLockPlayer::isLock].asBool();
        if (jv.isMember(KeyStrLockPlayer::lockType) && jv[KeyStrLockPlayer::lockType].isNumeric())
            nLockTimeType = jv[KeyStrLockPlayer::lockType].asInt();
        if (jv.isMember(KeyStrLockPlayer::lockReson) && jv[KeyStrLockPlayer::lockReson].isNumeric())
            nLockReason = (unsigned short)jv[KeyStrLockPlayer::pid].asInt();

        std::string strDesc;
        Formatter::Format(strDesc, "%s role[%d,%s] lockType:[%d] reson[%d]",
            bLock ? "lock" : "unlock", nPid, strPUID.c_str(), (int)nLockTimeType, (int)nLockReason);
        return strDesc;
    }

    void LockPlayerJsonToDescList(Json::Value &jv, std::list<std::string> &listDesc)
    {
        // not arry.
        return; 
    }

    
    namespace KeyStrSendMCoin
    {
        static std::string pid = "p";
        static std::string puid = "pu";
        static std::string ammount = "m";
        static std::string bindAmmount = "bc";
    }
    std::string SendMCoinToJson(unsigned short nPid, std::string strPuid, unsigned int nAmmount, unsigned int nBindAmmount)
    {
        Json::Value jv;
        jv[KeyStrSendMCoin::pid] = nPid;
        jv[KeyStrSendMCoin::puid] = strPuid;
        jv[KeyStrSendMCoin::ammount] = nAmmount;
        jv[KeyStrSendMCoin::bindAmmount] = nBindAmmount;

        return ToJsonString(s_strSendMCoinJsonHead, jv);
    }

    std::string SendMCoinJsonToDesc(Json::Value &jv)
    {
        unsigned short nPid = 0;
        std::string strPUID;
        unsigned int nAmount = 0;
        unsigned int nBindAmount = 0;

        if (jv.isMember(KeyStrSendMCoin::pid) && jv[KeyStrSendMCoin::pid].isNumeric())
            nPid = (unsigned short)jv[KeyStrSendMCoin::pid].asInt();
        if (jv.isMember(KeyStrSendMCoin::puid) && jv[KeyStrSendMCoin::puid].isString())
            strPUID = jv[KeyStrSendMCoin::puid].asString();
        if (jv.isMember(KeyStrSendMCoin::ammount) && jv[KeyStrSendMCoin::ammount].isNumeric())
            nAmount = jv[KeyStrSendMCoin::ammount].asInt();
        if (jv.isMember(KeyStrSendMCoin::bindAmmount) && jv[KeyStrSendMCoin::bindAmmount].isNumeric())
            nBindAmount = jv[KeyStrSendMCoin::bindAmmount].asInt();

        std::string strDesc;
        Formatter::Format(strDesc, "role[%d, %s] money[%d] bindCoin[%d]",
            nPid, strPUID.c_str(), nAmount, nBindAmount);
        return strDesc;
    }
    void SendMCoinJsonToDescList(Json::Value &jv, std::list<std::string> &listDesc)
    { // 不要

    }


    namespace KeyStrCompsition
    {
        static std::string endTime = "et";
        static std::string money = "my";
        static std::string maleItem = "mi";
        static std::string femaleItem = "fmi";
        static std::string title = "mt";
        static std::string context = "mc";
    }
    std::string CompsitionToJson(std::string strEndTime, int nMoney, std::string strItemMale, std::string strItemFemale, std::string strTitle, std::string strContext)
    {
        Json::Value jv;
        jv[KeyStrCompsition::endTime] = strEndTime;
        jv[KeyStrCompsition::money] = nMoney;
        jv[KeyStrCompsition::maleItem] = strItemFemale;
        jv[KeyStrCompsition::femaleItem] = strItemFemale;
        jv[KeyStrCompsition::title] = strTitle;
        jv[KeyStrCompsition::context] = strContext;

        return ToJsonString(s_strCompsition, jv);
    }
    std::string CompsitionJsonToDesc(Json::Value &jv)
    {
        std::string strEndTime;
        int nMoney = 0;
        std::string strItemMale;
        std::string strItemFemale;
        std::string strTitle;
        std::string strContext;

        if (jv.isMember(KeyStrCompsition::endTime) && jv[KeyStrCompsition::endTime].isString())
            strEndTime = jv[KeyStrCompsition::endTime].asString();
        if (jv.isMember(KeyStrCompsition::money) && jv[KeyStrCompsition::money].isNumeric())
            nMoney = jv[KeyStrCompsition::money].asInt();
        if (jv.isMember(KeyStrCompsition::maleItem) && jv[KeyStrCompsition::maleItem].isString())
            strItemMale = jv[KeyStrCompsition::maleItem].asString();
        if (jv.isMember(KeyStrCompsition::femaleItem) && jv[KeyStrCompsition::femaleItem].isString())
            strItemFemale = jv[KeyStrCompsition::femaleItem].asString();
        if (jv.isMember(KeyStrCompsition::title) && jv[KeyStrCompsition::title].isString())
            strTitle = jv[KeyStrCompsition::title].asString();
        if (jv.isMember(KeyStrCompsition::context) && jv[KeyStrCompsition::context].isString())
            strContext = jv[KeyStrCompsition::context].asString();

        std::string strDesc;
        Formatter::Format(strDesc, "%s, money:%d, maleItem[%s], femaleItem:[%s], info[%s|%s]",
            strEndTime.c_str(), nMoney, strItemMale.c_str(), strItemFemale.c_str(), strTitle.c_str(), strContext.c_str());
        return strDesc;
    }
    void CompsitionToDescList(Json::Value &jv, std::list<std::string> &listDesc)
    { // nothing

    }

    namespace KeyStrPrivManager
    {
        static std::string name = "n";
        static std::string password = "p";
    }
    std::string AddUserToJson(std::string userName, std::string userPassword)
    {
        Json::Value jv;
        jv[KeyStrPrivManager::name] = userName;
        jv[KeyStrPrivManager::password] = userPassword;
        return ToJsonString(s_strAddUser, jv);
    }

    std::string AddUserJsonToDesc(Json::Value &jv)
    {
        std::string userName;
        std::string userPassWord;

        if (jv.isMember(KeyStrPrivManager::name) && jv[KeyStrPrivManager::name].isString())
            userName = jv[KeyStrPrivManager::name].asString();
        if (jv.isMember(KeyStrPrivManager::password) && jv[KeyStrPrivManager::password].isString())
            userPassWord = jv[KeyStrPrivManager::password].asString();

        std::string strDesc;
        Formatter::Format(strDesc, "add user: [%s, %s]", userName.c_str(), userPassWord.c_str());
        return strDesc;
    }
    void AddUserToDescList(Json::Value &jv, std::list<std::string> &listDesc)
    {}

    std::string DelUserToJson(std::string userName)
    {
        Json::Value jv;
        jv[KeyStrPrivManager::name] = userName;
        return ToJsonString(s_strDelUser, jv);
    }

    std::string DelUserJsonToDesc(Json::Value &jv)
    {
        std::string userName;
        if (jv.isMember(KeyStrPrivManager::name) && jv[KeyStrPrivManager::name].isString())
            userName = jv[KeyStrPrivManager::name].asString();

        return std::string("del usr: [") + userName + "]";
    }
    void DelUserToDescList(Json::Value &jv, std::list<std::string> &listDesc)
    {

    }

    std::string ChangeGMPasswordToJson(std::string userName, std::string password)
    {
        Json::Value jv;
        jv[KeyStrPrivManager::name] = userName;
        jv[KeyStrPrivManager::password] = password;

        return ToJsonString(s_strChangeGMPassword, jv);
    }
    std::string ChangeGMPasswordJsonToDesc(Json::Value &jv)
    {
        std::string userName;
        std::string password;

        if (jv.isMember(KeyStrPrivManager::name) && jv[KeyStrPrivManager::name].isString())
            userName = jv[KeyStrPrivManager::name].asString();
        if (jv.isMember(KeyStrPrivManager::password) && jv[KeyStrPrivManager::password].isString())
            password = jv[KeyStrPrivManager::password].asString();


        std::string strDesc;
        Formatter::Format(strDesc, "change password: [%s,%s]", userName.c_str(), password.c_str());
        return strDesc;
    }
    void ChangeGMPasswordToDescList(Json::Value &jv, std::list<std::string> &listDesc)
    { // return; 

    }


    namespace KeyStrSetUserPriv
    {
        static std::string name = "n";
        static std::string priv = "p";
    }
    std::string SetUserPrivToJson(std::string userName, int priv)
    {
        Json::Value jv;
        jv[KeyStrSetUserPriv::name] = userName;
        jv[KeyStrSetUserPriv::priv] = priv;
        return ToJsonString(s_strSetUserPriv, jv);
    }
    std::string SetUserPrivJsonToDesc(Json::Value &jv)
    {
        std::string userName;
        int priv = 0;

        if (jv.isMember(KeyStrSetUserPriv::name) && jv[KeyStrSetUserPriv::name].isString())
            userName = jv[KeyStrSetUserPriv::name].asString();
        if (jv.isMember(KeyStrSetUserPriv::priv) && jv[KeyStrSetUserPriv::priv].isNumeric())
            priv = jv[KeyStrSetUserPriv::priv].asInt();

        std::string strDesc;
        Formatter::Format(strDesc, "set usr priv: [%s ,%d]", userName.c_str(), priv);
        return strDesc;
    }

    void SetUserPrivToDescList(Json::Value &jv, std::list<std::string> &listDesc)
    {}


    namespace KeyStrSetMallUpdateTime
    {
        static std::string updateTime = "t";
    }
    std::string SetMallUpdateTimeToJson(unsigned int updateTime)
    {
        Json::Value jv;
        jv[KeyStrSetMallUpdateTime::updateTime] = updateTime;

        return ToJsonString(s_strSetMallUpdateTime, jv);
    }

    std::string SetMallUpdateTimeJsonToDesc(Json::Value &jv)
    {
        unsigned int nTime = 0;
        if (jv.isMember(KeyStrSetMallUpdateTime::updateTime) && jv[KeyStrSetMallUpdateTime::updateTime].isNumeric())
            nTime = jv[KeyStrSetMallUpdateTime::updateTime].asUInt();

        return StandardTime(nTime);
    }

    void SetMallUpdateTimeToDescList(Json::Value &jv, std::list<std::string> &listDesc)
    {}


    namespace KeyStrGroupBuyControl
    {
        static std::string line = "l";
        static std::string startTime = "s";
        static std::string endTime = "e";
        static std::string interval = "i";
        static std::string minPeople = "min";
        static std::string maxPeople = "max";
    }
    std::string SetGroupBuyControlToJson(int nLine, unsigned int nStartTime, unsigned int nEndTime, int nInterval, int nMinPeople, int nMaxPeople)
    {
        Json::Value jv;
        jv[KeyStrGroupBuyControl::line] = nLine;
        jv[KeyStrGroupBuyControl::startTime] = nStartTime;
        jv[KeyStrGroupBuyControl::endTime] = nEndTime;
        jv[KeyStrGroupBuyControl::interval] = nInterval;
        jv[KeyStrGroupBuyControl::minPeople] = nMinPeople;
        jv[KeyStrGroupBuyControl::maxPeople] = nMaxPeople;

        return ToJsonString(s_strGroupBuyControl, jv);
    }
    std::string SetGroupBuyControlJsonToDesc(Json::Value &jv)
    {
        int nLine = 0;
        unsigned int nStartTime = 0;
        unsigned int nEndTime = 0;
        int nInterval = 0;
        int nMinPeople = 0;
        int nMaxPeople = 0;

        if (jv.isMember(KeyStrGroupBuyControl::line) && jv[KeyStrGroupBuyControl::line].isNumeric())
            nLine = jv[KeyStrGroupBuyControl::line].asInt();
        if (jv.isMember(KeyStrGroupBuyControl::startTime) && jv[KeyStrGroupBuyControl::startTime].isNumeric())
            nStartTime = jv[KeyStrGroupBuyControl::startTime].asUInt();
        if (jv.isMember(KeyStrGroupBuyControl::endTime) && jv[KeyStrGroupBuyControl::endTime].isNumeric())
            nEndTime = jv[KeyStrGroupBuyControl::endTime].asUInt();
        if (jv.isMember(KeyStrGroupBuyControl::interval) && jv[KeyStrGroupBuyControl::interval].isNumeric())
            nInterval = jv[KeyStrGroupBuyControl::interval].asInt();
        if (jv.isMember(KeyStrGroupBuyControl::minPeople) && jv[KeyStrGroupBuyControl::minPeople].isNumeric())
            nMinPeople = jv[KeyStrGroupBuyControl::minPeople].asInt();
        if (jv.isMember(KeyStrGroupBuyControl::maxPeople) && jv[KeyStrGroupBuyControl::maxPeople].isNumeric())
            nMaxPeople = jv[KeyStrGroupBuyControl::maxPeople].asInt();

        std::string strDesc;
        Formatter::Format(strDesc, "line[%d] time[%s,%s,%d] player[%d, %d]",
            nLine, StandardTime(nStartTime).c_str(), StandardTime(nEndTime).c_str(), nInterval,
            nMinPeople, nMaxPeople);
        return strDesc;
    }
    void SetGroupBuyControlToDescList(Json::Value &jv, std::list<std::string> &listDesc)
    {}


    namespace KeyStrAddPrivMCoin
    {
        static std::string puid = "p";
        static std::string money = "m";
        static std::string bindMoney = "b";
    }
    std::string AddPrivMCoinToJson(std::vector<PlayerMoneyInfo> &vecdMoneyInfo)
    {
        Json::Value jv;
        for (int i = 0; i < (int)vecdMoneyInfo.size(); ++i)
        {
            Json::Value jvItem;
            jvItem[KeyStrAddPrivMCoin::puid] = vecdMoneyInfo[i].m_PlayerPUID;
            jvItem[KeyStrAddPrivMCoin::money] = vecdMoneyInfo[i].m_nMoney;
            jvItem[KeyStrAddPrivMCoin::bindMoney] = vecdMoneyInfo[i].m_nBindMoney;

            jv.append(jvItem);
        }

        return ToJsonString(s_strAddPrivMCoin, jv);
    }

    std::string AddPrivMCoinJsonDescItem(Json::Value &jv)
    {
        std::string strPuid;
        int nMoney = 0;
        int nBindMoney = 0;

        if (jv.isMember(KeyStrAddPrivMCoin::puid) && jv[KeyStrAddPrivMCoin::puid].isString())
            strPuid = jv[KeyStrAddPrivMCoin::puid].asString();
        if (jv.isMember(KeyStrAddPrivMCoin::money) && jv[KeyStrAddPrivMCoin::money].isNumeric())
            nMoney = jv[KeyStrAddPrivMCoin::money].asInt();
        if (jv.isMember(KeyStrAddPrivMCoin::bindMoney) && jv[KeyStrAddPrivMCoin::bindMoney].isNumeric())
            nBindMoney = jv[KeyStrAddPrivMCoin::bindMoney].asInt();

        std::string strDesc;
        Formatter::Format(strDesc, "puid:%s, money:%d, bindMoney:%d", strPuid.c_str(), nMoney, nBindMoney);
        return strDesc;
    }

    std::string AddPrivMCoinJsonToDesc(Json::Value &jv)
    {
        if (!jv.isArray() || jv.empty())
            return "";

        if (jv.size() > 1)
            return "more items";

        return AddPrivMCoinJsonDescItem(jv[Json::UInt(0)]);
    }

    void AddPrivMCoinToDescList(Json::Value &jv, std::list<std::string> &listDesc)
    {
        if (!jv.isArray() || jv.empty())
            return ;

        for (int i = 0; i < (int)jv.size(); ++i)
        {
            listDesc.push_back(AddPrivMCoinJsonDescItem(jv[i]));
        }

        return;
    }

    namespace KeyStrChangeAccountPassword
    {
        static std::string userName = "u";
        static std::string password = "p";
    }
    std::string ChangeAccountPasswordToJson(std::string userName, std::string password)
    {
        Json::Value jv;
        jv[KeyStrChangeAccountPassword::userName] = userName;
        jv[KeyStrChangeAccountPassword::password] = password;

        return ToJsonString(s_strChangeAccountPassword, jv);
    }
    std::string ChangeAccountPasswordJsonToDesc(Json::Value &jv)
    {
        std::string userName;
        std::string password;
        if (jv.isMember(KeyStrChangeAccountPassword::userName) && jv[KeyStrChangeAccountPassword::userName].isString())
            userName = jv[KeyStrChangeAccountPassword::userName].asString();
        if (jv.isMember(KeyStrChangeAccountPassword::password) && jv[KeyStrChangeAccountPassword::password].isString())
            password = jv[KeyStrChangeAccountPassword::password].asString();

        return userName + ", " + password; // 用户名密码
    }
    void ChangeAccountPasswordToDescList(Json::Value &jv, std::list<std::string> &listDesc)
    {
        // do nothing.
    }

    namespace KeyStrSendBlackDeviceList
    {
        static std::string isForbiden = "f";
        static std::string deviceList = "l";
    }
    std::string SendBlackDeviceListToJson(bool forbiden, std::vector<string> &deviceList)
    {
        Json::Value jv;
        jv[KeyStrSendBlackDeviceList::isForbiden] = forbiden;

        Json::Value jvDeviceList;
        for (int i = 0; i < (int)deviceList.size(); ++i)
        {
            jvDeviceList.append(Json::Value(deviceList[i]));
        }

        jv[KeyStrSendBlackDeviceList::deviceList] = jvDeviceList;
        return ToJsonString(s_strSendBlackDeviceList, jv);
    }
    std::string SendBlackDeviceListJsonToDesc(Json::Value &jv)
    {
        std::string devices;
        bool isForbiden = true;

        if (jv.isMember(KeyStrSendBlackDeviceList::isForbiden) && jv[KeyStrSendBlackDeviceList::isForbiden].isBool())
            isForbiden = jv[KeyStrSendBlackDeviceList::isForbiden].asBool();
        if (jv.isMember(KeyStrSendBlackDeviceList::deviceList) && jv[KeyStrSendBlackDeviceList::deviceList].isArray())
        {
            for (int i = 0; i < (int)jv[KeyStrSendBlackDeviceList::deviceList].size(); ++i)
            {
                if (0 != i)
                    devices += ", ";
                if (jv[KeyStrSendBlackDeviceList::deviceList].isString())
                    devices += jv[KeyStrSendBlackDeviceList::deviceList].asString();
            }
        }

        return string(isForbiden ? "yes, " : "no, ") + "[" + devices + "]";
    }

    void SendBlackDeviceListToDescList(Json::Value &jv, std::list<std::string> &listDesc)
    { // nothing.

    }


    namespace KeyStrAddMedal
    {
        static std::string roleID = "r";
        static std::string medalID = "m";
    }
    std::string AddMedalToJson(int nRoleID, int nMedalID)
    {
        Json::Value jv;
        jv[KeyStrAddMedal::roleID] = nRoleID;
        jv[KeyStrAddMedal::medalID] = nMedalID;

        return ToJsonString(s_strAddMdeal, jv);
    }

    std::string AddMedalJsonToDesc(Json::Value &jv)
    {
        int nRoleID = 0;
        int nMedalID = 0;

        if (jv.isMember(KeyStrAddMedal::roleID) && jv[KeyStrAddMedal::roleID].isNumeric())
            nRoleID = jv[KeyStrAddMedal::roleID].asInt();
        if (jv.isMember(KeyStrAddMedal::medalID) && jv[KeyStrAddMedal::medalID].isNumeric())
            nMedalID = jv[KeyStrAddMedal::medalID].asInt();

        std::string strDesc;
        Formatter::Format(strDesc, "add: [%d,%d]", nRoleID, nMedalID);
        return strDesc;
    }

    void AddMedalToDescList(Json::Value &jv, std::list<std::string> &listDesc)
    { // do nothing.

    }



    namespace KeyStrAddMedal
    {
    }
    std::string DeleteVersionMedalToJson()
    {
        Json::Value jv; // nulll
        return ToJsonString(s_strDeleteVersionMdeal, jv);
    }
    std::string DeleteVersionMedalJsonToDesc(Json::Value &jv)
    {
        return "delete version medal!";
    }

    void DeleteVersionMedalToDescList(Json::Value &jv, std::list<std::string> &listDesc)
    { // do nothing.

    }



    namespace KeyStrInfoCenter
    {
        static std::string id = "id";
        static std::string group = "g";
        static std::string icon = "ic";
        static std::string ctype = "ty";
        static std::string title = "t";
        static std::string context = "ct";
        static std::string href = "h";
        static std::string hidPid = "hp";
    }
    std::string AddInfoCenterToJson(InfoCenterItemCM &infocenter)
    {
        Json::Value jv;
        jv[KeyStrInfoCenter::group] = infocenter.m_group;
        jv[KeyStrInfoCenter::icon] = infocenter.m_icon;
        jv[KeyStrInfoCenter::ctype] = infocenter.m_type;
        jv[KeyStrInfoCenter::title] = infocenter.m_strTitle;
        jv[KeyStrInfoCenter::context] = infocenter.m_strContext;
        jv[KeyStrInfoCenter::href] = infocenter.m_strHref;
        jv[KeyStrInfoCenter::hidPid] = infocenter.m_strHidPid;

        return ToJsonString(s_strAddInfoCenter, jv);
    }

    std::string AddInfoCenterJsonToDesc(Json::Value &jv)
    {
        InfoCenterItemCM i;
        if (jv.isMember(KeyStrInfoCenter::group) && jv[KeyStrInfoCenter::group].isNumeric())
            i.m_group = (unsigned char)jv[KeyStrInfoCenter::group].asInt();
        if (jv.isMember(KeyStrInfoCenter::icon) && jv[KeyStrInfoCenter::icon].isString())
            i.m_icon = jv[KeyStrInfoCenter::icon].asString();
        if (jv.isMember(KeyStrInfoCenter::ctype) && jv[KeyStrInfoCenter::ctype].isNumeric())
            i.m_type = (unsigned char)jv[KeyStrInfoCenter::ctype].asInt();
        if (jv.isMember(KeyStrInfoCenter::title) && jv[KeyStrInfoCenter::title].isString())
            i.m_strTitle = jv[KeyStrInfoCenter::title].asString();
        if (jv.isMember(KeyStrInfoCenter::context) && jv[KeyStrInfoCenter::context].isString())
            i.m_strContext = jv[KeyStrInfoCenter::context].asString();
        if (jv.isMember(KeyStrInfoCenter::href) && jv[KeyStrInfoCenter::href].isString())
            i.m_strHref = jv[KeyStrInfoCenter::href].asString();
        if (jv.isMember(KeyStrInfoCenter::hidPid) && jv[KeyStrInfoCenter::hidPid].isString())
            i.m_strHidPid = jv[KeyStrInfoCenter::hidPid].asString();

        std::string strDesc;
        Formatter::Format(strDesc, "add: group/icon/type[%d,%s,%d], pid[%s], info[%s|%s|%s]", i.m_group, i.m_icon.c_str(), i.m_type, i.m_strHidPid.c_str(),
            i.m_strTitle.c_str(), i.m_strContext.c_str(), i.m_strHref.c_str());
        return strDesc;
    }
    void AddInfoCenterToDescList(Json::Value &jv, std::list<std::string> &listDesc)
    {
        // None
    }

    std::string UpdateInfoCenterToJson(InfoCenterItemCM &infocenter)
    {
        Json::Value jv;
        jv[KeyStrInfoCenter::id] = infocenter.m_id;
        jv[KeyStrInfoCenter::group] = infocenter.m_group;
        jv[KeyStrInfoCenter::icon] = infocenter.m_icon;
        jv[KeyStrInfoCenter::ctype] = infocenter.m_type;
        jv[KeyStrInfoCenter::title] = infocenter.m_strTitle;
        jv[KeyStrInfoCenter::context] = infocenter.m_strContext;
        jv[KeyStrInfoCenter::href] = infocenter.m_strHref;
        jv[KeyStrInfoCenter::hidPid] = infocenter.m_strHidPid;

        return ToJsonString(s_strUpdateInfoCenter, jv);
    }
    std::string UpdateInfoCenterJsonToDesc(Json::Value &jv)
    {
        InfoCenterItemCM i;
        if (jv.isMember(KeyStrInfoCenter::group) && jv[KeyStrInfoCenter::group].isNumeric())
            i.m_group = (unsigned char)jv[KeyStrInfoCenter::group].asInt();
        if (jv.isMember(KeyStrInfoCenter::icon) && jv[KeyStrInfoCenter::icon].isString())
            i.m_icon = jv[KeyStrInfoCenter::icon].asString();
        if (jv.isMember(KeyStrInfoCenter::ctype) && jv[KeyStrInfoCenter::ctype].isNumeric())
            i.m_type = (unsigned char)jv[KeyStrInfoCenter::ctype].asInt();
        if (jv.isMember(KeyStrInfoCenter::title) && jv[KeyStrInfoCenter::title].isString())
            i.m_strTitle = jv[KeyStrInfoCenter::title].asString();
        if (jv.isMember(KeyStrInfoCenter::context) && jv[KeyStrInfoCenter::context].isString())
            i.m_strContext = jv[KeyStrInfoCenter::context].asString();
        if (jv.isMember(KeyStrInfoCenter::href) && jv[KeyStrInfoCenter::href].isString())
            i.m_strHref = jv[KeyStrInfoCenter::href].asString();
        if (jv.isMember(KeyStrInfoCenter::hidPid) && jv[KeyStrInfoCenter::hidPid].isString())
            i.m_strHidPid = jv[KeyStrInfoCenter::hidPid].asString();

        std::string strDesc;
        Formatter::Format(strDesc, "update: group/icon/type[%d,%s,%d], pid[%s], info[%s|%s|%s]", i.m_group, i.m_icon.c_str(), i.m_type, i.m_strHidPid.c_str(),
            i.m_strTitle.c_str(), i.m_strContext.c_str(), i.m_strHref.c_str());
        return strDesc;
    }

    void UpdateInfoCenterToDescList(Json::Value &jv, std::list<std::string> &listDesc)
    {
        // None
    }

    std::string RemoveInfoCenterToJson(int nInfoId)
    {
        Json::Value jv;
        jv[KeyStrInfoCenter::id] = nInfoId;

        return ToJsonString(s_strRemoveInfoCenter, jv);
    }
    std::string RemoveInfoCenterJsonToDesc(Json::Value &jv)
    {
        int nInfoID = 0;
        if (jv.isMember(KeyStrInfoCenter::id) && jv[KeyStrInfoCenter::id].isNumeric())
            nInfoID = jv[KeyStrInfoCenter::id].asInt();

        std::string strDesc;
        Formatter::Format(strDesc, "remove: %d", nInfoID);
        return strDesc;
    }

    void RemoveInfoCenterToDescList(Json::Value &jv, std::list<std::string> &listDesc)
    {
        // None
    }

}

