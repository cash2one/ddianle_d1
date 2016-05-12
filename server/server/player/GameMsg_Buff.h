/** GameMsg_Buff.h
* Description: buff��Ϣ
* Copyright (C) 2014 ddianle Inc. All rights reserved.
* Author: fanliangyuan
* Create time: 2014-9-23 19:55:16
*/
#ifndef __GAMEMSG_BUFF_H__
#define __GAMEMSG_BUFF_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"
//buff����
enum EBuffType
{
	EBuffType_None,
	EBuffType_Exp,			//����buff
	EBuffType_Transform,	//����buff
	EBuffType_RoomTop,		//�����ö�buff
	EBuffType_RoomColor,	//�����ɫbuff
	EBuffType_ChatColor		//���Ա�ɫbuff
};
//buff��Դ����
enum EBuffSrcType
{
	EBuffSrcType_None,
	EBuffSrcType_Gene,		//����
	EBuffSrcType_VIP,		//vip
	EBuffSrcType_Activity	//�
};

//buff����
struct BuffData
{
	unsigned char m_nBuffType;//buff����
	unsigned char m_nBuffSrcType;//buff��Դ����
	unsigned short m_nGeneID;//�Էǻ��������buff�����ֶ�ʼ����0��m_nGeneID�������ʾ��buff�ɽ��
	int m_nPercent;//�ٷֱ�
	int m_nLeftTime;//ʣ��ʱ�䣬�룬-1��ʾ����
	itemtype_t m_nItemType;//��Ʒid���ǻ��������buff���ֶ���0
	bool m_bCanRemove;//�Ƿ�ɽ��

	BuffData():m_nBuffType(0), m_nBuffSrcType(0), m_nGeneID(0), m_nPercent(0), m_nLeftTime(0), 
		m_nItemType(0), m_bCanRemove(false){}
	bool doEncode(CParamPool &IOBuff) const;
};

/////////////////////////////////////////////////
class GameMsg_C2S_GetAllBuff :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetAllBuff);
	GameMsg_C2S_GetAllBuff();
	~GameMsg_C2S_GetAllBuff();
public:
	virtual bool doDecode(CParamPool &IOBuff);
};

/////////////////////////////////////////////////
class GameMsg_S2C_GetAllBuff :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetAllBuff);
	GameMsg_S2C_GetAllBuff();
	~GameMsg_S2C_GetAllBuff();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	std::list<BuffData> m_listBuffData;
};

/////////////////////////////////////////////////
class GameMsg_C2S_UnApplyBuff :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_UnApplyBuff);
	GameMsg_C2S_UnApplyBuff();
	~GameMsg_C2S_UnApplyBuff();
public:
	virtual bool doDecode(CParamPool &IOBuff);
public:
	unsigned short m_nGeneId;
};

/////////////////////////////////////////////////
class GameMsg_S2C_UnApplyBuff :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_UnApplyBuff);
	GameMsg_S2C_UnApplyBuff();
	~GameMsg_S2C_UnApplyBuff();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	unsigned short m_nGeneId;
	stringw m_strError;//�ձ�ʾ�ɹ�
};

#endif

