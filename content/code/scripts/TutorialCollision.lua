local transSibling = nil
local isCollidingThisFrame = false
function OnCollision()
	--print("Colliding with TutorialCollision!")
	isCollidingThisFrame = true
end

function Init()
	--print("CollisionBlock:Init() called!")
	this_script:BindCollisionBoxEvent()
	this_script:GetOwnerAsGameObject():SetActive(false)
end

function Update(deltaTime)
	if not (this_script:GetCurrentSceneName() == "tutorial_1") then
		return
	end
	if transSibling == nil then
		transSibling = this_script:GetTransformComponent(this_script:GetOwner())
	end

	goName = this_script:GetOwnerAsGameObject():GetName()
		if goName == "tut_text_obj_1" then
			this_script:SetUITextComponentActiveState("Text_1", isCollidingThisFrame)
		elseif goName == "tut_text_obj_2" then
			this_script:SetUITextComponentActiveState("Text_2", isCollidingThisFrame)
		elseif goName == "tut_text_obj_3" then
			this_script:SetUITextComponentActiveState("Text_3", isCollidingThisFrame)
		elseif goName == "tut_text_obj_4" then
			this_script:SetUITextComponentActiveState("Text_4", isCollidingThisFrame)
		elseif goName == "tut_text_obj_5" then
			this_script:SetUITextComponentActiveState("Text_5", isCollidingThisFrame)
		end
		 
	if isCollidingThisFrame then
		isCollidingThisFrame = false
	end
end