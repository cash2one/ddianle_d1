--许愿星奖励
local StarReward =
{
	[1] = {id = 1,coststar = 50,malereward = "8085,1,2592000|31012,10,-1|31013,5,-1", femalereward = "8085,1,2592000|31012,10,-1|31013,5,-1", money = 0 },
	[2] = {id = 2,coststar = 100,malereward = "22049,1,2592000|31014,2,-1|31003,1,-1", femalereward = "22049,1,2592000|31014,2,-1|31003,1,-1", money = 0 },
	[3] = {id = 3,coststar = 200,malereward = "22073,1,2592000|31013,5,-1|31014,2,-1", femalereward = "22073,1,2592000|31013,5,-1|31014,2,-1", money = 0 },
}

--关卡可选道具列表
local SelectItemList = "";

--跳过关卡使用物品
local SkipParcloseItem = "31086,1,-1";

--搓神灯
local dustCost = 25;                       -- 搓神灯消耗
local dustIconName = "12221";              -- 图片名称
local dustActivityName = "搓神灯";         -- 功能名称
local dustMaleReward   = "31026 ,1,-1";    -- 男性奖励列表
local dustFemaleReward = "31026,1,-1";     -- 女性奖励列表
local dustMoney        = 50;               -- 金钱奖励
local dustStoryID        = 0;              -- 搓神灯剧情

--法力水晶
local crystalInitialValue = 5;            -- 水晶(体力)初始值
local crystalMaxValue = 60;               -- 水晶(体力)的最大值
local crystalRestoreTime = 6*60;          -- 水晶回复时间(秒)
local crystalCreateDust = "3:5";          -- 水晶产生尘埃数目范围
local buyCrystalInfo =
{
	[1] = { id = 1,iconName = "lamp-cys1", crystalCount = 1, price = 200, vipprice = 100},
	[2] = { id = 2,iconName = "lamp-cys2", crystalCount = 5, price = 880, vipprice = 440},
}
local NotVipBuyVitNum = 2;				  -- 非VIP玩家每天优惠购买体力次数

--奖励邮件标题
local StarRewardTitle = "背包满了"; 				-- 幸运星奖励邮件标题
local StarRewardContent = "背包满了";				-- 幸运星奖励邮件内容
local DustRewardTitle = "背包满了";					-- 搓神灯奖励邮件标题
local DustRewardContent = "背包满了";				-- 搓神灯奖励邮件内容
local ParcloseDropTitle = "背包满了";				-- 副本掉落奖励邮件标题
local ParcloseDropContent = "背包满了";				-- 副本掉落奖励邮件内容

-- 魔法神灯说明
local MagicLampDescTitle = "神灯咨询";
local MagicLampDescContent = "魔法神灯有三大功能，分别为“主线故事”、“主题关卡”、“水晶魔镜”\n\n    主线故事：\n    主线故事是魔法神灯的连续主线剧情关卡，共分数个章节，每个章节有数十个关卡。跳舞完成条件即可过关，挑战成功后怪物会随机掉落各种好玩的“变身卡”或“宝盒”和魔法星尘等物品。\n    收集齐15个魔法星尘可搓神灯一次，神灯会给玩家各种丰富的奖励。\n    同时过关时会有1~3星不同的评价，收集到的星星越多，可以获得对应的礼包哦。\n\n    主题关卡：\n    主题关卡是每月的主题特殊关卡，它与众不同的地方在于，玩家过关后会得到一定数量的碎片，用这些“碎片”可以兑换当月专属的“星座服饰”，每个不同的星座服饰都有自己的独特光效。\n\n    每次挑战主线/主题关卡都需要消耗1个体力，体力每30分钟会恢复1个。\n\n    水晶魔镜：\n    水晶魔镜为好友神灯信息展示地，每个玩家的好友们在魔法神灯闯过的关卡和评级等通关新奇都可以在“水晶魔镜”内进行查看，观察你的好友们的战绩哦！";

-- 每天好友最大邀请次数
local MaxInviteFriendCount = 10;

-- 封面上的宣传图(循环播放)
local PublicityPicture = 
{
[1]	= { icon = "cct_1", atlas = "cct"},
[2]	= { icon = "cct_2", atlas = "cct"},
[3]	= { icon = "shuangzi", atlas = "cct"},
}

-- 关卡失败经验
local FailExp = 10;

-- 关卡胜利基本经验
local WinBaseExp = 20;

-- 关卡胜利评分经验
local WinGradeExp = 
{
-- index is difficulty, easy: 1, normal: 2, hard: 3
[1]	    = { SS = 30, S = 20, A = 12, B = 8, C = 4, D = 0 },
[2]		= { SS = 45, S = 30, A = 18, B = 12, C = 6, D = 0 },
[3]		= { SS = 60, S = 40, A = 24, B = 16, C = 8, D = 0 }
}

-- 每次赠送的体力值
local OnceGiveVitNum = 5;

-- 赠送、收取体力的重置时间点(从0点开始的秒数)
local GiveGetVitResetTime = 23*60*60 + 59*60;

-- 收取体力的最大值
local GetVitMaxNum = 30;

-- 关卡扫荡对非vip玩家是否开放
local QuickFinishOpenToNotVip = true;

-- 关卡扫荡消耗物品
local QuickFinishCostItem = "32001,1,-1";

-------------------------------------------------------------------------------------------------------------------------------------

--副本信息
function AddDungeonInfo(index, value)
	if value ~= nil then 
		local id          = value["id"];	
		local dungeonname = value["dungeonname"];
		local iconName 	  = value["iconName"];
		local storyID 	  = value["storyID"];
		local parcloseList= value["parcloseList"];
		
		AddDungeonListConfig( index, id, dungeonname, iconName, storyID, parcloseList );	
	end
end

--加载目标信息
function AddTargetInfo(index, value)
	if value ~= nil then 
	    local id   			= value["id"];	
		local targetname   	= value["targetname"];
		local targetFlag   	= value["targetFlag"];	
		local targetValue   = value["targetValue"];	
		
		AddTargetInfoConfig( index, id, targetname, targetFlag, targetValue );	
	end
end

--关卡信息
function AddParcloseInfo(index, value)
	if value ~= nil then 
		local id          = value["id"];	
		local crystalName = value["crystalName"];
		local costCrystal = value["costCrystal"];
		local sceneID 	  = value["sceneID"];
		local musicID     = value["musicID"];
		local musicName   = value["musicName"];
		local musicModle  = value["musicModle"];
		local musicLevel  = value["musicLevel"];
		local targets     = value["targets"];
		local isboss      = value["isboss"];
		local pos_x       = value["pos_x"];
		local pos_y       = value["pos_y"];
		local storyBeginID = value["storyBeginID"];
		local storyEndID = value["storyEndID"];
		local score = value["score"];
		
		AddParcloseInfoConfig( index, id, costCrystal, sceneID, musicID, musicName, musicModle, musicLevel, targets, isboss, pos_x, pos_y , crystalName, storyBeginID, storyEndID, score );	
	end
end

--许愿星奖励信息
function AddStarReward(index, value)
	if value ~= nil then 
		local id           = value["id"];	
		local coststar     = value["coststar"];
		local malereward   = value["malereward"];
		local femalereward = value["femalereward"];
		local money   	   = value["money"];
		
		AddStarRewardConfig( index, id, coststar, malereward, femalereward, money );	
	end
end

--剧情信息
function AddStory(index, value)
	if value ~= nil then 
	    local storyType     = value["storyType"];
	    local storyID   	= value["storyID"];	
		local dungeonID   	= value["dungeonID"];	
		local parcloseID  	= value["parcloseID"];
		local npcList     	= value["npcList"];
		local stroryContent = value["stroryContent"];
		
		AddStoryConfig( index, storyID,  npcList, stroryContent );	
	end
end

--水晶购买信息
function AddBuyCrystalInfo(index, value)
	if value ~= nil then 
	    local id   			= value["id"];
		local iconName   	= value["iconName"];	
		local crystalCount  = value["crystalCount"];
		local price     	= value["price"];
		local vipprice      = value["vipprice"]
		
		AddBuyCrystalInfoConfig( index, id, iconName, crystalCount, price, vipprice );	
	end
end

-- 经验
function AddGradeExp(index,value)
	if value ~= nil then
		local SS = value["SS"];
		local S = value["S"];
		local A = value["A"];
		local B = value["B"];
		local C = value["C"];
		local D = value["D"];
		
		AddGradeExpConfig( index, SS, S, A, B, C, D );
	end
end

-- 宣传图
function AddPublicityPicture(index,value)
	if value ~= nil then
	local icon = value["icon"];
	local atlas = value["atlas"];
			
	AddPublicityPictureConfig( index, icon, atlas );
	end
end
	

table.foreach(StarReward, AddStarReward);
table.foreach(buyCrystalInfo,AddBuyCrystalInfo);
AddDustRewardConfig( dustCost, dustMaleReward, dustFemaleReward, dustMoney, dustActivityName, dustIconName, dustStoryID );
AddCrystalConfig( crystalInitialValue, crystalMaxValue, crystalRestoreTime, crystalCreateDust, NotVipBuyVitNum);
AddParcloseItemList( SelectItemList );
AddSkipParcloseItem( SkipParcloseItem );
AddMagicLampRewardMail( StarRewardTitle, StarRewardContent, DustRewardTitle, DustRewardContent, ParcloseDropTitle, ParcloseDropContent );
AddMagicLampDesc( MagicLampDescTitle, MagicLampDescContent, MaxInviteFriendCount);
AddBaseExpConfig( FailExp, WinBaseExp );
table.foreach( WinGradeExp, AddGradeExp );
table.foreach( PublicityPicture, AddPublicityPicture );
AddGiveGetVitConfig(OnceGiveVitNum, GiveGetVitResetTime, GetVitMaxNum);
AddQuickFinishConfig(QuickFinishOpenToNotVip, QuickFinishCostItem);