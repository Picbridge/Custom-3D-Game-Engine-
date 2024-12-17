local timeElapsed = 0

function Handler(message)
	return "Error: " .. message
end

function Init()
    print("LUA: Hello Init()!")
    my_script:GetRenderComponent(my_script:GetOwner()):SetColor(1.0, 1.0, 1.0)
    print("LUA: Calling TestFunc() from Lua!")
    my_script:TestFunc()
	local owner = my_script:GetOwnerAsGameObject()
    print(owner)
	local ownerID = owner:GetName()
	print(ownerID)
    print("LUA: Goodbye Init()!")
end

function Update(deltaTime)
    timeElapsed = timeElapsed + deltaTime

    -- Calculate color components based on timeElapsed
    local red = 0.5 * (1 + math.sin(timeElapsed * 2 * math.pi / 7))
    local green = 0.5 * (1 + math.sin(timeElapsed * 2 * math.pi / 7 + 2 * math.pi / 3))
    local blue = 0.5 * (1 + math.sin(timeElapsed * 2 * math.pi / 7 + 4 * math.pi / 3))

    -- Set the color on the RenderComponent
    my_script:GetRenderComponent(my_script:GetOwner()):SetColor(red, green, blue)
end


function Shutdown()
	print("LUA: Hello Shutdown()!")
end

function addAndDouble(a, b)
	print("LUA: addAndDouble works!")
	return (a + b) * 2
end

