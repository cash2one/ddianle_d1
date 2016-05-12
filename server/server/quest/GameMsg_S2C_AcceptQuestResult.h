#ifndef		__GAMEMSG_S2C_ACCEPTQUESTRESULT_H__
#define		__GAMEMSG_S2C_ACCEPTQUESTRESULT_H__ 

#include <list>
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/RoleEntity.h"
#include "QuestTargetSend.h"
#include "QuestListEntry.h"

using namespace std;

class CParamPool;
class CRoleEntity;

class GameMsg_S2C_AcceptQuestResult:
	public GameMsg_Base
{
public:
	GameMsg_S2C_AcceptQuestResult();
	GameMsg_S2C_AcceptQuestResult(unsigned short nQuestID, unsigned char nResult, unsigned char nState, QUESTINFO &questInfo, CRoleEntity &role);
	~GameMsg_S2C_AcceptQuestResult();
public:
	bool doEncode(CParamPool &IOBuff);
	void SetListEntry(CQuestListEntry questEntry);
public:
	unsigned short			m_nQuestID;			// ����ID
	unsigned char			m_nResult;			// 0��ʾ�ɹ���1 ��ʾʧ��(ʧ��ͨ��syschat������Ӧ��Ϣ���ݲ����͸���Ϣ)
	unsigned char			m_nState;			// 
	list<CQuestTargetSend>	m_listTargetInfo;	// ���µ���Ϣ	
    std::string             m_strJsonProgress;
};

#endif
// end of file


