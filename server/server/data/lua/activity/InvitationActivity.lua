---
---新玩家招募活动
---

--是否显示在主页, 0表示不显示，大于0表示显示
local exhibit = 0;

--活动图片名称
local regularImageName = "";
local thumbnailImageName = "huodongzhongxin_12";

--被邀请者活动起始时间
local activityBeginTime = "2014-09-24 0:00:00";
local activityEndTime = "2014-12-02 23:59:59";

--被邀请者活动标题, 活动内容
local activityName = "绑定邀请码";
local activityContent = "　　绑定邀请码，和朋友一起分享快乐恋舞时光吧！";

--显示等级
local visibleMaxLevel = 10;

--被邀请者的奖励
local maleReward = "134,1,604800|17134,1,604800|14134,1,604800|7024,1,2592000|31026,8,-1";
local femaleReward = "634,1,604800|17634,1,604800|14634,1,604800|7024,1,2592000|31026,8,-1";
local money = 2888;

--被邀请者的邮件标题和内容
local mailTitle = "奖励邮件标题";
local mailContent = "奖励邮件内容";

--被邀请者活动
function AddInvitaionInfo(weight)
	if weight ~= nil then
		AddInvitationActivity(weight, exhibit, regularImageName, thumbnailImageName, 
			activityBeginTime, activityEndTime, activityName, activityContent, 
			maleReward, femaleReward, money, mailTitle, mailContent, visibleMaxLevel);
	end
end
