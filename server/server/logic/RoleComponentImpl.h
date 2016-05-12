/*
*2011-11-9
*by wangshijun
*把entity组件化。
*/
#ifndef __ENTITY_COMPONENTIMPL_H__
#define __ENTITY_COMPONENTIMPL_H__

#include "GetRoleProcessorObj.h"
#include "../libServerFrame/QueryManager.h"
#include "../logic/PlayerManager.h"
#include "../../datastructure/IEntity.h"

#ifdef WIN32
#pragma warning(disable:4625 4626 4512)
#endif


class IProcessorObjGet;
extern CQueryManager*	g_pQueryMgr;
template< class Impl ,class ProcessorObjGet ,class DBProcessorObjGet>
class CRoleComponentImpl :
	public Impl
{
public:
#ifdef WIN32
	static  bool IsImpl(){return true;};
#endif
	CRoleComponentImpl<Impl,ProcessorObjGet,DBProcessorObjGet>(void)
	{

	};

	~CRoleComponentImpl<Impl,ProcessorObjGet,DBProcessorObjGet>(void)
	{
	};	

	virtual void Start()
	{
		Impl::Start();
		if( !s_bRegisterMsgMap )
		{
			Impl::RegComponentNetMsgMap();
			Impl::ClearProcessorObjGet();
			Impl::ClearQueryProcessorObjGet();
			s_bRegisterMsgMap  = true;
		}
	}

	IProcessorObjGet * CreateIProcessorObjGet()
	{
		ProcessorObjGet * pObj = new ProcessorObjGet;
		if( pObj )
		{
			pObj->m_nIndex = Impl::GetIndexInEntity();
		}
		return pObj;
	}

	IQueryProcessorObjGet * CreateIQueryProcessorObjGet()
	{
		DBProcessorObjGet * pObj = new DBProcessorObjGet;
		if( pObj )
		{
			pObj->m_nIndex = Impl::GetIndexInEntity();
		}
		return pObj;
	}
	//参数内存有通过new产生的情况则需要通过pDelHelper辅助释放（特别是回调方法没有调用到的情况），以防泄露
	virtual void AddQuery(int nQueryType, int nAttachment = 0, void* pAttachment = NULL, __int64 nTag = 0, void* pTag = NULL, CDelHelper* pDelHelper = NULL)
	{
		if( NULL != g_pQueryMgr )
		{
			QueryBase *pQuery = new QueryBase(nQueryType, nAttachment, pAttachment, nTag, pTag, pDelHelper);

			IEntity *pEntity = (IEntity *) Impl::Entity();
			if (pEntity != NULL)
			{
				pQuery->m_nSessionID = pEntity->GetSessionID();
				pQuery->m_nEntityIndex = pEntity->GetIndex();
				pQuery->m_nRoleID = pEntity->GetEntityID();
				g_pQueryMgr->AddQuery(pQuery);
			}
		}
	}
public :
	IProcessorObjGet *m_pProcessorObjGet;
	static bool s_bRegisterMsgMap ;
};

template< class Impl,class ProcessorObjGet ,class DBProcessorObjGet>
bool CRoleComponentImpl<Impl,ProcessorObjGet,DBProcessorObjGet>::s_bRegisterMsgMap  = false;

#endif



//end file


