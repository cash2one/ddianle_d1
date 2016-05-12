--------------------下面内容为舞团试炼配置--------------------

-- 舞团试炼开启等级
local openLevel = 2

-- 免费挑战机会
local freeChance = 3

-- 鼓舞(1.M币，2.金券，3.绑定M币)
local inspireCostType = 3
local inspireCostAmount = 10
-- 鼓舞加成百分比
-- 鼓舞1次，总体加1%，个人额外加1%
local inspirePercentForAll = 1
local inspirePercentForSelf = 1
-- 最大鼓舞次数
local inspireMaxCountForAll = 30
local inspireMaxCountForSelf = 30
-- 每次鼓舞奖励舞团贡献
local inspireRewardGroupContribution = 10

-- 随机物品个数
-- ranking排名，从1开始
-- count物品个数
local randItemCount = 
{
	{ ranking = 3, count = 2 },
	{ ranking = 10, count = 1 },
}

-- 邮件
local mailSender = "舞团试炼专员"
local mailTitle = "舞团试炼奖励"
local mailContent = "您的背包已满，舞团试炼专员已帮您找回奖励物品"

-- 说明
local help = "1.这是个说明。\n2.这是个说明。\n3.这是个说明。\n4.重要的事情说3遍！"

--------------------下面内容不建议修改--------------------

-- [公告] - 锁定前公告时间 - [锁定] - 锁定时间 - [重置] - 锁定时间 - [开启]

-- 重置时间
-- [0,6]，0表示周日
local resetWeekday = 1
-- [0,23]
local resetHour = 3
-- 锁定时间(分钟)
-- 建议5分钟以上
local lockTime = 10
-- 锁定前公告时间(分钟)
local announcementTime = 5

--------------------下面内容不可修改--------------------

local function LoadRandItemCount()
	for k,v in pairs(randItemCount) do
		if v ~= nil then
			local ranking = v["ranking"]
			local count = v["count"]

			LoadDanceGroupChallengeRandItemCount(ranking, count)
		end
	end
end

if ServerGetLine ~= nil and ServerGetLine() ~= nil then
	LoadRandItemCount()
	LoadDanceGroupChallengeInfos(freeChance, openLevel, 
		inspireCostType, inspireCostAmount, inspireMaxCountForSelf, 
		resetWeekday, resetHour, lockTime, announcementTime, 
		mailSender, mailTitle, mailContent, 
		help)
else
	LoadDanceGroupChallengeInfos(inspirePercentForAll, inspirePercentForSelf, 
		inspireMaxCountForAll, inspireMaxCountForSelf, inspireRewardGroupContribution, 
		resetWeekday, resetHour)
end
