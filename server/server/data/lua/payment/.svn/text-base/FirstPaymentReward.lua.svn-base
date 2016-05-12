--------------------------免费充值数据------------------------------
local FreeVerPaymentTable = 
{
    [1] = 
    { 
        StartFirstPaymentTime = "2015-2-4 00:00:00", 
        EndFirstPaymentTime = "2016-6-10 10:00:00", 
        MaleFirstPaymentReward = "8079,1,1296000|40120,1,604800|42120, 1, 604800|44120,1,604800|54120,1,604800|22043,1,604800", 
        FemaleFirstPaymentReward = "8079,1,1296000|40620,1,604800|57620, 1, 604800|33003,1,-1|54620,1,604800|22043,1,604800", 
        MailTitle = "首充奖励" ,
        MailContent = "您充了(%d)M币, 额外返还您——绑定M币：(%d)，金券：(%d) ！" ,
        ChargeLevel = 1,                                   -- 表明 返利活动有 'n' 个等级, 当设置为 0 时, 忽略下面charge设置的内容
        ChargeRange = "1,-1",       -- 用 | 分割 返利等级:  充值区间
        ChargeMcoin = "50,3000",         -- 用 | 分割 返利等级:  M币返还百分比(正整数) , 固定值
        ChargeMoney = "100,0",         -- 用 | 分割 返利等级:  金券返还百分比(正整数) , 固定值
    },
    
};
--------------------------收费充值数据------------------------------

local ChargeVerPaymentTable = {
    [1] = 
    { 
 
        StartFirstPaymentTime = "2015-2-4 00:00:00", 
        EndFirstPaymentTime = "2016-6-10 10:00:00", 
        MaleFirstPaymentReward = "8079,1,1296000|40120,1,604800|42120, 1, 604800|44120,1,604800|54120,1,604800|22043,1,604800", 
        FemaleFirstPaymentReward = "8079,1,1296000|40620,1,604800|57620, 1, 604800|33003,1,-1|54620,1,604800|22043,1,604800", 
        MailTitle = "首充奖励" ,
        MailContent = "您充了(%d)M币, 额外返还您——绑定M币：(%d)，金券：(%d) ！" ,
        ChargeLevel = 1,                                   -- 表明 返利活动有 'n' 个等级, 当设置为 0 时, 忽略下面charge设置的内容
        ChargeRange = "1,-1",       -- 用 | 分割 返利等级:  充值区间
        ChargeMcoin = "50,3000",         -- 用 | 分割 返利等级:  M币返还百分比(正整数) , 固定值
        ChargeMoney = "100,0",         -- 用 | 分割 返利等级:  金券返还百分比(正整数) , 固定值
    },

 
};

---------------------------------------------------------------------
function GetFirstPaymentReward()
	for key,payment in 	pairs(FreeVerPaymentTable) do
		AddFirstRechargeInfo(
        true,                                        -- version
        payment["StartFirstPaymentTime"],            -- Start Time   
        payment["EndFirstPaymentTime"],              -- End Time
        payment["MaleFirstPaymentReward"],           -- Male Reward
        payment["FemaleFirstPaymentReward"],         -- Female Reward
        payment["MailTitle"],                        -- Mail Title
        payment["MailContent"],                      -- Mail Content
        payment["ChargeLevel"],                      -- Charge Levels
        payment["ChargeRange"],                      -- Charge Range
        payment["ChargeMcoin"],                      -- Charge Mcoin
        payment["ChargeMoney"]);                     -- Charge Money 
	end
	
	for key,payment in 	pairs(ChargeVerPaymentTable) do
		AddFirstRechargeInfo(
        false,
        payment["StartFirstPaymentTime"],            -- Start Time   
        payment["EndFirstPaymentTime"],              -- End Time
        payment["MaleFirstPaymentReward"],           -- Male Reward
        payment["FemaleFirstPaymentReward"],         -- Female Reward
        payment["MailTitle"],                        -- Mail Title
        payment["MailContent"],                      -- Mail Content
        payment["ChargeLevel"],                      -- Charge Levels
        payment["ChargeRange"],                      -- Charge Range
        payment["ChargeMcoin"],                      -- Charge Mcoin
        payment["ChargeMoney"]);                     -- Charge Money 
	end
end

GetFirstPaymentReward();
