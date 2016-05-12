#include "LBS.h"
#include <algorithm>
#include <iterator>
#include <cmath>

#define MAX_LAT_DIFF 111700.0f//纬度变化一度的距离跨度，单位米
#define PI 3.14159265f

CLBS::CLBS()
{

}

CLBS::~CLBS()
{

}

CLBS& CLBS::Instance()
{
	static CLBS lbs;
	return lbs;
}

void CLBS::AddPos( const LBSPOS& pos, unsigned int nRoleID, ESexType sex )
{
	m_mapRole[sex][nRoleID] = pos;
	m_mapXPos[sex][pos.x].insert(nRoleID);
	m_mapYPos[sex][pos.y].insert(nRoleID);

	m_mapRole[ESexType_No][nRoleID] = pos;
	m_mapXPos[ESexType_No][pos.x].insert(nRoleID);
	m_mapYPos[ESexType_No][pos.y].insert(nRoleID);
}

void CLBS::RemovePos( unsigned int nRoleID, ESexType sex )
{
	Role2PosMap::iterator it = m_mapRole[sex].find(nRoleID);
	if ( it != m_mapRole[sex].end() )
	{
		float x = it->second.x;
		float y = it->second.y;

		m_mapXPos[sex][x].erase(nRoleID);
		m_mapYPos[sex][y].erase(nRoleID);
		m_mapRole[sex].erase(it);

		m_mapXPos[ESexType_No][x].erase(nRoleID);
		m_mapYPos[ESexType_No][y].erase(nRoleID);
		m_mapRole[ESexType_No].erase(nRoleID);
	}
}

bool CLBS::FindPos( unsigned int nRoleID, LBSPOS& pos )
{
	Role2PosMap::iterator it = m_mapRole[ESexType_No].find(nRoleID);
	if ( it != m_mapRole[ESexType_No].end() )
	{
		pos = it->second;
		return true;
	}
	return false;
}

size_t CLBS::GetPosRoundRole( unsigned int nRoleID, float fRadiusMetre, size_t nCount, Pos2RoleMap& mapPos, bool bOrderByDist, ESexType sex, const set<unsigned int>& setFilter )
{
	LBSPOS posCenter;
	if ( !FindPos(nRoleID, posCenter) )
	{
		return 0;
	}
	size_t nSum = 0;
	float fLatRadius = 0.0, fLitRadius = 0.0;
	set<unsigned int> setRolesX;
	set<unsigned int> setRolesY;
	set<unsigned int> setRoles;
	_CalcRadius(posCenter, fRadiusMetre, fLatRadius, fLitRadius);
	//纬度无边界处理，[-90, 90]
	_GetPosRoundRole(nRoleID, posCenter.y, fLatRadius, m_mapYPos[ESexType_No], setRolesY, sex, true, true);
	//经度有边界处理，[-180, 180]，为环状范围
	float fMinDiff = posCenter.x - fLitRadius;
	if ( fMinDiff < -180.0f )
	{
		_GetPosFromBeginOrEnd(true, posCenter.x, m_mapXPos[ESexType_No], sex, setRolesX);
		_GetPosFromBeginOrEnd(false, 360.0f + fMinDiff, m_mapXPos[ESexType_No], sex, setRolesX);
	}
	else
	{
		_GetPosRoundRole(nRoleID, posCenter.x, fLitRadius, m_mapXPos[ESexType_No], setRolesX, sex, false, true);
	}
	float fMaxDiff = posCenter.x + fLitRadius;
	if ( fMaxDiff > 180.0f )
	{
		_GetPosFromBeginOrEnd(false, posCenter.x, m_mapXPos[ESexType_No], sex, setRolesX);
		_GetPosFromBeginOrEnd(true, fMaxDiff - 360.0f, m_mapXPos[ESexType_No], sex, setRolesX);
	}
	else
	{
		_GetPosRoundRole(nRoleID, posCenter.x, fLitRadius, m_mapXPos[ESexType_No], setRolesX, sex, true, false);
	}
	set_intersection(setRolesX.begin(), setRolesX.end(), setRolesY.begin(), setRolesY.end(), inserter(setRoles, setRoles.end()));
	if ( !setFilter.empty() )
	{
		for (set<unsigned int>::const_iterator it = setFilter.begin(); it != setFilter.end(); ++it)
		{
			set<unsigned int>::iterator itr = setRoles.find(*it);
			if ( itr != setRoles.end() )
			{
				setRoles.erase(itr);
			}
		}
	}
	for (set<unsigned int>::iterator it = setRoles.begin(); it != setRoles.end(); ++it)
	{
		Role2PosMap::iterator rit = m_mapRole[sex].find(*it);
		if ( rit != m_mapRole[sex].end() )
		{
			float fDist = CalcDist(posCenter, rit->second);
			if ( fDist < fRadiusMetre )
			{
				mapPos[fDist].insert(*it);
				++nSum;
				if ( !bOrderByDist && nCount > 0 && nSum == nCount )
				{
					break;
				}
			}
		}
	}
	if ( bOrderByDist && nCount > 0 && nSum > nCount )
	{
		nSum = nCount;
		_CutMapToCount(mapPos, nCount);
	}
	return nSum;
}

size_t CLBS::_GetPosRoundRole( unsigned int nRoleID, float pos, float radius, const Pos2RoleMap& mapPos, set<unsigned int>& setRoles, ESexType sex, bool bForward, bool bBackward )
{
	Pos2RoleMap::const_iterator it = mapPos.find(pos);
	if ( it == mapPos.end() )
	{
		return 0;
	}
	size_t nSum = 0;
	setRoles.insert(it->second.begin(), it->second.end());
	setRoles.erase(nRoleID);
	nSum += it->second.size()-1;
	Pos2RoleMap::const_iterator it_forward = it;
	Pos2RoleMap::const_iterator it_backward = it;
	float fMin = pos;
	float fMax = pos;
	if ( bBackward )
	{
		fMin = pos - radius;
	}
	if ( bForward )
	{
		fMax = pos + radius;
		++it_forward;
	}
	while(bForward && it_forward != mapPos.end())
	{
		if ( fMin <= it_forward->first && it_forward->first <= fMax )
		{
			setRoles.insert(it_forward->second.begin(), it_forward->second.end());
			nSum += it_forward->second.size();
		}
		else
		{
			break;
		}
		++it_forward;
	}
	while(bBackward && it_backward != mapPos.begin())
	{
		--it_backward;
		if ( fMin <= it_backward->first && it_backward->first <= fMax )
		{
			setRoles.insert(it_backward->second.begin(), it_backward->second.end());
			nSum += it_backward->second.size();
		}
		else
		{
			break;
		}
	}
	//性别过滤
	if ( sex != ESexType_No )
	{
		for (set<unsigned int>::iterator it = setRoles.begin(); it != setRoles.end(); )
		{
			if ( m_mapRole[sex].find(*it) == m_mapRole[sex].end() )
			{
				setRoles.erase(it++);
				--nSum;
			}
			else
			{
				++it;
			}
		}
	}
	return nSum;
}

void CLBS::_CutMapToCount( Pos2RoleMap& mapPos, size_t nCount )
{
	size_t nSum = 0;
	for (Pos2RoleMap::iterator it = mapPos.begin(); it != mapPos.end(); )
	{
		if ( nSum == nCount )
		{
			mapPos.erase(it++);
		}
		else
		{
			if ( nSum + it->second.size() > nCount )
			{
				size_t nNeed = nCount - nSum;
				set<unsigned int>::iterator itr = it->second.begin();
				advance(itr, nNeed);
				it->second.erase(itr, it->second.end());
				nSum = nCount;
			}
			else
			{
				nSum += it->second.size();
			}
			++it;
		}
	}
}

float CLBS::CalcDist( const LBSPOS& p1, const LBSPOS& p2 )
{
	if ( p1 == p2 )
	{
		return 0.0;
	}
	float MLatA = p1.y * PI / 180.0f;
	float MLitA = p1.x * PI / 180.0f;
	float MLatB = p2.y * PI / 180.0f;
	float MLitB = p2.x * PI / 180.0f;
	float a = MLatA - MLatB;
	float b = MLitA - MLitB;
	float sin_halfa = sin(a/2.0f);
	float sin_halfb = sin(b/2.0f);
	//google 经验公式
	float fDist = 2 * asin(sqrt(sin_halfa*sin_halfa + cos(MLatA)*cos(MLatB)*sin_halfb*sin_halfb))*6378137.0f;
	return fDist;
}

void CLBS::_CalcRadius( const LBSPOS& pos, float fRadiusMetre, float& fLatRadius, float& fLitRadius )
{
	float fPerLatDist = MAX_LAT_DIFF;
	float fPerLitDist = MAX_LAT_DIFF * cos(pos.y * PI / 180.0f);
	fLatRadius = abs(fRadiusMetre / fPerLatDist);
	fLitRadius = abs(fRadiusMetre / fPerLitDist);
}

size_t CLBS::_GetPosFromBeginOrEnd( bool bBeginOrEnd, float pos, const Pos2RoleMap& mapPos, ESexType sex, set<unsigned int>& setRoles )
{
	size_t nSum = 0;
	if ( bBeginOrEnd )
	{
		for (Pos2RoleMap::const_iterator it = mapPos.begin();
			it != mapPos.end(); ++it)
		{
			if ( it->first < pos )
			{
				setRoles.insert(it->second.begin(), it->second.end());
				nSum += it->second.size();
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		for (Pos2RoleMap::const_reverse_iterator it = mapPos.rbegin();
			it != mapPos.rend(); ++it)
		{
			if ( it->first > pos )
			{
				setRoles.insert(it->second.begin(), it->second.end());
				nSum += it->second.size();
			}
			else
			{
				break;
			}
		}
	}
	//性别过滤
	if ( sex != ESexType_No )
	{
		for (set<unsigned int>::iterator it = setRoles.begin(); it != setRoles.end(); )
		{
			if ( m_mapRole[sex].find(*it) == m_mapRole[sex].end() )
			{
				setRoles.erase(it++);
				--nSum;
			}
			else
			{
				++it;
			}
		}
	}
	return nSum;
}

