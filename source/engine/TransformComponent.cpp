#include "pch.h"
#include "TransformComponent.h"

void TransformComponent::Init()
{
	m_pTransform = GetOwner()->GetTransform();
}

void TransformComponent::Update()
{
}

void TransformComponent::Shutdown()
{

}

void TransformComponent::SetPosition(glm::vec3 pos)
{
	m_pTransform->SetPosition(pos);
}

void TransformComponent::SetRotation(glm::vec3 angle)
{
	m_pTransform->SetRotation(angle);
}

void TransformComponent::SetScale(glm::vec3 scale)
{
	m_pTransform->SetScale(scale);
}

void TransformComponent::SetScale(float scale)
{
	m_pTransform->SetScale(glm::vec3(scale));
}
