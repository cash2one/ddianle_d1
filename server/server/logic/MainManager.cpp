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

    //商城
    CMall::Instance().Initialize();

    // 聊天
    if ( !CChatManager::Instance().Initialize() )
    {
        WriteLog(LOGLEVEL_ERROR, "CChatManager.Initialize() return false!");
        return false;
    }

    // 排行榜
    CRankConfigData::Instance().Initialize();

    // 成就handler初始化
    CRequirementHandlerManager::Instance().Init();

    // 命令行handler
    CCmdHanderManager::Instance().Init();

    //邮件
    CMailMgr::Instance().Init();

    // 位置管理器
    CLocationMgr::Instance().Initialize();

    // 累计在线活动
    if(!COnlineRewardInfoMgr::Instance().Init())
    {
        WriteLog(LOGLEVEL_ERROR, "COnlineRewardInfoMgr.Init() return false!\n");
        return false;
    }

    //准点在线数据初始化
    CInTimeOnlineInfoMgr::Instance().Init();

    //累积充值奖励
    if (!CCumulativeRechargeMgr::Instance().Init())
    {
        WriteLog(LOGLEVEL_ERROR, "CCumulativeRechargeMgr.Init() return false!\n");
        return false;
    }

    //累积充值奖励
    if (!CCumulativeSpendMgr::Instance().Init())
    {
        WriteLog(LOGLEVEL_ERROR, "CCumulativeSpendMgr.Init() return false!\n");
        return false;
    }

    // 累计消费获取勋章活动
    CumulativeSpendGotMedalMgr::Instance().Initialize();

    // 购买物品获取勋章活动
    BuyItemGotMedalMgr::Instance().Initialize();

    //新玩家招募活动
    if (!CInvitationActivityMgr::Instance().Init())
    {
        WriteLog(LOGLEVEL_ERROR, "CInvitationActivityMgr.Init() return false!\n");
        return false;
    }

    //兑换物品活动
    CExchangeItemInfoMgr::Instance().Init();

    if (!CMallDataMgr::Instance().Init())
    {
        WriteLog(LOGLEVEL_ERROR, "CMallDataMgr.Init() return false!\n");
        return false;
    }

    //结婚房间
    CWeddingRoomManager::Instance().Init();

    //纯文字活动初始化
    CPlainTextActivityInfoMgr::Instance().Init();

    //兑换活动
    CGiftCodeInfoMgr::Instance().Init();

    //圣诞免费兑换活动
    CXmasFreeRewardDataMgr::Instance().Init();

    //许愿活动
    if (!CFestivalWishDataMgr::Instance().Init())
    {
        WriteLog(LOGLEVEL_ERROR, "CFestivalWishDataMgr.Init() return false!\n");
        return false;
    }

    //拼图活动
    if (!CPuzzleDataMgr::Instance().InitPuzzle())
    {
        WriteLog(LOGLEVEL_ERROR, "CPuzzleDataMgr.InitPuzzle() return false!\n");
        return false;
    }

    //IOS专享活动
    CProprietaryActivityInfoMgr::Instance().Init();

    // 分享活动
    CSocialShareMgr::Instance().Init();

    //宝箱
    if (!CBoxInfoManager::Instance().CheckAllBox())
    {
        WriteLog(LOGLEVEL_ERROR, "CBoxInfoManager.CheckAllBox() return false!\n");
        return false;
    }

    //邂逅
    CMeetActivityMgr::Instance().Init();
    WriteLog(LOGLEVEL_DEBUG, "Init CMeetActivityMgr Done...\n");

    // 签到
    CCheckInDataManager::Instance().Init();

    // 照相馆
    CPhotoRoomManager::Instance().Init();
    WriteLog(LOGLEVEL_DEBUG, "Init PhotoRoomManager Done...\n");

    // 星恋魔法阵
    ConstellationMatchDataManager::Instance().Init();
    WriteLog(LOGLEVEL_DEBUG, "Init Constellation Match Data Manager Done...\n");

    // 宠物
    if (!PetEggService::Instance().Init() )
        return false;
    if (!PetService::Instance().Init() )
        return false;

    // 长效累冲
    LongactingCumulativeRechargeMgr::Instance().Init();
    WriteLog(LOGLEVEL_DEBUG, "Init LongactingCumulativeRechargeMgr  Done...\n");


	// 信息中心初始化：
	NotificationManager::Instance().InitInstance();

    CeremonyRoomManager::Instance().Initialize();
    EntityBigMamaManager::Instance().Initialize();
    GameAttributeManager::Instance().Initialize();
    //勋章数据初始化
    CMedalCfgMgr::Instance().Initialize();

    //图鉴数据初始化
    CHandbookCfgMgr::Instance().Initialize();

    //坐骑配置初始化
    CHorseCfgMgr::Instance().Initialize();
    // 舞团试炼
    if ( !DanceGroupChallengeManager::Instance().Initialize() )
    {
        WriteLog( LOGLEVEL_ERROR, "DanceGroupChallengeManager.Initialize() return false." );

        return false;
    }

    // 
    QuestRecordFactory::Init();

    // 舞团红包
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

