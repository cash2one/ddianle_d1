--节日祝福静态数据


--活动显示开始结束时间
local activityShowBeginTime = "2013-12-18 00:00:00";
local activityShowEndTime = "2014-1-9 00:00:00";

--祝福开始结束时间
local blessBeginTime = "2013-12-18 00:00:00";
local blessEndTime = "2014-1-9 00:00:00";


--祝福所需物品
local blessItemType = 31025;


--祝福语句,该部分最好只增加，数据库只存index不存具体语句
local blessWords = 
{
	[1] = {words = "圣诞节日少，祝福不能少，圣诞快乐不得了"};
	[2] = {words = "圣诞老人驾着雪橇只为把祝福给你送到：圣诞快乐！"};
	[3] = {words = "铃儿响，吉祥到；炉火旺，歌声扬；祈祷平安齐欢畅，祝你圣诞快乐！"};
	[4] = {words = "圣诞钟声敲响的那一刻，吉祥，幸福，一切归你，注意签收！"};
	[5] = {words = "白雪飘，钟声敲；铃儿响，喜气洋；圣歌唱，心花放。愿您圣诞开心伴！"};
}

--祝福别人得到的礼物
local sendBlessRewards =  
{
     maleReward = "31028,1,-1"; 
     femaleReward = "31028,1,-1"; 
     money = 0;
}


--祝福奖励,奖励最多只能为10个档次
--字段分别为:被祝福次数,男性玩家奖励,女性玩家奖励,金币
local blessRewards = 
{
	[1] = {beBlessedCount = 50, maleReward = "34091,1,-1", femaleReward = "34091,1,-1", money = 0},
	[2] = {beBlessedCount = 88, maleReward = "34092,1,-1", femaleReward = "34092,1,-1", money = 0},
	[3] = {beBlessedCount = 188, maleReward = "34093,1,-1", femaleReward = "34093,1,-1", money = 0},
}

function AddBlessActivityInfo_lua()
	AddFestivalBlessActivityInfoLua(activityShowBeginTime, activityShowEndTime, blessBeginTime, blessEndTime, blessItemType);
end

function AddSendBlessReward_lua()
	local maleReward = sendBlessRewards["maleReward"];
	local femaleReward = sendBlessRewards["femaleReward"];
	local money = sendBlessRewards["money"];
	AddFestivalSendBlessRewardLua(maleReward, femaleReward, money);
end

function AddBlessActivityWords_lua(index, value)
	if value ~= nil then
		local words = value["words"];
		AddFestivalBlessWordsLua(index, words);
	end
end

function AddBlessActivityReward_lua(index, value)
	if value ~= nil then
		local beBlessedCount = value["beBlessedCount"];		
		local maleReward = value["maleReward"];
		local femaleReward = value["femaleReward"];
		local money = value["money"];
		AddFestivalBlessRewardLua(index, beBlessedCount, maleReward, femaleReward, money);
	end
end


AddBlessActivityInfo_lua();
AddSendBlessReward_lua();
table.foreach(blessWords, AddBlessActivityWords_lua);
table.foreach(blessRewards, AddBlessActivityReward_lua);

