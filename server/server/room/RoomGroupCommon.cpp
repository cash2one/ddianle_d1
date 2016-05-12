#include "RoomGroupCommon.h"
#include "../roomLib/RoomLibBase.h"
#include "../roomLib/RoomDanceInfo.h"
#include "../roomLib/RoomPosManager.h"
#include "../roomLib/GameMsg_S2C_RoomMsg.h"

#include "../dancegroup/DanceGroupProcessor.h"

#include "../group/Global.h"
#include "../../socket/Log.h"



namespace RoomGroupCommon
{
    void SendMsgToAll(LWRoomBase *pRoom, GameMsg_Base &msg, unsigned int nExpectRole)
    {
        std::vector<std::pair<unsigned int, char> > vecListner;
        pRoom->ListenerRole(vecListner);

        for (size_t i = 0; i < vecListner.size(); ++i)
        {
            if (vecListner[i].first != nExpectRole)
            {
                CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByID(vecListner[i].first);
                if (NULL != pGroupRole)
                    pGroupRole->SendPlayerMsg(&msg); // 
            }
        }

        return;
    }


    void OutDancerRole(LWRoomBase *pRoom, LWRoomDanceInfo *pRoomDanceInfo, LWRoomPosManager *pRoomPosManager, 
        unsigned int nKickedRole, char quitTo, ERoomQuitType quitType, unsigned int nKickOperator)
    {
        CGroupRoleEntity *pGroupRoleEntity = CGroupPlayerManager::Instance().SearchRoleByID(nKickedRole);
        ENSURE_WITH_LOG_CMD(NULL != pGroupRoleEntity, return, "not find group role[%u] ent on Quit Room.", nKickedRole);

        ERoleRoomType eQuitRoleRoomType;
        unsigned int nQuitRolePos;
        ENSURE_WITH_LOG_CMD(pRoomPosManager->GetRolePos(pGroupRoleEntity->GetRoleID(), eQuitRoleRoomType, nQuitRolePos),
            return, "can't find role[%u] pos info.", nKickedRole);

        LWDanceRoleInfo* pDancer = pRoomDanceInfo->GetDanceRole(nKickedRole);
        if (NULL == pDancer)
            return; // 

        // clear from 
        pGroupRoleEntity->ClearGroupRoom();
        pRoomDanceInfo->RemoveRole(nKickedRole);
        pRoomPosManager->RemoveRole(nKickedRole);

        if (quitType != RoomQuitType_Logout)
        {
            GameMsg_S2C_QuitRoomSuc msgPlayerResponse;
            msgPlayerResponse.m_chQuitType = (char)quitType;
            msgPlayerResponse.m_chQuitTo = (char)quitTo;
            msgPlayerResponse.m_nRoleIDKicked = nKickOperator;
            pGroupRoleEntity->SendPlayerMsg(&msgPlayerResponse);
        }

        if (pRoomDanceInfo->HasDancer())
        {
            ERoleRoomType eHostRoomType = RoleRoomType_None;
            unsigned int nHostRolePos = 0;
            int nHostRoomState = RoleRoomState_None;

            // 如果没有房主了，要指定一个房主
            if (pGroupRoleEntity->GetRoleID() == pRoom->HostRoleID())
            {
                unsigned int newHost = pRoomPosManager->GetFirstDanceRole(); // 取出来第一个人当房主
                pRoom->SetHost(newHost); //
                pRoomDanceInfo->SetDancerState(newHost, RoleRoomState_Ready);
                pRoomPosManager->GetRolePos(pRoom->HostRoleID(), eHostRoomType, nHostRolePos);
                nHostRoomState = RoleRoomState_Ready;
            }
            else
            {
                pRoomPosManager->GetRolePos(pRoom->HostRoleID(), eHostRoomType, nHostRolePos);
                nHostRoomState = (int)pRoomDanceInfo->GetDancerState(pRoom->HostRoleID());
            }


            GameMsg_S2C_QuitPlayer quitBrod; // 
            quitBrod.m_chQuitType = (char)quitType;
            quitBrod.m_chRoleRoomType = (char)eQuitRoleRoomType;
            quitBrod.m_chRoleRoomPos = (char)nQuitRolePos; // 退出玩家的pos

            quitBrod.m_chHostRoomType = (char)eHostRoomType;
            quitBrod.m_chHostRoomPos = (char)nHostRolePos;
            quitBrod.m_chHostRoomState = (char)nHostRoomState;

            RoomGroupCommon::SendMsgToAll(pRoom, quitBrod, 0);
        }

        return;
    }

    void EncodeWholeRoomInfo(LWRoomBase *pRoom, LWRoomDanceInfo *pRoomDanceInfo, LWRoomPosManager *pRoomPosManager, 
        RoomWholeInfo &rWholeInfo, bool isHost)
    {
        rWholeInfo.m_nRoomID = pRoom->RoomSerial();
        //    rWholeInfo.m_strRoomName = "";
        //    rWholeInfo.m_strRoomPwd = "";
        rWholeInfo.m_bHasPwd = false;
        rWholeInfo.m_bIsHost = isHost;
        //     rWholeInfo.m_chScene = (char)GetS;
        //     rWholeInfo.m_nMusic = (short)m_nChoosedMusicID;
        //     rWholeInfo.m_chMode = (char)m_nChoosedMusicMode;
        //     rWholeInfo.m_chLevel = (char)m_nChoosedMusicLevel;
        rWholeInfo.m_eTeamMode = EDanceRoomTeamMode_None;
        rWholeInfo.m_chRoomColor = 0;

        LWDanceRoomPosInfo *pPosInfo = pRoomPosManager->GetRoomPosInfo(RoleRoomType_Dancer); //
        if (NULL != pPosInfo)
        {
            for (int i = 0; i < MAX_ROOM_PLAYER_COUNT && i < pPosInfo->MaxDancerCount(); ++i)
            {
                rWholeInfo.m_szPosDancer[i] = (char)(pPosInfo->IsPosOpend(i) ? RoomPos_Open : RoomPos_Close);

                PhoneRole phoneRole = pPosInfo->GetDancer(i);
                if (phoneRole.m_nRoleID != 0)
                {
                    LWDanceRoleInfo *pRoleDanceInfo = pRoomDanceInfo->GetDanceRole(phoneRole.m_nRoleID);
                    char nRoleState = (NULL != pRoleDanceInfo ? (char)pRoleDanceInfo->GetState() : (char)RoleRoomState_Wait);
                    RoomPlayerInfo playerInfo;

                    if (EncodePlayerInfo(phoneRole.m_nRoleID, pRoom->HostRoleID() == phoneRole.m_nRoleID, RoleRoomType_Dancer, (char)i, nRoleState, EDanceColorTeam_None, playerInfo))
                    {
                        rWholeInfo.m_lstDancer.push_back(playerInfo);
                    }
                }
            }
        }

    }
    // 通用
    bool EncodePlayerInfo(unsigned int nRoleID, bool isHost, char nRoleType, char nRolePos, char nRoleState, EDanceColorTeam team, RoomPlayerInfo &roomPlayrInfo)
    {
        CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByID(nRoleID);
        if (NULL == pGroupRole)
            return false;

        roomPlayrInfo.m_nRoleID = nRoleID;
        roomPlayrInfo.m_strRoleName = pGroupRole->GetName();
        roomPlayrInfo.m_bIsHost = isHost;
        roomPlayrInfo.m_chRoleType = nRoleType;
        roomPlayrInfo.m_chRolePos = nRolePos;
        roomPlayrInfo.m_chRoleState = nRolePos;
        roomPlayrInfo.m_eColorTeam = team;
        roomPlayrInfo.m_nRoleSex = pGroupRole->GetSex();
        roomPlayrInfo.m_nRoleSkin = pGroupRole->GetSkinColor();

        const CDanceGroupInfo *pDanceGroupInfo = CDanceGroupProcessor::Instance().GetDanceGroupInfo(pGroupRole->GetDanceGroupID());
        if (NULL != pDanceGroupInfo)
        {
            const CDanceGroupMember *pMember = pDanceGroupInfo->GetGroupMember(nRoleID);
            if (NULL != pMember)
            {
                roomPlayrInfo.m_nGroupTitle = pMember->m_nTitle;
            }
            roomPlayrInfo.m_strDanceGroupName = pDanceGroupInfo->m_DGBaseInfo.m_strGroupName;
            roomPlayrInfo.m_nDanceGroupBadge = pDanceGroupInfo->m_DGBaseInfo.m_nBadge;
            roomPlayrInfo.m_nDanceGroupEffect = pDanceGroupInfo->m_DGBaseInfo.m_nEffect;
        }

        roomPlayrInfo.m_bIsVIP = pGroupRole->IsVIP();
        roomPlayrInfo.m_nVIPLevel = pGroupRole->VIPLevel();
        roomPlayrInfo.m_nMoveType = 0; // 未设置
        roomPlayrInfo.m_nTransFormID = pGroupRole->GetTransformID();

        roomPlayrInfo.m_strData = pGroupRole->m_roomImageData.EncodeToString();

        return true;
    }


    void OnRoomDanceAutoStart(LWRoomBase *pRoom, LWRoomDanceInfo *pRoomDanceInfo, LWRoomPosManager *pRoomPosManager)
    {
        std::vector<unsigned int> vecEraseRole;
        pRoomDanceInfo->RemoveNotToStartRole(vecEraseRole);

        // 广播其他人这个消息
        GameMsg_S2C_QuitMany msgOtherResponse;

        for (size_t i = 0; i < vecEraseRole.size(); ++i)
        { // 从posManager中取得位置
            // 被踢人收到这个消息
            ERoleRoomType etype;
            unsigned int pos;
            pRoomPosManager->GetRolePos(vecEraseRole[i], etype, pos);
            msgOtherResponse.m_vecRoleRoomPos.push_back((char)pos);
            pRoomPosManager->RemoveRole(vecEraseRole[i]); // 踢掉这个人

            CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByID(vecEraseRole[i]);

            if (NULL != pGroupRole)
            {
                GameMsg_S2C_QuitRoomSuc msgQuitResponse;
                msgQuitResponse.m_chQuitType = (char)RoomQuitType_StartKick;
                msgQuitResponse.m_chQuitTo = (char)RoomQuitTo_World; // 踢到大厅去
                pGroupRole->SendPlayerMsg(&msgQuitResponse);
            }
        }

        if (pRoomDanceInfo->HasDancer())
        {
            ERoleRoomType eHostRoomType;
            unsigned int nHostPos;
            if (!pRoomPosManager->GetRolePos(pRoom->HostRoleID(), eHostRoomType, nHostPos))
            { // 房主不在了。
                unsigned int nNewHost = pRoomPosManager->GetFirstDanceRole();
                pRoom->SetHost(nNewHost); //
                pRoomPosManager->GetRolePos(nNewHost, eHostRoomType, nHostPos);
            }

            LWDanceRoleInfo *pHostRole = pRoomDanceInfo->GetDanceRole(pRoom->HostRoleID());
            // 其他人广播这个消息
            msgOtherResponse.m_chQuitType = (char)RoomQuitType_StartKick;
            msgOtherResponse.m_chRoleRoomType = (char)RoleRoomType_Dancer;
            //     msgOtherResponse.m_vecRoleRoomPos.assign(vecKickerPos.begin(), vecKickerPos.end());
            msgOtherResponse.m_chHostRoomType = (char)eHostRoomType;
            msgOtherResponse.m_chHostRoomPos = (char)nHostPos;
            msgOtherResponse.m_chHostRoomState = (NULL != pHostRole ? (char)pHostRole->GetState() : (char)RoleRoomState_ToStart);
            //    msgOtherResponse.m_strRoomPwd.assign("");
            SendMsgToAll(pRoom, msgOtherResponse, 0);

            pRoom->OnRoomDanceStartCallBack();
            GameMsg_S2C_StartRoomSuc msgRoomStart;
            SendMsgToAll(pRoom, msgRoomStart, 0);
        }
    }

    void OnRoomDanceAutoEnd(LWRoomBase *pRoom, LWRoomDanceInfo *pRoomDanceInfo, LWRoomPosManager *pRoomPosManager)
    {
        std::vector<unsigned int> vecEraseRole;
        pRoomDanceInfo->RemoveNotToEndRole(vecEraseRole);

        // 广播其他人这个消息
        GameMsg_S2C_QuitMany msgOtherResponse;

        for (size_t i = 0; i < vecEraseRole.size(); ++i)
        { // 从posManager中取得位置
            // 被踢人收到这个消息
            ERoleRoomType etype;
            unsigned int pos;
            pRoomPosManager->GetRolePos(vecEraseRole[i], etype, pos);
            msgOtherResponse.m_vecRoleRoomPos.push_back((char)pos);
            pRoomPosManager->RemoveRole(vecEraseRole[i]); // 踢掉这个人

            CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByID(vecEraseRole[i]);

            if (NULL != pGroupRole)
            {
                GameMsg_S2C_QuitRoomSuc msgQuitResponse;
                msgQuitResponse.m_chQuitType = (char)RoomQuitType_EndKick;
                msgQuitResponse.m_chQuitTo = (char)RoomQuitTo_World; // 踢到大厅去
                pGroupRole->SendPlayerMsg(&msgQuitResponse);
            }
        }

        if (pRoomDanceInfo->HasDancer())
        {
            ERoleRoomType eHostRoomType;
            unsigned int nHostPos;
            if (!pRoomPosManager->GetRolePos(pRoom->HostRoleID(), eHostRoomType, nHostPos))
            { // 房主不在了。
                unsigned int nNewHost = pRoomPosManager->GetFirstDanceRole();
                pRoom->SetHost(nNewHost); //
                pRoomPosManager->GetRolePos(nNewHost, eHostRoomType, nHostPos);
            }

            LWDanceRoleInfo *pHostRole = pRoomDanceInfo->GetDanceRole(pRoom->HostRoleID());
            // 其他人广播这个消息
            msgOtherResponse.m_chQuitType = (char)RoomQuitType_EndKick;
            msgOtherResponse.m_chRoleRoomType = (char)RoleRoomType_Dancer;
            //     msgOtherResponse.m_vecRoleRoomPos.assign(vecKickerPos.begin(), vecKickerPos.end());
            msgOtherResponse.m_chHostRoomType = (char)eHostRoomType;
            msgOtherResponse.m_chHostRoomPos = (char)nHostPos;
            msgOtherResponse.m_chHostRoomState = (NULL != pHostRole ? (char)pHostRole->GetState() : (char)RoleRoomState_ToEnd);
            //    msgOtherResponse.m_strRoomPwd.assign("");
            RoomGroupCommon::SendMsgToAll(pRoom, msgOtherResponse, 0);

            pRoom->OnRoomDanceEndCallBack(); // 房间结束消息
        }
    }

    void QuitAllDanceRoomRole(LWRoomBase *pRoom, LWRoomDanceInfo *pRoomDanceInfo, LWRoomPosManager *pRoomPosManager)
    {
        ENSURE_WITH_LOG_CMD(NULL != pRoom, return, "should not get null room ptr on QuitAllDanceRoomRole");
        ENSURE_WITH_LOG_CMD(NULL != pRoomDanceInfo, return, "should not get null room danceinfo ptr on QuitAllDanceRoomRole");
        ENSURE_WITH_LOG_CMD(NULL != pRoom, return, "should not get null room posManager ptr on QuitAllDanceRoomRole");
        // 遍历所有人，退出房间
        std::vector<std::pair<unsigned int, char> > vecListner;
        pRoom->ListenerRole(vecListner);

        // notice self
        GameMsg_S2C_QuitRoomSuc msgSelfResponse;
        msgSelfResponse.m_chQuitType = RoomQuitType_NoRoom; // 房间被销毁了。
        msgSelfResponse.m_chQuitTo = (char)RoomQuitTo_World; // 踢出去
        for (size_t i = 0; i < vecListner.size(); ++i)
        {
            // 广播退出消息
            CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByID(vecListner[i].first);
            ENSURE_WITH_LOG_CMD(NULL != pGroupRole, continue, "error get role[%u] group entity on destory room", vecListner[i]);

            // 玩家设置为没有房间的状态
            pGroupRole->ClearGroupRoom();
            pGroupRole->SendPlayerMsg(&msgSelfResponse);
        }

        pRoomDanceInfo->Clear();
        pRoomPosManager->ClearRoomPosInfo();
        
        return;
    }


} // end namespace.




