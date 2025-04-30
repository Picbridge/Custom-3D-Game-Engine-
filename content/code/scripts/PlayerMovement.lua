-- Copyright 2025 DigiPen (USA) Corporation
-- File Author(s): Grace Biggs
-- First-person player movement script for Grappling, With Emotions
-- Expects a sibling CameraComponent, PhysicsComponent, and CollisionComponent.
-- TODO: Catch and report silent access errors (i.e., calling an unbound function will halt script execution, but not produce an error)
-- TODO: Event system instead of IsKeyPressed(), etc.

local timeElapsed = 0

local prevMouseX = 0
local prevMouseY = 0
local mouseSensitivity = 0.1
local rot = vec3(0)
-- Components
local physSibling = nil
local transSibling = nil
local camSibling = nil
local dt = 0

local speed = 5
local startPos = vec3(0.0, 3.0, 0.0)

function Handler(message)
	return "Error: " .. message
end

function Init()
	print("PlayerMovement:Init() called!")
	phySibling = this_script:GetPhysicsComponent(this_script:GetOwner())
	--phySibling:SetFrictionCoefficient(0.0)
end

function Update(deltaTime)
	-- Get siblings
	if physSibling == nil then
		print("LUA: PhysicsComponent not found! Getting from owner")
		physSibling = this_script:GetPhysicsComponent(this_script:GetOwner())
	end
	if transSibling == nil then
		print("LUA: TransformComponent not found! Getting from owner")
		transSibling = this_script:GetTransformComponent(this_script:GetOwner())
	end
	if camSibling == nil then
		print("LUA: CameraComponent not found! Getting from owner")
		camSibling = this_script:GetCameraComponent(this_script:GetOwner())
	end
	assert(physSibling ~= nil, "LUA: PhysicsComponent not found!")
	assert(transSibling ~= nil, "LUA: TransformComponent not found!")
	assert(camSibling ~= nil, "LUA: CameraComponent not found!")

	if not camSibling:IsActive() then
		return
	end

	dt = deltaTime
	local invertedYAxis = SystemSettings:GetInstance():GetBoolSetting("Invert Y-Axis")
	-- focusedOnUI = UI:GetInstance():Hovering() -- TODO: Fix this from breaking the script
	rot = transSibling:GetRotation()

	-- if not focusedOnUI then
		-- Mouse rotation handling
		local x = Input:GetInstance():GetMouseX()
		local y = Input:GetInstance():GetMouseY()
		local dx = x - prevMouseX
		local dy = y - prevMouseY
		rot.y = rot.y + dx * mouseSensitivity -- Yaw (horizontal)
		
		if invertedYAxis then
			rot.x = rot.x - dy * mouseSensitivity -- Invert Y-axis
		else
			rot.x = rot.x + dy * mouseSensitivity
		end

		-- Clamp pitch to prevent over-rotation
		rot.x = math.max(math.min(rot.x, 89), -89)
	-- end
	
	prevMouseX = x
	prevMouseY = y

	
	local view = camSibling:GetViewMatrix()
	HandleMoveInput(view)
	HandleGrapple(view)

	-- Update transform with new position and rotation
	
	transSibling:SetRotation(rot)

	-- Position the Grapple gun
	local child = SERVICE_LOCATOR:GetGameObjectManager():GetGameObject("GUN")
	if child ~= nil then
		local forwardDir = VectorCalculation.GetForwardVecFromView(view)
		local rightDir = VectorCalculation.GetRightVecFromView(view)
		rot = vec3(-rot.x, -rot.y, 0)
		local childTrans = this_script:GetTransformComponent(child)
		childTrans:SetRotation(rot)
	end
end

function HandleMoveInput(view)
    local forwardDir = VectorCalculation.GetForwardVecFromView(view)
    local rightDir = VectorCalculation.GetRightVecFromView(view)
    local input = Input:GetInstance()
	
	-- Set Gun position relative to player
	local child = SERVICE_LOCATOR:GetGameObjectManager():GetGameObject("GUN")
	local childTrans = this_script:GetTransformComponent(child)
	childTrans:SetPosition((0.25 * rightDir) + (0.25 * forwardDir) + transSibling:GetPosition())
    
    -- Reset position (existing code)
    if ((input:IsKeyJustPressed(GLFW_KEY_BACKSPACE)) or (transSibling:GetPosition().y < -100)) then
		local pos = startPos
        transSibling:SetPosition(pos)
        camSibling:SetPosition(pos)
        physSibling:SetVelocity(dvec3(0.0))
    end
    
    -- Movement input handling
    local moveDir = vec3(0.0)
    if input:IsKeyPressed(GLFW_KEY_W) then moveDir = moveDir + forwardDir end
    if input:IsKeyPressed(GLFW_KEY_S) then moveDir = moveDir - forwardDir end
    if input:IsKeyPressed(GLFW_KEY_A) then moveDir = moveDir - rightDir   end
    if input:IsKeyPressed(GLFW_KEY_D) then moveDir = moveDir + rightDir   end

    -- Normalize only if moveDir isn't zero
    if moveDir:length() > 0 then
        moveDir = moveDir:normalize()
    end

    -- Jump handling (existing code)
	if phySibling:GetGrounded() then
		if input:IsKeyJustPressed(GLFW_KEY_SPACE) then
			local upDir = dvec3(0.0,1.0,0.0)
			local jumpForce = 10.0
			local jumpVec = jumpForce * upDir
			physSibling:ApplyForce(jumpVec)
		else
			-- handle grounded Movement
			-- make it sticky!
			-- find the normal of the Ground in the move direction
			local groundedNormal = vec3(physSibling:GetGroundedNormal())
			--local slopeLimit = math.cos(math.rad(45)) -- 45-degree slope limit
			--if SE_dot_product(groundedNormal, vec3(0, 1, 0)) < slopeLimit then
			--	print("Too steep to walk!")
			--	return
			--end
			-- flatten to XZ plane
			--moveDir.y = 0.0
			--moveDir = moveDir:normalize()
			--
			local dot = SE_dot_product(moveDir, groundedNormal)
			local orthoMove = moveDir - (dot * groundedNormal)
			if orthoMove:length() > 0 then
				orthoMove = orthoMove:normalize()
			end
			-- apply the velocity
			--local stickyForce = groundedNormal * -0.5
			--physSibling:ApplyForce(dvec3(stickyForce))
			physSibling:SetVelocity(dvec3(orthoMove * speed))
		end
	else
		-- handle non grounded movement
		physSibling:ApplyForce(dvec3(moveDir * speed * dt))
	end

end

function HandleGrapple(view)
	-- Get Player
	local player = this_script:GetOwner()
	local grappleSibling = this_script:GetGrappleComponent(player)
	local input = Input:GetInstance()
	if grappleSibling == nil then
		print("LUA: GrappleComponent not found!")
		return
	end
	if input:IsMouseButtonJustPressed(GLFW_MOUSE_BUTTON_LEFT) and not (grappleSibling:IsAttached()) then
		-- Grapple
		print("LUA: Grapple!")
		-- Get the direction of the camera
		local forwardDir = VectorCalculation.GetForwardVecFromView(view)

		local attachPoint = grappleSibling:CastGrapple(forwardDir)
		if attachPoint ~= nil then
			-- Attach Grapple)
			grappleSibling:AttachGrapple(attachPoint)
			AudioManager:GetInstance():PlaySound("sound_effects\\Kenney\\impactPlate_heavy_002-grapplehit.ogg")
		end
	elseif (input:IsMouseButtonJustPressed(GLFW_MOUSE_BUTTON_LEFT)) and (grappleSibling:IsAttached()) then
		-- Release Grapple
		print("LUA: Release Grapple!")
		grappleSibling:ReleaseGrapple()
	end
	if grappleSibling:IsAttached() then
		-- Allow grapple controls
		if input:IsKeyPressed(GLFW_KEY_E) then
			-- Reel In
			grappleSibling:ReelGrappleIn()
			AudioManager:GetInstance():PlaySound("sound_effects\\Kenney\\click4.ogg")
		end
		if input:IsKeyPressed(GLFW_KEY_Q) then
			-- Reel Out
			grappleSibling:ReelGrappleOut()
			AudioManager:GetInstance():PlaySound("sound_effects\\Kenney\\click5.ogg")
		end
	end
end

function Shutdown()
	print("LUA: Hello Shutdown()!")
end