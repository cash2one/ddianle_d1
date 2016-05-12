--新手步骤
local MaxGuideStep = 20;
local RewardGuideStep = 0;
local MailTitle = "新手奖励";
local MailContent = "这是您完成新手引导获得的物品，祝您在接下来的游戏中玩的愉快！";

--奖励物品

local MaleReward = {
[1] = "313,1,604800|42021, 1, 604800|44107,1,604800|54102,1,604800|1011,1,604800",--正太
[2] = "40133,1,604800|1015, 1, 604800|2241,1,604800|44091,1,604800|14290,1,604800",--型男
[3] = "40130,1,604800|2304,1,604800|44135,1,604800|54163,1,604800|1013,1,604800",--美男
};

local FemaleReward = {
[1] = "40689,1,604800|57689, 1, 604800|54689,1,604800|1508,1,604800",--萝莉
[2] = "951,1,604800|1514, 1, 604800|57607,1,604800|14965,1,604800",--纯情
[3] = "952,1,604800|2951, 1, 604800|4990,1,604800|14990,1,604800|1513,1,604800",--性感
};

function AddAllGuideReward()
   for key,reward in pairs(MaleReward) do
       AddGuideReward(RewardGuideStep,1,key,reward);
   end
   
   for key,reward in pairs(FemaleReward) do
       AddGuideReward(RewardGuideStep,2,key,reward);
   end
end

AddAllGuideReward();
AddGuideContent(MailTitle,MailContent);