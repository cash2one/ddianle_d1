
-- Price 月卡的价格（M币）
-- MonthDays 月卡持续天数
-- RenewDays 月卡可以续费天数
-- OpenBindMReward 月卡开通奖励
-- OpenMaleItemReward 月卡开通男性物品奖励
-- OpenFemaleItemReward 月卡开通女性物品奖励
-- OpenMoneyReward 月卡开通钱奖励
-- EveryDayBindMReward 月卡每日绑定M币奖励
-- EveryDayMaleItemReward 月卡每日男性物品奖励
-- EveryDayFemaleItemReward 月卡每日女性物品奖励
-- EveryDayMoneyReward 月卡每日钱奖励
-- desc 说明
local MonthCardInfo =
{
	[1] = { Price = 3000, MonthDays = 30, RenewDays = 3, OpenBindMReward = 2800, OpenMaleItemReward = "", OpenFemaleItemReward = "", OpenMoneyReward = 0, EveryDayBindMReward = 1000,
			EveryDayMaleItemReward = "", EveryDayFemaleItemReward = "", EveryDayMoneyReward = 0     , Desc = "    花费3000M币即可立即获得2800的绑定M币，30天内，每天登陆游戏都可领取1000的绑定M币，总计可获得32800绑定M币，10倍的返利，超值的享受！\n    备注：月卡同时只能持有一张，到期前3天可重新购买新的月卡。"};
}

function AddMonthCardInfo_Lua( index, value )
	if value ~= nil then
		local Price = value["Price"];
		local MonthDays = value["MonthDays"];
		local RenewDays = value["RenewDays"];
		local OpenBindMReward = value["OpenBindMReward"];
		local OpenMaleItemReward = value["OpenMaleItemReward"];
		local OpenFemaleItemReward = value["OpenFemaleItemReward"];
		local OpenMoneyReward = value["OpenMoneyReward"];
		local EveryDayBindMReward = value["EveryDayBindMReward"];
		local EveryDayMaleItemReward = value["EveryDayMaleItemReward"];
		local EveryDayFemaleItemReward = value["EveryDayFemaleItemReward"];
		local EveryDayMoneyReward = value["EveryDayMoneyReward"];
		local Desc = value["Desc"];
		
		AddMonthCardInfo( index, Price, MonthDays, RenewDays, OpenBindMReward, OpenMaleItemReward, OpenFemaleItemReward,OpenMoneyReward, 
				EveryDayBindMReward, EveryDayMaleItemReward, EveryDayFemaleItemReward, EveryDayMoneyReward, Desc );
	end
end

table.foreach(MonthCardInfo, AddMonthCardInfo_Lua);

