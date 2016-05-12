
mallTipTitle = "M币、绑定M币和金券使用说明";
mallTip = "　　现在商城中可以使用3种货币，金色图标—M币，粉色图标—绑定M币，紫色图标—金券；\n　　1、在商城中以金券（紫色图标）为单位的商品，当金券不足时，可使用绑定M币和M币支付，会优先使用绑定M币；\n　　2、在商城中以绑定M币（粉色图标）为单位的商品，当绑定M币不足时，可使用M币支付；\n　　3、在商城中以M币（金色图标）为单位的商品，只能使用M币进行支付，绑定M币和金券无法使用！\n　　4\赠送商品时只能使用M币进行支付！";

--体验卡相关，
-- 每天可以使用过的次数
local experienceCardTimesPerDay = 5;
-- 购买的物品可以使用的天数
local experienceCardDaysCanExperienced = 1;
-- 体验卡物品ID
local experienceCardItemType = 39000;
-- 可以体验的衣服类型
local experienceItemType = "0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17";

local maleRecommendEquipment = 
{
	[1] = { name = "", equipment = "40368|42368|44368|54368"},
	[2] = { name = "", equipment = "40308|42308|44308|54308"},
	[3] = { name = "", equipment = "40195|42195|44195|54195"},
	[4] = { name = "", equipment = "40200|42200|44200|54200"},
	[5] = { name = "", equipment = "40249|42249|44249|54249"},
     

};

local femaleEquipment = 
{


	[1] = { name = "", equipment = "40803|57803|54803"},
	[2] = { name = "", equipment = "40801|42801|44801|54713"},
	[3] = { name = "", equipment = "40755|42755|44755|54755"},
	[4] = { name = "", equipment = "40762|57762|54762"},
	[5] = { name = "", equipment = "40749|42749|44749|54749"},
};

function	addMaleEquipment(index, value)
	if value ~= nil	then 
		local name = value["name"];			
		local equipment = value["equipment"];
		AddMaleRecommendEquipmentInfo(index, name, equipment);
	end
end

function	addFemaleEquipment(index, value)
	if value ~= nil	then 
		local name = value["name"];			
		local equipment = value["equipment"];
		AddFemaleRecommendEquipmentInfo(index, name, equipment);
	end
end


table.foreach(maleRecommendEquipment, addMaleEquipment);
table.foreach(femaleEquipment, addFemaleEquipment);

SetMallTipInfo(mallTipTitle, mallTip);
SetExperienceCardProp(experienceCardTimesPerDay, experienceCardDaysCanExperienced, experienceCardItemType, experienceItemType);
