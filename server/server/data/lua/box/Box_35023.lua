--物品组概率,万分比
local BoxMaleGroup_35023 =
{
	[1] = 10000,
	[2] = 10000,
	--[3] = 1,
};

local BoxFemaleGroup_35023 =
{
	[1] = 10000,
	[2] = 10000,
	--[3] = 1,
};

--组物品概率,权重
--announce=true/false
local BoxMaleItem_35023 =
{
	[1] = { group = 1, item = "8004,1,-1", announce = false, rate = 5000 },
	[2] = { group = 1, item = "8005,1,-1", announce = false, rate = 5000 },
	[3] = { group = 1, item = "8006,1,-1", announce = false, rate = 5000 },
	[4] = { group = 1, item = "8007,1,-1", announce = false, rate = 5000 },
	[5] = { group = 1, item = "8010,1,-1", announce = false, rate = 5000 },
	[6] = { group = 1, item = "8011,1,-1", announce = false, rate = 5000 },
	[7] = { group = 1, item = "8013,1,-1", announce = false, rate = 5000 },
	[8] = { group = 1, item = "8015,1,-1", announce = false, rate = 5000 },
	[9] = { group = 1, item = "8016,1,-1", announce = false, rate = 4000 },
	[10] = { group = 1, item = "8017,1,-1", announce = false, rate = 5000 },
	[11] = { group = 1, item = "8021,1,-1", announce = false, rate = 5000 },
	[12] = { group = 1, item = "8022,1,-1", announce = false, rate = 5000 },
	[13] = { group = 1, item = "8024,1,-1", announce = false, rate = 5000 },
	[14] = { group = 1, item = "8025,1,-1", announce = false, rate = 5000 },
	[15] = { group = 1, item = "8026,1,-1", announce = false, rate = 5000 },
	[16] = { group = 1, item = "8027,1,-1", announce = false, rate = 5000 },
	[17] = { group = 1, item = "8032,1,-1", announce = false, rate = 5000 },
	[18] = { group = 1, item = "8036,1,-1", announce = false, rate = 5000 },
	[19] = { group = 1, item = "8001,1,-1", announce = false, rate = 5000 },
	[20] = { group = 1, item = "8002,1,-1", announce = false, rate = 5000 },
	[21] = { group = 1, item = "8031,1,-1", announce = true, rate = 1000 },
	[22] = { group = 2, item = "31041,5,-1", announce = false, rate = 100000 },
	
	
};

local BoxFemaleItem_35023 =
{
	[1] = { group = 1, item = "8004,1,-1", announce = false, rate = 5000 },
	[2] = { group = 1, item = "8005,1,-1", announce = false, rate = 5000 },
	[3] = { group = 1, item = "8006,1,-1", announce = false, rate = 5000 },
	[4] = { group = 1, item = "8007,1,-1", announce = false, rate = 5000 },
	[5] = { group = 1, item = "8010,1,-1", announce = false, rate = 5000 },
	[6] = { group = 1, item = "8011,1,-1", announce = false, rate = 5000 },
	[7] = { group = 1, item = "8013,1,-1", announce = false, rate = 5000 },
	[8] = { group = 1, item = "8015,1,-1", announce = false, rate = 5000 },
	[9] = { group = 1, item = "8016,1,-1", announce = false, rate = 4000 },
	[10] = { group = 1, item = "8017,1,-1", announce = false, rate = 5000 },
	[11] = { group = 1, item = "8021,1,-1", announce = false, rate = 5000 },
	[12] = { group = 1, item = "8022,1,-1", announce = false, rate = 5000 },
	[13] = { group = 1, item = "8024,1,-1", announce = false, rate = 5000 },
	[14] = { group = 1, item = "8025,1,-1", announce = false, rate = 5000 },
	[15] = { group = 1, item = "8026,1,-1", announce = false, rate = 5000 },
	[16] = { group = 1, item = "8027,1,-1", announce = false, rate = 5000 },
	[17] = { group = 1, item = "8032,1,-1", announce = false, rate = 5000 },
	[18] = { group = 1, item = "8036,1,-1", announce = false, rate = 5000 },
	[19] = { group = 1, item = "8001,1,-1", announce = false, rate = 5000 },
	[20] = { group = 1, item = "8002,1,-1", announce = false, rate = 5000 },
	[21] = { group = 1, item = "8031,1,-1", announce = true, rate = 1000 },
	[22] = { group = 2, item = "31041,5,-1", announce = false, rate = 100000 },
	
	
};


function AddBoxMaleGroup(index, value)
	if value ~= nil then
		AddBoxGroupInfo( 35023, index, value, true );
	end
end

function AddBoxFemaleGroup(index, value)
	if value ~= nil then
		AddBoxGroupInfo( 35023, index, value, false );
	end
end

function AddBoxMaleItem(index, value)
	if value ~= nil then
		local groupid = value["group"];
		local iteminfo = value["item"];
		local announce = value["announce"];
		local itemrate = value["rate"];
		
		AddBoxItemInfo( 35023, groupid, iteminfo, announce, itemrate, true );
	end
end

function AddBoxFemaleItem(index, value)
	if value ~= nil then
		local groupid = value["group"];
		local iteminfo = value["item"];
		local announce = value["announce"];
		local itemrate = value["rate"];
		
		AddBoxItemInfo( 35023, groupid, iteminfo, announce, itemrate, false );
	end
end


table.foreach( BoxMaleGroup_35023, AddBoxMaleGroup );
table.foreach( BoxFemaleGroup_35023, AddBoxFemaleGroup );

table.foreach( BoxMaleItem_35023, AddBoxMaleItem );
table.foreach( BoxFemaleItem_35023, AddBoxFemaleItem );

