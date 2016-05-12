-- 礼品内容
--send_on_login = 1系统自动发放
--send_on_login = 0玩家自己领取

local GiftData = 
{
   	--内测补偿第一档（2-10级）
	[1] = { send_on_login = 1, malereward = "8020,1,604800|7003,1,604800", femalereward="8020,1,604800|7003,1,604800", moneyreward=1000,message = "感谢您内测期间对我们游戏的支持和热爱，根据您的内测等级，您获得了相应的内测补偿。", mailtitle = "恋舞内测补偿", mailcontent = "感谢您内测期间对我们游戏的支持和热爱，根据您的内测等级，您获得了相应的内测补偿。" },	
	--内测补偿第二档（11-20级）
	[2] = { send_on_login = 1, malereward = "8020,1,604800|7003,1,604800", femalereward="8020,1,604800|7003,1,604800", moneyreward=3000,message = "感谢您内测期间对我们游戏的支持和热爱，根据您的内测等级，您获得了相应的内测补偿。", mailtitle = "恋舞内测补偿", mailcontent = "感谢您内测期间对我们游戏的支持和热爱，根据您的内测等级，您获得了相应的内测补偿。" },	
	--内测补偿第三档（21-30级）
	[3] = { send_on_login = 1, malereward = "8020,1,604800|7003,1,604800", femalereward="8020,1,604800|7003,1,604800", moneyreward=10000,message = "感谢您内测期间对我们游戏的支持和热爱，根据您的内测等级，您获得了相应的内测补偿。", mailtitle = "恋舞内测补偿", mailcontent = "感谢您内测期间对我们游戏的支持和热爱，根据您的内测等级，您获得了相应的内测补偿。" },	
	--内测补偿第四档（31-50级）
	[4] = { send_on_login = 1, malereward = "8020,1,604800|7003,1,604800", femalereward="8020,1,604800|7003,1,604800", moneyreward=20000,message = "感谢您内测期间对我们游戏的支持和热爱，根据您的内测等级，您获得了相应的内测补偿。", mailtitle = "恋舞内测补偿", mailcontent = "感谢您内测期间对我们游戏的支持和热爱，根据您的内测等级，您获得了相应的内测补偿。" },	
	--内测补偿第五档（51-70级）
	[5] = { send_on_login = 1, malereward = "8020,1,604800|7003,1,604800", femalereward="8020,1,604800|7003,1,604800", moneyreward=30000,message = "感谢您内测期间对我们游戏的支持和热爱，根据您的内测等级，您获得了相应的内测补偿。", mailtitle = "恋舞内测补偿", mailcontent = "感谢您内测期间对我们游戏的支持和热爱，根据您的内测等级，您获得了相应的内测补偿。" },	
	--内测补偿第六档（71级及以上）
	[6] = { send_on_login = 1, malereward = "8020,1,604800|7003,1,604800", femalereward="8020,1,604800|7003,1,604800", moneyreward=50000,message = "感谢您内测期间对我们游戏的支持和热爱，根据您的内测等级，您获得了相应的内测补偿。", mailtitle = "恋舞内测补偿", mailcontent = "感谢您内测期间对我们游戏的支持和热爱，根据您的内测等级，您获得了相应的内测补偿。" },	
	--好友归来 小伙伴们都想你了！
	[7] = { send_on_login = 1, malereward = "8046,1,604800|172,1,604800|17172,1,604800|14172,1,604800|32002,5,-1|31005,1,-1", femalereward="8046,1,604800|672,1,604800|17672,1,604800|14672,1,604800|32002,5,-1|31005,1,-1", moneyreward=5205,message = "特定玩家登陆游戏赠送奖励活动", mailtitle = "好友归来 小伙伴们都想你了！", mailcontent = "活动期间，注册时间>=2013年8月13日、最后登陆时间<=2014年2月2日,符合条件的流失老用户，只要重新登陆游戏，即可获得回归奖励！" },

}

function AddGiftData(index, value)
	if value ~= nil then
		local send_on_login = value["send_on_login"];
		local malereward = value["malereward"];
		local femalereward = value["femalereward"];
		local moneyreward = value["moneyreward"];
		local message = value["message"];
		local mailtitle = value["mailtitle"];
		local mailcontent = value["mailcontent"];
		
		AddGiftInfo(index, send_on_login, malereward, femalereward, moneyreward, message, mailtitle, mailcontent);
	end
end

table.foreach(GiftData, AddGiftData);
