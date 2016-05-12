#ifndef __AMUSEEVENT_STARTCARROUSEL__H__
#define __AMUSEEVENT_STARTCARROUSEL__H__


#include "AmuseEventBase.h"
#include <vector>
#include <string>

class CAmuseRoom;


class CAmuseEvent_StartCarrousel : public CAmuseEvent_Base
{
public:
	CAmuseEvent_StartCarrousel(CAmuseRoom* pAmuseRoom, bool bLoopEvent, int nEventTime, std::vector<std::string>& vecStartSongs);
	virtual ~CAmuseEvent_StartCarrousel();

public:
	virtual CAmuseEventParam_Base* ToParam(int nNowTime);

	virtual void GoStart(int nNowTime);

public:
	CAmuseRoom* m_pOwnerRoom;

	int m_nStartTime;
	std::string m_strStartSong;
	std::vector<std::string> m_vecStartSongs;
};

#endif
//file end

