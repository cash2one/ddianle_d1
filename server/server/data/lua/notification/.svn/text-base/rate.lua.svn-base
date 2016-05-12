-- 评价消息推送

-- 是否启用推送
enablePushRatingSystem=false;

-- 推送的最小时间间隔，单位为秒
pushRatingInvterval=3600;

-- 推送消息内容
pushRatingMessage="亲！来为您最爱的恋舞打个五星好评吧！";

-- 每周推送次数
pushRatingMaxCount=3;
-- 周几重置：0-周日，1-周一，2-周二 ……
pushResetWeekday=4;

-- 推送策略1：角色升级
pushRatingLevel = {5, 13, 22, 40};

-- 推送策略2：每日角色在线时长，单位为秒
enablePushRatingOnlineTime=true;
pushRatingOnlineTime = 900;

-- 推送策略3：每日完成第一个任务
enablePushRatingFinishFirstDailyQuest=true;

-- 推送策略4：每日完成第一次商城消费
enablePushRatingFinishFirstDailyShoping=true;


function AddPushRatingLevel(index, value)
	if value ~= nil then
		SetPushRatingLevel(value);
	end
end


SetPushRatingSystem(pushRatingInvterval, pushRatingMessage, enablePushRatingSystem, pushRatingMaxCount, pushResetWeekday,
    pushRatingOnlineTime, enablePushRatingOnlineTime, enablePushRatingFinishFirstDailyQuest, enablePushRatingFinishFirstDailyShoping);
table.foreach(pushRatingLevel, AddPushRatingLevel);

