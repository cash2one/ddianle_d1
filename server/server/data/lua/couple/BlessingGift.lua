local blessRewards = 
{
	--[1] = {maleReward = "37000,1,-1", femaleReward = "37000,1,-1"},
	--[2] = {maleReward = "34092,1,-1", femaleReward = "34092,1,-1"},
	--[3] = {beBlessedCount = 188, maleReward = "34093,1,-1", femaleReward = "34093,1,-1", money = 0},
}

function AddBlessingGift_lua(index, value)
	if value ~= nil then	
		local maleReward = value["maleReward"];
		local femaleReward = value["femaleReward"];
		AddBlessingGiftLua(index, maleReward, femaleReward);
	end
end

table.foreach(blessRewards, AddBlessingGift_lua);