--									舞团红包配置
--是否显示在主页, 0表示不显示，大于0表示显示
local exhibit = 1;

--活动图片名称
local regularImageName = "activity-ad_11";
local thumbnailName = "huodongzhongxin_11";

-- 红包份额配置
local RedEnvelope_Num = 20;

-- 红包领取时效(秒)
local RedEnvelope_ContinueTime = 8*60*60;

-- 红包全部领取后多长时间消失(秒)
local RedEnvelope_GetAll_DisappearTime = 2*60*60;

-- 红包活动开始、结束时间
local RedEnvelope_ActivityBeginTime = "2015-10-01 10:00:00";
local RedEnvelope_ActivityEndTime = "2015-12-01 10:00:00";

-- 红包活动显示起始时间
local showtime_begin = "2015-10-01 10:00:00";
local showtime_end = "2015-12-01 10:00:00";

--活动名称
local activity_title = "准点在线礼品多";
--活动内容
local activity_content = "只要在2015年6月21日~2015年7月12日活动期间，红包活动开始，可以强了哈";

-- 匿名用户名称
local anonymity_name = "不愿透露姓名的土豪";

-- 红包充值金额信息 
-- rechargemcoin 充值金额(货币)
-- currencytype 红包金钱类型
-- currencyvalue  红包金钱金额
-- minrate 最低红包百分比
-- maxrate 浮动最大倍数
local RedEnvelopeRecharge = 
{
	[1] = {rechargemcoin = 100, currencytype = 3, currencyvalue = 50, minrate = 2, maxrate = 50};
	[2] = {rechargemcoin = 1000, currencytype = 3, currencyvalue = 200, minrate = 2, maxrate = 50};
	[3] = {rechargemcoin = 3000, currencytype = 3, currencyvalue = 1000, minrate = 2, maxrate = 50};
}

function  AddRedEnvelopeRecharge_Lua(index, value)
    if value ~= nil then 
		local rechargemcoin = value["rechargemcoin"];
		local currencytype = value["currencytype"];
		local currencyvalue = value["currencyvalue"];
		local minrate = value["minrate"];
		local maxrate = value["maxrate"];
		AddRedEnvelopeRechargeConfig(rechargemcoin, currencytype, currencyvalue, minrate, maxrate);
    end
end

function AddRedEnvelopeActivity(weight)
	if weight ~= nil then 
		AddEnvelopeConfig(exhibit,weight,regularImageName, thumbnailName, RedEnvelope_Num, RedEnvelope_ContinueTime, RedEnvelope_GetAll_DisappearTime, RedEnvelope_ActivityBeginTime,
					RedEnvelope_ActivityEndTime,showtime_begin, showtime_end, activity_title, activity_content, anonymity_name);
    end
end


table.foreach(RedEnvelopeRecharge, AddRedEnvelopeRecharge_Lua);
