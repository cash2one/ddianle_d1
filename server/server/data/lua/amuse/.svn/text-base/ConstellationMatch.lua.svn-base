-- 星恋魔法阵lua配置

--开放时间
local openTime = 
{
			--星期几		--开始时间				--结束时间
	[1] = { weekday = 1, startTime = "12:00:00", endTime = "14:00:00" },
	[2] = { weekday = 1, startTime = "20:00:00", endTime = "22:00:00" },
	[3] = { weekday = 2, startTime = "12:00:00", endTime = "14:00:00" },
	[4] = { weekday = 2, startTime = "20:00:00", endTime = "22:00:00" },
	[5] = { weekday = 3, startTime = "12:00:00", endTime = "14:00:00" },
	[6] = { weekday = 3, startTime = "20:00:00", endTime = "22:00:00" },
	[7] = { weekday = 4, startTime = "12:00:00", endTime = "14:00:00" },
	[8] = { weekday = 4, startTime = "20:00:00", endTime = "22:00:00" },
	[9] = { weekday = 5, startTime = "12:00:00", endTime = "14:00:00" },
	[10] = { weekday = 5, startTime = "20:00:00", endTime = "22:00:00" },
	[11] = { weekday = 6, startTime = "12:00:00", endTime = "14:00:00" },
	[12] = { weekday = 6, startTime = "20:00:00", endTime = "22:00:00" },
	[13] = { weekday = 0, startTime = "12:00:00", endTime = "14:00:00" },
	[14] = { weekday = 0, startTime = "20:00:00", endTime = "22:00:00" },
};

--活动规则
local rule = "活动规则：你是否觉得在恋舞中知心知音的小伙伴，总无缘遇到呢？其实恋舞处处有真情，最合适你的小伙伴就在你身边！恋舞最新推出最新系统---星恋魔法阵，让你轻松找到和你最默契，最有缘分的小伙伴！恋舞的精彩游戏人生从此不再孤单！邂逅星恋魔法阵，邂逅专属感动节拍！每天中午12点至14点、晚上20点至22点期间，星恋魔法阵都将开启爱的魔法！";

--角色等级要求
local requiredRoleLevel = 10;

--首次进入提示
local firstEnterTip = "　　你是否觉得恋舞中知心知音的小伙伴总无缘遇到呢？其实恋舞处处有真情，最合适你的小伙伴就在你身边！\n　　星恋魔法阵，让你轻松找到和你最默契，最有缘分的小伙伴！恋舞的精彩游戏人生从此不再孤单！\n　　每天中午12点至14点、晚上20点至22点期间，星恋魔法阵都将开启爱的魔法！";

--优先配对的卡牌数量和冷却时间(单位：秒)
local priorityCardCount = 2;
local priorityCardCoolDown = 300;

--优先配对卡使用次数限制 + VIP特权(见vip/VIP.lua)
local maxUseTimes = 3;

--专属房间关闭时间，单位：秒
local roomCloseTime = 3600;

--星座匹配频率，单位：毫秒!!!
local matchInterval = 30000;

--- 星座对照表 ---
--[[
英文        中文   数字编号
Aquarius    水瓶座    1
Pisces      双鱼座    2
Aries       白羊座    3
Taurus      金牛座    4
Gemini      双子座    5
Cancer      巨蟹座    6
Leo         狮子座    7
Virgo       处女座    8
Libra       天秤座    9
Scorpio     天蝎座    10
Sagittarius 射手座    11
Capricornus 摩羯座    12
]]--

--最优星座配对表
local matchTable = 
{
--- 星座  最优匹配星座
	[1] = {5, 7, 9},
	[2] = {6, 8, 10},
	[3] = {7, 9, 11},
	[4] = {8, 10, 12},
	[5] = {9, 11, 1},
	[6] = {10, 11, 2},
	[7] = {3, 11, 1},
	[8] = {4, 12, 2},
	[9] = {3, 5, 1},
	[10] = {4, 6, 2},
	[11] = {3, 5, 7},
	[12] = {4, 6, 8},
};

--可以进入的休闲场景
local amuseSceneID = { 1, 2, 3 };

-------------------------------------------------
-------- 下面的内容策划同学不用关心了 ----------
-------------------------------------------------
for i = 1, #openTime do
	if openTime[i] ~= nil then
		local weekday = openTime[i]["weekday"];
		local startTime = openTime[i]["startTime"];
		local endTime = openTime[i]["endTime"];
		AddConstellationMatchOpenTime(weekday, startTime, endTime);
	end
end


for i = 1, #matchTable do
	if matchTable[i] ~= nil then
		for k, v in pairs(matchTable[i]) do
			if v ~= nil then
				local constellation = i;
				local matchConstellation = v;
				AddConstellationMatchTableInfo(constellation, matchConstellation);
			end
		end
	end
end


function AddAmuseSceneIDLua(index, value)
	if value ~= nil then
		local sceneID = value;
		AddConstellationMatchAmuseSceneIDInfo(index, sceneID);
	end
end

table.foreach(amuseSceneID, AddAmuseSceneIDLua);

SetConstellationMatchInfo(requiredRoleLevel, firstEnterTip, priorityCardCount, priorityCardCoolDown, maxUseTimes, roomCloseTime, matchInterval, rule);
