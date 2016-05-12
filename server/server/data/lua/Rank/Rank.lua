
--排行榜抬头名称
local RankHeaderName = 
{
    [1] = {name1 = "名次", name2 = "等级",   name3 = "玩家名", name4 = "消费MB"},
    [2] = {name1 = "名次", name2 = "等级",   name3 = "玩家名", name4 = "消耗大喇叭"},
    [3] = {name1 = "名次", name2 = "等级",   name3 = "玩家名", name4 = "经验值上升"},
    [4] = {name1 = "名次", name2 = "舞团ID", name3 = "舞团名", name4 = "荣誉值上升"},
    [5] = {name1 = "名次", name2 = "丈夫",   name3 = "妻子",   name4 = "亲密度增加"},
    [6] = {name1 = "名次", name2 = "等级",   name3 = "玩家名", name4 = "收藏总数"},
    [7] = {name1 = "名次", name2 = "等级",   name3 = "玩家名", name4 = "闪亮值"},
    [8] = {name1 = "名次", name2 = "等级",   name3 = "玩家名", name4 = "勋章积分"},
}

--排行榜大厅
--排列顺序为大厅中的显示顺序，不需要显示，则不填即可，但type不可重复
-- type: 0.购物狂       1.喇叭王        2.升级达人
--       3.荣誉舞团     4.明星夫妻      5.收藏家
--       6.闪亮之星     7.勋章积分
-- header: “排行榜抬头名称”中的值
-- title: 排行榜显示标题图片名
-- maxCount: 排行榜最大排名人数
local RankHall = 
{
    [1] = {type = 5, header = 6, title = "ph_md_shoucangjia", maxCount = 100},
    [2] = {type = 6, header = 7, title = "ph_md_shanliang", maxCount = 100},
    [3] = {type = 7, header = 8, title = "ph_md_xunzhang", maxCount = 100},
    [4] = {type = 0, header = 1, title = "ph_md_xiaofei", maxCount = 50},
    [5] = {type = 1, header = 2, title = "ph_md_labawang", maxCount = 50},
    [6] = {type = 2, header = 3, title = "ph_md_shengjidaren", maxCount = 50},
    [7] = {type = 3, header = 4, title = "ph_md_wutuan", maxCount = 50},
    [8] = {type = 4, header = 5, title = "ph_md_mingxing", maxCount = 50},
}

-- 最大离线时间(天)
local MaxOfflineTime = 7

-- 奖励规则
local RewardRule = "排行榜奖励规则"
-- 积分规则
local ScoreRule = "排行榜积分规则"

local RankMailSender = "排行榜专员"
local RankMailTitle = "排行榜奖励"
local RankMailContent = "由于您在恋舞OL中的杰出表现，我们在此给予您特殊的奖励。您可以在排行榜中查看您的排名哟~"

--------------------下面内容不可修改--------------------

--本周开始时间
--周几，[0,6]，0表示周末
local RankStartWeekDay = 4
--开始时间，[0,23]
local RankStartHour = 3

local function LoadRankHeaderNameLoop(index, value)
    if value ~= nil then
        local name1 = value["name1"]
        local name2 = value["name2"]
        local name3 = value["name3"]
        local name4 = value["name4"]

        AddRankHeaderNameInfo(index, name1, name2, name3, name4)
    end
end

local function LoadRankHallLoop(index, value)
    if value ~= nil then
        local type = value["type"]
        local header = value["header"]
        local title = value["title"]
        local maxCount = value["maxCount"]

        AddRankHallInfo(index, type, header, title, maxCount)
    end
end

if ServerGetLine ~= nil and ServerGetLine() ~= nil then
    LoadRankStartTimeInfo(RankStartWeekDay, RankStartHour)
    LoadRankMaxOfflineTime(MaxOfflineTime)
    LoadRankRule(RewardRule, ScoreRule)

    table.foreach(RankHall, LoadRankHallLoop)

    print("Game server load rank info complete!")
else
    LoadRankStartTimeInfo(RankStartWeekDay, RankStartHour)
    LoadRankMaxOfflineTime(MaxOfflineTime)
    LoadRankMailInfo(RankMailSender, RankMailTitle, RankMailContent)

    table.foreach(RankHeaderName, LoadRankHeaderNameLoop)
    table.foreach(RankHall, LoadRankHallLoop)

    print("Group server load rank info complete!")
end
