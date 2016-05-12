#ifndef	__GAMEMSG_CEREMONYROOM_H__
#define	__GAMEMSG_CEREMONYROOM_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DanceGroup.h"
#include "ceremonyRoomHead.h"


/// ���޼�¼
class CeremonyPraiseSendInfo
{
public:
    CeremonyPraiseSendInfo();
    void DoEncode(CParamPool &IOBuff);
    void DoDecode(CParamPool &IOBuff);

    /// ���޵����ID
    unsigned int m_nRoleID;
    /// ���޵��������
    std::string m_strRoleName;
    /// ����Ƿ���VIP
    bool m_bIsVIP;
    /// ���VIP�ȼ�
    unsigned short m_nVIPLevel;
    /// ������
    int m_nPraiseCount;

};

/// ���˻�õĵ��޼�¼
class CeremonyPraiseGainedInfo
{
public:
    CeremonyPraiseGainedInfo();
    void DoEncode(CParamPool &IOBuff);
    void DoDecode(CParamPool &IOBuff);

    /// ��õ��޵����ID
    unsigned int m_nRoleID;
    /// ��õ��޵��������
    std::string m_strRoleName;
    /// ����Ƿ���VIP
    bool m_bIsVIP;
    /// ���VIP�ȼ�
    unsigned short m_nVIPLevel;
    /// ��õ�����
    int m_nPraiseCount;
};

/// ��ʽ��־��Ϣ
class CeremonyRecordInfo
{
public:
    CeremonyRecordInfo();
    bool DoEncode(CParamPool &IOBuff);
    bool DoDecode(CParamPool &IOBuff);

    // recordID
    unsigned int m_nLogID;
    /// ������ʽ������ID
    unsigned int m_nApplicantID;
    /// ������ʽ����������
    std::string m_strApplicantName;
    /// �Ƿ���VIP
    bool m_bIsVIP;
    /// VIP�ȼ�
    unsigned short m_nVIPLevel;
    /// ������ʽ����
    unsigned int m_nCeremonyDate;
    /// ��ʽ���˵��޼�¼
    std::vector<CeremonyPraiseGainedInfo> m_vecPraiseInfo;
};



// ���봴������
class GameMsg_S2G_CeremonyApplyCreate : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_CeremonyApplyCreate);
    GameMsg_S2G_CeremonyApplyCreate();
    ~GameMsg_S2G_CeremonyApplyCreate() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int m_nDanceGroupID;
    unsigned int m_nRoleID;
    int m_nLine;
};

// ���봴��������
class GameMsg_G2S_CeremonyApplyCreateRet : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_CeremonyApplyCreateRet);
    GameMsg_G2S_CeremonyApplyCreateRet();
    ~GameMsg_G2S_CeremonyApplyCreateRet() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int m_nDanceGroupID;
    unsigned int m_nRoleID;
    unsigned char m_nErrorType; // 
};

// ����ɹ�����֪ͨ
class GameMsg_S2G_CeremonyCreateRoomNotify : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_CeremonyCreateRoomNotify);
    GameMsg_S2G_CeremonyCreateRoomNotify();
    ~GameMsg_S2G_CeremonyCreateRoomNotify() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int m_nDanceGroupID;
    unsigned int m_nCreateTime;
    unsigned int m_nOldCeremonyTimes;
    unsigned int m_nLine;
    std::string m_strLineName;
};

// ȡ�����䴴������ 
class GameMsg_S2G_CeremonyApplyCreateCancel : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_CeremonyApplyCreateCancel);
    GameMsg_S2G_CeremonyApplyCreateCancel();
    ~GameMsg_S2G_CeremonyApplyCreateCancel() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int m_nDanceGroupID;
    unsigned int m_nCreateID;
};

// ����ر�����
class GameMsg_S2G_CeremonyCloseRoomNotify : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_CeremonyCloseRoomNotify);
    GameMsg_S2G_CeremonyCloseRoomNotify();
    ~GameMsg_S2G_CeremonyCloseRoomNotify() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int m_nDanceGroupID;
    unsigned char m_nCloseReason; // �ر�ԭ��
};

class MsgStateChange
{
public:
    MsgStateChange();
    ~MsgStateChange();

    void doEncode(CParamPool &IOBuff);
    void doDecode(CParamPool &IOBuff);

    unsigned int m_nDanceGroupID;
    unsigned char m_nNewState;
};

// ����״̬���֪ͨ
class GameMsg_S2G_CeremonyRoomStateChange : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_CeremonyRoomStateChange);
    GameMsg_S2G_CeremonyRoomStateChange();
    ~GameMsg_S2G_CeremonyRoomStateChange() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    MsgStateChange m_msg;
};

class GameMsg_G2S_CeremonyRoomStateChange : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_CeremonyRoomStateChange);
    GameMsg_G2S_CeremonyRoomStateChange();
    ~GameMsg_G2S_CeremonyRoomStateChange() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    MsgStateChange m_msg;
};

class GameMsg_S2G_CeremonyRoomNewMember : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_CeremonyRoomNewMember);
    GameMsg_S2G_CeremonyRoomNewMember();
    ~GameMsg_S2G_CeremonyRoomNewMember();

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

    unsigned int m_nGroupID;
    std::vector<unsigned int> m_vecNewMember; // ��Щ�������ˣ���Ҫ�������Ա
};

// s->g д��־��Ϣ ��Ϊ����(��ʼд��д��ÿ�����˵ı�ͶƱ��¼��д�����)
class GameMsg_S2G_CeremonyRoomLogsStart : public GameMsg_Base
{
    //    MSG_S2G_CeremonyRoomLogsStart,				    // ��־��¼
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_CeremonyRoomLogsStart);
    GameMsg_S2G_CeremonyRoomLogsStart();
    ~GameMsg_S2G_CeremonyRoomLogsStart();

    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

    int m_nLine;                                        // 
    unsigned int m_nLogSeq;                                      // ����
    unsigned int m_nGroupID;

    CeremonyLogRoomInfo m_roomInfo;                     // 
};

class GameMsg_S2G_CeremonyRoomLogsItem : public GameMsg_Base
{
    //    MSG_S2G_CeremonyRoomLogsItem,                   // ÿ���˵ı�ͶƱ�˵���Ϣ
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_CeremonyRoomLogsItem);
    GameMsg_S2G_CeremonyRoomLogsItem();
    ~GameMsg_S2G_CeremonyRoomLogsItem();

    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

    int m_nLine;
    unsigned int m_nLogSeq;

    CeremonyPraiseLogInfo m_logItem;
};

class GameMsg_S2G_CeremonyRoomLogsEnd : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_CeremonyRoomLogsEnd);
    GameMsg_S2G_CeremonyRoomLogsEnd();
    ~GameMsg_S2G_CeremonyRoomLogsEnd();

    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

    int m_nLine;
    unsigned int m_nLogSeq;
};


/// C2S ������ʽ��־
class GameMsg_C2S_RequestCeremonyRecord : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_RequestCeremonyRecord);
    GameMsg_C2S_RequestCeremonyRecord();
    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);

    /// �ڼ�����־
    int m_nRecordIndex;
};

/// S2C ������ʽ��־���
class GameMsg_S2C_RequestCeremonyRecordRes : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_RequestCeremonyRecordRes);
    GameMsg_S2C_RequestCeremonyRecordRes();
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

    /// ��ǰ�ڼ�����־
    int m_nCurIndex;
    /// ��־����
    int m_nTotleCount;
    /// ��־��Ϣ
    /// ligh �����ID
    CeremonyRecordInfo m_recordInfo;
};


/// C2S ���������Ϣ
class GameMsg_C2S_RequestCeremonyPraise : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_RequestCeremonyPraise);
    GameMsg_C2S_RequestCeremonyPraise();
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

    unsigned int m_nLogID;
    /// ���ID
    unsigned int m_nRoleID;
    /// ����ҳ��
    int m_nPage;
    /// �������
    int m_nCount;
};

/// S2C ���������Ϣ���
class GameMsg_S2C_RequestCeremonyPraiseRes : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_RequestCeremonyPraiseRes);
    GameMsg_S2C_RequestCeremonyPraiseRes();
    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);

    unsigned int m_logID;
    /// ���ID
    unsigned int m_nRoleID;
    /// ��ǰҳ��
    int m_nCurPage;
    /// ������Ϣ����
    int m_nTotleCount;
    /// ������Ϣ�б�
    std::vector<CeremonyPraiseSendInfo> m_vecPraiseInfo;
};


/// S2C ͬ��������ʽ״̬
class GameMsg_S2C_SyncCeremonyState : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_SyncCeremonyState);

    GameMsg_S2C_SyncCeremonyState();
    ~GameMsg_S2C_SyncCeremonyState(){}
    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);

    /// ��ʽ״̬CeremonyState
    unsigned char m_nCeremonyState;
    int m_nLineID;
    /// �ٰ���ʽ����
    std::string m_strLineName;
};

// �¼� ����ʱ����
// ������ʽ������Ϣ
class GameMsg_S2C_SyncCeremonyConfig : public GameMsg_Base
{
public:
    GameMsg_S2C_SyncCeremonyConfig();
    ~GameMsg_S2C_SyncCeremonyConfig();

    virtual bool doEncode(CParamPool &IOBuff);

    unsigned char m_nFreeApplyTimes;
    unsigned char m_nApplyMoneyType;
    unsigned int m_nApplyMoneyCount;

    unsigned char m_nFreePraiseTimes;
    unsigned char m_nPraiseMoneyType;
    unsigned int m_nPraiseMoneyCount;

    float m_fStartDuration;          // ��������޵�ʱ��
};


// �¼�
// ������ʽ�Ѿ�ʹ�ô���֪ͨ
class GameMsg_S2C_SyncCermonyApplyTimes : public GameMsg_Base
{
public:
    GameMsg_S2C_SyncCermonyApplyTimes();
    ~GameMsg_S2C_SyncCermonyApplyTimes();
    virtual bool doEncode(CParamPool &IOBuff);

    unsigned char m_nUsedTimes;
};

//      MSG_S2G_CeremonyRoomStatics,                    // ����glbal����ͳ��
//     MSG_G2S_CeremonyRoomStatics,                    // �ظ�����ͳ��
class GameMsg_S2G_CeremonyRoomStatics : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_CeremonyRoomStatics);
    GameMsg_S2G_CeremonyRoomStatics();
    ~GameMsg_S2G_CeremonyRoomStatics();

    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);

};

class GameMsg_G2S_CeremonyRoomStatics : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_CeremonyRoomStatics);
    GameMsg_G2S_CeremonyRoomStatics();
    ~GameMsg_G2S_CeremonyRoomStatics();

    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);

    std::string m_strStatics; // ͳ�ƽ��
};

class CeremonyRoomSyncInfo
{
public:
    CeremonyRoomSyncInfo();
    ~CeremonyRoomSyncInfo() {}

    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

    unsigned int m_nRoomID; // ���ڷ�����˵����dancegroupID.
    unsigned char m_state; //
    unsigned int m_createTime;			// ��ʽ�ٰ�ʱ��
};

// ͬ��������Ϣ
class GameMsg_S2G_CeremonySyncRoomInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_CeremonySyncRoomInfo);
    GameMsg_S2G_CeremonySyncRoomInfo();
    ~GameMsg_S2G_CeremonySyncRoomInfo();

    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned int m_nLine;
    std::string m_strLineName;

    std::vector<CeremonyRoomSyncInfo> m_vecRooms;
};


#endif // __GAMEMSG_CEREMONYROOM_H__


