#ifndef __GAMEMSG_PLAYER_SWITCHLINE_H__
#define __GAMEMSG_PLAYER_SWITCHLINE_H__

#include <string>
#include "../../datastructure/DataStruct_DB.h"
#include "../../socket/GameMsg_Base.h"

using namespace std;

class GameMsg_C2S_SwitchLine :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_SwitchLine);
	GameMsg_C2S_SwitchLine(int nLine = -1, unsigned char nLocation = 0);
	~GameMsg_C2S_SwitchLine();
public:
	virtual bool doDecode( CParamPool& IoBuff );
public:
	int m_nLine;
	unsigned char m_nLoaction;	// ��û�м�¼���λ�õ�ǰ���¸��ֶ����� 0��ʾ�ڴ���
};


class GameMsg_S2C_SwitchLineRes:
	public GameMsg_Base
{
public:
	GameMsg_S2C_SwitchLineRes(int nDstLine = 0, unsigned char nLocation = 0, unsigned char nResult = 0);
	~GameMsg_S2C_SwitchLineRes();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	unsigned char m_nResult;	// 0�ɹ�,1��ʾ�л�ʧ��
	int m_nDstLine;
	unsigned char m_nLocation;
};

// S��ʾ��Ϸ������,A��ʾ�˺ŷ�����,G��ʾ���������
class GameMsg_S2A_PlayerSwithLine:
	public GameMsg_Base
{
public:
	GameMsg_S2A_PlayerSwithLine(int nSrcLine = 0, int nDstLine = 0);
	~GameMsg_S2A_PlayerSwithLine();
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2A_PlayerSwithLine);
	bool doEncode(CParamPool & IOBuff);
	bool doDecode(CParamPool & IOBuff);
private:
	int m_nSrcLine;
	int m_nDstLine;
};





#endif






