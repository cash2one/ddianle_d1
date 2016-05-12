#include "GameMsg_Base.h"
#include "Windefine.h"
#include <stdexcept>
#include "Log.h"

GameMsg_Base::GameMsg_Base(unsigned short nMsgType)
{
	nSize = 0;
	nMsg = nMsgType;
	SessionID = 0;
	nAccount = 0;
	nIndex = 0;
}
GameMsg_Base::GameMsg_Base( const GameMsg_Base &refMsg )
{
	nSize = refMsg.nSize;
	nMsg = refMsg.nMsg;
	SessionID = refMsg.SessionID;
	nAccount = refMsg.nAccount;
	nIndex = refMsg.nIndex;
}
GameMsg_Base::~GameMsg_Base(void)
{
}

bool GameMsg_Base::encodeMssage( CParamPool &IoBuff )
{
	bool bRes = false;
	int nOldPos = IoBuff.GetCurPosition();
	IoBuff.AddShort( 0 );
	IoBuff.AddShort( nMsg );
	IoBuff.AddInt64( SessionID );
	IoBuff.AddUInt( nAccount );
	IoBuff.AddUShort(nIndex);
	if( doEncode( IoBuff ) )
	{
		bRes = true;
	}
	else
	{
		ASSERT(0);
	}

	//…Ë÷√Size;
	int nFinalPos = IoBuff.GetCurPosition();
	IoBuff.SetCurPosition( nOldPos );
	IoBuff.ReplaceShort( short(nFinalPos - nOldPos) );
	IoBuff.SetCurPosition( nFinalPos );

	return bRes;
}
bool GameMsg_Base::decodeMssage( CParamPool &IoBuff  ,unsigned short nSize)
{
	int nOldPos = 0 ;
	try
	{
		nOldPos =  IoBuff.GetCurPosition();
		SessionID = IoBuff.GetInt64();
		nAccount = IoBuff.GetUInt();
		nIndex = IoBuff.GetUShort();
		if ( doDecode( IoBuff ) )
		{
			return true;
			//DataIn.Postion = DataIn.Postion + nSize - m_nMsgType ;
		}
		else
		{
			IoBuff.SetCurPosition( nOldPos + nSize - 4 );
		}
	}
	catch( overflow_error e )
	{
		int nDecodePacketSize = IoBuff.GetCurPosition() - nOldPos;
		WriteLog(LOGLEVEL_ERROR, "Decode Message ID = %d Error,ParamPool overflow : %s , decodeSize = %d , nSize = %d ." , (int)nMsg , e.what() , nDecodePacketSize , (int)nSize);
	}
	return false;
}

bool GameMsg_Base::doEncode( CParamPool &IoBuff )
{
	ASSERT(0);
	return false;
}
bool GameMsg_Base::doDecode( CParamPool &IoBuff )
{
	ASSERT(0);
	return false;
}



//file end

