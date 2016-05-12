--舞团活动数据

--活动规则描述
local ruleContent = "　　界面上有5颗不同颜色的星星，每获得一定的燃烧值，引线就会燃烧一点。当引线燃烧完毕时，星星将被炸落下来！\n　　玩家可以使用右侧不同的按钮，获得不同的燃烧值：\n　　点火按钮：每天可以使用1次，使用后获得1点燃烧值和5点舞团贡献度。\n　　燃烧按钮：每天可以使用3次，每次使用消耗50金券，使用后获得5点燃烧值和10点舞团贡献度。\n　　爆燃按钮：每天可使用200次，每次使用消耗100M币，使用后得到10点燃烧值和15点舞团贡献度。\n　　当全部星星被炸落后，所有按钮都无法使用，当日的舞团活动结束。";

--点一下操作
--次数
local normalCount = 1;
--动力值
local normalPower = 1;
--贡献值
local normalContribution = 5;


--花费金券操作
--次数
local moneyCount = 3;
--每次操作使用的金券
local moneyNum = 50;
--动力值
local moneyPower = 5;
--贡献值
local moneyContribution = 10;


--用力推操作(使用M币)
--次数
local mCoinCount = 200;
--每次操作使用的MCoin
local mCoinPerNum = 100;
--动力值
local mCoinPower = 10;
--贡献值
local mCointContribution = 15;

--星星属性值
--id 进度完成值 提供的舞团资源（初，中，高） 名称
local starValuesList = 
{
	[1] = { id=1, completeValue=35, conValueLow=0,conValueMid=0,conValueHigh=0, name="绿星资源"},
	[2] = { id=2, completeValue=125, conValueLow=0,conValueMid=0,conValueHigh=0, name="蓝星资源"},
	[3] = { id=3, completeValue=350, conValueLow=0,conValueMid=0,conValueHigh=0, name="紫星资源"},
	[4] = { id=4, completeValue=500, conValueLow=0,conValueMid=0,conValueHigh=0, name="橙星资源"},
	[5] = { id=5, completeValue=750, conValueLow=0,conValueMid=0,conValueHigh=0, name="红星资源"}
};

--首次点击“点一下”操作获得的奖励
local perDayRseward = { malePid="32001,1,-1", femalePid="32001,1,-1"};

--星星礼包奖励(暂时配置7天奖励，7天一个轮回)
local rewardItemList = 
{
	[1] = {day=1,itemList={
		[1] = { id=1, malePid="33003,1,-1",femalePid="33003,1,-1"},
		[2] = { id=2, malePid="40203,1,259200",femalePid="40641,1,259200"},
		[3] = { id=3, malePid="22006,1,86400",femalePid="22006,1,86400"},
		[4] = { id=4, malePid="6038,1,259200|31226,1,-1",femalePid="6038,1,259200|31226,1,-1"},
		[5] = { id=5, malePid="40141,1,259200|31226,1,-1",femalePid="40708,1,259200|31226,1,-1"}
	}},
	[2] = {day=2,itemList={
		[1] = { id=1, malePid="32001,1,-1",femalePid="32001,1,-1"},
		[2] = { id=2, malePid="5028,1,259200",femalePid="5028,1,259200"},
		[3] = { id=3, malePid="22007,1,86400",femalePid="22007,1,86400"},
		[4] = { id=4, malePid="40059,1,259200|31226,1,-1",femalePid="40521,1,259200|31226,1,-1"},
		[5] = { id=5, malePid="31026,1,-1|31226,1,-1",femalePid="31026,1,-1|31226,1,-1"}
	}},
	[3] = {day=3,itemList={
		[1] = { id=1, malePid="33003,1,-1",femalePid="33003,1,-1"},
		[2] = { id=2, malePid="54203,1,259200",femalePid="54641,1,259200"},
		[3] = { id=3, malePid="22008,1,86400",femalePid="22008,1,86400"},
		[4] = { id=4, malePid="6038,1,259200|31226,1,-1",femalePid="6038,1,259200|31226,1,-1"},
		[5] = { id=5, malePid="54141,1,259200|31226,1,-1",femalePid="54708,1,259200|31226,1,-1"}
	}},
	[4] = {day=4,itemList={
		[1] = { id=1, malePid="32001,1,-1",femalePid="32001,1,-1"},
		[2] = { id=2, malePid="5028,1,259200",femalePid="5028,1,259200"},
		[3] = { id=3, malePid="22018,1,86400",femalePid="22018,1,86400"},
		[4] = { id=4, malePid="54059,1,259200|31226,1,-1",femalePid="54521,1,259200|31226,1,-1"},
		[5] = { id=5, malePid="31026,1,-1|31226,1,-1",femalePid="31026,1,-1|31226,1,-1"}
	}},
	[5] = {day=5,itemList={
		[1] = { id=1, malePid="33003,1,-1",femalePid="33003,1,-1"},
		[2] = { id=2, malePid="44203,1,259200",femalePid="44641,1,259200"},
		[3] = { id=3, malePid="22006,1,86400",femalePid="22006,1,86400"},
		[4] = { id=4, malePid="42059,1,259200|31226,1,-1",femalePid="57521,1,259200|31226,1,-1"},
		[5] = { id=5, malePid="44141,1,259200|31226,1,-1",femalePid="44708,1,259200|31226,1,-1"}
	}},
	[6] = {day=6,itemList={
		[1] = { id=1, malePid="32001,1,-1",femalePid="32001,1,-1"},
		[2] = { id=2, malePid="5028,1,259200",femalePid="5028,1,259200"},
		[3] = { id=3, malePid="22013,1,86400",femalePid="22013,1,86400"},
		[4] = { id=4, malePid="6038,1,259200|31226,1,-1",femalePid="6038,1,259200|31226,1,-1"},
		[5] = { id=5, malePid="31026,1,-1|31226,1,-1",femalePid="31026,1,-1|31226,1,-1"}
	}},
	[0] = {day=0,itemList={
		[1] = { id=1, malePid="33001,1,-1",femalePid="33001,1,-1"},
		[2] = { id=2, malePid="42203,1,259200",femalePid="42641,1,259200"},
		[3] = { id=3, malePid="22007,1,86400",femalePid="22007,1,86400"},
		[4] = { id=4, malePid="44059,1,259200|31226,1,-1",femalePid="33003,1,-1|31226,1,-1"},
		[5] = { id=5, malePid="42141,1,259200|31226,1,-1",femalePid="42708,1,259200|31226,1,-1"}
	}}
};

function AddPacketRoutineGame( index, value )
	local day = value.day;
	for dayIndex, dayValue in pairs(value.itemList) do
		local id = dayValue[ "id" ];
		local malePid = dayValue[ "malePid" ];
		local femalePid = dayValue[ "femalePid" ];
		
		AddStarPacketItem( day, id, malePid, femalePid );
	end
end

function AddStarProgressRoutineGame( index, value )
	AddStarProgress( value.id, value.completeValue, value.conValueLow, value.conValueMid, value.conValueHigh, value.name );
end

function AddStarProgressRoutineGroup( index, value )
	AddStarProgressLua( value.id, value.completeValue, value.conValueLow, value.conValueMid, value.conValueHigh, value.name );
end


if ServerGetLine ~= nil and ServerGetLine() ~= nil then
	--添加礼包信息
	table.foreach(rewardItemList, AddPacketRoutineGame);
	--添加星星进度信息
	table.foreach(starValuesList, AddStarProgressRoutineGame);
	--添加基本信息
	AddStarBaseInfo( perDayRseward.malePid, perDayRseward.femalePid,normalCount, normalPower, normalContribution, moneyCount, moneyNum, moneyPower,moneyContribution,mCoinCount,mCoinPerNum,mCoinPower,mCointContribution, ruleContent );

	print(os.date("[SYSTEM][%m-%d %H:%M:%S]game server load live room finished!",os.time()));
else
	--添加礼包信息
	table.foreach(rewardItemList, AddPacketRoutineGame);
	--添加星星进度信息
	table.foreach(starValuesList, AddStarProgressRoutineGroup);
	--添加基本信息
	AddStarBaseInfo( perDayRseward.malePid, perDayRseward.femalePid,normalCount, normalPower, normalContribution, moneyCount, moneyNum, moneyPower,moneyContribution,mCoinCount,mCoinPerNum,mCoinPower,mCointContribution, ruleContent );

	print(os.date("[SYSTEM][%m-%d %H:%M:%S]group server load live room finished!",os.time()));
end