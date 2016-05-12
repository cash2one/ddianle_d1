----------分割线----------以下内容为分享活动通用配置----------

--是否显示大图标，0表示不显示，大于0表示显示(保留字段，填0)
local Exhibit = 0;

--活动标题
local Title = "灰姑娘之夜，分享得服饰";
--活动内容
local Content = "　　每周星期六20点至24点，只要在商城试穿服饰，并分享试穿的服饰截图，即可获得试穿服饰中发型、上衣(套装)、下装和鞋子1天的使用权。每次活动开启期间只能获得1次奖励。备注：非卖品不参与此次活动。";

--活动图片名称（大图片保留字段，填空）
local BigImageName = "";
local SmallImageName = "huodongzhongxin_16";

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
	[1] = {BeginTime = "2014-11-24 00:00:00", EndTime = "2016-11-30 23:59:59", Type = 2, Date = "6", Hours = {
		[1] = {BeginHour = "20:00:00", EndHour = "23:59:59"},

	}},
}

--分享文字信息
local MaleShareText = "　　灰姑娘之夜，试穿海量服饰，{0}好不好看？";
local FemaleShareText = "　　灰姑娘之夜，试穿海量服饰，{0}好不好看？";

--邮件标题
local MailTitle = "灰姑娘之夜，分享得服饰";
--邮件内容
local MailContent = "您完成了灰姑娘之夜活动，请接收奖励。";

----------分割线----------以下内容为通用配置中不同的数据----------

--奖励物品信息（暂时只填一组数据）
--Title:      奖励标题
--MaleText:   男性奖励描述文字
--FemaleText: 女性奖励描述文字
local RewardInfo = 
{
	[1] = {Title = "", MaleText = "您将会获得试穿服饰中发型、上衣(套装)、下装和鞋子1天的使用权(非卖品除外)", FemaleText = "您将会获得试穿服饰中发型、上衣(套装)、下装和鞋子1天的使用权(非卖品除外)"},
}

----------分割线----------以下内容为商城社交分享特有配置----------

--男性动作名
local MaleActionName = "CINDERELLA_BOY";
--女性动作名
local FemaleActionName = "CINDERELLA_GIRL";

--奖励试穿服饰的期限，单位: 天
local RewardItemTime = 1;

--奖励试穿服饰的部位
-- 0: 发型		 1: 表情	 2: 上衣	 3: 手套	 4: 下装
-- 5: 头饰		 6: 脸饰	 7: 肩膀	 8: 翅膀	 9: 左手持
--10: 右手持	11: 手腕	12: 臀部	13: 袜子	14: 鞋子
--15: 肤色		16: 全身装	17: 套装
local ClothPos = "0|2|4|14|17";

----------分割线----------以下内容不可修改----------

function AddMallSocialShareActivity(weight)
	AddMallSocialShareActivityInfo(Exhibit, weight, Title, Content, BigImageName, SmallImageName, BeginTime, EndTime, BeginShowTime, EndShowTime, MaleShareText, FemaleShareText, MailTitle, MailContent, MaleActionName, FemaleActionName, RewardItemTime, ClothPos);

	table.foreach(RewardInfo, AddMallRewardInfoLoop);
	table.foreach(TimeRange, AddMallTimeRangeLoop);
end

function AddMallRewardInfoLoop(index, value)
	if value ~= nil then
		local Title = value["Title"];
		local MaleText = value["MaleText"];
		local FemaleText = value["FemaleText"];

		AddMallShareRewardInfo(index, Title, MaleText, FemaleText);
	end
end

function AddMallTimeRange(value, beginTime, endTime, type, date)
	if value ~= nil then
		local BeginHour = value["BeginHour"];
		local EndHour = value["EndHour"];

		AddMallShareTimeRangeInfo(beginTime, endTime, type, date, BeginHour, EndHour);
	end
end

function AddMallTimeRangeLoop(index, value)
	if value ~= nil then
		local BeginTime = value["BeginTime"];
		local EndTime = value["EndTime"];
		local Type = value["Type"];
		local Date = value["Date"];
		local Hours = value["Hours"];

		for i, v in ipairs(Hours) do
			AddMallTimeRange(v, BeginTime, EndTime, Type, Date);
		end
	end
end
