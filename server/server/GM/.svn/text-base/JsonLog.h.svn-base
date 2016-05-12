#ifndef __GMSERVER_JSONLOG_H__
#define __GMSERVER_JSONLOG_H__

#include <string>
#include <list>
#include <vector>

namespace Json
{
    class Value;
}
class CAnnounceInfo;
class CRcvSysMailRoleInfo;
class PlayerMoneyInfo;
struct InfoCenterItemCM;

namespace GMJsonLog
{
    std::string JsonToDesc(std::string jsonString); 
    void JsonToDescList(std::string jsonString, std::list<std::string> &listDesc);


    std::string SendAnnounceOnceToJson(unsigned short nPID, std::string &strContext, bool bIsAdminChat, bool bIsVoiceChat, int nDealyTime);
    std::string SendAnnounceOnceJsonToDesc(Json::Value &jv); 
    void SendAnnounceOnceJsonToDescList(Json::Value &jv, std::list<std::string> &listDesc);

    std::string SendAnnounceIntervalToJson(CAnnounceInfo &announceInfo);
    std::string SendAnnounceIntervalJsonToDesc(Json::Value &jv); 
    void SendAnnounceIntervalJsonToDescList(Json::Value &jv, std::list<std::string> &listDesc);

    std::string SendSystemMailToJson(std::list<CRcvSysMailRoleInfo> &lstSendMail);
    std::string SendSystemMailJsonToDesc(Json::Value &jv); 
    void SendSystemMailJsonToDescList(Json::Value &jv, std::list<std::string> &listDesc);

    // Not do
//    GameMsg_C2S_QueryRangeGift

    std::string LockPlayerToJson(unsigned short nPid, std::string strPuid, bool bLock, int nLockTime, unsigned short nLockReason);
    std::string LockPlayerJsonToDesc(Json::Value &jv);
    void LockPlayerJsonToDescList(Json::Value &jv, std::list<std::string> &listDesc);

    std::string SendMCoinToJson(unsigned short nPid, std::string strPuid, unsigned int nAmmount, unsigned int nBindAmount);
    std::string SendMCoinJsonToDesc(Json::Value &jv);
    void SendMCoinJsonToDescList(Json::Value &jv, std::list<std::string> &listDesc);


    std::string CompsitionToJson(std::string strEndTime, int nMoney, std::string strItemMale, std::string strItemFemale, std::string strTitle, std::string strContext);
    std::string CompsitionJsonToDesc(Json::Value &jv);
    void CompsitionToDescList(Json::Value &jv, std::list<std::string> &listDesc);


    std::string AddUserToJson(std::string userName, std::string userPassword);
    std::string AddUserJsonToDesc(Json::Value &jv);
    void AddUserToDescList(Json::Value &jv, std::list<std::string> &listDesc);

    std::string DelUserToJson(std::string userName);
    std::string DelUserJsonToDesc(Json::Value &jv);
    void DelUserToDescList(Json::Value &jv, std::list<std::string> &listDesc);

    std::string ChangeGMPasswordToJson(std::string userName, std::string password);
    std::string ChangeGMPasswordJsonToDesc(Json::Value &jv);
    void ChangeGMPasswordToDescList(Json::Value &jv, std::list<std::string> &listDesc);


    std::string SetUserPrivToJson(std::string userName, int priv);
    std::string SetUserPrivJsonToDesc(Json::Value &jv);
    void SetUserPrivToDescList(Json::Value &jv, std::list<std::string> &listDesc);


    std::string SetMallUpdateTimeToJson(unsigned int updateTime);
    std::string SetMallUpdateTimeJsonToDesc(Json::Value &jv);
    void SetMallUpdateTimeToDescList(Json::Value &jv, std::list<std::string> &listDesc);


    std::string SetGroupBuyControlToJson(int nLine, unsigned int nStartTime, unsigned int nEndTime, int nInterval, int nMinPeople, int nMaxPeople);
    std::string SetGroupBuyControlJsonToDesc(Json::Value &jv);
    void SetGroupBuyControlToDescList(Json::Value &jv, std::list<std::string> &listDesc);

    std::string AddPrivMCoinToJson(std::vector<PlayerMoneyInfo> &vecdMoenyInfo);
    std::string AddPrivMCoinJsonToDesc(Json::Value &jv);
    void AddPrivMCoinToDescList(Json::Value &jv, std::list<std::string> &listDesc);


    std::string ChangeAccountPasswordToJson(std::string userName, std::string password);
    std::string ChangeAccountPasswordJsonToDesc(Json::Value &jv);
    void ChangeAccountPasswordToDescList(Json::Value &jv, std::list<std::string> &listDesc);

    std::string SendBlackDeviceListToJson(bool forbiden, std::vector<std::string> &deviceList);
    std::string SendBlackDeviceListJsonToDesc(Json::Value &jv);
    void SendBlackDeviceListToDescList(Json::Value &jv, std::list<std::string> &listDesc);

    // 勋章 添加、删除
    std::string AddMedalToJson(int nRoleID, int nMedalID);
    std::string AddMedalJsonToDesc(Json::Value &jv);
    void AddMedalToDescList(Json::Value &jv, std::list<std::string> &listDesc);

    std::string DeleteVersionMedalToJson();
    std::string DeleteVersionMedalJsonToDesc(Json::Value &jv);
    void DeleteVersionMedalToDescList(Json::Value &jv, std::list<std::string> &listDesc);


    // 信息中心 增改删
    std::string AddInfoCenterToJson(InfoCenterItemCM &infocenter);
    std::string AddInfoCenterJsonToDesc(Json::Value &jv);
    void AddInfoCenterToDescList(Json::Value &jv, std::list<std::string> &listDesc);

    std::string UpdateInfoCenterToJson(InfoCenterItemCM &infocenter);
    std::string UpdateInfoCenterJsonToDesc(Json::Value &jv);
    void UpdateInfoCenterToDescList(Json::Value &jv, std::list<std::string> &listDesc);

    std::string RemoveInfoCenterToJson(int nInfoId);
    std::string RemoveInfoCenterJsonToDesc(Json::Value &jv);
    void RemoveInfoCenterToDescList(Json::Value &jv, std::list<std::string> &listDesc);

}


#endif

