--场景建筑
--邀请类型 0-不能邀请 1-所有 2-同性 3-异性
local AmuseDevice_2 =
{
	[1] = { capacity = 1, invitetype = 0 },
	[2] = { capacity = 1, invitetype = 0 },
	[3] = { capacity = 1, invitetype = 0 },
	[4] = { capacity = 2, invitetype = 3 },
	[5] = { capacity = 2, invitetype = 3 },
	[6] = { capacity = 2, invitetype = 3 },
	[7] = { capacity = 2, invitetype = 3 },
	[8] = { capacity = 1, invitetype = 0 },
	[9] = { capacity = 1, invitetype = 0 },
	[10] = { capacity = 2, invitetype = 1 },
	[11] = { capacity = 2, invitetype = 1 },
	[12] = { capacity = 2, invitetype = 1 },
	[13] = { capacity = 2, invitetype = 1 },
};


function AddAmuseDevice_2(index, value)
	if value ~= nil then
		local capacity = value["capacity"];
		local invitetype = value["invitetype"];
		
		AddAmuseDeviceInfo( 2, index, capacity, invitetype );
	end
end

table.foreach( AmuseDevice_2, AddAmuseDevice_2 );

--推秋千(场景id, 秋千建筑id, 推动时间ms, 摇摆时间ms)
AddAmuseSwingEventInfo( 2, 3, 1500, 13500 );
--增加经验(场景id, 是否循环, 循环时间ms, 经验值)
AddAmuseExpEventInfo( 2, true, 60000, 5 );
--时光蛋(场景ID，设备编号)
AddAmuseEggInfo(2,10);
AddAmuseEggInfo(2,11);
AddAmuseEggInfo(2,12);
AddAmuseEggInfo(2,13);




