#ifndef __GAMEMSG_C2S_ENTERROOMRESULT_H__
#define __GAMEMSG_C2S_ENTERROOMRESULT_H__

#include "../socket/gamemsg_base.h"
#include <string>

enum
{
	EnterRoomResult_Success,
	EnterRoomResult_InvalidPassWord,		//密码不对
	EnterRoomResult_RoomNotExist,			//所选房间不存在
	EnterRoomResult_PlayerAlreadyInRoom,	//角色已经在房间内，不能再进入房间
	EnterRoomResult_RoomIsFull,				//房间已满
	EnterRoomResult_MatchStarted,			//房间已经开始比赛
	EnterRoomResult_NotEnoughLevel,			//级别不够，不能满足进入该区域房间的条件
	EnterRoomResult_NotEnoughGold,			//进钱不够，不能满足进入该区域房间的条件
	EnterRoomResult_QuickStartFailed,		//快速进入，没有找到合适的房间
	EnterRoomResult_OtherError,				//其他错误
};

class GAMEMSG_S2C_EnterRoomResult :
	public GAMEMSG_BASE
{
	typedef  GAMEMSG_BASE _baseClass;
public:
	GAMEMSG_S2C_EnterRoomResult(void);
	~GAMEMSG_S2C_EnterRoomResult(void);

	unsigned char m_nResult;
	std::string m_strErrorMsg;
public:
	virtual bool doEncode( CParamPool& IoBuff );
};
#endif


