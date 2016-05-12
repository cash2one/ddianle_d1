---舞团每天消耗
--开始时间
local DanceGroup_Start_time = "2014-7-17 00:00:00";
--结束时间
local DanceGroup_End_time = "2015-7-17 00:00:00";
--Tick时间
local DanceGroup_Trigger_Time = "00:00";

function AddDanceGroupTimerInfo_lua()
	AddDanceGroupTimerInfo( DanceGroup_Start_time, DanceGroup_End_time, DanceGroup_Trigger_Time)
end

AddDanceGroupTimerInfo_lua()
