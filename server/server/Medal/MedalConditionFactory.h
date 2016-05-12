#ifndef __MEDALCONDITIONFACTORY_H__
#define __MEDALCONDITIONFACTORY_H__

#include "MedalCondition.h"


enum EMedalProgressType
{
    EMedalProgressType_None                         = 0,
    EMedalProgressType_NewProductBuy_MB             = 1,        //������Ʒ��MB��������
    EMedalProgressType_NewProductBuy_BindMB         = 2,        //������Ʒ����MB��������
    EMedalProgressType_NewProductBuy_GOLD           = 3,        //������Ʒ��������������
    EMedalProgressType_HotSaleBuy_MB                = 4,        //����������MB��������
    EMedalProgressType_HotSaleBuy_BindMB            = 5,        //������������MB��������
    EMedalProgressType_HotSaleBuy_GOLD              = 6,        //����������������������
    EMedalProgressType_NeedNoteBuy_MB               = 7,        //�����վ���MB��������
    EMedalProgressType_NeedNoteBuy_BindMB           = 8,        //�����վ�����MB��������
    EMedalProgressType_NeedNoteBuy_GOLD             = 9,        //�����վ���������������
    EMedalProgressType_VipLevel                     = 10,       //����VIP�ȼ�����
    EMedalProgressType_BuyItem                      = 11,       //���������Ʒ����
    EMedalProgressType_BuyRank                      = 12,       //����������а���������
    EMedalProgressType_TrumpetRank                  = 13,       //�������������а���������
    EMedalProgressType_LevelRank                    = 14,       //���������������а���������
    EMedalProgressType_HonourDanceGroupRank         = 15,       //���������������а���������
    EMedalProgressType_ManWifeRank                  = 16,       //�������Ƿ������а���������
    EMedalProgressType_Level                        = 17,       //����ȼ�����
    EMedalProgressType_HaveItemType                 = 18,       //������ӵ�е���Ʒ����
    EMedalProgressType_HaveEffect                   = 19,       //�����Ч����
    EMedalProgressType_MedalScore                   = 20,       //����ѫ�»���
    EMedalProgressType_ClothEffect                  = 21,       //��������ֵ
    EMedalProgressType_EquipCount                   = 22,       //�����ղ�����
    EMedalProgressType_MedalScoreRank               = 23,       //����ѫ�»������а���������
    EMedalProgressType_ClothEffectRank              = 24,       //��������ֵ���а����������
    EMedalProgressType_EquipCountRank               = 25,       //�����ղ��������а����������
};

class CMedalConditionFactory
{
public:
    static IMedalCondition* CreateMedalCondition(int nType, IEntity *owner);
};

#endif //__MEDALCONDITIONFACTORY_H__

