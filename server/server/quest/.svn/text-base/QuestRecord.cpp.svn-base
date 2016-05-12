#include "QuestRecord.h"



const std::string keyMallBuy = "mb";                    // 商城购买
const std::string keyClotheEffectReplace = "cr";        // 魔法灌注
const std::string keyClotheEffectLevelUp = "cu";        // 魔法进阶
const std::string keyMoneyMagicCarry = "mg";            // 金券魔法阵
const std::string keyCoinMagicCarry = "mm";             // M币魔法阵

// {
//     "mb":[[502,1],[408,2]],
//     "cr":5,
//     "cu":4
// }

QuestRecordFactory::CreatorMap QuestRecordFactory::m_sParserCreator;
 
QuestRecordFactory::QuestRecordFactory()
{

}

QuestRecordFactory::~QuestRecordFactory()
{
    for (ParserMap::iterator iter = m_parserMap.begin(); iter != m_parserMap.end(); ++iter)
    {
        if (NULL != iter->second)
            delete iter->second;
        iter->second = NULL;
    }

    m_parserMap.clear();
}

void QuestRecordFactory::Init()
{
    RegisterQuestRecord(QuestRecordMallBuy);
    RegisterQuestRecord(QuestRecordClotheEffectReplace);
    RegisterQuestRecord(QuestRecordClotheEffectLevelUp);
    RegisterQuestRecord(QuestRecordMoneyMagicArray);
    RegisterQuestRecord(QuestRecordCoinMagicArray);

    return ; // 必须注册这些东西先
}


void QuestRecordFactory::RegisterParser(const std::string &key, QuestDyncCreate createFun)
{
    if (m_sParserCreator.find(key) != m_sParserCreator.end())
    {
        //...
        return ;
    }
    m_sParserCreator.insert(std::make_pair(key, createFun));

    return ;
}

void QuestRecordFactory::Parse(const std::string &strJson)
{
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(strJson, root))
        return;

    // 遍历所有的parser, 解析出来数据
    for (CreatorMap::iterator iter = m_sParserCreator.begin(); iter != m_sParserCreator.end(); ++iter)
    {
        if (root.isMember(iter->first))
        {
            QuestRecordBase* rec = iter->second(); //创建函数
            rec->FromJson(root[iter->first]); // 数据序列号进入了内存
            AddRecord(rec);
        }
    }

    return ;
}

std::string QuestRecordFactory::Serial()
{
    Json::Value root;
    for (ParserMap::iterator iter = m_parserMap.begin(); iter != m_parserMap.end(); ++iter)
    {
        Json::Value v = iter->second->ToJson();
        if (!v.isNull())
            root[iter->second->Key()] = v;
    }
    
    if (root.isNull())
        return "";

    Json::FastWriter fw;
    return fw.write(root);
}

void QuestRecordFactory::AddRecord(QuestRecordBase* rec)
{
    if (m_parserMap.find(rec->Key()) == m_parserMap.end())
        m_parserMap.insert(std::make_pair(rec->Key(), rec));
}

QuestRecordManager::QuestRecordManager()
{

}

QuestRecordManager::~QuestRecordManager()
{
    Clear();
}


QuestRecordFactory* QuestRecordManager::GetFactory(unsigned int questID)
{
    FactoryMap::iterator iter = m_factoryMap.find(questID);
    if (iter != m_factoryMap.end())
        return iter->second;

    return NULL;
}

QuestRecordFactory* QuestRecordManager::EnsureGetFactory(unsigned int questID)
{
    QuestRecordFactory* fact = GetFactory(questID);
    if (NULL != fact)
        return fact;
    fact = new QuestRecordFactory;
    m_factoryMap.insert(std::make_pair(questID, fact));

    return fact;
}

QuestRecordFactory* QuestRecordManager::Parse(unsigned int questID, const std::string &strJson)
{
    if (strJson.empty())
        return NULL;

    QuestRecordFactory* fact = EnsureGetFactory(questID);
    fact->Parse(strJson);
    return fact;
}

void QuestRecordManager::ReleaseQuestRecord(unsigned int questID)
{
    FactoryMap::iterator iter = m_factoryMap.find(questID);
    if (iter != m_factoryMap.end())
    {
        if (NULL != iter->second)
            delete iter->second, iter->second = NULL;

        m_factoryMap.erase(iter);
    }
}

void QuestRecordManager::Clear()
{
    for (FactoryMap::iterator iter = m_factoryMap.begin(); iter != m_factoryMap.end(); ++iter)
    {
        if (iter->second != NULL)
            delete iter->second, iter->second = NULL;
    }

    m_factoryMap.clear();
}

QuestRecordMallBuy::QuestRecordMallBuy(){}
QuestRecordMallBuy::~QuestRecordMallBuy()
{
    m_vecItemNums.clear();
}


void QuestRecordMallBuy::Init()
{

}

void QuestRecordMallBuy::FromJson(Json::Value &jvalue)
{
    m_vecItemNums.clear();
    if (!jvalue.isArray())
        return;

    for (size_t i = 0; i < jvalue.size(); ++i)
    {
        Json::Value vtemp = jvalue[i];
        if (!vtemp.isArray())
            continue;
        if (vtemp.size() != 2)
            continue;
        m_vecItemNums.push_back(std::make_pair(vtemp[Json::UInt(0)].asInt(), vtemp[Json::UInt(1)].asInt()));
    }

    return;
}

Json::Value QuestRecordMallBuy::ToJson()
{
    if (m_vecItemNums.empty())
        return Json::Value::null;

    Json::Value arys;
    
    for (size_t i = 0; i < m_vecItemNums.size(); ++i)
    {
        Json::Value tv;

        tv.append(m_vecItemNums[i].first);
        tv.append(m_vecItemNums[i].second);
        arys.append(tv);
    }

    return arys;
}


unsigned int QuestRecordMallBuy::GetItemNum(itemtype_t itemID)
{
    for (size_t i = 0; i < m_vecItemNums.size(); ++i)
    {
        if (m_vecItemNums[i].first == itemID)
            return m_vecItemNums[i].second;
    }

    return 0;
}

void QuestRecordMallBuy::AddItemNum(itemtype_t itemID, unsigned int count)
{
    for (size_t i = 0; i < m_vecItemNums.size(); ++i)
    {
        if (m_vecItemNums[i].first == itemID)
        {
            m_vecItemNums[i].second += count;
            return ;
        }
    }

    m_vecItemNums.push_back(std::make_pair(itemID, count));
    return ;
}

QuestRecordClotheEffectReplace::QuestRecordClotheEffectReplace()
    : m_num(0)
{}
QuestRecordClotheEffectReplace::~QuestRecordClotheEffectReplace(){}


void QuestRecordClotheEffectReplace::Init()
{

}

void QuestRecordClotheEffectReplace::FromJson(Json::Value &jvalue)
{
    m_num = jvalue.asInt();

    return ;
}

Json::Value QuestRecordClotheEffectReplace::ToJson()
{
    if (0 != m_num)
    {
        return Json::Value(m_num);
    }

    return Json::Value::null;
}

QuestRecordClotheEffectLevelUp::QuestRecordClotheEffectLevelUp()
    : m_num(0)
{

}
QuestRecordClotheEffectLevelUp::~QuestRecordClotheEffectLevelUp()
{}


void QuestRecordClotheEffectLevelUp::Init()
{

}

void QuestRecordClotheEffectLevelUp::FromJson(Json::Value &jvalue)
{
    m_num = jvalue.asInt();

    return ;
}

Json::Value QuestRecordClotheEffectLevelUp::ToJson()
{
    if (0 != m_num)
    {
        return Json::Value(m_num);
    }

    return Json::Value::null;
}

QuestRecordMoneyMagicArray::QuestRecordMoneyMagicArray()
    : m_num(0)
{

}
QuestRecordMoneyMagicArray::~QuestRecordMoneyMagicArray()
{}


void QuestRecordMoneyMagicArray::Init()
{

}

void QuestRecordMoneyMagicArray::FromJson(Json::Value &jvalue)
{
    m_num = jvalue.asInt();

    return;
}

Json::Value QuestRecordMoneyMagicArray::ToJson()
{
    if (0 != m_num)
    {
        return Json::Value(m_num);
    }

    return Json::Value::null;
}


QuestRecordCoinMagicArray::QuestRecordCoinMagicArray()
    : m_num(0)
{

}
QuestRecordCoinMagicArray::~QuestRecordCoinMagicArray()
{}


void QuestRecordCoinMagicArray::Init()
{

}

void QuestRecordCoinMagicArray::FromJson(Json::Value &jvalue)
{
    m_num = jvalue.asInt();

    return;
}

Json::Value QuestRecordCoinMagicArray::ToJson()
{
    if (0 != m_num)
    {
        return Json::Value(m_num);
    }

    return Json::Value::null;
}
