--这是恋舞广场中活动的脚本
--版本号 每次修改该文件需要更改改版本号
local version = 36;

-- 0	公告
-- 1	活动
-- 2	新闻

--linkTo 
--0无跳转
--1商城
--2充值

--pid
--0表示全部平台
--大于1表示相应平台
local announcementInfo = 
{




	[1]	= {
			type = 1, 
			title = "[FFEC8B]恋舞6月单笔充值返利[-]", 
			content = '[000000]活动时间：6月17日维护后至6月18日23点59分\n    活动说明：在活动期间内符合以下条件的玩家将获得返利绑定M币的奖励，活动最高返利到达30%\n    1、单笔充值1元~10元：则返利10%的绑定M币；\n    2、单笔充值11元~50元：则返利15%的绑定M币；\n    3、单笔充值51元~100元：则返利20%的绑定M币；\n    4、单笔充值101元~300元：则返利25%的绑定M币；\n    5、大于等于301元：则返利30%的绑定M币；[-]', 
			date = "2015-6-17", 
			hot = 1, 
			icon = 
			{	
			
			},
			linkTo = 2,
			pid = 0;
		  },

	

	[2]	= {
			type = 1, 
			title = "[FFEC8B]恋舞礼包热力来袭[-]", 
			content = '[000000]活动时间：6月17日维护后~7月14日23点59分\n    活动说明：活动期间“时空之恋礼包“、”引领潮流礼包““恋舞英雄礼包”商城热力出售，精美服饰和萌动英雄券等你开启哦！\n    时空之恋礼包：开启礼包必定可以获得“时空之恋男套/时空之恋女套”*永久+萌动英雄券*10个\n    引领潮流礼包：开启礼包必定可以获得“引领潮流男套/引领潮流女套”*永久+萌动英雄券*8个\n    恋舞英雄礼包：开启必定可得大量“萌动英雄券”，并随机获得各种精美服饰。[-]', 
			date = "2015-6-17", 
			hot = 1, 
			icon = 
			{	
			
			},
			linkTo = 1,
			pid = 0;
		  },

	[3]	= {
			type = 1, 
			title = "[FFEC8B]英雄大秀场之呆绿酷雷[-]", 
			content = '[000000]活动时间：6月17日维护后~6月30日23点59分\n    活动说明：\n    1、活动期间使用“恋舞英雄礼包”+“萌动英雄券A”*3个，兑换可获得“酷雷魔盒”1个，开启酷雷魔盒将有机会获得永久的“雷神之锤”和大量精美光效服饰！\n    2、活动期间使用“恋舞英雄礼包”+“萌动英雄券A”*5个，兑换可获得“呆绿魔盒”1个，开启呆绿盒将有机会获得永久的“绿色拳头手杖”和大量精美光效服饰！\n    备注：本任务在活动期间可重复完成[-]', 
			date = "2015-6-17", 
			hot = 1, 
			icon = 
			{	
			
			},
			linkTo = 1,
			pid = 0;
		  },

	[4]	= {
			type = 1, 
			title = "[FFEC8B]二次元盛典 热力四射high翻天[-]", 
			content = '[000000]活动时间：6月17日维护后~6月30日23点59分\n    活动说明：活动期间开启50次“呆绿魔盒”，可获得“绿色拳头手杖”*永久；开启50次“酷雷魔盒”，可获得“雷神之锤”*永久。\n    备注：奖励将在活动期间每周五发放1次，最终发放时间是7月2日。[-]', 
			date = "2015-6-17", 
			hot = 0, 
			icon = 
			{	
			
			},
			linkTo = 1,
			pid = 0;
		  },

	

	[5]	= {
			type = 1, 
			title = "[FFEC8B]小熊爱照相[-]", 
			content = '[000000]活动时间：6月17日维护后~6月30日23点59分\n    活动说明：活动期间参与累计消费活动，轻松拥有“小熊布偶”，带着小熊布偶去光影魔术馆照相并分享，即可获得1个“萌动英雄券A”。\n    备注：\n    1、只需累计消费30元，即可轻松获得可爱的小熊布偶*永久。拥有“小熊布偶”后，即可触发本任务。\n    2、该任务在活动期间每天可完成1次。[-]',
			date = "2015-6-17", 
			hot = 0,
			icon = 
			{	
			},
			linkTo = 0,
			pid = 0;
		  },


	[6]	= {
			type = 1, 
			title = "[FFEC8B]周末来恋舞，双倍经验爽不停[-]", 
			content = '[000000]活动时间：6月26日20点~22点；6月27日20点~22点\n    活动说明：活动期间多人一起（2人以上）在游戏大厅中跳舞，可获得双倍经验。[-]',
			date = "2015-6-17", 
			hot = 0,
			icon = 
			{	
			},
			linkTo = 0,
			pid = 0;
		  },

	[7]	= {
			type = 1, 
			title = "[FFEC8B]累计在线 天降好礼[-]", 
			content = '[000000]活动时间：6月20日；6月27日\n    活动说明：活动期间累计在线可得多个“萌动英雄券”和"欢乐卡"。\n    活动内容：\n    6月20日\n    累计在线15分钟可获得：288金券+欢乐卡*1个\n    累计在线30分钟可获得：388金券+萌动英雄券A*1个\n    累计在线45分钟可获得：688金券+萌动英雄券A*1个\n    6月27日\n    累计在线15分钟可获得：288金券+欢乐卡*1个\n    累计在线30分钟可获得：388金券+萌动英雄券A*1个\n    累计在线60分钟可获得：688金券+萌动英雄券A*1个[-]', 
			date = "2015-6-17", 
			hot = 0, 
			icon = 
			{	
			
			},
			linkTo = 0,
			pid = 0;
		  },

	[8]	= {
			type = 1, 
			title = "[FFEC8B]准点在线礼品多[-]", 
			content = '[000000]活动时间：6月21日;6月28日\n    活动说明：活动期间，都会在当日20点、22点开启准点在线活动，大量萌动英雄券等你来取。\n    活动内容：\n    20点：288金券+萌动英雄券A*1个\n    22点：688金券+萌动英雄券A*1个[-]', 
			date = "2015-6-17", 
			hot = 0, 
			icon =    
			{	
			
			},
			linkTo = 0,
			pid = 0;
		  },

	[9]	= {
			type = 1, 
			title = "[FFEC8B]时光蛋大提速[-]", 
			content = '[000000]时光蛋大改版啦！自6月17日维护后，时光蛋开蛋时间从原来的3天，永久的缩短为2天。\n    6月24日维护后~6月30日23点59分，时光蛋开蛋时间再次提速为1天。机会难得不可错过哦！[-]', 
			date = "2015-6-17", 
			hot = 0, 
			icon = 
			{	
			
			},
			linkTo = 0,
			pid = 0;
		  },

	[10]	= {
			type = 1, 
			title = "[FFEC8B]激情不灭，全民争做活跃达人[-]", 
			content = '[000000]恋舞天天乐，活跃达人在哪里？恋舞OL现推出了全新的“活跃度奖励”，恋舞友友们每日只需完成不同的“活跃达人”任务条件，就可获得活跃度奖励，每日获得的活跃度越多，可领取的奖励就越丰富哦。还等什么，快来争做活跃达人吧！[-]', 
			date = "2015-6-17", 
			hot = 0, 
			icon = 
			{	
			
			},
			linkTo = 0,
			pid = 0;
		  },
  
	[11]	= {
			type = 1, 
			title = "[FFEC8B]爱之告白，恋爱初体验[-]", 
			content = '[000000]爱之告白，恋爱初体验。单身恋舞友友们的福利来了，恋舞OL现在推出全新恋爱任务，只要你是单身玩家，即可享受到恋舞为你安排的一站式恋爱体验。摆脱单身，勇敢的去体验吧！[-]', 
			date = "2015-6-17", 
			hot = 0, 
			icon = 
			{	
			
			},
			linkTo = 0,
			pid = 0;
		  }, 

}

function GetAnnounceIcon(index, value)
	if value ~= nil then
		AddAnnounceIcons(value);
	end
end

function GetAnnouncementInfo( index, value )
	if value ~= nil then
		local type = value["type"];
		local title = value["title"];
		local content = value["content"];
		local date = value["date"];
		local hot = value["hot"];
		local icon = value["icon"];
		local linkTo = value["linkTo"];
		local pid = value["pid"];
		
		AddAnnouncement();
		AddAnnounceBriefInfo( type, title, content, date, hot, linkTo, pid );
		table.foreach(icon, GetAnnounceIcon);
		
	end
end

function GetAnnouncementVersion()
	AddAnnounceVersion(version);
end

table.foreach( announcementInfo, GetAnnouncementInfo );

GetAnnouncementVersion();