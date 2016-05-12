
#include "ImageData.h"
#include "datainfor/ItemConfigManager.h"
#include "../server/Medal/MedalCfgMgr.h"
#include "datainfor/ConfigManager.h"
#include "../socket/Log.h"


MedalDisplayInfo::MedalDisplayInfo()
    : m_nType(0)
    , m_strIcon("")
{
}

MedalDisplayInfo::MedalDisplayInfo(int nType, const std::string &rIcon)
    : m_nType(nType)
    , m_strIcon(rIcon)
{
}

MedalDisplayInfo::~MedalDisplayInfo()
{
}

bool MedalDisplayInfo::doEncode(CParamPool &IOBuff) const
{
    IOBuff << m_nType << stringw(m_strIcon);

    return true;
}

bool MedalDisplayInfo::doDecode(CParamPool &IOBuff)
{
    m_nType = IOBuff.GetInt();
    IOBuff.GetStringW(m_strIcon);

    return true;
}


unsigned short ParamHoldUShortPos(CParamPool &IOBuff)
{
    int nOldPos = IOBuff.GetCurPosition();
    IOBuff.AddUShort(0);

    return (unsigned short)nOldPos;
}
void ParamReleaseUShortPos(CParamPool &IOBuff, unsigned short nOldPos)
{
    unsigned int nFinalPos = IOBuff.GetCurPosition();
    IOBuff.SetCurPosition(nOldPos);
    IOBuff.ReplaceShort((short)(nFinalPos - nOldPos - sizeof(unsigned short)));
    IOBuff.SetCurPosition(nFinalPos);

    return;
}
unsigned short ParamHoldUCharPos(CParamPool &IOBuff)
{
    int nOldPos = IOBuff.GetCurPosition();
    IOBuff.AddUChar(0);

    return (unsigned short)nOldPos;
}
void ParamReleaseUCharPos(CParamPool &IOBuff, unsigned short nOldPos)
{
    unsigned int nFinalPos = IOBuff.GetCurPosition();
    IOBuff.SetCurPosition(nOldPos);
    IOBuff.ReplaceUChar((unsigned char)(nFinalPos - nOldPos - sizeof(unsigned char)));
    IOBuff.SetCurPosition(nFinalPos);

    return;
}


//////////////////////////////////////////////////////////////////////////
// RoleRoomImageData
RoleRoomImageData::RoleRoomImageData()
    : m_bHasCarriedPet(false) // 默认未携带（未上线）
    , m_nCurHorseTypeId(0)
    , m_nCurHorseOwnerId(0)
    , m_nHorseSeatIndex(0)
{
}

RoleRoomImageData::~RoleRoomImageData()
{

}

void RoleRoomImageData::CopyFrom(const RoleRoomImageData &src)
{
    this->m_vecRoleItem.assign(src.m_vecRoleItem.begin(), src.m_vecRoleItem.end());
    this->m_mapClothEffect.insert(src.m_mapClothEffect.begin(), src.m_mapClothEffect.end());   // 衣服特效

    this->m_mapClothEffectSevenColor.insert(src.m_mapClothEffectSevenColor.begin(), src.m_mapClothEffectSevenColor.end()); // 衣服特效七色彩虹

    this->m_mapGeneData.insert(src.m_mapGeneData.begin(), src.m_mapGeneData.end());

    this->m_bHasCarriedPet = src.m_bHasCarriedPet;

    this->m_listMedalIDs.assign(src.m_listMedalIDs.begin(), src.m_listMedalIDs.end());

    this->m_nCurHorseTypeId = src.m_nCurHorseTypeId;
    this->m_nCurHorseOwnerId = src.m_nCurHorseOwnerId;
    this->m_nHorseSeatIndex = src.m_nHorseSeatIndex;

    return;
}

void RoleRoomImageData::OnEquip(DBItem &equip, unsigned int nEffectID)
{
    m_vecRoleItem.push_back(equip);
    if (0 != nEffectID)
        m_mapClothEffect[equip.m_nItemID] = nEffectID; // 更新

    return;
}

void RoleRoomImageData::UnEquip(itemid_t equipID)
{
    if (m_vecRoleItem.empty())
        return;
    for (std::vector<DBItem>::iterator iter = m_vecRoleItem.begin(); iter != m_vecRoleItem.end(); )
    {
        if (equipID == iter->m_nItemID)
        {
            iter = m_vecRoleItem.erase(iter);
        }
        else
        {
            ++iter;
        }
    }

    m_mapClothEffect.erase(equipID); // 还要发送广播
}

void RoleRoomImageData::EffectChange(itemid_t equipID, unsigned int nEffectID)
{
    for (size_t i = 0; i < m_vecRoleItem.size(); ++i)
    {
        if (m_vecRoleItem[i].m_nItemID == equipID)
        {
            m_mapClothEffect.erase(nEffectID);
            return;
        }
    }

    return;
}

void RoleRoomImageData::AddSevenColor(unsigned short nSuitID, unsigned char nColor)
{
    SevenColorMap::iterator iter = m_mapClothEffectSevenColor.find(nSuitID);
    if (iter != m_mapClothEffectSevenColor.end())
    {
        iter->second.insert((ESevenColorType)nColor);
    }
    else
    {
        std::set<ESevenColorType> sevenColorSet;
        sevenColorSet.insert((ESevenColorType)nColor);
        m_mapClothEffectSevenColor.insert(std::make_pair(nSuitID, sevenColorSet));
    }

    return;
}


void RoleRoomImageData::UpdateGeneMap(EGeneNotifyAction eAction, const GeneData & gene)
{
    switch (eAction)
    {
    case EGeneNotifyAction_Add:
    case EGeneNotifyAction_Reset:
        m_mapGeneData[gene.nGeneId] = gene;
        break;

    case EGeneNotifyAction_Del:
        {
            GeneDataMap::iterator it = m_mapGeneData.find(gene.nGeneId);
            if (it != m_mapGeneData.end())
            {
                m_mapGeneData.erase(it);
            }
        }
        break;
    case EGeneNotifyAction_None:
    default:
        break;
    }
    return;
}

void RoleRoomImageData::AddMedals(int nMedalID)
{
    m_listMedalIDs.push_back(nMedalID); // 添加进来
}

void RoleRoomImageData::SyncHorseData(int nHorseTypeID, int nHorseOwnerID, int nSeatIndex)
{
    m_nCurHorseTypeId = nHorseTypeID;
    m_nCurHorseOwnerId = nHorseOwnerID;
    m_nHorseSeatIndex = nSeatIndex;

    return;
}


bool RoleRoomImageData::GetDbItem(itemid_t itemID, DBItem &dbItem)
{
    for (size_t i = 0; i < m_vecRoleItem.size(); ++i)
    {
        if (m_vecRoleItem[i].m_nItemID == itemID)
        {
            dbItem = m_vecRoleItem[i];
            return true;
        }
    }

    return false;
}


std::string RoleRoomImageData::EncodeToString()
{
    static char buff[65535] = { 0 };
    CParamPool IOBuff(buff, 65535);

    unsigned short nOldPos = ParamHoldUShortPos(IOBuff);
    RoomEncodeEquipString(IOBuff);
    RoomEncodeEffectString(IOBuff);
    ParamReleaseUShortPos(IOBuff, nOldPos);

    RoomEncodeGeneString(IOBuff);
    RoomEncodePetString(IOBuff);
    RoomEncodeMedalString(IOBuff);
    RoomEncodeHorseString(IOBuff);

    return IOBuff.GetBuffString(); // 取得数据，进去
}

bool RoleRoomImageData::RoomEncodeEquipString(CParamPool &IOBuff)
{
//     unsigned short nOldPos = ParamHoldUShortPos(IOBuff);
    IOBuff.AddUShort((unsigned short)m_vecRoleItem.size());
    for (int i = 0; i < (int)m_vecRoleItem.size(); ++i)
    {
        DBItem &item = m_vecRoleItem[i];
        IOBuff.AddUInt(item.m_nTypeID);
        IOBuff.AddUShort(item.m_nColumn);
        IOBuff.AddUShort(item.m_nIndex); // grid

        // ItemBase.
        unsigned char uTempPos = (unsigned char)ParamHoldUCharPos(IOBuff);
        IOBuff.AddShort(item.m_nUseTimes);
        IOBuff.AddUShort(item.m_nCount);
        if (item.m_nDuration > 0)
        {
            int nDuration = item.m_nDuration - (int)time(NULL);
            IOBuff.AddInt(nDuration);
        }
        else
        {
            IOBuff.AddInt(-1);
        }

        // item equip. // 添加
        ItemConfig *itemConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(item.m_nTypeID);
        if (NULL != itemConfig && itemConfig->IsEquip())
        {
            IOBuff.AddUChar(item.m_bEquiped ? 1 : 0);
            std::map<itemid_t, unsigned int>::iterator iter = m_mapClothEffect.find(item.m_nItemID);
            IOBuff.AddUInt(iter != m_mapClothEffect.end() ? iter->second : 0);
        }
        ParamReleaseUCharPos(IOBuff, uTempPos);
    }

//    ParamReleaseUShortPos(IOBuff, nOldPos);

    return true;
}

bool RoleRoomImageData::RoomEncodeEffectString(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_mapClothEffectSevenColor.size());
    std::map<unsigned short, std::set<ESevenColorType> >::iterator iter = m_mapClothEffectSevenColor.begin();
    for (; iter != m_mapClothEffectSevenColor.end(); ++iter)
    {
        IOBuff.AddUShort(iter->first);
        IOBuff.AddUShort((unsigned short)iter->second.size());
        std::set<ESevenColorType>::iterator iterType = iter->second.begin();
        for (; iterType != iter->second.end(); ++iterType)
        {
            IOBuff.AddUChar((unsigned char)*iterType);
        }
    }

    return true;
}

bool RoleRoomImageData::RoomEncodeGeneString(CParamPool &IOBuff)
{
    unsigned short nPos = ParamHoldUShortPos(IOBuff);

    IOBuff.AddUShort((unsigned short)m_mapGeneData.size());
    std::map<unsigned short, GeneData>::iterator iter = m_mapGeneData.begin();
    for (; iter != m_mapGeneData.end(); ++iter)
    {
        GeneData &geneData = iter->second;
        geneData.nParam2 = 0;
        memset(geneData.szCmdParam, 0x0, sizeof(geneData.szCmdParam));

        geneData.doEncode(IOBuff);
    }

    ParamReleaseUShortPos(IOBuff, nPos);

    return true;
}

bool RoleRoomImageData::RoomEncodePetString(CParamPool &IOBuff)
{
    IOBuff.AddBool(false); // 

    return true;
}

bool RoleRoomImageData::RoomEncodeMedalString(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_listMedalIDs.size());
    std::list<int>::iterator iter = m_listMedalIDs.begin();
    for (; iter != m_listMedalIDs.end(); ++iter)
    {
        MedalDisplayInfo displayInfo;
        displayInfo.m_nType = CMedalCfgMgr::Instance().GetMedalType(*iter);
        CMedalCfgMgr::Instance().GetMedalIcon(*iter, displayInfo.m_strIcon);

        displayInfo.doEncode(IOBuff);
    }

    return true;
}

bool RoleRoomImageData::RoomEncodeHorseString(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nCurHorseTypeId);
    IOBuff.AddInt(m_nCurHorseOwnerId);
    IOBuff.AddInt(m_nHorseSeatIndex);

    return true;
}
