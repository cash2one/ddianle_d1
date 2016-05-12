local SystemSetting = 
{
    --房间
    roomStartWait = 60000,                       -- 房间开始等待时间:ms
    roomEndWait = 30000,                         -- 房间结束等待时间:ms
    autoMatchCount = 2,                          -- 自动匹配房间人数
    autoMatchStep = 5000,                        -- 自动匹配时间间隔:ms
    defaultScene = 1,                            -- 默认场景
    defaultMode = 3,                             -- 默认模式 1太鼓模式 2传统模式 3泡泡模式
    defaultLevel = 1;                            -- 默认难度 1简单 2普通 3困难
    MinContributionForDemise = 0,                -- 舞团团长传位时新团长需要的最小贡献度
    minLevelCoupleRequired = 10,                 -- 成为情侣的等级要求
    minIntimacyMarriageRequired = 100,           -- 结婚的亲密度要求
    minRoomRewardPlayerNum = 3,                  -- 房间奖励需要的最小玩家数
    lobbyChatCDTime = 10,                        -- 大厅发言时间间隔
    lobbyChatRequireLevel = 1,                   -- 大厅发言等级要求
    photoRoomStartTime = "2014-3-5 00:00:00",    -- 摄影棚开放时间
    guideMusicName = "song0607",                 -- 新手教学歌曲
    defaultMoveType = 2,                         -- 默认飞行状态 0: None无状态 1: Walk 行走状态 2: Fly 飞行状态
    upgradeInstallerLevel = 20,                  -- 提示更新安装包的等级

    --直播房间
    liveRoomMinRoleLevel = 10,                   -- 直播房间进入直播房间等级要求
    liveRoomMaxAudienceCount = 500,              -- 直播房间最大观众数量
    liveRoomMaxGMCount = 2,                      -- 直播房间最大GM数量
    liveRoomCloseTime = 10,                      -- 直播房间自动关闭时间:s
    liveRoomCountDownTime = 10,                  -- 直播房间开始倒计时时间:s

    updateLBSPosInterval = 10,                   -- 更新LBS位置频率间隔，秒
    
    birthdayResetTime = "2014-11-21 19:00:00",   -- 更改生日重置时间

    openSendGift = true,                         -- 是否打开赠送功能，包括赠送、点愿、埋蛋送礼
    
    clothOpenEffectPos    = "0|2|4|14|17",       -- 装备特效开放部位
    
    downloadMaxConnections = 100,                -- 动态下载最大连接数
    downloadMaxBps = 10*1024*1024*8,             -- 动态下载最大速度/带宽
}

local LoadingAD = 
{
    [1] = {name = "loading_ad-01", open = false},
    [2] = {name = "loading_ad-02", open = true},
    [3] = {name = "loading_ad-03", open = true},
    [4] = {name = "loading_ad-04", open = false},
    [5] = {name = "loading_ad-05", open = false},
}

--功能开关：false为关闭功能，true为打开
local FunctionSwitch = 
{
    [1] = {name = "LBS", open = true},
    [2] = {name = "transform", open = true},                --变身栏位
    [3] = {name = "buffbar", open = true},    
    [4] = {name = "hideForIos", open = false},              --IOS审核开关控制 大于0时 客户端需要屏蔽一些功能
    [5] = {name = "Recharge", open = true},                 --充值开关
    [6] = {name = "WiddingRoom", open = true},              --甜蜜殿堂开关
    [7] = {name = "LuckDraw", open = true},                 --欢乐转开关
    [8] = {name = "magicLamp", open = true},                --魔法神灯开关
    [9] = {name = "Voice", open = true},                    --语音开关
    [10] = {name = "SocialShare", open = true},             --每日分享开关
    [11] = {name = "SpecialMall", open = true},             --特殊商店开关
    [12] = {name = "NewDivorce", open = false},             --新离婚系统
    [13] = {name = "homosexual", open = true},              --同性结婚
    [14] = {name = "Rank", open = true},                   --排行榜
    [15] = {name = "RankShowGay", open = false},            --排行榜是否显示同性
    [16] = {name = "Pet", open = false},                    --宠物
    [17] = {name = "ClothEffect", open = true},             --衣服镶嵌特效
    [18] = {name = "Medal", open = true},                   --勋章开关
    [19] = {name = "DanceGroupCeremony", open = true},      --入团仪式开关
    [20] = {name = "Handbook", open = true},                --图鉴开关
    [21] = {name = "DanceGroupChallenge", open = true},     --舞团试炼
    [22] = {name = "Horse", open = true},                   --坐骑开关
    [23] = {name = "WifiDownload", open = true},            --Wifi网络环境下自动下载
	[24] = {name = "MagicArray", open = true},              --幸运魔法阵
	[25] = {name = "BigMama", open = true},            		--舞团大妈活动开关
	[26] = {name = "FairyLand", open = true},            		--秘境开关
}

function ReadSetting()
    local roomStartWait = SystemSetting["roomStartWait"];
    local roomEndWait = SystemSetting["roomEndWait"];
    local autoMatchCount = SystemSetting["autoMatchCount"];
    local autoMatchStep = SystemSetting["autoMatchStep"];
    local defaultScene = SystemSetting["defaultScene"];
    local defaultMode = SystemSetting["defaultMode"];
    local defaultLevel = SystemSetting["defaultLevel"];
    local minLevelCoupleRequired = SystemSetting["minLevelCoupleRequired"];
    local minIntimacyMarriageRequired = SystemSetting["minIntimacyMarriageRequired"];
    local MinContributionForDemise = SystemSetting["MinContributionForDemise"];
    local minRoomRewardPlayerNum = SystemSetting["minRoomRewardPlayerNum"];
    local lobbyChatCDTime  = SystemSetting["lobbyChatCDTime"];
    local lobbyChatRequireLevel  = SystemSetting["lobbyChatRequireLevel"];
    local photoRoomStartTime = SystemSetting["photoRoomStartTime"];
    local guideMusicName = SystemSetting["guideMusicName"];
    local defaultMoveType = SystemSetting["defaultMoveType"];
    local upgradeInstallerLevel = SystemSetting["upgradeInstallerLevel"];
    local liveRoomMinRoleLevel = SystemSetting["liveRoomMinRoleLevel"];
    local liveRoomMaxAudienceCount = SystemSetting["liveRoomMaxAudienceCount"];
    local liveRoomMaxGMCount=SystemSetting["liveRoomMaxGMCount"];
    local liveRoomCloseTime = SystemSetting["liveRoomCloseTime"];
    local liveRoomCountDownTime = SystemSetting["liveRoomCountDownTime"];
    local updateLBSPosInterval = SystemSetting["updateLBSPosInterval"];
    local birthdayResetTime = SystemSetting["birthdayResetTime"];
    local openSendGift = SystemSetting["openSendGift"];
    local clothOpenEffectPos = SystemSetting["clothOpenEffectPos"];
    local downloadMaxConnections = SystemSetting["downloadMaxConnections"]
    local downloadMaxBps = SystemSetting["downloadMaxBps"]
    
    RoomKickSet(roomStartWait, roomEndWait);
    RoomMatchSet(autoMatchCount, autoMatchStep);
    RoomMusicSet(defaultScene, defaultMode, defaultLevel);
    
    MinContributionForDemiseSet(MinContributionForDemise);
    MinLevelCoupleRequiredSet(minLevelCoupleRequired);
    MinIntimacyMarriageRequiredSet(minIntimacyMarriageRequired);
    MinRoomRewardPlayerNumSet(minRoomRewardPlayerNum);
    SetChatRequirementLua(lobbyChatCDTime, lobbyChatRequireLevel);
    SetPhotoRoomInfoLua(photoRoomStartTime);
    SetGuideMusicNameLua(guideMusicName);
    SetDefaultMoveType(defaultMoveType);
    SetUpgradeInstallerLevel(upgradeInstallerLevel);
    LoadLiveRoomConfigInfo(liveRoomMinRoleLevel, liveRoomMaxAudienceCount, liveRoomMaxGMCount, liveRoomCloseTime, liveRoomCountDownTime);
    SetUpdateLBSPosInterval(updateLBSPosInterval);
    SetBirthdayResetTimeLua(birthdayResetTime);
    SetOpenSendGift(openSendGift);
    SetClothEffectPos(clothOpenEffectPos);
    SetDownloadParam(downloadMaxConnections, downloadMaxBps);
end


function GroupReadSetting()
    local roomStartWait = SystemSetting["roomStartWait"];
    local roomEndWait = SystemSetting["roomEndWait"];
    local autoMatchCount = SystemSetting["autoMatchCount"];
    local autoMatchStep = SystemSetting["autoMatchStep"];
    local defaultScene = SystemSetting["defaultScene"];
    local defaultMode = SystemSetting["defaultMode"];
    local defaultLevel = SystemSetting["defaultLevel"];
    local minLevelCoupleRequired = SystemSetting["minLevelCoupleRequired"];
    local minIntimacyMarriageRequired = SystemSetting["minIntimacyMarriageRequired"];
    local MinContributionForDemise = SystemSetting["MinContributionForDemise"];
    local minRoomRewardPlayerNum = SystemSetting["minRoomRewardPlayerNum"];
    local lobbyChatCDTime  = SystemSetting["lobbyChatCDTime"];
    local lobbyChatRequireLevel  = SystemSetting["lobbyChatRequireLevel"];
    local photoRoomStartTime = SystemSetting["photoRoomStartTime"];
    local guideMusicName = SystemSetting["guideMusicName"];
    local defaultMoveType = SystemSetting["defaultMoveType"];
    local upgradeInstallerLevel = SystemSetting["upgradeInstallerLevel"];
    local liveRoomMinRoleLevel = SystemSetting["liveRoomMinRoleLevel"];
    local liveRoomMaxAudienceCount = SystemSetting["liveRoomMaxAudienceCount"];
    local liveRoomMaxGMCount=SystemSetting["liveRoomMaxGMCount"];
    local liveRoomCloseTime = SystemSetting["liveRoomCloseTime"];
    local liveRoomCountDownTime = SystemSetting["liveRoomCountDownTime"];
    local updateLBSPosInterval = SystemSetting["updateLBSPosInterval"];
    local birthdayResetTime = SystemSetting["birthdayResetTime"];
    local openSendGift = SystemSetting["openSendGift"];
    local clothOpenEffectPos = SystemSetting["clothOpenEffectPos"];
    local downloadMaxConnections = SystemSetting["downloadMaxConnections"]
    local downloadMaxBps = SystemSetting["downloadMaxBps"]
    
    RoomKickSet(roomStartWait, roomEndWait);
    RoomMatchSet(autoMatchCount, autoMatchStep);
    RoomMusicSet(defaultScene, defaultMode, defaultLevel);
end

function AddLoadingAdData(index, value)
    if value ~= nil then 
        local loadingAdName = value["name"];
        local loadingAdOpen = value["open"];

        if loadingAdOpen == true then
            AddLoadingAdInfo(loadingAdName);
        end
    end
end

function AddFunctionSwitchLua(index, value)
    if value ~= nil then
        local fName = value["name"];
        local bOpen = value["open"];

        AddFunctionSwitch(index, fName, bOpen)
    end
end

if ServerGetLine ~= nil and ServerGetLine() ~= nil then
    ReadSetting();

    table.foreach(LoadingAD, AddLoadingAdData);
    table.foreach(FunctionSwitch, AddFunctionSwitchLua);

    print("Game server load system setting info complete!");
else
    GroupReadSetting();
    table.foreach(FunctionSwitch, AddFunctionSwitchLua);

    print("Group server load system setting info complete!");
end

