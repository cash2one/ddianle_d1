----------分割线----------以下内容为特殊商店通用配置----------

--时间区间
--Begin: 区间开始时间
--End:   区间结束时间，不可大于区间开始时间
--Title: 标题，最长16个字
--不能有重叠时间区间，会自动从小到大排序
local TimeRange = 
{
	[1] = {Begin = "2015-6-19 00:00:00", End = "2015-6-20 23:59:59", Title = "6月萌动猫咪情侣拍卖"},
	[2] = {Begin = "2015-6-26 00:00:00", End = "2015-6-27 23:59:59", Title = "爱意使者引领炽热夏日"},
	[3] = {Begin = "2015-7-3 00:00:00", End = "2015-7-4 23:59:59", Title = "7月蓝炎风采吉他竞拍"},
	[4] = {Begin = "2015-7-10 00:00:00", End = "2015-7-11 23:59:59", Title = "天团精彩紫幻竞拍季"},
}

--邮件标题
local MailTitle = "物品发送";
--邮件内容
local MailContent = "您竞拍的物品已发送，请及时查收！";

----------分割线----------以下内容为竞拍行配置----------

local Rule = "    拍卖活动期间，拍卖场会上架数个稀有珍品，每件物品都有一个起拍价。玩家可以在起拍价的基础上进行加价。加价有2种方式，自由选择金额加价或一键加价。一键加价增加的额度为商品起拍价的2%，每次加价有最小加价额度。每次加价竞拍，都会扣除竞拍所花费的M币。\n    当竞价成功后，系统会扣除您出价部分的M币。如果你竞拍成功并获得竞拍物品，那竞拍所花费的M币将会作为您竞拍物品的费用；如果您的竞价被超过，系统会立即返还该部分M币。竞拍结束后，竞拍价最高的玩家将获得竞拍物品。参与竞拍不可提前放弃。";

----------分割线----------以下内容不可修改----------

function AddAuctionTimeRangeLoop(index, value)
	if value ~= nil then
		local Begin = value["Begin"];
		local End = value["End"];
		local Title = value["Title"];

		AddAuctionTimeRangeInfo(Begin, End, Title);
	end
end

table.foreach(TimeRange, AddAuctionTimeRangeLoop);
AddAuctionInfo(MailTitle, MailContent, Rule);
