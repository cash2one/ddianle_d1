----------分割线----------以下内容为特殊商店通用配置----------

--时间区间
--Begin: 区间开始时间
--End:   区间结束时间，不可大于区间开始时间
--Title: 标题，最长16个字
--不能有重叠时间区间，会自动从小到大排序
local TimeRange = 
{
	[1] = {Begin = "2015-6-17 09:35:00", End = "2015-6-23 23:59:59", Title = "六月精彩团购第三弹"},
	[2] = {Begin = "2015-6-24 09:35:00", End = "2015-6-30 23:59:59", Title = "六月精彩团购第四弹"},
	[3] = {Begin = "2015-7-1 09:35:00", End = "2015-7-7 23:59:59", Title = "七月精彩团购第一弹"},
	[4] = {Begin = "2015-7-8 09:35:00", End = "2015-7-14 23:59:59", Title = "七月精彩团购第二弹"},
}

--邮件标题
local MailTitle = "物品发送";
--邮件内容
local MailContent = "您在团购中购买的物品已到货，请查收！";

----------分割线----------以下内容为限量销售通用配置----------

--下限
local AddLower = 0;
--上限
local AddUpper = 0;

----------分割线----------以下内容不可修改----------

function AddGroupBuyingTimeRangeLoop(index, value)
	if value ~= nil then
		local Begin = value["Begin"];
		local End = value["End"];
		local Title = value["Title"];

		AddGroupBuyingTimeRangeInfo(Begin, End, Title);
	end
end

table.foreach(TimeRange, AddGroupBuyingTimeRangeLoop);
AddGroupBuyingInfo(MailTitle, MailContent, AddLower, AddUpper);
