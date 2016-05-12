#ifndef _MEDALCONDITIONVIP_H__
#define _MEDALCONDITIONVIP_H__

#include "MedalCondition.h"
#include "../Rank/RankDefine.h"
#include <string>

//////////////////////////////////////////////////////////////////////////
class CMedalConditionVip : public IMedalCondition
{
public:
    CMedalConditionVip(IEntity *owner);
    virtual ~CMedalConditionVip(){};

    virtual bool process(IParam &param);
    virtual bool check();
    virtual bool CheckIsAllowGet(int nTotalProgress);
};

//////////////////////////////////////////////////////////////////////////
class CMedalConditionLevel : public IMedalCondition
{
public:
    CMedalConditionLevel(IEntity *owner);
    virtual ~CMedalConditionLevel(){};

    virtual bool process(IParam &param);
    virtual bool check();
};

//////////////////////////////////////////////////////////////////////////
class CMedalConditionMallConsume : public IMedalCondition
{
public:
    enum EMallConsumeType
    {
        eMallConsumeType_Invalid = 0,

        eMallConsumeType_NewMCoin = 1,          // 新品区M币消费
        eMallConsumeType_NewBindMCoin = 2,      // 新品区绑定M币消费
        eMallConsumeType_NewMoney = 3,          // 新品区金券消费

        eMallConsumeType_HotMCoin = 4,          // 热卖区M币消费
        eMallConsumeType_HotBindMCoin = 5,      // 热卖区绑定M币消费
        eMallConsumeType_HotMoney = 6,          // 热卖区金券消费

        eMallConsumeType_CouponMCoin = 7,       // 收券区M币消费
        eMallConsumeType_CouponBindMCoin = 8,   // 收券区绑定M币消费
        eMallConsumeType_CouponMoney = 9,       // 收券区金券消费

        eMallConsumeType_BuyItem = 10,          // 购买指定类型物品
    };

public:
    CMedalConditionMallConsume(int nConsumeType, IEntity *owner);
    virtual ~CMedalConditionMallConsume(){};

    virtual bool process(IParam &param);
    virtual bool check();

    void SetConsumeType(int nConsumeType);

protected:
    int m_nConsumeType;

private:
    int GetContainCount(std::list<unsigned int> destlist, std::list<unsigned int> findlist);
};

//////////////////////////////////////////////////////////////////////////
class CMedalConditionHaveItem : public IMedalCondition
{
public:
    CMedalConditionHaveItem(IEntity *owner);
    virtual ~CMedalConditionHaveItem(){};

    virtual bool process(IParam &param);
    virtual bool check();
};

class CMedalConditionHaveEffect : public IMedalCondition
{
public:
    CMedalConditionHaveEffect(IEntity *owner);
    virtual ~CMedalConditionHaveEffect(){};

    virtual bool process(IParam &param);
    virtual bool check();
};
//////////////////////////////////////////////////////////////////////////
class CMedalConditionRank : public IMedalCondition
{
public:
    CMedalConditionRank(ERankDataType eRankType, IEntity *owner);
    virtual ~CMedalConditionRank() {}

public:
    virtual bool process(IParam &param);
    virtual bool check();
    virtual bool CheckIsAllowGet(int nTotalProgress);
private:
    ERankDataType   m_eRankType;
};

//////////////////////////////////////////////////////////////////////////
class CMedalConditionMedalScore : public IMedalCondition
{
public:
    CMedalConditionMedalScore(IEntity *owner);
    virtual ~CMedalConditionMedalScore(){};

public:
    virtual bool process(IParam &param);
    virtual bool check();
};

//////////////////////////////////////////////////////////////////////////
class CMedalConditionClothEffect : public IMedalCondition
{
public:
    CMedalConditionClothEffect(IEntity *owner);
    virtual ~CMedalConditionClothEffect(){};

public:
    virtual bool process(IParam &param);
    virtual bool check();
};

/////////////////////////////////////////////////////////////////////////
class CMedalConditionEquipCount : public IMedalCondition
{
public:
    CMedalConditionEquipCount(IEntity *owner);
    virtual ~CMedalConditionEquipCount(){};

public:
    virtual bool process(IParam &param);
    virtual bool check();
};


#endif //_MEDALCONDITIONVIP_H__

