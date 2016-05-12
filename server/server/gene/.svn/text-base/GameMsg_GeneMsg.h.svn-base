#ifndef		__GAMEMSG_GENEmSG_H__
#define		__GAMEMSG_GENEmSG_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"
#include <string>


class GameMsg_S2C_AddGene:
	public GameMsg_Base
{
public:	
	GameMsg_S2C_AddGene();
	~GameMsg_S2C_AddGene();

	virtual bool doEncode(CParamPool &IOBuff);

public:
	unsigned int m_nRoleID;
	GeneData m_GeneData;
};

class GameMsg_S2C_DelGene:
	public GameMsg_Base
{
public:	
	GameMsg_S2C_DelGene();
	~GameMsg_S2C_DelGene();

	virtual bool doEncode(CParamPool &IOBuff);

public:
	unsigned int m_nRoleID;
	unsigned short m_nGeneID;
};

class GameMsg_S2C_ResetGene:
	public GameMsg_Base
{
public:	
	GameMsg_S2C_ResetGene();
	~GameMsg_S2C_ResetGene();

	virtual bool doEncode(CParamPool &IOBuff);

public:
	unsigned int m_nRoleID;
	unsigned short m_nGeneID;
	int m_nParam2;
};

#endif

