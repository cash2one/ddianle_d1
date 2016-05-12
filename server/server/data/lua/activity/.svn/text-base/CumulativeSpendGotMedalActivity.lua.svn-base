-- 累计消费获取勋章

--是否显示在主页, 0表示不显示,大于0表示显示
local exhibit = 1;
--活动标题
local title = "累计消费获取勋章"
--活动内容
local content = "活动期间，在游戏中累计消费，即可获得稀有勋章！"
--活动起始时间
local beginTime = "2015-08-1 00:00:00"
local endTime = "2018-09-30 23:59:59"
--活动开启时间
local openTime = "2015-08-27 00:00:00"
local closeTime = "2018-09-29 23:59:59"
--活动图片
local smallImage = "huodongzhongxin_20"
local bigImage = "activity-ad_20"

--消费累计类型(1:M币 2:绑定M币和M币)
local cumulativeType = 1

--奖励信息
local rewardInfos = 
{
    [1] = { requirenum = 1, malereward = "205", femalereward="205" },
    [2] = { requirenum = 2, malereward = "203", femalereward="203" }
}

------------------------------下面内容不可修改------------------------------

local function AddRewardInfos(index, value)
	if value ~= nil then
		local requirenum = value["requirenum"]
		local malereward = value["malereward"]
		local femalereward = value["femalereward"]

		AddCumulativeSpendGotMedalRewards(index, requirenum, malereward, femalereward)
	end
end

function AddCumulativeSpendGotMedalActivity(weight)
	if weight ~= nil then
		AddCumulativeSpendGotMedalInfo(exhibit, weight, beginTime, endTime, openTime, closeTime, 
			title, content, smallImage, bigImage, cumulativeType)

		table.foreach(rewardInfos, AddRewardInfos)
	end
end

