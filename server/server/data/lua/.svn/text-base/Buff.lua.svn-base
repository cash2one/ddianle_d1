--基因产生的buff对应的图标，格式：GeneID|Param1|Param2-Icon，其中参数个数不定，可以没有参数，或者是一个或者两个，同一基因id对应的参数个数必须一致
local BuffIcon = 
{
    [1] = {key = "1501|1", value = 36000},
    [2] = {key = "1501|2", value = 36002},
    [3] = {key = "1501|3", value = 36004},
    [4] = {key = "1501|4", value = 36006},
    [5] = {key = "1501|5", value = 36008},
    [6] = {key = "1501|6", value = 36010},
    [7] = {key = "1501|7", value = 36012},
    [8] = {key = "1501|8", value = 36014},
    [9] = {key = "1001|100", value = 33001},
    [10] = {key = "7001|30", value = 25005},
    [11] = {key = "7001|50", value = 25006},
    [12] = {key = "7001|100", value = 25007},
    [13] = {key = "7003|5", value = 25000},
    [14] = {key = "7003|13", value = 25001},
    [15] = {key = "7003|14", value = 25002},
    [16] = {key = "7003|15", value = 25003},
    [17] = {key = "7003|12", value = 25004},
    [18] = {key = "7004|1", value = 25008},
    [19] = {key = "7004|2", value = 25009},
    [20] = {key = "7004|3", value = 25010},
    [21] = {key = "7005|0", value = 25011},
    [22] = {key = "7004|4", value = 25017},
    [23] = {key = "7004|5", value = 25018},
    [24] = {key = "7001|5", value = 25019},
 [25] = {key = "1501|9", value = 36016},
 [26] = {key = "1501|10", value = 36018},
 [27] = {key = "1501|11", value = 36020},
 [28] = {key = "1501|12", value = 36022},
 [29] = {key = "1001|300", value = 33006},
 [30] = {key = "1001|200", value = 33007},



}
--可主动移除的基因，其余为不可移除
local BuffCanRemove = 
{
    [1] = 1501,
    [2] = 1001,
}
function AddBuffIconLua(index, value)
	if value ~= nil then 
		local k = value["key"];	
		local v = value["value"];
		
		AddBuffIcon(k, v);
	end
end
function AddBuffCanRemoveLua(index, value)
	if value ~= nil then 
		AddBuffCanRemove(value);
	end
end
table.foreach(BuffIcon, AddBuffIconLua);
table.foreach(BuffCanRemove, AddBuffCanRemoveLua);