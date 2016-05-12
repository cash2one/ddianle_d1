// QueryManager.h: interface for the CQueryManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __QUERYMANAGER_H__
#define __QUERYMANAGER_H__

#include "DBInterface.h"
#include "../../socket/Query.h"
#include "../../socket/QueryManagerBase.h"
#include <map>
using namespace std;

#define QUERYTAG_CheckName		1
#define QUERYTAG_Friend			2
#define QUERYTAG_Blacklist		3
#define QUERYTAG_Login			4

enum QUERY_TYPE
{
    QUERY_TYPE_BEGIN = 0x00000001,

    QUERY_GetRegistedRoleCount			,	//�õ�ע���ɫ����
    QUERY_GetRegistedPlayerCount		,	//�õ�ע���������
    QUERY_GetBatchID64					,	//�õ�һ��64λid

    QUERY_Bill_LoadInfo					,	//��ȡ�Ʒ���Ϣ
    QUERY_Bill_UpdatePt					,	//�۵�
    QUERY_Bill_ChongZhi					,	//��ֵ
    QUERY_Bill_ChongZhiByPUID			,	//PUID��ֵ
    QUERY_Bill_GetGift					,	//��ȡ��ֵ��Ʒ��m_nAttachment����ȡ����Ʒid,m_nTag����ȡ����Ʒ����, m_pTag����ȡ��Ʒ���ʺ����ƣ�charָ��,ע�������ͷſռ䣩
    QUERY_Bill_GiveGift					,	//��������,m_nAttachment����Ʒid,m_nTag�ǵ���Ʒ����, m_pTag������Ʒ���ʺ����ƣ�charָ��,ע�������ͷſռ䣩

    //��ȡ���п���ȡ��Ʒ��Ϣ(m_pAttachment������һ��list< BILLGIFT >ָ�룬m_pTag����ȡ��Ʒ���ʺ����ƣ�charָ�룩)
    //m_nAttachmentΪentity�±꣬m_nTagΪroleID,��2������Ҫ��������
    QUERY_Bill_LoadGiftList				,

    QUERY_Bill_UpdateChange				,
    QUERY_Bill_UpdateTotalGift			,	//��������ȡ��Ʒ���ܵ���,m_nAttachment���µ�����ȡ�ܽ����ĵ�����m_nTagΪ��ҵ��ʺţ���Account��
    QUERY_Bill_SaveChargeAction         ,   // �����ֵ��Ϊ

    //QUERY_Item_LoadGiftActivateCode		,	
    QUERY_Item_GetActivateGiftInfo				,	//��ѯ���������Ϣ
    QUERY_Item_GetActivateGift				,	//�������

    QUERY_Mall_LoadInfo		,
    QUERY_Mall_SaveLog		,
    QUERY_Mall_SigleItemLog ,				//������Ʒlog
    QUERY_Mall_DesireNum	,

    QUERY_Bill_UpdateChangeByUsername   ,	//���û���Ϊ��ѯ�������µ���ȯ��m_pAttachment������new�õ�char[MAX_NAME]����ŵ��ʺ�������ѯ���������ݿ�㸺��delete
    QUERY_Change_Getlog			        ,

    QUERY_CreateAccount					,	//�����˺���Ϣ
//    QUERY_GetAccountInfo				,	//�õ��˺���Ϣ
    QUERY_AccountLogin					,	//�Ǻ����汾��½��Ҫ��֤����
    QUERY_AcountQuickCreate				,	//���ٴ����˺�
    QUERY_CheckAccountIsExist			,	//��ѯ�˺��Ƿ�����Ϸ��
//    QUERY_GetInviteAccountInfo			,	//�õ����������˺���Ϣ
    QUERY_CreateRole					,	//������ɫ
    QUERY_CheckRoleIsExist				,	//��ѯ��ɫ�Ƿ�����Ϸ��
    QUERY_UpdateRole					,	//�޸Ľ�ɫ��Ϣ
    QUERY_DeleteRole					,	//ɾ����ɫ
    QUERY_GetRoleInfo					,	//ͨ��RoleID�õ���ɫ��Ϣ
    QUERY_UpdateAccountState			,	//�����˺�״̬
    QUERY_UpdateAccountLockState		,	//�����˺�����״̬
    QUERY_ACCOUNT_LoadAutoAccount       ,	//��ȡ�Զ����ɵ��˺�
    QUERY_ACCOUNT_UpdateAutoAccountState,	//�����Զ����ɵ�����˺ŵ�״̬0�����ã�1����
    QUERY_ACCOUNT_CreateNewAutoAccount,		//��������˺�
    QUERY_ACCOUNT_LoadActivateCode,			//����������
    QUERY_ACCOUNT_ActivateCode,				//ʹ�ü�����ɹ������ݿ�洢����

    QUERY_QuickCreateAccount			,	//���ٴ����˺�
    QUERY_ResetAccountState				,	//���������˺�״̬
    QUERY_AccountLogout,                    //�˺ŵǳ�
    QUERY_LoadCommands					,	//��ȡ��Ҫִ�е�����
    QUERY_GetRoleInfoBase				,	//�õ���ɫ�Ļ�����Ϣ
    QUERY_GetRoleInfoAttrSetting		,	//�õ���ɫ�����Ժ��趨
    QUERY_LoadRoleOfflineTime           ,   //�ϴε�¼ʱ��

    //2012-2-15
    QUERY_CheckRoleName					,	//��������Ƿ��ظ�
    QUERY_PlayerGetRoleInfo				,	//��һ�ȡ��һ��ҵ���Ϣ


    //��Щ��������Ҫ��������ָ���ѯ��ɺ��ɲ�ѯ�߳��ͷ�
    QUERY_LoadOfflineRoleData           , //��ѯһ����ɫ��Ʒ��Ϣ
    QUERY_LoadListOfflineRoleData       , //��ѯһ����ɫ��Ʒ��Ϣ
    QUERY_AddRoleItem					, //��һ����ɫ���һ����Ʒ������m_nAttachmentΪ��ɫRoleID������m_pAttachmentΪITEMINFOָ��
    QUERY_UpdateRoleItem				, //�޸�һ����ɫ��һ����Ʒ������m_nAttachmentΪ��Ʒ�����˽�ɫRoleID������m_pAttachmentΪITEMINFOָ��
    QUERY_RemoveRoleItem				, //ɾ����ɫ��һ����Ʒ������m_nTagΪitem��nItemID
    QUERY_RemoveRoleItemByType			, //ɾ����ɫ��һ����Ʒ��ͨ��roleid + itemtype
    QUERY_RemoveDynamicBoxRelated		, //ɾ����ɫ��̬����������ݣ�ͨ��roleid+itemid
    QUERY_UpdateCoolDown				, //������ƷCDʱ��
    QUERY_UpdateItemCount				, //�޸���Ʒ����,m_nAttachmentΪ������������m_nTagΪitem��nItemID
    QUERY_UpdateItemPosition			, //�޸���Ʒ��λ��,��m_pAttachmentΪITEMINFO_Positionָ��
    QUERY_Update2ItemPosition			, //�޸�2����Ʒ��λ��,��m_pAttachmentΪITEMINFO_Position2ָ��
    QUERY_UpdateItemOwner				, //�޸���Ʒ������,m_nAttachmentΪ��RoleID������m_nTagΪitem��nItemID
    QUERY_AddRenewItemLog               , //����һ�����Ѽ�¼
    QUERY_AddRoleClothGroup				, //��ӷ��δ���
    QUERY_UpdateRoleClothGroup			, //���·��δ���
    QUERY_InsertClothEffect             , //�����������Ч
    QUERY_UpdateClothEffect             , //���·�����Ч
    QUERY_InsertRoleEffect              , //��ӽ�ɫ�·���Ч
    QUERY_InsertHandbookProgress        , //����ͼ����װ����
    QUERY_UpdateHandbookProgress        , //����ͼ����װ����
    QUERY_InsertColorProgress           , //������ɫ�������
    QUERY_UpdateColorProgress           , //������ɫ�������
    QUERY_InsertStorageCapacity         , //����ֿ����
    QUERY_UpdateStorageCapacity         , //���²ֿ����

    //������ҵĵ���������
    QUERY_UpdateRoleMoney				, //�ı���ҵĽ�Ǯ	����ΪҪ����Ʒ����ͬ��������m_nAttachmentΪ��ɫRoleID������m_nTagΪitem�Ľ�Ǯ
    QUERY_UpdateRoleExpTime				, //�ı����˫������ֵ��ʣ��ʱ��	������m_nAttachmentΪ��ɫRoleID������m_nTagΪʣ���˫������ֵʱ�䣬����m_pAttachmentΪ�������ʣ���˫������ֵʱ���ʱ��
    QUERY_UpdateRoleBadgeNum            , //�ı��ɫ�Ļ��¸�����
    QUERY_UpdateRoleSignature			, //�޸����ǩ��
    QUERY_UpdateRoleMoneyByAccountId	, //�ı���ҵĽ�ȯ(��ֵ����)


    // friend
    QUERY_GetBeAppliedPlayerInfo		,   // ��ȡҪ��ӵ���ҵ���Ϣ
    QUERY_AddBlackListFriend			,	// �Ӻ�������Ӻ���					
    QUERY_AddRoleFriend					,	// ��Ӻ��ѻ������������m_nAttachmentΪ��ɫ��RoleID,����m_pAttachmentΪ���ѵ�FriendInfoָ�룬m_nTagΪ���ͣ�0��ʾ���ѣ�1��ʾ��������
    QUERY_RemoveRoleFriend				,	// ɾ�����ѻ������������m_nAttachmentΪ��ɫ��RoleID,����m_nTagΪ���ѵ�RoleID
    QUERY_AddBlackList					,	// ��Ӻ�����
    QUERY_RemoveBlackList				,	// ɾ��������
    QUERY_UpdateFriendEntry				,	// �����������ͣ����� ������ת��
    QUERY_BecomeFriend					,	// �������ɺ������������ѣ�Ҳ�п���ֱ��INSERT��ȥ
    QUERY_ReworkFriendNotes				,	// �޸ĺ��ѱ�ע
    QUERY_LoadFriendOfflineTime		    ,	// ���غ�������ʱ��


    QUERY_LockLotteryCode				,	// ��֤������

    QUERY_AddRoleGene					, //���ӻ���
    QUERY_DeleteRoleGene				, //ɾ������
    QUERY_UpdateRoleGene				, //���»���
    QUERY_UpdateRoleBagExpand			, //������չ

    QUERY_AddRoleSuggestion				, //���ӽ�ɫ��������
    QUERY_AddOnlineState				, //��������������Ϣ
    QUERY_ChangeRoleName				, //�޸Ľ�ɫ����
    QUERY_ModifyRoleSex					, //�޸Ľ�ɫ�Ա�
    QUERY_Bill_LoadCBChongzhiValue		, //��ȡ�ڲ��ֵ�ܶ�
    QUERY_Bill_DestoryCBChongzhiValue	, //��ȡ��ɣ���ֵ��־λΪ�����ٴ���ȡ״̬
    QUERY_Save_LoginNum					, //�����Ծ�û�����
    QUERY_ChangeBirthday				, //�޸Ľ�ɫ����


    //��ҳ�����Լ��̳ǹ���
    QUERY_LoadHomeNews					,
    QUERY_LoadShopNews					,


    //�ʼ�
    QUERY_LoadMail                      ,
    QUERY_AddMail                       ,
    QUERY_AddSysMail                    ,
    QUERY_GetAllMailItem                ,
    QUERY_GetMailMoney                  ,
    QUERY_ReadMail                      ,
    QUERY_DeleteMail                    ,
    QUERY_MailGetRoleID					,   //ͨ����ɫ����ȡ��ɫid
    QUERY_LoadMailStrangerSetting       ,   // �����Ƿ����İ�����ʼ�����
    QUERY_UpdateMailStrangerSetting     ,   // �����Լ��Ƿ����İ�����ʼ�


    // ����
    QUERY_AddRoleQuest					, 
    QUERY_UpdateRoleQuest				,	// Target��QuestInfoһ��
    QUERY_RemoveRoleQuest				, 
    QUERY_UpdateTargetInfo				,
    QUERY_UpdateQuestInfo				,	// ������QuestInfo����

    QUERY_AddRoleQuestHistory			,	
    QUERY_UpdateRoleQuestHistory		, 
    QUERY_RemoveRoleQuestHistory	, 
    QUERY_AddRepeatedQuestDone,
    QUERY_UpdateRepeatedQuestDone,

    // �ɾ�
    QUERY_AddAchievement,
    QUERY_UpdateAchievementState,

    // �ƺ�
    QUERY_AddTitle,

    // ǩ��
    QUERY_InsertCheckInInfo,
    QUERY_UpdateCheckInInfo,
    QUERY_AddCheckInGift,
    QUERY_RemoveCheckInGift,
    //QUERY_RemoveCheckInGiftByList,

    // VIP
    QUERY_SaveVIPInfo,
    QUERY_SaveVIPConsumedPt,

    // event
    QUERY_AddFirstPaymentInfo,			// ��¼�׳��¼�,FinishFirstPayment = 8
    QUERY_AddSendFirstPaymentReward,	// ��¼�����׳佱���¼�,HasSentFirstPaymentReward = 9

    QUERY_AddFirstPaymentInfoFeed,		// FinishFirstPayment_feed = 11,
    QUERY_AddSendFirstPaymentRewardFeed,// HasSentFirstPaymentReward_feed = 12

    QUERY_GetFirstPaymentReward,
    QUERY_GetFirstPaymentRewardByAccountId,
    QUERY_AddCompleteTeachGuideEvent,
    QUERY_AddCompleteProcGuideEvent,
    QUERY_UpdateCompleteProcGuideEvent,
    QUERY_AddOrUpdateEvent,
	QUERY_UpdateFixedChargeReward,			// ���¹̶�����׳�״̬
	QUERY_LoadFixedChargeReward,			// ���ڹ̶�����׳�״̬
    QUERY_UpdateMallSendItemEvent,
	QUERY_UpdateGuideDataEvent,

    // desired item
    QUERY_Desire_Insert,
    QUERY_Desire_Delete,

    // money
    QUERY_ResetMoney,
    QUERY_UpdateResetMoneyTime,
    QUERY_UpdateMoneyMailRemindTime,
    QUERY_UpdateMoneyMallRemindTime,

    // online reward
    QUERY_UpdateOnlineRewardInfo,

    // GM
    GetRcvGMSysMailRoleInfo,
    QUERY_QueryGiftByTimeRange,
    QUERY_QueryMultiGift,

    //gift
    QUERY_Activity_LoadGift,			//��������
    QUERY_Activity_GetGiftReward,		//��ȡ����

    //�ۼƳ�ֵ����
    QUERY_LoadCumulativeRechargeInfo,				//��ȡ�������ȡ�ĳ�ֵ����
    QUERY_SaveCumulativeRecharge,

    //�ۼ�����
    QUERY_LoadCumulativeSpendInfo,
    QUERY_LoadCumulativeSpendInfoForMedal,
    QUERY_Update_CumulativeReward,
    QUERY_Insert_CumulativeReward,
    QUERY_Insert_CumulativeSpendBill,
    QUERY_Insert_CumulativeSpendBindBill,

    // �ۼ����ѻ�ȡѫ��
    QUERY_LoadCumulativeSpendGotMedalReward,
    QUERY_InsertCumulativeSpendGotMedalReward,

    // ������Ʒ��ȡѫ��
    QUERY_LoadBuyItemGotMedalRewardedInfo,
    QUERY_SetBuyItemGotMedalRewarded,
    QUERY_InsertBuyItemGotMedalInfo,
    QUERY_UpdateBuyItemGotMedalInfo,

    //ʥ���-�����ȡ����
    QUERY_LoadXmasFreeRewardInfo,
    QUERY_SaveXmasFreeRewardInfo,

    //������Ը
    QUERY_LoadFestivalWishInfo,
    QUERY_SaveFestivalWishInfo,

    //����ף���
    QUERY_SaveFestivalBlessEntry,
    QUERY_SaveRoleFestivalBlessRewardInfo,
    QUERY_LoadRoleBlessActivityInfo,

    //IOSר�а潱����ȡ
    QUERY_AddProprietaryActivityReward,

    // ���ź��
    QUERY_RedEnvelope_LoadInfo,                 // ���غ��
    QUERY_RedEnvelope_InsertNew,                // ������
    QUERY_RedEnvelope_Update,                   // ���º����Ϣ
    QUERY_RedEnvelope_UpdateAnonymityEvent,     // ������������
    QUERY_RedEnvelope_LoadRoleInfo,             // ���ź�����������Ϣ

    //DanceGroup
    QUERY_DanceGroupLoadAllInfo,				//�������е�������Ϣ
    QUERY_DanceGroup_Create,					//��������
    QUERY_DanceGroup_Remove,					//�Ƴ�����
    QUERY_DanceGroup_UpdateDanceGroupBaseInfo,	//�������Ż�����Ϣ
    QUERY_DanceGroup_AddDanceGroupMember,		//������ų�Ա
    QUERY_DanceGroup_UpdateMember,				//�������ų�Ա��Ϣ
    QUERY_DanceGroup_UpdateMemberWithTime,		//����ʱ�䣨�ڶԸ��������������޸ĺ��ʱ��ᱻ���ã�
    QUERY_DanceGroup_DelDanceGroupMember,		//ɾ�����ų�Ա
    QUERY_DanceGroup_UpdateRequestDanceGroup,	//��������������Ϣ
    QUERY_DanceGroup_RemoveRequestDanceGroup,	//�Ƴ�����������Ϣ
    QUERY_DanceGroup_ChangeDanceGroupName,		//�޸���������
    QUERY_DanceGroup_UpdateActivityInfo, //�������Ż��Ϣ
    QUERY_DanceGroup_AddRecord, //���ż�¼���
    QUERY_DanceGroup_WriteQuitInfo,             // ��¼����˳�������Ϣ
    QUERY_DanceGroup_CeremonyLoadLog,           // ����������ʽ��־
    QUERY_DanceGroup_CeremonyWriteLog,          // ��¼���������ʽ��־

    QUERY_DanceGroup_LoadBigMamaGroupInfo,              // ���Ŵ����������ݼ���
    QUERY_DanceGroup_UpdateOrInsertBigMamaGroupInfo,    // ���Ŵ�����������д��
//    QUERY_DanceGroup_LoadBigMamaRoleInfo,               // ���Ŵ���������ݼ���
    QUERY_DanceGroup_UpdateOrInsertBigMamaRoleInfo,     // ���Ŵ����������д��

    //External
    QUERY_External_SendMail,					//�ⲿϵͳ�����ʼ�
    QUERY_External_SendMailGift,				//�ⲿϵͳ����Gift�ʼ�

    //��Ҹ�������
    QUERY_Player_SavePlayerSetting,

    //���»���
    QUERY_Couple_AddCouple,
    QUERY_Couple_DelCouple,
    QUERY_Couple_UpdateCoupleInfo,
    QUERY_Couple_UpdateIntimacy,
    QUERY_Couple_UpdateLoveDec,
    QUERY_Couple_UpdateCoupleBlessingValue,
    QUERY_Couple_AddBlessingRankInfo,
    QUERY_Couple_DelBlessingRankInfo,
    QUERY_Couple_UpdateBlessingRankValue,
    QUERY_Couple_LoadBlessingValueRank,
    QUERY_Couple_LoadOfflineCoupleInfo,
    QUERY_Couple_UpdateMarriageGiftDate,
    QUERY_Couple_AddDivorceAgreement,
    QUERY_Couple_AddDivorceAction,
    QUERY_Couple_UpdateCoupleAgreementID,
    QUERY_Couple_UpdateDivorceAgreementState,
    QUERY_Couple_UpdateDivorceAgreement,

    // ����ħ����
    QUERY_Insert_MagicArrayRecord,
    QUERY_Update_MagicArrayRecord,
    QUERY_Insert_MagicArrayExchange,
    QUERY_Update_MagicArrayExchange,

    //ƴͼ�
    QUERY_Puzzle_LoadPicture,
    QUERY_Puzzle_InsertPicture,
    QUERY_Puzzle_UpdatePicture,

    //�������ļ�
    QUERY_Invitation_UpdateInvitationCode,
    QUERY_Invitation_CheckInvitationCode,
    QUERY_Invitation_AddInvitation,
    QUERY_Invitation_GetInvitationInfo,
    QUERY_Invitation_AddInviterReward,
    QUERY_Invitation_AddAcceptorReward,

    //VIP���߹���
    QUERY_VIP_UpdateOnlineAnnounceTime,

    //������Ϣ����
    QUERY_Rating_NoPushRating,
    QUERY_Rating_PushRatedLevelUp,
    QUERY_Rating_PushRatedOnlineTime,
    QUERY_Rating_PushRatedDailyQuest,
    QUERY_Rating_PushRatedDailyShoping,
    QUERY_Rating_PushRatedCount,

    //��ȫ��������
    Query_Security_SetSecurityQuiz,

    //���������ֻ
    QUERY_FresherActivity_AddReward,

    //��������Ŀ��
    QUERY_FresherQuest_AddReward,
    QUERY_FresherQuest_BestMusicGrade,
    QUERY_Mall_AddMallBuyCount,

    //ֱ������
    Query_LiveRoom_GetDancerName,
    Query_LiveRoom_UpdateDancerName,
    Query_LiveRoom_LoadPointAndHot,
    Query_LiveRoom_GetPointAndHot,
    Query_LiveRoom_UpdateAuditionPoint,
    Query_LiveRoom_UpdateDancerHot,

    //����
    Query_LoadCompensation,				//�����ݿ����
    Query_InsertCompensation,			//GM��Ӳ���
    Query_InsertReceiveCompensation,	//�����ȡ����

    //ƽ̨��Ȩ
    Query_PlatformPrivilege_AddReward,

    //ʱ�⵰
    Query_CreateTimeEgg,
    Query_GetTimeEggsByRoleID,
    Query_OpenTimeEgg,
    Query_GetTimeEggs,

    //����ħ����
    Query_Constellation_EnterLobby,
    Query_Constellation_UpdateCardInfo,
    Query_Constellation_MatchCount,
    Query_Constellation_UpdateMatchRoom,

    //����
    QUERY_Add_Quest,
    QUERY_Update_Quest,

    // ����
    Query_Insert_MagicLampInfo,
    Query_Update_MagicLampInfo,
    Query_Load_FriendParcloseInfo,
    Query_Update_ParcloseInfo,
    Query_Insert_ParcloseInfo,
    Query_Insert_StarRewardIndex,
    Query_Update_DebrisInfo,
    Query_Update_ThemeParcloseInfo,
    Query_Insert_ThemeParcloseInfo,
    Query_Insert_FriendContactInfo,
    Query_Update_FriendContactInfo,
    Query_Update_ClearGiveGetVitInfo,
    Query_Update_ClearMailineQuickFinishNum,
    Query_Update_ClearThemeQuickFinishNum,
    Query_Update_Offline_FriendContactInfo,


    QUERY_Add_DanceGroupReward,
    QUERY_Del_DanceGroupReward,

    QUERY_AddCompleteAnimation,

    // ����״γ�ֵ״̬
    QUERY_ClearFirstPaymentInfo,

    QUERY_Bill_UpdateBindPt, // ���°�M����Ϣ

    QUERY_SaveSocialShare,//�����罻������Ϣ

    QUERY_InsertProcGuideReward,	//��ȡ���ֽ���

    // �¿�
    Query_Insert_MonthCardInfo,
    Query_Update_MonthCardInfo,

    QUERY_SaveSpecialMall,          // ���������̵���Ϣ
    QUERY_SaveAuctionBid,           // ���澺����Ϣ
    QUERY_SaveGroupBuyingCount,     // �����Ź�����
    QUERY_SetAuctionStateSuccess,   // ���������ɹ�
    QUERY_AuctionReturnPoint,       // ��������M��
    QUERY_GetGroupBuyingCount,      // ��ѯ�Ź�����
    QUERY_GetLimitedEditionCount,   // ��ѯ��������
    QUERY_GetAuctionBidInfo,        // ��ѯ������Ϣ

    // ���а�
    QUERY_Rank_GetRoleData,         // ��ѯ��ɫ���а�����
    QUERY_Rank_GetAllDatas,         // �������а���������
    QUERY_Rank_SaveData,            // ��¼����
    QUERY_Rank_UpdateDataState,     // �������а�����״̬
    QUERY_Rank_SaveHistory,         // ��¼��ʷ����

    //GM���߳�ֵ������¼Log��
    QUERY_GMPrivRecharge,

    Query_System_LoadBlackDevice,   // ���س�ֵ������

	QUERY_SendDeviceList,			//GM����豸������
	
	QUERY_ChangeAccountPassword,	//GM�޸��������

    QUERY_DeleteDeviceList,         //GM����豸������

    // �Ƽ�����
    QUERY_Recommend_UpdateCount,    // ����ˢ�´���
    QUERY_Recommend_UpdateResetTime,// ��������ʱ��

    // ��Ծ��ϵͳ
	QUERY_Activeness_UpdateOrInsert,
//    QUERY_Activeness_UpdateScore,   // ���Ŀ�꣬���ӷ���
//     QUERY_Activeness_UpdateTarget,  // ����Ŀ�����
//     QUERY_Activeness_Reward,        // ��ȡ�������۳�����

    // ����ϵͳ
    QUERY_PetEgg_LoadIncubator,         // ��ȡ��������Ϣ
    QUERY_PetEgg_UpdateIncubatorLevel,  // ���·������ȼ���Ϣ
    QUERY_PetEgg_Load,                  // ��ȡ���ﵰ��Ϣ
    QUERY_PetEgg_Create,                // �������ﵰ��Ϣ
    QUERY_PetEgg_UpdateState,           // ���³��ﵰ״̬
    QUERY_Pet_LoadExtendGridCount,      // ��ȡ��չ��λ����
    QUERY_Pet_UpdateExtendGridCount,    // ������չ��λ����
    QUERY_Pet_Create,                   // ����������Ϣ
    QUERY_Pet_ChangeName,               // �޸ĳ����ǳ�
    QUERY_Pet_UpdateState,              // �޸ĳ���״̬
    QUERY_Pet_UpdateQuality,            // �޸ĳ���Ʒ��
    QUERY_Pet_UpdateEvolution,          // �޸ĳ��������
    QUERY_Pet_UpdateEndurance,          // �޸ĳ�������
    QUERY_Pet_UpdateLevelAndExp,        // ��������
    QUERY_Pet_UpdateSkill,              // ���ܷ����仯
    QUERY_Pet_ResetSkill,               // ���ü���
    QUERY_Pet_UpdateGuideStep,          // ������������

	// ��Ϣ���ļ�������
	QUERY_InfoCenter_LoadAllInfo,		// ���������Ѿ���������Ϣ
	QUERY_InfoCenter_AddInfo,			// �·���һ����Ϣ
	QUERY_InfoCenter_UpdateInfo,		// ����һ����Ϣ
	QUERY_InfoCenter_RemoveInfo,		// ɾ��һ����Ϣ
	QUERY_InfoCenter_LoadRoleReadInfo,	// ��������Ѷ��б�
	QUERY_InfoCenter_UpdateRoleReadInfo,// д������Ѷ���Ϣ

    // ��Ч�۳�
    QUERY_LongactingCumulativeRechagre_UpdateIndex,   // ��Ч�۳�����콱����
	// Experineced cards;
	QUERY_ExperienceCard_UpdateTime,
	QUERY_ExperienceCard_AddExperiencedItem,
    //ѫ��
    QUERY_Medal_UpdateMedal,                        //����ѫ��
    QUERY_Medal_UpdateProgress,                     //���»��ѫ�������Ľ���
    QUERY_Medal_UpdateScore,                        //����ѫ�»���
    QUERY_Medal_DeleteProgress,                     //ɾ��һ����������
    QUERY_Medal_DeleteVersionMedal,                 //ɾ���汾ѫ��
    QUERY_Medal_AddMedalByGm,                       //Gm�������һ������ѫ��

    //ѫ�»
    QUERY_Medal_UpdateActConsumeValue,              //ѫ�»�ڼ������
    QUERY_Medal_UpdateBuyGiftCount,                 //ѫ�»�ڼ�Ĺ���ָ���������Ŀ

    //ͼ��ϵͳ
    QUERY_Handnbook_LoadGolbalCostumeData,                 //ͼ��ϵͳ����ȫ�ַ�������
    QUERY_Handnbook_LoadGolbalEffectData,                  //ͼ��ϵͳ����ȫ����Ч����

    QUERY_Handnbook_SaveGlobalEffectData,                  //ͼ��ϵͳ������Ч����
    QUERY_Handnbook_SaveGlobalCostumeData,                  //ͼ��ϵͳ����ȫ�ַ�������

    QUERY_Handnbook_UpdatePersonalData,                 //ͼ��ϵͳ�������ݸ���
    QUERY_Handnbook_InsertPersonalData,                 //ͼ��ϵͳ�������ݲ���
    
	// �˺�Ǩ��ϵͳ
	QUERY_AccountTran_CheckTranCode,	// �˺�Ǩ�Ƽ�������
	QUERY_AccountTran_UseTranCode,		// ʹ��trancode����account�˻�
	
    // ��������
    QUERY_DanceGroupChallenge_ResetChance,          // ������������
    QUERY_DanceGroupChallenge_UpdateChance,         // ������������
    QUERY_DanceGroupChallenge_ResetBuyChance,       // ���������������
    QUERY_DanceGroupChallenge_UpdateBuyChance,      // ���������������
    QUERY_DanceGroupChallenge_LoadRanking,          // ������������
    QUERY_DanceGroupChallenge_UpdateRankingState,   // ��������״̬
    QUERY_DanceGroupChallenge_RecordRanking,        // ��¼����
    QUERY_DanceGroupChallenge_LoadAllInfo,          // ���ر���������Ϣ
    QUERY_DanceGroupChallenge_UpdateActive,         // ���»���ֵ
    QUERY_DanceGroupChallenge_UpdateContribution,   // ���¹���
    QUERY_DanceGroupChallenge_OpenStorey,           // ����������
    QUERY_DanceGroupChallenge_UpdateStoreyScore,    // ���·���

    // �����ؾ�
    QUERY_DanceGroupFairyLandLoadAllInfo,          // ���������ؾ���Ϣ
//     QUERY_DanceGroupFairyLandLoadLastStartTime,        // �������������ʱ���
    QUERY_DanceGroupFairyLandUpdateLastStartTime,      // �������������ʱ���
    QUERY_DanceGroupFairyLandUpdateSystemStamp,    // ����ʱ�����

    QUERY_DanceGroupFairyLandDelGroupInfo,         // ɾ����������(������Ҫɾ�� GroupInfo ��PersonalInfo)
    QUERY_DanceGroupFairyLandAddGroupInfo,         // �����������
    QUERY_DanceGroupFairyLandDelPersonalInfo,      // ɾ����Ա����
    QUERY_DanceGroupFairyLandAddPersonalInfo,      // ��ӳ�Ա����

    QUERY_DanceGroupFairyLandUpdateCurChatper,      // �������Ų���
    QUERY_DanceGroupFairyLandUpdateBlassRank,       // ��������ף����
    QUERY_DanceGroupFairyLandUpdateTodayRank,       // �������Ž��շ�����
    QUERY_DacneGroupFairyLandUpdateBlessInfo,    // ��������ף������ʱ���

    QUERY_DanceGroupFairyLandUpdateTotalScore,                 //���¸����ܷ� ���������ʱ�����
    QUERY_DanceGroupFairyLandClearYesterdayScore,              //������շ���
//    QUERY_DanceGroupFairyLandUpdateLastChallengeChapterLvl,    //���¸������ս����
    QUERY_DanceGroupFairyLandUpdateBlassTimes,                 //���¸���ף������
    QUERY_DanceGroupFairyLandUpdateCurChapterChallengeTimes,   //���µ�ǰ������ս����
    QUERY_DanceGroupFairyLandUpdateStorageBox,                 //�����佱������

    QUERY_DanceGroupFairyLandUpdateJoinRewardFlag,             //���²��뽱��־λ
    QUERY_DanceGroupFairyLandUpdatePassedRewardFlag,           //���¹��ؽ���־λ
    QUERY_DanceGroupFairyLandUpdatePassedRewardRecord,         //���¹��ؽ�����¼

    QUERY_DanceGroupFairyLandUpdateGroupScore,       //���·���
    QUERY_DacneGroupFairyLandUpdateBlessEndTime,               //����ף������ʱ��

    QUERY_DanceGroupFairyLandClearAllPassedReward,        // ������й��ؽ�����ɾ�����ݣ�ɾ����־λ��
    QUERY_DanceGroupFairyLandClearAllStorageBoxReward,    // ������д����佱��
    QUERY_DanceGroupFairyNewDay,                          // 

    //log��ѯ����
    QUERY_TYPE_LOG_MIN,

    QUERY_LogPlayerAction,
    QUERY_LogRoleExp,
    QUERY_LogRoleQuest,
    QUERY_LogRoleAchievement,
    QUERY_LogRoleDancing,
    QUERY_LogRoleCheckIn,
    QUERY_LogRoleSend,
    QUERY_LogRoleChangeBill,
    QUERY_LogRoleChangeBindBill,
    QUERY_LogRoleChangeMoney,
    QUERY_LogRoleItem,
    QUERY_LogRoleChat,
    QUERY_LogRoleAmuse,
    QUERY_LogRoleTimeEgg,
    QUERY_LogDanceGroup,
    QUERY_LogRoleConstellationMatch,
    QUERY_LogRoleVip,
    QUERY_LogRoleMeetActivity,
    QUERY_LogRoleAttrChange,
    QUERY_LogRoleCouple,
    QUERY_LogRoleDebris,
    QUERY_LogRoleVisitRank,
    QUERY_LogRoleAddFriends,
    QUERY_LogRoleActiveness,
    QUERY_LogRoleDungeon,
    QUERY_LogRoleLuckDraw,
    QUERY_LogRoleMall,
    QUERY_LogRolePhotoRoom,
    QUERY_LogRoleAudition,
    QUERY_LogRoleLongacting,
    QUERY_LogRoleCheckingMarkFailed,
    QUERY_LogRoleClothEffect,
    QUERY_LogRoleClothEffectProgress,
    QUERY_LogRoleMedalLog,
    QUERY_LogRoleRedEnvelopeLog,
    QUERY_LogRoleMagicArray,
    QUERY_LogRoleSwicthLine,
    QUERY_LogRoleBigMamaDanceRecord,
    QUERY_LogGroupBigMamaDanceRecord,
    QUERY_LogPlayerType,

    QUERY_TYPE_LOG_MAX,                 // log��ѯ����
};

class CQueryLoad
{
public:
    int m_nTimes;//���ô���
    int m_nCost;//��ʱ�����룩
    CQueryLoad()
    {
        m_nTimes = 0;
        m_nCost = 0;
    }
    void operator += (int nCostTime)
    {
        m_nTimes++;
        m_nCost += nCostTime;
    }
protected:
private:
};
class CQueryManager : public CQueryManagerBase
{
public:
    CQueryManager();
    virtual ~CQueryManager();

public:
    bool OpenDatabase(bool bOpenAccountDB = true, bool bOpenRoleDB = true, bool bOpenLogDB = false, bool bOpenSubLogDB = false);

    bool OpenBackupAccountDB();

    int ProcessQuery(QueryBase* &pQuery);
    int GetQueueSize();
    bool IsQueueEmpty();
    int GetThreadState();
    bool RecreateThread();

    virtual void Release();

    virtual void AddQuery(int nQueryType, int nAttachment = 0, void* pAttachment = NULL, __int64 nTag = 0, void* pTag = NULL, CDelHelper* pDelHelper = NULL);
    virtual void AddQuery(QueryBase *query);

    template <typename T>
    void AddQuery( int nQueryType, SharedPtr<T> sharedAttachment, int nAttachment = 0, __int64 nTag = 0 )
    {
        QueryBase* pQuery = new Query<T>( nQueryType, sharedAttachment );
        pQuery->m_nAttachment = nAttachment;
        pQuery->m_nTag = nTag;
        AddQuery( pQuery );
    }

    QueryBase* GetLogQuery();

protected:
#ifdef WIN32
    static DWORD WINAPI LogQueryThreadProc(void* pParam);
#else
    static void* LogQueryThreadProc(void* pParam);
#endif

protected:
#ifdef WIN32
    HANDLE m_hThreadLogQuery;
    CRITICAL_SECTION m_lockLogQuery;
    HANDLE m_hEventLogQuery;
#else
    pthread_t m_hThreadLogQuery;
    pthread_mutex_t m_lockLogQuery; 
    pthread_cond_t  m_condLogQuery;
    pthread_mutex_t *m_pMutexLogQuery; 
    pthread_cond_t  *m_pCondLogQuery;
#endif

    // ��־���ѯ���У����ڴ���log DB�Ĳ�ѯ��������־��ѯ�̺߳��߼��߳�ʹ��
    // ��ѯ�Ľ�������������m_apQueryFinishedQueue��
    list< QueryBase* >m_apLogQueryQueue;

protected:
    CDBInterface m_dbiRole;
    CDBInterface m_dbiAccount;
    CDBInterface m_dbiLog;
    CDBInterface m_dbiSubLog;

    bool m_bEnableBackupAccountDB;            // �Ƿ�����һ��AccountDB����
    CDBInterface m_dbiBackupAccountDB;        // ��һ��AccountDB����ʱ����iOS�������˺Ŵ�����У��

    time_t m_timeRecord;//��¼���г��ȵ�ʱ��
    //map< int, CQueryLoad > mapQueryLoad;
private:
    int m_nLotteryMode;
};

#endif // __QUERYMANAGER_H__


//file end

