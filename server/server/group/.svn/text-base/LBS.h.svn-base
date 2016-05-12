#ifndef __LBS_H__
#define __LBS_H__

#include <map>
#include <set>
#include <vector>
#include <list>
#include "../../datastructure/Macro_Define.h"
using namespace std;

struct LBSPOS
{
	float x;//经度
	float y;//纬度

	LBSPOS():x(0),y(0){}
	LBSPOS(float _x, float _y):x(_x),y(_y){}
	bool operator == (const LBSPOS& rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}
};

typedef map<float, set<unsigned int> > Pos2RoleMap;//位置<=>玩家id
typedef map<unsigned int, LBSPOS> Role2PosMap;//玩家id<=>位置
//基于位置的服务
class CLBS
{
public:
	CLBS();
	~CLBS();
public:
	static CLBS& Instance();

	//增加玩家位置
	void AddPos(const LBSPOS& pos, unsigned int nRoleID, ESexType sex);
	//删除玩家位置
	void RemovePos(unsigned int nRoleID, ESexType sex);
	//查找玩家位置
	bool FindPos(unsigned int nRoleID, LBSPOS& pos);
	//获得指定玩家的附近玩家，过滤条件为距离，玩家个数，性别，特殊过滤列表（例如黑名单）
	size_t GetPosRoundRole(unsigned int nRoleID, float fRadiusMetre, size_t nCount, Pos2RoleMap& mapPos, bool bOrderByDist, ESexType sex, const set<unsigned int>& setFilter);
	//根据经纬度计算两点之间的距离
	float CalcDist(const LBSPOS& p1, const LBSPOS& p2);
private:
	//获得附近玩家
	size_t _GetPosRoundRole(unsigned int nRoleID, float pos, float radius, const Pos2RoleMap& mapPos, set<unsigned int>& setRoles, ESexType sex, bool bForward, bool bBackward);
	//根据一个维度（经度或者纬度）和性别在某段距离内查找玩家，可以指定向前搜索或者向后搜索，原因是经度是环状范围
	size_t _GetPosFromBeginOrEnd(bool bBeginOrEnd, float pos, const Pos2RoleMap& mapPos, ESexType sex, set<unsigned int>& setRoles);
	//将map进行裁剪，剔除多余玩家
	void _CutMapToCount(Pos2RoleMap& mapPos, size_t nCount);
	//根据点和范围（米）计算经纬度半径（因为非平面，而是不规则的球面，所以得出两个半径）
	void _CalcRadius(const LBSPOS& pos, float fRadiusMetre, float& fLatRadius, float& fLitRadius);
private:
	Pos2RoleMap m_mapXPos[ESexType_Max];//SexType_No存储两种性别
	Pos2RoleMap m_mapYPos[ESexType_Max];//SexType_No存储两种性别
	Role2PosMap m_mapRole[ESexType_Max];//SexType_No存储两种性别
};

#endif

