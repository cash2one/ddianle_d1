#ifndef __ENTITYDUNGEONCOMPONENT_H__
#define __ENTITYDUNGEONCOMPONENT_H__

#include "../logic/EntityComponent.h"
#include "../logic/RoleComponentImpl.h"
#include "DungeonMgr.h"
#include "../roomLib/RoomMarkInfo.h"
#include "../roomLib/RoomDataStruct.h"
#include "Dungeon.h"
#include "../../datastructure/datainfor/MagicLampConfig.h"
#include "../../datastructure/DataStruct_Base.h"

#ifdef WIN32
#pragma warning(disable:4625 4626)
#endif


class CEntityNetComponent;
class CEntityItemComponent;
class CEntityAttributeComponent;
class CRoomComponent;
class CEntityFriendComponent;
class CEntityVIPComponent;
class CEntityCoupleComponent;
class CAmuseRoomComponent;
class CEntityDanceGroupComponent;
class CPlayerInfoComponent;
class CEntityGeneComponent;
class PetComponent;
class CEntityQuestNewComponent;

class CDungeon;
class CParcloseEndTarget;

class CEntityDungeonComponent : public CEntityComponent
{
public: 
    CEntityDungeonComponent();
    virtual ~CEntityDungeonComponent(){};

public:
    virtual void Start();
    virtual void OnEvent( CComponentEvent& refEvent );
    virtual void OnLogin();
    virtual void OnLogout();
    virtual bool CreateFromDB(ROLEINFO_DB* pRoleInforDB); 
    virtual bool PacketToDB(ROLEINFO_DB *pRoleInforDB) const;
    virtual bool PacketToCache( ROLEINFO_DB* pRoleInforDB ) const;
    virtual void OnUpdate(const unsigned long &nTimeElapsed);
    virtual void SendExtraStaticData();
    virtual void SerializeComponent(CParamPool &IOBuff);
    virtual const char * SerializeName(){ return "Dungeon"; }
    virtual void PacketBuff(std::list<BuffData>& listBuff){};

public:
    // 获取功能基本信息
    void ActiveFirstParclose();
    void OnGetLanternBaseInfo( GameMsg_Base &msg, CSlotPeer &slotPeer );
    void _GetLanternBaseInfo( CParamPool &IOBuff );

    // 领取幸运星奖励
    void OnGetStarReward( GameMsg_Base &msg, CSlotPeer &slotPeer );
    int  _GetStarReward( unsigned int nRewardID );
    void _SendStarRewardErrorMsg( int nErr );

    // 获取剧情
    void OnGetDungeonStory( GameMsg_Base &msg, CSlotPeer &slotPeer );

    // 搓神灯
    void OnExchangeDustReward( GameMsg_Base &msg, CSlotPeer &slotPeer );
    int  _ExchangeDustReward();
    void _SendDustRewardErrorMsg( int nErr );

    // 获取副本详细信息
    void OnGetDungeonInfoDetail( GameMsg_Base &msg, CSlotPeer &slotPeer );

    // 获取关卡详细信息
    void OnGetParcloseInfoDetail( GameMsg_Base &msg, CSlotPeer &slotPeer );

    // 开始关卡
    void OnStartParclose( GameMsg_Base &msg, CSlotPeer &slotPeer );
    int _CheckParclose( EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID, const std::vector<itemtype_t> &items, unsigned int nFriendID = 0 );
    void _SendStartParcloseErrorMsg( int nErr );
    void OnStartDance();

    // 处理关卡消耗
    void _HandleParcloseCost( unsigned int nParcloseID, const std::vector<itemtype_t> &items );
    void _CreateMagicDust();

    // 处理邀请
    void OnInviteResponse( GameMsg_Base &msg, CSlotPeer &slotPeer );

    // 获取排行列表
    void OnGetRankList( GameMsg_Base &msg, CSlotPeer &slotPeer );
    void _OnGetRankList( GameMsg_Base &msg, CSlotPeer &slotPeer );

    // 购买水晶
    void OnGetBuyCrystalInfo( GameMsg_Base &msg, CSlotPeer &slotPeer );
    void OnBuyCrystal( GameMsg_Base &msg, CSlotPeer &slotPeer );
    int _BuyCrystal( unsigned int nIndex );
    void _SendBuyCrystalErrorMsg( int nErr );

    // 跳过关卡
    void OnSkipParclose( GameMsg_Base &msg, CSlotPeer &slotPeer );
    int _SkipParclose( EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID );
    void _SendSkipParcloseErrorMsg( int nErr );

    // 获取碎片包信息
    void OnExchangeDebrisEquip( GameMsg_Base &msg, CSlotPeer &slotPeer );
    int ExchangeDebrisEquip( unsigned int nDebrisID );
    void DeleteDebris( unsigned int nDebrisID, unsigned int nChanegCount );
    void AddDebris( unsigned int nDebrisID, unsigned int nChangeCount );
    unsigned int GetDebrisCount( unsigned int nDebrisID );

    void OnGetMainlineDungeonInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnGetThemeDungeonInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    int GetThemeState( unsigned int nOpenTime, unsigned int nEndTime );

    void GMActivedParclose( unsigned int nParcloseID );
    void GMAddDebris( unsigned int nDebrisID, unsigned int nDebrisCount );
	void GMActivedThemeParclose( unsigned int nThemeID, unsigned int nParcloseID );

    // 是否有新增主线副本
    bool IsNewMainlineParclose();

    // 是否有新增加主题副本
    bool IsNewThemeParclose();

    void OnSyncInviteFriendInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);

    CDungeon* GetDungeon() const;

private:
    // 魔法神灯是否激活
    bool IsMagicActived();

    // 是否激活
    bool ParcloseIsActived( EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID );

    // 激活关卡
    void ActiveMainlineParclose(unsigned int nParcloseID);
    void ActiveParclose(unsigned int nParcloseID );
    void ActiveThemeParclose(unsigned int nThemeID, unsigned int nParcloseID, bool bSave = true);

    // 设置关卡分数
    void SetMainlineParcloseStarCount(unsigned int nParcloseID, int nStarCount);
    void SetThemeParcloseStarCount(unsigned int nThemeID, unsigned int nParcloseID, int nStarCount);
    void SetParcloseStarCount(EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID, int nStarCount);

    // 副本是否激活
    bool IsDungeonActived( unsigned int nDungeonID );

    // 副本第一个关卡
    unsigned int GetDungeonFirstParcloseID( unsigned int nDungeonID );

    // 获取/设置邀请好友计时器
    int GetInviteFriendTimer(){return m_nInviteFriendTimer;}
    void SetInviteFriendTimer( int nTime ){m_nInviteFriendTimer = nTime;}

    // 碎片日志
    void SaveDebrisLog(int nAction,int nIsMainline, unsigned int nDebrisID, unsigned int nDebrisCount, unsigned int nDungeonID, unsigned int nParcloseID, unsigned int nLeftCount);

    // 神灯日志
    void InsertActionLog(CRoleDungeonLog::EDungeonAction eAction, unsigned int nRoleID, bool bIsMainline, unsigned int nDungeonID, unsigned int nParcloseID, int nValue);

    // 检查重置邀请信息
    void CheckAndResetInviteInfo();

    // 是否被邀请过
    bool IsInvited(unsigned int nRoleID);

//                   赠送、收取体力                   //
public:
    // 获取可以赠送好友信息
    void OnGetFriendContactInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);

    // 赠送体力
    void OnGiveVit(GameMsg_Base &msg, CSlotPeer &slotPeer);
    int GiveVitToFriend(unsigned int nFriendRoleID);
    void FriendGiveVit(unsigned int nFriendRoleID, std::string & strFriendRoleName, unsigned int nGiveVitNum);
    void _GiveVitToFriend(GameMsg_Base &msg, CSlotPeer &slotPeer);

    // 收取体力
    void OnGetVit(GameMsg_Base &msg, CSlotPeer &slotPeer);
    int GetVitFromFriend(unsigned int nFriendRoleID);

    // 获取/设置当天已经领取体力
    void SetTodayGetVitNum(int nAddNum);
    int GetTodayGetVitNum();
    void ResetTodayGetVitNum();

    // vip 购买体力
    int GetVipBuyVitCount();
    void AddTodayVipBuyVitNum(int nAddNum);

    // db
    void SyncContactToDB(const FriendContactInfo & contactinfo, ESyncToDBType eType);
    void SyncMagicToDB(ESyncToDBType eType);

    // 检查体力接收和赠送重置
    void CheckGiveGetVitDueTime();
    void CheckNextDayResetTime(unsigned int nNowTime);

    // 关卡扫荡
    void OnQuickFinishParclose(GameMsg_Base &msg, CSlotPeer &slotPeer);
    int QuickFinishParclose(bool bIsMainline, unsigned int nDungeonID, unsigned int nParcloseID);

    // 获取关卡扫荡次数
    unsigned int GetParcloseQuickFinishNum(EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID);

    // 更新扫荡次数
    void UpdateQuickFinishNum(EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID, unsigned int nNum);

public:
    // 获取激活最高关卡
    unsigned int GetTopActivedParcloseID();

    // 获取激活最高副本
    unsigned int GetTopActivedDungeonID();

    // 注册消息
    void RegComponentNetMsgMap();

    // 设置/获取OS类型
    int GetPhoneOS(){ return m_nPhoneOS; }
    void SetPhoneOS( int nOSType ){ m_nPhoneOS = nOSType; }

    // 
    unsigned int GetParcloseStarCount(EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID);

    // 设置房间信息
    void SetRoomInfo( CDungeon * pRoom, ERoleRoomState eState );
    void ClearRoomInfo();

    // 获取房间位置
    int GetRoomPos();

    // 获取许愿星
    unsigned int GetLuckStar();
    void UpdateLuckStar();

    // 获取魔法尘埃
    int GetMagicDust(){ return m_MagicLamp.m_nMagicDust; }
    void SetMagicDust( int nDust );

    // 获取水晶(体力)
    int GetCrystal() const;
    void SetCrystal(int nCrystal, bool bSave = true);
    void SetCrystalInitialValue( int nCrystal, unsigned int nCurTime );
    void CrystalReply(unsigned int nNowTime, bool bSyncToClient = true);
    void SyncCrystalToClient();

    // 获取/设置水晶回复时间
    int GetCrystalRestoreTime(){ return m_MagicLamp.m_nCrystalRestoreTime; }
    void SetCrystalRestoreTime(int nTime, bool bSave = true);

    // 获取统计
    CRoomMarkInfo & RoleMarkInfo(){ return m_MarkInfo;}

	// 添加邀请好友消息
    void AddInviteFriendInfo(unsigned int nFriendRoleID, EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID);
	void RemoveInviteFriendInfo( unsigned int nFriendRoleID );

    // 分数同步
    bool RecvRoundMark( int nMode, int nRound, unsigned int nMark, char chKeyRank, const std::vector<char>& vecRoundRank );

    // 下发消息
    void SendPlayerMsg( GameMsg_Base* pMsg );

    // 获取角色ID
    unsigned int GetRoleID(){ return m_pRoleAttr->GetRoleID(); }

    // 计算结果
    void CalcReward();
    void CalcParcloseReward(bool bSucess, EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID, ParcloseReward & parcloseReward);
    int CalcExp(bool bIsWiner, int nMusicMode, int nMusicLevel);
    void CalcTarget( unsigned int nParcloseID, std::vector<CParcloseEndTarget> & targets, bool & nIsSuc );
    void CalcGrade();
    int CalcTotalMark();
    int CalcRankMark(int nMusicMode, int nMusicLevel, int nRank, int &nSkillMark);

    // 计算通关获取的星星
    unsigned int CalcStarCount(CParcloseConfig * pParcloseConfig);

    // 关卡结算
    void ParcloseCalcResult();
    void SetRunningParcloseIndex( EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID );

    ERoleRoomState GetRoleState(){ return m_eRoleState; }
    void SetRoleState( ERoleRoomState eSate ){ m_eRoleState = eSate; }

public:
    CEntityCoupleComponent * GetRoleCouple(){ return m_pRoleCouple; }
    CEntityPhotoRoomComponent * GetRolePhoto(){ return m_pRolePhoto; }
    CAmuseRoomComponent * GetRoleAmuse(){ return m_pRoleAmuseRoom; }
    CRoomComponent * GetRoleRoom(){ return m_pRoleRoom; }
    CEntityAttributeComponent * GetRoleAttr(){ return m_pRoleAttr; }
    CEntityVIPComponent * GetRoleVip(){ return m_pRoleVip; }
    CEntityItemComponent * GetRoleItem(){ return m_pRoleItem; }
    CEntityGeneComponent * GetRoleGene(){ return m_pRoleGene; }
    CEntityDanceGroupComponent * GetRoleDanceGroup(){ return m_pRoleDanceGroup; }
    PetComponent *GetRolePet() { return m_pRolePet; }

protected:
    void LogRoleCheckingMarkFailed();

private:
    MagicLamp                                m_MagicLamp; 
    CDungeon*                                m_pParcloseRoom;               // 副本房间
    CRoomMarkInfo                            m_MarkInfo;                    // 分数统计
    int                                      m_nPhoneOS;                    // OS类型
    unsigned int                             m_nMainlineRunningIndex;       // 正在玩的主线关卡
    std::map<unsigned int,unsigned int>      m_ThemeRunningIndex;           // 正在玩的主题关卡
    MapMainlineParcloseRecord                m_MainlineParcloseRecord;      // 主线关卡记录
    MapThemeParcloseRecord                   m_ThemeParcloseRecord;         // 主题关卡记录
    std::map<unsigned int,unsigned int>      m_DebrisRecord;                // 碎片信息记录
    std::set<int>                            m_LuckStarReward;              // 许愿星奖励记录
    int                                      m_nCryStalUpdateTime;          // 同步时间
    int                                      m_nInviteFriendTimer;          // 邀请好友计时器
    unsigned int                             m_nServerActiveParcloseID;     // 第一个关卡是否第一次被激活
    std::set<unsigned int>                   m_ServerActiveThemeParcloseID; // 服务器主动激活的主题关卡
    ERoleRoomState                           m_eRoleState;
    CDungeonInviteInfo                       m_InviteInfo;                  // 副本邀请信息
    PlayerRoomStatus                         m_status;
    std::map<unsigned int,FriendContactInfo> m_mapFriendContact;            // 好友赠送体力

private:
    CRoomComponent							 * m_pRoleRoom;
    CEntityAttributeComponent				 * m_pRoleAttr;
    CEntityNetComponent						 * m_pRoleNet;
    CEntityItemComponent					 * m_pRoleItem;
    CEntityGeneComponent					 * m_pRoleGene;
    CEntityFriendComponent					 * m_pRoleFriend;
    CEntityVIPComponent						 * m_pRoleVip;
    CEntityCoupleComponent					 * m_pRoleCouple;
    CEntityPhotoRoomComponent				 * m_pRolePhoto;
    CAmuseRoomComponent						 * m_pRoleAmuseRoom;
    CEntityDanceGroupComponent				 * m_pRoleDanceGroup;
    CPlayerInfoComponent					 * m_pRolePlayerInfo;
    PetComponent					         * m_pRolePet;
    CEntityQuestNewComponent                 * m_pRoleQuestNew;

};
typedef CRoleComponentImpl<CEntityDungeonComponent,CGetRoleProcessorObj,CGetRoleProcessorObj> CRoleDungeon;

#endif
