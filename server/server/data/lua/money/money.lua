--这是金券清零的lua脚本
--每个月金券清零的时间
local MoneyRstDate = 
{
    day = 1,		--每月清零日期
    hour = 2,		--小时
    minute = 00,	--分钟
    second = 00,	--秒
}

--在金券清零的前X天内发邮件提醒玩家(在这些天内仅一次)
local nRemaindDaysBeforeMoneyRst = 7;

local mailTitle = "金券清零通知";
local mailContent = "您的金券将会在下月1号02:00:00清零，请尽快使用您的金券，祝您游戏愉快！";

function AddMoneyRstInfo()
    local day = MoneyRstDate["day"];
    local hour = MoneyRstDate["hour"];
    local minute = MoneyRstDate["minute"];
    local second = MoneyRstDate["second"];
    ReadMoneyRstInfo(day, hour, minute, second, nRemaindDaysBeforeMoneyRst, mailTitle, mailContent);
end


AddMoneyRstInfo();
