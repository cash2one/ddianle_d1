#ifndef __ENTITY_COMPONENT_EVENTID_H__
#define __ENTITY_COMPONENT_EVENTID_H__

enum eComponentEvent
{
    eComponentEvent_Relogin,
    eComponentEvent_TodayFirstLogin,                // ���յ�һ�ε�½
    eComponentEvent_FirstLogin,                     // ��һ�ε�½��Ϸ
    eComponentEvent_NormalLogin,                    // ������½

    eComponentEvent_QuestAcceptTimer,               // ������������
    eComponentEvent_QuestExpiredTimer,              // �����������

    eComponentEvent_KeepOnlineReward_Finished,      // ���߽�������
    eComponentEvent_KeepOnlineReward_Reset,         // ���߽�������
    eComponentEvent_KeepOnlineReward_Clear,         // ʱ�䵽��

    eComponentEvent_AntiAddiction,                  // ������״̬����
    eComponentEvent_AntiAddictionTip,               // ��������ʾ

    eComponentEvent_QuestUpdateTimer,               // ��������
    eComponentEvent_DailyQuestUpdateTimer,          // ����ÿ������

    eComponentEvent_DelayTransfrom,                 // �ӳٱ���

    eComponentEvent_CoupleRequestReplay,            // ���±�׻ظ�
    eComponentEvent_CoupleHasBeenSeparated,         // ���±���ɢ

    eComponentEvent_DanceGroupChallengeResetChance, // ����������������
    eComponentEvent_DanceGroupChallengeResetBuyChance,// �������������������
    eComponentEvent_DanceGroupChallengeResetRanking,// ����������������
    eComponentEvent_DanceGroupChallengeCloseAnnounce,// �رչ���
};

#endif

//end file




