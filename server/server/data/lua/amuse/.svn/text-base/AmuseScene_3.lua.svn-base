--场景建筑
--邀请类型 0-不能邀请 1-所有 2-同性 3-异性
local AmuseDevice_3 =
{
	[1] = { capacity = 8, invitetype = 0 },
};


function AddAmuseDevice_3(index, value)
	if value ~= nil then
		local capacity = value["capacity"];
		local invitetype = value["invitetype"];
		
		AddAmuseDeviceInfo( 3, index, capacity, invitetype );
	end
end

table.foreach( AmuseDevice_3, AddAmuseDevice_3 );

--潜水艇(场景id, 是否循环, 循环时间ms, 初始点)
AddAmuseBoatInfo( 3, true, 60000, 0 );
--增加经验(场景id, 是否循环, 循环时间ms, 经验值)
AddAmuseExpEventInfo( 3, true, 60000, 5 );





