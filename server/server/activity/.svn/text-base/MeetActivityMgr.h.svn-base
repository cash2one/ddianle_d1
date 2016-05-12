#ifndef		__MEETACTIVITYMGR_H__
#define		__MEETACTIVITYMGR_H__

#include "MeetActivity.h"

class CMeetConfig
{
public:
	CMeetConfig(){};
	virtual ~CMeetConfig(){};

public:
	unsigned int        m_nIndex;
	std::string         m_strIndexName;
	std::list<CItem>	m_listMaleReward;
	std::list<CItem>	m_listFemaleReward;
	std::list<CItem>    m_listCostItem;
};


class CMeetActivityMgr
{
public:
	CMeetActivityMgr();
	virtual ~CMeetActivityMgr();

	static CMeetActivityMgr & Instance();

public:
	void Init();
	void AddRewardInfo( unsigned int nIndex, CMeetConfig & config );
	void AddConfig( unsigned int nPhotoIndexCount, unsigned int nWaitTime, int nAmuseSceneID, const std::string & strTitle, const std::string & strContent );
	void Logout( unsigned int nRoleID, unsigned int nSex );
	void GetCostItemList( unsigned int nIndex, std::list<CItem> & items );
	const std::list<CItem> * GetCostItemList( unsigned int nIndex );
	itemtype_t GetCostItemID();
	void LeaveAmuseScene( unsigned int nRoleID );

public:
	void OnMeetMatch( unsigned int nRoleID, unsigned int nSex, unsigned int nCoupleID );
	void OnMeetCancel( unsigned int nRoleID, unsigned int nSex );
	void OnMeetLeaveScene( unsigned int nRoleID , unsigned int nSex );
	int  OnMeetCameraEvent( unsigned int nRoleID, unsigned int nIndex );
	void OnMeetEnd( unsigned int nRoleID, unsigned int nSex );
	void OnMeetEnterAmuseScene( unsigned int nRoleID );

private:
    void SendMatchFailed( unsigned int nErr, unsigned int nRoleID );
	void SendMatchSucceed( unsigned int nSelfRoleID, unsigned int nOppositeID );
	void SendRewardOutReward( unsigned int nRoleID, unsigned int nSex );

public:
	CMeetActivity *                         m_pMeetActivity;
	unsigned int                            m_nPhotoCount;
	unsigned int                            m_nWaitTime;
	int                                     m_nAmuseSceneID;
	std::string                             m_strMailTitle;
	std::string                             m_strMailContent;
	std::map<unsigned int,CMeetConfig>      m_MeetConfig;
    
};



#endif
