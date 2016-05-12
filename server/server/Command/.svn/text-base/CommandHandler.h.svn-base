#ifndef	__COMMANDHANDLER_H__
#define __COMMANDHANDLER_H__

#include "ICommandHandler.h"


/*************************************************************/
/*				    添加任务：addquest						*/
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
/*				    完成任务finquest						*/
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
/*				    获得accountID							*/
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
/*				    获得roleID								*/
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
/*				   设置经验addexp							*/
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
/*				   设置人气值sethot						*/
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
/*				   设置人气值setpre						*/
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
/*				   设置金币setmoney						*/
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
/*				   角色升级levelup 						*/
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
/*				   角色升级setlevel 						*/
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
/*				   添加物品additem							*/
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
/*				   物品冷却完毕itemcdall					*/
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
/*				修改系统时间settime						*/
/*				用法settime 2011-02-11 13:00:00				*/
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
	unsigned short m_nYear;				// 年份，诸如2012
	unsigned short m_nMonth;			// 月份
	unsigned short m_nDay;				// 天
	unsigned short m_nHour;				// 小时
	unsigned short m_nMinute;			// 分钟
	unsigned short m_nSecond;			// 秒
private:
	bool _ParseCmd(const string &strCmd);
	bool _SetSysTime();
};

/*************************************************************/
/*				   设置M币setpt		 						*/
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
/*				   设置在线时长setonlinetime		 		*/
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
/*		发送物品给玩家senditem(roleid, itemtype,count)	*/
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
/*		发送物品给玩家	whosyourdaddy					*/
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
/*		查询服务器数量	playercount					*/
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
/*		发送系统公告	adminchat							*/
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
/*		GM加M币	addmcoin							*/
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
/*		GM加绑定M币	addbindmcoin							*/
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
/*      GM装备增加特效    addLuckPoint                        */
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
/*		           通过聊天命令发送信息 					*/
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
/*		               修改亲密度--additinacy(int) 		*/
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
/*		模拟批量开宝箱openbox(boxid,count)	*/
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
/*	模拟战斗结束物品掉落danceitem(modeid, count) mode:1-4  	*/
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
/*						   修改VIP			  					*/
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
/*						修改VIP经验值  						*/
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
/*		模拟批量幸运转盘luckbet(mode,count)    0-免费 1-收费*/
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
/*      直播房间设置观众人数上限                                 */
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
/*      创建测试房间                                         */
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
/*      创建开放性场景测试房间                                         */
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
/*                激活关卡                               */
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
/*                签到                               */
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

// 入团仪式中，指定任意人为新人
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
/*      勋章                                                 */
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


