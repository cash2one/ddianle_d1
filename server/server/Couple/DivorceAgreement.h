#ifndef		__DIVORCEAGREEMENT_H__
#define		__DIVORCEAGREEMENT_H__

#include <map>
#include <string>
#include <list>
#include <deque>
#include "../../datastructure/DataStruct_Base.h"
#include "../../datastructure/DataStruct_Couple.h"


#define MAX_CHAT_RECORD_COUNT 20

class CDivorceAgreement
{
public:


public:
	// 加载行为
	void LoadDivorceAction( std::list<CDivorceAction> & actionlist );

	// 添加行为
	void AddDivorceAction( const CDivorceAction & action, bool bIsSave = true, bool bMaleSendMsg = true, bool bFemaleSendMsg = true );

	// 加载协议书信息
	void UpdateDivorceAgreementInfo( const CDivorceAgreementInfo & info ){ m_DivorceAgreementInfo = info;}
	CDivorceAgreementInfo & GetDivorceAgreementInfo(){ return m_DivorceAgreementInfo; }

	// 更新/设置协议书状态
	void UpdateDivorceAgreementState( eDivorceState state );
	eDivorceState GetAgreementState(){ return m_DivorceAgreementInfo.m_eState; }

	// 重置
	void Reset();

	// 获取/设置协议书ID
	unsigned int GetAgreementID(){ return m_DivorceAgreementInfo.GetAgreementID(); }
	void SetAgreementID( unsigned int nAgreementID ){ m_DivorceAgreementInfo.SetAgreementID( nAgreementID );}

	// 获取男/女角色ID
	unsigned int GetMaleRoleID(){ return m_DivorceAgreementInfo.m_nMaleRoleID; }
	unsigned int GetFemaleRoleID(){ return m_DivorceAgreementInfo.m_nFemaleRoleID; }

	// 设置/获取男方是否按手印
	void SetMaleIsAgree( bool bIsAgree ){ m_DivorceAgreementInfo.m_bMaleIsAgree = bIsAgree; }
	bool GetMaleIsAgree(){ return m_DivorceAgreementInfo.m_bMaleIsAgree; }

	// 设置/获取女方是否按手印
	void SetFemaleIsAgree( bool bIsAgree ){ m_DivorceAgreementInfo.m_bFemaleIsAgree = bIsAgree; }
	bool GetFemaleIsAgree(){ return m_DivorceAgreementInfo.m_bFemaleIsAgree; }
	
	// 获取/设置发起人ID
	unsigned int GetProposerID(){ return m_DivorceAgreementInfo.m_nProposerID; }
	void SetProposerID( unsigned int nRoleID ){ m_DivorceAgreementInfo.m_nProposerID = nRoleID; }

public:
	CDivorceAgreementInfo			m_DivorceAgreementInfo;  // 协议书 
	std::list<CDivorceAction>		m_DivorceAction;	 //	离婚行为
	std::deque<CDivorceActionIter>	m_DivorceChatIndex;  // 离婚聊天在行为里面的index(包含送礼)

};



#endif
