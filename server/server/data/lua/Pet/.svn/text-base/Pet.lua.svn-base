
--------------------下面内容为宠物配置--------------------

-- 宠物栏免费个数
local freeGridNum = 30
-- 宠物栏扩展个数
local extendGridNum = 30
-- 扩展宠物栏消耗金钱类型(1.M币，2.金券，3.绑定M币)
local extendCostType = 2
-- 扩展宠物栏消耗金钱量
local extendCostAmount = 11
-- 每次最多可解锁宠物栏位数
local maxUnlockCountOnce = 99

-- 宠物修改昵称消耗金钱类型(1.M币，2.金券，3.绑定M币)
local changeNameCostType = 2
-- 宠物修改昵称消耗金钱量
local changeNameCostAmount = 100

-- 喂养增加宠物经验的物品ID
local feedExpItems = {31203, 31204, 31205, 31206}
-- 喂养增加宠物耐力的物品ID
local feedEnduranceItems = {31207, 31208, 31209}

-- 宠物技能规则
local skillRule = "技能重置和升级规则"

-- 宠物跳舞获得基础经验值
local danceBaseExp = 1

-- 宠物玩魔法神灯成功获得经验值
local danceDungeonSuccessExp = 3
-- 宠物玩魔法神灯失败获得经验值
local danceDungeonFailExp = 1

-- 宠物跳舞耐力消耗
local danceCostEndurence = 1

-- 1点耐力恢复所需时间(秒)
local enduranceRecoverySeconds = 300

-- 锁定技能不被重置的消耗表
local lockSkillCost = 
{        -- 锁定技能的个数      消耗道具     道具个数
	[1] = { skillCount = 1, itemid = 31210, itemCount = 1},
	[2] = { skillCount = 2, itemid = 31210, itemCount = 2},
	[3] = { skillCount = 3, itemid = 31210, itemCount = 3},
	[4] = { skillCount = 4, itemid = 31210, itemCount = 4},
}

-- 技能重置消耗表
local resetSkillCost = 
{        -- 重置的次数    消耗金钱倍数, 1表示基础价，3就是3倍基础价
	[1] = { resetTimes = 1, ratio = 1},
	[2] = { resetTimes = 2, ratio = 3},
	[3] = { resetTimes = 3, ratio = 5},
}


-- 新手引导过程中，可以免费孵化和加速孵化的步骤编号（该编号需要客户端支持），如果为0，就不免费，一般不要修改
local freeIncubationGuideStep = 10
local freeAccelerateGuideStep = 13
-- 新手引导过程中，赠送宠物蛋的步骤编号
local rewardPetEggStep = 6
-- 新手引导过程中，赠送的宠物蛋
local rewardPetEgg = "38000,1,-1"

--------------------下面内容不可修改--------------------

LoadPetSetting(freeGridNum, extendGridNum, extendCostType, extendCostAmount, maxUnlockCountOnce, 
	changeNameCostType, changeNameCostAmount, skillRule, danceBaseExp, danceDungeonSuccessExp, 
	danceDungeonFailExp, danceCostEndurence, enduranceRecoverySeconds,
    freeIncubationGuideStep, freeAccelerateGuideStep, rewardPetEggStep, rewardPetEgg)

for i, v in pairs(feedExpItems) do
    AddPetFeedExpItem(v)
end

for i, v in pairs(feedEnduranceItems) do
    AddPetFeedEnduranceItem(v)
end

local function AddLockSkillCost(index, value)
	if value ~= nil then
		local skillCount = value["skillCount"]
		local itemid = value["itemid"]
		local itemCount = value["itemCount"]

		AddLockSkillCostInfo(skillCount, itemid, itemCount)
	end
end

table.foreach(lockSkillCost, AddLockSkillCost)

local function AddResetSkillCost(index, value)
	if value ~= nil then
		local resetTimes = value["resetTimes"]
		local ratio = value["ratio"]

		AddResetSkillCostInfo(resetTimes, ratio)
	end
end

table.foreach(resetSkillCost, AddResetSkillCost)
