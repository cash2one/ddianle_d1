---
---新玩家招募活动
---

--邀请者活动起始时间
local activityStartTime = "2014-09-24 0:00:00";
local activityEndTime = "2014-12-02 23:59:59";

--被邀请者需要达到的等级
local requiredLevel = 20;

--邀请者活动标题, 活动内容
local inviterActivityName = "欢乐时光 与闺蜜共聚恋舞！";
local inviterActivityContent = "　　新一期好友招募活动开启了！玩家招募自己的好友一起玩恋舞。被邀请的玩家进入游戏后，输入邀请者的邀请码，即可获得精美大礼包。被邀请者等级达到20级以上后，邀请者即可获得奖励哦。一起分享快乐的恋舞时光吧！";

--邀请者的邮件标题和内容
local inviterMailTitle = "累计邀请奖励";
local inviterMailContent = "累计邀请奖励";

--邀请者的累计奖励
local inviterCumulativeReward = 
{
	[1] = { cumulation = 2, maleReward = "7013,1,604800", femaleReward = "7013,1,604800", money = 888 };
	[2] = {	cumulation = 5,	maleReward = "31026,2,-1|7013,1,1296000", femaleReward = "31026,2,-1|7013,1,1296000", money = 0 };
	[3] = { cumulation = 12, maleReward = "31020,5,-1|7013,1,2592000", femaleReward = "31020,5,-1|7013,1,2592000", money = 0 };
	[4] = { cumulation = 25, maleReward = "31026,5,-1|7013,1,-1", femaleReward = "31026,5,-1|7013,1,-1", money = 2888 };

}

--分享界面的动画
local inviterShareAnimation =
{
	[1] = {malePose = "PHOTO_BOY_01", femalePose = "PHOTO_GIRL_01"};
	[2] = {malePose = "PHOTO_BOY_02", femalePose = "PHOTO_GIRL_02"};
	[3] = {malePose = "PHOTO_BOY_03", femalePose = "PHOTO_GIRL_03"};
	[4] = {malePose = "PHOTO_BOY_04", femalePose = "PHOTO_GIRL_04"};
	[5] = {malePose = "PHOTO_BOY_05", femalePose = "PHOTO_GIRL_05"};
};

function AddInviterCumulativeReward(index, value)
	if value ~= nil then
		local cumulation = value["cumulation"];
		local maleReward = value["maleReward"];
		local femaleReward = value["femaleReward"];
		local money = value["money"];
		AddInvitationInviterReward(index, cumulation, maleReward, femaleReward, money);
	end
end

function AddInviterShareAnimation(index, value)
	if value ~= nil then
		AddInvitationInviterAnimation(index, value["malePose"], value["femalePose"]);
	end
end

AddInvitationInviterActivity(inviterActivityName, inviterActivityContent, requiredLevel, inviterMailTitle, inviterMailContent, activityStartTime, activityEndTime);
table.foreach(inviterCumulativeReward, AddInviterCumulativeReward);
table.foreach(inviterShareAnimation, AddInviterShareAnimation);
