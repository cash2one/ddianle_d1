#ifndef		__COUPLEDATASTRUCTURE_H__
#define		__COUPLEDATASTRUCTURE_H__

#include <string>
#include <list>
#include "CoupleMacroDef.h"
#include "../../datastructure/DataStruct_Base.h"

class CParamPool;
class CEntityItemComponent;
class CEntityGeneComponent;
class PetComponent;
class CEntityMedalComponent;
class CEntityHorseComponent;

enum ECoupleMsgError
{
    ECoupleMsgError_Scuess,
    ECoupleMsgError_CanNotBlessingSelf,  // 不能自己祝福自己
    ECoupleMsgError_BlessingPairNot,     // 婚礼已经结束
    ECoupleMsgError_HaveBlessing,        // 已经祝福过了
    ECoupleMsgError_MoneyNotEnough,      // 钱不够
};


class CCoupleRequest
{
public:
    CCoupleRequest();
    ~CCoupleRequest();
public:
    unsigned int m_nRoleID;
    std::string m_strName;
};

//////////////////////////////////////////////////////////////////////////

class CCoupleRequestReplay
{
public:
    CCoupleRequestReplay();
    ~CCoupleRequestReplay() {}

public:
    void doEncode(CParamPool &IOBuff);

public:
    unsigned int    m_nPairID;
    unsigned int    m_nTargetID;
    std::string     m_strTargetName;
    unsigned int    m_nCoupleDate;
    bool            m_bAgree;
    std::string     m_strReason;
    unsigned int    m_nSmallRoleID;
};

//////////////////////////////////////////////////////////////////////////

class CWeddingRoomPlayerInfo
{
public:
    CWeddingRoomPlayerInfo();
    ~CWeddingRoomPlayerInfo();

    void doEncode(CParamPool & IOBuff);

public:
    unsigned int m_nRoleID;
    std::string m_strRoleName;
    unsigned char m_eRoleSex;
    char m_nRoleSkin;
    std::string m_strDanceGroupName;
    char m_nGroupTitle;
    unsigned short m_nDanceGroupBadge;
    unsigned short m_nDanceGroupEffect;
    bool m_bIsHost;
    unsigned char  m_eRoleType;
    bool           m_nIsVip;
    unsigned short m_nVipLv;
    CPlayerPosition m_playerPosition;
    unsigned short m_nTransFormID;

    CEntityItemComponent* m_pRoleItem;
    CEntityGeneComponent* m_pRoleGene;
    PetComponent*         m_pRolePet;
    CEntityMedalComponent   *m_pRoleMedal;
    CEntityHorseComponent   *m_pRoleHorse;
};

///////////////////////////////////////////////////////
class CWeddingRoomBriefInfo
{
public:
    CWeddingRoomBriefInfo();
    ~CWeddingRoomBriefInfo();
public:
    void doEncode(CParamPool & IOBuff);
public:
    unsigned int m_nRoomID;
    std::string m_strRoomName;
    bool m_bHasPassword;
    unsigned char m_nWeddingType;
    unsigned char m_arrPlayerSex[C_WEDDINGROOM_GEUST_COUNT];
    unsigned int m_nGroomID;
    std::string m_strGroomName;
    unsigned int m_nBrideID;
    std::string m_strBrideName;
};

///////////////////////////////////////////////////////
class CWeddingRoomWholeInfo
{
public:
    CWeddingRoomWholeInfo();
    ~CWeddingRoomWholeInfo();
public:
    void doEncode(CParamPool & IOBufff);
public:
    unsigned int m_nRoomID;
    std::string m_strRoomName;
    unsigned int m_nPairID;
    unsigned int m_nGroomID;
    unsigned int m_nBrideID;
    bool m_bHasPwd;
    std::string m_strRoomPwd;
    bool m_bIsHost;
    unsigned char m_nWeddingType;
    unsigned int m_nWeddingTimeRemaind;

    std::list<CWeddingRoomPlayerInfo> m_listPlayers;
};


///////////////////////////////////////////////////////
class CCoupleConfigDataMgr
{
public:
    CCoupleConfigDataMgr();
    virtual ~CCoupleConfigDataMgr(){};

    static CCoupleConfigDataMgr & Instance()
    {
        static CCoupleConfigDataMgr s_MarriageDateGiftDataMgr;
        return s_MarriageDateGiftDataMgr;
    }

public:
    void AddMaleReward(std::list<CItem> & gift){
        m_MerriageDateMaleGift = gift;
    }
    const std::list<CItem> & GetMaleReward(){
        return m_MerriageDateMaleGift;
    }
    void AddFemaleReward(std::list<CItem> & gift){
        m_MerriageDateFemaleGift = gift;
    }
    const std::list<CItem> & GetFemaleReward(){
        return m_MerriageDateFemaleGift;
    }

    void AddCoupleConfig(int nBlessing, int nValue, unsigned int nBlessingFeedBackItem, const std::map<unsigned int, itemtype_t> &boxid, unsigned int nRedEnvelopeCount);
    int GetRedEnvelopeBlessing(){
        return m_nRedEnvelopeBlessing;
    }
    int GetRedEnvelopeValue(){
        return m_nRedEnvelopeValue;
    }
    //int GetWeddingGiftBoxID( unsigned short nBlessingValue );
    unsigned int GetWeddingGiftBoxID(unsigned int nBlessingValue);
    unsigned int GetBlessingFeedBackItemID(){
        return m_nBlessingFeedBackItemID;
    }
    unsigned int GetRedEnvelopeNoticeCount(){
        return m_nRedEnvelopeNoticeCount;
    }
    void GetRedEnvelopeGift(std::map<unsigned int, itemtype_t> &giftinfo){
        giftinfo = m_CoupleRedEnvelopeGift;
    }

private:
    std::list<CItem>	m_MerriageDateMaleGift;
    std::list<CItem>	m_MerriageDateFemaleGift;

    int		m_nRedEnvelopeBlessing;
    int		m_nRedEnvelopeValue;
    unsigned int	m_nBlessingFeedBackItemID;
    std::map<unsigned int, itemtype_t> m_CoupleRedEnvelopeGift;
    unsigned int	m_nRedEnvelopeNoticeCount;
};

///////////////////////////////////////////////////////
class CBlessingGiftDataMgr
{
public:
    CBlessingGiftDataMgr(){};
    virtual ~CBlessingGiftDataMgr(){};

    static CBlessingGiftDataMgr & Instance()
    {
        static CBlessingGiftDataMgr s_BlessingGiftDataMgr;
        return s_BlessingGiftDataMgr;
    }

public:
    void AddMaleReward(std::list<CItem> & gift){
        m_listMaleReward = gift;
    }
    const std::list<CItem> & GetMaleReward(){
        return m_listMaleReward;
    }
    void AddFemaleReward(std::list<CItem> & gift){
        m_listFemaleReward = gift;
    }
    const std::list<CItem> & GetFemaleReward(){
        return m_listFemaleReward;
    }

private:
    std::list<CItem>	m_listMaleReward;
    std::list<CItem>	m_listFemaleReward;
};

//////////////////////////////////////////////////////
class CDivorceConfigDataMgr
{
public:
    CDivorceConfigDataMgr(){};
    virtual ~CDivorceConfigDataMgr(){};

    static CDivorceConfigDataMgr & Instance()
    {
        static CDivorceConfigDataMgr s_DivorceConfig;
        return s_DivorceConfig;
    }

public:
    void AddDivorceConfig(const CItem & divorceitem, const CItem & compulsoryitem, int nRemarryIntimacyRate, const std::string & strDesc);
    void AddDivorceMusicConfig(const std::string & strMusicName);
    void AddDivorceGiftConfig(unsigned int nMaleGiftID, unsigned int nFemaleGiftID);

    // 获取离婚道具
    const CItem & GetDivorceItem(){
        return m_DivorceItem;
    }

    // 获取强制离婚道具
    const CItem & GetCompulsoryItem(){
        return m_CompulsoryItem;
    }

    // 获取歌曲列表
    const std::list<string> & GetMusicList(){
        return m_MusicList;
    }

    // 获取交易物品列表
    const std::list<itemtype_t> & GetMaleGiftList(){
        return m_MaleGiftList;
    }
    const std::list<itemtype_t> & GetFemaleGiftList(){
        return m_FemaleGiftList;
    }

    // 获取复婚回复亲密度百分比
    int GetRemarryIntimacyRate(){
        return m_nRemarryIntimacyRate;
    }

    std::string GetDivorceDesc(){
        return m_strDivorceDesc;
    }

public:
    CItem	m_DivorceItem;
    CItem	m_CompulsoryItem;
    std::list<string> m_MusicList;
    std::string m_strDivorceDesc;
    int		m_nRemarryIntimacyRate;
    std::list<itemtype_t> m_MaleGiftList;
    std::list<itemtype_t> m_FemaleGiftList;

};


#endif

