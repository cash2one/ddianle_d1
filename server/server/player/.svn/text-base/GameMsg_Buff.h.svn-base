/** GameMsg_Buff.h
* Description: buff消息
* Copyright (C) 2014 ddianle Inc. All rights reserved.
* Author: fanliangyuan
* Create time: 2014-9-23 19:55:16
*/
#ifndef __GAMEMSG_BUFF_H__
#define __GAMEMSG_BUFF_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"
//buff类型
enum EBuffType
{
	EBuffType_None,
	EBuffType_Exp,			//经验buff
	EBuffType_Transform,	//变身buff
	EBuffType_RoomTop,		//房间置顶buff
	EBuffType_RoomColor,	//房间变色buff
	EBuffType_ChatColor		//发言变色buff
};
//buff来源类型
enum EBuffSrcType
{
	EBuffSrcType_None,
	EBuffSrcType_Gene,		//基因
	EBuffSrcType_VIP,		//vip
	EBuffSrcType_Activity	//活动
};

//buff数据
struct BuffData
{
	unsigned char m_nBuffType;//buff类型
	unsigned char m_nBuffSrcType;//buff来源类型
	unsigned short m_nGeneID;//对非基因产生的buff，此字段始终是0，m_nGeneID大于零表示此buff可解除
	int m_nPercent;//百分比
	int m_nLeftTime;//剩余时间，秒，-1表示永久
	itemtype_t m_nItemType;//物品id，非基因产生的buff此字段是0
	bool m_bCanRemove;//是否可解除

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
	stringw m_strError;//空表示成功
};

#endif

