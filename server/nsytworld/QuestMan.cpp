#include "QuestMan.h"
#include "../socket/Log.h"
#include "../datastructure/datainfor/ConfigManager.h"

CQuestIDGenerator::CQuestIDGenerator()
{
    m_nGroupIDBase = 0;
    m_QuestIDBase = 0;
}

CQuestIDGenerator::~CQuestIDGenerator()
{
}

CQuestGroup::CQuestGroup()
{
}

CQuestGroup::~CQuestGroup()
{
}

CQuestMan::CQuestMan(void)
    :m_nQuestBinDataSize(0)
{
    memset(m_szQuestBinData, 0, C_MAX_QUEST_DATA_SIZE);
}

CQuestMan::~CQuestMan(void)
{
    Destroy();
}

void CQuestMan::SkipUnuseData(CFileStream *pFs)
{
    // ID
    CQuestIDGenerator IDGenerator;
    unsigned short nBaseID = 0;
    pFs->ReadUShort( nBaseID);

    IDGenerator.m_nGroupIDBase = nBaseID;
    pFs->ReadUShort(nBaseID);
    IDGenerator.m_QuestIDBase = nBaseID;

    // Group
    map<unsigned short, CQuestGroup> mapGroup;	
    unsigned short nGroupCount = 0;
    pFs->ReadUShort( nGroupCount);

    if(nGroupCount > 0)
    {
        mapGroup.clear();
        for (int i = 0; i < nGroupCount; i++)
        {
            CQuestGroup group;
            group.ReadDoc(pFs);
            mapGroup.insert(make_pair(group.m_nGroupID,group));
        }
    }
}

long CQuestMan::QuestStaticBinDataSize()
{
    return m_nQuestBinDataSize;	
}

char *CQuestMan::QuestStaticBinData()
{
    return m_szQuestBinData;
}

// 释放所有对象
void CQuestMan::Destroy()
{
    map<unsigned short, CQuestDoc*>::iterator it = m_mapQuestDoc.begin();
    while(it != m_mapQuestDoc.end())
    {
        delete it->second;
        it++;
    };
    m_mapQuestDoc.clear();
    m_mapQusetDocTypeMap.clear();
}

bool CQuestMan::LoadQuestBinaryData(const char* pszFileName)
{
    CFileStream file;
    file.open(pszFileName,"rb");
    if(file.IsOpen()){
        fseek(file.GetFileHandle(), 0L, SEEK_END);
        m_nQuestBinDataSize = ftell(file.GetFileHandle());
        ASSERT(m_nQuestBinDataSize < C_MAX_QUEST_DATA_SIZE);
        if (m_nQuestBinDataSize < C_MAX_QUEST_DATA_SIZE ) {
            fseek(file.GetFileHandle(), 0L, SEEK_SET);
            file.ReadMemory(m_szQuestBinData, m_nQuestBinDataSize);
            file.close();
            return true;
        }
    }

    return false;
}

bool CQuestMan::LoadQuest(const char* pszFileName)
{
    return true;
    //CFileStream file;
    //file.open(pszFileName,"rb");
    //if(file.IsOpen()){
    //	//跳过对游戏数据无用的信息(对编辑器有用)
    //	SkipUnuseData(&file);
    //	unsigned short nNumber = 0;
    //	file.ReadUShort(nNumber);
    //	if(nNumber>0){
    //		for( int i = 0; i < nNumber;  i++){
    //			CQuestDoc *pQuestDoc = new CQuestDoc;
    //			if (pQuestDoc != NULL){
    //				pQuestDoc->ReadDoc(&file);
    //				Add(pQuestDoc);
    //				AddToTypeMap(pQuestDoc);
    //			}
    //		}
    //	}
    //	file.close();
    //	return true;
    //}
    //return false;
}

bool CQuestMan::SaveQuest(const char* pszFileName)
{
    CFileStream file;
    file.open(pszFileName,"wb");
    if(!file.IsOpen()){
        return false;
    }
    unsigned short nNumber = (unsigned short)m_mapQuestDoc.size();
    file.WriteUShort(nNumber);

    for(map<unsigned short, CQuestDoc *>::iterator i = m_mapQuestDoc.begin(); 
        i != m_mapQuestDoc.end(); ++i)
    {
        i->second->WriteDoc(&file);
    }
    file.close();

    return true;
}

bool CQuestMan::Add(CQuestDoc* pQuestDoc)
{
	if (pQuestDoc != NULL && pQuestDoc->m_CQusetBaseInfo.m_nQuestID > 0)
	{
		if (!IsValidQuest(pQuestDoc))
		{
			WriteLog(LOGLEVEL_WARNING, "[WARNING] Invalid quest: %d\n", (int)pQuestDoc->m_CQusetBaseInfo.m_nQuestID);
		}
		m_mapQuestDoc.insert(make_pair(pQuestDoc->m_CQusetBaseInfo.m_nQuestID, pQuestDoc));
		return true;
	}
	if (pQuestDoc != NULL)
	{
		delete pQuestDoc;
		pQuestDoc = NULL;
	}

    return false;
}

bool CQuestMan::IsValidQuest(CQuestDoc *pQuestDoc)
{
    if (pQuestDoc != NULL)
    {
        int nQuestID = (int)pQuestDoc->m_CQusetBaseInfo.m_nQuestID;

        // Music
        for (map<unsigned short, CQuestMusic *>::iterator it = pQuestDoc->m_mapMusicTarget.begin();
            it != pQuestDoc->m_mapMusicTarget.end(); ++it)
        {
            if (it->first == 0)
            {
                continue;
            }

			if (it->second == NULL)
			{
				WriteLog(LOGLEVEL_WARNING, "[WARNING] Quest[%d] Music not exist 1: music id = %d", nQuestID, (int)it->first);
				return false;
			}
			else if (ConfigManager::Instance().GetMusicConfigManager().GetByID(it->second->m_nMusicID) == NULL)
			{
				WriteLog(LOGLEVEL_WARNING, "[WARNING] Quest[%d] Music not exist 2: music id = %d", nQuestID, (int)it->first);
				return false;
			}
		}

		// Item use
		for (map<itemtype_t, CQuestItem *>::iterator it = pQuestDoc->m_mapItemUse.begin();
			it != pQuestDoc->m_mapItemUse.end(); ++it)
		{
			if (it->first == 0)
			{
				continue;
			}
			if (it->second == NULL)
			{
				WriteLog(LOGLEVEL_WARNING, "[WARNING] Quest[%d] use Item not exist 1: Item id = %d", nQuestID, (int)it->first);
				return false;
			}
			else if (ConfigManager::Instance().GetItemConfigManager().GetByID(it->second->m_nItemID) == NULL)
			{
				WriteLog(LOGLEVEL_WARNING, "[WARNING] Quest[%d] use Item not exist 2: Item id = %d", nQuestID, (int)it->first);
				return false;
			}
		}

		// Item collection
		for (map<itemtype_t, CQuestItem *>::iterator it = pQuestDoc->m_mapItemCollection.begin();
			it != pQuestDoc->m_mapItemCollection.end(); ++it)
		{
			if (it->first == 0)
			{
				continue;
			}
			if (it->second == NULL)
			{
				WriteLog(LOGLEVEL_WARNING, "[WARNING] Quest[%d] collection Item not exist 1: Item id = %d", nQuestID, (int)it->first);
				return false;
			}
			else if (ConfigManager::Instance().GetItemConfigManager().GetByID(it->second->m_nItemID) == NULL)
			{
				WriteLog(LOGLEVEL_WARNING, "[WARNING] Quest[%d] collection Item not exist 2: Item id = %d", nQuestID, (int)it->first);
				return false;
			}
		}

		// Item reward - male
		for (map<itemtype_t, CQuestItem *>::iterator it = pQuestDoc->m_nRewrad.m_mapRewardItemMale.begin();
			it != pQuestDoc->m_nRewrad.m_mapRewardItemMale.end(); ++it)
		{
			if (it->second == NULL)
			{
				WriteLog(LOGLEVEL_WARNING, "[WARNING] Quest[%d] male reward Item not exist 1: Item id = %d", nQuestID, (int)it->first);
				return false;
			}
			else if (ConfigManager::Instance().GetItemConfigManager().GetByID(it->second->m_nItemID) == NULL)
			{
				WriteLog(LOGLEVEL_WARNING, "[WARNING] Quest[%d] male reward Item not exist 2: Item id = %d", nQuestID, (int)it->first);
				return false;
			}
		}


		// Item reward - female
		for (map<itemtype_t, CQuestItem *>::iterator it = pQuestDoc->m_nRewrad.m_mapRewardItemFemale.begin();
			it != pQuestDoc->m_nRewrad.m_mapRewardItemFemale.end(); ++it)
		{
			if (it->second == NULL)
			{
				WriteLog(LOGLEVEL_WARNING, "[WARNING] Quest[%d] female reward Item not exis 1t: Item id = %d", nQuestID, (int)it->first);
				return false;
			}
			else if (ConfigManager::Instance().GetItemConfigManager().GetByID(it->second->m_nItemID) == NULL)
			{
				WriteLog(LOGLEVEL_WARNING, "[WARNING] Quest[%d] female reward Item not exist 2: Item id = %d", nQuestID, (int)it->first);
				return false;
			}
		}
	}
	
	return true;
}

void CQuestMan::AddToTypeMap(CQuestDoc *pQuestDoc)
{
    if (pQuestDoc != NULL && pQuestDoc->m_CQusetBaseInfo.m_nQuestID > 0)
    {
        unsigned char nType = pQuestDoc->m_CQusetBaseInfo.m_nType;
        map<unsigned char, map<unsigned short, CQuestDoc *> >::iterator it;
        it = m_mapQusetDocTypeMap.find(nType);
        if (it != m_mapQusetDocTypeMap.end())
        {
            map<unsigned short, CQuestDoc *> *mapTemp = &(it->second);
            mapTemp->insert(make_pair(pQuestDoc->m_CQusetBaseInfo.m_nQuestID, pQuestDoc));
        }
        else
        {
            map<unsigned short, CQuestDoc *> mapTemp;
            mapTemp.insert(make_pair(pQuestDoc->m_CQusetBaseInfo.m_nQuestID, pQuestDoc));
            m_mapQusetDocTypeMap.insert(make_pair(nType, mapTemp));
        }
    }
}

CQuestDoc * CQuestMan::GetQuestByID(unsigned short nQuestID) const
{
    map<unsigned short, CQuestDoc *>:: const_iterator it;
    it = m_mapQuestDoc.find(nQuestID);
    if (it != m_mapQuestDoc.end())
    {
        return it->second;
    }

    return NULL;
}

unsigned short CQuestMan::GetNumber() const
{
    return (unsigned short )m_mapQuestDoc.size();
}

map<unsigned short, CQuestDoc *> * CQuestMan::GetQuestDocBy(unsigned char nType)
{
    map<unsigned char, map<unsigned short, CQuestDoc*  > >::iterator it;
    it = m_mapQusetDocTypeMap.find(nType);

    if (it != m_mapQusetDocTypeMap.end())
    {
        return &(it->second);
    }

    return NULL;
}

CQuestManNew::CQuestManNew()
{
    m_QuestBinDataSize = 0;
    m_BinData = NULL;
}

CQuestManNew::~CQuestManNew()
{
    Destroy();
}

bool CQuestManNew::LoadQuest(const char* pszFileName)
{
    CFileStream file;
    file.open(pszFileName,"rb");
    if(file.IsOpen())
    {
        //跳过对游戏数据无用的信息(对编辑器有用)
        SkipUnuseData(&file);
        unsigned short nNumber = 0;
        file.ReadUShort(nNumber);
        if(nNumber>0)
        {
            for( int i = 0; i < nNumber;  i++)
            {
                CQuestDocNew *pQuestDoc = new CQuestDocNew;
                if (pQuestDoc != NULL)
                {
                    pQuestDoc->ReadDoc(&file);
                    Add(pQuestDoc);
                }
            }
        }
        file.close();
        return true;
    }
    return false;
}

bool CQuestManNew::Add(CQuestDocNew* pQuestDoc)
{
    m_mapQuest.insert(make_pair(pQuestDoc->m_BaseInfo.ID,pQuestDoc));
    return true;
}

CQuestDocNew* CQuestManNew::GetQuestByID(unsigned short nQuestID)
{
    std::map<unsigned short,CQuestDocNew*>::iterator it = m_mapQuest.find(nQuestID);
    if (it != m_mapQuest.end())
    {
        return it->second;
    }
    return NULL;
}

const map<unsigned short,CQuestDocNew*> * CQuestManNew::GetQuestDocByCondition(EQuest_Condition_Type type) const
{
    return NULL;
}

const map<unsigned short,CQuestDocNew*> * CQuestManNew::GetQuestDocByComplete(EQuest_Complete_Type type) const
{
    return NULL;
}

const map<unsigned short,CQuestDocNew*> * CQuestManNew::GetQuestDocByDestroy(EQuest_Destroy_Type type) const
{
    return NULL;
}

void CQuestManNew::Destroy()
{
    map<unsigned short,CQuestDocNew*>::iterator it = m_mapQuest.begin();
    for (;it != m_mapQuest.end() ; it++)
    {
        CQuestDocNew* pDoc = it->second;
        delete pDoc,pDoc = NULL;
    }
    if (m_BinData != NULL)
    {
        delete[] m_BinData;
        m_BinData = NULL;
        m_QuestBinDataSize = 0;
    }
    m_mapQuest.clear();
    m_mapCondition.clear();
    m_mapComplete.clear();
    m_mapReward.clear();
    m_mapDestroy.clear();
}

void CQuestManNew::SkipUnuseData(CFileStream *pFs)
{
    CQuestIDGenerator IDGenerator;
    unsigned short nBaseID = 0;
    pFs->ReadUShort( nBaseID);

    IDGenerator.m_nGroupIDBase = nBaseID;
    pFs->ReadUShort(nBaseID);
    IDGenerator.m_QuestIDBase = nBaseID;

    // Group
    map<unsigned short, CQuestGroup> mapGroup;	
    unsigned short nGroupCount = 0;
    pFs->ReadUShort( nGroupCount);

    if(nGroupCount > 0)
    {
        mapGroup.clear();
        for (int i = 0; i < nGroupCount; i++)
        {
            CQuestGroup group;
            group.ReadDoc(pFs);
            mapGroup.insert(make_pair(group.m_nGroupID,group));
        }
    }
}

bool CQuestManNew::LoadQuestBinData(const char* pszFileName)
{
    CFileStream file;
    file.open(pszFileName,"rb");
    if(file.IsOpen()){
        fseek(file.GetFileHandle(), 0L, SEEK_END);
        m_QuestBinDataSize = ftell(file.GetFileHandle());
        m_BinData = new char[m_QuestBinDataSize];
        if (m_BinData != NULL)
        {
            fseek(file.GetFileHandle(), 0L, SEEK_SET);
            file.ReadMemory(m_BinData, m_QuestBinDataSize);
            file.close();
            return true;
        }
    }
    return false;
}

//end file


