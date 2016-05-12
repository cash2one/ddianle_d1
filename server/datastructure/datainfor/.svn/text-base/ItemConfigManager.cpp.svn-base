#include "ItemConfigManager.h"
#include "ItemConfig.h"
#include "FileStream.h"
#include "../../socket/Windefine.h"
#include "../../socket/Utility.h"

ItemConfigManager::ItemConfigManager(void)
:m_nExtendItemDataSize(0)
{
    memset(m_szExtendItemBinData, 0, C_EXTEND_ITEM_DATA_SIZE);
}

ItemConfigManager::~ItemConfigManager(void)
{
    Destroy();
}

void ItemConfigManager::GetItemByClass(vector<ItemConfig*>& apItemInfo, unsigned char nClassType1, unsigned char nClassType2, unsigned char nClassType3)const
{
    std::map<itemtype_t, ItemConfig*>::const_iterator it = m_ItemMap.begin();
    ItemConfig*pItemInfo = NULL;
    for (; it != m_ItemMap.end(); it++)
    {
        pItemInfo = it->second;
        if ((pItemInfo->m_anType[0] == nClassType1) &&
            (pItemInfo->m_anType[1] == nClassType2) &&
            (pItemInfo->m_anType[2] == nClassType3)
            )
            apItemInfo.push_back(pItemInfo);
    }
};

bool ItemConfigManager::Add(ItemConfig* pItemInfo)
{
    if (pItemInfo == NULL)
    {
        return false;
    }

    map<itemtype_t, ItemConfig*>::iterator it = m_ItemMap.find(pItemInfo->m_nType);
    if (it != m_ItemMap.end())
    {
        delete it->second;
        m_ItemMap.erase(it);
    }

    bool bRes = m_ItemMap.insert(map<itemtype_t, ItemConfig*>::value_type(pItemInfo->m_nType, pItemInfo)).second;
    if (!bRes)
    {
        delete pItemInfo;
        pItemInfo = NULL;
    }
    else
    {
        for (list<GeneInfo *>::iterator itGene = pItemInfo->m_listGene.begin();
            itGene != pItemInfo->m_listGene.end(); ++itGene)
        {
            GeneInfo* pGeneInfo = *itGene;
            if (pGeneInfo != NULL && pGeneInfo->m_nGeneID > 0)
            {
                m_mapGene2ItemClass[pGeneInfo->m_nGeneID] = (EItemClassType)pItemInfo->m_anType[0];
            }
        }
    }

    return bRes;
}

bool ItemConfigManager::AddExtendItem(ItemConfig* pItemInfo)
{
    if (pItemInfo == NULL)
    {
        return false;
    }

    std::map<itemtype_t, ItemConfig*>::iterator it = m_ExtendItemMap.find(pItemInfo->m_nType);
    if (it != m_ExtendItemMap.end())
    {
        delete it->second;
        m_ExtendItemMap.erase(it);
    }
    bool bRes = m_ExtendItemMap.insert(map<itemtype_t, ItemConfig*>::value_type(pItemInfo->m_nType, pItemInfo)).second;
    if (!bRes)
    {
        delete pItemInfo;
        pItemInfo = NULL;
    }
    else
    {
        for (list<GeneInfo *>::iterator itGene = pItemInfo->m_listGene.begin();
            itGene != pItemInfo->m_listGene.end(); ++itGene)
        {
            GeneInfo* pGeneInfo = *itGene;
            if (pGeneInfo != NULL && pGeneInfo->m_nGeneID > 0)
            {
                m_mapGene2ItemClass[pGeneInfo->m_nGeneID] = (EItemClassType)pItemInfo->m_anType[0];
            }
        }
    }
    return bRes;
}

bool ItemConfigManager::LoadItemInfo(const char* pszFileName)
{
    Destroy();

    CFileStream file;
    file.open(pszFileName, "rb");
    bool bReturnValue = true;
    if (file.IsOpen())
    {
        unsigned short ulNumber = 0;
        file.ReadUShort(ulNumber);
        unsigned short i;
        if (ulNumber > 0)
        {
            for (i = 0; i < ulNumber; i++)
            {
                ItemConfig *pItemInfo = new ItemConfig;
                if (pItemInfo != NULL)
                {
                    pItemInfo->Load(file);
                    Add(pItemInfo);
                }
            }
        }
        // skin 
        ulNumber = 0;
        file.ReadUShort(ulNumber);
        if (ulNumber > 0){
            for (i = 0; i < ulNumber; i++){
                CSkinInfo* pSkinInfo = new CSkinInfo;
                if (pSkinInfo != NULL){
                    pSkinInfo->Load(file);
                    AddSkinInfo(pSkinInfo);
                }
            }
        }
        //animation
        ulNumber = 0;
        file.ReadUShort(ulNumber);
        if (ulNumber > 0){
            for (i = 0; i < ulNumber; i++){
                CItemAniInfo* pAniInfo = new CItemAniInfo;
                if (pAniInfo != NULL){
                    pAniInfo->Load(file);
                    AddItemAniInfo(pAniInfo);
                }
            }
        }
        file.close();
    }
    else{
        bReturnValue = false;
    }

    return bReturnValue;
}

bool ItemConfigManager::SaveItemInfo(const char* pszFileName)
{
    CFileStream file;
    CFileStream fileTxt;

    file.open(pszFileName, "wb");
    if (!file.IsOpen())
        return false;

    unsigned short ulNumber = (unsigned short)m_ItemMap.size();
    file.WriteUShort(ulNumber);

    std::map<itemtype_t, ItemConfig*>::const_iterator it = m_ItemMap.begin();
    ItemConfig*pItemInfo = NULL;
    for (; it != m_ItemMap.end(); it++)
    {
        pItemInfo = it->second;
        if (pItemInfo != NULL){
            pItemInfo->Save(file);
        }
    }

    // skin 
    ulNumber = (unsigned short)m_mapSkinInfo.size();
    file.WriteUShort(ulNumber);

    std::map<itemtype_t, CSkinInfo*>::const_iterator itSkin = m_mapSkinInfo.begin();
    CSkinInfo* pSkinInfo = NULL;
    for (; itSkin != m_mapSkinInfo.end(); ++itSkin)
    {
        pSkinInfo = itSkin->second;
        if (pSkinInfo != NULL)
        {
            pSkinInfo->Save(file);
        }
    }

    //Item animation
    ulNumber = (unsigned short)m_mapItemAniMap.size();
    file.WriteUShort(ulNumber);

    std::map<unsigned short, CItemAniInfo*>::const_iterator itItemAni = m_mapItemAniMap.begin();
    CItemAniInfo* pItemAniInfo = NULL;
    for (; itItemAni != m_mapItemAniMap.end(); ++itItemAni)
    {
        pItemAniInfo = itItemAni->second;
        if (pItemAniInfo != NULL)
        {
            pItemAniInfo->Save(file);
        }
    }

    //Item Transform
    ulNumber = (unsigned short)m_mapTransformInfo.size();
    file.WriteUShort(ulNumber);

    std::map<unsigned int, CItemTransformInfo*>::const_iterator itTrans = m_mapTransformInfo.begin();
    CItemTransformInfo* pTransformInfo = NULL;
    for (; itTrans != m_mapTransformInfo.end(); ++itTrans)
    {
        pTransformInfo = itTrans->second;
        if (pTransformInfo != NULL)
        {
            pTransformInfo->Save(file);
        }
    }

    file.close();

    return true;
}

bool ItemConfigManager::LoadExtendItemBinData(const char* pszFileName)
{
    CFileStream file;
    file.open(pszFileName, "rb");
    if (file.IsOpen()){
        fseek(file.GetFileHandle(), 0L, SEEK_END);
        m_nExtendItemDataSize = (int)ftell(file.GetFileHandle());
        ASSERT(m_nExtendItemDataSize < C_EXTEND_ITEM_DATA_SIZE);
        if (m_nExtendItemDataSize < C_EXTEND_ITEM_DATA_SIZE) {
            fseek(file.GetFileHandle(), 0L, SEEK_SET);
            file.ReadMemory(m_szExtendItemBinData, m_nExtendItemDataSize);
            file.close();
            return true;
        }
    }

    return false;
}

bool ItemConfigManager::LoadExtendItemInfo(const char* pszFileName)
{
    CFileStream file;
    file.open(pszFileName, "rb");
    bool bReturnValue = false;

    if (file.IsOpen()){
        fseek(file.GetFileHandle(), 0L, SEEK_END);
        m_nExtendItemDataSize = ftell(file.GetFileHandle());
        fseek(file.GetFileHandle(), 0L, SEEK_SET);
        ASSERT(m_nExtendItemDataSize < C_EXTEND_ITEM_DATA_SIZE);
        if (m_nExtendItemDataSize < C_EXTEND_ITEM_DATA_SIZE) {
            unsigned short ulNumber = 0;
            file.ReadUShort(ulNumber);
            if (ulNumber > 0) {
                for (int i = 0; i < ulNumber; i++) {
                    ItemConfig *pItemInfo = new ItemConfig;
                    if (pItemInfo != NULL){
                        pItemInfo->Load(file);
                        AddExtendItem(pItemInfo);
                    }
                }
            }

            bReturnValue = true;
            file.close();
        }
    }

    return bReturnValue;
}

bool ItemConfigManager::SaveExtendItemInfo(const char* pszFileName)
{
    if (pszFileName == NULL) {
        return false;
    }

    CFileStream file;
    CFileStream fileTxt;
    file.open(pszFileName, "wb");
    if (!file.IsOpen()) {
        return false;
    }

    unsigned short ulNumber = (unsigned short)m_ExtendItemMap.size();
    file.WriteUShort(ulNumber);
    for (std::map<itemtype_t, ItemConfig*>::const_iterator it = m_ExtendItemMap.begin(); it != m_ExtendItemMap.end(); ++it) {
        ItemConfig *pItemInfo = it->second;
        if (pItemInfo != NULL) {
            pItemInfo->Save(file);
        }
    }

    file.close();
    return true;
}


//根据ID得到对应的CItemInfo
ItemConfig * ItemConfigManager::GetByID(itemtype_t nType) const
{
    ItemConfig * pItem = NULL;
    if (nType > 0)
    {
        std::map<itemtype_t, ItemConfig*>::const_iterator itExtend = m_ExtendItemMap.find(nType);
        if (itExtend != m_ExtendItemMap.end())
        {
            pItem = itExtend->second;
        }
        else
        {
            std::map<itemtype_t, ItemConfig*>::const_iterator it = m_ItemMap.find(nType);
            if (it != m_ItemMap.end())
            {
                pItem = it->second;
            }
        }
    }

    return pItem;
}

EItemColumn ItemConfigManager::GetColumnByItemType(itemtype_t nType)
{
    if (nType > 0)
    {
        std::map<itemtype_t, ItemConfig*>::const_iterator itExtend = m_ExtendItemMap.find(nType);
        if (itExtend != m_ExtendItemMap.end())
        {
            return itExtend->second->GetItemColumnByType();
        }
        else
        {
            std::map<itemtype_t, ItemConfig*>::const_iterator it = m_ItemMap.find(nType);
            if (it != m_ItemMap.end())
            {
                return it->second->GetItemColumnByType();
            }
        }
    }

    return EItemColumn_Invalid;
}

bool ItemConfigManager::IsEquip(itemtype_t nItemType)const
{
    ItemConfig* pItemInfo = GetByID(nItemType);
    return pItemInfo->IsEquip();
}

bool ItemConfigManager::IsExpandable(itemtype_t nItemType)const
{
    ItemConfig* pItemInfo = GetByID(nItemType);
    return pItemInfo->IsExpandable();
}

int ItemConfigManager::ExtendItemBinDataSize()
{
    return m_nExtendItemDataSize;
}

const char * ItemConfigManager::ExtendItemBinData()
{
    return m_szExtendItemBinData;
}

//释放所有对象
void ItemConfigManager::Destroy()
{
    std::map<itemtype_t, ItemConfig*>::const_iterator it = m_ItemMap.begin();
    while (it != m_ItemMap.end())
    {
        delete (ItemConfig*)it->second;
        it++;
    };
    m_ItemMap.clear();

    for (std::map<itemtype_t, ItemConfig*>::const_iterator itExtend = m_ExtendItemMap.begin(); itExtend != m_ExtendItemMap.end(); ++itExtend)
    {
        delete (ItemConfig*)itExtend->second;
    };
    m_ExtendItemMap.clear();

    std::map< itemtype_t, CSkinInfo* >::const_iterator itSkin = m_mapSkinInfo.begin();
    for (; itSkin != m_mapSkinInfo.end(); ++itSkin)
    {
        delete (CSkinInfo*)itSkin->second;
    }
    m_mapSkinInfo.clear();

    std::map<unsigned int, CItemTransformInfo*>::iterator itTrans = m_mapTransformInfo.begin();
    while (itTrans != m_mapTransformInfo.end())
    {
        delete itTrans->second;
        ++itTrans;
    }
    m_mapTransformInfo.clear();

    std::map<unsigned short, CItemAniInfo*>::iterator itAni = m_mapItemAniMap.begin();
    while (itAni != m_mapItemAniMap.end())
    {
        delete itAni->second;
        ++itAni;
    }
    m_mapItemAniMap.clear();
}

bool ItemConfigManager::AddSkinInfo(CSkinInfo* pInfo)
{
    if (pInfo != NULL)
    {
        if (pInfo->m_nType > 0)
        {
            m_mapSkinInfo.insert(map<itemtype_t, CSkinInfo*>::value_type(pInfo->m_nType, pInfo));
            return true;
        }
        delete pInfo;
        pInfo = NULL;
    }

    return false;
}

CSkinInfo* ItemConfigManager::GetSkinInfoByID(itemtype_t nType) const
{
    std::map<itemtype_t, CSkinInfo*>::const_iterator it = m_mapSkinInfo.find(nType);
    if (it != m_mapSkinInfo.end())
    {
        return (CSkinInfo*)it->second;
    }
    return NULL;
}

CItemAniInfo* ItemConfigManager::GetItemAniByID(unsigned short nType) const
{
    std::map<unsigned short, CItemAniInfo*>::const_iterator it = m_mapItemAniMap.find(nType);
    if (it != m_mapItemAniMap.end())
    {
        return (CItemAniInfo*)it->second;
    }
    return NULL;
}

bool ItemConfigManager::AddItemAniInfo(CItemAniInfo* pInfo)
{
    if (pInfo != NULL)
    {
        if (pInfo->m_nType > 0)
        {
            m_mapItemAniMap.insert(map<unsigned short, CItemAniInfo*>::value_type(pInfo->m_nType, pInfo));
            return true;
        }
        delete pInfo;
        pInfo = NULL;
    }

    return false;
}

CItemTransformInfo* ItemConfigManager::GetTransformByID(unsigned int nId) const
{
    CItemTransformInfo* pInfo = NULL;
    std::map<unsigned int, CItemTransformInfo*>::const_iterator it = m_mapTransformInfo.find(nId);
    if (it != m_mapTransformInfo.end())
    {
        pInfo = it->second;
    }
    return pInfo;
}

bool ItemConfigManager::AddItemTransformInfo(CItemTransformInfo* pInfo)
{
    bool bRes = m_mapTransformInfo.insert(std::make_pair(pInfo->m_nId, pInfo)).second;
    if (!bRes)
    {
        delete pInfo;
        pInfo = NULL;
    }
    return bRes;
}

bool ItemConfigManager::LoadTransformInfo(const char* pszFileName)
{
    CFileStream file;
    file.open(pszFileName, "rb");
    bool bReturnValue = false;

    if (file.IsOpen()){
        unsigned short ulNumber = 0;
        file.ReadUShort(ulNumber);
        if (ulNumber > 0) {
            for (int i = 0; i < ulNumber; i++) {
                CItemTransformInfo *pInfo = new CItemTransformInfo;
                if (pInfo != NULL){
                    pInfo->Load(file);
                    AddItemTransformInfo(pInfo);
                }
            }
        }

        bReturnValue = true;
        file.close();
    }

    return bReturnValue;
}

bool ItemConfigManager::SaveTransformInfo(const char* pszFileName)
{
    if (pszFileName == NULL)
        return false;

    CFileStream file;
    CFileStream fileTxt;
    file.open(pszFileName, "wb");
    if (!file.IsOpen())
        return false;

    unsigned short ulNumber = (unsigned short)m_mapTransformInfo.size();
    file.WriteUShort(ulNumber);
    for (std::map<unsigned int, CItemTransformInfo*>::const_iterator it = m_mapTransformInfo.begin();
        it != m_mapTransformInfo.end(); ++it)
    {
        CItemTransformInfo *pInfo = it->second;
        if (pInfo != NULL)
            pInfo->Save(file);
    }

    file.close();
    return true;
}

EItemClassType ItemConfigManager::GetGeneItemClass(unsigned short nGeneID)
{
    std::map<int, EItemClassType>::iterator it = m_mapGene2ItemClass.find(nGeneID);
    if (it == m_mapGene2ItemClass.end())
    {
        return eItemClassType_None;
    }
    return it->second;
}

