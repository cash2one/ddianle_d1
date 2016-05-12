-- live room configuration

-- 整个比赛开始结束时间，每举行一次比赛，这个开始时间都要变更一次!
local liveStartTime = "2015-04-15 10:00:00";
local liveEndTime = "2015-7-15 23:59:59";

-- 比赛标题
local title = "恋舞全民星联赛";
-- 比赛规则
local rule = "　　众人期盼的《恋舞全民星联赛》即将火爆启动，如果你想一舞成名，如果你是专业的技术手，如果你想挑战自己挑战他人，那就赶紧参加吧\n比赛日期：2015年4月15日—2015年7月15日\n比赛每日开启时间：18：00~22：00\n比赛规则：\n1、	比赛模式、比赛难度、比赛音乐、比赛场景都是系统随机配置\n2、	比赛跳舞人数为2人对战，随机配对\n3、	比赛赢方将增加5点分、比赛输方将减掉3点分\n4、	每人都将有1500基础分\n5、	比赛开始后掉线将获得惩罚，10分钟内不得再次参加\n比赛奖励：排名前200的玩家将获得星联赛专属徽章，赛事精英徽章专属于你！";

-- 默认比赛服装
local defaultMaleCloth = "91|2042|4052|14128";
local defaultFemaleCloth = "626|2542|4552|14641";

-- 海选开始结束日期, 海选日内每天的海选开始结束时间
local startDate = "2015-04-15";
local endDate = "2015-07-15";
local startTime = "18:00:00";
local endTime = "22:00:00";
-- 海选基础分数
local baseScore = 1500;
-- 海选比赛赢的和输的得分
local winScore = 5;
local lossScore = -3;
-- 海选跳舞人数
local auditionDancerCount = 2;

-- 海选比赛模式配置, 1-Taiko 2-Tradition 3-Osu
local mode = {1,2,3,4,6,7};
-- 海选比赛难度配置, 0-随机, 1-容易, 2-普通, 3-困难
local difficulty = 0;
-- 海选比赛场景配置, 0-随机, 其它值在这个列表随机。如果要固定一个场景, 就只填写这个场景ID
local scenceID = {0};
-- 海选比赛音乐配置, 0-随机, 其它值在这个列表随机。如果要固定一首歌, 就只填写这首歌曲ID
local musicID = {0};
-- 海选掉线时间惩罚(秒)
local offlineWaitTime = 600;

-- 默认关闭投票
local enableVote = false;
-- 热度投票道具, 每个道具增加的热度值
local voteItem = 32009;
local voteItemHotValue = 1;
-- 一次使用的热度道具超过这个数量，发公告
local anounceItemCount = 10;
-- 热度刷新时间间隔
local voteRefreshInterval = 5;

-- 排行榜人数
local leaderboardSize = 200;

-- 排行榜变化趋势首次刷新时间
local tideFirstRefreshTime = "18:00:00";

-- 排行榜变化趋势刷新时间间隔
local tideRefreshInterval = 86400;

if ServerGetLine ~= nil and ServerGetLine() ~= nil then

	SetLiveRoomInfo(title, rule, defaultMaleCloth, defaultFemaleCloth, liveStartTime, liveEndTime, leaderboardSize);

	SetAuditionInfo(startDate, endDate, startTime, endTime, baseScore, winScore, lossScore, auditionDancerCount, difficulty, offlineWaitTime);

	for i = 1, #mode do
		if mode[i] ~= nil then
			AddAuditionModeInfo(mode[i]);
		end
	end
	
	for i = 1, #scenceID do
		if scenceID[i] ~= nil then
			AddAuditionScenceInfo(scenceID[i]);
		end
	end

	for i = 1, #musicID do
		if musicID[i] ~= nil then
			AddAuditionMusicInfo(musicID[i]);
		end
	end

	SetLiveRoomVoteInfo(enableVote, voteItem, voteItemHotValue, voteRefreshInterval, anounceItemCount);
	
	print("game server load live room finished!");
else
	SetLiveRoomGroupLua(liveStartTime, liveEndTime, leaderboardSize, tideFirstRefreshTime, tideRefreshInterval,baseScore);
	print("group server load live room finished!");
end