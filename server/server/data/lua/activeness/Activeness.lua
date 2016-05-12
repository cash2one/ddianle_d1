--活跃度系统

--名称、描述、开启和结束时间
local name = "活跃度系统";
local description = "每天只要完成以下目标就送好礼！";
local startTime = "2015-05-01 00:00:00";
local endTime = "2025-01-01 00:00:00";

--背包已满时，通过邮件发送奖励，邮件发送者名称，标题和内容，vip奖励邮件标题和内容（VIP相关配置暂时没有使用，可以不配置）
local mailSender = "系统";
local mailTitle = "活跃度奖励";
local mailContent = "您的背包已满，奖励已发放邮件";
local vipMailTitle = "VIP奖励邮件标题";
local vipMailContent = "VIP奖励邮件内容";

-----------------------------------------------------------------
-- BEGIN 这块配置策划不要改!!!
-----------------------------------------------------------------
--活跃度系统支持的目标类型，后面 activenessTarget 表中 targetType 字段只能填写下面这些目标
local TARGET_TYPE_CHECKIN = 1;            -- 完成每日签到
local TARGET_TYPE_DAILY_QUEST = 2;        -- 完成日常任务
local TARGET_TYPE_DUNGEON = 3;            -- 完成魔法神灯关卡
local TARGET_TYPE_DANCE_MODE = 4;         -- 完成不同模式的歌曲
local TARGET_TYPE_QUICK_DANCE = 5;        -- 使用一次快速开始并完成
local TARGET_TYPE_VIP_REWARD = 6;         -- 领取VIP福利
local TARGET_TYPE_SOCIAL_SHARE = 7;       -- 进行社交分享
local TARGET_TYPE_MAGICARRAY_CALL = 8;    -- 幸运魔法阵抽奖一次
local TARGET_TYPE_CLOTHEFFECT_REPLACE = 9;-- 魔法灌注一次
local TARGET_TYPE_CLOTHEFFECT_UPGRADE = 10;-- 魔法进阶一次
-----------------------------------------------------------------
-- END 这块配置策划不要改!!!
-----------------------------------------------------------------

--活跃度目标配置
local activenessTarget = 
{
    ------- 目标类型                               目标值          可获得分数    目标描述                        
    [1] = { targetType = TARGET_TYPE_CHECKIN,      targetValue = 1, score = 10, desc = "在福利袋里打开签到界面，并成功签到1次。", },
    [2] = { targetType = TARGET_TYPE_DAILY_QUEST,  targetValue = 1, score = 10, desc = "在任务列表里找到日常任务，按照任务条件并完成。", },
    [3] = { targetType = TARGET_TYPE_SOCIAL_SHARE, targetValue = 1, score = 10, desc = "在所有场景含有分享按钮（光影魔术馆，成绩界面，房间界面，时空之塔，恋舞商城，甜蜜殿堂）内拍照，并成功分享1次", },
    [4] = { targetType = TARGET_TYPE_DANCE_MODE,   targetValue = 4, score = 20, desc = "完成4种不同模式的歌曲包含（经典模式，泡泡模式，恋舞模式，彩虹模式，叮咚模式，劲舞模式，心动模式）", },
    [5] = { targetType = TARGET_TYPE_DUNGEON,      targetValue = 1, score = 10, desc = "在魔法神灯中，完成任意关卡。", },
    [6] = { targetType = TARGET_TYPE_QUICK_DANCE,  targetValue = 1, score = 10, desc = "在大厅点击快速开始，并完成匹配歌曲", },
    [7] = { targetType = TARGET_TYPE_VIP_REWARD,   targetValue = 1, score = 10, desc = "在福利袋中领取1次VIP每日福利。", },
	[8] = { targetType = TARGET_TYPE_MAGICARRAY_CALL,   targetValue = 1, score = 10, desc = "幸运魔法阵抽奖一次。", },
	[9] = { targetType = TARGET_TYPE_CLOTHEFFECT_REPLACE,   targetValue = 1, score = 10, desc = "魔法灌注一次。", },
	[10] = { targetType = TARGET_TYPE_CLOTHEFFECT_UPGRADE,   targetValue = 1, score = 10, desc = "魔法进阶一次。", },
};


-- 活跃度奖励
local activenessReward = 
{
-- 活跃度分数  奖励道具、金券和绑定M币
   [20] = { maleItem = "32002,1,-1|33003,1,-1",  femaleItem = "32002,1,-1|33003,1,-1",  money = 200 , bindMCoin = 0, 
           vipMaleItem = "", vipFemaleItem = "", vipMoney = 0, vipBindMCoin = 0}, -- VIP奖励暂时没有，即便配置也不会发放
		   
   [40] = { maleItem = "31012,1,-1|33001,1,-1", femaleItem = "31012,1,-1|33001,1,-1", money = 600 , bindMCoin = 0, 
           vipMaleItem = "", vipFemaleItem = "", vipMoney = 0, vipBindMCoin = 0},

   [60] = { maleItem = "31013,1,-1|33007,1,-1", femaleItem = "31013,1,-1|33007,1,-1", money = 0, bindMCoin = 200, 
           vipMaleItem = "", vipFemaleItem = "", vipMoney = 0, vipBindMCoin = 0},

};



------------------------------------------------------------------------------------------------------------------------------
--- 下面的内容，策划无需修改
------------------------------------------------------------------------------------------------------------------------------
SetActivenessInfo(name, description, startTime, endTime, mailSender, mailTitle, mailContent, vipMailTitle, vipMailContent);

function AddActivenessTargetRoutine(index, value)
    if value ~= nil then
        local targetType = value.targetType;
        local targetValue = value.targetValue;
		local targetScore = value.score;
        local targetDesc = value.desc;
        AddActivenessTargetInfo(targetType, targetValue, targetScore, targetDesc);
    end
end

function AddActivenessRewardRoutine(index, value)
    if index ~= nil and value ~= nil then
        local requireScore = index;
        local maleItem = value.maleItem;
        local femaleItem = value.femaleItem;
        local money = value.money;
        local bindMCoin = value.bindMCoin;
        local vipMaleItem = value.vipMaleItem;
        local vipFemaleItem = value.vipFemaleItem;
        local vipMoney = value.vipMoney;
        local vipBindMCoin = value.vipBindMCoin;
        AddActivenessRewardInfo(requireScore, maleItem, femaleItem, money, bindMCoin ,vipMaleItem, vipFemaleItem, vipMoney, vipBindMCoin);
    end
end

table.foreach(activenessTarget, AddActivenessTargetRoutine);
table.foreach(activenessReward, AddActivenessRewardRoutine);
