--"[]"内的数字表示奖励编号,最少为3个，最多为5个

--活动开始时间
local activity_start_time = "2014-8-19 00:00:00";
--活动结束时间
local activity_end_time = "2014-10-13 23:59:59";

local christmasFreeGifts = 
{
	[1] = {maleItems = "31076,1,-1", femaleItems = "31076,1,-1", money = 0};
	[2] = {maleItems = "31076,1,-1", femaleItems = "31076,1,-1", money = 0};
	[3] = {maleItems = "31076,1,-1", femaleItems = "31076,1,-1", money = 0};
};

function AddXmaxRewardInfo_lua(index, value)
    if value ~= nil then
       local maleItems = value["maleItems"];
       local femaleItems = value["femaleItems"]; 
       local money = value["money"];
       
       AddXmasFreeRewardInfoLua(index, maleItems, femaleItems, money);
    end
end

function AddXmasRewardActivityInfo_lua()
	AddXmasFreeActivityInfo(activity_start_time, activity_end_time);
end

table.foreach(christmasFreeGifts, AddXmaxRewardInfo_lua);
AddXmasRewardActivityInfo_lua();













