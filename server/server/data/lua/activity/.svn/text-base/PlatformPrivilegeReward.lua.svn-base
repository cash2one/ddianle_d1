--平台特权奖励

--奖励名次和内容
local rewardname = "平台特权奖励";
local rewardContent = "是平台会员，条件满足游戏需求";

--通过邮件发送奖励，邮件标题和内容
local mailTitle = "平台特权奖励标题";
local mailContent = "平台特权奖励内容";

--1,UC平台
--特权编号 满足需求最低等级 最高等级
local ucRewardList = {
	[1] = {vip = 1, lowLevel=1, highLevel=5, maleItem="7003,1,604800|5003,1,604800|33003,1,-1", femaleItem="7003,1,604800|5003,1,604800|33003,1,-1" },
	[2] = {vip = 2, lowLevel=1, highLevel=5, maleItem="10013,1,2592000|6002,1,2592000|31020,1,2592000", femaleItem="10013,1,2592000|6503,1,2592000|31020,1,25920000" },
	[3] = {vip = 3, lowLevel=1, highLevel=5, maleItem="25019,1,-1",femaleItem="25019,1,-1" },
	[4] = {vip = 4, lowLevel=3, highLevel=5, maleItem="22032,1,-1",femaleItem="22032,1,-1" },
	[5] = {vip = 5, lowLevel=3, highLevel=5, maleItem="7022,1,-1",femaleItem="7023,1,-1" },
};

AddPlatformPrivilege( rewardname, rewardContent, mailTitle, mailContent );

--UC
function AddUCRewardRoutine( index, value )
	if value ~= nil then
		AddPlatformPrivilegeReward( value.vip, value.lowLevel, value.highLevel, value.maleItem, value.femaleItem, 1 );
	end
end

table.foreach( ucRewardList, AddUCRewardRoutine );