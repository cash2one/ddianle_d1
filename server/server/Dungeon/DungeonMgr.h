#ifndef __DUNGEONMGR_H__
#define __DUNGEONMGR_H__

#include "DungeonDefine.h"
#include "../../datastructure/DataStruct_Base.h"
#include <set>
#include "Dungeon.h"
#include "EntityDungeonComponent.h"
#include "../../datastructure/datainfor/MagicLampConfig.h"


class CDungeon;
class CDungeonMgr
{
public:
    CDungeonMgr();
    virtual ~CDungeonMgr(){};

    static CDungeonMgr & Instance();

public:
    // 时间心跳
    void OnUpdate(const unsigned long& lTimeElapsed);
    void _InEndingList( CDungeon* pRoom );
    void _OutEndingList( CDungeon* pRoom );

    // 计算歌曲round个数
    unsigned int CalcMusicRoundCount( const std::string& strStageFile, Phone_OS osType );

    // 创建关卡
    int CreateParclose( EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID, unsigned int nFriendID, CEntityDungeonComponent & rRoleDungeonCOM );
    bool LoadStageInfo( CDungeon * pDungeonRoom, int nPhoneOS );

    // 生成关卡ID
    unsigned int GenerateParcloseID(){ return m_nParcloseIDIndex++; }

    // 分数同步
    void OnReportRoundMark( CDungeon * pRoom, unsigned int nRoleID, std::string  m_strMD5Code, unsigned int nMark, char cKeyRank, unsigned char cRound, const std::vector<char> & vecRoundRank);

    // 进度条同步
    void OnLoadingStartGameProgress( const GameMsg_Base& rMsg, unsigned int nRoleID, unsigned int nRoomID );

    // 关卡结束
    void OnParcloseEnd( unsigned int nRoomID, unsigned int nRoleID );

    // 特效同步
    void OnReportEffectChange(const GameMsg_Base& rMsg, unsigned int nRoomID, unsigned int nRoleID );

    // 添加/移除关卡
    void AddParcloseRoom( CDungeon * pDungeon );
    void RemoveParcloseRoom( unsigned int nRoomID );

    // 查找关卡
    CDungeon * FindRoom( unsigned int nRoomID );

    // 计算经验
    unsigned int CalcExp( unsigned int nDanceCount, unsigned int nMusicLevel, int nGrade );

    // 获取当天邀请好友情况
    std::set<unsigned int> * GetInvitedFriendInfo( unsigned int nRoleID);
    void AddInvitedFriend( unsigned int nSelfRoleID, unsigned int nFriendID );
    void ClearInvitedFriendInfo( unsigned int nRoleID );

    // 获取/设置邀请好友最新时间
    void SetInvitedFriendTime( unsigned int nRoleID, unsigned int nTime);
    unsigned int GetInvitedFriendTime( unsigned int nRoleID);

    //记录角色跳舞信息
    void _LogRoleDancing(unsigned int nRoleID, CRoleDancingLog::EDancingAction eDancingAction,
        int nScene, int nTeamMode, int nMusicID, int nMusicMode, int nMusicLevel, int nPlayerCount, int nRank, int nScore);

private:
    // 计算grade
    int CalcuGrade( CEntityDungeonComponent& rRoleDungeonCOM );

private:
    unsigned int							 m_nCurrentDungeonID; // 正在进行副本
    unsigned int                             m_nParcloseIDIndex;  // 关卡ID
    std::map<unsigned int,CDungeon*>         m_Rooms;			  // 关卡列表
    unsigned long							 m_nMgrNowTime;
    std::multimap<unsigned long, CDungeon*>  m_ToEndParcloseMap;		//key - time
    std::map<unsigned int,std::set<unsigned int> > m_InvitedFriend;     //roleid - friendid
    std::map<unsigned int,unsigned int>      m_InviteFriendTime;
    unsigned int                             m_nOnceGetVitNum;
    unsigned int                             m_nDailyMaxGetVitNum;
    unsigned int                             m_nGiveGetVitResetTime;
    unsigned int                             m_nGetVitNum;
};


#endif
