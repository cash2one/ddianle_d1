#include "MainManager.h"

#include "../../datastructure/datainfor/ConfigManager.h"
#include "../../datastructure/LogicInitData.h"
#include "../../datastructure/Localization.h"
#include "../libServerFrame/Main.h"
#include "../../socket/ClientSocketMgr.h"
#include "../room/RoomManager.h"
#include "../room/NewRoomManager.h"
#include "../room/LiveRoomManager.h"
#include "LogicCirculator.h"
#include "../chat/ChatManager.h"
#include "../achievement/RequirementHandlerManager.h"
#include "../Command/CommandHandlerManager.h"
#include "../mail/MailMgr.h"
#include "../Location/LocationMgr.h"
#include "../Money/MoneyMgr.h"
#include "PlayerManager.h"
#include "../CheckIn/CheckInfoMgr.h"
#include "../mall/Mall.h"
#include "../mall/MallDataMgr.h"
#include "../Couple/WeddingRoomManager.h"
#include "../activity/SystemActivityManager.h"
#include "../activity/OnlineRewardInfoMgr.h"
#include "../activity/CumulativeRechargeMgr.h"
#include "../activity/CumulativeSpendMgr.h"
#include "../activity/ExchangeItemInfoMgr.h"
#include "../activity/InTimeOnlineInfoMgr.h"
#include "../activity/PlainTextActivityInfoMgr.h"
#include "../activity/GiftCodeInfoMgr.h"
#include "../activity/XmasFreeRewardDataMgr.h"
#include "../activity/FestivalWishDataMgr.h"
#include "../activity/PuzzleDataMgr.h"
#include "../activity/InvitationActivityMgr.h"
#include "../activity/MeetActivityMgr.h"
#include "../activity/ProprietaryActivity.h"
#include "../activity/SocialShareMgr.h"
#include "../activity/LongactingCumulativeRechareMgr.h"
#include "../activity/BuyItemGotMedalMgr.h"
#include "../activity/RedEnvelope/RedEnvelopeActivityManager.h"
#include "../box/BoxInfoManager.h"
#include "../PhotoRoom/PhotoRoomManager.h"
#include "../AmuseRoom/AmuseRoomManager.h"
#include "../quest/QuestDataManager.h"
#include "../quest/QuestRecord.h"
#include "../compensation/CompensationManager.h"
#include "../AmuseRoom/ConstellationMatchDataManager.h"
#include "../util/TimerMgr.h"
#include "../Dungeon/EntityDungeonComponent.h"
#include "../Couple/DivorceAgreement.h"
#include "../Rank/RankConfigData.h"
#include "../Pet/PetEggService.h"
#include "../Pet/PetService.h"
#include "../Notification/NotificationManager.h"
#include "../Medal/MedalCfgMgr.h"
#include "../Handbook/HandbookCfg.h"
#include "../Horse/HorseCfg.h"
#include "../CeremonyRoom/ceremonyRoomManager.h"
#include "../attribute/GameAttributeManager.h"
#include "../dancegroup/EntityBigMama.h"
#include "../dancegroup/DanceGroupChallengeManager.h"


extern CLogicCirculator* g_pCirculator;

MainManager::MainManager()
{
}

MainManager::~MainManager()
{
    ConfigManager::Instance().DestroyAllData();
}

bool MainManager::Init()
{
    WriteLog(LOGLEVEL_SYSTEM, "========= Loading data... ==========");

    if(!ConfigManager::Instance().InitAllData(g_sPath.c_str(),true))
    {
        WriteLog(LOGLEVEL_ERROR, "CDataManager.InitAllData() return false!\n");
        return false;
    }

    if (!CPlayerManager::Instance().Create())
    {
        WriteLog(LOGLEVEL_ERROR, "CPlayerManager.Create() return false!\n");
        return false;
    }

    if (!CLogicInitData::Instance().InitAllData())
    {
        WriteLog(LOGLEVEL_ERROR, "CLogicInitData.InitAllData() return false!\n");
        return false;
    }

    if (!CLocalization::Instance().Init())
    {
        WriteLog(LOGLEVEL_ERROR, "CLocalization.Init() return false!\n");
        return false;
    }

    //�̳�
    CMall::Instance().Initialize();

    // ����
    if ( !CChatManager::Instance().Initialize() )
    {
        WriteLog(LOGLEVEL_ERROR, "CChatManager.Initialize() return false!");
        return false;
    }

    // ���а�
    CRankConfigData::Instance().Initialize();

    // �ɾ�handler��ʼ��
    CRequirementHandlerManager::Instance().Init();

    // ������handler
    CCmdHanderManager::Instance().Init();

    //�ʼ�
    CMailMgr::Instance().Init();

    // λ�ù�����
    CLocationMgr::Instance().Initialize();

    // �ۼ����߻
    if(!COnlineRewardInfoMgr::Instance().Init())
    {
        WriteLog(LOGLEVEL_ERROR, "COnlineRewardInfoMgr.Init() return false!\n");
        return false;
    }

    //׼���������ݳ�ʼ��
    CInTimeOnlineInfoMgr::Instance().Init();

    //�ۻ���ֵ����
    if (!CCumulativeRechargeMgr::Instance().Init())
    {
        WriteLog(LOGLEVEL_ERROR, "CCumulativeRechargeMgr.Init() return false!\n");
        return false;
    }

    //�ۻ���ֵ����
    if (!CCumulativeSpendMgr::Instance().Init())
    {
        WriteLog(LOGLEVEL_ERROR, "CCumulativeSpendMgr.Init() return false!\n");
        return false;
    }

    // �ۼ����ѻ�ȡѫ�»
    CumulativeSpendGotMedalMgr::Instance().Initialize();

    // ������Ʒ��ȡѫ�»
    BuyItemGotMedalMgr::Instance().Initialize();

    //�������ļ�
    if (!CInvitationActivityMgr::Instance().Init())
    {
        WriteLog(LOGLEVEL_ERROR, "CInvitationActivityMgr.Init() return false!\n");
        return false;
    }

    //�һ���Ʒ�
    CExchangeItemInfoMgr::Instance().Init();

    if (!CMallDataMgr::Instance().Init())
    {
        WriteLog(LOGLEVEL_ERROR, "CMallDataMgr.Init() return false!\n");
        return false;
    }

    //��鷿��
    CWeddingRoomManager::Instance().Init();

    //�����ֻ��ʼ��
    CPlainTextActivityInfoMgr::Instance().Init();

    //�һ��
    CGiftCodeInfoMgr::Instance().Init();

    //ʥ����Ѷһ��
    CXmasFreeRewardDataMgr::Instance().Init();

    //��Ը�
    if (!CFestivalWishDataMgr::Instance().Init())
    {
        WriteLog(LOGLEVEL_ERROR, "CFestivalWishDataMgr.Init() return false!\n");
        return false;
    }

    //ƴͼ�
    if (!CPuzzleDataMgr::Instance().InitPuzzle())
    {
        WriteLog(LOGLEVEL_ERROR, "CPuzzleDataMgr.InitPuzzle() return false!\n");
        return false;
    }

    //IOSר��
    CProprietaryActivityInfoMgr::Instance().Init();

    // ����
    CSocialShareMgr::Instance().Init();

    //����
    if (!CBoxInfoManager::Instance().CheckAllBox())
    {
        WriteLog(LOGLEVEL_ERROR, "CBoxInfoManager.CheckAllBox() return false!\n");
        return false;
    }

    //����
    CMeetActivityMgr::Instance().Init();
    WriteLog(LOGLEVEL_DEBUG, "Init CMeetActivityMgr Done...\n");

    // ǩ��
    CCheckInDataManager::Instance().Init();

    // �����
    CPhotoRoomManager::Instance().Init();
    WriteLog(LOGLEVEL_DEBUG, "Init PhotoRoomManager Done...\n");

    // ����ħ����
    ConstellationMatchDataManager::Instance().Init();
    WriteLog(LOGLEVEL_DEBUG, "Init Constellation Match Data Manager Done...\n");

    // ����
    if (!PetEggService::Instance().Init() )
        return false;
    if (!PetService::Instance().Init() )
        return false;

    // ��Ч�۳�
    LongactingCumulativeRechargeMgr::Instance().Init();
    WriteLog(LOGLEVEL_DEBUG, "Init LongactingCumulativeRechargeMgr  Done...\n");


	// ��Ϣ���ĳ�ʼ����
	NotificationManager::Instance().InitInstance();

    CeremonyRoomManager::Instance().Initialize();
    EntityBigMamaManager::Instance().Initialize();
    GameAttributeManager::Instance().Initialize();
    //ѫ�����ݳ�ʼ��
    CMedalCfgMgr::Instance().Initialize();

    //ͼ�����ݳ�ʼ��
    CHandbookCfgMgr::Instance().Initialize();

    //�������ó�ʼ��
    CHorseCfgMgr::Instance().Initialize();
    // ��������
    if ( !DanceGroupChallengeManager::Instance().Initialize() )
    {
        WriteLog( LOGLEVEL_ERROR, "DanceGroupChallengeManager.Initialize() return false." );

        return false;
    }

    // 
    QuestRecordFactory::Init();

    // ���ź��
    RedEnvelopeActivityManager::Instance().Initialize();

    // 
    NewRoomManager::Instance().Init();

    WriteLog(LOGLEVEL_SYSTEM, "========= All data loaded! ==========");

    return true;
}

void MainManager::OnUpdate(unsigned long lTimeElapsed)
{
    CPlayerManager::Instance().OnUpdate(lTimeElapsed);
    CRoomManager::Instance().OnUpdate(lTimeElapsed);
    CLiveRoomManager::Instance().OnUpdate(lTimeElapsed);
    CDungeonMgr::Instance().OnUpdate(lTimeElapsed);
    CWeddingRoomManager::Instance().OnUpdate(lTimeElapsed);
    CSystemActivityManager::Instance().OnUpdate(lTimeElapsed);
    CPhotoRoomManager::Instance().OnUpdate(lTimeElapsed);
    CAmuseRoomManager::Instance().OnUpdate(lTimeElapsed);
    CTimerMgr::Instance().OnUpdate(lTimeElapsed);
    CeremonyRoomManager::Instance().OnUpdate(lTimeElapsed);
    GameAttributeManager::Instance().OnUpdate(lTimeElapsed);

    unsigned int nNow = (unsigned int)time( NULL );
    CumulativeSpendGotMedalMgr::Instance().OnUpdate( nNow );
    BuyItemGotMedalMgr::Instance().OnUpdate( nNow );
}

