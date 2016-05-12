#ifndef __EXTRA_ACTIVITY_INFO_MGR_H__
#define __EXTRA_ACTIVITY_INFO_MGR_H__

class CExtraActivityInfoMgr
{
public:
	CExtraActivityInfoMgr();
	~CExtraActivityInfoMgr();

	static CExtraActivityInfoMgr & Instance();

	void SetAmuseSceneID(int nID);
	int GetAmuseSceneID();
private:
	int m_nAmuseSceneID;
};

#endif


