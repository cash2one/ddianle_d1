-- 安全保护问题

-- 安全保护问题表
local quizTable = 
{
	[1] = "你爸爸的名字是什么?";
	[2] = "你妈妈的名字是什么?";
	[3] = "你的中学是什么?";
	[4] = "你的大学是什么?";
	[5] = "你的身份证号码是多少?";
};

-- 填写安全保护问题可以获得的奖励
local quizReward = { maleItem = "165,1,-1|2165,1,-1|4165,1,-1", femaleItem = "665,1,-1|17665,1,-1", money = 300 };
local mailTitle = "奖励邮件标题";
local mailContent = "奖励邮件内容";


function AddSecurityQuizRoutine(index, value)
	if value ~= nil then
		AddSecurityQuiz(index, value);
	end
end

table.foreach(quizTable, AddSecurityQuizRoutine);
AddSecurityQuizReward(quizReward.maleItem, quizReward.femaleItem, quizReward.money, mailTitle, mailContent);
