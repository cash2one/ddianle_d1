/********************************************************************
	created:	2010/12/04
	filename: 	DataStructure\unittype.h
	file ext:	h
	author:		meijiuhua
	
	purpose:	所有unit的type,unit是指所有可以放在道具栏的东西，
				包括武器等，挂件等，血瓶等以及其他物品
*********************************************************************/

#ifndef __ITEM_TYPE_H__
#define __ITEM_TYPE_H__

/////////////////////////////////////////////////////////////////////
//一级类型
/////////////////////////////////////////////////////////////////////

//装备，道具
enum EItemClassType
{
    eItemClassType_None = 0,
    eItemClassType_Equip,       //装备
    eItemClassType_Expendable,  //道具(消耗品)
    eItemClassType_Max
};


////////////////////////////////////////////////////////////////////
//二级类型
////////////////////////////////////////////////////////////////////

// 装备子类型
enum EItemEquipType
{
    eItemEquipType_None = 0,
    eItemEquipType_Cloth,       //衣服和饰品
    eItemEquipType_Badge,       //徽章
    eItemEquipType_Horse,       //坐骑
    eItemEquipType_Max
};

// 消耗品子类型
enum EItemExpendableType
{
    eItemExpendableType_None = 0,
    eItemExpendableType_Function,   //功能类
    eItemExpendableType_Social,     //社交类
    eItemExpendableType_Addition,   //增益类
    eItemExpendableType_Packet,     //礼包类
    eItemExpendableType_Box,        //宝箱类
    eItemExpendableType_TransForm,  //变身类
    eItemExpendableType_DynamicBox, //动态宝箱
    eItemExpendableType_PetEgg,     //宠物蛋
    eItemExpendableType_PetItem,    //宠物物品
    eItemExpendableType_EffectStone,//特效石
    eItemExpendableType_Max
};


////////////////////////////////////////////////////////////////////
// 三级类型
////////////////////////////////////////////////////////////////////

// 衣服子类型
enum EItemClothType
{
    eItemClothType_Invalid = -1,
    eItemClothType_Hair,          //发型
    eItemClothType_Face,          //表情
    eItemClothType_Body,          //上衣
    eItemClothType_Glove,         //手套
    eItemClothType_Leg,           //下装
    eItemClothType_Cap,           //头饰
    eItemClothType_FacialContent, //脸饰
    eItemClothType_Shoulder,      //肩膀
    eItemClothType_Wing,          //翅膀
    eItemClothType_LeftHand,      //左手持
    eItemClothType_RightHand,     //右手持
    eItemClothType_Wrist,         //手腕
    eItemClothType_Hip,           //臀部
    eItemClothType_Sock,          //袜子
    eItemClothType_Feet,          //鞋子 
    eItemClothType_Skin,          //肤色 
    eItemClothType_All,           //全身装，除ItemCloth_Type_Skin
    eItemClothType_Suit,          //套装
    eItemClothType_Max,

};

// 服饰属性类型
enum EClothAttrType
{
    EClothAttrType_None,
    EClothAttrType_HuaLi,       // 华丽 
    EClothAttrType_JianYue,     // 简约
    EClothAttrType_YouYa,       // 优雅
    EClothAttrType_HuoPo,       // 活泼
    EClothAttrType_ChengShu,    // 成熟
    EClothAttrType_KeAi,        // 可爱
    EClothAttrType_XingGan,     // 性感
    EClothAttrType_QingChun,    // 清纯
    EClothAttrType_BaoNuan,     // 保暖
    EClothAttrType_QingLiang,   // 清凉
};

// 徽章类型
enum EItemBadgeType
{
    eItemBadgeType_WingdingRing  =  1,  //结婚戒指
    eItemBadgeType_SpecialRing,         //个性戒指
    eItemBadgeType_VIP,                 //VIP徽章
    eItemBadgeType_Show,                //展示特效
    eItemBadgeType_ExpEffect,           //经验效果
    eItemBadgeType_CardDecoration,      //名片装饰
};

// 衣服特效七色类型
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
	EItemError_CannotUse,//设定为不能使用的物品
	EItemUse_NeedCoolDown,//cooldown时间没到
	EItemUse_NeedAttrib,//条件不够
	EItemUse_NeedSex,//性别不符
	EItemUse_NeedLevel,//级别不够
	EItemUse_EquipPos,//装备位置不正确
	EItemUse_ItemPos,//物品放在指定位置上
	EItemUse_LegWithBodyleg,//穿全身衣服的时候不能穿裤
	EItemUse_BodylegWithLeg,//穿裤的时候不能穿全身衣服
	EItemUse_ColumnFull,//物品栏满
	EItemUse_UniqueExist,//已拥有唯一的物品
	EItemUse_RequireEqup,//只能对装备栏中的装备进行操作
	EItemUse_DropPlayerColumn,//不能直接把装备在身上的物品丢弃
	EItemUse_CannotDrop,//该物品不能丢弃
	EItemUse_CreatItemError,//创建物品错误
	EItemUse_Busy,//你正在做其他操作，没办法做这个操作
	EItemUse_NeedTarget,//需要指定目标
	EItemUse_NoTargetSelf,//不能以自己为目标
	EItemUse_InvalidTarget,//非法目标
	EItemUse_LevelUpPlayerColumn,//不能直接升级装备在身上的装备
	EItemUse_LevelUpNoEquip,//只能升级装备
	EItemUse_CannotLevelUp,//该物品不能升级
	EItemUse_ColumnFullForQuest,//物品栏满,不能接任务
	EItemUse_MaxMoney,//金钱已达到最大值,不能再获得金钱。
	EItemUse_OnlyMale,//只能对男性角色使用
	EItemUse_OnlyFemale,//只能对女性角色使用
	EItemUse_BeibaoKuozhan_Max,//背包扩展已经达到上限
	EItemUse_ConsortOffline,//配偶不在线
	EItemUse_SamePos,//相同的位置
	EItemUse_ScriptError,//脚本错误。
	EItemUse_ScriptErrorReturn,//脚本返回执行错误。
	EItemUse_TitleCannotDrop,//称号不能丢。
	EItemUse_EmptyPos,//该位置上没有物品。
	EItemUse_Unknown,//未知的原因
	EItemUse_VIPRequire,//VIP物品
	EItemUse_PoorVIPLevel,//VIP等级不够
	EItemUse_ParaError,
	EItemUse_Badge_Grid_Grid_Full,//徽章格子已满
	EItemUse_Badage_Grid_More_Grid,	//现有格子满，需要开启新的徽章格子//装备物品的错误放在这里是因为装备徽章使用的是UseItem的接口，但又调用了EquipItem的接口
	EItemUse_NotMarriageUseRing,//没结婚不能使用戒指
	EItemUse_HaveEquipMarriageRing,    //已经穿戴婚戒
	EItemUse_PatchUseCountNot,      // 批量使用物品个数不够
	EItemUse_NOtCanEquip,           // 不能装备
	EItemUse_FunctionNotOpen,       // 功能未开放
	EItemUse_ConfigError,           // 配置错误
	EItemUse_ParameterErr,          // 参数错误
	EItemError_NotEnoughItem,       // 物品数量不够
	EItemUse_ClothHasEffect,        // 衣服已经存在特效
	EItemUse_EffectConfigNot,       // 特效配置不存在
	EItemUse_RemoveItemError,       // 删除物品失败
	EItemUse_MaxLevel,              // 已经是最大等级
	EItemUse_ClothEffectExist,      // 衣服特效已经存在
	EItemUse_ClothNotAddEffect,     // 衣服不能加特效
	EItemUse_ClothPosNotOpen,       // 衣服部位未开放增加特效
	EItemUse_NotEnoughMoney,        // 钱不够
    EItemUse_NotInDanceGroup,       // 舞团不存在
};


#endif //__ITEM_TYPE_H__

