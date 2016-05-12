--徐良活动数据
local activityStartTime = "2014-8-19 00:00:00";
local activityEndTime = "2014-10-13 23:59:59";


--许愿消耗的物品个数 最后一项个endtimes表示最高次数，次数交集无法启动服务器
local christmasWishItemRequire = 
{
	[1] = {startTimes = 1, endTimes = 3, ItemType = 31024, ItemCount = 1},
	
};

--许愿得到的物品
local christmasWishItemReward = 
{
	[1] = {maleItems = "31026,1,-1", femaleItems = "31026,1,-1", money = 0},
	[2] = {maleItems = "31026,1,-1", femaleItems = "31026,1,-1", money = 0},
	[3] = {maleItems = "31026,1,-1", femaleItems = "31026,1,-1", money = 0},
	
};


function AddXmasMakeWishActivityData_lua()
	AddXmasMakeWishActivityDataLua(activityStartTime, activityEndTime);
end;

function AddXmasMakeWishItemRequire(index, value)
	if value ~= nil then
		local startTimes = value["startTimes"];
		local endTimes = value["endTimes"];
		local ItemType = value["ItemType"];
		local ItemCount = value["ItemCount"];
		AddXmasMakeWishItemDataRawLua(startTimes, endTimes, ItemType, ItemCount);
	end
end



function AddXmasMakeWishItemReward(index, value)
	if value ~= nil then
		local maleItems = value["maleItems"];
		local femaleItems = value["femaleItems"];
		local money = value["money"];
		AddXmasMakeWishRewardDataLua(index, maleItems, femaleItems, money);
	end
end


AddXmasMakeWishActivityData_lua();
table.foreach(christmasWishItemRequire, AddXmasMakeWishItemRequire);
table.foreach(christmasWishItemReward, AddXmasMakeWishItemReward);




