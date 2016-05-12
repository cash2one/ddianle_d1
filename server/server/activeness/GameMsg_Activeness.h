/**
* Filename: GameMsg_Activeness
* Description: ��Ծ��ϵͳ��Ϣ����
* Copyright (C) 2015 ddianle Inc. All rights reserved.
* Author: xieshaobin
* Create time: 2015-05-14 17:46:12
*/

#ifndef _GAMEMSG_ACTIVENESS_H_
#define _GAMEMSG_ACTIVENESS_H_

#include "../share/CommonMsgDef.h"
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"
#include "../Pattern/Param.h"

#include <string>

//////////////////////////////////////////////////////////////////////////
// ��ϢID����
enum eMSG_Activeness
{
    MSG_ACTIVENESS_BEGIN = MSG_TYPE_Activeness,

    MSG_C2S_GetActivenessData,      // ��ȡ��Ծ��ϵͳ����
    MSG_S2C_GetActivenessDataRes,   // ��ȡ��Ծ��ϵͳ���ݵķ���

    MSG_C2S_GetActivenessReward,    // ��ȡ��Ծ��ϵͳ����
    MSG_S2C_GetActivenessRewardRes, // ��ȡ��Ծ��ϵͳ����Ľ��
};


//////////////////////////////////////////////////////////////////////////
// ��Ծ��ģ�鹫�����ݽṹ
//////////////////////////////////////////////////////////////////////////

class ActivenessTargetConfig
{
public:
    enum EType
    {
        EType_Invalid = 0,
        EType_Checkin = 1,          // ���ÿ��ǩ��
        EType_DailyQuest = 2,       // ����ճ�����
        EType_Dungeon = 3,          // ���ħ����ƹؿ�
        EType_DanceMode = 4,        // ��ɲ�ͬģʽ�ĸ���
        EType_QuickDance = 5,       // ʹ��һ�ο��ٿ�ʼ�����
        EType_VipReward = 6,        // ��ȡVIP����
        EType_SocialShare = 7,      // �����罻����
        EType_MagicArrayCall = 8,           // ����ħ�����ٻ�һ��
        EType_ClothEffectReplace = 9,       // ������Ч��ע
        EType_ClothEffectUpgrade = 10,      // ������Ч����
        EType_Max,
    };

public:
    ActivenessTargetConfig();
    virtual ~ActivenessTargetConfig();

    void Set(int nTargetType, int nTargetValue, int nTargetScore, const char *strDesc);

    virtual void doEncode(CParamPool & IOBuff);

    // Ŀ�����ƺ�Ŀ������һһ��Ӧ�����໥ת��
    static const std::string &GetNameFromType(int nType);
    static int GetTypeFromName(const std::string &name);

public:
    int m_nTargetType;          // Ŀ������
    int m_nTargetValue;         // ���Ŀ��������ֵ
    int m_nTargetScore;         // �����Ļ�Ծ�Ȼ���
    std::string m_strName;      // ���ƣ���Ŀ������һһ��Ӧ������set����ʱ���Զ�����
    std::string m_strDesc;      // ����
};

class ActivenessRewardConfig
{
public:
    ActivenessRewardConfig();
    virtual ~ActivenessRewardConfig();

public:
    void Set(int nRequireScore, const char *strMaleItem, const char *strFemaleItem, int nMoney,
        int nBindMCoin, const char *strVIPMaleItem, const char *strVIPFemaleItem, int nVIPMoney, int nVIPBindMCoin);

    virtual void doEncode(CParamPool & IOBuff, ESexType sex);

    int GetRequireScore() const;

public:
    int m_nRequireScore;
    CReward m_maleReward;
    CReward m_femaleReward;
    CReward m_vipMaleReward;
    CReward m_vipFemaleReward;
};

/////////////////////////////////////////////////
class GameMsg_C2S_GetActivenessData :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetActivenessData);
    GameMsg_C2S_GetActivenessData();
    ~GameMsg_C2S_GetActivenessData();
public:
    virtual bool doDecode(CParamPool &IOBuff);
};

/////////////////////////////////////////////////

// ��Ծ��Ŀ����ɽ���
class ActivenessTarget 
{
public:
    ActivenessTarget(const ActivenessTargetConfig & targetConfig);
    virtual ~ActivenessTarget(){};
 
private:
    ActivenessTarget & operator = (const ActivenessTarget& progress);

public:
    void doEncode(CParamPool & IOBuff);
    void SetProgress(int nProgress);
    bool IsTargetComplete();
    int  GetTargetType() const;
    int  GetTargetScore() const;
    int  GetProgress() const;
    virtual bool UpdateProgress(IParam & param);

public:
    const ActivenessTargetConfig &  m_TargetConfig;
    int                             m_nProgress;    // Ŀ�����
};

// ��Ծ��Ŀ����ɽ���
class ActivenessDanceModeTarget : public ActivenessTarget
{
public:
    ActivenessDanceModeTarget(const ActivenessTargetConfig & targetConfig);    
    virtual ~ActivenessDanceModeTarget() {};

private:
    ActivenessDanceModeTarget & operator = (const ActivenessDanceModeTarget & danceModeTarget);

public:
    void SetFinishedDanceMode(std::set<int> & finishedDanceMonde);
    bool UpdateProgress(IParam & param);
    void GetFinisheDanceMode(std::set<int> & finishedDanceMode) const;

public:
    std::set<int>                  m_setFinishedDanceMode; //�������ģʽ
};

class ActivenessTargetFactory
{
public:
    ActivenessTargetFactory();
    virtual ~ActivenessTargetFactory(){};

public:
    static ActivenessTargetFactory & Instance();
    ActivenessTarget * CreateTarget(const ActivenessTargetConfig & targetConfig);

};

//////////////////////////////////////////////////////////////////////////
// ��Ծ�Ƚ�����ȡ״̬
class ActivenessReward
{
public:
    ActivenessReward(const ActivenessRewardConfig & rewardConfig);
    ~ActivenessReward() {};

private:
    ActivenessReward & operator = (ActivenessReward & reward);


public:
    void doEncode(CParamPool & IOBuff, ESexType sex);

public:
    const ActivenessRewardConfig &  m_ActivenessRewardConfig;
    int                             m_nRewardIndex;     // �����ȼ����
    bool                            m_bReceived;       // �����Ƿ���ȡ

};

//////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_GetActivenessDataRes :
    public GameMsg_Base
{
public:
    GameMsg_S2C_GetActivenessDataRes();
    ~GameMsg_S2C_GetActivenessDataRes();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    ESexType m_eSex;                                        // ����Ա𣬷�����Ϣ����
    int m_nActiveness;                                      // ��ҵ��ջ�Ծ��
    std::list<ActivenessReward> m_listRewardState;     // ������ȡ���
    std::list<ActivenessTarget> m_listTargetProgress;// Ŀ�������
};


/////////////////////////////////////////////////
class GameMsg_C2S_GetActivenessReward :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetActivenessReward);
    GameMsg_C2S_GetActivenessReward();
    ~GameMsg_C2S_GetActivenessReward();
public:
    virtual bool doDecode(CParamPool &IOBuff);
public:
    int m_nRewardIndex;     // �����ȼ����
};

/////////////////////////////////////////////////
class GameMsg_S2C_GetActivenessRewardRes :
    public GameMsg_Base
{
public:
    GameMsg_S2C_GetActivenessRewardRes();
    ~GameMsg_S2C_GetActivenessRewardRes();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    std::string m_strError;
    int m_nRewardIndex;     // �����ȼ����
    bool m_bResult;         // �����Ƿ���ȡ�ɹ�
};


#endif //_GAMEMSG_ACTIVENESS_H_

