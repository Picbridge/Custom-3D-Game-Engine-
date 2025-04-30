local timeElapsed = 0

function Handler(message)
	return "Error: " .. message
end

function Init()
end

function Update(deltaTime)
    timeElapsed = timeElapsed + deltaTime
	if timeElapsed > 5 then
		timeElapsed = 0
		this_script:GetParticleComponent(this_script:GetOwner()):Toggle()
	end

end


function Shutdown()
	print("LUA: Hello Shutdown()!")
end

function addAndDouble(a, b)
	print("LUA: addAndDouble works!")
	return (a + b) * 2
end

