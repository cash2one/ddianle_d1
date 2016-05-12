#ifndef __GAMEMSG_MEDAL_H__
#define __GAMEMSG_MEDAL_H__

#include "../../datastructure/ImageData.h"
#include "../../socket/GameMsg_Base.h"
#include "../share/ServerMsgDef.h"
#include <string>
#include <vector>


enum eMSM_MEDAL
{
    MSG_MEDAL_BEGIN = MSG_TYPE_MEDAL,

    MSG_C2S_GET_MEDALINFO,                    //请求勋章详细数据
    MSG_S2C_GET_MEDALINFO,                    //返回勋章详细

    MSG_C2S_GET_CONDITIONINFO,                //请求勋章条件
    MSG_S2C_GET_CONDITIONINFO,                //返回请求勋章条件

    MSG_C2S_MEDALHANDBOOK,                    //请求勋章图鉴
    MSG_S2C_MEDALHANDBOOK,                    //返回勋章图鉴

    MSG_C2S_MEDAL_EXCHANGE_INFO,                   //请求勋章兑换数据
    MSG_S2C_MEDAL_EXCHANGE_INFO,                   //返回勋章兑换数据

    MSG_C2S_MEDAL_SHOW,                       //请求设置勋章显示
    MSG_S2C_MEDAL_SHOW,                       //返回设置勋章展示

    MSG_C2S_MEDAL_EXCHANGE,                   //请求勋章兑换
    MSG_S2C_MEDAL_EXCHANGE,                   //返回勋章兑换

    MSG_C2S_MEDAL_ACT_INFO,                    //请求激活勋章数据
    MSG_S2C_MEDAL_ACT_INFO,                    //返回激活勋章数据

    MSG_C2S_MEDAL_CONDITION_REWARD,                     //请求领取
    MSG_S2C_MEDAL_CONDITION_REWARD,                     //返回领取

    MSG_S2C_MEDAL_SHOW_INIT,                    //登录后发送客户端需要的显示勋章数据

//    MSG_S2C_SyncMedalInfoToOthers,              // 同步勋章信息给其他玩家

    MSG_S2C_PUSH_CONDITION_COMPLETE_MEDAL,             //推送完成条件的 奖励勋章

    MSG_C2S_GetMedalScore,
    MSG_S2C_GetMedalScore,

    MSG_C2S_MEDALSCORE_DESCRIBE,                   //请求勋章积分描述
    MSG_S2C_MEDALSCORE_DESCRIBE,                   //返回勋章积分描述
};

class GameMsg_C2S_GetMedalInfo: public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetMedalInfo);
    GameMsg_C2S_GetMedalInfo();
    ~GameMsg_C2S_GetMedalInfo();
public:
    virtual bool doDecode(CParamPool &IOBuff);
public:
    std::vector<int> m_vecMedalID;
};

class GameMsg_S2C_GetMedalInfo: public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetMedalInfo);
    GameMsg_S2C_GetMedalInfo();
    ~GameMsg_S2C_GetMedalInfo();

    class PerMedalInfo
    {
    public:
        PerMedalInfo();
        ~PerMedalInfo(){};
    public:
        bool doEncode(CParamPool &IOBuff);

    public:
        int nMedalID;
        int nState;
        int nNeedMedalMoney;
        int nType;
        std::string strName;
        std::string strIcon;
        std::string strDescribe;
    };

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    std::list<PerMedalInfo> m_listPerMedalInfo; 
};
//////////////////////////////////////////////////////////////////////////

class GameMSg_C2S_GetConditionInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMSg_C2S_GetConditionInfo);
    GameMSg_C2S_GetConditionInfo();
    ~GameMSg_C2S_GetConditionInfo();
public:
    virtual bool doDecode(CParamPool &IOBuff);
};

class GameMSg_S2C_GetConditionInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMSg_S2C_GetConditionInfo);
    GameMSg_S2C_GetConditionInfo();
    ~GameMSg_S2C_GetConditionInfo();

public:
    class PerConditionInfo
    {
    public:
        PerConditionInfo();
        ~PerConditionInfo(){}
    
    public:
        bool doEncode(CParamPool &IOBuff);
    
    public:
        int nCondititonID;
        int nTypeID;
        int nRewardMedalID;
        int nCurProgress;
        int nTotalProtgress;
        unsigned char isHaveGot;
        std::list<unsigned int > m_listDestID;
    };

public:
    bool doEncode(CParamPool &IOBuff);

public:
    std::list<PerConditionInfo> m_listPerConditionInfo;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_MedalHandBook: public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_MedalHandBook);
    GameMsg_C2S_MedalHandBook();
    ~GameMsg_C2S_MedalHandBook();
public:
    virtual bool doDecode(CParamPool &IOBuff);
};

class GameMsg_S2C_MedalHandBook: public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_MedalHandBook);
    GameMsg_S2C_MedalHandBook();
    ~GameMsg_S2C_MedalHandBook();
public:
    virtual bool doEncode(CParamPool &IOBuff);
public:
    class PerMedalInfo
    {
    public:
        PerMedalInfo(){}
        ~PerMedalInfo(){}

    public:
        bool doEncode(CParamPool &IOBuff);

    public:
        int m_nMedalId;
        int m_nType;
        int m_nDuration;
    };
public:
    std::list<PerMedalInfo> m_listPerMedalInfo;
};
//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_Medal_Exchange_Info: public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_Medal_Exchange_Info);
    GameMsg_C2S_Medal_Exchange_Info();
    ~GameMsg_C2S_Medal_Exchange_Info();
public:
    virtual bool doDecode(CParamPool &IOBuff);
};

class GameMsg_S2C_Medal_Exchange_Info:public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_Medal_Exchange_Info);
    GameMsg_S2C_Medal_Exchange_Info();
    ~GameMsg_S2C_Medal_Exchange_Info();

public:
    class PerExchangeInfo
    {
    public:
        PerExchangeInfo(){}
        ~PerExchangeInfo(){}
        bool doEncode(CParamPool &IOBuff);

        int m_nMedalId;
        int m_nPrice;
    };

    virtual bool doEncode(CParamPool &IOBuff);

public:
    std::list<PerExchangeInfo> m_listPerExchangeInfo;

};
//////////////////////////////////////////////////////////////////////////
class GameMSg_C2S_Medal_Show: public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMSg_C2S_Medal_Show);
    GameMSg_C2S_Medal_Show();
    ~GameMSg_C2S_Medal_Show();
public:
    virtual bool doDecode(CParamPool &IOBuff);
public:
    std::list<int> m_listMedalID;

};

enum eSetMedalShowRes
{
    eSetMedalShowFail = 0,
    eSetMedalShowSuccess= 1,
};

class GameMSg_S2C_Medal_Show:public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMSg_S2C_Medal_Show);
    GameMSg_S2C_Medal_Show();
    ~GameMSg_S2C_Medal_Show();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char m_nReCode;
    std::list<int> m_listPerShowID;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_Medal_Exchange: public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_Medal_Exchange);
    GameMsg_C2S_Medal_Exchange();
    ~GameMsg_C2S_Medal_Exchange();
public:
    virtual bool doDecode(CParamPool &IOBuff);

    int m_nMedalID;
    unsigned char m_byScene;
};

enum eMedalExchangeRes
{
    eMedalExchangeFail = 0,
    eMedalExchangeSuccess = 1,
};

class GameMsg_S2C_Medal_Exchange:public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_Medal_Exchange);
    GameMsg_S2C_Medal_Exchange();
    ~GameMsg_S2C_Medal_Exchange();

public:

    virtual bool doEncode(CParamPool &IOBuff);

public:
    int m_nMedalID;
    int m_nRetCode;
    int m_nMedalType;

};

//////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_Medal_ActivityInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_Medal_ActivityInfo);
    GameMsg_C2S_Medal_ActivityInfo();
    ~GameMsg_C2S_Medal_ActivityInfo(){};
public:
    virtual bool doDecode(CParamPool &IOBuff){return true;}
};

class GameMsg_S2C_Medal_ActivityInfo:public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_Medal_ActivityInfo);
    GameMsg_S2C_Medal_ActivityInfo();
    ~GameMsg_S2C_Medal_ActivityInfo(){};

    virtual bool doEncode(CParamPool &IOBuff);

public:
    class perShowInfo
    {
    public:
        int m_nID;
        int m_nType;
        std::string m_strIcon;

    public:
        bool doEncode(CParamPool &IOBuff)
        {
            IOBuff.AddInt(m_nID);
            IOBuff.AddInt(m_nType);
            IOBuff.AddStringW(m_strIcon);
            return true;
        }
    };
    std::list<perShowInfo> m_listPerInfo;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_Medal_GetConditionReward : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_Medal_GetConditionReward);
    GameMsg_C2S_Medal_GetConditionReward();
    ~GameMsg_C2S_Medal_GetConditionReward(){};
public:
    virtual bool doDecode(CParamPool &IOBuff)
    {
        m_nConditionid = IOBuff.GetInt();
        m_bySceneid = IOBuff.GetUChar(); 
        return true;
    }

public:
    int m_nConditionid;
    unsigned char m_bySceneid;
};

enum eMedalConditionReward
{
    eMedalGetConditionRewardFail = 0,
    eMedalGetConditionRewardSuccess = 1,
    emedalGetConditionRewardNull = 2,
    eMedalGetConditionRewardAlreadyGet = 3,
    eMedalGetConditionRewardVipTimeOut = 4,
};

class GameMsg_S2C_Medal_GetConditionReward: public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_Medal_GetConditionReward);
    GameMsg_S2C_Medal_GetConditionReward();
    ~GameMsg_S2C_Medal_GetConditionReward(){};
public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    int m_nMedalID;
    int m_nRetCode;
    int m_nMedalType;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_MedalInit : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_MedalInit);
    GameMsg_S2C_MedalInit();
    ~GameMsg_S2C_MedalInit(){};
public:
    virtual bool doEncode(CParamPool &IOBuff);

public:

    class perShowMedal
    {
    public:
        perShowMedal(){}
        ~perShowMedal(){}

        bool doEncode(CParamPool &IOBuff)
        {
            IOBuff.AddInt(m_MedalID);
            IOBuff.AddStringW(m_strIcon);
            return true;
        }
        int m_MedalID;
        std::string m_strIcon;
    };

    std::list<perShowMedal> m_listPerShowMedal;
};


class GameMsg_S2C_PushCompleteMedal : public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_PushCompleteMedal);

    virtual bool doEncode(CParamPool &IOBuff);

    std::set<int> m_setAllMedalId;
};


class GameMsg_C2S_GetMedalScore : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetMedalScore);
    GameMsg_C2S_GetMedalScore();
    ~GameMsg_C2S_GetMedalScore() {}

public:
    bool doDecode(CParamPool &IOBuff);
};

class GameMsg_S2C_GetMedalScore : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetMedalScore);
    GameMsg_S2C_GetMedalScore();
    ~GameMsg_S2C_GetMedalScore() {}

public:
    bool doEncode(CParamPool &IOBuff);

    int m_nScore;
};


class GameMsg_C2S_Medal_GetScoreDescribe : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_Medal_GetScoreDescribe);
    GameMsg_C2S_Medal_GetScoreDescribe();
    ~GameMsg_C2S_Medal_GetScoreDescribe(){};
public:
    virtual bool doDecode(CParamPool &IOBuff)
    {
        return true;
    }
};

class GameMsg_S2C_Medal_GetScoreDescribe : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_Medal_GetScoreDescribe);
    GameMsg_S2C_Medal_GetScoreDescribe();
    ~GameMsg_S2C_Medal_GetScoreDescribe(){};
public:
    virtual bool doEncode(CParamPool &IOBuff)
    {
        IOBuff.AddStringW(m_strDescribe);
        return true;
    }

    std::string m_strDescribe;
};

#endif

