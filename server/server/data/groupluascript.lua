print("Load groupluascript.lua start")
local g_path = GetServerPath()
--系统设置
dofile(g_path.."data/lua/SystemSetting/SystemSetting.lua")
-- mail gift
dofile(g_path.."data/lua/external/mailgift.lua")
-- dance group
dofile(g_path.."data/lua/DanceGroup/DanceGroup_Activity.lua")
dofile(g_path.."data/lua/DanceGroup/DanceGroup.lua")
dofile(g_path.."data/lua/DanceGroup/DanceGroupCeremonyRoom.lua")
-- 舞团试炼
dofile(g_path.."data/lua/DanceGroup/DanceGroupChallenge.lua")

-- 舞团秘境
dofile(g_path.."data/lua/DanceGroup/DanceGroupFairlyLand.lua")

-- 红包
dofile(g_path.."data/lua/activity/RedEnvelopeActivity.lua")
dofile(g_path.."data/lua/activity/Activitiesgroup.lua")

-- live room
dofile(g_path.."data/lua/room/liveroom.lua")
--好友查询
dofile(g_path.."data/lua/friend/FindFriend.lua")
print("Load live room lua data Complete")

--特殊商店
dofile(g_path.."data/lua/SpecialMall/GroupBuying.lua")
dofile(g_path.."data/lua/SpecialMall/LimitedEdition.lua")
dofile(g_path.."data/lua/SpecialMall/Auction.lua")
print("Load Special Mall lua data Complete")

--排行榜
dofile(g_path.."data/lua/Rank/Rank.lua")
dofile(g_path.."data/lua/Rank/RankReward.lua")

--聊天
dofile(g_path.."data/lua/Chat/Chat.lua")

--舞团大妈
dofile(g_path.."data/lua/BigMama/BigMama.lua")

dofile(g_path.."data/lua/Exp.lua")
dofile(g_path.."data/lua/room/roomcheck.lua")
--音乐
dofile(g_path.."data/lua/Music/Board.lua")
--背景音乐
dofile(g_path.."data/lua/BGM/BGM.lua")

--VIP
dofile(g_path.."data/lua/VIP/VIP.lua")

print("Load groupluascript.lua finish")
