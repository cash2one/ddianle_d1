
--[[
排行榜奖励
在“排行榜大厅列表”中的type必须设置奖励，否则无奖励发放
second: 必须大于0，表示[x, second]区间
reward: 奖励物品(可不填)
7天: 604800      3天: 259200      1天: 86400       永久: -1
money: 金券(可不填)
bindmcoin: 绑定M币(可不填)

reward = {
    {second = 3, reward = "23001,1,604800", money = 0, bindmcoin = 0},
    {second = 10, reward = "23001,1,259200", money = 0, bindmcoin = 0},
    {second = 15, reward = "23001,1,86400", money = 0, bindmcoin = 0},
}
如上示例表示：[1,3]发放7天物品，[4,10]发放3天物品，[11,15]发放1天物品
--]]

-- 购物狂奖励
local MCoinReward = {
    {second = 10, reward = "22057,1,604800"},
    {second = 20, reward = "22057,1,259200"},
    {second = 50, reward = "22057,1,86400"},
}

-- 喇叭王奖励
local HornReward = {
    {second = 10, reward = "22058,1,604800"},
    {second = 20, reward = "22058,1,259200"},
    {second = 50, reward = "22058,1,86400"},
}

-- 升级达人奖励
local ExpReward = {
    {second = 10, reward = "22059,1,604800"},
    {second = 20, reward = "22059,1,259200"},
    {second = 50, reward = "22059,1,86400"},
}

-- 荣誉舞团奖励
local GroupHonoReward = {
    {second = 10, reward = "22061,1,604800"},
    {second = 20, reward = "22061,1,259200"},
    {second = 50, reward = "22061,1,86400"},
}

-- 明星夫妻奖励
local IntimacyReward = {
    {second = 10, reward = "22060,1,604800"},
    {second = 20, reward = "22060,1,259200"},
    {second = 50, reward = "22060,1,86400"},
}

-- 收藏家奖励
local ClothsReward = {
    {second = 1, reward = "32014,5,-1|32022,10,-1|32340,15,-1|32038,30,-1", bindmcoin = 5000, money = 500},
    {second = 2, reward = "32013,4,-1|32021,9,-1|32339,14,-1|32037,29,-1", bindmcoin = 4000, money = 400},
    {second = 3, reward = "32012,3,-1|32020,8,-1|32338,13,-1|32036,28,-1", bindmcoin = 3000, money = 300},
    {second = 10, reward = "32011,2,-1|32019,7,-1|32337,12,-1|32035,27,-1", bindmcoin = 2000, money = 200},
    {second = 20, reward = "32010,1,-1|32018,6,-1|32336,11,-1|32034,26,-1", bindmcoin = 1000, money = 100},
}

-- 闪亮之星奖励
local ClothEffectReward = {
    {second = 1, reward = "32340,15,-1|32332,20,-1", bindmcoin = 5500},
    {second = 2, reward = "32339,14,-1|32331,19,-1", bindmcoin = 4500},
    {second = 3, reward = "32338,13,-1|32330,18,-1", bindmcoin = 3500},
    {second = 20, reward = "32337,12,-1|32329,17,-1", bindmcoin = 2500},
    {second = 50, reward = "32336,11,-1|32328,16,-1", bindmcoin = 1500},
}

-- 勋章积分
local MedalReward = {
    {second = 1, reward = "32030,25,-1|32038,30,-1", bindmcoin = 5550},
    {second = 2, reward = "32029,24,-1|32037,29,-1", bindmcoin = 4550},
    {second = 3, reward = "32028,23,-1|32036,28,-1", bindmcoin = 3550},
    {second = 50, reward = "32027,22,-1|32035,27,-1", bindmcoin = 2550},
    {second = 100, reward = "32026,21,-1|32034,26,-1", bindmcoin = 1550},
}

--------------------下面内容不可修改--------------------

local function LoadRankReward(type, value)
    if value ~= nil then
        local second = value["second"] or 1
        local reward = value["reward"] or ""
        local money = value["money"] or 0
        local bindmcoin = value["bindmcoin"] or 0

        AddRankRewardInfo(type, second, reward, money, bindmcoin)
    end
end

local function LoadRankRewardLoop(type, value)
    if value ~= nil then
        for k,v in pairs(value) do
            LoadRankReward(type, v)
        end
    end
end

--[[
type:   0.购物狂        1.喇叭王        2.升级达人
        3.荣誉舞团      4.明星夫妻      5.收藏家
        6.闪亮之星      7.勋章积分
--]]

LoadRankRewardLoop(0, MCoinReward)
LoadRankRewardLoop(1, HornReward)
LoadRankRewardLoop(2, ExpReward)
LoadRankRewardLoop(3, GroupHonoReward)
LoadRankRewardLoop(4, IntimacyReward)
LoadRankRewardLoop(5, ClothsReward)
LoadRankRewardLoop(6, ClothEffectReward)
LoadRankRewardLoop(7, MedalReward)

