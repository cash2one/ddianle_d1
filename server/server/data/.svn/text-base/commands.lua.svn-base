--0 一般玩家
--1 GM
--0xffffffff Admin,管理员，最高权限


local commands = 
{
--任务属性
	accountid			= {1,0xffffffff},					-- 角色accountID
	roleid				= {1,0xffffffff},					-- 角色roleID
	addexp				= {1,0xffffffff},					-- 设置角色经验
	addpt				= {1,0xffffffff},					-- 设置pt
	sethot				= {1,0xffffffff},					-- 设置人气
	setpre				= {1,0xffffffff},					-- 设置声望
	setmoney			= {1,0xffffffff},					-- 设置金钱
	levelup				= {1,0xffffffff},					-- 升级
	setlevel			= {1,0xffffffff},					-- 设置等级
	setonlinetime		= {1,0xffffffff},					        -- 设置在线时长
	setofftime			= {1,0xffffffff},					-- 设置离线时间
--服务器
	setsrvtime			= {0xffffffff},						-- 设置服务器时间
	time				= {1,0xffffffff},					-- 查看服务器时间
--物品
	additem				= {1,0xffffffff},					-- 添加某个物品
	itemcool			= {1,0xffffffff},					-- 所有物品道具冷却归零
	senditem			= {1,0xffffffff},					-- 发送物品给玩家
--任务
	addquest			= {1,0xffffffff},					-- 添加某个任务
	finquest			= {1,0xffffffff},					-- 完成某个任务
--成就
--充值
	addmcoin			= {1,0xffffffff},					-- 命令行充值
	addbindmcoin        = {1,0xffffffff},					-- 命令行加绑定M币
--debug
    debug				= {1,0xffffffff},					--debug命令    
--addintimacy
	addintimacy			= {1,0xffffffff},					--修改亲密度
--openbox
	openbox				= {1,0xffffffff},					--模拟开宝箱
--dropdanceitem
	dropdanceitem		= {1,0xffffffff},					--跳舞物品掉落
--addvipexp
    addvipexp           = {1,0xffffffff},					--修改vip经验值
--setviplevel           
    setviplevel         = {1,0xffffffff},					--设置vip等级
--luckbet           
    luckbet				= {1,0xffffffff},					--模拟欢乐转 //luckbet bNotFree nCount
--liveroom           
    liveroom			= {1,0xffffffff},					--直播房间参数调整 2-观众人数上限 3-直播房间关闭时间 4-开始比赛倒计时时间
--room
    room				= {1,0xffffffff},					-- 用法 //room num 创建num个跳舞房间
--amuse
    amuse				= {1,0xffffffff},					-- 用法 //amuse amuseid num 创建场景ID为amuseid，数量为num个的开放性场景房间
--任务
    acceptquest		= {1,0xffffffff},					    --添加新任务
--关卡
    activedParclose = {1,0xffffffff},					    --激活关卡
	addDebris ={1,0xffffffff},
	activedTheme = {1,0xffffffff},
--服务器在线玩家人数
    getserverplayer = {1,0xffffffff},
    ceremonyroomnum = {1,0xffffffff},
	
--增加舞团荣誉值
    AddDanceGroupHonor = {1,0xffffffff},                    -- 用法 //AddDanceGroupHonor DanceGroupID ChangeValue
--根据舞团名申请舞团
	EnterDanceGroupByName = {1,0xffffffff},                    -- 用法 //EnterDanceGroupByName groupName
--可以指定任意人为新人
	RookieAnyBody = {1,0xffffffff},                    -- 用法 //RookieAnyBody roleID, seq
--增加
    addAuditionScore = {1,0xffffffff},                    -- 用法 //addAuditionScore 50
--签到
	clearcheckindata = {1,0xffffffff},                     -- 用法 //clearcheckindata
--活跃度通知
	activeness = {1,0xffffffff},						-- 用法 // 1-7分别为：checkin, quest_daily, dungeon, dance_mode, match_room, vip_daily_reward, social_share
-- 给穿在身上的衣服增加特效
	addClothEffect = {1,0xffffffff},					-- 用法 //addClothEffect 1 给衣服增加1的特效，这个特效对应的身体部位必须穿有衣服。
-- 幸运魔法阵概率测试命令
	magicarray = {1,0xffffffff},						-- 用法 // magicarray 1 1 10 (分别代表：魔法阵ID、概率类型（1：单抽、2：十年抽、3：首次）、次数)
	addmedal = {1,0xffffffff},							-- 用法 // addmedal 勋章ID
}

function GetCommandsInfo(command, value)
	if command ~= nil then	
		local count = table.getn(value);
		for i = 1, count do
			AddCmd(command, value[i]);
		end
	end
end

table.foreach(commands, GetCommandsInfo);


