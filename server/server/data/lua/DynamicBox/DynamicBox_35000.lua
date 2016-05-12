--物品组概率,万分比
local DynamicBoxMaleGroup_35000 =
{
	--[1] = 10000,
	--[2] = 50,
	--[3] = 1,
};

local DynamicBoxFemaleGroup_35000 =
{
	--[1] = 10000,
	--[2] = 60,
	--[3] = 1,
};

-- 动态绑定关联组信息(关联值:关联值兑换比例:物品兑换比例)
local DynamicGroup = "35001:1|1";

--组物品概率,权重
--announce=true/false
local DynamicBoxMaleItem_35000 =
{
	[1] = { group = 1, item = "31016,1,-1", announce = false, rate = 4000 },

	--[2] = { group = 2, item = "31016,10,-1", announce = true, rate = 200 },
	--[3] = { group = 3, item = "34006,4,-1", announce = true, rate = 200 },
};

local DynamicBoxFemaleItem_35000 =
{
	[1] = { group = 1, item = "31016,1,-1", announce = false, rate = 4000 },
	
	--[2] = { group = 2, item = "31016,10,-1", announce = true, rate = 200 },
	--[3] = { group = 3, item = "34006,4,-1", announce = true, rate = 200 },
};


function AddDynamicBoxMaleGroup(index, value)
	if value ~= nil then
		AddDynamicBoxGroupInfo( 35000, index, value, true );
	end
end

function AddDynamicBoxFemaleGroup(index, value)
	if value ~= nil then
		AddDynamicBoxGroupInfo( 35000, index, value, false );
	end
end

function AddDynamicBoxMaleItem(index, value)
	if value ~= nil then
		local groupid = value["group"];
		local iteminfo = value["item"];
		local announce = value["announce"];
		local itemrate = value["rate"];
		
		AddDynamicBoxItemInfo( 35000, groupid, iteminfo, announce, itemrate, true );
	end
end

function AddDynamicBoxFemaleItem(index, value)
	if value ~= nil then
		local groupid = value["group"];
		local iteminfo = value["item"];
		local announce = value["announce"];
		local itemrate = value["rate"];
		
		AddDynamicBoxItemInfo( 35000, groupid, iteminfo, announce, itemrate, false );
	end
end


table.foreach( DynamicBoxMaleGroup_35000, AddDynamicBoxMaleGroup );
table.foreach( DynamicBoxFemaleGroup_35000, AddDynamicBoxFemaleGroup );

table.foreach( DynamicBoxMaleItem_35000, AddDynamicBoxMaleItem );
table.foreach( DynamicBoxFemaleItem_35000, AddDynamicBoxFemaleItem );
AddDynamicBoxRelatedGroupInfo( 35000, DynamicGroup );

