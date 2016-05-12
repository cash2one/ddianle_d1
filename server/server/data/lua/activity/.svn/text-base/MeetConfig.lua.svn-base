
--邂逅过程中拍摄点总个数
local photoIndexCount = 5;

--邂逅过程中等待时间(秒)
local waitTime = 120;

--邂逅结算后进入开放场景ID
local amuseSceneID = 4;

--玩家邂逅结算时不在线邮件title
local mailTitle = "邂逅奖励";

--玩家邂逅结算时不在线邮件content
local mailContent = "你在邂逅活动中获得如下奖励";

--玩家合拍次数奖励,名称,消耗
local photoIndexReward = 
{
	[1] = { indexname = "思念与回忆", malereward = "32001,2,-1", femalereward = "32001,2,-1", costitem = "31076,1,-1"},
	[2] = { indexname = "图书馆de偶遇", malereward = "32001,2,-1", femalereward = "32001,2,-1", costitem = "31076,1,-1"},
	[3] = { indexname = "我de心思你最懂", malereward = "32002,2,-1", femalereward = "32002,2,-1", costitem = "31076,1,-1"},
	[4] = { indexname = "第一次de约会", malereward = "32002,2,-1", femalereward = "32002,2,-1", costitem = "31076,1,-1"},
	[5] = { indexname = "女神de降临", malereward = "33004,1,-1", femalereward = "33004,1,-1", costitem = "31076,1,-1"},
}

function AddMeetReward(index, value)
	if value ~= nil then 
		local indexname    = value["indexname"];	
		local malereward   = value["malereward"];
		local femalereward = value["femalereward"];
		local costitem     = value["costitem"];
		
		AddMeetRewardInfo( index, indexname, malereward, femalereward, costitem );	
	end
end


AddMeetConfigInfo( photoIndexCount, waitTime, mailTitle, mailContent, amuseSceneID );
table.foreach(photoIndexReward, AddMeetReward);
