--舞团入团仪式lua数据

--每日免费的申请次数
local freeApplyTimes = 1;
--仪式申请消耗的金钱类型
local applyCostType = 3;
--仪式申请消耗的金钱数量
local applyCostNum = 1000;

--每次仪式免费的点赞次数
local freePraiseTimes = 1;
--仪式点赞消耗的金钱类型
local praiseCostType = 3;
--仪式点赞消耗的金钱数量
local praiseCostNum = 50;
--仪式点赞冷却时间(秒)
local praiseCD = 10;

--仪式申请到开启的最大时长(秒)
local applyDuration = 60;
--仪式开启到结束的最大计时长度(秒)
local startDuration = 300;

--玩家退团再入团的冷却时间：
--玩家加入老团的冷却时间小于两个加入新团的冷却时间
local enterNewGroupCD = 86400;
local enterOldGroupCD = 172800;


--新人奖励：
local prizeOfNewman = 
{
	-- 邮件内容,奖励，可以填写多个
	-- 第一名到第八名的奖励，(邮件内容,奖励ID)
	{sender="舞团专员", title="舞团入团仪式获赞奖励", ctx="恭喜你获得了舞团入团仪式最佳人气新人奖！", item="35356,3,-1"},
	{sender="舞团专员", title="舞团入团仪式获赞奖励", ctx="恭喜你参与舞团入团仪式！", item="35356,1,-1"},
	{sender="舞团专员", title="舞团入团仪式获赞奖励", ctx="恭喜你参与舞团入团仪式！", item="35356,1,-1"},
	{sender="舞团专员", title="舞团入团仪式获赞奖励", ctx="恭喜你参与舞团入团仪式！", item="35356,1,-1"},
	{sender="舞团专员", title="舞团入团仪式获赞奖励", ctx="恭喜你参与舞团入团仪式！", item="35356,1,-1"},
	{sender="舞团专员", title="舞团入团仪式获赞奖励", ctx="恭喜你参与舞团入团仪式！", item="35356,1,-1"},
	{sender="舞团专员", title="舞团入团仪式获赞奖励", ctx="恭喜你参与舞团入团仪式！", item="35356,1,-1"},
	{sender="舞团专员", title="舞团入团仪式获赞奖励", ctx="恭喜你参与舞团入团仪式！", item="35356,1,-1"},
}

--参与者奖励
local prizeOfNormal = 
{
	-- 奖励邮件，参与点赞的人数, 奖励物品
	{sender="舞团专员", title="舞团入团仪式点赞奖励", ctx="参与仪式的人数达到了1~10人的档次,感谢您的努力！", praiser=10, item="35354,1,-1"},
	{sender="舞团专员", title="舞团入团仪式点赞奖励", ctx="参与仪式的人数达到了10~30人的档次,感谢您的努力！", praiser=30, item="35354,3,-1"},
	{sender="舞团专员", title="舞团入团仪式点赞奖励", ctx="参与仪式的人数达到了30~50人的档次,感谢您的努力！", praiser=50, item="35355,1,-1"},
	{sender="舞团专员", title="舞团入团仪式点赞奖励", ctx="参与仪式的人数达到了50~100人的档次,感谢您的努力！", praiser=100, item="35355,3,-1"},
	{sender="舞团专员", title="舞团入团仪式点赞奖励", ctx="参与仪式的人数达到了100~150人的档次,感谢您的努力！", praiser=150, item="35356,1,-1"},
	{sender="舞团专员", title="舞团入团仪式点赞奖励", ctx="参与仪式的人数达超过了150人,感谢您的努力！", praiser=210, item="35356,3,-1"},
}

function AddNewmanPrize_lua(index, value)
	AddCeremonyNewManPrize(value["sender"], value["title"], value["ctx"], value["item"]);
end

function AddNormalPrize_lua(index, value)
	AddCeremonyNormalPrize(value["sender"], value["title"], value["ctx"], value["praiser"],value["item"]);
end

SetCeremonyApplyInfo(freeApplyTimes, applyCostType, applyCostNum);
SetCeremonyPraiseInfo(freePraiseTimes, praiseCostType, praiseCostNum, praiseCD);
SetCeremonyBaseInfo(applyDuration, startDuration, enterNewGroupCD, enterOldGroupCD);
table.foreach(prizeOfNewman, AddNewmanPrize_lua);	
table.foreach(prizeOfNormal, AddNormalPrize_lua);