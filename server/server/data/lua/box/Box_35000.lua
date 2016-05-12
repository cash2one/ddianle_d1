--物品组概率,万分比
local BoxMaleGroup_35000 =
{
	[1] = 10000,
	--[2] = 50,
	--[3] = 1,
};

local BoxFemaleGroup_35000 =
{
	[1] = 10000,
	--[2] = 60,
	--[3] = 1,
};

--组物品概率,权重
--announce=true/false
local BoxMaleItem_35000 =
{
	[1] = { group = 1, item = "31016,1,-1", announce = false, rate = 4000 },
	[2] = { group = 1, item = "32001,2,-1", announce = false, rate = 1000 },
	[3] = { group = 1, item = "32001,5,-1", announce = false, rate = 800 },
	[4] = { group = 1, item = "32002,1,-1", announce = false, rate = 747 },
	[5] = { group = 1, item = "82,1,2592000", announce = true, rate = 5 },
	[6] = { group = 1, item = "14081,1,2592000", announce = true, rate = 5 },
	[7] = { group = 1, item = "46,1,2592000", announce = true, rate = 10 },
	[8] = { group = 1, item = "14076,1,2592000", announce = true, rate = 10 },
	[9] = { group = 1, item = "4046,1,2592000", announce = true, rate = 10 },
	[10] = { group = 1, item = "5014,1,2592000", announce = true, rate = 5 },
	[11] = { group = 1, item = "46,1,-1", announce = true, rate = 3 },
	[12] = { group = 1, item = "14076,1,-1", announce = true, rate = 3 },
	[13] = { group = 1, item = "4046,1,-1", announce = true, rate = 1 },
	[14] = { group = 1, item = "5014,1,-1", announce = true, rate = 1 },
	[15] = { group = 1, item = "2029,1,604800", announce = false, rate = 200 },
	[16] = { group = 1, item = "2069,1,604800", announce = false, rate = 150 },
	[17] = { group = 1, item = "2443,1,604800", announce = false, rate = 150 },
	[18] = { group = 1, item = "4028,1,604800", announce = false, rate = 150 },
	[19] = { group = 1, item = "4004,1,604800", announce = false, rate = 150 },
	[20] = { group = 1, item = "4023,1,604800", announce = false, rate = 150 },
	[21] = { group = 1, item = "5013,1,604800", announce = false, rate = 150 },
	[22] = { group = 1, item = "6005,1,604800", announce = false, rate = 150 },
	[23] = { group = 1, item = "7012,1,604800", announce = false, rate = 150 },
	[24] = { group = 1, item = "8021,1,604800", announce = false, rate = 150 },
	[25] = { group = 1, item = "9012,1,604800", announce = false, rate = 150 },
	[26] = { group = 1, item = "10015,1,604800", announce = false, rate = 150 },
	[27] = { group = 1, item = "25000,1,86400", announce = false, rate = 150 },
	[28] = { group = 1, item = "25001,1,86400", announce = false, rate = 150 },
	[29] = { group = 1, item = "25002,1,86400", announce = false, rate = 150 },
	[30] = { group = 1, item = "25005,1,86400", announce = false, rate = 150 },
	[31] = { group = 1, item = "25009,1,86400", announce = false, rate = 150 },
	[32] = { group = 1, item = "14053,1,604800", announce = false, rate = 200 },
	[33] = { group = 1, item = "14004,1,604800", announce = false, rate = 200 },
	[34] = { group = 1, item = "40,1,604800", announce = false, rate = 200 },
	[35] = { group = 1, item = "4031,1,604800", announce = false, rate = 200 },
	--[3] = { group = 2, item = "31016,10,-1", announce = true, rate = 200 },
	--[4] = { group = 3, item = "34006,4,-1", announce = true, rate = 200 },
};

local BoxFemaleItem_35000 =
{
	[1] = { group = 1, item = "31016,1,-1", announce = false, rate = 4000 },
	[2] = { group = 1, item = "32001,2,-1", announce = false, rate = 1000 },
	[3] = { group = 1, item = "32001,5,-1", announce = false, rate = 800 },
	[4] = { group = 1, item = "32002,1,-1", announce = false, rate = 747 },
	[5] = { group = 1, item = "582,1,2592000", announce = true, rate = 5 },
	[6] = { group = 1, item = "14582,1,2592000", announce = true, rate = 5 },
	[7] = { group = 1, item = "544,1,2592000", announce = true, rate = 10 },
	[8] = { group = 1, item = "14577,1,2592000", announce = true, rate = 10 },
	[9] = { group = 1, item = "13505,1,2592000", announce = true, rate = 10 },
	[10] = { group = 1, item = "5014,1,2592000", announce = true, rate = 5 },
	[11] = { group = 1, item = "544,1,-1", announce = true, rate = 3 },
	[12] = { group = 1, item = "14577,1,-1", announce = true, rate = 3 },
	[13] = { group = 1, item = "13505,1,-1", announce = true, rate = 1 },
	[14] = { group = 1, item = "5014,1,-1", announce = true, rate = 1 },
	[15] = { group = 1, item = "2573,1,604800", announce = false, rate = 200 },
	[16] = { group = 1, item = "2505,1,604800", announce = false, rate = 150 },
	[17] = { group = 1, item = "2508,1,604800", announce = false, rate = 150 },
	[18] = { group = 1, item = "4818,1,604800", announce = false, rate = 150 },
	[19] = { group = 1, item = "4573,1,604800", announce = false, rate = 150 },
	[20] = { group = 1, item = "4807,1,604800", announce = false, rate = 150 },
	[21] = { group = 1, item = "5013,1,604800", announce = false, rate = 150 },
	[22] = { group = 1, item = "6503,1,604800", announce = false, rate = 150 },
	[23] = { group = 1, item = "7012,1,604800", announce = false, rate = 150 },
	[24] = { group = 1, item = "8021,1,604800", announce = false, rate = 150 },
	[25] = { group = 1, item = "9012,1,604800", announce = false, rate = 150 },
	[26] = { group = 1, item = "10015,1,604800", announce = false, rate = 150 },
	[27] = { group = 1, item = "25000,1,86400", announce = false, rate = 150 },
	[28] = { group = 1, item = "25001,1,86400", announce = false, rate = 150 },
	[29] = { group = 1, item = "25002,1,86400", announce = false, rate = 150 },
	[30] = { group = 1, item = "25005,1,86400", announce = false, rate = 150 },
	[31] = { group = 1, item = "25009,1,86400", announce = false, rate = 150 },
	[32] = { group = 1, item = "14511,1,604800", announce = false, rate = 200 },
	[33] = { group = 1, item = "14569,1,604800", announce = false, rate = 200 },
	[34] = { group = 1, item = "522,1,604800", announce = false, rate = 200 },
	[35] = { group = 1, item = "4554,1,604800", announce = false, rate = 200 },
	--[3] = { group = 2, item = "31016,10,-1", announce = true, rate = 200 },
	--[4] = { group = 3, item = "34006,4,-1", announce = true, rate = 200 },
};


function AddBoxMaleGroup(index, value)
	if value ~= nil then
		AddBoxGroupInfo( 35000, index, value, true );
	end
end

function AddBoxFemaleGroup(index, value)
	if value ~= nil then
		AddBoxGroupInfo( 35000, index, value, false );
	end
end

function AddBoxMaleItem(index, value)
	if value ~= nil then
		local groupid = value["group"];
		local iteminfo = value["item"];
		local announce = value["announce"];
		local itemrate = value["rate"];
		
		AddBoxItemInfo( 35000, groupid, iteminfo, announce, itemrate, true );
	end
end

function AddBoxFemaleItem(index, value)
	if value ~= nil then
		local groupid = value["group"];
		local iteminfo = value["item"];
		local announce = value["announce"];
		local itemrate = value["rate"];
		
		AddBoxItemInfo( 35000, groupid, iteminfo, announce, itemrate, false );
	end
end


table.foreach( BoxMaleGroup_35000, AddBoxMaleGroup );
table.foreach( BoxFemaleGroup_35000, AddBoxFemaleGroup );

table.foreach( BoxMaleItem_35000, AddBoxMaleItem );
table.foreach( BoxFemaleItem_35000, AddBoxFemaleItem );

