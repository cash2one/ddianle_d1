/********************************************************************
	created:	2011/01/10
	author:		meijiuhua
	filename: 	server\logic\LogicInitData.h
	
	purpose:	
*********************************************************************/


#ifndef __CLogicInitData_H_
#define __CLogicInitData_H_

#include <vector>
#include <utility>
#include <list>
#include <string>
using namespace std;

#include "Macro_Define.h"

#define SET_EXP_GAIN		1
#define SET_MONEY_GAIN		2
#define SET_ITEMDROP_GAIN	3
#define SET_ALL_GAIN		4

class CLogicInitData
{
protected:
	CLogicInitData();
	~CLogicInitData();

public:
	unsigned short m_nExpGain;							//经验倍数
	unsigned short m_nMoneyGain;						//金钱倍数
	unsigned short m_nItemDropGain;						//物品掉落倍数

	int *m_anExpNeed;									//升级需要的exp
	int m_nAutoTimeMin;									//随机保存时间最小值
    int m_nAutoTimeMax;                                 //随机保存时间最大值
	//[级别]
	unsigned short m_nMaxLevel;

	//[物品栏]
	unsigned short m_anItemMaxNumber[EItemColumn_Max];//
	unsigned short m_anItemExpandNumber[EItemColumn_Max];//

	std::vector< std::pair< unsigned int, unsigned int > > m_aInitItem;	//男性初始物品
	std::vector< std::pair< unsigned int, unsigned int > > m_aInitItem2;//女性初始物品

	unsigned short m_nDefaultColor[CHATCOLORTYPE];//
private :
	unsigned int m_DefaultColth[2][DEFUALT_COLTH_NUM];//男女的初始装。


public:
	static CLogicInitData& Instance();

	bool InitAllData();

	unsigned short GetMaxLevel()const{return m_nMaxLevel;}
	int GetExpNeed(unsigned short nCurLevel)const;
	
    unsigned int GetColumnMaxSlotNum(EItemColumn eColumn);
	unsigned int GetColumnMaxSlotNum(EBagColumn eColumn);
	unsigned int GetColumnMaxSlotNum(EEquipColumn eColumn);
    unsigned int GetDesireMaxSize();
	unsigned short GetExpGainPer()const{return m_nExpGain;};
	unsigned short GetMoneyGainPer()const{return m_nMoneyGain;};
	unsigned short GetItemDropGainPer()const{return m_nItemDropGain;};

	void SetGainPer(unsigned short nGain,char nType = SET_ALL_GAIN);

	void SetExpGainPer(unsigned short nExpGain){SetGainPer(nExpGain,SET_EXP_GAIN);};
	void SetMoneyGainPer(unsigned short nMoneyGain){SetGainPer(nMoneyGain,SET_MONEY_GAIN);};
	void SetItemDropGainPer(unsigned short nItemDropGain){SetGainPer(nItemDropGain,SET_ITEMDROP_GAIN);};

	void AddGainPer(short nGain,char nType);

	void AddExpGainPer(short nExpGain){AddGainPer(nExpGain,SET_EXP_GAIN);};
	void AddMoneyGainPer(short nMoneyGain){AddGainPer(nMoneyGain,SET_MONEY_GAIN);};
	void AddItemDropGainPer(short nItemDropGain){AddGainPer(nItemDropGain,SET_ITEMDROP_GAIN);};
	unsigned int GetDefaultColth( bool bMan ,unsigned int nPart );
	bool IsDefaultColth(unsigned int nTypeID) const;
private:
	void _SetParam(unsigned short & nOriGain,short nGain);
};


#endif	//__CLogicInitData_H_


//end file


