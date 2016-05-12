--心动模式配置

--经典模式心动值配置表
local TaikoBeckoning = 
{
    [1] = 0,
    [2] = 0,
    [3] = 5,
    [4] = 10,
    [5] = 20,
}
--经典模式心动值配置表
local TraditionBeckoning = 
{
    [1] = 0,
    [2] = 0,
    [3] = 20,
    [4] = 40,
    [5] = 80,
}
--心动特效配置表，特效id对应心动值
local BeckoningEffect = 
{
    [1] = 0,
    [2] = 200,
    [3] = 520,
    [4] = 1314,
}

function AddTaikoBeckoning(index, value)
    AddTaikoBeckoningInfo(index, value);
end

function AddTraditionBeckoning(index, value)
    AddTraditionBeckoningInfo(index, value);
end

function AddBeckoningEffect(index, value)
    AddBeckoningEffectInfo(index, value);
end

table.foreach(TaikoBeckoning, AddTaikoBeckoning);
table.foreach(TraditionBeckoning, AddTraditionBeckoning);
table.foreach(BeckoningEffect, AddBeckoningEffect);
