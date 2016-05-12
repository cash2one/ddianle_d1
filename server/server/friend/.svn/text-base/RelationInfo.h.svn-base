#ifndef		__RELATIONINFO_H__
#define		__RELATIONINFO_H__

#include "../../datastructure/DataStruct_Base.h"
#include <map>

using namespace std;

#define REQGENDER_ANY			0	// 0 表示任意性别
#define REQGENDER_OPPOSITE		1	// 1 表示异性
#define REQGENDER_SAMEMALE		2	// 2 表示男男
#define REQGENDER_SAMEFEMALE	3	// 3 表示女女

class CFriendRelationInfoManager
{
protected:
	CFriendRelationInfoManager();
	~CFriendRelationInfoManager();
public:
	static CFriendRelationInfoManager & Instance();
	void AddFriendRelation(int nRelation, int nLeft, int nRight);
	void AddFriendRelationNameDefInfo(int nNameDef, std::string strName , int nReqGender);
	bool IsCallingValid(int nCallCode, int nGender);
	bool RelationExists(int nLeftCallCode, int nCallRightCode, int nLeftGender, int nRightGender);
	int GetRelation(int nLeftNameDef, int nRightNameDef);
	void GetCallingName(int nNameDef, std::string & strName);
private:
	map< int, CCalling > m_mapRelationNameDef;			// namedef, Ccalling
	map< int, vector<CFriendRelation> > m_mapRelation;	// namedef, vector<FriendRelation>
private:
	void _InsertRelationInfo(int nRelation, int nLeftCode, int nRightCode);
};




#endif


