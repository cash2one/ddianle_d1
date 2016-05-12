--VIP最高等级
local maxlevel = 10;

--消费M币兑换成长值的比例
local consumedPtExpBouns = 100;

--商城赠送增加VIP经验 最大限额M币
local sendaddvipexpmaxbill = 50000;

--VIP升级奖励信息 true表示拥有该权限，false表示没有该权限
local VIPPrivInfo = 
{
	[1] = {	level = 1,						--VIP等级
			levelupexp = 1,					--当前级别所需成长值
			dailyExpBonus = 2,				--每日VIP可领取成长值
			VIPSeapkersFontEffe = false,		--VIP喇叭字体特效
			canKickRoomPlayer = true,		--房间踢人
			exclusiveSelfInfoBoard = false, --专属个人信息面板
			exclusiveRoomAction = false,	--专属房间动作
			enterRoomSoundEffe = false,		--进入房间音效
			expRateBounus = 5,				--经验加成
			roleNameColor = 11,				--角色名颜色
			VIPLevelUpAnnounce = 0,			--VIP升级公告 0 表示无， 1表示 红色系统公告 2表示走马灯公告
			enterRoomViewEffe = "vip_1_enter_effect",	--进入房间特效,空表示无
			maleReward = "32001,10,-1|31026,2,-1|7003,1,2592000|8042,1,-1", femaleReward = "32001,10,-1|31026,2,-1|7003,1,2592000|8042,1,-1", money = 600,			
			onlineAnnounce = false,			--上线公告
			onlineAnnounceCoolDown = 600,   --公告冷却时间,
			maxFriendCount = 250,			--好友的上限
			maxBlackListCount = 250,		--黑名单上限
			constellationMatchCount = 1,	--星恋魔法阵卡牌使用次数限制
			clothgroupCount = 4,            --服饰搭配个数，vip越高就越多或者不变，至少是4，因为有3个是免费(非vip玩家可以使用)
			dailybindbbill = 10,             --每日绑定M币奖励
			dailyitem = "32001,1,-1",					--每日物品奖励
			MagicBuyVitNum = 5,				--魔法神灯每日优惠购买体力次数
			danceGroupChallengeExtraChance = 1,	--舞团试炼额外次数
			StorageCapacity = 11,			-- 仓库的初始容量
            danceGroupFairlyLandExChance = 10,          --舞团秘境额外挑战次数
            danceGroupFairlyLandVipBuf = 10,          --舞团秘境vip buf 百分比
		  };
	[2] = {	level = 2,		
			levelupexp = 30,
			dailyExpBonus = 2;	
			VIPSeapkersFontEffe = false,		
			canKickRoomPlayer = true,	
			exclusiveSelfInfoBoard = false,
			exclusiveRoomAction = false,	
			enterRoomSoundEffe = false,		
			expRateBounus = 5,			
			roleNameColor = 11,				
			VIPLevelUpAnnounce = 0,			
			enterRoomViewEffe = "vip_1_enter_effect",
			maleReward = "32002,5,-1|31026,5,-1|7003,1,-1|149,1,-1", femaleReward = "32002,5,-1|31026,5,-1|7003,1,-1|649,1,-1", money = 2000,			
			onlineAnnounce = false,
			onlineAnnounceCoolDown = 600,
			maxFriendCount = 300,
			maxBlackListCount = 300,
			
			constellationMatchCount = 1,
			clothgroupCount = 5,
			dailybindbbill = 0,             			--每日绑定M币奖励
			dailyitem = "",		--每日物品奖励
			MagicBuyVitNum = 6,
			danceGroupChallengeExtraChance = 2,
			StorageCapacity = 12,			-- 仓库的初始容量
            danceGroupFairlyLandExChance = 11,          --舞团秘境额外挑战次数
            danceGroupFairlyLandVipBuf = 11,          --舞团秘境vip buf 百分比
		  };
  	[3] = {	level = 3,				
			levelupexp = 100,
			dailyExpBonus = 2,	
			VIPSeapkersFontEffe = false,	
			canKickRoomPlayer = true,
			exclusiveSelfInfoBoard = false,
			exclusiveRoomAction = true,
			enterRoomSoundEffe = false,
			expRateBounus = 10,		
			roleNameColor = 11,	
			VIPLevelUpAnnounce = 0,
			enterRoomViewEffe = "vip_2_enter_effect",
			maleReward = "32002,10,-1|31026,10,-1|7007,1,2592000|14149,1,-1", femaleReward = "32002,10,-1|31026,10,-1|7007,1,2592000|14649,1,-1", money = 6000,			
			onlineAnnounce = false,
			onlineAnnounceCoolDown = 600,
			maxFriendCount = 350,
			maxBlackListCount = 350,
			constellationMatchCount = 1,
			clothgroupCount = 6,
			dailybindbbill = 0,             			--每日绑定M币奖励
			dailyitem = "32001,1,-1",		--每日物品奖励
			MagicBuyVitNum = 7,
			danceGroupChallengeExtraChance = 3,
			StorageCapacity = 12,			-- 仓库的初始容量
            danceGroupFairlyLandExChance = 12,          --舞团秘境额外挑战次数
            danceGroupFairlyLandVipBuf = 12,          --舞团秘境vip buf 百分比
			};
  	[4] = {	level = 4,				
			levelupexp = 300,
			dailyExpBonus = 2;	
			VIPSeapkersFontEffe = false,	
			canKickRoomPlayer = true,
			exclusiveSelfInfoBoard = false,
			exclusiveRoomAction = true,
			enterRoomSoundEffe = true,
			expRateBounus = 10,		
			roleNameColor = 11,	
			VIPLevelUpAnnounce = 0,
			enterRoomViewEffe = "vip_2_enter_effect",
			maleReward = "32002,20,-1|31026,20,-1|7007,1,-1|2149,1,-1|4149,1,-1", femaleReward = "32002,20,-1|31026,20,-1|7007,1,-1|17649,1,-1", money = 12000,			
			onlineAnnounce = false,
			onlineAnnounceCoolDown = 600,
			maxFriendCount = 400,
			maxBlackListCount = 400,
			
			constellationMatchCount = 1,
			clothgroupCount = 7,
			dailybindbbill = 0,             			--每日绑定M币奖励
			dailyitem = "32001,1,-1",		--每日物品奖励
			MagicBuyVitNum = 8,
			danceGroupChallengeExtraChance = 4,
			StorageCapacity = 12,			-- 仓库的初始容量
            danceGroupFairlyLandExChance = 13,          --舞团秘境额外挑战次数
            danceGroupFairlyLandVipBuf = 13,          --舞团秘境vip buf 百分比
		  };
  	[5] = {	level = 5,				
			levelupexp = 600,
			dailyExpBonus = 2,	
			VIPSeapkersFontEffe = false,	
			canKickRoomPlayer = true,
			exclusiveSelfInfoBoard = false,
			exclusiveRoomAction = true,
			enterRoomSoundEffe = true,
			expRateBounus = 15,		
			roleNameColor = 11,	
			VIPLevelUpAnnounce = 1,
			enterRoomViewEffe = "vip_3_enter_effect",
			maleReward = "32002,30,-1|31026,30,-1|8043,1,2592000|147,1,-1", femaleReward = "32002,30,-1|31026,30,-1|8043,1,2592000|647,1,-1", money = 24000,			
			onlineAnnounce = false,
			onlineAnnounceCoolDown = 600,
			maxFriendCount = 450,
			maxBlackListCount = 450,
			
			constellationMatchCount = 1,
			clothgroupCount = 8,
			dailybindbbill = 0,             			--每日绑定M币奖励
			dailyitem = "32001,1,-1",		--每日物品奖励
			MagicBuyVitNum = 8,
			danceGroupChallengeExtraChance = 5,
			StorageCapacity = 12,			-- 仓库的初始容量
            danceGroupFairlyLandExChance = 14,          --舞团秘境额外挑战次数
            danceGroupFairlyLandVipBuf = 14,          --舞团秘境vip buf 百分比
		  };
  	[6] = {	level = 6,				
			levelupexp = 1200,
			dailyExpBonus = 2,	
			VIPSeapkersFontEffe = false,	
			canKickRoomPlayer = true,
			exclusiveSelfInfoBoard = false,
			exclusiveRoomAction = true,
			enterRoomSoundEffe = true,
			expRateBounus = 15,		
			roleNameColor = 11,	
			VIPLevelUpAnnounce = 1,
			enterRoomViewEffe = "vip_3_enter_effect",
			maleReward = "32002,50,-1|31026,50,-1|8043,1,-1|14147,1,-1", femaleReward = "32002,50,-1|31026,50,-1|8043,1,-1|14647,1,-1", money = 50000,			
			onlineAnnounce = false,
			onlineAnnounceCoolDown = 600,
			maxFriendCount = 500,
			maxBlackListCount = 500,
			
			constellationMatchCount = 1,
			clothgroupCount = 9,
			dailybindbbill = 0,             			--每日绑定M币奖励
			dailyitem = "32001,1,-1",		--每日物品奖励
			MagicBuyVitNum = 8,
			danceGroupChallengeExtraChance = 6,
			StorageCapacity = 12,			-- 仓库的初始容量
            danceGroupFairlyLandExChance = 15,          --舞团秘境额外挑战次数
            danceGroupFairlyLandVipBuf = 15,          --舞团秘境vip buf 百分比
		 };
  	[7] = {	level = 7,				
			levelupexp = 2500,
			dailyExpBonus = 2,	
			VIPSeapkersFontEffe = false,	
			canKickRoomPlayer = true,
			exclusiveSelfInfoBoard = false,
			exclusiveRoomAction = true,
			enterRoomSoundEffe = true,
			expRateBounus = 20,		
			roleNameColor = 11,	
			VIPLevelUpAnnounce = 2,
			enterRoomViewEffe = "vip_4_enter_effect",
			maleReward = "32002,100,-1|31026,100,-1|22011,1,-1|2147,1,-1|4147,1,-1", femaleReward = "32002,100,-1|31026,100,-1|22011,1,-1|17647,1,-1", money = 60000,			
			onlineAnnounce = false,
			onlineAnnounceCoolDown = 600,
			maxFriendCount = 500,
			maxBlackListCount = 500,
			
			constellationMatchCount = 2,
			clothgroupCount = 10,
			dailybindbbill = 0,             			--每日绑定M币奖励
			dailyitem = "32001,1,-1",		--每日物品奖励
			MagicBuyVitNum = 8,
			danceGroupChallengeExtraChance = 7,
			StorageCapacity = 12,			-- 仓库的初始容量
            danceGroupFairlyLandExChance = 16,          --舞团秘境额外挑战次数
            danceGroupFairlyLandVipBuf = 16,          --舞团秘境vip buf 百分比
		 };
		 
  	[8] = {	level = 8,				
			levelupexp = 4200,
			dailyExpBonus = 2,	
			VIPSeapkersFontEffe = true,	
			canKickRoomPlayer = true,
			exclusiveSelfInfoBoard = false,
			exclusiveRoomAction = true,
			enterRoomSoundEffe = true,
			expRateBounus = 20,		
			roleNameColor = 11,	
			VIPLevelUpAnnounce = 2,
			enterRoomViewEffe = "vip_4_enter_effect",
			maleReward = "32002,120,-1|31026,120,-1|31020,30,-1|148,1,-1", femaleReward = "32002,120,-1|31026,120,-1|31020,30,-1|648,1,-1", money = 65000,			
			onlineAnnounce = false,
			onlineAnnounceCoolDown = 600,
			maxFriendCount = 500,
			maxBlackListCount = 500,
			
			constellationMatchCount = 2,
			clothgroupCount = 11,
			dailybindbbill = 0,             			--每日绑定M币奖励
			dailyitem = "",		--每日物品奖励
			MagicBuyVitNum = 8,
			danceGroupChallengeExtraChance = 8,
			StorageCapacity = 12,			-- 仓库的初始容量
            danceGroupFairlyLandExChance = 17,          --舞团秘境额外挑战次数
            danceGroupFairlyLandVipBuf = 17,          --舞团秘境vip buf 百分比
		 };
		 
  	[9] = {	level = 9,				
			levelupexp = 6500,
			dailyExpBonus = 2,	
			VIPSeapkersFontEffe = true,	
			canKickRoomPlayer = true,
			exclusiveSelfInfoBoard = false,
			exclusiveRoomAction = true,
			enterRoomSoundEffe = true,
			expRateBounus = 25,		
			roleNameColor = 11,	
			VIPLevelUpAnnounce = 2,
			enterRoomViewEffe = "vip_5_enter_effect",
			maleReward = "32002,135,-1|31026,135,-1|22007,1,2592000|14148,1,-1", femaleReward = "32002,135,-1|31026,135,-1|22007,1,2592000|14648,1,-1", money = 70000,			
			onlineAnnounce = false,
			onlineAnnounceCoolDown = 600,
			maxFriendCount = 500,
			maxBlackListCount = 500,
			
			constellationMatchCount = 2,
			clothgroupCount = 12,
			dailybindbbill = 0,             			--每日绑定M币奖励
			dailyitem = "",		--每日物品奖励
			MagicBuyVitNum = 8,
			danceGroupChallengeExtraChance = 9,
			StorageCapacity = 12,			-- 仓库的初始容量
            danceGroupFairlyLandExChance = 18,          --舞团秘境额外挑战次数
            danceGroupFairlyLandVipBuf = 18,          --舞团秘境vip buf 百分比
		 };
		 
  	[10] = { level = 10,				
			levelupexp = 9800,
			dailyExpBonus = 2,	
			VIPSeapkersFontEffe = true,	
			canKickRoomPlayer = true,
			exclusiveSelfInfoBoard = false,
			exclusiveRoomAction = true,
			enterRoomSoundEffe = true,
			expRateBounus = 25,		
			roleNameColor = 11,	
			VIPLevelUpAnnounce = 2,
			enterRoomViewEffe = "vip_5_enter_effect",
			maleReward = "32002,150,-1|31026,150,-1|22007,1,-1|2148,1,-1|4148,1,-1", femaleReward = "32002,150,-1|31026,150,-1|22007,1,-1|17648,1,-1", money = 80000,			
			onlineAnnounce = true,
			onlineAnnounceCoolDown = 3600,
			maxFriendCount = 500,
			maxBlackListCount = 500,
			
			constellationMatchCount = 2,
			clothgroupCount = 13,
			dailybindbbill = 0,             			--每日绑定M币奖励
			dailyitem = "",		--每日物品奖励
			MagicBuyVitNum = 8,
			danceGroupChallengeExtraChance = 10,
			StorageCapacity = 12,			-- 仓库的初始容量
            danceGroupFairlyLandExChance = 19,          --舞团秘境额外挑战次数
            danceGroupFairlyLandVipBuf = 19,          --舞团秘境vip buf 百分比
		 };

}

function AddVIPPrivInfo_Lua(index, value)
	if value ~= nil then
		local level = value["level"];
		local levelupexp = value["levelupexp"];
		local dailyExpBonus = value["dailyExpBonus"];
		local VIPSeapkersFontEffe = value["VIPSeapkersFontEffe"];
		local canKickRoomPlayer = value["canKickRoomPlayer"];
		local exclusiveSelfInfoBoard = value["exclusiveSelfInfoBoard"];
		local exclusiveRoomAction = value["exclusiveRoomAction"];
		local enterRoomSoundEffe = value["enterRoomSoundEffe"];
		local expRateBounus = value["expRateBounus"];
		local roleNameColor = value["roleNameColor"];
		local VIPLevelUpAnnounce = value["VIPLevelUpAnnounce"];
		local enterRoomViewEffe = value["enterRoomViewEffe"];
		local maleReward = value["maleReward"];
		local femaleReward = value["femaleReward"];
		local money = value["money"];
		local onlineAnnounce = value["onlineAnnounce"];
		local onlineAnnounceCoolDown = value["onlineAnnounceCoolDown"];
		local maxFriendCount = value["maxFriendCount"];
		local maxBlackListCount = value["maxBlackListCount"];
		local constellationMatchCount = value["constellationMatchCount"];
		local clothgroupCount = value["clothgroupCount"];
		local dailybindbbill = value["dailybindbbill"];
		local dailyitem = value["dailyitem"];
		local MagicBuyVitNum = value["MagicBuyVitNum"];
		local danceGroupChallengeExtraChance = value["danceGroupChallengeExtraChance"];
		local StorageCapacity = value["StorageCapacity"];
        local danceGroupFairlyLandExChance = value["danceGroupFairlyLandExChance"];
        local danceGroupFairlyLandVipBuf = value["danceGroupFairlyLandVipBuf"];
		LuaAddVIPPrivInfo(level, levelupexp, dailyExpBonus, VIPSeapkersFontEffe, canKickRoomPlayer, exclusiveSelfInfoBoard, exclusiveRoomAction, enterRoomSoundEffe, expRateBounus, 
						  roleNameColor, VIPLevelUpAnnounce, enterRoomViewEffe, maleReward, femaleReward, money, 
						  onlineAnnounce, onlineAnnounceCoolDown, maxFriendCount, maxBlackListCount,
						  constellationMatchCount, clothgroupCount, dailybindbbill, dailyitem, MagicBuyVitNum, danceGroupChallengeExtraChance , StorageCapacity, danceGroupFairlyLandExChance,danceGroupFairlyLandVipBuf);
	end
end

function AddVIPInfo_Lua()
	LuaAddVIPInfo(maxlevel, consumedPtExpBouns, sendaddvipexpmaxbill );
end

table.foreach(VIPPrivInfo, AddVIPPrivInfo_Lua);
AddVIPInfo_Lua();
