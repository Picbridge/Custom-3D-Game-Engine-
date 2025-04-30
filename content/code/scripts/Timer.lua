local uiComp = nil
local timer = 0.0
local finished = false
local time

function SetFinishState(state)
	if finished then
		do return end
	end
	finished = state
	AudioManager:GetInstance():PlaySound("sound_effects\\Kenney\\confirmation_002.ogg")
	Game.SetLevelTime(time)
end

function Init()
	print("Timer:Init() called!")
	uiComp = this_script:FindUIComponent("Timer")
end

function Update(deltaTime)
	if SystemSettings:GetInstance():GetBoolSetting("Pause") or finished then
		do return end
	end

	timer = timer + deltaTime
	time = string.format("%.2f", timer)
	uiComp:SetStringBuffer("time: " .. time)
end