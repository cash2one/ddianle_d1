#ifndef __SECURITY_COMPONENT_H_H__
#define __SECURITY_COMPONENT_H_H__ 1

#include "../logic/EntityComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../../datastructure/DataStruct_DB.h"

#include "SecurityQuiz.h"

class CSecurityComponent :
	public CEntityComponent
{
public:
	CSecurityComponent();
	~CSecurityComponent();

public:
	void Start();
	
	void SerializeComponent(CParamPool &IOBuff);
	const char* SerializeName() { return "Security"; }

	void RegComponentNetMsgMap();

public:
	void OnSetSecurityQuiz(GameMsg_Base &msg, CSlotPeer &slotPeer);

protected:
	void SerializeSecurityQuiz(CParamPool &IOBuff);

protected:
	CEntityAttributeComponent   * m_pAttr;
	CEntityNetComponent         * m_pNet;

protected:
	SecurityQuizList              m_securityQuiz;
};

typedef CRoleComponentImpl<CSecurityComponent,CGetRoleProcessorObj,CGetRoleProcessorObj> CRoleSecurity;

#endif //__SECURITY_COMPONENT_H_H__
