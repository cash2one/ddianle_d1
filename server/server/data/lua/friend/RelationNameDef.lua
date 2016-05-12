--称谓有性别要求
--哥哥、弟弟、姐姐、妹妹等有性别要求

-- name 留着方便策划填写
local FriendRelationName = 
{
	[1]	= {name = "好友", gender = 0}, 
	[2]	= {name = "哥哥", gender = 1},
	[3]	= {name = "弟弟", gender = 1},
	[4]	= {name = "姐姐", gender = 2},
	[5]	= {name = "妹妹", gender = 2},
	[6]	= {name = "老大", gender = 0},
	[7]	= {name = "小弟", gender = 0},
	[8]	= {name = "师傅", gender = 0},
	[9]	= {name = "弟子", gender = 0},
	[10] = {name = "主人", gender = 0},
	[11] = {name = "仆人", gender = 0},
	[12] = {name = "偶像", gender = 0},
	[13] = {name = "粉丝", gender = 0}
}

function GetFriendRelationNameDef(index, value)
	if value ~= nil then 
		local gender = value["gender"];
		local name = value["name"];
		AddFriendRelationNameDef(index, name, gender);
	end
end

table.foreach( FriendRelationName, GetFriendRelationNameDef );

