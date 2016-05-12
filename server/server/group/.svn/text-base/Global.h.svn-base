/********************************************************************
	created:	 2012/01/10
	author:		 meijiuhua
    FileName:     Global.h
    Description:  global data manager (Group Server) Head File
********************************************************************/

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include  <time.h>
#include  <string>
#include  <list>
#include  <map>
#include  "../../datastructure/Macro_Define.h"
#include  "../../datastructure/DataStruct_Base.h"
#include "../../datastructure/ImageData.h"

#include "../login/GameMsg_S2G_Login.h"
#include "../roomLib/RoomDataStruct.h"

using namespace std;

class  CGroupRoleEntity;
class CGroupPlayerManager;
class GameMsg_Base;
extern map<unsigned int , CGroupRoleEntity> g_mapRoleEntity;

//给游戏服务器发消息, 指定第几线(0-9)
void SendMsg2GameServer(LPGameMsg_Base pMsg, int nLine);
//向游戏服务器广播消息，某一线除外（默认值LINES_IN_GROUP表示没有例外）
void BroadcastMsg2GameServer(LPGameMsg_Base pMsg, int nLineExcluded);
//向游戏服务器广播消息
void BroadcastMsg2GameServer(LPGameMsg_Base pMsg);


//向客户端转发消息
void DirectMsgToClient(GameMsg_Base* pMsg, const PLAYER_TAG& tagRole, int nLine);
void DirectMsgToClient(GameMsg_Base* pMsg, CGroupRoleEntity* pRole);

//发送聊天框消息
bool  SendChatWindowMessage(CGroupRoleEntity*  pRole, const char* szText);
//发送弹出框消息
bool  SendCommDialogMessage(CGroupRoleEntity*  pRole, const char* szText);

#ifndef WIN32
#define _vsnprintf	vsnprintf
#endif

//group server上的RoleEntity类
class CGroupRoleEntity
{
public:
    CGroupRoleEntity();
    ~CGroupRoleEntity();

public:
    __int64         m_nSession;
    unsigned int    m_nAccount;
    unsigned int    m_nRoleID;
    char            m_szRoleName[MAX_NAME_LEN]; // 角色名称
    unsigned char   m_nSex;
    unsigned short  m_nIndex;
    unsigned short  m_nLevel;
    int             m_nMoney;
    unsigned int    m_nStarCount;               // 闯关 幸运星
    bool            m_bIsVIP;
    unsigned short  m_nVIPLevel;
    int             m_nLine;                    // 角色在哪条线上
    string          m_strLineName;
    unsigned int    m_nRoomID;
    int             m_nRoleRoomType;
    int             m_nRoleRoomState;
    int             m_nRoleLocation;
    unsigned char   m_bTodayFirstLogin;         // 是不是今天第一次登录
    short           m_nAge;
    short           m_nConstellation;
    unsigned int    m_nLoginTimeOnGroup;        // GroupServer上登陆时间
    unsigned short  m_nTopParcloseID;           // 最高关卡ID
    unsigned int    m_nDivorceAgreementOtherRoleID; // 离婚协议书对方角色ID
    unsigned int    m_nDanceGroupID;
    unsigned char   m_nSkinColor;												// 皮肤颜色
    unsigned short m_nTransFormID;      // 变身ID

    CCoupleInfo     m_coupleInfo;
    float           m_fX;//经度
    float           m_fY;//纬度

    RoleRoomImageData m_roomImageData;          // 同步维护可视化信息

private:
    unsigned int    m_nGroupRoomID;
    ERoomType       m_eGroupRoomType;

public:
    list<unsigned int>  m_listFriend;   // 好友名单；只有roleid
    list<unsigned int>  m_listBlackList;// 黑名单
    //list<unsigned int>	m_listMuteList;// 屏蔽名单

public:
    unsigned int GetRoleID() { return m_nRoleID; }

    // 获取幸运星数目
    unsigned int GetStarCount() { return m_nStarCount; }
    void SetStarCount(unsigned int nStarCount) { m_nStarCount = nStarCount; }

    // 最高关卡ID
    unsigned int GetTopParcloseID() { return m_nTopParcloseID; }
    void SetTopParcloseID(unsigned short nParcloseID) { m_nTopParcloseID = nParcloseID; }


    void SetRoleName(const char * szName);// 必须传入大小为MAX_NAME_LEN长度的数组或者指针指向的内存地址
    const char* GetName() { return m_szRoleName; }
    unsigned short GetLevel() { return m_nLevel; }
    int GetMoney() { return m_nMoney; }
    unsigned char GetSex() { return m_nSex; }
    void SetLevel(unsigned short nLevel) { m_nLevel = nLevel; }
    void SetMoney(int nMoney) { m_nMoney = nMoney; }
    int GetLine() { return m_nLine; }
    const string& GetLineName() { return m_strLineName; }
    int GetRoomState();
    unsigned int GetSyncRoomID() { return m_nRoomID; }
    int GetRoleLocation() { return m_nRoleLocation; }
    void SetRoleLocation(int nLocation) { m_nRoleLocation = nLocation; }
    void SetRoleAge(short nAge) { m_nAge = nAge; }
    short GetRoleAge() const { return m_nAge; }
    void SetRoleConstellation(short nConstellation) { m_nConstellation = nConstellation; }
    short GetRoleConstellation() const { return m_nConstellation; }
    PLAYER_TAG GetPlayer_Tag();
    bool ModifyMoney(int nValue,int nCauseType,int nAffixData);
    void SysChat(const char* szFormat, ...);//聊天框消息
    bool AddUpdateMsg(GameMsg_Base*pMsg, bool bNotifySelf, bool bNotifyOther);
    //好友和黑名单
    bool HasFriend(unsigned int nRoleID);
    bool HasBlackList(unsigned int nRoleID);
    void AddFriend(unsigned int nRoleID);
    void RemoveFriend(unsigned int nRoleID);
    void AddBlackList(unsigned int nRoleID);
    void RemoveBlackList(unsigned int nRoleID);
    const std::list<unsigned int>& GetFriendList() { return m_listFriend; }
    //void RemoveMuteList(unsigned int nRoleID);
    bool IsInBlackList(unsigned int nFriendID);
    const list<unsigned int>& GetBlackList() { return m_listBlackList; }

    void SetIsVIP(bool bIsVIP);
    bool IsVIP() const;

    void SetVIPLevel(unsigned short nVIPLevel);
    unsigned short VIPLevel() const;

    //情侣婚姻
    const CCoupleInfo& GetCoupleInfo() { return m_coupleInfo; }
    void SetPairID(unsigned int nPairID);
    unsigned int PairID();

    void SetCoupleID(unsigned int nCoupleID);
    unsigned int CoupleID();

    unsigned int GetDivorecAgreementOtherRoleID() { return m_nDivorceAgreementOtherRoleID; }

    void SetCoupleDate(unsigned int nCoupleDay);
    unsigned int CoupleDate();

    void SetMarriageDate(unsigned int nMarriageDate);
    unsigned int MarriageDate();

    void SetCoupleIntimacy(int nIntimacyInc);

    void SetCoupleName(std::string& strName);
    void SetCoupleSmallRoleID(unsigned int nRoleID) { m_coupleInfo.m_nSmallRoleID = nRoleID; }

    void ResetCoupleInfo();

    void SetDanceGroupID(unsigned int nDanceGroupID) { m_nDanceGroupID = nDanceGroupID; }
    unsigned int GetDanceGroupID() const { return m_nDanceGroupID; }

    float GetX() const { return m_fX; }
    void SetX(float val) { m_fX = val; }
    float GetY() const { return m_fY; }
    void SetY(float val) { m_fY = val; }

    // 更新人物属性
    void UpdateRoleInfo(CWorldRoleUpdateAttrInfo &updateInfo);
    void UpdateRoomInfo(CWorldRoleUpdateRoomInfo &updateInfo);

    unsigned char GetSkinColor() const { return m_nSkinColor; }
    void SetSkinColor(unsigned char newColor) { m_nSkinColor = newColor; }
    unsigned short GetTransformID() const{ return m_nTransFormID; }
    void SetTransformID(unsigned short nTransFormID) { m_nTransFormID = nTransFormID; }
    // Group上房间相关的信息
    ERoomType GetGroupRoomType();
    unsigned int GetGroupRoomID();
    void ClearGroupRoom(); // 不在房间中了
    void SetNewGroupRoom(ERoomType roomType, unsigned int nRoomID);

public:
    void SendPlayerMsg(GameMsg_Base *pMsg);
    static void SendPlayerMsgByAccountID(GameMsg_Base &msg, unsigned int nAccount);
    static void SendPlayerMsgByRoleID(GameMsg_Base &msg, unsigned int nRoleID);
};

// 离线人物形象数据管理( 婚姻祝福排行榜,婚姻动态 )
class CGroupOffLineRoleEntity
{
public :
    CGroupOffLineRoleEntity();
    virtual ~CGroupOffLineRoleEntity();

public :
    bool IsLoadImageData();

    // 获取离线数据
    RoleImageOfflineData * GetOfflineRoleImageData();
    void GetOfflineRoleImageData(RoleImageOfflineData & offlineData);

    // 更新数据
    void UpdateOfflineData(const RoleImageOfflineData & offlinedata);

    bool IsLoadOfflineTimeData();
    unsigned int GetOfflineTimeData();
    unsigned int * GetOfflineTimePoint();
    void UpdateOfflineTimeData(unsigned int nOfflineTime);

private :
    RoleImageOfflineData              m_OfflineRoleImageData;        // 形象数据
    unsigned int                      m_nOfflineTime;                // 离线时间
};

// 采用预先加载的方式( 在获取的时候，必须先加载,不是时时查询的 )
class  CGroupOffLinePlayerManager
{
public:
	CGroupOffLinePlayerManager();
	virtual ~CGroupOffLinePlayerManager();

public :
    // 初始化
    static CGroupOffLinePlayerManager& Instance();

	// 获取离线玩家形象数据
	CGroupOffLineRoleEntity * GetOfflineRoleImageData(unsigned int nRoleID);

	// 加载离线玩家形象数据
	void LoadOfflineRoleImageData(unsigned int nRoleID);
    void LoadOfflineRoleImageData(const std::set<unsigned int> & setRoleID);


    void LoadOfflineTimeData(unsigned int nRoleID);
    unsigned int * GetOfflineTimePoint(unsigned int nRoleID);
    unsigned int GetOfflineTime(unsigned int nRoleID);
    void UpdateOfflineTimeData(unsigned int nRoleID, unsigned int nOfflineTime);

private :
    std::map<unsigned int,CGroupOffLineRoleEntity*>     m_OffLinePlayerMgr;
};

//对RoleEntity进行管理的类
class  CGroupPlayerManager
{
protected:
	CGroupPlayerManager();
	~CGroupPlayerManager();

public:
	static CGroupPlayerManager& Instance()
	{
		static CGroupPlayerManager stcGPMgr;
		return stcGPMgr;
	}
	void OnLogin(GameMsg_S2G_Login *pMsg, int nLine, const string &strLineName); //角色上线（把对象插入map）
	//void OnLogout(LPGAMEMSG_S2G_Logout pMsg);//角色下线（把对象从map移除）
	//void OnRoomStatChange(GAMEMSG_S2G_RoomStatChange * pMsg);
	//void OnRoleNameChange(GAMEMSG_S2G_RoleNameChange * pMsg);
	//void OnRoleSexChange(GAMEMSG_S2G_RoleSexChange * pMsg);
	//void OnRoleLevelChange(GAMEMSG_S2G_RoleLevelChange * pMsg);
	//void OnRolePreChange(GAMEMSG_S2G_RolePreChange * pMsg);
	//void OnRoleMoneyChange(GAMEMSG_S2G_RoleMoneyChange * pMsg);
	//void OnRoleContributeChange(GAMEMSG_S2G_RoleContributeChange * pMsg);
	//void OnRefresh(GameMsg_Base* pMsg);	//角色数据刷新
	void OnGameServerDisconnect(int nLine);//游戏服务器断开(清除所有在该线上的玩家信息)
	//全区公告
	void NoticeAllPlayer(char * strContent);

    // SendMsgToAllPlayer:
    void SendMsgToAll(GameMsg_Base &msg);

	CGroupRoleEntity * SearchRoleByID(unsigned int nRoleID)
	{
		map<unsigned int , CGroupRoleEntity>::iterator  pos; 
		if ( (pos= g_mapRoleEntity.find(nRoleID)) != g_mapRoleEntity.end() )
		{
			return  &(pos->second);
		}
		return  NULL;	
	}

	CGroupRoleEntity* SearchRoleByName(const char* szName)
	{
		map<unsigned int , CGroupRoleEntity>::iterator it;
		for(it=g_mapRoleEntity.begin();it!=g_mapRoleEntity.end();++it)
		{
			if(strcmp(it->second.GetName(), szName)==0)
			{
				return &(it->second);
			}
		}
		return NULL;
	}

	CGroupRoleEntity * SearchRoleByAccountID(unsigned int nAccountID);

	void GetAllGroupPlayer( );
};


#endif//__GLOBAL_H__





//file end

