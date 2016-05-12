--0 一般玩家
--3
--2
--1 GM
--0xffffffff Admin,管理员，最高权限


local commands = 
{
--添加GM权限
	GrantGMCmd			= {0xffffffff},						-- 权限授权	
--删除GM权限
	RemoveGMCmd			= {0xffffffff},						-- 权限删除	
--公告发送
	Announce			= {1,0xffffffff},					-- 发送公告
--邮件发送
	SendSysMail			= {0xffffffff},						-- 发送系统邮件
--兑换码
	GenGiftCode			= {1,0xffffffff},					-- 产生兑换码
	QueryGiftCode		= {1,0xffffffff},					-- 查询激活码
--发送系统公告
   AdminChat			= {1, 0xffffffff},
}

function GetPrivsInfo(commands, value)
	if commands ~= nil then	
		local count = table.getn(value);
		for i = 1, count do
			AddPriv(commands, value[i]);
		end
	end
end

table.foreach(commands, GetPrivsInfo);


