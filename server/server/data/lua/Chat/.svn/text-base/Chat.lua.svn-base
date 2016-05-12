
--自动发言间隔时间(秒)
local AutoChatInterval = 3
--自动发言最长暂停时间(秒)
local AutoChatMaxPauseTime = 240

--------------------下面内容用于控制网络带宽，可不修改--------------------

--group server自动发言发送间隔时间(毫秒)
local AutoChatSendInterval = 100

--------------------下面内容不可修改--------------------

if ServerGetLine ~= nil and ServerGetLine() ~= nil then
	SetAutoChatInfo(AutoChatInterval, AutoChatMaxPauseTime)
else
	SetAutoChatSendInterval(AutoChatSendInterval)
end
