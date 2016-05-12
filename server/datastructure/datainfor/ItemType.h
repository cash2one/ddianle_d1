/********************************************************************
	created:	2010/12/04
	filename: 	DataStructure\unittype.h
	file ext:	h
	author:		meijiuhua
	
	purpose:	����unit��type,unit��ָ���п��Է��ڵ������Ķ�����
				���������ȣ��Ҽ��ȣ�Ѫƿ���Լ�������Ʒ
*********************************************************************/

#ifndef __ITEM_TYPE_H__
#define __ITEM_TYPE_H__

/////////////////////////////////////////////////////////////////////
//һ������
/////////////////////////////////////////////////////////////////////

//װ��������
enum EItemClassType
{
    eItemClassType_None = 0,
    eItemClassType_Equip,       //װ��
    eItemClassType_Expendable,  //����(����Ʒ)
    eItemClassType_Max
};


////////////////////////////////////////////////////////////////////
//��������
////////////////////////////////////////////////////////////////////

// װ��������
enum EItemEquipType
{
    eItemEquipType_None = 0,
    eItemEquipType_Cloth,       //�·�����Ʒ
    eItemEquipType_Badge,       //����
    eItemEquipType_Horse,       //����
    eItemEquipType_Max
};

// ����Ʒ������
enum EItemExpendableType
{
    eItemExpendableType_None = 0,
    eItemExpendableType_Function,   //������
    eItemExpendableType_Social,     //�罻��
    eItemExpendableType_Addition,   //������
    eItemExpendableType_Packet,     //�����
    eItemExpendableType_Box,        //������
    eItemExpendableType_TransForm,  //������
    eItemExpendableType_DynamicBox, //��̬����
    eItemExpendableType_PetEgg,     //���ﵰ
    eItemExpendableType_PetItem,    //������Ʒ
    eItemExpendableType_EffectStone,//��Чʯ
    eItemExpendableType_Max
};


////////////////////////////////////////////////////////////////////
// ��������
////////////////////////////////////////////////////////////////////

// �·�������
enum EItemClothType
{
    eItemClothType_Invalid = -1,
    eItemClothType_Hair,          //����
    eItemClothType_Face,          //����
    eItemClothType_Body,          //����
    eItemClothType_Glove,         //����
    eItemClothType_Leg,           //��װ
    eItemClothType_Cap,           //ͷ��
    eItemClothType_FacialContent, //����
    eItemClothType_Shoulder,      //���
    eItemClothType_Wing,          //���
    eItemClothType_LeftHand,      //���ֳ�
    eItemClothType_RightHand,     //���ֳ�
    eItemClothType_Wrist,         //����
    eItemClothType_Hip,           //�β�
    eItemClothType_Sock,          //����
    eItemClothType_Feet,          //Ь�� 
    eItemClothType_Skin,          //��ɫ 
    eItemClothType_All,           //ȫ��װ����ItemCloth_Type_Skin
    eItemClothType_Suit,          //��װ
    eItemClothType_Max,

};

// ������������
enum EClothAttrType
{
    EClothAttrType_None,
    EClothAttrType_HuaLi,       // ���� 
    EClothAttrType_JianYue,     // ��Լ
    EClothAttrType_YouYa,       // ����
    EClothAttrType_HuoPo,       // ����
    EClothAttrType_ChengShu,    // ����
    EClothAttrType_KeAi,        // �ɰ�
    EClothAttrType_XingGan,     // �Ը�
    EClothAttrType_QingChun,    // �崿
    EClothAttrType_BaoNuan,     // ��ů
    EClothAttrType_QingLiang,   // ����
};

// ��������
enum EItemBadgeType
{
    eItemBadgeType_WingdingRing  =  1,  //����ָ
    eItemBadgeType_SpecialRing,         //���Խ�ָ
    eItemBadgeType_VIP,                 //VIP����
    eItemBadgeType_Show,                //չʾ��Ч
    eItemBadgeType_ExpEffect,           //����Ч��
    eItemBadgeType_CardDecoration,      //��Ƭװ��
};

// �·���Ч��ɫ����
enum ESevenColorType
{
    eSevenColorType_None,
    eSevenColorType_Color1,
    eSevenColorType_Color2,
    eSevenColorType_Color3,
    eSevenColorType_Color4,
    eSevenColorType_Color5,
    eSevenColorType_Color6,
    eSevenColorType_Color7,
    eSevenColorType_Max,
};

enum EItemError
{
	EItemError_Success = 0,
	EItemError_CannotUse,//�趨Ϊ����ʹ�õ���Ʒ
	EItemUse_NeedCoolDown,//cooldownʱ��û��
	EItemUse_NeedAttrib,//��������
	EItemUse_NeedSex,//�Ա𲻷�
	EItemUse_NeedLevel,//���𲻹�
	EItemUse_EquipPos,//װ��λ�ò���ȷ
	EItemUse_ItemPos,//��Ʒ����ָ��λ����
	EItemUse_LegWithBodyleg,//��ȫ���·���ʱ���ܴ���
	EItemUse_BodylegWithLeg,//�����ʱ���ܴ�ȫ���·�
	EItemUse_ColumnFull,//��Ʒ����
	EItemUse_UniqueExist,//��ӵ��Ψһ����Ʒ
	EItemUse_RequireEqup,//ֻ�ܶ�װ�����е�װ�����в���
	EItemUse_DropPlayerColumn,//����ֱ�Ӱ�װ�������ϵ���Ʒ����
	EItemUse_CannotDrop,//����Ʒ���ܶ���
	EItemUse_CreatItemError,//������Ʒ����
	EItemUse_Busy,//������������������û�취���������
	EItemUse_NeedTarget,//��Ҫָ��Ŀ��
	EItemUse_NoTargetSelf,//�������Լ�ΪĿ��
	EItemUse_InvalidTarget,//�Ƿ�Ŀ��
	EItemUse_LevelUpPlayerColumn,//����ֱ������װ�������ϵ�װ��
	EItemUse_LevelUpNoEquip,//ֻ������װ��
	EItemUse_CannotLevelUp,//����Ʒ��������
	EItemUse_ColumnFullForQuest,//��Ʒ����,���ܽ�����
	EItemUse_MaxMoney,//��Ǯ�Ѵﵽ���ֵ,�����ٻ�ý�Ǯ��
	EItemUse_OnlyMale,//ֻ�ܶ����Խ�ɫʹ��
	EItemUse_OnlyFemale,//ֻ�ܶ�Ů�Խ�ɫʹ��
	EItemUse_BeibaoKuozhan_Max,//������չ�Ѿ��ﵽ����
	EItemUse_ConsortOffline,//��ż������
	EItemUse_SamePos,//��ͬ��λ��
	EItemUse_ScriptError,//�ű�����
	EItemUse_ScriptErrorReturn,//�ű�����ִ�д���
	EItemUse_TitleCannotDrop,//�ƺŲ��ܶ���
	EItemUse_EmptyPos,//��λ����û����Ʒ��
	EItemUse_Unknown,//δ֪��ԭ��
	EItemUse_VIPRequire,//VIP��Ʒ
	EItemUse_PoorVIPLevel,//VIP�ȼ�����
	EItemUse_ParaError,
	EItemUse_Badge_Grid_Grid_Full,//���¸�������
	EItemUse_Badage_Grid_More_Grid,	//���и���������Ҫ�����µĻ��¸���//װ����Ʒ�Ĵ��������������Ϊװ������ʹ�õ���UseItem�Ľӿڣ����ֵ�����EquipItem�Ľӿ�
	EItemUse_NotMarriageUseRing,//û��鲻��ʹ�ý�ָ
	EItemUse_HaveEquipMarriageRing,    //�Ѿ��������
	EItemUse_PatchUseCountNot,      // ����ʹ����Ʒ��������
	EItemUse_NOtCanEquip,           // ����װ��
	EItemUse_FunctionNotOpen,       // ����δ����
	EItemUse_ConfigError,           // ���ô���
	EItemUse_ParameterErr,          // ��������
	EItemError_NotEnoughItem,       // ��Ʒ��������
	EItemUse_ClothHasEffect,        // �·��Ѿ�������Ч
	EItemUse_EffectConfigNot,       // ��Ч���ò�����
	EItemUse_RemoveItemError,       // ɾ����Ʒʧ��
	EItemUse_MaxLevel,              // �Ѿ������ȼ�
	EItemUse_ClothEffectExist,      // �·���Ч�Ѿ�����
	EItemUse_ClothNotAddEffect,     // �·����ܼ���Ч
	EItemUse_ClothPosNotOpen,       // �·���λδ����������Ч
	EItemUse_NotEnoughMoney,        // Ǯ����
    EItemUse_NotInDanceGroup,       // ���Ų�����
};


#endif //__ITEM_TYPE_H__

