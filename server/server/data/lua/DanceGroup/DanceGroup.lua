--舞团lua数据

--舞团最高等级
local maxGroupDanceLevel = 30;


--舞团成员每天登陆获得舞团荣誉奖励的等级
local memberLoginGroupHonorBonusLevelNeed = 10;

--舞团成员每天登陆获得舞团荣誉值
local memberLoginGropHonorBonus = 1;

--舞团成员房间跳舞获得舞团荣誉奖励的跳舞人数
local minRoomDancersGroupHonorBonusNeed = 3;

--舞团成员房间跳舞奖励档次1
local memberMatchBonusLeve1 = 1;

--每捐赠一单位资源A舞团得到的荣誉
local incDGHonorNumPerDonatedResA = 10;
--每捐赠一单位资源B舞团得到的荣誉
local incDGHonorNumPerDonatedResB = 100;
--每捐赠一单位资源C舞团得到的荣誉
local incDGHonorNumPerDonatedResC = 1000;

--每捐赠一单位资源A得到舞团成员得到的贡献值
local incMemberContributionNumPerDonatedResA = 10;
--每捐赠一单位资源B得到舞团成员得到的贡献值
local incMemberContributionNumPerDonatedResB = 100;
--每捐赠一单位资源C得到舞团成员得到的贡献值
local incMemberContributionNumPerDonatedResC = 1000;

--舞团升级经验
--level表示当前等级，res_a、res_b、res_c表示当前等级升级所需资源(PS:当前等级)
local group_level_up_res = 
{
	[1] = {level = 1, res_a = 180, res_b = 0, res_c = 0};
	[2] = {level = 2, res_a = 420, res_b = 0, res_c = 0};
	[3] = {level = 3, res_a = 720, res_b = 1, res_c = 0};
	[4] = {level = 4, res_a = 1080, res_b = 0, res_c = 0};
	[5] = {level = 5, res_a = 1500, res_b = 3, res_c = 0};
	[6] = {level = 6, res_a = 2300, res_b = 0, res_c = 0};
	[7] = {level = 7, res_a = 3200, res_b = 7, res_c = 0};
	[8] = {level = 8, res_a = 4300, res_b = 0, res_c = 0};
	[9] = {level = 9, res_a = 5400, res_b = 11, res_c = 1};
	[10] = {level = 10, res_a = 6700, res_b = 7, res_c = 0};
	[11] = {level = 11, res_a = 8600, res_b = 9, res_c = 2};
	[12] = {level = 12, res_a = 10500, res_b = 10, res_c = 0};
	[13] = {level = 13, res_a = 13000, res_b = 12, res_c = 2};
	[14] = {level = 14, res_a = 15400, res_b = 15, res_c = 0};
	[15] = {level = 15, res_a = 18000, res_b = 18, res_c = 2};
	[16] = {level = 16, res_a = 21400, res_b = 21, res_c = 0};
	[17] = {level = 17, res_a = 25000, res_b = 25, res_c = 3};
	[18] = {level = 18, res_a = 29000, res_b = 28, res_c = 0};
	[19] = {level = 19, res_a = 33000, res_b = 33, res_c = 3};
	[20] = {level = 20, res_a = 37400, res_b = 38, res_c = 4};
	[21] = {level = 21, res_a = 43000, res_b = 42, res_c = 5};
	[22] = {level = 22, res_a = 48500, res_b = 48, res_c = 6};
	[23] = {level = 23, res_a = 54500, res_b = 55, res_c = 7};
	[24] = {level = 24, res_a = 60800, res_b = 60, res_c = 8};
	[25] = {level = 25, res_a = 67400, res_b = 68, res_c = 9};
	[26] = {level = 26, res_a = 75000, res_b = 75, res_c = 10};
	[27] = {level = 27, res_a = 83400, res_b = 85, res_c = 11};
	[28] = {level = 28, res_a = 92000, res_b = 90, res_c = 12};
	[29] = {level = 29, res_a = 100800, res_b =100, res_c = 15};
}

function AddDanceGroupInfo()
	AddDanceGrupInfoLua(maxGroupDanceLevel, memberLoginGroupHonorBonusLevelNeed, memberLoginGropHonorBonus, minRoomDancersGroupHonorBonusNeed, memberMatchBonusLeve1,
						incDGHonorNumPerDonatedResA, incDGHonorNumPerDonatedResB, incDGHonorNumPerDonatedResC,
						incMemberContributionNumPerDonatedResA, incMemberContributionNumPerDonatedResB, incMemberContributionNumPerDonatedResC);
end

function  AddGroupLevelUpRes_Lua(index, value)
    if value ~= nil then 
		local level = value["level"];
		local res_a = value["res_a"];
		local res_b = value["res_b"];
		local res_c = value["res_c"];
		AddGroupDanceLeveUpRequireResLua(level, res_a, res_b, res_c);
    end
end

AddDanceGroupInfo();
table.foreach(group_level_up_res, AddGroupLevelUpRes_Lua);
