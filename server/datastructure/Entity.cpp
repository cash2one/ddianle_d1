#include "StdAfx.h"
#include "Entity.h"
#include "../server/logic/EntityComponent.h"
#include "../server/music/MusicBoardMgr.h"
#include "../server/music/BGMMgr.h"
#include "../server/SystemSetting/SystemSettingMgr.h"
#include "DataStruct_DB.h"
#include "../server/mall/Mall.h"
#include "../server/mall/MallDataMgr.h"
#include "../server/activity/AllActivitiesConfigMgr.h"

extern string g_sPath;

CEntity::CEntity(void)
{
	m_bValid = true;

	m_EntityType = EntityType_CEntity;

	m_nComponentID = 0;
	m_bStart = false;
}

CEntity::~CEntity(void)
{
	DestroyComponent();
}

void CEntity::EntityDestroy()
{
	m_bValid = false;
}

//void CEntity::NotifyOtherPlayer(const PLAYERTAG*pTag)
//{
//	//GameMsg_Entity_Create * pMsg = GetCreateMsg();
//	//if(pMsg)
//	//{
//	//	SendGameMsg(pMsg, pTag);
//	//	delete pMsg;
//	//}
//}

//void CEntity::NotifyAllPlayer()
//{
//	//这个方法目前只支持 CServerRoleEntity 调用，物品 itementity 暂时还没实现
//	//比如创建了一个不属于任何人的物品，又需要大家知道这个物品的诞生，是需要通知需要知道这一事件的 CServerRoleEntity
//};


//struct PLAYERTAG;
//int SendGameMsg (LPGameMsg_Base pMsg, const PLAYERTAG* pTag);


void CEntity::EntityUpdate(const unsigned long &nTimeElapsed)
{
	ComponentCollection::iterator it = m_Components.begin();
	ComponentCollection::iterator itend = m_Components.end();
	while(it != itend)
	{
		it->second->OnUpdate(nTimeElapsed);
		++it; 
	}
}

void CEntity::AttachComponent( CEntityComponent & pComponent )
{
	const type_info &info_type = typeid(pComponent);
	if(m_Components.find( info_type.name() ) == m_Components.end())
	{
		m_Components[ info_type.name() ] = &pComponent;
		m_ComponentArray.push_back(&pComponent);
		pComponent.SetIndexInEntity( m_ComponentArray.size()-1 );
		pComponent.AttachToEntity(this);

		if( m_bStart )
		{
			pComponent.Start();
		}
	}
}

void CEntity::DestroyComponent()
{
	ComponentCollection::iterator it = m_Components.begin();
	ComponentCollection::iterator itend = m_Components.end();
	while(it != itend)
	{
		delete it->second;
		it->second = NULL;
		++it; 
	}
}
//组件事件
void CEntity::SendEvent( int nID,void * pData )
{
	CComponentEvent e;
	e.nID = nID;
	e.m_pData = pData;
	ComponentCollection::iterator it = m_Components.begin();
	ComponentCollection::iterator itend = m_Components.end();
	while(it != itend)
	{
		it->second->OnEvent(e);
		++it; 
	}
}
void CEntity::OnLogin()
{
	ComponentCollection::iterator it = m_Components.begin();
	ComponentCollection::iterator itend = m_Components.end();
	while(it != itend)
	{
		it->second->OnLogin();
		++it; 
	}
}
void CEntity::OnLogout()
{
	ComponentCollection::iterator it = m_Components.begin();
	ComponentCollection::iterator itend = m_Components.end();
	while(it != itend)
	{
		it->second->OnLogout();
		++it; 
	}
}

void CEntity::Start()
{
	m_bStart = true;

	ComponentCollection::iterator it = m_Components.begin();
	ComponentCollection::iterator itend = m_Components.end();
	while(it != itend)
	{
		it->second->Start();
		++it; 
	}

}

void CEntity::SerializeAllComponent( CParamPool &IOBuff )
{
    ComponentCollection::iterator it = m_Components.begin();
    ComponentCollection::iterator itend = m_Components.end();

    IOBuff.AddUShort((unsigned short)m_Components.size());
    for (; it != itend; it++)
    {
        int nOldPos = IOBuff.GetCurPosition();
        IOBuff.AddUShort(0);

        it->second->SerializeComponent(IOBuff);

        if (IOBuff.GetCurPosition() - nOldPos - sizeof(unsigned short) != 0)
        {
            int nFinalPos = IOBuff.GetCurPosition();
            IOBuff.SetCurPosition(nOldPos);
            IOBuff.ReplaceShort((short)(nFinalPos - nOldPos - sizeof(unsigned short)));
            IOBuff.SetCurPosition(nFinalPos);
            IOBuff.AddStringW(it->second->SerializeName());
        }
    }
}

void CEntity::SerializeExtraData( CParamPool& IOBuff )
{
	// music board info
	int nOldPos = IOBuff.GetCurPosition();
	IOBuff.AddUShort( 0 );

	CMusicBoardManager::Instance().SerializeBoardInfo( IOBuff );
	if( IOBuff.GetCurPosition() - nOldPos - sizeof(unsigned short) != 0 )
	{
		int nFinalPos = IOBuff.GetCurPosition();
		IOBuff.SetCurPosition( nOldPos );
		IOBuff.ReplaceShort( (short)(nFinalPos - nOldPos - sizeof(unsigned short)) );
		IOBuff.SetCurPosition( nFinalPos );
		IOBuff.AddStringW( CMusicBoardManager::Instance().SerializeName() );
	}

	nOldPos = IOBuff.GetCurPosition();
	IOBuff.AddUShort(0);

	CBGMMgr::instance().SerializeBGMInfo(IOBuff);
	if( IOBuff.GetCurPosition() - nOldPos - sizeof(unsigned short) != 0 )
	{
		int nFinalPos = IOBuff.GetCurPosition();
		IOBuff.SetCurPosition( nOldPos );
		IOBuff.ReplaceShort( (short)(nFinalPos - nOldPos - sizeof(unsigned short)) );
		IOBuff.SetCurPosition( nFinalPos );
		IOBuff.AddStringW( CBGMMgr::instance().SerializeName() );
	}

	nOldPos = IOBuff.GetCurPosition();
	IOBuff.AddUShort(0);
	CSystemSettingMgr::Instance().SerializeSystemSettingInfo(IOBuff);
	if( IOBuff.GetCurPosition() - nOldPos - sizeof(unsigned short) != 0 )
	{
		int nFinalPos = IOBuff.GetCurPosition();
		IOBuff.SetCurPosition( nOldPos );
		IOBuff.ReplaceShort( (short)(nFinalPos - nOldPos - sizeof(unsigned short)) );
		IOBuff.SetCurPosition( nFinalPos );
		IOBuff.AddStringW( CSystemSettingMgr::Instance().SerializeName() );
	}

	nOldPos = IOBuff.GetCurPosition();
	IOBuff.AddUShort(0);
	CMallDataMgr::Instance().SerializeRecommendEquipments(IOBuff);
	if (IOBuff.GetCurPosition() - nOldPos - sizeof(unsigned short) != 0) {
		int nFinalPos = IOBuff.GetCurPosition();
		IOBuff.SetCurPosition( nOldPos );
		IOBuff.ReplaceShort( (short)(nFinalPos - nOldPos - sizeof(unsigned short)) );
		IOBuff.SetCurPosition( nFinalPos );
		IOBuff.AddStringW( CMallDataMgr::Instance().SerializeName() );
	}

	nOldPos = IOBuff.GetCurPosition();
	IOBuff.AddUShort(0);
	CAllActivitiesConfigMgr::Instance().Serialize(IOBuff);
	if (IOBuff.GetCurPosition() - nOldPos - sizeof(unsigned short) != 0) {
		int nFinalPos = IOBuff.GetCurPosition();
		IOBuff.SetCurPosition( nOldPos );
		IOBuff.ReplaceShort( (short)(nFinalPos - nOldPos - sizeof(unsigned short)) );
		IOBuff.SetCurPosition( nFinalPos );
		IOBuff.AddStringW( CAllActivitiesConfigMgr::Instance().SerializeName() );
	}
}

bool CEntity::CreateFromDB(ROLEINFO_DB* pRoleInforDB)
{
	bool bRes = false;
	if (pRoleInforDB != NULL)
	{
		ComponentArray::iterator itr = m_ComponentArray.begin();
		ComponentArray::iterator itrEnd = m_ComponentArray.end();

		for (; itr != itrEnd; itr++)
		{
			bRes = (*itr)->CreateFromDB(pRoleInforDB);
			if (!bRes)
			{
				break;
			}
		}
	}

	return bRes;
}

//把数据打包到ROLEINFO_DB中，用于存储数据库或者网络通信
bool CEntity::PacketToDB(ROLEINFO_DB *pRoleInforDB) const
{
	bool bRes = false;

	if (pRoleInforDB != NULL) {
		ComponentArray::const_iterator itr = m_ComponentArray.begin();
		ComponentArray::const_iterator itrEnd = m_ComponentArray.end();

		for (; itr != itrEnd; itr++)
		{
			bRes = (*itr)->PacketToDB(pRoleInforDB);
			if (!bRes)
			{
				break;
			}
		}
	}

	return bRes;
}

bool CEntity::PacketToCache(ROLEINFO_DB *pRoleCache) const
{
    bool bRes = false;

    if (pRoleCache != NULL) {
        ComponentArray::const_iterator itr = m_ComponentArray.begin();
        ComponentArray::const_iterator itrEnd = m_ComponentArray.end();

        for (; itr != itrEnd; itr++)
        {
            bRes = (*itr)->PacketToCache(pRoleCache);
            ENSURE_WITH_LOG_CMD(bRes, break, "component[%s] packet to cache error...", (*itr)->SerializeName());
        }
    }

    return bRes;
}


void CEntity::SendExtraStaticData()
{
	ComponentCollection::iterator it = m_Components.begin();
	ComponentCollection::iterator itend = m_Components.end();
	while(it != itend)
	{
		it->second->SendExtraStaticData();
		++it; 
	}
}

void CEntity::PacketBuff( std::list<BuffData>& listBuff )
{
	ComponentCollection::iterator it = m_Components.begin();
	ComponentCollection::iterator itend = m_Components.end();
	while(it != itend)
	{
		it->second->PacketBuff(listBuff);
		++it; 
	}
}




//end file


