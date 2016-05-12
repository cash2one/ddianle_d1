/*
 * GameMsg_SpecialMall.h
 * Description: �����̵�Э��
 * Copyright (C) 2014 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2014-12-10 11:37:32
 */

#ifndef __GAMEMSG_SPECIALMALL_H__
#define __GAMEMSG_SPECIALMALL_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_SpecialMall.h"

//////////////////////////////////////////////////////////////////////////

enum ESpecialMallState
{
    ESpecialMallState_None = 0,
    ESpecialMallState_ComingSoon,       // ������ʼ
    ESpecialMallState_OnSale,           // ������
    ESpecialMallState_Finished,         // �ѽ���
};

enum ESpecialMallError
{
    ESpecialMallError_None = 0,         // ����״̬
    ESpecialMallError_ValidParam,       // ��������ȷ
    ESpecialMallError_NotOpen,          // δ����
    ESpecialMallError_NotFound,         // �޸���Ʒ
    ESpecialMallError_Exist,            // �Ѵ���������Ʒ
    ESpecialMallError_SoldOut,          // ������
    ESpecialMallError_SexNotMatch,      // �Ա𲻷�
    ESpecialMallError_MoneyNotEnough,   // ��Ǯ����
    ESpecialMallError_AlreadyBuy,       // �Ѿ������
    ESpecialMallError_BidNotEnough,     // ����̫��
    ESpecialMallError_BidTimeOut,       // ���Ľ���
    ESpecialMallError_BidAlreadyTop,    // ��߳�����
    ESpecialMallError_CanNotBuyBadgeCard,// �޷�����û��¿�
};

//////////////////////////////////////////////////////////////////////////

/**
* GameMsg_C2S_SpecialMall_State:
* ��ȡ�����̵�״̬
*/

class GameMsg_C2S_SpecialMall_State : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_SpecialMall_State);
    GameMsg_C2S_SpecialMall_State();
    ~GameMsg_C2S_SpecialMall_State() {}

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);
};

//////////////////////////////////////////////////////////////////////////

/**
* GameMsg_S2C_SpecialMall_StateResult:
* ��ȡ�����̵�״̬���
*/

class GameMsg_S2C_SpecialMall_StateResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_SpecialMall_StateResult);
    GameMsg_S2C_SpecialMall_StateResult();
    ~GameMsg_S2C_SpecialMall_StateResult() {}

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char   m_nGroupBuyingState;
    std::string     m_strGroupBuyingTitle;

    unsigned char   m_nLimitedEditionState;
    std::string     m_strLimitedEditionTitle;

    unsigned char   m_nAuctionState;
    std::string     m_strAuctionTitle;
    std::string     m_strAuctionRule;
};

//////////////////////////////////////////////////////////////////////////

/**
* GameMsg_C2S_GroupBuying_Items:
* ��ȡ�Ź���Ʒ
*/

class GameMsg_C2S_GroupBuying_Items : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GroupBuying_Items);
    GameMsg_C2S_GroupBuying_Items();
    ~GameMsg_C2S_GroupBuying_Items() {}

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

public:
    bool            m_bSearch;      // �Ƿ��ǲ�ѯ
    std::string     m_strSearchKey; // �ؼ���
    unsigned short  m_uNextPage;    // ��ҳҳ��
    unsigned short  m_uCount;       // ÿҳ����
};

//////////////////////////////////////////////////////////////////////////

/**
* GameMsg_S2C_GroupBuying_ItemsResult:
* ��ȡ�Ź���Ʒ���
*/

class GameMsg_S2C_GroupBuying_ItemsResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GroupBuying_ItemsResult);
    GameMsg_S2C_GroupBuying_ItemsResult();
    ~GameMsg_S2C_GroupBuying_ItemsResult() {}

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char                       m_nState;
    float                               m_fCD;
    bool                                m_bSearch;
    std::string                         m_strSearchKey;
    unsigned short                      m_uCurPage;
    unsigned short                      m_uTotalCount;
    std::list<CGroupBuyingItemData>     m_listItems;
};

//////////////////////////////////////////////////////////////////////////

/**
* GameMsg_C2S_GroupBuying_Buy:
* �����Ź���Ʒ
*/

class GameMsg_C2S_GroupBuying_Buy : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GroupBuying_Buy);
    GameMsg_C2S_GroupBuying_Buy();
    ~GameMsg_C2S_GroupBuying_Buy() {}

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned short  m_uIdx;
    itemtype_t  m_uGoodsID;
};

//////////////////////////////////////////////////////////////////////////

/**
* GameMsg_S2C_GroupBuying_BuyResult:
* �����Ź���Ʒ���
*/

class GameMsg_S2C_GroupBuying_BuyResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GroupBuying_BuyResult);
    GameMsg_S2C_GroupBuying_BuyResult();
    ~GameMsg_S2C_GroupBuying_BuyResult() {}

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char   m_nErrorType;
    CItem           m_cItem;
};

//////////////////////////////////////////////////////////////////////////

/**
* GameMsg_C2S_LimitedEdition_Items:
* ��ȡ����������Ʒ
*/

class GameMsg_C2S_LimitedEdition_Items : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_LimitedEdition_Items);
    GameMsg_C2S_LimitedEdition_Items();
    ~GameMsg_C2S_LimitedEdition_Items() {}

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

public:
    bool            m_bSearch;      // �Ƿ��ǲ�ѯ
    std::string     m_strSearchKey; // �ؼ���
    unsigned short  m_uNextPage;    // ��ҳҳ��
    unsigned short  m_uCount;       // ÿҳ����
};

//////////////////////////////////////////////////////////////////////////

/**
* GameMsg_S2C_LimitedEdition_ItemsResult:
* ��ȡ����������Ʒ���
*/

class GameMsg_S2C_LimitedEdition_ItemsResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_LimitedEdition_ItemsResult);
    GameMsg_S2C_LimitedEdition_ItemsResult();
    ~GameMsg_S2C_LimitedEdition_ItemsResult() {}

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char                       m_nState;
    float                               m_fCD;
    bool                                m_bSearch;
    std::string                         m_strSearchKey;
    unsigned short                      m_uCurPage;
    unsigned short                      m_uTotalCount;
    std::list<CLimitedEditionItemData>  m_listItems;
};

//////////////////////////////////////////////////////////////////////////

/**
* GameMsg_S2C_LimitedEdition_Buy:
* ��������������Ʒ
*/

class GameMsg_C2S_LimitedEdition_Buy : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_LimitedEdition_Buy);
    GameMsg_C2S_LimitedEdition_Buy();
    ~GameMsg_C2S_LimitedEdition_Buy() {}

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned short  m_uIdx;
    itemtype_t      m_uGoodsID;
};

//////////////////////////////////////////////////////////////////////////

/**
* GameMsg_S2C_LimitedEdition_BuyResult:
* ��������������Ʒ���
*/

class GameMsg_S2C_LimitedEdition_BuyResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_LimitedEdition_BuyResult);
    GameMsg_S2C_LimitedEdition_BuyResult();
    ~GameMsg_S2C_LimitedEdition_BuyResult() {}

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char   m_nErrorType;
    CItem           m_cItem;
};

//////////////////////////////////////////////////////////////////////////

/**
* GameMsg_C2S_Auction_Items:
* ��ȡ��������Ʒ
*/

class GameMsg_C2S_Auction_Items : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_Auction_Items);
    GameMsg_C2S_Auction_Items();
    ~GameMsg_C2S_Auction_Items() {}

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

public:
    bool            m_bSearch;      // �Ƿ��ǲ�ѯ
    std::string     m_strSearchKey; // �ؼ���
    unsigned short  m_uNextPage;    // ��ҳҳ��
    unsigned short  m_uCount;       // ÿҳ����
};

//////////////////////////////////////////////////////////////////////////

/**
* GameMsg_S2C_Auction_ItemsResult:
* ��ȡ��������Ʒ���
*/

class GameMsg_S2C_Auction_ItemsResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_Auction_ItemsResult);
    GameMsg_S2C_Auction_ItemsResult();
    ~GameMsg_S2C_Auction_ItemsResult() {}

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char                       m_nState;
    float                               m_fCD;
    bool                                m_bSearch;
    std::string                         m_strSearchKey;
    unsigned short                      m_uCurPage;
    unsigned short                      m_uTotalCount;
    std::list<CAuctionItemData>         m_listItems;
};

//////////////////////////////////////////////////////////////////////////

/**
* GameMsg_S2C_Auction_Bid:
* ���ľ�������Ʒ
*/

class GameMsg_C2S_Auction_Bid : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_Auction_Bid);
    GameMsg_C2S_Auction_Bid();
    ~GameMsg_C2S_Auction_Bid() {}

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned short  m_uIdx;
    itemtype_t      m_uGoodsID;
    unsigned int    m_uPrice;
};

//////////////////////////////////////////////////////////////////////////

/**
* GameMsg_S2C_Auction_BidResult:
* ���ľ�������Ʒ���
*/

class GameMsg_S2C_Auction_BidResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_Auction_BidResult);
    GameMsg_S2C_Auction_BidResult();
    ~GameMsg_S2C_Auction_BidResult() {}

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char   m_nErrorType;
    unsigned short  m_uIdx;
    CItem           m_cItem;
};

//////////////////////////////////////////////////////////////////////////

/**
* GameMsg_S2C_Auction_Surpassed:
* �����г���֪ͨ
*/

class GameMsg_S2C_Auction_Surpassed : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_Auction_Surpassed);
    GameMsg_S2C_Auction_Surpassed();
    ~GameMsg_S2C_Auction_Surpassed() {}

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned short  m_uIdx;
    CItem           m_cItem;
};

//////////////////////////////////////////////////////////////////////////

/**
* GameMsg_S2C_Auction_BidSuccess:
* ���ĳɹ�
*/

class GameMsg_S2C_Auction_BidSuccess : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_Auction_BidSuccess);
    GameMsg_S2C_Auction_BidSuccess();
    ~GameMsg_S2C_Auction_BidSuccess() {}

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

public:
    CItem           m_cItem;
};

//////////////////////////////////////////////////////////////////////////

/**
* GameMsg_G2S_SpecialMall_Pay:
* ����֧��
*/

class GameMsg_G2S_SpecialMall_Pay : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_SpecialMall_Pay);
    GameMsg_G2S_SpecialMall_Pay();
    ~GameMsg_G2S_SpecialMall_Pay() {}

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char   m_nSpecialMallType;
    unsigned short  m_uRange;
    unsigned short  m_uIdx;
    CItem           m_cItem;
    unsigned int    m_uBuyPrice;
    unsigned char   m_nBuyType;
    time_t          m_tEndTime;
    std::string     m_strMailTitle;
    std::string     m_strMailContent;
};

//////////////////////////////////////////////////////////////////////////

/**
* GameMsg_S2G_SpecialMall_PayResult:
* ֧�����
*/

class GameMsg_S2G_SpecialMall_PayResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_SpecialMall_PayResult);
    GameMsg_S2G_SpecialMall_PayResult();
    ~GameMsg_S2G_SpecialMall_PayResult() {}

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char   m_nSpecialMallType;
    unsigned short  m_uRange;
    unsigned short  m_uIdx;
    bool            m_bSuccess;
};

//////////////////////////////////////////////////////////////////////////

/**
* GameMsg_G2S_Auction_UpdateState:
* ���¾���״̬
*/

class GameMsg_G2S_Auction_UpdateState : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_Auction_UpdateState);
    GameMsg_G2S_Auction_UpdateState();
    ~GameMsg_G2S_Auction_UpdateState() {}

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned short  m_uIdx;
    unsigned int    m_uCurPrice;
    bool            m_bTimeOut;
};

//////////////////////////////////////////////////////////////////////////

/**
* GameMsg_G2S_Auction_BidSuccess:
* ���ĳɹ�
*/

class GameMsg_G2S_Auction_BidSuccess : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_Auction_BidSuccess);
    GameMsg_G2S_Auction_BidSuccess();
    ~GameMsg_G2S_Auction_BidSuccess() {}

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned short  m_uIdx;
    CItem           m_cItem;
    unsigned int    m_uPrice;
};

//////////////////////////////////////////////////////////////////////////

/**
* GameMsg_G2S_Auction_BidResult:
* ���ľ�������Ʒ���
*/

class GameMsg_G2S_Auction_BidResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_Auction_BidResult);
    GameMsg_G2S_Auction_BidResult();
    ~GameMsg_G2S_Auction_BidResult() {}

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char   m_nErrorType;
    unsigned short  m_uIdx;
    CItem           m_cItem;
    unsigned int    m_uPrice;
};

//////////////////////////////////////////////////////////////////////////

/**
* GameMsg_G2S_Auction_Surpassed:
* �����г���֪ͨ
*/

class GameMsg_G2S_Auction_Surpassed : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_Auction_Surpassed);
    GameMsg_G2S_Auction_Surpassed();
    ~GameMsg_G2S_Auction_Surpassed() {}

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned short  m_uIdx;
    CItem           m_cItem;
    unsigned int    m_uPrice;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_G2S_Auction_ItemsResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_Auction_ItemsResult);
    GameMsg_G2S_Auction_ItemsResult();
    ~GameMsg_G2S_Auction_ItemsResult() {}

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char                       m_nState;
    float                               m_fCD;
    bool                                m_bSearch;
    std::string                         m_strSearchKey;
    unsigned short                      m_uCurPage;
    unsigned short                      m_uTotalCount;
    unsigned int                        m_tEndTime;
    std::list<CAuctionItemData>         m_listItems;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_Auction_CheckTopPrice : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_Auction_CheckTopPrice);
    GameMsg_S2G_Auction_CheckTopPrice();
    ~GameMsg_S2G_Auction_CheckTopPrice() {}

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

public:
    std::map<long long, CAuctionTopPriceData> m_mapTopInfo;    // key: logID
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_G2S_Auction_CheckTopPriceResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_Auction_CheckTopPriceResult);
    GameMsg_G2S_Auction_CheckTopPriceResult();
    ~GameMsg_G2S_Auction_CheckTopPriceResult() {}

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

public:
    std::map<long long, bool>  m_mapTopInfo;   // logID -> bool
};

#endif // __GAMEMSG_SPECIALMALL_H__

