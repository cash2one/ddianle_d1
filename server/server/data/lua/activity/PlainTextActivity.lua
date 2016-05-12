--是否显示在主页, 0表示不显示,大于0表示显示
local exhibit = 1;

--活动图片名称
local regularImageName = "activity-ad_10";
local thumbnailName = "huodongzhongxin_10";

--活动标题
local activity_title = "二次元盛典 热力四射high翻天";

--活动内容
local activity_content = "    活动时间：6月17日0点~6月30日23点59分\n    1.小熊爱照相：活动期间，累计消费30元，即可轻松获得可爱的小熊布偶，带着小熊布偶去光影魔术馆照相吧，每天进行1次照相分享，即可免费获得1个萌动英雄券A。\n    2.英雄福利有保障：活动期间，开启50次呆绿魔盒，即可获得绿色拳头手杖*永久；开启50次酷雷魔盒，即可获得雷神之锤*永久。\n    备注：奖励将在活动期间每周五发放1次，最终发放时间是7月2日。";

-- 活动显示起始时间
local show_begin_time = "2015-06-17 00:00:00";
local show_end_time = "2015-06-30 23:59:59";

local RewardInfo = 
{
      [1] = { rewardname = "小熊布偶*永久", malereward = "31186,1,-1", femalereward="31186,1,-1", money = 0 },
      [2] = { rewardname = "开启50次呆绿魔盒", malereward = "9074,1,-1", femalereward="9074,1,-1", money = 0 },
      [3] = { rewardname = "开启50次酷雷魔盒", malereward = "9054,1,-1", femalereward="9054,1,-1", money = 0 },
}

function AddPlainTextActivityRewardTableInfo(infotable)
	for index, value in ipairs(infotable) do
		local rewardname = value["rewardname"];	
		local malereward = value["malereward"];
		local femalereward = value["femalereward"];
		local money = value["money"];
		
		AddPlainTextActivityReward(rewardname, malereward, femalereward, money);
	end
end


function AddPlainTextActivityInfo(weight)
	if weight ~= nil then
	AddPlainTextActivityBriefInfo(exhibit, weight, regularImageName, thumbnailName, activity_title, activity_content, show_begin_time, show_end_time);	
	AddPlainTextActivityRewardTableInfo(RewardInfo);
	end
end