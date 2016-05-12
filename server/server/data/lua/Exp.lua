-- 玩家角色跳舞经验配置表

local GradePercentInfo =
{
perfect = 100, cool = 80, good = 50, bad = 20, miss = 0
}

local GradeExpInfo = 
{
-- index is difficulty, easy: 1, normal: 2, hard: 3
[1]	    = { SS = 30, S = 20, A = 12, B = 8, C = 4, D = 0 },
[2]		= { SS = 45, S = 30, A = 18, B = 12, C = 6, D = 0 },
[3]		= { SS = 60, S = 40, A = 24, B = 16, C = 8, D = 0 }
}

local PlaceExpInfo = 
{
-- index is place
[1]     = { count4 = 100, count3 = 80, count2 = 50, count1 = 0 },
[2]     = { count4 = 70, count3 = 50, count2 = 20 },
[3]     = { count4 = 60, count3 = 30 },
[4]     = { count4 = 40 }
}

function AnalyseGradePercent()
	local perfect = ( GradePercentInfo["perfect"] or 0 );
	local cool = ( GradePercentInfo["cool"] or 0 );
	local good = ( GradePercentInfo["good"] or 0 );
	local bad = ( GradePercentInfo["bad"] or 0 );
	local miss = ( GradePercentInfo["miss"] or 0 );
		
	AddGradePercent( perfect, cool, good, bad, miss );
end

function AnalyseGradeExp( index, value )
	if value ~= nil then
		local ss = ( value["SS"] or 0 );
		local s = ( value["S"] or 0 );
		local a = ( value["A"] or 0 );
		local b = ( value["B"] or 0 );
		local c = ( value["C"] or 0 );
		local d = ( value["D"] or 0 );
		
		AddGradeExp( index, ss, s, a, b, c, d );
	end
end

function AnalysePlaceExp( index, value )
	if value ~= nil then
		local count4 = ( value["count4"] or 0 );
		local count3 = ( value["count3"] or 0 );
		local count2 = ( value["count2"] or 0 );
		local count1 = ( value["count1"] or 0 );
		
		AddPlaceExp( index, count4, count3, count2, count1 );
	end
end

AnalyseGradePercent();
table.foreach( GradeExpInfo, AnalyseGradeExp );
table.foreach( PlaceExpInfo, AnalysePlaceExp );