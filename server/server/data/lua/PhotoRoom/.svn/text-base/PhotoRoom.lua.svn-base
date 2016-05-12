--摄影棚相关数据

--最多一次邀请的玩家数
local maxInvitePlayerCount = 3;

--邀请玩家的时间间隔:秒
local invitePlayerInterval = 3;

--被邀请者时间相应时间:秒
local inviteeResponseExpiredTime = 60;


--倒计时提示时间，务必别超过房间才存活时间，否则无效
--notifycationType: 0表示红色系统提示 1弹框提示 2 红色系统提示+弹框提示
local notificationTimer = 
{
	[1] = {time = 600, notificationType = 0},
	[2] = {time = 300, notificationType = 0},
	[3] = {time = 120, notificationType = 0},
	[4] = {time = 60, notificationType = 1},
};

--主题对应的NPC信息，4表示明星主题
local themeNPCInfo = 
{
    [4] = {{name="徐良", sex=1, skin=1, headwear="", hair="hair_55.clh", face="face_1024.clh", 
            shoulders="", gloves="gloves_3000.clh", righthand="", lefthand="", 
            cloth="coat_2225.clh", shoes="shoes_14225.clh", suit="", leg="pant_4225.clh"}},
}

function AddPhotoThemeNPCInfo_lua(index, value)
    if	value ~= nil then
        table.foreach(value, function(i, v) 
        local name = v["name"];
        local sex = v["sex"];
        local skin = v["skin"];
        local headwear = v["headwear"];
        local hair = v["hair"];
        local face = v["face"];
        local shoulders = v["shoulders"];
        local gloves = v["gloves"];
        local righthand = v["righthand"];
        local lefthand = v["lefthand"];
        local cloth = v["cloth"];
        local shoes = v["shoes"];
        local suit = v["suit"];
        local leg = v["leg"];
        
        AddPhotoThemeNPCInfoLua(index, name, sex, skin, headwear, hair, face, shoulders, gloves, righthand, lefthand, cloth, shoes, suit, leg);
        end);
    end
end

function addPhotoStudio_lua()
	AddPhotoStudioDataLua(maxInvitePlayerCount, invitePlayerInterval, inviteeResponseExpiredTime);
end


function AddNotificationTimer_lua(index, value)
	if	value ~= nil then
		local time = value["time"];	
		local notificationType = value["notificationType"];
		AddPhotoStudioNotificationDataLua(index, time, notificationType);
	end 
end

addPhotoStudio_lua()
table.foreach(notificationTimer, AddNotificationTimer_lua);
table.foreach(themeNPCInfo, AddPhotoThemeNPCInfo_lua);




