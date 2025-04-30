local timeElapsed = 0

function Handler(message)
	return "Error: " .. message
end

function Init()
    print("LUA: Hello Init()!")
    this_script:GetRenderComponent(this_script:GetOwner()):SetColor(1.0, 1.0, 1.0)
    print("LUA: Calling TestFunc() from Lua!")
    this_script:TestFunc()
	local owner = this_script:GetOwnerAsGameObject()
    print(owner)
	local ownerID = owner:GetName()
	print(ownerID)
    print("LUA: Goodbye Init()!")
    this_script:GetParticleComponent(this_script:GetOwner()):Toggle()
end

function Update(deltaTime)
    timeElapsed = timeElapsed + deltaTime

    -- Calculate color components based on timeElapsed
    local red = 0.5 * (1 + math.sin(timeElapsed * 2 * math.pi / 7))
    local green = 0.5 * (1 + math.sin(timeElapsed * 2 * math.pi / 7 + 2 * math.pi / 3))
    local blue = 0.5 * (1 + math.sin(timeElapsed * 2 * math.pi / 7 + 4 * math.pi / 3))

    -- Set the color on the RenderComponent
    this_script:GetRenderComponent(this_script:GetOwner()):SetColor(red, green, blue)

    local x = 360 * 0.5 * (1 + math.sin(timeElapsed * 2 * math.pi / 5))
    local y = 360 * 0.5 * (1 + math.sin(timeElapsed * 2 * math.pi / 5 + 2 * math.pi / 3))
    local z = 360 * 0.5 * (1 + math.sin(timeElapsed * 2 * math.pi / 5 + 4 * math.pi / 3))
    local vecxyz = vec3(x, y, z)

    this_script:GetTransformComponent(this_script:GetOwner()):SetRotation(vecxyz)
end


function Shutdown()
	print("LUA: Hello Shutdown()!")
end

function addAndDouble(a, b)
	print("LUA: addAndDouble works!")
	return (a + b) * 2
end

