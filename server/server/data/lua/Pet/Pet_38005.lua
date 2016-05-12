--------------------下面内容为宠物蛋孵化列表配置--------------------

-- 孵化列表ID:
-- Pet_XXX.lua文件的孵化列表ID则为XXX
local id = 38005


-- 宠物蛋孵化列表
-- petTypeID为宠物类型ID，必须存在，否则孵化会有问题
local randomPetList = 
{
    -- 宠物类型      宠物潜质(参见Excel配置)
	{ petTypeID = 1, potential = 2, announce = false, rate = 300},
	{ petTypeID = 1, potential = 3, announce = false, rate = 500},
	{ petTypeID = 1, potential = 4, announce = false, rate = 190},
	{ petTypeID = 1, potential = 5, announce = false, rate = 10},
	{ petTypeID = 2, potential = 2, announce = false, rate = 300},
	{ petTypeID = 2, potential = 3, announce = false, rate = 500},
	{ petTypeID = 2, potential = 4, announce = false, rate = 190},
	{ petTypeID = 2, potential = 5, announce = false, rate = 10},
	{ petTypeID = 3, potential = 2, announce = false, rate = 300},
	{ petTypeID = 3, potential = 3, announce = false, rate = 500},
	{ petTypeID = 3, potential = 4, announce = false, rate = 190},
	{ petTypeID = 3, potential = 5, announce = false, rate = 10},
	{ petTypeID = 4, potential = 2, announce = false, rate = 300},
	{ petTypeID = 4, potential = 3, announce = false, rate = 500},
	{ petTypeID = 4, potential = 4, announce = false, rate = 190},
	{ petTypeID = 4, potential = 5, announce = false, rate = 10},
	{ petTypeID = 5, potential = 2, announce = false, rate = 300},
	{ petTypeID = 5, potential = 3, announce = false, rate = 500},
	{ petTypeID = 5, potential = 4, announce = false, rate = 190},
	{ petTypeID = 5, potential = 5, announce = false, rate = 10},
	--{ petTypeID = 2, potential = 1, announce = false, rate = 30},
	--{ petTypeID = 3, potential = 1, announce = true, rate = 10},
	--{ petTypeID = 4, potential = 1, announce = false, rate = 60},
	--{ petTypeID = 5, potential = 1, announce = false, rate = 30},
	--{ petTypeID = 6, potential = 1, announce = true, rate = 10},
	--{ petTypeID = 7, potential = 1, announce = false, rate = 60},
	--{ petTypeID = 8, potential = 1, announce = false, rate = 30},
	--{ petTypeID = 9, potential = 1, announce = true, rate = 10},
	--{ petTypeID = 10, potential = 1, announce = false, rate = 60},
	--{ petTypeID = 11, potential = 1, announce = false, rate = 30},
	--{ petTypeID = 12, potential = 1, announce = true, rate = 10},
	--{ petTypeID = 13, potential = 1, announce = false, rate = 60},
	--{ petTypeID = 14, potential = 1, announce = false, rate = 30},
	--{ petTypeID = 15, potential = 1, announce = true, rate = 10},
	--{ petTypeID = 16, potential = 1, announce = false, rate = 60},
}

--------------------下面内容不可修改--------------------

local function readPetInfo(index, value)
	if value ~= nil then
		local petTypeID = value["petTypeID"]
		local potential = value["potential"]
		local announce = value["announce"]
		local rate = value["rate"]

		AddRandomPetInfo(id, petTypeID, potential, announce, rate)
	end
end

table.foreach(randomPetList, readPetInfo)

