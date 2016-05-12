#ifndef __LIBSERVERFRAME_ROOMLIBBASE_H__
#define __LIBSERVERFRAME_ROOMLIBBASE_H__


#include <string>
#include <vector>
#include <map>

#include "../Macros/Macros.h"
#include "RoomDataStruct.h"

class LWRoomDanceInfo;
class LWRoomPosManager;
class GameMsg_Base;

class LWRoomBase
{
protected:
    LWRoomBase(ERoomType eType, unsigned int nRoomID);

public:
    virtual ~LWRoomBase();

public:
    void SetPassword(const std::string &rPassword) { m_strPassword = rPassword; }
    void ChangeRoomInfo(const std::string &rName, const std::string &rPassword = "");

    bool HasPassword() const { return !m_strPassword.empty(); }
    bool CheckPassword(const std::string &rPassword) const { return (m_strPassword == rPassword); }

    // 房主ID
    unsigned int HostRoleID();
    void SetHost(unsigned int nHostID);

    // 房间函数
    virtual EQuitRoomRes OnQuitRoom(unsigned int nRoleID, char quitTo);
    virtual EEnterRoomRes OnTryEnterRoom(unsigned int nRoleID, char nRoomRoleType, char nPhoneOs);
    virtual EKickPlayerRes OnKickPlayer(unsigned int nKickRole, char nKickedRoleRoomType, char nKickedRolePos, unsigned int nKickedRole);
    virtual EChangeRoleRoomStateRes OnChangeRoleRoomState(unsigned int nRoleID, char toState);
    virtual EChangeRoomPosStateRes OnChangeRoomPosState(unsigned int nRoleID, ERoleRoomType eRoleType, char roomPos, char roomState);
    virtual EPromoteRoomHost OnPromoteRoomHost(unsigned int nRoleID, char nNewHostRoomType, char nNewHostPos);

    // 房间回调函数
    virtual void OnRoomDanceAutoStart();
    virtual void OnRoomDanceAutoEnd();
    virtual void OnRoomDanceStartCallBack();
    virtual void OnRoomDanceEndCallBack();

    virtual void OnRoomDestoryCallBack();

public: // dance need.
    // 返回跳舞组件
    virtual LWRoomDanceInfo* GetRoomDanceInfo() { return NULL; }
    // 返回房间位置管理组件
    virtual LWRoomPosManager* GetPosManager() { return NULL; }
    // 收听房间广播的玩家,roleID, phone type.
    virtual void ListenerRole(std::vector<std::pair<unsigned int, char> > &vecListner);
    // 跳舞开始前特殊检测
    virtual EStartRoomRes StartRoomDanceSpecialTest(unsigned int nRoleID) { return StartRoom_Success; }
    // 房间编号，一般情况下，直接用房间ID做为编号。
    // 特殊情况下，比如秘境房间，需要 有房间ID和房间编号的转换规则
    virtual unsigned int RoomSerial() const;

public:
    // 房间ID
    unsigned int GetOriRoomID() const;

protected:
    CLASS_PROPERTY_READONLY(ERoomType, m_eType, Type);
    CLASS_PROPERTY_BY_REF(std::string, m_strName, RoomName);
    
protected:
    unsigned int m_nRoomID;
    unsigned int m_nHostRoleID;
    std::string m_strPassword;
};


// 紧管理房间的生存。房间自身的逻辑由房间处理
class LWRoomManager
{
public:
    LWRoomManager(ERoomType eRt);
    virtual ~LWRoomManager();

    virtual bool Init(){ return true; };

    LWRoomBase* CreateNewRoom(); // 创建房间
    LWRoomBase* GetRoom(unsigned int nRoomID);
    void DestroyRoom(unsigned int nRoomID); // 删除一个房间

    void AddToWaitStartList(time_t tillTime, unsigned int nRoomID);             // 加入等待开启列表
    void RemoveFromWaitStartList(unsigned int nRoomID);                         // 从等待开启列表中移出

    void AddToWaitEndList(time_t tillTime, unsigned int nRoomID);               // 加入等待结束列表
    void RemoveFromWaitEndList(unsigned int nRoomID);                           // 从等待结束列表中移出
    // auto start/close room dance. 
    virtual void OnMgrRoomDanceAutoStart(LWRoomBase* pRoom);
    virtual void OnMgrRoomDanceAutoEnd(LWRoomBase* pRoom);

    // 刷新manager.
    virtual void OnUpdate(time_t now);              
    virtual ECreateRoomRes CreateRoom(unsigned int nCreateRole, const 
        std::string &strRoomName, const std::string strRoomPwd, const char phoneOs);
    // 退出房间
    virtual EQuitRoomRes QuitFromRoom(LWRoomBase *room, unsigned int nRoleID, char quitTo); // 退出房间
    // 玩家下线：
    virtual void OnRoleLogout(LWRoomBase *room, unsigned int nRoleID); // 玩家退出房间

    virtual void OutRole(LWRoomBase *room, unsigned int nRoleID, char quitTo, ERoomQuitType quitType);
    // 子类Mgr处理自己关于房间的一些销毁工作
    virtual void OnDestroyRoom(unsigned int nRoomID);
    // 房间编号转换为ID
    virtual unsigned int RoomSerialToID(unsigned int nRoomSerial) const;
protected:
    // 创建自己类型的房间
    virtual LWRoomBase *RoomFactory(unsigned int nRoomID) = 0;
    
protected:
    // 如果容器空间不够，则更改容器大小。
    void MakeSureRoomSize(unsigned int nRoomID); // 

private:
    typedef struct _TimeRoom{
        time_t tillTime;
        unsigned int nRoomID;
    } TimeRoom;
    typedef std::list<TimeRoom> TimeRoomList;
    typedef std::map<unsigned int, TimeRoomList::iterator> TimeRoomMap;

    void AddToWaitList(time_t tillTime, unsigned int nRoomID, TimeRoomList &roomList, TimeRoomMap &roomMap);
    void RemoveFromWaitList(unsigned int nRoomID, TimeRoomList &roomList, TimeRoomMap &roomMap);

protected:
    unsigned int    m_nPreUsedRoomID;
    std::vector<LWRoomBase *> m_vectRooms;
    std::list<LWRoomBase *> m_listIdleRooms; // 空闲房间。
    ERoomType m_eRoomType;

    // wait List
    TimeRoomList m_waitStartRoomList;
    TimeRoomList m_waitEndRoomList;
    TimeRoomMap m_waitStartRoomMap;
    TimeRoomMap m_waitEndRoomMap;
};

// LWRoomBase* RoomFactory(ERoomType eRT); // 创建


namespace RoomCommonFunc
{
    // error msg.
    std::string CreateRoomErrorString(ECreateRoomRes eRes); //
    std::string EnterRoomErrorString(EEnterRoomRes eRes); //
    std::string QuitRoomErrorString(EQuitRoomRes eRes); //
    std::string KickPlayerErrorString(EKickPlayerRes eRes); //
    std::string ChangeRoleRoomStateErrorString(EChangeRoleRoomStateRes eRes); //
    std::string ChangeRoomPosStateErrorString(EChangeRoomPosStateRes eRes); //
    std::string PromoteRoomHostErrorString(EPromoteRoomHost eRes);
    std::string StartRoomErrorString(EStartRoomRes eRes); //

};



#endif // __LIBSERVERFRAME_ROOMLIBBASE_H__