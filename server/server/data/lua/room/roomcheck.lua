-- mode说明: 1-Taiko     2-Tradition     3-Osu     4-Rhythm     5-heartbeats
local RoomCheckData = 
{
	[1] = { time=120, nomiss = 10, mark=3000 },
	[2] = { time=120, nomiss = 3, mark=3000 },
	[3] = { time=120, nomiss = 10, mark=3000 },
        [4] = { time=120, nomiss = 10, mark=3000 },
	[5] = { time=120, nomiss = 5, mark=3000 },
        [6] = { time=120, nomiss = 3, mark=3000 },
	[7] = { time=120, nomiss = 5, mark=3000 },
}

function AddRoomCheckData( index, value )
	if value ~= nil then
		local checktime = value["time"];
		local checknomiss = value["nomiss"];
		local checkmark = value["mark"];
		
		AddRoomCheck( index, checktime, checknomiss, checkmark );
	end
end

table.foreach( RoomCheckData, AddRoomCheckData );


