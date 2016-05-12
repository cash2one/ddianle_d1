#ifndef __LIBSERVERFRAME_IMAGEDATA_H__
#define __LIBSERVERFRAME_IMAGEDATA_H__


#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>

#include "DataStruct_Base.h"
#include "DataStruct_DB.h"
// #include "../server/roomLib/GameMsgNewRoom.h"

class CParmaPool;

class MedalDisplayInfo
{
public:
    MedalDisplayInfo();
    MedalDisplayInfo(int nType, const std::string &rIcon);
    ~MedalDisplayInfo();

public:
    bool doEncode(CParamPool &IOBuff) const;
    bool doDecode(CParamPool &IOBuff);

public:
    int         m_nType;
    std::string m_strIcon;
};


unsigned short ParamHoldUShortPos(CParamPool &IOBuff);
void ParamReleaseUShortPos(CParamPool &IOBuff, unsigned short nOldPos);
unsigned short ParamHoldUCharPos(CParamPool &IOBuff);
void ParamReleaseUCharPos(CParamPool &IOBuff, unsigned short nOldPos);

//typedef ParamHold<unsigned short>()

class RoleRoomImageData
{
public:
    RoleRoomImageData();
    ~RoleRoomImageData();
    typedef std::map<itemid_t, unsigned int> ClotheEffectMap;
    typedef std::map<unsigned short, std::set<ESevenColorType> > SevenColorMap;
    typedef std::map<unsigned short, GeneData> GeneDataMap;

    string EncodeToString();

    void CopyFrom(const RoleRoomImageData &src);

    void OnEquip(DBItem &equip, unsigned int nEffectID);                // 穿装备
    void UnEquip(itemid_t equipID);                                     // 脱装备
    void EffectChange(itemid_t equipID, unsigned int nEffectID);        // 光效改变
    void AddSevenColor(unsigned short nSuitID, unsigned char nColor);   // 七彩虹
    void UpdateGeneMap(EGeneNotifyAction eAction, const GeneData & gene);                           // 基因数据更新

    void AddMedals(int nMedalID);
    void SyncHorseData(int nHorseTypeID, int nHorseOwnerID, int nSeatIndex);

public:
    bool GetDbItem(itemid_t itemID, DBItem &dbItem);
    
private:
    bool RoomEncodeEquipString(CParamPool &IOBuff);
    bool RoomEncodeEffectString(CParamPool &IOBuff);
    bool RoomEncodePetString(CParamPool &IOBuff);
    bool RoomEncodeGeneString(CParamPool &IOBuff);
    bool RoomEncodeMedalString(CParamPool &IOBuff);
    bool RoomEncodeHorseString(CParamPool &IOBuff);


public:

    std::vector<DBItem> m_vecRoleItem;      // 玩家物品
    ClotheEffectMap m_mapClothEffect;   // 衣服特效
    SevenColorMap  m_mapClothEffectSevenColor; // 衣服特效七色彩虹
    GeneDataMap     m_mapGeneData;

    bool                    m_bHasCarriedPet;   // 宠物未上线，设定为未携带

    std::list<int>          m_listMedalIDs;

    // horse, all
    int m_nCurHorseTypeId;
    int m_nCurHorseOwnerId;
    int m_nHorseSeatIndex;
};


#endif // __LIBSERVERFRAME_IMAGEDATA_H__
