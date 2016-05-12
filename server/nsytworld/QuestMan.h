#ifndef __QUESTMAN_H__
#define __QUESTMAN_H__

#include "QuestDoc.h"
#include "QuestDef.h"
#include "../datastructure/Macro_Define.h"
#include <map>
using namespace std;

const int QUEST_UPDATE_HEARTBEAT_SECONDS = 60;
const int C_MAX_QUEST_DATA_SIZE = 64000;	//�����������, MAX_PACKET_SIZE, ����Ϊ65500,��Ϊ�����Ƿְ����䣬���Ըô�Сֻ��С��һ�����Ĵ�С

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
//�ɰ�����ϵͳ
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
    CQuestDoc * GetQuestByID(unsigned short nQuestId) const;					// ����ID�õ���Ӧ������
    map<unsigned short, CQuestDoc *> * GetQuestDocBy(unsigned char nType);		// ��ö�Ӧ���͵������б�
    inline unsigned short GetNumber() const;

    map<unsigned short, CQuestDoc *> & GetAllQuest(){return m_mapQuestDoc;};

    long QuestStaticBinDataSize();
    char *QuestStaticBinData();

    void Destroy();																		

private:
    void SkipUnuseData(CFileStream *pFs);										// ���������ļ�ͷ���ı༭�������ݣ����顢������
    void AddToTypeMap(CQuestDoc *pQuestDoc);
    bool IsValidQuest(CQuestDoc *pQuestDoc);

private:
    long m_nQuestBinDataSize;
    char m_szQuestBinData[C_MAX_QUEST_DATA_SIZE];
    std::vector<char>  m_vecQuestBinData;
    map<unsigned short, CQuestDoc *> m_mapQuestDoc;								// ȫ������
    map< unsigned char,  map<unsigned short, CQuestDoc*> > m_mapQusetDocTypeMap;// ���������ͷ���
};

//////////////////////////////////////////////////////////////////////////
//�°�����ϵͳ
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

    //ͨ��ID��������
    CQuestDocNew* GetQuestByID(unsigned short nQuestID);

    //ͨ������������������
    const map<unsigned short,CQuestDocNew*> * GetQuestDocByCondition(EQuest_Condition_Type type) const;

    //ͨ�����������������
    const map<unsigned short,CQuestDocNew*> * GetQuestDocByComplete(EQuest_Complete_Type type) const;

    //ͨ������������������
    const map<unsigned short,CQuestDocNew*> * GetQuestDocByDestroy(EQuest_Destroy_Type type) const;

    const map<unsigned short,CQuestDocNew*> * GetAllQuestDoc() const {return &m_mapQuest;}

    void Destroy();

    int GetBinDataSize(){return m_QuestBinDataSize;}
    const char* GetBinData(){return m_BinData;}

private:
    // ���������ļ�ͷ���ı༭�������ݣ����顢������
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

