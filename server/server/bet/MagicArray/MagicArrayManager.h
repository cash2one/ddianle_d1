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
    // ��ʼ��DB ����
    void CreateFromDB(unsigned int nRoleID, const std::map<unsigned int,MagicArrayRecord> & mapRecord, 
        const MagicArrayExchangeRecord & exchangeRecord);

    bool PacketToCache(std::map<unsigned int, MagicArrayRecord> & mapRecord, MagicArrayExchangeRecord & exchangeRecord) const;

    // ����
    int OnceMagicCall(CBetComponent & betComponent, unsigned int nMagicArrayID, CItem & fixedItem, std::list<CallReward> & listRewardItem);

    // ʮ����
    int TenMagicCall(CBetComponent & betComponent, unsigned int nMagicArrayID, CItem & fixedItem, std::list<CallReward> & listRewardItem);

    // ��ȡħ������Ϣ
    void SyncMagicArrayInfo(CBetComponent & betComponent);

    // �һ�
    int ExchangeItem(CBetComponent & betComponent, unsigned int nExchangeID);

    // ͬ���һ���Ʒ��Ϣ
    void SyncExchangeItemInfo(CBetComponent & betComponent);

    // ˢ�¶һ�����
    int RefreshExchangeItem(CBetComponent & betComponent);

    // ����ٻ�ʣ��ʱ��
    unsigned int GetCallCDLeftTime(unsigned int nMagicArrayID);

    // ����Ƿ�����ٻ�
    void CheckCanFreeCall(std::list<unsigned int> & listMagicArrayID);

private:
    // ���浽DB
    void SyncMagicArrayRecordToDB(unsigned int nRoleID, unsigned int nMagicArrayID);
    void SynceMagicArrayExchangeToDB(unsigned int nRoleID, ESyncToDBType eType);

    // �Ƿ���/������ѵ���
    bool IsFreeCall(unsigned int nMagicArrayID);
    void UpdateFreeCallTime(unsigned int nMagicArrayID);

    // �Ƿ���/�����״θ��ѵ���
    bool IsFirstOnceCall(unsigned int nMagicArrayID);
    void UpdateIsFirstCall(unsigned int nMagicArrayID);

    // ��ȡ/���³�ȡ�ۼƴ���
    unsigned int GetCallAccumulatedCount(unsigned int nMagicArrayID);
    void UpdateCallAccumulatedCount(unsigned int nMagicArrayID, int nCount);

    // ��һ�α���
    bool IsFirstSave(unsigned int nMagicArrayID);

    // log��־
    void InsertIntoLog(unsigned int nRoleID, CMagicArrayLog::EMagicArrayAction eAction, unsigned int nMagicArrayID);
private:
	std::map<unsigned int,MagicArrayRecord>		m_mapMagciArrayRecord;
    MagicArrayExchangeRecord                    m_MagicArrayExchange;
    std::set<unsigned int>                      m_setIsFirstSave;
};


#endif//__LUCK_EXCHANGE_MGR_H__


