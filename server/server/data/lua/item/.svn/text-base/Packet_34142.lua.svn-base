
-------------------以下是一个礼包的定义
--礼品包的详细信息
function GetPacketItemsInfo_34142(RoleIndex, PacketID)
	local Sex = GetRoleSex(RoleIndex)
	if Sex == SexType_Male then
		return "83    ,1   ,604800, 14082      ,1   ,604800 ,22003      ,1   ,2592000"		--物品1ID 数量 有效时间1, 物品2ID 数量2 有效时间2, 物品ID3 数量3 有效时间3
	else
		return "524   ,1   ,604800, 14803      ,1   ,604800 ,22003      ,1   ,2592000"		--物品1ID 数量 有效时间1, 物品2ID 数量2 有效时间2, 物品ID3 数量3 有效时间3
	end
end

-- 礼包定义,需要填物品ID号,数量
function PacketGetReward_34142 (RoleIndex)
	local nPacketID = 34142
	local Sex = GetRoleSex(RoleIndex)
	--礼包的道具数量,必填
	local Count = 3
	local ItemTable = {}
	--------------------------------ItemID    数量  有效期         
	
	if Sex == SexType_Male then    
		AddItemToTable(ItemTable,     83    ,1   ,604800)
		AddItemToTable(ItemTable,     14082      ,1   ,604800)
		AddItemToTable(ItemTable,     22003      ,1   ,2592000)


	else
		AddItemToTable(ItemTable,     524    ,1   ,604800)
		AddItemToTable(ItemTable,     14803      ,1   ,604800)
		AddItemToTable(ItemTable,     22003      ,1   ,2592000)


	end		
	
	if CanAddPacketItem(RoleIndex,nPacketID) == 1 then
		for	i = 1, Count do
			AddItemToRole(RoleIndex,ItemTable[i][1],ItemTable[i][2], ItemTable[i][3],nPacketID);		
		end
			ModifyRoleMoney(RoleIndex, 0);	------第二个参数表示增加的金券数,如果需要添加金券,则需要修改该数字；没有该函数或该数字为0（这条语句）表示金券为0,
		return 1
	else
		return 0
	end	
end


--注册获取礼包信息函数
-- 礼包ID 礼包描述函数
RegisterGetPacketInfo(34142, GetPacketItemsInfo_34142)
--注册礼包使用函数
--                    礼包ID 礼包获取函数
RegisterGetPacketItem(34142, PacketGetReward_34142)




