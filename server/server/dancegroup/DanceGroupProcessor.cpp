#include "DanceGroupProcessor.h"
#include "../group/Global.h"

#include "../Rank/GlobalRankListManager.h"
#include "../mail/WordMailManager.h"

#include "../libServerFrame/QueryManager.h"
#include "DanceGroupMsgDef.h"

#include "GameMsg_C2S_DanceGroup.h"
#include "GameMsg_S2C_DanceGroup.h"
#include "GameMsg_S2G_DanceGroup.h"
#include "GameMsg_G2S_DanceGroup.h"

#include "../../socket/GameMsg_Map.h"
#include "../../socket/Formatter.h"
#include "../util/CommonFunc.h"
#include "../../datastructure/Localization.h"

#include <stdarg.h>
#include "../CeremonyRoom/CeremonyBriefManager.h"
#include "BigMamaGroup.h"
#include "../room/GroupRoomPrecessor.h"
#include "../roomLib/GameMsgRoomBroadcast.h"
#include "../DanceGroupFairyLand/DanceGroupFairyLandProcessor.h"

extern CQueryManager* g_pQueryMgr;

CDanceGroupProcessor::CDanceGroupProcessor(void)
    : GameMsg_Processor(true, true)
    , m_bDanceGroupComplete(false)
{
}

CDanceGroupProcessor::~CDanceGroupProcessor(void)
{
}

CDanceGroupProcessor& CDanceGroupProcessor::Instance()
{
    static CDanceGroupProcessor s_Instance;

    return s_Instance;
}

bool CDanceGroupProcessor::Init()
{
    RegisterMessageBeforeLoad();
    CeremonyBriefManager::Instance().Initialize();
    LoadAllDanceGroup();
    BigMamaGroup::Instance().Initialize();

    return true;
}

bool CDanceGroupProcessor::IsLoadComplete()
{
    return m_bDanceGroupComplete;
}

void CDanceGroupProcessor::OnUpdate(time_t tNow)
{
    if (!IsLoadComplete())
        return;
    else if (tNow <= (time_t)cDANCE_GROUP_DISMISS_RETENTION_TIME)
        return;

    unsigned int nDismissTime = (unsigned int)tNow - cDANCE_GROUP_DISMISS_RETENTION_TIME;

    std::string strTitle = CLocalization::Instance().GetString("Mail_Title_DismissDanceGroup", g_ServerConfig.m_nMaxMailTitleLength);
    std::string strContent = CLocalization::Instance().GetString("Mail_Content_DismissDanceGroup", g_ServerConfig.m_nMaxMailContentLength - 1 - cMAX_DANCE_GROUP_NAME_LEN);

    std::map<unsigned int, CDanceGroupInfo *>::const_iterator itrDGInfo;
    std::map<unsigned int, CRequestDanceGroupInfo>::iterator itrRquestInfo;
    CDanceGroupInfo *pDGInfo = NULL;

    for (std::multimap<unsigned int, unsigned int>::iterator bRemoveItr = m_DGManager.m_multimapRemovedGroup.begin(); 
         bRemoveItr != m_DGManager.m_multimapRemovedGroup.end(); )
    {
        if (bRemoveItr->first <= nDismissTime)
        {
            itrDGInfo = m_DGManager.m_GroupMap.find(bRemoveItr->second);

            if (m_DGManager.m_GroupMap.end() == itrDGInfo || NULL == itrDGInfo->second)
            {
                m_DGManager.m_multimapRemovedGroup.erase(bRemoveItr++);

                continue;
            }

            pDGInfo = itrDGInfo->second;

            if (!pDGInfo->IsValid(tNow))
            {
                for (itrRquestInfo = pDGInfo->m_RequestMap.begin(); 
                     itrRquestInfo != pDGInfo->m_RequestMap.end(); ++itrRquestInfo)
                {
                    RemoveRequestEnterDanceGroupInfo(pDGInfo->m_DGBaseInfo.m_nGroupID, 
                        itrRquestInfo->second.m_nRoleID, true, false);
                }
                pDGInfo->m_RequestMap.clear();

                std::string strFullContent;
                SafeFormat(strFullContent, strContent, pDGInfo->m_DGBaseInfo.m_strGroupName.c_str());
                SendMailToDanceGroupMember(*pDGInfo, EMailType_DismissDanceGroup, strTitle, strFullContent);

                CGlobalRankListManager::Instance().RemoveFromRank(ERankDataType_Group_Honor, pDGInfo->m_DGBaseInfo.m_nGroupID);

                ProcessRemoveDanceGroup(pDGInfo->m_DGBaseInfo.m_nGroupID);

                m_DGManager.m_multimapRemovedGroup.erase(bRemoveItr++);
            }
            else
                ++bRemoveItr;
        }
        else
            break;
    }

    for (std::map<unsigned int, std::map<unsigned int, unsigned int> >::iterator bRequestItr = m_DGManager.m_mapRequestTime.begin(); 
         bRequestItr != m_DGManager.m_mapRequestTime.end(); )
    {
        if (bRequestItr->first <= (unsigned int)tNow)
        {
            std::map<unsigned int, unsigned int> mapRole = bRequestItr->second;
            for (std::map<unsigned int, unsigned int>::iterator bRoleItr = mapRole.begin(), 
                 eRoleItr = mapRole.end(); bRoleItr != eRoleItr; ++bRoleItr)
            {
                RemoveRequestEnterDanceGroupInfo(bRoleItr->second, bRoleItr->first, true, true);
            }

            m_DGManager.m_mapRequestTime.erase(bRequestItr++);
        }
        else
            break;
    }
}

const CDanceGroupInfo* CDanceGroupProcessor::GetDanceGroupInfo(unsigned int nGroupID)
{
    return m_DGManager.GetDanceGroupInfo(nGroupID);
}

void CDanceGroupProcessor::OpenChallengeStoreyAndRecord( unsigned int nGroupID, const CDanceGroupMember *pMember, 
    int nStorey, int nDecreaseActive, const std::string &rContent )
{
    CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo( nGroupID );

    if ( pDGInfo == NULL || pMember == NULL )
        return;

    bool bSuccess = ChangeChallengeActive( pDGInfo, pMember, nDecreaseActive, false, "OpenStorey" );

    if ( bSuccess )
    {
        SaveDanceGroupRecordsInfo( pDGInfo, pMember->m_nRoleID, (unsigned int)time( NULL ), rContent );
    }
}

void CDanceGroupProcessor::UpdateDanceGroupContribution( unsigned int nGroupID, unsigned int nRoleID, int nValue, const std::string& rType )
{
    if ( nValue == 0 )
        return;

    CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo( nGroupID );
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByID( nRoleID );

    if ( pDGInfo == NULL || pRoleEntity == NULL )
        return;

    CDanceGroupMember *pMember = pDGInfo->GetGroupMember( nRoleID );
    if ( pMember == NULL )
        return;

    unsigned int nOldValue = pMember->m_nContribution;
    unsigned int nOldTotalValue = pMember->m_nTotalContribution;

    pMember->ChangeContribution( nValue );

    SaveDanceGroupLog( nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, nRoleID, pRoleEntity->GetName(), 
        ( nValue > 0 ? EDanceGroupLog_MemberContributionAdd : EDanceGroupLog_MemberContributionDec ), 
        abs( nValue ), nOldValue, pMember->m_nContribution, rType.c_str() );

    if ( nValue > 0 )
    {
        SaveDanceGroupLog( nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 
            nRoleID, pRoleEntity->GetName(), EDanceGroupLog_MemberIntergralAdd, 
            nValue, nOldTotalValue, pMember->m_nTotalContribution );
    }

    UpdateDanceGroupMemberInfo( nGroupID, *pMember );
}

void CDanceGroupProcessor::RegisterMessageBeforeLoad()
{
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetDanceGroupInfoList);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GetMyDanceGroupInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_CreateDanceGroup);

    RegMsgProcessor(MSG_C2S_GetDanceGroupInfoList, &CDanceGroupProcessor::OnGetDanceGroupInfoList);
    RegMsgProcessor(MSG_S2G_GetMyDanceGroupInfo, &CDanceGroupProcessor::OnGetMyDanceGroupInfo);
    RegMsgProcessor(MSG_S2G_CreateDanceGroup, &CDanceGroupProcessor::OnCreateDanceGroup);

    RegQueryProcessor(QUERY_DanceGroupLoadAllInfo, &CDanceGroupProcessor::OnLoadAllDanceGroupInfo);
    RegQueryProcessor(QUERY_DanceGroup_Create, &CDanceGroupProcessor::OnDanceGroupCreated);
}

void CDanceGroupProcessor::RegisterMessageAfterLoaded()
{
    if (!IsLoadComplete())
        return;

    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_CancelRequestEnterDanceGroup);

    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GetDanceGroupInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_UpdateDanceGroupMemberInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_DismissDanceGroup);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_CancelDismissDanceGroup);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_RequestEnterDanceGroup);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_RequestEnterDanceGroupByName);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_ReplyRequestEnterDanceGroup);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_ReplyRequestListEnterDanceGroup);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_ChangeDanceGroupName);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_ChangeDanceGroupColor);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_ChangeDanceGroupBadge);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_ChangeDanceGroupProfile);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_ChangeDanceGroupAnnouncement);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_ChangeDanceGroupTitle);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_ChangeDanceGroupTitleName);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_ChangeDanceGroupLeader);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_ChangeDanceGroupHonor);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_AddDanceGroupContribution);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_ReduceDanceGroupContribution);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_UnlockDanceGroupBadge);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_ChangeDanceGroupBadgeOrEffect);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_DonateDanceGroupResource);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_UpgradeDanceGroup);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GetDanceGroupRecrods);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_ExitDanceGroup);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_KickOutDanceGroupMember);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_DanceGroupActivityGetStarInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_DanceGroupActivityAddStarPower);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_DanceGroupActivityGetStarPowerInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_DanceGroupActivityOpenStarPacket);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_DanceGroupShopGetInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_DanceGroupShopExchange);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_AcceptDanceGroupMemberInvitation);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_CanBeInvitedCheck);
    


    RegMsgProcessor(MSG_C2S_CancelRequestEnterDanceGroup, &CDanceGroupProcessor::OnCancelRequestEnterDanceGroup);

    RegMsgProcessor(MSG_S2G_GetDanceGroupInfo, &CDanceGroupProcessor::OnGetDanceGroupInfo);
    RegMsgProcessor(MSG_S2G_UpdateDanceGroupMemberInfo, &CDanceGroupProcessor::OnUpdateDanceGroupMemberInfo);
    RegMsgProcessor(MSG_S2G_DismissDanceGroup, &CDanceGroupProcessor::OnDismissDanceGroup);
    RegMsgProcessor(MSG_S2G_CancelDismissDanceGroup, &CDanceGroupProcessor::OnCancelDismissDanceGroup);
    RegMsgProcessor(MSG_S2G_RequestEnterDanceGroup, &CDanceGroupProcessor::OnRequestEnterDanceGroup);
    RegMsgProcessor(MSG_S2G_RequestEnterDanceGroupByName, &CDanceGroupProcessor::OnRequestEnterDanceGroupByName);
    RegMsgProcessor(MSG_S2G_ReplyRequestEnterDanceGroup, &CDanceGroupProcessor::OnReplyRequestEnterDanceGroup);
    RegMsgProcessor(MSG_S2G_ReplyRequestListEnterDanceGroup, &CDanceGroupProcessor::OnReplyRequestListEnterDanceGroup);
    RegMsgProcessor(MSG_S2G_ChangeDanceGroupName, &CDanceGroupProcessor::OnChangeDanceGroupName);
    RegMsgProcessor(MSG_S2G_ChangeDanceGroupColor, &CDanceGroupProcessor::OnChangeDanceGroupColor);
    RegMsgProcessor(MSG_S2G_ChangeDanceGroupBadge, &CDanceGroupProcessor::OnChangeDanceGroupBadge);
    RegMsgProcessor(MSG_S2G_ChangeDanceGroupProfile, &CDanceGroupProcessor::OnChangeDanceGroupProfile);
    RegMsgProcessor(MSG_S2G_ChangeDanceGroupAnnouncement, &CDanceGroupProcessor::OnChangeDanceGroupAnnouncement);
    RegMsgProcessor(MSG_S2G_ChangeDanceGroupTitle, &CDanceGroupProcessor::OnChangeDanceGroupTitle);
    RegMsgProcessor(MSG_S2G_ChangeDanceGroupTitleName, &CDanceGroupProcessor::OnChangeDanceGroupTitleName);
    RegMsgProcessor(MSG_S2G_ChangeDanceGroupLeader, &CDanceGroupProcessor::OnChangeDanceGroupLeader);
    RegMsgProcessor(MSG_S2G_ChangeDanceGroupHonor, &CDanceGroupProcessor::OnChangeDanceGroupHonor);
    RegMsgProcessor(MSG_S2G_AddDanceGroupContribution, &CDanceGroupProcessor::OnAddDanceGroupContribution);
    RegMsgProcessor(MSG_S2G_ReduceDanceGroupContribution, &CDanceGroupProcessor::OnReduceDanceGroupContribution);
    RegMsgProcessor(MSG_S2G_UnlockDanceGroupBadge, &CDanceGroupProcessor::OnUnlockDanceGroupBadge);
    RegMsgProcessor(MSG_S2G_ChangeDanceGroupBadgeOrEffect, &CDanceGroupProcessor::OnChangeDanceGroupBadgeOrEffect);
    RegMsgProcessor(MSG_S2G_DonateDanceGroupResource, &CDanceGroupProcessor::OnDonateDanceGroupResource);
    RegMsgProcessor(MSG_S2G_UpgradeDanceGroup, &CDanceGroupProcessor::OnUpgradeDanceGroup);
    RegMsgProcessor(MSG_S2G_GetDanceGroupRecords, &CDanceGroupProcessor::OnGetDanceGroupRecords);
    RegMsgProcessor(MSG_S2G_ExitDanceGroup, &CDanceGroupProcessor::OnExitDanceGroup);
    RegMsgProcessor(MSG_S2G_KickOutDanceGroupMember, &CDanceGroupProcessor::OnKickOutDanceGroupMember);
    RegMsgProcessor(MSG_S2G_DanceGroupActivityGetStarInfo, &CDanceGroupProcessor::OnDanceGroupActivityGetStarInfo);
    RegMsgProcessor(MSG_S2G_DanceGroupActivityAddStarPower, &CDanceGroupProcessor::OnDanceGroupActivityAddStarPower);
    RegMsgProcessor(MSG_S2G_DanceGroupActivityGetStarPowerInfo, &CDanceGroupProcessor::OnDanceGroupActivityGetStarPowerInfo);
    RegMsgProcessor(MSG_S2G_DanceGroupActivityOpenStarPacket, &CDanceGroupProcessor::OnDanceGroupActivityOpenStarPacket);
//     RegMsgProcessor(MSG_S2G_DanceGroupShopGetInfo, &CDanceGroupProcessor::OnDanceGroupShopGetInfo);
//     RegMsgProcessor(MSG_S2G_DanceGroupShopExchange, &CDanceGroupProcessor::OnDanceGroupShopExchange);
    RegMsgProcessor(MSG_S2G_AcceptDanceGroupMemberInvitation, &CDanceGroupProcessor::OnAcceptDanceGroupMemberInvitation);
    RegMsgProcessor(MSG_S2G_CanBeInviteCheck, &CDanceGroupProcessor::OnCanBeInviteCheck);

    RegMsgCreatorAndProcessor(GameMsg_S2G_AddDanceGroupChallengeActive, &CDanceGroupProcessor::OnAddDanceGroupChallengeActive);


    RegQueryProcessor(QUERY_DanceGroup_ChangeDanceGroupName, &CDanceGroupProcessor::OnDanceGroupNameChanged);
}

bool CDanceGroupProcessor::LoadAllDanceGroup()
{
    g_pQueryMgr->AddQuery(QUERY_DanceGroupLoadAllInfo, 0, &m_DGManager);
    return true;
}

void CDanceGroupProcessor::OnGetDanceGroupInfoList(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_GetDanceGroupInfoList &rMsg = dynamic_cast<GameMsg_C2S_GetDanceGroupInfoList &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (NULL != pRoleEntity)
    {
        GameMsg_S2C_SendDanceGroupInfoList resMsg;

        if (!IsLoadComplete())
        {
            resMsg.m_nBeginIndex = rMsg.m_nBeginIndex;
            resMsg.m_nCount = rMsg.m_nCount;
            resMsg.m_nTotal = 0;

            pRoleEntity->SendPlayerMsg(&resMsg);

            return;
        }

        std::vector<CDanceGroupInfo *> vectDGInfo;
        CDanceGroupInfo *pDGInfo = NULL;

        unsigned int nBeginRank = rMsg.m_nBeginIndex;

        if (nBeginRank < 1)
            nBeginRank = 1;

        unsigned int nEndRank = rMsg.m_nBeginIndex + rMsg.m_nCount;
        unsigned int nInsertCount = 0;
        unsigned int nRank = 1;

        // 先取已经申请舞团信息
        const std::set<unsigned int> &rSetRequestList = m_DGManager.GetRequestGroupList(pRoleEntity->m_nRoleID);
        for (std::set<unsigned int>::const_iterator bSetIt = rSetRequestList.begin(), eSetIt = rSetRequestList.end(); 
             bSetIt != eSetIt; ++bSetIt)
        {
            pDGInfo = m_DGManager.GetDanceGroupInfo(*bSetIt);
            nRank = m_DGRankManager.GetRankByKey(*bSetIt);

            if (NULL != pDGInfo && 0 != nRank)
            {
                // 第一页需要先显示申请信息
                if (1 == nBeginRank)
                    vectDGInfo.push_back(pDGInfo);

                // 判断插入条数
                if (nRank >= nBeginRank)
                    ++nInsertCount;
            }
        }

        // 向前移动相应的插入条数
        if (nBeginRank > nInsertCount)
            nBeginRank -= nInsertCount;
        else
            nBeginRank = 1;

        CDanceGroupRankInfo *pDGRankInfo = NULL;

        std::list<CRankInfo *> listRankInfo;
        m_DGRankManager.GetRange(nBeginRank, nEndRank, listRankInfo);
        for (std::list<CRankInfo *>::const_iterator bListIt = listRankInfo.begin(), eListIt = listRankInfo.end(); 
             bListIt != eListIt; ++bListIt)
        {
            pDGRankInfo = dynamic_cast<CDanceGroupRankInfo *>(*bListIt);

            // 剔除申请信息
            if (NULL != pDGRankInfo && NULL != pDGRankInfo->m_pDanceGroupInfo && 
                rSetRequestList.find(pDGRankInfo->m_pDanceGroupInfo->m_DGBaseInfo.m_nGroupID) == rSetRequestList.end())
            {
                vectDGInfo.push_back(pDGRankInfo->m_pDanceGroupInfo);
            }
        }

        CDanceGroupMember *pMember = NULL;

        unsigned int count = rMsg.m_nCount;

        for (std::vector<CDanceGroupInfo *>::const_iterator itr = vectDGInfo.begin(); 
             itr != vectDGInfo.end() && count > 0; ++itr, --count)
        {
            pDGInfo = *itr;
            CDanceGroupBaseInfo &rBaseInfo = pDGInfo->m_DGBaseInfo;
            CDanceGroupItem item;

            item.m_nGroupID = rBaseInfo.m_nGroupID;
            item.m_strGroupName = rBaseInfo.m_strGroupName;
            item.m_nLeaderRoleID = rBaseInfo.m_nLeaderRoleID;
            item.m_strLeaderRoleName = rBaseInfo.m_strLeaderRoleName;
            item.m_nLevel = rBaseInfo.m_nLevel;
            item.m_nBadge = rBaseInfo.m_nBadge;
            item.m_nHonor = rBaseInfo.m_nHonor;
            item.m_strProfile = rBaseInfo.m_strProfile;
            item.m_nCreateTime = rBaseInfo.m_nCreateTime;
            item.m_bCanRequestEnter = rBaseInfo.m_bCanRequestEnter;
            item.m_bHaveRequest = pDGInfo->IsInRequestQueue(pRoleEntity->m_nRoleID);
            item.m_nMemberCount = (unsigned short)pDGInfo->GetMemberCount();
            item.m_nMaxMemberCount = (unsigned short)GetDanceGroupMaxMemberCount(rBaseInfo.m_nLevel);
            item.m_nEffectID = rBaseInfo.m_nEffect;

            pMember = pDGInfo->GetGroupMember(item.m_nLeaderRoleID);
            if (NULL != pMember)
            {
                item.m_bLeaderIsVIP = pMember->m_bIsVIP;
                item.m_nLeaderVIPLevel = pMember->m_nVIPLevel;
            }

            resMsg.m_DGItemList.push_back(item);
        }

        resMsg.m_nBeginIndex = rMsg.m_nBeginIndex;
        resMsg.m_nCount = (unsigned short)resMsg.m_DGItemList.size();
        resMsg.m_nTotal = m_DGManager.m_GroupMap.size();

        pRoleEntity->SendPlayerMsg(&resMsg);
    }
}

void CDanceGroupProcessor::OnCancelRequestEnterDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_CancelRequestEnterDanceGroup &rMsg = dynamic_cast<GameMsg_C2S_CancelRequestEnterDanceGroup &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (0 != rMsg.m_nGroupID && NULL != pRoleEntity)
    {
        GameMsg_S2C_CancelRequestEnterDanceGroupResult resMsg;

        resMsg.m_nResult = (unsigned short)m_DGManager.CheckCancelRequestEnterDanceGroupCond(pRoleEntity->m_nRoleID, rMsg.m_nGroupID);

        if (ECancelRequestEnterDanceGroupResult_Success == resMsg.m_nResult)
            RemoveRequestEnterDanceGroupInfo(rMsg.m_nGroupID, pRoleEntity->m_nRoleID, false, true);

        pRoleEntity->SendPlayerMsg(&resMsg);
    }
}

void CDanceGroupProcessor::OnGetDanceGroupInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_GetDanceGroupInfo &rMsg = dynamic_cast<GameMsg_S2G_GetDanceGroupInfo &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);
    CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);

    if (NULL == pRoleEntity)
        return;

    if (NULL != pDGInfo)
    {
        CDanceGroupBaseInfo &rBaseInfo = pDGInfo->m_DGBaseInfo;

        GameMsg_S2C_SendSimpleDanceGroupInfo resMsg;
        resMsg.m_DGItem.m_nGroupID = rBaseInfo.m_nGroupID;
        resMsg.m_DGItem.m_strGroupName = rBaseInfo.m_strGroupName;
        resMsg.m_DGItem.m_nLeaderRoleID = rBaseInfo.m_nLeaderRoleID;
        resMsg.m_DGItem.m_strLeaderRoleName = rBaseInfo.m_strLeaderRoleName;
        resMsg.m_DGItem.m_nLevel = rBaseInfo.m_nLevel;
        resMsg.m_DGItem.m_nBadge = rBaseInfo.m_nBadge;
        resMsg.m_DGItem.m_nEffectID = rBaseInfo.m_nEffect;
        resMsg.m_DGItem.m_nHonor = rBaseInfo.m_nHonor;
        resMsg.m_DGItem.m_strProfile = rBaseInfo.m_strProfile;
        resMsg.m_DGItem.m_nCreateTime = rBaseInfo.m_nCreateTime;
        resMsg.m_DGItem.m_bCanRequestEnter = rBaseInfo.m_bCanRequestEnter;
        resMsg.m_DGItem.m_bHaveRequest = pDGInfo->IsInRequestQueue(pRoleEntity->m_nRoleID);
        resMsg.m_DGItem.m_nMemberCount = (unsigned short)pDGInfo->GetMemberCount();
        resMsg.m_DGItem.m_nMaxMemberCount = (unsigned short)GetDanceGroupMaxMemberCount(rBaseInfo.m_nLevel);

        pRoleEntity->SendPlayerMsg(&resMsg);
    }
    else
    {
        GameMsg_S2C_GetDanceGroupInfoResult resMsg;
        resMsg.m_nResult = EGetDanceGroupInfoResult_NotExist;

        pRoleEntity->SendPlayerMsg(&resMsg);
    }
}

void CDanceGroupProcessor::OnGetMyDanceGroupInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_GetMyDanceGroupInfo &rMsg = dynamic_cast<GameMsg_S2G_GetMyDanceGroupInfo &>(msg);
    CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);

    GameMsg_G2S_GetMyDanceGroupInfo resMsg;
    resMsg.m_nClickedPlace = rMsg.m_nClickedPlace;

    if (IsLoadComplete() && NULL != pDGInfo)
    {
        resMsg.m_pDGInfo = new CDanceGroupInfo(*pDGInfo);
    }

    SendPlayerMsgByAccountID(msg.nAccount, resMsg);
}

void CDanceGroupProcessor::OnUpdateDanceGroupMemberInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_UpdateDanceGroupMemberInfo &rMsg = dynamic_cast<GameMsg_S2G_UpdateDanceGroupMemberInfo &>(msg);
    CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (NULL != pDGInfo && NULL != pRoleEntity)
    {
        CDanceGroupMember *pDGMember = pDGInfo->GetGroupMember(pRoleEntity->m_nRoleID);

        if (NULL != pDGMember)
        {
            pDGMember->m_strRoleName = pRoleEntity->m_szRoleName;
            pDGMember->m_nLevel = pRoleEntity->m_nLevel;
            pDGMember->m_nState = rMsg.m_nState;
            pDGMember->m_bIsVIP = pRoleEntity->m_bIsVIP;
            pDGMember->m_nVIPLevel = pRoleEntity->m_nVIPLevel;
            pDGMember->m_nLastOffLineTime = rMsg.m_nLastOffLineTime;

            unsigned int nExceptRoleID = 0;
            if (EPlayerState_Offline == rMsg.m_nState)
                nExceptRoleID = pRoleEntity->m_nRoleID;

            SendUpdateDanceGroupMemberInfo(rMsg.m_nGroupID, nExceptRoleID, *pDGMember);
        }

        if (pDGInfo->m_DGBaseInfo.m_nLeaderRoleID == pRoleEntity->m_nRoleID && 
            pDGInfo->m_DGBaseInfo.m_strLeaderRoleName != pRoleEntity->m_szRoleName)
        {
            pDGInfo->m_DGBaseInfo.m_strLeaderRoleName = pRoleEntity->m_szRoleName;

            UpdateDanceGroupBaseInfo(pDGInfo->m_DGBaseInfo, EDanceGroupBaseInfoUpdateType_Data);
        }
    }
}

void CDanceGroupProcessor::OnCreateDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_CreateDanceGroup &rMsg = dynamic_cast<GameMsg_S2G_CreateDanceGroup &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    GameMsg_G2S_CreateDanceGroupResult createResultMsg;
    createResultMsg.m_nResult = ECreateDanceGroupResult_Success;
    createResultMsg.m_pDGInfo = NULL;

    if (!IsLoadComplete())
        createResultMsg.m_nResult = ECreateDanceGroupResult_Initializing;
    else if (pRoleEntity->GetDanceGroupID() != 0)
        createResultMsg.m_nResult = ECreateDanceGroupResult_HaveAGroup;
    DanceGroupQuitInfo *quitInfo = m_DGManager.GetQuitInfo(pRoleEntity->GetRoleID());
    if (NULL != quitInfo)
    {
        int needCD = (CDanceGroupDataMgr::Instance().m_nEnterNewGroupCD);
        int CD = (int)((time(NULL) - quitInfo->m_nQuitStamp));
        if (CD < needCD)
        {
            createResultMsg.m_nResult = (unsigned short)ECreateDanceGroupResult_CreateCD;
            createResultMsg.m_nCreateCD = needCD - CD;
        }
    }

    if (createResultMsg.m_nResult != ECreateDanceGroupResult_Success)
    {
        pRoleEntity->SendPlayerMsg(&createResultMsg);

        return;
    }


    CDanceGroupInfo *pDGInfo = new CDanceGroupInfo;

    if (NULL != pRoleEntity && NULL != pDGInfo)
    {
        CDanceGroupBaseInfo &rBaseInfo = pDGInfo->m_DGBaseInfo;
        CDanceGroupActivityInfo &rActivityInfo = pDGInfo->m_DGActivityInfo;

        rBaseInfo.m_strGroupName = rMsg.m_strGroupName;
        rBaseInfo.m_nLeaderRoleID = pRoleEntity->m_nRoleID;
        rBaseInfo.m_strLeaderRoleName = pRoleEntity->m_szRoleName;
        rBaseInfo.m_nLevel = 1;
        rBaseInfo.m_nColor = 0;
        rBaseInfo.m_nBadge = rMsg.m_nBadge;
        rBaseInfo.m_nEffect = 0;
        rBaseInfo.m_strProfile = rMsg.m_strProfile;
        rBaseInfo.m_strBadgeEffectLock = "";
        rBaseInfo.m_nCreateTime = (unsigned int)time(NULL);
        rActivityInfo.m_nDayPacket = 1;
        rActivityInfo.m_nStarLevel = 1;
        rActivityInfo.m_nCurrentPowerValue = 0;
        rBaseInfo.m_nCeremonyTimes = 0;
        rBaseInfo.m_nCeremonyStamp = 0; 

        CDanceGroupMember member;
        member.m_nRoleID = pRoleEntity->m_nRoleID;
        member.m_strRoleName = pRoleEntity->m_szRoleName;
        member.m_nTitle = EGroupTitle_Leader;
        member.m_nSex = pRoleEntity->m_nSex;
        member.m_nLevel = pRoleEntity->m_nLevel;
        member.m_nState = EPlayerState_Idle;
        member.m_nLastOffLineTime = rMsg.m_nLastOffLineTime;
        member.m_bIsVIP = pRoleEntity->m_bIsVIP;
        member.m_nVIPLevel = pRoleEntity->m_nVIPLevel;
        member.m_nProperty = EMemberProperty_Creator; // 

        pDGInfo->AddMemberInfo(member);

        CDelHelper delHelper(pDGInfo);

        g_pQueryMgr->AddQuery(QUERY_DanceGroup_Create, 0, pDGInfo, 0, NULL, &delHelper);
    }
}

void CDanceGroupProcessor::OnDismissDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_DismissDanceGroup &rMsg = dynamic_cast<GameMsg_S2G_DismissDanceGroup &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (0 != rMsg.m_nGroupID && NULL != pRoleEntity)
    {
        GameMsg_S2C_DismissDanceGroupResult resMsg;

        resMsg.m_nResult = (unsigned short)m_DGManager.CheckDismissDanceGroupCond(pRoleEntity->m_nRoleID, rMsg.m_nGroupID);

        if (EDismissDanceGroupResult_Success == resMsg.m_nResult)
        {
            CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);

            if (NULL != pDGInfo && m_DGManager.ProcessRemoveDanceGroup(rMsg.m_nGroupID))
            {
                SaveDanceGroupLog(rMsg.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 
                    pRoleEntity->m_nRoleID, pRoleEntity->m_szRoleName, EDanceGroupLog_Destory, 
                    pDGInfo->m_DGBaseInfo.m_nBadge, pDGInfo->m_DGBaseInfo.m_nEffect);

                UpdateDanceGroupBaseInfo(pDGInfo->m_DGBaseInfo, EDanceGroupBaseInfoUpdateType_Data);

                // send mail to members
                std::string strTitle = CLocalization::Instance().GetString("Mail_Title_PreDismissDanceGroup", g_ServerConfig.m_nMaxMailTitleLength);
                std::string strContent = CLocalization::Instance().GetString("Mail_Content_PreDismissDanceGroup", g_ServerConfig.m_nMaxMailContentLength - 1 - cMAX_DANCE_GROUP_NAME_LEN);

                std::string strFullContent;
                SafeFormat(strFullContent, strContent, pDGInfo->m_DGBaseInfo.m_strGroupName.c_str());
                SendMailToDanceGroupMember(*pDGInfo, EMailType_PreDismissDanceGroup, strTitle, strFullContent);
            }
        }

        pRoleEntity->SendPlayerMsg(&resMsg);
    }
}

void CDanceGroupProcessor::OnCancelDismissDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_CancelDismissDanceGroup &rMsg = dynamic_cast<GameMsg_S2G_CancelDismissDanceGroup &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (NULL != pRoleEntity)
    {
        GameMsg_S2C_CancelDismissDanceGroupResult resMsg;

        resMsg.m_nResult = (unsigned short)m_DGManager.CheckCancelDismissDanceGroupCond(pRoleEntity->m_nRoleID, rMsg.m_nGroupID);

        if (ECancelDismissDanceGroupResult_Success == resMsg.m_nResult)
        {
            CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);

            if (NULL != pDGInfo && m_DGManager.ProcessCancelRemoveDanceGroup(rMsg.m_nGroupID))
            {
                UpdateDanceGroupBaseInfo(pDGInfo->m_DGBaseInfo, EDanceGroupBaseInfoUpdateType_Data);
            }
        }

        pRoleEntity->SendPlayerMsg(&resMsg);
    }
}

void CDanceGroupProcessor::OnRequestEnterDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_RequestEnterDanceGroup &rMsg = dynamic_cast<GameMsg_S2G_RequestEnterDanceGroup &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (NULL == pRoleEntity)
        return;

    CDanceGroupInfo *pDGInfo = NULL;
    GameMsg_S2C_RequestEnterDanceGroupResult resMsg;

    DanceGroupQuitInfo *quitInfo = m_DGManager.GetQuitInfo(pRoleEntity->GetRoleID());
    if (NULL != quitInfo)
    {
        bool bIsOldGroup = (quitInfo->m_nDanceGroupID == rMsg.m_nRequestGroupID);
        int needCD = (bIsOldGroup ? CDanceGroupDataMgr::Instance().m_nEnterOldGroupCD : CDanceGroupDataMgr::Instance().m_nEnterNewGroupCD);
        int CD = (int)((time(NULL) - quitInfo->m_nQuitStamp));
        if (CD < needCD)
        {
            resMsg.m_nResult = (unsigned short)(bIsOldGroup ? ERequestEnterDanceGroupResult_CDRepeatGroup : ERequestEnterDanceGroupResult_CDNewGroup);
            resMsg.m_fApplyCDTime = float(needCD - CD);
            
            pRoleEntity->SendPlayerMsg(&resMsg);
            return ;
        }
    }


    if (0 != rMsg.m_nMyGroupID)
    {
        pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nMyGroupID);

        if (NULL != pDGInfo && pDGInfo->IsValid(time(NULL) ) )
        {
            CDanceGroupMember *pDGMember = pDGInfo->GetGroupMember(pRoleEntity->m_nRoleID);

            if (NULL != pDGMember)
            {
                resMsg.m_nResult = ERequestEnterDanceGroupResult_HaveAGroup;

                pRoleEntity->SendPlayerMsg(&resMsg);

                return;
            }
        }
    }

    resMsg.m_nResult = (unsigned short)m_DGManager.CheckRequestEnterDanceGroupCond(pRoleEntity->m_nRoleID, rMsg.m_nRequestGroupID);

    if (ERequestEnterDanceGroupResult_Success == resMsg.m_nResult)
    {
        CRequestDanceGroupInfo requestInfo = CRequestDanceGroupInfo();

        requestInfo.m_nRoleID = pRoleEntity->m_nRoleID;
        requestInfo.m_strRoleName = pRoleEntity->m_szRoleName;
        requestInfo.m_nSex = pRoleEntity->m_nSex;
        requestInfo.m_nLevel = pRoleEntity->m_nLevel;
        requestInfo.m_nExpiredTime = (unsigned int)time(NULL) + cREQUEST_ENTER_DANCE_GROUP_EXPIRED_TIME;
        requestInfo.m_bIsVIP = pRoleEntity->m_bIsVIP;
        requestInfo.m_nVIPLevel = pRoleEntity->m_nVIPLevel;

        m_DGManager.AddRequestDanceGroupInfo(rMsg.m_nRequestGroupID, requestInfo);

        SaveRequestEnterDanceGroupInfo(rMsg.m_nRequestGroupID, requestInfo);

        GameMsg_G2S_AddRequestDanceGroupInfo addRequestInfo;
        addRequestInfo.m_nGroupID = rMsg.m_nRequestGroupID;
        addRequestInfo.m_RequestInfo = requestInfo;

        SendMsg2DanceGroupManager(rMsg.m_nRequestGroupID, addRequestInfo);
    }

    pRoleEntity->SendPlayerMsg(&resMsg);
}

void CDanceGroupProcessor::OnRequestEnterDanceGroupByName(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_RequestEnterDanceGroupByName &rMsg = dynamic_cast<GameMsg_S2G_RequestEnterDanceGroupByName &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (NULL == pRoleEntity)
        return;

    GameMsg_S2C_RequestEnterDanceGroupResult resMsg;

    if (0 != rMsg.m_nMyGroupID)
    {
        CDanceGroupInfo *pDGInfo = NULL;
        pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nMyGroupID);

        if (NULL != pDGInfo && pDGInfo->IsValid(time(NULL) ) )
        {
            CDanceGroupMember *pDGMember = pDGInfo->GetGroupMember(pRoleEntity->m_nRoleID);

            if (NULL != pDGMember)
            {
                resMsg.m_nResult = ERequestEnterDanceGroupResult_HaveAGroup;

                pRoleEntity->SendPlayerMsg(&resMsg);

                return;
            }
        }
    }


    CDanceGroupInfo* pRequestGroupInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_strRequestDanceGroupName);
    if (NULL == pRequestGroupInfo)
        return ;
    
    resMsg.m_nResult = (unsigned short)m_DGManager.CheckRequestEnterDanceGroupCond(pRoleEntity->m_nRoleID, pRequestGroupInfo->m_DGBaseInfo.m_nGroupID);

    if (ERequestEnterDanceGroupResult_Success == resMsg.m_nResult)
    {
        CRequestDanceGroupInfo requestInfo = CRequestDanceGroupInfo();

        requestInfo.m_nRoleID = pRoleEntity->m_nRoleID;
        requestInfo.m_strRoleName = pRoleEntity->m_szRoleName;
        requestInfo.m_nSex = pRoleEntity->m_nSex;
        requestInfo.m_nLevel = pRoleEntity->m_nLevel;
        requestInfo.m_nExpiredTime = (unsigned int)time(NULL) + cREQUEST_ENTER_DANCE_GROUP_EXPIRED_TIME;
        requestInfo.m_bIsVIP = pRoleEntity->m_bIsVIP;
        requestInfo.m_nVIPLevel = pRoleEntity->m_nVIPLevel;

        m_DGManager.AddRequestDanceGroupInfo(pRequestGroupInfo->m_DGBaseInfo.m_nGroupID, requestInfo);

        SaveRequestEnterDanceGroupInfo(pRequestGroupInfo->m_DGBaseInfo.m_nGroupID, requestInfo);

        GameMsg_G2S_AddRequestDanceGroupInfo addRequestInfo;
        addRequestInfo.m_nGroupID = pRequestGroupInfo->m_DGBaseInfo.m_nGroupID;
        addRequestInfo.m_RequestInfo = requestInfo;

        SendMsg2DanceGroupManager(pRequestGroupInfo->m_DGBaseInfo.m_nGroupID, addRequestInfo);
    }

    pRoleEntity->SendPlayerMsg(&resMsg);
}


void CDanceGroupProcessor::OnReplyRequestEnterDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_ReplyRequestEnterDanceGroup &rMsg = dynamic_cast<GameMsg_S2G_ReplyRequestEnterDanceGroup &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (NULL != pRoleEntity)
    {
        GameMsg_S2C_ReplyRequestEnterDanceGroupResult resMsg;
        resMsg.m_nRequestRoleID = rMsg.m_nRequestRoleID;
        resMsg.m_nResult = EReplyRequestEnterDanceGroupResult_Success;

        CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);

        if (!rMsg.m_bAllow)
        {
            RemoveRequestEnterDanceGroupInfo(rMsg.m_nGroupID, rMsg.m_nRequestRoleID, true, true);
        }
        else if (NULL != pDGInfo)
        {
            resMsg.m_nResult = ProcessRequestEnterDanceGroup(pDGInfo, pRoleEntity->m_nRoleID, 
                pRoleEntity->m_szRoleName, rMsg.m_nRequestRoleID, (unsigned int)time(NULL));
        }
        else
        {
            resMsg.m_nResult = EReplyRequestEnterDanceGroupResult_GroupNotExist;
        }

        pRoleEntity->SendPlayerMsg(&resMsg);
    }
}

void CDanceGroupProcessor::OnReplyRequestListEnterDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_ReplyRequestListEnterDanceGroup &rMsg = dynamic_cast<GameMsg_S2G_ReplyRequestListEnterDanceGroup &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (NULL != pRoleEntity)
    {
        GameMsg_S2C_ReplyRequestListEnterDanceGroupResult resMsg;
        resMsg.m_nRoleID = pRoleEntity->m_nRoleID;
        resMsg.m_bAllow = rMsg.m_bAllow;
        resMsg.m_nAllowCount = 0;
        resMsg.m_nResult = EReplyRequestEnterDanceGroupResult_Success;

        CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);

        if (!rMsg.m_bAllow)
        {
            RemoveRequestEnterDanceGroupInfo(rMsg.m_nGroupID, rMsg.m_listRoleID, true, true);
        }
        else if (NULL != pDGInfo)
        {
            unsigned short nResult = EReplyRequestEnterDanceGroupResult_Success;
            unsigned int nRecordTime = (unsigned int)time(NULL);

            for (std::list<unsigned int>::const_iterator bIt = rMsg.m_listRoleID.begin(), eIt = rMsg.m_listRoleID.end(); 
                 bIt != eIt; ++bIt)
            {
                nResult = ProcessRequestEnterDanceGroup(pDGInfo, pRoleEntity->m_nRoleID, pRoleEntity->m_szRoleName, *bIt, nRecordTime);

                if (EReplyRequestEnterDanceGroupResult_Success == nResult)
                {
                    ++nRecordTime;
                    ++resMsg.m_nAllowCount;
                }
                else if (EReplyRequestEnterDanceGroupResult_NoInRequestQueue != nResult)
                {
                    // EReplyRequestEnterDanceGroupResult_NotInGroup
                    // EReplyRequestEnterDanceGroupResult_NotInGroupErr
                    // EReplyRequestEnterDanceGroupResult_GroupNotExist
                    // EReplyRequestEnterDanceGroupResult_NoAuthority
                    // EReplyRequestEnterDanceGroupResult_MaxCount
                    break;
                }
            }

            resMsg.m_nResult = nResult;
        }
        else
            resMsg.m_nResult = EReplyRequestEnterDanceGroupResult_GroupNotExist;

        pRoleEntity->SendPlayerMsg(&resMsg);
    }
}

void CDanceGroupProcessor::OnChangeDanceGroupName(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_ChangeDanceGroupName &rMsg = dynamic_cast<GameMsg_S2G_ChangeDanceGroupName &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (NULL != pRoleEntity)
    {
        time_t nTimeNow = time(NULL);
        unsigned int nTimeCoolDown = 0;
        GameMsg_S2C_ChangeDanceGroupNameResult resMsg;

        resMsg.m_nResult = (unsigned short)m_DGManager.CheckChangeDanceGroupNameCond(pRoleEntity->m_nRoleID, rMsg.m_nGroupID, nTimeNow, nTimeCoolDown);

        if (EChangeDanceGroupNameResult_Success == resMsg.m_nResult)
        {
            SaveChangeDanceGroupNameInfo(rMsg.m_nGroupID, pRoleEntity->m_nRoleID, rMsg.m_strNewGroupName, (unsigned int)nTimeNow);
        }
        else
        {
            resMsg.m_strNewGroupName = rMsg.m_strNewGroupName;
            resMsg.m_nTimeCoolDown = nTimeCoolDown;

            pRoleEntity->SendPlayerMsg(&resMsg);
        }
    }
}

void CDanceGroupProcessor::OnChangeDanceGroupColor(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_ChangeDanceGroupColor &rMsg = dynamic_cast<GameMsg_S2G_ChangeDanceGroupColor &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (NULL != pRoleEntity)
    {
        GameMsg_S2C_ChangeDanceGroupColorResult resMsg;

        resMsg.m_nResult = (unsigned short)m_DGManager.CheckChangeDanceGroupColorCond(pRoleEntity->m_nRoleID, rMsg.m_nGroupID);

        if (EChangeDanceGroupColorResult_Success == resMsg.m_nResult)
        {
            CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);

            if (NULL != pDGInfo)
            {
                pDGInfo->m_DGBaseInfo.m_nColor = rMsg.m_nColor;

                UpdateDanceGroupBaseInfo(pDGInfo->m_DGBaseInfo, EDanceGroupBaseInfoUpdateType_Data);
            }
        }

        pRoleEntity->SendPlayerMsg(&resMsg);
    }
}

void CDanceGroupProcessor::OnChangeDanceGroupBadge(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_ChangeDanceGroupBadge &rMsg = dynamic_cast<GameMsg_S2G_ChangeDanceGroupBadge &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (NULL != pRoleEntity)
    {
        GameMsg_S2C_ChangeDanceGroupBadgeResult resMsg;

        resMsg.m_nResult = (unsigned short)m_DGManager.CheckChangeDanceGroupBadgeCond(pRoleEntity->m_nRoleID, rMsg.m_nGroupID);

        if (EChangeDanceGroupBadgeResult_Success == resMsg.m_nResult)
        {
            CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);

            if (NULL != pDGInfo)
            {
                pDGInfo->m_DGBaseInfo.m_nBadge = rMsg.m_nBadge;

                UpdateDanceGroupBaseInfo(pDGInfo->m_DGBaseInfo, EDanceGroupBaseInfoUpdateType_Data);
            }
        }

        pRoleEntity->SendPlayerMsg(&resMsg);
    }
}

void CDanceGroupProcessor::OnChangeDanceGroupProfile(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_ChangeDanceGroupProfile &rMsg = dynamic_cast<GameMsg_S2G_ChangeDanceGroupProfile &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (NULL != pRoleEntity)
    {
        GameMsg_S2C_ChangeDanceGroupProfileResult resMsg;

        resMsg.m_nResult = (unsigned short)m_DGManager.CheckChangeDanceGroupProfileCond(pRoleEntity->m_nRoleID, rMsg.m_nGroupID);

        if (EChangeDanceGroupProfileResult_Success == resMsg.m_nResult)
        {
            CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);

            if (NULL != pDGInfo)
            {
                pDGInfo->m_DGBaseInfo.m_strProfile = rMsg.m_strProfile;

                UpdateDanceGroupBaseInfo(pDGInfo->m_DGBaseInfo, EDanceGroupBaseInfoUpdateType_Profile);
            }
        }

        pRoleEntity->SendPlayerMsg(&resMsg);
    }
}

void CDanceGroupProcessor::OnChangeDanceGroupAnnouncement(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_ChangeDanceGroupAnnouncement &rMsg = dynamic_cast<GameMsg_S2G_ChangeDanceGroupAnnouncement &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (NULL != pRoleEntity)
    {
        GameMsg_S2C_ChangeDanceGroupAnnouncementResult resMsg;

        resMsg.m_nResult = (unsigned short)m_DGManager.CheckChangeDanceGroupAnnouncementCond(pRoleEntity->m_nRoleID, rMsg.m_nGroupID);

        if (EChangeDanceGroupAnnouncementResult_Success == resMsg.m_nResult)
        {
            CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);

            if (NULL != pDGInfo)
            {
                pDGInfo->m_DGBaseInfo.m_strAnnouncement = rMsg.m_strAnnouncement;

                UpdateDanceGroupBaseInfo(pDGInfo->m_DGBaseInfo, EDanceGroupBaseInfoUpdateType_Announcement);
            }
        }

        pRoleEntity->SendPlayerMsg(&resMsg);
    }
}

std::string CDanceGroupProcessor::MemberPropertyString(EMemberProperty prop)
{
    switch (prop)
    {
    case EMemberProperty_Creator:
        return "创始人";
        break;
    case EMemberProperty_OldMember:
        return "老团员";
        break;
    case EMemberProperty_MengXin:
        return "小萌新";
        break;
    default:
        return "missed";
        break;
    }
}


void CDanceGroupProcessor::ChangeMemberProperty(unsigned int nRoleID, unsigned int nDanceGroupID)
{
//    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByID(nRoleID);
    CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(nDanceGroupID);
    ENSURE_WITH_LOG_CMD(NULL != pDGInfo, return, "error to get danceGroup[%u] on role[%u] addproperty", nDanceGroupID, nRoleID);
    CDanceGroupMember *pMember = pDGInfo->GetGroupMember(nRoleID);
    ENSURE_WITH_LOG_CMD(NULL != pMember, return, "error to get role[%u] memberinfo in dancegroup[%u] on addproperty", nRoleID, nDanceGroupID);

    pMember->m_nProperty = EMemberProperty_OldMember;

//    DanceGroup_Record_ChangeMemberProperty=玩家%s<小萌新>变更为<老团员>
    std::string strRecord;
    SafeFormat(strRecord, CLocalization::Instance().GetString("DanceGroup_Record_ChangeMemberProperty"),
        pMember->m_strRoleName.c_str());

    SaveDanceGroupRecordsInfo(pDGInfo, pDGInfo->m_DGBaseInfo.m_nLeaderRoleID, (unsigned int)time(NULL), strRecord);

    UpdateDanceGroupMemberInfo(pDGInfo->m_DGBaseInfo.m_nGroupID, *pMember);

//     SaveDanceGroupLog(pDGInfo->m_DGBaseInfo.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, pMember->m_nRoleID, 
//         pMember->m_strRoleName, EDanceGroupLog_MemberTitleChange, nOldTitle, pMember->m_nTitle);
}

void CDanceGroupProcessor::AddGroupCeremonyTime(unsigned int nDanceGroupID, bool bIsFreeTime)
{
    CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(nDanceGroupID);
    ENSURE_WITH_LOG_CMD(NULL != pDGInfo, return, "error to get danceGroup[%u] on add ceremonyTimes", nDanceGroupID);

    pDGInfo->m_DGBaseInfo.ResetCeremonyStamp(time(NULL));
    bool bGroupFree = (pDGInfo->m_DGBaseInfo.m_nCeremonyTimes + 1 < CDanceGroupDataMgr::Instance().m_nCeremonyFreeApplyTimes);
    if (!bIsFreeTime && bGroupFree)
    { // 不记录
        ENSURE_WITH_LOG_CMD(LOGLEVEL_ERROR, return, "not equal ceremonyTime[%u]", pDGInfo->m_DGBaseInfo.m_nCeremonyTimes);
    }
    else
    {
        pDGInfo->m_DGBaseInfo.m_nCeremonyTimes++;
    }

    // 更新
    UpdateDanceGroupBaseInfo(pDGInfo->m_DGBaseInfo, EDanceGroupBaseInfoUpdateType_CermonyTimes);

    return ;
}


void CDanceGroupProcessor::OnChangeDanceGroupTitle(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_ChangeDanceGroupTitle &rMsg = dynamic_cast<GameMsg_S2G_ChangeDanceGroupTitle &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (NULL != pRoleEntity)
    {
        GameMsg_S2C_ChangeDanceGroupTitleResult resMsg;

        resMsg.m_nResult = (unsigned short)m_DGManager.CheckChangeDanceGroupTitleCond(pRoleEntity->m_nRoleID, rMsg.m_nGroupID, rMsg.m_nTargetRoleID, rMsg.m_nNewTitle);

        if (EChangeDanceGroupTitleResult_Success == resMsg.m_nResult)
        {
            CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);

            if (NULL != pDGInfo)
            {
                CDanceGroupMember *pMember = pDGInfo->GetGroupMember(rMsg.m_nTargetRoleID);

                if (NULL != pMember)
                {
                    unsigned char nOldTitle = pMember->m_nTitle;
                    string strTitle;

                    pMember->m_nTitle = rMsg.m_nNewTitle;
                    strTitle.clear();

                    switch (pMember->m_nTitle)
                    {
                    case EGroupTitle_Leader:
                        {
                            strTitle = pDGInfo->m_DGBaseInfo.m_nTitleName1;

                            if (strTitle.empty())
                                strTitle = CLocalization::Instance().GetString("DanceGroup_Record_Title1");
                        }
                        break;

                    case EGroupTitle_ViceLeader:
                        {
                            strTitle = pDGInfo->m_DGBaseInfo.m_nTitleName2;

                            if (strTitle.empty())
                                strTitle = CLocalization::Instance().GetString("DanceGroup_Record_Title2");
                        }
                        break;

                    case EGroupTitle_Assistant:
                        {
                            strTitle = pDGInfo->m_DGBaseInfo.m_nTitleName3;

                            if (strTitle.empty())
                                strTitle = CLocalization::Instance().GetString("DanceGroup_Record_Title3");
                        }
                        break;

                    case EGroupTitle_SeniorMember:
                        {
                            strTitle = pDGInfo->m_DGBaseInfo.m_nTitleName4;

                            if (strTitle.empty())
                                strTitle = CLocalization::Instance().GetString("DanceGroup_Record_Title4");
                        }
                        break;

                    case EGroupTitle_JuniorMember:
                        {
                            strTitle = pDGInfo->m_DGBaseInfo.m_nTitleName5;

                            if (strTitle.empty())
                                strTitle = CLocalization::Instance().GetString("DanceGroup_Record_Title5");
                        }
                        break;
                    }

                    std::string strRecord;
                    SafeFormat(strRecord, CLocalization::Instance().GetString("DanceGroup_Record_ChangeMemberTitle"),
                        pDGInfo->m_DGBaseInfo.m_strLeaderRoleName.c_str(), pMember->m_strRoleName.c_str(), strTitle.c_str());

                    SaveDanceGroupRecordsInfo(pDGInfo, pDGInfo->m_DGBaseInfo.m_nLeaderRoleID, (unsigned int)time(NULL), strRecord);

                    UpdateDanceGroupMemberInfo(pDGInfo->m_DGBaseInfo.m_nGroupID, *pMember);

                    SaveDanceGroupLog(pDGInfo->m_DGBaseInfo.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, pMember->m_nRoleID, 
                        pMember->m_strRoleName, EDanceGroupLog_MemberTitleChange, nOldTitle, pMember->m_nTitle);

                    // 更新舞团试炼活力值
                    if ( CanOpenChallenge( pMember->m_nTitle ) )
                    {
                        GameMsg_G2S_UpdateDanceGroupChallengeActive updateMsg;
                        updateMsg.m_nGroupID = pDGInfo->m_DGBaseInfo.m_nGroupID;
                        updateMsg.m_nActive = pDGInfo->m_DGBaseInfo.m_nChallengeActive;
                        SendPlayerMsgByRoleID( pMember->m_nRoleID, updateMsg );
                    }
                }
            }
        }

        pRoleEntity->SendPlayerMsg(&resMsg);
    }
}

void CDanceGroupProcessor::OnChangeDanceGroupTitleName(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_ChangeDanceGroupTitleName &rMsg = dynamic_cast<GameMsg_S2G_ChangeDanceGroupTitleName &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (NULL != pRoleEntity)
    {
        GameMsg_S2C_ChangeDanceGroupTitleNameResult resMsg;

        resMsg.m_nResult = (unsigned short)m_DGManager.CheckChangeDanceGroupTitleNameCond(pRoleEntity->m_nRoleID, rMsg.m_nGroupID);

        if (EChangeDanceGroupTitleNameResult_Success == resMsg.m_nResult)
        {
            CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);

            if (NULL != pDGInfo)
            {
                bool bError = false;

                switch (rMsg.m_nTitle)
                {
                case EGroupTitle_Leader:
                    pDGInfo->m_DGBaseInfo.m_nTitleName1 = rMsg.m_strTitleName;
                    break;

                case EGroupTitle_ViceLeader:
                    pDGInfo->m_DGBaseInfo.m_nTitleName2 = rMsg.m_strTitleName;
                    break;

                case EGroupTitle_Assistant:
                    pDGInfo->m_DGBaseInfo.m_nTitleName3 = rMsg.m_strTitleName;
                    break;

                case EGroupTitle_SeniorMember:
                    pDGInfo->m_DGBaseInfo.m_nTitleName4 = rMsg.m_strTitleName;
                    break;

                case EGroupTitle_JuniorMember:
                    pDGInfo->m_DGBaseInfo.m_nTitleName5 = rMsg.m_strTitleName;
                    break;

                default:
                    {
                        LOG_ERROR << "Change dance group title name type(" 
                                  << rMsg.m_nTitle 
                                  << ") is wrong." << std::endl;

                        bError = true;
                    }
                    break;
                }

                if (!bError)
                    UpdateDanceGroupBaseInfo(pDGInfo->m_DGBaseInfo, EDanceGroupBaseInfoUpdateType_Data);
            }
        }

        pRoleEntity->SendPlayerMsg(&resMsg);
    }
}

void CDanceGroupProcessor::OnChangeDanceGroupLeader(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_ChangeDanceGroupLeader &rMsg = dynamic_cast<GameMsg_S2G_ChangeDanceGroupLeader &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (NULL != pRoleEntity)
    {
        GameMsg_S2C_ChangeDanceGroupLeaderResult resMsg;

        resMsg.m_nResult = (unsigned short)m_DGManager.CheckChangeDanceGroupLeaderCond(pRoleEntity->m_nRoleID, rMsg.m_nGroupID, rMsg.m_nNewLeaderID);

        if (EChangeDanceGroupLeaderResult_Success == resMsg.m_nResult)
        {
            CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);

            if (NULL != pDGInfo)
            {
                CDanceGroupMember *pOldLeader = pDGInfo->GetGroupMember(pRoleEntity->m_nRoleID);
                CDanceGroupMember *pNewLeader = pDGInfo->GetGroupMember(rMsg.m_nNewLeaderID);

                if (NULL != pOldLeader && NULL != pNewLeader)
                {
                    pOldLeader->m_nTitle = EGroupTitle_JuniorMember;
                    pNewLeader->m_nTitle = EGroupTitle_Leader;

                    pDGInfo->m_DGBaseInfo.m_nLeaderRoleID = pNewLeader->m_nRoleID;
                    pDGInfo->m_DGBaseInfo.m_strLeaderRoleName = pNewLeader->m_strRoleName;

                    SaveDanceGroupLog(rMsg.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 
                        pOldLeader->m_nRoleID, pOldLeader->m_strRoleName, EDanceGroupLog_LeaderChange, 
                        pNewLeader->m_nRoleID, pNewLeader->m_strRoleName.c_str());

                    UpdateDanceGroupBaseInfo(pDGInfo->m_DGBaseInfo, EDanceGroupBaseInfoUpdateType_Data);
                    UpdateDanceGroupMemberInfo(rMsg.m_nGroupID, *pOldLeader);
                    UpdateDanceGroupMemberInfo(rMsg.m_nGroupID, *pNewLeader);

                    // 更新舞团试炼活力值
                    GameMsg_G2S_UpdateDanceGroupChallengeActive updateMsg;
                    updateMsg.m_nGroupID = pDGInfo->m_DGBaseInfo.m_nGroupID;
                    updateMsg.m_nActive = pDGInfo->m_DGBaseInfo.m_nChallengeActive;
                    SendPlayerMsgByRoleID( pNewLeader->m_nRoleID, updateMsg );

                    // send mail to members
                    std::string strTitle = CLocalization::Instance().GetString("Mail_Title_ChangeDanceGroupLeader", g_ServerConfig.m_nMaxMailTitleLength);
                    std::string strContent = CLocalization::Instance().GetString("Mail_Content_ChangeDanceGroupLeader", g_ServerConfig.m_nMaxMailContentLength - 1 - MAX_NAME_LEN * 2);

                    std::string strFullContent;
                    SafeFormat(strFullContent, strContent, pOldLeader->m_strRoleName.c_str(), pNewLeader->m_strRoleName.c_str());
                    SendMailToDanceGroupMember(*pDGInfo, EMailType_ChangeDanceGroupLeader, strTitle, strFullContent);
                }
            }
        }

        pRoleEntity->SendPlayerMsg(&resMsg);
    }
}

void CDanceGroupProcessor::OnChangeDanceGroupHonor(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_ChangeDanceGroupHonor &rMsg = dynamic_cast<GameMsg_S2G_ChangeDanceGroupHonor &>(msg);

    if (0 != rMsg.m_nChangeHonor)
    {
        CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);

        if (NULL != pDGInfo)
        {
            std::string strType("ChangeDanceGroupHonour");
            ChangeDanceGroupHonor(pDGInfo, rMsg.m_nChangeHonor, rMsg.m_nCause, rMsg.m_nPara, strType);

            UpdateDanceGroupBaseInfo(pDGInfo->m_DGBaseInfo, EDanceGroupBaseInfoUpdateType_Data);
        }
    }
}

void CDanceGroupProcessor::OnAddDanceGroupContribution(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_AddDanceGroupContribution &rMsg = dynamic_cast<GameMsg_S2G_AddDanceGroupContribution &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (NULL != pRoleEntity && 0 != rMsg.m_nChangeValue)
    {
        UpdateDanceGroupContribution( rMsg.m_nGroupID, pRoleEntity->GetRoleID(), (int)rMsg.m_nChangeValue, "Add" );
    }
}


void CDanceGroupProcessor::OnReduceDanceGroupContribution(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_ReduceDanceGroupContribution &rMsg = dynamic_cast<GameMsg_S2G_ReduceDanceGroupContribution &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (NULL != pRoleEntity && 0 != rMsg.m_nChangeValue)
    {
        UpdateDanceGroupContribution(rMsg.m_nGroupID, pRoleEntity->GetRoleID(), -(int)rMsg.m_nChangeValue, "Reduce");
    }
}

void CDanceGroupProcessor::OnUnlockDanceGroupBadge(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_UnlockDanceGroupBadge &rMsg = dynamic_cast<GameMsg_S2G_UnlockDanceGroupBadge &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);
    CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGoupID);

    if (NULL != pRoleEntity && NULL != pDGInfo)
    {
        GameMsg_S2C_UnlockDanceGroupBadgeResult resMsg;
        resMsg.m_nType = rMsg.m_nType;
        resMsg.m_nUnlockID = rMsg.m_nUnlockID;

        if (pDGInfo->m_DGBaseInfo.m_nResA >= rMsg.m_nResA && 
            pDGInfo->m_DGBaseInfo.m_nResB >= rMsg.m_nResB && 
            pDGInfo->m_DGBaseInfo.m_nResC >= rMsg.m_nResC)
        {
            std::string strID;
            Formatter(strID) << rMsg.m_nUnlockID;

            if (std::string::npos != pDGInfo->m_DGBaseInfo.m_strBadgeEffectLock.find(strID))
            {
                resMsg.m_nErrorCode = EDanceGroupBadgeResult_EffectHasUnlock;
            }
            else
            {
                unsigned int nOldValue = 0;

                if (rMsg.m_nResA > 0)
                {
                    nOldValue = pDGInfo->m_DGBaseInfo.m_nResA;
                    pDGInfo->m_DGBaseInfo.m_nResA -= rMsg.m_nResA;

                    SaveDanceGroupLog(pDGInfo->m_DGBaseInfo.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 
                        pRoleEntity->m_nRoleID, pRoleEntity->m_szRoleName, EDanceGroupLog_ResConsume, 
                        "ResA", rMsg.m_nResA, nOldValue, pDGInfo->m_DGBaseInfo.m_nResA, "UnlockTuanhui");
                }

                if (rMsg.m_nResB > 0)
                {
                    nOldValue = pDGInfo->m_DGBaseInfo.m_nResB;
                    pDGInfo->m_DGBaseInfo.m_nResB -= rMsg.m_nResB;

                    SaveDanceGroupLog(pDGInfo->m_DGBaseInfo.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 
                        pRoleEntity->m_nRoleID, pRoleEntity->m_szRoleName, EDanceGroupLog_ResConsume, 
                        "ResB", rMsg.m_nResB, nOldValue, pDGInfo->m_DGBaseInfo.m_nResB, "UnlockTuanhui");
                }

                if (rMsg.m_nResC > 0)
                {
                    nOldValue = pDGInfo->m_DGBaseInfo.m_nResC;
                    pDGInfo->m_DGBaseInfo.m_nResC -= rMsg.m_nResC;

                    SaveDanceGroupLog(pDGInfo->m_DGBaseInfo.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 
                        pRoleEntity->m_nRoleID, pRoleEntity->m_szRoleName, EDanceGroupLog_ResConsume, 
                        "ResC", rMsg.m_nResC, nOldValue, pDGInfo->m_DGBaseInfo.m_nResC, "UnlockTuanhui");
                }

                pDGInfo->m_DGBaseInfo.m_strBadgeEffectLock += ",";
                pDGInfo->m_DGBaseInfo.m_strBadgeEffectLock += strID;

                if (',' == pDGInfo->m_DGBaseInfo.m_strBadgeEffectLock[0])
                    pDGInfo->m_DGBaseInfo.m_strBadgeEffectLock.erase(0, 1);

                std::string strRecord;
                SafeFormat( strRecord, CLocalization::Instance().GetString( "DanceGroup_Record_ConsumeRes" ),
                    pRoleEntity->m_szRoleName, rMsg.m_nResA, rMsg.m_nResB, rMsg.m_nResC,
                    CLocalization::Instance().GetString( "DanceGroup_Record_UnlockBadge" ).c_str() );
                SaveDanceGroupRecordsInfo( pDGInfo, pRoleEntity->m_nRoleID, (unsigned int)time( NULL ), strRecord );

                UpdateDanceGroupBaseInfo(pDGInfo->m_DGBaseInfo, EDanceGroupBaseInfoUpdateType_Data);

                resMsg.m_nErrorCode = EDanceGroupBadgeResult_Success;
            }
        }
        else
            resMsg.m_nErrorCode = EDanceGroupBadgeResult_NoEnoughRes;

        pRoleEntity->SendPlayerMsg(&resMsg);
    }
}

void CDanceGroupProcessor::OnChangeDanceGroupBadgeOrEffect(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_ChangeDanceGroupBadgeOrEffect &rMsg = dynamic_cast<GameMsg_S2G_ChangeDanceGroupBadgeOrEffect &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);
    CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);

    if (NULL != pRoleEntity && NULL != pDGInfo)
    {
        GameMsg_G2S_ChangeDanceGroupBadgeOrEffectResult resMsg;
        resMsg.m_nRoleID = pRoleEntity->m_nRoleID;
        resMsg.m_nBadge = rMsg.m_nBadgeID;
        resMsg.m_nEffect = rMsg.m_nEffectID;

        if (pDGInfo->m_DGBaseInfo.m_nResA >= rMsg.m_nResA && 
            pDGInfo->m_DGBaseInfo.m_nResB >= rMsg.m_nResB && 
            pDGInfo->m_DGBaseInfo.m_nResC >= rMsg.m_nResC)
        {
            int nOldResAValue = pDGInfo->m_DGBaseInfo.m_nResA;
            int nOldResBValue = pDGInfo->m_DGBaseInfo.m_nResB;
            int nOldResCValue = pDGInfo->m_DGBaseInfo.m_nResC;

            pDGInfo->m_DGBaseInfo.m_nResA -= rMsg.m_nResA;
            pDGInfo->m_DGBaseInfo.m_nResB -= rMsg.m_nResB;
            pDGInfo->m_DGBaseInfo.m_nResC -= rMsg.m_nResC;

            if (0 != rMsg.m_nBadgeID)
                pDGInfo->m_DGBaseInfo.m_nBadge = rMsg.m_nBadgeID;
            if (0 != rMsg.m_nEffectID)
                pDGInfo->m_DGBaseInfo.m_nEffect = rMsg.m_nEffectID;

            resMsg.m_nBadge = pDGInfo->m_DGBaseInfo.m_nBadge;
            resMsg.m_nEffect = pDGInfo->m_DGBaseInfo.m_nEffect;

            std::string strRecord;
            SafeFormat(strRecord, CLocalization::Instance().GetString("DanceGroup_Record_ConsumeRes"), 
                pRoleEntity->m_szRoleName, rMsg.m_nResA, rMsg.m_nResB, rMsg.m_nResC, 
                CLocalization::Instance().GetString("DanceGroup_Record_ChangeTuanhui").c_str());

            SaveDanceGroupRecordsInfo(pDGInfo, pRoleEntity->m_nRoleID, (unsigned int)time(NULL), strRecord);

            if (rMsg.m_nResA > 0)
            {
                SaveDanceGroupLog(pDGInfo->m_DGBaseInfo.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 
                    pRoleEntity->m_nRoleID, pRoleEntity->m_szRoleName, EDanceGroupLog_ResConsume, "ResA", 
                    rMsg.m_nResA, nOldResAValue, pDGInfo->m_DGBaseInfo.m_nResA, "SaveTuanhui");
            }
            if (rMsg.m_nResB > 0)
            {
                SaveDanceGroupLog(pDGInfo->m_DGBaseInfo.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 
                    pRoleEntity->m_nRoleID, pRoleEntity->m_szRoleName, EDanceGroupLog_ResConsume, "ResB", 
                    rMsg.m_nResB, nOldResBValue, pDGInfo->m_DGBaseInfo.m_nResB, "SaveTuanhui");
            }
            if (rMsg.m_nResC > 0)
            {
                SaveDanceGroupLog(pDGInfo->m_DGBaseInfo.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 
                    pRoleEntity->m_nRoleID, pRoleEntity->m_szRoleName, EDanceGroupLog_ResConsume, "ResC", 
                    rMsg.m_nResC, nOldResCValue, pDGInfo->m_DGBaseInfo.m_nResC, "SaveTuanhui");
            }

            UpdateDanceGroupBaseInfo(pDGInfo->m_DGBaseInfo, EDanceGroupBaseInfoUpdateType_Data);
            SendMsg2DanceGroupMember(rMsg.m_nGroupID, 0, resMsg);

            CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);
            if (NULL != pDGInfo)
            {
                for (std::map<unsigned int, CDanceGroupMember *>::const_iterator bIt = pDGInfo->m_MemberMap.begin(), eIt = pDGInfo->m_MemberMap.end();
                    bIt != eIt; ++bIt)
                {
                    if (NULL != bIt->second)
                    {
                        CGroupRoleEntity* pGroupRole = CGroupPlayerManager::Instance().SearchRoleByID(bIt->second->m_nRoleID);
                        if (NULL != pGroupRole)
                        {
                            GroupRoomProcessor::Instance().OnUpdateDanceGroupBadgeOrEffect(pGroupRole, resMsg.m_nBadge, resMsg.m_nEffect);
                        }
                    }
                }
            }

        }
        else
        {
            resMsg.m_nErrorCode = EDanceGroupBadgeResult_NoEnoughRes;

            pRoleEntity->SendPlayerMsg(&resMsg);
        }
    }
}

void CDanceGroupProcessor::OnDonateDanceGroupResource(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_DonateDanceGroupResource &rMsg = dynamic_cast<GameMsg_S2G_DonateDanceGroupResource &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (NULL != pRoleEntity)
    {
        GameMsg_G2S_DonateDanceGroupResourceResult resMsg;
        resMsg.m_nResA = rMsg.m_nResA;
        resMsg.m_nResB = rMsg.m_nResB;
        resMsg.m_nResC = rMsg.m_nResC;
        resMsg.m_nResult = (unsigned short)m_DGManager.CheckDonateDanceGroupResourceCond(pRoleEntity->m_nRoleID, rMsg.m_nGroupID, rMsg.m_nResA, rMsg.m_nResB, rMsg.m_nResC);

        if (EDonateDanceGroupResourceResult_Success == resMsg.m_nResult)
        {
            CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);

            if (NULL != pDGInfo)
            {
                unsigned int nOldValue = 0;

                if (rMsg.m_nResA > 0)
                {
                    nOldValue = pDGInfo->m_DGBaseInfo.m_nResA;
                    pDGInfo->m_DGBaseInfo.m_nResA = MIN(pDGInfo->m_DGBaseInfo.m_nResA + rMsg.m_nResA, cMAX_GROUP_RES_A);

                    SaveDanceGroupLog(pDGInfo->m_DGBaseInfo.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 
                        pRoleEntity->m_nRoleID, pRoleEntity->m_szRoleName, EDanceGroupLog_ResDonate, cITEM_TYPE_RES_A, 
                        "ResA", rMsg.m_nResA, nOldValue, pDGInfo->m_DGBaseInfo.m_nResA);
                }

                if (rMsg.m_nResB > 0)
                {
                    nOldValue = pDGInfo->m_DGBaseInfo.m_nResB;
                    pDGInfo->m_DGBaseInfo.m_nResB = MIN(pDGInfo->m_DGBaseInfo.m_nResB + rMsg.m_nResB, cMAX_GROUP_RES_B);

                    SaveDanceGroupLog(pDGInfo->m_DGBaseInfo.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 
                        pRoleEntity->m_nRoleID, pRoleEntity->m_szRoleName, EDanceGroupLog_ResDonate, cITEM_TYPE_RES_B, 
                        "ResB", rMsg.m_nResB, nOldValue, pDGInfo->m_DGBaseInfo.m_nResB);
                }

                if (rMsg.m_nResC > 0)
                {
                    nOldValue = pDGInfo->m_DGBaseInfo.m_nResC;
                    pDGInfo->m_DGBaseInfo.m_nResC = MIN(pDGInfo->m_DGBaseInfo.m_nResC + rMsg.m_nResC, cMAX_GROUP_RES_C);

                    SaveDanceGroupLog(pDGInfo->m_DGBaseInfo.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 
                        pRoleEntity->m_nRoleID, pRoleEntity->m_szRoleName, EDanceGroupLog_ResDonate, cITEM_TYPE_RES_C, 
                        "ResC", rMsg.m_nResC, nOldValue, pDGInfo->m_DGBaseInfo.m_nResC);
                }

                CDanceGroupDataMgr &rDGDataMgr = CDanceGroupDataMgr::Instance();
                unsigned int nAdd = 
                    rMsg.m_nResA * rDGDataMgr.IncDGHonorNumPerDonatedResA() + 
                    rMsg.m_nResB * rDGDataMgr.IncDGHonorNumPerDonatedResB() + 
                    rMsg.m_nResC * rDGDataMgr.IncDGHonorNumPerDonatedResC();

                if (nAdd > 0)
                {
                    std::string strType("Donate");
                    ChangeDanceGroupHonor(pDGInfo, nAdd, (int)EChangeDanceGroupHonorCause_MemberDonate, pRoleEntity->m_nRoleID, strType);
                }

                CDanceGroupMember *pMember = pDGInfo->GetGroupMember(pRoleEntity->m_nRoleID);

                if (NULL != pMember)
                {
                    nAdd = 
                        rMsg.m_nResA * rDGDataMgr.IncMemberContributionNumPerDonatedResA() + 
                        rMsg.m_nResB * rDGDataMgr.IncMemberContributionNumPerDonatedResB() + 
                        rMsg.m_nResC * rDGDataMgr.IncMemberContributionNumPerDonatedResC();

                    if (nAdd > 0)
                    {
                        nOldValue = pMember->m_nContribution;
                        unsigned int nOldTotalValue = pMember->m_nTotalContribution;

                        pMember->ChangeContribution(nAdd);

                        UpdateDanceGroupMemberInfo(rMsg.m_nGroupID, *pMember);

                        SaveDanceGroupLog(pDGInfo->m_DGBaseInfo.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 
                            pRoleEntity->m_nRoleID, pRoleEntity->m_szRoleName, EDanceGroupLog_MemberContributionAdd, 
                            nAdd, nOldValue, pMember->m_nContribution, "Donate");
                        SaveDanceGroupLog(pDGInfo->m_DGBaseInfo.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 
                            pRoleEntity->m_nRoleID, pRoleEntity->m_szRoleName, EDanceGroupLog_MemberIntergralAdd, 
                            nAdd, nOldTotalValue, pMember->m_nTotalContribution);
                    }
                }

                std::string strRecord;
                SafeFormat(strRecord, CLocalization::Instance().GetString("DanceGroup_Record_DonateRes"), 
                    pRoleEntity->m_szRoleName, rMsg.m_nResA, rMsg.m_nResB, rMsg.m_nResC);

                SaveDanceGroupRecordsInfo(pDGInfo, pRoleEntity->m_nRoleID, (unsigned int)time(NULL), strRecord);

                UpdateDanceGroupBaseInfo(pDGInfo->m_DGBaseInfo, EDanceGroupBaseInfoUpdateType_Data);
            }
        }
        else
        {
            WriteLog(LOGLEVEL_ERROR, "[DANCE GROUP] donate failed: error code=%d roleid=%u resA=%d resB=%d resC=%d",
                resMsg.m_nResult, pRoleEntity->GetRoleID(), resMsg.m_nResA, resMsg.m_nResB, resMsg.m_nResC);
        }

        pRoleEntity->SendPlayerMsg(&resMsg);
    }
}

void CDanceGroupProcessor::OnUpgradeDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_UpgradeDanceGroup &rMsg = dynamic_cast<GameMsg_S2G_UpgradeDanceGroup &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (NULL != pRoleEntity)
    {
        GameMsg_S2C_UpgradeDanceGroupResult resMsg;
        resMsg.m_nResult = (unsigned short)m_DGManager.CheckUpgradeDanceGroupCond(pRoleEntity->m_nRoleID, rMsg.m_nGroupID);

        if (EUpgradeGroupDanceResult_Success == resMsg.m_nResult)
        {
            CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);

            if (NULL != pDGInfo)
            {
                unsigned short nOldLevel = pDGInfo->m_DGBaseInfo.m_nLevel;
                unsigned int nOldResAValue = pDGInfo->m_DGBaseInfo.m_nResA;
                unsigned int nOldResBValue = pDGInfo->m_DGBaseInfo.m_nResB;
                unsigned int nOldResCValue = pDGInfo->m_DGBaseInfo.m_nResC;

                m_DGManager.DanceGroupUpgrade(pDGInfo);

                resMsg.m_nCurLevel = pDGInfo->m_DGBaseInfo.m_nLevel;

                SaveDanceGroupLog(pDGInfo->m_DGBaseInfo.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 
                    pRoleEntity->m_nRoleID, pRoleEntity->m_szRoleName, EDanceGroupLog_Levelup, 
                    nOldLevel, resMsg.m_nCurLevel);

                CDGLevelUpRequireRes *pLevelInfo = CDanceGroupDataMgr::Instance().GetLevelUpRequireResBy(resMsg.m_nCurLevel);

                if (NULL != pLevelInfo)
                {
                    resMsg.m_nNextLevelUpResA = pLevelInfo->m_nResA;
                    resMsg.m_nNextLevelUpResB = pLevelInfo->m_nResB;
                    resMsg.m_nNextLevelUpResC = pLevelInfo->m_nResC;
                }

                pLevelInfo = CDanceGroupDataMgr::Instance().GetLevelUpRequireResBy(nOldLevel);

                if (NULL != pLevelInfo)
                {
                    std::string strRecord;
                    SafeFormat( strRecord, CLocalization::Instance().GetString( "DanceGroup_Record_ConsumeRes" ),
                        pRoleEntity->m_szRoleName, pLevelInfo->m_nResA, pLevelInfo->m_nResB, pLevelInfo->m_nResC,
                        CLocalization::Instance().GetString( "DanceGroup_Record_LevelUpDes" ).c_str() );

                    SaveDanceGroupRecordsInfo( pDGInfo, pRoleEntity->m_nRoleID, (unsigned int)time( NULL ), strRecord );

                    if (pLevelInfo->m_nResA > 0)
                    {
                        SaveDanceGroupLog(pDGInfo->m_DGBaseInfo.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 
                            pRoleEntity->m_nRoleID, pRoleEntity->m_szRoleName, EDanceGroupLog_ResConsume, "ResA", 
                            pLevelInfo->m_nResA, nOldResAValue, pDGInfo->m_DGBaseInfo.m_nResA, "LevelUp");
                    }

                    if (pLevelInfo->m_nResB > 0)
                    {
                        SaveDanceGroupLog(pDGInfo->m_DGBaseInfo.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 
                            pRoleEntity->m_nRoleID, pRoleEntity->m_szRoleName, EDanceGroupLog_ResConsume, "ResB", 
                            pLevelInfo->m_nResB, nOldResBValue, pDGInfo->m_DGBaseInfo.m_nResB, "LevelUp");
                    }

                    if (pLevelInfo->m_nResC > 0)
                    {
                        SaveDanceGroupLog(pDGInfo->m_DGBaseInfo.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 
                            pRoleEntity->m_nRoleID, pRoleEntity->m_szRoleName, EDanceGroupLog_ResConsume, "ResC", 
                            pLevelInfo->m_nResC, nOldResCValue, pDGInfo->m_DGBaseInfo.m_nResC, "LevelUp");
                    }
                }

                UpdateDanceGroupBaseInfo(pDGInfo->m_DGBaseInfo, EDanceGroupBaseInfoUpdateType_Data);
            }
        }

        pRoleEntity->SendPlayerMsg(&resMsg);
    }
}

void CDanceGroupProcessor::OnGetDanceGroupRecords(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_GetDanceGroupRecrods &rMsg = dynamic_cast<GameMsg_S2G_GetDanceGroupRecrods &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (NULL != pRoleEntity)
    {
        GameMsg_S2C_GetDanceGroupRecrodsResult resMsg;
        resMsg.m_nErrorCode = (unsigned short)m_DGManager.CheckDanceGroupRecrods(pRoleEntity->m_nRoleID, rMsg.m_nGroupID);

        if (EDanceGroupRecrodsResult_Success == resMsg.m_nErrorCode)
        {
            CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);

            if (NULL != pDGInfo)
            {
                UpdateDanceGroupRecordsTime(pDGInfo);

                resMsg.m_recordList = pDGInfo->m_recordsMap;
            }
        }

        pRoleEntity->SendPlayerMsg(&resMsg);
    }
}

void CDanceGroupProcessor::OnExitDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_ExitDanceGroup &rMsg = dynamic_cast<GameMsg_S2G_ExitDanceGroup &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (NULL != pRoleEntity)
    {
        GameMsg_S2C_ExitDanceGroupResult resMsg;
        resMsg.m_nResult = (unsigned short)m_DGManager.CheckExitDanceGroupCond(pRoleEntity->m_nRoleID, rMsg.m_nGroupID);

        if (EExitDanceGroupResult_Success == resMsg.m_nResult)
        {
            CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);

            if (NULL != pDGInfo)
            {
                // 广播退出消息：
                GameMsg_G2S_BroadcastMemberQuit quitBrod;
                quitBrod.m_nQuitRoleID = pRoleEntity->m_nRoleID;
                quitBrod.m_nGroupID = rMsg.m_nGroupID;
                BroadcastMsg2GameServer(&quitBrod);

                // 退出了。
                SaveRoleQuitGroupInfo(pRoleEntity->GetRoleID(), rMsg.m_nGroupID, (unsigned int)time(NULL));

                std::string strRecord;
                SafeFormat(strRecord, CLocalization::Instance().GetString("DanceGroup_Record_Exit"), 
                    pRoleEntity->m_szRoleName);

                SaveDanceGroupRecordsInfo(pDGInfo, pRoleEntity->m_nRoleID, (unsigned int)time(NULL), strRecord);

                SaveDanceGroupLog(pDGInfo->m_DGBaseInfo.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 
                    pRoleEntity->m_nRoleID, pRoleEntity->m_szRoleName, EDanceGroupLog_MemberLeave);

                RemoveDanceGroupMember(rMsg.m_nGroupID, pRoleEntity->m_nRoleID);

                std::string strTitle = CLocalization::Instance().GetString("Mail_Title_ExitDanceGroup", g_ServerConfig.m_nMaxMailTitleLength);
                std::string strContent = CLocalization::Instance().GetString("Mail_Content_ExitDanceGroup", g_ServerConfig.m_nMaxMailContentLength - 1 - MAX_NAME_LEN);
                std::string strFullContent;
                SafeFormat(strFullContent, strContent, pRoleEntity->m_szRoleName);
                SendMailToDanceGroupManager(*pDGInfo, EMailType_ExitDanceGroup, strTitle, strFullContent);

                GroupRoomProcessor::Instance().OnUpdateDanceGroupName(pRoleEntity, "");
                GroupRoomProcessor::Instance().OnRoleQuitFromDanceGroup(pRoleEntity);
                CDanceGroupFairyLandProcessor::Instance().OnRoleQuitGroup(rMsg.m_nGroupID, pRoleEntity->GetRoleID());

            }
        }

        pRoleEntity->SendPlayerMsg(&resMsg);
    }
}

void CDanceGroupProcessor::OnKickOutDanceGroupMember(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_KickOutDanceGroupMember &rMsg = dynamic_cast<GameMsg_S2G_KickOutDanceGroupMember &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (NULL != pRoleEntity)
    {
        GameMsg_S2C_KickOutDanceGroupMemberResult resMsg;
        resMsg.m_nResult = (unsigned short)m_DGManager.CheckKickOutDanceGroupMemberCond(pRoleEntity->m_nRoleID, rMsg.m_nGroupID, rMsg.m_nTargetRoleID);

        if (EKickOutDanceGroupMemberResult_Success == resMsg.m_nResult)
        {
            CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);

            if (NULL != pDGInfo)
            {
                CDanceGroupMember *pMember = pDGInfo->GetGroupMember(rMsg.m_nTargetRoleID);

                if (NULL != pMember)
                {
                    // 踢人广播：仪式捕获该信息， liguanghao add.
                    GameMsg_G2S_BroadcastMemberKicked kickBrod;
                    kickBrod.m_nGroupID = rMsg.m_nGroupID;
                    kickBrod.m_nKickedRoleID = rMsg.m_nTargetRoleID;
                    BroadcastMsg2GameServer(&kickBrod);

                    std::string strRecord;
                    SafeFormat(strRecord, CLocalization::Instance().GetString("DanceGroup_Record_KickOutMember"), 
                        pRoleEntity->m_szRoleName, pMember->m_strRoleName.c_str());

                    SaveDanceGroupRecordsInfo(pDGInfo, pRoleEntity->m_nRoleID, (unsigned int)time(NULL), strRecord);

                    CDanceGroupMember *pMemberOperator = pDGInfo->GetGroupMember(pRoleEntity->m_nRoleID);

                    if (NULL != pMemberOperator)
                    {
                        SaveDanceGroupLog(pDGInfo->m_DGBaseInfo.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 
                            pMember->m_nRoleID, pMember->m_strRoleName, EDanceGroupLog_MemberKicked, 
                            pRoleEntity->m_nRoleID, pRoleEntity->m_szRoleName, pMemberOperator->m_nTitle);
                    }
                }

                SaveRoleQuitGroupInfo(rMsg.m_nTargetRoleID, rMsg.m_nGroupID, (unsigned int)time(NULL));

                RemoveDanceGroupMember(rMsg.m_nGroupID, rMsg.m_nTargetRoleID);

                GameMsg_S2C_KickedOutDanceGroup kickedOutMsg;
                SendPlayerMsgByRoleID(rMsg.m_nTargetRoleID, kickedOutMsg);

                CGroupRoleEntity *pKicker = CGroupPlayerManager::Instance().SearchRoleByID(rMsg.m_nTargetRoleID);
                if (NULL != pKicker)
                {
                    GroupRoomProcessor::Instance().OnUpdateDanceGroupName(pKicker, ""); // 
                    GroupRoomProcessor::Instance().OnRoleQuitFromDanceGroup(pKicker);
                }

                CDanceGroupFairyLandProcessor::Instance().OnRoleQuitGroup(rMsg.m_nGroupID, rMsg.m_nTargetRoleID);
            }
        }

        pRoleEntity->SendPlayerMsg(&resMsg);
    }
}

void CDanceGroupProcessor::OnDanceGroupActivityGetStarInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_DanceGroupActivityGetStarInfo &rMsg = dynamic_cast<GameMsg_S2G_DanceGroupActivityGetStarInfo &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (NULL != pRoleEntity)
    {
        GameMsg_S2C_DanceGroupActivityGetStarInfoResult resMsg;
        resMsg.m_nErrorCode = EDanceGroupActivityStar_Success;

        CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);

        if (NULL != pDGInfo)
        {
            ResetDanceGroupActivityData(pDGInfo, true);

            CDanceGroupMember *pMember = pDGInfo->GetGroupMember(pRoleEntity->m_nRoleID);

            if (NULL != pMember)
            {
                ResetDanceGroupMemberActivityData(rMsg.m_nGroupID, pMember, true);

                resMsg.m_nDayPacket = pDGInfo->m_DGActivityInfo.m_nDayPacket;
                resMsg.m_nCurrentStarLevel = pDGInfo->m_DGActivityInfo.m_nStarLevel;
                resMsg.m_nCurrentPowerValue = pDGInfo->m_DGActivityInfo.m_nCurrentPowerValue;
                resMsg.m_nNormalCount = pMember->m_nNormalCount;
                resMsg.m_nTotalNormalCount = CDanceGroupDataMgr::Instance().m_nActivityStarNormalCount;
                resMsg.m_nMoneyCount = pMember->m_nMoneyCount;
                resMsg.m_nMoneyNum = (unsigned short)CDanceGroupDataMgr::Instance().m_nActivityStarMoneyNum;
                resMsg.m_nTotalMoneyCount = CDanceGroupDataMgr::Instance().m_nActivityStarMoneyCount;
                resMsg.m_nPointCount = pMember->m_nPointCount;
                resMsg.m_nMCoinNum = (unsigned short)CDanceGroupDataMgr::Instance().m_nActivityStarPointNum;
                resMsg.m_nTotalMCoinCount = CDanceGroupDataMgr::Instance().m_nActivityStarPointCount;
                resMsg.m_strRuleContent = CDanceGroupDataMgr::Instance().m_strRuleContent;
                resMsg.m_mStarProgress = CDanceGroupDataMgr::Instance().GetStarBaseInfoList();

                bool bResult = CDanceGroupDataMgr::Instance().GetPacketListByDay(resMsg.m_mPacketList, pRoleEntity->m_nSex, (unsigned short)GetWeekDay());

                if (bResult && resMsg.m_mStarProgress.size() == resMsg.m_mPacketList.size())
                {
                    if (!pMember->m_strPacketUnlockList.empty())
                        SplitString(pMember->m_strPacketUnlockList, ",", resMsg.m_listOpenPacket);

                    resMsg.m_listReward = CDanceGroupDataMgr::Instance().GetRewardListBySex((ESexType)pRoleEntity->m_nSex);
                }
                else
                    resMsg.m_nErrorCode = EDanceGroupActivityStar_Failed;
            }
            else
                resMsg.m_nErrorCode = EDanceGroupActivityStar_NotGroupMember;
        }
        else
            resMsg.m_nErrorCode = EDanceGroupActivityStar_DGNotExist;

        pRoleEntity->SendPlayerMsg(&resMsg);
    }
}

void CDanceGroupProcessor::OnDanceGroupActivityAddStarPower(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_DanceGroupActivityAddStarPower &rMsg = dynamic_cast<GameMsg_S2G_DanceGroupActivityAddStarPower &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (NULL != pRoleEntity)
    {
        GameMsg_S2C_DanceGroupActivityAddStarPowerResult resMsg;
        resMsg.m_nType = rMsg.m_nType;
        resMsg.m_nErrorCode = (unsigned short)m_DGManager.CheckDanceGroupActivityStar(pRoleEntity->m_nRoleID, rMsg.m_nGroupID);

        if (EDanceGroupActivityStar_Success != resMsg.m_nErrorCode)
        {
            pRoleEntity->SendPlayerMsg(&resMsg);

            return;
        }

        CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);

        if (NULL != pDGInfo)
        {
            ResetDanceGroupActivityData(pDGInfo, false);

            std::map<unsigned short, CDGActivityStarBaseInfo>::iterator starInfoItr = 
                CDanceGroupDataMgr::Instance().GetStarBaseInfoList().find(pDGInfo->m_DGActivityInfo.m_nStarLevel);

            if (CDanceGroupDataMgr::Instance().GetStarBaseInfoList().end() == starInfoItr)
            {
                resMsg.m_nErrorCode = EDanceGroupActivityStar_Failed;

                pRoleEntity->SendPlayerMsg(&resMsg);

                return;
            }

            CDanceGroupMember *pMember = pDGInfo->GetGroupMember(pRoleEntity->m_nRoleID);
            resMsg.m_nStarLevel = pDGInfo->m_DGActivityInfo.m_nStarLevel;
            resMsg.m_nCurrentPowerValue = (unsigned short)pDGInfo->m_DGActivityInfo.m_nCurrentPowerValue;

            if (NULL != pMember)
            {
                ResetDanceGroupMemberActivityData(rMsg.m_nGroupID, pMember, false);

                unsigned int nAddPower = 0;
                int nAddContribution = 0;

                switch (rMsg.m_nType)
                {
                case EDanceGroupActivityStarType_Normal:
                    {
                        if (pMember->m_nNormalCount < CDanceGroupDataMgr::Instance().m_nActivityStarNormalCount)
                        {
                            pMember->m_nNormalCount++;
                            nAddPower = CDanceGroupDataMgr::Instance().m_nActivityStarNormalPower;
                            nAddContribution = CDanceGroupDataMgr::Instance().m_nActivityStarNormalContribution;
                        }
                        else
                            resMsg.m_nErrorCode = EDanceGroupActivityStar_NoCount;
                    }
                    break;

                case EDanceGroupActivityStarType_Money:
                case EDanceGroupActivityStarType_Point2Money:
                    {
                        if (pMember->m_nMoneyCount < CDanceGroupDataMgr::Instance().m_nActivityStarMoneyCount)
                        {
                            pMember->m_nMoneyCount++;
                            nAddPower = CDanceGroupDataMgr::Instance().m_nActivityStarMoneyPower;
                            nAddContribution = CDanceGroupDataMgr::Instance().m_nActivityStarMoneyContribution;
                        }
                        else
                            resMsg.m_nErrorCode = EDanceGroupActivityStar_NoCount;
                    }
                    break;

                case EDanceGroupActivityStarType_Point:
                    {
                        if (pMember->m_nPointCount < CDanceGroupDataMgr::Instance().m_nActivityStarPointCount)
                        {
                            pMember->m_nPointCount++;
                            nAddPower = CDanceGroupDataMgr::Instance().m_nActivityStarPointPower;
                            nAddContribution = CDanceGroupDataMgr::Instance().m_nActivityStarPointContribution;
                        }
                        else
                            resMsg.m_nErrorCode = EDanceGroupActivityStar_NoCount;
                    }
                    break;

                default:
                    resMsg.m_nErrorCode = EDanceGroupActivityStar_Failed;
                    break;
                }

                if (EDanceGroupActivityStar_Success != resMsg.m_nErrorCode)
                {
                    pRoleEntity->SendPlayerMsg(&resMsg);

                    return;
                }

                unsigned int nOldPower = pDGInfo->m_DGActivityInfo.m_nCurrentPowerValue;
                unsigned int nOldContribution = pMember->m_nContribution;
                unsigned int nOldTotalContribution = pMember->m_nTotalContribution;

                pDGInfo->m_DGActivityInfo.m_nCurrentPowerValue += nAddPower;
                pMember->ChangeContribution(nAddContribution);

                UpdateDanceGroupMemberWithTime(rMsg.m_nGroupID, *pMember);

                SaveDanceGroupLog(pDGInfo->m_DGBaseInfo.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 
                    pMember->m_nRoleID, pMember->m_strRoleName, EDanceGroupLog_MemberContributionAdd, 
                    nAddContribution, nOldContribution, pMember->m_nContribution, "Activity");
                SaveDanceGroupLog(pDGInfo->m_DGBaseInfo.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 
                    pMember->m_nRoleID, pMember->m_strRoleName, EDanceGroupLog_MemberIntergralAdd, 
                    nAddContribution, nOldTotalContribution, pMember->m_nTotalContribution);
                SaveDanceGroupLog(pDGInfo->m_DGBaseInfo.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 
                    pMember->m_nRoleID, pMember->m_strRoleName, EDanceGroupLog_Activity_PowerAdd, 
                    nAddPower, nOldPower, pDGInfo->m_DGActivityInfo.m_nCurrentPowerValue);

                if (pDGInfo->m_DGActivityInfo.m_nCurrentPowerValue >= starInfoItr->second.m_nProgress)
                {
                    pDGInfo->m_DGActivityInfo.m_nStarLevel++;
                    pDGInfo->m_DGActivityInfo.m_nCurrentPowerValue -= starInfoItr->second.m_nProgress;

                    pDGInfo->m_DGBaseInfo.m_nResA += starInfoItr->second.m_nContributionLow;
                    pDGInfo->m_DGBaseInfo.m_nResB += starInfoItr->second.m_nContributionMid;
                    pDGInfo->m_DGBaseInfo.m_nResC += starInfoItr->second.m_nContributionHigh;

                    UpdateDanceGroupBaseInfo(pDGInfo->m_DGBaseInfo, EDanceGroupBaseInfoUpdateType_Data);

                    GameMsg_G2S_UpdateDanceGroupActivityStarState updateMsg;
                    updateMsg.m_nGroupID = rMsg.m_nGroupID;
                    updateMsg.m_nStarLevel = pDGInfo->m_DGActivityInfo.m_nStarLevel;
                    updateMsg.m_nCurrentPower = pDGInfo->m_DGActivityInfo.m_nCurrentPowerValue;

                    SendMsg2DanceGroupMember(rMsg.m_nGroupID, 0, updateMsg);
                }

                SaveUpdateDanceGroupActivityInfo(rMsg.m_nGroupID, pDGInfo->m_DGActivityInfo);

                resMsg.m_nStarLevel = pDGInfo->m_DGActivityInfo.m_nStarLevel;
                resMsg.m_nCurrentPowerValue = (unsigned short)pDGInfo->m_DGActivityInfo.m_nCurrentPowerValue;

                pRoleEntity->SendPlayerMsg(&resMsg);
            }
        }
    }
}

void CDanceGroupProcessor::OnDanceGroupActivityGetStarPowerInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_DanceGroupActivityGetStarPowerInfo &rMsg = dynamic_cast<GameMsg_S2G_DanceGroupActivityGetStarPowerInfo &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (NULL != pRoleEntity)
    {
        GameMsg_S2C_DanceGroupActivityGetStarPowerInfoResult resMsg;
        resMsg.m_nErrorCode = EDanceGroupActivityStar_Success;

        CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);

        if (NULL != pDGInfo)
        {
            resMsg.m_nStarLevel = pDGInfo->m_DGActivityInfo.m_nStarLevel;
            resMsg.m_nCrurrentPower = pDGInfo->m_DGActivityInfo.m_nCurrentPowerValue;
        }
        else
            resMsg.m_nErrorCode = EDanceGroupActivityStar_DGNotExist;

        pRoleEntity->SendPlayerMsg(&resMsg);
    }
}

void CDanceGroupProcessor::OnDanceGroupActivityOpenStarPacket(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_DanceGroupActivityOpenStarPacket &rMsg = dynamic_cast<GameMsg_S2G_DanceGroupActivityOpenStarPacket &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (NULL != pRoleEntity)
    {
        GameMsg_S2C_DanceGroupActivityOpenStarPacketResult resMsg;
        resMsg.m_nStarLevel = rMsg.m_nStarLevel;
        resMsg.m_nErrorCode = EDanceGroupActivityStar_Failed;

        CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);

        if (NULL != pDGInfo)
        {
            ResetDanceGroupActivityData(pDGInfo, true);

            CDanceGroupMember *pMember = pDGInfo->GetGroupMember(pRoleEntity->m_nRoleID);

            if (NULL != pMember && pDGInfo->m_DGActivityInfo.m_nStarLevel > rMsg.m_nStarLevel)
            {
                ResetDanceGroupMemberActivityData(rMsg.m_nGroupID, pMember, false);

                std::string strLevel;
                Formatter(strLevel) << rMsg.m_nStarLevel;

                if (std::string::npos != pMember->m_strPacketUnlockList.find(strLevel))
                {
                    resMsg.m_nErrorCode = EDanceGroupActivityStar_PacketOpened;

                    pRoleEntity->SendPlayerMsg(&resMsg);

                    return;
                }

                std::map<unsigned short, std::list<CItem> > listPacketItem;

                if (CDanceGroupDataMgr::Instance().GetPacketListByDay(listPacketItem, pMember->m_nSex, (unsigned short)GetWeekDay() ) )
                {
                    if (listPacketItem.end() != listPacketItem.find(rMsg.m_nStarLevel))
                    {
                        pMember->m_strPacketUnlockList += ",";
                        pMember->m_strPacketUnlockList += strLevel;

                        if (',' == pMember->m_strPacketUnlockList[0])
                            pMember->m_strPacketUnlockList.erase(0, 1);

                        resMsg.m_nErrorCode = EDanceGroupActivityStar_Success;

                        SaveUpdateDanceGroupMemberWithTime(rMsg.m_nGroupID, *pMember);
                    }
                }
            }
        }

        pRoleEntity->SendPlayerMsg(&resMsg);
    }
}

// void CDanceGroupProcessor::OnDanceGroupShopGetInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
// {
//     GameMsg_S2G_DanceGroupShopGetInfo &rMsg = dynamic_cast<GameMsg_S2G_DanceGroupShopGetInfo &>(msg);
//     CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);
// 
//     if (NULL != pRoleEntity)
//     {
//         GameMsg_G2S_DanceGroupShopGetInfoResult resMsg;
//         resMsg.m_nErrorCode = EDanceGroupShopResult_Success;
// 
//         CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);
// 
//         if (NULL != pDGInfo)
//         {
//             CDanceGroupMember *pMember = pDGInfo->GetGroupMember(pRoleEntity->m_nRoleID);
// 
//             if (NULL != pMember)
//             {
//                 ResetDanceGroupMemberActivityData(rMsg.m_nGroupID, pMember, true);
// 
//                 resMsg.m_mapUseCount = pMember->m_mShopExchangeCount;
//             }
//         }
// 
//         pRoleEntity->SendPlayerMsg(&resMsg);
//     }
// }

// void CDanceGroupProcessor::OnDanceGroupShopExchange(GameMsg_Base &msg, CSlotPeer &slotPeer)
// {
//     GameMsg_S2G_DanceGroupShopExchange &rMsg = dynamic_cast<GameMsg_S2G_DanceGroupShopExchange &>(msg);
//     CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);
// 
//     if (NULL != pRoleEntity)
//     {
//         GameMsg_S2C_DanceGroupShopExchangeResult resMsg;
//         resMsg.m_nIndex = rMsg.m_nIndex;
//         resMsg.m_nErrorCode = EDanceGroupShopResult_Success;
// 
//         CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nGroupID);
// 
//         if (NULL != pDGInfo)
//         {
//             CDanceGroupMember *pMember = pDGInfo->GetGroupMember(pRoleEntity->m_nRoleID);
// 
//             if (NULL != pMember)
//             {
//                 if (EMemberProperty_MengXin != pMember->m_nProperty)
//                 {
//                     ResetDanceGroupMemberActivityData(rMsg.m_nGroupID, pMember, false);
// 
//                     if (pMember->m_nContribution >= rMsg.m_nNeedContribution)
//                     {
//                         std::map<unsigned short, unsigned short>::iterator itrCount = pMember->m_mShopExchangeCount.find(rMsg.m_nIndex);
// 
//                         if (pMember->m_mShopExchangeCount.end() != itrCount)
//                         {
//                             if (itrCount->second < rMsg.m_nTotalCount)
//                             {
//                                 itrCount->second++;
// 
//                                 resMsg.m_nUseCount = itrCount->second;
//                             }
//                             else
//                                 resMsg.m_nErrorCode = EDanceGroupShopResult_LimitExchangCount;
//                         }
//                         else
//                         {
//                             resMsg.m_nUseCount = 1;
// 
//                             pMember->m_mShopExchangeCount.insert(std::make_pair(rMsg.m_nIndex, (unsigned short)(1)));
//                         }
// 
//                         if (EDanceGroupShopResult_Success == resMsg.m_nErrorCode)
//                         {
//                             int nOldContribution = pMember->m_nContribution;
//                             int nValue = rMsg.m_nNeedContribution;
// 
//                             pMember->ChangeContribution(-nValue);
// 
//                             SaveDanceGroupLog(pDGInfo->m_DGBaseInfo.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 
//                                 pMember->m_nRoleID, pMember->m_strRoleName, EDanceGroupLog_MemberContributionDec, 
//                                 nValue, nOldContribution, pMember->m_nContribution, "DGShopExchange");
// 
//                             UpdateDanceGroupMemberWithTime(rMsg.m_nGroupID, *pMember);
//                         }
//                     }
//                     else
//                         resMsg.m_nErrorCode = EDanceGroupShopResult_NotEnoughContribution;
//                 }
//                 else 
//                     resMsg.m_nErrorCode = EDanceGroupShopResult_ShouldNotMengXin;
//             }
//         }
// 
//         pRoleEntity->SendPlayerMsg(&resMsg);
//     }
// }

void CDanceGroupProcessor::OnAcceptDanceGroupMemberInvitation(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_AcceptDanceGroupMemberInvitation &rMsg = dynamic_cast<GameMsg_S2G_AcceptDanceGroupMemberInvitation &>(msg);
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if (pRoleEntity != NULL && rMsg.m_nDanceGroupID != 0)
    {
        GameMsg_G2S_AcceptDanceGroupMemberInvitationResult acceptResMsg;
        acceptResMsg.m_nResult = (unsigned short)m_DGManager.CheckAcceptInvitationCond(rMsg.m_nDanceGroupID, pRoleEntity->GetDanceGroupID() );

        if (acceptResMsg.m_nResult == EAcceptDanceGroupMemberInvitationResult_Success)
        {
            CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(rMsg.m_nDanceGroupID);

            if (pDGInfo != NULL)
            {
                std::string strRecord;
                SafeFormat(strRecord, CLocalization::Instance().GetString("DanceGroup_Record_Invitation"), 
                    rMsg.m_strInviterRoleName.c_str(), pRoleEntity->m_szRoleName);

                SaveDanceGroupRecordsInfo(pDGInfo, pRoleEntity->GetRoleID(), (unsigned int)time(NULL), strRecord);

                CRequestDanceGroupInfo requestInfo;
                requestInfo.m_nRoleID = pRoleEntity->GetRoleID();
                requestInfo.m_strRoleName = pRoleEntity->m_szRoleName;
                requestInfo.m_nSex = pRoleEntity->m_nSex;
                requestInfo.m_nLevel = pRoleEntity->m_nLevel;
                requestInfo.m_bIsVIP = pRoleEntity->m_bIsVIP;
                requestInfo.m_nVIPLevel = pRoleEntity->m_nVIPLevel;

                // add new member
                AddDanceGroupMemberInfo(rMsg.m_nDanceGroupID, requestInfo);

                SaveDanceGroupLog(rMsg.m_nDanceGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 
                    requestInfo.m_nRoleID, requestInfo.m_strRoleName, EDanceGroupLog_MemberJoin);

                // clear requests
                ClearAllRequestsInfoByRoleID(requestInfo.m_nRoleID);

                // notify player
                GameMsg_G2S_EnterDanceGroupSuccess enterSuccessMsg;
                enterSuccessMsg.m_pDGInfo = new CDanceGroupInfo(*pDGInfo);

                pRoleEntity->SendPlayerMsg(&enterSuccessMsg);

                pRoleEntity->SetDanceGroupID(rMsg.m_nDanceGroupID);
            }
        }

        pRoleEntity->SendPlayerMsg(&acceptResMsg);
    }
}

void CDanceGroupProcessor::OnCanBeInviteCheck(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_CanBeInvitedCheck &rmsg = (GameMsg_S2G_CanBeInvitedCheck &)msg;

    DanceGroupQuitInfo *quitInfo = m_DGManager.GetQuitInfo(rmsg.m_nInvitedRole);

    GameMsg_G2S_CanBeInvitedCheck sendMsg;
    sendMsg.m_nInvitedRole = rmsg.m_nInvitedRole;
    sendMsg.m_nRet = EInviteDanceGroupMemberResult_Success;

    if (NULL != quitInfo)
    {
        bool bIsOldGroup = (quitInfo->m_nDanceGroupID == rmsg.m_nToGroupID);
        int needCD = (bIsOldGroup ? CDanceGroupDataMgr::Instance().m_nEnterOldGroupCD : CDanceGroupDataMgr::Instance().m_nEnterNewGroupCD);
        int CD = (int)((time(NULL) - quitInfo->m_nQuitStamp));
        if (CD < needCD)
        {
            sendMsg.m_nRet = EInviteDanceGroupMemberResult_InCD;
        }
    }

    SendPlayerMsgByAccountID(msg.nAccount, sendMsg);
}


void CDanceGroupProcessor::OnAddDanceGroupChallengeActive(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_AddDanceGroupChallengeActive &rMsg = (GameMsg_S2G_AddDanceGroupChallengeActive &)msg;
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    do 
    {
        BREAK_IF( pRoleEntity == NULL );

        CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo( rMsg.m_nGroupID );
        BREAK_IF( pDGInfo == NULL );

        CDanceGroupMember *pMember = pDGInfo->GetGroupMember( pRoleEntity->GetRoleID() );

        ChangeChallengeActive( pDGInfo, pMember, (unsigned int)rMsg.m_nAddValue, true, "Add" );
    } while (0);
}

void CDanceGroupProcessor::OnLoadAllDanceGroupInfo(QueryBase &rQuery)
{
    // 重置仪式举办次数

    std::map<unsigned int, CDanceGroupInfo *> &danceGroupMap = m_DGManager.GetDanceGroupInfoList();
    std::map<unsigned int, CDanceGroupInfo *>::iterator iter = danceGroupMap.begin();
    time_t now = time(NULL);

    for (; iter != danceGroupMap.end(); ++iter)
    {
        CDanceGroupInfo *pInfo = iter->second;
        ENSURE_WITH_LOG_CMD(NULL != pInfo, continue, "error to find null CDanceGroupInfo in map.");
        
        pInfo->m_DGBaseInfo.ResetCeremonyStamp(now); // 重置
    }

    CDanceGroupInfo *pDGInfo = NULL;
    CDanceGroupRankInfo *pDGRankInfo = NULL;

    for (std::list<CDanceGroupInfo *>::const_iterator bIt = m_DGManager.m_GroupList.begin(), eIt = m_DGManager.m_GroupList.end(); 
         bIt != eIt; ++bIt)
    {
        pDGInfo = *bIt;

        if (NULL != pDGInfo)
        {
            pDGRankInfo = new CDanceGroupRankInfo(*pDGInfo);
            if (NULL != pDGRankInfo)
            {
                if (0 == m_DGRankManager.AddNewRankInfo(pDGRankInfo))
                {
                    delete pDGRankInfo;
                    pDGRankInfo = NULL;
                }
            }
        }
    }

    m_bDanceGroupComplete = true;
    RegisterMessageAfterLoaded();
    CeremonyBriefManager::Instance().InitializeAfterLoadDanceGroup();

    LOG_DEBUG << "Dance Group Info Load Complete." << std::endl;
}

void CDanceGroupProcessor::OnDanceGroupCreated(QueryBase &rQuery)
{
    CDanceGroupInfo *pDGInfo = (CDanceGroupInfo *)rQuery.m_pAttachment;

    if (NULL == pDGInfo)
        return;

    unsigned int nRoleID = pDGInfo->m_DGBaseInfo.m_nLeaderRoleID;
    GameMsg_G2S_CreateDanceGroupResult resMsg;
    resMsg.m_nResult = (unsigned short)rQuery.m_nAttachment;

    if (ECreateDanceGroupResult_Success == resMsg.m_nResult)
    {
        CDanceGroupBaseInfo &rBaseInfo = pDGInfo->m_DGBaseInfo;

        SaveDanceGroupLog(rBaseInfo.m_nGroupID, rBaseInfo.m_strGroupName, 
            rBaseInfo.m_nLeaderRoleID, rBaseInfo.m_strLeaderRoleName, 
            EDanceGroupLog_Create, rBaseInfo.m_nBadge);

        resMsg.m_pDGInfo = new CDanceGroupInfo(*pDGInfo);

        if (m_DGManager.AddDanceGroupInfo(pDGInfo))
        {
            CDanceGroupRankInfo *pDGRankInfo = new CDanceGroupRankInfo(*pDGInfo);

            if (NULL != pDGRankInfo)
            {
                if (0 == m_DGRankManager.AddNewRankInfo(pDGRankInfo))
                {
                    delete pDGRankInfo;
                    pDGRankInfo = NULL;
                }
            }

            ClearAllRequestsInfoByRoleID(pDGInfo->m_DGBaseInfo.m_nLeaderRoleID);

            CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByID(nRoleID);

            if (pRoleEntity != NULL)
                pRoleEntity->SetDanceGroupID(rBaseInfo.m_nGroupID);
            GroupRoomProcessor::Instance().OnUpdateDanceGroupName(pRoleEntity, pDGInfo->m_DGBaseInfo.m_strGroupName); // 更新
            CDanceGroupMember *pMember = pDGInfo->GetGroupMember(nRoleID);
            if (NULL != pMember && NULL != pRoleEntity && NULL != pDGInfo)
            {
                GroupRoomProcessor::Instance().OnUpdateDanceGroupTitle(pRoleEntity, pMember->m_nTitle);
                GroupRoomProcessor::Instance().OnUpdateDanceGroupBadgeOrEffect(pRoleEntity, pDGInfo->m_DGBaseInfo.m_nBadge, pDGInfo->m_DGBaseInfo.m_nEffect);
            }
        }
        else
        {
            LOG_ERROR << "Add dance group info failed. GroupID = " 
                      << pDGInfo->m_DGBaseInfo.m_nGroupID << std::endl;

            delete pDGInfo;
            pDGInfo = NULL;
        }
    }
    else
    {
        delete pDGInfo;
        pDGInfo = NULL;

        resMsg.m_pDGInfo = NULL;
    }

    SendPlayerMsgByRoleID(nRoleID, resMsg);

    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByID(nRoleID);
    if (NULL != pGroupRole)
        BigMamaGroup::Instance().OnRoleJoinDanceGroup(*pGroupRole);
}

void CDanceGroupProcessor::OnDanceGroupNameChanged(QueryBase &rQuery)
{
    CChangeDanceGroupNameQuery *pChangeInfo = (CChangeDanceGroupNameQuery *)rQuery.m_pAttachment;
    unsigned int nRoleID = (unsigned int)rQuery.m_nTag;

    if (NULL == pChangeInfo)
        return;

    GameMsg_S2C_ChangeDanceGroupNameResult resMsg;
    resMsg.m_nResult = (unsigned short)rQuery.m_nAttachment;
    resMsg.m_strNewGroupName = pChangeInfo->m_strNewName;

    if (EChangeDanceGroupNameResult_Success == resMsg.m_nResult)
    {
        CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(pChangeInfo->m_nGroupID);

        if (NULL != pDGInfo)
        {
            CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByID(nRoleID);

            if (NULL != pRoleEntity)
            {
                SaveDanceGroupLog(pChangeInfo->m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, nRoleID, 
                    pRoleEntity->m_szRoleName, EDanceGroupLog_NameChange, pDGInfo->m_DGBaseInfo.m_strGroupName.c_str(), 
                    pChangeInfo->m_strNewName.c_str());
            }

            pDGInfo->m_DGBaseInfo.m_strGroupName = pChangeInfo->m_strNewName;
            pDGInfo->m_DGBaseInfo.m_nLastChangeGroupNameTime = pChangeInfo->m_nChangeTime;

            SendUpdateDanceGroupBaseInfo(pDGInfo->m_DGBaseInfo, EDanceGroupBaseInfoUpdateType_Data);

            SendMsg2DanceGroupMember(pChangeInfo->m_nGroupID, 0, resMsg);
            CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(pChangeInfo->m_nGroupID);
            if (NULL != pDGInfo)
            {
                for (std::map<unsigned int, CDanceGroupMember *>::const_iterator bIt = pDGInfo->m_MemberMap.begin(), eIt = pDGInfo->m_MemberMap.end();
                    bIt != eIt; ++bIt)
                {
                    if (NULL != bIt->second)
                    {
                        GroupRoomProcessor::Instance().OnUpdateDanceGroupName(
                            CGroupPlayerManager::Instance().SearchRoleByID(bIt->second->m_nRoleID), 
                            pDGInfo->m_DGBaseInfo.m_strGroupName);
                    }
                }
            }

        }
        else
            LOG_ERROR << "Can not find CDanceGroupInfo" << std::endl;
    }
    else
        SendPlayerMsgByRoleID(nRoleID, resMsg);

    delete pChangeInfo;
    pChangeInfo = NULL;
}

void CDanceGroupProcessor::SaveRequestEnterDanceGroupInfo(unsigned int nGroupID, const CRequestDanceGroupInfo &rRequestInfo)
{
    if (0 == nGroupID)
        return;

    CRequestDanceGroupInfo *pRequestInfo = new CRequestDanceGroupInfo;

    if (NULL != pRequestInfo)
    {
        *pRequestInfo = rRequestInfo;

        g_pQueryMgr->AddQuery(QUERY_DanceGroup_UpdateRequestDanceGroup, nGroupID, pRequestInfo);
    }
}

void CDanceGroupProcessor::SaveRemoveRequestEnterDanceGroupInfo(unsigned int nGroupID, const std::list<unsigned int> &rListRoleID)
{
    if (0 == rListRoleID.size() || 0 == nGroupID)
        return;

    std::list<unsigned int> *pListRoleID = new std::list<unsigned int>;

    if (NULL != pListRoleID)
    {
        *pListRoleID = rListRoleID;

        g_pQueryMgr->AddQuery(QUERY_DanceGroup_RemoveRequestDanceGroup, nGroupID, pListRoleID);
    }
}

void CDanceGroupProcessor::SaveDanceGroupRecordsInfo(CDanceGroupInfo *pDGInfo, unsigned int nRecordRoleID, unsigned int nRecordTime, const std::string &strContent)
{
    if (NULL != pDGInfo && 0 != nRecordRoleID && 0 != nRecordTime)
    {
        CDanceGroupRecordsInfo *pRecordsInfo = new CDanceGroupRecordsInfo;

        if (NULL != pRecordsInfo)
        {
            pRecordsInfo->m_nGroupId = pDGInfo->m_DGBaseInfo.m_nGroupID;
            pRecordsInfo->m_nRoleId = nRecordRoleID;
            pRecordsInfo->m_strContent = strContent;
            pRecordsInfo->m_nRecordTime = nRecordTime;
            pRecordsInfo->m_nDay = 1;

            pDGInfo->m_recordsMap.insert(std::make_pair(nRecordTime, *pRecordsInfo) );

            g_pQueryMgr->AddQuery(QUERY_DanceGroup_AddRecord, pDGInfo->m_DGBaseInfo.m_nGroupID, pRecordsInfo);
        }
    }
}

void CDanceGroupProcessor::SaveDanceGroupLog(unsigned int nGroupID, const std::string &strGroupName, unsigned int nRoleID, const std::string &strRoleName, EDanceGroupLogAction eAction, ...)
{
    if (0 == nGroupID)
        return;

    CDanceGroupLog *pLog = new CDanceGroupLog;

    if (NULL != pLog)
    {
        pLog->m_nGroupId = nGroupID;
        pLog->m_strDGName = strGroupName;
        pLog->m_nRoleId = nRoleID;
        pLog->m_strRoleName = strRoleName;

        va_list argsList;
        va_start(argsList, eAction);
        pLog->ValuesToStr(eAction, argsList);
        va_end(argsList);

        g_pQueryMgr->AddQuery(QUERY_LogDanceGroup, 0, pLog);
    }
}

void CDanceGroupProcessor::SaveAddDanceGroupMemberInfo(unsigned int nGroupID, const CDanceGroupMember &rMemberInfo)
{
    if (0 == nGroupID)
        return;

    CDanceGroupMember *pMemberInfo = new CDanceGroupMember;

    if (NULL != pMemberInfo)
    {
        *pMemberInfo = rMemberInfo;

        g_pQueryMgr->AddQuery(QUERY_DanceGroup_AddDanceGroupMember, nGroupID, pMemberInfo);
    }
}

void CDanceGroupProcessor::SaveChangeDanceGroupBaseInfo(const CDanceGroupBaseInfo &rBaseInfo)
{
    CDanceGroupBaseInfo *pBaseInfo = new CDanceGroupBaseInfo;

    if (NULL != pBaseInfo)
    {
        *pBaseInfo = rBaseInfo;

        g_pQueryMgr->AddQuery(QUERY_DanceGroup_UpdateDanceGroupBaseInfo, 0, pBaseInfo);
    }
}

void CDanceGroupProcessor::SaveChangeDanceGroupNameInfo(unsigned int nGroupID, unsigned int nRoleID, const std::string &strNewName, unsigned int nChangeTime)
{
    if (0 == nGroupID || 0 == nRoleID || 0 == strNewName.size())
        return;

    CChangeDanceGroupNameQuery *pQueryInfo = new CChangeDanceGroupNameQuery;

    if (NULL != pQueryInfo)
    {
        pQueryInfo->m_nGroupID = nGroupID;
        pQueryInfo->m_strNewName = strNewName;
        pQueryInfo->m_nChangeTime = nChangeTime;

        CDelHelper delHelper(pQueryInfo);

        g_pQueryMgr->AddQuery(QUERY_DanceGroup_ChangeDanceGroupName, -1, pQueryInfo, nRoleID, NULL, &delHelper);
    }
}

void CDanceGroupProcessor::SaveUpdateDanceGroupMemberInfo(unsigned int nGroupID, const CDanceGroupMember &rMemberInfo)
{
    if (0 == nGroupID)
        return;

    CDanceGroupMember *pMember = new CDanceGroupMember;

    if (NULL != pMember)
    {
        *pMember = rMemberInfo;

        g_pQueryMgr->AddQuery(QUERY_DanceGroup_UpdateMember, nGroupID, pMember);
    }
}

void CDanceGroupProcessor::SaveRemoveDanceGroupMemberInfo(unsigned int nGroupID, unsigned int nRoleID)
{
    if (0 == nGroupID || 0 == nRoleID)
        return;

    g_pQueryMgr->AddQuery(QUERY_DanceGroup_DelDanceGroupMember, nGroupID, NULL, nRoleID);
}

void CDanceGroupProcessor::SaveUpdateDanceGroupActivityInfo(unsigned int nGroupID, const CDanceGroupActivityInfo &rDGActivityInfo)
{
    if (0 == nGroupID)
        return;

    CDanceGroupActivityInfo *pActivityInfo = new CDanceGroupActivityInfo;

    if (NULL != pActivityInfo)
    {
        *pActivityInfo = rDGActivityInfo;

        g_pQueryMgr->AddQuery(QUERY_DanceGroup_UpdateActivityInfo, nGroupID, pActivityInfo);
    }
}

void CDanceGroupProcessor::SaveUpdateDanceGroupMemberWithTime(unsigned nGroupID, const CDanceGroupMember &rMemberInfo)
{
    if (0 == nGroupID)
        return;

    CDanceGroupMember *pMember = new CDanceGroupMember;

    if (NULL != pMember)
    {
        *pMember = rMemberInfo;

        g_pQueryMgr->AddQuery(QUERY_DanceGroup_UpdateMemberWithTime, nGroupID, pMember);
    }
}

void CDanceGroupProcessor::ClearAllRequestsInfoByRoleID(unsigned int nRoleID)
{
    if (0 == nRoleID)
        return;

    std::list<unsigned int> listRoleID;
    listRoleID.push_back(nRoleID);

    std::set<unsigned int> requestsList = m_DGManager.GetRequestGroupList(nRoleID);

    for (std::set<unsigned int>::const_iterator bIt = requestsList.begin(), eIt = requestsList.end(); 
         bIt != eIt; ++bIt)
    {
        RemoveRequestEnterDanceGroupInfo(*bIt, listRoleID, false, true);
    }
}

void CDanceGroupProcessor::RemoveRequestEnterDanceGroupInfo(unsigned int nGroupID, unsigned int nRoleID, bool bNotifyRequestRole, bool bRemoveFromDG)
{
    if (0 == nGroupID || 0 == nRoleID)
        return;

    std::list<unsigned int> listRoleID;
    listRoleID.push_back(nRoleID);

    RemoveRequestEnterDanceGroupInfo(nGroupID, listRoleID, bNotifyRequestRole, bRemoveFromDG);
}

void CDanceGroupProcessor::RemoveRequestEnterDanceGroupInfo(unsigned int nGroupID, const std::list<unsigned int> &rListRoleID, bool bNotifyRequestRole, bool bRemoveFromDG)
{
    if (0 == nGroupID || 0 == rListRoleID.size())
        return;

    SaveRemoveRequestEnterDanceGroupInfo(nGroupID, rListRoleID);

    GameMsg_G2S_RemoveRequestDanceGroupInfo removeRequestInfo;
    removeRequestInfo.m_nGroupID = nGroupID;

    GameMsg_S2C_BeFefuseRefresh refreshMsg;
    CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(nGroupID);

    if (NULL != pDGInfo)
    {
        CDanceGroupBaseInfo &rBaseInfo = pDGInfo->m_DGBaseInfo;

        refreshMsg.m_DGItem.m_nGroupID = rBaseInfo.m_nGroupID;
        refreshMsg.m_DGItem.m_strGroupName = rBaseInfo.m_strGroupName;
        refreshMsg.m_DGItem.m_nLeaderRoleID = rBaseInfo.m_nLeaderRoleID;
        refreshMsg.m_DGItem.m_strLeaderRoleName = rBaseInfo.m_strLeaderRoleName;
        refreshMsg.m_DGItem.m_nLevel = rBaseInfo.m_nLevel;
        refreshMsg.m_DGItem.m_nBadge = rBaseInfo.m_nBadge;
        refreshMsg.m_DGItem.m_nHonor = rBaseInfo.m_nHonor;
        refreshMsg.m_DGItem.m_strProfile = rBaseInfo.m_strProfile;
        refreshMsg.m_DGItem.m_nCreateTime = rBaseInfo.m_nCreateTime;
        refreshMsg.m_DGItem.m_bCanRequestEnter = rBaseInfo.m_bCanRequestEnter;
        refreshMsg.m_DGItem.m_nMemberCount = (unsigned short)pDGInfo->GetMemberCount();
        refreshMsg.m_DGItem.m_nMaxMemberCount = (unsigned short)GetDanceGroupMaxMemberCount(rBaseInfo.m_nLevel);
        refreshMsg.m_DGItem.m_nEffectID = rBaseInfo.m_nEffect;
    }

    for (std::list<unsigned int>::const_iterator bIt = rListRoleID.begin(), eIt = rListRoleID.end(); 
         bIt != eIt; ++bIt)
    {
        if (0 == *bIt)
            continue;

        m_DGManager.RemoveRequestDanceGroupInfo(nGroupID, *bIt, bRemoveFromDG);

        removeRequestInfo.m_nRoleID = *bIt;

        SendMsg2DanceGroupManager(nGroupID, removeRequestInfo);

        if (bNotifyRequestRole && NULL != pDGInfo)
        {
            refreshMsg.m_DGItem.m_bHaveRequest = pDGInfo->IsInRequestQueue(*bIt);

            SendPlayerMsgByRoleID(*bIt, refreshMsg);
        }
    }
}

unsigned short CDanceGroupProcessor::ProcessRequestEnterDanceGroup(CDanceGroupInfo *pDGInfo, unsigned int nRoleID, const char *pRoleName, unsigned int nRequestRoleID, unsigned int nRecordTime)
{
    unsigned short nResult = EReplyRequestEnterDanceGroupResult_NotInGroup;

    if (NULL == pDGInfo)
    {
        return EReplyRequestEnterDanceGroupResult_GroupNotExist;
    }
    else if (0 != nRoleID && pRoleName != NULL && 0 != nRequestRoleID)
    {
        unsigned int nGroupID = pDGInfo->m_DGBaseInfo.m_nGroupID;
        nResult = (unsigned short)m_DGManager.CheckReplyRequestEnterDanceGroupCond(nRoleID, nGroupID, nRequestRoleID);

        if (EReplyRequestEnterDanceGroupResult_Success != nResult)
            return nResult;

        const CRequestDanceGroupInfo &rRequestInfo = pDGInfo->GetRequestDanceGroupInfo(nRequestRoleID);

        std::string strRecord;
        SafeFormat(strRecord, CLocalization::Instance().GetString("DanceGroup_Record_AddDGMember"), 
            pRoleName, rRequestInfo.m_strRoleName.c_str());

        SaveDanceGroupRecordsInfo(pDGInfo, nRequestRoleID, nRecordTime, strRecord);

        // add new member
        AddDanceGroupMemberInfo(nGroupID, rRequestInfo);

        SaveDanceGroupLog(nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, rRequestInfo.m_nRoleID, rRequestInfo.m_strRoleName, EDanceGroupLog_MemberJoin);

        // clear requests
        ClearAllRequestsInfoByRoleID(nRequestRoleID);

        // notify player
        GameMsg_G2S_EnterDanceGroupSuccess enterSuccessMsg;
        enterSuccessMsg.m_pDGInfo = new CDanceGroupInfo(*pDGInfo);

        // 舞团仪式状态通知：
        CGroupRoleEntity *pRequestRoleEntity = CGroupPlayerManager::Instance().SearchRoleByID(nRequestRoleID);

        if (pRequestRoleEntity != NULL)
        {
            pRequestRoleEntity->SendPlayerMsg(&enterSuccessMsg);

            pRequestRoleEntity->SetDanceGroupID(nGroupID);
            // 发送舞团仪式状态。
            CeremonyBriefManager::Instance().OnPlayerEnterDanceGroup(*pRequestRoleEntity);
            GroupRoomProcessor::Instance().OnUpdateDanceGroupName(pRequestRoleEntity, pDGInfo->m_DGBaseInfo.m_strGroupName);
            CDanceGroupMember* pMember = pDGInfo->GetGroupMember(nRequestRoleID);
            if (NULL != pMember)
                GroupRoomProcessor::Instance().OnUpdateDanceGroupTitle(pRequestRoleEntity, pMember->m_nTitle);
            GroupRoomProcessor::Instance().OnUpdateDanceGroupBadgeOrEffect(pRequestRoleEntity, pDGInfo->m_DGBaseInfo.m_nBadge, pDGInfo->m_DGBaseInfo.m_nEffect);
        }

        // 舞团大妈状态通知：
        if (NULL != pRequestRoleEntity)
        {
            BigMamaGroup::Instance().OnRoleJoinDanceGroup(*pRequestRoleEntity); // 
        }
    }

    return nResult;
}

void CDanceGroupProcessor::ProcessRemoveDanceGroup(unsigned int nGroudID)
{
    g_pQueryMgr->AddQuery(QUERY_DanceGroup_Remove, nGroudID);

    m_DGRankManager.RemoveRankInfo(nGroudID);

    GameMsg_S2C_DanceGroupRemoved removedMsg;

    CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(nGroudID);
    CGroupRoleEntity *pRoleEntity = NULL;

    if (pDGInfo != NULL)
    {
        for (std::map<unsigned int, CDanceGroupMember *>::const_iterator itr = pDGInfo->m_MemberMap.begin(); 
             itr != pDGInfo->m_MemberMap.end(); ++itr)
        {
            if (itr->second == NULL)
                continue;

            pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByID(itr->second->m_nRoleID);

            if (pRoleEntity != NULL)
            {
                pRoleEntity->SendPlayerMsg(&removedMsg);

                pRoleEntity->SetDanceGroupID(0);
                GroupRoomProcessor::Instance().OnUpdateDanceGroupName(pRoleEntity, ""); 
            }
        }
    }

    m_DGManager.RemoveDanceGroupInfo(nGroudID);
    CDanceGroupFairyLandProcessor::Instance().OnDanceGroupDisband(nGroudID);
}

void CDanceGroupProcessor::AddDanceGroupMemberInfo(unsigned int nGroupID, const CRequestDanceGroupInfo &rRequestInfo)
{
    if (0 != nGroupID)
    {
        CDanceGroupMember member;
        member.m_nRoleID = rRequestInfo.m_nRoleID;
        member.m_strRoleName = rRequestInfo.m_strRoleName;
        member.m_nTitle = EGroupTitle_JuniorMember;
        member.m_nSex = rRequestInfo.m_nSex;
        member.m_nLevel = rRequestInfo.m_nLevel;
        member.m_nState = EPlayerState_Offline;
        member.m_nContribution = 0;
        member.m_nTotalContribution = 0;
        member.m_nLastOffLineTime = (unsigned int)time(NULL);
        member.m_bIsVIP = rRequestInfo.m_bIsVIP;
        member.m_nVIPLevel = rRequestInfo.m_nVIPLevel;
        member.m_nInTime = (unsigned int)time(NULL);
        member.m_nProperty = EMemberProperty_MengXin;

        m_DGManager.AddDanceGroupMemberInfo(nGroupID, member);

        SaveAddDanceGroupMemberInfo(nGroupID, member);

        GameMsg_G2S_AddDanceGroupMemberInfo addMemberMsg;
        addMemberMsg.m_nGroupID = nGroupID;
        addMemberMsg.m_MemberInfo = member;

        SendMsg2DanceGroupMember(nGroupID, rRequestInfo.m_nRoleID, addMemberMsg);

        //
        CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(rRequestInfo.m_nRoleID);
        if (NULL != pRole)
        {
            CDanceGroupFairyLandProcessor::Instance().SendFairyLandMsg(*pRole); // 
        }
    }
}

void CDanceGroupProcessor::UpdateDanceGroupMemberInfo(unsigned int nGroupID, const CDanceGroupMember &rMemberInfo)
{
    SaveUpdateDanceGroupMemberInfo(nGroupID, rMemberInfo);

    SendUpdateDanceGroupMemberInfo(nGroupID, 0, rMemberInfo);
}

void CDanceGroupProcessor::UpdateDanceGroupMemberWithTime(unsigned int nGroupID, const CDanceGroupMember &rMemberInfo)
{
    SaveUpdateDanceGroupMemberWithTime(nGroupID, rMemberInfo);

    SendUpdateDanceGroupMemberInfo(nGroupID, 0, rMemberInfo);
}

void CDanceGroupProcessor::RemoveDanceGroupMember(unsigned int nGroupID, unsigned int nRoleID)
{
    SaveRemoveDanceGroupMemberInfo(nGroupID, nRoleID);

    m_DGManager.RemoveDanceGroupMember(nGroupID, nRoleID);

    SendRemoveDanceGroupMemberInfo(nGroupID, nRoleID);

    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByID(nRoleID);

    if (pRoleEntity != NULL)
        pRoleEntity->SetDanceGroupID(0);
}

void CDanceGroupProcessor::UpdateDanceGroupRecordsTime(CDanceGroupInfo *pDGInfo)
{
    if (NULL != pDGInfo)
    {
        time_t tNow = time(NULL);
        time_t tLastProcessTime = pDGInfo->m_DGActivityInfo.m_nRecordProcessTime;

        if (IsTimeIsToday(tLastProcessTime) || tNow <= tLastProcessTime)
            return;

        pDGInfo->m_DGActivityInfo.m_nRecordProcessTime = (unsigned int)tNow;

        for (std::map<unsigned int, CDanceGroupRecordsInfo>::iterator bIt = pDGInfo->m_recordsMap.begin(), 
             eIt = pDGInfo->m_recordsMap.end(); bIt != eIt; )
        {
            bIt->second.m_nDay++;

            if (bIt->second.m_nDay > 3)
                pDGInfo->m_recordsMap.erase(bIt++);
            else
                ++bIt;
        }
    }
}

void CDanceGroupProcessor::ResetDanceGroupActivityData(CDanceGroupInfo *pDGInfo, bool bWriteDB)
{
    if (NULL != pDGInfo)
    {
        time_t tNow = time(NULL);
        time_t tLastProcessTime = pDGInfo->m_DGActivityInfo.m_nProcessTime;

        if (IsTimeIsToday(tLastProcessTime) || tNow <= tLastProcessTime)
            return;

        pDGInfo->m_DGActivityInfo.m_nProcessTime = (unsigned int)tNow;
        pDGInfo->m_DGActivityInfo.m_nCurrentPowerValue = 0;
        pDGInfo->m_DGActivityInfo.m_nStarLevel = 1;

        if (bWriteDB)
            SaveUpdateDanceGroupActivityInfo(pDGInfo->m_DGBaseInfo.m_nGroupID, pDGInfo->m_DGActivityInfo);
    }
}

void CDanceGroupProcessor::ResetDanceGroupMemberActivityData(unsigned int nGroupID, CDanceGroupMember *pMember, bool bWriteDB)
{
    if (NULL != pMember)
    {
        time_t tNow = time(NULL);
        time_t tLastProcessTime = pMember->m_nLastProcessTime;

        if (IsTimeIsToday(tLastProcessTime) || tNow <= tLastProcessTime)
            return;

        pMember->m_nLastProcessTime = (unsigned int)tNow;
        pMember->m_nNormalCount = 0;
        pMember->m_nMoneyCount = 0;
        pMember->m_nPointCount = 0;
        pMember->m_strPacketUnlockList.clear();
        pMember->m_mShopExchangeCount.clear();

        if (bWriteDB)
            UpdateDanceGroupMemberWithTime(nGroupID, *pMember);
    }
}

void CDanceGroupProcessor::UpdateDanceGroupBaseInfo(const CDanceGroupBaseInfo &rBaseInfo, EDanceGroupBaseInfoUpdateType eUpdateType)
{
    SaveChangeDanceGroupBaseInfo(rBaseInfo);

    unsigned int nSrcRank = 0;
    unsigned int nDesRank = 0;
    m_DGRankManager.UpdateRankInfo(rBaseInfo.m_nGroupID, rBaseInfo.m_nHonor, nSrcRank, nDesRank);

    SendUpdateDanceGroupBaseInfo(rBaseInfo, eUpdateType);
}

void CDanceGroupProcessor::SendUpdateDanceGroupBaseInfo(const CDanceGroupBaseInfo &rBaseInfo, EDanceGroupBaseInfoUpdateType eUpdateType)
{
    GameMsg_G2S_UpdateDanceGroupBaseInfo baseInfoMsg;
    baseInfoMsg.m_DGBaseInfo = rBaseInfo;
    baseInfoMsg.m_nUpdateType = (unsigned short)eUpdateType;

    SendMsg2DanceGroupMember(rBaseInfo.m_nGroupID, 0, baseInfoMsg);
}

void CDanceGroupProcessor::SendUpdateDanceGroupMemberInfo(unsigned int nGroupID, unsigned int nExceptRoleID, const CDanceGroupMember &rMemberInfo)
{
    if (0 != nGroupID)
    {
        GameMsg_G2S_UpdateDanceGroupMemberInfo updateMemberInfo;
        updateMemberInfo.m_nGroupID = nGroupID;
        updateMemberInfo.m_MemberInfo = rMemberInfo;

        GroupRoomProcessor::Instance().OnUpdateDanceGroupTitle(CGroupPlayerManager::Instance().SearchRoleByID(rMemberInfo.m_nRoleID), rMemberInfo.m_nTitle);

        SendMsg2DanceGroupMember(nGroupID, nExceptRoleID, updateMemberInfo);
    }
}

void CDanceGroupProcessor::SendRemoveDanceGroupMemberInfo(unsigned nGroupID, unsigned int nRoleID)
{
    if (0 != nGroupID && 0 != nRoleID)
    {
        GameMsg_G2S_RemoveDanceGroupMemberInfo removeMemberInfo;
        removeMemberInfo.m_nGroupID = nGroupID;
        removeMemberInfo.m_nRoleID = nRoleID;

        SendMsg2DanceGroupMember(nGroupID, 0, removeMemberInfo);
    }
}

void CDanceGroupProcessor::ChangeDanceGroupHonor(CDanceGroupInfo *pDGInfo, int nChangeHonor, int nCause, int nPara, std::string &rStrType)
{
    if (NULL == pDGInfo)
        return;

    int nOldValue = pDGInfo->m_DGBaseInfo.m_nHonor;

    pDGInfo->ChangeDanceGroupHonor(nChangeHonor, (EChangeDanceGroupHonorCause)nCause, nPara);

    if (nChangeHonor > 0)
    {
        CGlobalRankListManager::Instance().ChangeDanceGroupRankValue( pDGInfo->m_DGBaseInfo.m_nGroupID, nChangeHonor );

        SaveDanceGroupLog(pDGInfo->m_DGBaseInfo.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 0, "", 
            EDanceGroupLog_HonourAdd, nChangeHonor, nOldValue, pDGInfo->m_DGBaseInfo.m_nHonor, rStrType.c_str() );
    }
    else
    {
        SaveDanceGroupLog(pDGInfo->m_DGBaseInfo.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 0, "", 
            EDanceGroupLog_HonourDec, abs(nChangeHonor), nOldValue, pDGInfo->m_DGBaseInfo.m_nHonor, rStrType.c_str() );
    }
}

void CDanceGroupProcessor::SaveRoleQuitGroupInfo(unsigned int nRoleID, unsigned int nDanceGroupID, unsigned int stamp)
{
    DanceGroupQuitInfo *pQuitInfo = new DanceGroupQuitInfo;
    pQuitInfo->Set(nRoleID, nDanceGroupID, stamp);
    m_DGManager.InsertRoleQuitInfo(*pQuitInfo); // 

    g_pQueryMgr->AddQuery(QUERY_DanceGroup_WriteQuitInfo, 0, pQuitInfo);
}

bool CDanceGroupProcessor::ChangeChallengeActive( CDanceGroupInfo *pDGInfo, 
    const CDanceGroupMember *pMember, unsigned int nValue, bool bAdd, const std::string &rType )
{
    if ( pDGInfo == NULL || pMember == NULL || nValue == 0 || IsTimeIsToday( pMember->m_nInTime ) )
        return false;
    else if ( !bAdd && pDGInfo->m_DGBaseInfo.m_nChallengeActive < nValue )
        return false;

    unsigned int nOldValue = pDGInfo->m_DGBaseInfo.m_nChallengeActive;
    if ( bAdd )
    {
        pDGInfo->m_DGBaseInfo.m_nChallengeActive += nValue;

        if ( pDGInfo->m_DGBaseInfo.m_nChallengeActive > MAX_CHALLENGE_ACTIVE )
        {
            pDGInfo->m_DGBaseInfo.m_nChallengeActive = MAX_CHALLENGE_ACTIVE;
        }
    }
    else
    {
        pDGInfo->m_DGBaseInfo.m_nChallengeActive -= nValue;
    }

    g_pQueryMgr->AddQuery( QUERY_DanceGroupChallenge_UpdateActive, 
        pDGInfo->m_DGBaseInfo.m_nGroupID, 0, pDGInfo->m_DGBaseInfo.m_nChallengeActive );
    SaveDanceGroupLog( pDGInfo->m_DGBaseInfo.m_nGroupID, pDGInfo->m_DGBaseInfo.m_strGroupName, 0, "", 
        bAdd ? EDanceGroupLog_ChallengeActiveAdd : EDanceGroupLog_ChallengeActiveDec, nValue, nOldValue, 
        pDGInfo->m_DGBaseInfo.m_nChallengeActive, rType.c_str());

    GameMsg_G2S_UpdateDanceGroupChallengeActive updataMsg;
    updataMsg.m_nGroupID = pDGInfo->m_DGBaseInfo.m_nGroupID;
    updataMsg.m_nActive = pDGInfo->m_DGBaseInfo.m_nChallengeActive;

    SendMsg2DanceGroupManager(pDGInfo->m_DGBaseInfo.m_nGroupID, updataMsg);

    return true;
}

void CDanceGroupProcessor::ZeroResetCermonyApplyTimes(time_t now)
{
    if (!IsLoadComplete())
        return ;

    std::map<unsigned int, CDanceGroupInfo *> &danceGroupMap = m_DGManager.GetDanceGroupInfoList();
    std::map<unsigned int, CDanceGroupInfo *>::iterator iter = danceGroupMap.begin();

    for (; iter != danceGroupMap.end(); ++iter)
    {
        CDanceGroupInfo *pInfo = iter->second;
        ENSURE_WITH_LOG_CMD(NULL != pInfo, continue, "error to find null CDanceGroupInfo in map.");

        if (pInfo->m_DGBaseInfo.m_nCeremonyTimes != 0 && !IsSameDay(now, pInfo->m_DGBaseInfo.m_nCeremonyStamp))
        {
            pInfo->m_DGBaseInfo.m_nCeremonyTimes = 0;
            pInfo->m_DGBaseInfo.m_nCeremonyStamp = 0; // 清理了

            // 同步到gameServer和广播
            UpdateDanceGroupBaseInfo(pInfo->m_DGBaseInfo, EDanceGroupBaseInfoUpdateType_CermonyTimes); // 时间变更
        }
    }

    return ;
}


void CDanceGroupProcessor::SendMsg2DanceGroupManager(unsigned int nGroupID, GameMsg_Base &msg)
{
    if (0 != nGroupID)
    {
        CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(nGroupID);

        if (NULL != pDGInfo)
        {
            std::list<unsigned int> listManagerID;
            pDGInfo->GetManagerMember(listManagerID);

            for (std::list<unsigned int>::const_iterator bIt = listManagerID.begin(), eIt = listManagerID.end();
                 bIt != eIt; ++bIt)
            {
                SendPlayerMsgByRoleID(*bIt, msg);
            }
        }
    }
}

void CDanceGroupProcessor::SendMsg2DanceGroupMember(unsigned int nGroupID, unsigned int nExceptRoleID, GameMsg_Base &msg)
{
    if (0 != nGroupID)
    {
        CDanceGroupInfo *pDGInfo = m_DGManager.GetDanceGroupInfo(nGroupID);
        if (NULL != pDGInfo)
        {
            for (std::map<unsigned int, CDanceGroupMember *>::const_iterator bIt = pDGInfo->m_MemberMap.begin(), eIt = pDGInfo->m_MemberMap.end(); 
                 bIt != eIt; ++bIt)
            {
                if (NULL != bIt->second && bIt->second->m_nRoleID != nExceptRoleID)
                {
                    SendPlayerMsgByRoleID(bIt->second->m_nRoleID, msg);
                }
            }
        }
    }
}

void CDanceGroupProcessor::SendMailToDanceGroupManager( CDanceGroupInfo &rDGInfo, EMailType eMailType, const std::string &rTitle, const std::string &rContent )
{
    if ( !rTitle.empty() )
    {
        std::list<unsigned int> listManagerID;

        rDGInfo.GetManagerMember(listManagerID);

        for (std::list<unsigned int>::const_iterator bIt = listManagerID.begin(), eIt = listManagerID.end(); 
             bIt != eIt; ++bIt)
        {
            CWMailManager::Instance().SendSystemMailToPlayer( *bIt, eMailType, rTitle, rContent );
        }
    }
}

void CDanceGroupProcessor::SendMailToDanceGroupMember( const CDanceGroupInfo &rDGInfo, EMailType eMailType, const std::string &rTitle, const std::string &rContent )
{
    if ( !rTitle.empty() )
    {
        for (std::map<unsigned int, CDanceGroupMember *>::const_iterator bIt = rDGInfo.m_MemberMap.begin(), eIt = rDGInfo.m_MemberMap.end(); 
             bIt != eIt; ++bIt)
        {
            CWMailManager::Instance().SendSystemMailToPlayer( bIt->first, eMailType, rTitle, rContent );
        }
    }
}

void CDanceGroupProcessor::SendPlayerMsgByRoleID(unsigned int nRoleID, GameMsg_Base &msg)
{
    if (0 != nRoleID)
    {
        CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByID(nRoleID);

        if (NULL != pRoleEntity)
            pRoleEntity->SendPlayerMsg(&msg);
    }
}

void CDanceGroupProcessor::SendPlayerMsgByAccountID(unsigned int nAccountID, GameMsg_Base &msg)
{
    if (0 != nAccountID)
    {
        CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(nAccountID);

        if (NULL != pRoleEntity)
            pRoleEntity->SendPlayerMsg(&msg);
    }
}



