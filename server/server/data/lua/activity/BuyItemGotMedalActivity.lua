-- 购买物品获取勋章

--是否显示在主页, 0表示不显示,大于0表示显示
local exhibit = 1;
--活动标题
local title = "购买物品获取勋章"
--活动内容
local content = "活动期间，在游戏中购买相应物品，即可获得稀有勋章！"
--活动起始时间
local beginTime = "2015-08-1 00:00:00"
local endTime = "2018-09-30 23:59:59"
--活动开启时间
local openTime = "2015-08-27 00:00:00"
local closeTime = "2018-09-29 23:59:59"
--活动图片
local smallImage = "huodongzhongxin_20"
local bigImage = "activity-ad_20"

--奖励信息
local rewardInfos = 
{
    [1] = { itemtype = 32001, num = 5, malereward = "101", femalereward="101" },
    [2] = { itemtype = 32003, num = 10, malereward = "102", femalereward="102" },
    [3] = { itemtype = 32004, num = 15, malereward = "103|104", femalereward="103|104" }
}

------------------------------下面内容不可修改------------------------------

local function AddRewardInfos(index, value)
	if value ~= nil then
		local itemtype = value["itemtype"]
		local num = value["num"]
		local malereward = value["malereward"]
		local femalereward = value["femalereward"]

		AddBuyItemGotMedalRewards(index, itemtype, num, malereward, femalereward)
	end
end

function AddBuyItemGotMedalActivity(weight)
	if weight ~= nil then
		AddBuyItemGotMedalInfo(exhibit, weight, beginTime, endTime, openTime, closeTime, 
			title, content, smallImage, bigImage)

		table.foreach(rewardInfos, AddRewardInfos)
	end
end

