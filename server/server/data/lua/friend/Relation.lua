-- 0表示任意性别
-- 1表示异性
-- 2表示男男
-- 3表示女女

-- 1-好友
-- 2-兄弟
-- 3-兄妹
-- 4-姐弟
-- 5-姐妹
-- 6-帮派
-- 7-师徒
-- 8-主仆
-- 9-偶像粉丝

local FriendRelation = 
{
	[1]		= { left = 1, right = 1},
	[2]		= { left = 2, right = 3},
	[3]		= { left = 2, right = 5},
	[4]		= { left = 4, right = 3},
	[5]		= { left = 4, right = 5},
	[6]		= { left = 6, right = 7},
	[7]		= { left = 8, right = 9},
	[8]		= { left = 10, right = 11},
	[9]		= { left = 12, right = 13}
}

function GetFriendRelation(index, value)
	if value ~= nil then 
		local left = value["left"];
		local right = value["right"];
		
		AddFriendRelationInfo(index, left, right);
	end
end

table.foreach( FriendRelation, GetFriendRelation );

