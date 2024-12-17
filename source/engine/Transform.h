#pragma once
class Transform
{
public:
	Transform() : m_model(glm::identity<glm::mat4>()),
		m_projection(glm::identity<glm::mat4>()),
		m_view(glm::identity<glm::mat4>()),
		m_position(glm::vec3(0.0f)),
		m_rotation(glm::vec3(0.0f)),
		m_scale(glm::vec3(1.0f)),
		m_translationMatrix(glm::identity<glm::mat4>()),
		m_rotationMatrix(glm::identity<glm::mat4>()),
		m_scaleMatrix(glm::identity<glm::mat4>()) {}

	//--------------------------------
	//Setters
	//--------------------------------

	//@brief Sets the position of the transform
	//@param position : The position of the transform
	void SetPosition(glm::vec3 position);

	//@brief Sets the rotation of the transform
	//@param rotation : The rotation of the transform
	void SetRotation(glm::vec3 rotation);

	//@brief Sets the scale of the transform
	//@param scale : The scale of the transform
	void SetScale(glm::vec3 scale);

	//@brief Assign projection matrix to the transform
	//@param projection : The projection matrix
	void SetProjection(glm::mat4 projection);

	//@brief Assign view matrix to the transform
	//@param view : The view matrix
	void SetView(glm::mat4 view);

	//--------------------------------
	//Getters
	//--------------------------------

	//@brief Returns the position of the transform
	//@return glm::vec3 the position of the transform
	const glm::vec3 GetPosition() const { return m_position; }

	//@brief Returns the rotation of the transform
	//@return glm::vec3 the rotation of the transform
	const glm::vec3 GetRotation() const { return m_rotation; }

	//@brief Returns the scale of the transform
	//@return glm::vec3 the scale of the transform
	const glm::vec3 GetScale() const { return m_scale; }

	//@brief Returns the translation matrix of the transform
	//@return glm::mat4 the translation matrix of the transform
	const glm::mat4 GetTranslationMatrix() const { return m_translationMatrix; }

	//@brief Returns the rotation matrix of the transform
	//@return glm::mat4 the rotation matrix of the transform
	const glm::mat4 GetRotationMatrix() const { return m_rotationMatrix; }

	//@brief Returns the scale matrix of the transform
	//@return glm::mat4 the scale matrix of the transform
	const glm::mat4 GetScaleMatrix() const { return m_scaleMatrix; }

	//@brief Returns the model matrix of the transform
	//@return glm::mat4 the model matrix of the transform
	const glm::mat4 GetModel() const { return m_model; }

	//@brief Returns the projection matrix of the transform
	//@return glm::mat4 the projection matrix of the transform
	const glm::mat4 GetProjection() const { return m_projection; }

	//@brief Returns the view matrix of the transform
	//@return glm::mat4 the view matrix of the transform
	const glm::mat4 GetView() const { return m_view; }

private:
	glm::mat4 m_model;
	glm::mat4 m_projection;
	glm::mat4 m_view;

	glm::mat4 m_translationMatrix;
	glm::mat4 m_rotationMatrix;
	glm::mat4 m_scaleMatrix;

	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;

	//@brief Helper to update the model matrix whenever the transform changes
	void updateModelMatrix();
};