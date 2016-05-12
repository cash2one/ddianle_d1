-- 额外经验奖励活动脚本
local ExtraExpActivityData = 
{
    [1] = { begintime = "2015-06-26 0:00:00", endtime = "2015-06-26 23:59:00", starttime="20:00", stoptime="22:00", rewardexprate = 100 },
    [2] = { begintime = "2015-06-27 0:00:00", endtime = "2015-06-27 23:59:00", starttime="20:00", stoptime="22:00", rewardexprate = 100 },
    [3] = { begintime = "2015-07-11 0:00:00", endtime = "2015-07-11 23:59:00", starttime="20:00", stoptime="22:00", rewardexprate = 100 },
    [4] = { begintime = "2015-07-12 0:00:00", endtime = "2015-07-12 23:59:00", starttime="20:00", stoptime="22:00", rewardexprate = 100 },
}

function AddExtraExpActivityData(index, value)
	if value ~= nil then
		local begintime = value["begintime"];
		local endtime = value["endtime"];
		local starttime = value["starttime"];
		local stoptime = value["stoptime"];
		local rewardexprate = value["rewardexprate"];
		
		AddExtraExpActivity(index, begintime, endtime, starttime, stoptime, rewardexprate);
	end
end

table.foreach(ExtraExpActivityData, AddExtraExpActivityData);
