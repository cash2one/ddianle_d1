--是否显示在主页, 0表示不显示，大于0表示显示
local exhibit = 0;

--活动图片名称
local regularImageName = "activity-ad_01";
local thumbnailName = "huodongzhongxin_01";

--活动标题
local activity_title = "激活码兑奖点";
--活动内容
local activity_content = "　　在下方输入框中输入激活码即可兑换相应的礼品!";

-- 活动显示起始时间
local show_begin_time = "2013-11-3 00:00:00";
local show_end_time = "2020-1-1 00:00:00";

local activity_begin_time = "2013-11-3 00:00:00";
local activity_end_time = "2020-1-1 00:00:00";

function AddGiftCodeInfo(weight)
	if weight ~= nil then
	  AddGiftCodeBriefInfo(exhibit, weight, regularImageName, thumbnailName, activity_title, activity_content, show_begin_time, show_end_time, activity_begin_time, activity_end_time);	
	end
end
