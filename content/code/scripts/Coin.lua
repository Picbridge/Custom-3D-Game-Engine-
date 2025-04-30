local timeElapsed = 0
local transSibling = nil
local particleSibling = nil

function Handler(message)
	return "Error: " .. message
end

function Init()
	this_script:BindCollisionBoxEvent()
end

function Update(deltaTime)
    timeElapsed = timeElapsed + deltaTime
	if transSibling == nil then
		transSibling = this_script:GetTransformComponent(this_script:GetOwner())
	end
	assert(transSibling ~= nil, "LUA: TransformComponent not found!")
	local rot = transSibling:GetRotation()
	rot.y = rot.y + deltaTime * 50
	transSibling:SetRotation(rot)

	
    -- Move the object up and down along the y-axis
    local pos = transSibling:GetPosition()
    local amplitude = 0.0  -- The height of the movement
    local frequency = 1.0  -- The speed of the oscillation
    pos.y = pos.y + math.sin(timeElapsed * frequency) * amplitude * deltaTime
    transSibling:SetPosition(pos)
	
	if particleSibling == nil then
		particleSibling = this_script:GetParticleComponent(this_script:GetOwner()) 
	end
	
	assert(particleSibling ~= nil, "LUA: ParticleComponent not found!")
	particleSibling:SetPosition(pos)
end

function OnCollision()
	print("Colliding with CollisionBlock!")
	
	Game.AddPoint()
	transSibling:SetPosition(vec3(100,100,100))
	AudioManager:GetInstance():PlaySound("sound_effects\\Kenney\\confirmation_003.ogg")
	--DESTROYS THE BLOCK
	--this_script:GetOwner():Destroy()
	--local col_comp = this_script:GetCollisionComponent(this_script:GetOwner())
	--CollisionManager:GetInstance():RemoveCollisionComponent(col_comp)
end

function Shutdown()

end