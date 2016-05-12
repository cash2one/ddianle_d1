/********************************************************************
created:	2010/12/01
filename: 	DataStructure\Entity.h
author:		meijiuhua

purpose:	所有的实体的基类.
实体包括player,monster(包括npc),portal(传送门),location,
小火球等魔法,射出的箭等
*********************************************************************/

#ifndef __CEntity_H__
#define __CEntity_H__

#include "IEntity.h"
#include "datainfor/EntityType.h"
#include "DataStruct_Game.h"
#include "../server/share/ServerDataStruct.h"
#include <map>
#include <vector>
#include <string>
#include <typeinfo>

#ifdef WIN32
//(可能)尚未初始化即被使用
#pragma warning(error:4700 4701)
#endif

class GameMsg_Base;//GameMsg_Entity_Update;
class CEntityComponent;
struct ROLEINFO_DB;
typedef map<string,CEntityComponent*> ComponentCollection;
typedef vector<CEntityComponent*> ComponentArray;
struct BuffData;
template< class Impl ,class ProcessorObjGet ,class DBProcessorObjGet>
class CRoleComponentImpl ;

class CEntity:public IEntity
{
public:
	Entity_Type m_EntityType;

	bool m_bValid;		//这个对象是否有效，如果无效，它将要从内存中删除
protected:
	CEntity(void);	//防止建立CEntity实例

protected:
	friend class CSrvManagerBase;
	friend class CPlayerManager;
	virtual ~CEntity(void);
public:

	bool IsPlayer() const{return m_EntityType == EntityType_CRoleEntity;};
	bool IsNPC() const{return m_EntityType == EntityType_CNPCEntity;};
	bool IsItem() const{return m_EntityType == EntityType_CItemEntity;};

	virtual	const char* GetName() const {return "";};

	virtual bool IsValid() const 
	{
		return m_bValid;
	}
	void SetValid(bool b)
	{
		m_bValid = b;
	};

	virtual void EntityUpdate(const unsigned long &nTimeElapsed);

	//注意，析构代码没必要放在这里，（但在这里可以提前把不再需要的数据delete 掉）
	virtual void EntityDestroy();

	//当这个entity的状态改变，需要通知周围的player的时候，把打包好的消息放在这里
	//无论是客户端还是服务器端，都使用这个方式发消息
	//这里的消息会自动删除，所以要new msg,并且不能一个多用
	//virtual bool AddUpdateMsg(GameMsg_Base*pMsg,bool bNotifySelf,bool bNotifyOther );

	//这个方法目前只支持 CServerRoleEntity 调用，物品 itementity 暂时还没实现
	//比如创建了一个不属于任何人的物品，又需要大家知道这个物品的诞生，是需要通知需要知道这一事件的 CServerRoleEntity
	//virtual void NotifyAllPlayer();

	//void NotifyOtherPlayer(const PLAYERTAG*pTag);
	//virtual GameMsg_Entity_Create * GetCreateMsg(){return NULL;};

//组件形式嵌入代码。
public:
	void AttachComponent( CEntityComponent & pComponent );
	template< class T > T* AttachComponent( )
	{
#ifdef WIN32
		T::IsImpl();
#endif

	  T * pt = new T;
	  AttachComponent( *pt );
	  return pt;
	}

	void DestroyComponent();

private:
	ComponentCollection m_Components;
	ComponentArray m_ComponentArray;
	int m_nComponentID;

	bool m_bStart;
public:
	template< class T > T* GetComponent() const
	{
#ifdef WIN32
		T::IsImpl();
#endif
		T* pResult = NULL;
		const type_info & info_type = typeid(T);
		pResult = (T*)GetComponent(info_type);
		return pResult;
	}

	CEntityComponent* GetComponent(const type_info & info_type) const
	{
		CEntityComponent * pResult = NULL;
		ComponentCollection::const_iterator it = m_Components.find ( info_type.name() );
		if( it != m_Components.end() )
		{
			pResult = it->second;
		}
		return pResult;
	}

	CEntityComponent * GetComponent( int Index ) const
	{
		return m_ComponentArray[ Index ];
	}

//组件事件
	void SendEvent( int nID,void * pData );
	void OnLogin();
	void OnLogout();
	bool CreateFromDB(ROLEINFO_DB* pRoleInforDB);
	bool PacketToDB(ROLEINFO_DB*pRoleInforDB) const;
    bool PacketToCache(ROLEINFO_DB *pRoleCache) const;
	
	void SendExtraStaticData();
	void PacketBuff(std::list<BuffData>& listBuff);

//Entity 开始运行。
	void Start();

	void SerializeAllComponent( CParamPool &IOBuff );
	void SerializeExtraData( CParamPool& IOBuff );
};

#endif


//end file


