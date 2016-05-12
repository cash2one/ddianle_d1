-- 关底奖励基础配置
local RoomRewardBaseData = 
{
    mailtitle = "关底奖励";
    mailcontent = "恭喜你获得关底奖励";
}
-- 随机掉落数量配置（权重）
local RandRoomRewardCountData = 
{

    [1] = { count = "0", rate=70 },
    [2] = { count = "1", rate=30 },
    [3] = { count = "2", rate=0 },
    [4] = { count = "3", rate=0 },

}
-- 必掉物品配置（权重）
local CertainRoomRewardData = 
{

}
-- 随机基础物品配置（权重）
local RandBaseRoomRewardData = 
{
    [1] = { malereward = "31012,1,-1", femalereward="31012,1,-1", rate=10000 },
    [2] = { malereward = "31013,1,-1", femalereward="31013,1,-1", rate=10 },
    [3] = { malereward = "31014,1,-1", femalereward="31014,1,-1", rate=1 },
}
-- 随机模式物品配置（权重）
-- mode说明: 1-Taiko     2-Tradition     3-Osu     4-Rhythm
local RandModeRoomRewardData = 
{
    --[1] = { mode=1, malereward = "34006,1,-1", femalereward="34006,1,-1", rate=200 },
    --[2] = { mode=1, malereward = "34007,1,-1", femalereward="34007,1,-1", rate=200 },
    --[3] = { mode=2, malereward = "34008,1,-1", femalereward="34008,1,-1", rate=200 },
    --[4] = { mode=2, malereward = "34009,1,-1", femalereward="34009,1,-1", rate=200 },
    --[5] = { mode=3, malereward = "34010,1,-1", femalereward="34010,1,-1", rate=200 },
}
function ReadRoomRewardBaseData()
    local mailtitle = RoomRewardBaseData["mailtitle"];
    local mailcontent = RoomRewardBaseData["mailcontent"];

    RoomRewardBaseDataSet(mailtitle, mailcontent);
end

function AddRandRoomRewardCountData(index, value)
	if value ~= nil then
		local count = value["count"];
		local rate = value["rate"];
		
		AddRandRoomRewardCountInfo(count, rate);
	end
end

function AddCertainRoomRewardData(index, value)
	if value ~= nil then
		local malereward = value["malereward"];
		local femalereward = value["femalereward"];
		local rate = value["rate"];
		
		AddCertainRoomRewardInfo(malereward, femalereward, rate);
	end
end

function AddRandBaseRoomRewardData(index, value)
	if value ~= nil then
		local malereward = value["malereward"];
		local femalereward = value["femalereward"];
		local rate = value["rate"];
		
		AddRandRoomRewardInfo(0, malereward, femalereward, rate);
	end
end

function AddRandModeRoomRewardData(index, value)
	if value ~= nil then
		local mode = value["mode"];
		local malereward = value["malereward"];
		local femalereward = value["femalereward"];
		local rate = value["rate"];
		
		AddRandRoomRewardInfo(mode, malereward, femalereward, rate);
	end
end

ReadRoomRewardBaseData();
table.foreach(RandRoomRewardCountData, AddRandRoomRewardCountData);
table.foreach(CertainRoomRewardData, AddCertainRoomRewardData);
table.foreach(RandBaseRoomRewardData, AddRandBaseRoomRewardData);
table.foreach(RandModeRoomRewardData, AddRandModeRoomRewardData);
