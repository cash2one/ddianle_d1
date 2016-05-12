#ifndef __MALLMSGDEFINE_H__
#define __MALLMSGDEFINE_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/datainfor/ItemType.h"
#include "../../datastructure/LogicInitData.h"
#include "../../datastructure/DataStruct_Base.h"
#include "../../datastructure/DataStruct_DB.h"
#include "../../datastructure/datainfor/ItemConfigManager.h"
#include "ClothEffect.h"
#include <map>

class ItemBase;
class ItemEquip;

class GameMsg_S2C_RefreshItem :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;

public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_RefreshItem);
	GameMsg_S2C_RefreshItem(void);
	~GameMsg_S2C_RefreshItem(void);

public:
    virtual bool doEncode( CParamPool& IOBuff );

public:
	itemtype_t                         m_nItemType;
	unsigned short                     m_nColumn;
	std::map<unsigned short,ItemBase*> m_mapItems;
};

class GameMsg_S2C_BatchRefreshItem :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_BatchRefreshItem);
    GameMsg_S2C_BatchRefreshItem(void);
    ~GameMsg_S2C_BatchRefreshItem(void);

public:
    virtual bool doEncode( CParamPool& IOBuff );

public:
    std::map<itemgrid_t, ItemBase*>    m_mapChangeItem;
};

class GameMsg_S2C_UseItemFailed :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
//	GAMEMSG_CREATEDECLARE(GameMsg_S2C_UseItemFailed);
	GameMsg_S2C_UseItemFailed();
	~GameMsg_S2C_UseItemFailed(void);
	unsigned char m_nErrorCode;
public:
	virtual bool doEncode( CParamPool& IOBuff );
};

class GameMsg_C2S_UseItem :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_UseItem);
	GameMsg_C2S_UseItem(void);
	~GameMsg_C2S_UseItem(void);

	unsigned short nColumn;
	unsigned short nIndex;

	unsigned int nDestRoleId;

public:
	virtual bool doDecode( CParamPool& IOBuff );
};

///////////////////////////////////////////////////////
class GameMsg_C2S_BatchUseItem :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_BatchUseItem);
	GameMsg_C2S_BatchUseItem(void);
	~GameMsg_C2S_BatchUseItem(void);

public:
	virtual bool doDecode( CParamPool& IOBuff );

public:
	unsigned short m_nColumn;
	unsigned short m_nIndex;
	itemtype_t m_nItemID;
	int m_nCount;
};
///////////////////////////////////////////////////////

class GameMsg_S2C_PatchUseItemEffect :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_PatchUseItemEffect);
	GameMsg_S2C_PatchUseItemEffect(void);
	~GameMsg_S2C_PatchUseItemEffect(void);

public:
	virtual bool doEncode( CParamPool& IOBuff );

public:
	itemtype_t m_nItemID;
	int m_nCount;
	std::string m_strDesc;
};

///////////////////////////////////////////////////////

class GameMsg_C2S_ChangeEquipItem :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ChangeEquipItem);
	GameMsg_C2S_ChangeEquipItem(void);
	~GameMsg_C2S_ChangeEquipItem(void);
	
	short nIndexs[eItemClothType_Max][2];
	//short nIndexsBadge[BADGE_BAG_MAXNUM][2];
public:
	virtual bool doDecode( CParamPool& IOBuff );
};


class GameMsg_S2C_ChangeEquipItemFail :
	public GameMsg_Base
{
public:
	GameMsg_S2C_ChangeEquipItemFail();
	~GameMsg_S2C_ChangeEquipItemFail();
public:
	virtual bool doEncode( CParamPool& IOBuff );
public:
	unsigned short m_nColumn;
	unsigned short m_nIndex;
	unsigned char m_nError;
};

class GameMsg_S2C_ChangeEquipItem : public GameMsg_Base
{
    typedef  GameMsg_Base _baseClass;

public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_ChangeEquipItem);
    GameMsg_S2C_ChangeEquipItem(void);
    ~GameMsg_S2C_ChangeEquipItem(void);

public:
    virtual bool doEncode( CParamPool& IOBuff );

public:
    unsigned int           m_nRoleID;
    CEntityItemComponent * m_pItemComponent;
    std::map<int,int>      m_mapOldEquipSlot;
    std::map<int,int>      m_mapNewEquipSlot;
};

class GameMsg_C2S_RemoveItem :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_RemoveItem);
	GameMsg_C2S_RemoveItem(void);
	~GameMsg_C2S_RemoveItem(void);

	unsigned short nColumn;
	unsigned short nIndex;

public:
	virtual bool doDecode( CParamPool& IOBuff );
};



class GameMsg_C2S_ItemColumn :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ItemColumn);
	GameMsg_C2S_ItemColumn(void);
	~GameMsg_C2S_ItemColumn(void);

	unsigned short nColumn;
	unsigned short nAddIndex;

public:
	virtual bool doDecode( CParamPool& IOBuff );
};


class GameMsg_S2C_ItemColumn :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_ItemColumn);
	GameMsg_S2C_ItemColumn(void);
	~GameMsg_S2C_ItemColumn(void);

	unsigned char  nResult;
	unsigned short nColumn;
	unsigned short nTotalGridNum;
	string strError;

public:
	virtual bool doEncode( CParamPool& IOBuff );
};


class GameMsg_S2C_RefreshMoney :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_RefreshMoney);
	GameMsg_S2C_RefreshMoney(void);
	~GameMsg_S2C_RefreshMoney(void);

	int  nMoney;

public:
	virtual bool doEncode( CParamPool& IOBuff );
};


class GameMsg_S2C_RefreshPt :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_RefreshPt);
	GameMsg_S2C_RefreshPt(void);
	~GameMsg_S2C_RefreshPt(void);

public:
    virtual bool doEncode( CParamPool& IOBuff );

public:
	unsigned int m_nPoint;
    unsigned int m_nTotalPoint;
};

class GameMsg_S2C_RefreshBindPt :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GameMsg_S2C_RefreshBindPt(void);
	~GameMsg_S2C_RefreshBindPt(void);

	unsigned int nBindPoint;

public:
	virtual bool doEncode( CParamPool& IOBuff );
};

class GameMsg_S2C_RefreshExp :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_RefreshExp);
	GameMsg_S2C_RefreshExp(void);
	~GameMsg_S2C_RefreshExp(void);

	unsigned int  nExp;

public:
	virtual bool doEncode( CParamPool& IOBuff );
};


class GameMsg_S2C_RefreshLevel :
    public GameMsg_Base
{
    typedef  GameMsg_Base _baseClass;

public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_RefreshLevel);

    GameMsg_S2C_RefreshLevel(void);
    ~GameMsg_S2C_RefreshLevel(void);

    virtual bool doEncode( CParamPool& IOBuff );

public:
    unsigned short m_nLevel;
    unsigned int m_nExp;
    int m_nFunctionID;
};


class GameMsg_S2C_RefreshChange :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_RefreshChange);
	GameMsg_S2C_RefreshChange(void);
	~GameMsg_S2C_RefreshChange(void);

	unsigned int  nChange;

public:
	virtual bool doEncode( CParamPool& IOBuff );
};

class GameMsg_S2C_RefreshHot :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GameMsg_S2C_RefreshHot(void);
	~GameMsg_S2C_RefreshHot(void);

	unsigned int  nHot;

public:
	virtual bool doEncode( CParamPool& IOBuff );
};

class GameMsg_S2C_RefreshTransform :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GameMsg_S2C_RefreshTransform(void);
	~GameMsg_S2C_RefreshTransform(void);

	int  nTransformId;

public:
	virtual bool doEncode( CParamPool& IOBuff );
};

class GameMsg_S2C_SimulateRandomItem :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GameMsg_S2C_SimulateRandomItem(void);
	~GameMsg_S2C_SimulateRandomItem(void);

	std::map<CItem, unsigned int> allItem;

public:
	virtual bool doEncode( CParamPool& IOBuff );
};

/////////////////////////////////////////////////
class GameMsg_S2C_OpenBoxResult :
	public GameMsg_Base
{
public:
	GameMsg_S2C_OpenBoxResult();
	~GameMsg_S2C_OpenBoxResult();

public:
	unsigned int	m_nCount;
	itemtype_t      m_nItemType;
	std::list<CItem> m_listItems;

public:
	virtual bool doEncode( CParamPool& IOBuff );
};

/////////////////////////////////////////////////
class GameMsg_S2C_OpenPackageResult :
	public GameMsg_Base
{
public:
	GameMsg_S2C_OpenPackageResult();
	~GameMsg_S2C_OpenPackageResult();

public:
	unsigned int m_nCount;
	itemtype_t m_nItemTypeID;
	std::list<CItem> m_listItems;
    std::map<int, int> m_mapCurrency;

public:
	virtual bool doEncode( CParamPool& IOBuff );
};

/////////////////////////////////////////////////
class GameMsg_S2C_SynExtendItemBinData :
	public GameMsg_Base
{
public:
	GameMsg_S2C_SynExtendItemBinData();
	~GameMsg_S2C_SynExtendItemBinData();
public:
	virtual bool doEncode( CParamPool& IOBuff );
public:
	unsigned short m_nBinDataSize;
	char m_szExtendItemBinData[C_EXTEND_ITEM_DATA_SIZE];
};

/////////////////////////////////////////////////
class GameMsg_S2C_GetClothGroups :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetClothGroups);
	GameMsg_S2C_GetClothGroups();
	~GameMsg_S2C_GetClothGroups();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	unsigned char m_nCurGroupID;//当前搭配
	std::map<unsigned char, RoleClothGroup> m_mapClothGroup;
};

class ClothGroupItem
{
public:
	ClothGroupItem(){};
	virtual ~ClothGroupItem(){};
	bool operator==(const ClothGroupItem & item);

public:
	bool doEncode(CParamPool &IOBuff) const;
	bool doDecode(CParamPool &IOBuff);

public:
	unsigned char m_nType;//部位枚举
    itemtype_t m_nItemType;
};

/////////////////////////////////////////////////
class GameMsg_C2S_SelectClothGroup :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_SelectClothGroup);
	GameMsg_C2S_SelectClothGroup();
	~GameMsg_C2S_SelectClothGroup();
public:
	virtual bool doDecode(CParamPool &IOBuff);
public:
	unsigned char m_nGroupID;//搭配id
};

/////////////////////////////////////////////////
class GameMsg_S2C_SelectClothGroup :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_SelectClothGroup);
	GameMsg_S2C_SelectClothGroup();
	~GameMsg_S2C_SelectClothGroup();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	unsigned char m_nGroupID;//搭配id
	stringw m_strError;//总错误码，空字符串表示成功
	std::map<itemtype_t, stringw> m_mapItemError;//有问题的道具id及其对应错误码
};

/////////////////////////////////////////////////
class GameMsg_C2S_UpdateClothGroup :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_UpdateClothGroup);
	GameMsg_C2S_UpdateClothGroup();
	~GameMsg_C2S_UpdateClothGroup();
public:
	virtual bool doDecode(CParamPool &IOBuff);
public:
	unsigned char m_nGroupID;//搭配id
	std::list<ClothGroupItem> m_listItems;//装备的部位
};

/////////////////////////////////////////////////
class GameMsg_S2C_UpdateClothGroup :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_UpdateClothGroup);
	GameMsg_S2C_UpdateClothGroup();
	~GameMsg_S2C_UpdateClothGroup();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	unsigned char m_nGroupID;//搭配id
	stringw m_strError;//总错误码，空字符串表示成功
	std::map<itemtype_t, stringw> m_mapItemError;//有问题的道具id及其对应错误码
	std::list<ClothGroupItem> m_listItems;//装备的部位
};
/////////////////////////////////////////////////
class GameMsg_C2S_UpdateClothGroupName :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_UpdateClothGroupName);
	GameMsg_C2S_UpdateClothGroupName();
	~GameMsg_C2S_UpdateClothGroupName();
public:
	virtual bool doDecode(CParamPool &IOBuff);
public:
	unsigned char m_nGroupID;//搭配id
	stringw m_strGroupName;//搭配新名字
};

/////////////////////////////////////////////////
class GameMsg_S2C_UpdateClothGroupName :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_UpdateClothGroupName);
	GameMsg_S2C_UpdateClothGroupName();
	~GameMsg_S2C_UpdateClothGroupName();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	unsigned char m_nGroupID;//搭配id
	stringw m_strError;//错误码，空字符串表示成功
	stringw m_strGroupName;//搭配新名字
};

/////////////////////////////////////////////////
class GameMsg_C2S_GetCoolDown :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetCoolDown);
	GameMsg_C2S_GetCoolDown();
	~GameMsg_C2S_GetCoolDown();
public:
	virtual bool doDecode(CParamPool &IOBuff);
};

/////////////////////////////////////////////////
class GameMsg_S2C_GetCoolDown :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetCoolDown);
	GameMsg_S2C_GetCoolDown();
	~GameMsg_S2C_GetCoolDown();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	std::list< CoolDownData > m_listItemCoolDown;
};

/////////////////////////////////////////////////
class GameMsg_S2C_UpdateCoolDown :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_UpdateCoolDown);
	GameMsg_S2C_UpdateCoolDown();
	~GameMsg_S2C_UpdateCoolDown();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	CoolDownData m_ItemCoolDown;
};

/////////////////////////////////////////////////
class GameMsg_C2S_ReplaceClothEffect :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_ReplaceClothEffect);
    GameMsg_C2S_ReplaceClothEffect();
    ~GameMsg_C2S_ReplaceClothEffect();

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned short      m_nClothColumn;
    unsigned short      m_nClothSlot;
    unsigned int        m_nEffectID;
};


/////////////////////////////////////////////////
class GameMsg_S2C_ReplaceClothEffectFail :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_ReplaceClothEffectFail);
    GameMsg_S2C_ReplaceClothEffectFail();
    ~GameMsg_S2C_ReplaceClothEffectFail();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    int      m_nError;
};

/////////////////////////////////////////////////
class GameMsg_S2C_ReplaceClothEffectResult :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_ReplaceClothEffectResult);
    GameMsg_S2C_ReplaceClothEffectResult();
    ~GameMsg_S2C_ReplaceClothEffectResult();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
};

/////////////////////////////////////////////////
class GameMsg_C2S_LevelUpEffect :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_LevelUpEffect);
    GameMsg_C2S_LevelUpEffect();
    ~GameMsg_C2S_LevelUpEffect();

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned short     m_nItemColumn;
    unsigned short     m_nItemSlot;
};

/////////////////////////////////////////////////
class GameMsg_S2C_LevelUpEffectFail :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_LevelUpEffectFail);
    GameMsg_S2C_LevelUpEffectFail();
    ~GameMsg_S2C_LevelUpEffectFail();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    int      m_nError;
};

/////////////////////////////////////////////////
class GameMsg_S2C_LevelUpEffectResult :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_LevelUpEffectResult);
    GameMsg_S2C_LevelUpEffectResult();
    ~GameMsg_S2C_LevelUpEffectResult();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned int          m_nLevelUpEffectID;
    itemtype_t            m_nItemtype;
};

/////////////////////////////////////////////////
class GameMsg_S2C_RefreshHandbookProgress :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_RefreshHandbookProgress);
    GameMsg_S2C_RefreshHandbookProgress();
    ~GameMsg_S2C_RefreshHandbookProgress();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned short                m_nSuitID;
    HandbookProgress *            m_pHandbookProgress;
};

/////////////////////////////////////////////////
class GameMsg_S2C_RefreshColorProgress :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_RefreshColorProgress);
    GameMsg_S2C_RefreshColorProgress();
    ~GameMsg_S2C_RefreshColorProgress();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned short               m_nSuitID;
    SevenColorProgress *         m_pSevenColorProgress;
};


/////////////////////////////////////////////////
struct MoveItemAction
{
    int nFromColumn;
    int nFromSlot;
    int nToColumn;
    int nToSlot;

    void doDecode(CParamPool & IOBuff)
    {
        nFromColumn = IOBuff.GetInt();
        nFromSlot = IOBuff.GetInt();
        nToColumn = IOBuff.GetInt();
        nToSlot = IOBuff.GetInt();
    }
};

class GameMsg_C2S_MoveItem:
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_MoveItem);
    GameMsg_C2S_MoveItem();
    ~GameMsg_C2S_MoveItem();

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    std::list<itemgrid_t>       m_listFromGrid;
};

/////////////////////////////////////////////////
class GameMsg_S2C_MoveItemFail :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_MoveItemFail);
    GameMsg_S2C_MoveItemFail();
    ~GameMsg_S2C_MoveItemFail();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    int         m_nErrorFlag;
    int         m_nLeftCapacity;
};

/////////////////////////////////////////////////
class GameMsg_S2C_MoveItemScuess :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_MoveItemScuess);
    GameMsg_S2C_MoveItemScuess();
    ~GameMsg_S2C_MoveItemScuess();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    std::list<itemgrid_t>   m_listGrid;
};

/////////////////////////////////////////////////
class GameMsg_C2S_AddStorageCapacity :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_AddStorageCapacity);
    GameMsg_C2S_AddStorageCapacity();
    ~GameMsg_C2S_AddStorageCapacity();

public:
    virtual bool doDecode(CParamPool &IOBuff);

};

/////////////////////////////////////////////////
class GameMsg_S2C_AddStorageCapacityFail :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_AddStorageCapacityFail);
    GameMsg_S2C_AddStorageCapacityFail();
    ~GameMsg_S2C_AddStorageCapacityFail();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    int         m_nErrorFlag;
};

/////////////////////////////////////////////////
class GameMsg_S2C_AddStorageCapacityScuess :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_AddStorageCapacityScuess);
    GameMsg_S2C_AddStorageCapacityScuess();
    ~GameMsg_S2C_AddStorageCapacityScuess();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned int       m_nStorageCapacity;
};

/////////////////////////////////////////////////
class GameMsg_S2C_SyncBagConfig :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_SyncBagConfig);
    GameMsg_S2C_SyncBagConfig();
    ~GameMsg_S2C_SyncBagConfig();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    int       m_nBadgeCapacity;
    int       m_nStorageCapacity;
    std::string m_strBagDescribe;
    int       m_nAddStorageCapacityCurrencyType;
    int       m_nAddStorageCapacityCurrencyValue;
};

/////////////////////////////////////////////////
class GameMsg_S2C_NoticeStorageItemUsed :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_NoticeStorageItemUsed);
    GameMsg_S2C_NoticeStorageItemUsed();
    ~GameMsg_S2C_NoticeStorageItemUsed();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    std::list<CItem>    m_listUsedItem;
};

/////////////////////////////////////////////////
class GameMsg_S2C_NoticeItemInStorage :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_NoticeItemInStorage);
    GameMsg_S2C_NoticeItemInStorage();
    ~GameMsg_S2C_NoticeItemInStorage();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    itemtype_t      m_nItemType;
};

/////////////////////////////////////////////////
class GameMsg_C2S_RequestGuideFinish :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_RequestGuideFinish);
    GameMsg_C2S_RequestGuideFinish();
    ~GameMsg_C2S_RequestGuideFinish();

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned int           m_nGuideEnum;
};

/////////////////////////////////////////////////
class GameMsg_S2C_RequestGuideFinishResult :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_RequestGuideFinishResult);
    GameMsg_S2C_RequestGuideFinishResult();
    ~GameMsg_S2C_RequestGuideFinishResult();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned int      m_nGuideData;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_RefreshPerpetualEquipCount : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE( GameMsg_S2C_RefreshPerpetualEquipCount );

public:
    virtual bool doEncode( CParamPool &IOBuff );

public:
    unsigned int    m_nCount;
};

class GameMsg_S2C_RefreshPerpetualEffectScore : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE( GameMsg_S2C_RefreshPerpetualEffectScore );

public:
    virtual bool doEncode( CParamPool &IOBuff );

public:
    unsigned int    m_nScore;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_SerializeItemInfo : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE( GameMsg_S2C_SerializeItemInfo );

public:
    virtual bool doEncode( CParamPool &IOBuff );

public:
    bool            m_bEnd;
    unsigned short  m_nBuffSize;
    const char*     m_pBuff;
};

#endif



