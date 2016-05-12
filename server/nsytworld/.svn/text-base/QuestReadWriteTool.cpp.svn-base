#include "QuestReadWriteTool.h"
#include "QuestDoc.h"


CQusetReadWriteTool::CQusetReadWriteTool()
{
}

CQusetReadWriteTool::~CQusetReadWriteTool()
{
}

void CQusetReadWriteTool::ReadItemToMap(map<itemtype_t, CQuestItem *>& mapItem, CFileStream *pFs)
{
    unsigned short nCount = 0;
    pFs->ReadUShort(nCount);

    if (nCount > 0 && pFs != NULL)
    {
        for (int i = 0; i < nCount; i++)
        {
            CQuestItem *pItem = new CQuestItem;
            if (pItem != NULL)
            {
                pItem->ReadDoc(pFs);
                mapItem.insert(make_pair(pItem->m_nItemID, pItem));
            }
        }
    }
}

void CQusetReadWriteTool::WriteItemToMap(map<itemtype_t, CQuestItem *>& mapItem, CFileStream *pFs)
{
    if (pFs != NULL)
    {
        unsigned short nCount = (unsigned short)mapItem.size();
        if (nCount > 0)
        {
            map<itemtype_t, CQuestItem *>::iterator it;
            for (it = mapItem.begin(); it != mapItem.end(); it++)
            {
                it->second->WriteDoc(pFs);
            }
        }
    }
}

void CQusetReadWriteTool::ReadMusicToMap(map<unsigned short, CQuestMusic *>& mapMusic, CFileStream *pFs)
{
    unsigned short nCount = 0;
    pFs->ReadUShort(nCount);

    if (nCount > 0 && pFs != NULL)
    {
        for (int i = 0; i < nCount; i++)
        {
            CQuestMusic *pMusic = new CQuestMusic;
            pMusic->ReadDoc(pFs);
            mapMusic.insert(make_pair(pMusic->m_nMusicID, pMusic));
        }
    }
}

void CQusetReadWriteTool::WriteMusicToMap(map<unsigned short, CQuestMusic *>& mapMusic, CFileStream *pFs)
{
    if (pFs != NULL)
    {
        unsigned short nCount = (unsigned short)mapMusic.size();
        if (nCount > 0)
        {
            map<unsigned short, CQuestMusic *>::iterator it;
            for (it = mapMusic.begin(); it != mapMusic.end(); it++)
            {
                it->second->WriteDoc(pFs);
            }
        }
    }
}


// end of file

