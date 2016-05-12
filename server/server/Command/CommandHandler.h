#ifndef	__COMMANDHANDLER_H__
#define __COMMANDHANDLER_H__

#include "ICommandHandler.h"


/*************************************************************/
/*				    �������addquest						*/
/***********************************************************/
class CCmdAddQusetHandler:
	public ICmdHandler
{
public:
	CCmdAddQusetHandler();
	~CCmdAddQusetHandler();
public:
	virtual string Cmd()const;	
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};


/*************************************************************/
/*				    �������finquest						*/
/***********************************************************/
class CCmdFinQuestHandler:
	public ICmdHandler
{
public:
	CCmdFinQuestHandler();
	~CCmdFinQuestHandler();
public:
	virtual string Cmd()const;	
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);

};

/*************************************************************/
/*				    ���accountID							*/
/***********************************************************/
class CCmdAccountID:
	public ICmdHandler
{
public:
	CCmdAccountID();
	~CCmdAccountID();
public:
	virtual string Cmd()const;	
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

/*************************************************************/
/*				    ���roleID								*/
/***********************************************************/
class CCmdRoleID:
	public ICmdHandler
{
public:
	CCmdRoleID();
	~CCmdRoleID();
public:
	virtual string Cmd()const;	
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};


/*************************************************************/
/*				   ���þ���addexp							*/
/***********************************************************/
class CCmdAddExp:
	public ICmdHandler
{
public:
	CCmdAddExp();
	~CCmdAddExp();
public:
	virtual string Cmd()const;	
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

/*************************************************************/
/*				   ��������ֵsethot						*/
/***********************************************************/
class CCmdSetHot:
	public ICmdHandler
{
public:
	CCmdSetHot();
	~CCmdSetHot();
public:
	virtual string Cmd()const;	
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

/*************************************************************/
/*				   ��������ֵsetpre						*/
/***********************************************************/
class CCmdSetPre:
	public ICmdHandler
{
public:
	CCmdSetPre();
	~CCmdSetPre();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

/*************************************************************/
/*				   ���ý��setmoney						*/
/***********************************************************/
class CCmdSetMoney:
	public ICmdHandler
{
public:
	CCmdSetMoney();
	~CCmdSetMoney();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

/*************************************************************/
/*				   ��ɫ����levelup 						*/
/***********************************************************/
class CCmdLevelUp:
	public ICmdHandler
{
public:
	CCmdLevelUp();
	~CCmdLevelUp();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

/*************************************************************/
/*				   ��ɫ����setlevel 						*/
/***********************************************************/
class CCmdSetLevel:
	public ICmdHandler
{
public:
	CCmdSetLevel();
	~CCmdSetLevel();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};


/*************************************************************/
/*				   �����Ʒadditem							*/
/***********************************************************/
class CCmdAddItem:
	public ICmdHandler
{
public:
	CCmdAddItem();
	~CCmdAddItem();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};


/*************************************************************/
/*				   ��Ʒ��ȴ���itemcdall					*/
/***********************************************************/
class CCmdItemCDAll:
	public ICmdHandler
{
public:
	CCmdItemCDAll();
	~CCmdItemCDAll();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};


/*************************************************************/
/*				�޸�ϵͳʱ��settime						*/
/*				�÷�settime 2011-02-11 13:00:00				*/
/***********************************************************/
class CCmdSetSysTime:
	public ICmdHandler
{
public:
	CCmdSetSysTime();
	~CCmdSetSysTime();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
public:
	unsigned short m_nYear;				// ��ݣ�����2012
	unsigned short m_nMonth;			// �·�
	unsigned short m_nDay;				// ��
	unsigned short m_nHour;				// Сʱ
	unsigned short m_nMinute;			// ����
	unsigned short m_nSecond;			// ��
private:
	bool _ParseCmd(const string &strCmd);
	bool _SetSysTime();
};

/*************************************************************/
/*				   ����M��setpt		 						*/
/***********************************************************/
class CCmdAddPt:
	public ICmdHandler
{
public:
	CCmdAddPt();
	~CCmdAddPt();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

/*************************************************************/
/*				   ��������ʱ��setonlinetime		 		*/
/***********************************************************/
class CCmdSetOnlineTime:
	public ICmdHandler
{
public:
	CCmdSetOnlineTime();
	~CCmdSetOnlineTime();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

/*************************************************************/
/*		������Ʒ�����senditem(roleid, itemtype,count)	*/
/***********************************************************/
class CCmdSendItem:
	public ICmdHandler
{
public:
	CCmdSendItem();
	~CCmdSendItem();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

/*************************************************************/
/*		������Ʒ�����	whosyourdaddy					*/
/***********************************************************/
class CCmdWhosyoudaddy:
	public ICmdHandler
{
public:
	CCmdWhosyoudaddy();
	~CCmdWhosyoudaddy();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};


/*************************************************************/
/*		��ѯ����������	playercount					*/
/***********************************************************/
class CCmdPlayerCount:
	public ICmdHandler
{
public:
	CCmdPlayerCount();
	~CCmdPlayerCount();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

/*************************************************************/
/*		����ϵͳ����	adminchat							*/
/***********************************************************/
class CCmdAdminChat:
	public ICmdHandler
{
public:
	CCmdAdminChat();
	~CCmdAdminChat();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

/*************************************************************/
/*		GM��M��	addmcoin							*/
/***********************************************************/
class CCmdAddMCoin:
	public ICmdHandler
{
public:
	CCmdAddMCoin();
	~CCmdAddMCoin();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

/*************************************************************/
/*		GM�Ӱ�M��	addbindmcoin							*/
/***********************************************************/
class CCmdAddBindMCoin:
	public ICmdHandler
{
public:
	CCmdAddBindMCoin();
	~CCmdAddBindMCoin();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

/*************************************************************/
/*      GMװ��������Ч    addLuckPoint                        */
/***********************************************************/
class CCmdAddClothEffect:
    public ICmdHandler
{
public:
    CCmdAddClothEffect();
    ~CCmdAddClothEffect();

public:
    virtual string Cmd()const;
    virtual ICmdHandler *CreateCmdHandler();
    virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};


/*************************************************************/
/*		           ͨ�������������Ϣ 					*/
/***********************************************************/
class CCmdDebug:
	public ICmdHandler
{
public:
	CCmdDebug();
	~CCmdDebug();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

/*************************************************************/
/*		               �޸����ܶ�--additinacy(int) 		*/
/***********************************************************/
class CCmdAddIntimacy:
	public ICmdHandler
{
public:
	CCmdAddIntimacy();
	~CCmdAddIntimacy();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

/*************************************************************/
/*		ģ������������openbox(boxid,count)	*/
/***********************************************************/
class CCmdOpenBox:
	public ICmdHandler
{
public:
	CCmdOpenBox();
	~CCmdOpenBox();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

/*****************************************************************/
/*	ģ��ս��������Ʒ����danceitem(modeid, count) mode:1-4  	*/
/***************************************************************/
class CCmdDropDanceItem:
	public ICmdHandler
{
public:
	CCmdDropDanceItem();
	~CCmdDropDanceItem();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

/*****************************************************************/
/*						   �޸�VIP			  					*/
/***************************************************************/
class CCmdSetVIPLevel:
	public ICmdHandler
{
public:
	CCmdSetVIPLevel();
	~CCmdSetVIPLevel();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

/*****************************************************************/
/*						�޸�VIP����ֵ  						*/
/***************************************************************/
class CCmdAddVIPExp:
	public ICmdHandler
{
public:
	CCmdAddVIPExp();
	~CCmdAddVIPExp();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

/*************************************************************/
/*		ģ����������ת��luckbet(mode,count)    0-��� 1-�շ�*/
/***********************************************************/
class CCmdLuckBet:
	public ICmdHandler
{
public:
	CCmdLuckBet();
	~CCmdLuckBet();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

/*************************************************************/
/*      ֱ���������ù�����������                                 */
/*************************************************************/
class CCmdLiveRoom:
	public ICmdHandler
{
public:
	CCmdLiveRoom();
	~CCmdLiveRoom();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

/*************************************************************/
/*      �������Է���                                         */
/*************************************************************/
class CCmdCreateRoom:
	public ICmdHandler
{
public:
	CCmdCreateRoom();
	~CCmdCreateRoom();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};


/*************************************************************/
/*      ���������Գ������Է���                                         */
/*************************************************************/
class CCmdCreateAmuseRoom:
	public ICmdHandler
{
public:
	CCmdCreateAmuseRoom();
	~CCmdCreateAmuseRoom();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

class CCmdAcceptQuest : 
	public ICmdHandler
{
public:
	CCmdAcceptQuest();
	~CCmdAcceptQuest();
public:
	virtual string Cmd() const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

/*************************************************************/
/*                ����ؿ�                               */
/*************************************************************/
class CCmdActivitedParclose:
	public ICmdHandler
{
public:
	CCmdActivitedParclose();
	~CCmdActivitedParclose();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

class CCmdActivitedThemeParclose:
	public ICmdHandler
{
public:
	CCmdActivitedThemeParclose();
	~CCmdActivitedThemeParclose();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

/*************************************************************/
/*                ǩ��                               */
/*************************************************************/
class CCmdCheckIn:
	public ICmdHandler
{
public:
	CCmdCheckIn();
	~CCmdCheckIn();
public:
	virtual string Cmd()const;
	virtual ICmdHandler *CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};



class CCmdAddDebris:
    public ICmdHandler
{
public:
    CCmdAddDebris();
    ~CCmdAddDebris();
public:
    virtual string Cmd()const;
    virtual ICmdHandler *CreateCmdHandler();
    virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

class CCmdAddLiveRoomAuditionScore:
    public ICmdHandler
{
public:
    CCmdAddLiveRoomAuditionScore();
    ~CCmdAddLiveRoomAuditionScore();
public:
    virtual string Cmd()const;
    virtual ICmdHandler *CreateCmdHandler();
    virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

class CCmdGetServerPlayer :
	public ICmdHandler
{
public:
	CCmdGetServerPlayer();
	~CCmdGetServerPlayer();

public:
	virtual string Cmd()const;
	virtual ICmdHandler* CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

class CCmdAddDanceGroupHonor : 
	public ICmdHandler
{
public:
	CCmdAddDanceGroupHonor();
	~CCmdAddDanceGroupHonor();

public:
	virtual string Cmd()const;
	virtual ICmdHandler* CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

class CCmdEnterDanceGroupByName : 
    public ICmdHandler
{
public:
    CCmdEnterDanceGroupByName();
    ~CCmdEnterDanceGroupByName();

public:
    virtual string Cmd()const;
    virtual ICmdHandler* CreateCmdHandler();
    virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

// ������ʽ�У�ָ��������Ϊ����
class CCmdRookieAnyBody : 
    public ICmdHandler
{
public:
    CCmdRookieAnyBody();
    ~CCmdRookieAnyBody();

public:
    virtual string Cmd()const;
    virtual ICmdHandler* CreateCmdHandler();
    virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

// CCmdGetServerPlayer
class CCmdGetCeremonyRoomNum :
    public ICmdHandler
{
public:
    CCmdGetCeremonyRoomNum();
    ~CCmdGetCeremonyRoomNum();

public:
    virtual string Cmd()const;
    virtual ICmdHandler* CreateCmdHandler();
    virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

class CCmdActivenessComp : 
	public ICmdHandler
{
public:
	CCmdActivenessComp();
	~CCmdActivenessComp();

public:
	virtual string Cmd()const;
	virtual ICmdHandler* CreateCmdHandler();
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

class CCmdMagicArray :
    public ICmdHandler
{
public:
    CCmdMagicArray();
    ~CCmdMagicArray();

public:
    virtual string Cmd()const;
    virtual ICmdHandler* CreateCmdHandler();
    virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};

/*************************************************************/
/*      ѫ��                                                 */
/*************************************************************/

class CCmdMedalComp : public ICmdHandler
{
public:
    CCmdMedalComp();
    ~CCmdMedalComp();

public:
    virtual string Cmd() const;
    virtual ICmdHandler* CreateCmdHandler();
    virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd);
};


#endif


