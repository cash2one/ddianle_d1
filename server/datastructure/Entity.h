/********************************************************************
created:	2010/12/01
filename: 	DataStructure\Entity.h
author:		meijiuhua

purpose:	���е�ʵ��Ļ���.
ʵ�����player,monster(����npc),portal(������),location,
С�����ħ��,����ļ���
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
//(����)��δ��ʼ������ʹ��
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

	bool m_bValid;		//��������Ƿ���Ч�������Ч������Ҫ���ڴ���ɾ��
protected:
	CEntity(void);	//��ֹ����CEntityʵ��

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

	//ע�⣬��������û��Ҫ����������������������ǰ�Ѳ�����Ҫ������delete ����
	virtual void EntityDestroy();

	//�����entity��״̬�ı䣬��Ҫ֪ͨ��Χ��player��ʱ�򣬰Ѵ���õ���Ϣ��������
	//�����ǿͻ��˻��Ƿ������ˣ���ʹ�������ʽ����Ϣ
	//�������Ϣ���Զ�ɾ��������Ҫnew msg,���Ҳ���һ������
	//virtual bool AddUpdateMsg(GameMsg_Base*pMsg,bool bNotifySelf,bool bNotifyOther );

	//�������Ŀǰֻ֧�� CServerRoleEntity ���ã���Ʒ itementity ��ʱ��ûʵ��
	//���紴����һ���������κ��˵���Ʒ������Ҫ���֪�������Ʒ�ĵ���������Ҫ֪ͨ��Ҫ֪����һ�¼��� CServerRoleEntity
	//virtual void NotifyAllPlayer();

	//void NotifyOtherPlayer(const PLAYERTAG*pTag);
	//virtual GameMsg_Entity_Create * GetCreateMsg(){return NULL;};

//�����ʽǶ����롣
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

//����¼�
	void SendEvent( int nID,void * pData );
	void OnLogin();
	void OnLogout();
	bool CreateFromDB(ROLEINFO_DB* pRoleInforDB);
	bool PacketToDB(ROLEINFO_DB*pRoleInforDB) const;
    bool PacketToCache(ROLEINFO_DB *pRoleCache) const;
	
	void SendExtraStaticData();
	void PacketBuff(std::list<BuffData>& listBuff);

//Entity ��ʼ���С�
	void Start();

	void SerializeAllComponent( CParamPool &IOBuff );
	void SerializeExtraData( CParamPool& IOBuff );
};

#endif


//end file


