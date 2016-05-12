#ifndef __CLOTHEFFECT_H__
#define __CLOTHEFFECT_H__

#include "../../datastructure/datainfor/ItemType.h"
#include "../../datastructure/DataStruct_Base.h"
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"

#include <map>
#include <set>

class HandbookProgress
{
public:
    HandbookProgress(unsigned int nRoleID, unsigned short nSuitID);
    virtual ~HandbookProgress();

public:
    // 初始化数据
    void Initialize(const std::map<unsigned char,std::string> & handbookProgressData);
    void SaveToMap( std::map<unsigned char, std::string> &rHandbookProgress ) const;

    // 计算进度
    unsigned int CalcProgress();

    // 更新进度
    void UpdateProgress(unsigned int nEffectID);

    // 同步数据
    void doEncode(CParamPool &IOBuff);

    // 是否收集完成
    bool IsCollectFinish();

private:
    // 获取最大等级的特效ID
    unsigned int GetMaxLevelEffectID(const std::set<unsigned int> & setEffectID);

    // 同步到DB
    void SyncToDB(EItemClothType eClothType, ESyncToDBType eSyncType);

    // 发送消息
    void SendPlayerMsg(GameMsg_Base * pMsg);

    // log
    void LogHandbookProgress(EItemClothType eItemType, std::set<unsigned int> & setAddEffectID);

public:
    unsigned int                                            m_nRoleID;              // 角色ID
    unsigned short                                          m_nSuitID;              // 套装ID
    unsigned int                                            m_nProgress;            // 进度
    std::map<EItemClothType, std::set<unsigned int> >       m_mapClothPosEffect;    // 衣服部位对应特效ID
};

class SevenColorProgress
{
public:
    SevenColorProgress(unsigned int nRoleID, unsigned short nSuitID);
    virtual ~SevenColorProgress();

public:
    // 初始化数据
    void Initialize( const std::map<unsigned char, std::string> & colorProgressData );
    void SaveToMap( std::map<unsigned char, std::string> &rColorProgress ) const;

    // 更新进度
    void UpdateProgress(unsigned int nEffectID, itemtype_t nRelatedItemType);

    // 获取进度
    int GetColorUpgress(ESevenColorType eColorType) const;

    // 同步数据
    void doEncode(CParamPool &IOBuff);
    void doEncodeToBuff( std::string &rBuff ) const;

    // 同步信息给其他人
    void PackItemForOther(CParamPool &IOBuff);
    void PackItemForOther(std::map<unsigned short,std::set<ESevenColorType> > & mapSevenColorProgress);

private:
    // 初始化套装的衣服部位
    void InitializeClothPos();

    // 同步到DB
    void SyncToDB(ESevenColorType eColorType, ESyncToDBType eSyncType);

    // 计算进度百分比
    int CalcProgressRate(const std::set<EItemClothType> & setClothPos);

    // 发送消息
    void SendPlayerMsg(GameMsg_Base * pMsg);
    void SendPlayerMsgToOthers(GameMsg_Base * pMsg);
    void SendMsg2GroupServer(GameMsg_Base * pMsg);

    // log日志
    void LogColorProgress(ESevenColorType eColorType, unsigned short nClothPos);

public:
    unsigned int                                            m_nRoleID;              // 角色ID
    unsigned short                                          m_nSuitID;              // 套装ID
    std::set<EItemClothType>                                m_setClothPos;          // 套装衣服部位
    unsigned int                                            m_nBodyLegCount;        // 上衣、下衣配置数目
    unsigned int                                            m_nSuitCount;           // 套装配置数目
    std::map<ESevenColorType,std::set<EItemClothType> >     m_mapColorClothPos;     // 七色完成部位
    std::set<ESevenColorType>                               m_setColorProgress;     // 七色完成进度
};

class ClothEffectHandbook
{
public:
    ClothEffectHandbook();
    virtual ~ClothEffectHandbook();

public:
    // 初始化数据
    void Initialize(unsigned int nRoleID, const std::map<unsigned short,std::map<unsigned char,std::string> > & handbookProgressData,
        const std::map<unsigned short, std::map<unsigned char, std::string> > & colorProgressData );
    void PacketToCache( std::map<unsigned short, std::map<unsigned char, std::string> > &rHandbookProgress,
        std::map<unsigned short, std::map<unsigned char, std::string> > &rColorProgress ) const;

    // 销毁数据
    void Destory();

    // 同步数据到客户端
    void SerializeComponent(CParamPool &IOBuff);
    void SerializeToBuff( std::string &rBuff ) const;

    // 更新进度
    void UpdateProgress(unsigned int nEffectID, itemtype_t nRelatedItemType);
    void UpdateHandbookProgress(unsigned short nSuitID, unsigned int nEffectID);
    void UpdateColorProgress(unsigned short nSuitID, unsigned short nClothPos, unsigned int nEffectID, itemtype_t nRelatedItemType);

    // 套装是否收集完成
    bool IsSuitCollectFinish(unsigned short nSuitID);

    // 获取套装收集完成个数
    unsigned int GetFinishCount();

    // 同步信息给其他人
    void PackItemForOther(CParamPool & IOBuff);
    void PackItemForOther(std::map<unsigned short,std::set<ESevenColorType> > & mapSevenColorProgress);

public:
    unsigned int                                               m_nRoleID;                   // 角色ID
    std::map<unsigned short,HandbookProgress*>                 m_mapHandbookProgress;       // 套装收集进度
    std::map<unsigned short,SevenColorProgress*>               m_mapSevenColorProgress;     // 七色花瓣收集进度
};

class ClothEffect
{
public:
    ClothEffect();
    virtual ~ClothEffect();

public:
    // 初始化数据
    void CreateFromDB(unsigned int nRoleID, const std::set<unsigned int> & setRoleEffect, const std::map<unsigned short,std::map<unsigned char,std::string> > & handbookProgressData,
        const std::map<unsigned short, std::map<unsigned char, std::string> > & colorProgressData );
    void PacketToCache( std::set<unsigned int> &rRoleEffect, std::map<unsigned short, std::map<unsigned char, std::string> > &rHandbookProgressData,
        std::map<unsigned short, std::map<unsigned char, std::string> > &rColorProgressData ) const;

    // 同步数据到客户端
    void SerializeToBuff( std::string &rBuff ) const;

    // 销毁数据
    void Destory();

    // 特效变动(镶嵌到衣服,升级,物品添加)
    void EffectChange(unsigned int nEffectID, itemtype_t nRelatedItemType);

    // 是否在角色特效列表中
    bool IsInRoleEffect(unsigned int nEffectID);

    // 添加特效到角色列表中
    void AddRoleEffect(unsigned int nEffectID);

    // 套装是否收集完成
    bool IsSuitCollectFinish(unsigned short nSuitID);
    
    // 同步信息给其他人
    void PackItemForOther(CParamPool & IOBuff);
    void PackItemForOther(std::map<unsigned short,std::set<ESevenColorType> > & mapSevenColorProgress);

    // 检查是否有列表中特效ID
    void CheckEffectListExist(const std::list<unsigned int> & listCheck, std::list<unsigned int> & listReturn);

    // 获取图鉴完成个数
    unsigned int GetHandbookFinishCount();

public:
    unsigned int                    m_nRoleID;                  // 角色ID
    std::set<unsigned int>          m_setRoleClothEffect;       // 角色拥有的特效
    ClothEffectHandbook             m_ClothEffectHandbook;      // 特效图鉴
};


#endif
