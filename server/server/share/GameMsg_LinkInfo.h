#ifndef __GameMsg_LINKINFO_H__
#define __GameMsg_LINKINFO_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"

#define INVALID_GROUP_LINE	0x7F
typedef struct LINKSERVERINFO
{
	unsigned int   addr;   //linkserver�ĵ�ַ
	unsigned short nPort;  //linkserver�Ķ˿�
	unsigned short nClient;//linkserver��ǰ���ӵĿͻ�����
	unsigned short nGroup; //linkserver����
	int nLine;  //Linkserver����
	string strName;
	unsigned char nInstallerID;	//�Ƿ���������
	bool bEnableAudition;  //�Ƿ���ֱ�����������ѡ��

	LINKSERVERINFO()
	{
		addr = 0;
		nPort = 0;
		nClient = 0;
		nGroup = INVALID_GROUP_LINE;
		nLine = (int)INVALID_GROUP_LINE;
		nInstallerID = INSTALLER_INVALID;
		bEnableAudition = false;
	}
	void Clear()
	{
		addr = 0;
		nPort = 0;
		nClient = 0;
		strName = "";
		nGroup = INVALID_GROUP_LINE;
		nLine = (int)INVALID_GROUP_LINE;
		nInstallerID = INSTALLER_INVALID;
		bEnableAudition = false;
	}
}*LPLINKSERVERINFO;

class GameMsg_LinkInfo :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_LinkInfo);
	GameMsg_LinkInfo(void);
	~GameMsg_LinkInfo(void);

	LINKSERVERINFO info;
public:
	virtual bool doEncode( CParamPool& IoBuff );
	virtual bool doDecode( CParamPool& IoBuff );
};

#endif


//file end

