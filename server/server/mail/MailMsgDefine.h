#ifndef __MAILMSGDEFINE_H__
#define __MAILMSGDEFINE_H__

#include "../../socket/GameMsg_Base.h"
#include "MailDefine.h"
#include <list>

using namespace std;

class GameMsg_C2S_GetMailBox :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetMailBox);
	GameMsg_C2S_GetMailBox(void);
	~GameMsg_C2S_GetMailBox(void);

	unsigned short nPage;

public:
	virtual bool doDecode( CParamPool& IOBuff );
};



class GameMsg_S2C_GetMailBox : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_GetMailBox);

public:
    virtual bool doEncode( CParamPool &IOBuff );

    void SetPoolData( const char *pData, unsigned int nDataSize );

public:
    unsigned short m_nCount;

private:
    std::string m_strPoolData;
};


class GameMsg_C2S_GetMailInfo : public GameMsg_Base
{
    typedef GameMsg_Base _baseClass;

public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetMailInfo);
    GameMsg_C2S_GetMailInfo(void);
    ~GameMsg_C2S_GetMailInfo(void) {}

public:
    virtual bool doDecode( CParamPool& IOBuff );

public:
    unsigned char   m_nType;
    __int64         m_nMailID;
};



class GameMsg_S2C_GetMailInfo : public GameMsg_Base
{
    typedef GameMsg_Base _baseClass;

public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetMailInfo);
    GameMsg_S2C_GetMailInfo(void);
    ~GameMsg_S2C_GetMailInfo(void) {}

public:
    virtual bool doEncode( CParamPool& IOBuff );

public:
    unsigned char   m_nResult;      // 0 == MAIL_OK
    MailInfo        m_mailInfo;
};



class GameMsg_C2S_SendMail : public GameMsg_Base
{
    typedef  GameMsg_Base _baseClass;

public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_SendMail);
    GameMsg_C2S_SendMail(void);
    ~GameMsg_C2S_SendMail(void) {}

public:
    void SetTitle(const std::string &rTitle);
    const std::string& GetTitle() const { return m_strTitle; }

    void SetContent(const std::string &rContent);
    const std::string& GetContent() const { return m_strContent; }

    virtual bool doDecode( CParamPool& IOBuff );

public:
    unsigned int    m_nReceiverID;
    char            m_szReceiverName[MAX_NAME_LEN];

protected:
    std::string     m_strTitle;
    std::string     m_strContent;
};



class GameMsg_S2C_SendMail :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_SendMail);
	GameMsg_S2C_SendMail(void);
	~GameMsg_S2C_SendMail(void);

	unsigned char  nResult;

public:
	virtual bool doEncode( CParamPool& IOBuff );
};


class GameMsg_C2S_DeleteMail :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_DeleteMail);
	GameMsg_C2S_DeleteMail(void);
	~GameMsg_C2S_DeleteMail(void);

	std::list<__int64>  aListMailID;

public:
	virtual bool doDecode( CParamPool& IOBuff );
};



class GameMsg_S2C_DeleteMail :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_DeleteMail);
	GameMsg_S2C_DeleteMail(void);
	~GameMsg_S2C_DeleteMail(void);

	unsigned char  nResult;
	std::string    strReturn;
public:
	virtual bool doEncode( CParamPool& IOBuff );
};


class GameMsg_C2S_GetMailItem :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetMailItem);
	GameMsg_C2S_GetMailItem(void);
	~GameMsg_C2S_GetMailItem(void);

	__int64 nMailID;

public:
	virtual bool doDecode( CParamPool& IOBuff );
};



class GameMsg_S2C_GetMailItem :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetMailItem);
	GameMsg_S2C_GetMailItem(void);
	~GameMsg_S2C_GetMailItem(void);

	unsigned char  nResult;
	string m_strError;

public:
	virtual bool doEncode( CParamPool& IOBuff );
};

class GameMsg_S2C_ReceiveMail :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_ReceiveMail);
	GameMsg_S2C_ReceiveMail(void);
	~GameMsg_S2C_ReceiveMail(void);

public:
	virtual bool doEncode( CParamPool& IOBuff );

public:
    __int64 nMailID;
};


class GameMsg_S2C_UnReadMail :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GameMsg_S2C_UnReadMail(void);
	~GameMsg_S2C_UnReadMail(void);

public:
	virtual bool doEncode( CParamPool& IOBuff );

public:
	unsigned short m_nUnReadCount;
};

#endif


//end file


