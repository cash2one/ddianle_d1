#include "BigMamaRoom.h"
#include "DanceRoom.h"
#include "../../datastructure/RoleEntity.h"
#include "../logic/PlayerManager.h"

#include "../music/MusicStageFileManager.h"

#include "../roomLib/GameMsg_S2C_RoomMsg.h"

#include "../../socket/Md5.h"
#include "../../socket/Formatter.h"

#define INIT_ROOM_COUNT     1
#define RESERVE_ROOM_COUNT  50

BigMamaRoomManager::BigMamaRoomManager()
    : m_nPreUsedRoomID( INVALID_ROOM_ID )
    , m_vectRooms( INIT_ROOM_COUNT )
{
}

BigMamaRoomManager::~BigMamaRoomManager()
{
    for ( std::vector<DanceBaseRoom *>::iterator vecItr = m_vectRooms.begin(); 
        vecItr != m_vectRooms.end(); ++vecItr )
    {
        SAFE_DELETE( *vecItr );
    }
    m_vectRooms.clear();

    while ( !m_listIdleRooms.empty() )
    {
        SAFE_DELETE( *m_listIdleRooms.begin() );
        m_listIdleRooms.pop_front();
    }
}

unsigned int BigMamaRoomManager::CreateAndEnterRoom(unsigned int nRoleID, const std::string &rRoomName,
    int nScene, int nMusic, unsigned char nMusicMode, unsigned char nMusicLevel )
{
    DanceBaseRoom *pRoom = NULL;

    if ( !m_listIdleRooms.empty() )
    {
        pRoom = *m_listIdleRooms.begin();
        m_listIdleRooms.pop_front();
    }

    if ( pRoom == NULL )
        pRoom = new DanceBaseRoom( eRoomType_BigMama, ++m_nPreUsedRoomID, rRoomName );
    else
        pRoom->SetRoomName( rRoomName );

    if ( m_nPreUsedRoomID == m_vectRooms.size() )
        m_vectRooms.resize( m_nPreUsedRoomID + RESERVE_ROOM_COUNT );

    RoomDancer* pDancer = new RoomDancer( nRoleID );

    pRoom->SetScene( nScene );
    pRoom->ChangeChoosedMusicInfo( nMusic, nMusicMode, nMusicLevel );
    pRoom->DancerIn( pDancer );
    m_vectRooms[pRoom->GetRoomID()] = pRoom;

    return pRoom->GetRoomID();
}

bool BigMamaRoomManager::Prepare(unsigned int nRoomID, unsigned char nPhoneOS, std::string &rCheckKey, std::string &rStage)
{
    DanceBaseRoom *pRoom = FindRoom( nRoomID );

    if ( pRoom == NULL )
        return false;

    pRoom->Prepare();

    if ( !LoadStageInfo( pRoom, nPhoneOS, rStage ) )
    {
        RemoveRoom( nRoomID );

        return false;
    }

    rCheckKey = pRoom->GetCheckCode();

    return true;
}

void BigMamaRoomManager::Start(unsigned int nRoomID)
{
    DanceBaseRoom *pRoom = FindRoom( nRoomID );

    if ( pRoom == NULL )
        return;

    pRoom->Start();

    GameMsg_S2C_StartRoomSuc startMsg;
    pRoom->SendMessage( startMsg );
}

bool BigMamaRoomManager::End(unsigned int nRoomID, unsigned int nRoleID, unsigned int &rScore, CRoomMarkInfo &rMark)
{
    DanceBaseRoom *pRoom = FindRoom( nRoomID );

    if ( pRoom == NULL )
        return false;
    else if ( pRoom->GetDancerPos( nRoleID ) == INVALID_DANCE_ROOM_POS )
        return false;
    else if ( !pRoom->HasStart() )
        return false;

    if ( pRoom->HasLegalEndTime() )
    {
        pRoom->CalcDanceResult();
        rScore = pRoom->GetDancerScore( nRoleID );
        pRoom->GetDancerMark( nRoleID, rMark );
    }

    pRoom->End();

    RemoveRoom( nRoomID );

    return true;
}

// 把进度广播给房间里的所有人
void BigMamaRoomManager::OnLoadingProgress(unsigned int nRoleID, unsigned int nRoomID, unsigned char nRate)
{ // 房间里只有一个人， 把进度转发给他自己，
    DanceBaseRoom *pRoom = FindRoom(nRoomID);

    if (pRoom == NULL)
        return;
    else if (pRoom->GetState() != EDanceRoomState_Preparing) // 只有在这
        return;
    
    CRoleEntity *pRole = CPlayerManager::Instance().GetEntityByRoleID(nRoleID);
    if (NULL == pRole)
        return;

    GameMsg_S2C_LoadingStartRoomProgress msgLoadingSend;
    msgLoadingSend.m_nRoleID = nRoleID;
    msgLoadingSend.m_nRate = nRate >= 100 ? 100 : nRate;
    pRole->SendPlayerMsg(&msgLoadingSend);

    return ;
}


void BigMamaRoomManager::ReportRoundMark(unsigned int nRoomID, unsigned int nRoleID, const std::string &rMD5Code,
    unsigned int nMark, char nKeyRank, unsigned char nRound, const std::vector<char> &rRoundRank )
{
    if ( RoomRoundRank_None < nKeyRank && nKeyRank < RoomRoundRank_Max && nRound > 0 )
    {
        DanceBaseRoom *pRoom = FindRoom( nRoomID );

        if ( pRoom == NULL || !pRoom->HasStart() )
        {
            return;
        }

        std::string strCode;
        std::vector<int> vectRoundRank( rRoundRank.begin(), rRoundRank.end() );
        Formatter(strCode) << pRoom->GetCheckCode() << (int)nRound << (int)nKeyRank << nMark;

        for ( unsigned int i = 0; i < vectRoundRank.size(); ++i )
        {
            std::string strRank;
            Formatter(strRank) << vectRoundRank[i];
            strCode.append( strRank );
        }

        unsigned char szMD5Code[16] = {0};
        MDString( strCode.c_str(), szMD5Code );
        BinaryData2String( szMD5Code, sizeof(szMD5Code) ).swap( strCode );

        if ( rMD5Code != strCode )
        {
            return;
        }

        pRoom->RecvRoundMark( nRoleID, (int)pRoom->GetPlayMusicMode(), nRound, nMark, nKeyRank, vectRoundRank );
    }
}

bool BigMamaRoomManager::IsValidRoomID(unsigned int nRoomID) const
{
    if ( INVALID_ROOM_ID < nRoomID && nRoomID <= m_nPreUsedRoomID )
        return true;

    return false;
}

DanceBaseRoom* BigMamaRoomManager::FindRoom(unsigned int nRoomID) const
{
    if ( IsValidRoomID( nRoomID ) )
        return m_vectRooms[nRoomID];

    return NULL;
}

void BigMamaRoomManager::RemoveRoom(unsigned int nRoomID)
{
    if ( !IsValidRoomID( nRoomID ) )
        return;

    DanceBaseRoom *pRoom = m_vectRooms[nRoomID];
    if ( pRoom == NULL )
        return;

    pRoom->Clear();
    m_listIdleRooms.push_back( pRoom );
    m_vectRooms[nRoomID] = NULL;
}

bool BigMamaRoomManager::LoadStageInfo(DanceBaseRoom *pRoom, unsigned char nPhoneOS, std::string &rStage) const
{
    if ( pRoom == NULL )
        return false;

    int nMusicMode = (int)pRoom->GetPlayMusicMode();
    std::string strStageFile;
    CMusicStageFileManager::GetStageFileName( pRoom->GetMusicSource(), 
        nMusicMode, (int)pRoom->GetPlayMusicLevel(), strStageFile );

    if ( strStageFile.empty() )
        return false;

    std::vector<char> androidStage, iosStage, winStage;
    if ( CMusicStageFileManager::LoadStage( strStageFile, androidStage, iosStage, winStage ) )
    {
        int nStageTime = CMusicStageFileManager::CalcuStageTime( nMusicMode, androidStage, iosStage, winStage );
        pRoom->SetCheckTime( nStageTime );

        std::vector<char> &rInfo = iosStage;
        if ( nPhoneOS == OS_Android )
        {
            rInfo = androidStage;
        }
        else if ( nPhoneOS == OS_WP8 )
        {
            rInfo = winStage;
        }

        rStage.assign( rInfo.begin(), rInfo.end() );

        return true;
    }

    return false;
}

