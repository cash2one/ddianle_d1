#ifndef	__QUESTREADWRITETOOL_H__
#define __QUESTREADWRITETOOL_H__

#include "../datastructure/datainfor/FileStream.h"
#include "../datastructure/DataStruct_Base.h"
#include <map>

using namespace std;

class CQuestItem;
class CQuestMusic;


//////////////////////////////////////////////////////////////////////////
//任务读配置和写保存
//////////////////////////////////////////////////////////////////////////
class CQusetReadWriteTool
{
public:
    CQusetReadWriteTool();
    ~CQusetReadWriteTool();

public:
    static void ReadItemToMap(map<itemtype_t, CQuestItem *>& m_mapItem, CFileStream *pFs);
    static void WriteItemToMap(map<itemtype_t, CQuestItem *>& m_mapItem, CFileStream *pFs);

    static void ReadMusicToMap(map<unsigned short, CQuestMusic *>& m_mapMusic, CFileStream *pFs);
    static void WriteMusicToMap(map<unsigned short, CQuestMusic *>& m_mapMusic, CFileStream *pFs);
};



#endif

// end of file


