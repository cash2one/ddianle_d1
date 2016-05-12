-- 好友模糊搜索

-- 好友搜索CD时间
local findFriendCoolDown = 1;
-- 好友搜索结果最大人数
local findFriendMaxCount = 50;

-- 查找好友的条件
local findFriendType = 
{ 
    -- index：显示顺序，值越小，在下拉列表中越靠前
    -- desc：客户端搜索好友的下拉列表上的文字描述
	[1] = { index = 0, desc = "好友ID", nType = 0 }, -- 0固定表示搜索ID
	[2] = { index = 1, desc = "好友昵称", nType = 1 }, -- 1固定表示搜索昵称
};

-- 玩家等级搜索条件
local findFriendLevel = 
{
    -- index：显示顺序，值越小，在下拉列表中越靠前
    -- desc：客户端搜索好友的下拉列表上的文字描述
    -- levelMin：-1表示不限，大于0表示搜索的最小等级（包括这个等级）
    -- levelMax：大于0表示搜索的最大等级（包括这个等级）
	[1] = { index = 0, desc = "不限", levelMin = -1, levelMax = -1 },
	[2] = { index = 1, desc = "1-50级", levelMin = 1, levelMax = 50 },
	[3] = { index = 2, desc = "51-100级", levelMin = 51, levelMax = 100 },
	[4] = { index = 3, desc = "100级以上", levelMin = 100, levelMax = -1 },
};

-- 玩家性别搜索条件
local findFriendSex = 
{
    -- index：显示顺序，值越小，在下拉列表中越靠前
    -- desc：客户端搜索好友的下拉列表上的文字描述
	[1] = { index = 0, desc = "不限", sex = -1 }, -- -1表示搜索全部
	[2] = { index = 1, desc = "男", sex = 1 }, -- 1固定表示搜索男性
	[3] = { index = 2, desc = "女", sex = 2 }, -- 2固定表示搜索女性
};

-- 年龄搜索条件
local findFriendAge = 
{
    -- index：显示顺序，值越小，在下拉列表中越靠前
    -- desc：客户端搜索好友的下拉列表上的文字描述
    -- ageMin：-1表示不限，大于0表示搜索的最小年龄（包括这个边界值）
    -- ageMax：大于0表示搜索的最大年龄（包括这个边界值）
	[1] = { index = 0, desc = "不限", ageMin = -1, ageMax = -1 },
	[2] = { index = 1, desc = "0-16岁", ageMin = 0, ageMax = 16 },
	[3] = { index = 2, desc = "17-22岁", ageMin = 17, ageMax = 22 },
	[4] = { index = 3, desc = "23-30岁", ageMin = 23, ageMax = 30 },
	[5] = { index = 4, desc = "31-40岁", ageMin = 31, ageMax = 40 },
	[6] = { index = 5, desc = "40岁以上", ageMin = 41, ageMax = -1 },
};

-- 星座搜索条件
local findFriendConstellation = 
{
    -- index：显示顺序，值越小，在下拉列表中越靠前
    -- desc：客户端搜索好友的下拉列表上的文字描述
	-- constellation：-1表示不限，实际的星座数值，要和前面的desc保持一致
	[1]  = { index = 0,  desc = "不限",   constellation = -1 },
	[2]  = { index = 1,  desc = "白羊座", constellation = 2 },	-- 这里的星座和文字描述要求一一对应
	[3]  = { index = 2,  desc = "金牛座", constellation = 3 },
	[4]  = { index = 3,  desc = "双子座", constellation = 4 },
	[5]  = { index = 4,  desc = "巨蟹座", constellation = 5 },
	[6]  = { index = 5,  desc = "狮子座", constellation = 6 },
	[7]  = { index = 6,  desc = "处女座", constellation = 7 },
	[8]  = { index = 7,  desc = "天秤座", constellation = 8 },
	[9]  = { index = 8,  desc = "天蝎座", constellation = 9 },
	[10] = { index = 9,  desc = "射手座", constellation = 10},
	[11] = { index = 10, desc = "摩羯座", constellation = 11},
	[12] = { index = 11, desc = "水瓶座", constellation = 0 },
	[13] = { index = 12, desc = "双鱼座", constellation = 1 },
};


-------------------------------------------------------------------------------
-- 以下内容策划不用改动
-------------------------------------------------------------------------------
SetFindFriendInfo(findFriendCoolDown, findFriendMaxCount);

function AddFindFriendTypeLua(key, value)
	if value ~= nil then
		AddFindFriendTypeInfo(value["index"], value["desc"], value["nType"]);
	end
end
table.foreach(findFriendType, AddFindFriendTypeLua);

function AddFindFriendLevelLua(key, value)
	if value ~= nil then
		local index = value["index"];
		local desc = value["desc"];
		local levelMin = value["levelMin"];
		local levelMax = value["levelMax"];
		AddFindFriendLevelInfo(index, desc, levelMin, levelMax);
	end
end
table.foreach(findFriendLevel, AddFindFriendLevelLua);


function AddFindFriendSexLua(key, value)
	if value ~= nil then
		local index = value["index"];
		local desc = value["desc"];
		local sex = value["sex"];
		AddFindFriendSexInfo(index, desc, sex);
	end
end
table.foreach(findFriendSex, AddFindFriendSexLua);


function AddFindFriendAgeLua(key, value)
	if value ~= nil then
		local index = value["index"];
		local desc = value["desc"];
		local ageMin = value["ageMin"];
		local ageMax = value["ageMax"];
		AddFindFriendAgeInfo(index, desc, ageMin, ageMax);
	end
end
table.foreach(findFriendAge, AddFindFriendAgeLua);


function AddFindFriendConstellationLua(key, value)
	if value ~= nil then
		local index = value["index"];
		local desc = value["desc"];
		local constellation = value["constellation"];
		AddFindFriendConstellationInfo(index, desc, constellation);
	end
end
table.foreach(findFriendConstellation, AddFindFriendConstellationLua);

