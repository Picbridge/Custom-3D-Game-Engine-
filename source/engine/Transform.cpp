#include "pch.h"

void Transform::SetPosition(const glm::vec3 newPos)
{
	m_position = newPos;
	updateModelMatrix();
}

void Transform::SetRotation(const glm::vec3 newRot)
{
	m_rotation = newRot;
	updateModelMatrix();
}

void Transform::SetScale(const glm::vec3 newScale)
{
	m_scale = newScale;
	updateModelMatrix();

}

void Transform::SetProjection(const glm::mat4 projection)
{
	m_projection = projection;
}

void Transform::SetView(const glm::mat4 view)
{
	m_view = view;
}

void Transform::updateModelMatrix()
{
	m_scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);
	m_rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	m_translationMatrix = glm::translate(glm::mat4(1.0f), m_position);

	m_model = m_translationMatrix * m_rotationMatrix * m_scaleMatrix;
}
