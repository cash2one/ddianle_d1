--										长效累冲奖励

--充值档次奖励
local RechargeReward = 
{
	[1] = { mcoin = 10, malereward = "32001,2,-1", femalereward = "32001,2,-1", money = 10, bindmcoin = 20},
	[2] = { mcoin = 100, malereward = "32001,2,-1", femalereward = "32001,2,-1", money = 100, bindmcoin = 30},
	[3] = { mcoin = 500, malereward = "32001,2,-1", femalereward = "32001,2,-1", money = 1000, bindmcoin = 40},
	[4] = { mcoin = 10000, malereward = "32001,2,-1", femalereward = "32001,2,-1", money = 1000, bindmcoin = 50},
	[5] = { mcoin = 10001, malereward = "32001,2,-1", femalereward = "32001,2,-1", money = 10, bindmcoin = 20},
	[6] = { mcoin = 10002, malereward = "32001,2,-1", femalereward = "32001,2,-1", money = 100, bindmcoin = 30},
	[7] = { mcoin = 10003, malereward = "32001,2,-1", femalereward = "32001,2,-1", money = 1000, bindmcoin = 40},
	[8] = { mcoin = 10004, malereward = "32001,2,-1", femalereward = "32001,2,-1", money = 1000, bindmcoin = 50},
	[9] = { mcoin = 10005, malereward = "32001,2,-1", femalereward = "32001,2,-1", money = 10, bindmcoin = 20},
	[10] = { mcoin = 10006, malereward = "32001,2,-1", femalereward = "32001,2,-1", money = 100, bindmcoin = 30},
	[11] = { mcoin = 10007, malereward = "32001,2,-1", femalereward = "32001,2,-1", money = 1000, bindmcoin = 40},
	[12] = { mcoin = 10008, malereward = "32001,2,-1", femalereward = "32001,2,-1", money = 1000, bindmcoin = 50},
}

-- 玩家中途下线奖励发邮件
-- 标题
local mailTitle = "sad"
-- 内容
local mailContent = "sawdwwww"

----------------------------------------------------------------------------------------------------------------------
function AddRechargeReward(index, value)
	if value ~= nil then 
		local mcoin    = value["mcoin"];	
		local malereward   = value["malereward"];
		local femalereward = value["femalereward"];
		local money     = value["money"];
		local bindmcoin     = value["bindmcoin"];
		
		AddLongActingRechargeConfig( index, mcoin, malereward, femalereward, money, bindmcoin);	
	end
end

AddLongActingRechargeMail( mailTitle, mailContent);
table.foreach(RechargeReward, AddRechargeReward);