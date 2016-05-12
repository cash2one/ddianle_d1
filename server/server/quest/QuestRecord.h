#ifndef __QUESTRECORDOBJ_H__
#define __QUESTRECORDOBJ_H__

#include "../../socket/json/json.h"

#include "../../datastructure/DataStruct_Base.h"
#include <string>
#include <map>

//////////////////////////////////////////////////////////////////////////
// ˵����
// ���ļ�������������Ķ����������Ҫ����ģ���¼�Ĳ�������������ԭ�в�������Ʒʹ�ô�������������������ʹ�ô�����
// ���ݿ���json��ʽ��¼����
// ÿ�����͵Ĳ�����Ӧ QuestRecordBase��һ������
// ÿ����Ҷ�Ӧһ�� QuestRecordManager�����ڹ������е��������
// ÿ�� QuestRecordManager �а��� N�� QuestRecordFactory�� ÿ�� QuestRecordFactory��Ӧһ������Ĳ���
// �����һ���µ����͵Ĳ���֮����Ҫ���� ��RegisterQuestRecord ע������
// ȡĳ�������Ӧ�Ķ������һ��ͨ�� QuestRecordManager�е����½ӿڣ�
// QuestRecordFactory* GetFactory(unsigned int questID);            // ȡfactory
// QuestRecordFactory* EnsureGetFactory(unsigned int questID);      // ȡfactory�����û���򴴽�һ��
// template <class T> T* GetQuestRecord(unsigned int questID);      // ȡĳ���Ͳ���
// template <class T> T* EnsureGetQuestRecord(unsigned int questID);// ȡĳ���Ͳ��������û���򴴽�һ��
// QuestRecordManager �������������һ��ʵ����


extern const std::string keyMallBuy;
extern const std::string keyClotheEffectReplace;
extern const std::string keyClotheEffectLevelUp;
extern const std::string keyMoneyMagicCarry;            // ��ȯħ����
extern const std::string keyCoinMagicCarry;             // M��ħ����

class QuestRecordBase;
typedef QuestRecordBase* (*QuestDyncCreate)();


class QuestRecordFactory
{
public:
    QuestRecordFactory();
    ~QuestRecordFactory();
    static void Init();
    static void RegisterParser(const std::string &key, QuestDyncCreate createFun);

    void Parse(const std::string &strJson);
    std::string Serial();
    
    template <class T> T* GetReocord();
    template <class T> T* EnsureGetReocord();

    void AddRecord(QuestRecordBase* rec);

    typedef std::map<std::string, QuestRecordBase*> ParserMap;
    ParserMap m_parserMap; // ������������ݣ�ֻ�д��ڵĲŻ���

    typedef std::map<std::string, QuestDyncCreate> CreatorMap;
    static CreatorMap m_sParserCreator; // ȫ��һ�ݣ���������
};

template <class T> 
T* QuestRecordFactory::GetReocord()
{
    ParserMap::iterator iter = m_parserMap.find(T::_SKey());
    if (iter == m_parserMap.end())
        return ((T*)NULL);

    return (T*)iter->second;
}

template <class T> 
T* QuestRecordFactory::EnsureGetReocord()
{
    T* rec = GetReocord<T>();
    if (NULL == rec)
    {
        rec = new T();
        rec->Init();
        AddRecord(rec);
    }

    return rec;
}

// ����һ���˵�������������
class QuestRecordManager
{
public:
    QuestRecordManager();
    ~QuestRecordManager();

    // ȡ�ã�
    QuestRecordFactory* GetFactory(unsigned int questID);
    QuestRecordFactory* EnsureGetFactory(unsigned int questID);
    
    template <class T> T* GetQuestRecord(unsigned int questID);
    template <class T> T* EnsureGetQuestRecord(unsigned int questID);

    QuestRecordFactory* Parse(unsigned int questID, const std::string &strJson);

    void ReleaseQuestRecord(unsigned int questID);
    void Clear();

private:
    typedef std::map<unsigned int, QuestRecordFactory*> FactoryMap;
    FactoryMap m_factoryMap;
};


template <class T> T* QuestRecordManager::GetQuestRecord(unsigned int questID)
{
    QuestRecordFactory* pFact = GetFactory(questID);
    if (NULL == pFact)
        return ((T*)NULL);

    return pFact->GetReocord<T>();
}
template <class T> T* QuestRecordManager::EnsureGetQuestRecord(unsigned int questID)
{
    QuestRecordFactory *pFact = EnsureGetFactory(questID);
    return pFact->EnsureGetReocord<T>();
}



#define ClassRecordKey(key, clsName) \
    static QuestRecordBase* _Create() {return new clsName;}\
    static std::string _SKey() {return key;}\
    virtual std::string Key() const {return key;}

#define RegisterQuestRecord(clsName) QuestRecordFactory::RegisterParser(clsName::_SKey(), clsName::_Create)

class QuestRecordBase
{
public:
    QuestRecordBase() {};
    virtual ~QuestRecordBase() {};
    virtual void Init() = 0;

    virtual std::string Key() const = 0;
    virtual void FromJson(Json::Value &jvalue) = 0;
    virtual Json::Value ToJson() = 0;

};

class QuestRecordMallBuy : public QuestRecordBase
{
public:
    QuestRecordMallBuy();
    ~QuestRecordMallBuy();
    virtual void Init();

    ClassRecordKey(keyMallBuy, QuestRecordMallBuy);
    virtual void FromJson(Json::Value &jvalue);
    virtual Json::Value ToJson();

    static QuestRecordBase* DyncCreate() {return new QuestRecordMallBuy;};

    unsigned int GetItemNum(itemtype_t itemID);
    void AddItemNum(itemtype_t itemID, unsigned int count);

public:
    typedef std::vector<std::pair<itemtype_t, unsigned int> > ItemVec;
    ItemVec m_vecItemNums;
};

class QuestRecordClotheEffectReplace : public QuestRecordBase
{
public:
    QuestRecordClotheEffectReplace();
    ~QuestRecordClotheEffectReplace();
    virtual void Init();

    ClassRecordKey(keyClotheEffectReplace, QuestRecordClotheEffectReplace);
    virtual void FromJson(Json::Value &jvalue);
    virtual Json::Value ToJson();

public:
    int m_num;
};

class QuestRecordClotheEffectLevelUp : public QuestRecordBase
{
public:
    QuestRecordClotheEffectLevelUp();
    ~QuestRecordClotheEffectLevelUp();
    virtual void Init();

    ClassRecordKey(keyClotheEffectLevelUp, QuestRecordClotheEffectLevelUp);
    virtual void FromJson(Json::Value &jvalue);
    virtual Json::Value ToJson();

public:
    int m_num;
};



class QuestRecordMoneyMagicArray : public QuestRecordBase
{
public:
    QuestRecordMoneyMagicArray();
    ~QuestRecordMoneyMagicArray();
    virtual void Init();

    ClassRecordKey(keyMoneyMagicCarry, QuestRecordMoneyMagicArray);
    virtual void FromJson(Json::Value &jvalue);
    virtual Json::Value ToJson();

public:
    int m_num;
};

class QuestRecordCoinMagicArray : public QuestRecordBase
{
public:
    QuestRecordCoinMagicArray();
    ~QuestRecordCoinMagicArray();
    virtual void Init();

    ClassRecordKey(keyCoinMagicCarry, QuestRecordCoinMagicArray);
    virtual void FromJson(Json::Value &jvalue);
    virtual Json::Value ToJson();

public:
    int m_num;
};


#endif // __QUESTRECORDOBJ_H__

