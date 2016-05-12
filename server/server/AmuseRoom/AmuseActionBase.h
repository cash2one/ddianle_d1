#ifndef __AMUSEACTION_BASE__H__
#define __AMUSEACTION_BASE__H__

#include <list>
#include <stdlib.h>

#ifdef WIN32
#pragma warning(disable:4100)
#endif

class CAmuseRoomComponent;
class CAmuseActionParam_Base;


class CAmuseAction_Base
{
public:
	CAmuseAction_Base(CAmuseRoomComponent* pAmusePlayer, unsigned int nActionID, int nActionType, bool bUnique, bool bSync)
	:m_pActPlayer(pAmusePlayer)
	,m_nActionID(nActionID)
	,m_nActionType(nActionType)
	,m_bUnique(bUnique)
	,m_bSync(bSync)
	,m_bSuccess(false)
	{
	}

	virtual ~CAmuseAction_Base()
	{
	}

	virtual bool FormParam(CAmuseActionParam_Base* pActionParam) = 0;
	virtual CAmuseActionParam_Base* ToParam(int nNowTime) { return NULL; }

	virtual bool GoProcess(std::list<CAmuseAction_Base*>& rNewAction) = 0;	// return false if the action is useless
	virtual void GoTerminate() {}

public:
	CAmuseRoomComponent* ActPlayer() const
	{
		return m_pActPlayer;
	}

	unsigned int ActionID() const
	{
		return m_nActionID;
	}

	int ActionType() const
	{
		return m_nActionType;
	}

	bool IsUnique() const
	{
		return m_bUnique;
	}

	bool NeedSync() const
	{
		return m_bSync;
	}

	bool ProcessSuc() const
	{
		return m_bSuccess;
	}

protected:
	CAmuseRoomComponent* m_pActPlayer;

	unsigned int m_nActionID;
	int m_nActionType;
	bool m_bUnique;			//whether only one action for the same type
	bool m_bSync;			//whether need sync to other player
	bool m_bSuccess;		//whether processed success
};

#endif
//file end

