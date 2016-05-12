--服务器全线配置

local allServerInfo = 
{
-- 线号   线号       线的名称             是否是新手线           是否是比赛海选线
-- 数字   数字       文本                 true-是新手线 false-不是新手线 
  [1]  = {line = 1,  lineName = "一线",   isFresherLine = false, isAuditionLine = false },
  [2]  = {line = 2,  lineName = "二线",   isFresherLine = false, isAuditionLine = false },
  [3]  = {line = 3,  lineName = "新手一线",   isFresherLine = true, isAuditionLine = false },
  [4]  = {line = 4,  lineName = "新手二线",   isFresherLine = true, isAuditionLine = false },
  [5]  = {line = 5,  lineName = "五线",   isFresherLine = false, isAuditionLine = false },
  [6]  = {line = 6,  lineName = "六线",   isFresherLine = false, isAuditionLine = false },
  [7]  = {line = 7,  lineName = "七线",   isFresherLine = false, isAuditionLine = false },
  [8]  = {line = 8,  lineName = "八线",   isFresherLine = false, isAuditionLine = false },
  [9]  = {line = 9,  lineName = "九线",   isFresherLine = false, isAuditionLine = false },
  [10] = {line = 10, lineName = "十线",   isFresherLine = false, isAuditionLine = true },
  [11] = {line = 11, lineName = "十一线", isFresherLine = false, isAuditionLine = true },
  [12] = {line = 12, lineName = "十二线", isFresherLine = false, isAuditionLine = true },
  [13] = {line = 13, lineName = "十三线", isFresherLine = false, isAuditionLine = false },
  [14] = {line = 14, lineName = "十四线", isFresherLine = false, isAuditionLine = false },
  [15] = {line = 15, lineName = "十五线", isFresherLine = false, isAuditionLine = false },
  [16] = {line = 16, lineName = "十六线", isFresherLine = false, isAuditionLine = false },
  [17] = {line = 17, lineName = "十七线", isFresherLine = false, isAuditionLine = false },
  [18] = {line = 18, lineName = "十八线", isFresherLine = false, isAuditionLine = false },
  [19] = {line = 19, lineName = "十九线", isFresherLine = false, isAuditionLine = false },
  [20] = {line = 20, lineName = "二十线", isFresherLine = false, isAuditionLine = false }
};

local thisServerInfo = allServerInfo[ServerGetLine()+1]
if thisServerInfo ~= nil then
    SetServerName(thisServerInfo["lineName"], thisServerInfo["isFresherLine"], thisServerInfo["isAuditionLine"])
end
