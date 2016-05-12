#include "RelationInfo.h"

#include <algorithm>


CFriendRelationInfoManager::CFriendRelationInfoManager()
{
}

CFriendRelationInfoManager::~CFriendRelationInfoManager()
{
}

CFriendRelationInfoManager &CFriendRelationInfoManager:: Instance()
{
	static CFriendRelationInfoManager relationManager;
	return relationManager;
}

void CFriendRelationInfoManager::AddFriendRelation(int nRelation, int nLeft, int nRight)
{
	if (nRelation > 0 ) {
		_InsertRelationInfo(nRelation, nLeft, nRight);
		_InsertRelationInfo(nRelation, nRight, nLeft);
	}
}

void CFriendRelationInfoManager::AddFriendRelationNameDefInfo(int nNameDef, std::string strName , int nReqGender)
{
	map< int, CCalling >::iterator itr = m_mapRelationNameDef.find(nNameDef);
	if (itr == m_mapRelationNameDef.end()) {
		m_mapRelationNameDef.insert(make_pair(nNameDef, CCalling(nNameDef, strName, nReqGender) ));	
	} 
}

bool CFriendRelationInfoManager::IsCallingValid(int nNameDef, int nGender)
{
	map< int, CCalling >::iterator itr = m_mapRelationNameDef.find(nNameDef);
	if (itr != m_mapRelationNameDef.end()) {
		return (itr->second.m_nReqGender == 0 || itr->second.m_nReqGender == nGender ); 	
	}

	return false;
}

bool CFriendRelationInfoManager::RelationExists(int nLeftCallCode, int nRightCallCode, int nLeftGender, int nRightGender)
{
	if (nLeftGender == ESexType_No || nRightGender == ESexType_No) {
		return false;
	}
	if (!IsCallingValid(nLeftCallCode, nLeftGender) || !IsCallingValid(nRightCallCode, nRightGender)) {
		return false;
	}

	map<int, vector<CFriendRelation> >::const_iterator itr = m_mapRelation.find(nLeftCallCode);
	CFriendRelation relationInfo(0,nLeftCallCode, nRightCallCode);
	vector<CFriendRelation>::const_iterator vecItr = find(itr->second.begin(), itr->second.end(), relationInfo);
	if (vecItr != itr->second.end()) {
			return true;
	}

	return false;
}

void CFriendRelationInfoManager::_InsertRelationInfo(int nRelation, int nLeft, int nRight)
{
	if (nRelation > 0 ) {
		CFriendRelation relationInfo(nRelation, nLeft, nRight);
		map<int, vector<CFriendRelation> >::iterator itr = m_mapRelation.find(nLeft);

		if (itr != m_mapRelation.end()) {	// existed
			if (find(itr->second.begin(), itr->second.end(), relationInfo) == itr->second.end()) {
				itr->second.push_back(relationInfo);
			}
		} else {	// not existed
			vector<CFriendRelation> vecFriendRelationInfo;	
			vecFriendRelationInfo.push_back(relationInfo);
			m_mapRelation.insert(make_pair(nLeft, vecFriendRelationInfo));
		} 
	}
}

int CFriendRelationInfoManager::GetRelation( int nLeftNameDef, int nRightNameDef )
{
	map< int, vector<CFriendRelation> >::iterator itr =  m_mapRelation.find(nLeftNameDef);
	if (itr != m_mapRelation.end()) {
		CFriendRelation relationInfo(0, nLeftNameDef, nRightNameDef);
		vector<CFriendRelation>::iterator vecItr = find(itr->second.begin(), itr->second.end(), relationInfo);
		if (vecItr != itr->second.end()) {
			return vecItr->m_nRelation;
		}
	}

	return 0;
}

void CFriendRelationInfoManager::GetCallingName(int nNameDef, std::string & strName)
{
	map< int, CCalling >::iterator itr = m_mapRelationNameDef.find(nNameDef);
	if (itr != m_mapRelationNameDef.end()) {
		strName = itr->second.m_strName;
	}
}











