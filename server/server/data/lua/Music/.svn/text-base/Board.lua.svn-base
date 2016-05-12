local BoardInfo = 
{
[494]		= { new = 1, hot = 0 },		--""
[611]	        = { new = 1, hot = 0 },		--""
[599]	        = { new = 1, hot = 0 },		--""
[590]		= { new = 1, hot = 0 },		--""
[664]	        = { new = 1, hot = 0 },		--""
[672]	        = { new = 1, hot = 0 },		--""
[639]		= { new = 1, hot = 0 },		--""
[595]	        = { new = 1, hot = 0 },		--""
[596]	        = { new = 1, hot = 0 },		--""
[665]		= { new = 1, hot = 0 },		--""
[185]	        = { new = 0, hot = 1 },		--""
[137]	        = { new = 0, hot = 1 },		--""
[067]	        = { new = 0, hot = 1 },		--""
[174]	        = { new = 0, hot = 1 },		--""
[214]	        = { new = 0, hot = 1 },		--""
[475]	        = { new = 0, hot = 1 },		--""
[197]		= { new = 0, hot = 1 }  	--""
}

-- Taiko mode music ID table
local fresherSongTaiko = { 344 };
-- Tradition mode music ID table
local fresherSongTradition = { 344 };
-- Osu mode music ID table
local fresherSongOsu = { 344 };
-- Rhythm mode music ID table
--local fresherSongRhythm = { 344 };


function AnalyseBoardInfo( index, value )
	if value ~= nil then
		local new = ( value["new"] or 0 ) > 0;
		local hot = ( value["hot"] or 0 ) > 0;
		
		AddMusicBoard( index, new, hot );
	end
end

function AddFresherSongTable(mode, songTable)
	for k, v in ipairs(songTable) do
		AddFresherSong(mode, v);
	end
end

table.foreach( BoardInfo, AnalyseBoardInfo );

AddFresherSongTable(1, fresherSongTaiko);
AddFresherSongTable(2, fresherSongTradition);
AddFresherSongTable(3, fresherSongOsu);
--AddFresherSongTable(4, fresherSongRhythm);
