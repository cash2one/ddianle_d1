#ifndef __GAMEMSG_BIGMAMASERVER_H__
#define __GAMEMSG_BIGMAMASERVER_H__

#include "../../socket/GameMsg_Base.h"
#include "DataStruct_BigMama.h"


// �����ȡ�ؿ���Ϣ
class GameMsg_S2G_BigMamaStageInfoRequest : public GameMsg_Base
{// ���Զ�ͬ���ؿ���Ϣ
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2G_BigMamaStageInfoRequest)
public:
//     GameMsg_S2G_BigMamaStageInfoRequest();
//     ~GameMsg_S2G_BigMamaStageInfoRequest();
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

    unsigned int m_nDanceGroupID;

};

// �ظ�������
class GameMsg_G2S_BigMamaStageInfoNotify : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_G2S_BigMamaStageInfoNotify)
public:
//     GameMsg_G2S_BigMamaStageInfoNotify();
//     ~GameMsg_G2S_BigMamaStageInfoNotify();
    bool doEncode(CParamPool& IOBuff);
    bool doDecode(CParamPool& IOBuff);

    unsigned char m_result;         // 0���ɹ��� 1����Ѿ��ر�
    BigMamaProperty m_property;   // ������ʾ�Ĺؿ���Ϣ
};


// ����һ��bigmama
class GameMsg_S2G_BigMamaBeatSucess : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2G_BigMamaBeatSucess)
public:
//     GameMsg_S2G_BigMamaBeatSucess();
//     ~GameMsg_S2G_BigMamaBeatSucess();
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

    unsigned int m_nDanceGroupID;               // ��������ID
    unsigned int m_nRoleID;
    unsigned char m_nRound;                // �ڼ��ֵ�boss

};

// �״̬��Ϣ���֪ͨ (��ʼ���߹ر�)
class GameMsg_G2S_BigMamaStateNotify : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_G2S_BigMamaStateNotify)

public:
//     GameMsg_G2S_BigMamaStateNotify();
//     ~GameMsg_G2S_BigMamaStateNotify();
    bool doEncode(CParamPool& IOBuff);
    bool doDecode(CParamPool& IOBuff);

    // ����״̬: 0-�ر� 1-����
    unsigned char m_nState;
    unsigned int m_nStartTime;
    unsigned int m_nEndTime;
};

// �ؿ���Ϣͬ��
class GameMsg_G2S_BigMamaSyncStageInfo : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_G2S_BigMamaSyncStageInfo)
public:
//     GameMsg_G2S_BigMamaSyncStageInfo();
//     ~GameMsg_G2S_BigMamaSyncStageInfo();

    bool doEncode(CParamPool& IOBuff);
    bool doDecode(CParamPool& IOBuff);

    BigMamaProperty m_property;
};

class GameMsg_G2S_BigMamaInfoNotifyOnLogin : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_G2S_BigMamaInfoNotifyOnLogin)
public:
//     GameMsg_G2S_BigMamaInfoNotifyOnLogin();
//     ~GameMsg_G2S_BigMamaInfoNotifyOnLogin();
    bool doEncode(CParamPool& IOBuff);
    bool doDecode(CParamPool& IOBuff);

    bool m_bOpened;
    unsigned int m_nStartTime;
    unsigned int m_nEndTime;
    BigMamaProperty m_property;
};

class GameMsg_G2S_BigMamaInfoNotifyOnJoinDanceGroup : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_G2S_BigMamaInfoNotifyOnJoinDanceGroup)
public:
    bool doEncode(CParamPool& IOBuff);
    bool doDecode(CParamPool& IOBuff);

    bool m_bOpened;
    unsigned int m_nStartTime;
    unsigned int m_nEndTime;
    BigMamaProperty m_property;
};


class GameMsg_S2G_BigMamaFightRequest : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2G_BigMamaFightRequest)
public:
//     GameMsg_S2G_BigMamaFightRequest();
//     ~GameMsg_S2G_BigMamaFightRequest();
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);


    unsigned int m_nDanceGroupID;
    unsigned int m_nRoleID;
    unsigned char m_nPhoneOS;
};

class GameMsg_G2S_BigMamaFightResult : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_G2S_BigMamaFightResult)
public:
//     GameMsg_G2S_BigMamaFightResult();
//     ~GameMsg_G2S_BigMamaFightResult();
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

    EFightBigMamaResult m_nResult;
    unsigned int m_nStageID;
    unsigned char m_nRound;
    unsigned char m_nPhoneOS;
};


class GameMsg_S2G_BigMamaGetRewardRequest : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2G_BigMamaGetRewardRequest)
public:
//     GameMsg_S2G_BigMamaGetRewardRequest();
//     ~GameMsg_S2G_BigMamaGetRewardRequest();
    bool doEncode(CParamPool& IOBuff);
    bool doDecode(CParamPool& IOBuff);

    unsigned int m_nDanceGroupID;
    unsigned int m_nRoleID; 
    bool m_bJoined; // �Ƿ�μ��ˡ�
};


class GameMsg_G2S_BigMamaGetRewardResult : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_G2S_BigMamaGetRewardResult)

public:
//     GameMsg_G2S_BigMamaGetRewardResult();
//     ~GameMsg_G2S_BigMamaGetRewardResult();
    bool doEncode(CParamPool& IOBuff);
    bool doDecode(CParamPool& IOBuff);

    // ����ö��: 0-�ɹ�
    EBigMamaRewardResult m_nErrorType;
    // ������Ϣ
    unsigned char m_rewardType;
};

#endif // __GAMEMSG_BIGMAMASERVER_H__

