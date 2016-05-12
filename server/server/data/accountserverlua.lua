local g_path = GetServerPath()
-- firstpayment
dofile(g_path.."data/lua/payment/FirstPaymentReward.lua")
dofile(g_path.."data/lua/payment/RechargeReward.lua")
dofile(g_path.."data/lua/payment/fixedCharge.lua");
dofile(g_path.."data/lua/accouonConfigs/configs.lua")
dofile(g_path.."data/lua/PlayerCache/PlayerCache.lua")
print("Load Lua finish！");
