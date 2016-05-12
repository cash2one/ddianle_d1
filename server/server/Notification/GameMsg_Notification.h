#ifndef __GAMEMSG_NOTIFICATION_H__
#define __GAMEMSG_NOTIFICATION_H__ 1

#include "../../socket/GameMsg_Base.h"


class GameMsg_C2S_NoPushRatingNotification :
	public	GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_NoPushRatingNotification);
	GameMsg_C2S_NoPushRatingNotification();
	~GameMsg_C2S_NoPushRatingNotification();
public:
	virtual bool doDecode(CParamPool &IOBuff);
};


class GameMsg_S2C_PushRatingNotification:
	public	GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_PushRatingNotification);
	GameMsg_S2C_PushRatingNotification();	
	~GameMsg_S2C_PushRatingNotification();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	std::string m_strMessage;
};

//////////////////////////////////////////////////////////////////////////
// ��Ϣ���ĵ���Ϣ
// ������Ϣ���ĵ��²���������ɾ���ģ�
struct InfoCenterTranFull
{
	unsigned int			m_id;
	unsigned int			m_updateTime;
	unsigned char			m_group;
	std::string				m_icon;
	unsigned char			m_type; // ���ţ�����
	std::string				m_strTitle;
	std::string				m_strContext;
	std::string				m_strHref;
};


enum EInfoCenterErr
{
	EInfoCenter_OK,
	EInfoCenter_MissID,
};

enum EInfoCenterType
{
	EInfoCenterType_News = 0,
	EInfoCenterType_Share,
};

enum EInfoCenterOp
{
	EInfoCenterOp_Add = 1,
	EInfoCenterOp_Update,
	EInfoCenterOp_Remove,
};

class GameMsg_S2C_InfoCenterNotification : 
	public	GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_InfoCenterNotification);
	GameMsg_S2C_InfoCenterNotification();	
	~GameMsg_S2C_InfoCenterNotification();
public:
	void Set(unsigned int id, unsigned int opTime, unsigned char type, unsigned char group, unsigned char opType);
	virtual bool doEncode(CParamPool &IOBuff);
public:
	unsigned int m_id;
	unsigned int m_opTime;
	unsigned char m_type; //0:���ţ�1������
	unsigned char m_group; 
	unsigned char m_opType; // �����ģ�ɾ. 1,2,3
};

// ����ID��Ӧ������
class GameMsg_C2S_InfoCenterGetIDRes : 
	public	GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_InfoCenterGetIDRes);
	GameMsg_C2S_InfoCenterGetIDRes();	
	~GameMsg_C2S_InfoCenterGetIDRes();
public:
	virtual bool doDecode(CParamPool &IOBuff);

public:
	std::vector<unsigned int> m_ids; // �������Ϣ����
};


class GameMsg_S2C_InfoCenterGetIDRes : 
	public	GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_InfoCenterGetIDRes);
	GameMsg_S2C_InfoCenterGetIDRes();	
	~GameMsg_S2C_InfoCenterGetIDRes();
public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	unsigned char m_nErros;
	std::vector<InfoCenterTranFull> m_infos;	// ��Ϣ����
};



// �������Ϊ�Ѷ�
class GameMsg_C2S_InfoCenterMarkReaded : 
	public	GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_InfoCenterMarkReaded);
	GameMsg_C2S_InfoCenterMarkReaded();	
	~GameMsg_C2S_InfoCenterMarkReaded();
public:
	virtual bool doDecode(CParamPool &IOBuff);

public:
	std::vector<unsigned int> m_ids; // �������Ϣ����
};


#endif //__GAMEMSG_NOTIFICATION_H__

