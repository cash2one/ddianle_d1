#ifndef __GameMsg_S2C_ROOMREPORTPERRESULT_H__
#define __GameMsg_S2C_ROOMREPORTPERRESULT_H__

#include "../socket/GameMsg_Base.h"

class GameMsg_S2C_RoomReportPerResult :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GameMsg_S2C_RoomReportPerResult(void);
	~GameMsg_S2C_RoomReportPerResult(void);

	unsigned int nRoleId;
	unsigned int nResult;	//�ܷ���

	// -3 ��ʾ�˽�ʧ��
	// -2 ��ʾNormal
	// -1 ��ʾGreat
	//  0 ��ʾʲô��û��
	//  1 ��ʾPerfect
	// >1 ��ʾ����Perfect
	char nSate;				//״̬

public :
	virtual bool doEncode( CParamPool &IoBuff );
};

#endif


