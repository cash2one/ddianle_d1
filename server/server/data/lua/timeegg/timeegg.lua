--时光蛋配置

--时光蛋开蛋时限，分钟
local TimeEggOpenDuration = 2880
--时光蛋销毁时限，分钟，开蛋时限之后开始计时
local TimeEggDestroyDuration = 21600
--固定奖励金券
local FixedMoneyReward = 0
--共同开启奖励金券
local ExtraMoneyReward = 0
--纪念品折扣
local GiftDiscount = 70
-- 帮助内容
local egg_help_title = "时光蛋使用帮助";
local egg_help_content = "　　时光蛋可在时空之塔的旋转木马和秋千与熊场景中使用；持有时光蛋的玩家在场景中点击蛋形设施的地点时，可选择共同使用道具的玩家。彼此写下时光心语并互赠时光纪念品，即可成功埋下时光蛋。3天后，双方可回到埋蛋的地方，开启专属彼此的时光回忆，一同开启还会有额外的幸福加分奖励哦！";
--道具需要配置商城中正在售卖的道具
local Items = 
{
     costItems =  "31071,1,-1"; --种蛋消耗道具
     giftItems = "34575,1,-1|35203,1,-1|34576,1,-1";--纪念品道具列表
     togetherMaleRewardItems = "31073,1,-1"; --共同开蛋男性玩家奖励
     togetherFemaleRewardItems = "31073,1,-1"; --共同开蛋女性玩家奖励
     maleRewardItems = ""; --男性玩家奖励
     femaleRewardItems = ""; --女性玩家奖励
}

SetTimeEggSettingInfo(TimeEggOpenDuration, TimeEggDestroyDuration, Items["costItems"], Items["giftItems"], Items["togetherMaleRewardItems"], Items["togetherFemaleRewardItems"], Items["maleRewardItems"], Items["femaleRewardItems"], FixedMoneyReward, ExtraMoneyReward, GiftDiscount, egg_help_title, egg_help_content);