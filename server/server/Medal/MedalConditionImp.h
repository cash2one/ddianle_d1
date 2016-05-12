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

        eMallConsumeType_NewMCoin = 1,          // ��Ʒ��M������
        eMallConsumeType_NewBindMCoin = 2,      // ��Ʒ����M������
        eMallConsumeType_NewMoney = 3,          // ��Ʒ����ȯ����

        eMallConsumeType_HotMCoin = 4,          // ������M������
        eMallConsumeType_HotBindMCoin = 5,      // ��������M������
        eMallConsumeType_HotMoney = 6,          // ��������ȯ����

        eMallConsumeType_CouponMCoin = 7,       // ��ȯ��M������
        eMallConsumeType_CouponBindMCoin = 8,   // ��ȯ����M������
        eMallConsumeType_CouponMoney = 9,       // ��ȯ����ȯ����

        eMallConsumeType_BuyItem = 10,          // ����ָ��������Ʒ
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

