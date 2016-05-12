--场景建筑
--邀请类型 0-不能邀请 1-所有 2-同性 3-异性
local AmuseDevice_1 =
{
	[1] = { capacity = 2, invitetype = 3 },
	[2] = { capacity = 2, invitetype = 3 },
	[3] = { capacity = 2, invitetype = 3 },
	[4] = { capacity = 2, invitetype = 3 },
	[5] = { capacity = 2, invitetype = 3 },
	[6] = { capacity = 2, invitetype = 3 },
	[7] = { capacity = 2, invitetype = 3 },
	[8] = { capacity = 2, invitetype = 3 },
	[9] = { capacity = 2, invitetype = 3 },
	[10] = { capacity = 2, invitetype = 3 },
	[11] = { capacity = 1, invitetype = 0 },
	[12] = { capacity = 1, invitetype = 0 },
	[13] = { capacity = 1, invitetype = 0 },
	[14] = { capacity = 1, invitetype = 0 },
	[15] = { capacity = 1, invitetype = 0 },
	[16] = { capacity = 1, invitetype = 0 },
	[17] = { capacity = 1, invitetype = 0 },
	[18] = { capacity = 1, invitetype = 0 },
	[19] = { capacity = 1, invitetype = 0 },
	[20] = { capacity = 1, invitetype = 0 },
	[21] = { capacity = 1, invitetype = 0 },
	[22] = { capacity = 1, invitetype = 0 },
	[23] = { capacity = 1, invitetype = 0 },
	[24] = { capacity = 1, invitetype = 0 },
	[25] = { capacity = 1, invitetype = 0 },
	[26] = { capacity = 1, invitetype = 0 },
	[27] = { capacity = 1, invitetype = 0 },
	[28] = { capacity = 2, invitetype = 1 },
	[29] = { capacity = 2, invitetype = 1 },
	[30] = { capacity = 2, invitetype = 1 },
	[31] = { capacity = 2, invitetype = 1 },
};


function AddAmuseDevice(index, value)
	if value ~= nil then
		local capacity = value["capacity"];
		local invitetype = value["invitetype"];
		
		AddAmuseDeviceInfo( 1, index, capacity, invitetype );
	end
end

table.foreach( AmuseDevice_1, AddAmuseDevice );

--旋转木马(场景id, 是否循环, 循环时间ms)
AddAmuseCarrouselEventInfo( 1, true, 60000, "goround01", "goround02", "goround03" );
--增加经验(场景id, 是否循环, 循环时间ms, 经验值)
AddAmuseExpEventInfo( 1, true, 60000, 5 );
--时光蛋(场景ID，设备编号)
AddAmuseEggInfo(1,28);
AddAmuseEggInfo(1,29);
AddAmuseEggInfo(1,30);
AddAmuseEggInfo(1,31);





