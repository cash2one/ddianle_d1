-- 礼品内容
--send_on_login = 1系统自动发放
--send_on_login = 0玩家自己领取

local MailGiftData = 
{
	[1] = { malereward = "8020,1,604800|7003,1,604800", femalereward="8020,1,604800|7003,1,604800", moneyreward=1000, mailtitle = "禮品", mailcontent = "感謝你對我們的支持！這是給你的禮物。" },	
}

function AddMailGiftData(index, value)
	if value ~= nil then
		local malereward = value["malereward"];
		local femalereward = value["femalereward"];
		local moneyreward = value["moneyreward"];
		local mailtitle = value["mailtitle"];
		local mailcontent = value["mailcontent"];
		
		AddMailGiftInfo(index, malereward, femalereward, moneyreward, mailtitle, mailcontent);
	end
end

table.foreach(MailGiftData, AddMailGiftData);
