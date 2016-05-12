--防沉迷相关数据

--防沉迷开关
local openAntiaddiction = 0
--连续离线多少分钟才能脱离沉迷状态
local offlineMinutes = 300

--防沉迷配置， 配置id对应配置内容，timeRange单位是分钟，"a,b"表示半开区间[a, b)，"a"表示a到无穷
local antiaddictionConfig = 
{
    [1] = 
	{
		{timeRange="180,300", tipDiffMinutes=30, tipText="您已进入疲劳游戏时间，您的游戏收益将减半，为了您的健康，请尽快下线休息", danceExpDecr=0.5, amuseExpDecr=0.5, questExpDecr=0.5, questGoldTickDecr=0.5, fightItemDropCountDecr=0.5},
		{timeRange="300", tipDiffMinutes=15, tipText="您已进入不健康游戏时间，您的游戏收益将归零，您的累计下线时间满5小时后，才能恢复正常", danceExpDecr=1, amuseExpDecr=1, questExpDecr=1, questGoldTickDecr=1, fightItemDropCountDecr=1},
	},
};

--各个平台对应的防沉迷配置，配置id对应平台id集合（以逗号间隔），每个平台只能对应一个配置，不能对应多个
local platformAntiaddictionConfig = 
{
	[1] = {platform="8"},
}

function AddAntiaddictionConfig_lua(index, value)
    if value ~= nil then
        table.foreach(value, function(i, v) 
        local timeRange = v["timeRange"];--时间区间，分钟
        local tipDiffMinutes = v["tipDiffMinutes"];--提示间隔分钟数
        local tipText = v["tipText"];--提示内容
        local danceExpDecr = v["danceExpDecr"];--跳舞经验减量百分比，0.5表示减少50%
        local amuseExpDecr = v["amuseExpDecr"];--开放性场景经验减量百分比，0.5表示减少50%
        local questExpDecr = v["questExpDecr"];--任务经验减量百分比，0.5表示减少50%
        local questGoldTickDecr = v["questGoldTickDecr"];--任务获得金券减量百分比，0.5表示减少50%
        local fightItemDropCountDecr = v["fightItemDropCountDecr"];--战斗道具掉落数量减量百分比，0.5表示减少50%，大类是1的不掉
        AddAntiaddictionConfigLua(index, timeRange, tipDiffMinutes, tipText, danceExpDecr, amuseExpDecr, questExpDecr, questGoldTickDecr, fightItemDropCountDecr);
        end);
    end
end

function SetPlatformAntiaddictionConfig_lua(index, value)
    if value ~= nil then
        local platform = value["platform"];
        SetPlatformAntiaddictionConfigLua(index, platform);
    end
end
SetOpenAntiaddictionLua(openAntiaddiction)
SetOfflineMinutesLua(offlineMinutes)
table.foreach(antiaddictionConfig, AddAntiaddictionConfig_lua);
table.foreach(platformAntiaddictionConfig, SetPlatformAntiaddictionConfig_lua);