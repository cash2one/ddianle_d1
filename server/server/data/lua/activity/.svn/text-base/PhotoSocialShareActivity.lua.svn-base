----------分割线----------以下内容为分享活动通用配置----------

--是否显示大图标，0表示不显示，大于0表示显示(保留字段，填0)
local Exhibit = 0;

--活动标题
local Title = "合影挑战";
--活动内容
local Content = "　　每周星期日6点至22点，至少2名玩家在摄影棚拍照并分享，即可获得绑定M币奖励！";

--活动图片名称（大图片保留字段，填空）
local BigImageName = "";
local SmallImageName = "huodongzhongxin_03";

--活动起始时间
local BeginTime = "2014-11-24 00:00:00";
local EndTime = "2016-12-30 23:59:59";

--活动显示起始时间
--开始显示时间不填，则为当前系统时间
--结束显示时间不填，则为活动结束时间
local BeginShowTime = "";
local EndShowTime = "";

--具体时间区间
--BeginTime: 时间区间开始时间，不可小于活动开始时间
--EndTime: 时间区间结束时间，不可大于活动结束时间
--Type: 1表示以月为周期，2表示以周为周期，3表示以日为周期
--Date: 多个值以|分割
--      当Type为1时，填1~32，32表示月末，每个月的最后一天都符合
--      当Type为2时，填1~7
--      当Type为3时，忽略
--BeginHour: 1天内开始时间
--EndHour: 1天内结束时间，例：8:00:00~11:59:59、14:00:00~20:59:59，跨天的时间分成两段填写，不可有重叠时间段
local TimeRange = 
{
	[1] = {BeginTime = "2014-11-24 00:00:00", EndTime = "2016-12-30 23:59:59", Type = 2, Date = "7", Hours = {
		[1] = {BeginHour = "6:00:00", EndHour = "21:59:59"},

	}},
}

--分享文字信息
local MaleShareText = "　　据说晒合影的人会一直在一起，你要不要试试";
local FemaleShareText = "　　据说晒合影的人会一直在一起，你要不要试试";

--邮件标题
local MailTitle = "合影挑战";
--邮件内容
local MailContent = "您参加了——合影挑战活动，获得奖励请查看邮件附件！";



----------分割线----------以下内容为通用配置中不同的数据----------

--奖励物品信息（暂时只填一组数据）
--Title:        奖励标题(已废弃)
--MaleReward:   男性奖励物品信息，多个以|分割
--FemaleReward: 女性奖励物品信息
--Money:        奖励金券
--BMoney:       奖励绑定M币
local RewardInfo = 
{
	[1] = {Title = "", MaleReward = "", FemaleReward = "", Money = 0, BMoney = 500},
}

local VIPRewardInfo = 
{
    [1] = {Title = "", MaleReward = "", FemaleReward = "", Money = 0, BMoney = 300},
}

----------分割线----------以下内容为照相馆社交分享特有配置----------

--拍照人数要求下限
local PeopleNumber = 2;

----------分割线----------以下内容不可修改----------

function AddPhotoSocialShareActivity(weight)
    AddPhotoSocialShareActivityInfo(Exhibit, weight, Title, Content, BigImageName, SmallImageName, BeginTime, EndTime, BeginShowTime, EndShowTime, MaleShareText, FemaleShareText, MailTitle, MailContent, PeopleNumber);

    for index, value in ipairs(RewardInfo) do
        AddPhotoRewardInfoLoop(index, value, 0)
    end

    for index, value in ipairs(VIPRewardInfo) do
        AddPhotoRewardInfoLoop(index, value, 1)
    end

    table.foreach(TimeRange, AddPhotoTimeRangeLoop);
end

-- rewardtype: 0-普通玩家、1-VIP玩家
function AddPhotoRewardInfoLoop(index, value, rewardtype)
    if value ~= nil then
        local Title = value["Title"];
        local MaleReward = value["MaleReward"];
        local FemaleReward = value["FemaleReward"];
        local Money = value["Money"];
        local BMoney = value["BMoney"];

        AddPhotoShareRewardInfo(index, Title, MaleReward, FemaleReward, Money, BMoney, rewardtype);
    end
end

function AddPhotoTimeRange(value, beginTime, endTime, type, date)
	if value ~= nil then
		local BeginHour = value["BeginHour"];
		local EndHour = value["EndHour"];

		AddPhotoShareTimeRangeInfo(beginTime, endTime, type, date, BeginHour, EndHour);
	end
end

function AddPhotoTimeRangeLoop(index, value)
	if value ~= nil then
		local BeginTime = value["BeginTime"];
		local EndTime = value["EndTime"];
		local Type = value["Type"];
		local Date = value["Date"];
		local Hours = value["Hours"];

		for i, v in ipairs(Hours) do
			AddPhotoTimeRange(v, BeginTime, EndTime, Type, Date);
		end
	end
end
