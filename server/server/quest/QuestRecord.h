#ifndef __QUESTRECORDOBJ_H__
#define __QUESTRECORDOBJ_H__

#include "../../socket/json/json.h"

#include "../../datastructure/DataStruct_Base.h"
#include <string>
#include <map>

//////////////////////////////////////////////////////////////////////////
// 说明：
// 本文件用来管理任务的额外参数（需要任务模块记录的参数）。不包括原有参数（物品使用次数，歌曲次数，功能使用次数）
// 数据库以json格式记录数据
// 每种类型的参数对应 QuestRecordBase的一个子类
// 每个玩家对应一个 QuestRecordManager。用于管理所有的任务参数
// 每个 QuestRecordManager 中包含 N个 QuestRecordFactory。 每个 QuestRecordFactory对应一条任务的参数
// 当添加一个新的类型的参数之后，需要调用 宏RegisterQuestRecord 注册任务
// 取某个任务对应的额外参数一律通过 QuestRecordManager中的以下接口：
// QuestRecordFactory* GetFactory(unsigned int questID);            // 取factory
// QuestRecordFactory* EnsureGetFactory(unsigned int questID);      // 取factory，如果没有则创建一个
// template <class T> T* GetQuestRecord(unsigned int questID);      // 取某类型参数
// template <class T> T* EnsureGetQuestRecord(unsigned int questID);// 取某类型参数，如果没有则创建一个
// QuestRecordManager 在任务组件中有一个实例。


extern const std::string keyMallBuy;
extern const std::string keyClotheEffectReplace;
extern const std::string keyClotheEffectLevelUp;
extern const std::string keyMoneyMagicCarry;            // 金券魔法阵
extern const std::string keyCoinMagicCarry;             // M币魔法阵

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
    ParserMap m_parserMap; // 任务关联的数据，只有存在的才会有

    typedef std::map<std::string, QuestDyncCreate> CreatorMap;
    static CreatorMap m_sParserCreator; // 全局一份，包含所有
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

// 管理一个人的所有任务数据
class QuestRecordManager
{
public:
    QuestRecordManager();
    ~QuestRecordManager();

    // 取得，
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

