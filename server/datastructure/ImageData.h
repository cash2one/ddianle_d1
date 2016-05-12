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

    void OnEquip(DBItem &equip, unsigned int nEffectID);                // ��װ��
    void UnEquip(itemid_t equipID);                                     // ��װ��
    void EffectChange(itemid_t equipID, unsigned int nEffectID);        // ��Ч�ı�
    void AddSevenColor(unsigned short nSuitID, unsigned char nColor);   // �߲ʺ�
    void UpdateGeneMap(EGeneNotifyAction eAction, const GeneData & gene);                           // �������ݸ���

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

    std::vector<DBItem> m_vecRoleItem;      // �����Ʒ
    ClotheEffectMap m_mapClothEffect;   // �·���Ч
    SevenColorMap  m_mapClothEffectSevenColor; // �·���Ч��ɫ�ʺ�
    GeneDataMap     m_mapGeneData;

    bool                    m_bHasCarriedPet;   // ����δ���ߣ��趨ΪδЯ��

    std::list<int>          m_listMedalIDs;

    // horse, all
    int m_nCurHorseTypeId;
    int m_nCurHorseOwnerId;
    int m_nHorseSeatIndex;
};


#endif // __LIBSERVERFRAME_IMAGEDATA_H__
