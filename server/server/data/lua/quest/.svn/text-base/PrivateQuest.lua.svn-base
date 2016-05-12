--收费版本的特殊任务

local private_quest = 
{
	[1] = {questid = 214};

};

function AddPrivateQuestLua(index, value)
	if value ~= nil then
		local questid = value["questid"];	
		AddPrivateQuest(questid);
	end
end

table.foreach(private_quest, AddPrivateQuestLua);
