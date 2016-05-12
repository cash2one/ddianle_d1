#ifndef	__GAMEMSG_CEREMONYROOMSC_H__
#define	__GAMEMSG_CEREMONYROOMSC_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DanceGroup.h"
#include "../../datastructure/DataStruct_Base.h"

class CParamPool;
class CDanceGroupInfo;


class CeremonyPlayerBaseInfo
{
public:
    CeremonyPlayerBaseInfo();
    virtual ~CeremonyPlayerBaseInfo() {}

    virtual bool DoEncode(CParamPool &IOBuff);

    unsigned int m_nPlayerID;
    std::string m_strPlayerName;
    unsigned char m_nPlayerSex;
    bool m_bIsVIP;
    unsigned short m_nVIPLevel;
    /// �������ְλ-�ų� ���ų� ���� ��ʽ��Ա ��ϰ��Ա
    unsigned char m_nDanceGroupPos;
    /// �����������-��ʼ�� ����Ա С����
    unsigned char m_nDanceGroupProperty;
    /// ��ʽ�������-0���� 1������ 2����
    unsigned char m_nPlayerType;
    /// ��ʽ�������-1��8
    int m_nRookieIndex;
};

class CEntityItemComponent;
class CEntityGeneComponent;
class PetComponent;
class CEntityMedalComponent;
class CEntityHorseComponent;

/// ��ʽ�����ʾ��Ϣ-��Ҫ��ʾ��������ƶ�
class CeremonyPlayerShowInfo : public CeremonyPlayerBaseInfo
{
public:
    CeremonyPlayerShowInfo();
    virtual ~CeremonyPlayerShowInfo() {}

    virtual bool DoEncode(CParamPool &IOBuff);

    unsigned short m_nDanceGroupBadgeId;
    unsigned short m_nDanceGroupEffectId;
    std::string m_strDanceGroupName;
    /// ����ƶ�����-���� ����
    unsigned char m_nMoveType;
    /// ��ҳ���
    float m_fOrient;
    /// �������x
    float m_fPosX;
    /// �������y
    float m_fPosY;
    /// �������z
    float m_fPosZ;
    /// ��ҷ�ɫ
    unsigned char m_nPlayerSkin;
    /// ��ұ�������ID
    unsigned short m_nTransformId;
    CEntityItemComponent* m_pItemCOM;
    CEntityGeneComponent* m_pGeneCOM;
    PetComponent*         m_pPetCOM;
    CEntityMedalComponent *m_pMedalCOM;
    CEntityHorseComponent *m_pHorseCOM;
};

/// ��ʽ���˽�����Ϣ
class CeremonyRookieInfo
{
public:
    CeremonyRookieInfo();
    bool DoEncode(CParamPool &IOBuff);

    /// ���ID
    unsigned int m_nRoleID;
    /// ��ʽ�������
    int m_nRookieIndex;
    /// �������ҽ���
    std::string m_strIntruduction;
};

/// ��ʽ������Ϣ
class CeremonyRoomInfo
{
public:
    CeremonyRoomInfo();
    void DoEncode(CParamPool &IOBuff);

    /// ��ʽ״̬-CeremonyState
    unsigned char m_nCeremonyState;
    /// ��ʽ��������ʱ
    float m_nCeremontyCD;
    /// ��ʽ�������б� �����˰�����ͨ��� ������ ����
    std::vector<CeremonyPlayerShowInfo> m_vecCeremonyPlayers;
};


/// C2S ����������ʽ
class GameMsg_C2S_RequestApplyCeremony : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_RequestApplyCeremony);
    GameMsg_C2S_RequestApplyCeremony();

    virtual bool doDecode(CParamPool &IOBuff);
};

/// S2C ����������ʽ���
class GameMsg_S2C_RequestApplyCeremonyRes : public GameMsg_Base
{
public:
    GameMsg_S2C_RequestApplyCeremonyRes();

    virtual bool doEncode(CParamPool &IOBuff);

    /// ����ö��-0��ʾ�ɹ�
    unsigned char m_nErrorType;

};


/// C2S ���������ʽ����
class GameMsg_C2S_RequestEnterCeremony : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_RequestEnterCeremony);
    GameMsg_C2S_RequestEnterCeremony();

    virtual bool doDecode(CParamPool &IOBuff);

};

/// S2C ������ʽ������
class GameMsg_S2C_EnterCeremonyRes : public GameMsg_Base
{
public:
    GameMsg_S2C_EnterCeremonyRes();
    ~GameMsg_S2C_EnterCeremonyRes(){}

    virtual bool doEncode(CParamPool &IOBuff);

    /// ����ö��-0��ʾ�ɹ�
    unsigned char m_nErrorType;
    /// ��ʽ������Ϣ
    CeremonyRoomInfo m_CeremonyRoomInfo;
};

/// C2S �����˳���ʽ����
class GameMsg_C2S_RequestQuitCeremony : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_RequestQuitCeremony);
    GameMsg_C2S_RequestQuitCeremony();

    virtual bool doDecode(CParamPool &IOBuff);

public:
    char            m_nQuitTo;
};

/// S2C �˳���ʽ������
class GameMsg_S2C_QuitCeremonyRes : public GameMsg_Base
{
public:
    GameMsg_S2C_QuitCeremonyRes();
    virtual bool doEncode(CParamPool &IOBuff);

    /// ����ö��-0��ʾ�ɹ�
    unsigned char m_nErrorType;
    /// �˳�����
    unsigned char m_nQuitType;
    /// ������ID
    unsigned int m_nKickerID;
    // �˳�ȥ�ĵط�
    char    m_nQuitTo;
};

/// C2S ����ˢ����ʽ����,
class GameMsg_C2S_RequestRefreshCeremony : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_RequestRefreshCeremony);
    GameMsg_C2S_RequestRefreshCeremony();

    virtual bool doDecode(CParamPool &IOBuff);

};

/// S2C ˢ����ʽ������
class GameMsg_S2C_RefreshCeremonyRes : public GameMsg_Base
{
public:
    GameMsg_S2C_RefreshCeremonyRes();
    virtual bool doEncode(CParamPool &IOBuff);

    /// ����ö��-0��ʾ�ɹ�
    unsigned char m_nErrorType;
    /// ��ʽ������Ϣ
    CeremonyRoomInfo m_CeremonyRoomInfo;
};

/// C2S ����ʼ��ʽ
class GameMsg_C2S_RequestStartCeremony : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_RequestStartCeremony);
    GameMsg_C2S_RequestStartCeremony();

    virtual bool doDecode(CParamPool &IOBuff);
};

/// S2C ������ʽ���
class GameMsg_S2C_StartCeremonyRes : public GameMsg_Base
{
public:
    GameMsg_S2C_StartCeremonyRes();
    virtual bool doEncode(CParamPool &IOBuff);

    /// ����ö��-0��ʾ�ɹ�
    unsigned char m_nErrorType;
    /// ��ʽ���˽�����Ϣ
    std::vector<CeremonyRookieInfo> m_vecRookieInfos;
};

/// S2C ������ҽ�����ʽ����
class GameMsg_S2C_OtherPlayerEnterCeremony : public GameMsg_Base
{
public:
    GameMsg_S2C_OtherPlayerEnterCeremony();
    virtual bool doEncode(CParamPool &IOBuff);

    CeremonyPlayerShowInfo m_PlayerInfo;
};

/// S2C ��������˳���ʽ����
class GameMsg_S2C_OtherPlayerQuitCeremony : public GameMsg_Base
{
public:
    GameMsg_S2C_OtherPlayerQuitCeremony();
    virtual bool doEncode(CParamPool &IOBuff);

    unsigned int m_nPlayerID;
};

/// C2S ��Ϊ��ʽ����
class GameMsg_C2S_SetCeremonyRookie : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_SetCeremonyRookie);
    GameMsg_C2S_SetCeremonyRookie();
    virtual bool doDecode(CParamPool &IOBuff);

    /// ���ID
    unsigned int m_nRoleId;
    /// �������
    int m_nRookieIndex;
};

/// S2C ��Ϊ��ʽ���˽��
class GameMsg_S2C_SetCeremonyRookieRes : public GameMsg_Base
{
public:
    GameMsg_S2C_SetCeremonyRookieRes();
    virtual bool doEncode(CParamPool &IOBuff);

    unsigned char m_nErrorType;
};

/// C2S ȡ����ʽ����
class GameMsg_C2S_CancelCeremonyRookie : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_CancelCeremonyRookie);
    GameMsg_C2S_CancelCeremonyRookie();
    virtual bool doDecode(CParamPool &IOBuff);

    /// ���ID
    unsigned int m_nRoleId;
};

/// S2C ȡ����ʽ���˽��
class GameMsg_S2C_CancelCeremonyRookieRes : public GameMsg_Base
{
public:
    GameMsg_S2C_CancelCeremonyRookieRes();
    virtual bool doEncode(CParamPool &IOBuff);

    /// ����ö��-0��ʾ�ɹ�
    unsigned char m_nErrorType;
};

/// S2C ͬ����ʽ������Ϣ
class GameMsg_S2C_SyncCeremonyRookie : public GameMsg_Base
{
public:
    GameMsg_S2C_SyncCeremonyRookie();
    virtual bool doEncode(CParamPool &IOBuff);

    /// ���ID
    unsigned int m_nRoleId;
    /// �Ƿ���Ϊ����
    bool m_bIsRookie;
    CeremonyPlayerShowInfo m_PlayerInfo;
};

/// C2S ����������ʽ����
class GameMsg_C2S_KickCeremonyPlayer : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_KickCeremonyPlayer);
    GameMsg_C2S_KickCeremonyPlayer();
    virtual bool doDecode(CParamPool &IOBuff);

    unsigned int m_nRoleId;
};

/// S2C ����������ʽ������
class GameMsg_S2C_KickCeremonyPlayerRes : public GameMsg_Base
{
public:
    GameMsg_S2C_KickCeremonyPlayerRes();
    virtual bool doEncode(CParamPool &IOBuff);

    /// ����ö��-0��ʾ�ɹ�
    unsigned char m_nErrorType;
    /// ���ID
    unsigned int m_nRoleId;
};

/// C2S �������ҽ���
class GameMsg_C2S_SaveCeremonyIntruduction : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_SaveCeremonyIntruduction);
    GameMsg_C2S_SaveCeremonyIntruduction();
    virtual bool doDecode(CParamPool &IOBuff);

    std::string m_strIntruduction;
};

// S2C�������ҽ���
class GameMsg_S2C_SaveCeremonyIntruductionRes : public GameMsg_Base
{
public:
    GameMsg_S2C_SaveCeremonyIntruductionRes();
    ~GameMsg_S2C_SaveCeremonyIntruductionRes();
    virtual bool doEncode(CParamPool &IOBuff);

    unsigned char m_nErrorType;
};


/// C2S �����˵���
class GameMsg_C2S_RequestPraiseRookie : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_RequestPraiseRookie);
    GameMsg_C2S_RequestPraiseRookie();
    virtual bool doDecode(CParamPool &IOBuff);

    /// Ҫ�޵��������
    int m_nRookieIndex;
    /// Ҫ�޵�����ID
    unsigned int m_nRoleID;
    /// �ܹ�������
    int m_nCount;
};

/// S2C �����˵��޽��
class GameMsg_S2C_RequestPraiseRookieRes : public GameMsg_Base
{
public:
    GameMsg_S2C_RequestPraiseRookieRes();
    virtual bool doEncode(CParamPool &IOBuff);

    /// ����ö��-0��ʾ�ɹ�
    unsigned char m_nErrorType;
};

// ligh add.
/// s2c ���˻���޵�����
class GameMsg_S2C_SyncPraiseInfo : public GameMsg_Base
{
public:
    GameMsg_S2C_SyncPraiseInfo();
    ~GameMsg_S2C_SyncPraiseInfo();

    virtual bool doEncode(CParamPool &IOBuff);

    unsigned int m_roleID;              // �����޵�����
    unsigned int m_praiseNum;           // role��õ��ܵ�����
};

// �Զ�������ʱ�򣬷��俪��ʧ�ܣ��㲥����ر�
class GameMsg_S2C_CeremonyRoomStartFail : public GameMsg_Base
{
public:
    GameMsg_S2C_CeremonyRoomStartFail();
    ~GameMsg_S2C_CeremonyRoomStartFail();

    virtual bool doEncode(CParamPool &IOBuff);

    unsigned char m_nFailReason;
};

#endif // __GAMEMSG_CEREMONYROOMSC_H__
