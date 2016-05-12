--------------------下面内容为舞团秘境配置--------------------

-- 邮件
local mailSender = "舞团秘境专员"
local mailTitle = "舞团秘境奖励"
local mailcontext = "舞团秘境context"
local storage_box_mail_context = "rinige"
--------------------下面内容不建议修改--------------------

-- 储物箱发放周期[0,6]，0表示周日
local storage_box_reward = 6;
-- 储物箱发放时间
local storage_box_reward_send_time = "00:10:00";


-- 公告活动关闭时间
local end_announcement_time = "23:55:00";
-- 公告内容
local end_announcement_content = "舞团秘境将于00：00关闭";

--开始时间和结束时间配置在一天以内
-- 跳舞时间段必须在活动时间段中，并且跳舞结束时间要早于活动结束时间
--活动开始时间
local begin_time = "9:00:00";
--活动结束时间
local end_time = "23:59:59";

--房间允许开始时间
local allow_dance_begin_time = "9:00:00";
--房间允许结束时间
local allow_dance_end_time = "23:55:00";

--奖励发放时间(这个时间点 发放关卡参与奖励)
local reward_send_time = "00:10:00";

--祝福增益(百分比,Eg. 20就是20%)
local bless_buff = 20;

--排行榜规则
local rank_rule = "偏爱的减肥安居房卡卡里的风景拉飞机快拉倒  就拉到附近卡拉丁方科技安静的法律框架的房间啊了看法拉克安静的开发拉家带口立法法大法老地方科技即可领奖爱看的偏爱的减肥安居房卡卡里的风景拉飞机快拉倒  就拉到附近卡拉丁方科技安静的法律框架的房间啊了看法拉克安静的开发拉家带口立法法大法老地方科技即可领奖2342311爱看的偏爱的减肥安居房卡卡里的风景拉飞机快拉倒  就拉到附近卡拉丁方科技安静的法律框架的房间啊了看法拉克安静的开发拉家带口立法法大法老地方科技即可领奖爱看的";

--储物箱规则
local storagebox_rule = "偏爱的减肥安居房卡卡里的风景拉飞机快拉倒  就拉到附近卡拉丁方科技安静的法律框架的房间啊了看法拉克安静的开发拉家带口立法法大法老地方科技即可领奖爱看的偏爱的减肥安居房卡卡里的风景拉飞机快拉倒  就拉到附近卡拉丁方科技安静的法律框架的房间啊了看法拉克安静的开发拉家带口立法法大法老地打发打发23112方科技即可领奖爱看的";

local bless_rule = "偏爱的减肥安居房卡卡里的风景拉飞机快拉倒  就拉到附近卡拉丁方科技安静的法律框架的房间啊了看法拉克安静的开发拉家带口立法法大法老地方科技即可领奖爱看的偏爱的减肥安居房卡卡里的风景拉飞机快拉倒  就拉到附近卡拉丁方科技安静的法律框架的房间啊了看法拉克安静的开发拉家带口立法法大法老地方科技即可领奖爱看的1111";

--单次祝福增加时间(单位秒)
local bless_time = 10000;

--单次祝福消耗mb
local bless_price = 100;

--------------------下面内容不可修改--------------------

LoadFairlyLandConfig(mailSender, mailTitle, mailcontext, storage_box_mail_context, storage_box_reward, storage_box_reward_send_time,
end_announcement_time,end_announcement_content,begin_time,end_time,allow_dance_begin_time,allow_dance_end_time,reward_send_time, bless_buff,
rank_rule, storagebox_rule , bless_rule,bless_time ,bless_price)
