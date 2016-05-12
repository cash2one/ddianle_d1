#ifndef __AMUSEPARAM__H__
#define __AMUSEPARAM__H__

#include <string>
#include "../../datastructure/DataStruct_Base.h"
#include "../mall/MallDefine.h"
class CParamPool;


///////////////////////////////////////////////////////
class CAmuseActionParam_Base
{
public:
	CAmuseActionParam_Base(int nActionType);
	virtual ~CAmuseActionParam_Base();

	virtual bool doDecode(CParamPool &IOBuff) = 0;
	virtual bool doEncode(CParamPool &IOBuff) = 0;

public:
	char m_nActionType;
};


///////////////////////////////////////////////////////
class CAmuseActionParam_UseDevice : public CAmuseActionParam_Base
{
public:
	CAmuseActionParam_UseDevice();
	virtual ~CAmuseActionParam_UseDevice();

	virtual bool doDecode(CParamPool &IOBuff);
	virtual bool doEncode(CParamPool &IOBuff);

public:
	unsigned int m_nPlayerID;
	char m_nDeviceID;
	char m_nPartnerType;
	int m_nStartTime;
	bool m_bInviter;
	list<CItem> m_listCost;
};


///////////////////////////////////////////////////////
class CAmuseActionParam_LeaveDevice : public CAmuseActionParam_Base
{
public:
	CAmuseActionParam_LeaveDevice();
	virtual ~CAmuseActionParam_LeaveDevice();

	virtual bool doDecode(CParamPool &IOBuff);
	virtual bool doEncode(CParamPool &IOBuff);

public:
	unsigned int m_nPlayerID;
};


///////////////////////////////////////////////////////
class CAmuseActionParam_InviteDevice : public CAmuseActionParam_Base
{
public:
	CAmuseActionParam_InviteDevice();
	virtual ~CAmuseActionParam_InviteDevice();

	virtual bool doDecode(CParamPool &IOBuff);
	virtual bool doEncode(CParamPool &IOBuff);

public:
	unsigned int m_nInviteeID;
};


///////////////////////////////////////////////////////
class CAmuseActionParam_RequestDevice : public CAmuseActionParam_Base
{
public:
	CAmuseActionParam_RequestDevice();
	virtual ~CAmuseActionParam_RequestDevice();

	virtual bool doDecode(CParamPool &IOBuff);
	virtual bool doEncode(CParamPool &IOBuff);

public:
	unsigned int m_nPlayerID;
	std::string m_strPlayerName;
	char m_chPlayerSex;
	bool m_bIsVIP;
	unsigned short m_nVIPLevel;

	char m_nDeviceID;
	std::string m_strRequestTips;
};


///////////////////////////////////////////////////////
class CAmuseActionParam_ResponseDevice : public CAmuseActionParam_Base
{
public:
	CAmuseActionParam_ResponseDevice();
	virtual ~CAmuseActionParam_ResponseDevice();

	virtual bool doDecode(CParamPool &IOBuff);
	virtual bool doEncode(CParamPool &IOBuff);

public:
	unsigned int m_nInviterID;
	char m_nDeviceID;
	bool m_bAccept;
};


///////////////////////////////////////////////////////
class CAmuseActionParam_UsePose : public CAmuseActionParam_Base
{
public:
	CAmuseActionParam_UsePose();
	virtual ~CAmuseActionParam_UsePose();

	virtual bool doDecode(CParamPool &IOBuff);
	virtual bool doEncode(CParamPool &IOBuff);

public:
	unsigned int m_nPlayerID;
	char m_nPoseID;
};


///////////////////////////////////////////////////////
class CAmuseActionParam_LeavePose : public CAmuseActionParam_Base
{
public:
	CAmuseActionParam_LeavePose();
	virtual ~CAmuseActionParam_LeavePose();

	virtual bool doDecode(CParamPool &IOBuff);
	virtual bool doEncode(CParamPool &IOBuff);

public:
	unsigned int m_nPlayerID;
};

///////////////////////////////////////////////////////
class CAmuseActionParam_GetTimeEggGiftList : public CAmuseActionParam_Base
{
public:
	CAmuseActionParam_GetTimeEggGiftList();
	virtual ~CAmuseActionParam_GetTimeEggGiftList();

	virtual bool doDecode(CParamPool &IOBuff);
	virtual bool doEncode(CParamPool &IOBuff);
public:
	std::vector<itemtype_t> m_listGift;//礼物列表
	std::vector<itemtype_t> m_listDesire;//对方许愿栏
	unsigned int m_nGiftDiscount;
};

///////////////////////////////////////////////////////
class CAmuseActionParam_CommitTimeEggInfo : public CAmuseActionParam_Base
{
public:
	CAmuseActionParam_CommitTimeEggInfo();
	virtual ~CAmuseActionParam_CommitTimeEggInfo();

	virtual bool doDecode(CParamPool &IOBuff);
	virtual bool doEncode(CParamPool &IOBuff);

public:
	string m_strWords;//留言
	VecPeddleGoods m_listGift;//纪念品
	unsigned int m_nPlayerID;
};

///////////////////////////////////////////////////////
class CAmuseActionParam_CreateTimeEgg : public CAmuseActionParam_Base
{
public:
	CAmuseActionParam_CreateTimeEgg();
	virtual ~CAmuseActionParam_CreateTimeEgg();

	virtual bool doDecode(CParamPool &IOBuff);
	virtual bool doEncode(CParamPool &IOBuff);
};

///////////////////////////////////////////////////////
class CAmuseActionParam_OpenTimeEgg : public CAmuseActionParam_Base
{
public:
	CAmuseActionParam_OpenTimeEgg();
	virtual ~CAmuseActionParam_OpenTimeEgg();

	virtual bool doDecode(CParamPool &IOBuff);
	virtual bool doEncode(CParamPool &IOBuff);
public:
	unsigned char m_chOpenType;//开启类型
	unsigned int m_nSelfID;//自身id
	unsigned int m_nPartnerID;//独自开启时，此字段是0
	string m_strPartnerName;//独自开启时，此字段是空
	string m_strPartnerWords;//对方留言，发给房间内其他人的时候是空字符串
	list<CItem> m_listFixedReward;//固定奖励，发给房间内其他人的时候是空
	list<CItem> m_listExtraReward;//额外奖励，发给房间内其他人的时候是空
	list<CItem> m_listGiftReward;//对方送的纪念品，发给房间内其他人的时候是空
	int m_nFixedMoneyReward;//固定奖励金券
	int m_nExtraMoneyReward;//额外奖励金券
};


///////////////////////////////////////////////////////
class CAmuseEventParam_Base
{
public:
	CAmuseEventParam_Base(int nEventType);
	virtual ~CAmuseEventParam_Base();

	virtual bool doEncode(CParamPool &IOBuff) = 0;

public:
	char m_nEventType;
};


///////////////////////////////////////////////////////
class CAmuseEventParam_StartCarrousel : public CAmuseEventParam_Base
{
public:
	CAmuseEventParam_StartCarrousel();
	virtual ~CAmuseEventParam_StartCarrousel();

	virtual bool doEncode(CParamPool &IOBuff);

public:
	int m_nStartTime;
	std::string m_strStartSong;
};


///////////////////////////////////////////////////////
class CAmuseEventParam_AddExp : public CAmuseEventParam_Base
{
public:
	CAmuseEventParam_AddExp();
	virtual ~CAmuseEventParam_AddExp();

	virtual bool doEncode(CParamPool &IOBuff);

public:
	unsigned int m_nPlayerID;
	int m_nAddExp;
};


///////////////////////////////////////////////////////
class CAmuseEventParam_StartSwing : public CAmuseEventParam_Base
{
public:
	CAmuseEventParam_StartSwing();
	virtual ~CAmuseEventParam_StartSwing();

	virtual bool doEncode(CParamPool &IOBuff);

public:
	char m_nStartPower;
	int m_nStartTime;
};


///////////////////////////////////////////////////////
class CAmuseEventParam_StartBoat : public CAmuseEventParam_Base
{
public:
	CAmuseEventParam_StartBoat();
	virtual ~CAmuseEventParam_StartBoat();

	virtual bool doEncode(CParamPool &IOBuff);

public:
	char m_nDesDock;
	int m_nStartTime;
};


///////////////////////////////////////////////////////
class CAmuseActionParamFactory
{
protected:
	CAmuseActionParamFactory() {}
	~CAmuseActionParamFactory() {}

public:
	static CAmuseActionParam_Base* BuildActionParam(int nActionType);
};


///////////////////////////////////////////////////////
class PlayMakerInfo
{
public:
	PlayMakerInfo();
	PlayMakerInfo(const PlayMakerInfo &rhs);
	~PlayMakerInfo();

	void doEncode(CParamPool & IOBuff);
	void doDecode(CParamPool & IOBuff);

public:
	int m_nFsmID;
	std::string m_strFsmEventName;

	//playMaker param, key: param name, value: param value
	std::map<std::string, std::string> m_paramMap;
};


#endif
//file end

