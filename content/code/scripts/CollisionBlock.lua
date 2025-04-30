local transSibling = nil
function OnCollision()
	print("Colliding with CollisionBlock!")
	
	Game.FinishLevel()
	transSibling:SetPosition(vec3(100,100,100))

	--DESTROYS THE BLOCK
	--this_script:GetOwner():Destroy()
	--local col_comp = this_script:GetCollisionComponent(this_script:GetOwner())
	--CollisionManager:GetInstance():RemoveCollisionComponent(col_comp)
end

function Init()
	print("CollisionBlock:Init() called!")
	this_script:BindCollisionBoxEvent()
	this_script:GetOwnerAsGameObject():SetActive(false)
end

function Update(deltaTime)
	--print("DeltaTime: ", deltaTime)
	if transSibling == nil then
		transSibling = this_script:GetTransformComponent(this_script:GetOwner())
	end
end