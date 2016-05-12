-- 这个要跟平台保持一致
local fixedCharge =
{
  [1]={ money=60,gift=60,item="item1"},
  [2]={ money=1280,gift=1280,item="item2"},
  [3]={ money=2280,gift=2280,item="item3"},
  [4]={ money=10280,gift=10280,item="item4"},
};

-- 开启时间列表
local FreeVerRechargeTable = 
{
    [1] = 
    { 
        StartRechargeTime = "2015-10-17 0:00:00", 
        EndRechargeTime = "2015-11-18 23:59:59"
    },
    
    [2] = 
    { 
        StartRechargeTime = "2015-11-26 0:00:00", 
        EndRechargeTime = "2015-12-27 23:59:59"
    },
    
    [3] = 
    { 
        StartRechargeTime = "2016-1-03 0:00:00", 
        EndRechargeTime = "2016-2-04 23:59:59"
    },
};

function ReadFixedChargeInfo( index, value )
	if value ~= nil then
		AddFixedChargeInfo(value.money, value.gift, value.item);
	end
end

function ReadFixedChargeConfig( index, value )
	if value ~= nil then
		SetFixedChargeConfig(value.StartRechargeTime, value.EndRechargeTime);
	end
end


table.foreach(fixedCharge, ReadFixedChargeInfo);
table.foreach(FreeVerRechargeTable, ReadFixedChargeConfig);
