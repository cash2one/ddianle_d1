--开始结束时间
local beginTime = "11:00:00";
local endTime = "14:00:00";

-- 大妈的最大波数
local maxBigMamaRound = 2;

-- 小恋/小软的npcID
local npcXiaoLian = 1;
local npcXiaoRuan = 2;

-- 走马灯
local announcementString = "大妈霸占了广场舞，迅速"; 
local announcementSpace = 1*60; -- 走马灯时间间隔

-- 新手引导的奖励
local malerewardGuide = "8020,1,604800|7003,1,604800";
local femalrewardGuide = "8020,1,604800|7003,1,604800";

-- 小恋小软的信息
local LuckData = 
{
	[1] = {	
			npcID = 1,								-- npc ID
			baseProb = 1000,							-- 刷出来的基础概率， 万分比
			probGrowRatio = 10,						-- 刷出概率的成长系数 （baseProb+probGrowRatio*舞团等级）
		  };
	[2] = {
			npcID = 2,
			baseProb = 1000,
			probGrowRatio = 10,
		  };
}

local introduceString = "1,进击的广场舞大妈每天12：00~14：00开启，总共五个关卡，以舞团为单位，每一个关卡被任意一个玩家挑战成功，开启新的关卡，还在原先关卡挑战的成员舞蹈结束后后也将进入最新关卡；\n2，未通过入团仪式的玩家（小萌新）无法参加该日常活动；\n3，在任意关卡都有机会碰到小恋或小软关卡，刷新出小恋或小软关卡的成员可不用挑战就可带领全舞团成员领取最终通关奖励；\n4，在挑战的过程无任何奖。挑战失败、无人挑战也均无奖励，玩家需要完成5个关卡或刷新出福利关卡小恋或小软，才算达到通关条件；\n5，达到通关条件后，未参与该活动的舞团成员会得到基础奖励；参与挑战的成员会得到2倍的基础奖励；完成最后一关的成员能获得3倍的基础奖励；任意人碰到小恋或小软关卡，不仅全员通关，且全舞团成员除了能拿到自己的基础奖外还能拿到最大奖；奖励在活动界面领取即可。"

-- 奖励：
-- 未参与(1)/参与(2)/击杀(3)/小恋(4)/小软(5)
local Rewards = 
{ 
	[1] = {	
		malereward = "8020,1,604800|7003,1,604800", 
		femalereward="8020,1,604800|7003,1,604800", 
		moneyreward=1000,							-- 金币
		bindCoin=0,									-- 绑定MB
		danceGroupContribution=10,					-- 舞团贡献
	};
	[2] = {	
		malereward = "8020,1,604800|7003,1,604800", 
		femalereward="8020,1,604800|7003,1,604800", 
		moneyreward=1000,							-- 金币
		bindCoin=0,									-- 绑定MB
		danceGroupContribution=10,					-- 舞团贡献
	};
	[3] = {	
		malereward = "8020,1,604800|7003,1,604800", 
		femalereward="8020,1,604800|7003,1,604800", 
		moneyreward=1000,							-- 金币
		bindCoin=0,									-- 绑定MB
		danceGroupContribution=10,					-- 舞团贡献
	};
	[4] = {
		malereward = "8020,1,604800|7003,1,604800", 
		femalereward="8020,1,604800|7003,1,604800", 
		moneyreward=1000,							-- 金币
		bindCoin=0,									-- 绑定MB
		danceGroupContribution=10,					-- 舞团贡献
	};
	[5] = {	
		malereward = "8020,1,604800|7003,1,604800", 
		femalereward="8020,1,604800|7003,1,604800", 
		moneyreward=1000,							-- 金币
		bindCoin=0,									-- 绑定MB
		danceGroupContribution=10,					-- 舞团贡献
	};

}


function AddRewards_Lua(index, value)
		AddBigMamaReward(index, value["malereward"]
		, value["femalereward"]
		, value["moneyreward"]
		, value["bindCoin"]
		, value["danceGroupContribution"]);
end

function AddLuckData_Lua(index, value)
	AddBigMamaLuckData(value["npcID"], value["baseProb"], value["probGrowRatio"]);
end

SetBigMamaConfig(beginTime, endTime, maxBigMamaRound, npcXiaoLian, npcXiaoRuan, announcementString, announcementSpace, introduceString);
table.foreach(Rewards, AddRewards_Lua);
table.foreach(LuckData, AddLuckData_Lua);
SetBigMamaGuideReward(malerewardGuide, femalrewardGuide);
