#ifndef		__QUESTLISTENTRY_H__
#define		__QUESTLISTENTRY_H__
#include <string>
#include <list>
#include "../../nsytworld/QuestDoc.h"
#include "QuestTargetSend.h"

using namespace std;

class CParamPool;
class QUESTINFO;
class CQuestTargetSend;
class CRoleEntity;

class CQuestListEntry
{
public:
	CQuestListEntry();
	CQuestListEntry(QUESTINFO *pQuestInfo, CRoleEntity *pRole);
	~CQuestListEntry();
public:
	void doEncode(CParamPool &IOBuff);
public:
	unsigned short				m_nQuestID;
	unsigned char				m_nState;
	list<CQuestTargetSend>		m_listTargetInfo;
    std::string                 m_strJsonProgress;
};


#endif




// end of file


