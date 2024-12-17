#pragma once
class TransformComponent : public Component
{
public:
	TransformComponent() :m_pTransform(nullptr) 
	{
		defineMember();
	}
	~TransformComponent() {}

	void Init() override;
	void Update() override;
	void Shutdown() override;

	//-------------------
	//Setters
	//-------------------
	
	//@brief Sets the GameObject position
	//@param pos : New position of the GameObject
	void SetPosition(glm::vec3 pos);

	//@brief Sets the GameObject rotation
	//@param angle : New rotation of the GameObject
	void SetRotation(glm::vec3 angle);

	//@brief Sets the GameObject scale
	//@param scale : New scale of the GameObject
	void SetScale(glm::vec3 scale);

	//@brief Sets the GameObject scale
	//@param scale : New scale of the GameObject
	void SetScale(float scale);

	//-------------------
	//Getters
	//-------------------

	//@brief Returns the position of the GameObject
	//@return glm::vec3 the position of the GameObject
	glm::vec3 GetPosition() { return m_pTransform->GetPosition(); }
	//@brief Returns the rotation of the GameObject
	//@return glm::vec3 the rotation of the GameObject
	glm::vec3 GetRotation() { return m_pTransform->GetRotation(); }
	//@brief Returns the scale of the GameObject
	//@return glm::vec3 the scale of the GameObject
	glm::vec3 GetScale() { return m_pTransform->GetScale(); }
	//@brief Returns the translation matrix of the GameObject
	glm::mat4 GetTranslationMatrix() { return m_pTransform->GetTranslationMatrix(); }
	//@brief Returns the rotation matrix of the GameObject
	//@return glm::mat4 the rotation matrix of the GameObject
	glm::mat4 GetRotationMatrix() { return m_pTransform->GetRotationMatrix(); }
	//@brief Returns the scale matrix of the GameObject
	//@return glm::mat4 the scale matrix of the GameObject
	glm::mat4 GetScaleMatrix() { return m_pTransform->GetScaleMatrix(); }
	//@brief Returns the model matrix of the GameObject
	//@return glm::mat4 the model matrix of the GameObject
	glm::mat4 GetModelMatrix() { return m_pTransform->GetModel(); }

private:
	Transform* m_pTransform;

	void defineMember() override
	{
		m_setters["position"] = [this](std::any value) { SetPosition(std::any_cast<glm::vec3>(value)); };
		m_setters["rotation"] = [this](std::any value) { SetRotation(std::any_cast<glm::vec3>(value)); };
		m_setters["scale"] = [this](std::any value) { SetScale(std::any_cast<glm::vec3>(value)); };

		m_getters["position"] = [this]() -> std::any { return GetPosition(); };
		m_getters["rotation"] = [this]() -> std::any { return GetRotation(); };
		m_getters["scale"] = [this]() -> std::any { return GetScale(); };
	}
	
};

