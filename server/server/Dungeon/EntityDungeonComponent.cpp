#include "EntityDungeonComponent.h"
#include "../../datastructure/DataStruct_Base.h"
#include "../../socket/GameMsg_Map.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../item/EntityItemComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../room/RoomComponent.h"
#include "../friend/EntityFriendComponent.h"
#include "../vip/EntityVIPComponent.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../PhotoRoom/EntityPhotoRoomComponent.h"
#include "../AmuseRoom/AmuseRoomComponent.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../player/PlayerInfoComponent.h"
#include "../gene/EntityGeneComponent.h"
#include "../Pet/PetComponent.h"
#include "DungeonDataMgr.h"
#include "../item/ItemProcess.h"
#include "GameMsg_G2S_Dungeon.h"
#include "GameMsg_S2G_Dungeon.h"
#include "../logic/LogicCirculator.h"
#include "../../datastructure/datainfor/MagicLampConfig.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../mall/CurrencyType.h"
#include "../util/CommonFunc.h"
#include "GameMsg_Dungeon.h"
#include "../grade/GradeInfoMgr.h"
#include "../Pet/PetComponent.h"
#include "../Pet/PetConfig/PetConfigManager.h"
#include "../vip/VIPDataMgr.h"
#include "../quest/EntityQuestNewComponent.h"

#define DUNGEON_UPDATE_DELAY	1000  //毫秒
#define INVITE_FRIEND_TIMER    10	  //秒

CEntityDungeonComponent::CEntityDungeonComponent()
:CEntityComponent(true, true)
, m_pParcloseRoom(NULL)
, m_nPhoneOS(0)
, m_nMainlineRunningIndex(0)
, m_nCryStalUpdateTime(0)
, m_nInviteFriendTimer(0)
, m_nServerActiveParcloseID(0)
, m_eRoleState(RoleRoomState_None)
{
    m_pRoleRoom	        = NULL;
    m_pRoleAttr         = NULL;
    m_pRoleNet          = NULL;
    m_pRoleItem         = NULL;
    m_pRoleGene         = NULL;
    m_pRoleFriend       = NULL;
    m_pRoleVip          = NULL;
    m_pRoleCouple       = NULL;
    m_pRolePhoto        = NULL;
    m_pRoleAmuseRoom    = NULL;
    m_pRoleDanceGroup   = NULL;
    m_pRolePlayerInfo   = NULL;
    m_pRolePet          = NULL;
    m_pRoleQuestNew     = NULL;
}

void CEntityDungeonComponent::Start()
{
    m_pRoleAttr   = GetComponent<CRoleAttribute>();
    m_pRoleItem   = GetComponent<CRoleItem>();
    m_pRoleNet    = GetComponent<CRoleNet>();
    m_pRoleRoom   = GetComponent<CRoleRoom>();
    m_pRoleFriend = GetComponent<CRoleFriend>(); 
    m_pRoleVip    = GetComponent<CRoleVIP>();
    m_pRoleCouple = GetComponent<CRoleCouple>();
    m_pRolePhoto  = GetComponent<CRolePhotoRoom>();
    m_pRoleAmuseRoom = GetComponent<CRoleAmuseRoom>();
    m_pRoleDanceGroup = GetComponent<CRoleDanceGroup>();
    m_pRolePlayerInfo = GetComponent<CPlayerInfo>();
    m_pRoleGene       = GetComponent<CRoleGene>();
    m_pRolePet = GetComponent<CRolePet>();
    m_pRoleQuestNew = GetComponent<CRoleQuestNew>();
}

void CEntityDungeonComponent::OnEvent( CComponentEvent& refEvent )
{
}

void CEntityDungeonComponent::OnLogin()
{

}

void CEntityDungeonComponent::OnLogout()
{
    if ( m_pParcloseRoom != NULL )
    {
        // log
        CDungeonMgr::Instance()._LogRoleDancing(GetRoleID(), CRoleDancingLog::EDancingAction_End,
            m_pParcloseRoom->PlayScene(), m_pParcloseRoom->TeamMode(), m_pParcloseRoom->PlayMusicID(), m_pParcloseRoom->PlayMusicMode(), 
            m_pParcloseRoom->PlayMusicLevel(),m_pParcloseRoom->DancerCount(), m_MarkInfo.Place(), m_MarkInfo.Mark());

        m_pParcloseRoom->QuitRoom( m_pRoleAttr->GetRoleID(), true );

		// update magiclamp info
		SyncMagicToDB(ESyncToDBType_Update);
    }
}

bool CEntityDungeonComponent::CreateFromDB( ROLEINFO_DB* pRoleInforDB )
{
    // 属性记录
    m_MagicLamp = pRoleInforDB->m_MagicLamp;

    // 处理水晶回复
    CrystalReply((unsigned int)time(NULL), false);

    // 主线关卡记录
    std::list<MainlineParcloseRecord>::iterator it = pRoleInforDB->m_MainlineParcloseList.begin();
    for ( ; it != pRoleInforDB->m_MainlineParcloseList.end(); ++it )
    {
        m_MainlineParcloseRecord[ it->nParcloseID ] = *it;
    }

    // 主题关卡记录
    std::list<ThemeParcloseRecord>::iterator itTheme = pRoleInforDB->m_ThemeParcloseList.begin();
    for ( ; itTheme != pRoleInforDB->m_ThemeParcloseList.end(); ++itTheme )
    {
        m_ThemeParcloseRecord[itTheme->nThemeID].insert( make_pair( itTheme->nParcloseID, *itTheme));
    }

    // 设置正在进行的关卡
    MapMainlineParcloseRecord::reverse_iterator iter = m_MainlineParcloseRecord.rbegin();
    if ( iter != m_MainlineParcloseRecord.rend() )
    {
        m_nMainlineRunningIndex = iter->first;
    }

    MapThemeParcloseRecord::iterator itrun = m_ThemeParcloseRecord.begin();
    for ( ; itrun != m_ThemeParcloseRecord.end(); ++itrun )
    {
        std::map<unsigned int,ThemeParcloseRecord>::reverse_iterator itend = itrun->second.rbegin();
        if ( itend != itrun->second.rend())
        {
            m_ThemeRunningIndex[itrun->first] = itend->second.nParcloseID;
        }
    }

    // 幸运星奖励
    m_LuckStarReward = pRoleInforDB->m_LuckStarReward;

    // 碎片信息
    m_DebrisRecord = pRoleInforDB->m_PlayerDebrisInfo;

    // 好友交互信息
    m_mapFriendContact = pRoleInforDB->m_mapFriendContactInfo;

    return true;
}

bool CEntityDungeonComponent::PacketToDB( ROLEINFO_DB *pRoleInforDB ) const 
{
    return false;
}

bool CEntityDungeonComponent::PacketToCache( ROLEINFO_DB* pRoleInforDB ) const
{
    if ( pRoleInforDB == NULL )
        return false;

    pRoleInforDB->m_MagicLamp = m_MagicLamp;

    pRoleInforDB->m_MainlineParcloseList.clear();
    for ( MapMainlineParcloseRecord::const_iterator mainlineItr = m_MainlineParcloseRecord.begin();
        mainlineItr != m_MainlineParcloseRecord.end(); ++mainlineItr )
    {
        pRoleInforDB->m_MainlineParcloseList.push_back( mainlineItr->second );
    }

    pRoleInforDB->m_ThemeParcloseList.clear();
    for ( MapThemeParcloseRecord::const_iterator themeItr = m_ThemeParcloseRecord.begin();
        themeItr != m_ThemeParcloseRecord.end(); ++themeItr )
    {
        const std::map<unsigned int, ThemeParcloseRecord> &rThemeRecord = themeItr->second;
        if ( rThemeRecord.empty() )
            continue;

        for ( std::map<unsigned int, ThemeParcloseRecord>::const_iterator itr = rThemeRecord.begin();
            itr != rThemeRecord.end(); ++itr )
        {
            pRoleInforDB->m_ThemeParcloseList.push_back( itr->second );
        }
    }

    pRoleInforDB->m_LuckStarReward = m_LuckStarReward;
    pRoleInforDB->m_PlayerDebrisInfo = m_DebrisRecord;

    return true;
}

void CEntityDungeonComponent::OnUpdate( const unsigned long &nTimeElapsed )
{
    m_nCryStalUpdateTime += (int)nTimeElapsed;

    if (m_nCryStalUpdateTime >= DUNGEON_UPDATE_DELAY && IsMagicActived())
    {
        m_nCryStalUpdateTime -= DUNGEON_UPDATE_DELAY;

        int nCurTime = (int)time(NULL);

        // 邀请好友定时器
        if (m_nInviteFriendTimer > 0 && nCurTime >= m_nInviteFriendTimer)
        {
            _SendStartParcloseErrorMsg(EDungeonError_InviteFriendTimeOut);
            m_nInviteFriendTimer = 0;
        }

        // 水晶回复
        CrystalReply(nCurTime);

        // 赠送、收取、扫荡重置时间
        CheckNextDayResetTime(nCurTime);
    }
}

void CEntityDungeonComponent::SendExtraStaticData()
{
}

void CEntityDungeonComponent::SerializeComponent( CParamPool &IOBuff )
{	
}

void CEntityDungeonComponent::RegComponentNetMsgMap()
{
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetLanternBaseInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetStarReward);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ExchangeDustReward);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetDungeonInfoDetail);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetParcloseInfoDetail);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetDungeonStory);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_InviteFriendResponse);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetDungeonList);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetBuyCrystalInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_BuyCrystal);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_StartParclose);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_SkipParcose);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ExchangeDebrisEquip);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetMainlineInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetThemeInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetFriendContactInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GiveVitToFriend);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetVitFromFriend);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_QuickFinishParclose);

    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_GetRankListResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_SyncDungeonInviteFriendInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_GiveVitToFriend);


    RegMsgProcessor(MSG_C2S_GetLanternBaseInfo, &CEntityDungeonComponent::OnGetLanternBaseInfo);
    RegMsgProcessor(MSG_C2S_GetStarReward, &CEntityDungeonComponent::OnGetStarReward);
    RegMsgProcessor(MSG_C2S_GetDungeonStory, &CEntityDungeonComponent::OnGetDungeonStory);
    RegMsgProcessor(MSG_C2S_ExchangeDustReward, &CEntityDungeonComponent::OnExchangeDustReward);
    RegMsgProcessor(MSG_C2S_GetDungeonInfoDetail, &CEntityDungeonComponent::OnGetDungeonInfoDetail);
    RegMsgProcessor(MSG_C2S_GetParcloseInfoDetail, &CEntityDungeonComponent::OnGetParcloseInfoDetail);
    RegMsgProcessor(MSG_C2S_InviteFriendResponse, &CEntityDungeonComponent::OnInviteResponse);
    RegMsgProcessor(MSG_C2S_GetDungeonRankList, &CEntityDungeonComponent::OnGetRankList);
    RegMsgProcessor(MSG_C2S_GetBuyCrystalInfo, &CEntityDungeonComponent::OnGetBuyCrystalInfo);
    RegMsgProcessor(MSG_C2S_BuyCrystal, &CEntityDungeonComponent::OnBuyCrystal);
    RegMsgProcessor(MSG_C2S_StartParclose, &CEntityDungeonComponent::OnStartParclose);
    RegMsgProcessor(MSG_C2S_SkipParclose, &CEntityDungeonComponent::OnSkipParclose);
    RegMsgProcessor(MSG_C2S_ExchangeDebrisEquip, &CEntityDungeonComponent::OnExchangeDebrisEquip);
    RegMsgProcessor(MSG_C2S_GetMainlineInfo, &CEntityDungeonComponent::OnGetMainlineDungeonInfo);
    RegMsgProcessor(MSG_C2S_GetThemeInfo, &CEntityDungeonComponent::OnGetThemeDungeonInfo);
    RegMsgProcessor(MSG_C2S_GetFriendContactInfo, &CEntityDungeonComponent::OnGetFriendContactInfo);
    RegMsgProcessor(MSG_C2S_GiveVitToFriend, &CEntityDungeonComponent::OnGiveVit);
    RegMsgProcessor(MSG_C2S_GetVitFromFrined, &CEntityDungeonComponent::OnGetVit);
    RegMsgProcessor(MSG_C2S_QuickFinishParclose, &CEntityDungeonComponent::OnQuickFinishParclose);

    RegMsgProcessor(MSG_G2S_GetRankListResult, &CEntityDungeonComponent::_OnGetRankList);
    RegMsgProcessor(MSG_G2S_SyncInviteFriendInfo, &CEntityDungeonComponent::OnSyncInviteFriendInfo);
    RegMsgProcessor(MSG_G2S_GiveVitToFriend, &CEntityDungeonComponent::_GiveVitToFriend);

}

void CEntityDungeonComponent::ActiveFirstParclose()
{
    // 先激活第一个关卡
    if ( m_MainlineParcloseRecord.empty() )
    {
        int nCurTime = (int)time(NULL);

        unsigned int nMaxCrystal = CDungeonDataMgr::instance().GetMaxCrystalValue();
        unsigned int nInitialValue = CDungeonDataMgr::instance().GetCrystalInitialValue();
        if ( nInitialValue <= nMaxCrystal && nInitialValue > 0 )
        {
            SetCrystalInitialValue( CDungeonDataMgr::instance().GetCrystalInitialValue(), nCurTime );

            unsigned int nFirstParcloseID = ConfigManager::Instance().GetMagicLampConfigMgr().GetFirstParcloseID();
            ActiveMainlineParclose( nFirstParcloseID );

            m_nServerActiveParcloseID = nFirstParcloseID;
        }
    }

    // 检查有没新的副本关卡配置，主动去激活
    MapMainlineParcloseRecord::reverse_iterator it = m_MainlineParcloseRecord.rbegin();
    if ( it != m_MainlineParcloseRecord.rend() )
    {
        if ( it->second.nStarCount > 0 )
        {
            unsigned int nNextParcloseID =  ConfigManager::Instance().GetMagicLampConfigMgr().GetMainlineNextParcloseID( it->first );
            if ( nNextParcloseID != it->first )
            {
                MapMainlineParcloseRecord::iterator iter = m_MainlineParcloseRecord.find( nNextParcloseID );
                if ( iter == m_MainlineParcloseRecord.end() )
                {
                    ActiveMainlineParclose( nNextParcloseID );

                    m_nServerActiveParcloseID = nNextParcloseID;
                }
                else
                {
                    if ( iter->second.bIsActived == false )
                    {
                        iter->second.bIsActived = true;
                    }
                }
            }
        }
    }

    // 主动激活主题副本
    const std::map<unsigned int,CThemeConfig*> & themeconfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetThemeConfigList();
    std::map<unsigned int,CThemeConfig*>::const_iterator itTheme = themeconfig.begin();
    for ( ; itTheme != themeconfig.end(); ++itTheme )
    {
        MapThemeParcloseRecord::iterator iter = m_ThemeParcloseRecord.find( itTheme->first );
        if ( iter == m_ThemeParcloseRecord.end() )
        {
            unsigned int nFirstParcloseID = ConfigManager::Instance().GetMagicLampConfigMgr().GetThemeFirstParcloseID( itTheme->first );
            ActiveThemeParclose( itTheme->first, nFirstParcloseID, false );

            m_ServerActiveThemeParcloseID.insert( itTheme->first );
        }  
    }
}

void CEntityDungeonComponent::OnGetLanternBaseInfo( GameMsg_Base &msg, CSlotPeer &slotPeer )
{
    GameMsg_C2S_GetLanternBaseInfo & c2sMsg = ( GameMsg_C2S_GetLanternBaseInfo &)msg;
    m_nPhoneOS = c2sMsg.m_nOsType;

    ActiveFirstParclose();

    GameMsg_S2C_GetLanternBaseInfoResult resultMsg;
    resultMsg.m_pRoleDungeon = this;
    m_pRoleNet->SendPlayerMsg( &resultMsg );
}

void CEntityDungeonComponent::_GetLanternBaseInfo( CParamPool &IOBuff )
{
    //玩家已获得的许愿星数
    IOBuff.AddUShort( (unsigned short)GetLuckStar() ); 

    // 许愿星奖励
    const StarRewardInfoTable & starconfig = CDungeonDataMgr::instance().getStarRewardConfig();
    IOBuff.AddUShort((unsigned short)starconfig.size());
    StarRewardInfoTable::const_iterator it = starconfig.begin();
    for ( ; it != starconfig.end(); ++it )
    {
        IOBuff.AddChar( (char)it->second.m_nID );
        std::set<int>::iterator its = m_LuckStarReward.find( it->second.m_nID );
        if ( its != m_LuckStarReward.end() )
        {
            IOBuff.AddBool( true );
        }
        else
        {
            IOBuff.AddBool( false );
        }
        IOBuff.AddUShort( (unsigned short)it->second.m_nCostStar );
        if ( m_pRoleAttr->GetSex() == ESexType_Male )
        {
            IOBuff.AddUShort((unsigned short)it->second.m_listMaleReward.size());
            std::list<CItem>::const_iterator itt = it->second.m_listMaleReward.begin();
            for ( ; itt != it->second.m_listMaleReward.end(); ++itt )
            {
                itt->doEncode( IOBuff );
            }
        }
        else
        {
            IOBuff.AddUShort((unsigned short)it->second.m_listFemaleReward.size());
            std::list<CItem>::const_iterator itt = it->second.m_listFemaleReward.begin();
            for ( ; itt != it->second.m_listFemaleReward.end(); ++itt )
            {
                itt->doEncode( IOBuff );
            }
        }
        IOBuff.AddInt( it->second.m_nMoney );	
    }

    // 魔法尘埃
    const CDustRewardConfig & dust = CDungeonDataMgr::instance().getDustRewardConfig();
    IOBuff.AddStringW( dust.m_strActivityName );
    IOBuff.AddStringW( dust.m_strIconName );
    IOBuff.AddUShort( (unsigned short)dust.m_nCostDust );
    IOBuff.AddUShort( (unsigned short)GetMagicDust() );
    IOBuff.AddBool( dust.m_nStoryID > 0 ? true : false ); //剧情TODO

    // 法力水晶
    const CCrystalConfig & crystal = CDungeonDataMgr::instance().getCrystalConfig();
    IOBuff.AddUShort( (unsigned short)GetCrystal() );
    IOBuff.AddUShort( (unsigned short)crystal.m_nMaxValue );
    int nCurTime = (int)time(NULL);
    int nCurCrystalRestoreTime = GetCrystalRestoreTime();
    unsigned int nRestoreTimeConfig = CDungeonDataMgr::instance().GetCrystalRestoreTime();
    if ( nCurCrystalRestoreTime > 0  && nCurTime >= nCurCrystalRestoreTime )
    {
        IOBuff.AddFloat( nRestoreTimeConfig - (nCurTime - nCurCrystalRestoreTime) );
    }
    else
    {
        IOBuff.AddFloat( (float)crystal.m_nRestoreTime );
    }

    // 可选物品列表
    const std::list<CItem> & items = CDungeonDataMgr::instance().GetParcloseItemList();
    IOBuff.AddUShort((unsigned short)items.size());
    std::list<CItem>::const_iterator iti = items.begin();
    for ( ; iti != items.end(); ++iti )
    {
        IOBuff.AddUInt( iti->m_nItemType );
    }

    // 功能描述
    const CMagicLampDesc & desc = CDungeonDataMgr::instance().GetDesc();
    IOBuff.AddStringW( desc.m_strTitle.c_str());
    IOBuff.AddStringW( desc.m_strContent.c_str());

    // 主线副本是否有新增
    IOBuff.AddBool( IsNewMainlineParclose());

    // 主题副本是否有新增
    IOBuff.AddBool( IsNewThemeParclose());

    // 邀请好友信息
    CheckAndResetInviteInfo();
    unsigned short nMaxInviteFriendNum = (unsigned short)CDungeonDataMgr::instance().GetMaxInviteFriendNum();
    IOBuff.AddUShort( nMaxInviteFriendNum- m_InviteInfo.m_nCostInviteNum );
    IOBuff.AddUShort( (unsigned short)m_InviteInfo.m_setInviteFriends.size() );
    std::set<unsigned int>::iterator itInvite = m_InviteInfo.m_setInviteFriends.begin();
    for ( ; itInvite != m_InviteInfo.m_setInviteFriends.end(); ++itInvite)
    {
        IOBuff.AddUInt( *itInvite );
    }

    // 碎片信息
    std::map<unsigned int,CDebrisConfig*> & debrisconfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetDebrisConfigList();
    IOBuff.AddUShort( (unsigned short)debrisconfig.size());
    std::map<unsigned int,CDebrisConfig*>::iterator itDebris = debrisconfig.begin();
    for ( ; itDebris != debrisconfig.end(); ++itDebris )
    {
        IOBuff.AddUInt( itDebris->second->m_nDebrisID );
        IOBuff.AddStringW( itDebris->second->m_strName );
        IOBuff.AddStringW( itDebris->second->m_strIconName );
        IOBuff.AddStringW( itDebris->second->m_strAtlasName );
        IOBuff.AddUInt( itDebris->second->m_nNeedNum );
        IOBuff.AddUInt( GetDebrisCount(itDebris->second->m_nDebrisID));
        IOBuff.AddUChar((unsigned char)itDebris->second->m_nCurrencyType);
        IOBuff.AddUInt( itDebris->second->m_nCurrencyCount);

        if ( m_pRoleAttr->GetSex() == ESexType_Male )
        {
            IOBuff.AddUShort((unsigned short)itDebris->second->m_MaleExchangeItem.size());
            std::vector<CItem>::const_iterator itItem = itDebris->second->m_MaleExchangeItem.begin();
            for ( ; itItem != itDebris->second->m_MaleExchangeItem.end(); ++itItem )
            {
                itItem->doEncode( IOBuff );
            }
        }
        else
        {
            IOBuff.AddUShort((unsigned short)itDebris->second->m_FemaleExchangeItem.size());
            std::vector<CItem>::const_iterator itItem = itDebris->second->m_FemaleExchangeItem.begin();
            for ( ; itItem != itDebris->second->m_FemaleExchangeItem.end(); ++itItem )
            {
                itItem->doEncode( IOBuff );
            }
        }
    }
    
    // 封面上的宣传图
    const std::vector<CPublicityPictureConfig> &  picture = CDungeonDataMgr::instance().GetPublicityPictureConfig();
    IOBuff.AddUShort((unsigned short)picture.size());
    std::vector<CPublicityPictureConfig>::const_iterator itPhoto = picture.begin();
    for ( ; itPhoto != picture.end(); ++itPhoto )
    {
        IOBuff.AddStringW( itPhoto->m_strIcon );
        IOBuff.AddStringW( itPhoto->m_strAtlas );
    }

    // 赠送获取体力
    IOBuff.AddUInt(CDungeonDataMgr::instance().OnceGiveVitNum());
    IOBuff.AddUInt(CDungeonDataMgr::instance().GetDailyGetVitMaxNum());
    IOBuff.AddUInt(m_MagicLamp.m_nTodayHaveGetVitNum);
    IOBuff.AddUInt(CDungeonDataMgr::instance().GetNextDayResetTime());

    // 扫荡
    IOBuff.AddBool(CDungeonDataMgr::instance().QucikFinishOpenToNotVip());
    CItem costItem = CDungeonDataMgr::instance().GetQuickFinishItem();
    IOBuff.AddUInt(costItem.m_nItemType);
    IOBuff.AddUShort(costItem.m_nItemCount);
}

void CEntityDungeonComponent::OnGetStarReward( GameMsg_Base &msg, CSlotPeer &slotPeer )
{
    GameMsg_C2S_GetStarReward &c2sMsg = ( GameMsg_C2S_GetStarReward &)msg;

    int nError = _GetStarReward( c2sMsg.m_nRewardID );
    if ( nError != EDungeonError_Sucess )
    {
        _SendStarRewardErrorMsg( nError );
    }
}

void CEntityDungeonComponent::OnGetDungeonStory( GameMsg_Base &msg, CSlotPeer &slotPeer )
{
    GameMsg_C2S_GetDungeonStory & c2smsg = (GameMsg_C2S_GetDungeonStory&)msg;

    unsigned int nStoryID = 0;
    switch( c2smsg.m_nStoryType )
    {
    case  eStoryType_Dust:
        {
            nStoryID = CDungeonDataMgr::instance().GetDustStoryID();
        }
        break;
    case  eStoryType_Dungeon:
        {
            if ( c2smsg.m_bIsMainline )
            {
                CDungeonConfig * pDungeonConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetDungeonConfig( c2smsg.m_nDungeonID );
                if ( pDungeonConfig != NULL )
                {
                    nStoryID = pDungeonConfig->m_nStoryID;
                }
            }
            else
            {
                CThemeConfig * pThemeConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetThemeConfig( c2smsg.m_nDungeonID );
                if ( pThemeConfig != NULL )
                {
                    nStoryID = pThemeConfig->m_nStoryID;
                }
            }

        }
        break;
    case  eStoryType_ParcloseBegin:
        {
            EDungeonType eType = c2smsg.m_bIsMainline ? EDungeonType_Mainline : EDungeonType_Theme;
            CParcloseConfig * pParcloseConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetParcloseConfig( eType, c2smsg.m_nDungeonID, c2smsg.m_nParcloseID );
            if ( pParcloseConfig != NULL )
            {
                nStoryID = pParcloseConfig->m_nBeginStoryID;
            }
        }
        break;
    case  eStory_ParcloseEnd:
        {
            EDungeonType eType = c2smsg.m_bIsMainline ? EDungeonType_Mainline : EDungeonType_Theme;
            CParcloseConfig * pParcloseConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetParcloseConfig( eType, c2smsg.m_nDungeonID, c2smsg.m_nParcloseID );
            if ( pParcloseConfig != NULL )
            {
                nStoryID = pParcloseConfig->m_nEndStoryID;
            }
        }
        break;
    default:
        break;
    }

    CStoryConfig * pStoryConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetStoryConfig( nStoryID );
    if ( pStoryConfig != NULL )
    {
        GameMsg_S2C_GetDungeonStoryResult resultstory;
        resultstory.m_nDungeonID  = c2smsg.m_nDungeonID;
        resultstory.m_nParcloseID = c2smsg.m_nParcloseID;
        resultstory.m_nStoryType  = c2smsg.m_nStoryType;
        resultstory.m_listNpc     = pStoryConfig->m_NPCList;
        resultstory.m_listStory   = pStoryConfig->m_ContentList;

        m_pRoleNet->SendPlayerMsg( &resultstory );
    }
    else
    {
        GameMsg_S2C_GetDungeonStoryFail failMsg;
        failMsg.m_nStoryType = c2smsg.m_nStoryType;
        failMsg.m_nDungeonID = c2smsg.m_nDungeonID;
        failMsg.m_nParcloseID = c2smsg.m_nParcloseID;
        failMsg.m_strError = "EDungeonError_NotFindStoryConfig";

        m_pRoleNet->SendPlayerMsg( &failMsg );
    }
}

int	CEntityDungeonComponent::_GetStarReward( unsigned int nRewardID )
{
    CStarRewardConfig * pReward = CDungeonDataMgr::instance().GetStarRewardConfig( nRewardID );
    if ( pReward == NULL )
    {
        return EDungeonError_StarConfigErr;
    }

    if ( pReward->m_nCostStar > GetLuckStar() )
    {
        return EDungeonError_StarNotEnough;
    }

    // 领取过
    std::set<int>::iterator it = m_LuckStarReward.find( pReward->m_nID );
    if ( it != m_LuckStarReward.end() )
    {
        return EDungeonError_RewardHaved;
    }

    // 添加物品
    const CMagicLampRewardMail &  mail = CDungeonDataMgr::instance().GetRewardMail();
    if ( m_pRoleAttr->GetSex() == ESexType_Male )
    {
        CItemProcess::AddOrMailItems(*(CRoleEntity *)Entity(), pReward->m_listMaleReward, EItemAction_Add_DungeonStar, 0, true, EMailType_MagicLamp_StarReward, mail.m_strStarRewardTitle, mail.m_strStarRewardContent );
    }
    else
    {
        CItemProcess::AddOrMailItems(*(CRoleEntity *)Entity(), pReward->m_listFemaleReward, EItemAction_Add_DungeonStar, 0, true, EMailType_MagicLamp_StarReward, mail.m_strStarRewardTitle, mail.m_strStarRewardContent );
    }

    m_pRoleAttr->ChangeMoney( pReward->m_nMoney, EChangeMoneyCause_Add_DungeonStar, 0);

    // 保存
    m_LuckStarReward.insert( pReward->m_nID );
    AddQuery(Query_Insert_StarRewardIndex, m_pRoleAttr->GetRoleID(), NULL, pReward->m_nID );

    // 返回成功值
    GameMsg_S2C_GetStarRewardSuc msgSuc;
    msgSuc.m_nRewardID  = nRewardID;
    msgSuc.m_listReward = m_pRoleAttr->GetSex() == ESexType_Male ? pReward->m_listMaleReward : pReward->m_listFemaleReward;
    msgSuc.m_nMoney     = pReward->m_nMoney;
    m_pRoleNet->SendPlayerMsg( &msgSuc );

    return EDungeonError_Sucess;
}

void CEntityDungeonComponent::_SendStarRewardErrorMsg( int nErr )
{
    GameMsg_S2C_GetStarRewardFail msgFail;

    switch( nErr)
    {
    case  EDungeonError_StarConfigErr : msgFail.m_strError = "EDungeonError_StarConfigErr";break;
    case  EDungeonError_RewardHaved :   msgFail.m_strError = "EDungeonError_RewardHaved";break;
    case  EDungeonError_StarNotEnough : msgFail.m_strError = "EDungeonError_StarNotEnough";break;
    }

    m_pRoleNet->SendPlayerMsg( &msgFail );
}

void CEntityDungeonComponent::_SendDustRewardErrorMsg( int nErr )
{
    GameMsg_S2C_ExchangeDustRewardFail msgFail;

    switch( nErr)
    {
    case  EDungeonError_DustConfigErr : msgFail.m_strError = "EDungeonError_DustConfigErr";break;
    case  EDungeonError_DustNotEnough : msgFail.m_strError = "EDungeonError_DustNotEnough";break;
    }

    m_pRoleNet->SendPlayerMsg( &msgFail );
}

void CEntityDungeonComponent::OnExchangeDustReward( GameMsg_Base &msg, CSlotPeer &slotPeer )
{
    int nError = _ExchangeDustReward();
    if ( nError != EDungeonError_Sucess )
    {
        _SendDustRewardErrorMsg( nError );
    }
}

unsigned int CEntityDungeonComponent::GetParcloseStarCount(EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID)
{
    if (eType == EDungeonType_Mainline)
    {
        MapMainlineParcloseRecord::iterator it = m_MainlineParcloseRecord.find(nParcloseID);
        if ( it != m_MainlineParcloseRecord.end() )
        {
            return it->second.nStarCount;
        }
    }
    else
    {
        MapThemeParcloseRecord::iterator it = m_ThemeParcloseRecord.find(nDungeonID);
        if (it != m_ThemeParcloseRecord.end())
        {
            std::map<unsigned int,ThemeParcloseRecord>::iterator itr = it->second.find(nParcloseID);
            if (itr != it->second.end())
            {
                return itr->second.nStarCount;
            }
        }
    }

    return 0;
}

int  CEntityDungeonComponent::_ExchangeDustReward()
{
    CDustRewardConfig * pDustConfig = CDungeonDataMgr::instance().GetDustRewardConfig();
    if ( pDustConfig == NULL )
    {
        return EDungeonError_DustConfigErr;
    }

    if ( GetMagicDust() < (int)pDustConfig->m_nCostDust )
    {
        return EDungeonError_DustNotEnough;
    }

    const CMagicLampRewardMail &  mail = CDungeonDataMgr::instance().GetRewardMail();
    if ( m_pRoleAttr->GetSex() == ESexType_Male )
    {
        CItemProcess::AddOrMailItems(*(CRoleEntity *)Entity(), pDustConfig->m_listMaleReward, EItemAction_Add_DungeonDust, 0, true, EMailType_MagicLamp_DustReward, mail.m_strDustRewardTitle, mail.m_strDustRewardContent );
    }
    else
    {
        CItemProcess::AddOrMailItems(*(CRoleEntity *)Entity(), pDustConfig->m_listFemaleReward, EItemAction_Add_DungeonDust, 0, true, EMailType_MagicLamp_DustReward, mail.m_strDustRewardTitle, mail.m_strDustRewardContent );
    }

    m_pRoleAttr->ChangeMoney( pDustConfig->m_nMoney, EChangeMoneyCause_Add_DungeonDust, 0 );

    // 保存数据
    SetMagicDust( (-1)*pDustConfig->m_nCostDust );
    WriteLog(LOGLEVEL_DEBUG, "Debug:MagicLampDust: roleid:%u_Del_Dust:%d", m_pRoleAttr->GetRoleID(), m_MagicLamp.m_nMagicDust );

    // 返回成功值
    GameMsg_S2C_ExchangeDustRewardSuc msgSuc;
    msgSuc.m_nDustCount  = GetMagicDust();
    msgSuc.m_listReward  = m_pRoleAttr->GetSex() == ESexType_Male ? pDustConfig->m_listMaleReward : pDustConfig->m_listFemaleReward;
    msgSuc.m_nMoney      = pDustConfig->m_nMoney;
    m_pRoleNet->SendPlayerMsg( &msgSuc );

    return EDungeonError_Sucess;
}

void CEntityDungeonComponent::OnGetDungeonInfoDetail( GameMsg_Base &msg, CSlotPeer &slotPeer )
{
    GameMsg_C2S_GetDungeonInfoDetail & c2sMsg = (GameMsg_C2S_GetDungeonInfoDetail&)msg;

    bool bIsFind = false;
    if ( c2sMsg.m_bIsMainline )
    {
        // 更新new显示
        if ( IsNewMainlineParclose())
        {
            m_MagicLamp.m_nMaxMainline = ConfigManager::Instance().GetMagicLampConfigMgr().GetMaxMainlineID();
            SyncMagicToDB(ESyncToDBType_Update);
        }

        CDungeonConfig * pDungeonConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetDungeonConfig( c2sMsg.m_nDungeonID );
        if ( pDungeonConfig != NULL )
        {
            GameMsg_S2C_GetDungeonInfoDetailResult resultMsg;
            resultMsg.m_pDungeonConfig = pDungeonConfig;
            resultMsg.m_nCurrentParcloseID = m_nMainlineRunningIndex;
            resultMsg.m_DungeonRecord = m_MainlineParcloseRecord;
            resultMsg.m_nServerActiveParcloseID = m_nServerActiveParcloseID;
            m_pRoleNet->SendPlayerMsg( & resultMsg );

            bIsFind = true;
        }

        m_nServerActiveParcloseID = 0;
    }
    else
    {
        // 更新new显示
        if ( IsNewThemeParclose())
        {
            m_MagicLamp.m_nMaxTheme = ConfigManager::Instance().GetMagicLampConfigMgr().GetMaxMainlineID();
            SyncMagicToDB(ESyncToDBType_Update);
        }

        CThemeConfig * pThemeConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetThemeConfig( c2sMsg.m_nDungeonID );
        if ( pThemeConfig != NULL )
        {
            GameMsg_S2C_GetDungeonInfoDetailResult resultMsg;
            resultMsg.m_pThemeConfig = pThemeConfig;
            resultMsg.m_ThemeRunningIndex = m_ThemeRunningIndex;
            MapThemeParcloseRecord::iterator it = m_ThemeParcloseRecord.find( pThemeConfig->m_nThemeID );
            if ( it != m_ThemeParcloseRecord.end())
            {
                resultMsg.m_ThemeRecord = it->second;

                unsigned int nFirstParcloseID = ConfigManager::Instance().GetMagicLampConfigMgr().GetThemeFirstParcloseID( pThemeConfig->m_nThemeID );
                std::map<unsigned int,ThemeParcloseRecord>::iterator itParclose = it->second.find( nFirstParcloseID );
                if ( itParclose != it->second.end())
                {
                    ThemeParcloseRecord * pRecord = new ThemeParcloseRecord( itParclose->second );
                    if ( pRecord != NULL )
                    {
                        AddQuery( Query_Insert_ThemeParcloseInfo, m_pRoleAttr->GetRoleID(), pRecord );
                    }
                }
            }
            m_pRoleNet->SendPlayerMsg( & resultMsg );

            bIsFind = true;
        }

        std::set<unsigned int>::iterator itFind = m_ServerActiveThemeParcloseID.find( c2sMsg.m_nDungeonID );
        if ( itFind != m_ServerActiveThemeParcloseID.end() )
        {
            m_ServerActiveThemeParcloseID.erase( itFind );
        }
    }

    if ( !bIsFind )
    {
        GameMsg_S2C_GetDungeonInfoDetailFail failmsg;
        failmsg.m_strError = "DungeonError_NotFindDungeonID";
        m_pRoleNet->SendPlayerMsg( &failmsg );
    }
}

void CEntityDungeonComponent::OnGetParcloseInfoDetail( GameMsg_Base &msg, CSlotPeer &slotPeer )
{
    GameMsg_C2S_GetParcloseInfoDetail & c2sMsg = (GameMsg_C2S_GetParcloseInfoDetail&)msg;

    bool bIsFind = false;
    if ( c2sMsg.m_bIsMainline )
    {
        CParcloseConfig * pParcloseConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetMainlineParcloseConfig( c2sMsg.m_nParcloseID );
        if ( pParcloseConfig != NULL )
        {
            GameMsg_S2C_GetParcloseInfoDetailResult resultMsg;
            resultMsg.m_pParcloseConfig = pParcloseConfig;
            resultMsg.m_nSex = m_pRoleAttr->GetSex();

            const std::list<CItem> & costitem = CDungeonDataMgr::instance().GetSkipParcloseItem();
            std::list<CItem>::const_iterator it = costitem.begin();
            if ( it != costitem.end() )
            {
                resultMsg.m_nSkipParcloseItem = it->m_nItemType;
                resultMsg.m_nSkipParcloseItemCount = it->m_nItemCount;
            }

            CDungeonDataMgr::instance().GetMainParcloseDrop( c2sMsg.m_nParcloseID, resultMsg.m_MaleItems, resultMsg.m_FemaleItems, resultMsg.m_DropDebrisInfo );
            resultMsg.m_nLuckStar = GetParcloseStarCount(c2sMsg.m_bIsMainline ? EDungeonType_Mainline : EDungeonType_Theme, c2sMsg.m_nDungeonID, c2sMsg.m_nParcloseID);
            m_pRoleNet->SendPlayerMsg( & resultMsg );

            bIsFind = true;
        }
    }
    else
    {
        CThemeParcloseConfig *  pThemeParcloseConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetThemeParcloseConfig( c2sMsg.m_nDungeonID, c2sMsg.m_nParcloseID );
        if ( pThemeParcloseConfig != NULL )
        {
            GameMsg_S2C_GetParcloseInfoDetailResult resultMsg;
            resultMsg.m_pThemeParcloseConfig = pThemeParcloseConfig;
            resultMsg.m_nSex = m_pRoleAttr->GetSex();

            const std::list<CItem> & costitem = CDungeonDataMgr::instance().GetSkipParcloseItem();
            std::list<CItem>::const_iterator it = costitem.begin();
            if ( it != costitem.end() )
            {
                resultMsg.m_nSkipParcloseItem = it->m_nItemType;
                resultMsg.m_nSkipParcloseItemCount = it->m_nItemCount;
            }

            CDungeonDataMgr::instance().GetThemeParcloseDrop( c2sMsg.m_nDungeonID, c2sMsg.m_nParcloseID, resultMsg.m_MaleItems, resultMsg.m_FemaleItems, resultMsg.m_DropDebrisInfo );
            resultMsg.m_nLuckStar = GetParcloseStarCount(c2sMsg.m_bIsMainline ? EDungeonType_Mainline : EDungeonType_Theme, c2sMsg.m_nDungeonID, c2sMsg.m_nParcloseID);
            m_pRoleNet->SendPlayerMsg( & resultMsg );

            bIsFind = true;
        }
    }

    if ( !bIsFind )
    {
        GameMsg_S2C_GetParcloseInfoDetailFail failMsg;
        failMsg.m_strError = "DungeonError_NotFindParcloseID";
        m_pRoleNet->SendPlayerMsg( &failMsg );
    }	
}

void CEntityDungeonComponent::OnInviteResponse(  GameMsg_Base &msg, CSlotPeer &slotPeer  )
{
    GameMsg_C2S_InviteFriendResponse & c2sMsg = (GameMsg_C2S_InviteFriendResponse&)msg;

    // 开始副本
    CRoleEntity * pInviter = CPlayerManager::Instance().GetEntityByRoleID( c2sMsg.m_nInviterID );
    if ( pInviter == NULL || c2sMsg.m_nInviterID == m_pRoleAttr->GetRoleID())
    {
        return;
    }

    if ( pInviter->GetRoleDungeon()->GetInviteFriendTimer() == 0 )
    {
        return;
    }

    pInviter->GetRoleDungeon()->SetInviteFriendTimer( 0 );
    if ( c2sMsg.m_bIsAccept )
    {
        if (m_pRoleRoom->IsInRoom())
        {
            pInviter->GetRoleDungeon()->_SendStartParcloseErrorMsg(EDungeonError_InRoom);
            _SendStartParcloseErrorMsg(EDungeonError_InRoom);

            return;
        }

        if ( m_pRoleCouple->WeddingRoom() != NULL)
        {
            pInviter->GetRoleDungeon()->_SendStartParcloseErrorMsg(EDungeonError_InCoupleRoom);
            _SendStartParcloseErrorMsg(EDungeonError_InCoupleRoom);

            return;
        }

        if ( m_pRolePhoto->PhotoRoom() != NULL) 
        {
            pInviter->GetRoleDungeon()->_SendStartParcloseErrorMsg(EDungeonError_InPhoteRoom);
            _SendStartParcloseErrorMsg(EDungeonError_InPhoteRoom);

            return;
        }

        if ( m_pRoleAmuseRoom->AmuseRoom() != NULL)
        {
            pInviter->GetRoleDungeon()->_SendStartParcloseErrorMsg(EDungeonError_InAmuseRoom);
            _SendStartParcloseErrorMsg(EDungeonError_InAmuseRoom);

            return;
        }

        if ( m_pRoleDanceGroup->GetCeremonyRoom() != NULL)
        {
            pInviter->GetRoleDungeon()->_SendStartParcloseErrorMsg(EDungeonError_InCeremonyRoom);
            _SendStartParcloseErrorMsg(EDungeonError_InCeremonyRoom);

            return;
        }

        std::vector<itemtype_t> items;
        EDungeonType eDungeonType = c2sMsg.m_bIsMainline ?EDungeonType_Mainline:EDungeonType_Theme;
        int nError = CDungeonMgr::Instance().CreateParclose(eDungeonType, c2sMsg.m_nDungeonID, c2sMsg.m_nParcloseID, m_pRoleAttr->GetRoleID(), *pInviter->GetRoleDungeon() );
        if ( nError == EDungeonError_Sucess )
        {
            // Host
            pInviter->GetRoleDungeon()->_HandleParcloseCost( c2sMsg.m_nParcloseID, items );

            // os
            SetPhoneOS( c2sMsg.m_nPhoneOS );

            // 成功消息
            m_pParcloseRoom->StartParclose(); 

            // 邀请成功的好友
            pInviter->GetRoleDungeon()->AddInviteFriendInfo(GetRoleID(), eDungeonType, c2sMsg.m_nDungeonID, c2sMsg.m_nParcloseID);
        }
        else
        {
            pInviter->GetRoleDungeon()->_SendStartParcloseErrorMsg( nError );
            _SendStartParcloseErrorMsg( nError );
        }
    }
    else
    {
        pInviter->GetRoleDungeon()->_SendStartParcloseErrorMsg( EDungeonError_FriendRefuse );
    }
}

void CEntityDungeonComponent::OnGetRankList( GameMsg_Base &msg, CSlotPeer &slotPeer )
{
    GameMsg_C2S_GetDungeonList & c2sMsg = (GameMsg_C2S_GetDungeonList&)msg;

    GameMsg_S2G_GetRankList requestMsg;
    requestMsg.m_nCurrentPage = c2sMsg.m_nPage;
    requestMsg.m_nRoleID = m_pRoleAttr->GetRoleID();
    ::SendMsg2GroupServer(&requestMsg);
}

void CEntityDungeonComponent::_OnGetRankList( GameMsg_Base &msg, CSlotPeer &slotPeer )
{
    GameMsg_G2S_GetRankListResult & g2sMsg = (GameMsg_G2S_GetRankListResult&)msg;

    unsigned int nRankCount = (unsigned int)g2sMsg.m_RankList.size();

    GameMsg_S2C_GetDungeonListResult resultMsg;
    resultMsg.m_nCurPage = g2sMsg.m_nCurrentPage;
    resultMsg.m_nTotalCount = nRankCount;

    int nBeginRank = g2sMsg.m_nCurrentPage *PARCLOSE_FRIENDRANK_PAGENUM ;
    std::list<DungeonRankInfo>::iterator it = g2sMsg.m_RankList.begin();
    int nTempIndex = 0;
    for ( ; it != g2sMsg.m_RankList.end(); ++it )
    {
        if (  nTempIndex >= nBeginRank && nTempIndex < nBeginRank + PARCLOSE_FRIENDRANK_PAGENUM )
        {
            CDungeonRank clientRank;
            clientRank.m_nFriendID = it->nRoleID;
            clientRank.m_nRank = nTempIndex + 1;
            clientRank.m_strFriendName = it->strRoleName;
            clientRank.m_nStarCount = it->nStarCount;
            clientRank.m_strDungeonName = ConfigManager::Instance().GetMagicLampConfigMgr().GetDungeonNameByParcloseID( it->nParcloseID );

            CParcloseConfig * pParcloseConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetMainlineParcloseConfig( it->nParcloseID );
            if ( pParcloseConfig != NULL )
            {
                clientRank.m_strParcloseName = pParcloseConfig->m_strParcloseName;
            }
            
            resultMsg.m_RankList.push_back( clientRank );
        }
        nTempIndex++;
    }

    resultMsg.m_nSelfRank = (unsigned short)g2sMsg.m_nSelfRank;

    unsigned int nTopDugeonID = GetTopActivedDungeonID();
    unsigned int nTopParcloseID = GetTopActivedParcloseID();

    resultMsg.m_nTopDungeonID = nTopDugeonID;
    resultMsg.m_strTopDungeonName = ConfigManager::Instance().GetMagicLampConfigMgr().GetDungeonNameByParcloseID( nTopParcloseID );
    resultMsg.m_nTopParcloseID = nTopParcloseID;
    CParcloseConfig * pParcloseConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetMainlineParcloseConfig( nTopParcloseID );
    if ( pParcloseConfig != NULL )
    {
        resultMsg.m_strTopParcloseName = pParcloseConfig->m_strParcloseName;
    }
    
    m_pRoleNet->SendPlayerMsg( &resultMsg );
}

void CEntityDungeonComponent::OnGetBuyCrystalInfo( GameMsg_Base &msg, CSlotPeer &slotPeer )
{
    GameMsg_C2S_GetBuyCrystalInfo c2smsg = (GameMsg_C2S_GetBuyCrystalInfo&)msg;

    GameMsg_S2C_GetBuyCrystalInfoResult resultmsg;
    resultmsg.m_mapBuyCrystalConfig = CDungeonDataMgr::instance().GetBuyCrystalConfig();
    resultmsg.m_nFavorableBuyVitCount = CVIPDataMgr::Instance().GetBuyVitCount(m_pRoleVip->VIPLevel())
        + CDungeonDataMgr::instance().GetFavorableBuyVitCount();
    resultmsg.m_nHaveBuyVitCount = m_MagicLamp.m_nFavorableBuyVitCount; 
    SendPlayerMsg(&resultmsg);
}

void CEntityDungeonComponent::OnBuyCrystal(  GameMsg_Base &msg, CSlotPeer &slotPeer )
{
    GameMsg_C2S_BuyCrystal & c2sMsg = ( GameMsg_C2S_BuyCrystal& )msg;

    int nError = _BuyCrystal( c2sMsg.m_nIndex );
    if ( nError == EDungeonError_Sucess )
    {
        GameMsg_S2C_BuyCrystalSuc sucMsg;
        sucMsg.m_nCrystalCount = GetCrystal();
        sucMsg.m_nTodayVipBuyVitNum = GetVipBuyVitCount();
        m_pRoleNet->SendPlayerMsg( &sucMsg );
    }
    else
    {
        _SendBuyCrystalErrorMsg( nError );
    }
}

void CEntityDungeonComponent::OnSkipParclose( GameMsg_Base &msg, CSlotPeer &slotPeer )
{
    GameMsg_C2S_SkipParcose & skipMsg = ( GameMsg_C2S_SkipParcose &)msg;
    int nError = _SkipParclose( skipMsg.m_bIsMainline ? EDungeonType_Mainline : EDungeonType_Theme, skipMsg.m_nDungeonID, skipMsg.m_nParcloseID );
    if ( nError == EDungeonError_Sucess )
    {
        if( skipMsg.m_bIsMainline )
        {
            SetMainlineParcloseStarCount( skipMsg.m_nParcloseID, 1 );

            GameMsg_S2C_SkipParcloseSuc sucMsg;
            sucMsg.m_nCurrentParcloseStar = 1;
            sucMsg.m_nAllStartCount = (unsigned short)GetLuckStar();
            unsigned int nNextParcloseID = ConfigManager::Instance().GetMagicLampConfigMgr().GetMainlineNextParcloseID( skipMsg.m_nParcloseID );
            sucMsg.m_nNextParcloseID = nNextParcloseID;
            sucMsg.m_nNextDungeonID = ConfigManager::Instance().GetMagicLampConfigMgr().GetDungeonIDByParcloseID( nNextParcloseID );
            m_pRoleNet->SendPlayerMsg( &sucMsg );

            if ( sucMsg.m_nNextDungeonID > skipMsg.m_nDungeonID )
            {
                m_nServerActiveParcloseID = sucMsg.m_nNextParcloseID;
            }
        }
        else
        {
            SetThemeParcloseStarCount(skipMsg.m_nDungeonID, skipMsg.m_nParcloseID, 1);

            GameMsg_S2C_SkipParcloseSuc sucMsg;
            unsigned int nNextParcloseID = ConfigManager::Instance().GetMagicLampConfigMgr().GetThemeNextParcloseID( skipMsg.m_nDungeonID, skipMsg.m_nParcloseID );
            sucMsg.m_nNextParcloseID = nNextParcloseID;
            sucMsg.m_nNextDungeonID = skipMsg.m_nDungeonID;
            m_pRoleNet->SendPlayerMsg( &sucMsg );
        }
    }
    else
    {
        _SendSkipParcloseErrorMsg( nError );
    }
}

void CEntityDungeonComponent::OnExchangeDebrisEquip( GameMsg_Base &msg, CSlotPeer &slotPeer )
{
    GameMsg_C2S_ExchangeDebrisEquip requiremsg = (GameMsg_C2S_ExchangeDebrisEquip&)msg;

    int nError = ExchangeDebrisEquip( requiremsg.m_nDebrisID );
    if ( nError != EDungeonError_Sucess )
    {
        GameMsg_S2C_ExchangeDebrisEquipFail failmsg;
        failmsg.m_cErrorFlag = (char)nError;
        SendPlayerMsg( &failmsg );
    }
}

void CEntityDungeonComponent::DeleteDebris( unsigned int nDebrisID, unsigned int nChanegCount )
{
    std::map<unsigned int,unsigned int>::iterator it = m_DebrisRecord.find( nDebrisID );
    if ( it != m_DebrisRecord.end() )
    {
		unsigned int nDeleteCount = nChanegCount;
		if ( it->second < nChanegCount )
		{
			nDeleteCount = it->second;
		}

		it->second -= nDeleteCount;
	
        //DB
        unsigned int * pDebrisInfo  = new unsigned int[2];
        *pDebrisInfo = nDebrisID;
        *(pDebrisInfo+1) = it->second;
        AddQuery( Query_Update_DebrisInfo, m_pRoleAttr->GetRoleID(), pDebrisInfo);

		if ( it->second <= 0 )
		{
			m_DebrisRecord.erase( it );
		}     
    }
}

void CEntityDungeonComponent::AddDebris( unsigned int nDebrisID, unsigned int nChangeCount )
{
	unsigned int nNewDebrisCount = 0;
    std::map<unsigned int,unsigned int>::iterator it = m_DebrisRecord.find( nDebrisID );
    if ( it != m_DebrisRecord.end() )
    {
		nNewDebrisCount = it->second + nChangeCount;
    }
    else
	{
		nNewDebrisCount = nChangeCount;
    }

	m_DebrisRecord[ nDebrisID ] = nNewDebrisCount;

    //DB
    unsigned int * pDebrisInfo  = new unsigned int[2];
    *pDebrisInfo = nDebrisID;
    *(pDebrisInfo+1) = nNewDebrisCount;
    AddQuery( Query_Update_DebrisInfo, m_pRoleAttr->GetRoleID(), pDebrisInfo);
}

unsigned int CEntityDungeonComponent::GetDebrisCount( unsigned int nDebrisID )
{
    std::map<unsigned int,unsigned int>::iterator it = m_DebrisRecord.find( nDebrisID );
    if ( it != m_DebrisRecord.end() )
    {
        return it->second;
    }

    return 0;
}

void CEntityDungeonComponent::OnGetMainlineDungeonInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2C_GetMainlineInfoResult resultmsg;
    const CMainlineConfigTable & dungeonfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetDungeonConfigTable();
    CMainlineConfigTable::const_iterator it = dungeonfig.begin();
    for ( ; it != dungeonfig.end(); ++it )
    {
        MsgDungeonInfo info;
        info.nDungeonID = it->second->m_nDungeonID;
        info.strName = it->second->m_strDungeonName;
        info.strIcon = it->second->m_strIconName;
        info.strAtlas = it->second->m_strIconAtlas;
        info.cState  = 1; // 主线一直在进行中
        info.bIsActived = IsDungeonActived( it->second->m_nDungeonID );

        unsigned int nDungeonFirstParcloseID = ConfigManager::Instance().GetMagicLampConfigMgr().GetFirstParcloseID();
        if ( nDungeonFirstParcloseID == m_nServerActiveParcloseID || !info.bIsActived )
        {
            info.bHasStory = it->second->m_nStoryID > 0 ? true : false ;
        }
        else
        {
            info.bHasStory = false;
        }

        resultmsg.m_listDungeonInfo.push_back( info );
    }
    SendPlayerMsg( &resultmsg );
}

int CEntityDungeonComponent::GetThemeState( unsigned int nOpenTime, unsigned int nEndTime )
{
    unsigned int nNowTime = (unsigned int)time(NULL);
    if ( nOpenTime > nNowTime )
    {
        return 0;
    }
    else if ( nOpenTime <= nNowTime && nEndTime >= nNowTime )
    {
        return 1;
    }
    else if ( nEndTime < nNowTime )
    {
        return 2;
    }

    return 0;
}

void CEntityDungeonComponent::OnGetThemeDungeonInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2C_GetThemeInfoResult resultmsg;

    const std::map<unsigned int,CThemeConfig*> & themeconfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetThemeConfigList();
    std::map<unsigned int,CThemeConfig*>::const_iterator it = themeconfig.begin();
    for ( ; it != themeconfig.end(); ++it )
    {
        MsgDungeonInfo info;
        info.nDungeonID = it->second->m_nThemeID;
        info.strName = it->second->m_strName;
        info.strIcon = it->second->m_strThemeIcon;
        info.strAtlas = it->second->m_strThemeAtlas;
        info.cState  = (char)GetThemeState( it->second->m_nOpenTime, it->second->m_nEndTime );
        info.bIsActived = true;

        std::set<unsigned int>::iterator iter = m_ServerActiveThemeParcloseID.find( it->second->m_nThemeID );
        if ( iter != m_ServerActiveThemeParcloseID.end() )
        {
            info.bHasStory = it->second->m_nStoryID > 0 ? true :false ;
        }
        else
        {
            info.bHasStory = false;
        }
        
        resultmsg.m_listDungeonInfo.push_back( info );
    }
    SendPlayerMsg( &resultmsg );
}

int CEntityDungeonComponent::ExchangeDebrisEquip( unsigned int nDebrisID )
{
    // 获取配置
    CDebrisConfig * pDebrisConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetDebrisConfig( nDebrisID );
    if ( pDebrisConfig == NULL || pDebrisConfig->m_nNeedNum <= 0 )
    {
        return EDungeonError_DebrisConfigErr;
    }

    // 检查数目
    unsigned int nDebrisCount = 0;
    std::map<unsigned int,unsigned int>::iterator it = m_DebrisRecord.find( nDebrisID );
    nDebrisCount = it == m_DebrisRecord.end() ? 0 : it->second;
    if ( nDebrisCount < pDebrisConfig->m_nNeedNum )
    {
        return EDungeonError_DebrisCountNot;
    }

    // 检查货币
    switch( pDebrisConfig->m_nCurrencyType )
    {
    case EMallCurrencyType_MCoin:
        if ( m_pRoleAttr->GetPoint() < pDebrisConfig->m_nCurrencyCount )
        {
            return EDungeonError_MoneyNotEnough;
        }
        m_pRoleAttr->ChangeBill( (-1)*pDebrisConfig->m_nCurrencyCount,EChangeBillCause_Del_DebrisExchange );
        break;
    case  EMallCurrencyType_Money :
        if ( m_pRoleAttr->GetMoney() < (int)pDebrisConfig->m_nCurrencyCount )
        {
            return EDungeonError_MoneyNotEnough;
        }
        m_pRoleAttr->ChangeMoney( (-1)*pDebrisConfig->m_nCurrencyCount, EChangeMoneyCause_Del_DebrisExchange, 0);
        break;
    case EMallCurrencyType_BindMCoin:
        if ( m_pRoleAttr->GetBindPoint() < pDebrisConfig->m_nCurrencyCount )
        {
            return EDungeonError_MoneyNotEnough;
        }
        m_pRoleAttr->ChangeBindBill((-1)*pDebrisConfig->m_nCurrencyCount,EChangeBindBillCause_Del_DebrisExchange);
        break;
    default: break;
    }

    std::list<CItem> maleItem;
    std::list<CItem> femaleItem;

    std::vector<CItem>::iterator iter = pDebrisConfig->m_MaleExchangeItem.begin();
    for ( ; iter != pDebrisConfig->m_MaleExchangeItem.end(); ++iter )
    {
        maleItem.push_back( *iter );
    }

    std::vector<CItem>::iterator iter1 = pDebrisConfig->m_FemaleExchangeItem.begin();
    for ( ; iter1 != pDebrisConfig->m_FemaleExchangeItem.end(); ++iter1 )
    {
        femaleItem.push_back( *iter1 );
    }

    // 添加奖励
    if ( m_pRoleAttr->GetSex() == ESexType_Male )
    {
        CItemProcess::AddItems( *(CRoleEntity *)Entity(), maleItem, EItemAction_Add_DebrisExchange, 0, true );
    }
    else
    {
        CItemProcess::AddItems( *(CRoleEntity *)Entity(), femaleItem, EItemAction_Add_DebrisExchange, 0, true );
    }

    // 扣除物品
    DeleteDebris( nDebrisID, pDebrisConfig->m_nNeedNum );

    GameMsg_S2C_ExchangeDebrisEquipSuc sucmsg;
    sucmsg.m_nDebrisID = nDebrisID;
    if ( m_pRoleAttr->GetSex() == ESexType_Male )
    {
        sucmsg.m_listExchangeEquip = maleItem;
    }
    else
    {
        sucmsg.m_listExchangeEquip = femaleItem;
    }
    sucmsg.m_nRemainCount = GetDebrisCount( nDebrisID );
    SendPlayerMsg( &sucmsg );

    // log
    SaveDebrisLog( CRoleDungeonDebrisLog::EDebrisAction_Exchange,0,nDebrisID, pDebrisConfig->m_nNeedNum,0,0,sucmsg.m_nRemainCount);

    return EDungeonError_Sucess;
}

int CEntityDungeonComponent::_SkipParclose( EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID )
{
    CParcloseConfig * pParcloseConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetParcloseConfig( eType, nDungeonID, nParcloseID );
    if ( pParcloseConfig == NULL)
    {
        return EDungeonError_NotFindParcloseID;
    }

    if ( !ParcloseIsActived( eType, nDungeonID, nParcloseID ))
    {
        return EDungeonError_ParcloseNotActived;
    }

    unsigned int nNextParcloseID = ConfigManager::Instance().GetMagicLampConfigMgr().GetNextParcloseID( eType, nDungeonID, nParcloseID );
    if ( nNextParcloseID != nParcloseID && ParcloseIsActived( eType, nDungeonID, nNextParcloseID ))
    {
        return EDungeonError_ParcloseHasActived;
    }

    if ( nNextParcloseID == nParcloseID )
    {
        if ( GetParcloseStarCount(eType, nDungeonID, nParcloseID) >= 1 )
        {
            return EDungeonError_ParcloseHasActived;
        }
    }

    const std::list<CItem> & costitem = CDungeonDataMgr::instance().GetSkipParcloseItem();
    std::list<CItem>::const_iterator it = costitem.begin();
    for ( ; it != costitem.end(); ++it )
    {
        if ( m_pRoleItem->GetItemCount( it->m_nItemType,false, false) < it->m_nItemCount )
        {
            return EDungeonError_ItemNotEnough;
        }
    }

    it = costitem.begin();
    for ( ; it != costitem.end(); ++it )
    {
        m_pRoleItem->RemoveItemByType( it->m_nItemType, it->m_nItemCount, EItemAction_Del_SkipParclose);
    }

    if ( eType == EDungeonType_Mainline )
    {
        ActiveMainlineParclose( nNextParcloseID );
    }
    else
    {
        ActiveThemeParclose( nDungeonID, nNextParcloseID );
    }

    // log
    InsertActionLog(CRoleDungeonLog::EDungeonAction_UseSkipItem, m_pRoleAttr->GetRoleID(), 0, 0, 0, it->m_nItemType);

    return EDungeonError_Sucess;
}

int CEntityDungeonComponent::_BuyCrystal( unsigned int nIndex )
{
    CBuyCrystalConfig * pBuyCrystalConfig = CDungeonDataMgr::instance().GetBuyCrystalConfig( nIndex );
    if ( pBuyCrystalConfig == NULL || pBuyCrystalConfig->m_nPrice <= 0 || pBuyCrystalConfig->m_nCrystalCount <= 0 )
    {
        return EDungeonError_BuyCrystalConfigErr;
    }

    // 上限
    const CCrystalConfig  & crystalconfig = CDungeonDataMgr::instance().getCrystalConfig();
    if ( crystalconfig.m_nMaxValue <= 0 || crystalconfig.m_nRestoreTime <= 0 )
    {
        return EDungeonError_CrystalConfigErr;
    }

    if ( GetCrystal() >= (int)crystalconfig.m_nMaxValue )
    {
        return EDungeonError_CrystalIsFull;
    }

    int nCostMPoint = 0;
    if (m_pRoleVip->IsVIP())
    {
        if (GetVipBuyVitCount() >= (int)crystalconfig.m_nFavorableBuyVitCount + 
                    CVIPDataMgr::Instance().GetBuyVitCount(m_pRoleVip->VIPLevel()))
        {
            nCostMPoint = pBuyCrystalConfig->m_nPrice;
        }
        else
        {
            nCostMPoint = pBuyCrystalConfig-> m_nFavorablePrice;
        }
    }
    else
    {
        if (GetVipBuyVitCount() >= (int)crystalconfig.m_nFavorableBuyVitCount)
        {
            nCostMPoint = pBuyCrystalConfig->m_nPrice;
        }
        else
        {
            nCostMPoint = pBuyCrystalConfig->m_nFavorablePrice;
        }
    }

    if ((int)m_pRoleAttr->GetPoint() < nCostMPoint)
    {
        return EDungeonError_MoneyNotEnough;
    }

    SetCrystal(	pBuyCrystalConfig->m_nCrystalCount );
    m_pRoleAttr->ChangeBill( -1*nCostMPoint, EChangeBillCause_Del_BuyCrystal, true );

    if (nCostMPoint == (int)pBuyCrystalConfig->m_nFavorablePrice)
    {
        AddTodayVipBuyVitNum(1);
        SyncMagicToDB(ESyncToDBType_Update);
    }

    // log
    InsertActionLog(CRoleDungeonLog::EDungeonAction_BuyCrystal, m_pRoleAttr->GetRoleID(), 0, 0 , 0, pBuyCrystalConfig->m_nCrystalCount);
    return EDungeonError_Sucess;
}

void CEntityDungeonComponent::_SendBuyCrystalErrorMsg( int nErr )
{
    GameMsg_S2C_BuyCrystalFail msgFail;

    switch( nErr)
    {
    case  EDungeonError_BuyCrystalConfigErr : msgFail.m_strError = "EDungeonError_BuyCrystalConfigErr";break;
    case  EDungeonError_CrystalConfigErr : msgFail.m_strError = "EDungeonError_CrystalConfigErr";break;
    case  EDungeonError_CrystalIsFull : msgFail.m_strError = "EDungeonError_CrystalIsFull";break;
    case  EDungeonError_MoneyNotEnough : msgFail.m_strError = "EDungeonError_MoneyNotEnough";break;
    case  EDungeonError_BuyCrystalMuch : msgFail.m_strError = "EDungeonError_BuyCrystalMuch";break;
    }

    m_pRoleNet->SendPlayerMsg( &msgFail );
}

void CEntityDungeonComponent::_SendSkipParcloseErrorMsg( int nErr )
{
    GameMsg_S2C_SkipParcloseFail msgFail;

    switch( nErr)
    {
    case  EDungeonError_NotFindParcloseID : msgFail.m_strError = "EDungeonError_NotFindParcloseID";break;
    case  EDungeonError_ParcloseNotActived : msgFail.m_strError = "EDungeonError_ParcloseNotActived";break;
    case  EDungeonError_ParcloseNotCanSkip : msgFail.m_strError = "EDungeonError_ParcloseNotCanSkip";break;
    case  EDungeonError_ItemNotEnough : msgFail.m_strError = "EDungeonError_ItemNotEnough";break;
    case  EDungeonError_ParcloseHasActived : msgFail.m_strError = "EDungeonError_ParcloseHasActived";break;
    }

    m_pRoleNet->SendPlayerMsg( &msgFail );
}

bool CEntityDungeonComponent::IsMagicActived()
{
    if (m_MainlineParcloseRecord.empty())
    {
        return false;
    }

    return true;
}

bool CEntityDungeonComponent::ParcloseIsActived( EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID )
{
    switch( eType )
    {
    case  EDungeonType_Mainline :
        {
            MapMainlineParcloseRecord::iterator it = m_MainlineParcloseRecord.find( nParcloseID );
            if ( it != m_MainlineParcloseRecord.end() )
            {
                return it->second.bIsActived;
            }
        }
        break;
    case  EDungeonType_Theme :
        {
            MapThemeParcloseRecord::iterator it = m_ThemeParcloseRecord.find( nDungeonID );
            if ( it != m_ThemeParcloseRecord.end() )
            {
                std::map<unsigned int,ThemeParcloseRecord>::iterator iter = it->second.find( nParcloseID );
                if ( iter != it->second.end() )
                {
                    return iter->second.bIsActived;
                }
            }

        }
        break;
    }

    return false;
}

void CEntityDungeonComponent::ActiveParclose(unsigned int nParcloseID )
{
    if ( m_pParcloseRoom != NULL )
    {
        if ( m_pParcloseRoom->GetDungeonType() == EDungeonType_Mainline )
        {
            ActiveMainlineParclose( nParcloseID );
        }
        else if ( m_pParcloseRoom->GetDungeonType() == EDungeonType_Theme )
        {
            ActiveThemeParclose( m_pParcloseRoom->GetDungeonID(), nParcloseID );
        }
    }
}

void CEntityDungeonComponent::ActiveMainlineParclose( unsigned int nParcloseID )
{
    MapMainlineParcloseRecord::iterator it = m_MainlineParcloseRecord.find( nParcloseID );
    if (it != m_MainlineParcloseRecord.end())
    {
        return ;	
    }

    MainlineParcloseRecord info;
    info.nParcloseID = nParcloseID;
    info.bIsActived = true;
    info.nUpdateTime = (unsigned int)time(NULL);
    m_MainlineParcloseRecord[ nParcloseID ] = info;

    MainlineParcloseRecord * pRecord = new MainlineParcloseRecord( info );
    if ( pRecord != NULL )
    {
        AddQuery(Query_Insert_ParcloseInfo, m_pRoleAttr->GetRoleID(), pRecord);
    }

    // sync group
    GameMsg_S2G_UpdateParcloseInfo syncMsg;
    syncMsg.m_nRoleID = m_pRoleAttr->GetRoleID();
    syncMsg.m_nParcloseID = nParcloseID;
    syncMsg.m_nStarCount = GetLuckStar();
    ::SendMsg2GroupServer(&syncMsg);
}

void CEntityDungeonComponent::ActiveThemeParclose( unsigned int nThemeID, unsigned int nParcloseID, bool bSave )
{
    MapThemeParcloseRecord::iterator it = m_ThemeParcloseRecord.find( nThemeID );
    if ( it != m_ThemeParcloseRecord.end())
    {
        std::map<unsigned int,ThemeParcloseRecord>::iterator iter = it->second.find( nParcloseID );
        if ( iter != it->second.end())
        {
            return;
        }
    }

    ThemeParcloseRecord record;
    record.nThemeID = nThemeID;
    record.nParcloseID = nParcloseID;
    record.bIsActived = true;
    record.nUpdateTime = (unsigned int)time(NULL);
    m_ThemeParcloseRecord[nThemeID].insert( std::make_pair(nParcloseID,record));

    if ( bSave )
    {
        ThemeParcloseRecord * pRecord = new ThemeParcloseRecord( record );
        if ( pRecord != NULL )
        {
            AddQuery( Query_Insert_ThemeParcloseInfo, m_pRoleAttr->GetRoleID(), pRecord );
        }
    } 
}

void CEntityDungeonComponent::GMActivedParclose( unsigned int nParcloseID )
{
    const std::map<unsigned int,CParcloseConfig*> & parcloselist = ConfigManager::Instance().GetMagicLampConfigMgr().GetParcloseConfigList();
    std::map<unsigned int,CParcloseConfig*>::const_iterator it = parcloselist.begin();

    for ( ; it != parcloselist.end(); ++it )
    {
        if ( it->second->m_nParcloseID <= nParcloseID )
        {
            ActiveMainlineParclose( it->second->m_nParcloseID );
        }
        else
        {
            break;
        }
    }
}

void CEntityDungeonComponent::GMActivedThemeParclose( unsigned int nThemeID, unsigned int nParcloseID )
{
	const std::map<unsigned int,CThemeParcloseConfig*> *  pParcloseList = ConfigManager::Instance().GetMagicLampConfigMgr().GetThemeParcloseList(nThemeID);
	if ( pParcloseList != NULL )
	{
		std::map<unsigned int,CThemeParcloseConfig*>::const_iterator it = pParcloseList->begin();
		for ( ; it != pParcloseList->end(); ++it )
		{
			if ( it->second->m_nParcloseID <= nParcloseID )
			{
				ActiveThemeParclose( nThemeID, it->second->m_nParcloseID );
			}
			else
			{
				break;
			}
		}
	}
}

void CEntityDungeonComponent::GMAddDebris( unsigned int nDebrisID, unsigned int nDebrisCount )
{
    AddDebris(nDebrisID, nDebrisCount);
}

bool CEntityDungeonComponent::IsNewMainlineParclose()
{
    return m_MagicLamp.m_nMaxMainline < (int)ConfigManager::Instance().GetMagicLampConfigMgr().GetMaxMainlineID();
}

bool CEntityDungeonComponent::IsNewThemeParclose()
{
    return m_MagicLamp.m_nMaxTheme < (int)ConfigManager::Instance().GetMagicLampConfigMgr().GetMaxThemeID();
}

void CEntityDungeonComponent::OnSyncInviteFriendInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_SyncDungeonInviteFriendInfo & syncinfo = (GameMsg_G2S_SyncDungeonInviteFriendInfo&)msg;

    m_InviteInfo = syncinfo.m_DungeonInviteInfo;
}

void CEntityDungeonComponent::SetParcloseStarCount( EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID, int nStarCount)
{
    switch( eType )
    {
    case EDungeonType_Mainline:
        SetMainlineParcloseStarCount( nParcloseID, nStarCount );
        break;
    case EDungeonType_Theme:
        SetThemeParcloseStarCount(nDungeonID, nParcloseID, nStarCount);
        break;
    default:
        break;
    }
}

void CEntityDungeonComponent::SetMainlineParcloseStarCount( unsigned int nParcloseID, int nStarCount )
{
    MapMainlineParcloseRecord::iterator it = m_MainlineParcloseRecord.find(nParcloseID);
    if (it != m_MainlineParcloseRecord.end())
    {
        if ((int)it->second.nStarCount < nStarCount)
        {
            it->second.nStarCount = (unsigned int)nStarCount;
            it->second.nUpdateTime = (unsigned int)time(NULL);

            MainlineParcloseRecord *pRecord = new MainlineParcloseRecord(it->second);
            if (pRecord != NULL)
            {
                AddQuery(Query_Update_ParcloseInfo, m_pRoleAttr->GetRoleID(), pRecord);
            }

            UpdateLuckStar();

            // 不需要传入完成,检测所有
            m_pRoleQuestNew->OnMagiclampComplete();
        }
    }
}

void CEntityDungeonComponent::SetThemeParcloseStarCount( unsigned int nThemeID, unsigned int nParcloseID, int nStarCount )
{
    MapThemeParcloseRecord::iterator it = m_ThemeParcloseRecord.find( nThemeID );
    if ( it != m_ThemeParcloseRecord.end())
    {
        std::map<unsigned int,ThemeParcloseRecord>::iterator iter = it->second.find( nParcloseID );
        if ( iter != it->second.end())
        {
            if ( iter->second.nStarCount < nStarCount)
            {
                iter->second.nStarCount = (unsigned short)nStarCount;
                iter->second.nUpdateTime = (unsigned int)time(NULL);
            }

            ThemeParcloseRecord * pRecord = new ThemeParcloseRecord( iter->second );
            if ( pRecord != NULL )
            {
                AddQuery( Query_Update_ThemeParcloseInfo, m_pRoleAttr->GetRoleID(), pRecord );
            }
        }
    }
}

bool CEntityDungeonComponent::IsDungeonActived( unsigned int nDungeonID )
{
    CDungeonConfig * pDungeonConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetDungeonConfig( nDungeonID );
    if ( pDungeonConfig != NULL )
    {
        if ( !pDungeonConfig->m_ParcloseList.empty() )
        {
            MapMainlineParcloseRecord::iterator iter = m_MainlineParcloseRecord.find( pDungeonConfig->m_ParcloseList[0]);
            if ( iter != m_MainlineParcloseRecord.end() )
            {
                return iter->second.bIsActived;
            }
        }
    }

    return false;
}

unsigned int CEntityDungeonComponent::GetDungeonFirstParcloseID( unsigned int nDungeonID )
{
    CDungeonConfig * pDungeonConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetDungeonConfig( nDungeonID );
    if ( pDungeonConfig != NULL )
    {
        if ( !pDungeonConfig->m_ParcloseList.empty() )
        {
            return pDungeonConfig->m_ParcloseList[0];
        }
    }

    return 0;
}

void CEntityDungeonComponent::RemoveInviteFriendInfo( unsigned int nFriendRoleID )
{
    std::set<unsigned int>::iterator it = m_InviteInfo.m_setInviteFriends.find( nFriendRoleID );
    if ( it != m_InviteInfo.m_setInviteFriends.end() )
    {
        m_InviteInfo.m_setInviteFriends.erase(it);
    }

	// 更新邀请好友消息
	GameMsg_S2C_UpdateInviteFriend friendmsg;
	friendmsg.m_nRemainInviteFriendCount = CDungeonDataMgr::instance().GetMaxInviteFriendNum() - m_InviteInfo.m_nCostInviteNum;
	friendmsg.m_setInvitedFriends = m_InviteInfo.m_setInviteFriends;
	SendPlayerMsg( &friendmsg );

	// sync group invite friend
	GameMsg_S2G_RemoveDungeonInviteInfo deleteinfo;
	deleteinfo.m_nRoleID = GetRoleID();
	deleteinfo.m_nDeleteRoleID = nFriendRoleID;
	::SendMsg2GroupServer(&deleteinfo);
}

void CEntityDungeonComponent::AddInviteFriendInfo(unsigned int nFriendRoleID, EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID)
{
    CParcloseConfig * pParcloseConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetParcloseConfig(eType, nDungeonID, nParcloseID);
    if (pParcloseConfig == NULL)
    {
        return;
    }

    bool bFriendTarget = false;
    std::vector<unsigned int>::iterator it = pParcloseConfig->m_Targets.begin();
    for (; it != pParcloseConfig->m_Targets.end(); ++it)
    {
        CTargetConfig * pTargetConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetTargetConfig(*it);
        if (pTargetConfig != NULL)
        {
            bFriendTarget = pTargetConfig->m_nFlag == (unsigned int)ETargetFlag_InviteFriend;
        }
    }

    if (!bFriendTarget)
    {
        if (nFriendRoleID == m_pRoleCouple->CoupleID())
        {
            return;
        }
    }

    m_InviteInfo.m_nCostInviteNum++;
    m_InviteInfo.m_nLastInviteTime = (unsigned int)time(NULL);
    m_InviteInfo.m_setInviteFriends.insert( nFriendRoleID );

    // 更新邀请好友消息
    GameMsg_S2C_UpdateInviteFriend friendmsg;
    friendmsg.m_nRemainInviteFriendCount = CDungeonDataMgr::instance().GetMaxInviteFriendNum() - m_InviteInfo.m_nCostInviteNum;
    friendmsg.m_setInvitedFriends = m_InviteInfo.m_setInviteFriends;
    SendPlayerMsg( &friendmsg );

    // sync group invite friend
    GameMsg_S2G_AddDungeonInviteFriendInfo s2gmsg;
    s2gmsg.m_nRoleID = GetRoleID();
    s2gmsg.m_nFriendRoleID = nFriendRoleID;
    s2gmsg.m_nInviteTime = m_InviteInfo.m_nLastInviteTime;
    ::SendMsg2GroupServer(&s2gmsg);
}

void CEntityDungeonComponent::CheckAndResetInviteInfo()
{
    if ( m_InviteInfo.m_nLastInviteTime != 0 && !IsTimeIsToday(m_InviteInfo.m_nLastInviteTime) )
    {
        m_InviteInfo.Reset();

        // sync group clear 
        GameMsg_S2G_ClearDungeonInviteFriendInfo clearinfo;
        clearinfo.m_nRoleID = GetRoleID();
        ::SendMsg2GroupServer(&clearinfo);
    }
}

bool CEntityDungeonComponent::IsInvited( unsigned int nRoleID )
{
    std::set<unsigned int>::iterator it = m_InviteInfo.m_setInviteFriends.find( nRoleID );
    if ( it != m_InviteInfo.m_setInviteFriends.end() )
    {
        return true;
    }

    return false;
}

void CEntityDungeonComponent::SaveDebrisLog( int nAction,int nIsMainline, unsigned int nDebrisID, unsigned int nDebrisCount, unsigned int nDungeonID, unsigned int nParcloseID,unsigned int nLeftCount)
{
    //log
    CRoleDungeonDebrisLog * pLog = new CRoleDungeonDebrisLog();
    pLog->m_nRoleID = GetRoleID();
    pLog->m_eAction = (CRoleDungeonDebrisLog::EDebrisAction)nAction;
    pLog->m_nIsMainline = nIsMainline;
    pLog->m_nDungeonID = nDungeonID;
    pLog->m_nParcloseID = nParcloseID;
    pLog->m_nDebrisID = nDebrisID;
    pLog->m_nDebrisCount = nDebrisCount;
    pLog->m_nLeftDebrisCount = nLeftCount;
    
    g_pQueryMgr->AddQuery(QUERY_LogRoleDebris, 0, pLog);
}

void CEntityDungeonComponent::InsertActionLog( CRoleDungeonLog::EDungeonAction eAction, unsigned int nRoleID, bool bIsMainline, unsigned int nDungeonID, unsigned int nParcloseID, int nValue )
{
    CRoleDungeonLog * pLog = new CRoleDungeonLog();
    pLog->m_nRoleID = GetRoleID();
    pLog->m_eAction = eAction;
    pLog->m_bIsMainline = bIsMainline;
    pLog->m_nDungeonID = nDungeonID;
    pLog->m_nParcloseID = nParcloseID;
    pLog->m_nValue = nValue;

    g_pQueryMgr->AddQuery(QUERY_LogRoleDungeon, 0, pLog);
}

unsigned int CEntityDungeonComponent::GetTopActivedParcloseID()
{
    MapMainlineParcloseRecord::reverse_iterator it = m_MainlineParcloseRecord.rbegin();
    if ( it != m_MainlineParcloseRecord.rend() )
    {
        return it->first;
    }

    return 0;
}

unsigned int CEntityDungeonComponent::GetTopActivedDungeonID()
{
    unsigned int nParcloseID = GetTopActivedParcloseID();
    return ConfigManager::Instance().GetMagicLampConfigMgr().GetDungeonIDByParcloseID( nParcloseID );
}

void CEntityDungeonComponent::OnStartParclose( GameMsg_Base &msg, CSlotPeer &slotPeer )
{
    GameMsg_C2S_StartParclose & c2sMsg = (GameMsg_C2S_StartParclose&)msg;
    WriteLog(LOGLEVEL_DEBUG, "Debug:MagicLamp:beginParclose roleid:%u_parcloseID:%u_friendID:%u", m_pRoleAttr->GetRoleID(), c2sMsg.m_nParcloseID, c2sMsg.m_nFriendID );

    int nError = _CheckParclose(c2sMsg.m_bIsMainline?EDungeonType_Mainline:EDungeonType_Theme,c2sMsg.m_nDungeonID,c2sMsg.m_nParcloseID, c2sMsg.m_listItem, c2sMsg.m_nFriendID);
    if ( nError != EDungeonError_Sucess )
    {
        _SendStartParcloseErrorMsg( nError );
        return;
    }

    if ( c2sMsg.m_nFriendID == 0 )
    {   
        nError = CDungeonMgr::Instance().CreateParclose( c2sMsg.m_bIsMainline?EDungeonType_Mainline:EDungeonType_Theme, c2sMsg.m_nDungeonID, c2sMsg.m_nParcloseID, 0, *this );
        if ( nError == EDungeonError_Sucess )
        {
            // 
            _HandleParcloseCost( c2sMsg.m_nParcloseID, c2sMsg.m_listItem );
            WriteLog(LOGLEVEL_DEBUG, "Debug:MagicLamp:CheckParclose roleid:%u_parcloseID:%u_friendID:%u", m_pRoleAttr->GetRoleID(), c2sMsg.m_nParcloseID, c2sMsg.m_nFriendID );

            // 发送成功消息
            m_pParcloseRoom->StartParclose();

            // log
            InsertActionLog(CRoleDungeonLog::EDungeonAction_JoinDungeon, m_pRoleAttr->GetRoleID(), c2sMsg.m_bIsMainline, c2sMsg.m_nDungeonID, c2sMsg.m_nParcloseID, 0 );
        }
        else
        {
            _SendStartParcloseErrorMsg( nError );
        }
    }
}



bool CEntityDungeonComponent::RecvRoundMark(int nMode, int nRound, unsigned int nMark, char chKeyRank, const std::vector<char>& vecRoundRank)
{
    std::vector<int> vecRank;
    for (size_t i = 0; i < vecRoundRank.size(); ++i)
    {
        vecRank.push_back(vecRoundRank[i]);
    }

    bool bRes = m_MarkInfo.AddRoundMark(nMode, nRound, nMark, chKeyRank, vecRank);

    LogRoleCheckingMarkFailed();

    return bRes;
}

void CEntityDungeonComponent::SendPlayerMsg( GameMsg_Base* pMsg )
{
    m_pRoleNet->SendPlayerMsg( pMsg );
}

void CEntityDungeonComponent::CalcGrade()
{
    int nGrade = RoomDanceGrade_D;
    if (RoleMarkInfo().TotalRankBase() > 0)
    {
        int nAccuracy = CalcTotalMark() / RoleMarkInfo().TotalRankBase();
        if (nAccuracy >= 100)
        {
            nGrade = RoomDanceGrade_SS;
        }
        else if (nAccuracy >= 90)
        {
            if ( RoleMarkInfo().TotalRankPercent(RoomRoundRank_Good) < 1
                && RoleMarkInfo().TotalRankPercent(RoomRoundRank_Bad) < 1
                && RoleMarkInfo().TotalRankCount(RoomRoundRank_Miss) == 0 )
            {
                nGrade = RoomDanceGrade_S;
            }
            else
            {
                nGrade = RoomDanceGrade_A;
            }
        }
        else if (nAccuracy >= 80)
        {
            if (RoleMarkInfo().TotalRankCount(RoomRoundRank_Miss) == 0)
            {
                nGrade = RoomDanceGrade_A;
            }
            else
            {
                nGrade = RoomDanceGrade_B;
            }
        }
        else if (nAccuracy >= 70)
        {
            if (RoleMarkInfo().TotalRankCount(RoomRoundRank_Miss) == 0)
            {
                nGrade = RoomDanceGrade_B;
            }
            else
            {
                nGrade = RoomDanceGrade_C;
            }
        }
        else if (nAccuracy >= 60)
        {
            nGrade = RoomDanceGrade_C;
        }
    }

    m_MarkInfo.WinGrade( nGrade );
}

int CEntityDungeonComponent::CalcTotalMark()
{
    int nTotalMark = 0;

    int nMusicMode = MusicMode_None;
    int nMusicLevel = MusicLevel_None;
    if (m_pParcloseRoom != NULL)
    {
        nMusicMode = m_pParcloseRoom->PlayMusicMode();
        nMusicLevel = m_pParcloseRoom->PlayMusicLevel();
    }

    for (int nRank = RoomRoundRank_Miss; nRank < RoomRoundRank_Max; ++nRank)
    {
        int nSkillMark = 0;
        int nRankMark = CalcRankMark(nMusicMode, nMusicLevel, nRank, nSkillMark);

        // 将宠物技能额外增加的基础分加入总分中
        int nTotalSkillMark = RoleMarkInfo().TotalRankCount(nRank) * nSkillMark;
        RoleMarkInfo().AddMark(nTotalSkillMark);

        nTotalMark += RoleMarkInfo().TotalRankCount(nRank) * nRankMark;
    }

    if (m_status.HasCastPetSkill())
    {
        // 将宠物技能额外增加比例的总分加入总分中
        int nSkillMarkRatio = m_status.GetSkillDanceMarkRatio(nMusicMode);
        int nTotalSkillMark =  RoleMarkInfo().Mark() * (int)((double)nSkillMarkRatio / 100);
        RoleMarkInfo().AddMark(nTotalSkillMark);

        nTotalMark += nTotalSkillMark;
    }

    return nTotalMark;
}

unsigned int CEntityDungeonComponent::CalcStarCount(CParcloseConfig * pParcloseConfig)
{
    if (pParcloseConfig == NULL || m_pParcloseRoom == NULL)
    {
        return 0;
    }

    unsigned int nStarCount = 0;
    std::vector<unsigned int>::iterator it = pParcloseConfig->m_Targets.begin();
    for (; it != pParcloseConfig->m_Targets.end(); ++it)
    {
        CTargetConfig * pTargetConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetTargetConfig(*it);
        if (pTargetConfig != NULL)
        {
            switch(pTargetConfig->m_nFlag)
            {
            case ETargetFlag_Mark: nStarCount = pTargetConfig->GetStarCount(m_pParcloseRoom->GetAllDancerScore()); break;
            case ETargetFlag_ComboCount: nStarCount = pTargetConfig->GetStarCount(m_MarkInfo.GetComboCount());break;
            case ETargetFlag_Judge: nStarCount = pTargetConfig->GetStarCount(m_MarkInfo.Grade());break;
            default:break;
            }
        }

        if (nStarCount == 0)
        {
            continue;
        }
    }

    return nStarCount;
}

int CEntityDungeonComponent::CalcRankMark(int nMusicMode, int nMusicLevel, int nRank, int &nSkillMark)
{
    nSkillMark = 0;
    int nMark = CGradeInfoManager::Instance().GetGradePercent(nRank);

    // 计算宠物技能额外增加的分数（Perfect点击的基础分）
    if (m_status.HasCastPetSkill() && nRank == RoomRoundRank_Perfect)
    {
        nSkillMark = m_status.GetSkillRankMark(nMusicMode, nMusicLevel, nRank);

        nMark += nSkillMark;
    }

    return nMark;
}

void CEntityDungeonComponent::SetRunningParcloseIndex( EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID )
{
    switch( eType )
    {
    case EDungeonType_Mainline :
        {
            m_nMainlineRunningIndex = nParcloseID;
        }
        break;
    case EDungeonType_Theme :
        {
            std::map<unsigned int,unsigned int>::iterator it = m_ThemeRunningIndex.find( nDungeonID );
            if ( it != m_ThemeRunningIndex.end() )
            {
                it->second = nParcloseID;
            }
        }
        break;
    }
}

void CEntityDungeonComponent::ParcloseCalcResult()
{
    // grade 
    CalcGrade();

    // reward result
    CalcReward();

    // sync to activeness
    if ( m_MarkInfo.IsLegalMark(m_pRoleRoom->Room()->PlayMusicMode()))
    {
        // 通知观察者
        CommonParam param;
        param.SetParam(std::string("activeness"), std::string("dungeon"));
        NotifyAll(param);

        param.Clear();
        param.SetParam(std::string("activeness"), std::string("dance_mode"));
        param.SetParam(std::string("dance_mode"), m_pParcloseRoom->PlayMusicMode());
        NotifyAll(param);
    }

    // log
    CDungeonMgr::Instance()._LogRoleDancing(GetRoleID(), CRoleDancingLog::EDancingAction_End,
        m_pRoleRoom->Room()->PlayScene(), m_pRoleRoom->Room()->TeamMode(), m_pRoleRoom->Room()->PlayMusicID(), m_pRoleRoom->Room()->PlayMusicMode(), 
        m_pRoleRoom->Room()->PlayMusicLevel(),m_pRoleRoom->Room()->DancerCount(), m_MarkInfo.Place(), m_MarkInfo.Mark());
   
    // clear
    m_pRoleRoom->OutRoom();
    m_eRoleState = RoleRoomState_None;
    m_pParcloseRoom = NULL;
}

void CEntityDungeonComponent::CalcTarget( unsigned int nParcloseID, std::vector<CParcloseEndTarget> & targets, bool & nIsSuc )
{
    CParcloseConfig * pParcloseConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetParcloseConfig(m_pParcloseRoom->GetDungeonType(), m_pParcloseRoom->GetDungeonID(), m_pParcloseRoom->GetParcloseID());
    if ( pParcloseConfig == NULL)
    {
        return ;
    }

    bool bISSuccess = true;
    int nHostScore = m_pParcloseRoom->GetDancerScoreByPos(DUNGEON_HOST_POS);
    int nFriendScore = m_pParcloseRoom->GetDancerScoreByPos(DUNGEON_FRIEND_POS);
    std::vector<unsigned int>::iterator itt = pParcloseConfig->m_Targets.begin();
    for ( ; itt != pParcloseConfig->m_Targets.end(); ++itt )
    {
        CTargetConfig * pTargetConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetTargetConfig( *itt );
        if ( pTargetConfig == NULL )
        {
            continue;
        }

        int nSucValue = 0;
        if ( pTargetConfig->m_nFlag == ETF_TOTALMARK )
        {
            if ( nFriendScore != -1)
            {
                if ( nHostScore == -1)
                {
                    nSucValue =  nFriendScore ;
                }
                else
                {
                    nSucValue = nFriendScore + nHostScore;
                }
            }
            else
            {
                nSucValue = nHostScore;
            }
            nSucValue = nSucValue >= 0 ? nSucValue : 0; 
        }
        else
        {
            nSucValue = m_MarkInfo.GetTargetValue( (eTargetFlag)pTargetConfig->m_nFlag );	
        }

        int nNeedValue = atoi( pTargetConfig->m_strValue.c_str() );
        if ( nNeedValue == 0 && nSucValue == 0 )
        {
            bISSuccess = false;
        }

        if ( nNeedValue > 0 && nNeedValue > nSucValue )
        {
            bISSuccess = false;
        }

        CParcloseEndTarget targetmsg;
        targetmsg.m_IsAchieve = bISSuccess;
        char szDescribe[64] = {0};
        char szNeedValue[64] = {0};
        char szPlayerValue[64] = {0};

        if ( pTargetConfig->m_nFlag == ETF_GRADE )
        {
            string strValue = "";
            switch( nSucValue )
            {
            case  RoomDanceGrade_SS:strValue= "SS";break;
            case  RoomDanceGrade_S:strValue= "S";break;
            case  RoomDanceGrade_A:strValue= "A";break;
            case  RoomDanceGrade_B:strValue= "B";break;
            case  RoomDanceGrade_C:strValue= "C";break;
            case  RoomDanceGrade_D:strValue= "D";break;
            }
            sprintf( szPlayerValue, "%s", strValue.c_str() );


            switch( nNeedValue )
            {
            case  RoomDanceGrade_SS:strValue= "SS";break;
            case  RoomDanceGrade_S:strValue= "S";break;
            case  RoomDanceGrade_A:strValue= "A";break;
            case  RoomDanceGrade_B:strValue= "B";break;
            case  RoomDanceGrade_C:strValue= "C";break;
            case  RoomDanceGrade_D:strValue= "D";break;
            }
            sprintf( szNeedValue, "%s", strValue.c_str() );

            sprintf( szDescribe, pTargetConfig->m_strDesc.c_str(), strValue.c_str());
        }
        else if ( pTargetConfig->m_nFlag == ETF_FRIEND )
        {
            if ( nSucValue > 0 )
            {
                sprintf( szPlayerValue, "%s", "1" );
            }
            else
            {
                sprintf( szPlayerValue, "%s", "0" );

            }
            sprintf( szNeedValue, "%s", "1" );

            sprintf( szDescribe, pTargetConfig->m_strDesc.c_str(), pTargetConfig->m_strValue.c_str());
        }
        else if ( pTargetConfig->m_nFlag == ETF_LOVER )
        {
            if ( nSucValue > 0 )
            {
                sprintf( szPlayerValue, "%s", "1" );
            }
            else
            {
                sprintf( szPlayerValue, "%s", "0" );

            }
            sprintf( szNeedValue, "%s", "1" );

            sprintf( szDescribe, pTargetConfig->m_strDesc.c_str(), pTargetConfig->m_strValue.c_str());
        }
        else
        {
            sprintf( szPlayerValue, "%d", nSucValue );
            sprintf( szNeedValue, "%d", nNeedValue );


            char temp[64];
            sprintf( temp, "%d", nNeedValue );
            sprintf( szDescribe, pTargetConfig->m_strDesc.c_str(), temp );
        }

        targetmsg.m_strPlayerTarget = szPlayerValue;
        targetmsg.m_strNeedTarget	 = szNeedValue;
        targetmsg.m_strTargetDes	 = szDescribe;

        targets.push_back( targetmsg );
    }

    nIsSuc = bISSuccess;
}

void CEntityDungeonComponent::CalcParcloseReward(bool bSucess, EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID, ParcloseReward & parcloseReward)
{
    CParcloseConfig * pParcloseConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetParcloseConfig(eType, nDungeonID, nParcloseID);
    if (pParcloseConfig == NULL)
    {
        return;
    }

    // exp
    parcloseReward.m_nExp = CalcExp( true, pParcloseConfig->m_nMusicMode, pParcloseConfig->m_nMusicLevel);
    m_pRoleAttr->AddAbsExp( parcloseReward.m_nExp, CRoleExpLog::EAddExpCause_Match, 0 );

    if (m_status.HasCastPetSkill())
    {
        m_pRoleAttr->ChangeMoney(m_status.GetSkillRewardMoney(), EChangeMoneyCause_Add_PetSkillRoomReward, 0);
    }

    if (bSucess)
    {
        // drop Item
        CDungeonDataMgr::instance().CalcParcloseDrop(m_pRoleAttr->GetSex(), eType, nDungeonID, nParcloseID, parcloseReward.m_listItem, parcloseReward.m_listDebris);

        const CMagicLampRewardMail &  mail = CDungeonDataMgr::instance().GetRewardMail();
        CItemProcess::AddOrMailItems(*(CRoleEntity *)Entity(), parcloseReward.m_listItem, EItemAction_Add_Parclose, 0, true, EMailType_MagicLamp_ParcloseReward, mail.m_strParcloseDropTitle, mail.m_strParcloseDropContent );

        // 宠物额外奖励
        if (m_status.HasCastPetSkill())
        {
            int nDropRatio = m_status.GetSkillRewardItemRatio();
            while (nDropRatio > 0)
            {
                CItemProcess::AddOrMailItems(*(CRoleEntity *)Entity(), parcloseReward.m_listItem, EItemAction_Add_Parclose, 0, true, EMailType_MagicLamp_ParcloseReward, mail.m_strParcloseDropTitle, mail.m_strParcloseDropContent );
                nDropRatio--;
            }
        }

        // 保存碎片
        std::list<DebrisReward>::iterator itDebris = parcloseReward.m_listDebris.begin();
        for ( ; itDebris != parcloseReward.m_listDebris.end(); ++itDebris )
        {
            AddDebris(itDebris->nDebrisID, itDebris->nDebrisCount);

            unsigned int nLeftDebrisCount = GetDebrisCount( itDebris->nDebrisID );
            int nIsMainline = eType == EDungeonType_Mainline ? 1: 0;
            SaveDebrisLog( CRoleDungeonDebrisLog::EDebrisAction_Get, nIsMainline,itDebris->nDebrisID, itDebris->nDebrisCount, nDungeonID, nParcloseID, nLeftDebrisCount);
        }
    }

    // 宠物增加经验
    // 成功，则增加"评级经验+成功经验"，否则增加"评级经验+失败经验"
    // 具体实现，见CalcExp()
    m_pRolePet->AddExp(m_pRolePet->GetCarriedPetID(), (unsigned int)m_status.GetBonusExp());
}

void CEntityDungeonComponent::CalcReward()
{
	if (m_pParcloseRoom == NULL)
	{
		return;
	}

    bool bSucess = true;
    unsigned int nCurrentParcloseID = m_pParcloseRoom->GetParcloseID();
    unsigned int nNextParcloseID = ConfigManager::Instance().GetMagicLampConfigMgr().GetNextParcloseID( m_pParcloseRoom->GetDungeonType(), m_pParcloseRoom->GetDungeonID(), nCurrentParcloseID );
  
    GameMsg_S2C_ParcloseEnd endMsg;
    m_pParcloseRoom->SyncRoomEndScore( endMsg.m_listPlayerScore );
    CalcTarget( nCurrentParcloseID, endMsg.m_Targets, endMsg.m_bSucess );

    CParcloseConfig * pParcloseConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetParcloseConfig( m_pParcloseRoom->GetDungeonType(), m_pParcloseRoom->GetDungeonID(), nCurrentParcloseID );
    if ( pParcloseConfig != NULL )
    {
        if ( ParcloseIsActived( m_pParcloseRoom->GetDungeonType(), m_pParcloseRoom->GetDungeonID(), nCurrentParcloseID))
        {
            endMsg.m_bHaveStory = false;
        }
        else
        {
            endMsg.m_bHaveStory = pParcloseConfig->m_nEndStoryID > 0 ? true : false;
        }
    }

    bSucess = endMsg.m_bSucess;
    if ( bSucess )
    {
        endMsg.m_nNextDungeonID = ConfigManager::Instance().GetMagicLampConfigMgr().GetDungeonIDByParcloseID( nNextParcloseID );
    }
    else
    {
        endMsg.m_nNextDungeonID = ConfigManager::Instance().GetMagicLampConfigMgr().GetDungeonIDByParcloseID( nCurrentParcloseID );
    }
    endMsg.m_nLastParcloseID = nNextParcloseID;


    CalcParcloseReward(bSucess, m_pParcloseRoom->GetDungeonType(), m_pParcloseRoom->GetDungeonID(), m_pParcloseRoom->GetParcloseID(),
                        endMsg.m_ParcloseReward);

    if (bSucess && m_pParcloseRoom->IsHost(GetRoleID()))
    {
        // starCount
        endMsg.m_nStarCount = CalcStarCount(pParcloseConfig);

        // active parclose
        if ( nNextParcloseID != nCurrentParcloseID )
        {
            ActiveParclose( nNextParcloseID );

            if ( endMsg.m_nNextDungeonID > (int)m_pParcloseRoom->GetDungeonID() )
            {
                m_nServerActiveParcloseID = nNextParcloseID;
            }
        }

        // set starcount
        SetParcloseStarCount( m_pParcloseRoom->GetDungeonType(), m_pParcloseRoom->GetDungeonID(),nCurrentParcloseID, endMsg.m_nStarCount );

        // 当前进行副本
        SetRunningParcloseIndex( m_pParcloseRoom->GetDungeonType(), m_pParcloseRoom->GetDungeonID(), nCurrentParcloseID );
    }

    endMsg.m_nStarAllNum = GetLuckStar();
    m_pRoleNet->SendPlayerMsg( &endMsg );
}

int CEntityDungeonComponent::CalcExp(bool bIsWiner, int nMusicMode, int nMusicLevel)
{
    int nTotalExp = CDungeonDataMgr::instance().GetParcloseCalcExp(bIsWiner, m_MarkInfo.Grade(), nMusicLevel);
    if (m_pRolePet != NULL && m_pRolePet->HasCarriedPet())
    {
        int nExtraExpRatio = 0;
        if (m_status.HasCastPetSkill())
        {
            nExtraExpRatio = m_status.GetSkillRewardExpRatio(nMusicMode, nMusicLevel);
        }

        nTotalExp += nTotalExp * (int)((double)nExtraExpRatio / 100);

        int nGradeExp = CDungeonDataMgr::instance().GetDungeonGradeExp(m_MarkInfo.Grade(), nMusicLevel);
        int nPetDungeonDanceBaseExp = PetConfigManager::Instance().GetDanceDungeonExp( bIsWiner );
        m_status.SetBonusExp(nPetDungeonDanceBaseExp + nGradeExp);
    }

    return nTotalExp;
}

int CEntityDungeonComponent::_CheckParclose( EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID, const std::vector<itemtype_t> &items, unsigned int nFriendID )
{
    CParcloseConfig * pParcloseConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetMainlineParcloseConfig( nParcloseID );
    if ( pParcloseConfig == NULL)
    {
        return EDungeonError_NotFindParcloseID;
    }

    if ( !ParcloseIsActived( eType, nDungeonID, nParcloseID ))
    {
        return EDungeonError_ParcloseNotActived;
    }

    if ( (int)pParcloseConfig->m_nCostCrystal > GetCrystal() )
    {
        return EDungeonError_CrystalNotEnough;
    }

    std::vector<itemtype_t>::const_iterator it = items.begin();
    for ( ; it != items.end(); ++it )
    {
        if ( m_pRoleItem->GetItemCount( *it, false, false) < 1 )
        {
            return EDungeonError_ItemNotEnough;
        }
    }

    if ( nFriendID > 0 )
    {
        CRoleEntity * pFriend = CPlayerManager::Instance().GetEntityByRoleID( nFriendID );
        if( pFriend == NULL )
        {
            return EDungeonError_FriendNotOnLine;
        }

		if (nFriendID == m_pRoleAttr->GetRoleID())
		{
			return EDungeonError_NotFriend;
		}

        if (pFriend->GetRoleRoom()->IsInRoom())
        {
            return EDungeonError_InviteFriendTimeOut;// 先用好友忙代替好友在房间中
        }

        if ( pFriend->WeddingRoom() != NULL)
        {
            return EDungeonError_InviteFriendTimeOut;// 先用好友忙代替好友在房间中
        }

        if ( pFriend->PhotoRoom() != NULL) 
        {
            return EDungeonError_InviteFriendTimeOut;// 先用好友忙代替好友在房间中
        }

        if ( pFriend->AmuseRoom() != NULL)
        {
            return EDungeonError_InviteFriendTimeOut;// 先用好友忙代替好友在房间中
        }

        if (pFriend->GetCeremonyRoom() != NULL)
        {
            return EDungeonError_InCeremonyRoom;
        }

        CheckAndResetInviteInfo();
		if ( m_pRoleCouple->CoupleID() != nFriendID )
		{
            if ( m_InviteInfo.m_nCostInviteNum >= CDungeonDataMgr::instance().GetMaxInviteFriendNum() )
            {
                return EDungeonError_InviteFriendCountNot;
            }

            if ( IsInvited( nFriendID ))
            {
                return EDungeonError_FriendHasInvited;
            }
		}

        GameMsg_S2C_BeInviteToParclose inviteMsg;
        inviteMsg.m_eType = eType;
        inviteMsg.m_nDungeonID = nDungeonID;
        inviteMsg.m_nParcloseID = nParcloseID;
        inviteMsg.m_nInviterID = m_pRoleAttr->GetRoleID();
        inviteMsg.m_strInviterName = m_pRoleAttr->GetRoleName();
        inviteMsg.m_nIsVip = m_pRoleVip->IsVIP();
        inviteMsg.m_nVipLv = m_pRoleVip->VIPLevel();
        pFriend->SendPlayerMsg( &inviteMsg );

        // 定时器
        int nCurTime = (int)time(NULL);
        m_nInviteFriendTimer = nCurTime + INVITE_FRIEND_TIMER ;
    }

    return EDungeonError_Sucess;
}

void CEntityDungeonComponent::_HandleParcloseCost( unsigned int nParcloseID, const std::vector<itemtype_t> &items )
{
    CParcloseConfig * pParcloseConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetMainlineParcloseConfig( nParcloseID );
    if ( pParcloseConfig != NULL )
    {
        unsigned int nMaxValue = CDungeonDataMgr::instance().GetMaxCrystalValue();
        if ( GetCrystal() == (int)nMaxValue && GetCrystalRestoreTime() != -1 )
        {
            int nCurTime = (int)time(NULL);
            SetCrystalRestoreTime( nCurTime );

            SyncCrystalToClient();
        }

        SetCrystal((-1)*pParcloseConfig->m_nCostCrystal);

        // log
        InsertActionLog(CRoleDungeonLog::EDungeonAction_UseCrystal, m_pRoleAttr->GetRoleID(), 0, 0 , 0, pParcloseConfig->m_nCostCrystal);

        // 产生魔法尘埃
        _CreateMagicDust();

        // 物品效果
        std::vector<itemtype_t>::const_iterator it = items.begin();
        for ( ; it != items.end(); ++it )
        {
            itemtype_t nItemID = *it;
            switch( nItemID )
            {
            case ITEM_PARCLOSEEFFECT_ADDMARK:m_MarkInfo.AddMarkGeneEffect( ITEM_PARCLOSEEFFECT_RATE );break;
            case ITEM_PARCLOSEEFFECT_MISSTOCOOL:m_MarkInfo.AddMissToCoolGeneEffect( ITEM_PARCLOSEEFFECT_RATE );break;
            case ITEM_PARCLOSEEFFECT_COOLTOPERFECT:m_MarkInfo.AddCoolToPerfectGeneEffect( ITEM_PARCLOSEEFFECT_RATE );break;
            }

            if ( !m_pRoleItem->RemoveItemByType( nItemID, 1, EItemAction_Del_Parclose))
            {
                WriteLog(LOGLEVEL_ERROR, "删除物品失败%u\n", m_pRoleAttr->GetRoleID() );
            }
        }
    }
}

void CEntityDungeonComponent::_CreateMagicDust()
{
    const CCrystalConfig & crytalConfig = CDungeonDataMgr::instance().getCrystalConfig();
    if ( crytalConfig.m_nMinDustCount > 0 && crytalConfig.m_nMaxDustCount >= crytalConfig.m_nMinDustCount )
    {
        int nValue = Random( crytalConfig.m_nMinDustCount, crytalConfig.m_nMaxDustCount );
        SetMagicDust( nValue );

        WriteLog(LOGLEVEL_DEBUG, "Debug:MagicLampDust: roleid:%u_Add_Dust:%d", m_pRoleAttr->GetRoleID(), m_MagicLamp.m_nMagicDust );
    }
}

void CEntityDungeonComponent::_SendStartParcloseErrorMsg( int nErr )
{
    GameMsg_S2C_StartParcloseFail msgFail;

    switch( nErr)
    {
    case  EDungeonError_NotFindParcloseID :
        {
            msgFail.m_strError = "EDungeonError_NotFindParcloseID";
            msgFail.m_nErrFlag = ESPE_NotFindParcloseID;
        }
        break;
    case  EDungeonError_ParcloseNotActived :
        {
            msgFail.m_strError = "EDungeonError_ParcloseNotActived";
            msgFail.m_nErrFlag = ESPE_ParcloseNotActived;
        }
        break;
    case  EDungeonError_ItemNotEnough :
        {
            msgFail.m_strError = "EDungeonError_ItemNotEnough";
            msgFail.m_nErrFlag = ESPE_ItemNotEnough;
        }
        break;
    case  EDungeonError_NotFriend :
        {
            msgFail.m_strError = "EDungeonError_NotFriend";
            msgFail.m_nErrFlag = ESPE_NotFriend;
        }
        break;
    case  EDungeonError_InRoom :
        {
            msgFail.m_strError = "EDungeonError_InRoom";
            msgFail.m_nErrFlag = ESPE_InRoom;
        }
        break;
    case  EDungeonError_InCoupleRoom : 
        {
            msgFail.m_strError = "EDungeonError_InCoupleRoom";
            msgFail.m_nErrFlag = ESPE_InCoupleRoom;
        }
        break;
    case  EDungeonError_InPhoteRoom : 
        {
            msgFail.m_strError = "EDungeonError_InPhoteRoom";
            msgFail.m_nErrFlag = ESPE_InPhoteRoom;
        }
        break;
    case  EDungeonError_InAmuseRoom : 
        {
            msgFail.m_strError = "EDungeonError_InAmuseRoom";
            msgFail.m_nErrFlag = ESPE_InAmuseRoom;
        }
        break;
    case  EDungeonError_DungeonCreateNew : 
        {
            msgFail.m_strError = "EDungeonError_DungeonCreateNew";
            msgFail.m_nErrFlag = ESPN_NewRoomErr;
        }
        break;
    case  EDungeonError_FriendNotOnLine :
        {
            msgFail.m_strError = "EDungeonError_FriendNotOnLine";
            msgFail.m_nErrFlag = ESPN_FrindNotOnline;
        }
        break;
    case  EDungeonError_FriendRefuse :
        {
            msgFail.m_strError = "EDungeonError_FriendRefuse";
            msgFail.m_nErrFlag = ESPN_FriendRefuse;
        }
        break;
    case  EDungeonError_InviteFriendTimeOut :
        {
            msgFail.m_strError = "EDungeonError_InviteFriendTimeOut";
            msgFail.m_nErrFlag = ESPN_InviteFriendTimeOut;
        }
        break;
    }

    m_pRoleNet->SendPlayerMsg( &msgFail );
}

void CEntityDungeonComponent::SetRoomInfo( CDungeon * pRoom, ERoleRoomState eState )
{
    m_MarkInfo.ClearMark();

    m_pParcloseRoom = pRoom;
    m_eRoleState = eState;
}

CDungeon* CEntityDungeonComponent::GetDungeon() const
{
    return m_pParcloseRoom;
}

void CEntityDungeonComponent::ClearRoomInfo()
{
    m_pParcloseRoom = NULL;
    m_eRoleState = RoleRoomState_None;
}

void CEntityDungeonComponent::SetCrystal( int nCrystal, bool bSave)
{
    if ( nCrystal == 0 )
    {
        return;
    }

    if ( m_MagicLamp.m_nCrytsal + nCrystal < 0 )
    {
        return;
    }

    int nMaxValue = (int)CDungeonDataMgr::instance().GetMaxCrystalValue();
    if (m_MagicLamp.m_nCrytsal >= nMaxValue && nCrystal > 0)
    {
        return; 
    }

    m_MagicLamp.m_nCrytsal += nCrystal;

    if (bSave)
    {
        SyncMagicToDB(ESyncToDBType_Update);
    }
}

void CEntityDungeonComponent::SetCrystalRestoreTime(int nTime, bool bSave)
{
    m_MagicLamp.m_nCrystalRestoreTime = nTime;
	if (bSave)
	{
		SyncMagicToDB(ESyncToDBType_Update);
	}
}

void CEntityDungeonComponent::CrystalReply(unsigned int nNowTime, bool bSyncToClient)
{
    int nMaxCrystal = (int)CDungeonDataMgr::instance().GetMaxCrystalValue();
    int nCurCrystalTime = GetCrystalRestoreTime();
    if (GetCrystal() < nMaxCrystal && nCurCrystalTime > 0)
    {
        unsigned int nRestoreTimeConfig = CDungeonDataMgr::instance().GetCrystalRestoreTime();
        unsigned int nPassTime = nNowTime - nCurCrystalTime;
        if ( nPassTime > 0 && nPassTime >= nRestoreTimeConfig )
        {
            int nAddCrystal = nPassTime/nRestoreTimeConfig;

            if (nAddCrystal + GetCrystal() > nMaxCrystal)
            {
                nAddCrystal = nMaxCrystal - GetCrystal();
            }

            SetCrystal(nAddCrystal, false);
            SetCrystalRestoreTime(nNowTime, false);

            if (bSyncToClient)
            {
                SyncCrystalToClient();
            }
        }
    }
}

void CEntityDungeonComponent::SyncCrystalToClient()
{
    int nRestoreTimeConfig = (int)CDungeonDataMgr::instance().GetCrystalRestoreTime();
    int nNowTime = (int)time(NULL);
    int nLeftTime = nRestoreTimeConfig - (nNowTime - GetCrystalRestoreTime());

    if (nLeftTime >= 0)
    {
        GameMsg_S2C_UpdateCrystal updateMsg;
        updateMsg.m_nCrystal = GetCrystal();
        updateMsg.m_nTime = nLeftTime;
        m_pRoleNet->SendPlayerMsg( &updateMsg );
    }
}

void CEntityDungeonComponent::SetCrystalInitialValue( int nCrystal, unsigned int nCurTime  )
{
    if ( nCrystal == 0 )
    {
        return;
    }

    if ( m_MagicLamp.m_nCrytsal + nCrystal < 0 )
    {
        return;
    }

    m_MagicLamp.m_nCrytsal += nCrystal;
    m_MagicLamp.m_nCrystalRestoreTime = nCurTime;
    SyncMagicToDB(ESyncToDBType_Insert);
}

int CEntityDungeonComponent::GetCrystal() const
{
    return m_MagicLamp.m_nCrytsal;
}

void CEntityDungeonComponent::SetMagicDust( int nDust )
{
    if ( nDust == 0 )
    {
        return;
    }

    if ( m_MagicLamp.m_nMagicDust + nDust < 0 )
    {
        return;
    }

    m_MagicLamp.m_nMagicDust += nDust;
    SyncMagicToDB(ESyncToDBType_Update);
}

unsigned int CEntityDungeonComponent::GetLuckStar()
{
    int nAllStarCount = 0;
    MapMainlineParcloseRecord::iterator it = m_MainlineParcloseRecord.begin();
    for ( ; it != m_MainlineParcloseRecord.end(); ++it )
    {
        nAllStarCount += it->second.nStarCount;
    }

    return nAllStarCount;
}

void CEntityDungeonComponent::UpdateLuckStar()
{
    unsigned int nLuckStar = GetLuckStar();
    m_MagicLamp.m_nLuckStar = nLuckStar;
    SyncMagicToDB(ESyncToDBType_Update);

    // sync group
    GameMsg_S2G_UpdateParcloseInfo syncMsg;
    syncMsg.m_nRoleID = m_pRoleAttr->GetRoleID();
    syncMsg.m_nParcloseID = GetTopActivedParcloseID();
    syncMsg.m_nStarCount = nLuckStar;
    ::SendMsg2GroupServer(&syncMsg);
}

int CEntityDungeonComponent::GetRoomPos()
{
    if ( m_pParcloseRoom != NULL )
    {
        return m_pParcloseRoom->GetPos( m_pRoleAttr->GetRoleID() );
    }

    return -1;
}

void CEntityDungeonComponent::OnStartDance()
{
    m_status.Clear();
    if (m_pParcloseRoom != NULL)
    {
        bool bCostEndurance = m_pRolePet->CastPetDanceSkill(m_status.SkillMap());
        m_status.CostEndurance(bCostEndurance);
    }
}

void CEntityDungeonComponent::LogRoleCheckingMarkFailed()
{
    if ( m_pParcloseRoom == NULL || m_pRoleAttr == NULL )
        return;

    const CheckingMarkFailedInfo &rInfo = m_MarkInfo.GetCheckingMarkFailedInfo();

    if ( !rInfo.HasFailed() )
        return;

    CRoleCheckingMarkFailedLog *pLog = new CRoleCheckingMarkFailedLog;

    pLog->m_nRoleID = m_pRoleAttr->GetRoleID();
    pLog->m_nRoomType = (short)m_pParcloseRoom->RoomType();
    pLog->m_nScene = m_pParcloseRoom->PlayScene();
    pLog->m_nTeamMode = (short)m_pParcloseRoom->TeamMode();
    pLog->m_nMusicID = m_pParcloseRoom->PlayMusicID();
    pLog->m_nMusicMode = m_pParcloseRoom->PlayMusicMode();
    pLog->m_nMusicLevel = m_pParcloseRoom->PlayMusicLevel();
    pLog->m_nPlayerCount = m_pParcloseRoom->DancerCount();
    pLog->m_nStartTime = m_pParcloseRoom->StartTime();
    pLog->m_nRound = rInfo.GetRound();
    pLog->m_nKeyRank = rInfo.GetKeyRank();
    pLog->m_nLastSpecial = rInfo.GetLastSpecial();
    pLog->m_strRoundRank = rInfo.GetRoundRank();
    pLog->m_nMark = rInfo.GetMark();
    pLog->m_nCheckingMark = rInfo.GetCheckingMark();

    AddQuery( QUERY_LogRoleCheckingMarkFailed, 0, pLog );
}

void CEntityDungeonComponent::OnGetFriendContactInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2C_GetFriendContactInfoResult resultmsg;
    resultmsg.m_mapFriendContact = m_mapFriendContact;
    SendPlayerMsg(&resultmsg);
}

void CEntityDungeonComponent::OnGiveVit(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_GiveVitToFriend & c2smsg = (GameMsg_C2S_GiveVitToFriend&)msg;

    std::list<unsigned int> listOfflineFriend;
    std::list<unsigned int> listNotFriend;
    std::list<unsigned int> listScuessFriend;

    std::list<unsigned int>::iterator it = c2smsg.m_listFriendRoleID.begin();
    for (; it != c2smsg.m_listFriendRoleID.end(); ++it)
    {
        int nError = GiveVitToFriend(*it);
        if (nError == EDungeonError_NotFriend)
        {
            listNotFriend.push_back(*it);
        }
        else
        {
            if (nError == EDungeonError_PlayerNotOnLine)
            {
                listOfflineFriend.push_back(*it);
            }
            listScuessFriend.push_back(*it);
        }
    }

    if (!listOfflineFriend.empty())
    {
        GameMsg_S2G_GiveVitToFriend g2smsg;
        g2smsg.m_nGiveVitRoleID = m_pRoleAttr->GetRoleID();
        g2smsg.m_nVitNum = CDungeonDataMgr::instance().OnceGiveVitNum();
        g2smsg.m_listFriendRoleID = listOfflineFriend;

        ::SendMsg2GroupServer(&g2smsg);
    }

    GameMsg_S2C_GiveVitToFriendResult resultmsg;
    resultmsg.m_listSucessFriendRoleID = listScuessFriend;
    resultmsg.m_listFailFriendRoleID = listNotFriend;
    SendPlayerMsg(&resultmsg);
}

void CEntityDungeonComponent::_GiveVitToFriend(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_GiveVitToFriend g2smsg = (GameMsg_G2S_GiveVitToFriend&)msg;

    unsigned int nVitNum = CDungeonDataMgr::instance().OnceGiveVitNum();
    FriendGiveVit(g2smsg.m_nFriendRoleID, g2smsg.m_strFriendRoleName, nVitNum);
}

int CEntityDungeonComponent::GiveVitToFriend(unsigned int nFriendRoleID)
{
    // 是否是好友关系
    if (!m_pRoleFriend->IsFriend(nFriendRoleID))
    {
        return EDungeonError_NotFriend;
    }

    // 是否今天赠送过
    unsigned int nGiveVitNum = CDungeonDataMgr::instance().OnceGiveVitNum();
    std::map<unsigned int,FriendContactInfo>::iterator itContact = m_mapFriendContact.find(nFriendRoleID);
    if (itContact != m_mapFriendContact.end())
    {
        if (itContact->second.m_nGiveVitTime != 0 || itContact->second.m_nGiveVitNum != 0)
        {
            return EDungeonError_FriendHasGiveVit;
        }

        itContact->second.m_nContactNum++;
        itContact->second.m_nGiveVitNum = nGiveVitNum;
        itContact->second.m_nGiveVitTime = (unsigned int)time(NULL);

        SyncContactToDB(itContact->second, ESyncToDBType_Update);
    }
    else
    {
        FriendContactInfo contactinfo;
        contactinfo.m_nFriendRoleID = nFriendRoleID;
        contactinfo.m_nContactNum++;
        contactinfo.m_nGiveVitNum = nGiveVitNum;
        contactinfo.m_nGiveVitTime = (unsigned int)time(NULL);
        m_mapFriendContact[nFriendRoleID] = contactinfo;

        SyncContactToDB(contactinfo, ESyncToDBType_Insert);
    }

    CRoleEntity * pFriendEntity = CPlayerManager::Instance().GetEntityByRoleID(nFriendRoleID);
    if (pFriendEntity != NULL)
    {
        std::string strFriendRoleName = m_pRoleAttr->GetRoleName();
        pFriendEntity->GetComponent<CRoleDungeon>()->FriendGiveVit(m_pRoleAttr->GetRoleID(), strFriendRoleName, nGiveVitNum);
    }
    else
    {
        return EDungeonError_PlayerNotOnLine;
    }

    return EDungeonError_Sucess;
}

void CEntityDungeonComponent::FriendGiveVit(unsigned int nFriendRoleID, std::string & strFriendRoleName, unsigned int nGiveVitNum)
{
    std::map<unsigned int,FriendContactInfo>::iterator it = m_mapFriendContact.find(nFriendRoleID);
    if (it != m_mapFriendContact.end())
    {
        it->second.m_nContactNum++;
        it->second.m_nBeGiveVitNum = nGiveVitNum;
        it->second.m_nBeGiveVitTime = (unsigned int)time(NULL);

        SyncContactToDB(it->second, ESyncToDBType_Update);
    }
    else
    {
        FriendContactInfo friendcontact;
        friendcontact.m_nContactNum++;
        friendcontact.m_nFriendRoleID = nFriendRoleID;
        friendcontact.m_nBeGiveVitNum = nGiveVitNum;
        friendcontact.m_nBeGiveVitTime = (unsigned int)time(NULL);
        m_mapFriendContact[nFriendRoleID] = friendcontact;

        SyncContactToDB(friendcontact, ESyncToDBType_Insert);
    }

    // sync to client
    GameMsg_S2C_FriendNoticeByGiveVit notice;
    notice.m_nGiveVitRoleID = nFriendRoleID;
    notice.m_strGiveVitRoleName = strFriendRoleName;
    notice.m_nGiveVitNum = nGiveVitNum;
    SendPlayerMsg(&notice);
}

void CEntityDungeonComponent::SetTodayGetVitNum(int nAddNum)
{
    if (m_MagicLamp.m_nTodayHaveGetVitNum + nAddNum >= 0)
    {
        m_MagicLamp.m_nTodayHaveGetVitNum += nAddNum;
    }
}

int CEntityDungeonComponent::GetVipBuyVitCount()
{
    return m_MagicLamp.m_nFavorableBuyVitCount;
}

void CEntityDungeonComponent::AddTodayVipBuyVitNum(int nAddNum)
{
    if (m_MagicLamp.m_nFavorableBuyVitCount + nAddNum < 0)
    {
        return;
    }

    m_MagicLamp.m_nFavorableBuyVitCount += nAddNum;
}

int CEntityDungeonComponent::GetTodayGetVitNum()
{
    return m_MagicLamp.m_nTodayHaveGetVitNum;
}

void CEntityDungeonComponent::ResetTodayGetVitNum()
{
    m_MagicLamp.m_nTodayHaveGetVitNum = 0;
}

void CEntityDungeonComponent::OnGetVit(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_GetVitFromFriend & c2smsg = (GameMsg_C2S_GetVitFromFriend&)msg;

    std::list<unsigned int> listGetScuessID;
    std::map<unsigned int, unsigned short> listGetFailID;
    std::list<unsigned int>::iterator it = c2smsg.m_listFriendRoleID.begin();
    for (; it != c2smsg.m_listFriendRoleID.end(); ++it)
    {
        int nError = GetVitFromFriend(*it);
        if (nError == EDungeonError_Sucess)
        {
            listGetScuessID.push_back(*it);
        }
        else
        {
            listGetFailID.insert(make_pair(*it, (unsigned short)nError));
        }
    }

    GameMsg_S2C_GetVitFromFriendResult resultmsg;
    resultmsg.m_listScuessFriendRoleID = listGetScuessID;
    resultmsg.m_mapFailFriend = listGetFailID;
    SendPlayerMsg(&resultmsg);
}

void CEntityDungeonComponent::SyncContactToDB(const FriendContactInfo & contactinfo, ESyncToDBType eType)
{
    FriendContactInfo * pContactInfo = new FriendContactInfo(contactinfo);

    switch(eType)
    {
    case ESyncToDBType_Insert:
        {
            CDelHelper delHelper(pContactInfo);
            AddQuery(Query_Insert_FriendContactInfo, m_pRoleAttr->GetRoleID(), pContactInfo, 0, NULL, &delHelper);
        }
        break;
    case ESyncToDBType_Update:
        {
            CDelHelper delHelper(pContactInfo);
            AddQuery(Query_Update_FriendContactInfo, m_pRoleAttr->GetRoleID(), pContactInfo, 0, NULL, &delHelper);
        }
        break;
    case ESyncToDBType_None:
    default:break;
    }
}

void CEntityDungeonComponent::SyncMagicToDB(ESyncToDBType eType)
{
    MagicLamp * pMagicLamp = new MagicLamp(m_MagicLamp);
    switch(eType)
    {
    case ESyncToDBType_Insert:
        {
            CDelHelper delHelper(pMagicLamp);
            AddQuery( Query_Insert_MagicLampInfo, m_pRoleAttr->GetRoleID(), pMagicLamp);
        }
        break;
    case ESyncToDBType_Update:
        {
            CDelHelper delHelper(pMagicLamp);
            AddQuery( Query_Update_MagicLampInfo, m_pRoleAttr->GetRoleID(), pMagicLamp);
        }
        break;
    case ESyncToDBType_None:break;
    default:break;
    }
}

int CEntityDungeonComponent::GetVitFromFriend(unsigned int nFriendRoleID)
{
    std::map<unsigned int,FriendContactInfo>::iterator it = m_mapFriendContact.find(nFriendRoleID);
    if (it == m_mapFriendContact.end())
    {
        return EDungeonError_NotVitGet;
    }

    if (it->second.m_nBeGiveVitNum == 0)
    {
        return EDungeonError_NotVitGet;
    }

    if (GetCrystal() >= (int)CDungeonDataMgr::instance().GetMaxCrystalValue())
    {
        return EDungeonError_CrystalIsFull;
    }

    if(GetTodayGetVitNum() >= (int)CDungeonDataMgr::instance().GetDailyGetVitMaxNum())
    {
        return EDungeonError_TodayGetVitMax;
    }

    SetCrystal((int)it->second.m_nBeGiveVitNum, false);
    SetTodayGetVitNum((int)it->second.m_nBeGiveVitNum);
    SyncMagicToDB(ESyncToDBType_Update);

    it->second.m_nBeGiveVitNum = 0;
    it->second.m_nBeGiveVitTime = 0;
    SyncContactToDB(it->second, ESyncToDBType_Update);

    return EDungeonError_Sucess;
}

void CEntityDungeonComponent::OnQuickFinishParclose(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_QuickFinishParclose & c2smsg = (GameMsg_C2S_QuickFinishParclose&)msg;
    int nError = QuickFinishParclose(c2smsg.m_bIsMainline, c2smsg.m_nDungeonID, c2smsg.m_nParcloseID);
    if (nError != EDungeonError_Sucess)
    {
        GameMsg_S2C_QuickFinishParcloseFail failmsg;
        failmsg.m_nError = nError;
        SendPlayerMsg(&failmsg);
    }
}

int CEntityDungeonComponent::QuickFinishParclose(bool bIsMainline, unsigned int nDungeonID, unsigned int nParcloseID)
{
    EDungeonType eDungeonType = bIsMainline ? EDungeonType_Mainline : EDungeonType_Theme ;
    CParcloseConfig * pParcloseConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetParcloseConfig(eDungeonType, nDungeonID, nParcloseID);
    if (pParcloseConfig == NULL)
    {
        return EDungeonError_NotFindParcloseID;
    }

    // 关卡能否扫荡
    if (bIsMainline)
    {
        if (GetParcloseStarCount(eDungeonType, nDungeonID, nParcloseID) < 3)
        {
            return EDungeonError_NotHave3Star;
        }
    }
    else
    {
        if (GetParcloseStarCount(eDungeonType, nDungeonID, nParcloseID) <= 0)
        {
            return EDungeonError_ParcloseNotPassed;
        }
    }

    if (!CDungeonDataMgr::instance().QucikFinishOpenToNotVip() && !m_pRoleVip->IsVIP())
    {
        return EDungeonError_NotVip;
    }

    // 关卡扫荡次数
    unsigned int nQuickFinishNum = GetParcloseQuickFinishNum(eDungeonType, nDungeonID, nParcloseID);
    if (nQuickFinishNum >= pParcloseConfig->m_nDailyQuickFinishNum)
    {
        return EDungeonError_NotQuickFinishNum;
    }

    // 扫荡券
    CItem costItem = CDungeonDataMgr::instance().GetQuickFinishItem();
    if (m_pRoleItem->GetItemCount(costItem.m_nItemType, false, false) < costItem.m_nItemCount)
    {
        return EDungeonError_ItemNotEnough;
    }

    // 体力
    if (m_MagicLamp.m_nCrytsal < (int)pParcloseConfig->m_nCostCrystal)
    {
        return EDungeonError_CrystalNotEnough;
    }

    // 加奖励
    ParcloseReward parcloseReward;
    CalcParcloseReward(true, eDungeonType, nDungeonID, nParcloseID, parcloseReward);

    // 扣除体力
    SetCrystal((-1)*pParcloseConfig->m_nCostCrystal);
    SyncCrystalToClient();

    // 扣除扫荡卡
    m_pRoleItem->RemoveItemByType(costItem.m_nItemType, costItem.m_nItemCount, EItemAction_Del_QuickFinishParclose);

    // 更新扫荡次数
    UpdateQuickFinishNum(eDungeonType, nDungeonID, nParcloseID, nQuickFinishNum + 1);

    // snyc to client
    GameMsg_S2C_QuickFinishParcloseSucess sucessmsg;
    sucessmsg.m_ParcloseReward = parcloseReward;
    SendPlayerMsg(&sucessmsg);

    return EDungeonError_Sucess;
}

void CEntityDungeonComponent::UpdateQuickFinishNum(EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID, unsigned int nNum)
{
    switch (eType)
    {
    case EDungeonType_Mainline:
        {
            MapMainlineParcloseRecord::iterator it = m_MainlineParcloseRecord.find(nParcloseID);
            if (it != m_MainlineParcloseRecord.end())
            {
                it->second.nQuickFinishNum = nNum;
                it->second.nUpdateTime = (unsigned int)time(NULL);

                MainlineParcloseRecord * pRecord = new MainlineParcloseRecord( it->second );
                if ( pRecord != NULL )
                {
                    AddQuery( Query_Update_ParcloseInfo, m_pRoleAttr->GetRoleID(), pRecord );
                }
            }
        }
        break;
    case EDungeonType_Theme:
        {
            MapThemeParcloseRecord::iterator it = m_ThemeParcloseRecord.find(nDungeonID);
            if (it != m_ThemeParcloseRecord.end())
            {
                std::map<unsigned int,ThemeParcloseRecord>::iterator iter = it->second.find(nParcloseID);
                if (iter != it->second.end())
                {
                    iter->second.nQuickFinishNum = nNum;
                    iter->second.nUpdateTime = (unsigned int)time(NULL);

                    ThemeParcloseRecord * pRecord = new ThemeParcloseRecord( iter->second );
                    if ( pRecord != NULL )
                    {
                        AddQuery( Query_Update_ThemeParcloseInfo, m_pRoleAttr->GetRoleID(), pRecord );
                    }
                }
            }
        }
        break;
    }
}

unsigned int CEntityDungeonComponent::GetParcloseQuickFinishNum(EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID)
{
    switch (eType)
    {
    case EDungeonType_Mainline:
        {
            MapMainlineParcloseRecord::iterator it = m_MainlineParcloseRecord.find(nParcloseID);
            if (it != m_MainlineParcloseRecord.end())
            {
                return it->second.nQuickFinishNum;
            }
        }
        break;
    case EDungeonType_Theme:
        {
            MapThemeParcloseRecord::iterator it = m_ThemeParcloseRecord.find(nDungeonID);
            if (it != m_ThemeParcloseRecord.end())
            {
                std::map<unsigned int,ThemeParcloseRecord>::iterator iter = it->second.find(nParcloseID);
                if (iter != it->second.end())
                {
                    return iter->second.nQuickFinishNum;
                }
            }
        }
        break;
    }

    return 0;
}

void CEntityDungeonComponent::CheckNextDayResetTime(unsigned int nNowTime)
{
    if ((unsigned int)m_MagicLamp.m_nNextDayResetTime > nNowTime)
    {
        return;
    }

    if (m_MagicLamp.m_nNextDayResetTime == 0)
    {
        m_MagicLamp.m_nNextDayResetTime = nNowTime;
    }

    unsigned int nResetSecond = CDungeonDataMgr::instance().GetNextDayResetTime();
    time_t nResetTime = GetTodayStartTime(nNowTime) + (time_t)nResetSecond;
    if (IsSameDayAndSecond(nNowTime, nResetTime) || DiffDays(nNowTime, m_MagicLamp.m_nNextDayResetTime, nResetSecond) >= 1)
    {
        // reset give get vit
        std::map<unsigned int,FriendContactInfo>::iterator it = m_mapFriendContact.begin();
        for (; it != m_mapFriendContact.end(); ++it)
        {
            if (it->second.m_nBeGiveVitNum > 0 || it->second.m_nGiveVitNum > 0)
            {
                it->second.m_nGiveVitTime = 0;
                it->second.m_nGiveVitNum = 0;
                it->second.m_nBeGiveVitNum = 0;
                it->second.m_nBeGiveVitTime = 0;
            }
        }
        AddQuery(Query_Update_ClearGiveGetVitInfo, m_pRoleAttr->GetRoleID());

        // reset quickfinish num
        MapMainlineParcloseRecord::iterator itMainline = m_MainlineParcloseRecord.begin();
        for (; itMainline != m_MainlineParcloseRecord.end(); ++itMainline)
        {
            itMainline->second.nQuickFinishNum = 0;
        }
        AddQuery(Query_Update_ClearMailineQuickFinishNum, m_pRoleAttr->GetRoleID());

        MapThemeParcloseRecord::iterator itTheme = m_ThemeParcloseRecord.begin();
        for (; itTheme != m_ThemeParcloseRecord.end(); ++itTheme)
        {
            std::map<unsigned int,ThemeParcloseRecord>::iterator iter = itTheme->second.begin();
            for (;iter != itTheme->second.end(); ++iter)
            {
                iter->second.nQuickFinishNum = 0;
            }
        }
        AddQuery(Query_Update_ClearThemeQuickFinishNum, m_pRoleAttr->GetRoleID());

        // reset time
        m_MagicLamp.m_nNextDayResetTime = nNowTime;

        // reset vip buy vit
        m_MagicLamp.m_nFavorableBuyVitCount = 0;

        // reset get vit num
        ResetTodayGetVitNum();

        // update to db
        SyncMagicToDB(ESyncToDBType_Update);
    }
}

void CEntityDungeonComponent::CheckGiveGetVitDueTime()
{
    time_t nTodayStartTime = GetTodayStartTime(time(NULL));
    unsigned int nResetTime = CDungeonDataMgr::instance().GetNextDayResetTime();
    unsigned int nLastGiveGetVitTime = (unsigned int)time(NULL);

    std::map<unsigned int,FriendContactInfo>::iterator it = m_mapFriendContact.begin();
    for (; it != m_mapFriendContact.end(); ++it)
    {
        if (it->second.m_nGiveVitTime < nLastGiveGetVitTime)
        {
            nLastGiveGetVitTime = it->second.m_nGiveVitTime;
        }

        if (it->second.m_nBeGiveVitTime < nLastGiveGetVitTime)
        {
            nLastGiveGetVitTime = it->second.m_nBeGiveVitTime;
        }
    }

    if (nTodayStartTime + nResetTime - nLastGiveGetVitTime > 24*60*60 )
    {
        std::map<unsigned int,FriendContactInfo>::iterator it = m_mapFriendContact.begin();
        for (; it != m_mapFriendContact.end(); ++it)
        {
            if (it->second.m_nBeGiveVitNum > 0 || it->second.m_nGiveVitNum > 0)
            {
                it->second.m_nGiveVitTime = 0;
                it->second.m_nGiveVitNum = 0;
                it->second.m_nBeGiveVitNum = 0;
                it->second.m_nBeGiveVitTime = 0;
            }
        }

        AddQuery(Query_Update_ClearGiveGetVitInfo, m_pRoleAttr->GetRoleID());
    }
}

