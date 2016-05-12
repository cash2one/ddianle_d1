--新手七天目标

--任务名称和描述
local questName = "新手七天目标";
local questDescription = "新玩家创建角色后7天内，只要完成以下目标就送好礼！";
--活动开启和结束时间
local activityStartTime = "2014-08-19 00:00:00";
local activityEndTime = "2099-01-01 00:00:00";

--背包已满时，通过邮件发送奖励，邮件标题和内容
local mailTitle = "失物招领";
local mailContent = "您上次在新手七日目标中遗失了一些物品哦！不过不用担心，小恋已经帮您找回来啦！感谢您对恋舞OL的支持，我们将加倍努力为您带来更好的服务。";

--vip奖励邮件标题和内容
local vipMailTitle = "失物招领";
local vipMailContent = "您上次在新手七日目标中遗失了一些物品哦！不过不用担心，小恋已经帮您找回来啦！感谢您对恋舞OL的支持，我们将加倍努力为您带来更好的服务。";

--活动时长, 单位：天
local questDuration = 7;

-----------------------------------------------------------------
-- BEGIN 这块配置策划不要改!!!
-----------------------------------------------------------------
-- quest任务表中的任务目标类型targetType字段只能填写如下列出的这些类型
local TARGETTYPE_ROLELEVEL = 3;          -- 角色等级类型任务
local TARGETTYPE_FRIENDCOUNT = 5;        -- 好友数量类型任务
local TARGETTYPE_GRADE = 9;              -- 跳舞判定类型任务
local TARGETTYPE_BUYITEMCOUNT = 10;      -- 道具购买数量类型任务
local TARGETTYPE_JOINDANCEGROUP = 11;    -- 加入舞团目标
local TARGETTYPE_HASLOVER = 12;          -- 拥有情侣任务

-- 判定类型的任务目标，值只能是如下几种值
local DANCEGRADE_D = 1;     -- D级判定
local DANCEGRADE_C = 2;     -- C级判定
local DANCEGRADE_B = 3;     -- B级判定
local DANCEGRADE_A = 4;     -- A级判定
local DANCEGRADE_S = 5;     -- S级判定
local DANCEGRADE_SS = 6;    -- SS级判定
-----------------------------------------------------------------
-- END 这块配置策划不要改!!!
-----------------------------------------------------------------

--7天任务表
local quest = 
{
    ------- 第几天   任务类型                                 任务目标                  任务目标描述        奖励...
    [1] = { day = 1, targetType = TARGETTYPE_ROLELEVEL,      targetValue = 3,           desc = "努力等级达到3级吧！", maleItem = "8009,1,-1",  femaleItem = "8009,1,-1",  money = 600,bindMCoin = 0 ,vipMaleItem = "31020,5,-1", vipFemaleItem = "31020,5,-1" , vipMoney = 0 , vipBindMCoin = 0};
    [2] = { day = 2, targetType = TARGETTYPE_GRADE,          targetValue = 2,           desc = "努力获得C级评价吧！", maleItem = "10061,1,-1",   femaleItem = "10061,1,-1",   money = 1200,bindMCoin = 0 ,vipMaleItem = "31017,1,-1", vipFemaleItem = "31017,1,-1" , vipMoney = 0 , vipBindMCoin = 0 };
    [3] = { day = 3, targetType = TARGETTYPE_FRIENDCOUNT,    targetValue = 1,           desc = "寻找1名小伙伴一起游戏吧！", maleItem = "25002,1,2592000|31026,2,-1", femaleItem = "25002,1,2592000|31026,2,-1", money = 0,bindMCoin = 0 ,vipMaleItem = "31012,2,-1", vipFemaleItem = "31012,2,-1" , vipMoney = 0 , vipBindMCoin = 0 };
    [4] = { day = 4, targetType = TARGETTYPE_JOINDANCEGROUP, targetValue = 1,           desc = "加入舞团大家庭吧！", maleItem = "31012,2,-1|31026,2,-1", femaleItem = "31012,2,-1|31026,2,-1", money = 0,bindMCoin = 0 ,vipMaleItem = "31010,1,-1", vipFemaleItem = "31010,1,-1" , vipMoney = 0 , vipBindMCoin = 0 };
    [5] = { day = 5, targetType = TARGETTYPE_BUYITEMCOUNT,   targetValue = 1,           desc = "用金券购买1次服饰！",   maleItem = "5048,1,-1|31026,1,-1",  femaleItem = "5048,1,-1|31026,1,-1",  money = 0,bindMCoin = 0 ,vipMaleItem = "31021,5,-1", vipFemaleItem = "31021,5,-1" , vipMoney = 2015 , vipBindMCoin = 0 };
    [6] = { day = 6, targetType = TARGETTYPE_GRADE,          targetValue = 3,           desc = "努力获得B级评价吧！",maleItem = "32002,3,-1|31026,5,-1", femaleItem = "32002,3,-1|31026,5,-1", money = 0,bindMCoin = 0 ,vipMaleItem = "25011,1,2592000", vipFemaleItem = "25011,1,2592000" , vipMoney = 0 , vipBindMCoin = 0 };
    [7] = { day = 7, targetType = TARGETTYPE_ROLELEVEL,      targetValue = 5,           desc = "努力等级达到5级吧！",maleItem = "7036,1,-1",  femaleItem = "7036,1,-1",  money = 1600,bindMCoin = 0 ,vipMaleItem = "31005,1,-1", vipFemaleItem = "31005,1,-1" , vipMoney = 0 , vipBindMCoin = 0 };
};


SetFresherQuest(questName, questDescription, mailTitle, mailContent, questDuration, activityStartTime, activityEndTime, vipMailTitle, vipMailContent);

function AddFresherQuestRoutine(index, value)
    if value ~= nil then
        local day = value.day;
        local targetType = value.targetType;
        local questTargetValue = value.targetValue;
        local targetDesc = value.desc;
        local maleItem = value.maleItem;
        local femaleItem = value.femaleItem;
        local money = value.money;
        local bindMCoin = value.bindMCoin;
        local vipMaleItem = value.vipMaleItem;
        local vipFemaleItem = value.vipFemaleItem;
        local vipMoney = value.vipMoney;
        local vipBindMCoin = value.vipBindMCoin;
        AddFresherQuest(day, targetType, questTargetValue, targetDesc, maleItem, femaleItem, money, bindMCoin ,vipMaleItem, vipFemaleItem, vipMoney, vipBindMCoin);
    end
end

table.foreach(quest, AddFresherQuestRoutine);
