-- 拼图活动

-- 活动起始时间
local activity_begin_time = "2015-06-17 00:00:00";
local activity_end_time = "2015-06-30 23:59:59";

-- 帮助内容
local activity_help_title = "粉恋运动 夏日劲酷";
local activity_help_content = " 活动期间，玩家通过参与兑换活动等方式收集粉恋运动碎片，拼图碎片可用来完成拼图并获得对应的精美光效服饰。";

local Puzzle_1 =
{
	title = "粉恋运动拼图",
	texture = "pintu_1",
	announce = true,
	normaltype = 31184,
	normalcount = 1,
	specialtype = 31033,
	specialcount = 0,
	malereward = "40245,1,-1|42245,1,-1|44245,1,-1|54245,1,-1",
	femalereward = "40745,1,-1|42745,1,-1|44745,1,-1|54745,1,-1",
	moneyreward = 0,
}





function AddPuzzleData(index, value)
	if value ~= nil then
		local title = value["title"];
		local texture = value["texture"];
		local announce = value["announce"];
		local normaltype = value["normaltype"];
		local normalcount = value["normalcount"];
		local specialtype = value["specialtype"];
		local specialcount = value["specialcount"];
		local malereward = value["malereward"];
		local femalereward = value["femalereward"];
		local moneyreward = value["moneyreward"];
		
		AddPuzzleDetail(index, title, texture, announce, normaltype, normalcount, specialtype, specialcount, malereward, femalereward, moneyreward);
	end
end


AddPuzzleBrief( activity_begin_time, activity_end_time, activity_help_title, activity_help_content );

AddPuzzleData( 1, Puzzle_1 );
AddPuzzleData( 2, Puzzle_2 );
AddPuzzleData( 3, Puzzle_3 );


