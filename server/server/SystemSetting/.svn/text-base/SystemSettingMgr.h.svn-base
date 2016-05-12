#ifndef __GAME_SYSTEM_SETTING_H__
#define __GAME_SYSTEM_SETTING_H__

#include "../../socket/GameMsg_Base.h"

#include <string>
#include <list>
#include <map>


class CSystemSettingMgr : public GameMsg_Base
{
private:
    CSystemSettingMgr();
    ~CSystemSettingMgr();

public:
    static CSystemSettingMgr & Instance();
    const char *SerializeName();

    void SetFirstPaymentReward(bool bEnableFirstPayment);
    bool CanFirstPaymentReward() const;

    void SetRecharge(bool bEnableRecharge);
    bool CanRecharge() const;

    void SerializeSystemSettingInfo(CParamPool & IOBuff);
    void SerializeLoadingADList(CParamPool & IOBuff);
    void SerializeLoadingBeckoningValue(CParamPool & IOBuff);
    void SerializeFunctionSwitch(CParamPool & IOBuff);

    //设置舞团团长传位时新团长需要的最小贡献度
    void SetMinContributionForDemise(unsigned int nMinContributionForDemise);

    void SetMinLevelCoupleRequired(unsigned short nMinLevelCoupleRequired);
    unsigned short MinLevelCoupleRequired();

    void SetMinIntimacyMarriageRequired(int nIntimacyMarriageRequired);
    int MinIntimacyMarriageRequired();

    //设置房间奖励需要的最小玩家数
    void SetMinRoomRewardPlayerNum(int nMinRoomRewardPlayerNum);

    //获取房间奖励需要的最小玩家数
    int GetMinRoomRewardPlayerNum();

    void SetLobbyChatCDTime(int nChatCDTime);
    int LobbyChatCDTime();

    void SetLobbyChatRequirLeve(int nLobbyChatRequirLeve);
    int LobbyChatRequireLevel();

    void SetPhotoRoomStartTime(int nPhotoRoomStartTime);
    int PhotoRoomStartTime();
    int PhotoRoomStartRemainTime();

    void SetInvitationStatus(bool status);
    bool IsInvitationOpen();

    void SetSecurityQuizStatus(bool status);
    bool IsSecurityQuizOpen();

    void SetGuideMusic(const std::string &strMusic);

    void SetDefaultMoveTypeInfo(int nDefaultMoveType);
    unsigned char DefaultMoveType() const;

    void SetUpgradeInstallerLevelInfo(int nUpgradeInstallerLevel);
    int UpgradeInstallerLevel() const;

    void AddLoadingAD(const std::string &strLoadingAD);

    unsigned short GetUpdateLBSPosInterval() const;
    void SetUpdateLBSPosInterval(unsigned short val);

    unsigned int GetBirthdayResetTime() const;
    void SetBirthdayResetTime(unsigned int nBirthdayResetTime);

    bool IsOpenSendGift() const {
        return m_bOpenSendGift;
    }
    void SetOpenSendGift(bool bOpenSendGift);

    //心动模式
    void AddBeckoningValue(int nMode, int nRank, int nValue);
    void AddBeckoningEffect(int nLevel, int nValue);
    int GetBeckoningValue(int nMode, int nRank);
    const std::map<int, int>& GetBeckoningEffectMap() const;
    const std::map<int, std::map<int, int> >& GetBeckoningValueMap() const;

    //获得功能开关
    bool IsFunctionOpen(int nFunction);

    //设置功能开关
    void SetFunctionSwitch(int nFunction, bool bOpen);

    // Buff
    int GetBuffIconItem(const std::vector<int>& vecKey);
    void SetBuffIconItem(const std::string& k, int v);
    void SetBuffCanRemove(int nGeneID);
    bool CanRemoveBuff(int nGeneID);

    // Social share
    std::string GetSocialShareText() const {
        return m_strSocialShareText;
    }
    void SetSocialShareText(const std::string &val) {
        m_strSocialShareText = val;
    }
    int GetSocialShareRewardBindMCoin() const {
        return m_nSocialShareRewardBindMCoin;
    }
    void SetSocialShareRewardBindMCoin(int val) {
        m_nSocialShareRewardBindMCoin = val;
    }
    std::string GetSocialShareRewardDesc() const {
        return m_strSocialShareRewardDesc;
    }
    void SetSocialShareRewardDesc(const std::string &val) {
        m_strSocialShareRewardDesc = val;
    }

    // 衣服开放特效部位
    void SetClothEffectPos(std::list<unsigned short> & listClothPos);

    // 衣服部位是否开放增加特效
    bool IsOpenClothEffectPos(unsigned short nClothPos);

    void SetDownloadParam(int nMaxConnections, int nMaxBps);

public:
    // recharge
    bool    m_bEnableFirstRechargeReward;
    bool    m_bEnableRecharge;


    //dance group
    unsigned int    m_nMinContributionForDemise;    //舞团团长传位时新团长需要的最小贡献度

    //couple
    unsigned short m_nMinLevelCoupleRequired;
    int m_nMinIntimacyMarriageRequired;
    int m_nMinRoomRewardPlayerNum;                  //房间奖励需要的最小玩家数

    //chat
    int m_nLobbyChatCDTime;                         //大厅发言的CD时间
    int m_nLobbyChatRequireLevel;                   //大厅发言等级要求

    int m_nPhotoRoomStartTime;
    bool m_bInvitationIsOpen;
    bool m_bSecurityQuizOpen;                       //密码保护问题是否开启

    std::string m_strGuideMusic;                    //新手引导音乐

    unsigned char m_nDefaultMoveType;               // 默认飞行状态
    int m_nUpgradeInstallerLevel;

    std::list<std::string> m_listLoadingAD;         // Loading 海报图

    unsigned short m_nUpdateLBSPosInterval;         // 更新LBS位置间隔时间，秒

    unsigned int m_nBirthdayResetTime;              // 更改生日重置时间

    bool m_bOpenSendGift;

    //心动模式
    std::map<int, std::map<int, int> > m_mapBeckoningValue; //心动值配置
    std::map<int, int> m_mapBeckoningEffect;                //心动特效配置，特效id<=>心动值

    //功能开关
    std::map<unsigned short, bool> m_mapFunctionSwitch;

    //buff
    std::map<string, int> m_mapBuffIcon;                    //buff图标映射表
    std::map<int, int> m_mapBuffIconParamCount;             //buff图标搜索辅助表
    std::set<int> m_setBuffCanRemove;                       //可主动移除的buff集合

    std::string m_strSocialShareText;                       //社交分享字符串
    std::string m_strSocialShareRewardDesc;                 //社交分享奖励说明文字
    int         m_nSocialShareRewardBindMCoin;              //社交分享奖励绑定m币

    std::list<unsigned short>   m_listClothOpenEffectPos;   //衣服开放部位

    // 动态下载
    int m_nDownloadMaxConnections;                          // 最大下载连接数
    int m_nDownloadMaxBps;                                  // 最大下载速度/带宽
};

#endif //__GAME_SYSTEM_SETTING_H__

