--离婚规则
local DivorceDescription = "离婚说明";

--强制离婚道具
local CompulsoryItem = "31118,1,-1";

--复婚后亲密度回复百分比
local RemarryIntimacyRate = 80;

--离婚道具
local DivorceItem = "31019,1,-1";

--离婚赠送物品列表
local DivorceGift =
{
	[1] = { maleitemid = 32001, femaleitemid = 32001 },
	[2] = { maleitemid = 33001, femaleitemid = 33001 },
	[3] = { maleitemid = 42054, femaleitemid = 17949 }, 
	[4] = { maleitemid = 42059, femaleitemid = 42657 },
}

--离婚歌曲
local MusicList = 
{
	[1] = { musicname = "goround01" },
	[2] = { musicname = "goround02" },
	[3] = { musicname = "goround03" },
}

----------------------------------------------------------
function AddDivorceGiftConfigLua(index, value)
	if value ~= nil then	
		local maleitemid = value["maleitemid"];
		local femaleitemid = value["femaleitemid"];
		AddDivorceGiftConfig( maleitemid, femaleitemid);
	end
end

function AddDivorceMusicConfigLua(index, value)
	if value ~= nil then	
		local musicname = value["musicname"];
		AddDivorceMusicConfig( musicname);
	end
end

table.foreach(DivorceGift, AddDivorceGiftConfigLua);
table.foreach(MusicList, AddDivorceMusicConfigLua);
AddDivorceConfig( DivorceDescription,CompulsoryItem,RemarryIntimacyRate,DivorceItem);





