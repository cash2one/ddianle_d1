#ifndef __MAGICLUCKDRAW_MGR_H__
#define __MAGICLUCKDRAW_MGR_H__

#include "../../item/ItemProcess.h"
#include "../../../datastructure/LogStructDef.h"
#include <vector>

class CBetComponent;
enum EMagicArrayErrorFlag
{
    EMagicArrayErrorFlag_Scuess,
    EMagicArrayErrorFlag_MoneyNotEnough,
	EMagicArrayErrorFlag_ConfigNot,
    EMagicArrayErrorFlag_ItemNotEnough,
    EMagicArrayErrorFlag_RefreshNotEnoughMoney,
};

enum EMagicArrayCallType
{
	EMagicArrayCallType_Once,
	EMagicArrayCallType_Ten,
};

class CallReward;

class MagicArrayManager
{
public:
    MagicArrayManager(void);
    virtual ~MagicArrayManager(void);

public:
    // 初始化DB 数据
    void CreateFromDB(unsigned int nRoleID, const std::map<unsigned int,MagicArrayRecord> & mapRecord, 
        const MagicArrayExchangeRecord & exchangeRecord);

    bool PacketToCache(std::map<unsigned int, MagicArrayRecord> & mapRecord, MagicArrayExchangeRecord & exchangeRecord) const;

    // 单抽
    int OnceMagicCall(CBetComponent & betComponent, unsigned int nMagicArrayID, CItem & fixedItem, std::list<CallReward> & listRewardItem);

    // 十连抽
    int TenMagicCall(CBetComponent & betComponent, unsigned int nMagicArrayID, CItem & fixedItem, std::list<CallReward> & listRewardItem);

    // 获取魔法阵信息
    void SyncMagicArrayInfo(CBetComponent & betComponent);

    // 兑换
    int ExchangeItem(CBetComponent & betComponent, unsigned int nExchangeID);

    // 同步兑换物品信息
    void SyncExchangeItemInfo(CBetComponent & betComponent);

    // 刷新兑换奖励
    int RefreshExchangeItem(CBetComponent & betComponent);

    // 免费召唤剩余时间
    unsigned int GetCallCDLeftTime(unsigned int nMagicArrayID);

    // 检查是否免费召唤
    void CheckCanFreeCall(std::list<unsigned int> & listMagicArrayID);

private:
    // 保存到DB
    void SyncMagicArrayRecordToDB(unsigned int nRoleID, unsigned int nMagicArrayID);
    void SynceMagicArrayExchangeToDB(unsigned int nRoleID, ESyncToDBType eType);

    // 是否是/更新免费单抽
    bool IsFreeCall(unsigned int nMagicArrayID);
    void UpdateFreeCallTime(unsigned int nMagicArrayID);

    // 是否是/更新首次付费单抽
    bool IsFirstOnceCall(unsigned int nMagicArrayID);
    void UpdateIsFirstCall(unsigned int nMagicArrayID);

    // 获取/更新抽取累计次数
    unsigned int GetCallAccumulatedCount(unsigned int nMagicArrayID);
    void UpdateCallAccumulatedCount(unsigned int nMagicArrayID, int nCount);

    // 第一次保存
    bool IsFirstSave(unsigned int nMagicArrayID);

    // log日志
    void InsertIntoLog(unsigned int nRoleID, CMagicArrayLog::EMagicArrayAction eAction, unsigned int nMagicArrayID);
private:
	std::map<unsigned int,MagicArrayRecord>		m_mapMagciArrayRecord;
    MagicArrayExchangeRecord                    m_MagicArrayExchange;
    std::set<unsigned int>                      m_setIsFirstSave;
};


#endif//__LUCK_EXCHANGE_MGR_H__


