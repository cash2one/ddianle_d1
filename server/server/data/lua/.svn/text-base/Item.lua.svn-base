local ItemAddFAT = {}
local ItemRemoveFAT = {}
local PacketInfoFAT = {}
local PacketCurrencyFAT = {}
local PacketGetItemFAT = {}
local VIPPacketFAT = {}

function ExcludeItemToTable(ItemTable,ItemIds,ItemCount,IsBind,prob,denominator)
    local nCount = #ItemIds
    local    ResultID =0
    if nCount > 0 then
        ResultID = ItemIds[math.random(1,nCount)]
        table.insert(ItemTable,{ResultID,ItemCount,IsBind,prob,denominator})
    else
        return;
    end
end

function AddItemToTable( ItemTabls,ItemID,ItemCount,ValidTime)
    table.insert(ItemTabls,{ItemID,ItemCount,ValidTime})
end

function RegisterItemAddRemove( ItemID,ItemAddFunction,ItemRemovefunction )
    if ItemAddFunction == nil and ItemRemovefunction == nil then
        print("注册了空物品脚本函数:");
    end
    if ItemAddFAT[ItemID] == nil then
        ItemAddFAT[ItemID]= ItemAddFunction
    else
        print("重复注册物品ItemAddRemove脚本ID:"..ItemID)
    end
    
    if ItemRemoveFAT[ItemID] == nil then
        ItemRemoveFAT[ItemID]= ItemRemovefunction
    else
        print("重复注册物品ItemRemovefunction脚本ID:"..ItemID)
    end
end

function RegisterGetPacketInfo(PacketID, func)
	if func == nil then
		print("注册了空签到礼包描述脚本函数")
	end
	
	if PacketInfoFAT[PacketID] == nil then
		PacketInfoFAT[PacketID] = func
	else
		print("重复注册礼品描述脚本ID:"..PacketID)
	end
end

function RegisterGetPacketCurrencyInfo(PacketID, func)
	if func == nil then
		print(os.date("[SYSTEM][%m-%d %H:%M:%S]注册了空签到礼包描述脚本函数",os.time()));
	end
	
	if PacketCurrencyFAT[PacketID] == nil then
		PacketCurrencyFAT[PacketID] = func
	else
		print(os.date("[SYSTEM][%m-%d %H:%M:%S]重复注册礼品描述脚本ID",os.time()));
	end
end

function RegisterGetPacketItem(PacketID, func)
	if func == nil then
		print("注册了空礼包获取函数:"..PacketID );
	end
	
	if PacketGetItemFAT[PacketID] == nil then
		PacketGetItemFAT[PacketID] = func
	else
		print("重复注册礼品包获取脚本ID:"..PacketID)
	end
end

function RegisterActivatedVIPRewardID(Type,func)
    if func == nil then
        print("注册了购买VIP赠礼包脚本函数");
    end
    if VIPPacketFAT[Type] == nil then
        VIPPacketFAT[Type]= func
    else
		print("重复注册购买VIP赠礼包脚本ID:"..Type)
    end
end

function ItemAdd( ItemID,RoleIndex )
    if ItemAddFAT[ ItemID ] ~= nil then
        ItemAddFAT[ ItemID ]( RoleIndex );
    end
end

function ItemRemove( ItemID,RoleIndex )
    if ItemRemoveFAT[ ItemID ] ~= nil then
        ItemRemoveFAT[ ItemID ]( RoleIndex );
    end
end

function GetPacketItemInfo( PacketID,RoleIndex )
    if PacketInfoFAT[ PacketID ] ~= nil then
        return PacketInfoFAT[ PacketID ]( RoleIndex );
    else
        print( "没有找到相应的获取礼包详细信息的脚本,ID:"..PacketID )
        return "0"
    end
end

function GetPacketCurrencyInfo( PacketID,RoleIndex )
    if PacketCurrencyFAT[ PacketID ] ~= nil then
        return PacketCurrencyFAT[ PacketID ]( RoleIndex );
    else
		print(os.date("[SYSTEM][%m-%d %H:%M:%S]没有找到相应的获取礼包详细信息的脚本",os.time()));
        return "0"
    end
end

function GetPacketItems(PacketID, RoleIndex)
    if PacketGetItemFAT[ PacketID ] ~= nil then
        return PacketGetItemFAT[ PacketID ]( RoleIndex )
    else
        print( "没有找到相应的礼包获取脚本,ID:"..PacketID )
        return 0,"获取礼包失败"
    end
end

function GetActivatedVIPRewardID(RoleIndex, Type)
	if VIPPacketFAT[Type] ~= nil then
		VIPPacketFAT[Type](RoleIndex)	
	else
		print("没有找到开通VIP获得礼包的脚本函数,模式:"..Type)
	end
end


