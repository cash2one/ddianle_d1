#ifndef __GAMEMSG_MAIL_DEFINE_H__
#define __GAMEMSG_MAIL_DEFINE_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"

class GameMsg_S2G_SendMail :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_SendMail);
	GameMsg_S2G_SendMail(void);
	~GameMsg_S2G_SendMail(void);

	unsigned int nNoticeRoleID;
	MailInfo m_mailInfo;
public:
	virtual bool doDecode( CParamPool& IOBuff );
	virtual bool doEncode( CParamPool& IOBuff );
};

class GameMsg_G2S_SendMail :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_SendMail);
	GameMsg_G2S_SendMail(void);
	~GameMsg_G2S_SendMail(void);

	MailInfo m_mailInfo;
public:
	virtual bool doDecode( CParamPool& IOBuff );
	virtual bool doEncode( CParamPool& IOBuff );
};

class GameMsg_A2S_SendMail:
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE( GameMsg_A2S_SendMail );
	GameMsg_A2S_SendMail( void );
	~GameMsg_A2S_SendMail( void );

public:
	unsigned int m_nRoleID;
	MailInfo m_mailInfo;
public:
	virtual bool doDecode( CParamPool& IOBuff );
	virtual bool doEncode( CParamPool& IOBuff );
};

// 请求自己是否设置不接受陌生人
class GameMsg_C2S_ReceiveStrangeMailFlag : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_ReceiveStrangeMailFlag);
    GameMsg_C2S_ReceiveStrangeMailFlag();
    ~GameMsg_C2S_ReceiveStrangeMailFlag();

    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);

};

class GameMsg_S2C_ReceiveStrangeMailFlag : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_ReceiveStrangeMailFlag);
    GameMsg_S2C_ReceiveStrangeMailFlag();
    ~GameMsg_S2C_ReceiveStrangeMailFlag();

    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);


    bool m_bSet; 
};

// 设置 是否接受陌生人消息标记
class GameMsg_C2S_SetReceiveStrangeMailFlag : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_SetReceiveStrangeMailFlag);
    GameMsg_C2S_SetReceiveStrangeMailFlag();
    ~GameMsg_C2S_SetReceiveStrangeMailFlag();

    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);

    bool m_bSet;
};

class GameMsg_S2C_SetReceiveStrangeMailFlag : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_SetReceiveStrangeMailFlag);
    GameMsg_S2C_SetReceiveStrangeMailFlag();
    ~GameMsg_S2C_SetReceiveStrangeMailFlag();

    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);

    bool m_bSet;
};


// 是否能跟某人发送邮件
class GameMsg_C2S_CanSendMail : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_CanSendMail);
    GameMsg_C2S_CanSendMail();
    ~GameMsg_C2S_CanSendMail();
    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);

    // 如果有id不为0，则使用id判断，否则，使用roleName判断
    unsigned int m_nReceiveRoleID; // 收件人ID
    std::string m_strReceiveRoleName; // 收件人名字
};

class GameMsg_S2C_CanSendMail : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_CanSendMail);
    GameMsg_S2C_CanSendMail();
    ~GameMsg_S2C_CanSendMail();
    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);

    unsigned char m_nRet; // 邮件检查错误码
};


class GameMsg_S2G_ValidSendMail : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_ValidSendMail);
    GameMsg_S2G_ValidSendMail();
    ~GameMsg_S2G_ValidSendMail();
    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);

    MailInfoDB m_mailInfoDB;
};


class GameMsg_G2S_ValidSendMail : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_ValidSendMail);
    GameMsg_G2S_ValidSendMail();
    ~GameMsg_G2S_ValidSendMail();
    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);

    unsigned char m_nRes; // 是否能发
    MailInfoDB m_mailInfoDB;
};


#endif


//end



