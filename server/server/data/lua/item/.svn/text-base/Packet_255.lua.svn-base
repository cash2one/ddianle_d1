
-------------------以下是一个礼包的定义
--礼品包的详细信息
function GetPacketItemsInfo_255(RoleIndex, PacketID)
	local Sex = GetRoleSex(RoleIndex)
	if Sex == SexType_Male then
		return "1,1,1，2,1,1, 3,1,1"			--物品1ID 数量 有效时间1， 物品2ID 数量2 有效时间2， 物品ID3 数量3 有效时间3
	else
		return "4,1,1, 5,1,1, 6,1,1"			--物品1ID 数量 有效时间1， 物品2ID 数量2 有效时间2， 物品ID3 数量3 有效时间3
	end
end

function GetPacketCurrencyInfo_255(RoleIndex, PacketID)
	return "3:1|2:999"
end

-- 礼包定义，需要填物品ID号，数量
function PacketGetReward_255 (RoleIndex)
	local nPacketID = 255
	local Sex = GetRoleSex(RoleIndex)
	--礼包的道具数量，必填
	local Count = 3
	local ItemTable = {}
	--------------------------------ItemID    数量  有效期         
	
	if Sex == SexType_Male then    
		AddItemToTable(ItemTable,     1        ,1    ,0)      
		AddItemToTable(ItemTable,     2        ,1    ,0)
		AddItemToTable(ItemTable,     3        ,1    ,0)
	else
		AddItemToTable(ItemTable,     4        ,1    ,0)    
		AddItemToTable(ItemTable,     5        ,1    ,0)   
		AddItemToTable(ItemTable,     6        ,1    ,0)          
	end		
	
	if CanAddPacketItem(RoleIndex,nPacketID) == 1 then
		for	i = 1, Count do
			AddItemToRole(RoleIndex,ItemTable[i][1],ItemTable[i][2], ItemTable[i][3],nPacketID);		
		end
		
        ModifyRoleMoney(RoleIndex, 999);	------第二个参数表示增加的金券数，如果需要添加金券，则需要修改该数字；没有该函数或该数字为0（这条语句）表示金券为0，
        ModifyRoleBindMCoin(RoleIndex, 1);
        ------第二个参数表示增加的绑定M币，如果需要添加绑定M币，则需要修改该数字；没有这行代码或该数字为0表示不增加绑定M币
        
		return 1
	else
		return 0
	end	
end


--注册获取礼包信息函数
-- 礼包ID 礼包描述函数
RegisterGetPacketInfo(255, GetPacketItemsInfo_255)
RegisterGetPacketCurrencyInfo(255, GetPacketCurrencyInfo_255)
--注册礼包使用函数
--                    礼包ID 礼包获取函数
RegisterGetPacketItem(255, PacketGetReward_255)




