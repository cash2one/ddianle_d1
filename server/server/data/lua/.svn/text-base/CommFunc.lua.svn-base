--通用函数
hourse1 = 3600
hourse24 = 86400
days30=2592000

color_default = 1 
color_purple = 2
color_blue = 3
color_green = 4

MAX_MUSIC_ID = 99

SexType_Male =1
SexType_Female =2

ItemColumn_Expandable    =0--道具栏
ItemColumn_Equip=1        --装备栏
ItemColumn_Other=3        --其他物品

function formattime( ntime )
 if ntime == nil or ntime <= 0 then 
 return "";
 end
 local day = 0;
 local hour = 0;
 local mins = 0;
 local strResult ="";
 if ntime > 86400 then
     day = math.modf( ntime/86400);
     ntime = math.fmod( ntime,86400 );
     strResult=day.."天"
 end
 
 if ntime > 3600 then
     hour = math.modf( ntime/3600 );
     ntime = math.fmod( ntime,3600 );
     strResult = strResult..hour.."小时"
 end
 
 if ntime > 60 then
     mins = math.modf( ntime/60 );
     strResult = strResult..mins.."分钟"
 end
 return strResult
end

