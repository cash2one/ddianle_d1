/*
*2011-11-9
*by wangshijun
*把entity组件化。
*/

#ifndef __ENTITY_COMPONENT_H__
#define __ENTITY_COMPONENT_H__

#include "../../socket/Query.h"
#include "../../socket/GameMsg_Processor.h"
#include "../../datastructure/IEntity.h"
#include "ISerializable.h"
#include "../libServerFrame/QueryManager.h"
#include "../Pattern/Observer.h"
#include <string>

using namespace std;

extern CQueryManager*   g_pQueryMgr;

class IEntity;
class CParamPool;
struct ROLEINFO_DB;
class CDelHelper;
struct BuffData;

struct CComponentEvent 
{
    int nID;
    void * m_pData;
};


class CEntityComponent : public GameMsg_Processor, public ISerializable, public CommonPublisher
{
public:
    CEntityComponent(bool bProNetMsg, bool bProQueryMsg);
    virtual ~CEntityComponent(void);

public:
    virtual void Start() {}
    virtual bool CreateFromDB(ROLEINFO_DB* pRoleInforDB) { return true; }
    virtual bool PacketToDB(ROLEINFO_DB* pRoleInforDB) const { return true; }
    virtual bool PacketToCache(ROLEINFO_DB* pRoleInforDB) const { return true; }
    virtual void OnLogin() {}
    virtual void OnLogout() {}
    virtual void OnEvent(CComponentEvent& refEvent ) {}
    virtual void OnUpdate(const unsigned long& nTimeElapsed) {}
    virtual void SendExtraStaticData() {}
    virtual void PacketBuff(std::list<BuffData>& listBuff) {}
    virtual void RegComponentNetMsgMap() {}

public:
    virtual const char* SerializeName() { return ""; }
    virtual void SerializeComponent(CParamPool &IOBuff) {}

public:
    virtual void NotifyAll(IParam &param);

public:
    template< class T > T* GetComponent( )
    {
#ifdef WIN32
        T::IsImpl();
#endif
        T *pResult = NULL;
        const type_info &info_type = typeid(T);
        if (m_pEntity != NULL)
        {
            pResult = (T*)m_pEntity->GetComponent(info_type);
        }
        return pResult;
    }

public:
    int GetIndexInEntity();
    void SetIndexInEntity(int nIndexInObject);
    void AttachToEntity(IEntity * pEntity);

protected:
    IEntity * Entity();

public:
    // 参数内存有通过new产生的情况则需要通过pDelHelper辅助释放（特别是回调方法没有调用到的情况），以防泄露
    virtual void AddQuery(int nQueryType, int nAttachment = 0, void* pAttachment = NULL,
        __int64 nTag = 0, void* pTag = NULL, CDelHelper* pDelHelper = NULL) = 0;

    template <typename T>
    void AddQuery( int nQueryType, SharedPtr<T> sharedAttachment, int nAttachment = 0, __int64 nTag = 0 )
    {
        if ( g_pQueryMgr != NULL && m_pEntity != NULL )
        {
            QueryBase* pQuery = new Query<T>( nQueryType, sharedAttachment );
            pQuery->m_nSessionID = m_pEntity->GetSessionID();
            pQuery->m_nEntityIndex = m_pEntity->GetIndex();
            pQuery->m_nRoleID = m_pEntity->GetEntityID();
            pQuery->m_nAttachment = nAttachment;
            pQuery->m_nTag = nTag;

            g_pQueryMgr->AddQuery( pQuery );
        }
    }

private:
    IEntity *m_pEntity;
    int m_nIndexInEntity;
    string m_ComponentName;
};

#endif //__ENTITY_COMPONENT_H__

