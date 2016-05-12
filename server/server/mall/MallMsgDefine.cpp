#include "MallMsgDefine.h"
#include "../share/ServerMsgDef.h"
#include "Mall.h"
#include "../../datastructure/datainfor/ConfigManager.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_C2S_MallGoodsInfo
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_C2S_MallGoodsInfo::GameMsg_C2S_MallGoodsInfo(void):_baseClass(MSG_C2S_MALL_GETGOODS)
{
	nType = 0;
}

GameMsg_C2S_MallGoodsInfo::~GameMsg_C2S_MallGoodsInfo(void)
{
}

bool GameMsg_C2S_MallGoodsInfo::doDecode( CParamPool& IOBuff )
{
	nType = IOBuff.GetUChar();
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_MallGoodsInfo
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_MallGoodsInfo::GameMsg_S2C_MallGoodsInfo(void):_baseClass(MSG_S2C_MALL_GETGOODSRESULT)
{
	nDataSize = 0;
	pData = NULL;
}

GameMsg_S2C_MallGoodsInfo::~GameMsg_S2C_MallGoodsInfo(void)
{
	if (pData != NULL)
	{
		delete []pData;
		pData = NULL;
	}
}

bool GameMsg_S2C_MallGoodsInfo::doEncode( CParamPool& IOBuff )
{
	if (pData != NULL && nDataSize > 0)
	{
		IOBuff.AddMemory(pData,nDataSize);
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_C2S_MallSomeGoods
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_C2S_MallSomeGoods::GameMsg_C2S_MallSomeGoods(void):_baseClass(MSG_C2S_MALL_GETSOMEGOODS)
{
}

GameMsg_C2S_MallSomeGoods::~GameMsg_C2S_MallSomeGoods(void)
{
}

bool GameMsg_C2S_MallSomeGoods::doDecode( CParamPool& IOBuff )
{
	unsigned short nCount = IOBuff.GetUShort();
	if(nCount > 0)
    {
		for (unsigned short id = 0 ; id < nCount ; id++)
		{
			itemtype_t nType = IOBuff.GetUInt();
			aListType.push_back(nType);
		}
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_MallSomeGoodsResult
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_MallSomeGoodsResult::GameMsg_S2C_MallSomeGoodsResult(void):_baseClass(MSG_S2C_MALL_GETSOMEGOODSRESULT)
{
}

GameMsg_S2C_MallSomeGoodsResult::~GameMsg_S2C_MallSomeGoodsResult(void)
{
}

bool GameMsg_S2C_MallSomeGoodsResult::doEncode( CParamPool& IOBuff )
{
	unsigned short nCount = (unsigned short)aGoodMap.Count();
	IOBuff.AddUShort(nCount);
	if (nCount > 0)
	{
		list<itemtype_t> & listGoods = aGoodMap.GetKeyList();
		list<itemtype_t>::iterator it = listGoods.begin();
		for (;it != listGoods.end();it++)
		{	
			MallGood * pGood = aGoodMap.Find(*it);
			if(pGood != NULL){
				pGood->doEncode(IOBuff);
			}
			else
				return false;
		}
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_C2S_MallBuy
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_C2S_MallBuy::GameMsg_C2S_MallBuy(void):_baseClass(MSG_C2S_MALL_BUY)
{
	m_nBuyFromType = 0;
}

GameMsg_C2S_MallBuy::~GameMsg_C2S_MallBuy(void)
{
}

bool GameMsg_C2S_MallBuy::doDecode( CParamPool& IOBuff )
{
	vecGoods.clear();
	m_nBuyFromType = IOBuff.GetUChar();
	unsigned short nCount = IOBuff.GetUShort();
	for (unsigned short i = 0 ; i < nCount ; i++)
	{
		PeddleGood aPeddleGood;
		aPeddleGood.doDecode(IOBuff);
		vecGoods.push_back(aPeddleGood);
	}
	return true;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_MallMallBuyResult
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_MallMallBuyResult::GameMsg_S2C_MallMallBuyResult(void):_baseClass(MSG_S2C_MALL_BUYRESULT)
{
	m_nBuyFromType = 0;
	nResult = MALL_OK;
	strReturn = "";
}

GameMsg_S2C_MallMallBuyResult::~GameMsg_S2C_MallMallBuyResult(void)
{
}

bool GameMsg_S2C_MallMallBuyResult::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUChar(m_nBuyFromType);
	IOBuff.AddUChar(nResult);
	IOBuff.AddStringW(strReturn.c_str());
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_C2S_MallSend
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_C2S_MallSend::GameMsg_C2S_MallSend(void):_baseClass(MSG_C2S_MALL_SEND)
{
	m_nDestRoleID = 0;
}

GameMsg_C2S_MallSend::~GameMsg_C2S_MallSend(void)
{
}

bool GameMsg_C2S_MallSend::doDecode( CParamPool& IOBuff )
{
	vecGoods.clear();
	m_nDestRoleID = IOBuff.GetUInt();
	unsigned short nCount = IOBuff.GetUShort();
	for (unsigned short i = 0 ; i < nCount ; i++)
	{
		PeddleGood aPeddleGood;
		aPeddleGood.doDecode(IOBuff);
		vecGoods.push_back(aPeddleGood);
	}
	
	std::string strTemp;
	IOBuff.GetStringW(strTemp);
	if (strTemp.size() > SEND_GIFT_MESSAGE_LEN  - 1) {
		m_strInfo.assign(strTemp.c_str(), SEND_GIFT_MESSAGE_LEN - 1);
	} else {
		m_strInfo = strTemp;
	}

	return true;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_MallMallSendResult
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_MallMallSendResult::GameMsg_S2C_MallMallSendResult(void):_baseClass(MSG_S2C_MALL_SENDRESULT)
{
	nResult = MALL_OK;
	strReturn = "";
}

GameMsg_S2C_MallMallSendResult::~GameMsg_S2C_MallMallSendResult(void)
{
}

bool GameMsg_S2C_MallMallSendResult::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUChar(nResult);
	IOBuff.AddStringW(strReturn.c_str());
	return true;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_C2S_MallAskFor
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_C2S_MallAskFor::GameMsg_C2S_MallAskFor(void):_baseClass(MSG_C2S_MALL_ASKFOR)
{
	m_nDestRoleID = 0;
	m_strInfo = "";
}

GameMsg_C2S_MallAskFor::~GameMsg_C2S_MallAskFor(void)
{
}

bool GameMsg_C2S_MallAskFor::doDecode( CParamPool& IOBuff )
{
	m_nDestRoleID = IOBuff.GetUInt();

	int nSize = IOBuff.GetUShort();
	while(nSize-- > 0) {
		CAskMallGoodInfo good;
		good.doDecode(IOBuff);
		m_listAskGoods.push_back(good);
	}

	std::string strTemp;
	IOBuff.GetStringW(strTemp);
	if (strTemp.size() > SEND_GIFT_MESSAGE_LEN  - 1) {
		m_strInfo.assign(strTemp.c_str(), SEND_GIFT_MESSAGE_LEN - 1);
	} else {
		m_strInfo = strTemp;
	}

	return true;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_MallMallSendResult
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_MallMallAskForResult::GameMsg_S2C_MallMallAskForResult(void):_baseClass(MSG_S2C_MALL_ASKFORRESULT)
{
	nResult = MALL_OK;
	strReturn = "";
}

GameMsg_S2C_MallMallAskForResult::~GameMsg_S2C_MallMallAskForResult(void)
{
}

bool GameMsg_S2C_MallMallAskForResult::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUChar(nResult);
	IOBuff.AddStringW(strReturn.c_str());
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_MallMallBeAskedFor
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_MallMallBeAskedFor::GameMsg_S2C_MallMallBeAskedFor(void):_baseClass(MSG_S2C_MALL_BEASKED)
{
	memset(szAskForRoleName,0,MAX_NAME_LEN);
	nAskForRoleID = 0;
}

GameMsg_S2C_MallMallBeAskedFor::~GameMsg_S2C_MallMallBeAskedFor(void)
{
}

bool GameMsg_S2C_MallMallBeAskedFor::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddStringW(szAskForRoleName);
	IOBuff.AddUInt(nAskForRoleID);

	IOBuff.AddUShort((unsigned short)m_listAskGoodInfo.size());
	for (list<CAskMallGoodInfo>::iterator itr = m_listAskGoodInfo.begin(); itr != m_listAskGoodInfo.end(); ++itr) {
		itr->doEncode(IOBuff);
	}

	unsigned short nCount = (unsigned short)aGoodMap.Count();
	IOBuff.AddUShort(nCount);
	if (nCount > 0)
	{
		list<itemtype_t> & listGoods = aGoodMap.GetKeyList();
		list<itemtype_t>::iterator it = listGoods.begin();
		for (;it != listGoods.end();it++)
		{	
			MallGood * pGood = aGoodMap.Find(*it);
			if(pGood != NULL){
				pGood->doEncode(IOBuff);
			}
			else
				return false;
		}
	}
	IOBuff.AddStringW(m_strInfo.c_str());
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_C2S_MallDesire
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_C2S_MallDesire::GameMsg_C2S_MallDesire(void):_baseClass(MSG_C2S_Desire)
{
	m_nTypeID = 0;
}

GameMsg_C2S_MallDesire::~GameMsg_C2S_MallDesire(void)
{
}

bool GameMsg_C2S_MallDesire::doDecode( CParamPool& IOBuff )
{
	m_nTypeID = IOBuff.GetUInt();
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_C2S_RequestDesireList::GameMsg_C2S_RequestDesireList(void) :_baseClass(MSG_C2S_RequestDesireList)
{
}

GameMsg_C2S_RequestDesireList::~GameMsg_C2S_RequestDesireList(void)
{
}

bool GameMsg_C2S_RequestDesireList::doDecode(CParamPool& IOBuff)
{
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_RequestDesireListResult::GameMsg_S2C_RequestDesireListResult(void) :_baseClass(MSG_S2C_RequestDesireListResult)
{
}

GameMsg_S2C_RequestDesireListResult::~GameMsg_S2C_RequestDesireListResult(void)
{
}

bool GameMsg_S2C_RequestDesireListResult::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_listDesire.size());
    std::list<itemtype_t>::iterator it = m_listDesire.begin();
    for (; it != m_listDesire.end(); ++it)
    {
        IOBuff.AddUInt(*it);
    }

    return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_MallDesireResult
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_MallDesireResult::GameMsg_S2C_MallDesireResult(void):_baseClass(MSG_S2C_DesireResult)
{
	nResult = MALL_OK;
	strReturn = "";
}

GameMsg_S2C_MallDesireResult::~GameMsg_S2C_MallDesireResult(void)
{
}

bool GameMsg_S2C_MallDesireResult::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUChar(nResult);
	IOBuff.AddStringW(strReturn.c_str());
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_C2S_RemoveDesire
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_C2S_RemoveDesire::GameMsg_C2S_RemoveDesire()
:GameMsg_Base(MSG_C2S_RemoveDesire)
,m_nTypeID(0)
{

}

GameMsg_C2S_RemoveDesire::~GameMsg_C2S_RemoveDesire()
{

}

bool GameMsg_C2S_RemoveDesire::doDecode(CParamPool & IOBuff)
{
	m_nTypeID = IOBuff.GetUInt();

	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_RemoveDesireItemFail
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_RemoveDesireItemFail::GameMsg_S2C_RemoveDesireItemFail()
:GameMsg_Base(MSG_S2C_RemoveDesireFail)
,m_strEorror("")
{

}

GameMsg_S2C_RemoveDesireItemFail::~GameMsg_S2C_RemoveDesireItemFail()
{

}

bool GameMsg_S2C_RemoveDesireItemFail::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddStringW(m_strEorror.c_str());
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_RemoveDesireScuess::GameMsg_S2C_RemoveDesireScuess()
:GameMsg_Base(MSG_S2C_RemoveDesireScuess)
, m_nItemType(0)
{

}

GameMsg_S2C_RemoveDesireScuess::~GameMsg_S2C_RemoveDesireScuess()
{

}

bool GameMsg_S2C_RemoveDesireScuess::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddUInt(m_nItemType);
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_C2S_RenewItem
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_C2S_RenewItem::GameMsg_C2S_RenewItem(void):_baseClass(MSG_C2S_MALL_RENEW)
{
	nColumn = 0;
	nIndex = 0;
	m_bFirstUseChange = false;
}

GameMsg_C2S_RenewItem::~GameMsg_C2S_RenewItem(void)
{
}

bool GameMsg_C2S_RenewItem::doDecode( CParamPool& IOBuff )
{
	nColumn = IOBuff.GetUShort();
	nIndex = IOBuff.GetUShort();

	m_bFirstUseChange = IOBuff.GetBool();
	unsigned short nCount = IOBuff.GetUShort();
	for (unsigned short i = 0 ; i < nCount ; i++)
	{
		PeddleGood aPeddleGood;
		aPeddleGood.doDecode(IOBuff);
		vecGoods.push_back(aPeddleGood);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_MallRenewResult
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_MallRenewResult::GameMsg_S2C_MallRenewResult(void):_baseClass(MSG_S2C_MALL_RENEWRESULT)
{
	nResult = MALL_OK;
	strReturn = "";
}

GameMsg_S2C_MallRenewResult::~GameMsg_S2C_MallRenewResult(void)
{
}

bool GameMsg_S2C_MallRenewResult::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUChar(nResult);
	IOBuff.AddStringW(strReturn.c_str());
	return true;
}

GameMsg_C2S_SendWishItem::GameMsg_C2S_SendWishItem():
GameMsg_Base(MSG_C2S_MALL_SENDWISHITEM)
{
}

GameMsg_C2S_SendWishItem::~GameMsg_C2S_SendWishItem()
{
}

bool GameMsg_C2S_SendWishItem::doDecode(CParamPool & IOBuff)
{
	m_vecGoods.clear();
	m_nDestRoleID = IOBuff.GetUInt();
	m_bFirstUseChange = IOBuff.GetBool();
	unsigned short nCount = IOBuff.GetUShort();
	for (unsigned short i = 0 ; i < nCount ; i++)
	{
		PeddleGood aPeddleGood;
		aPeddleGood.doDecode(IOBuff);
		m_vecGoods.push_back(aPeddleGood);
	}

	std::string strTemp;
	IOBuff.GetStringW(strTemp);
	if (strTemp.size() > SEND_GIFT_MESSAGE_LEN  - 1) {
		m_strInfo.assign(strTemp.c_str(), SEND_GIFT_MESSAGE_LEN - 1);
	} else {
		m_strInfo = strTemp;
	}

	return true;
}

GameMsg_S2C_SendWishItemSuc::GameMsg_S2C_SendWishItemSuc():
GameMsg_Base(MSG_S2C_MALL_SENDWISHITEM_SUC),
m_nRoleID(0),
m_nItemID(0)
{
}

GameMsg_S2C_SendWishItemSuc::~GameMsg_S2C_SendWishItemSuc()
{
}

bool GameMsg_S2C_SendWishItemSuc::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddUInt(m_nItemID);

	return true;
}

GameMsg_S2C_SendWishItemFail::GameMsg_S2C_SendWishItemFail():
GameMsg_Base(MSG_S2C_MALL_SENDWISHITEM_FAIL),
m_strErrorMsg("")
{
}

GameMsg_S2C_SendWishItemFail::~GameMsg_S2C_SendWishItemFail()
{
}

bool GameMsg_S2C_SendWishItemFail::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW(m_strErrorMsg.c_str());

	return true;
}


GameMsg_C2S_BeAskedFor_Send::GameMsg_C2S_BeAskedFor_Send()
:GameMsg_Base(MSG_C2S_MALL_BEASKED_SEND)
,m_nDestRoleID(0)
{

}

GameMsg_C2S_BeAskedFor_Send::~GameMsg_C2S_BeAskedFor_Send()
{

}

bool GameMsg_C2S_BeAskedFor_Send::doDecode( CParamPool &IOBuff )
{
	m_nDestRoleID = IOBuff.GetUInt();
	unsigned short nCount = IOBuff.GetUShort();
	for (unsigned short i = 0 ; i < nCount ; i++)
	{
		PeddleGood aPeddleGood;
		aPeddleGood.doDecode(IOBuff);
		vecGoods.push_back(aPeddleGood);
	}

	IOBuff.GetStringW(m_strInfo);
	return true;
}

GameMsg_S2C_BeAskedFor_SendRes::GameMsg_S2C_BeAskedFor_SendRes()
:GameMsg_Base(MSG_S2C_MALL_BEASKED_SENDRESULT)
,m_nResult(0)
,m_strReturn("")
{

}

GameMsg_S2C_BeAskedFor_SendRes::~GameMsg_S2C_BeAskedFor_SendRes()
{

}

bool GameMsg_S2C_BeAskedFor_SendRes::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUChar(m_nResult);
	IOBuff.AddStringW(m_strReturn.c_str());

	return true;
}



GameMsg_C2S_EnterMall::GameMsg_C2S_EnterMall()
:GameMsg_Base(MSG_C2S_MALL_EnterMall)
{

}

GameMsg_C2S_EnterMall::~GameMsg_C2S_EnterMall()
{

}

bool GameMsg_C2S_EnterMall::doDecode( CParamPool & IOBuff )
{
	return true;
}


GameMsg_S2C_EnterMallRes::GameMsg_S2C_EnterMallRes()
:GameMsg_Base(MSG_S2C_MALL_EnterMallRes)
,m_bNotifyWillResetMoney(false)
{

}

GameMsg_S2C_EnterMallRes::~GameMsg_S2C_EnterMallRes()
{

}

bool GameMsg_S2C_EnterMallRes::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddBool(m_bNotifyWillResetMoney);
	IOBuff.AddStringW(m_strMallTipTitle.c_str());
	IOBuff.AddStringW(m_strMallTip.c_str());

	return true;
}

GameMsg_S2C_AllGoodsInfo::GameMsg_S2C_AllGoodsInfo()
:GameMsg_Base(MSG_S2C_MALL_AllGoodsInfo)
{
	m_nMessageCount = 0;
	m_nMessageIndex = 0;
	m_nGoodsCount = 0;
}
GameMsg_S2C_AllGoodsInfo::~GameMsg_S2C_AllGoodsInfo(void)
{

}

bool GameMsg_S2C_AllGoodsInfo::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUShort(m_nMessageCount);
	IOBuff.AddUShort(m_nMessageIndex);
	
	int nCount = 0;	// 实际发的商品个数
	unsigned int nOldPos = IOBuff.GetCurPosition();
	IOBuff.AddUShort(m_nGoodsCount);
	
	for (list<itemtype_t>::iterator it = m_listGoods.begin();
		it != m_listGoods.end() && nCount < m_nGoodsCount;
		it++)
	{
		ItemConfig * pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(*it);	
		MallGood * pGood = CMall::Instance().FindGoods(*it);

		if (pItemInfo != NULL && pGood != NULL && pGood->m_nState == 1 && pGood->m_nType > 0)
		{
			pGood->doEncode(IOBuff);
			++nCount;
		}
	}

	if (nCount != m_nGoodsCount)
	{
		unsigned int nFinalPos = IOBuff.GetCurPosition();
		IOBuff.SetCurPosition( nOldPos );
		IOBuff.ReplaceShort((short)nCount);
		IOBuff.SetCurPosition( nFinalPos );
	}

	return true;
}


GameMsg_C2S_ActionEnterMall::GameMsg_C2S_ActionEnterMall()
    :GameMsg_Base(MSG_C2S_MALLAction_EnterMall)
{

}

GameMsg_C2S_ActionEnterMall::~GameMsg_C2S_ActionEnterMall()
{

}

bool GameMsg_C2S_ActionEnterMall::doDecode( CParamPool & IOBuff )
{
    return true;
}
GameMsg_C2S_ExperienceCardInfo::GameMsg_C2S_ExperienceCardInfo(void)
	: GameMsg_Base(MSG_C2S_MallExperienceCardInfo)
{

}

GameMsg_C2S_ExperienceCardInfo::~GameMsg_C2S_ExperienceCardInfo(void)
{

}

bool GameMsg_C2S_ExperienceCardInfo::doDecode( CParamPool& IOBuff )
{
	return true;
}

GameMsg_S2C_ExperienceCardInfoRes::GameMsg_S2C_ExperienceCardInfoRes()
	: GameMsg_Base(MSG_S2C_MallExperienceCardInfoRes)
	, m_usedTime(0)
	, m_nCountDown(0)
{
}

GameMsg_S2C_ExperienceCardInfoRes::~GameMsg_S2C_ExperienceCardInfoRes(void)
{


}

bool GameMsg_S2C_ExperienceCardInfoRes::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUShort((unsigned short)m_vecExperiencedItem.size());
	for (size_t i = 0; i < m_vecExperiencedItem.size(); ++i)
	{
		IOBuff.AddUInt(m_vecExperiencedItem[i]);
	}
	IOBuff.AddUShort(m_usedTime);
	IOBuff.AddUInt(m_nCountDown);

	return true;
}


GameMsg_S2C_ExperiencedItemNtf::GameMsg_S2C_ExperiencedItemNtf(void)
	: GameMsg_Base(MSG_S2C_MallExperienceCardInfoNtf)
	, m_usedTime(0)
	, m_nCountDown(0)
{

}

GameMsg_S2C_ExperiencedItemNtf::~GameMsg_S2C_ExperiencedItemNtf(void)
{

}

bool GameMsg_S2C_ExperiencedItemNtf::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUShort((unsigned short)m_vecExperiencedItemNew.size());
	for (size_t i = 0; i < m_vecExperiencedItemNew.size(); ++i)
	{
		IOBuff.AddUInt(m_vecExperiencedItemNew[i]);
	}
	IOBuff.AddUShort(m_usedTime);
	IOBuff.AddUInt(m_nCountDown);
	return true;
}



