--									光效石兑换配置

-- 活动开始、结束时间
local EffectStoneExchange_ActivityBeginTime = "2016-01-01 10:00:00";
local EffectStoneExchange_ActivityEndTime = "2016-12-01 10:00:00";

--活动名称
local activity_title = "系统消息";

--活动内容
local activity_content = "亲爱的友友大家好，光效石兑换活动开始了，请注意查收。";

--活动图片名称
local regularImageName = "activity-ad_02";
local thumbnailName = "huodongzhongxin_02";

--兑换信息 
local ExchangeItem = 
{
	[1] = {old_item = "33001,1,-1", new_item = "32001,10,-1"};
	[2] = {old_item = "33002,1,-1", new_item = "32001,10,-1"};
}
---------------------------------------------------------------------------------------------------
function  AddEffectStoneExchange_Lua(index, value)
    if value ~= nil then 
		local old_item = value["old_item"];
		local new_item = value["new_item"];

		AddEffectStoneExchangeItem(old_item, new_item);
    end
end

table.foreach(ExchangeItem, AddEffectStoneExchange_Lua);
AddEffectStoneExchangeConfig(EffectStoneExchange_ActivityBeginTime, EffectStoneExchange_ActivityEndTime, activity_title, activity_content, regularImageName, thumbnailName);




