#ifndef __SPECIALQUESTMGR_H__
#define __SPECIALQUESTMGR_H__

#include <map>


class CPrivateQuest
{
public:
	CPrivateQuest();
	~CPrivateQuest();
public:
	unsigned short m_nQuestID;
};

class CSpecailQuestMgr 
{
private:
	CSpecailQuestMgr();
	~CSpecailQuestMgr();
public:
	static CSpecailQuestMgr & Instance();
public:
	void AddPrivateQuest(const CPrivateQuest & prviateQuest);

	const std::map<unsigned short, CPrivateQuest> & PrivateQuestMap() const;
public:
	std::map<unsigned short, CPrivateQuest> m_mapPrivateQuest;
};








#endif


