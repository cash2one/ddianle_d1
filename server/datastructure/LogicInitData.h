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
	unsigned short m_nExpGain;							//���鱶��
	unsigned short m_nMoneyGain;						//��Ǯ����
	unsigned short m_nItemDropGain;						//��Ʒ���䱶��

	int *m_anExpNeed;									//������Ҫ��exp
	int m_nAutoTimeMin;									//�������ʱ����Сֵ
    int m_nAutoTimeMax;                                 //�������ʱ�����ֵ
	//[����]
	unsigned short m_nMaxLevel;

	//[��Ʒ��]
	unsigned short m_anItemMaxNumber[EItemColumn_Max];//
	unsigned short m_anItemExpandNumber[EItemColumn_Max];//

	std::vector< std::pair< unsigned int, unsigned int > > m_aInitItem;	//���Գ�ʼ��Ʒ
	std::vector< std::pair< unsigned int, unsigned int > > m_aInitItem2;//Ů�Գ�ʼ��Ʒ

	unsigned short m_nDefaultColor[CHATCOLORTYPE];//
private :
	unsigned int m_DefaultColth[2][DEFUALT_COLTH_NUM];//��Ů�ĳ�ʼװ��


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


