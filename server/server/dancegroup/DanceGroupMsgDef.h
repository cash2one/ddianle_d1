#ifndef __DANCE_GROUP_MSG_DEF_H__
#define __DANCE_GROUP_MSG_DEF_H__

#include "../share/CommonMsgDef.h"

//������Ϣ
enum eMSG_DanceGroup
{
	MSG_DANCEGROUP_BEGIN =  MSG_TYPE_DANCEGROUP,

	// C2S
	MSG_C2S_RequestMyDanceGroupInfo,				//�����Լ���������Ϣ
	MSG_C2S_CreateDanceGroup,						//��������
	MSG_C2S_RequestEnterDanceGroup,					//����
	MSG_C2S_CancelRequestEnterDanceGroup,			//ȡ������
	MSG_C2S_ReplyRequestEnterDanceGroup,			//��������
	MSG_C2S_ReplyRequestListEnterDanceGroup,		//�����������빦��
	MSG_C2S_GetDanceGroupInfo,						//��ȡ������Ϣ
	MSG_C2S_GetDanceGroupInfoList,					//��ȡ������Ϣ�б�
	MSG_C2S_ExitDanceGroup,							//�˳�����
	MSG_C2S_ChangeDanceGroupLeader,					//���Ŵ�λ
	MSG_C2S_DismissDanceGroup,						//��ɢ����
	MSG_C2S_CancelDismissDanceGroup,				//ȡ����ɢ����
	MSG_C2S_ChangeDanceGroupTitle,					//�ı�����ְλ
	MSG_C2S_KickOutDanceGroupMember,				//�������ų�Ա
	MSG_C2S_ChangeDanceGroupColor,					//�޸�������ɫ
	MSG_C2S_ChangeDanceGroupBadge,					//�޸����Ż���
	MSG_C2S_ChangeDanceGroupTitleName,				//�޸�����ְλ����
	MSG_C2S_ChangeDanceGroupProfile,				//�޸����ż��
	MSG_C2S_ChangeDanceGroupAnnouncement,			//�޸����Ź���
	MSG_C2S_GetDanceGroupBadgeInfos,				//��ȡ�����Ż���Ϣ
	MSG_C2S_UnlockDanceGroupBadge,					//�����Ż���Ч(����)
	MSG_C2S_ChangeDanceGroupBadgeOrEffect,			//�޸����Ż��»�Ч��
	MSG_C2S_DanceGroupActivityGetStarInfo,			//��ȡ���Ż������Ϣ
	MSG_C2S_DanceGroupActivityGetStarPowerInfo,
	MSG_C2S_DanceGroupActivityAddStarPower,			//���Ż���Ӷ���ֵ����
	MSG_C2S_DanceGroupActivityAddStarPowerPoint2Money,
	MSG_C2S_DanceGroupActivityOpenStarPacket,		//���Ż�����
	MSG_C2S_DanceGroupShopGetInfo,					//�����̵���Ϣ��ȡ
	MSG_C2S_DanceGroupShopExchange,					//�����̵���Ʒ�һ�
	MSG_C2S_GetDanceGroupRecords,					//��ȡ���ż�¼

	// S2C
	MSG_S2C_CreateDanceGroupResult,					//�������Ž��
	MSG_S2C_RequestEnterDanceGroupResult,			//����ظ�
	MSG_S2C_CancelRequestEnterDanceGroupResult,		//ȡ������ظ�
	MSG_S2C_EnterDanceGroupSuccess,					//�ɹ���������
	MSG_S2C_GetDanceGroupInfoResult,				//��ȡ���Ž��
	MSG_S2C_SendDanceGroupInfo,						//����������Ϣ
	MSG_S2C_SendSimpleDanceGroupInfo,				//���ͼ�������Ϣ
	MSG_S2C_SendDanceGroupInfoList,					//����������Ϣ�б�
	MSG_S2C_ExitDanceGroupResult,					//�˳����Ž��
	MSG_S2C_ChangeDanceGroupLeaderResult,			//���Ŵ�λ���
	MSG_S2C_DismissDanceGroupResult,				//��ɢ���Ž��
	MSG_S2C_CancelDismissDanceGroupResult,			//ȡ����ɢ���Ž��
	MSG_S2C_ChangeDanceGroupTitleResult,			//�ı�����ְλ���
	MSG_S2C_KickOutDanceGroupMemberResult,			//�������ų�Ա���
	MSG_S2C_KickedOutDanceGroup,					//������������
	MSG_S2C_ChangeDanceGroupColorResult,			//�޸�������ɫ���
	MSG_S2C_ChangeDanceGroupBadgeResult,			//�޸����Ż��½��
	MSG_S2C_ChangeDanceGroupTitleNameResult,		//�޸�����ְλ���ƽ��
	MSG_S2C_ChangeDanceGroupProfileResult,			//�޸����ż����
	MSG_S2C_ChangeDanceGroupAnnouncementResult,		//�޸����Ź�����
	MSG_S2C_DanceGroupRemoved,						//���ű��Ƴ�
	MSG_S2C_UpdateDanceGroupBaseInfo,				//���Ż�����Ϣ���
	MSG_S2C_AddDanceGroupMember,					//���ų�Ա��Ϣ����
	MSG_S2C_UpdateDanceGroupMemberInfo,				//���ų�Ա��Ϣ���
	MSG_S2C_RemoveDanceGroupMember,					//���ų�Ա�Ƴ�
	MSG_S2C_NotifyRequestEnterDanceGroup,			//֪ͨ������������������
	MSG_S2C_NotifyRemoveRequestEnter,				//֪ͨ���Ƴ�����
//	MSG_S2C_UpdateDanceGroupName,					//֪ͨ�������ַ������
//	MSG_S2C_UpdateDanceGroupTitle,					//֪ͨ����ְλ��Ϣ�������
	MSG_S2C_ReplyRequestEnterDanceGroupResult,		//����������
	MSG_S2C_ReplyRequestListEnterDanceGroupResult,	//��������������
	MSG_S2C_GetDanceGroupBadgeInfosResult,			//��ȡ�Ż���Ϣ���
	MSG_S2C_UnlockDanceGroupBadgeResult,			//�����Ż���Ч(����)���
//	MSG_S2C_ChangeDanceGroupBadgeOrEffectResult,	//�޸Ļ��»�Ч�����
	MSG_S2C_DanceGroupActivityGetStarInfoResult,	//���Ż��û�����Ϣ
	MSG_S2C_DanceGroupActivityGetStarPowerInfoResult,
	MSG_S2C_DanceGroupActivityAddStarPowerResult,	//���Ż���Ӷ���ֵ����
	MSG_S2C_DanceGroupActivityStarAddPowerResultMCoin2MoneyResult,
	MSG_S2C_NotifyDanceGroupActivityStarState,
	MSG_S2C_DanceGroupActivityOpenStarPacketResult,	//���Ż�����
	MSG_S2C_DanceGroupActivityReset, //���������û��Ϣ
	MSG_S2C_DanceGroupShopGetInfoResult,			//�����̵���Ϣ��ȡ
	MSG_S2C_DanceGroupShopExchangeResult,			//�����̵�һ�
	MSG_S2C_GetDanceGroupRecrodsResult,
	MSG_S2C_DanceGroupAddRecordsNotify,//�����̵�֪ͨ
	MSG_S2C_DanceGroupChangeDayNotify,//ÿ�����һ�����ż�¼
	MSG_S2C_DanceGroupBeRefuseRefresh,// ���뱻�ܾ�ˢ�´���������Ϣ

	//added
	MSG_C2S_DonateDanceGroupResource,				//������Դ
	MSG_C2S_UpgradeDanceGroup,						//��������
	MSG_S2C_DonateDanceGroupResourceResult,			//������Դ���
	MSG_S2C_UpgradeDanceGroupResult,				//�������Ž��
	MSG_C2S_ChangeDanceGroupName,					//�޸���������
	MSG_S2C_ChangeDanceGroupNameResult,				//�޸��������ֽ��

	MSG_S2C_UpdateDanceGroupAnnounce,				//�������Ź���
	MSG_S2C_UpdateDanceGroupProfile,				//�������ż��
	MSG_S2C_DanceGroupInfo,							//��������ȫ����Ϣ

    // �������ų�Ա
    MSG_C2S_InviteDanceGroupMember,                 // �������ų�Ա
    MSG_S2C_InviteDanceGroupMemberResult,           // �������ų�Ա���
    MSG_S2C_BeInvitedAsDanceGroupMember,            // �������������
    MSG_C2S_AcceptDanceGroupMemberInvitation,       // �������ų�Ա����
    MSG_S2C_AcceptDanceGroupMemberInvitationResult, // �������ų�Ա������

	// liguanghao add, ������ʽ
    MSG_S2C_SyncCeremonyConfig,                     //������ʽ������Ϣ
    MSG_S2C_SyncCermonyApplyTimes,                  //ͬ��������ʽ�Ѿ��������
    MSG_C2S_RequestApplyCeremony,					//����������ʽ
	MSG_S2C_RequestApplyCeremonyRes,				//����������ʽ���
	MSG_S2C_SyncCeremonyState,						//ͬ��������ʽ״̬

	MSG_C2S_RequestEnterCeremony,					//���������ʽ����
	MSG_S2C_EnterCeremonyRes,						//������ʽ������

	MSG_C2S_RequestQuitCeremony,					//�����˳���ʽ����
	MSG_S2C_QuitCeremonyRes,						//�˳���ʽ������

	MSG_C2S_RequestRefreshCeremony,					//����ˢ����ʽ����
	MSG_S2C_RefreshCeremonyRes,						//ˢ����ʽ������

	MSG_C2S_RequestStartCeremony,					//����ʼ��ʽ
	MSG_S2C_StartCeremonyRes,						//��ʼ��ʽ���
    MSG_S2C_CeremonyRoomStartFail,                  //�Զ���ʽ����ʧ��          

	MSG_S2C_OtherPlayerEnterCeremony,				//������ҽ�����ʽ����
	MSG_S2C_OtherPlayerQuitCeremony,				//��������˳���ʽ����

	MSG_C2S_SetCeremonyRookie,						//��Ϊ��ʽ����
	MSG_S2C_SetCeremonyRookieRes,					//��Ϊ��ʽ���˽��
	MSG_C2S_CancelCeremonyRookie,					//ȡ����ʽ����
	MSG_S2C_CancelCeremonyRookieRes,				//ȡ����ʽ���˽��
	MSG_S2C_SyncCeremonyRookie,						//ͬ����ʽ������Ϣ

	MSG_C2S_KickCeremonyPlayer,						//����������ʽ����
	MSG_S2C_KickCeremonyPlayerRes,					//����������ʽ������

	MSG_C2S_SaveCeremonyIntruduction,				//�������ҽ���
    MSG_S2C_SaveCeremonyIntruductionRes,            
	MSG_C2S_RequestCeremonyRecord,					//������ʽ��־
	MSG_S2C_RequestCeremonyRecordRes,				//������ʽ��־���
	MSG_C2S_RequestCeremonyPraise,
	MSG_S2C_RequestCeremonyPraiseRes,
	MSG_C2S_RequestPraiseRookie,
	MSG_S2C_RequestPraiseRookieRes,
    MSG_S2C_SyncPraiseInfo,                         // ͬ��������Ϣ

    // ��������
    MSG_S2C_SyncChallengeConfig,                    // ͬ������������Ϣ
    MSG_C2S_RequestEnterChallenge,                  // �������
    MSG_S2C_RequestEnterChallengeRes,
    MSG_C2S_RequestChallengeStoreyList,             // ��������б�
    MSG_S2C_RequestChallengeStoreyListRes,
    MSG_C2S_RequestChallengeStoreyInfo,             // ������ϸ��Ϣ
    MSG_S2C_RequestChallengeStoreyInfoRes,
    MSG_C2S_RequestChallengeRankList,               // ��������
    MSG_S2C_RequestChallengeRankListRes,
    MSG_C2S_RequestGetChallengeReward,              // �����ȡ����
    MSG_S2C_RequestGetChallengeRewardRes,
    MSG_S2C_ChallengeRewardNotice,                  // ���Ի�ȡ����֪ͨ
    MSG_C2S_RequestChallengeInspire,                // �������
    MSG_S2C_RequestChallengeInspireRes,
    MSG_C2S_RequestAddChallengeChance,              // �������Ӵ���
    MSG_S2C_RequestAddChallengeChanceRes,
    MSG_S2C_RefreshChallengeChanceNotice,           // ˢ�´���֪ͨ
    MSG_C2S_RequestOpenChallengeStorey,             // ������
    MSG_S2C_RequestOpenChallengeStoreyRes,
    MSG_C2S_RequestPrepareChallenge,                // ����׼����ս
    MSG_S2C_RequestPrepareChallengeRes,
    MSG_C2S_RequestStartChallenge,                  // ����ʼ��ս
    MSG_S2C_RequestStartChallengeRes,
    MSG_S2C_ChallengeStageEnd,                      // �ؿ�����
    MSG_S2C_ChallengeStoreyFinishedNotice,          // ��ǰ�����֪ͨ
    MSG_S2C_ChallengeClosedNotice,                  // �����ر�֪ͨ


    // ���Ŵ��裺
    MSG_S2C_SyncBigMamaConfig,
    MSG_C2S_RequestBigMamaStageInfo,
    MSG_S2C_SyncBigMamaStageInfo,
    MSG_C2S_RequestFightBigMama,
    MSG_S2C_RequestFightBigMamaRes,
    MSG_C2S_RequestGetBigMamaReward,
    MSG_S2C_RequestGetBigMamaRewardRes,
    MSG_C2S_CompleteBigMamaGuide,
    MSG_C2S_RequestBigMamaGuideReward,
    MSG_S2C_NoticeBigMamaState,
    MSG_S2C_BigMamaIntroduce,
};

enum EMsgDanceGroupServer
{
    MSG_DANCEGROUP_SERVER_BEGIN = MSG_TYPE_DANCEGROUP_SERVER,
    // S2G
    MSG_S2G_CreateDanceGroup,						//��������
    MSG_S2G_RequestEnterDanceGroup,					//����
    MSG_S2G_RequestEnterDanceGroupByName,           //������ר�ã�ͨ��groupName��������
    MSG_S2G_ReplyRequestEnterDanceGroup,			//��������
    MSG_S2G_ReplyRequestListEnterDanceGroup,//������������
    MSG_S2G_ExitDanceGroup,							//�˳�����
    MSG_S2G_ChangeDanceGroupLeader,					//���Ŵ�λ
    MSG_S2G_DismissDanceGroup,						//��ɢ����
    MSG_S2G_CancelDismissDanceGroup,				//ȡ����ɢ����
    MSG_S2G_ChangeDanceGroupTitle,					//�ı�����ְλ
    MSG_S2G_KickOutDanceGroupMember,				//�������ų�Ա
    MSG_S2G_ChangeDanceGroupColor,					//�޸�������ɫ
    MSG_S2G_ChangeDanceGroupBadge,					//�޸����Ż���
    MSG_S2G_ChangeDanceGroupTitleName,				//�޸�����ְλ����
    MSG_S2G_ChangeDanceGroupProfile,				//�޸����ż��
    MSG_S2G_ChangeDanceGroupAnnouncement,			//�޸����Ź���
    MSG_S2G_UpdateDanceGroupMemberInfo,				//���³�Ա��Ϣ
    MSG_S2G_DonateDanceGroupResource,				//������Դ
    MSG_S2G_UpgradeDanceGroup,						//��������			
    MSG_S2G_ChangeDanceGroupName,					//�޸���������
    MSG_S2G_ChangeDanceGroupHonor,					//�޸���������
    MSG_S2G_UnlockDanceGroupBadge,					//�����Ż�
    MSG_S2G_ChangeDanceGroupBadgeOrEffect,			//�޸Ļ��»�Ч��
    MSG_S2G_DanceGroupTuanhuiPerDayConsume,//�����Ż�ÿ�����Ĳ���
    MSG_S2G_DanceGroupActivityAddStarPower,			//���Ż���Ӷ���ֵ
    MSG_S2G_DanceGroupActivityOpenStarPacket,		//���������
    MSG_S2G_UpdateDanceGroupActivityReward,
    MSG_S2G_DanceGroupShopExchange,					//�����̵�һ�
    MSG_S2G_DanceGroupShopResetExchangeCount,//�����̵����öһ�����
    MSG_S2G_GetDanceGroupRecords,					//������ż�¼
    MSG_S2G_DanceGroupAddRecord, //������ż�¼
    MSG_S2G_AddDanceGroupContribution,				//�������Ź���ֵ
    MSG_S2G_ReduceDanceGroupContribution,			// �������Ź���ֵ
    MSG_S2G_GetDanceGroupInfo,                      // ��ȡ������Ϣ
    MSG_S2G_GetMyDanceGroupInfo,                    // ��ȡ������Ϣ
    MSG_S2G_DanceGroupActivityGetStarInfo,          // ��ȡ����������Ϣ
    MSG_S2G_DanceGroupActivityGetStarPowerInfo,     // ��ȡ���Ŷ���ֵ
    MSG_S2G_DanceGroupShopGetInfo,                  // ��ȡ�����̵���Ϣ
    MSG_S2G_AcceptDanceGroupMemberInvitation,       // �������ų�Ա����
    MSG_S2G_AddDanceGroupChallengeActive,           // ����������������ֵ
    MSG_S2G_CanBeInviteCheck,                       // �������Ƿ���Խ�������

    // G2S
    MSG_G2S_CreateDanceGroupResult,					//�������Ž��
    MSG_G2S_EnterDanceGroupSuccess,					//�ɹ���������
    MSG_G2S_DonateDanceGroupResourceResult,			//������Դ���
    MSG_G2S_UpdateDanceGroupActivityStarState,		//�������Ż(���Ǳ��ʱ�����)
    MSG_G2S_UpdateDanceGroupActivityInfo,
    MSG_G2S_DanceGroupActivityReset,
    MSG_G2S_DanceGroupAddRecordResult, //������ż�¼
    MSG_G2S_DanceGroupShopGetInfoResult,

    MSG_G2S_UpdateDanceGroupBaseInfo,				//�������Ż�����Ϣ
    MSG_G2S_AddDanceGroupMemberInfo,				//������ų�Ա��Ϣ
    MSG_G2S_UpdateDanceGroupMemberInfo,				//�������ų�Ա��Ϣ
    MSG_G2S_RemoveDanceGroupMemberInfo,				//�Ƴ����ų�Ա��Ϣ
    MSG_G2S_AddRequestDanceGroupInfo,				//�������������Ϣ
    MSG_G2S_RemoveRequestDanceGroupInfo,			//�Ƴ�����������Ϣ
    MSG_G2S_GetMyDanceGroupInfo,                    // ��ȡ������Ϣ
    MSG_G2S_AcceptDanceGroupMemberInvitationResult, // �������ų�Ա������
    MSG_G2S_BroadcastMemberKicked,                  // �㲥��ұ��߳�����Ϣ
    MSG_G2S_BroadcastMemberQuit,                    // �㲥����˳����ŵ���Ϣ
    MSG_G2S_UpdateDanceGroupChallengeActive,        // ����������������ֵ
    MSG_G2S_CanBeInviteCheck,                       // �������Ƿ���Խ������Żظ�
    MSG_G2S_ChangeDanceGroupBadgeOrEffectResult,	//�޸Ļ��»�Ч�����

    // ligh add ������ʽ
    MSG_S2G_CeremonyApplyCreate,					// ���봴������
    MSG_G2S_CeremonyApplyCreateRet,					// ���봴������
    MSG_S2G_CeremonyCreateRoomNotify,				// ��������֪ͨ�������ɹ���ʱ��
    MSG_S2G_CeremonyApplyCreateCancel,				// ȡ�����루����ʧ�ܵ�ʱ��
    MSG_S2G_CeremonyCloseRoomNotify,				// ����ر�����
    MSG_S2G_CeremonyRoomStateChange,				// ����״̬�ı�
    MSG_G2S_CeremonyRoomStateChange,				// ����״̬�ı�
    MSG_S2G_CeremonyRoomNewMember,				    // ����֪ͨ
    // ��ʼ����,ͶƱ��Ϣ,���ͽ���
    MSG_S2G_CeremonyRoomLogsStart,				    // ��־��¼
    MSG_S2G_CeremonyRoomLogsItem,                   // ÿ���˵�ͶƱ��Ϣ
    MSG_S2G_CeremonyRoomLogsEnd,                    // ���ͽ���
    MSG_S2G_CeremonyRoomStatics,                    // ����glbal����ͳ��
    MSG_G2S_CeremonyRoomStatics,                    // �ظ�����ͳ��
    Msg_S2G_CeremonySyncRoomInfo,                   // ��ʽ������Ϣͬ����	


    // ��������
    MSG_S2G_EnterChallenge,                         // ������������
    MSG_G2S_EnterChallengeRes,
    MSG_S2G_GetChallengeStoreyInfo,                 // ��ȡ��������Ϣ
    MSG_G2S_GetChallengeStoreyInfoRes,
    MSG_S2G_GetChallengeRankList,                   // ��ȡ���а���Ϣ
    MSG_G2S_GetChallengeRankListRes,
    MSG_S2G_ChallengeInspire,                       // ����
    MSG_G2S_ChallengeInspireRes,
    MSG_S2G_OpenChallengeStorey,                    // ����
    MSG_G2S_OpenChallengeStoreyRes,
    MSG_S2G_PrepareChallenge,                       // ׼����ս
    MSG_G2S_PrepareChallengeRes,
    MSG_S2G_ReportChallengeScore,                   // �ύ��ս����
    MSG_G2S_ReportChallengeScoreRes,
    MSG_G2S_ChallengeStoreyFinishedNotice,          // ��ǰ�����֪ͨ
    MSG_G2S_ChallengeRewardNotice,                  // ���Ի�ȡ����֪ͨ

    // ���Ŵ����ڲ���Ϣ��
    MSG_S2G_BigMamaStageInfoRequest,                // �������ؿ���Ϣ
    MSG_G2S_BigMamaStageInfoNotify,                 // �ظ��ؿ���Ϣ
    MSG_S2G_BigMamaBeatSucess,                      // ����ɹ���Ϣ
    MSG_G2S_BigMamaStateNotify,                     // ������״̬���֪ͨ
    MSG_G2S_BigMamaSyncStageInfo,                   // �����µĹؿ���Ϣ
    MSG_G2S_BigMamaInfoNotifyOnLogin,               // ��������
    MSG_G2S_BigMamaInfoNotifyOnJoinDanceGroup,      // ��������
    MSG_S2G_BigMamaFightRequest,                    // ����ս��
    MSG_G2S_BigMamaFightResult,                     // ����ս���ظ�
    MSG_S2G_BigMamaGetRewardRequest,                // �����콱
    MSG_G2S_BigMamaGetRewardResult,                 // �����콱�ظ�

};

#endif


