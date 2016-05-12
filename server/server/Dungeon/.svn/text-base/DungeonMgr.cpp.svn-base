#include "DungeonMgr.h"
#include "DungeonDataMgr.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../PhotoRoom/EntityPhotoRoomComponent.h"
#include "../room/RoomComponent.h"
#include "../AmuseRoom/AmuseRoomComponent.h"
#include "../../socket/Md5.h"
#include "../../socket/Ini.h"
#include "../../socket/Formatter.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../grade/GradeInfoMgr.h"
#include "../util/CommonFunc.h"
#include "../roomLib/GameMsg_C2S_RoomMsg.h"
#include "../roomLib/GameMsg_S2C_RoomMsg.h"
#include "../roomLib/RoomSettingMgr.h"
#include "../SystemSetting/SystemSettingMgr.h"
#include "../music/MusicStageFileManager.h"
#include "../util/Split.h"
#include "../dancegroup/EntityDanceGroupComponent.h"


#include <fstream>

extern string g_sPath;

CDungeonMgr::CDungeonMgr()
    :m_nCurrentDungeonID(0)
    ,m_nParcloseIDIndex( 0 )
    ,m_nMgrNowTime(0)
{

}

CDungeonMgr & CDungeonMgr::Instance()
{
    static CDungeonMgr s_instance;
    return s_instance;
}

void CDungeonMgr::OnUpdate(const unsigned long& lTimeElapsed)
{
    m_nMgrNowTime += lTimeElapsed;
    std::multimap<unsigned long, CDungeon*>::iterator it = m_ToEndParcloseMap.begin();
    for ( ; it != m_ToEndParcloseMap.end(); )
    {
        if ( it->first <= m_nMgrNowTime )
        {
            CDungeon* pRoom = it->second;
            if ( pRoom != NULL )
            {
                pRoom->ParcloseEnd();
                it++; // m_ToEndParcloseMap中的 这个pRoom 会在RemoveRoom方法中删除
                RemoveParcloseRoom( pRoom->RoomID() );
            }
            else
            {
                m_ToEndParcloseMap.erase( it++ );
            }
        }
        else
        {
            break;
        }
    }
}

void CDungeonMgr::_InEndingList( CDungeon* pRoom )
{
    unsigned long lNowTime = m_nMgrNowTime + RoomSettingMgr::Instance().RoomEndWait();
    m_ToEndParcloseMap.insert( std::make_pair( lNowTime, pRoom ) );
}

void CDungeonMgr::_OutEndingList( CDungeon* pRoom )
{
    if ( pRoom != NULL )
    {
        std::multimap<unsigned long, CDungeon*>::iterator it = m_ToEndParcloseMap.begin();
        for ( ; it != m_ToEndParcloseMap.end(); ++it )
        {
            if ( it->second != NULL && it->second->RoomID() == pRoom->RoomID() )
            {
                m_ToEndParcloseMap.erase( it );
                break;
            }
        }
    }
}

unsigned int CDungeonMgr::CalcMusicRoundCount( const std::string& strStageFile, Phone_OS osType )
{
    std::vector<char> vecStageData;
    if (!CMusicStageFileManager::LoadStage(strStageFile, osType, vecStageData))
    {
        WriteLog(LOGLEVEL_ERROR, "CDungeonMgr::CalcMusicRoundCount load stage failed!");
        return 0;
    }

    std::string text(vecStageData.begin(), vecStageData.end());
    bool bFoundStageData = false;
    bool bIsOsuStage = false;   // 泡泡模式关卡标志
    if (text.find("[HitObjects]") != text.npos) // 泡泡模式关卡数据部分是[HitObjects]开头的
    {
        bFoundStageData = true;
        bIsOsuStage = true;
    }
    else if (text.find("#PATSTART") != text.npos) //所有其它模式的关卡数据部分都是#PATSTART开头的
    {
        bFoundStageData = true;
        bIsOsuStage = false;
    }

    unsigned int nRoundCount = 0;
    if (bFoundStageData)
    {
        std::vector<std::string> lines;
        Split(text, "\r\n", lines);
        for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it)
        {
            std::string &strLine  = *it;
            if (bIsOsuStage)
            {
                std::vector<std::string> strTemp;
                SplitString(strLine, ",", strTemp);

                if ( strTemp.size() == 7 )
                {
                    nRoundCount++;
                }
            }
            else
            {
                if (strLine != "#PATSTART" && strLine.size() >= 10)
                {
                    nRoundCount++;
                }
            }
        }

        if (!bIsOsuStage)
        {
            nRoundCount = nRoundCount/2;
        }
    }

    return nRoundCount;
}

bool CDungeonMgr::LoadStageInfo( CDungeon * pDungeonRoom, int nPhoneOS )
{
    std::vector<char> vecAndroidStage;
    std::vector<char> vecIosStage;
    std::vector<char> vecWinStage;
    const std::string& strStageFile = pDungeonRoom->PlayMusicStage();

    if ( !strStageFile.empty() && CMusicStageFileManager::LoadStage( strStageFile, vecAndroidStage, vecIosStage, vecWinStage ) )
    {
        int nStageTime = CMusicStageFileManager::CalcuStageTime( pDungeonRoom->PlayMusicMode(), vecAndroidStage, vecIosStage, vecWinStage );
        pDungeonRoom->SetCheckTime( nStageTime );

        return true;
    }

    return false;
}


int CDungeonMgr::CreateParclose( EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID, unsigned int nFriendID, CEntityDungeonComponent & rRoleDungeonCOM )
{	 
    if (rRoleDungeonCOM.GetRoleRoom()->IsInRoom())
    {
        return EDungeonError_InRoom;
    }

    if (rRoleDungeonCOM.GetRoleCouple()->WeddingRoom() != NULL)
    {
        return EDungeonError_InCoupleRoom;
    }

    if (rRoleDungeonCOM.GetRolePhoto()->PhotoRoom() != NULL) 
    {
        return EDungeonError_InPhoteRoom;
    }

    if (rRoleDungeonCOM.GetRoleAmuse()->AmuseRoom() != NULL)
    {
        return EDungeonError_InAmuseRoom;
    }

    if (rRoleDungeonCOM.GetRoleDanceGroup()->GetCeremonyRoom() != NULL)
    {
        return EDungeonError_InCeremonyRoom;
    }

    CRoleEntity * pFriendEntity = CPlayerManager::Instance().GetEntityByRoleID( nFriendID );
    if ( nFriendID > 0 && pFriendEntity == NULL )
    {
        return EDungeonError_FriendNotOnLine;
    }

    unsigned int nRoomID = GenerateParcloseID();
    CParcloseConfig * pParcloseConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetParcloseConfig( eType, nDungeonID, nParcloseID );
    if ( pParcloseConfig == NULL)
    {
        return EDungeonError_NotFindParcloseID;
    }

    CDungeon* pDungeon = new CDungeon( nRoomID, eType, nDungeonID, *pParcloseConfig );
    if ( pDungeon == NULL )
    {
        return EDungeonError_DungeonCreateNew;
    }

    // 设置自己相关信息
    rRoleDungeonCOM.SetRoomInfo( pDungeon, RoleRoomState_Start );
    pDungeon->SetHostComponent( &rRoleDungeonCOM );

    // 设置好友相关信息
    if ( nFriendID > 0 )
    {
        pFriendEntity->GetRoleDungeon()->SetRoomInfo( pDungeon, RoleRoomState_Start );
        pDungeon->SetFriendID( nFriendID );
        pDungeon->SetRoomTeamMode(EDanceRoomTeamMode_Team);
        pDungeon->SetFriendComponent( pFriendEntity->GetRoleDungeon() );
        rRoleDungeonCOM.RoleMarkInfo().SetFriendID( nFriendID );
        //
        if ( rRoleDungeonCOM.GetRoleCouple()->CoupleID() == nFriendID )
        {
            rRoleDungeonCOM.RoleMarkInfo().SetCoupleID( nFriendID );
        }
    }

    // 初始化
    pDungeon->RoomPrepare();

    // 加载stage配置
    if ( !LoadStageInfo( pDungeon, rRoleDungeonCOM.GetPhoneOS()))
    {
        rRoleDungeonCOM.ClearRoomInfo();
        rRoleDungeonCOM.GetRoleRoom()->OutRoom();

        CRoleEntity * pRoleEntity = CPlayerManager::Instance().GetEntityByRoleID( nFriendID );
        if ( pRoleEntity != NULL )
        {
            pRoleEntity->GetRoleDungeon()->ClearRoomInfo();
            pRoleEntity->GetRoleRoom()->OutRoom();
        }

        return EDungeonError_StageNotFind;
    }

    //  添加
    AddParcloseRoom( pDungeon );

    // 自己日志
    _LogRoleDancing(rRoleDungeonCOM.GetRoleID(), CRoleDancingLog::EDancingAction_Start,
        pDungeon->PlayScene(), pDungeon->TeamMode(), pDungeon->PlayMusicID(), pDungeon->PlayMusicMode(), pDungeon->PlayMusicLevel(),
        pDungeon->DancerCount(), -1, 0);

    // 好友日志
    if (nFriendID > 0)
    {
        _LogRoleDancing(nFriendID, CRoleDancingLog::EDancingAction_Start,
            pDungeon->PlayScene(), pDungeon->TeamMode(), pDungeon->PlayMusicID(), pDungeon->PlayMusicMode(), pDungeon->PlayMusicLevel(),
            pDungeon->DancerCount(), -1, 0);
    }

    return EDungeonError_Sucess;
}

void CDungeonMgr::AddParcloseRoom( CDungeon * pDungeon )
{
    std::map<unsigned int,CDungeon*>::iterator it = m_Rooms.find( pDungeon->RoomID() );
    if ( it == m_Rooms.end() )
    {
        m_Rooms[ pDungeon->RoomID() ] = pDungeon;
    }
}

void CDungeonMgr::RemoveParcloseRoom( unsigned int nRoomID )
{
    std::map<unsigned int,CDungeon*>::iterator it = m_Rooms.find( nRoomID );
    if ( it != m_Rooms.end() )
    {
        _OutEndingList( it->second );
        delete it->second;it->second = NULL;
        m_Rooms.erase( it );
    }
}

CDungeon * CDungeonMgr::FindRoom( unsigned int nRoomID )
{
    std::map<unsigned int,CDungeon*>::iterator it = m_Rooms.find( nRoomID );
    if ( it != m_Rooms.end() )
    {
        return it->second;
    }

    return NULL;
}

unsigned int CDungeonMgr::CalcExp( unsigned int nDanceCount, unsigned int nMusicLevel, int nGrade )
{
    int nPlaceExp = CGradeInfoManager::Instance().GetPlaceExp( 1, nDanceCount );
    int nGradeExp = CGradeInfoManager::Instance().GetGradeExp( nMusicLevel, nGrade );

    return  nPlaceExp + nGradeExp;
}

void CDungeonMgr::OnLoadingStartGameProgress( const GameMsg_Base& rMsg, unsigned int nRoleID, unsigned int nRoomID )
{
    const GameMsg_C2S_LoadingStartRoomProgress & rMsgLoaddingRcv = (const GameMsg_C2S_LoadingStartRoomProgress &)rMsg;	

    CDungeon *  pParcloseRoom = FindRoom( nRoomID );
    if ( pParcloseRoom != NULL )
    {
        GameMsg_S2C_LoadingStartRoomProgress msgLoadingSend;
        msgLoadingSend.m_nRoleID = nRoleID;
        msgLoadingSend.m_nRate =  rMsgLoaddingRcv.m_nRate >= 100 ? 100 : rMsgLoaddingRcv.m_nRate;
        pParcloseRoom->SendMsgToAll(&msgLoadingSend);
    }
}

void CDungeonMgr::OnReportRoundMark( CDungeon * pRoom, unsigned int nRoleID, std::string strMD5Code, unsigned int nMark, char cKeyRank, unsigned char cRound, const std::vector<char> & vecRoundRank)
{
    if ( cKeyRank > RoomRoundRank_None && cKeyRank < RoomRoundRank_Max && cRound > 0 )
    {
        if ( pRoom != NULL && pRoom->HasStart() /*&& rRoleRoomCOM.RoleRoomType() == RoleRoomType_Dancer*/ )
        {
            std::string strSrcCode;
            Formatter(strSrcCode) << pRoom->PlayCheckCode() << (int)cRound << (int)cKeyRank << nMark;

            for (size_t i = 0; i < vecRoundRank.size(); ++i)
            {
                std::string strRank;
                Formatter(strRank) << (int)vecRoundRank[i];
                strSrcCode.append(strRank);
            }

            unsigned char szMD5Code[16] = { 0 };
            MDString( strSrcCode.c_str(), szMD5Code );

            std::string strCheckCode = BinaryData2String( szMD5Code, sizeof(szMD5Code) );
            if ( strMD5Code.compare( strCheckCode ) == 0 )
            {
                int nMusicMode = pRoom->PlayMusicMode();
                CEntityDungeonComponent * pDungeonComponent = pRoom->GetComponent( nRoleID );
                if ( pDungeonComponent != NULL )
                {
                    if ( pDungeonComponent->RecvRoundMark( nMusicMode, cRound, nMark, cKeyRank, vecRoundRank ) )
                    {
                        GameMsg_S2C_SyncRoundMark msgResponse;
                        msgResponse.m_chDancerPos = (char)pDungeonComponent->GetRoomPos();
                        msgResponse.m_chRound = (char)pDungeonComponent->RoleMarkInfo().Round();
                        msgResponse.m_chRank = (char)pDungeonComponent->RoleMarkInfo().LastRank();//for model3 ,this is AllComboFlag; > 1 = AllCombo;
                        msgResponse.m_nMark = pDungeonComponent->RoleMarkInfo().Mark();
                        msgResponse.m_nSpecial = pDungeonComponent->RoleMarkInfo().LastSpecialCount();
                        pRoom->SendMsgToAll( &msgResponse );

                        int nBossCount = pRoom->GetBossCount();
                        for ( int i = 0; i < nBossCount; ++i )
                        {
                            GameMsg_S2C_SyncRoundMark bossMark;
                            bossMark.m_chDancerPos = (char)(MAX_DUNGEON_PLAYER_COUNT+i);
                            bossMark.m_nMark = pRoom->GetBossScore( pRoom->GetBossID(i), cRound );
                            pRoom->SendMsgToAll( &bossMark );
                        }						
                    }
                }	
            }
        }
    }
}

void CDungeonMgr::OnParcloseEnd( unsigned int nRoomID, unsigned int nRoleID )
{
    CDungeon * pRoom = FindRoom( nRoomID );
    if ( pRoom != NULL )
    {
        CEntityDungeonComponent * pDungeonComponent = pRoom->GetComponent( nRoleID );
        if ( pDungeonComponent != NULL )
        {
            pDungeonComponent->SetRoleState( RoleRoomState_ToEnd );
            if ( pRoom->IsReadyEnd() )
            {
                _OutEndingList( pRoom );
                pRoom->ParcloseEnd();

                RemoveParcloseRoom( pRoom->RoomID() );
            }
            else
            {
                _InEndingList( pRoom );
            }
        }
    }
}

void CDungeonMgr::OnReportEffectChange(const GameMsg_Base& rMsg, unsigned int nRoomID, unsigned int nRoleID )
{
    const GameMsg_C2S_ReportEffectChange rMsgRequest = (const GameMsg_C2S_ReportEffectChange&)rMsg;

    CDungeon * pRoom =  FindRoom( nRoomID );
    if ( pRoom != NULL && pRoom->HasStart() )
    {
        std::string strSrcCode;
        Formatter(strSrcCode) << pRoom->PlayCheckCode() << rMsgRequest.m_nMark 
            << rMsgRequest.m_nSpecail << (int)rMsgRequest.m_chRound;

        unsigned char szMD5Code[16] = { 0 };
        MDString( strSrcCode.c_str(), szMD5Code );

        std::string strCheckCode = BinaryData2String( szMD5Code, sizeof(szMD5Code) );
        if ( rMsgRequest.m_strMD5Code.compare( strCheckCode ) == 0 )
        {
            GameMsg_S2C_SyncEffectState msgResponse;
            msgResponse.m_nMark = rMsgRequest.m_nMark;
            msgResponse.m_nSpecial = rMsgRequest.m_nSpecail;
            msgResponse.m_chRound = rMsgRequest.m_chRound;
            msgResponse.m_nDancerPos = (char)pRoom->GetPos( nRoleID );

            pRoom->SendMsgToAll( &msgResponse, nRoleID );
        }
    }
}

std::set<unsigned int> * CDungeonMgr::GetInvitedFriendInfo(unsigned int nRoleID)
{
    std::map<unsigned int,std::set<unsigned int> >::iterator it = m_InvitedFriend.find( nRoleID );
    if ( it != m_InvitedFriend.end() )
    {
        return &(it->second);
    }

    return NULL;
}

void CDungeonMgr::AddInvitedFriend( unsigned int nRoleID, unsigned int nFriendID )
{
    std::map<unsigned int,std::set<unsigned int> >::iterator it = m_InvitedFriend.find( nRoleID );
    if ( it != m_InvitedFriend.end() )
    {
        it->second.insert( nFriendID );
    }
    else
    {
        m_InvitedFriend[nRoleID].insert(nFriendID);
    }
}

void CDungeonMgr::ClearInvitedFriendInfo( unsigned int nRoleID )
{
    std::map<unsigned int,std::set<unsigned int> >::iterator it = m_InvitedFriend.find( nRoleID );
    if ( it != m_InvitedFriend.end() )
    {
        it->second.clear();
    }
}

void CDungeonMgr::SetInvitedFriendTime(unsigned int nRoleID, unsigned int nTime)
{
    std::map<unsigned int,unsigned int>::iterator it = m_InviteFriendTime.find( nRoleID );
    if ( it != m_InviteFriendTime.end())
    {
        it->second = nTime;
    }
    else
    {
        m_InviteFriendTime[nRoleID] = nTime;
    }
};

unsigned int CDungeonMgr::GetInvitedFriendTime(unsigned int nRoleID)
{
    std::map<unsigned int,unsigned int>::iterator it = m_InviteFriendTime.find( nRoleID );
    if ( it != m_InviteFriendTime.end())
    {
        return it->second;
    }

    return 0;
}

int CDungeonMgr::CalcuGrade( CEntityDungeonComponent& rRoleDungeonCOM )
{
    int nGrade = RoomDanceGrade_D;

    if ( rRoleDungeonCOM.RoleMarkInfo().TotalRankBase() > 0 )
    {
        int nAccuracy = 0;
        nAccuracy += rRoleDungeonCOM.RoleMarkInfo().TotalRankCount( RoomRoundRank_Perfect ) * CGradeInfoManager::Instance().GetGradePercent( RoomRoundRank_Perfect );
        nAccuracy += rRoleDungeonCOM.RoleMarkInfo().TotalRankCount( RoomRoundRank_Cool ) * CGradeInfoManager::Instance().GetGradePercent( RoomRoundRank_Cool );
        nAccuracy += rRoleDungeonCOM.RoleMarkInfo().TotalRankCount( RoomRoundRank_Good ) * CGradeInfoManager::Instance().GetGradePercent( RoomRoundRank_Good );
        nAccuracy += rRoleDungeonCOM.RoleMarkInfo().TotalRankCount( RoomRoundRank_Bad ) * CGradeInfoManager::Instance().GetGradePercent( RoomRoundRank_Bad );
        nAccuracy += rRoleDungeonCOM.RoleMarkInfo().TotalRankCount( RoomRoundRank_Miss ) * CGradeInfoManager::Instance().GetGradePercent( RoomRoundRank_Miss );
        nAccuracy /= rRoleDungeonCOM.RoleMarkInfo().TotalRankBase();

        if ( nAccuracy == 100 )
        {
            nGrade = RoomDanceGrade_SS;
        }
        else if ( nAccuracy >= 90 )
        {
            if ( rRoleDungeonCOM.RoleMarkInfo().TotalRankPercent( RoomRoundRank_Good ) < 1
                && rRoleDungeonCOM.RoleMarkInfo().TotalRankPercent( RoomRoundRank_Bad ) < 1
                && rRoleDungeonCOM.RoleMarkInfo().TotalRankCount( RoomRoundRank_Miss ) == 0 )
            {
                nGrade = RoomDanceGrade_S;
            }
            else
            {
                nGrade = RoomDanceGrade_A;
            }
        }
        else if ( nAccuracy >= 80 )
        {
            if ( rRoleDungeonCOM.RoleMarkInfo().TotalRankCount( RoomRoundRank_Miss ) == 0 )
            {
                nGrade = RoomDanceGrade_A;
            }
            else
            {
                nGrade = RoomDanceGrade_B;
            }
        }
        else if ( nAccuracy >= 70 )
        {
            if ( rRoleDungeonCOM.RoleMarkInfo().TotalRankCount( RoomRoundRank_Miss ) == 0 )
            {
                nGrade = RoomDanceGrade_B;
            }
            else
            {
                nGrade = RoomDanceGrade_C;
            }
        }
        else if ( nAccuracy >= 60 )
        {
            nGrade = RoomDanceGrade_C;
        }

    }

    return nGrade;
}

//记录角色跳舞信息
void CDungeonMgr::_LogRoleDancing(unsigned int nRoleID, CRoleDancingLog::EDancingAction eDancingAction,
    int nScene, int nTeamMode, int nMusicID, int nMusicMode, int nMusicLevel, int nPlayerCount, int nRank, int nScore)
{
    CRoleDancingLog * pLog = new CRoleDancingLog();
    pLog->m_nRoleID = nRoleID;
    pLog->m_eDancingAction = eDancingAction;
    pLog->m_nScene = nScene;
    pLog->m_nMusicID = nMusicID;
    pLog->m_nMusicMode = nMusicMode;
    pLog->m_nMusicLevel = nMusicLevel;
    pLog->m_nPlayerCount = nPlayerCount;
    pLog->m_nRank = nRank;
    pLog->m_nScore = nScore;
    pLog->m_nTeamMode = nTeamMode;
    pLog->m_bIsDungeon = true;

    g_pQueryMgr->AddQuery(QUERY_LogRoleDancing, 0, pLog);
}

