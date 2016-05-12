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
	// ������Ϊ
	void LoadDivorceAction( std::list<CDivorceAction> & actionlist );

	// �����Ϊ
	void AddDivorceAction( const CDivorceAction & action, bool bIsSave = true, bool bMaleSendMsg = true, bool bFemaleSendMsg = true );

	// ����Э������Ϣ
	void UpdateDivorceAgreementInfo( const CDivorceAgreementInfo & info ){ m_DivorceAgreementInfo = info;}
	CDivorceAgreementInfo & GetDivorceAgreementInfo(){ return m_DivorceAgreementInfo; }

	// ����/����Э����״̬
	void UpdateDivorceAgreementState( eDivorceState state );
	eDivorceState GetAgreementState(){ return m_DivorceAgreementInfo.m_eState; }

	// ����
	void Reset();

	// ��ȡ/����Э����ID
	unsigned int GetAgreementID(){ return m_DivorceAgreementInfo.GetAgreementID(); }
	void SetAgreementID( unsigned int nAgreementID ){ m_DivorceAgreementInfo.SetAgreementID( nAgreementID );}

	// ��ȡ��/Ů��ɫID
	unsigned int GetMaleRoleID(){ return m_DivorceAgreementInfo.m_nMaleRoleID; }
	unsigned int GetFemaleRoleID(){ return m_DivorceAgreementInfo.m_nFemaleRoleID; }

	// ����/��ȡ�з��Ƿ���ӡ
	void SetMaleIsAgree( bool bIsAgree ){ m_DivorceAgreementInfo.m_bMaleIsAgree = bIsAgree; }
	bool GetMaleIsAgree(){ return m_DivorceAgreementInfo.m_bMaleIsAgree; }

	// ����/��ȡŮ���Ƿ���ӡ
	void SetFemaleIsAgree( bool bIsAgree ){ m_DivorceAgreementInfo.m_bFemaleIsAgree = bIsAgree; }
	bool GetFemaleIsAgree(){ return m_DivorceAgreementInfo.m_bFemaleIsAgree; }
	
	// ��ȡ/���÷�����ID
	unsigned int GetProposerID(){ return m_DivorceAgreementInfo.m_nProposerID; }
	void SetProposerID( unsigned int nRoleID ){ m_DivorceAgreementInfo.m_nProposerID = nRoleID; }

public:
	CDivorceAgreementInfo			m_DivorceAgreementInfo;  // Э���� 
	std::list<CDivorceAction>		m_DivorceAction;	 //	�����Ϊ
	std::deque<CDivorceActionIter>	m_DivorceChatIndex;  // �����������Ϊ�����index(��������)

};



#endif
