#ifndef __LOG_STRUCT_DEF_H__
#define __LOG_STRUCT_DEF_H__

#include "Macro_Define.h"
#include "../socket/Windefine.h"
#include "DataStruct_Base.h"
#include <string>

class ISQLLog
{
public:
    ISQLLog(){}
    virtual ~ISQLLog(){}
    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen) = 0;
};
//��ɫ��½�ǳ�
class CPlayerActionLog : public ISQLLog
{
public:
    enum EPlayerAction
    {
        EPlayerAction_Unknown,	//δ֪
        EPlayerAction_Login,	//��½
        EPlayerAction_Logout	//�ǳ�
    };
    CPlayerActionLog();
    ~CPlayerActionLog();
    unsigned int m_nRoleID;			//��ɫ���
    EPlayerAction m_ePlayerAction;	//����
    int m_nMoney;			//G��
    unsigned int m_nCash;			//M��

    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);
}; 

// ��ɫ����
class CPlayerSwitchLineLog : public ISQLLog
{
public:
    CPlayerSwitchLineLog();
    virtual ~CPlayerSwitchLineLog();

    enum EPlayerAction
    {
        EPlayerAction_Unknown,	//δ֪
        EPlayerAction_Login,	//��½
        EPlayerAction_Logout	//�ǳ�
    };

public:
    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int        m_nRoleID;
    EPlayerAction       m_eAction;
    unsigned int        m_nLine;
};

//��ɫ����
class CRoleExpLog : public ISQLLog
{
public:
    enum EAddExpCause
    {
        EAddExpCause_Unknown,		//δ֪
        EAddExpCause_Quest,			//�������л��
        EAddExpCause_Match,			//�ӱ����л��
        EAddExpCause_Item,			//�ӵ��߻��
        EAddExpCause_GMCommand,		//GM������
        EAddExpCause_AmuseRoom,		//���ų������
    };
    CRoleExpLog();
    ~CRoleExpLog();
    unsigned int m_nRoleID;			//��ɫ���
    EAddExpCause m_eAddExpCause;	//���Ӿ����ԭ��
    unsigned int m_nAddExp;			//���ӵľ���
    unsigned int m_nNewLevel;		//�µĵȼ�
    unsigned int m_nNewExp;			//�µľ���ֵ

    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);
};

//��ɫ����
class CRoleQuestLog : public ISQLLog
{
public:
    CRoleQuestLog();
    ~CRoleQuestLog();
    unsigned int m_nRoleID;			//��ɫ���
    unsigned int m_nQuestID;		//������

    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);
};

//��ɫ�ɾ�
class CRoleAchievementLog : public ISQLLog
{
public:
    enum EAchievementAction
    {
        EAchievementAction_Unknown,		//δ֪
        EAchievementAction_Complete,	//��ɳɾ�
        EAchievementAction_GetReward,	//��ȡ�ɾͽ���
    };
    CRoleAchievementLog();
    ~CRoleAchievementLog();
    unsigned int m_nRoleID;						//��ɫ���
    EAchievementAction m_eAchievementAction;	//�ɾͶ���
    unsigned int m_nAchievementID;				//�ɾͱ��

    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);
};

//��ɫ����
class CRoleDancingLog : public ISQLLog
{
public:
    CRoleDancingLog();
    ~CRoleDancingLog();

    enum EDancingAction
    {
        EDancingAction_Unknown,		//δ֪״̬
        EDancingAction_Start,		//��ʼ����
        EDancingAction_End			//��������
    };

public:
    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int        m_nRoleID;          //��ɫ���
    EDancingAction      m_eDancingAction;   //���趯��
    int                 m_nScene;           //����
    int                 m_nMusicID;         //�������
    int                 m_nMusicMode;       //ģʽ
    int                 m_nMusicLevel;      //�Ѷ�
    int                 m_nPlayerCount;     //�������
    int                 m_nRank;            //����
    int                 m_nScore;           //�÷�
    int                 m_nTeamMode;        //���ģʽ
    bool                m_bIsDungeon;       //�Ƿ���ħ����Ƹ���
};

//��ɫǩ��
class CRoleCheckInLog : public ISQLLog
{
public:
    enum ECheckInAction
    {
        ECheckInAction_Unknown,		//δ֪״̬
        ECheckInAction_Check,		//ǩ��
        ECheckInAction_Recheck,		//��ǩ
    };
    CRoleCheckInLog();
    ~CRoleCheckInLog();

public:
    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int m_nRoleID;			//��ɫ���
    ECheckInAction m_eCheckInAction;//ǩ������
    bool m_bIsVip;
    unsigned int m_nVipLevel;
    unsigned int m_nDayIndex;
};

//��ɫ����
class CRoleSendLog : public ISQLLog
{
public:
    enum ESendType
    {
        ESendType_Unknown,		//δ֪״̬
        ESendType_Buy,			//��������
        ESendType_Wish,			//��Ը����
        ESendType_Ask,			//��Ҫ����
    };
    CRoleSendLog();
    ~CRoleSendLog();
    unsigned int m_nRoleID;			//��ɫ���
    ESendType m_eSendType;			//ǩ������
    itemtype_t m_nItemType;		    //��Ʒ����
    unsigned short m_nItemCount;	//��Ʒ����
    int m_nDuration;				//��Ʒ����
    unsigned int m_nTargetRoleID;	//�����˱��
    __int64 m_nMailID;				//�ʼ����

    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);
};

//��ɫ�ı�M��ֵ��Log
class CRoleChangeBillLog : public ISQLLog
{
public:
    CRoleChangeBillLog();
    ~CRoleChangeBillLog();

    unsigned int m_nRoleID;					//��ɫ���
    int m_nChangeValue;						//�ı��ֵ
    EChangeBillCause m_eChangeBillCause;	//�ı�ԭ��
    unsigned int m_nNewPT;					//�ı���M��ֵ

    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);
};

//��ɫ�ı��M��ֵ��Log
class CRoleChangeBindBillLog : public ISQLLog
{
public:
    CRoleChangeBindBillLog();
    ~CRoleChangeBindBillLog();

    unsigned int m_nRoleID;					//��ɫ���
    int m_nChangeValue;						//�ı��ֵ
    EChangeBindBillCause m_eChangeBindBillCause;//�ı�ԭ��
    unsigned int m_nNewBindPT;				//�ı��İ�M��ֵ

    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);
};


//��ɫ�ı��ȯֵ��Log
class CRoleChangeMoneyLog : public ISQLLog
{
public:
    CRoleChangeMoneyLog();
    ~CRoleChangeMoneyLog();

    unsigned int m_nRoleID;					//��ɫ���
    int m_nChangeValue;						//�ı��ֵ
    EChangeMoneyCause m_eChangeMoneyCause;	//�ı�ԭ��
    unsigned int m_nNewMoney;				//�ı��Ľ�ȯֵ

    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);
};

class CRoleItemLog : public ISQLLog
{
public:
    CRoleItemLog();
    ~CRoleItemLog();
    unsigned int m_nRoleID;				//��ɫ���
    EItemAction m_eItemAction;			//����
    EItemColumn m_eItemColumn;			//��Ʒ��λ
    __int64 m_nItemID;					//��ƷΨһ���
    itemtype_t m_nItemType;			    //��Ʒ����
    unsigned short m_nOldCount;			//����Ʒ����
    int m_nOldDuration;					//����Ʒ����
    unsigned short m_nNewCount;			//����Ʒ����
    int m_nNewDuration;					//����Ʒ����
    __int64 m_nAdditionalValue;			//����ֵ

    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);

};

class CRoleChatLog : public ISQLLog
{
public:
    CRoleChatLog();
    ~CRoleChatLog();
public:
    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);
public:
    unsigned int m_nRoleID;
    eChatChannel m_eChannel;
    bool m_bVoiceChat;
    std::string m_strText;
};

class CRoleAmuseLog : public ISQLLog
{
public:
    enum EAction
    {
        EAction_None,
        EAction_Enter,
        EAction_Exit
    };
public:
    CRoleAmuseLog();
    ~CRoleAmuseLog();
public:
    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);
public:
    unsigned int m_nRoleID;
    int m_nSceneID;
    EAction m_nAction;
    int m_nStayTime;
};
class CRoleTimeEggLog : public ISQLLog
{
public:
    enum ETimeEggAction
    {
        ETimeEggAction_None,
        ETimeEggAction_Create,
        ETimeEggAction_Open
    };
public:
    CRoleTimeEggLog();
    ~CRoleTimeEggLog();
public:
    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);
public:
    unsigned int m_nRoleID;
    ETimeEggAction m_eEggAction;
    int m_nSceneID;
};

//����log
enum EDanceGroupLogAction
{
    EDanceGroupLog_None = 0,
    EDanceGroupLog_Create = 1,//��������
    EDanceGroupLog_Destory, //��������
    EDanceGroupLog_MemberJoin,//��Ա����
    EDanceGroupLog_MemberLeave,//��Ա�˳�
    EDanceGroupLog_MemberKicked,//������Ա
    EDanceGroupLog_MemberContributionAdd,//��Ա���׶�����
    EDanceGroupLog_MemberContributionDec,//��Ա���׶ȼ���
    EDanceGroupLog_MemberIntergralAdd,//��Ա��������
    EDanceGroupLog_HonourAdd,//������������
    EDanceGroupLog_HonourDec,//������������
    EDanceGroupLog_Levelup,//��������
    EDanceGroupLog_ResConsume,//������Դ����
    EDanceGroupLog_ResDonate,//������Դ����
    EDanceGroupLog_LeaderChange,//�ų����
    EDanceGroupLog_MemberTitleChange,//���ų�Աְλ���
    EDanceGroupLog_NameChange,//�������Ʊ��
    EDanceGroupLog_ProptyChange, // С���±��
    EDanceGroupLog_Activity_PowerAdd, //���Ż�����Ƕ���֮���ӣ�
    EDanceGroupLog_ChallengeActiveAdd,  // ��������ֵ����
    EDanceGroupLog_ChallengeActiveDec,  // ��������ֵ����
};
class CDanceGroupLog : public ISQLLog
{
public:
    CDanceGroupLog();
    virtual ~CDanceGroupLog(){}

public:
    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);

public:
    //ʹ��ʱע���������
    void ValuesToStr(EDanceGroupLogAction eAction, va_list ap);

public:
    EDanceGroupLogAction m_eAction;
    unsigned int m_nGroupId;
    std::string m_strDGName;
    unsigned int m_nRoleId;
    std::string m_strRoleName;
    std::string m_strParam;
};

//////////////////////////////////////////////////////////////////////////
class CRoleConstellationMatchLog : public ISQLLog
{
public:
    enum EConstellationMatch
    {
        EConstellationMatchAction_None,
        EConstellationMatchAction_EnterLobby,	// ��������ħ�������
        EConstellationMatchAction_UseCard,		// ʹ���ƽ������
        EConstellationMatchAction_MatchFail,	// ���ʧ��
        EConstellationMatchAction_MatchSuc,		// ��Գɹ�
        EConstellationMatchAction_EnterRoom,	// ���볡������
    };
public:
    CRoleConstellationMatchLog();
    virtual ~CRoleConstellationMatchLog();
public:
    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);
public:
    unsigned int m_nRoleID;
    std::string m_strRoleName;
    EConstellationMatch m_eAction;
    int m_nCardConstellation;
    unsigned int m_nMatchRoleID;
    std::string m_strMatchRoleName;
    int m_nSceneID;
    std::string m_strSceneName;
};

//////////////////////////////////////////////////////////////////////////
class CRoleVipLog : public ISQLLog
{
public:
    enum ERoleVipAction
    {
        ERoleVipAction_None,
        ERoleVipAction_OpenVip,
        ERoleVipAction_LevelUp,
        ERoleVipAction_AddExp,
        ERoleVipAction_AddDuration,
        ERoleVipAction_Expired,
    };

public:
    CRoleVipLog();
    virtual ~CRoleVipLog();
public:
    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);
public:
    unsigned int m_nRoleID;
    std::string m_strRoleName;
    ERoleVipAction m_eAction;
    ERoleVipExpCause m_eExpCause;
    int m_nAddValue;
};


class CMeetActivityLog : public ISQLLog
{
public:
    enum EMeetAction
    {
        EMeetAction_None,
        EMeetAction_Join,
        EMeetAction_Sucess,
        EMeetAction_Fail,
        EMeetAction_Photo,
        EMeetAction_Reward,
        EMeetAction_InAmuse,
    };

public:
    CMeetActivityLog();
    virtual ~CMeetActivityLog();

public:
    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int m_nRoleID;
    EMeetAction m_eMeetAction;
    unsigned int m_nCoupleRoleID;
    unsigned int m_nAmuseID;
    unsigned int m_nTime;
};

class CRoleAttrChangeLog : public ISQLLog
{
public:
    CRoleAttrChangeLog();
    virtual ~CRoleAttrChangeLog();

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int m_nRoleID;
    ERoleAttrChangeCause m_eChangeCause;

    std::string m_strOldAttrValue;
    std::string m_strNewAttrValue;
};

class CRoleCoupleLog : public ISQLLog
{
public:
    CRoleCoupleLog();
    virtual ~CRoleCoupleLog();

    enum ERoleCoupleCause
    {
        ERoleCoupleCause_None,
        ERoleCoupleCause_ToLover,
        ERoleCoupleCause_OutLover,
        ERoleCoupleCause_ToCouple,
        ERoleCoupleCause_AddBlessingValue,
        ERoleCoupleCause_AddIntimacy,
        ERoleCoupleCause_OutCouple,
        ERoleCoupleCause_RedEvnelopeCount,	 // �������
        ERoleCoupleCause_CreateWeddingRoom,	 // �����鷿
        ERoleCoupleCause_DestoryWeddingRoom, // ���ٻ鷿
        ERoleCoupleCause_CoupleReward,       // ���º��
    };

    enum ECoupleDestoryCase
    {
        ECoupleDestoryCase_None,
        ECoupleDestoryCase_Update,                  // ʱ�䵽
        ECoupleDestoryCase_OutCoupleSeparated,      // ���
        ECoupleDestoryCase_OutCoupleHandle,         // ���
        ECoupleDestoryCase_OutCoupleRequest,        // ���
        ECoupleDestoryCase_Marriage,                // ���
    };

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int m_nMaleRoleID;
    unsigned int m_nFemaleRoleID;
    ERoleCoupleCause m_eCoupleCause;

    int	m_nBlessingValue;
    int m_nIntimacy;
    int m_nRedEvnelope;
};

class CRoleDungeonDebrisLog : public ISQLLog
{
public:
    CRoleDungeonDebrisLog();
    virtual ~CRoleDungeonDebrisLog();

    enum EDebrisAction
    {
        EDebrisAction_None,
        EDebrisAction_Get,
        EDebrisAction_Exchange,
    };

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int m_nRoleID;
    EDebrisAction m_eAction;
    unsigned int m_nIsMainline;
    unsigned int m_nDungeonID;
    unsigned int m_nParcloseID;
    unsigned int m_nDebrisID;
    unsigned int m_nDebrisCount;
    unsigned int m_nLeftDebrisCount;
};

//////////////////////////////////////////////////////////////////////////

class CRoleVisitRankLog : public ISQLLog
{
public:
    CRoleVisitRankLog();
    virtual ~CRoleVisitRankLog() {}

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    enum EVisitRankAction
    {
        EVisitRankAction_Hall,          // �������
        EVisitRankAction_MCoin,         // �����
        EVisitRankAction_Horn,          // ������
        EVisitRankAction_Exp,           // ��������
        EVisitRankAction_Group_Honor,   // ��������
        EVisitRankAction_Intimacy,      // ���Ƿ���
        EVisitRankAction_Cloths,        // �ղؼ�
        EVisitRankAction_ClothEffect,   // ����֮��
        EVisitRankAction_Medal,         // ѫ�»���
    };

    unsigned int        m_nRoleID;
    EVisitRankAction    m_eAction;
    unsigned short      m_nDetailType;  // see ERankDetailType
    unsigned short      m_nMyRank;
};

//////////////////////////////////////////////////////////////////////////

class CRoleFriendsLog : public ISQLLog
{
public:
    CRoleFriendsLog();
    ~CRoleFriendsLog() {}

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    enum EFriendsAction
    {
        EFriendsAction_Delete = 0,      // ɾ������
        EFriendsAction_Add,             // ��Ӻ���

        EFriendsAction_Max
    };

    enum EFriendsMode
    {
        EFriendsMode_None = 0,

        EFriendsMode_Del_Normal,        // ɾ������-��ͨ
        EFriendsMode_Del_ToBlack,       // ɾ������-ɾ����������
        EFriendsMode_Del_FromBlack,     // ɾ������-�Ӻ�����ɾ��

        EFriendsMode_Add_Normal = 10,   // �Ӻ���-��ͨ
        EFriendsMode_Add_Recommend,     // �Ӻ���-�Ƽ�
        EFriendsMode_Add_Black,         // �Ӻ���-������
    };

    unsigned int        m_nRoleID;
    unsigned int        m_nFriendID;
    EFriendsAction      m_eAction;
    EFriendsMode        m_eMode;
};


class CRoleActivenessLog : public ISQLLog
{
public:
    CRoleActivenessLog();
    ~CRoleActivenessLog() {}

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    enum EActivenessAction
    {
        EActivenessAction_Unknown = 0,
        EActivenessAction_AddScore,     // ���ӻ���
        EActivenessAction_Reward,       // ��ȡ����
        EActivenessAction_FinishTarget, // ���Ŀ��
        EActivenessAction_DailyReset,   // ��������

        EActivenessAction_Max
    };

    unsigned int      m_nRoleID;
    EActivenessAction m_eAction;
    int               m_nScore;
    int               m_nTarget;
};

// ħ�����
class CRoleDungeonLog : public ISQLLog
{
public:
    CRoleDungeonLog();
    ~CRoleDungeonLog() {}

    enum EDungeonAction
    {
        EDungeonAction_UnKnown = 0,
        EDungeonAction_JoinDungeon,      // ���븱��
        EDungeonAction_BeInvite,         // ��������븱��
        EDungeonAction_UseCrystal,       // ����ˮ��
        EDungeonAction_BuyCrystal,       // ����ˮ����������
        EDungeonAction_UseSkipItem,      // ʹ�������ؿ����ߣ��������£�
    };

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int    m_nRoleID;
    EDungeonAction  m_eAction;
    bool            m_bIsMainline;
    unsigned int    m_nDungeonID;
    unsigned int    m_nParcloseID;
    int             m_nValue;

};

// �̳�
class CRoleMallLog : public ISQLLog
{
public:
    CRoleMallLog();
    virtual ~CRoleMallLog();

    enum EMallAction
    {
        EMallAction_UnKnown = 0,
        EMallAction_Enter,            // �齱
    };

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int    m_nRoleID;
    EMallAction     m_eAction;
};

// �����
class CRolePhotoRoomLog : public ISQLLog
{
public:
    CRolePhotoRoomLog();
    virtual ~CRolePhotoRoomLog();

    enum EPhotoRoomAction
    {
        ERolePhotoRoomAction_UnKnown = 0,
        ERolePhotoRoomAction_Enter,            // ����
        ERolePhotoRoomAction_UseButton,        // ʹ�����ఴť
    };

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int        m_nRoleID;
    EPhotoRoomAction    m_eAction;
};

// ��ѡ
class CRoleAuditionLog : public ISQLLog
{
public:
    CRoleAuditionLog();
    virtual ~CRoleAuditionLog();

    enum EAuditionAction
    {
        EAuditionAction_UnKnown = 0,
        EAuditionAction_Match,              // ����ƥ��
        EAuditionAction_MatchSuc,           // ƥ��ɹ�
        EAuditionAction_MatchFail,          // ƥ��ʧ��
    };

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int        m_nRoleID;
    EAuditionAction     m_eAction;
    unsigned int        m_nMatchPlayer;
};

// ��Ч�۳�
class RoleLongactingRechargeLog : public ISQLLog
{
public:
    RoleLongactingRechargeLog();
    virtual ~RoleLongactingRechargeLog();

    enum ELongactingAction
    {
        ELongactingAction_UnKnown = 0,
        ELongactingAction_Reward = 1,
    };

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int            m_nRoleID;
    ELongactingAction       m_eAction;
    unsigned int            m_nRewardIndex;
    bool                    m_bIsSucess;
    bool                    m_bIsOnline;
    unsigned int            m_nTotalMPoint;
};

//////////////////////////////////////////////////////////////////////////

class CRoleCheckingMarkFailedLog : public ISQLLog
{
public:
    CRoleCheckingMarkFailedLog();
    ~CRoleCheckingMarkFailedLog() {}

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int    m_nRoleID;
    short           m_nRoomType;
    int             m_nScene;
    short           m_nTeamMode;
    int             m_nMusicID;
    int             m_nMusicMode;
    int             m_nMusicLevel;
    int             m_nPlayerCount;
    int             m_nStartTime;
    int             m_nRound;
    int             m_nKeyRank;
    int             m_nLastSpecial;
    std::string     m_strRoundRank;
    unsigned int    m_nMark;
    unsigned int    m_nCheckingMark;
};

// �·���Ч��־
class ClothEffectLog : public ISQLLog
{
public:
    ClothEffectLog();
    virtual ~ClothEffectLog();

    enum EClothEffectAction
    {
        EClothEffectAction_UnKnown = 0,
        EClothEffectAction_GM = 1, // GM���
        EClothEffectAction_Replace = 2, // �滻
        EClothEffectAction_Upgrade = 3, // ����
    };

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int             m_nRoleID;
    EClothEffectAction       m_eAction;
    itemid_t                 m_nItemID;
    unsigned int             m_nOldEffectID;
    unsigned int             m_nNewEffectID;
};

//�·���Ч������־
class ClothEffectProgressLog : public  ISQLLog
{
public:
    ClothEffectProgressLog();
    virtual ~ClothEffectProgressLog();

    enum EClothEffectProgressAction
    {
        EClothEffectProgressAction_UnKnown = 0,
        EClothEffectProgressAction_Handbook,
        EClothEffectProgressAction_Color,
    };

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int                    m_nRoleID;
    EClothEffectProgressAction      m_eAction;
    unsigned int                    m_nSuitID;
    unsigned int                    m_nParam1;
    std::string                     m_strParam2;
};

//ѫ����־
class CMedalLog : public ISQLLog
{
public:
    CMedalLog();
    virtual ~CMedalLog();

    enum EMedalSource
    {
        EMedalUnKonw = 0,                           // δ֪
        EMedalActivityBuyItem = 1,                  // ����ָ�����߻
        EMedalActivityCumulativeSpend = 2,          // �ۼ����ѻ
        EMedalCondition = 3,                        // ������ȡ
        EMedalExchage = 4,                          // ѫ�¶һ�
    };

    enum EMedalAction
    {
        EMedalUnKonwAction = 0,
        EMedalActivated = 1,
        EMedalInvalid = 2,
    };

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int                    m_nRoleID;
    int                             m_nMedalID;
    int                             m_nConditionID;
    int                             m_nTimeStamp;
    EMedalSource                    m_eActivity;
    EMedalAction                    m_eAction;
};

// ���ź����־
class CRedEnvelopeLog : public  ISQLLog
{
public:
    CRedEnvelopeLog();
    virtual ~CRedEnvelopeLog();

    enum ERedEnvelopeAction
    {
        ERedEnvelopeAction_UnKnown = 0,
        ERedEnvelopeAction_Recharge = 1,            // ��ֵ
        ERedEnvelopeAction_CreateRedEnvelope = 2,   // �����ɹ�
        ERedEnvelopeAction_Open = 3,                // �����
    };

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int                    m_nRoleID;
    ERedEnvelopeAction              m_eAction;
    unsigned int                    m_nRedEnvelopeID;
    int                             m_nOpenIndex;
    int                             m_nRedEnvelopeMoney;
};

// ����ħ������־
class CMagicArrayLog : public ISQLLog
{
public:
    CMagicArrayLog();
    virtual ~CMagicArrayLog();

    enum EMagicArrayAction
    {
        EMagicArrayAction_UnKnown = 0,
        EMagicArrayAction_FreeCall = 1,         // ��ѵ���
        EMagicArrayAction_OnceCall = 2,         // ����
        EMagicArrayAction_TenCall = 3,          // ʮ����
        EMagicArrayAction_Exchange = 4,         // �һ�
        EMagicArrayAction_RefreshExchange = 5,  // ˢ�¶һ���Ʒ
        EMagicArrayAction_ExchangeReset = 6,  // �һ������
    };

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int                    m_nRoleID;
    EMagicArrayAction               m_eAction;
    unsigned int                    m_nMagicArrayID;
};



// ��Ҵ�����־
class CRoleBigMamaDanceRecord : public ISQLLog
{
public:
    CRoleBigMamaDanceRecord();
    CRoleBigMamaDanceRecord(unsigned int nRoleID, unsigned int nDanceGroupID, int nDanceRound);
    virtual ~CRoleBigMamaDanceRecord();

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);
    void InsertIntoLog();
public:
    unsigned int                    m_nRoleID;
    unsigned int                    m_nDanceDanceGroupID;
    int                             m_nDanceRound;
};

class CGroupBigMamaDanceRecord : public ISQLLog
{
public:
    CGroupBigMamaDanceRecord();
    CGroupBigMamaDanceRecord(unsigned int nDanceGroupID, unsigned int nDanceRound, int nBeatType, unsigned int nBeatRole);
    virtual ~CGroupBigMamaDanceRecord();

    enum
    {
        EBeatType_Role = 0,
        EBeatType_XiaoLian,
        EBeatType_XiaoRuan,
    };

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int                    m_nDanceGroupID;
    unsigned int                    m_nDanceRound;
    int                             m_nBeatType; 
    unsigned int                    m_nBeatRole;
};

//���Ϊ�ο����͵ĵ�¼��־
class CPlayerType : public ISQLLog
{
public:
    CPlayerType();
    virtual ~CPlayerType();

public:
    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int m_nRoleID;
    unsigned int m_nLastLoginTime;
    int          m_nUserType;
};


#endif //__LOG_STRUCT_DEF_H__

