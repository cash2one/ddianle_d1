-- 一个红包的祝福值
local RedEnvelopeBlessing = 6;

-- 一个红包的价值(M币)
local RedEnvelopeValue = 600;

-- 新婚礼盒(红包个数:礼包ID)
local WeddingGiftBox_Common = "15:34406";  --普通
local WeddingGiftBox_Exquisite = "50:34407"; --精美
local WeddingGiftBox_Luxury = "150:34408"; --豪华

-- 祝福反馈礼物(动态礼盒)
local BlessingFeedBackGift = 37000;

-- 红字提示红包数目
local RedNoticeRedEnvelopeCount = 20;

-- 结婚纪念日礼物
local MerriageDateGift = 
{
	--[1] = {maleReward = "34091,1,-1", femaleReward = "34091,1,-1"},
}
--------------------------------------------------------------------------------------------------
function AddMerriageDateGiftLua(index, value)
	if value ~= nil then	
		local maleReward = value["maleReward"];
		local femaleReward = value["femaleReward"];
		AddMerriageDateGift( index, maleReward, femaleReward );
	end
end

AddCoupleConfig( RedEnvelopeBlessing, RedEnvelopeValue, BlessingFeedBackGift, WeddingGiftBox_Common, WeddingGiftBox_Exquisite, WeddingGiftBox_Luxury, RedNoticeRedEnvelopeCount );
table.foreach(MerriageDateGift, AddMerriageDateGiftLua);