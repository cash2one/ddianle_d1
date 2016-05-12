#ifndef __QUESTMAN_H__
#define __QUESTMAN_H__

#include "QuestDoc.h"
#include "QuestDef.h"
#include "../datastructure/Macro_Define.h"
#include <map>
using namespace std;

const int QUEST_UPDATE_HEARTBEAT_SECONDS = 60;
const int C_MAX_QUEST_DATA_SIZE = 64000;	//最大任务数据, MAX_PACKET_SIZE, 最大包为65500,因为不考虑分包传输，所以该大小只能小于一个包的大小

class CQuestIDGenerator
{
public:
    CQuestIDGenerator();
    ~CQuestIDGenerator();
public:
    unsigned short m_nGroupIDBase;
    unsigned short  m_QuestIDBase;
};

class CQuestGroup
{
public:
    CQuestGroup();
    ~CQuestGroup();
public:
    unsigned short m_nGroupID;
    string m_strGroupName;
public:
    void ReadDoc(CFileStream *pFs)
    {
        pFs->ReadUShort(m_nGroupID);
        pFs->ReadString2(m_strGroupName);
    }

    void WriteDoc(CFileStream *pFs)
    {
        pFs->WriteUShort(m_nGroupID);
        pFs->WriteString2(m_strGroupName);
    }
};

//////////////////////////////////////////////////////////////////////////
//旧版任务系统
//////////////////////////////////////////////////////////////////////////
class CQuestMan
{
public:
    CQuestMan();
    virtual ~CQuestMan();

    bool LoadQuestBinaryData(const char* pszFileName);
    bool LoadQuest(const char* pszFileName);
    bool SaveQuest(const char* pszFileName);

    bool Add(CQuestDoc* pQuestDoc);
    CQuestDoc * GetQuestByID(unsigned short nQuestId) const;					// 根据ID得到对应的任务
    map<unsigned short, CQuestDoc *> * GetQuestDocBy(unsigned char nType);		// 获得对应类型的任务列表
    inline unsigned short GetNumber() const;

    map<unsigned short, CQuestDoc *> & GetAllQuest(){return m_mapQuestDoc;};

    long QuestStaticBinDataSize();
    char *QuestStaticBinData();

    void Destroy();																		

private:
    void SkipUnuseData(CFileStream *pFs);										// 跳过存在文件头部的编辑器的数据（分组、组名）
    void AddToTypeMap(CQuestDoc *pQuestDoc);
    bool IsValidQuest(CQuestDoc *pQuestDoc);

private:
    long m_nQuestBinDataSize;
    char m_szQuestBinData[C_MAX_QUEST_DATA_SIZE];
    std::vector<char>  m_vecQuestBinData;
    map<unsigned short, CQuestDoc *> m_mapQuestDoc;								// 全部任务
    map< unsigned char,  map<unsigned short, CQuestDoc*> > m_mapQusetDocTypeMap;// 按任务类型分类
};

//////////////////////////////////////////////////////////////////////////
//新版任务系统
//////////////////////////////////////////////////////////////////////////
class CQuestManNew
{
public:
    CQuestManNew();
    ~CQuestManNew();

public:
    bool LoadQuest(const char* pszFileName);
    bool LoadQuestBinData(const char* pszFileName);

    bool Add(CQuestDocNew* pQuestDoc);

    //通过ID查找任务
    CQuestDocNew* GetQuestByID(unsigned short nQuestID);

    //通过触发条件查找任务
    const map<unsigned short,CQuestDocNew*> * GetQuestDocByCondition(EQuest_Condition_Type type) const;

    //通过完成条件查找任务
    const map<unsigned short,CQuestDocNew*> * GetQuestDocByComplete(EQuest_Complete_Type type) const;

    //通过销毁条件查找任务
    const map<unsigned short,CQuestDocNew*> * GetQuestDocByDestroy(EQuest_Destroy_Type type) const;

    const map<unsigned short,CQuestDocNew*> * GetAllQuestDoc() const {return &m_mapQuest;}

    void Destroy();

    int GetBinDataSize(){return m_QuestBinDataSize;}
    const char* GetBinData(){return m_BinData;}

private:
    // 跳过存在文件头部的编辑器的数据（分组、组名）
    void SkipUnuseData(CFileStream *pFs);

private:
    map<unsigned short,CQuestDocNew*>                                m_mapQuest;
    map<EQuest_Condition_Type,map<unsigned short,CQuestDocNew*> >     m_mapCondition;
    map<EQuest_Complete_Type,map<unsigned short,CQuestDocNew*> >      m_mapComplete;
    map<EQuest_Destroy_Type,map<unsigned short,CQuestDocNew*> >       m_mapDestroy;
    map<EQuest_Reward_Type,map<unsigned short,CQuestReward*> >        m_mapReward;
    int                                                              m_QuestBinDataSize;
    char*                                                            m_BinData;
};

#endif // __QUESTMAN_H__

//end file

